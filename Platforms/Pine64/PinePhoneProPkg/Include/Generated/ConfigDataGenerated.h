/** @file ConfigDataGenerated.h

  Autogenerated configuration headers from mu_feature_config

  Copyright (c) Microsoft Corporation.
  SPDX-License-Identifier: BSD-2-Clause-Patent

**/

#ifndef CONFIGDATAGENERATED_H
#define CONFIGDATAGENERATED_H
// The config public header must be included prior to this file
// Generated Header
//  Script: /home/joel.winarske@toyotaconnected.com/development/MU_PLATFORM_ROCKCHIP/Features/CONFIG/SetupDataPkg/Tools/KnobService.py
//  Schema: /home/joel.winarske@toyotaconnected.com/development/MU_PLATFORM_ROCKCHIP/Platforms/Pine64/PinePhoneProPkg/CfgData/PinePhoneProPkgCfgData.xml

typedef struct {
  UINT32 DummyKnob1;
  UINT32 DummyKnob2;
  BOOLEAN PowerOnPort0;
  UINT32 DummyKnob3;
} KNOB_VALUES;

CONST KNOB_VALUES gKnobDefaultValues = 
{
  .DummyKnob1=333ul,
  .DummyKnob2=444ul,
  .PowerOnPort0=TRUE,
  .DummyKnob3=555ul,
};
#ifdef CONFIG_INCLUDE_CACHE
KNOB_VALUES gKnobCachedValues = 
{
    .DummyKnob1=333ul,
    .DummyKnob2=444ul,
    .PowerOnPort0=TRUE,
    .DummyKnob3=555ul,
};
#define CONFIG_CACHE_ADDRESS(knob) (&gKnobCachedValues.knob)
#else // CONFIG_INCLUDE_CACHE
#define CONFIG_CACHE_ADDRESS(Knob) (NULL)
#endif // CONFIG_INCLUDE_CACHE


BOOLEAN ValidateKnobNoConstraints(CONST VOID * Buffer)
{
  (VOID)Buffer;
  return TRUE;
}

#define ValidateKnobContentDummyKnob1 ValidateKnobNoConstraints

#define ValidateKnobContentDummyKnob2 ValidateKnobNoConstraints

#define ValidateKnobContentPowerOnPort0 ValidateKnobNoConstraints

#define ValidateKnobContentDummyKnob3 ValidateKnobNoConstraints


KNOB_DATA gKnobData[5] = {
  {
    KNOB_DummyKnob1,
    &gKnobDefaultValues.DummyKnob1,
    CONFIG_CACHE_ADDRESS(DummyKnob1),
    sizeof(UINT32),
    "DummyKnob1",
    11, // Length of name (including NULL terminator)
    {0x5e6a1994,0xe683,0x4d8c,{0x9d,0xc1,0xf1,0xf6,0x34,0x90,0xfc,0x60}}, // 5E6A1994-E683-4D8C-9DC1-F1F63490FC60
    7,
    &ValidateKnobContentDummyKnob1
  },
  {
    KNOB_DummyKnob2,
    &gKnobDefaultValues.DummyKnob2,
    CONFIG_CACHE_ADDRESS(DummyKnob2),
    sizeof(UINT32),
    "DummyKnob2",
    11, // Length of name (including NULL terminator)
    {0x5e6a1994,0xe683,0x4d8c,{0x9d,0xc1,0xf1,0xf6,0x34,0x90,0xfc,0x60}}, // 5E6A1994-E683-4D8C-9DC1-F1F63490FC60
    7,
    &ValidateKnobContentDummyKnob2
  },
  {
    KNOB_PowerOnPort0,
    &gKnobDefaultValues.PowerOnPort0,
    CONFIG_CACHE_ADDRESS(PowerOnPort0),
    sizeof(BOOLEAN),
    "PowerOnPort0",
    13, // Length of name (including NULL terminator)
    {0x5e6a1994,0xe683,0x4d8c,{0x9d,0xc1,0xf1,0xf6,0x34,0x90,0xfc,0x60}}, // 5E6A1994-E683-4D8C-9DC1-F1F63490FC60
    7,
    &ValidateKnobContentPowerOnPort0
  },
  {
    KNOB_DummyKnob3,
    &gKnobDefaultValues.DummyKnob3,
    CONFIG_CACHE_ADDRESS(DummyKnob3),
    sizeof(UINT32),
    "DummyKnob3",
    11, // Length of name (including NULL terminator)
    {0x5e6a1994,0xe683,0x4d8c,{0x9d,0xc1,0xf1,0xf6,0x34,0x90,0xfc,0x60}}, // 5E6A1994-E683-4D8C-9DC1-F1F63490FC60
    7,
    &ValidateKnobContentDummyKnob3
  },
  {
    KNOB_MAX,
    NULL,
    NULL,
    0,
    NULL,
    0,
    {0,0,0,{0,0,0,0,0,0,0,0}},
    0,
    NULL
  }
};

UINTN gNumKnobs = KNOB_MAX;
#endif // CONFIGDATAGENERATED_H
