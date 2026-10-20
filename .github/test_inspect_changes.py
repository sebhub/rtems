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


def _write_file(path: str, text: str = "") -> None:
    Path(path).parent.mkdir(parents=True, exist_ok=True)
    Path(path).write_text(text, encoding="utf-8")


def _add_extra(source: list[str]) -> None:
    _write(
        _BSP / "objextra.yml", {
            "build-type": "objects",
            "enabled-by": {
                "not": "RTEMS_QUAL"
            },
            "source": source,
            "type": "build"
        })


def _demo_with_extra() -> None:
    _write_file("bsps/arm/demo/a.c")
    _write_file("bsps/shared/b.c")
    _write_file("bsps/shared/c.c")
    _add_extra(["bsps/shared/c.c"])
    _commit("bsps/arm/demo: Add the extra objects")


def test_follow_moves_a_file(repository: Path) -> None:
    """ A qualified build item may follow a file which the commit moves. """
    del repository
    _demo_with_extra()
    subprocess.run(["git", "mv", "bsps/shared/c.c", "bsps/shared/d.c"],
                   check=True)
    _add_extra(["bsps/shared/d.c"])
    commit = _commit("bsps/shared: Rename c.c")
    assert inspect_changes._get_categories(commit) == ["source", "build-qual"]


def test_follow_drops_an_unused_file(repository: Path) -> None:
    """ A qualified build item may drop a file which the directory no longer
    lists. """
    del repository
    _demo_with_extra()
    _add_extra([])
    _write_file("bsps/arm/demo/a.c", "int a;\n")
    commit = _commit("bsps/arm/demo: Stop to use c.c")
    assert inspect_changes._get_categories(commit) == ["source", "build-qual"]


def test_follow_keeps_a_used_file(repository: Path) -> None:
    """ A qualified build item may not drop a file which a build item of the
    directory still lists. """
    del repository
    _demo_with_extra()
    _add_extra([])
    obj = dict(_OBJ)
    obj["source"] = _OBJ["source"] + ["bsps/shared/c.c"]
    _write(_BSP / "obj.yml", obj)
    _write_file("bsps/arm/demo/a.c", "int a;\n")
    commit = _commit("bsps/arm/demo: Move c.c")
    assert inspect_changes._get_categories(commit) == ["build-qual", "source"]


def test_follow_adds_a_new_file(repository: Path) -> None:
    """ A qualified build item may gain a file which the commit adds. """
    del repository
    _demo_with_extra()
    _write_file("bsps/shared/e.c")
    _add_extra(["bsps/shared/c.c", "bsps/shared/e.c"])
    commit = _commit("bsps/shared: Add e.c")
    assert inspect_changes._get_categories(commit) == ["source", "build-qual"]


def test_follow_adds_no_other_file(repository: Path) -> None:
    """ A qualified build item may not gain a file which neither the commit
    adds nor a source build item of the directory listed. """
    del repository
    _demo_with_extra()
    _write_file("cpukit/dev/f.c")
    _commit("cpukit/dev: Add f.c")
    _add_extra(["bsps/shared/c.c", "cpukit/dev/f.c"])
    _write_file("bsps/arm/demo/a.c", "int a;\n")
    commit = _commit("bsps/arm/demo: Use f.c")
    assert inspect_changes._get_categories(commit) == ["build-qual", "source"]


def test_follow_moves_an_item(repository: Path) -> None:
    """ A qualified build item may move together with its files. """
    del repository
    _demo_with_extra()
    Path("bsps/demo2").mkdir(parents=True)
    subprocess.run(["git", "mv", "bsps/shared/c.c", "bsps/demo2/c.c"],
                   check=True)
    other = Path("spec/build/bsps/arm/demo2")
    other.mkdir(parents=True)
    subprocess.run(["git", "mv", str(_BSP / "objextra.yml"),
                    str(other / "objextra.yml")],
                   check=True)
    _write(
        other / "objextra.yml", {
            "build-type": "objects",
            "enabled-by": {
                "not": "RTEMS_QUAL"
            },
            "source": ["bsps/demo2/c.c"],
            "type": "build"
        })
    commit = _commit("bsps: Move c.c")
    assert inspect_changes._get_categories(commit) == ["source", "build-qual"]


def test_follow_accepts_an_empty_list(repository: Path) -> None:
    """ A qualified build item may have an empty source list. """
    del repository
    _demo_with_extra()
    _write(
        _BSP / "objqual.yml", {
            "build-type": "objects",
            "enabled-by": "RTEMS_QUAL",
            "source": None,
            "type": "build"
        })
    _commit("build-qual: Add an empty qualified item")
    subprocess.run(["git", "mv", "bsps/shared/c.c", "bsps/shared/d.c"],
                   check=True)
    _add_extra(["bsps/shared/d.c"])
    _write(
        _BSP / "objqual.yml", {
            "build-type": "objects",
            "enabled-by": "RTEMS_QUAL",
            "source": None,
            "type": "build",
            "links": []
        })
    commit = _commit("bsps/shared: Rename c.c")
    assert inspect_changes._get_categories(commit) == ["build-qual", "source"]


def _unlisted(repository: Path, commit: str) -> tuple[str, int]:
    findings = inspect_changes._Findings()
    status = inspect_changes._check_unlisted(repository, commit, findings,
                                             "url")
    return status, findings.error_count


def test_unlisted_rejects_a_removal(repository: Path) -> None:
    """ A file which leaves the extra objects needs another build item. """
    _demo_with_extra()
    _add_extra([])
    commit = _commit("build-qual: Remove c.c from objextra")
    assert _unlisted(repository, commit) == (inspect_changes._ERROR, 1)


def test_unlisted_accepts_a_move(repository: Path) -> None:
    """ A file which moves from the extra objects stays in a build. """
    _demo_with_extra()
    _add_extra([])
    obj = dict(_OBJ)
    obj["source"] = _OBJ["source"] + ["bsps/shared/c.c"]
    _write(_BSP / "obj.yml", obj)
    commit = _commit("build-qual: Move c.c to obj")
    assert _unlisted(repository, commit) == (inspect_changes._OK, 0)


def test_unlisted_accepts_a_deletion(repository: Path) -> None:
    """ A file which the commit deletes leaves every build item. """
    _demo_with_extra()
    subprocess.run(["git", "rm", "-q", "bsps/shared/c.c"], check=True)
    _add_extra([])
    commit = _commit("bsps/shared: Remove c.c")
    assert _unlisted(repository, commit) == (inspect_changes._SKIP, 0)
