# Welcome to [MU_PLATFORM_ROCKCHIP](https://github.com/jwinarske/MU_PLATFORM_ROCKCHIP/)


MU Platform (UEFI) for Rockchip

Status

* Currently a Work In Progress (WIP)
* PinePhonePro is active dev target
* Full stack building with latest ATF,OP-TEE, and MU
* loads and runs from RAM -> custom BL31 -> BL32 -> BL33 using USB boot
* Current focus area is UEFI
* USB boot not yet tested on Pinebook Pro
* optee_os needs crypto drivers
* Update TAs to use mbedtls - common and friendly licensing
* Jumps into UEFI image printing UEFI version
* Test out (ATF + SPM) EL3


Project Goals
* Make Rockchip an option for enterprise deployment
* OSS aarch64 Secure Boot (Measured Boot + fTPM)
* Linux FDT boot with Secure Boot enabled
  * Red Hat Enterprise Linux (RHEL)
  * Fedora
* Windows 11 Bitlocker
* ARM System Ready Compliance

Dev Environments:
* Fedora
* Ubuntu
* Windows not supported, PRs welcome

### Install system runtime packages

Fedora

    sudo dnf install mono-complete nuget make python3 python3-pip gcc-aarch64-linux-gnu libusb-devel cmake

Ubuntu

    sudo apt-get install mono-complete nuget make python3 python3-pip gcc-aarch64-linux-gnu gcc-arm-none-eabi build-essential git libusb-1.0-0-dev device-tree-compiler ninja-build
    sudo snap install cmake --classic


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
    stuart_setup -c Platforms/Pine64/PinePhonePro/PlatformBuild.py
    stuart_update -c Platforms/Pine64/PinePhonePro/PlatformBuild.py
    stuart_build -c Platforms/Pine64/PinePhonePro/PlatformBuild.py TOOL_CHAIN_TAG=GCC5

### Build Stack

    cd tools
    ./build.sh

### Build stack and Run on Pine Phone Pro using USB

Press and hold recovery button while plugging in USB to Pine Phone Pro device.  Release button after unit is powered on.  Issue:

    ./mkrun.sh

### Serial Console Cable

    sudo minicom -D /dev/ttyUSB0 -b 115200

Ubuntu requires or console cable will not show up as /dev/ttyUSB0
    
    sudo apt remove brltty
    

### JTAG Setup

Requires changing GPIO pin muxing for SD card pins to JTAG:

    sudo dnf install openocd
    sudo openocd -s ../tcl -f ./JTAG/rk3399/openocd_rk3399.cfg -c "adapter speed 15000"

### Current Boot Log

