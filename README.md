# MU_PLATFORM_ROCKCHIP

MU Platform (UEFI) for Rockchip

Status

* PinePhoneProPkg is active dev target

* Full stack building with latest ATF,OP-TEE, and MU

* loads and runs from RAM -> custom BL31 -> BL32 -> BL33 using USB boot

* Jumps into UEFI image printing UEFI version

* Current focus area is UEFI

* USB boot not yet tested on Pinebook Pro

* optee_os needs ASLR seed implementation
https://patchwork.ozlabs.org/project/uboot/patch/20200331094001.13441-3-troy.lin@rock-chips.com/
https://github.com/rockchip-linux/kernel/issues/223


Project Goals
* Linux FDT boot with Secure Boot enabled
  * Red Hat Enterprise Linux (RHEL)
  * Fedora
* Windows 11 with Secure Boot enabled
* ARM System Ready Compliance

Dev Environments:
* Fedora
* Ubuntu
* Windows not supported, PRs welcome

### Install system runtime packages

Fedora

    sudo dnf install mono-complete nuget make python3 python3-pip gcc-aarch64-linux-gnu libusb-devel

Ubuntu

    sudo apt-get install mono-complete nuget make python3 python3-pip gcc-aarch64-linux-gnu gcc-arm-none-eabi build-essential git libusb-1.0-0-dev device-tree-compiler


Download and install the following toolchains and add bin folders to PATH

* arm-gnu-toolchain-12.2.rel1-x86_64-arm-none-linux-gnueabihf
* arm-gnu-toolchain-12.2.mpacbti-rel1-x86_64-arm-none-eabi

https://developer.arm.com/downloads/-/arm-gnu-toolchain-downloads

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

### Build Stack

    cd tools
    ./build.sh

### Build stack and Run on Pine Phone Pro using USB

Press and hold recovery button while plugging in USB to Pine Phone Pro device.  Release button after unit is powered on.  Issue:

    ./mkrun.sh

### JTAG Setup

Requires changing GPIO pin muxing for SD card pins to JTAG:

    sudo dnf install openocd
    sudo openocd -s ../tcl -f ./JTAG/rk3399/openocd_rk3399.cfg -c "adapter speed 15000"
