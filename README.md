# MU_PLATFORM_ROCKCHIP

MU Platform (UEFI) for Rockchip

Status:
* PinePhoneProPkg is active dev target
* Full stack building, no pre-built artifacts
* loads and runs from RAM -> custom BL31 -> BL32 -> BL33 using USB boot
* Jumps into UEFI image printing UEFI version
* More work to create a working image
* Not tested on Pinebook Pro

Goals:
* Linux FDT boot with Secure Boot enabled
  Red Hat Enterprise Linux (RHEL)
  Fedora
* Windows 11 with Secure Boot enabled
* ARM Compliance


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

### Run stack on Pine Phone Pro using USB

Press and hold recovery button while plugging in USB to Pine Phone Pro device.  Release button after unit is powered on.  Issue:

    ./mkrun.sh

### JTAG Setup

Requires changing GPIO pin muxing for SD card pins to JTAG:

    sudo dnf install openocd
    sudo openocd -s ../tcl -f ./JTAG/rk3399/openocd_rk3399.cfg -c "adapter speed 15000"
