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

#include "VmxHandler.h"

#include "Intrin.h"
#include "Vmx.h"     

#ifdef _WIN64
void
NTAPI
__monitor_patch_guard(
    __inout struct _TSB * Tsb
)
{
    // patchguard code clear dr7

    if (0x10f ==
        __rdu16(Tsb->GuestState.GuestRip
            + Tsb->GuestState.Length)) {
        // restore idt

        __vmx_vmwrite_common(
            GUEST_IDTR_BASE,
            (u64)Tsb->Registers.Idtr.Base);

        __vmx_vmwrite_common(
            GUEST_IDTR_LIMIT,
            Tsb->Registers.Idtr.Limit);

        // KiDispatchException

        //
        // If the exception is a break point, then convert the break point to a
        // fault.
        //

        // if (ExceptionRecord->ExceptionCode == STATUS_BREAKPOINT) {
        //     ContextRecord.Rip -= 1;
        // }

        Tsb->GuestState.GuestRip += 1;

        __inject_exception(
            Tsb,
            VECTOR_BP,
            NO_ERROR_CODE,
            EXCEPTION);

        // use hardware breakpoint set dr0

        // __ins_writedr(0, Tsb->GuestState.GuestRip + Tsb->GuestState.Length);
        // __ins_writedr(6, DR6_SETBITS | (1 << 0));
        // __vmx_vmwrite_common(GUEST_DR7, DR7_SETBITS | (1 << 0));
    }
}
#endif // _WIN64

void
NTAPI
__vmx_print_interrupt(
    __inout struct _TSB * Tsb
)
{
    INTERRUPT_ADDRESS InterruptAddress = { 0 };
    u32 Index = 0;
    u32 Count = 0;

#ifndef _WIN64
    PKIDTENTRY IdtEntry = NULL;
#else
    PKIDTENTRY64 IdtEntry = NULL;
#endif // !_WIN64

#ifdef DEBUG
    vDbgPrint(
        "[Daat] < %04x : %p >\n",
        Tsb->Registers.Idtr.Limit,
        Tsb->Registers.Idtr.Base);
#endif // DEBUG

    IdtEntry = (ptr)(u)Tsb->Registers.Idtr.Base;

#ifndef _WIN64
    Count = (Tsb->Registers.Idtr.Limit + 1) / sizeof(KIDTENTRY);
#else
    Count = (Tsb->Registers.Idtr.Limit + 1) / sizeof(KIDTENTRY64);
#endif // !_WIN64

    for (Index = 0;
        Index < Count;
        Index++) {
#ifndef _WIN64
        InterruptAddress.Offset = IdtEntry[Index].Offset;
        InterruptAddress.ExtendedOffset = IdtEntry[Index].ExtendedOffset;
#else
        InterruptAddress.OffsetLow = IdtEntry[Index].OffsetLow;
        InterruptAddress.OffsetMiddle = IdtEntry[Index].OffsetMiddle;
        InterruptAddress.OffsetHigh = IdtEntry[Index].OffsetHigh;
#endif // !_WIN64

#ifdef DEBUG
        vDbgPrint(
            "[Daat] %04x : %p\n",
            Index,
            InterruptAddress.Address);
#endif // DEBUG
    }
}

