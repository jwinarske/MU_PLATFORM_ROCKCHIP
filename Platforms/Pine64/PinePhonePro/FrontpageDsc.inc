
[LibraryClasses.common]
  #
  # Libraries to support Shell
  #
  ShellLib|ShellPkg/Library/UefiShellLib/UefiShellLib.inf
  ShellCommandLib|ShellPkg/Library/UefiShellCommandLib/UefiShellCommandLib.inf
  ShellCEntryLib|ShellPkg/Library/UefiShellCEntryLib/UefiShellCEntryLib.inf
  HandleParsingLib|ShellPkg/Library/UefiHandleParsingLib/UefiHandleParsingLib.inf
  BcfgCommandLib|ShellPkg/Library/UefiShellBcfgCommandLib/UefiShellBcfgCommandLib.inf
  NetLib|NetworkPkg/Library/DxeNetLib/DxeNetLib.inf
  #
  # Contains intrinsics for crypto operations
  #
  IntrinsicLib|CryptoPkg/Library/IntrinsicLib/IntrinsicLib.inf
  #
  # contains NULL routines to support UI operations associated with DFCI.
  #
  DfciUiSupportLib|DfciPkg/Library/DfciUiSupportLibNull/DfciUiSupportLibNull.inf
  #
  # contains crypto support functions for the DFCI recovery feature.
  #
  DfciRecoveryLib|DfciPkg/Library/DfciRecoveryLib/DfciRecoveryLib.inf
  #
  # Contains openSSL library used by BaseCryptoLib
  #
  OpensslLib|CryptoPkg/Library/OpensslLib/OpensslLib.inf
  #
  # Stubs functions that should contain base logic for querying, setting, and verifying user passwords.
  #
  BaseCryptLib|CryptoPkg/Library/BaseCryptLib/BaseCryptLib.inf
  #
  # Supports DFCI Groups.
  #
  DfciGroupLib|DfciPkg/Library/DfciGroupLibNull/DfciGroups.inf
  XmlTreeQueryLib|XmlSupportPkg/Library/XmlTreeQueryLib/XmlTreeQueryLib.inf
  XmlTreeLib|XmlSupportPkg/Library/XmlTreeLib/XmlTreeLib.inf
  #
  # Backwards compatibility with DFCI V1 functions.
  #
  DfciV1SupportLib|DfciPkg/Library/DfciV1SupportLibNull/DfciV1SupportLibNull.inf
  FltUsedLib|MdePkg/Library/FltUsedLib/FltUsedLib.inf
  #
  # Library instance that understands the MsXml Settings Schema and providers helper functions
  #
  DfciXmlSettingSchemaSupportLib|DfciPkg/Library/DfciXmlSettingSchemaSupportLib/DfciXmlSettingSchemaSupportLib.inf

  #
  # Library instance that understands the MsXml Permission Schema and providers helper functions
  #
  DfciXmlPermissionSchemaSupportLib|DfciPkg/Library/DfciXmlPermissionSchemaSupportLib/DfciXmlPermissionSchemaSupportLib.inf

  #
  # Library instance that understands the MsXml Device Id Schema and providers helper functions
  #
  DfciXmlDeviceIdSchemaSupportLib|DfciPkg/Library/DfciXmlDeviceIdSchemaSupportLib/DfciXmlDeviceIdSchemaSupportLib.inf

  #
  # Library instance that understands the MsXml Device Id Schema and providers helper functions
  #
  DfciXmlIdentitySchemaSupportLib|DfciPkg/Library/DfciXmlIdentitySchemaSupportLib/DfciXmlIdentitySchemaSupportLib.inf

  #
  # Library instance that understands Zero Touch
  #
  ZeroTouchSettingsLib|ZeroTouchPkg/Library/ZeroTouchSettings/ZeroTouchSettings.inf
  SafeIntLib|MdePkg/Library/BaseSafeIntLib/BaseSafeIntLib.inf
  BmpSupportLib|MdeModulePkg/Library/BaseBmpSupportLib/BaseBmpSupportLib.inf
  #
  # Null library for displaying device state
  #
  DisplayDeviceStateLib|MsGraphicsPkg/Library/DisplayDeviceStateLibNull/DisplayDeviceStateLibNull.inf
  #
  # This library is for showing the main system boot graphics.
  #
  BootGraphicsLib|MsGraphicsPkg/Library/BootGraphicsLib/BootGraphicsLib.inf
  #
  # BootGraphicsProviderLib that uses PCDs and raw files in the firmware volumes
  #
  BootGraphicsProviderLib|Pine64Pkg/Library/BootGraphicsProviderLib/BootGraphicsProviderLib.inf
  #
  # This library contains access functions for dealing with the copying of an MS_UI_THEME
  #
  MsUiThemeCopyLib|MsGraphicsPkg/Library/MsUiThemeCopyLib/MsUiThemeCopyLib.inf
  #
  # Library to provide platform version information
  #
  MuUefiVersionLib|Pine64Pkg/Library/MuUefiVersionLib/MuUefiVersionLib.inf
  #
  # This is the Thermal Services Library that encapsulates the platform specific thermal information.
  #
  ThermalServicesLib|PcBdsPkg/Library/ThermalServicesLibNull/ThermalServicesLibNull.inf
  #
  # Null Power Services Library class to support Platforms that dont have battery
  #
  PowerServicesLib|PcBdsPkg/Library/PowerServicesLibNull/PowerServicesLibNull.inf
  #
  # Empty Platform Library of power check.
  #
  MsPlatformPowerCheckLib|PcBdsPkg/Library/MsPlatformPowerCheckLibNull/MsPlatformPowerCheckLibNull.inf
  #
  # Empty Console Message Library
  #
  ConsoleMsgLib|PcBdsPkg/Library/ConsoleMsgLibNull/ConsoleMsgLibNull.inf
  #
  # Library to help set the console to known points (low res, native res)
  #
  GraphicsConsoleHelperLib|PcBdsPkg/Library/GraphicsConsoleHelperLib/GraphicsConsoleHelper.inf
  DeviceStateLib|MdeModulePkg/Library/DeviceStateLib/DeviceStateLib.inf
  #
  # Device specific actions in support of PlatformBootManagerLib.
  # Sort of a wrapper for MsPlatformDevicesLib, which needs to be written for each platform.
  #
  DeviceBootManagerLib|Pine64Pkg/Library/DeviceBootManagerLib/DeviceBootManagerLib.inf
  #
  # LIbrary to display the On screen notifications for the platform
  #
  UiRectangleLib|MsGraphicsPkg/Library/BaseUiRectangleLib/BaseUiRectangleLib.inf
  #
  # display the device state notifications for the platform using color bars
  #
  DisplayDeviceStateLib|MsGraphicsPkg/Library/ColorBarDisplayDeviceStateLib/ColorBarDisplayDeviceStateLib.inf
  #
  # interfaces with alternate boot variable
  #
  MsAltBootLib|Pine64Pkg/Library/MsAltBootLib/MsAltBootLib.inf
  #
  # Library that is attached BdsDxe to implement Microsoft extensions to UefiBootManagerLib.
  #
  MsBootOptionsLib|PcBdsPkg/Library/MsBootOptionsLib/MsBootOptionsLib.inf
  #
  # A shared place for all colors used in this package
  #
  MsColorTableLib|MsGraphicsPkg/Library/MsColorTableLib/MsColorTableLib.inf
  #
  # Library that is attached to drivers that require networking.
  #
  MsNetworkDependencyLib|PcBdsPkg/Library/MsNetworkDependencyLib/MsNetworkDependencyLib.inf
  #
  # Library to provide interface on Reboot Reason non volatile varialbles
  #
  MsNVBootReasonLib|Pine64Pkg/Library/MsNVBootReasonLib/MsNVBootReasonLib.inf
  #
  # An architecture agnostic math library providing reasonable approximations for various functions in software
  #
  MathLib|MsCorePkg/Library/MathLib/MathLib.inf
  #
  # This library is for showing the main system boot graphics.
  #
  BootGraphicsLib|MsGraphicsPkg/Library/BootGraphicsLib/BootGraphicsLib.inf
  #
  # Library to support Device disable Settings  (get/set)
  #
  MsBootManagerSettingsLib|Pine64Pkg/Library/MsBootManagerSettingsDxeLib/MsBootManagerSettingsDxeLib.inf
  #
  # Library to provide Dialog Boxes in a Simple Window Manager environment.
  #
  SwmDialogsLib|MsGraphicsPkg/Library/SwmDialogsLib/SwmDialogs.inf
  #
  # BootGraphicsProviderLib that uses PCDs and raw files in the firmware volumes
  #
  BootGraphicsProviderLib|Pine64Pkg/Library/BootGraphicsProviderLib/BootGraphicsProviderLib.inf
  #
  # Library that platform implemented to determine boot type
  #
  MsBootPolicyLib|Pine64Pkg/Library/MsBootPolicyLib/MsBootPolicyLib.inf
  BmpSupportLib|MdeModulePkg/Library/BaseBmpSupportLib/BaseBmpSupportLib.inf
  #
  # Supplies the theme for this platform to the UEFI settings UI
  #
  MsUiThemeLib|MsGraphicsPkg/Library/MsUiThemeLib/Dxe/MsUiThemeLib.inf
  SafeIntLib|MdePkg/Library/BaseSafeIntLib/BaseSafeIntLib.inf
  #
  # Library for determining if Manufacturing mode should be enabled on this boot. 
  # This libray is what everyone should use to determine which mode we are in.
  # It can be called from PEI or DXE.
  #
  UIToolKitLib|MsGraphicsPkg/Library/SimpleUIToolKit/SimpleUIToolKit.inf
  ResetUtilityLib|MdeModulePkg/Library/ResetUtilityLib/ResetUtilityLib.inf
  BaseBinSecurityLib|MdePkg/Library/BaseBinSecurityLibNull/BaseBinSecurityLibNull.inf
  SecurityLockAuditLib|MdeModulePkg/Library/SecurityLockAuditLibNull/SecurityLockAuditLibNull.inf

