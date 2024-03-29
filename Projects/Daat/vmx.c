/*
*
* Copyright (c) 2015 - 2021 by blindtiger. All rights reserved.
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
* The Initial Developer of the Original Code is blindtiger.
*
*/

#include <defs.h>

#include "Vmx.h"     
 
#include "Intrin.h"    
#include "VmxHandler.h" 

CCHAR FindFirstSetRight[256] = {
    0, 0, 1, 0, 2, 0, 1, 0, 3, 0, 1, 0, 2, 0, 1, 0,
    4, 0, 1, 0, 2, 0, 1, 0, 3, 0, 1, 0, 2, 0, 1, 0,
    5, 0, 1, 0, 2, 0, 1, 0, 3, 0, 1, 0, 2, 0, 1, 0,
    4, 0, 1, 0, 2, 0, 1, 0, 3, 0, 1, 0, 2, 0, 1, 0,
    6, 0, 1, 0, 2, 0, 1, 0, 3, 0, 1, 0, 2, 0, 1, 0,
    4, 0, 1, 0, 2, 0, 1, 0, 3, 0, 1, 0, 2, 0, 1, 0,
    5, 0, 1, 0, 2, 0, 1, 0, 3, 0, 1, 0, 2, 0, 1, 0,
    4, 0, 1, 0, 2, 0, 1, 0, 3, 0, 1, 0, 2, 0, 1, 0,
    7, 0, 1, 0, 2, 0, 1, 0, 3, 0, 1, 0, 2, 0, 1, 0,
    4, 0, 1, 0, 2, 0, 1, 0, 3, 0, 1, 0, 2, 0, 1, 0,
    5, 0, 1, 0, 2, 0, 1, 0, 3, 0, 1, 0, 2, 0, 1, 0,
    4, 0, 1, 0, 2, 0, 1, 0, 3, 0, 1, 0, 2, 0, 1, 0,
    6, 0, 1, 0, 2, 0, 1, 0, 3, 0, 1, 0, 2, 0, 1, 0,
    4, 0, 1, 0, 2, 0, 1, 0, 3, 0, 1, 0, 2, 0, 1, 0,
    5, 0, 1, 0, 2, 0, 1, 0, 3, 0, 1, 0, 2, 0, 1, 0,
    4, 0, 1, 0, 2, 0, 1, 0, 3, 0, 1, 0, 2, 0, 1, 0
};

s64 MtrrMask = 0xffffffull << PAGE_SHIFT;
u8 MtrrMaxRangeShift = 36;

u32
NTAPI
FindFirstSetRightBit(
    __in u64 Set
)
{
    u32 BitNumber = 0;

    for (BitNumber = 0;
        !(Set & 0xff);
        BitNumber += 8, Set >>= 8);

    return FindFirstSetRight[Set & 0xff] + BitNumber;
}

u64
NTAPI
MaskToLength(
    __in u64 Mask
)
{
    if (Mask == 0) {
        // Zero Mask signifies a length of 2**36 or 2**40
        return(((u64)1 << MtrrMaxRangeShift));
    }
    else {
        return(((u64)1 << FindFirstSetRightBit(Mask)));
    }
}

status
NTAPI
__vmx_prepare_mtrr(
    __inout struct _CSB * Csb
)
{
    status Status = STATUS_SUCCESS;
    u Index = 0;
    u Length = 0;
    u Base = 0;
    u Mask = 0;
    u ReginSize = 0;
    MTRR_DEFAULT Default = { 0 };
    MTRR_CAPABILITIES Capabilities = { 0 };
    MTRR_RANGE MtrrRange = { 0 };

    Capabilities.Alignment = __ins_readmsr(IA32_MTRRCAP);
    Default.Alignment = __ins_readmsr(IA32_MTRR_DEF_TYPE);

    //
    // If h/w mtrr support is not enabled, disable OS support
    //

    if (0 == Default.MtrrEnabled ||
        0 == Capabilities.VarCnt ||
        MTRR_TYPE_UNCACHEABLE != Default.Type) {
        Status = STATUS_NOT_SUPPORTED;
    }
    else {
        ReginSize =
            sizeof(ONE_RANGE) * Capabilities.VarCnt
            + FIELD_OFFSET(RANGE_INFO, Ranges);

        Csb->RangeInfo = __malloc(ReginSize);

        if (NULL != Csb->RangeInfo) {
            RtlZeroMemory(Csb->RangeInfo, ReginSize);

            Csb->RangeInfo->Capabilities.Alignment = Capabilities.Alignment;
            Csb->RangeInfo->Default.Alignment = Default.Alignment;
            Csb->RangeInfo->DefaultCachedType = MTRR_TYPE_MAX;

            //
            // Read current MTRR settings for various cached range types
            // and add them to the range database
            //

            for (Index = 0;
                Index < Capabilities.VarCnt;
                Index++) {
                MtrrRange.Base.Alignment = __ins_readmsr(IA32_MTRR_PHYSBASE0 + Index * 2);
                MtrrRange.Mask.Alignment = __ins_readmsr(IA32_MTRR_PHYSMASK0 + Index * 2);

                Base = MtrrRange.Base.Alignment & MtrrMask;
                Mask = MtrrRange.Mask.Alignment & MtrrMask;

                if (0 != MtrrRange.Mask.Valid) {
                    Csb->RangeInfo->MaxRange++;

                    Length = MaskToLength(Mask);

                    //
                    // Add this MTRR to the range database
                    //

                    Base &= Mask;

                    Csb->RangeInfo->Ranges[Index].Base = Base;
                    Csb->RangeInfo->Ranges[Index].Limit = Base + Length - 1;
                    Csb->RangeInfo->Ranges[Index].Type = MtrrRange.Base.Type;

                    //
                    // Check for default cache type
                    //

                    if (MtrrRange.Base.Type == MTRR_TYPE_WRITE_BACK) {
                        Csb->RangeInfo->DefaultCachedType = MTRR_TYPE_WRITE_BACK;
                    }

                    if (Csb->RangeInfo->DefaultCachedType == MTRR_TYPE_MAX  &&
                        MtrrRange.Base.Type == MTRR_TYPE_WRITE_THROUGH) {
                        Csb->RangeInfo->DefaultCachedType = MTRR_TYPE_WRITE_THROUGH;
                    }
                }
            }

            //
            // If a default type for "cached" was not found, assume write-back
            //

            if (Csb->RangeInfo->DefaultCachedType == MTRR_TYPE_MAX) {
                Csb->RangeInfo->DefaultCachedType = MTRR_TYPE_WRITE_BACK;
            }
        }
    }

    return Status;
}

