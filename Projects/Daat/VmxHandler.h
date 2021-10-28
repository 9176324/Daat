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

#ifndef _VMXHANDLER_H_
#define _VMXHANDLER_H_

#ifdef __cplusplus
/* Assume byte packing throughout */
extern "C" {
#endif	/* __cplusplus */

    typedef
        void
        (NTAPI * INTERRUPT_HANDLER) (
            void
            );

    typedef struct _INTERRUPTION_FRAME {
        u ProgramCounter;
        u SegCs;
        u Eflags;
    }INTERRUPTION_FRAME, *PINTERRUPTION_FRAME;

    typedef union _INTERRUPT_ADDRESS {
        struct {
#ifndef _WIN64
            u16 Offset;
            u16 ExtendedOffset;
#else
            u16 OffsetLow;
            u16 OffsetMiddle;
            u32 OffsetHigh;
#endif // !_WIN64
        };

        u Address;
    } INTERRUPT_ADDRESS, *PINTERRUPT_ADDRESS;

#ifdef _WIN64
    void
        NTAPI
        __monitor_patch_guard(
            __inout struct _TSB * Tsb
        );
#endif // _WIN64

    void
        NTAPI
        __vmx_print_interrupt(
            __inout struct _TSB * Tsb
        );

    typedef
        void
        (NTAPI * PVMHANDLER)(
            __inout struct _TSB * Block
            );

    void
        NTAPI
        __inject_exception(
            __inout struct _TSB * Tsb,
            __in u64 Vector,
            __in_opt u64 ErrorCode,
            __in u64 Type
        );

#ifdef __cplusplus
}
#endif	/* __cplusplus */

#endif // !_VMXHANDLER_H_
