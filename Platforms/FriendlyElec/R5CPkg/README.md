Build steps

    export GCC5_AARCH64_PREFIX=/usr/bin/aarch64-linux-gnu-
    stuart_setup -c Platforms/FriendlyElec/R5CPkg/PlatformBuild.py
    stuart_update -c Platforms/FriendlyElec/R5CPkg/PlatformBuild.py
    stuart_build -c Platforms/FriendlyElec/R5CPkg/PlatformBuild.py TOOL_CHAIN_TAG=GCC5
