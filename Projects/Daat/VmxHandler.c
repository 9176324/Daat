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
* The Initial Developer of the Original e is blindtiger.
*
*/

#include <defs.h>

#include "VmxHandler.h"

#include "Vmx.h"

void
NTAPI
__monitor_set_dr7(
    __inout PCCB Block
)
{
#ifdef _WIN64
    u16ptr GuestRip = NULL;

    // patchguard code clear dr7

    GuestRip =
        (ptr)(Block->GuestState.GuestRip +
            Block->GuestState.InstructionLength);

    if (0x10f == *GuestRip) {
        // restore idt
        __vmx_vmwrite_common(GUEST_IDTR_BASE, (u64)Block->Registers.Idtr.Base);
        __vmx_vmwrite_common(GUEST_IDTR_LIMIT, Block->Registers.Idtr.Limit);
        __vmx_vmwrite_common(GUEST_DR7, DR7_SETBITS);

        // use hardware breakpoint
        // __ops_writedr(0, Block->GuestState.GuestRip + Block->GuestState.InstructionLength);
        // __ops_writedr(6, DR6_SETBITS | (1 << 0));
        // __vmx_vmwrite_common(GUEST_DR7, DR7_SETBITS | (1 << 0));

        __inject_exception(Block, VECTOR_BP, NO_ERROR_CODE, EXCEPTION);
    }
#endif // _WIN64
}

void
NTAPI
__inject_exception(
    __inout PCCB Block,
    __in u64 Vector,
    __in_opt u64 ErrorCode,
    __in u64 Type
)
{
    __vmx_vmread_common(
        VM_EXIT_INFO_IDT_VECTORING,
        &Block->GuestState.IdtVectoring);

    if (0 != Block->GuestState.IdtVectoring.Valid) {
        // check whether the pending vector and injecting vector can generate a double fault

        Block->GuestState.EntryInterruption.Vector = VECTOR_DF;
    }
    else {
        Block->GuestState.EntryInterruption.Vector = Vector;
    }

    Block->GuestState.EntryInterruption.Type = Type;
    Block->GuestState.EntryInterruption.Valid = 1;

    if (NO_ERROR_CODE != ErrorCode) {
        Block->GuestState.EntryInterruption.DeliverErrorCode = 1;

        __vmx_vmwrite(
            VMX_ENTRY_EXCEPTION_ERROR_CODE,
            ErrorCode);
    }

    Block->GuestState.EntryInterruption.Information &= 0x00000000FFFFFFFFULL;

    __vmx_vmwrite_common(
        VMX_ENTRY_INTERRUPT_INFO,
        Block->GuestState.EntryInterruption.Information);

    __vmx_vmwrite(
        VMX_ENTRY_INSTRUCTION_LENGTH,
        Block->GuestState.InstructionLength);
}

void
NTAPI
__vm_exception_nmi(
    __inout PCCB Block
)
{
    u64 ErrorCode = NO_ERROR_CODE;

    __vmx_vmread_common(
        VM_EXIT_INFO_INTERRUPT_INFO,
        &Block->GuestState.ExitInterruption.Information);

    if (0 != Block->GuestState.ExitInterruption.ErrorCode) {
        __vmx_vmread_common(
            VM_EXIT_INFO_EXCEPTION_ERROR_CODE,
            &ErrorCode);
    }

    switch (Block->GuestState.ExitInterruption.Vector) {
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
        Block,
        Block->GuestState.ExitInterruption.Vector,
        ErrorCode,
        Block->GuestState.ExitInterruption.Type);
}

void
NTAPI
__vm_interrupt(
    __inout PCCB Block
)
{

}

void
NTAPI
__vm_null(
    __inout PCCB Block
)
{

}

void
NTAPI
__vm_cpuid(
    __inout PCCB Block
)
{
    CPUINFO CpuInfo = { 0 };

    __ops_cpuid(
        Block->Registers.Eax,
        Block->Registers.Ecx,
        &CpuInfo);

    if (1 == Block->Registers.Eax &&
        0 == Block->Registers.Ecx) {
        CpuInfo.Ecx &= ~(1UL << 5);
    }

#ifndef _WIN64
    Block->Registers.Eax = CpuInfo.Eax;
    Block->Registers.Ebx = CpuInfo.Ebx;
    Block->Registers.Ecx = CpuInfo.Ecx;
    Block->Registers.Edx = CpuInfo.Edx;
#else
    Block->Registers.Rax = CpuInfo.Eax;
    Block->Registers.Rbx = CpuInfo.Ebx;
    Block->Registers.Rcx = CpuInfo.Ecx;
    Block->Registers.Rdx = CpuInfo.Edx;
#endif // !_WIN64
}