void
NTAPI
__inject_exception(
    __inout struct _TSB * Tsb,
    __in u64 Vector,
    __in_opt u64 ErrorCode,
    __in u64 Type
)
{
    __vmx_vmread_common(
        VM_EXIT_INFO_IDT_VECTORING,
        &Tsb->GuestState.Interrupt);

    if (0 != Tsb->GuestState.Interrupt.Valid) {
        // check whether the pending vector and injecting vector can generate a double fault

        Tsb->GuestState.Entry.Vector = VECTOR_DF;
    }
    else {
        Tsb->GuestState.Entry.Vector = Vector;
    }

    Tsb->GuestState.Entry.Type = Type;
    Tsb->GuestState.Entry.Valid = 1;

    if (NO_ERROR_CODE != ErrorCode) {
        Tsb->GuestState.Entry.DeliverErrorCode = 1;

        __ins_vmx_vmwrite(
            VMX_ENTRY_EXCEPTION_ERROR_CODE,
            ErrorCode);
    }

    Tsb->GuestState.Entry.Information &= 0x00000000ffffffffull;

    __vmx_vmwrite_common(
        VMX_ENTRY_INTERRUPT_INFO,
        Tsb->GuestState.Entry.Information);

    __ins_vmx_vmwrite(
        VMX_ENTRY_INSTRUCTION_LENGTH,
        Tsb->GuestState.Length);
}

void
NTAPI
__vm_null(
    __inout PTSB Tsb
)
{

}

void
NTAPI
__vm_exception_nmi(
    __inout PTSB Tsb
)
{
    u64 ErrorCode = NO_ERROR_CODE;

    __vmx_vmread_common(
        VM_EXIT_INFO_INTERRUPT_INFO,
        &Tsb->GuestState.Exit.Alignment);

    if (0 != Tsb->GuestState.Exit.ErrorCode) {
        __vmx_vmread_common(
            VM_EXIT_INFO_EXCEPTION_ERROR_CODE,
            &ErrorCode);
    }

    switch (Tsb->GuestState.Exit.Vector) {
    case VECTOR_NMI: {
        break;
    }

    case VECTOR_DB: {

        break;
    }

    case VECTOR_BP: {
        break;
    }

    default: {
        break;
    }
    }

    __inject_exception(
        Tsb,
        Tsb->GuestState.Exit.Vector,
        ErrorCode,
        Tsb->GuestState.Exit.Type);
}

void
NTAPI
__vm_interrupt(
    __inout PTSB Tsb
)
{

}

void
NTAPI
__vm_cpuid(
    __inout PTSB Tsb
)
{
    CPUINFO CpuInfo = { 0 };

    __ins_cpuid(
        Tsb->Registers.Eax,
        Tsb->Registers.Ecx,
        &CpuInfo);

    if (1 == Tsb->Registers.Eax &&
        0 == Tsb->Registers.Ecx) {
        CpuInfo.Ecx &= ~(1UL << 5);
    }

#ifndef _WIN64
    Tsb->Registers.Eax = CpuInfo.Eax;
    Tsb->Registers.Ebx = CpuInfo.Ebx;
    Tsb->Registers.Ecx = CpuInfo.Ecx;
    Tsb->Registers.Edx = CpuInfo.Edx;
#else
    Tsb->Registers.Rax = CpuInfo.Eax;
    Tsb->Registers.Rbx = CpuInfo.Ebx;
    Tsb->Registers.Rcx = CpuInfo.Ecx;
    Tsb->Registers.Rdx = CpuInfo.Edx;
#endif // !_WIN64
}

void
NTAPI
__vm_invd(
    __inout PTSB Tsb
)
{
    __ins_invd();
}

void
NTAPI
__vm_rdtsc(
    __inout PTSB Tsb
)
{
    ULARGE_INTEGER Tsc = { 0 };

    Tsc.QuadPart = __ins_rdtsc();

#ifndef _WIN64
    Tsb->Registers.Eax = Tsc.LowPart;
    Tsb->Registers.Edx = Tsc.HighPart;
#else
    Tsb->Registers.Rax = Tsc.LowPart;
    Tsb->Registers.Rdx = Tsc.HighPart;
#endif // !_WIN64
}

void
NTAPI
__vm_vmx(
    __inout PTSB Tsb
)
{
    __vmx_vmread_common(GUEST_RFLAGS, &Tsb->GuestState.GuestRFlags);

    Tsb->GuestState.GuestRFlags |= EFLAGS_ZF | EFLAGS_CF;

    __vmx_vmwrite_common(GUEST_RFLAGS, Tsb->GuestState.GuestRFlags);
}

