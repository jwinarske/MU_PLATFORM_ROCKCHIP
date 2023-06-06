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
    EARLY_TA_PATHS=`pwd`/../../MSFT/ms-tpm-20-ref/Samples/ARM32-FirmwareTPM/optee_ta/out/fTPM/bc50d971-d4c9-42c4-82cb-343fb7f37896.stripped.elf \
    V=1 all -j

Artifacts
    ./out/arm-plat-rockchip/core/tee-pager_v2.bin
    ./out/arm-plat-rockchip/core/tee-header_v2.bin
    ./out/arm-plat-rockchip/core/tee-raw.bin
    ./out/arm-plat-rockchip/core/tee.bin
    ./out/arm-plat-rockchip/core/tee-pageable_v2.bin
    ./out/arm-plat-rockchip/core/tee.elf
    ./out/arm-plat-rockchip/export-ta_arm64/lib/87bb6ae8-4b1d-49fe-9986-2b966132c309.elf
    ./out/arm-plat-rockchip/export-ta_arm64/lib/be807bbd-81e1-4dc4-bd99-3d363f240ece.elf
    ./out/arm-plat-rockchip/export-ta_arm64/lib/4b3d937e-d57e-418b-8673-1c04f2420226.elf
    ./out/arm-plat-rockchip/export-ta_arm64/lib/71855bba-6055-4293-a63f-b0963a737360.elf


include core/arch/arm/cpu/cortex-armv8-0.mk
$(call force,CFG_TEE_CORE_NB_CORE,6)
$(call force,CFG_ARM_GICV3,y)
CFG_CRYPTO_WITH_CE ?= y

CFG_TZDRAM_START ?= 0x30000000
CFG_TZDRAM_SIZE  ?= 0x02000000
CFG_SHMEM_START  ?= 0x32000000
CFG_SHMEM_SIZE   ?= 0x00400000


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
    clean all fip \
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