/** @file
*
*  Copyright (c) 2018, Microsoft Corporation. All rights reserved.
*
*  This program and the accompanying materials
*  are licensed and made available under the terms and conditions of the BSD License
*  which accompanies this distribution.  The full text of the license may be found at
*  http://opensource.org/licenses/bsd-license.php
*
*  THE PROGRAM IS DISTRIBUTED UNDER THE BSD LICENSE ON AN "AS IS" BASIS,
*  WITHOUT WARRANTIES OR REPRESENTATIONS OF ANY KIND, EITHER EXPRESS OR IMPLIED.
*
**/

#include <Library/ArmPlatformLib.h>
#include <Library/DebugLib.h>
#include <Library/HobLib.h>
#include <Library/PcdLib.h>
#include <Library/IoLib.h>
#include <Library/MemoryAllocationLib.h>

#include <Rk3399.h>


#define MEMORY_ATTRIBUTES_PCDCACHEENABLE    -1


ARM_MEMORY_REGION_DESCRIPTOR Rk3399MemoryDescriptor[] =
{
#if 0 //ndef CONFIG_HEADLESS
  // Main memory
  {
    FixedPcdGet64 (PcdSystemMemoryBase) + FixedPcdGet64 (PcdArmLcdDdrFrameBufferSize),
    FixedPcdGet64 (PcdSystemMemoryBase) + FixedPcdGet64 (PcdArmLcdDdrFrameBufferSize),
    FixedPcdGet64 (PcdSystemMemorySize) - FixedPcdGet64 (PcdArmLcdDdrFrameBufferSize),
    MEMORY_ATTRIBUTES_PCDCACHEENABLE,
  },
  // Frame buffer
  {
    FixedPcdGet64 (PcdArmLcdDdrFrameBufferBase),
    FixedPcdGet64 (PcdArmLcdDdrFrameBufferBase),
    FixedPcdGet64 (PcdArmLcdDdrFrameBufferSize),
    ARM_MEMORY_REGION_ATTRIBUTE_UNCACHED_UNBUFFERED,
  },
#endif
  // Main memory
  {
    FixedPcdGet64 (PcdSystemMemoryBase),
    FixedPcdGet64 (PcdSystemMemoryBase),
    FixedPcdGet64 (PcdSystemMemorySize),
    MEMORY_ATTRIBUTES_PCDCACHEENABLE,
  },
//#endif
#ifdef CONFIG_OPTEE
  {
    FixedPcdGet64 (PcdTrustZoneSharedMemoryBase),
    FixedPcdGet64 (PcdTrustZoneSharedMemoryBase),
    FixedPcdGet64 (PcdTrustZoneSharedMemorySize),
	MEMORY_ATTRIBUTES_PCDCACHEENABLE,
  },
#endif
  {
    GIC500_BASE,
    GIC500_BASE,
    GIC500_SIZE,
    ARM_MEMORY_REGION_ATTRIBUTE_DEVICE,
  },
  {
    UART2_BASE,
    UART2_BASE,
    UART2_SIZE,
    ARM_MEMORY_REGION_ATTRIBUTE_DEVICE,
  },
  {
    GRF_BASE,
    GRF_BASE,
    GRF_SIZE,
    ARM_MEMORY_REGION_ATTRIBUTE_DEVICE,
  },
  {
    CRU_BASE,
    CRU_BASE,
    CRU_SIZE,
    ARM_MEMORY_REGION_ATTRIBUTE_DEVICE,
  },
  {
    PMUCRU_BASE,
    PMUCRU_BASE,
    PMUCRU_SIZE,
    ARM_MEMORY_REGION_ATTRIBUTE_DEVICE,
  },
  {
    PMUGRF_BASE,
    PMUGRF_BASE,
    PMUGRF_SIZE,
    ARM_MEMORY_REGION_ATTRIBUTE_DEVICE,
  },
  {
    GPIO0_BASE,
    GPIO0_BASE,
    GPIO0_SIZE,
    ARM_MEMORY_REGION_ATTRIBUTE_DEVICE,
  },
  {
    GPIO1_BASE,
    GPIO1_BASE,
    GPIO1_SIZE,
    ARM_MEMORY_REGION_ATTRIBUTE_DEVICE,
  },
  {
    GPIO2_BASE,
    GPIO2_BASE,
    GPIO2_SIZE,
    ARM_MEMORY_REGION_ATTRIBUTE_DEVICE,
  },
  {
    GPIO3_BASE,
    GPIO3_BASE,
    GPIO3_SIZE,
    ARM_MEMORY_REGION_ATTRIBUTE_DEVICE,
  },
  {
    GPIO4_BASE,
    GPIO4_BASE,
    GPIO4_SIZE,
    ARM_MEMORY_REGION_ATTRIBUTE_DEVICE,
  },
  {
    PWM_BASE,
    PWM_BASE,
    PWM_SIZE,
    ARM_MEMORY_REGION_ATTRIBUTE_DEVICE
  },
  {
    //
    // End of table
    //
    0,
    0,
    0,
    0,
  },
};

#define MAX_VIRTUAL_MEMORY_MAP_DESCRIPTORS (sizeof(Rk3399MemoryDescriptor) / sizeof(Rk3399MemoryDescriptor[0]))

// DDR attributes
#define DDR_ATTRIBUTES_CACHED           ARM_MEMORY_REGION_ATTRIBUTE_WRITE_BACK
#define DDR_ATTRIBUTES_UNCACHED         ARM_MEMORY_REGION_ATTRIBUTE_UNCACHED_UNBUFFERED

