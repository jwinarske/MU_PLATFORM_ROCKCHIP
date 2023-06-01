/** @file
 *
 *  Misc SoC helpers for RK356x
 *
 *  Copyright (c) 2022, Jared McNeill <jmcneill@invisible.ca>
 *
 *  SPDX-License-Identifier: BSD-2-Clause-Patent
 *
 **/

#include <Library/IoLib.h>
#include <Library/DebugLib.h>
#include <Library/SocLib.h>
#include <Library/BaseLib.h>

#include <IndustryStandard/Rk356x.h>

// The RK356x boot ROM stores the boot device in SRAM at this offset.
#define BOOT_DEVICE_REG                 (SYSTEM_SRAM + 0x10)


// PMU_GRF registers
#define PMU_GRF_IO_VSEL0                      (PMU_GRF + 0x0140)
#define PMU_GRF_IO_VSEL1                      (PMU_GRF + 0x0144)
#define PMU_GRF_IO_VSEL2                      (PMU_GRF + 0x0148)

SOC_BOOT_DEVICE
SocGetBootDevice (
  VOID
  )
{
  UINT32 Value;
  
  Value = MmioRead32 (BOOT_DEVICE_REG);

  switch (Value) {
  case SOC_BOOT_DEVICE_NAND:
  case SOC_BOOT_DEVICE_EMMC:
  case SOC_BOOT_DEVICE_SPINOR:
  case SOC_BOOT_DEVICE_SPINAND:
  case SOC_BOOT_DEVICE_SD:
  case SOC_BOOT_DEVICE_USB:
    return Value;

  default:
    ASSERT (FALSE);
    return SOC_BOOT_DEVICE_UNKNOWN;
  }
}

VOID
SocSetDomainVoltage (
  PMU_IO_DOMAIN IoDomain,
  PMU_IO_VOLTAGE IoVoltage
  )
{
  UINT32 Mask;

  switch (IoDomain) {
  case PMUIO2:
    Mask = BIT1 | BIT5;
    if (IoVoltage == VCC_3V3) {
      MmioWrite32 (PMU_GRF_IO_VSEL2, (Mask << 16) | BIT5);
    } else {
      MmioWrite32 (PMU_GRF_IO_VSEL2, (Mask << 16) | BIT1);
    }
    break;

  case VCCIO1...VCCIO7:
    Mask = 1U << IoDomain;
    if (IoVoltage == VCC_3V3) {
      MmioWrite32 (PMU_GRF_IO_VSEL0, Mask << 16);
      MmioWrite32 (PMU_GRF_IO_VSEL1, (Mask << 16) | Mask);
    } else {
      MmioWrite32 (PMU_GRF_IO_VSEL0, (Mask << 16) | Mask);
      MmioWrite32 (PMU_GRF_IO_VSEL1, Mask << 16);
    }
    break;
  }
}