void
NTAPI
__vm_invd(
    __inout PCCB Block
)
{
    __ops_invd();
}

void
NTAPI
__vm_rdtsc(
    __inout PCCB Block
)
{
    ULARGE_INTEGER Tsc = { 0 };

    Tsc.QuadPart = __ops_rdtsc();

#ifndef _WIN64
    Block->Registers.Eax = Tsc.LowPart;
    Block->Registers.Edx = Tsc.HighPart;
#else
    Block->Registers.Rax = Tsc.LowPart;
    Block->Registers.Rdx = Tsc.HighPart;
#endif // !_WIN64
}

void
NTAPI
__vm_vmx(
    __inout PCCB Block
)
{
    __vmx_vmread_common(GUEST_RFLAGS, &Block->GuestState.GuestRFlags);

    Block->GuestState.GuestRFlags |= EFLAGS_ZF | EFLAGS_CF;

    __vmx_vmwrite_common(GUEST_RFLAGS, Block->GuestState.GuestRFlags);
}

void
NTAPI
__vm_cr_access(
    __inout PCCB Block
)
{
    u *Cr = NULL;
    u *GpReg = NULL;

    __vmx_vmread_common(
        VM_EXIT_INFO_QUALIFICATION,
        &Block->GuestState.Qualification);

    Cr = Block->Registers.Cr + Block->GuestState.Qualification.CR.Number;
    GpReg = Block->Registers.Reg + Block->GuestState.Qualification.CR.GpReg;

    if (0 != Block->GuestState.Qualification.CR.Type) {
        // MOV from CR

        __vmx_vmread_common(GUEST_CR0, &Block->Registers.Cr0);
        __vmx_vmread_common(GUEST_CR3, &Block->Registers.Cr3);
        __vmx_vmread_common(GUEST_CR4, &Block->Registers.Cr4);

        Block->Registers.Cr2 = __ops_readcr(2);

        *GpReg = *Cr;
    }
    else {
        // MOV to CR

        *Cr = *GpReg;

        __vmx_vmwrite_common(GUEST_CR0, Block->Registers.Cr0);
        __vmx_vmwrite_common(GUEST_CR3, Block->Registers.Cr3);
        __vmx_vmwrite_common(GUEST_CR4, Block->Registers.Cr4);

        __ops_writecr(2, Block->Registers.Cr2);
    }
}

void
NTAPI
__vm_dr_access(
    __inout PCCB Block
)
{
    u * DrReg = NULL;
    u * GpReg = NULL;

    __vmx_vmread_common(GUEST_DR7, &Block->Registers.Dr7);

    if (DR7_GD == (Block->Registers.Dr7 & DR7_GD)) {
        Block->Registers.Dr6 = __ops_readdr(6);

        Block->Registers.Dr6 |= DR6_BD;

        __ops_writedr(6, Block->Registers.Dr6);

        Block->Registers.Dr7 &= ~DR7_GD;

        __vmx_vmwrite_common(GUEST_DR7, Block->Registers.Dr7);

        Block->GuestState.InstructionLength = 0;

        __inject_exception(Block, VECTOR_DB, NO_ERROR_CODE, EXCEPTION);
    }
    else {
        __vmx_vmread_common(
            VM_EXIT_INFO_QUALIFICATION,
            &Block->GuestState.Qualification);

        if (4 == Block->GuestState.Qualification.DR.Number ||
            5 == Block->GuestState.Qualification.DR.Number) {
            if (CR4_DE == (Block->Registers.Cr4 & CR4_DE)) {
                Block->GuestState.InstructionLength = 0;

                __inject_exception(Block, VECTOR_UD, NO_ERROR_CODE, EXCEPTION);

                return;
            }
            else {
                // Dr4 <==> Dr6
                // Dr5 <==> Dr7

                Block->GuestState.Qualification.DR.Number += 2;
            }
        }

        DrReg = Block->Registers.Dr + Block->GuestState.Qualification.DR.Number;
        GpReg = Block->Registers.Reg + Block->GuestState.Qualification.DR.GpReg;

        if (0 != Block->GuestState.Qualification.DR.Direction) {
            // MOV DR -> GPR

            if (7 != Block->GuestState.Qualification.DR.Number) {
                *DrReg = __ops_readdr(Block->GuestState.Qualification.DR.Number);
            }

            *GpReg = *DrReg;
        }
        else {
            // MOV DR <- GPR

            *DrReg = *GpReg;

            if (7 != Block->GuestState.Qualification.DR.Number) {
                __ops_writedr(
                    Block->GuestState.Qualification.DR.Number,
                    *DrReg);
            }
            else {
                __vmx_vmwrite_common(GUEST_DR7, Block->Registers.Dr7);

                __monitor_set_dr7(Block);
            }
        }
    }
}

