# @file
# Script to Build OemPkg Mu UEFI firmware
#
# Copyright (c) Microsoft Corporation.
# Copyright (c) Joel Winarske.
# SPDX-License-Identifier: BSD-2-Clause-Patent
##
import os
import logging
import datetime
import uuid

from edk2toolext.environment import shell_environment
from edk2toolext.environment.uefi_build import UefiBuilder
from edk2toolext.invocables.edk2_platform_build import BuildSettingsManager
from edk2toolext.invocables.edk2_setup import SetupSettingsManager, RequiredSubmodule
from edk2toolext.invocables.edk2_update import UpdateSettingsManager
from edk2toolext.invocables.edk2_pr_eval import PrEvalSettingsManager
from edk2toollib.utility_functions import RunCmd, GetHostInfo
from typing import Tuple
from pathlib import Path
from io import StringIO

# Declare test whose failure will not return a non-zero exit code
FAILURE_EXEMPT_TESTS = {
    "BootAuditTestApp.efi": datetime.datetime(2023, 3, 7, 0, 0, 0),
    "VariablePolicyFuncTestApp.efi": datetime.datetime(2023, 3, 7, 0, 0, 0),
    "DeviceIdTestApp.efi": datetime.datetime(2023, 3, 7, 0, 0, 0),
    "DxePagingAuditTestApp.efi": datetime.datetime(2023, 3, 7, 0, 0, 0),
    "MemoryProtectionTestApp.efi": datetime.datetime(2023, 4, 5, 0, 0, 0),
    "MemoryAttributeProtocolFuncTestApp.efi": datetime.datetime(2023, 4, 5, 0, 0, 0),
}

# Allow failure exempt tests to be ignored for 90 days
FAILURE_EXEMPT_OMISSION_LENGTH = 90 * 24 * 60 * 60


# ####################################################################################### #
#                                Common Configuration                                     #
# ####################################################################################### #
class CommonPlatform:
    """ Common settings for this platform.  Define static data here and use
        for the different parts of stuart
    """
    PackagesSupported = ("OemPkg",)
    ArchSupported = "X64"
    TargetsSupported = ("DEBUG", "RELEASE")
    Scopes = ('edk2-build', 'cibuild', 'configdata')
    WorkspaceRoot = os.path.dirname(os.path.dirname(os.path.dirname(os.path.dirname(os.path.abspath(__file__)))))
    PackagesPath = (
        "Platforms/FriendlyElec/OemPkg",
        "MU_BASECORE",
        "Common/MU",
        "Common/MU_TIANO",
        "Common/MU_OEM_SAMPLE",
        "Features/DFCI",
        "Features/CONFIG",
        "Features/MM_SUPV",
        "edk2-rockchip"
    )

    @staticmethod
    def add_common_command_line_options(parser_obj) -> None:
        """Adds command line options common to settings managers."""
        parser_obj.add_argument('--codeql',
                                dest='codeql',
                                action='store_true',
                                default=False,
                                help="Optional - Produces CodeQL results from the build. See "
                                     "MU_BASECORE/.pytool/Plugin/CodeQL/Readme.md for more information.")

    @staticmethod
    def retrieve_common_command_line_options(args_) -> bool:
        """Retrieves command line options common to settings managers."""
        return args_.codeql

    @staticmethod
    def get_active_scopes(codeql_enabled: bool) -> Tuple[str, str, str]:
        """Returns the active scopes for the platform."""
        active_scopes = CommonPlatform.Scopes

        # Enable the CodeQL plugin if chosen on command line
        if codeql_enabled:
            if GetHostInfo().os == "Linux":
                active_scopes += ("codeql-linux-ext-dep",)
            else:
                active_scopes += ("codeql-windows-ext-dep",)
            active_scopes += ("codeql-build", "codeql-analyze")

        return active_scopes

    # ####################################################################################### #
    #                         Configuration for Update & Setup                                #
    # ####################################################################################### #


