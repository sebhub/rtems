#!/usr/bin/env python3
# SPDX-License-Identifier: BSD-2-Clause
""" Read a package of the GitHub container registry. """

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
#
# The commands read a package through the OCI distribution API of ghcr.io.
# The environment variables GITHUB_ACTOR and TOKEN provide the credentials.
# The exit status is 0 if the package version exists, 2 if it does not exist,
# and 1 on any other error.

import argparse
import base64
import json
import os
import shutil
import sys
import time
import urllib.error
import urllib.parse
import urllib.request

_MISSING = 2

_ATTEMPTS = 5

_MANIFEST_TYPES = ("application/vnd.oci.image.manifest.v1+json, "
                   "application/vnd.oci.image.index.v1+json, "
                   "application/vnd.docker.distribution.manifest.v2+json")


class _NoRedirect(urllib.request.HTTPRedirectHandler):

    def redirect_request(self, req, fp, code, msg, headers, newurl):
        return None


def _open(request: urllib.request.Request, follow: bool = False):
    """
    Open the request.  Follow a redirect without the authorization header.
    """
    opener = urllib.request.build_opener(_NoRedirect)
    for attempt in range(_ATTEMPTS):
        try:
            return opener.open(request, timeout=60)
        except urllib.error.HTTPError as error:
            if follow and error.code in (301, 302, 303, 307, 308):
                return _open(urllib.request.Request(error.headers["Location"]))
            if error.code < 500 and error.code not in (408, 429):
                raise
            if attempt == _ATTEMPTS - 1:
                raise
        except urllib.error.URLError:
            if attempt == _ATTEMPTS - 1:
                raise
        time.sleep(10 * (attempt + 1))
    raise AssertionError("unreachable")


def _split(reference: str) -> tuple[str, str]:
    name, tag = reference.rsplit(":", 1)
    host, repository = name.split("/", 1)
    if host != "ghcr.io":
        raise ValueError(f"the registry of '{reference}' is not ghcr.io")
    return repository, tag


def _token(repository: str) -> str:
    credentials = f"{os.environ['GITHUB_ACTOR']}:{os.environ['TOKEN']}"
    query = urllib.parse.urlencode({
        "service": "ghcr.io",
        "scope": f"repository:{repository}:pull"
    })
    request = urllib.request.Request(f"https://ghcr.io/token?{query}")
    request.add_header(
        "Authorization",
        "Basic " + base64.b64encode(credentials.encode()).decode())
    with _open(request) as response:
        return json.load(response)["token"]


def _manifest(repository: str, tag: str, token: str) -> dict | None:
    request = urllib.request.Request(
        f"https://ghcr.io/v2/{repository}/manifests/{tag}")
    request.add_header("Authorization", f"Bearer {token}")
    request.add_header("Accept", _MANIFEST_TYPES)
    try:
        with _open(request) as response:
            return json.load(response)
    except urllib.error.HTTPError as error:
        if error.code == 404:
            return None
        raise


def _fetch(repository: str, manifest: dict, title: str, token: str,
           path: str) -> None:
    digests = [
        layer["digest"] for layer in manifest["layers"] if layer.get(
            "annotations", {}).get("org.opencontainers.image.title") == title
    ]
    if len(digests) != 1:
        raise ValueError(f"the package has no single layer titled '{title}'")
    request = urllib.request.Request(
        f"https://ghcr.io/v2/{repository}/blobs/{digests[0]}")
    request.add_header("Authorization", f"Bearer {token}")
    for attempt in range(_ATTEMPTS):
        try:
            with _open(request, follow=True) as response, open(path,
                                                               "wb") as dst:
                shutil.copyfileobj(response, dst)
            return
        except (ConnectionError, TimeoutError):
            if attempt == _ATTEMPTS - 1:
                raise
        time.sleep(10 * (attempt + 1))


def main(argv: list[str]) -> int:
    """ Run the command. """
    parser = argparse.ArgumentParser(description="Read a package of ghcr.io.")
    commands = parser.add_subparsers(dest="command", required=True)
    exists = commands.add_parser("exists",
                                 help="check that the package version exists")
    exists.add_argument("reference", help="ghcr.io/OWNER/NAME:TAG")
    fetch = commands.add_parser(
        "fetch", help="write the layer with the title to the file")
    fetch.add_argument("reference", help="ghcr.io/OWNER/NAME:TAG")
    fetch.add_argument("title", help="the title annotation of the layer")
    fetch.add_argument("path", help="the file to write")
    args = parser.parse_args(argv[1:])
    try:
        repository, tag = _split(args.reference)
        token = _token(repository)
        manifest = _manifest(repository, tag, token)
        if manifest is None:
            return _MISSING
        if args.command == "fetch":
            _fetch(repository, manifest, args.title, token, args.path)
    except (urllib.error.URLError, ValueError, KeyError) as error:
        print(f"registry.py: {args.reference}: {error}", file=sys.stderr)
        return 1
    return 0


if __name__ == "__main__":
    sys.exit(main(sys.argv))
