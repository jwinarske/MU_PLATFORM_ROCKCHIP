/** @file

  Helper library to support the query of the in-effect MFCI policy and
  translate raw policy to PINE64 defined operation modes.

  Copyright (c) Microsoft Corporation
  SPDX-License-Identifier: BSD-2-Clause-Patent
**/

#ifndef PINE64_MFCI_LIB_H_
#define PINE64_MFCI_LIB_H_

#define PINE64_UEFI_CUSTOMER_MODE       0
#define PINE64_UEFI_MANUFACTURING_MODE  1

typedef UINT64 PINE64_UEFI_OPERATION_MODE;

/**
 * Inspect whether the current operation mode is categorized as manufacturing mode.
 *
 * @param[out] OperationMode      64 bit value from MFCI framework, indicating the current operation mode
 *
 * @return PINE64_UEFI_CUSTOMER_MODE         Current mode is customer mode.
 * @return PINE64_UEFI_MANUFACTURING_MODE    Current operation mode is manufacturing mode.
 */
PINE64_UEFI_OPERATION_MODE
EFIAPI
GetMfciSystemOperationMode (
  VOID
  );

#endif //PINE64_MFCI_LIB_H_