class SettingsManager(UpdateSettingsManager, SetupSettingsManager, PrEvalSettingsManager):

    def __init__(self):
        self.codeql = False
        self.ActualArchitectures = None

    def AddCommandLineOptions(self, parser_obj):
        """Add command line options to the argparser"""
        CommonPlatform.add_common_command_line_options(parser_obj)

    def RetrieveCommandLineOptions(self, args_):
        """Retrieve command line options from the argparser"""
        self.codeql = CommonPlatform.retrieve_common_command_line_options(args_)

    def GetPackagesSupported(self):
        """ return iterable of edk2 packages supported by this build.
        These should be edk2 workspace relative paths """
        return CommonPlatform.PackagesSupported

    def GetArchitecturesSupported(self):
        """ return iterable of edk2 architectures supported by this build """
        return CommonPlatform.ArchSupported

    def GetTargetsSupported(self):
        """ return iterable of edk2 target tags supported by this build """
        return CommonPlatform.TargetsSupported

    def GetRequiredSubmodules(self):
        """Return iterable containing RequiredSubmodule objects.

        !!! note
            If no RequiredSubmodules return an empty iterable
        """
        return [
            RequiredSubmodule("MU_BASECORE", True),
            RequiredSubmodule("Common/MU", True),
            RequiredSubmodule("Common/MU_TIANO", True),
            RequiredSubmodule("Common/MU_OEM_SAMPLE", True),
            RequiredSubmodule("Features/DFCI", True),
            RequiredSubmodule("Features/CONFIG", True),
            RequiredSubmodule("Features/MM_SUPV", True),
        ]

    def SetArchitectures(self, list_of_requested_architectures):
        """ Confirm the requests architecture list is valid and configure SettingsManager
        to run only the requested architectures.

        Raise Exception if a list_of_requested_architectures is not supported
        """
        unsupported = set(list_of_requested_architectures) - set(self.GetArchitecturesSupported())
        if len(unsupported) > 0:
            error_string = (
                    "Unsupported Architecture Requested: " + " ".join(unsupported))
            logging.critical(error_string)
            raise Exception(error_string)
        self.ActualArchitectures = list_of_requested_architectures

    def GetWorkspaceRoot(self):
        """ get WorkspacePath """
        print('WorkspaceRoot: %s' % CommonPlatform.WorkspaceRoot)
        return CommonPlatform.WorkspaceRoot

    def GetActiveScopes(self):
        """ return tuple containing scopes that should be active for this process """
        return CommonPlatform.get_active_scopes(self.codeql)

    def FilterPackagesToTest(self, changed_files_list: list, potential_packages_list: list) -> list:
        """ Filter other cases that this package should be built
        based on changed files. This should cover things that can't
        be detected as dependencies. """
        build_these_packages = []
        possible_packages = potential_packages_list.copy()
        for f in changed_files_list:
            # BaseTools files that might change the build
            if "BaseTools" in f:
                if os.path.splitext(f) not in [".txt", ".md"]:
                    build_these_packages = possible_packages
                    break

            # if the azure pipeline platform template file changed
            if "platform-build-run-steps.yml" in f:
                build_these_packages = possible_packages
                break

        return build_these_packages

    def GetPlatformDscAndConfig(self) -> tuple:
        """ If a platform desires to provide its DSC then Policy 4 will evaluate if
        any of the changes will be built in the dsc.

        The tuple should be (<workspace relative path to dsc file>, <input dictionary of dsc key value pairs>)
        """
        return "FriendlyElec/OemPkg/OemPkg.dsc", {}

    def GetName(self) -> str:
        return "Oem"

    def GetPackagesPath(self):
        """ Return a list of paths that should be mapped as edk2 PackagesPath """
        return CommonPlatform.PackagesPath

    # ####################################################################################### #
    #                         Actual Configuration for Platform Build                         #
    # ####################################################################################### #


