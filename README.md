# MU_PLATFORM_ROCKCHIP

MU Platform (Microsoft UEFI) for Rockchip

### Install system runtime packages

    sudo dnf install mono-complete nuget make python3 python3-pip gcc-aarch64-linux-gnu gcc-arm-linux-gnu

Download and install toolchain and add the bin folder to path

https://developer.arm.com/-/media/Files/downloads/gnu/12.2.rel1/binrel/arm-gnu-toolchain-12.2.rel1-x86_64-arm-none-linux-gnueabihf.tar.xz?rev=9929cb6c0e8948f0ba1a621167fcd56d&hash=1259035C716B41C675DCA7D76913684B5AD8C239

### Install dependent Python modules

    pip3 install -r pip-requirements.txt

### Build PinebookPro UEFI

    export GCC5_AARCH64_PREFIX=/usr/bin/aarch64-linux-gnu-
    stuart_setup -c Platforms/Pine64/PinebookPkg/PlatformBuild.py
    stuart_update -c Platforms/Pine64/PinebookPkg/PlatformBuild.py
    stuart_build -c Platforms/Pine64/PinebookPkg/PlatformBuild.py TOOL_CHAIN_TAG=GCC5

### Build PinePhonePro UEFI

    export GCC5_AARCH64_PREFIX=/usr/bin/aarch64-linux-gnu-
    stuart_setup -c Platforms/Pine64/PinePhoneProPkg/PlatformBuild.py
    stuart_update -c Platforms/Pine64/PinePhoneProPkg/PlatformBuild.py
    stuart_build -c Platforms/Pine64/PinePhoneProPkg/PlatformBuild.py TOOL_CHAIN_TAG=GCC5

### Build R5C

    export GCC5_AARCH64_PREFIX=/usr/bin/aarch64-linux-gnu-
    stuart_setup -c Platforms/FriendlyElec/R5CPkg/PlatformBuild.py
    stuart_update -c Platforms/FriendlyElec/R5CPkg/PlatformBuild.py
    stuart_build -c Platforms/FriendlyElec/R5CPkg/PlatformBuild.py TOOL_CHAIN_TAG=GCC5