```
[15157311] handing off to BL31
NOTICE:  BL31: v2.9(debug):v2.9.0-dirty
NOTICE:  BL31: Built : 13:54:11, Jun 11 2023
INFO:    GICv3 with legacy support detected.
INFO:    ARM GICv3 driver initialized in EL3
INFO:    Maximum SPI INTID supported: 287
INFO:    plat_rockchip_pmu_init(1624): pd status 93cf833e
INFO:    BL31: Initializing runtime services
INFO:    BL31: cortex_a53: CPU workaround for 855873 was applied
INFO:    BL31: cortex_a53: CPU workaround for 1530924 was applied
INFO:    BL31: Initializing BL32
D/TC:0   plat_get_aslr_seed:116 Warning: no ASLR seed
D/TC:0   add_phys_mem:635 ROUNDDOWN((0xF8000000 + 0x06E00000), CORE_MMU_PGDIR_SIZE) type IO_SEC 0xfee00000 size 0x00200000
D/TC:0   add_phys_mem:635 ROUNDDOWN((0xF8000000 + 0x071A0000), CORE_MMU_PGDIR_SIZE) type IO_NSEC 0xff000000 size 0x00200000
D/TC:0   add_phys_mem:635 ROUNDDOWN((0xF8000000 + 0x07330000), CORE_MMU_PGDIR_SIZE) type IO_SEC 0xff200000 size 0x00200000
D/TC:0   add_phys_mem:635 TEE_SHMEM_START type NSEC_SHM 0x32000000 size 0x00400000
D/TC:0   add_phys_mem:635 TA_RAM_START type TA_RAM 0x30200000 size 0x01e00000
D/TC:0   add_phys_mem:635 VCORE_UNPG_RW_PA type TEE_RAM_RW 0x30070000 size 0x00190000
D/TC:0   add_phys_mem:635 VCORE_UNPG_RX_PA type TEE_RAM_RX 0x30000000 size 0x00070000
D/TC:0   add_va_space:675 type RES_VASPACE size 0x00a00000
D/TC:0   add_va_space:675 type SHM_VASPACE size 0x02000000
D/TC:0   dump_mmap_table:800 type TEE_RAM_RX   va 0x30000000..0x3006ffff pa 0x30000000..0x3006ffff size 0x00070000 (smallpg)
D/TC:0   dump_mmap_table:800 type TEE_RAM_RW   va 0x30070000..0x301fffff pa 0x30070000..0x301fffff size 0x00190000 (smallpg)
D/TC:0   dump_mmap_table:800 type SHM_VASPACE  va 0x30200000..0x321fffff pa 0x00000000..0x01ffffff size 0x02000000 (pgdir)
D/TC:0   dump_mmap_table:800 type RES_VASPACE  va 0x32200000..0x32bfffff pa 0x00000000..0x009fffff size 0x00a00000 (pgdir)
D/TC:0   dump_mmap_table:800 type TA_RAM       va 0x32c00000..0x349fffff pa 0x30200000..0x31ffffff size 0x01e00000 (pgdir)
D/TC:0   dump_mmap_table:800 type NSEC_SHM     va 0x34a00000..0x34dfffff pa 0x32000000..0x323fffff size 0x00400000 (pgdir)
D/TC:0   dump_mmap_table:800 type IO_SEC       va 0x34e00000..0x34ffffff pa 0xfee00000..0xfeffffff size 0x00200000 (pgdir)
D/TC:0   dump_mmap_table:800 type IO_NSEC      va 0x35000000..0x351fffff pa 0xff000000..0xff1fffff size 0x00200000 (pgdir)
D/TC:0   dump_mmap_table:800 type IO_SEC       va 0x35200000..0x353fffff pa 0xff200000..0xff3fffff size 0x00200000 (pgdir)
D/TC:0   core_mmu_xlat_table_alloc:526 xlat tables used 1 / 5
D/TC:0   core_mmu_xlat_table_alloc:526 xlat tables used 2 / 5
I/TC: 
I/TC: OP-TEE version: 3.21.0 (gcc version 11.3.0 (Ubuntu 11.3.0-1ubuntu1~22.04.1)) #2 Sun Jun 11 04:54:09 UTC 2023 aarch64
I/TC: WARNING: This OP-TEE configuration might be insecure!
I/TC: WARNING: Please check https://optee.readthedocs.io/en/latest/architecture/porting_guidelines.html
I/TC: Primary CPU initializing
D/TC:0 0 boot_init_primary_late:1457 Executing at offset 0 with virtual load address 0x30000000
D/TC:0 0 call_preinitcalls:21 level 2 mobj_mapped_shm_init()
D/TC:0 0 mobj_mapped_shm_init:463 Shared memory address range: 30200000, 32200000
D/TC:0 0 call_initcalls:40 level 1 register_time_source()
D/TC:0 0 call_initcalls:40 level 1 teecore_init_pub_ram()
D/TC:0 0 call_initcalls:40 level 3 platform_init()
D/TC:0 0 platform_secure_ddr_region:35 protecting region 1: 0x30000000-0x32000000
D/TC:0 0 call_initcalls:40 level 3 check_ta_store()
D/TC:0 0 check_ta_store:417 TA store: "REE"
D/TC:0 0 call_initcalls:40 level 3 verify_pseudo_tas_conformance()
D/TC:0 0 call_initcalls:40 level 3 tee_cryp_init()
D/TC:0 0 call_initcalls:40 level 4 tee_fs_init_key_manager()
D/TC:0 0 call_initcalls:40 level 6 mobj_init()
D/TC:0 0 call_initcalls:40 level 6 default_mobj_init()
I/TC: Primary CPU switching to normal world boot
INFO:    BL31: Preparing for EL3 exit to normal world
INFO:    Entry point address = 0xa00000
INFO:    SPSR = 0x3c5
UEFI firmware (version 0.1 built at 10:35:20 on Jun 13 2023)
add-symbol-file /home/joel/development/MU_PLATFORM_ROCKCHIP/Build/PinePhonePro/DEBUG_GCC5/AARCH64/ArmPlatformPkg/PrePi/PeiUniCore/DEBUG/ArmPlatformPrePiUniCore.dll 0xA00240
ArmIsMpCore
add-symbol-file /home/joel/development/MU_PLATFORM_ROCKCHIP/Build/PinePhonePro/DEBUG_GCC5/AARCH64/MdeModulePkg/Core/Dxe/DxeMain/DEBUG/DxeCore.dll 0xBF235000
Loading DxeCore at 0x00BF234000 EntryPoint=0x00BF23F3C8
CoreInitializeMemoryServices:
  BaseAddress - 0xC00000 Length - 0xBB400000 MinimalMemorySizeNeeded - 0x5065000
```