[PcdsFixedAtBuild.common]
  # a PCD that controls the enumeration and connection of ConIn's. When true, ConIn is only connected once a console input is requests 
  gEfiMdeModulePkgTokenSpaceGuid.PcdConInConnectOnDemand|TRUE

# Enable SHELL to build instead of just taking the binary
  gEfiMdePkgTokenSpaceGuid.PcdUefiLibMaxPrintBufferSize|16000
  gEfiShellPkgTokenSpaceGuid.PcdShellProfileMask|0x1f    # All profiles

  gMsGraphicsPkgTokenSpaceGuid.PcdUiThemeInDxe|TRUE
  gEfiMdeModulePkgTokenSpaceGuid.PcdBootManagerInBootOrder|TRUE
#TODO  gEfiMdeModulePkgTokenSpaceGuid.PcdPlatformRecoverySupported|FALSE
  # The GUID of Frontpage.inf from Pine64Pkg
  # b01d8615-fbdb-45f8-bb5a-c9d69dc0e79e
  gEfiMdeModulePkgTokenSpaceGuid.PcdBootManagerMenuFile|{ 0x15, 0x86, 0x1d, 0xb0, 0xdb, 0xfd, 0xf8, 0x45, 0xbb, 0x5a, 0xc9, 0xd6, 0x9d, 0xc0, 0xe7, 0x9e }