/**
  Return the Virtual Memory Map of your platform

  This Virtual Memory Map is used by MemoryInitPei Module to initialize the MMU on your platform.

  @param[out]   VirtualMemoryMap    Array of ARM_MEMORY_REGION_DESCRIPTOR describing a Physical-to-
                                    Virtual Memory mapping. This array must be ended by a zero-filled
                                    entry

**/
VOID
ArmPlatformGetVirtualMemoryMap (
  IN ARM_MEMORY_REGION_DESCRIPTOR **VirtualMemoryMap
  )
{
  ARM_MEMORY_REGION_ATTRIBUTES cacheAttributes;
  UINTN index;
  ARM_MEMORY_REGION_DESCRIPTOR *virtualMemoryTable;
  EFI_RESOURCE_ATTRIBUTE_TYPE  ResourceAttributes;

  ASSERT (VirtualMemoryMap != NULL);

  DEBUG ((EFI_D_VERBOSE, "Enter: ArmPlatformGetVirtualMemoryMap\n"));

  ResourceAttributes = (
                       EFI_RESOURCE_ATTRIBUTE_PRESENT |
                       EFI_RESOURCE_ATTRIBUTE_INITIALIZED |
                       EFI_RESOURCE_ATTRIBUTE_WRITE_COMBINEABLE |
                       EFI_RESOURCE_ATTRIBUTE_WRITE_THROUGH_CACHEABLE |
                       EFI_RESOURCE_ATTRIBUTE_WRITE_BACK_CACHEABLE |
                       EFI_RESOURCE_ATTRIBUTE_TESTED
                       );

  virtualMemoryTable = (ARM_MEMORY_REGION_DESCRIPTOR *)AllocatePages (EFI_SIZE_TO_PAGES (sizeof(ARM_MEMORY_REGION_DESCRIPTOR) * MAX_VIRTUAL_MEMORY_MAP_DESCRIPTORS));
  if (virtualMemoryTable == NULL) {
    return;
  }

  cacheAttributes = ARM_MEMORY_REGION_ATTRIBUTE_WRITE_BACK;

  DEBUG ((EFI_D_VERBOSE, "cacheAttributes=0x%d\n", cacheAttributes));

  for (index = 0; index < MAX_VIRTUAL_MEMORY_MAP_DESCRIPTORS; index++) {

    virtualMemoryTable[index].PhysicalBase = Rk3399MemoryDescriptor[index].PhysicalBase;
    virtualMemoryTable[index].VirtualBase = Rk3399MemoryDescriptor[index].VirtualBase;
    virtualMemoryTable[index].Length = Rk3399MemoryDescriptor[index].Length;

    if (Rk3399MemoryDescriptor[index].Attributes == MEMORY_ATTRIBUTES_PCDCACHEENABLE) {
      virtualMemoryTable[index].Attributes = cacheAttributes;
    } else {
      virtualMemoryTable[index].Attributes = Rk3399MemoryDescriptor[index].Attributes;
    }
  }

  ASSERT ((index) <= MAX_VIRTUAL_MEMORY_MAP_DESCRIPTORS);

#ifndef CONFIG_HEADLESS
  // Reserve frame buffer
  BuildResourceDescriptorHob (
    EFI_RESOURCE_MEMORY_RESERVED,
    EFI_RESOURCE_ATTRIBUTE_PRESENT |
      EFI_RESOURCE_ATTRIBUTE_INITIALIZED |
      EFI_RESOURCE_ATTRIBUTE_TESTED,
    FixedPcdGet64 (PcdArmLcdDdrFrameBufferBase),
    FixedPcdGet64 (PcdArmLcdDdrFrameBufferSize));
#endif

#ifdef CONFIG_OPTEE
  // Reserve OP-TEE private memory
  BuildResourceDescriptorHob (
    EFI_RESOURCE_MEMORY_RESERVED,
    EFI_RESOURCE_ATTRIBUTE_PRESENT |
    EFI_RESOURCE_ATTRIBUTE_INITIALIZED |
    EFI_RESOURCE_ATTRIBUTE_TESTED,
    FixedPcdGet32 (PcdTrustZonePrivateMemoryBase),
    FixedPcdGet32 (PcdTrustZonePrivateMemorySize)
    );
#endif

  // Reserve TPM2 Control Area
  BuildResourceDescriptorHob (
    EFI_RESOURCE_MEMORY_RESERVED,
    EFI_RESOURCE_ATTRIBUTE_PRESENT |
    EFI_RESOURCE_ATTRIBUTE_INITIALIZED |
    EFI_RESOURCE_ATTRIBUTE_TESTED,
    FixedPcdGet32 (PcdTpm2AcpiBufferBase),
    FixedPcdGet32 (PcdTpm2AcpiBufferSize)
    );

  // Reserve Global Data area
  BuildResourceDescriptorHob (
    EFI_RESOURCE_MEMORY_RESERVED,
    EFI_RESOURCE_ATTRIBUTE_PRESENT |
      EFI_RESOURCE_ATTRIBUTE_INITIALIZED |
      EFI_RESOURCE_ATTRIBUTE_TESTED,
    FixedPcdGet32 (PcdGlobalDataBaseAddress),
    FixedPcdGet32 (PcdGlobalDataSize)
    );

  // Reserve TFA memory
  BuildResourceDescriptorHob (
    EFI_RESOURCE_MEMORY_RESERVED,
    EFI_RESOURCE_ATTRIBUTE_PRESENT |
    EFI_RESOURCE_ATTRIBUTE_INITIALIZED |
    EFI_RESOURCE_ATTRIBUTE_TESTED,
    0x40000,
    0x400000
    );

  *VirtualMemoryMap = virtualMemoryTable;
}
