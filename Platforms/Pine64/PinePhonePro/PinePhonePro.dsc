#
#  Copyright (c) 2023, Joel Winarske. All rights reserved.
#  Copyright (c) 2018, Microsoft Corporation. All rights reserved.
#  Copyright (c) 2013-2018, ARM Limited. All rights reserved.
#
#  This program and the accompanying materials
#  are licensed and made available under the terms and conditions of the BSD License
#  which accompanies this distribution.  The full text of the license may be found at
#  http://opensource.org/licenses/bsd-license.php
#
#  THE PROGRAM IS DISTRIBUTED UNDER THE BSD LICENSE ON AN "AS IS" BASIS,
#  WITHOUT WARRANTIES OR REPRESENTATIONS OF ANY KIND, EITHER EXPRESS OR IMPLIED.
#

################################################################################
#
# Defines Section - statements that will be processed to create a Makefile.
#
################################################################################
[Defines]
  DEFINE ROCKCHIP_FAMILY         = RK3399
  DEFINE BOARD_NAME              = PINE_PHONE_PRO

  PLATFORM_NAME                  = PinePhonePro
  PLATFORM_GUID                  = 1076b222-be41-48ff-9438-3669d47aedb7
  PLATFORM_VERSION               = 0.1
  DSC_SPECIFICATION              = 0x00010019
  OUTPUT_DIRECTORY               = Build/$(PLATFORM_NAME)
  SUPPORTED_ARCHITECTURES        = AARCH64
  BUILD_TARGETS                  = DEBUG|RELEASE
  SKUID_IDENTIFIER               = DEFAULT
  FLASH_DEFINITION               = $(PLATFORM_NAME).fdf
  BOARD_DIR                      = Platforms/Pine64/$(PLATFORM_NAME)


  CONFIG_HEADLESS                = FALSE
  CONFIG_FRONTPAGE               = TRUE

  CONFIG_MPCORE                  = TRUE
  CONFIG_USB                     = TRUE
  CONFIG_PCIE                    = TRUE

  CONFIG_OPTEE                   = TRUE
  CONFIG_OPTEE_PROFILE           = TRUE
  CONFIG_AUTH_VAR                = TRUE
  CONFIG_SECURE_BOOT             = TRUE
  CONFIG_MEASURED_BOOT           = FALSE

  CONFIG_DUMP_SYMBOL_INFO        = TRUE

################################################################################
#
# includes Section - statements common to all Rockchip boards
#
################################################################################

# Include common peripherals
!include Rk3399Pkg/Rk3399CommonDsc.inc
!if $(CONFIG_FRONTPAGE) == TRUE
!include FrontpageDsc.inc
!endif

################################################################################
#
# Board specific Section - entries specific to this Platform
#
################################################################################
[LibraryClasses.common]
  ArmPlatformLib|$(BOARD_DIR)/Library/Rk3399BoardLib/Rk3399BoardLib.inf
  
# TODO - added for build break
  RegisterFilterLib|MdePkg/Library/RegisterFilterLibNull/RegisterFilterLibNull.inf
  DxeMemoryProtectionHobLib|MdeModulePkg/Library/MemoryProtectionHobLibNull/DxeMemoryProtectionHobLibNull.inf
  MemoryBinOverrideLib|MdeModulePkg/Library/MemoryBinOverrideLibNull/MemoryBinOverrideLibNull.inf
  VariablePolicyHelperLib|MdeModulePkg/Library/VariablePolicyHelperLib/VariablePolicyHelperLib.inf
  MemoryTypeInformationChangeLib|MdeModulePkg/Library/MemoryTypeInformationChangeLibNull/MemoryTypeInformationChangeLibNull.inf
  VariableFlashInfoLib|MdeModulePkg/Library/BaseVariableFlashInfoLib/BaseVariableFlashInfoLib.inf
  
  MuTelemetryHelperLib|MsWheaPkg/Library/MuTelemetryHelperLib/MuTelemetryHelperLib.inf

  UpdateFacsHardwareSignatureLib|Pine64Pkg/Library/UpdateFacsHardwareSignatureLib/UpdateFacsHardwareSignatureLib.inf
  FrameBufferMemDrawLib|MsGraphicsPkg/Library/FrameBufferMemDrawLib/FrameBufferMemDrawLib.inf
  #FrameBufferMemDrawLib|MsGraphicsPkg/Library/FrameBufferMemDrawLibNull/FrameBufferMemDrawLibNull.inf
  FrameBufferBltLib|MdeModulePkg/Library/FrameBufferBltLib/FrameBufferBltLib.inf

