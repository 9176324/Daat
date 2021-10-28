/*
*
* Copyright (c) 2015 - 2021 by blindtiger. All rights reserved.
*
* The contents of this file are subject to the Mozilla Public License Version
* 2.0 (the "License") you may not use this file except in compliance with
* the License. You may obtain a copy of the License at
* http://www.mozilla.org/MPL/
*
* Software distributed under the License is distributed on an "AS IS" basis,
* WITHOUT WARRANTY OF ANY KIND, either express or implied. SEe the License
* for the specific language governing rights and limitations under the
* License.
*
* The Initial Developer of the Original Code is blindtiger.
*
*/

#ifndef _INTRIN_H_
#define _INTRIN_H_

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

    typedef struct _TSB * PTSB;

    void
        NTAPI
        __ins_cpuid(
            __in s32 Function,
            __in s32 SubFunction,
            __out PCPUINFO CpuInfo
        );

    void
        NTAPI
        __ins_invd(
            void
        );

    u64
        NTAPI
        __ins_rdtsc(
            void
        );

    u64
        NTAPI
        __ins_rdtscp(
            __out u32ptr Aux
        );

    void
        NTAPI
        __ins_xsetbv(
            __in u32 Xcr,
            __in u64 Value
        );

    u32
        NTAPI
        __ins_sldt(
            __in u16ptr Selector
        );

    u32
        NTAPI
        __ins_str(
            __in u16ptr Selector
        );

    u32
        NTAPI
        __ins_sgdt(
            __in u16ptr Limit
        );

    u32
        NTAPI
        __ins_lgdt(
            __in u16ptr Limit
        );

    u32
        NTAPI
        __ins_sidt(
            __in u16ptr Limit
        );

    u32
        NTAPI
        __ins_lidt(
            __in u16ptr Limit
        );

    u64
        NTAPI
        __ins_readmsr(
            __in u32 Register
        );

    void
        NTAPI
        __ins_writemsr(
            __in u32 Register,
            __in u64 Value
        );

    u
        NTAPI
        __ins_readcr(
            __in u32 Register
        );

    void
        NTAPI
        __ins_writecr(
            __in u32 Register,
            __in u Value
        );

    u
        NTAPI
        __ins_readdr(
            __in u32 Register
        );

    void
        NTAPI
        __ins_writedr(
            __in u32 Register,
            __in u Value
        );

    u32
        NTAPI
        __ins_segment_limit(
            __in u32 Selector
        );

    u32
        NTAPI
        __ins_segment_ar(
            __in u32 Selector
        );

    typedef struct _EPT_DESCRIPTOR {
        u64 EptPointer;
        u64 Reserved;
    }EPT_DESCRIPTOR, *PEPT_DESCRIPTOR;

    typedef enum _VMX_RESULT VMX_RESULT;

    VMX_RESULT
        NTAPI
        __ins_invept(
            __in u32 Type,
            __in PEPT_DESCRIPTOR Descriptor
        );

    VMX_RESULT
        NTAPI
        __ins_invvpid(
            __in u32 Type,
            __in PEPT_DESCRIPTOR Descriptor
        );

    VMX_RESULT
        NTAPI
        __ins_vmx_on(
            __in u64 * SupportPhysicalAddress
        );

    VMX_RESULT
        NTAPI
        __ins_vmx_off(
            void
        );

    VMX_RESULT
        NTAPI
        __ins_vmx_vmclear(
            __in u64 * VmcsPhysicalAddress
        );

    VMX_RESULT
        NTAPI
        __ins_vmx_vmptrld(
            __in u64 * VmcsPhysicalAddress
        );

    VMX_RESULT
        NTAPI
        __ins_vmx_vmptrst(
            __in u64 * VmcsPhysicalAddress
        );

    VMX_RESULT
        NTAPI
        __ins_vmx_vmread(
            __in u Field,
            __out u * Value
        );

    VMX_RESULT
        NTAPI
        __ins_vmx_vmwrite(
            __in u Field,
            __in u Value
        );

    VMX_RESULT
        NTAPI
        __ins_vmx_vmlaunch(
            void
        );

    VMX_RESULT
        NTAPI
        __vmx_vmresume(
            void
        );

#ifdef __cplusplus
}
#endif	/* __cplusplus */

#endif // !_INTRIN_H_
