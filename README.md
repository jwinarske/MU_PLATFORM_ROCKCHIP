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
    export PYTOOL_TEMPORARILY_IGNORE_NESTED_EDK_PACKAGES=true
    stuart_setup -c Platforms/Pine64/PinebookPkg/PlatformBuild.py
    stuart_update -c Platforms/Pine64/PinebookPkg/PlatformBuild.py
    stuart_build -c Platforms/Pine64/PinebookPkg/PlatformBuild.py TOOL_CHAIN_TAG=GCC5

### Build PinePhonePro UEFI

    export GCC5_AARCH64_PREFIX=/usr/bin/aarch64-linux-gnu-
    export PYTOOL_TEMPORARILY_IGNORE_NESTED_EDK_PACKAGES=true
    stuart_setup -c Platforms/Pine64/PinePhoneProPkg/PlatformBuild.py
    stuart_update -c Platforms/Pine64/PinePhoneProPkg/PlatformBuild.py
    stuart_build -c Platforms/Pine64/PinePhoneProPkg/PlatformBuild.py TOOL_CHAIN_TAG=GCC5

levinboot for loading via USB:

    sudo tools/usbtool --call sramstage-usb.bin --bulk --load 4200000 path/to/trusted-firmware-a/build/rk3399/release/bl31/bl31.elf --load 4000000 dramstage.bin --start 4000000 4102000

### Build R5C

    export GCC5_AARCH64_PREFIX=/usr/bin/aarch64-linux-gnu-
    stuart_setup -c Platforms/FriendlyElec/R5CPkg/PlatformBuild.py
    stuart_update -c Platforms/FriendlyElec/R5CPkg/PlatformBuild.py
    stuart_build -c Platforms/FriendlyElec/R5CPkg/PlatformBuild.py TOOL_CHAIN_TAG=GCC5

### levin boot usb loader or Pinebook Pro

    CROSS=aarch64-linux-gnu
    export CC=$CROSS-gcc
    export OBJCOPY=$CROSS-objcopy
    export LD=$CROSS-ld

    cd IPL/levinboot
    git apply ../../SecureBoot/patches/0001-Fix-for-aarch64-linux-gnu-gcc-12.2.1.patch

    mkdir _build && cd _build
    CFLAGS="-O3 -mno-outline-atomics" ../configure.py --payload-{lz4,gzip,zstd,initcpio,sd,emmc,nvme,spi} --with-tf-a-headers ../../../Silicon/Arm/TFA/include/export --boards pbp
    ninja

### JTAG Setup

    sudo dnf install openocd
    sudo openocd -s ../tcl -f /usr/share/openocd/scripts/interface/jlink.cfg -f ./JTAG/rk3399/openocd_rk3399.cfg -c "adapter speed 15000"