[Components.AARCH64]

  #
  # Spoofs button press to automatically boot to FrontPage.
  #
  Pine64Pkg/FrontpageButtonsVolumeUp/FrontpageButtonsVolumeUp.inf

  #
  # Application that presents and manages FrontPage.
  #
  Pine64Pkg/FrontPage/FrontPage.inf
  # Surface Boot Manager (Menu) application
  #
  # Application that presents & manages the Boot Menu Setup on Front Page.
  #
  Pine64Pkg/BootMenu/BootMenu.inf

  PcBdsPkg/MsBootPolicy/MsBootPolicy.inf

  MdeModulePkg/Universal/BootManagerPolicyDxe/BootManagerPolicyDxe.inf
  #
  # Library to support Remote/IT/Admin Settings such as DFCI
  #
  MdeModulePkg/Universal/RegularExpressionDxe/RegularExpressionDxe.inf
  #
  # Main component of DFCI. Manages and enforces all DFCI settings.
  #
  DfciPkg/SettingsManager/SettingsManagerDxe.inf {
        #Platform should add all it settings libs here
  <LibraryClasses>
        NULL|ZeroTouchPkg/Library/ZeroTouchSettings/ZeroTouchSettings.inf
        NULL|DfciPkg/Library/DfciSettingsLib/DfciSettingsLib.inf
        DfciSettingPermissionLib|DfciPkg/Library/DfciSettingPermissionLib/DfciSettingPermissionLib.inf
        NULL|Pine64Pkg/Library/MsBootManagerSettingsDxeLib/MsBootManagerSettingsDxeLib.inf
  <PcdsFeatureFlag>
     gDfciPkgTokenSpaceGuid.PcdSettingsManagerInstallProvider|TRUE
  }
  #
  # AuthManager provides authentication for DFCI. AuthManagerNull passes out a consistent token to allow the rest 
  # of FrontPage to be developed and tested while RngLib or other parts of the authentication process are being developed.
  #
  DfciPkg/AuthManagerNull/AuthManagerNull.inf
  #
  # Processes ingoing and outgoing DFCI settings requests.
  #
  DfciPkg/DfciManager/DfciManager.inf
  #
  # Manages windows and fonts to be drawn by the RenderingEngine.
  #
  MsGraphicsPkg/SimpleWindowManagerDxe/SimpleWindowManagerDxe.inf
  #
  # Produces EfiGraphicsOutputProtocol to draw graphics to the screen.
  #
  MsGraphicsPkg/RenderingEngineDxe/RenderingEngineDxe.inf
  #
  # Finds and takes ownership of gEfiGraphicsOutputProtocolGuid so RenderingEngine can use it.
  #
  MsGraphicsPkg/GopOverrideDxe/GopOverrideDxe.inf
  #
  # Driver for On Screen Keyboard.
  #
  MsGraphicsPkg/OnScreenKeyboardDxe/OnScreenKeyboardDxe.inf
  #
  # Installs protocol to share the UI theme. If PcdUiThemeInDxe, this will involve calling the PlatformThemeLib directly.
  # Otherwise, the theme will have been generated in PEI and it will be located on a HOB.
  #
  MsGraphicsPkg/MsUiTheme/Dxe/MsUiThemeProtocol.inf
  #
  # Produces FORM DISPLAY ENGINE protocol. Handles input, displays strings.
  #
  MsGraphicsPkg/DisplayEngineDxe/DisplayEngineDxe.inf
  
  