void
NTAPI
__vm_msr_read(
    __inout PCCB Block
)
{
    ULARGE_INTEGER Msr = { 0 };

    switch (Block->Registers.Ecx) {
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
        Msr.QuadPart = __ops_readmsr(Block->Registers.Ecx);
        Msr.QuadPart &= ~FC_LOCKED;

        break;
    }

    default: {
        Msr.QuadPart = __ops_readmsr(Block->Registers.Ecx);

        break;
    }
    }

#ifndef _WIN64
    Block->Registers.Eax = Msr.LowPart;
    Block->Registers.Edx = Msr.HighPart;
#else
    Block->Registers.Rax = Msr.LowPart;
    Block->Registers.Rdx = Msr.HighPart;
#endif // !_WIN64
}

void
NTAPI
__vm_msr_write(
    __inout PCCB Block
)
{
    ULARGE_INTEGER Msr = { 0 };

    Msr.LowPart = Block->Registers.Eax;
    Msr.HighPart = Block->Registers.Edx;

    switch (Block->Registers.Ecx) {
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
        __ops_writemsr(Block->Registers.Ecx, Msr.QuadPart);

        break;
    }
    }
}

void
NTAPI
__vm_rdtscp(
    __inout PCCB Block
)
{
    ULARGE_INTEGER Tsc = { 0 };
    u32 Aux = 0;

    Tsc.QuadPart = __ops_rdtscp(&Aux);

#ifndef _WIN64
    Block->Registers.Eax = Tsc.LowPart;
    Block->Registers.Edx = Tsc.HighPart;

    Block->Registers.Ecx = Aux;
#else
    Block->Registers.Rax = Tsc.LowPart;
    Block->Registers.Rdx = Tsc.HighPart;

    Block->Registers.Rcx = Aux;
#endif // !_WIN64
}

void
NTAPI
__vm_xsetbv(
    __inout PCCB Block
)
{
    ULARGE_INTEGER Xcr = { 0 };

    Xcr.LowPart = Block->Registers.Eax;
    Xcr.HighPart = Block->Registers.Edx;

    __ops_xsetbv(Block->Registers.Ecx, Xcr.QuadPart);
}

PEXIT_HANDLER VmxHandlers[] = {
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
    __vm_null, // [VMX_EXIT_MTF_EXIT]
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
    __vm_null, // [VMX_EXIT_EPT_VIOLATION]
    __vm_null, // [VMX_EXIT_EPT_MISCONFIG]

    __vm_vmx, // [VMX_EXIT_INVEPT]
    __vm_rdtscp, // [VMX_EXIT_RDTSCP]
    __vm_null, // [VMX_EXIT_VMX_TIMER_EXIT]
    __vm_vmx, // [VMX_EXIT_INVVPID]
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
    PCCB CurrentBlock = NULL;

    CurrentBlock = CONTAINING_RECORD(Registers, CCB, Registers);

    __vmx_vmread_common(GUEST_RIP, &CurrentBlock->GuestState.GuestRip);
    __vmx_vmread_common(VM_EXIT_INFO_REASON, &CurrentBlock->GuestState.Reason);
    __vmx_vmread_common(VM_EXIT_INFO_INSTRUCTION_LENGTH, &CurrentBlock->GuestState.InstructionLength);

    VmxHandlers[CurrentBlock->GuestState.Reason.BasicReason](CurrentBlock);

    CurrentBlock->GuestState.GuestRip += CurrentBlock->GuestState.InstructionLength;

    __vmx_vmwrite_common(GUEST_RIP, CurrentBlock->GuestState.GuestRip);
    // __vmx_vmwrite_common(GUEST_IDTR_BASE, (u64)CurrentBlock->Registers.Idtr.Base);
    // __vmx_vmwrite_common(GUEST_IDTR_LIMIT, CurrentBlock->Registers.Idtr.Limit);

    RestoreRegisters(Registers);
}