#  MsPlatformDevicesLib|Silicon/Rockchip/Rk3399Pkg/Library/MsPlatformDevicesLib/MsPlatformDevicesLib.inf

  OrderedCollectionLib|MdePkg/Library/BaseOrderedCollectionRedBlackTreeLib/BaseOrderedCollectionRedBlackTreeLib.inf
  MemoryTypeInfoSecVarCheckLib|MdeModulePkg/Library/MemoryTypeInfoSecVarCheckLib/MemoryTypeInfoSecVarCheckLib.inf
  PasswordPolicyLib|Pine64Pkg/Library/PasswordPolicyLibNull/PasswordPolicyLibNull.inf
  SecureBootVariableLib|SecurityPkg/Library/SecureBootVariableLib/SecureBootVariableLib.inf
  MuSecureBootKeySelectorLib|MsCorePkg/Library/MuSecureBootKeySelectorLib/MuSecureBootKeySelectorLib.inf
  SecureBootKeyStoreLib|Pine64Pkg/Library/SecureBootKeyStoreLibOem/SecureBootKeyStoreLibOem.inf
  PlatformPKProtectionLib|SecurityPkg/Library/PlatformPKProtectionLibVarPolicy/PlatformPKProtectionLibVarPolicy.inf
  DfciSettingChangedNotificationLib|DfciPkg/Library/DfciSettingChangedNotificationLib/DfciSettingChangedNotificationLibNull.inf

  # option
  RngLib|MdePkg/Library/BaseRngLib/BaseRngLib.inf

  # DtClient
#  DtPlatformDtbLoaderLib|EmbeddedPkg/Library/DxeDtPlatformDtbLoaderLibDefault/DxeDtPlatformDtbLoaderLibDefault.inf
#  AndroidBootImgLib|EmbeddedPkg/Library/AndroidBootImgLib/AndroidBootImgLib.inf


[LibraryClasses.common.DXE_RUNTIME_DRIVER]
  VariablePolicyLib|MdeModulePkg/Library/VariablePolicyLib/VariablePolicyLibRuntimeDxe.inf

[LibraryClasses.common.PEIM]
  PcdDatabaseLoaderLib|MdeModulePkg/Library/PcdDatabaseLoaderLib/Pei/PcdDatabaseLoaderLibPei.inf  # MU_CHANGE

[LibraryClasses.common.DXE_DRIVER]
  PcdDatabaseLoaderLib|MdeModulePkg/Library/PcdDatabaseLoaderLib/Dxe/PcdDatabaseLoaderLibDxe.inf

[Components.common]
  #
  # ACPI Support
  #
  MdeModulePkg/Universal/Acpi/AcpiTableDxe/AcpiTableDxe.inf
  MdeModulePkg/Universal/Acpi/AcpiPlatformDxe/AcpiPlatformDxe.inf
  $(BOARD_DIR)/AcpiTables/AcpiTables.inf

  #
  # SMBIOS/DMI
  #
  MdeModulePkg/Universal/SmbiosDxe/SmbiosDxe.inf
  $(BOARD_DIR)/Drivers/SmbiosPlatformDxe/SmbiosPlatformDxe.inf

  #
  # Fastboot
  #
  EmbeddedPkg/Application/AndroidFastboot/AndroidFastbootApp.inf

#  EmbeddedPkg/Drivers/FdtClientDxe/FdtClientDxe.inf
#  EmbeddedPkg/Drivers/DtPlatformDxe/DtPlatformDxe.inf
#  EmbeddedPkg/Application/AndroidBoot/AndroidBootApp.inf
#  EmbeddedPkg/Drivers/VirtualKeyboardDxe/VirtualKeyboardDxe.inf

################################################################################
#
# Pcd Section - list of all EDK II PCD Entries defined by this Platform
#
################################################################################

[PcdsFeatureFlag.common]
  ## If TRUE, Graphics Output Protocol will be installed on virtual handle created by ConsplitterDxe.
  #  It could be set FALSE to save size.
  gEfiMdeModulePkgTokenSpaceGuid.PcdConOutGopSupport|TRUE

  gEfiMdeModulePkgTokenSpaceGuid.PcdTurnOffUsbLegacySupport|TRUE

[PcdsFixedAtBuild.common]
  # FirmwareRevision 0.1
  gEfiMdeModulePkgTokenSpaceGuid.PcdFirmwareRevision|0x00000001

  # System memory size (4GB)
  # Limit to 3GB of DRAM at the top of the 32bit address space
!if $(CONFIG_OPTEE) == TRUE
  # OpTEE is loaded at top of memory by Arm-TF. Reduce memory size to avoid collision.
  gArmTokenSpaceGuid.PcdSystemMemorySize|0xBE000000
