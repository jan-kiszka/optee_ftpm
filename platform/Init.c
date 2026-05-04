/* SPDX-License-Identifier: BSD-2-Clause */
/*
 * Based on TCG TPM reference implementation V184
 * TPMCmd/Platform/src/Init.c
 *
 * Copyright 2010-2022 Microsoft Corporation
 * Copyright 2022-2025 Trusted Computing Group and its contributors
 *
 * All rights reserved.
 */

#include "Platform.h"

// Notification at very start of TPM_Init();
LIB_EXPORT void _plat__StartTpmInit(void)
{
    // call platform reset functions, that have no TPM dependencies
    // needs the failure change
    _plat_internal_resetFailureData();
}

LIB_EXPORT void _plat__EndOkTpmInit(void)
{
    // call platform reset functions that depend on previous TPM initialization
    // (none in this implementation)
}