VMX_RESULT
NTAPI
__vmx_vmwrite_common(
    __in u Field,
    __in u64 Value
)
{
    VMX_RESULT Result = VMX_SUCCEED;
    u64 ErrorCode = 0;

    switch ((Field >> ENCODE_SHIFT) & ENCODE_MASK) {
    case ENCODE_16:
    case ENCODE_32: {
        Value &= 0x00000000FFFFFFFF;

        Result = __ins_vmx_vmwrite(Field, Value);

        break;
    }

    case ENCODE_64: {
#ifndef _WIN64
        Result = __ins_vmx_vmwrite(Field, Value);

        if (VMX_SUCCEED == Result) {
            Result = __ins_vmx_vmwrite(Field + 1, Value >> 32);
        }
#else
        Result = __ins_vmx_vmwrite(Field, Value);
#endif // !_WIN64

        break;
    }

    case ENCODE_NATURAL: {
        Result = __ins_vmx_vmwrite(Field, Value);

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
    __in u Field,
    __out u64 * Value
)
{
    VMX_RESULT Result = VMX_SUCCEED;
    u64 ErrorCode = 0;

    switch ((Field >> ENCODE_SHIFT) & ENCODE_MASK) {
    case ENCODE_16:
    case ENCODE_32: {
        Result = __ins_vmx_vmread(Field, Value);

        break;
    }

    case ENCODE_64: {
#ifndef _WIN64
        Result = __ins_vmx_vmread(Field, Value);

        if (VMX_SUCCEED == Result) {
            Result = __ins_vmx_vmread(Field + 1, (u32ptr)Value + 1);
        }
#else
        Result = __ins_vmx_vmread(Field, Value);
#endif // !_WIN64

        break;
    }

    case ENCODE_NATURAL: {
        Result = __ins_vmx_vmread(Field, Value);

        break;
    }

    default: {
        Result = VMX_FAIL_INVALID;

        break;
    }
    }

    return Result;
}

u32
NTAPI
__vmx_ajust_memory_type(
    __in struct _CSB * Csb,
    __in u64 Address,
    __in u32 Type
)
{
    u32 Index = 0;

    for (Index = 0;
        Index < Csb->RangeInfo->Capabilities.VarCnt;
        Index++) {
        if (((Address >= Csb->RangeInfo->Ranges[Index].Base) &&
            (Address + (_2MB - 1)) <= Csb->RangeInfo->Ranges[Index].Limit)) {
            Type = Csb->RangeInfo->Ranges[Index].Type;
        }
    }

    return Type;
}

void
NTAPI
__vmx_prepare_ept(
    __inout struct _CSB * Csb
)
{
    u Index = 0;
    u SubIndex = 0;
    EPT_PML4E Pml4e = { 0 };
    EPT_PDPTE Pdpte = { 0 };
    EPT_PDE Pde = { 0 };

    Pde.Read = 1;
    Pde.Write = 1;
    Pde.Execute = 1;
    Pde.Large = 1;

    for (Index = 0;
        Index < PER_PAGE * PER_PAGE;
        Index++) {
        Pde.PageFrameNumber = Index * _2MB;

        Pde.Type = __vmx_ajust_memory_type(
            Csb,
            Pdpte.PageFrameNumber,
            MTRR_TYPE_WRITE_BACK);

        Csb->Region.Pde[Index] = Pde.Alignment;
    }

    Pdpte.Read = 1;
    Pdpte.Write = 1;
    Pdpte.Execute = 1;

    for (Index = 0;
        Index < PER_PAGE;
        Index++) {
        Pdpte.PageFrameNumber =
            MmGetPhysicalAddress(Csb->Region.Pde + Index * PER_PAGE).QuadPart >> PAGE_SHIFT;

        Csb->Region.Pdpte[Index] = Pdpte.Alignment;
    }

    Pml4e.Read = 1;
    Pml4e.Write = 1;
    Pml4e.Execute = 1;

    Pml4e.PageFrameNumber =
        MmGetPhysicalAddress(Csb->Region.Pdpte).QuadPart >> PAGE_SHIFT;

    Csb->Region.Pml4e[0] = Pml4e.Alignment;
}

status
NTAPI
__vmx_prepare_csb(
    __inout struct _CSB * Csb
)
{
    status Status = STATUS_SUCCESS;

    ReadCpuFeature(&Csb->CpuFeature);

    if (0 != FEATURE_VMX(&Csb->CpuFeature)) {
        if (FC_LOCKED == (__ins_readmsr(IA32_FEATURE_CONTROL) & FC_LOCKED)) {
            if (FC_VMXON_INSMX != (__ins_readmsr(IA32_FEATURE_CONTROL) & FC_VMXON_INSMX)) {
                vDbgPrint("[Daat] FC_VMXON_INSMX bit clear\n");
            }

            if (FC_VMXON_OUTSMX != (__ins_readmsr(IA32_FEATURE_CONTROL) & FC_VMXON_OUTSMX)) {
                vDbgPrint("[Daat] FC_VMXON_OUTSMX bit clear\n");
            }

            Csb->Controls.BaseInfo = __ins_readmsr(IA32_VMX_BASIC);

            Csb->MsrBitmap = MmGetPhysicalAddress(Csb->Region.MsrBitmap);
            Csb->EptPml4e = MmGetPhysicalAddress(Csb->Region.Pml4e);

            EnableMsr(Csb->Region.MsrBitmap, IA32_DEBUGCTL);
            EnableMsr(Csb->Region.MsrBitmap, IA32_SYSENTER_CS);
            EnableMsr(Csb->Region.MsrBitmap, IA32_SYSENTER_ESP);
            EnableMsr(Csb->Region.MsrBitmap, IA32_SYSENTER_EIP);
            EnableMsr(Csb->Region.MsrBitmap, IA32_FS_BASE);
            EnableMsr(Csb->Region.MsrBitmap, IA32_GS_BASE);
            EnableMsr(Csb->Region.MsrBitmap, IA32_FEATURE_CONTROL);

            // Csb->ExceptionBitmap.LowPart |= 1UL << VECTOR_NMI;
            // Csb->ExceptionBitmap.LowPart |= 1UL << VECTOR_DB;
            // Csb->ExceptionBitmap.LowPart |= 1UL << VECTOR_BP;

            if (Csb->Controls.BaseInfo & (1ULL << 55)) {
                Csb->Controls.PinFixed.QuadPart = __ins_readmsr(IA32_VMX_TRUE_PINBASED_CTLS);
                Csb->Controls.PrimaryFixed.QuadPart = __ins_readmsr(IA32_VMX_TRUE_PROCBASED_CTLS);
                Csb->Controls.ExitFixed.QuadPart = __ins_readmsr(IA32_VMX_TRUE_EXIT_CTLS);
                Csb->Controls.EntryFixed.QuadPart = __ins_readmsr(IA32_VMX_TRUE_ENTRY_CTLS);

                Csb->Controls.PinFixed.LowPart |=
                    __ins_readmsr(IA32_VMX_PINBASED_CTLS) & ~PIN_CONTROLS_DEFINED;

                Csb->Controls.PrimaryFixed.LowPart |=
                    __ins_readmsr(IA32_VMX_PROCBASED_CTLS) & ~PRIMARY_CONTROLS_DEFINED;

                Csb->Controls.ExitFixed.LowPart |=
                    __ins_readmsr(IA32_VMX_EXIT_CTLS) & ~EXIT_CONTROLS_DEFINED;

                Csb->Controls.EntryFixed.LowPart |=
                    __ins_readmsr(IA32_VMX_ENTRY_CTLS) & ~ENTRY_CONTROLS_DEFINED;
            }
            else {
                Csb->Controls.PinFixed.QuadPart = __ins_readmsr(IA32_VMX_PINBASED_CTLS);
                Csb->Controls.PrimaryFixed.QuadPart = __ins_readmsr(IA32_VMX_PROCBASED_CTLS);
                Csb->Controls.ExitFixed.QuadPart = __ins_readmsr(IA32_VMX_EXIT_CTLS);
                Csb->Controls.EntryFixed.QuadPart = __ins_readmsr(IA32_VMX_ENTRY_CTLS);
            }

            if (SECONDARY_CONTROLS ==
                (Csb->Controls.PrimaryFixed.HighPart & SECONDARY_CONTROLS)) {
                Csb->Controls.SecondaryFixed.QuadPart = __ins_readmsr(IA32_VMX_SECONDARY_CTLS);

                vDbgPrint(
                    "[Daat] secondary fixed control < %08x | %08x >\n",
                    Csb->Controls.SecondaryFixed.HighPart,
                    Csb->Controls.SecondaryFixed.LowPart);
            }

            Csb->Controls.Pin.LowPart &= Csb->Controls.PinFixed.HighPart;
            Csb->Controls.Pin.LowPart |= Csb->Controls.PinFixed.LowPart;

            vDbgPrint(
                "[Daat] pin control < %08x | %08x >\n",
                Csb->Controls.Pin.HighPart,
                Csb->Controls.Pin.LowPart);

            if (0 != FEATURE_RDTSCP(&Csb->CpuFeature)) {
                Csb->Controls.Secondary.LowPart |= ENABLE_RDTSCP;
            }

            Csb->Controls.EptVpidCapabilities = __ins_readmsr(IA32_VMX_EPT_VPID_CAP);

            if (((Csb->Controls.EptVpidCapabilities & EPT_CAP_GAW48) != 0) &&
                ((Csb->Controls.EptVpidCapabilities & EPT_CAP_WB) != 0) &&
                ((Csb->Controls.EptVpidCapabilities & EPT_CAP_SP2M) != 0)) {
                Status = __vmx_prepare_mtrr(Csb);

                if (TRACE(Status)) {
                    __vmx_prepare_ept(Csb);

                    Csb->Controls.Secondary.LowPart |= ENABLE_EPT | ENABLE_VPID;
                }
            }

            Csb->Controls.Secondary.LowPart &= Csb->Controls.SecondaryFixed.HighPart;
            Csb->Controls.Secondary.LowPart |= Csb->Controls.SecondaryFixed.LowPart;

            vDbgPrint(
                "[Daat] secondary control < %08x | %08x >\n",
                Csb->Controls.Secondary.HighPart,
                Csb->Controls.Secondary.LowPart);

            Csb->Controls.Primary.LowPart |= DR_EXITING;
            Csb->Controls.Primary.LowPart |= MSR_BITMAP_ACTIVE;
            Csb->Controls.Primary.LowPart |= SECONDARY_CONTROLS;

            Csb->Controls.Primary.LowPart &= Csb->Controls.PrimaryFixed.HighPart;
            Csb->Controls.Primary.LowPart |= Csb->Controls.PrimaryFixed.LowPart;

            vDbgPrint(
                "[Daat] primary control < %08x | %08x >\n",
                Csb->Controls.Primary.HighPart,
                Csb->Controls.Primary.LowPart);

            Csb->Controls.Exit.LowPart |= ENTRY_CONTROL_LOAD_DEBUG_CONTROLS;

#ifdef _WIN64
            Csb->Controls.Exit.LowPart |= EXIT_CONTROL_HOST_ADDR_SPACE_SIZE;
#endif // _WIN64

            Csb->Controls.Exit.LowPart &= Csb->Controls.ExitFixed.HighPart;
            Csb->Controls.Exit.LowPart |= Csb->Controls.ExitFixed.LowPart;

            vDbgPrint(
                "[Daat] exit control < %08x | %08x >\n",
                Csb->Controls.Exit.HighPart,
                Csb->Controls.Exit.LowPart);

            Csb->Controls.Entry.LowPart |= EXIT_CONTROL_SAVE_DEBUG_CONTROLS;

#ifdef _WIN64
            Csb->Controls.Entry.LowPart |= ENTRY_CONTROL_LONG_MODE_GUEST;
#endif // _WIN64

            Csb->Controls.Entry.LowPart &= Csb->Controls.EntryFixed.HighPart;
            Csb->Controls.Entry.LowPart |= Csb->Controls.EntryFixed.LowPart;

            vDbgPrint(
                "[Daat] entry control < %08x | %08x >\n",
                Csb->Controls.Entry.HighPart,
                Csb->Controls.Entry.LowPart);
        }
        else {
            vDbgPrint("[Daat] FC_LOCKED bit clear\n");

            Status = STATUS_UNSUCCESSFUL;
        }
    }
    else {
        vDbgPrint("[Daat] vmx not supported\n");

        Status = STATUS_UNSUCCESSFUL;
    }

    return Status;
}

VMX_RESULT
NTAPI
__vmx_prepare_vmcs(
    __in PTSB Tsb
)
{
    VMX_RESULT Result = VMX_SUCCEED;

    Tsb->Region.Support.Identifier = Tsb->Csb->Controls.Identifier;
    Tsb->Region.Vmcs.Identifier = Tsb->Csb->Controls.Identifier;

    Tsb->Support = MmGetPhysicalAddress(&Tsb->Region.Support);
    Tsb->Vmcs = MmGetPhysicalAddress(&Tsb->Region.Vmcs);

    Tsb->Cr0ReadShadow.LowPart =
        Tsb->Cr0.QuadPart =
        Tsb->Registers.Cr0 = __ins_readcr(0);

    Tsb->Cr0Fixed.LowPart = __ins_readmsr(IA32_VMX_CR0_FIXED0);
    Tsb->Cr0Fixed.HighPart = __ins_readmsr(IA32_VMX_CR0_FIXED1);

    Tsb->Cr0.LowPart &= Tsb->Cr0Fixed.HighPart;;
    Tsb->Cr0.LowPart |= Tsb->Cr0Fixed.LowPart;

    vDbgPrint(
        "[Daat] cr0 < %016x >\n",
        Tsb->Cr0.QuadPart);

    __ins_writecr(0, Tsb->Cr0.QuadPart);

    vDbgPrint(
        "[Daat] cr0 read shadow < %016x >\n",
        Tsb->Cr0ReadShadow.QuadPart);

    Tsb->Cr0Mask.LowPart = CR0_PE | CR0_NE | CR0_PG;

    vDbgPrint(
        "[Daat] cr0 mask < %016x >\n",
        Tsb->Cr0Mask.QuadPart);

    Tsb->Cr4ReadShadow.QuadPart =
        Tsb->Cr4.QuadPart =
        Tsb->Registers.Cr4 = __ins_readcr(4);

    Tsb->Cr4Fixed.LowPart = __ins_readmsr(IA32_VMX_CR4_FIXED0);
    Tsb->Cr4Fixed.HighPart = __ins_readmsr(IA32_VMX_CR4_FIXED1);

    Tsb->Cr4.LowPart &= Tsb->Cr4Fixed.HighPart;;
    Tsb->Cr4.LowPart |= Tsb->Cr4Fixed.LowPart;

    vDbgPrint(
        "[Daat] cr4 < %016x >\n",
        Tsb->Cr4.QuadPart);

    __ins_writecr(4, Tsb->Cr4.QuadPart);

    vDbgPrint(
        "[Daat] cr4 read shadow < %016x >\n",
        Tsb->Cr4ReadShadow.QuadPart);

    Tsb->Cr4Mask.LowPart = CR4_VMXE;

    vDbgPrint(
        "[Daat] cr4 mask < %016x >\n",
        Tsb->Cr4Mask.QuadPart);

    Result = __ins_vmx_on(&Tsb->Support);

    if (VMX_SUCCEED == Result) {
        Result = __ins_vmx_vmclear(&Tsb->Vmcs);

        if (VMX_SUCCEED == Result) {
            Result = __ins_vmx_vmptrld(&Tsb->Vmcs);

            if (VMX_SUCCEED != Result) {
                vDbgPrint("[Daat] vmptrld failed\n");
            }
        }
        else {
            vDbgPrint("[Daat] vmclear failed\n");
        }

        if (VMX_SUCCEED != Result) {
            __ins_vmx_off();
        }
    }
    else {
        vDbgPrint("[Daat] vmxon failed\n");
    }

    return Result;
}

void
NTAPI
__vmx_prepare_segment(
    __in PKDESCRIPTOR Gdtr,
    __in UINT16 Selector,
    __out PVMX_GDTENTRY VmxGdtEntry
)
{
#ifndef _WIN64
    PKGDTENTRY GdtEntry = NULL;
#else
    PKGDTENTRY64 GdtEntry = NULL;
#endif // !_WIN64

    RtlZeroMemory(VmxGdtEntry, sizeof(VMX_GDTENTRY));

    if ((0 == Selector) ||
        0 != (Selector & SELECTOR_TABLE_INDEX)) {
        VmxGdtEntry->Selector = 0;
        VmxGdtEntry->Limit = 0;
        VmxGdtEntry->Base = 0;
        VmxGdtEntry->AccessRights = 0x10000;
    }

#ifndef _WIN64
    GdtEntry =
        (PKGDTENTRY)((u)Gdtr->Base + (Selector & ~RPL_MASK));

    VmxGdtEntry->Selector = Selector;
    VmxGdtEntry->Limit = __ins_segment_limit(Selector);
    VmxGdtEntry->BaseLow = GdtEntry->BaseLow;
    VmxGdtEntry->BaseMiddle = GdtEntry->HighWord.Bits.BaseMid;
    VmxGdtEntry->BaseHigh = GdtEntry->HighWord.Bits.BaseHi;
    VmxGdtEntry->AccessRights = __ins_segment_ar(Selector);
#else
    GdtEntry =
        (PKGDTENTRY64)((u)Gdtr->Base + (Selector & ~RPL_MASK));

    VmxGdtEntry->Selector = Selector;
    VmxGdtEntry->Limit = __ins_segment_limit(Selector);
    VmxGdtEntry->BaseLow = GdtEntry->BaseLow;
    VmxGdtEntry->BaseMiddle = GdtEntry->Bits.BaseMiddle;
    VmxGdtEntry->BaseHigh = GdtEntry->Bits.BaseHigh;

    VmxGdtEntry->BaseUpper =
        0 == (GdtEntry->Bits.Type & 0x10) ? GdtEntry->BaseUpper : 0;

    VmxGdtEntry->AccessRights = __ins_segment_ar(Selector);
#endif // !_WIN64
}

VMX_RESULT
NTAPI
__vmx_entry(
    __in PREGISTERS_FRAME Registers
)
{
    VMX_RESULT Result = VMX_SUCCEED;
    PTSB Tsb = NULL;
    u32 Index = 0;
    u64 ErrorCode = 0;
    VMX_GDTENTRY VmxGdtEntry = { 0 };
    EPTP Eptp = { 0 };

    Tsb = CONTAINING_RECORD(Registers, TSB, Registers);

    Result = __vmx_prepare_vmcs(Tsb);

    if (VMX_SUCCEED == Result) {
        __ins_sgdt(&Tsb->Registers.Gdtr.Limit);

        __vmx_prepare_segment(
            &Tsb->Registers.Gdtr,
            Tsb->Registers.SegEs,
            &VmxGdtEntry);

        vDbgPrint(
            "[Daat] segment < %02x | %p | %p | %04x >\n",
            VmxGdtEntry.Selector,
            VmxGdtEntry.Limit,
            VmxGdtEntry.Base,
            VmxGdtEntry.AccessRights);

        __vmx_vmwrite_common(GUEST_ES_SELECTOR, VmxGdtEntry.Selector);
        __vmx_vmwrite_common(GUEST_ES_BASE, VmxGdtEntry.Base);
        __vmx_vmwrite_common(GUEST_ES_LIMIT, VmxGdtEntry.Limit);
        __vmx_vmwrite_common(GUEST_ES_AR, VmxGdtEntry.AccessRights);
        __vmx_vmwrite_common(HOST_ES_SELECTOR, VmxGdtEntry.Selector & ~RPL_MASK);

        __vmx_prepare_segment(
            &Tsb->Registers.Gdtr,
            Tsb->Registers.SegCs,
            &VmxGdtEntry);

        vDbgPrint(
            "[Daat] segment < %02x | %p | %p | %04x >\n",
            VmxGdtEntry.Selector,
            VmxGdtEntry.Limit,
            VmxGdtEntry.Base,
            VmxGdtEntry.AccessRights);

        __vmx_vmwrite_common(GUEST_CS_SELECTOR, VmxGdtEntry.Selector);
        __vmx_vmwrite_common(GUEST_CS_BASE, VmxGdtEntry.Base);
        __vmx_vmwrite_common(GUEST_CS_LIMIT, VmxGdtEntry.Limit);
        __vmx_vmwrite_common(GUEST_CS_AR, VmxGdtEntry.AccessRights);
        __vmx_vmwrite_common(HOST_CS_SELECTOR, VmxGdtEntry.Selector & ~RPL_MASK);

        __vmx_prepare_segment(
            &Tsb->Registers.Gdtr,
            Tsb->Registers.SegSs,
            &VmxGdtEntry);

        vDbgPrint(
            "[Daat] segment < %02x | %p | %p | %04x >\n",
            VmxGdtEntry.Selector,
            VmxGdtEntry.Limit,
            VmxGdtEntry.Base,
            VmxGdtEntry.AccessRights);

        __vmx_vmwrite_common(GUEST_SS_SELECTOR, VmxGdtEntry.Selector);
        __vmx_vmwrite_common(GUEST_SS_BASE, VmxGdtEntry.Base);
        __vmx_vmwrite_common(GUEST_SS_LIMIT, VmxGdtEntry.Limit);
        __vmx_vmwrite_common(GUEST_SS_AR, VmxGdtEntry.AccessRights);
        __vmx_vmwrite_common(HOST_SS_SELECTOR, VmxGdtEntry.Selector & ~RPL_MASK);

        __vmx_prepare_segment(
            &Tsb->Registers.Gdtr,
            Tsb->Registers.SegDs,
            &VmxGdtEntry);

        vDbgPrint(
            "[Daat] segment < %02x | %p | %p | %04x >\n",
            VmxGdtEntry.Selector,
            VmxGdtEntry.Limit,
            VmxGdtEntry.Base,
            VmxGdtEntry.AccessRights);

        __vmx_vmwrite_common(GUEST_DS_SELECTOR, VmxGdtEntry.Selector);
        __vmx_vmwrite_common(GUEST_DS_BASE, VmxGdtEntry.Base);
        __vmx_vmwrite_common(GUEST_DS_LIMIT, VmxGdtEntry.Limit);
        __vmx_vmwrite_common(GUEST_DS_AR, VmxGdtEntry.AccessRights);
        __vmx_vmwrite_common(HOST_DS_SELECTOR, VmxGdtEntry.Selector & ~RPL_MASK);

        __vmx_prepare_segment(
            &Tsb->Registers.Gdtr,
            Tsb->Registers.SegFs,
            &VmxGdtEntry);

        VmxGdtEntry.Base = __ins_readmsr(IA32_FS_BASE);

        vDbgPrint(
            "[Daat] segment < %02x | %p | %p | %04x >\n",
            VmxGdtEntry.Selector,
            VmxGdtEntry.Limit,
            VmxGdtEntry.Base,
            VmxGdtEntry.AccessRights);

        __vmx_vmwrite_common(GUEST_FS_SELECTOR, VmxGdtEntry.Selector);
        __vmx_vmwrite_common(GUEST_FS_BASE, VmxGdtEntry.Base);
        __vmx_vmwrite_common(GUEST_FS_LIMIT, VmxGdtEntry.Limit);
        __vmx_vmwrite_common(GUEST_FS_AR, VmxGdtEntry.AccessRights);
        __vmx_vmwrite_common(HOST_FS_SELECTOR, VmxGdtEntry.Selector & ~RPL_MASK);
        __vmx_vmwrite_common(HOST_FS_BASE, VmxGdtEntry.Base);

        __vmx_prepare_segment(
            &Tsb->Registers.Gdtr,
            Tsb->Registers.SegGs,
            &VmxGdtEntry);

        VmxGdtEntry.Base = __ins_readmsr(IA32_GS_BASE);

        vDbgPrint(
            "[Daat] segment < %02x | %p | %p | %04x >\n",
            VmxGdtEntry.Selector,
            VmxGdtEntry.Limit,
            VmxGdtEntry.Base,
            VmxGdtEntry.AccessRights);

        __vmx_vmwrite_common(GUEST_GS_SELECTOR, VmxGdtEntry.Selector);
        __vmx_vmwrite_common(GUEST_GS_BASE, VmxGdtEntry.Base);
        __vmx_vmwrite_common(GUEST_GS_LIMIT, VmxGdtEntry.Limit);
        __vmx_vmwrite_common(GUEST_GS_AR, VmxGdtEntry.AccessRights);
        __vmx_vmwrite_common(HOST_GS_SELECTOR, VmxGdtEntry.Selector & ~RPL_MASK);
        __vmx_vmwrite_common(HOST_GS_BASE, VmxGdtEntry.Base);

        __ins_sldt(&Tsb->Registers.Ldtr);

        __vmx_prepare_segment(
            &Tsb->Registers.Gdtr,
            Tsb->Registers.Ldtr,
            &VmxGdtEntry);

        vDbgPrint(
            "[Daat] segment < %02x | %p | %p | %04x >\n",
            VmxGdtEntry.Selector,
            VmxGdtEntry.Limit,
            VmxGdtEntry.Base,
            VmxGdtEntry.AccessRights);

        __vmx_vmwrite_common(GUEST_LDTR_SELECTOR, VmxGdtEntry.Selector);
        __vmx_vmwrite_common(GUEST_LDTR_BASE, VmxGdtEntry.Base);
        __vmx_vmwrite_common(GUEST_LDTR_LIMIT, VmxGdtEntry.Limit);
        __vmx_vmwrite_common(GUEST_LDTR_AR, VmxGdtEntry.AccessRights);

        __ins_str(&Tsb->Registers.Tr);

        __vmx_prepare_segment(
            &Tsb->Registers.Gdtr,
            Tsb->Registers.Tr,
            &VmxGdtEntry);

        vDbgPrint(
            "[Daat] segment < %02x | %p | %p | %04x >\n",
            VmxGdtEntry.Selector,
            VmxGdtEntry.Limit,
            VmxGdtEntry.Base,
            VmxGdtEntry.AccessRights);

        __vmx_vmwrite_common(GUEST_TR_SELECTOR, VmxGdtEntry.Selector);
        __vmx_vmwrite_common(GUEST_TR_BASE, VmxGdtEntry.Base);
        __vmx_vmwrite_common(GUEST_TR_LIMIT, VmxGdtEntry.Limit);
        __vmx_vmwrite_common(GUEST_TR_AR, VmxGdtEntry.AccessRights);
        __vmx_vmwrite_common(HOST_TR_SELECTOR, VmxGdtEntry.Selector & ~RPL_MASK);
        __vmx_vmwrite_common(HOST_TR_BASE, VmxGdtEntry.Base);

        vDbgPrint(
            "[Daat] gdtr < %04x | %p >\n",
            Tsb->Registers.Gdtr.Limit,
            Tsb->Registers.Gdtr.Base);

        __vmx_vmwrite_common(GUEST_GDTR_LIMIT, Tsb->Registers.Gdtr.Limit);
        __vmx_vmwrite_common(GUEST_GDTR_BASE, (u64)Tsb->Registers.Gdtr.Base);
        __vmx_vmwrite_common(HOST_GDTR_BASE, (u64)Tsb->Registers.Gdtr.Base);

        __ins_sidt(&Tsb->Registers.Idtr.Limit);

        vDbgPrint(
            "[Daat] idtr < %04x | %p >\n",
            Tsb->Registers.Idtr.Limit,
            Tsb->Registers.Idtr.Base);

        __vmx_vmwrite_common(GUEST_IDTR_LIMIT, Tsb->Registers.Idtr.Limit);
        __vmx_vmwrite_common(GUEST_IDTR_BASE, (u64)Tsb->Registers.Idtr.Base);
        __vmx_vmwrite_common(HOST_IDTR_BASE, (u64)Tsb->Registers.Idtr.Base);

        Tsb->Registers.Cr3 = __ins_readcr(3);

        __vmx_vmwrite_common(GUEST_CR0, Tsb->Cr0.QuadPart);
        __vmx_vmwrite_common(GUEST_CR3, Tsb->Registers.Cr3);
        __vmx_vmwrite_common(GUEST_CR4, Tsb->Cr4.QuadPart);

        Tsb->Registers.Dr7 = __ins_readdr(7);

        __vmx_vmwrite_common(GUEST_DR7, Tsb->Registers.Dr7);
        __vmx_vmwrite_common(GUEST_RFLAGS, Tsb->Registers.EFlags);

        __vmx_vmwrite_common(GUEST_RSP, (u64)Tsb->Registers.StackPointer);
        __vmx_vmwrite_common(GUEST_RIP, (u64)Tsb->Registers.ProgramCounter);

        __vmx_vmwrite_common(GUEST_DEBUGCTL, __ins_readmsr(IA32_DEBUGCTL));
        __vmx_vmwrite_common(GUEST_SYSENTER_CS, __ins_readmsr(IA32_SYSENTER_CS));
        __vmx_vmwrite_common(GUEST_SYSENTER_ESP, __ins_readmsr(IA32_SYSENTER_ESP));
        __vmx_vmwrite_common(GUEST_SYSENTER_EIP, __ins_readmsr(IA32_SYSENTER_EIP));

        __vmx_vmwrite_common(GUEST_INTERRUPTIBILITY, 0);
        __vmx_vmwrite_common(GUEST_ACTIVITY_STATE, 0);
        __vmx_vmwrite_common(GUEST_VMCS_LINK_PTR, VMCS_NONE);

        __vmx_vmwrite_common(HOST_CR0, Tsb->Cr0.QuadPart);
        __vmx_vmwrite_common(HOST_CR3, Tsb->Registers.Cr3);
        __vmx_vmwrite_common(HOST_CR4, Tsb->Cr4.QuadPart);

        __vmx_vmwrite_common(HOST_RSP, (u64)&Tsb->Registers);
        __vmx_vmwrite_common(HOST_RIP, (u64)__vmx_trap);

        __vmx_vmwrite_common(HOST_SYSENTER_CS, __ins_readmsr(IA32_SYSENTER_CS));
        __vmx_vmwrite_common(HOST_SYSENTER_ESP, __ins_readmsr(IA32_SYSENTER_ESP));
        __vmx_vmwrite_common(HOST_SYSENTER_EIP, __ins_readmsr(IA32_SYSENTER_EIP));

        __vmx_vmwrite_common(VMX_CR0_MASK, Tsb->Cr0Mask.QuadPart);
        __vmx_vmwrite_common(VMX_CR0_READ_SHADOW, Tsb->Cr0ReadShadow.QuadPart);

        __vmx_vmwrite_common(VMX_CR4_MASK, Tsb->Cr4Mask.QuadPart);
        __vmx_vmwrite_common(VMX_CR4_READ_SHADOW, Tsb->Cr4ReadShadow.QuadPart);

        __vmx_vmwrite_common(VMX_MSR_BITMAP, Tsb->Csb->MsrBitmap.QuadPart);
        __vmx_vmwrite_common(VMX_EXCEPTION_BITMAP, Tsb->Csb->ExceptionBitmap.QuadPart);

        __vmx_vmwrite_common(VMX_TSC_OFFSET, 0);

        __vmx_vmwrite_common(VMX_PIN_CONTROLS, Tsb->Csb->Controls.Pin.QuadPart);

        if (ENABLE_EPT ==
            (Tsb->Csb->Controls.Secondary.LowPart & ENABLE_EPT)) {
            Eptp.Pwl = 3;
            Eptp.Psmt = MTRR_TYPE_WRITE_BACK;
            Eptp.PageFrameNumber = Tsb->Csb->EptPml4e.QuadPart >> PAGE_SHIFT;

            __ins_vmx_vmwrite(VMX_EPTP, Eptp.Alignment);
            __ins_vmx_vmwrite(VMX_VPID, 1);
        }

        __vmx_vmwrite_common(VMX_SECONDARY_PROCESSOR_CONTROLS, Tsb->Csb->Controls.Secondary.QuadPart);
        __vmx_vmwrite_common(VMX_PRIMARY_PROCESSOR_CONTROLS, Tsb->Csb->Controls.Primary.QuadPart);
        __vmx_vmwrite_common(VMX_EXIT_CONTROLS, Tsb->Csb->Controls.Exit.QuadPart);
        __vmx_vmwrite_common(VMX_ENTRY_CONTROLS, Tsb->Csb->Controls.Entry.QuadPart);

        Result = __ins_vmx_vmlaunch();

        if (VMX_SUCCEED != Result) {
            if (VMX_FAIL_INVALID == Result) {
                __vmx_vmread_common(VMX_INSTRUCTION_ERROR_CODE, &ErrorCode);
            }

            vDbgPrint(
                "[Daat] vmlaunch failed < %p >\n",
                ErrorCode);

            __ins_vmx_off();
        }
    }

    return Result;
}

void
NTAPI
VmxStartProcessors(
    __in struct _KDPC * Dpc,
    __in ptr DeferredContext,
    __in ptr SystemArgument1,
    __in ptr SystemArgument2
)
{
    PKEVENT Notify = NULL;
    PTSB Tsb = NULL;

    Tsb = Dpc->DeferredContext;
    Notify = Dpc->SystemArgument1;

#ifdef DEBUG
    vDbgPrint(
        "[Daat] < %p : %p > current processor\n",
        KeGetCurrentProcessorNumber(),
        Tsb);
#endif // DEBUG

    __vmx_start(&Tsb->Registers);

    KeSetEvent(Notify, LOW_PRIORITY, FALSE);
}

void
NTAPI
VmxStartAllProcessors(
    __inout struct _CSB ** Block
)
{
    PCSB Csb = NULL;
    PCCHAR NumberProcessors = NULL;
    s8 Index = 0;
    UNICODE_STRING RoutineString = { 0 };
    KEVENT Notify = { 0 };
    KDPC Dpc = { 0 };

    // Dpc → __vmx_start → __vmx_entry → vmxon → vmclear → vmptrld → vmlaunch → vmresume
    //          ↪ store return address Non-volatile register & eflags → ↪__vmx_trap ⤴

    RtlInitUnicodeString(&RoutineString, L"KeNumberProcessors");

    NumberProcessors = MmGetSystemRoutineAddress(&RoutineString);

    Csb =
        __malloc(*NumberProcessors * sizeof(TSB) + sizeof(CSB));

    if (NULL != Csb) {
        RtlZeroMemory(
            Csb,
            *NumberProcessors * sizeof(TSB) + sizeof(CSB));

        if (NT_SUCCESS(__vmx_prepare_csb(Csb))) {
            KeInitializeEvent(
                &Notify,
                SynchronizationEvent,
                FALSE);

            for (Index = 0;
                Index < *NumberProcessors;
                Index++) {
                Csb->Tsb[Index].Number = Index;
                Csb->Tsb[Index].Csb = Csb;

                KeClearEvent(&Notify);

                KeInitializeDpc(
                    &Dpc,
                    VmxStartProcessors,
                    &Csb->Tsb[Index]);

                KeSetTargetProcessorDpc(&Dpc, Index);
                KeSetImportanceDpc(&Dpc, HighImportance);

                if (FALSE != KeInsertQueueDpc(
                    &Dpc,
                    &Notify,
                    NULL)) {
                    KeWaitForSingleObject(
                        &Notify,
                        Executive,
                        KernelMode,
                        FALSE,
                        NULL);
                }
            }

            if (NULL != Block) {
                __try {
                    *Block = Csb;
                }
                __except (EXCEPTION_EXECUTE_HANDLER) {
                    NOTHING;
                }
            }
        }
    }
}