class PlatformBuilder(UefiBuilder, BuildSettingsManager):
    def __init__(self):
        UefiBuilder.__init__(self)
        self.codeql = False

    def AddCommandLineOptions(self, parser_obj):
        """ Add command line options to the argparser """

        # In an effort to support common server based builds this parameter is added.  It is
        # checked for correctness but is never uses as this platform only supports a single set of
        # architectures.
        parser_obj.add_argument('-a', "--arch",
                                dest="build_arch",
                                type=str,
                                default="X64",
                                help="Optional - CSV of architecture to build.  X64 will use X64 for PEI and X64 for "
                                     "DXE and is the only valid option for this platform.")

        CommonPlatform.add_common_command_line_options(parser_obj)

    def RetrieveCommandLineOptions(self, args_):
        """  Retrieve command line options from the argparser """
        if args_.build_arch.upper() != "X64":
            raise Exception(
                "Invalid Arch Specified.  Please see comments in "
                "PlatformBuild.py::PlatformBuilder::AddCommandLineOptions")

        self.codeql = CommonPlatform.retrieve_common_command_line_options(args_)

    def GetWorkspaceRoot(self):
        """ get WorkspacePath """
        return CommonPlatform.WorkspaceRoot

    def GetPackagesPath(self):
        """ Return a list of workspace relative paths that should be mapped as edk2 PackagesPath """
        result = [
            shell_environment.GetBuildVars().GetValue("FEATURE_CONFIG_PATH", ""),
            shell_environment.GetBuildVars().GetValue("FEATURE_MM_SUPV_PATH", "")
        ]
        for a in CommonPlatform.PackagesPath:
            result.append(a)
        return result

    def GetActiveScopes(self):
        """ return tuple containing scopes that should be active for this process """
        return CommonPlatform.get_active_scopes(self.codeql)

    def GetName(self):
        """ Get the name of the repo, platform, or product being build. Used for naming the log file, among others """
        # Check the FlashImage bool and rename the log file if true.
        # Two builds are done during CI: one without the --FlashOnly flag
        # followed by one with the flag. self.FlashImage will be true if the
        # --FlashOnly flag is passed, meaning we will keep separate build and run logs
        if self.FlashImage:
            return "OemPkg_Run"
        return "OemPkg"

    def GetLoggingLevel(self, logger_type):
        """ Get the logging level for a given type
        base == lowest logging level supported
        con  == Screen logging
        txt  == plain text file logging
        md   == markdown file logging
        """
        # return logging.DEBUG
        return logging.INFO
        # return super().GetLoggingLevel(logger_type)

    def SetPlatformEnv(self):
        logging.debug("PlatformBuilder SetPlatformEnv")
        self.env.SetValue("PRODUCT_NAME", "OEM", "Platform Hardcoded")
        self.env.SetValue("ACTIVE_PLATFORM", "OemPkg/OemPkg.dsc", "Platform Hardcoded")
        self.env.SetValue("TARGET_ARCH", "X64", "Platform Hardcoded")
        self.env.SetValue("DISABLE_DEBUG_MACRO_CHECK", "TRUE", "Default to true")
        self.env.SetValue("EMPTY_DRIVE", "FALSE", "Default to false")
        self.env.SetValue("RUN_TESTS", "FALSE", "Default to false")
        self.env.SetValue("SHUTDOWN_AFTER_RUN", "FALSE", "Default to false")
        # needed to make FV size build report happy
        self.env.SetValue("BLD_*_BUILDID_STRING", "Unknown", "Default")
        # Default turn on build reporting.
        self.env.SetValue("BUILDREPORTING", "TRUE", "Enabling build report")
        self.env.SetValue("BUILDREPORT_TYPES", "PCD DEPEX FLASH BUILD_FLAGS LIBRARY FIXED_ADDRESS HASH",
                          "Setting build report types")
        self.env.SetValue("BLD_*_MEMORY_PROTECTION", "TRUE", "Default")
        # Include the MFCI test cert by default, override on the commandline with "BLD_*_SHIP_MODE=TRUE" if you want
        # the retail MFCI cert
        self.env.SetValue("BLD_*_SHIP_MODE", "FALSE", "Default")
        self.env.SetValue("CONF_AUTOGEN_INCLUDE_PATH",
                          self.mws.join(self.ws, "Platforms", "FriendlyElec", "OemPkg", "Include"), "Platform Defined")

        self.env.SetValue("YAML_POLICY_FILE", self.mws.join(self.ws, "OemPkg", "PolicyData", "PolicyDataUsb.yaml"),
                          "Platform Hardcoded")
        self.env.SetValue("POLICY_DATA_STRUCT_FOLDER", self.mws.join(self.ws, "OemPkg", "Include"), "Platform Defined")
        self.env.SetValue('POLICY_REPORT_FOLDER', self.mws.join(self.ws, "OemPkg", "PolicyData"), "Platform Defined")
        self.env.SetValue('MU_SCHEMA_DIR', self.mws.join(self.ws, "Platforms", "FriendlyElec", "OemPkg", "CfgData"),
                          "Platform Defined")
        self.env.SetValue('MU_SCHEMA_FILE_NAME', "OemPkgCfgData.xml", "Platform Hardcoded")
        self.env.SetValue('CONF_PROFILE_PATHS',
                          self.mws.join(self.ws, 'Platforms', 'FriendlyElec', 'OemPkg', 'CfgData',
                                        'Profile0OemPkgCfgData.csv') + " " +
                          self.mws.join(self.ws, 'Platforms', 'FriendlyElec', 'OemPkg', 'CfgData',
                                        'Profile1OemPkgCfgData.csv'),
                          "Platform Hardcoded"
                          )

        # Globally set CodeQL failures to be ignored in this repo.
        # Note: This has no impact if CodeQL is not active/enabled.
        self.env.SetValue("STUART_CODEQL_AUDIT_ONLY", "true", "Platform Defined")

        # Enabled all the SMM modules
        self.env.SetValue("BLD_*_SMM_ENABLED", "TRUE", "Default")

        return 0

    def SetPlatformEnvAfterTarget(self):
        logging.debug("PlatformBuilder SetPlatformEnvAfterTarget")
        if os.name == 'nt':
            self.env.SetValue("VIRTUAL_DRIVE_PATH", Path(self.env.GetValue("BUILD_OUTPUT_BASE"), "VirtualDrive.vhd"),
                              "Platform Hardcoded.")
        else:
            self.env.SetValue("VIRTUAL_DRIVE_PATH", Path(self.env.GetValue("BUILD_OUTPUT_BASE"), "VirtualDrive.img"),
                              "Platform Hardcoded.")

        return 0

    def PlatformPreBuild(self):
        # Here we build the secure policy blob for build system to use and add into the targeted FV
        policy_example_dir = self.mws.join(self.mws.WORKSPACE, "MmSupervisorPkg", "SupervisorPolicyTools",
                                           "MmIsolationPoliciesExample.xml")
        output_dir = os.path.join(self.env.GetValue("BUILD_OUTPUT_BASE"), "Policy")
        if not os.path.isdir(output_dir):
            os.makedirs(output_dir)
        output_name = os.path.join(output_dir, "secure_policy.bin")

        ret = self.Helper.MakeSupervisorPolicy(xml_file_path=policy_example_dir, output_binary_path=output_name)
        if ret != 0:
            raise Exception("SupervisorPolicyMaker Failed: Errorcode %d" % ret)
        self.env.SetValue("BLD_*_POLICY_BIN_PATH", output_name, "Set generated secure policy path")
        return ret

    def __SetEsrtGuidVars(self, var_name, guid_str, desc_string):
        cur_guid = uuid.UUID(guid_str)
        self.env.SetValue("BLD_*_%s_REGISTRY" % var_name, guid_str, desc_string)
        self.env.SetValue("BLD_*_%s_BYTES" % var_name,
                          "'{" + (",".join(("0x%X" % byte) for byte in cur_guid.bytes_le)) + "}'", desc_string)
        return

    def FlashRomImage(self):
        run_tests = (self.env.GetValue("RUN_TESTS", "FALSE").upper() == "TRUE")
        output_base = self.env.GetValue("BUILD_OUTPUT_BASE")
        shutdown_after_run = (self.env.GetValue("SHUTDOWN_AFTER_RUN", "FALSE").upper() == "TRUE")
        empty_drive = (self.env.GetValue("EMPTY_DRIVE", "FALSE").upper() == "TRUE")
        test_regex = self.env.GetValue("TEST_REGEX", "")
        drive_path = self.env.GetValue("VIRTUAL_DRIVE_PATH")
        run_paging_audit = False

        # General debugging information for users
        if run_tests:
            if test_regex == "":
                logging.warning("Running tests, but no Tests specified. use TEST_REGEX to specify tests to run.")

            if not empty_drive:
                logging.info("EMPTY_DRIVE=FALSE. Old files can persist, could effect test results.")

            if not shutdown_after_run:
                logging.info("SHUTDOWN_AFTER_RUN=FALSE. You will need to close qemu manually to gather test results.")

        # Get a reference to the virtual drive, creating / wiping as necessary
        # Helper located at QemuPkg/Plugins/VirtualDriveManager
        virtual_drive = self.Helper.get_virtual_drive(drive_path)
        if empty_drive:
            virtual_drive.wipe()

        if not virtual_drive.exists():
            virtual_drive.make_drive()

        # Add tests if requested, auto run if requested
        # Creates a startup script with the requested tests
        if test_regex != "":
            test_list = []
            for pattern in test_regex.split(","):
                test_list.extend(Path(output_base, "X64").glob(pattern))

            if any("DxePagingAuditTestApp.efi" in os.path.basename(test) for test in test_list):
                run_paging_audit = True

            self.Helper.add_tests(virtual_drive, test_list, auto_run=run_tests, auto_shutdown=shutdown_after_run,
                                  paging_audit=run_paging_audit)
        # Otherwise add an empty startup script
        else:
            virtual_drive.add_startup_script([], auto_shutdown=shutdown_after_run)

        # Get the version number (repo release)
        outstream = StringIO()
        version = "Unknown"
        ret = RunCmd('git', "rev-parse HEAD", outstream=outstream)
        if ret == 0:
            commithash = outstream.getvalue().strip()
            outstream = StringIO()
            # See git-describe docs for a breakdown of this command output
            ret = RunCmd("git", f'describe {commithash} --tags', outstream=outstream)
            if ret == 0:
                version = outstream.getvalue().strip()

        self.env.SetValue("VERSION", version, "Set Version value")

        # Run Qemu
        # Helper located at Platforms/FriendlyElec/OemPkg/Plugins/QemuRunner
        ret = self.Helper.QemuRun(self.env)
        if ret != 0:
            logging.critical("Failed running Qemu")
            return ret

        if not run_tests:
            return 0

        # Gather test results if they were run.
        now = datetime.datetime.now()
        fet = FAILURE_EXEMPT_TESTS
        feol = FAILURE_EXEMPT_OMISSION_LENGTH

        if run_paging_audit:
            self.Helper.generate_paging_audit(virtual_drive, Path(drive_path).parent / "unit_test_results",
                                              self.env.GetValue("VERSION"))

        # Filter out tests that are exempt
        test_list = ''
        tests = list(filter(lambda file: file.name not in fet or not (now - fet.get(file.name)).total_seconds() < feol,
                            test_list))
        tests_exempt = list(
            filter(lambda file: file.name in fet and (now - fet.get(file.name)).total_seconds() < feol, test_list))
        if len(tests_exempt) > 0:
            self.Helper.report_results(virtual_drive, tests_exempt, Path(drive_path).parent / "unit_test_results")
        # Helper located at QemuPkg/Plugins/VirtualDriveManager
        return self.Helper.report_results(virtual_drive, tests, Path(drive_path).parent / "unit_test_results")


