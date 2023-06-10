# Continuation of [FV.FvMain]
  INF MsGraphicsPkg/GopOverrideDxe/GopOverrideDxe.inf
  INF DfciPkg/AuthManagerNull/AuthManagerNull.inf
  INF DfciPkg/SettingsManager/SettingsManagerDxe.inf
  INF MsGraphicsPkg/MsUiTheme/Dxe/MsUiThemeProtocol.inf
  INF MsGraphicsPkg/RenderingEngineDxe/RenderingEngineDxe.inf
  INF MsGraphicsPkg/DisplayEngineDxe/DisplayEngineDxe.inf
  INF Pine64Pkg/BootMenu/BootMenu.inf
  INF Pine64Pkg/FrontPage/FrontPage.inf
  INF PcBdsPkg/MsBootPolicy/MsBootPolicy.inf
  INF MdeModulePkg/Universal/BootManagerPolicyDxe/BootManagerPolicyDxe.inf
  INF MdeModulePkg/Universal/RegularExpressionDxe/RegularExpressionDxe.inf
  INF DfciPkg/DfciManager/DfciManager.inf
  INF MsGraphicsPkg/OnScreenKeyboardDxe/OnScreenKeyboardDxe.inf
  INF Pine64Pkg/FrontpageButtonsVolumeUp/FrontpageButtonsVolumeUp.inf
  INF MsGraphicsPkg/SimpleWindowManagerDxe/SimpleWindowManagerDxe.inf
  # Change AARCH64 to the appropriate architecture for your platform.
  FILE APPLICATION=PCD(gPcBdsPkgTokenSpaceGuid.PcdShellFile) {
    SECTION PE32=$(OUTPUT_DIRECTORY)/$(TARGET)_$(TOOL_CHAIN_TAG)/AARCH64/Shell.efi
    SECTION UI= "EdkShell"
  }
  FILE FREEFORM = PCD(gPine64PkgTokenSpaceGuid.PcdLogoFile) {
    SECTION RAW = Pine64Pkg/FrontPage/Resources/BootLogo.bmp
    SECTION UI = "Logo"
  }
  FILE FREEFORM = PCD(gPine64PkgTokenSpaceGuid.PcdFrontPageLogoFile) {
    SECTION RAW = Pine64Pkg/FrontPage/Resources/FrontpageLogo.bmp
  }
  FILE FREEFORM = PCD(gPine64PkgTokenSpaceGuid.PcdLowBatteryFile) {
    SECTION RAW = Pine64Pkg/FrontPage/Resources/LBAT.bmp
  }
  FILE FREEFORM = PCD(gPine64PkgTokenSpaceGuid.PcdThermalFile) {
    SECTION RAW = Pine64Pkg/FrontPage/Resources/THOT.bmp
  }