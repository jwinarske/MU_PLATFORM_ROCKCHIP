#!/usr/bin/env bash

set -e

pushd staging/tfa
../../tools/extractelf.py ../../Silicon/Arm/TFA/build/rk3399/debug/bl31/bl31.elf tfa
popd

pushd staging/optee
../../tools/extractelf.py ../../Silicon/OP-TEE/optee_os/out/arm-plat-rockchip/core/tee.elf optee
popd

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
  --load   A00000 Build/PinePhonePro/DEBUG_GCC5/FV/PINEPHONEPRO.fd \
  --load   B00000 dtb/rk3399-pinephone-pro.dtb \
  --load  4000000 IPL/levinboot/_build/dramstage.bin \
  --start 4000000 4102000