void
NTAPI
__vm_cr_access(
    __inout PTSB Tsb
)
{
    u *Cr = NULL;
    u *GpReg = NULL;

    __vmx_vmread_common(
        VM_EXIT_INFO_QUALIFICATION,
        &Tsb->GuestState.Qualification);

    Cr = Tsb->Registers.Cr + Tsb->GuestState.Qualification.CR.Number;
    GpReg = Tsb->Registers.Reg + Tsb->GuestState.Qualification.CR.GpReg;

    if (0 != Tsb->GuestState.Qualification.CR.Type) {
        // MOV from CR

        __vmx_vmread_common(GUEST_CR0, &Tsb->Registers.Cr0);
        __vmx_vmread_common(GUEST_CR3, &Tsb->Registers.Cr3);
        __vmx_vmread_common(GUEST_CR4, &Tsb->Registers.Cr4);

        Tsb->Registers.Cr2 = __ins_readcr(2);

        *GpReg = *Cr;
    }
    else {
        // MOV to CR

        *Cr = *GpReg;

        __vmx_vmwrite_common(GUEST_CR0, Tsb->Registers.Cr0);
        __vmx_vmwrite_common(GUEST_CR3, Tsb->Registers.Cr3);
        __vmx_vmwrite_common(GUEST_CR4, Tsb->Registers.Cr4);

        __ins_writecr(2, Tsb->Registers.Cr2);
    }
}

void
NTAPI
__vm_dr_access(
    __inout PTSB Tsb
)
{
    u * DrReg = NULL;
    u * GpReg = NULL;

    __vmx_vmread_common(GUEST_DR7, &Tsb->Registers.Dr7);

    if (DR7_GD == (Tsb->Registers.Dr7 & DR7_GD)) {
        Tsb->Registers.Dr6 = __ins_readdr(6);

        Tsb->Registers.Dr6 |= DR6_BD;

        __ins_writedr(6, Tsb->Registers.Dr6);

        Tsb->Registers.Dr7 &= ~DR7_GD;

        __vmx_vmwrite_common(GUEST_DR7, Tsb->Registers.Dr7);

        Tsb->GuestState.Length = 0;

        __inject_exception(Tsb, VECTOR_DB, NO_ERROR_CODE, EXCEPTION);
    }
    else {
        __vmx_vmread_common(
            VM_EXIT_INFO_QUALIFICATION,
            &Tsb->GuestState.Qualification);

        if (4 == Tsb->GuestState.Qualification.DR.Number ||
            5 == Tsb->GuestState.Qualification.DR.Number) {
            if (CR4_DE == (Tsb->Registers.Cr4 & CR4_DE)) {
                Tsb->GuestState.Length = 0;

                __inject_exception(Tsb, VECTOR_UD, NO_ERROR_CODE, EXCEPTION);

                return;
            }
            else {
                // Dr4 <==> Dr6
                // Dr5 <==> Dr7

                Tsb->GuestState.Qualification.DR.Number += 2;
            }
        }

        DrReg = Tsb->Registers.Dr + Tsb->GuestState.Qualification.DR.Number;
        GpReg = Tsb->Registers.Reg + Tsb->GuestState.Qualification.DR.GpReg;

        if (0 != Tsb->GuestState.Qualification.DR.Direction) {
            // MOV DR -> GPR

            if (7 != Tsb->GuestState.Qualification.DR.Number) {
                *DrReg = __ins_readdr(Tsb->GuestState.Qualification.DR.Number);
            }

            *GpReg = *DrReg;
        }
        else {
            // MOV DR <- GPR

            *DrReg = *GpReg;

            if (7 != Tsb->GuestState.Qualification.DR.Number) {
                __ins_writedr(
                    Tsb->GuestState.Qualification.DR.Number,
                    *DrReg);
            }
            else {
                __vmx_vmwrite_common(GUEST_DR7, Tsb->Registers.Dr7);

#ifdef _WIN64                              
                // __monitor_patch_guard(Tsb);
#endif // _WIN64
            }
        }
    }
}

