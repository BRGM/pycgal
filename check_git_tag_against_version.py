import subprocess
import shlex
import re
from packaging.version import parse as vparse

from pycgal import __version__ as version

src_version = vparse(version)

description = subprocess.check_output(shlex.split("git describe")).strip()
if type(description) is bytes:
    description = description.decode("utf-8")
git_info = re.match("(v\d+\.\d+\.\d+)(.*)", description)
assert (
    git_info is not None
), f"Could not retrieve version information from git description: {description}"

vtag, ahead = git_info.groups()
git_version = vparse(vtag)

if len(ahead) == 0:
    print(f"Exactly on tag: {vtag}")
    assert src_version == git_version, f"Version is v{version} but should be {vtag}!"
else:
    print(f"Ahead of tag: {vtag}")
    assert (
        src_version > git_version
    ), f"Version is v{version} but should be ahead of {vtag}!"
