/** @file ConfigServiceGenerated.h

  Autogenerated configuration headers from mu_feature_config

  Copyright (c) Microsoft Corporation.
  SPDX-License-Identifier: BSD-2-Clause-Patent

**/

#ifndef CONFIGSERVICEGENERATED_H
#define CONFIGSERVICEGENERATED_H
// The config public header must be included prior to this file
// Generated Header
//  Script: /mnt/raid10/rockchip/MU_PLATFORM_ROCKCHIP/Features/CONFIG/SetupDataPkg/Tools/KnobService.py
//  Schema: /mnt/raid10/rockchip/MU_PLATFORM_ROCKCHIP/Platforms/FriendlyElec/R5CPkg/CfgData/R5CPkgCfgData.xml

STATIC CONST UINT16  CachedPolicySize = 0xe9;
STATIC CHAR8 CachedPolicy[0xe9];
STATIC BOOLEAN CachedPolicyInitialized = FALSE;
STATIC_ASSERT(0xe9 <= MAX_UINT16, "Config too large!");

STATIC
EFI_STATUS
InitConfigPolicyCache (
  VOID
)
{
  EFI_STATUS Status;
  UINT16 ConfPolSize = CachedPolicySize;

  Status = GetPolicy (PcdGetPtr (PcdConfigurationPolicyGuid), NULL, CachedPolicy, &ConfPolSize);
  if ((EFI_ERROR (Status)) || (ConfPolSize != CachedPolicySize)) {
    ASSERT (FALSE);
    return Status;
  }

  CachedPolicyInitialized = TRUE;

  return Status;
}

// Schema-defined knobs
// DummyKnob1 knob
// Get the current value of the DummyKnob1 knob
EFI_STATUS ConfigGetDummyKnob1 (
  UINT32 *Knob
  )
{
  EFI_STATUS Status;
  CONST UINTN Offset = 50;

  if (Knob == NULL) {
    return EFI_INVALID_PARAMETER;
  }

  if (!CachedPolicyInitialized) {
    Status = InitConfigPolicyCache ();
    if (EFI_ERROR (Status)) {
      ASSERT (FALSE);
      return Status;
    }
  }

  if (Offset + sizeof(UINT32) > CachedPolicySize) {
    ASSERT (FALSE);
    return EFI_COMPROMISED_DATA;
  }

  CopyMem(Knob, CachedPolicy + Offset, sizeof (UINT32));
  return EFI_SUCCESS;
}

// DummyKnob2 knob
// Get the current value of the DummyKnob2 knob
EFI_STATUS ConfigGetDummyKnob2 (
  UINT32 *Knob
  )
{
  EFI_STATUS Status;
  CONST UINTN Offset = 108;

  if (Knob == NULL) {
    return EFI_INVALID_PARAMETER;
  }

  if (!CachedPolicyInitialized) {
    Status = InitConfigPolicyCache ();
    if (EFI_ERROR (Status)) {
      ASSERT (FALSE);
      return Status;
    }
  }

  if (Offset + sizeof(UINT32) > CachedPolicySize) {
    ASSERT (FALSE);
    return EFI_COMPROMISED_DATA;
  }

  CopyMem(Knob, CachedPolicy + Offset, sizeof (UINT32));
  return EFI_SUCCESS;
}

// PowerOnPort0 knob
// Get the current value of the PowerOnPort0 knob
EFI_STATUS ConfigGetPowerOnPort0 (
  BOOLEAN *Knob
  )
{
  EFI_STATUS Status;
  CONST UINTN Offset = 170;

  if (Knob == NULL) {
    return EFI_INVALID_PARAMETER;
  }

  if (!CachedPolicyInitialized) {
    Status = InitConfigPolicyCache ();
    if (EFI_ERROR (Status)) {
      ASSERT (FALSE);
      return Status;
    }
  }

  if (Offset + sizeof(BOOLEAN) > CachedPolicySize) {
    ASSERT (FALSE);
    return EFI_COMPROMISED_DATA;
  }

  CopyMem(Knob, CachedPolicy + Offset, sizeof (BOOLEAN));
  return EFI_SUCCESS;
}

// DummyKnob3 knob
// Get the current value of the DummyKnob3 knob
EFI_STATUS ConfigGetDummyKnob3 (
  UINT32 *Knob
  )
{
  EFI_STATUS Status;
  CONST UINTN Offset = 225;

  if (Knob == NULL) {
    return EFI_INVALID_PARAMETER;
  }

  if (!CachedPolicyInitialized) {
    Status = InitConfigPolicyCache ();
    if (EFI_ERROR (Status)) {
      ASSERT (FALSE);
      return Status;
    }
  }

  if (Offset + sizeof(UINT32) > CachedPolicySize) {
    ASSERT (FALSE);
    return EFI_COMPROMISED_DATA;
  }

  CopyMem(Knob, CachedPolicy + Offset, sizeof (UINT32));
  return EFI_SUCCESS;
}

#endif // CONFIGSERVICEGENERATED_H