void
NTAPI
__vm_msr_read(
    __inout PTSB Tsb
)
{
    ULARGE_INTEGER Msr = { 0 };

    switch (Tsb->Registers.Ecx) {
    case IA32_DEBUGCTL: {
        __vmx_vmread_common(GUEST_DEBUGCTL, &Msr.QuadPart);

        break;
    }

    case IA32_SYSENTER_CS: {
        __vmx_vmread_common(GUEST_SYSENTER_CS, &Msr.QuadPart);

        break;
    }

    case IA32_SYSENTER_ESP: {
        __vmx_vmread_common(GUEST_SYSENTER_ESP, &Msr.QuadPart);

        break;
    }

    case IA32_SYSENTER_EIP: {
        __vmx_vmread_common(GUEST_SYSENTER_EIP, &Msr.QuadPart);

        break;
    }

    case IA32_FS_BASE: {
        __vmx_vmread_common(GUEST_FS_BASE, &Msr.QuadPart);

        break;
    }

    case IA32_GS_BASE: {
        __vmx_vmread_common(GUEST_GS_BASE, &Msr.QuadPart);

        break;
    }

    case IA32_FEATURE_CONTROL: {
        Msr.QuadPart = __ins_readmsr(Tsb->Registers.Ecx);
        Msr.QuadPart &= ~FC_LOCKED;

        break;
    }

    default: {
        Msr.QuadPart = __ins_readmsr(Tsb->Registers.Ecx);

        break;
    }
    }

#ifndef _WIN64
    Tsb->Registers.Eax = Msr.LowPart;
    Tsb->Registers.Edx = Msr.HighPart;
#else
    Tsb->Registers.Rax = Msr.LowPart;
    Tsb->Registers.Rdx = Msr.HighPart;
#endif // !_WIN64
}

void
NTAPI
__vm_msr_write(
    __inout PTSB Tsb
)
{
    ULARGE_INTEGER Msr = { 0 };

    Msr.LowPart = Tsb->Registers.Eax;
    Msr.HighPart = Tsb->Registers.Edx;

    switch (Tsb->Registers.Ecx) {
    case IA32_DEBUGCTL: {
        __vmx_vmwrite_common(GUEST_DEBUGCTL, Msr.QuadPart);

        break;
    }

    case IA32_SYSENTER_CS: {
        __vmx_vmwrite_common(GUEST_SYSENTER_CS, Msr.QuadPart);

        break;
    }

    case IA32_SYSENTER_ESP: {
        __vmx_vmwrite_common(GUEST_SYSENTER_ESP, Msr.QuadPart);

        break;
    }

    case IA32_SYSENTER_EIP: {
        __vmx_vmwrite_common(GUEST_SYSENTER_EIP, Msr.QuadPart);

        break;
    }

    case IA32_FS_BASE: {
        __vmx_vmwrite_common(GUEST_FS_BASE, Msr.QuadPart);

        break;
    }

    case IA32_GS_BASE: {
        __vmx_vmwrite_common(GUEST_GS_BASE, Msr.QuadPart);

        break;
    }

    case IA32_FEATURE_CONTROL: {

        break;
    }

    default: {
        __ins_writemsr(Tsb->Registers.Ecx, Msr.QuadPart);

        break;
    }
    }
}

void
NTAPI
__vm_mtf(
    __inout PTSB Tsb
)
{

}

void
NTAPI
__vm_invept(
    __inout PTSB Tsb
)
{
    // __ins_invept();
}

void
NTAPI
__vm_invvpid(
    __inout PTSB Tsb
)
{
    // __ins_invvpid();
}

