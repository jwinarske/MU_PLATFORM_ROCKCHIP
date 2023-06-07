# Secure Boot

_BL33 refers to UFEI FV_

Download and install toolchain and add the bin folder to path

https://developer.arm.com/-/media/Files/downloads/gnu/12.2.rel1/binrel/arm-gnu-toolchain-12.2.rel1-x86_64-arm-none-linux-gnueabihf.tar.xz?rev=9929cb6c0e8948f0ba1a621167fcd56d&hash=1259035C716B41C675DCA7D76913684B5AD8C239

### Build optee-os-tadevkit
   
    pip3 install pycryptodome

    cd Silicon/OP-TEE/optee_os
    git apply ../../../SecureBoot/patches/optee_os/0001-allow-setting-sysroot-for-libgcc-lookup.patch
    git apply ../../../SecureBoot/patches/optee_os/0002-optee-enable-clang-support.patch
    git apply ../../../SecureBoot/patches/optee_os/0003-core-link-add-no-warn-rwx-segments.patch
    git apply ../../../SecureBoot/patches/optee_os/0004-core-Define-section-attributes-for-clang.patch

    export PATH=$PATH:/home/joel/Downloads/arm-gnu-toolchain-12.2.rel1-x86_64-arm-none-linux-gnueabihf/bin

    make \
    CROSS_COMPILE=arm-none-linux-gnueabihf- \
    PLATFORM=rockchip-rk3399 \
    CFG_ARM64_core=y \
    CFG_RPMB_FS=y \
    CFG_RPMB_FS_DEV_ID=0 \
    CFG_CORE_HEAP_SIZE=524288 \
    CFG_RPMB_WRITE_KEY=y \
    CFG_CORE_HEAP_SIZE=524288 \
    CFG_CORE_DYN_SHM=y \
    CFG_RPMB_TESTKEY=y \
    CFG_REE_FS=n \
    CFG_CORE_ARM64_PA_BITS=48 \
    CFG_TEE_CORE_LOG_LEVEL=1 \
    CFG_TEE_TA_LOG_LEVEL=1 \
    CFG_SCTLR_ALIGNMENT_CHECK=n \
    CFG_TEE_BENCHMARK=n \
    CFG_CORE_SEL1_SPMC=y \
    CFG_ULIBS_SHARED=y \
    NOWERROR=1 \
    OPTEE_CLIENT_EXPORT=`pwd`/out/usr \
    TEEC_EXPORT=`pwd`/out/usr \
    all -j


### Build Trusted App (TA) fTPM

   
    cd Silicon/MSFT/ms-tpm-20-ref
    git submodule update --init --recursive
    git apply ../../../SecureBoot/patches/fTPM/0001-add-enum-to-ta-flags.patch
    cd Samples/ARM32-FirmwareTPM/optee_ta
    make \
    TA_CROSS_COMPILE=aarch64-linux-gnu- \
    TA_CPU=cortex-a53 \
    CFG_FTPM_USE_WOLF=y \
    TA_DEV_KIT_DIR=`pwd`/../../../../../OP-TEE/optee_os/out/arm-plat-rockchip/export-ta_arm64 \
    OPTEE_CLIENT_EXPORT=`pwd`/../../../../../OP-TEE/optee_os/out/usr/ \
    TEEC_EXPORT=`pwd`/../../../../../OP-TEE/optee_os/out/usr/ \
    -I`pwd`/../../../../../OP-TEE/optee_os \
    CFG_ARM64_ta_arm64=y \
    -j


