# SPDX-License-Identifier: BSD-2-Clause
""" Tests the category of the commits of a change set. """

# Copyright (C) 2026 embedded brains GmbH & Co. KG
#
# Redistribution and use in source and binary forms, with or without
# modification, are permitted provided that the following conditions
# are met:
# 1. Redistributions of source code must retain the above copyright
#    notice, this list of conditions and the following disclaimer.
# 2. Redistributions in binary form must reproduce the above copyright
#    notice, this list of conditions and the following disclaimer in the
#    documentation and/or other materials provided with the distribution.
#
# THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
# AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
# IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
# ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE
# LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
# CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
# SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
# INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
# CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
# ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
# POSSIBILITY OF SUCH DAMAGE.

from pathlib import Path
import subprocess

import pytest
import yaml

import inspect_changes

_BSP = Path("spec/build/bsps/arm/demo")

_GROUP = {
    "enabled-by": True,
    "links": [{
        "role": "build-dependency",
        "uid": "obj"
    }],
    "type": "build"
}

_OBJ = {
    "build-type": "objects",
    "enabled-by": True,
    "source": ["bsps/arm/demo/a.c", "bsps/shared/b.c"],
    "type": "build"
}


def _write(path: Path, data: dict) -> None:
    path.parent.mkdir(parents=True, exist_ok=True)
    path.write_text(yaml.safe_dump(data), encoding="utf-8")


def _commit(message: str) -> str:
    subprocess.run(["git", "add", "-A"], check=True)
    subprocess.run(["git", "commit", "-q", "-m", message], check=True)
    return subprocess.check_output(["git", "rev-parse", "HEAD"],
                                   encoding="utf-8").strip()


@pytest.fixture(name="repository")
def _fixture_repository(tmp_path: Path, monkeypatch: pytest.MonkeyPatch):
    monkeypatch.chdir(tmp_path)
    for key, value in (("GIT_AUTHOR_NAME", "A"), ("GIT_COMMITTER_NAME", "A"),
                       ("GIT_AUTHOR_EMAIL", "a@b"), ("GIT_COMMITTER_EMAIL",
                                                     "a@b")):
        monkeypatch.setenv(key, value)
    subprocess.run(["git", "init", "-q"], check=True)
    _write(_BSP / "grp.yml", _GROUP)
    _write(_BSP / "obj.yml", _OBJ)
    _commit("bsps/arm/demo: Add the BSP")
    return tmp_path


def _split(extra_enabled_by: object, extra_source: list[str],
           obj_source: list[str]) -> str:
    group = dict(_GROUP)
    group["links"] = _GROUP["links"] + [{
        "role": "build-dependency",
        "uid": "objextra"
    }, {
        "role": "build-dependency",
        "uid": "objqual"
    }]
    _write(_BSP / "grp.yml", group)
    obj = dict(_OBJ)
    obj["source"] = obj_source
    _write(_BSP / "obj.yml", obj)
    _write(
        _BSP / "objextra.yml", {
            "build-type": "objects",
            "enabled-by": extra_enabled_by,
            "source": extra_source,
            "type": "build"
        })
    _write(
        _BSP / "objqual.yml", {
            "build-type": "objects",
            "enabled-by": "RTEMS_QUAL",
            "source": ["cpukit/score/src/threadplaindispatch.c"],
            "type": "build"
        })
    return _commit("build-qual: Add the qualified build of the BSP")


def test_split_is_build_qual(repository: Path) -> None:
    """ A split which keeps the builds without RTEMS_QUAL is build-qual. """
    del repository
    commit = _split({"not": "RTEMS_QUAL"}, ["bsps/shared/b.c"],
                    ["bsps/arm/demo/a.c"])
    assert inspect_changes._get_categories(commit) == ["build-qual"]


def test_split_needs_not_rtems_qual(repository: Path) -> None:
    """ A new extra item which the qualified build enables is no split. """
    del repository
    commit = _split(True, ["bsps/shared/b.c"], ["bsps/arm/demo/a.c"])
    assert inspect_changes._get_categories(commit) == [
        "build-qual", "source"
    ]


def test_split_keeps_each_entry(repository: Path) -> None:
    """ An entry which leaves the source item needs an extra item. """
    del repository
    commit = _split({"not": "RTEMS_QUAL"}, [], ["bsps/arm/demo/a.c"])
    assert inspect_changes._get_categories(commit) == [
        "build-qual", "source"
    ]


def test_split_adds_no_entry(repository: Path) -> None:
    """ A split adds no entry to a source item. """
    del repository
    commit = _split({"not": "RTEMS_QUAL"}, ["bsps/shared/b.c"],
                    ["bsps/arm/demo/a.c", "bsps/arm/demo/c.c"])
    assert inspect_changes._get_categories(commit) == [
        "build-qual", "source"
    ]


def test_split_configuration_is_pkg() -> None:
    """ The split configuration of a release cut is a package file. """
    assert inspect_changes.get_category(".eb/split-config.yml") == "pkg"


_TEST = {
    "actions": [{
        "set-test-state": {
            "reason": "The test exceeds the flash.\n",
            "state": "exclude",
            "tests": ["sp01", "sp71"]
        }
    }],
    "build-type": "option",
    "enabled-by": True,
    "type": "build"
}


def _split_test(state: str) -> str:
    test = dict(_TEST)
    test["actions"] = [{
        "set-test-state": {
            "reason": "The test exceeds the flash.\n",
            "state": "exclude",
            "tests": ["sp01"]
        }
    }]
    _write(_BSP / "tst.yml", test)
    _write(
        _BSP / "tstextra.yml", {
            "actions": [{
                "set-test-state": {
                    "reason": "The test needs the extra build.\n",
                    "state": state,
                    "tests": ["sp71"]
                }
            }],
            "build-type": "option",
            "enabled-by": {
                "not": "RTEMS_QUAL"
            },
            "type": "build"
        })
    return _commit("build-qual: Add the extra test states of the BSP")


def test_split_moves_list_elements(repository: Path) -> None:
    """ A test state may move into an extra item. """
    del repository
    _write(_BSP / "tst.yml", _TEST)
    _commit("bsps/arm/demo: Add the test states")
    commit = _split_test("exclude")
    assert inspect_changes._get_categories(commit) == ["build-qual"]


def test_split_keeps_the_test_state(repository: Path) -> None:
    """ A test which moves keeps its test state. """
    del repository
    _write(_BSP / "tst.yml", _TEST)
    _commit("bsps/arm/demo: Add the test states")
    commit = _split_test("expected-fail")
    assert inspect_changes._get_categories(commit) == [
        "build-qual", "source"
    ]
