#
# Copyright (c) 2023, Joel Winarske <joel.winarske@gmail.com>
#
# SPDX-License-Identifier: BSD-2-Clause-Patent
#

#
# Board specific initialization
#
INF Platforms/FriendlyElec/R5CPkg/Drivers/BoardInitDxe/BoardInitDxe.inf

#
# Devicetree support
#
INF Platforms/Rockchip/Rk356x/Drivers/FdtDxe/FdtDxe.inf

#
# ACPI Support
#
INF MdeModulePkg/Universal/Acpi/AcpiTableDxe/AcpiTableDxe.inf
INF MdeModulePkg/Universal/Acpi/BootGraphicsResourceTableDxe/BootGraphicsResourceTableDxe.inf
INF Platforms/Rockchip/Rk356x/Drivers/PlatformAcpiDxe/PlatformAcpiDxe.inf
INF RuleOverride = ACPITABLE Platforms/Rockchip/Rk356x/AcpiTables/$(PLATFORM_NAME).inf

#
# SMBIOS Support
#
INF Platforms/Rockchip/Rk356x/Drivers/PlatformSmbiosDxe/PlatformSmbiosDxe.inf
INF MdeModulePkg/Universal/SmbiosDxe/SmbiosDxe.inf

#
# TianoCore logo (splash screen)
#
INF MdeModulePkg/Logo/LogoDxe.inf
