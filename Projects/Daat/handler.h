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

    typedef struct _CCB * PCCB;

    typedef
        VOID
        (NTAPI * PVM_HANDLER)(
            __inout PCCB Block
            );

    typedef union _REASON {
        struct {
            ULONG64 BasicReason : 16;
            ULONG64 NOTHING : 12;
            ULONG64 PendingMtf : 1;
            ULONG64 ExitRoot : 1;
            ULONG64 ExitFail : 1;
            ULONG64 EnterFail : 1;
        };
    }REASON, *PREASON;

    typedef union _QUALIFICATION {
        struct {
            ULONG64 Size : 3;
            ULONG64 Direction : 1;
            ULONG64 String : 1;
            ULONG64 Rep : 1;
            ULONG64 Encoding : 1;
            ULONG64 NOTHING : 9;
            ULONG64 Port : 16;
        } IO;

        struct {
            ULONG64 Number : 4;
            ULONG64 Type : 2;
            ULONG64 LMSW : 2;
            ULONG64 GPReg : 4;
            ULONG64 NOTHING : 4;
            ULONG64 LMSWSource : 16;
        } CR;

        struct {
            ULONG64 Number : 3;
            ULONG64 NOTHING : 1;
            ULONG64 Direction : 1;
            ULONG64 NOTHING : 3;
            ULONG64 GPReg : 4;
            ULONG64 NOTHING : 20;
        } DR;

        struct {
            ULONG64 Selector : 16;
            ULONG64 NOTHING : 14;
            ULONG64 Source : 2;
        }TS;

        struct {
            ULONG64 Offset : 12;
            ULONG64 Access : 3;
        } APIC;

        struct {
            ULONG64 Vector;
        } VAPIC_EOI;

        struct {
            ULONG64 Read : 1;
            ULONG64 Write : 1;
            ULONG64 Execute : 1;
            ULONG64 Readable : 1;
            ULONG64 Writeable : 1;
            ULONG64 Executable : 1;
            ULONG64 NOTHING : 1;
            ULONG64 gla1 : 1;
            ULONG64 gla2 : 1;
            ULONG64 NOTHING : 3;  /* bits 11:9 */
            ULONG64 NmiBlock : 1;
            ULONG64 NOTHING : 19;
            ULONG64 NOTHING : 32;
        } EPT;
    }QUALIFICATION, *PQUALIFICATION;

    typedef union _INTERRUPTION_INFO {
        ULONG64 Info;

        struct {
            ULONG64 Vector : 8;
            ULONG64 Type : 3;
            ULONG64 DeliverErrorCode : 1;
            ULONG64 NmiUnmasking : 1;
            ULONG64 NOTHING : 18;
            ULONG64 Valid : 1;
        };
    }INTERRUPTION_INFO, *PINTERRUPTION_INFO;

    typedef struct _GUEST_STATE {
        ULONG64 GuestRip;
        ULONG64 GuestRsp;
        ULONG64 GuestRFlags;
        ULONG64 InstructionLength;
        REASON Reason;
        QUALIFICATION Qualification;
    } GUEST_STATE, *PGUEST_STATE;

    typedef struct _CPUINFO {
        LONG Eax;
        LONG Ebx;
        LONG Ecx;
        LONG Edx;
    } CPUINFO, *PCPUINFO;

    enum {
        INTERRUPT = 0,
        NMI = 2,
        EXCEPTION = 3,
        SWINT = 4,
        PRIV_TRAP = 5,
        UNPRIV_TRAP = 6,
        OTHER = 7
    };

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

    VOID
        NTAPI
        __ops_xsetbv(
            __in ULONG Xcr,
            __in ULONG64 Value
        );

#ifdef __cplusplus
}
#endif	/* __cplusplus */

#endif // !_HANDLER_H_
