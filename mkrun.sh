#!/usr/bin/env bash

set -e

# For TA SDK 32 flavor
export PATH=$PATH:$HOME/Downloads/arm-gnu-toolchain-12.2.rel1-x86_64-arm-none-linux-gnueabihf/bin

# For SoC M0
export PATH=$PATH:$HOME/Downloads/arm-gnu-toolchain-12.2.mpacbti-rel1-x86_64-arm-none-eabi/bin

rm -rf staging |true

build_levinboot() {

  pushd IPL/levinboot

  git reset --hard 2fbeba71d46929d5e6980911d482e65ad6fb17f1
  git apply ../0005-Boot-BL31-BL32-BL33-from-RAM.patch

  rm -rf _build |true
  mkdir _build && pushd _build

  CROSS=aarch64-linux-gnu
  CC=$CROSS-gcc OBJCOPY=$CROSS-objcopy LD=$CROSS-ld ../configure.py --with-tf-a-headers ../../../Silicon/Arm/TFA/include/export --boards pbp
  ninja

  mkdir tools && pushd tools
  CC=gcc ../../tools/configure
  ninja
  popd

  popd
  popd
}

pushd tools
./build.sh
popd

pushd staging
mkdir tfa
cd tfa
../../tools/extractelf.py ../../Silicon/Arm/TFA/build/rk3399/debug/bl31/bl31.elf tfa
popd

pushd staging
mkdir optee
cd optee
../../tools/extractelf.py ../../Silicon/OP-TEE/optee_os/out/arm-plat-rockchip/core/tee.elf optee
popd

build_levinboot

sudo IPL/levinboot/_build/tools/usbtool \
  --call IPL/levinboot/_build/sramstage-usb.bin \
  --bulk \
  --load 40000 staging/tfa/tfa_0x00040000.bin \
  --load ff3b0000 staging/tfa/tfa_0xff3b0000.bin \
  --load ff8c0000 staging/tfa/tfa_0xff8c0000.bin \
  --load ff8c1000 staging/tfa/tfa_0xff8c1000.bin \
  --load ff8c2000 staging/tfa/tfa_0xff8c2000.bin \
  --load 30000000 staging/optee/optee_0x30000000.bin \
  --load 30200000 staging/optee/optee_0x30200000.bin \
  --load   A00000 Build/PinePhoneProPkg/DEBUG_GCC5/FV/PINEPHONEPRO.fd \
  --load   B00000 dtb/rk3399-pinephone-pro.dtb \
  --load  4000000 IPL/levinboot/_build/dramstage.bin \
  --start 4000000 4102000