if __name__ == "__main__":
    import argparse
    import sys
    from edk2toolext.invocables.edk2_update import Edk2Update
    from edk2toolext.invocables.edk2_setup import Edk2PlatformSetup
    from edk2toolext.invocables.edk2_platform_build import Edk2PlatformBuild

    print("Invoking Stuart")
    print("     ) _     _")
    print("    ( (^)-~-(^)")
    print("__,-.\\_( 0 0 )__,-.___")
    print("  'W'   \\   /   'W'")
    print("         >o<")
    SCRIPT_PATH = os.path.relpath(__file__)
    parser = argparse.ArgumentParser(add_help=False)
    parse_group = parser.add_mutually_exclusive_group()
    parse_group.add_argument("--update", "--UPDATE",
                             action='store_true', help="Invokes stuart_update")
    parse_group.add_argument("--setup", "--SETUP",
                             action='store_true', help="Invokes stuart_setup")
    args, remaining = parser.parse_known_args()
    new_args = ["stuart", "-c", SCRIPT_PATH]
    new_args = new_args + remaining
    sys.argv = new_args
    if args.setup:
        print("Running stuart_setup -c " + SCRIPT_PATH)
        Edk2PlatformSetup().Invoke()
    elif args.update:
        print("Running stuart_update -c " + SCRIPT_PATH)
        Edk2Update().Invoke()
    else:
        print("Running stuart_build -c " + SCRIPT_PATH)
        Edk2PlatformBuild().Invoke()
