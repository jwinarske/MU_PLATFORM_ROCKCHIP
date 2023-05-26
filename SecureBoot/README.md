# Secure Boot

_BL33 refers to UFEI FV_

Download and install toolchain and add the bin folder to path

https://developer.arm.com/-/media/Files/downloads/gnu/12.2.rel1/binrel/arm-gnu-toolchain-12.2.rel1-x86_64-arm-none-linux-gnueabihf.tar.xz?rev=9929cb6c0e8948f0ba1a621167fcd56d&hash=1259035C716B41C675DCA7D76913684B5AD8C239

Build optee-os-tadevkit
   
    pip3 install pycryptodome

    cd optee_os
    git apply ../patches/optee_os/0001-allow-setting-sysroot-for-libgcc-lookup.patch
    git apply ../patches/optee_os/0002-optee-enable-clang-support.patch
    git apply ../patches/optee_os/0003-core-link-add-no-warn-rwx-segments.patch
    git apply ../patches/optee_os/0004-core-Define-section-attributes-for-clang.patch

    make \
    CROSS_COMPILE=arm-none-linux-gnueabihf- \
    PLATFORM=rockchip \
    PLATFORM_FLAVOR=rk3399 \
    CFG_ARM64_core=y \
    CFG_RPMB_FS=y \
    CFG_RPMB_FS_DEV_ID=0 CFG_CORE_HEAP_SIZE=524288 CFG_RPMB_WRITE_KEY=y \
    CFG_CORE_HEAP_SIZE=524288 CFG_CORE_DYN_SHM=y CFG_RPMB_TESTKEY=y \
    CFG_REE_FS=n CFG_CORE_ARM64_PA_BITS=48  CFG_TEE_CORE_LOG_LEVEL=1 \
    CFG_TEE_TA_LOG_LEVEL=1 CFG_SCTLR_ALIGNMENT_CHECK=n \
    CFG_TEE_BENCHMARK=n \
    CFG_CORE_SEL1_SPMC=y \
    CFG_ULIBS_SHARED=y \
    NOWERROR=1 \
    OPTEE_CLIENT_EXPORT=`pwd`/out/usr \
    TEEC_EXPORT=`pwd`/out/usr \
    V=1 \
    -j


Build Trusted App (TA) fTPM

    cd ms-tpm-20-ref
    git submodule update --init --recursive
    git apply ../patches/fTPM/0001-add-enum-to-ta-flags.patch
    cd Samples/ARM32-FirmwareTPM/optee_ta
    make \
    TA_CROSS_COMPILE=aarch64-linux-gnu- \
    TA_CPU=cortex-a55 \
    CFG_FTPM_USE_WOLF=y \
    TA_DEV_KIT_DIR=`pwd`/../../../../optee_os/out/arm-plat-rockchip/export-ta_arm64 \
    OPTEE_CLIENT_EXPORT=`pwd`/../../../../optee_os/out/usr/ \
    TEEC_EXPORT=`pwd`/../../../../optee_os/out/usr/ \
    -I`pwd`/../../../../optee_os \
    CFG_ARM64_ta_arm64=y \
    -j


Build OPTEE OS

   make \
   CROSS_COMPILE=arm-none-linux-gnueabihf- \
   PLATFORM=rockchip \
   PLATFORM_FLAVOR=rk3399 \
   CFG_ARM64_core=y \
   CFG_RPMB_FS=y \
   CFG_RPMB_FS_DEV_ID=0 CFG_CORE_HEAP_SIZE=524288 CFG_RPMB_WRITE_KEY=y \
   CFG_CORE_HEAP_SIZE=524288 CFG_CORE_DYN_SHM=y CFG_RPMB_TESTKEY=y \
   CFG_REE_FS=n CFG_CORE_ARM64_PA_BITS=48  CFG_TEE_CORE_LOG_LEVEL=1 \
   CFG_TEE_TA_LOG_LEVEL=1 CFG_SCTLR_ALIGNMENT_CHECK=n \
   CFG_TEE_BENCHMARK=n \
   CFG_CORE_SEL1_SPMC=y \
   CFG_ULIBS_SHARED=y \
   NOWERROR=1 \
   OPTEE_CLIENT_EXPORT=`pwd`/out/usr \
   TEEC_EXPORT=`pwd`/out/usr \
   NOWERROR=1 \
   EARLY_TA_PATHS=`pwd`/../ms-tpm-20-ref/Samples/ARM32-FirmwareTPM/optee_ta/out/fTPM/${FTPM_UUID}.stripped.elf \
   V=1


Build Trusted Firmware A

   wget https://review.trustedfirmware.org/changes/TF-A%2Ftrusted-firmware-a~16952/revisions/11/archive?format=tgz -O trusted-firmware-a.tar.gz
   mkdir trusted-firmware-a
   tar -xf trusted-firmware-a.tar.gz -C trusted-firmware-a
   cd trusted-firmware-a
   make \
   CROSS_COMPILE=aarch64-linux-gnu- \
   BL32=../optee_os/out/arm-plat-rockchip/core/tee.bin \
   FVP_TSP_RAM_LOCATION=tdram \
   FVP_SHARED_DATA_LOCATION=tdram \
   SPD=opteed \
   V=1


https://optee.readthedocs.io/en/latest/architecture/secure_boot.html?highlight=BL32#armv8-a-using-the-authentication-framework-in-tf-a

https://github.com/ARM-software/arm-trusted-firmware/tree/master/docs/getting_started
https://github.com/ARM-software/arm-trusted-firmware/blob/master/docs/design/auth-framework.rst
https://github.com/ARM-software/arm-trusted-firmware/blob/master/docs/design/trusted-board-boot.rst
