/** @file ConfigClientGenerated.h

  Autogenerated configuration headers from mu_feature_config

  Copyright (c) Microsoft Corporation.
  SPDX-License-Identifier: BSD-2-Clause-Patent

**/

#ifndef CONFIGCLIENTGENERATED_H
#define CONFIGCLIENTGENERATED_H
#include <Uefi.h>
// Generated Header
//  Script: /mnt/raid10/rockchip/MU_PLATFORM_ROCKCHIP/Features/CONFIG/SetupDataPkg/Tools/KnobService.py
//  Schema: /mnt/raid10/rockchip/MU_PLATFORM_ROCKCHIP/Platforms/Pine64/PinebookProPkg/CfgData/PinebookProPkgCfgData.xml

#pragma pack(push, 1)
// Schema-defined enums
// Schema-defined structures
#pragma pack(pop)

// Schema-defined knobs
// DummyKnob1 knob


EFI_STATUS ConfigGetDummyKnob1 (UINT32 *Knob);
// DummyKnob2 knob


EFI_STATUS ConfigGetDummyKnob2 (UINT32 *Knob);
// PowerOnPort0 knob


EFI_STATUS ConfigGetPowerOnPort0 (BOOLEAN *Knob);
// DummyKnob3 knob


EFI_STATUS ConfigGetDummyKnob3 (UINT32 *Knob);

#define KNOB_DummyKnob1 0
#define KNOB_DummyKnob2 1
#define KNOB_PowerOnPort0 2
#define KNOB_DummyKnob3 3
#define KNOB_MAX 4
#endif // CONFIGCLIENTGENERATED_H