### Build OPTEE OS
 
    make \
    CROSS_COMPILE=arm-none-linux-gnueabihf- \
    PLATFORM=rockchip-rk3399 \
    CFG_ARM64_core=y \
    CFG_TEE_CORE_LOG_LEVEL=4 \
    CFG_TEE_TA_LOG_LEVEL=4 \
    CFG_RPMB_FS=y \
    CFG_RPMB_FS_DEV_ID=0 \
    CFG_RPMB_WRITE_KEY=y \
    CFG_CORE_HEAP_SIZE=524288 \
    CFG_CORE_DYN_SHM=y \
    CFG_RPMB_TESTKEY=y \
    CFG_REE_FS=n \
    CFG_TEE_BENCHMARK=n \
    CFG_CORE_ARM64_PA_BITS=48 \
    CFG_SCTLR_ALIGNMENT_CHECK=n \
    CFG_ULIBS_SHARED=y \
    CFG_CORE_SEL1_SPMC=y \
    NOWERROR=1 \
    OPTEE_CLIENT_EXPORT=`pwd`/out/usr \
    TEEC_EXPORT=`pwd`/out/usr \
    EARLY_TA_PATHS=`pwd`/../../MSFT/ms-tpm-20-ref/Samples/ARM32-FirmwareTPM/optee_ta/out/fTPM/bc50d971-d4c9-42c4-82cb-343fb7f37896.stripped.elf \
    V=1 all mem_usage -j


Determine entry point address matches CFG_TZDRAM_START

    readelf -h ./out/arm-plat-rockchip/core/tee.elf

CFG_TZDRAM_START ?= 0x30000000
CFG_TZDRAM_SIZE  ?= 0x02000000
CFG_SHMEM_START  ?= 0x32000000
CFG_SHMEM_SIZE   ?= 0x00400000

Locate pager to TZDRAM CFG_TZDRAM_START

    ./out/arm-plat-rockchip/core/tee-pager_v2.bin

### Build Trusted Firmware A with SPD=opteed

BL2 (levinboot) -> BL31 (TFA) -> BL32 (OP-TEE) -> BL33 (UEFI)

Download AArch32 bare-metal target (arm-none-eabi) and add to path: 

https://developer.arm.com/downloads/-/arm-gnu-toolchain-downloads

https://developer.arm.com/-/media/Files/downloads/gnu/12.2.mpacbti-rel1/binrel/arm-gnu-toolchain-12.2.mpacbti-rel1-x86_64-arm-none-eabi.tar.xz?rev=71e595a1f2b6457bab9242bc4a40db90&hash=37B0C59767BAE297AEB8967E7C54705BAE9A4B95

    cd Silicon/Arm/TFA
    make \
    CROSS_COMPILE=aarch64-linux-gnu- \
    PLAT=rk3399 \
    SPD=opteed \
    BL32=../../OP-TEE/optee_os/out/arm-plat-rockchip/core/tee-header_v2.bin \
    BL32_EXTRA1=../../OP-TEE/optee_os/out/arm-plat-rockchip/core/tee-pager_v2.bin \
    BL32_EXTRA2=../../OP-TEE/optee_os/out/arm-plat-rockchip/core/tee-pageable_v2.bin \
    CFG_TEE_TA_LOG_LEVEL=4 \
    CFG_TA_DEBUG=1 \
    FEATURE_DETECTION=1 \
    DEBUG=1 \
    clean bl31 \
    V=1

Print fip.bin info

    tools/fiptool/fiptool --verbose info ./build/rk3399/release/fip.bin
    DEBUG: toc_header[name]: 0xAA640001
    DEBUG: toc_header[serial_number]: 0x12345678
    DEBUG: toc_header[flags]: 0x0
    EL3 Runtime Firmware BL31: offset=0x88, size=0xFF882000, cmdline="--soc-fw", sha256=d9739120b9952e0cfda1b4187f26d256883383e66ff676f4a710ab5f32fa34ae
    Secure Payload BL32 (Trusted OS): offset=0xFF882088, size=0xA0DB4, cmdline="--tos-fw", sha256=d61b5b820ac75a29e21f032787ea912f8e59dcd65c5e5442d212398ef86ff1e8

Useful Variables:

* CFG_TA_DEBUG: Enables debug logs in the Terminal_1 console.

* CFG_TEE_TA_LOG_LEVEL: Defines the log level used for the debug messages.

* CFG_TA_MEASURED_BOOT: Enables support for measured boot on the fTPM.

