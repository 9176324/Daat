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

#include "handler.h"

#include "vcpu.h"
#include "vmx.h"

VOID
NTAPI
__vm_null(
    __inout PCCB Block
)
{
}

VOID
NTAPI
__vm_cpuid(
    __inout PCCB Block
)
{
    CPUINFO CpuInfo = { 0 };

    __ops_cpuid(Block->Registers.Eax, Block->Registers.Ecx, &CpuInfo);

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

VOID
NTAPI
__vm_invd(
    __inout PCCB Block
)
{
    __ops_invd();
}

VOID
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

VOID
NTAPI
__vm_vmx(
    __inout PCCB Block
)
{
    Block->GuestState.GuestRFlags |= EFLAGS_ZF | EFLAGS_CF;
}

VOID
NTAPI
__vm_cr_access(
    __inout PCCB Block
)
{
    ULONG_PTR *Cr = NULL;
    ULONG_PTR *GPReg = NULL;

    Cr = Block->Registers.Cr + Block->GuestState.Qualification.CR.Number;
    GPReg = Block->Registers.Reg + Block->GuestState.Qualification.CR.GPReg;

    if (0 != Block->GuestState.Qualification.CR.Type) {
        // MOV from CR

        *GPReg = *Cr;
    }
    else {
        // MOV to CR

        *Cr = *GPReg;
    }
}

VOID
NTAPI
__vm_dr_access(
    __inout PCCB Block
)
{
    ULONG_PTR * Dr = NULL;
    ULONG_PTR * GPReg = NULL;

    CaptureDebugRegisters(&Block->Registers);

    __vmx_vmread_common(GUEST_DR7, &Block->Registers.Dr7);

    if (Block->Registers.Dr7 & DR7_GD) {
        Block->Registers.Dr7 &= ~DR7_GD;
        Block->Registers.Dr6 |= DR6_BD;

        __vmx_vmwrite_common(GUEST_DR7, Block->Registers.Dr7);

        Block->GuestState.EntryInterruption.Vector = VECTOR_DB;
        Block->GuestState.EntryInterruption.DeliverErrorCode = 0;
        Block->GuestState.EntryInterruption.Type = EXCEPTION;
        Block->GuestState.EntryInterruption.Valid = TRUE;

        Block->GuestState.EntryInterruption.Information &= 0x00000000FFFFFFFF;

        __vmx_vmwrite_common(VMX_ENTRY_INTERRUPT_INFO, Block->GuestState.EntryInterruption.Information);
    }
    else {
        if (4 == Block->GuestState.Qualification.DR.Number ||
            5 == Block->GuestState.Qualification.DR.Number) {
            if (CR4_DE == (Block->Registers.Cr4 &CR4_DE)) {
                Block->GuestState.EntryInterruption.Vector = VECTOR_UD;
                Block->GuestState.EntryInterruption.DeliverErrorCode = 0;
                Block->GuestState.EntryInterruption.Type = EXCEPTION;
                Block->GuestState.EntryInterruption.Valid = TRUE;

                Block->GuestState.EntryInterruption.Information &= 0x00000000FFFFFFFF;

                __vmx_vmwrite_common(VMX_ENTRY_INTERRUPT_INFO, Block->GuestState.EntryInterruption.Information);
            }
        }
        else {
            Dr = Block->Registers.Dr + Block->GuestState.Qualification.DR.Number;
            GPReg = Block->Registers.Reg + Block->GuestState.Qualification.CR.GPReg;

            if (0 != Block->GuestState.Qualification.DR.Direction) {
                // MOV DR -> GPR

                *GPReg = *Dr;
            }
            else {
                // MOV DR <- GPR

                *Dr = *GPReg;

                RestoreDebugRegisters(&Block->Registers);
            }
        }
    }
}

VOID
NTAPI
__vm_rdtscp(
    __inout PCCB Block
)
{
    ULARGE_INTEGER Tsc = { 0 };
    ULONG Aux = 0;

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

VOID
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

PVM_HANDLER Handlers[] = {
    __vm_null, // [VMX_EXIT_INT_EXCEPTION_NMI] An SW interrupt, exception or NMI has occurred
    __vm_null, // [VMX_EXIT_EXT_INTERRUPT] An external interrupt has occurred
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
    __vm_null, // __vm_vmx, // [VMX_EXIT_VMCALL]
    __vm_null, // __vm_vmx, // [VMX_EXIT_VMCLEAR]
    __vm_null, // __vm_vmx, // [VMX_EXIT_VMLAUNCH]
    __vm_null, // __vm_vmx, // [VMX_EXIT_VMPTRLD]
    __vm_null, // __vm_vmx, // [VMX_EXIT_VMPTRST]
    __vm_null, // __vm_vmx, // [VMX_EXIT_VMREAD]
    __vm_null, // __vm_vmx, // [VMX_EXIT_VMRESUME]
    __vm_null, // __vm_vmx, // [VMX_EXIT_VMWRITE]
    __vm_null, // __vm_vmx, // [VMX_EXIT_VMXOFF]
    __vm_null, // __vm_vmx, // [VMX_EXIT_VMXON]
    __vm_cr_access, // [VMX_EXIT_CR_ACCESS] Guest accessed a control register
    __vm_dr_access, // [VMX_EXIT_DR_ACCESS] Guest attempted access to debug register
    __vm_null, // __vm_io, // [VMX_EXIT_IO] Guest attempted I/O
    __vm_null, // [VMX_EXIT_MSR_READ] Guest attempted to read an MSR
    __vm_null, // [VMX_EXIT_MSR_WRITE] Guest attempted to write an MSR
    __vm_null, // [VMX_EXIT_FAILED_VMENTER_GS] VMENTER failed due to guest state
    __vm_null, // [VMX_EXIT_FAILED_VMENTER_MSR] VMENTER failed due to MSR loading
    __vm_null, // [VMX_EXIT_MWAIT]
    __vm_null, // [VMX_EXIT_MTF_EXIT]
    __vm_null, // [VMX_EXIT_MONITOR]
    __vm_null, // [VMX_EXIT_PAUSE]
    __vm_null, // [VMX_EXIT_MACHINE_CHECK]
    __vm_null, // [VMX_EXIT_TPR_BELOW_THRESHOLD]
    __vm_null, // [VMX_EXIT_APIC_ACCESS]
    __vm_null, // [VMX_EXIT_GDT_IDT_ACCESS]
    __vm_null, // [VMX_EXIT_LDT_TR_ACCESS]
    __vm_null, // [VMX_EXIT_EPT_VIOLATION]
    __vm_null, // [VMX_EXIT_EPT_MISCONFIG]
    __vm_null, // [VMX_EXIT_INVEPT]
    __vm_rdtscp, // [VMX_EXIT_RDTSCP]
    __vm_null, // [VMX_EXIT_VMX_TIMER_EXIT]
    __vm_null, // [VMX_EXIT_INVVPID]
    __vm_null, // [VMX_EXIT_WBINVD]
    __vm_xsetbv, // [VMX_EXIT_XSETBV]
    __vm_null, // [VMX_EXIT_APIC_WRITE]
    __vm_null, // [VMX_EXIT_RDRAND]
    __vm_null, // [VMX_EXIT_INVPCID]
    __vm_null, // __vm_vmx, // [VMX_EXIT_VMFUNC]
    __vm_null, // [VMX_EXIT_ENCLS]
    __vm_null, // [VMX_EXIT_RDSEED]
    __vm_null, // [VMX_EXIT_XSAVES]
    __vm_null// [VMX_EXIT_XRSTORS]
};

VOID
NTAPI
__vm_exit_dispatch(
    __inout PREGISTERS_FRAME Registers
)
{
    PCCB CurrentBlock = NULL;

    CurrentBlock = CONTAINING_RECORD(Registers, CCB, Registers);

    __vmx_vmread_common(GUEST_RIP, &CurrentBlock->GuestState.GuestRip);
    __vmx_vmread_common(GUEST_RSP, &CurrentBlock->GuestState.GuestRsp);
    __vmx_vmread_common(GUEST_RFLAGS, &CurrentBlock->GuestState.GuestRFlags);
    __vmx_vmread_common(VM_EXIT_INFO_REASON, &CurrentBlock->GuestState.Reason);
    __vmx_vmread_common(VM_EXIT_INFO_INTERRUPT_INFO, &CurrentBlock->GuestState.ExitInterruption);
    __vmx_vmread_common(VM_EXIT_INFO_IDT_VECTORING, &CurrentBlock->GuestState.IdtVectoring);
    __vmx_vmread_common(VM_EXIT_INFO_QUALIFICATION, &CurrentBlock->GuestState.Qualification);
    __vmx_vmread_common(VM_EXIT_INFO_INSTRUCTION_LENGTH, &CurrentBlock->GuestState.InstructionLength);
    __vmx_vmread_common(VM_EXIT_INFO_INSTRUCTION_INFO, &CurrentBlock->GuestState.Instruction);

    Handlers[CurrentBlock->GuestState.Reason.BasicReason](CurrentBlock);

    CurrentBlock->GuestState.GuestRip += CurrentBlock->GuestState.InstructionLength;

    __vmx_vmwrite_common(GUEST_RFLAGS, CurrentBlock->GuestState.GuestRFlags);
    __vmx_vmwrite_common(GUEST_RSP, CurrentBlock->GuestState.GuestRsp);
    __vmx_vmwrite_common(GUEST_RIP, CurrentBlock->GuestState.GuestRip);

    RestoreRegisters(Registers);
}