!else
  gArmTokenSpaceGuid.PcdSystemMemorySize|0xC0000000
!endif

  #
  # NV Storage PCDs. Use base of 0x30370000 for SNVS?
  #
  gEfiMdeModulePkgTokenSpaceGuid.PcdFlashNvStorageVariableBase|0x30370000
  gEfiMdeModulePkgTokenSpaceGuid.PcdFlashNvStorageVariableSize|0x00004000
  gEfiMdeModulePkgTokenSpaceGuid.PcdFlashNvStorageFtwWorkingBase|0x30374000
  gEfiMdeModulePkgTokenSpaceGuid.PcdFlashNvStorageFtwWorkingSize|0x00004000
  gEfiMdeModulePkgTokenSpaceGuid.PcdFlashNvStorageFtwSpareBase|0x30378000
  gEfiMdeModulePkgTokenSpaceGuid.PcdFlashNvStorageFtwSpareSize|0x00004000

  # RK3399
  gArmPlatformTokenSpaceGuid.PcdCoreCount|8
  gArmPlatformTokenSpaceGuid.PcdClusterCount|2

  gArmTokenSpaceGuid.PcdVFPEnabled|1

  #
  # RkPlatformPkg
  #

  ## RkPlatformPkg - Serial Terminal
  gRkPlatformTokenSpaceGuid.PcdSerialRegisterBase|0x30860000

  ## RkPlatformPackage - Debug UART instance UART1 0x30860000
  gRkPlatformTokenSpaceGuid.PcdKdUartInstance|1

  # uSDHCx | RK3399 Pine Phone Pro Connections
  #-------------------------------------
  # uSDHC1 | eMMC
  # uSDHC2 | SD Card slot
  # uSDHC3 | N/A
  # uSDHC4 | N/A
  #
  gRkPlatformTokenSpaceGuid.PcdSdhc1Enable|TRUE
  gRkPlatformTokenSpaceGuid.PcdSdhc2Enable|TRUE
  gRkPlatformTokenSpaceGuid.PcdSdhc3Enable|FALSE
  gRkPlatformTokenSpaceGuid.PcdSdhc4Enable|FALSE

  gRkPlatformTokenSpaceGuid.PcdSdhc1CardDetectSignal|0xFF00
  gRkPlatformTokenSpaceGuid.PcdSdhc1WriteProtectSignal|0xFF01
  gRkPlatformTokenSpaceGuid.PcdSdhc2CardDetectSignal|0xFF00
  gRkPlatformTokenSpaceGuid.PcdSdhc2WriteProtectSignal|0xFF01
  gRkPlatformTokenSpaceGuid.PcdSdhc3CardDetectSignal|0xFF00
  gRkPlatformTokenSpaceGuid.PcdSdhc3WriteProtectSignal|0xFF01
  gRkPlatformTokenSpaceGuid.PcdSdhc4CardDetectSignal|0xFF00
  gRkPlatformTokenSpaceGuid.PcdSdhc4WriteProtectSignal|0xFF01

  ## SBSA Watchdog Count
!ifndef DISABLE_SBSA_WATCHDOG
  gArmPlatformTokenSpaceGuid.PcdWatchdogCount|2
!endif

  #
  # ARM Generic Interrupt Controller
  #
  gArmTokenSpaceGuid.PcdGicDistributorBase|0x38800000
  gArmTokenSpaceGuid.PcdGicRedistributorsBase|0x38880000

!if TRUE == FALSE
  #
  # Watchdog
  #
  gArmTokenSpaceGuid.PcdGenericWatchdogControlBase|0x30280000
#  gArmTokenSpaceGuid.PcdGenericWatchdogRefreshBase| *** unused ***
  gArmTokenSpaceGuid.PcdGenericWatchdogEl2IntrNum|110
!endif

  #
  # ARM Architectural Timer Frequency
  #
  gEmbeddedTokenSpaceGuid.PcdMetronomeTickPeriod|1000

  gEfiMdeModulePkgTokenSpaceGuid.PcdResetOnMemoryTypeInformationChange|FALSE

  #
  # SMBIOS entry point version
  #
  gEfiMdeModulePkgTokenSpaceGuid.PcdSmbiosVersion|0x0300
  gEfiMdeModulePkgTokenSpaceGuid.PcdSmbiosDocRev|0x0

[PcdsPatchableInModule]
  # Use system default resolution
  gEfiMdeModulePkgTokenSpaceGuid.PcdVideoHorizontalResolution|0
  gEfiMdeModulePkgTokenSpaceGuid.PcdVideoVerticalResolution|0