* CFG_TA_EVENT_LOG_SIZE: Defines the size, in bytes, of the larger event log that the fTPM is able to store, as this buffer is allocated at build time. This must be at least the same as the size of the event log generated by TF-A. If this build option is not defined, the fTPM falls back to a default value of 1024 bytes, which is enough for this PoC, so this variable is not defined in FTPM_FLAGS.


https://optee.readthedocs.io/en/latest/architecture/secure_boot.html?highlight=BL32#armv8-a-using-the-authentication-framework-in-tf-a

https://github.com/ARM-software/arm-trusted-firmware/tree/master/docs/getting_started
https://github.com/ARM-software/arm-trusted-firmware/blob/master/docs/design/auth-framework.rst
https://github.com/ARM-software/arm-trusted-firmware/blob/master/docs/design/trusted-board-boot.rst

https://trustedfirmware-a.readthedocs.io/en/latest/components/secure-partition-manager-mm.html

https://trustedfirmware-a.readthedocs.io/en/latest/design_documents/measured_boot_poc.html
https://github.com/tpm2-software/tpm2-tools

* Firmware Image Package (FIP)
https://trustedfirmware-a.readthedocs.io/en/v2.6/design/firmware-design.html#firmware-image-package-fip

* MU Example
https://github.com/ms-iot/MU_PLATFORM_NXP

* edk2 aarch64 platform examples
https://github.com/tianocore/edk2-platforms

### Debug Boot Log