#######################################
#  Shell
#######################################
!if $(CONFIG_INCLUDE_SHELL) == TRUE

  !if $(CONFIG_INCLUDE_TFTP_COMMAND) == TRUE
    ShellPkg/DynamicCommand/TftpDynamicCommand/TftpDynamicCommand.inf
  !endif

  ShellPkg/Application/Shell/Shell.inf {
    <LibraryClasses>
      NULL|ShellPkg/Library/UefiShellLevel2CommandsLib/UefiShellLevel2CommandsLib.inf
      NULL|ShellPkg/Library/UefiShellLevel1CommandsLib/UefiShellLevel1CommandsLib.inf
      NULL|ShellPkg/Library/UefiShellLevel3CommandsLib/UefiShellLevel3CommandsLib.inf
      NULL|ShellPkg/Library/UefiShellDriver1CommandsLib/UefiShellDriver1CommandsLib.inf
      NULL|ShellPkg/Library/UefiShellInstall1CommandsLib/UefiShellInstall1CommandsLib.inf
      NULL|ShellPkg/Library/UefiShellDebug1CommandsLib/UefiShellDebug1CommandsLib.inf
      NULL|ShellPkg/Library/UefiShellNetwork1CommandsLib/UefiShellNetwork1CommandsLib.inf
      NULL|ShellPkg/Library/UefiShellNetwork2CommandsLib/UefiShellNetwork2CommandsLib.inf
    <PcdsFixedAtBuild>
     gEfiShellPkgTokenSpaceGuid.PcdShellLibAutoInitialize|FALSE
  }

!endif