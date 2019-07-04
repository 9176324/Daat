/*
*
* Copyright (c) 2018 by blindtiger. All rights reserved.
*
* The contents of this file are subject to the Mozilla Public License Version
* 2.0 (the "License"); you may not use this file except in compliance with
* the License. You may obtain a copy of the License at
* http://www.mozilla.org/MPL/
*
* Software distributed under the License is distributed on an "AS IS" basis,
* WITHOUT WARRANTY OF ANY KIND, either express or implied. SEe the License
* for the specific language governing rights and limitations under the
* License.
*
* The Initial Developer of the Original e is blindtiger.
*
*/

#ifndef _HANDLER_H_
#define _HANDLER_H_

#ifdef __cplusplus
/* Assume byte packing throughout */
extern "C" {
#endif	/* __cplusplus */

    typedef struct _CPUINFO {
        ULONG Eax;
        ULONG Ebx;
        ULONG Ecx;
        ULONG Edx;
    } CPUINFO, *PCPUINFO;

    typedef struct _CCB * PCCB;

    VOID
        NTAPI
        __ops_cpuid(
            __in LONG Function,
            __in LONG SubFunction,
            __out PCPUINFO CpuInfo
        );

    VOID
        NTAPI
        __ops_invd(
            VOID
        );

    ULONG64
        NTAPI
        __ops_rdtsc(
            VOID
        );

    ULONG64
        NTAPI
        __ops_rdtscp(
            __out PULONG Aux
        );

    VOID
        NTAPI
        __ops_xsetbv(
            __in ULONG Xcr,
            __in ULONG64 Value
        );

    VOID
        NTAPI
        __inject_exception(
            __inout PCCB Block,
            __in ULONG64 Vector,
            __in_opt ULONG64 ErrorCode,
            __in ULONG64 Type
        );

#ifdef __cplusplus
}
#endif	/* __cplusplus */

#endif // !_HANDLER_H_
