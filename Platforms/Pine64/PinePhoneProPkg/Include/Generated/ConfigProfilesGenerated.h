/** @file ConfigProfilesGenerated.h

  Autogenerated configuration headers from mu_feature_config

  Copyright (c) Microsoft Corporation.
  SPDX-License-Identifier: BSD-2-Clause-Patent

**/

#ifndef CONFIGPROFILESGENERATED_H
#define CONFIGPROFILESGENERATED_H
// The config public header must be included prior to this file
// Generated Header
//  Script: /home/joel.winarske@toyotaconnected.com/development/MU_PLATFORM_ROCKCHIP/Features/CONFIG/SetupDataPkg/Tools/KnobService.py
//  Schema: /home/joel.winarske@toyotaconnected.com/development/MU_PLATFORM_ROCKCHIP/Platforms/Pine64/PinePhoneProPkg/CfgData/PinePhoneProPkgCfgData.xml
//  Profile: /home/joel.winarske@toyotaconnected.com/development/MU_PLATFORM_ROCKCHIP/Platforms/Pine64/PinePhoneProPkg/CfgData/Profile0PinePhoneProPkgCfgData.csv
//  Profile: /home/joel.winarske@toyotaconnected.com/development/MU_PLATFORM_ROCKCHIP/Platforms/Pine64/PinePhoneProPkg/CfgData/Profile1PinePhoneProPkgCfgData.csv

// Profile Profile0PinePhoneProPkgCfgData
typedef struct {
  UINT32 DummyKnob1;
  UINT32 DummyKnob2;
  BOOLEAN PowerOnPort0;
  UINT32 DummyKnob3;
} PROFILE_Profile0PinePhoneProPkgCfgData_DATA;

PROFILE_Profile0PinePhoneProPkgCfgData_DATA ProfileProfile0PinePhoneProPkgCfgDataData = {
    .DummyKnob1=555ul,
    .DummyKnob2=555ul,
    .PowerOnPort0=FALSE,
    .DummyKnob3=666ul,
};

#define PROFILE_PROFILE0PINEPHONEPROPKGCFGDATA_OVERRIDES
#define PROFILE_PROFILE0PINEPHONEPROPKGCFGDATA_OVERRIDES_COUNT 4
KNOB_OVERRIDE ProfileProfile0PinePhoneProPkgCfgDataOverrides[PROFILE_PROFILE0PINEPHONEPROPKGCFGDATA_OVERRIDES_COUNT + 1] = {
  {
    .Knob = KNOB_DummyKnob1,
    .Value = &ProfileProfile0PinePhoneProPkgCfgDataData.DummyKnob1,
  },
  {
    .Knob = KNOB_DummyKnob2,
    .Value = &ProfileProfile0PinePhoneProPkgCfgDataData.DummyKnob2,
  },
  {
    .Knob = KNOB_PowerOnPort0,
    .Value = &ProfileProfile0PinePhoneProPkgCfgDataData.PowerOnPort0,
  },
  {
    .Knob = KNOB_DummyKnob3,
    .Value = &ProfileProfile0PinePhoneProPkgCfgDataData.DummyKnob3,
  },
  {
    .Knob = KNOB_MAX,
    .Value = NULL,
  }
};

// Profile Profile1PinePhoneProPkgCfgData
typedef struct {
  UINT32 DummyKnob2;
  UINT32 DummyKnob3;
} PROFILE_Profile1PinePhoneProPkgCfgData_DATA;

PROFILE_Profile1PinePhoneProPkgCfgData_DATA ProfileProfile1PinePhoneProPkgCfgDataData = {
    .DummyKnob2=555ul,
    .DummyKnob3=666ul,
};

#define PROFILE_PROFILE1PINEPHONEPROPKGCFGDATA_OVERRIDES
#define PROFILE_PROFILE1PINEPHONEPROPKGCFGDATA_OVERRIDES_COUNT 2
KNOB_OVERRIDE ProfileProfile1PinePhoneProPkgCfgDataOverrides[PROFILE_PROFILE1PINEPHONEPROPKGCFGDATA_OVERRIDES_COUNT + 1] = {
  {
    .Knob = KNOB_DummyKnob2,
    .Value = &ProfileProfile1PinePhoneProPkgCfgDataData.DummyKnob2,
  },
  {
    .Knob = KNOB_DummyKnob3,
    .Value = &ProfileProfile1PinePhoneProPkgCfgDataData.DummyKnob3,
  },
  {
    .Knob = KNOB_MAX,
    .Value = NULL,
  }
};


#define PROFILE_COUNT 2
PROFILE gProfileData[PROFILE_COUNT + 1] = 
{
  {
    .Overrides = ProfileProfile0PinePhoneProPkgCfgDataOverrides,
    .OverrideCount = 4,
  },
  {
    .Overrides = ProfileProfile1PinePhoneProPkgCfgDataOverrides,
    .OverrideCount = 2,
  },
  {
    .Overrides = NULL,
    .OverrideCount = 0,
  }
};

UINTN gNumProfiles = PROFILE_COUNT;
#endif // CONFIGPROFILESGENERATED_H