void
NTAPI
__vm_ept_violation(
    __inout PTSB Tsb
)
{

}

void
NTAPI
__vm_ept_misconfig(
    __inout PTSB Tsb
)
{
    Tsb->GuestState.GuestRip += 1;

    __inject_exception(
        Tsb, 
        VECTOR_BP, 
        NO_ERROR_CODE,
        EXCEPTION);
}

void
NTAPI
__vm_rdtscp(
    __inout PTSB Tsb
)
{
    ULARGE_INTEGER Tsc = { 0 };
    u32 Aux = 0;

    Tsc.QuadPart = __ins_rdtscp(&Aux);

#ifndef _WIN64
    Tsb->Registers.Eax = Tsc.LowPart;
    Tsb->Registers.Edx = Tsc.HighPart;

    Tsb->Registers.Ecx = Aux;
#else
    Tsb->Registers.Rax = Tsc.LowPart;
    Tsb->Registers.Rdx = Tsc.HighPart;

    Tsb->Registers.Rcx = Aux;
#endif // !_WIN64
}

void
NTAPI
__vm_xsetbv(
    __inout PTSB Tsb
)
{
    ULARGE_INTEGER Xcr = { 0 };

    Xcr.LowPart = Tsb->Registers.Eax;
    Xcr.HighPart = Tsb->Registers.Edx;

    __ins_xsetbv(Tsb->Registers.Ecx, Xcr.QuadPart);
}

