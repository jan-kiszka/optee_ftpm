/* SPDX-License-Identifier: BSD-2-Clause */
/*
 * Copyright (c) 2026, Siemens AG
 *
 * Based on TCG TPM reference implementation V184
 * TPMCmd/Platform/src/Failure.c
 *
 * Copyright 2010-2022 Microsoft Corporation
 * Copyright 2022-2025 Trusted Computing Group and its contributors
 *
 * All rights reserved.
 */

//** Includes and locals
#include "Platform.h"
#include <assert.h>
#include <setjmp.h>
#include <stdio.h>

// Macro to suppress warning about intentionally unreferenced parameters.
#ifndef UNREFERENCED_PARAMETER
#define UNREFERENCED_PARAMETER(a) do { (void)(a); } while (0)
#endif

#if LONGJMP_SUPPORTED
// in RunCommand.c
extern jmp_buf s_FailureModeJumpBuffer;
#endif

#if ALLOW_FORCE_FAILURE_MODE
static BOOL s_forceFailureMode;  // flag to force failure mode during test
BOOL        _plat_internal_IsForceFailureMode()
{
    return s_forceFailureMode;
}
LIB_EXPORT void _plat__SetForceFailureMode()
{
    s_forceFailureMode = TRUE;
}
#endif

#if FAIL_TRACE
// The name of the function that triggered failure mode.
static const char* s_failFunctionName;
// The line in the file at which the error was signaled.
static uint32_t s_failLine;
#endif  // FAIL_TRACE

// A numeric indicator of the location that triggered failure mode.
static uint64_t s_failureLocation;
// the reason for the failure.
static uint32_t s_failCode;
static BOOL     s_IsInFailureMode = FALSE;

void            _plat_internal_resetFailureData(void)
{
#if ALLOW_FORCE_FAILURE_MODE
    s_forceFailureMode = FALSE;
#endif

#if FAIL_TRACE
    // The name of the function that triggered failure mode.
    s_failFunctionName = NULL;
    // The line in the file at which the error was signaled.
    s_failLine = 0;
#endif  // FAIL_TRACE

    // A numeric indicator of the location that triggered failure mode.
    s_failureLocation = 0;
    // the reason for the failure.
    s_failCode        = 0;
    s_IsInFailureMode = FALSE;
}

// Indicates to the TPM Library that a failure has occurred.
// This is REQURIED to return true after any call to _plat__Fail.
// It MAY return true for any other reason the platform deems appropriate.
LIB_EXPORT BOOL _plat__InFailureMode(void)
{
    return s_IsInFailureMode;
}

//***_plat__Fail()
// This is the platform depended failure exit for the TPM.
LIB_EXPORT NORETURN_IF_LONGJMP void _plat__Fail(
#if FAIL_TRACE
    const char* function,
    int         line,
#endif
    uint64_t locationCode,
    int      failureCode)
{
#if FAIL_TRACE
    UNREFERENCED_PARAMETER(function);
    UNREFERENCED_PARAMETER(line);
#endif
    UNREFERENCED_PARAMETER(locationCode);
    UNREFERENCED_PARAMETER(failureCode);
    TEE_Panic(TEE_ERROR_BAD_STATE);
    while (true); /* Not reached */
}

LIB_EXPORT UINT32 _plat__GetFailureCode(void)
{
    return s_failCode;
}

LIB_EXPORT uint64_t _plat__GetFailureLocation(void)
{
    return s_failureLocation;
}

#if FAIL_TRACE
LIB_EXPORT const char* _plat__GetFailureFunctionName(void)
{
    return s_failFunctionName;
}

LIB_EXPORT uint32_t _plat__GetFailureLine(void)
{
    return s_failLine;
}
#endif
