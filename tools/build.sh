#!/usr/bin/env bash

set -e

# set to 1 to enable verbose build output
VERBOSE=0

# For TA SDK 32 flavor
CROSS_COMPILE_32_HFP=$HOME/arm-gnu-toolchain-12.2.rel1-x86_64-arm-none-linux-gnueabihf/bin

# For SoC Cortex-M0
CROSS_COMPILE_32=$HOME/arm-gnu-toolchain-12.2.mpacbti-rel1-x86_64-arm-none-eabi/bin

# UEFI build type: RELEASE_GCC5 or DEBUG_GCC5
UEFI_BUILD_TYPE=DEBUG_GCC5

CWD=`pwd`

# OP-TEE repos
OPTEE_OS_DIR=$CWD/../Silicon/OP-TEE/optee_os
OPTEE_EXAMPLES_DIR=$CWD/../Silicon/OP-TEE/optee_examples

# UEFI Volume Info tool
UEFI_TOOLS_DIR=$CWD/../MU_BASECORE/BaseTools/Bin/Mu-Basetools_extdep/Linux-x86

BL31=$CWD/../Silicon/Arm/TFA/build/rk3399/debug/bl31/bl31.elf
BL32=$OPTEE_OS_DIR/out/arm-plat-rockchip/core/tee.elf

RKBIN=$CWD/../Silicon/Rockchip/rkbin

TA_STAGING_DIR=$CWD/../staging/ta

TRUST_INI=RK3399TRUST.ini
MINIALL_INI=RK3399MINIALL.ini

# SD Image Layout
SD_BLOCK_SIZE=512

# UEFI
SD_IMG_OFFSET_UEFI=$((1024 * 1024 / $SD_BLOCK_SIZE)) # 0x100000


build_ta_sdk() {
  echo " => Building Trusted App SDK"

  pushd $OPTEE_OS_DIR
  git reset --hard e8abbcfbdf63437a640d5fd87b7e191caab6445e
  rm -rf out |true

  PATH=$PATH:${CROSS_COMPILE_32_HFP} \
  make \
  CROSS_COMPILE=arm-none-linux-gnueabihf- \
  PLATFORM=rockchip \
  PLATFORM_FLAVOR=rk3399 \
  CFG_DT=n \
  CFG_EXTERNAL_DTB_OVERLAY=n \
  CFG_ARM64_core=y \
  CFG_RPMB_FS=y \
  CFG_RPMB_FS_DEV_ID=0 \
  CFG_CORE_HEAP_SIZE=524288 \
  CFG_RPMB_WRITE_KEY=y \
  CFG_CORE_DYN_SHM=y \
  CFG_RPMB_TESTKEY=y \
  CFG_REE_FS=n \
  CFG_CORE_ARM64_PA_BITS=48 \
  CFG_TEE_CORE_LOG_LEVEL=4 \
  CFG_TEE_TA_LOG_LEVEL=4 \
  CFG_SCTLR_ALIGNMENT_CHECK=n \
  CFG_TEE_BENCHMARK=n \
  CFG_ULIBS_SHARED=y \
  NOWERROR=1 \
  OPTEE_CLIENT_EXPORT=$CWD/out/usr \
  TEEC_EXPORT=$CWD/out/usr \
  all V=$VERBOSE -j

  popd
}

compile_optee_example() {
  rm -rf dyn_list |true 
  make \
  CROSS_COMPILE=aarch64-linux-gnu- \
  PLATFORM=rockchip-rk3399 \
  TA_DEV_KIT_DIR=$CWD/../../../optee_os/out/arm-plat-rockchip/export-ta_arm64 \
  V=$VERBOSE -j

  mkdir -p ${TA_STAGING_DIR}
  cp *.stripped.elf ${TA_STAGING_DIR}
  rm -rf dyn_list
}

build_optee_examples() {
  echo " => Building optee_examples (TA)"

  pushd $OPTEE_EXAMPLES_DIR
  git reset --hard

  pushd acipher/ta
  compile_optee_example
  popd

  pushd aes/ta
  compile_optee_example
  popd

  pushd hello_world/ta
  compile_optee_example
  popd

  pushd secure_storage/ta
  compile_optee_example
  popd

  pushd hotp/ta
  compile_optee_example
  popd

  pushd plugins/ta
  compile_optee_example
  popd

  pushd random/ta
  compile_optee_example
  popd

  pushd secure_storage/ta
  compile_optee_example
  popd

  find -iname *.stripped.elf

  popd
}

build_ftpm() {
  echo " => Building fTPM (TA)"

  pushd ../Silicon/MSFT/ms-tpm-20-ref/

  git submodule update --init --recursive
  git reset --hard
  git apply ../../../SecureBoot/patches/fTPM/0001-add-enum-to-ta-flags.patch

  pushd Samples/ARM32-FirmwareTPM/optee_ta

  rm -rf out |true
  
  make \
  TA_CROSS_COMPILE=aarch64-linux-gnu- \
  TA_CPU=cortex-a53 \
  CFG_FTPM_USE_WOLF=y \
  CFG_ARM64_ta_arm64=y \
  TA_DEV_KIT_DIR=$OPTEE_OS_DIR/out/arm-plat-rockchip/export-ta_arm64 \
  OPTEE_CLIENT_EXPORT=$OPTEE_OS_DIR/out/usr/ \
  TEEC_EXPORT=$OPTEE_OS_DIR/out/usr/ \
  -I$OPTEE_OS_DIR \
  all -j

  elf=`find -iname bc50d971-d4c9-42c4-82cb-343fb7f37896.stripped.elf`
  cp $elf ${TA_STAGING_DIR}
  echo "** fTPM - MSFT **"
  readelf -h $elf

  popd
  popd
}