VOID
PrintPmuGrfIoVsel (
    VOID
    )
{
  UINT32 value = MmioRead32 (0xFDC20140);
  DEBUG((DEBUG_INFO,"PMU_GRF_IO_VSEL0: %08X\n", value));
  DEBUG((DEBUG_INFO,"VCCIO2 voltage control selection: %a\n", ((value & BIT0) == BIT0) ? "from GRF" : "from GPIO0_A7"));
  DEBUG((DEBUG_INFO,"VCCIO1 1.8V control: %a\n", ((value & BIT1) == BIT1) ? "Enable" : "Disable"));
  DEBUG((DEBUG_INFO,"VCCIO2 1.8V control: %a\n", ((value & BIT2) == BIT2) ? "Enable" : "Disable"));
  DEBUG((DEBUG_INFO,"VCCIO3 1.8V control: %a\n", ((value & BIT3) == BIT3) ? "Enable" : "Disable"));
  DEBUG((DEBUG_INFO,"VCCIO4 1.8V control: %a\n", ((value & BIT4) == BIT4) ? "Enable" : "Disable"));
  DEBUG((DEBUG_INFO,"VCCIO5 1.8V control: %a\n", ((value & BIT5) == BIT5) ? "Enable" : "Disable"));
  DEBUG((DEBUG_INFO,"VCCIO6 1.8V control: %a\n", ((value & BIT6) == BIT6) ? "Enable" : "Disable"));
  DEBUG((DEBUG_INFO,"VCCIO7 1.8V control: %a\n", ((value & BIT7) == BIT7) ? "Enable" : "Disable"));
  DEBUG((DEBUG_INFO,"VCCIO1 2.5V control: %a\n", ((value & BIT8) == BIT8) ? "Enable" : "Disable"));
  DEBUG((DEBUG_INFO,"VCCIO2 2.5V control: %a\n", ((value & BIT9) == BIT9) ? "Enable" : "Disable"));
  DEBUG((DEBUG_INFO,"VCCIO3 2.5V control: %a\n", ((value & BIT10) == BIT10) ? "Enable" : "Disable"));
  DEBUG((DEBUG_INFO,"VCCIO4 2.5V control: %a\n", ((value & BIT11) == BIT11) ? "Enable" : "Disable"));
  DEBUG((DEBUG_INFO,"VCCIO5 2.5V control: %a\n", ((value & BIT12) == BIT12) ? "Enable" : "Disable"));
  DEBUG((DEBUG_INFO,"VCCIO6 2.5V control: %a\n", ((value & BIT13) == BIT13) ? "Enable" : "Disable"));
  DEBUG((DEBUG_INFO,"VCCIO7 2.5V control: %a\n", ((value & BIT14) == BIT14) ? "Enable" : "Disable"));

  value = MmioRead32 (0xFDC20144);
  DEBUG((DEBUG_INFO,"PMU_GRF_IO_VSEL1: %08X\n", value));
  DEBUG((DEBUG_INFO,"VCCIO1 3.3V control: %a\n", ((value & BIT0) == BIT0) ? "Enable" : "Disable"));
  DEBUG((DEBUG_INFO,"VCCIO2 3.3V control: %a\n", ((value & BIT2) == BIT1) ? "Enable" : "Disable"));
  DEBUG((DEBUG_INFO,"VCCIO3 3.3V control: %a\n", ((value & BIT2) == BIT2) ? "Enable" : "Disable"));
  DEBUG((DEBUG_INFO,"VCCIO4 3.3V control: %a\n", ((value & BIT3) == BIT3) ? "Enable" : "Disable"));
  DEBUG((DEBUG_INFO,"VCCIO5 3.3V control: %a\n", ((value & BIT4) == BIT4) ? "Enable" : "Disable"));
  DEBUG((DEBUG_INFO,"VCCIO6 3.3V control: %a\n", ((value & BIT5) == BIT5) ? "Enable" : "Disable"));
  DEBUG((DEBUG_INFO,"VCCIO7 3.3V control: %a\n", ((value & BIT6) == BIT6) ? "Enable" : "Disable"));
  DEBUG((DEBUG_INFO,"VCCIO1 iddq control: %a\n", ((value & BIT7) == BIT7) ? "Enable" : "Disable"));
  DEBUG((DEBUG_INFO,"VCCIO2 iddq control: %a\n", ((value & BIT8) == BIT8) ? "Enable" : "Disable"));
  DEBUG((DEBUG_INFO,"VCCIO3 iddq control: %a\n", ((value & BIT9) == BIT9) ? "Enable" : "Disable"));
  DEBUG((DEBUG_INFO,"VCCIO4 iddq control: %a\n", ((value & BIT10) == BIT10) ? "Enable" : "Disable"));
  DEBUG((DEBUG_INFO,"VCCIO5 iddq control: %a\n", ((value & BIT11) == BIT11) ? "Enable" : "Disable"));
  DEBUG((DEBUG_INFO,"VCCIO6 iddq control: %a\n", ((value & BIT12) == BIT12) ? "Enable" : "Disable"));
  DEBUG((DEBUG_INFO,"VCCIO7 iddq control: %a\n", ((value & BIT13) == BIT13) ? "Enable" : "Disable"));

  value = MmioRead32 (0xFDC20148);
  DEBUG((DEBUG_INFO,"PMU_GRF_IO_VSEL2: %08X\n", value));
  DEBUG((DEBUG_INFO,"PMUIO2 1.8V control: %a\n", ((value & BIT1) == BIT1) ? "Enable" : "Disable"));
  DEBUG((DEBUG_INFO,"PMUIO2 2.5V control: %a\n", ((value & BIT3) == BIT3) ? "Enable" : "Disable"));
  DEBUG((DEBUG_INFO,"PMUIO2 3.3V control: %a\n", ((value & BIT5) == BIT5) ? "Enable" : "Disable"));
  DEBUG((DEBUG_INFO,"PMUIO1 iddq control: %a\n", ((value & BIT6) == BIT6) ? "Enable" : "Disable"));
  DEBUG((DEBUG_INFO,"PMUIO2 iddq control: %a\n", ((value & BIT7) == BIT7) ? "Enable" : "Disable"));
}