PVMHANDLER VmxHandlers[] = {
    __vm_exception_nmi, // [VMX_EXIT_INT_EXCEPTION_NMI] An SW interrupt, exception or NMI has occurred
    __vm_interrupt, // [VMX_EXIT_EXT_INTERRUPT] An external interrupt has occurred
    __vm_null, // [VMX_EXIT_TRIPLE_FAULT] Triple fault occurred
    __vm_null, // [VMX_EXIT_INIT_EVENT] INIT signal arrived
    __vm_null, // [VMX_EXIT_SIPI_EVENT] SIPI signal arrived
    __vm_null, // [VMX_EXIT_SMI_IO_EVENT]
    __vm_null, // [VMX_EXIT_SMI_OTHER_EVENT]
    __vm_null, // [VMX_EXIT_PENDING_INTERRUPT]
    __vm_null, // [VMX_EXIT_PENDING_NMI]
    __vm_null, // [VMX_EXIT_TASK_SWITCH] Guest attempted a task switch

    __vm_cpuid, // [VMX_EXIT_CPUID]
    __vm_null, // [VMX_EXIT_GETSEC] Guest executed GETSEC instruction
    __vm_null, // [VMX_EXIT_HLT] Guest executed HLT instruction
    __vm_invd, // [VMX_EXIT_INVD] Guest executed INVD instruction
    __vm_null, // [VMX_EXIT_INVLPG] Guest executed INVLPG instruction
    __vm_null, // [VMX_EXIT_RDPMC] Guest executed RDPMC instruction
    __vm_rdtsc, // [VMX_EXIT_RDTSC] Guest executed RDTSC instruction
    __vm_null, // [VMX_EXIT_RSM] Guest executed RSM instruction in SMM
    __vm_vmx, // [VMX_EXIT_VMCALL]
    __vm_vmx, // [VMX_EXIT_VMCLEAR]

    __vm_vmx, // [VMX_EXIT_VMLAUNCH]
    __vm_vmx, // [VMX_EXIT_VMPTRLD]
    __vm_vmx, // [VMX_EXIT_VMPTRST]
    __vm_vmx, // [VMX_EXIT_VMREAD]
    __vm_vmx, // [VMX_EXIT_VMRESUME]
    __vm_vmx, // [VMX_EXIT_VMWRITE]
    __vm_vmx, // [VMX_EXIT_VMXOFF]
    __vm_vmx, // [VMX_EXIT_VMXON]
    __vm_cr_access, // [VMX_EXIT_CR_ACCESS] Guest accessed a control register
    __vm_dr_access, // [VMX_EXIT_DR_ACCESS] Guest attempted access to debug register

    __vm_null, // __vm_io, // [VMX_EXIT_IO] Guest attempted I/O
    __vm_msr_read, // [VMX_EXIT_MSR_READ] Guest attempted to read an MSR
    __vm_msr_write, // [VMX_EXIT_MSR_WRITE] Guest attempted to write an MSR
    __vm_null, // [VMX_EXIT_FAILED_VMENTER_GS] VMENTER failed due to guest state
    __vm_null, // [VMX_EXIT_FAILED_VMENTER_MSR] VMENTER failed due to MSR loading
    __vm_null, // nothing
    __vm_null, // [VMX_EXIT_MWAIT]
    __vm_mtf, // [VMX_EXIT_MTF_EXIT]
    __vm_null, // nothing
    __vm_null, // [VMX_EXIT_MONITOR]

    __vm_null, // [VMX_EXIT_PAUSE]
    __vm_null, // [VMX_EXIT_MACHINE_CHECK]
    __vm_null, // nothing
    __vm_null, // [VMX_EXIT_TPR_BELOW_THRESHOLD]
    __vm_null, // [VMX_EXIT_APIC_ACCESS]
    __vm_null, // [VMX_EXIT_VIRTUALIZED_EOI]
    __vm_null, // [VMX_EXIT_GDT_IDT_ACCESS]
    __vm_null, // [VMX_EXIT_LDT_TR_ACCESS]
    __vm_ept_violation, // [VMX_EXIT_EPT_VIOLATION]
    __vm_ept_misconfig, // [VMX_EXIT_EPT_MISCONFIG]

    __vm_null, // [VMX_EXIT_INVEPT]
    __vm_rdtscp, // [VMX_EXIT_RDTSCP]
    __vm_null, // [VMX_EXIT_VMX_TIMER_EXIT]
    __vm_null, // [VMX_EXIT_INVVPID]
    __vm_null, // [VMX_EXIT_WBINVD]
    __vm_xsetbv, // [VMX_EXIT_XSETBV] // Guest executed XSETBV instruction
    __vm_null, // [VMX_EXIT_APIC_WRITE]
    __vm_null, // [VMX_EXIT_RDRAND]
    __vm_null, // [VMX_EXIT_INVPCID]
    __vm_vmx, // [VMX_EXIT_VMFUNC]

    __vm_null, // [VMX_EXIT_ENCLS]
    __vm_null, // [VMX_EXIT_RDSEED]
    __vm_null, // [VMX_EXIT_PML_FULL]
    __vm_null, // [VMX_EXIT_XSAVES]
    __vm_null, // [VMX_EXIT_XRSTORS]
    __vm_null, // nothing
    __vm_null, // [VMX_EXIT_SPP_RELATED]
    __vm_null, // [VMX_EXIT_UMWAIT]
    __vm_null // [VMX_EXIT_TPAUSE]
};

void
NTAPI
__vm_exit_dispatch(
    __inout PREGISTERS_FRAME Registers
)
{
    PTSB Tsb = NULL;

    Tsb = CONTAINING_RECORD(Registers, TSB, Registers);

    __vmx_vmread_common(GUEST_RIP, &Tsb->GuestState.GuestRip);
    __vmx_vmread_common(VM_EXIT_INFO_REASON, &Tsb->GuestState.Reason);
    __vmx_vmread_common(VM_EXIT_INFO_INSTRUCTION_LENGTH, &Tsb->GuestState.Length);

    VmxHandlers[Tsb->GuestState.Reason.BasicReason](Tsb);

    Tsb->GuestState.GuestRip += Tsb->GuestState.Length;

    __vmx_vmwrite_common(GUEST_RIP, Tsb->GuestState.GuestRip);

    RestoreRegisters(Registers);
}
