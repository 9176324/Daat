/*
*
* Copyright (c) 2018 by blindtiger. All rights reserved.
*
* The contents of this file are subject to the Mozilla Public License Version
* 2.0 (the "License")); you may not use this file except in compliance with
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

#include <defs.h>

#include "vmx.h"

#include "vcpu.h"

VMX_RESULT
NTAPI
__vmx_vmwrite_common(
    __in SIZE_T Field,
    __in ULONG64 Value
)
{
    VMX_RESULT Result = VMX_SUCCEED;
    ULONG64 ErrorCode = 0;

    switch ((Field >> ENCODE_SHIFT) & ENCODE_MASK) {
    case ENCODE_16:
    case ENCODE_32: {
        Value &= 0x00000000FFFFFFFF;

        Result = __vmx_vmwrite(Field, Value);

        break;
    }

    case ENCODE_64: {
#ifndef _WIN64
        Result = __vmx_vmwrite(Field, Value);

        if (VMX_SUCCEED == Result) {
            Result = __vmx_vmwrite(Field + 1, Value >> 32);
        }
#else
        Result = __vmx_vmwrite(Field, Value);
#endif // !_WIN64

        break;
    }

    case ENCODE_NATURAL: {
        Result = __vmx_vmwrite(Field, Value);

        break;
    }

    default: {
        Result = VMX_FAIL_INVALID;

        break;
    }
    }

    return Result;
}

VMX_RESULT
NTAPI
__vmx_vmread_common(
    __in SIZE_T Field,
    __out ULONG64 * Value
)
{
    VMX_RESULT Result = VMX_SUCCEED;
    ULONG64 ErrorCode = 0;

    switch ((Field >> ENCODE_SHIFT) & ENCODE_MASK) {
    case ENCODE_16:
    case ENCODE_32: {
        Result = __vmx_vmread(Field, Value);

        break;
    }

    case ENCODE_64: {
#ifndef _WIN64
        Result = __vmx_vmread(Field, Value);

        if (VMX_SUCCEED == Result) {
            Result = __vmx_vmread(Field + 1, (PULONG)Value + 1);
        }
#else
        Result = __vmx_vmread(Field, Value);
#endif // !_WIN64

        break;
    }

    case ENCODE_NATURAL: {
        Result = __vmx_vmread(Field, Value);

        break;
    }

    default: {
        Result = VMX_FAIL_INVALID;

        break;
    }
    }

    return Result;
}

VOID
NTAPI
__vmx_prepare_segment(
    __in PKDESCRIPTOR Descriptor,
    __in UINT16 Selector,
    __out PSEGMENT_DESCRIPTOR SegmentDescriptor
)
{
#ifndef _WIN64
    PKGDTENTRY GdtEntry = NULL;
#else
    PKGDTENTRY64 GdtEntry = NULL;
#endif // !_WIN64

    RtlZeroMemory(SegmentDescriptor, sizeof(SEGMENT_DESCRIPTOR));

    if ((0 == Selector) ||
        0 != (Selector & SELECTOR_TABLE_INDEX)) {
        SegmentDescriptor->Selector = 0;
        SegmentDescriptor->Limit = 0;
        SegmentDescriptor->Base = 0;
        SegmentDescriptor->AccessRights = 0x10000;
    }

#ifndef _WIN64
    GdtEntry =
        (PKGDTENTRY)((ULONG_PTR)Descriptor->Base + (Selector & ~RPL_MASK));

    SegmentDescriptor->Selector = Selector;
    SegmentDescriptor->Limit = __ops_segment_limit(Selector);
    SegmentDescriptor->BaseLow = GdtEntry->BaseLow;
    SegmentDescriptor->BaseMiddle = GdtEntry->HighWord.Bits.BaseMid;
    SegmentDescriptor->BaseHigh = GdtEntry->HighWord.Bits.BaseHi;
    SegmentDescriptor->AccessRights = __ops_segment_ar(Selector);
#else
    GdtEntry =
        (PKGDTENTRY64)((ULONG_PTR)Descriptor->Base + (Selector & ~RPL_MASK));

    SegmentDescriptor->Selector = Selector;
    SegmentDescriptor->Limit = __ops_segment_limit(Selector);
    SegmentDescriptor->BaseLow = GdtEntry->BaseLow;
    SegmentDescriptor->BaseMiddle = GdtEntry->Bits.BaseMiddle;
    SegmentDescriptor->BaseHigh = GdtEntry->Bits.BaseHigh;

    SegmentDescriptor->BaseUpper =
        0 == (GdtEntry->Bits.Type & 0x10) ? GdtEntry->BaseUpper : 0;

    SegmentDescriptor->AccessRights = __ops_segment_ar(Selector);
#endif // !_WIN64
}

NTSTATUS
NTAPI
__vmx_entry(
    __in PREGISTERS_FRAME Registers
)
{
    NTSTATUS Status = STATUS_SUCCESS;
    VMX_RESULT Result = VMX_SUCCEED;
    ULONG Index = 0;
    ULONG64 ErrorCode = 0;
    SEGMENT_DESCRIPTOR SegmentDescriptor = { 0 };
    PCCB CurrentBlock = NULL;

    CaptureControlRegisters(Registers);
    CaptureSegmentRegisters(Registers);

    CurrentBlock = CONTAINING_RECORD(Registers, CCB, Registers);

    ReadCpuFeature(&CurrentBlock->Feature);

    if (0 == X86_FEATURE_VMX(&CurrentBlock->Feature)) {
        DbgPrint("[Sefirot] [Daat] vmx not supported\n");

        return STATUS_UNSUCCESSFUL;
    }

    if (FC_LOCKED == (__ops_readmsr(IA32_FEATURE_CONTROL) & FC_LOCKED)) {
        if (FC_VMXON_INSMX != (__ops_readmsr(IA32_FEATURE_CONTROL) & FC_VMXON_INSMX)) {
            DbgPrint("[Sefirot] [Daat] FC_VMXON_INSMX bit clear\n");
        }

        if (FC_VMXON_OUTSMX != (__ops_readmsr(IA32_FEATURE_CONTROL) & FC_VMXON_OUTSMX)) {
            DbgPrint("[Sefirot] [Daat] FC_VMXON_OUTSMX bit clear\n");
        }
    }
    else {
        DbgPrint("[Sefirot] [Daat] FC_LOCKED bit clear\n");

        return STATUS_UNSUCCESSFUL;
    }

    CurrentBlock->VmxInfo.BaseInfo = __ops_readmsr(IA32_VMX_BASIC);

    CurrentBlock->Region.VmsSupport.Identifier = CurrentBlock->VmxInfo.Identifier;
    CurrentBlock->Region.Vmcs.Identifier = CurrentBlock->VmxInfo.Identifier;

    CurrentBlock->VmsSupport = MmGetPhysicalAddress(&CurrentBlock->Region.VmsSupport);
    CurrentBlock->Vmcs = MmGetPhysicalAddress(&CurrentBlock->Region.Vmcs);
    CurrentBlock->Bitmap = MmGetPhysicalAddress(&CurrentBlock->Region.Bitmap);

    CurrentBlock->VmxInfo.Cr0.LowPart = CurrentBlock->Registers.Cr0;

    CurrentBlock->VmxInfo.Cr0Fixed.LowPart = __ops_readmsr(IA32_VMX_CR0_FIXED0);
    CurrentBlock->VmxInfo.Cr0Fixed.HighPart = __ops_readmsr(IA32_VMX_CR0_FIXED1);

    CurrentBlock->VmxInfo.Cr0.LowPart &= CurrentBlock->VmxInfo.Cr0Fixed.HighPart;;
    CurrentBlock->VmxInfo.Cr0.LowPart |= CurrentBlock->VmxInfo.Cr0Fixed.LowPart;

    DbgPrint(
        "[Sefirot] [Daat] cr0 < %p >\n",
        CurrentBlock->VmxInfo.Cr0.QuadPart);

    __ops_writecr0(CurrentBlock->VmxInfo.Cr0.QuadPart);

    CurrentBlock->VmxInfo.Cr0ReadShadow.LowPart = CurrentBlock->Registers.Cr0;

    DbgPrint(
        "[Sefirot] [Daat] cr0 read shadow < %p >\n",
        CurrentBlock->VmxInfo.Cr0ReadShadow.QuadPart);

    CurrentBlock->VmxInfo.Cr0Mask.LowPart = CR0_PE | CR0_NE | CR0_PG;

    DbgPrint(
        "[Sefirot] [Daat] cr0 mask < %p >\n",
        CurrentBlock->VmxInfo.Cr0Mask.QuadPart);

    CurrentBlock->VmxInfo.Cr4.LowPart = CurrentBlock->Registers.Cr4;

    CurrentBlock->VmxInfo.Cr4Fixed.LowPart = __ops_readmsr(IA32_VMX_CR4_FIXED0);
    CurrentBlock->VmxInfo.Cr4Fixed.HighPart = __ops_readmsr(IA32_VMX_CR4_FIXED1);

    CurrentBlock->VmxInfo.Cr4.LowPart &= CurrentBlock->VmxInfo.Cr4Fixed.HighPart;;
    CurrentBlock->VmxInfo.Cr4.LowPart |= CurrentBlock->VmxInfo.Cr4Fixed.LowPart;

    DbgPrint(
        "[Sefirot] [Daat] cr4 < %p >\n",
        CurrentBlock->VmxInfo.Cr4.QuadPart);

    __ops_writecr4(CurrentBlock->VmxInfo.Cr4.QuadPart);

    CurrentBlock->VmxInfo.Cr4ReadShadow.LowPart = CurrentBlock->Registers.Cr4;

    DbgPrint(
        "[Sefirot] [Daat] cr4 read shadow < %p >\n",
        CurrentBlock->VmxInfo.Cr4ReadShadow.QuadPart);

    CurrentBlock->VmxInfo.Cr4Mask.LowPart = CR4_VMXE;

    DbgPrint(
        "[Sefirot] [Daat] cr4 mask < %p >\n",
        CurrentBlock->VmxInfo.Cr4Mask.QuadPart);

    Result = __vmx_on(&CurrentBlock->VmsSupport);

    if (VMX_SUCCEED != Result) {
        DbgPrint("[Sefirot] [Daat] vmxon failed\n");

        return STATUS_UNSUCCESSFUL;
    }

    Result = __vmx_vmclear(&CurrentBlock->Vmcs);

    if (VMX_SUCCEED != Result) {
        DbgPrint("[Sefirot] [Daat] vmclear failed\n");

        __vmx_off();

        return STATUS_UNSUCCESSFUL;
    }

    Result = __vmx_vmptrld(&CurrentBlock->Vmcs);

    if (VMX_SUCCEED != Result) {
        DbgPrint("[Sefirot] [Daat] vmptrld failed\n");

        __vmx_off();

        return STATUS_UNSUCCESSFUL;
    }

    __vmx_prepare_segment(
        &CurrentBlock->Registers.Gdtr,
        CurrentBlock->Registers.SegEs,
        &SegmentDescriptor);

    DbgPrint(
        "[Sefirot] [Daat] segment < %02x | %p | %p | %04x >\n",
        SegmentDescriptor.Selector,
        SegmentDescriptor.Limit,
        SegmentDescriptor.Base,
        SegmentDescriptor.AccessRights);

    __vmx_vmwrite_common(GUEST_ES_SELECTOR, SegmentDescriptor.Selector);
    __vmx_vmwrite_common(GUEST_ES_BASE, SegmentDescriptor.Base);
    __vmx_vmwrite_common(GUEST_ES_LIMIT, SegmentDescriptor.Limit);
    __vmx_vmwrite_common(GUEST_ES_AR, SegmentDescriptor.AccessRights);
    __vmx_vmwrite_common(HOST_ES_SELECTOR, SegmentDescriptor.Selector & ~RPL_MASK);

    __vmx_prepare_segment(
        &CurrentBlock->Registers.Gdtr,
        CurrentBlock->Registers.SegCs,
        &SegmentDescriptor);

    DbgPrint(
        "[Sefirot] [Daat] segment < %02x | %p | %p | %04x >\n",
        SegmentDescriptor.Selector,
        SegmentDescriptor.Limit,
        SegmentDescriptor.Base,
        SegmentDescriptor.AccessRights);

    __vmx_vmwrite_common(GUEST_CS_SELECTOR, SegmentDescriptor.Selector);
    __vmx_vmwrite_common(GUEST_CS_BASE, SegmentDescriptor.Base);
    __vmx_vmwrite_common(GUEST_CS_LIMIT, SegmentDescriptor.Limit);
    __vmx_vmwrite_common(GUEST_CS_AR, SegmentDescriptor.AccessRights);
    __vmx_vmwrite_common(HOST_CS_SELECTOR, SegmentDescriptor.Selector & ~RPL_MASK);

    __vmx_prepare_segment(
        &CurrentBlock->Registers.Gdtr,
        CurrentBlock->Registers.SegSs,
        &SegmentDescriptor);

    DbgPrint(
        "[Sefirot] [Daat] segment < %02x | %p | %p | %04x >\n",
        SegmentDescriptor.Selector,
        SegmentDescriptor.Limit,
        SegmentDescriptor.Base,
        SegmentDescriptor.AccessRights);

    __vmx_vmwrite_common(GUEST_SS_SELECTOR, SegmentDescriptor.Selector);
    __vmx_vmwrite_common(GUEST_SS_BASE, SegmentDescriptor.Base);
    __vmx_vmwrite_common(GUEST_SS_LIMIT, SegmentDescriptor.Limit);
    __vmx_vmwrite_common(GUEST_SS_AR, SegmentDescriptor.AccessRights);
    __vmx_vmwrite_common(HOST_SS_SELECTOR, SegmentDescriptor.Selector & ~RPL_MASK);

    __vmx_prepare_segment(
        &CurrentBlock->Registers.Gdtr,
        CurrentBlock->Registers.SegDs,
        &SegmentDescriptor);

    DbgPrint(
        "[Sefirot] [Daat] segment < %02x | %p | %p | %04x >\n",
        SegmentDescriptor.Selector,
        SegmentDescriptor.Limit,
        SegmentDescriptor.Base,
        SegmentDescriptor.AccessRights);

    __vmx_vmwrite_common(GUEST_DS_SELECTOR, SegmentDescriptor.Selector);
    __vmx_vmwrite_common(GUEST_DS_BASE, SegmentDescriptor.Base);
    __vmx_vmwrite_common(GUEST_DS_LIMIT, SegmentDescriptor.Limit);
    __vmx_vmwrite_common(GUEST_DS_AR, SegmentDescriptor.AccessRights);
    __vmx_vmwrite_common(HOST_DS_SELECTOR, SegmentDescriptor.Selector & ~RPL_MASK);

    __vmx_prepare_segment(
        &CurrentBlock->Registers.Gdtr,
        CurrentBlock->Registers.SegFs,
        &SegmentDescriptor);

    SegmentDescriptor.Base = __ops_readmsr(IA32_FS_BASE);

    DbgPrint(
        "[Sefirot] [Daat] segment < %02x | %p | %p | %04x >\n",
        SegmentDescriptor.Selector,
        SegmentDescriptor.Limit,
        SegmentDescriptor.Base,
        SegmentDescriptor.AccessRights);

    __vmx_vmwrite_common(GUEST_FS_SELECTOR, SegmentDescriptor.Selector);
    __vmx_vmwrite_common(GUEST_FS_BASE, SegmentDescriptor.Base);
    __vmx_vmwrite_common(GUEST_FS_LIMIT, SegmentDescriptor.Limit);
    __vmx_vmwrite_common(GUEST_FS_AR, SegmentDescriptor.AccessRights);
    __vmx_vmwrite_common(HOST_FS_SELECTOR, SegmentDescriptor.Selector & ~RPL_MASK);
    __vmx_vmwrite_common(HOST_FS_BASE, SegmentDescriptor.Base);

    __vmx_prepare_segment(
        &CurrentBlock->Registers.Gdtr,
        CurrentBlock->Registers.SegGs,
        &SegmentDescriptor);

    SegmentDescriptor.Base = __ops_readmsr(IA32_GS_BASE);

    DbgPrint(
        "[Sefirot] [Daat] segment < %02x | %p | %p | %04x >\n",
        SegmentDescriptor.Selector,
        SegmentDescriptor.Limit,
        SegmentDescriptor.Base,
        SegmentDescriptor.AccessRights);

    __vmx_vmwrite_common(GUEST_GS_SELECTOR, SegmentDescriptor.Selector);
    __vmx_vmwrite_common(GUEST_GS_BASE, SegmentDescriptor.Base);
    __vmx_vmwrite_common(GUEST_GS_LIMIT, SegmentDescriptor.Limit);
    __vmx_vmwrite_common(GUEST_GS_AR, SegmentDescriptor.AccessRights);
    __vmx_vmwrite_common(HOST_GS_SELECTOR, SegmentDescriptor.Selector & ~RPL_MASK);
    __vmx_vmwrite_common(HOST_GS_BASE, SegmentDescriptor.Base);

    __vmx_prepare_segment(
        &CurrentBlock->Registers.Gdtr,
        CurrentBlock->Registers.Ldtr,
        &SegmentDescriptor);

    DbgPrint(
        "[Sefirot] [Daat] segment < %02x | %p | %p | %04x >\n",
        SegmentDescriptor.Selector,
        SegmentDescriptor.Limit,
        SegmentDescriptor.Base,
        SegmentDescriptor.AccessRights);

    __vmx_vmwrite_common(GUEST_LDTR_SELECTOR, SegmentDescriptor.Selector);
    __vmx_vmwrite_common(GUEST_LDTR_BASE, SegmentDescriptor.Base);
    __vmx_vmwrite_common(GUEST_LDTR_LIMIT, SegmentDescriptor.Limit);
    __vmx_vmwrite_common(GUEST_LDTR_AR, SegmentDescriptor.AccessRights);

    __vmx_prepare_segment(
        &CurrentBlock->Registers.Gdtr,
        CurrentBlock->Registers.Tr,
        &SegmentDescriptor);

    DbgPrint(
        "[Sefirot] [Daat] segment < %02x | %p | %p | %04x >\n",
        SegmentDescriptor.Selector,
        SegmentDescriptor.Limit,
        SegmentDescriptor.Base,
        SegmentDescriptor.AccessRights);

    __vmx_vmwrite_common(GUEST_TR_SELECTOR, SegmentDescriptor.Selector);
    __vmx_vmwrite_common(GUEST_TR_BASE, SegmentDescriptor.Base);
    __vmx_vmwrite_common(GUEST_TR_LIMIT, SegmentDescriptor.Limit);
    __vmx_vmwrite_common(GUEST_TR_AR, SegmentDescriptor.AccessRights);
    __vmx_vmwrite_common(HOST_TR_SELECTOR, SegmentDescriptor.Selector & ~RPL_MASK);
    __vmx_vmwrite_common(HOST_TR_BASE, SegmentDescriptor.Base);

    DbgPrint(
        "[Sefirot] [Daat] gdtr < %04x | %p >\n",
        CurrentBlock->Registers.Gdtr.Limit,
        CurrentBlock->Registers.Gdtr.Base);

    __vmx_vmwrite_common(GUEST_GDTR_LIMIT, CurrentBlock->Registers.Gdtr.Limit);
    __vmx_vmwrite_common(GUEST_GDTR_BASE, (ULONG64)CurrentBlock->Registers.Gdtr.Base);
    __vmx_vmwrite_common(HOST_GDTR_BASE, (ULONG64)CurrentBlock->Registers.Gdtr.Base);

    DbgPrint(
        "[Sefirot] [Daat] idtr < %04x | %p >\n",
        CurrentBlock->Registers.Idtr.Limit,
        CurrentBlock->Registers.Idtr.Base);

    __vmx_vmwrite_common(GUEST_IDTR_LIMIT, CurrentBlock->Registers.Idtr.Limit);
    __vmx_vmwrite_common(GUEST_IDTR_BASE, (ULONG64)CurrentBlock->Registers.Idtr.Base);
    __vmx_vmwrite_common(HOST_IDTR_BASE, (ULONG64)CurrentBlock->Registers.Idtr.Base);

    __vmx_vmwrite_common(GUEST_CR0, CurrentBlock->VmxInfo.Cr0.QuadPart);
    __vmx_vmwrite_common(GUEST_CR3, CurrentBlock->Registers.Cr3);
    __vmx_vmwrite_common(GUEST_CR4, CurrentBlock->VmxInfo.Cr4.QuadPart);

    __vmx_vmwrite_common(GUEST_DR7, CurrentBlock->Registers.Dr7);
    __vmx_vmwrite_common(GUEST_RFLAGS, CurrentBlock->Registers.EFlags);

    __vmx_vmwrite_common(GUEST_RSP, (ULONG64)CurrentBlock->Registers.StackPointer);
    __vmx_vmwrite_common(GUEST_RIP, (ULONG64)CurrentBlock->Registers.ProgramCounter);

    __vmx_vmwrite_common(GUEST_DEBUGCTL, __ops_readmsr(IA32_DEBUGCTL));
    __vmx_vmwrite_common(GUEST_SYSENTER_CS, __ops_readmsr(IA32_SYSENTER_CS));
    __vmx_vmwrite_common(GUEST_SYSENTER_ESP, __ops_readmsr(IA32_SYSENTER_ESP));
    __vmx_vmwrite_common(GUEST_SYSENTER_EIP, __ops_readmsr(IA32_SYSENTER_EIP));

    __vmx_vmwrite_common(GUEST_INTERRUPTIBILITY, 0);
    __vmx_vmwrite_common(GUEST_ACTIVITY_STATE, 0);
    __vmx_vmwrite_common(GUEST_VMCS_LINK_PTR, VMCS_NONE);

    __vmx_vmwrite_common(HOST_CR0, CurrentBlock->VmxInfo.Cr0.QuadPart);
    __vmx_vmwrite_common(HOST_CR3, CurrentBlock->Registers.Cr3);
    __vmx_vmwrite_common(HOST_CR4, CurrentBlock->VmxInfo.Cr4.QuadPart);

    __vmx_vmwrite_common(HOST_RSP, (ULONG64)&CurrentBlock->Registers);
    __vmx_vmwrite_common(HOST_RIP, (ULONG64)__vmx_vmentry);

    __vmx_vmwrite_common(HOST_SYSENTER_CS, __ops_readmsr(IA32_SYSENTER_CS));
    __vmx_vmwrite_common(HOST_SYSENTER_ESP, __ops_readmsr(IA32_SYSENTER_ESP));
    __vmx_vmwrite_common(HOST_SYSENTER_EIP, __ops_readmsr(IA32_SYSENTER_EIP));

    __vmx_vmwrite_common(VMX_CR0_MASK, CurrentBlock->VmxInfo.Cr0Mask.QuadPart);
    __vmx_vmwrite_common(VMX_CR0_READ_SHADOW, CurrentBlock->VmxInfo.Cr0ReadShadow.QuadPart);

    __vmx_vmwrite_common(VMX_CR4_MASK, CurrentBlock->VmxInfo.Cr4Mask.QuadPart);
    __vmx_vmwrite_common(VMX_CR4_READ_SHADOW, CurrentBlock->VmxInfo.Cr4ReadShadow.QuadPart);

    __vmx_vmwrite_common(VMX_MSR_BITMAP, CurrentBlock->Bitmap.QuadPart);
    __vmx_vmwrite_common(VMX_TSC_OFFSET, 0);

    if (CurrentBlock->VmxInfo.BaseInfo & (1ULL << 55)) {
        CurrentBlock->VmxInfo.PinFixed.QuadPart = __ops_readmsr(IA32_VMX_TRUE_PINBASED_CTLS);
        CurrentBlock->VmxInfo.PrimaryFixed.QuadPart = __ops_readmsr(IA32_VMX_TRUE_PROCBASED_CTLS);
        CurrentBlock->VmxInfo.ExitFixed.QuadPart = __ops_readmsr(IA32_VMX_TRUE_EXIT_CTLS);
        CurrentBlock->VmxInfo.EntryFixed.QuadPart = __ops_readmsr(IA32_VMX_TRUE_ENTRY_CTLS);

        CurrentBlock->VmxInfo.PinFixed.LowPart |=
            __ops_readmsr(IA32_VMX_PINBASED_CTLS) & ~PIN_CONTROLS_DEFINED;

        CurrentBlock->VmxInfo.PrimaryFixed.LowPart |=
            __ops_readmsr(IA32_VMX_PROCBASED_CTLS) & ~PRIMARY_CONTROLS_DEFINED;

        CurrentBlock->VmxInfo.ExitFixed.LowPart |=
            __ops_readmsr(IA32_VMX_EXIT_CTLS) & ~EXIT_CONTROLS_DEFINED;

        CurrentBlock->VmxInfo.EntryFixed.LowPart |=
            __ops_readmsr(IA32_VMX_ENTRY_CTLS) & ~ENTRY_CONTROLS_DEFINED;
    }
    else {
        CurrentBlock->VmxInfo.PinFixed.QuadPart = __ops_readmsr(IA32_VMX_PINBASED_CTLS);
        CurrentBlock->VmxInfo.PrimaryFixed.QuadPart = __ops_readmsr(IA32_VMX_PROCBASED_CTLS);
        CurrentBlock->VmxInfo.ExitFixed.QuadPart = __ops_readmsr(IA32_VMX_EXIT_CTLS);
        CurrentBlock->VmxInfo.EntryFixed.QuadPart = __ops_readmsr(IA32_VMX_ENTRY_CTLS);
    }

    DbgPrint(
        "[Sefirot] [Daat] pin fixed control < %p >\n",
        CurrentBlock->VmxInfo.PinFixed.QuadPart);

    DbgPrint(
        "[Sefirot] [Daat] primary fixed control < %p >\n",
        CurrentBlock->VmxInfo.PrimaryFixed.QuadPart);

    DbgPrint(
        "[Sefirot] [Daat] exit fixed control < %p >\n",
        CurrentBlock->VmxInfo.ExitFixed.QuadPart);

    DbgPrint(
        "[Sefirot] [Daat] entry fixed control < %p >\n",
        CurrentBlock->VmxInfo.EntryFixed.QuadPart);

    if (SECONDARY_CONTROLS ==
        (CurrentBlock->VmxInfo.PrimaryFixed.HighPart & SECONDARY_CONTROLS)) {
        CurrentBlock->VmxInfo.SecondaryFixed.QuadPart = __ops_readmsr(IA32_VMX_SECONDARY_CTLS);

        DbgPrint(
            "[Sefirot] [Daat] secondary fixed control < %p >\n",
            CurrentBlock->VmxInfo.SecondaryFixed.QuadPart);
    }

    CurrentBlock->VmxInfo.Pin.LowPart |= 0;
    CurrentBlock->VmxInfo.Pin.LowPart &= CurrentBlock->VmxInfo.PinFixed.HighPart;
    CurrentBlock->VmxInfo.Pin.LowPart |= CurrentBlock->VmxInfo.PinFixed.LowPart;

    __vmx_vmwrite_common(VMX_PIN_CONTROLS, CurrentBlock->VmxInfo.Pin.QuadPart);

    DbgPrint(
        "[Sefirot] [Daat] pin control < %p >\n",
        CurrentBlock->VmxInfo.Pin.QuadPart);

    if (0 != X86_FEATURE_RDTSCP(&CurrentBlock->Feature)) {
        // TODO: Check VMX capabilities to ensure ENABLE_RDTSCP is available
        CurrentBlock->VmxInfo.Secondary.LowPart |= ENABLE_RDTSCP;
    }

    CurrentBlock->VmxInfo.Secondary.LowPart &= CurrentBlock->VmxInfo.SecondaryFixed.HighPart;
    CurrentBlock->VmxInfo.Secondary.LowPart |= CurrentBlock->VmxInfo.SecondaryFixed.LowPart;

    __vmx_vmwrite_common(VMX_SECONDARY_PROCESSOR_CONTROLS, CurrentBlock->VmxInfo.Secondary.QuadPart);

    DbgPrint(
        "[Sefirot] [Daat] secondary control  < %p >\n",
        CurrentBlock->VmxInfo.Secondary.QuadPart);

    CurrentBlock->VmxInfo.Primary.LowPart |= MSR_BITMAP_ACTIVE | SECONDARY_CONTROLS;

    CurrentBlock->VmxInfo.Primary.LowPart &= CurrentBlock->VmxInfo.PrimaryFixed.HighPart;
    CurrentBlock->VmxInfo.Primary.LowPart |= CurrentBlock->VmxInfo.PrimaryFixed.LowPart;

    __vmx_vmwrite_common(VMX_PRIMARY_PROCESSOR_CONTROLS, CurrentBlock->VmxInfo.Primary.QuadPart);

    DbgPrint(
        "[Sefirot] [Daat] primary control < %p >\n",
        CurrentBlock->VmxInfo.Primary.QuadPart);

#ifndef _WIN64
    CurrentBlock->VmxInfo.Exit.LowPart |= 0;
    CurrentBlock->VmxInfo.Exit.LowPart &= CurrentBlock->VmxInfo.ExitFixed.HighPart;
    CurrentBlock->VmxInfo.Exit.LowPart |= CurrentBlock->VmxInfo.ExitFixed.LowPart;

    __vmx_vmwrite_common(VMX_EXIT_CONTROLS, CurrentBlock->VmxInfo.Exit.QuadPart);

    DbgPrint(
        "[Sefirot] [Daat] exit control < %p >\n",
        CurrentBlock->VmxInfo.Exit.QuadPart);

    CurrentBlock->VmxInfo.Entry.LowPart |= 0;
    CurrentBlock->VmxInfo.Entry.LowPart &= CurrentBlock->VmxInfo.EntryFixed.HighPart;
    CurrentBlock->VmxInfo.Entry.LowPart |= CurrentBlock->VmxInfo.EntryFixed.LowPart;

    __vmx_vmwrite_common(VMX_ENTRY_CONTROLS, CurrentBlock->VmxInfo.Entry.QuadPart);

    DbgPrint(
        "[Sefirot] [Daat] entry control < %p >\n",
        CurrentBlock->VmxInfo.Entry.QuadPart);
#else
    CurrentBlock->VmxInfo.Exit.LowPart |= EXIT_CONTROL_HOST_ADDR_SPACE_SIZE;
    CurrentBlock->VmxInfo.Exit.LowPart &= CurrentBlock->VmxInfo.ExitFixed.HighPart;
    CurrentBlock->VmxInfo.Exit.LowPart |= CurrentBlock->VmxInfo.ExitFixed.LowPart;

    __vmx_vmwrite_common(VMX_EXIT_CONTROLS, CurrentBlock->VmxInfo.Exit.QuadPart);

    DbgPrint(
        "[Sefirot] [Daat] exit control < %p >\n",
        CurrentBlock->VmxInfo.Exit.QuadPart);

    CurrentBlock->VmxInfo.Entry.LowPart |= ENTRY_CONTROL_LONG_MODE_GUEST;
    CurrentBlock->VmxInfo.Entry.LowPart &= CurrentBlock->VmxInfo.EntryFixed.HighPart;
    CurrentBlock->VmxInfo.Entry.LowPart |= CurrentBlock->VmxInfo.EntryFixed.LowPart;

    __vmx_vmwrite_common(VMX_ENTRY_CONTROLS, CurrentBlock->VmxInfo.Entry.QuadPart);

    DbgPrint(
        "[Sefirot] [Daat] entry control < %p >\n",
        CurrentBlock->VmxInfo.Entry.QuadPart);
#endif // !_WIN64

    Result = __vmx_vmlaunch();

    if (VMX_SUCCEED != Result) {
        if (VMX_FAIL_INVALID == Result) {
            __vmx_vmread_common(VMX_INSTRUCTION_ERROR_CODE, &ErrorCode);
        }

        DbgPrint(
            "[Sefirot] [Daat] vmlaunch failed < %p >\n",
            ErrorCode);

        __vmx_off();

        return STATUS_UNSUCCESSFUL;
}

    return Status;
}
