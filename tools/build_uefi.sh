#!/usr/bin/env bash

set -e

build_uefi() {
  board=$1
  echo " => Building ${board}"

  export GCC5_AARCH64_PREFIX=/usr/bin/aarch64-linux-gnu-
  export PYTOOL_TEMPORARILY_IGNORE_NESTED_EDK_PACKAGES=true
  stuart_setup -c Platforms/${board}/PlatformBuild.py
  stuart_update -c Platforms/${board}/PlatformBuild.py
  stuart_build -c Platforms/${board}/PlatformBuild.py TOOL_CHAIN_TAG=GCC5
}

build_uefi $1