build_optee_os() {
  echo " => Building tee-pager_v2.bin"

  pushd $OPTEE_OS_DIR

  PATH=$PATH:${CROSS_COMPILE_32_HFP} \
  make \
  CROSS_COMPILE=arm-none-linux-gnueabihf- \
  PLATFORM=rockchip \
  PLATFORM_FLAVOR=rk3399 \
  CFG_DT=n \
  CFG_EXTERNAL_DTB_OVERLAY=n \
  CFG_ARM64_core=y \
  CFG_CORE_HEAP_SIZE=524288 \
  CFG_CORE_DYN_SHM=y \
  CFG_CORE_ARM64_PA_BITS=48 \
  CFG_SCTLR_ALIGNMENT_CHECK=n \
  CFG_RPMB_FS=y \
  CFG_RPMB_FS_DEV_ID=0 \
  CFG_RPMB_WRITE_KEY=y \
  CFG_RPMB_TESTKEY=y \
  CFG_REE_FS=n \
  CFG_TEE_CORE_LOG_LEVEL=4 \
  CFG_TEE_TA_LOG_LEVEL=4 \
  CFG_TA_MEASURED_BOOT=y \
  CFG_TA_EVENT_LOG_SIZE=2048 \
  CFG_TA_DEBUG=1 \
  CFG_TEE_BENCHMARK=n \
  CFG_ULIBS_SHARED=y \
  NOWERROR=1 \
  OPTEE_CLIENT_EXPORT=out/usr \
  TEEC_EXPORT=out/usr \
  EARLY_TA_PATHS="${TA_STAGING_DIR}/bc50d971-d4c9-42c4-82cb-343fb7f37896.stripped.elf \
                  out/arm-plat-rockchip/ta/avb/023f8f1a-292a-432b-8fc4-de8471358067.stripped.elf" \
  all mem_usage V=$VERBOSE -j

  elf=`find -iname tee.elf`
  echo "** TFA - ${elf} **"
  readelf -h $elf

  popd
}

build_atf() {
  echo " => Building bl31.elf"

  pushd ../Silicon/Arm/TFA
  git reset --hard d3e71ead6ea5bc3555ac90a446efec84ef6c6122
  rm -rf plat/rockchip/rk3399/include/shared/bl32_param.h |true
  git apply ../../../SecureBoot/patches/0001-USB-load-to-RAM-config.patch

  rm -rf build |true

  PATH=$PATH:${CROSS_COMPILE_32} \
  make \
  CROSS_COMPILE=aarch64-linux-gnu- \
  PLAT=rk3399 \
  SPD=opteed \
  ERRATA_A53_1530924=1 \
  MEASURED_BOOT=1 \
  BL32=$OPTEE_OS_DIR/out/arm-plat-rockchip/core/tee.elf \
  DEBUG=1 \
  all V=$VERBOSE -j

  elf=`find -iname bl31.elf`
  echo "** TFA ${elf} **"
  readelf -h $elf
 
  popd
}

build_uefi() {
  echo " => Building UEFI .fd"
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
  cat ${RKBIN}/tools/ddrbin_param.txt |
    sed 's/^uart baudrate=.*$/uart baudrate=115200/' |
    sed 's/^dis_printf_training=.*$/dis_printf_training=1/' \
      > ddrbin_param.txt

  # generate ddrbin_param_dump.txt
  ${RKBIN}/tools/ddrbin_tool ./ddrbin_param.txt ${RKBIN}/${DDR}
 
  # dump
  ${RKBIN}/tools/ddrbin_tool -g ./ddrbin_param_dump.txt ${RKBIN}/${DDR}

  # Create idblock.bin
  (cd ${RKBIN} && tools/boot_merger RKBOOT/${MINIALL_INI})
  ${RKBIN}/tools/boot_merger unpack -i ${RKBIN}/rk3399_loader_*.bin -o .
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

  BL33=../Build/${board}Pkg/${UEFI_BUILD_TYPE}/FV/${board_upper}.fd

  # extract PT_LOAD regions to individual files
  ./extractelf.py ${BL31} atf
  ./extractelf.py ${BL32} tee
  ls -la *_0x*.bin

  # UEFI fd volume info
  PATH=$PATH:${UEFI_TOOLS_DIR} ${UEFI_TOOLS_DIR}/VolInfo ${BL33}

  # create .its from template
  sed "s,@BOARDTYPE@,${type},g" image-sd.its > ${board_upper}_EFI.its

  # create image
  ${RKBIN}/tools/mkimage -f ${board_upper}_EFI.its -E ${board_upper}_EFI.itb

  # add UEFI to image
  dd if=${BL33} of=${board_upper}_EFI.itb bs=512 seek=$SD_IMG_OFFSET_UEFI

  rm -f *_0x*.bin ${board_upper}_EFI.its
}

make_image_sd() {

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
build_optee_examples
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

make_image_sd PinebookPro rk3399-pinebook-pro
make_image_sd PinePhonePro rk3399-pinephone-pro