```
NOTICE:  BL31: v2.9(debug):v2.9.0-72-gab55eade3-dirty
NOTICE:  BL31: Built : 11:28:48, Jun  7 2023
INFO:    GICv3 with legacy support detected.
INFO:    ARM GICv3 driver initialized in EL3
INFO:    Maximum SPI INTID supported: 287
INFO:    plat_rockchip_pmu_init(1624): pd status 93cf833e
INFO:    BL31: Initializing runtime services
INFO:    BL31: cortex_a53: CPU workaround for 855873 was applied
WARNING: BL31: cortex_a53: CPU workaround for 1530924 was missing!
INFO:    BL31: Initializing BL32
D/TC:0   get_aslr_seed:1595 No fdt
D/TC:0   plat_get_aslr_seed:118 Warning: no ASLR seed
D/TC:0   add_phys_mem:665 VCORE_UNPG_RX_PA type TEE_RAM_RX 0x30000000 size 0x000a9000
D/TC:0   add_phys_mem:665 VCORE_UNPG_RW_PA type TEE_RAM_RW 0x300a9000 size 0x00157000
D/TC:0   add_phys_mem:665 ta_base type TA_RAM 0x30200000 size 0x01e00000
D/TC:0   add_phys_mem:665 GIC_BASE type IO_SEC 0xfee00000 size 0x00200000
D/TC:0   add_phys_mem:665 CFG_EARLY_CONSOLE_BASE type IO_NSEC 0xff000000 size 0x00200000
D/TC:0   add_phys_mem:665 SGRF_BASE type IO_SEC 0xff200000 size 0x00200000
D/TC:0   add_phys_mem:665 TEE_SHMEM_START type NSEC_SHM 0x32000000 size 0x00400000
D/TC:0   add_va_space:705 type RES_VASPACE size 0x00a00000
D/TC:0   add_va_space:705 type SHM_VASPACE size 0x02000000
D/TC:0   dump_mmap_table:831 type TEE_RAM_RX   va 0x30000000..0x300a8fff pa 0x30000000..0x300a8fff size 0x000a9000 (smallpg)
D/TC:0   dump_mmap_table:831 type TEE_RAM_RW   va 0x300a9000..0x301fffff pa 0x300a9000..0x301fffff size 0x00157000 (smallpg)
D/TC:0   dump_mmap_table:831 type SHM_VASPACE  va 0x30200000..0x321fffff pa 0x00000000..0x01ffffff size 0x02000000 (pgdir)
D/TC:0   dump_mmap_table:831 type RES_VASPACE  va 0x32200000..0x32bfffff pa 0x00000000..0x009fffff size 0x00a00000 (pgdir)
D/TC:0   dump_mmap_table:831 type TA_RAM       va 0x32c00000..0x349fffff pa 0x30200000..0x31ffffff size 0x01e00000 (pgdir)
D/TC:0   dump_mmap_table:831 type NSEC_SHM     va 0x34a00000..0x34dfffff pa 0x32000000..0x323fffff size 0x00400000 (pgdir)
D/TC:0   dump_mmap_table:831 type IO_SEC       va 0x34e00000..0x34ffffff pa 0xfee00000..0xfeffffff size 0x00200000 (pgdir)
D/TC:0   dump_mmap_table:831 type IO_NSEC      va 0x35000000..0x351fffff pa 0xff000000..0xff1fffff size 0x00200000 (pgdir)
D/TC:0   dump_mmap_table:831 type IO_SEC       va 0x35200000..0x353fffff pa 0xff200000..0xff3fffff size 0x00200000 (pgdir)
D/TC:0   core_mmu_xlat_table_alloc:526 xlat tables used 1 / 5
D/TC:0   core_mmu_xlat_table_alloc:526 xlat tables used 2 / 5
I/TC: 
I/TC: No non-secure external DT
D/TC:0 0 get_console_node_from_dt:74 No console directive from DTB
I/TC: OP-TEE version: 3.21.0-168-g322cf9e33-dev (gcc version 12.2.1 20220819 (Red Hat Cross 12.2.1-2) (GCC)) #56 Wed Jun  7 18:28:48 UTC 2023 aarch64
I/TC: WARNING: This OP-TEE configuration might be insecure!
I/TC: WARNING: Please check https://optee.readthedocs.io/en/latest/architecture/porting_guidelines.html
I/TC: Primary CPU initializing
D/TC:0 0 boot_init_primary_late:1463 Executing at offset 0 with virtual load address 0x30000000
D/TC:0 0 call_preinitcalls:21 level 2 mobj_mapped_shm_init()
D/TC:0 0 mobj_mapped_shm_init:463 Shared memory address range: 30200000, 32200000
D/TC:0 0 call_initcalls:40 level 1 register_time_source()
D/TC:0 0 call_initcalls:40 level 1 teecore_init_pub_ram()
D/TC:0 0 call_initcalls:40 level 2 probe_dt_drivers_early()
D/TC:0 0 call_initcalls:40 level 3 platform_init()
D/TC:0 0 platform_secure_ddr_region:35 protecting region 1: 0x30000000-0x32000000
D/TC:0 0 call_initcalls:40 level 3 check_ta_store()
D/TC:0 0 check_ta_store:417 TA store: "early TA"
D/TC:0 0 check_ta_store:417 TA store: "REE"
D/TC:0 0 call_initcalls:40 level 3 early_ta_init()
D/TC:0 0 early_ta_init:56 Early TA bc50d971-d4c9-42c4-82cb-343fb7f37896 size 165734 (compressed, uncompressed 351232)
D/TC:0 0 early_ta_init:56 Early TA 023f8f1a-292a-432b-8fc4-de8471358067 size 33338 (compressed, uncompressed 59280)
D/TC:0 0 call_initcalls:40 level 3 verify_pseudo_tas_conformance()
D/TC:0 0 call_initcalls:40 level 3 tee_cryp_init()
D/TC:0 0 call_initcalls:40 level 4 tee_fs_init_key_manager()
D/TC:0 0 call_initcalls:40 level 5 probe_dt_drivers()
D/TC:0 0 call_initcalls:40 level 6 mobj_init()
D/TC:0 0 call_initcalls:40 level 6 default_mobj_init()
D/TC:0 0 call_initcalls:40 level 7 release_probe_lists()
D/TC:0 0 call_finalcalls:59 level 1 release_external_dt()
I/TC: Primary CPU switching to normal world boot
INFO:    BL31: Preparing for EL3 exit to normal world
INFO:    Entry point address = 0xa00000
INFO:    SPSR = 0x3c5
UEFI firmware (version  built at 09:12:48 on Jun  7 2023)
```