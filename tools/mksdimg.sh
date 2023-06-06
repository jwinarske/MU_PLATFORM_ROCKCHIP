#!/usr/bin/env bash

# set -x

TRUST_INI=RK3399TRUST.ini
MINIALL_INI=RK3399MINIALL.ini

UEFI_BUILD_TYPE=DEBUG_GCC5

RKBIN=../Silicon/Rockchip/rkbin

build_uefi() (
  echo " => Building UEFI.bin"
  board=$1

  pushd ../

  export GCC5_AARCH64_PREFIX=/usr/bin/aarch64-linux-gnu-
  export PYTOOL_TEMPORARILY_IGNORE_NESTED_EDK_PACKAGES=true
  stuart_setup -c Platforms/Pine64/${board}Pkg/PlatformBuild.py
  stuart_update -c Platforms/Pine64/${board}Pkg/PlatformBuild.py
  stuart_build -c Platforms/Pine64/${board}ProPkg/PlatformBuild.py TOOL_CHAIN_TAG=GCC5

  popd
)

build_idblock() {
  echo " => Building idblock.bin"
  FLASHFILES="FlashData.bin FlashBoot.bin"
  rm -f idblock.bin rk3399_ddr_933MHz_*.bin rk3399_usbplug_*.bin ${FLASHFILES}

  # Default DDR image uses 1.5M baud. Patch it to use 115200 to match UEFI first.
  cat $(pwd)/${RKBIN}/tools/ddrbin_param.txt |
    sed 's/^uart baudrate=.*$/uart baudrate=115200/' |
    sed 's/^dis_printf_training=.*$/dis_printf_training=1/' \
      >ddrbin_param.txt
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
  board=$1
  type=$2
  board_upper=$(echo $board | tr '[:lower:]' '[:upper:]')
  echo " => Building FIT"
  ./extractbl31.py ${RKBIN}/${BL31}
  cat uefi.its | sed "s,@BOARDTYPE@,${type},g" >${board_upper}_EFI.its
  ./${RKBIN}/tools/mkimage -f ${board_upper}_EFI.its -E ${board_upper}_EFI.itb
  dd if=../Build/PinePhoneProPkg/${UEFI_BUILD_TYPE}/FV/PINE_PHONE_PRO_UEFI.fd of=${board_upper}_EFI.itb bs=512 seek=$((1024 * 1024 / 512))
  rm -f bl31_0x*.bin ${board_upper}_EFI.its
}

make_sdcard() {

  build_uefi $1
  build_fit $1 $2
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
}

BL31=$(grep '^PATH=.*_bl31_' ${RKBIN}/RKTRUST/${TRUST_INI} | cut -d = -f 2-)
DDR=$(grep '^Path1=.*_ddr_' ${RKBIN}/RKBOOT/${MINIALL_INI} | cut -d = -f 2-)

echo "BL31: ${BL31}"
echo "DDR: ${DDR}"

test -r ${RKBIN}/${BL31} || (
  echo "${RKBIN}/${BL31} not found"
  false
)

make_sdcard PinebookPro PinebookPro
make_sdcard PinePhonePro PinePhonePro

# set +x
