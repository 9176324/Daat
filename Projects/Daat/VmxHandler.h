/*
*
* Copyright (c) 2015 - 2021 by blindtiger. All rights reserved.
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

#ifndef _VMXHANDLER_H_
#define _VMXHANDLER_H_

#ifdef __cplusplus
/* Assume byte packing throughout */
extern "C" {
#endif	/* __cplusplus */

    typedef struct _CPUINFO {
        u32 Eax;
        u32 Ebx;
        u32 Ecx;
        u32 Edx;
    } CPUINFO, *PCPUINFO;

    typedef struct _CCB * PCCB;

    void
        NTAPI
        __ops_cpuid(
            __in s32 Function,
            __in s32 SubFunction,
            __out PCPUINFO CpuInfo
        );

    void
        NTAPI
        __ops_invd(
            void
        );

    u64
        NTAPI
        __ops_rdtsc(
            void
        );

    u64
        NTAPI
        __ops_rdtscp(
            __out u32ptr Aux
        );

    void
        NTAPI
        __ops_xsetbv(
            __in u32 Xcr,
            __in u64 Value
        );

    void
        NTAPI
        __inject_exception(
            __inout PCCB Block,
            __in u64 Vector,
            __in_opt u64 ErrorCode,
            __in u64 Type
        );

#ifdef __cplusplus
}
#endif	/* __cplusplus */

#endif // !_VMXHANDLER_H_
