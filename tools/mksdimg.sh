#!/usr/bin/env bash

# set -x

BL31=../Silicon/Arm/TFA/build/rk3399/debug/bl31/bl31.elf
BL32=../Silicon/OP-TEE/optee_os/out/arm-plat-rockchip/core/tee.elf

TRUST_INI=RK3399TRUST.ini
MINIALL_INI=RK3399MINIALL.ini

UEFI_BUILD_TYPE=DEBUG_GCC5

RKBIN=../Silicon/Rockchip/rkbin

# SD Image Layout
SD_BLOCK_SIZE=512

# UEFI
SD_IMG_OFFSET_UEFI=$((1024 * 1024 / $SD_BLOCK_SIZE)) # 0x100000


build_ta_sdk() {

  pushd ../Silicon/OP-TEE/optee_os
  git reset --hard
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

  popd
}


build_ftpm() {

  pushd ../Silicon/MSFT/ms-tpm-20-ref/
  git submodule update --init --recursive
  git reset --hard
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

  popd
}

build_optee_os() {

  pushd ../Silicon/OP-TEE/optee_os

  make \
  CROSS_COMPILE=arm-none-linux-gnueabihf- \
  PLATFORM=rockchip-rk3399 \
  CFG_ARM64_core=y \
  CFG_RPMB_FS=y \
  CFG_RPMB_FS_DEV_ID=0 \
  CFG_CORE_HEAP_SIZE=524288 \
  CFG_RPMB_WRITE_KEY=y \
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
  V=1 all mem_usage -j

  readelf -h ./out/arm-plat-rockchip/core/tee.elf

  popd
}

build_atf() {
  pushd ../Silicon/Arm/TFA

  make \
  CROSS_COMPILE=aarch64-linux-gnu- \
  PLAT=rk3399 \
  SPD=opteed \
  CFG_TEE_TA_LOG_LEVEL=4 \
  CFG_TA_DEBUG=1 \
  DEBUG=1 \
  BL32=../../OP-TEE/optee_os/out/arm-plat-rockchip/core/tee-header_v2.bin \
  BL32_EXTRA1=../../OP-TEE/optee_os/out/arm-plat-rockchip/core/tee-pager_v2.bin \
  BL32_EXTRA2=../../OP-TEE/optee_os/out/arm-plat-rockchip/core/tee-pageable_v2.bin \
  V=1 bl31 -j

  readelf -h ./build/rk3399/debug/bl31/bl31.elf

  popd
}

build_uefi() {
  echo " => Building UEFI.bin"
  board=$1

  pushd ../

  export GCC5_AARCH64_PREFIX=/usr/bin/aarch64-linux-gnu-
  export PYTOOL_TEMPORARILY_IGNORE_NESTED_EDK_PACKAGES=true
  stuart_setup -c Platforms/Pine64/${board}Pkg/PlatformBuild.py
  stuart_update -c Platforms/Pine64/${board}Pkg/PlatformBuild.py
  stuart_build -c Platforms/Pine64/${board}Pkg/PlatformBuild.py TOOL_CHAIN_TAG=GCC5

  popd
}

build_idblock() {
  echo " => Building idblock.bin"
  FLASHFILES="FlashData.bin FlashBoot.bin"
  rm -f idblock.bin rk3399_ddr_933MHz_*.bin rk3399_usbplug_*.bin ${FLASHFILES}

  # Default DDR image uses 1.5M baud. Patch it to use 115200 to match UEFI first.
  cat $(pwd)/${RKBIN}/tools/ddrbin_param.txt |
    sed 's/^uart baudrate=.*$/uart baudrate=115200/' |
    sed 's/^dis_printf_training=.*$/dis_printf_training=1/' \
      >ddrbin_param.txt
  # generate ddrbin_param_dump.txt
  ${RKBIN}/tools/ddrbin_tool ./ddrbin_param.txt ${RKBIN}/${DDR}
  ${RKBIN}/tools/ddrbin_tool -g ./ddrbin_param_dump.txt ${RKBIN}/${DDR}

  # Create idblock.bin
  (cd ${RKBIN} && ./tools/boot_merger RKBOOT/${MINIALL_INI})
  ./${RKBIN}/tools/boot_merger unpack -i ${RKBIN}/rk3399_loader_*.bin -o .
  cat ${FLASHFILES} >idblock.bin

  pushd ${RKBIN}
  git reset --hard
  popd

  rm ddrbin_param*.txt rk3399_*.bin ${RKBIN}/rk3399_loader*.bin ${FLASHFILES}
}

build_fit() {
  echo " => Building FIT"

  board=$1
  type=$2

  board_upper=$(echo $board | tr '[:lower:]' '[:upper:]')

  # extract regions to relocate from elf, save each as .bin
  ./extractbl31.py ${BL31}
  ./extractbl31.py ${BL32}
  ls -la bl31*.bin

  # create board .its from template
  sed "s,@BOARDTYPE@,${type},g" uefi.its > ${board_upper}_EFI.its

  # create .itb
  ./${RKBIN}/tools/mkimage -f ${board_upper}_EFI.its -E ${board_upper}_EFI.itb

  BL33=../Build/${board}Pkg/${UEFI_BUILD_TYPE}/FV/${board_upper}.fd

  # write UEFI image to SD image
  dd if=${BL33} of=${board_upper}_EFI.itb bs=512 seek=$SD_IMG_OFFSET_UEFI
  rm -f bl31_0x*.bin ${board_upper}_EFI.its
}

make_sdcard() {

  build_atf
  build_uefi $1
  build_fit $1 $2 $3
  build_idblock

  board=$1
  type=$2
  board_upper=$(echo $board | tr '[:lower:]' '[:upper:]')

  rm -f sdcard.img
  fallocate -l 33M sdcard.img
  parted -s sdcard.img mklabel gpt
  parted -s sdcard.img unit s mkpart loader 64 8MiB
  parted -s sdcard.img unit s mkpart uboot 8MiB 16MiB
  parted -s sdcard.img unit s mkpart env 16MiB 32MiB

  cp sdcard.img ${board_upper}_EFI.img
  dd if=idblock.bin of=${board_upper}_EFI.img seek=64 conv=notrunc
  dd if=${board_upper}_EFI.itb of=${board_upper}_EFI.img seek=20480 conv=notrunc

  rm -f sdcard.img idblock.bin ${board_upper}_EFI.itb

  fdisk -l ${board_upper}_EFI.img
}

build_ta_sdk
build_ftpm
build_optee_os
build_atf

#BL31=$(grep '^PATH=.*_bl31_' ${RKBIN}/RKTRUST/${TRUST_INI} | cut -d = -f 2-)
DDR=$(grep '^Path1=.*_ddr_' ${RKBIN}/RKBOOT/${MINIALL_INI} | cut -d = -f 2-)

echo "BL31: ${BL31}"
echo "DDR: ${DDR}"

test -r ${BL31} || (
  echo "${BL31} not found"
  false
)

make_sdcard PinebookPro rk3399-pinebook-pro
make_sdcard PinePhonePro rk3399-pinephone-pro

# set +x
