#!/usr/bin/env python3
# SPDX-License-Identifier: BSD-2-Clause
""" Provide a command line interface to inspect change sets. """

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
import sys
import tempfile

from specitems import CommonMarkContent, get_arguments

_OK = ":white_check_mark"

_WARNING = ":warning:"

_ERROR = ":x:"

_CATEGORIES = {
    ".gitignore": "source",
    ".github": "ci",
    "Doxyfile": "source",
    "INSTALL": "source",
    "LICENSE": "source",
    "LICENSE.Apache-2.0": "source",
    "LICENSE.BSD-2-Clause": "source",
    "LICENSE.BSD-3-Clause": "source",
    "LICENSE.CC-BY-SA-4.0": "source",
    "LICENSE.Freescale": "source",
    "LICENSE.GPL-2.0": "source",
    "LICENSE.JFFS2": "source",
    "LICENSE.LLVM": "source",
    "LICENSE.NET": "source",
    "LICENSE.RPCXDR": "source",
    "MAINTAINERS": "source",
    "Makefile": "source",
    "Makefile.maint": "source",
    "README": "source",
    "bsps": "source",
    "build_tools.py": "pkg",
    "c": "source",
    "config-bsps": "pkg",
    "config-tools": "pkg",
    "cpukit": "source",
    "gccdeps.py": "source",
    "inspect_changes.py": "ci",
    "make": "source",
    "pyproject.toml": "pkg",
    "rtems-bsps": "source",
    "rtemslogo.png": "source",
    "spec": "spec",
    "spec-pkg-bsps": "pkg",
    "spec-pkg-tools": "pkg",
    "specware.yml": "source",
    "test-logs": "pkg",
    "testsuites": "source",
    "uv.lock": "pkg",
    "waf": "source",
    "wscript": "source",
    "yaml": "source",
}


def _get_repository_path() -> Path:
    repository = Path(".").absolute()
    while not (repository / ".git").is_dir():
        repository = repository.parent
    return repository


def _get_commits(repository: Path, base_ref: str,
                 head_ref: str) -> list[tuple[str, dict[str, str]]]:
    commits: list[tuple[str, dict[str, str]]] = []
    stdout = subprocess.check_output(
        ["git", "log", "--format=format:%H %s", f"{base_ref}^..{head_ref}"],
        encoding="utf-8")
    log = tuple(line.partition(" ") for line in stdout.splitlines())
    for commit, _, subject in reversed(log):
        categories: set[str] = set()
        stdout = subprocess.check_output([
            "git", "diff-tree", "--no-commit-id", "--name-only", "-r", commit
        ],
                                         encoding="utf-8")
        files: list[str] = []
        for name in stdout.splitlines():
            files.append(name)
            path = Path(name)
            category = _CATEGORIES.get(path.parts[0], "unknown")
            if category == "spec" and path.parts[1] == "build":
                category = "source"
            categories.add(category)
        commits.append((commit, {
            "subject": subject,
            "categories": sorted(categories),
            "files": sorted(files)
        }))
    return commits


def _get_branch() -> str:
    try:
        branch = subprocess.check_output(
            ["git", "symbolic-ref", "--short", "HEAD"],
            encoding="utf-8").strip()
    except subprocess.CalledProcessError:
        branch = "inspect-changes"
        subprocess.run(["git", "checkout", "-B", branch], check=True)
    return branch


def _specverify(ok: bool,
                format_status: str,
                url: str,
                errors: CommonMarkContent,
                tmp_dir: str,
                path: str,
                uid_errors_are_warnings: bool = False) -> tuple[bool, str]:
    cmd = ["specmakeverify", path]
    if uid_errors_are_warnings:
        cmd.append("--uid-errors-are-warnings")
    result = subprocess.run(cmd,
                            cwd=tmp_dir,
                            capture_output=True,
                            encoding="utf-8")
    if result.returncode != 0:
        format_status = ":x"
        ok = False
        errors.add(f"In {url}, there are specification "
                   f"format errors in directory '{path}':")
        if result.stdout:
            errors.add_code_block(result.stdout, language="")
        if result.stderr:
            errors.add_code_block(result.stderr, language="")
    return ok, format_status


def main(argv: list[str]) -> None:
    """ Build the tools for the specified architectures. """

    def _add_arguments(parser):
        parser.add_argument("url",
                            metavar="URL",
                            nargs=1,
                            help="the repository URL")
        parser.add_argument("base_ref",
                            metavar="BASE_REF",
                            nargs=1,
                            help="the base Git reference")
        parser.add_argument("head_ref",
                            metavar="HEAD_REF",
                            nargs=1,
                            help="the head Git reference")

    args = get_arguments(argv[1:],
                         description=sys.modules[__name__].__doc__,
                         add_arguments=(_add_arguments, ))
    repository = _get_repository_path()
    print(repository)
    commits = _get_commits(repository, args.base_ref[0], args.head_ref[0])
    print(commits)
    branch = _get_branch()
    print("branch", branch)
    rows = [["Subject", "Category", "Clean", "Format", "Status"]]
    errors = CommonMarkContent()
    with tempfile.TemporaryDirectory() as tmp_dir:
        print(tmp_dir)
        subprocess.run([
            "git", "clone", "--depth",
            str(len(commits)), "--branch", branch, "--single-branch",
            f"file://{repository}", tmp_dir
        ],
                       check=True)
        for commit, info in commits:
            ok = True
            url = f"{args.url[0]}/commit/{commit}"
            subprocess.run(["git", "checkout", commit],
                           cwd=tmp_dir,
                           check=True)
            format_status = _OK
            ok, format_status = _specverify(ok, format_status, url, errors,
                                            tmp_dir, "spec")
            ok, format_status = _specverify(ok, format_status, url, errors,
                                            tmp_dir, "spec-pkg-bsps", True)
            ok, format_status = _specverify(ok, format_status, url, errors,
                                            tmp_dir, "spec-pkg-tools", True)
            subprocess.run(["specwareexport", "--no-documentation"],
                           cwd=tmp_dir,
                           check=False)
            git_status = subprocess.check_output(["git", "status", "--short"],
                                                 cwd=tmp_dir,
                                                 encoding="utf-8")
            if git_status:
                if info["categories"] == ["spec"]:
                    clean = _WARNING
                else:
                    ok = False
                    clean = _ERROR
                    errors.add(f"In {url}, the repository is not clean:")
                    errors.add_code_block(git_status, language="")
            else:
                clean = _OK
            subprocess.run(["git", "checkout", "."], cwd=tmp_dir, check=True)
            subprocess.run(["git", "clean", "-fd", "."],
                           cwd=tmp_dir,
                           check=True)
            git_status_2 = subprocess.check_output(
                ["git", "status", "--short"], cwd=tmp_dir, encoding="utf-8")
            assert not git_status_2
            category_info = ", ".join(info["categories"])
            if len(info["categories"]) != 1 or "unknown" in info["categories"]:
                ok = False
                category_info = f"{category_info} :interrobang:"
                errors.add(f"In {url}, the change set belongs to more than one category.")
            if ok:
                status = _OK
            else:
                status = ":x"
            rows.append([
                f"[{info['subject']}]({url})", category_info, clean,
                format_status, status
            ])
        if clean != _OK:
            errors.add(f"In {url}, the repository is not clean:")
            errors.add_code_block(git_status, language="")
    content = CommonMarkContent()
    content.add_simple_table(rows)
    content.add(errors)
    print(str(content))
    if errors:
        return 1
    return 0


if __name__ == "__main__":
    main(sys.argv)
