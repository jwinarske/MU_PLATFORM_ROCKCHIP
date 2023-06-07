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

load images direct to RAM using USB:

    tools/extractelf.py Silicon/Arm/TFA/build/rk3399/debug/bl31/bl31.elf tfa
    tools/extractelf.py Silicon/OP-TEE/optee_os/out/arm-plat-rockchip/core/tee.elf optee

    sudo IPL/levinboot/_build/tools/usbtool --call IPL/levinboot/_build/sramstage-usb.bin \
    --bulk \
    --load 40000 tfa_0x00040000.bin \
    --load ff3b0000 tfa_0xff3b0000.bin \
    --load ff8c0000 tfa_0xff8c0000.bin \
    --load ff8c1000 tfa_0xff8c1000.bin \
    --load ff8c2000 tfa_0xff8c2000.bin \
    --load 30000000 optee_0x30000000.bin \
    --load 30200000 optee_0x30200000.bin \
    --load A00000 Build/PinePhoneProPkg/DEBUG_GCC5/FV/PINEPHONEPRO.fd \
    --load 00B00000 dtb/rk3399-pinephone-pro.dtb \
    --load 04000000 IPL/levinboot/_build/dramstage.bin \
    --start 04000000 4102000

* We ignore bin files that have size of zero.  e.g. tfa_0xff8c2000.bin

The above step requires a matching image structure for bl32 and bl33 passed.  This happens for the SD card image.

### Build R5C

    export GCC5_AARCH64_PREFIX=/usr/bin/aarch64-linux-gnu-
    stuart_setup -c Platforms/FriendlyElec/R5CPkg/PlatformBuild.py
    stuart_update -c Platforms/FriendlyElec/R5CPkg/PlatformBuild.py
    stuart_build -c Platforms/FriendlyElec/R5CPkg/PlatformBuild.py TOOL_CHAIN_TAG=GCC5

### Full Stack Build

See tools/mksdimg.sh

### JTAG Setup

    sudo dnf install openocd
    sudo openocd -s ../tcl -f /usr/share/openocd/scripts/interface/jlink.cfg -f ./JTAG/rk3399/openocd_rk3399.cfg -c "adapter speed 15000"
