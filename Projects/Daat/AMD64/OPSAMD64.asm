;
;
; Copyright (c) 2018 by blindtiger. All rights reserved.
;
; The contents of this file are subject to the Mozilla Public License Version
; 2.0 (the "License"); you may not use this file except in compliance with
; the License. You may obtain a copy of the License at
; http://www.mozilla.org/MPL/
;
; Software distributed under the License is distributed on an "AS IS" basis,
; WITHOUT WARRANTY OF ANY KIND, either express or implied. SEe the License
; for the specific language governing rights and limitations under the
; License.
;
; The Initial Developer of the Original Code is blindtiger.
;
;

include ksamd64.inc
include macamd64.inc

        __vmx_entry PROTO Registers : PTR
        __vm_exit_dispatch PROTO Registers : PTR

RfP1Home EQU 00000h
RfP2Home EQU 00008h
RfP3Home EQU 00010h
RfP4Home EQU 00018h
RfP5Home EQU 00020h
RfP6Home EQU 00028h
RfReg EQU 00030h
RfRax EQU 00030h
RfEax EQU 00030h
RfAx EQU 00030h
RfAl EQU 00030h
RfAh EQU 00031h
RfRcx EQU 00038h
RfEcx EQU 00038h
RfCx EQU 00038h
RfCl EQU 00038h
RfCh EQU 00039h
RfRdx EQU 00040h
RfEdx EQU 00040h
RfDx EQU 00040h
RfDl EQU 00040h
RfDh EQU 00041h
RfRbx EQU 00048h
RfEbx EQU 00048h
RfBx EQU 00048h
RfBl EQU 00048h
RfBh EQU 00049h
RfRsp EQU 00050h
RfEsp EQU 00050h
RfSp EQU 00050h
RfStackPointer EQU 00050h
RfRbp EQU 00058h
RfEbp EQU 00058h
RfBp EQU 00058h
RfRsi EQU 00060h
RfEsi EQU 00060h
RfSi EQU 00060h
RfRdi EQU 00068h
RfEdi EQU 00068h
RfDi EQU 00068h
RfR8 EQU 00070h
RfR8d EQU 00070h
RfR9 EQU 00078h
RfR9d EQU 00078h
RfR10 EQU 00080h
RfR10d EQU 00080h
RfR11 EQU 00088h
RfR11d EQU 00088h
RfR12 EQU 00090h
RfR12d EQU 00090h
RfR13 EQU 00098h
RfR13d EQU 00098h
RfR14 EQU 000a0h
RfR14d EQU 000a0h
RfR15 EQU 000a8h
RfR15d EQU 000a8h
RfRip EQU 000b0h
RfProgramCounter EQU 000b0h
RfEFlags EQU 000b8h
RfXmm0 EQU 000c0h
RfXmm1 EQU 000d0h
RfXmm2 EQU 000e0h
RfXmm3 EQU 000f0h
RfXmm4 EQU 00100h
RfXmm5 EQU 00110h
RfXmm6 EQU 00120h
RfXmm7 EQU 00130h
RfXmm8 EQU 00140h
RfXmm9 EQU 00150h
RfXmm10 EQU 00160h
RfXmm11 EQU 00170h
RfXmm12 EQU 00180h
RfXmm13 EQU 00190h
RfXmm14 EQU 001a0h
RfXmm15 EQU 001b0h
RfMxCsr EQU 001c0h
RfDr EQU 001c8h
RfDr0 EQU 001c8h
RfDr1 EQU 001d0h
RfDr2 EQU 001d8h
RfDr3 EQU 001e0h
RfDr6 EQU 001f8h
RfDr7 EQU 00200h
RfSegEs EQU 00208h
RfSegCs EQU 0020ah
RfSegSs EQU 0020ch
RfSegDs EQU 0020eh
RfSegFs EQU 00210h
RfSegGs EQU 00212h
RfLdtr EQU 00214h
RfTr EQU 00216h
RfGdtr EQU 0021eh
RfIdtr EQU 0022eh
RfCr EQU 00238h
RfCr0 EQU 00238h
RfCr2 EQU 00248h
RfCr3 EQU 00250h
RfCr4 EQU 00258h
RfCr8 EQU 00278h

REGISTERS_FRAME_LENGTH EQU 00280h

; ULONG
;     NTAPI
;     __ops_sldt(
;         __in PUSHORT Selector 
;     );

        LEAF_ENTRY __ops_sldt, _TEXT$00
        
        sldt word ptr [rcx]
        
        ret
        
        LEAF_END __ops_sldt, _TEXT$00
        
; ULONG
;     NTAPI
;     __ops_str(
;         __in PUSHORT Selector 
;     );

        LEAF_ENTRY __ops_str, _TEXT$00
        
        str word ptr [rcx]
        
        ret
        
        LEAF_END __ops_str, _TEXT$00
        
; ULONG
;     NTAPI
;     __ops_sgdt(
;         __in PUSHORT Limit
;     );

        LEAF_ENTRY __ops_sgdt, _TEXT$00
        
        sgdt fword ptr [rcx] ; &Descriptor->Limit
        
        ret
        
        LEAF_END __ops_sgdt, _TEXT$00
        
; ULONG
;     NTAPI
;     __ops_lgdt(
;         __in PUSHORT Limit
;     );

        LEAF_ENTRY __ops_lgdt, _TEXT$00
        
        lgdt fword ptr [rcx] ; &Descriptor->Limit
        
        ret
        
        LEAF_END __ops_lgdt, _TEXT$00
        
; ULONG
;     NTAPI
;     __ops_sidt(
;         __in PUSHORT Limit
;     );

        LEAF_ENTRY __ops_sidt, _TEXT$00
        
        sidt fword ptr [rcx] ; &Descriptor->Limit
        
        ret
        
        LEAF_END __ops_sidt, _TEXT$00
        
; ULONG
;     NTAPI
;     __ops_lidt(
;         __in PUSHORT Limit
;     );

        LEAF_ENTRY __ops_lidt, _TEXT$00
        
        lidt fword ptr [rcx] ; &Descriptor->Limit
        
        ret
        
        LEAF_END __ops_lidt, _TEXT$00
        
; ULONG
;     NTAPI
;     __ops_segment_limit(
;         __in ULONG Selector
;     );

        LEAF_ENTRY __ops_segment_limit, _TEXT$00

        mov eax, ecx

        lsl eax, eax

        ret

        LEAF_END __ops_segment_limit, _TEXT$00
        
; ULONG
;     NTAPI
;     __ops_segment_ar(
;         __in ULONG Selector
;     );

        LEAF_ENTRY __ops_segment_ar, _TEXT$00

        mov eax, ecx

        lar eax, eax

        shr eax, 8
        and eax, 0f0ffh

        test eax, eax
        jnz @f

        mov eax, 10000h

@@ :
        ret

        LEAF_END __ops_segment_ar, _TEXT$00
        
; ULONG64
;     NTAPI
;     __ops_readmsr(
;         __in ULONG Register
;     );

        LEAF_ENTRY __ops_readmsr, _TEXT$00

        rdmsr

        shl rdx, 20h
        or rax, rdx

        ret

        LEAF_END __ops_readmsr, _TEXT$00
        
; VOID
;     NTAPI
;     __ops_writemsr(
;         __in ULONG Register,
;         __in ULONG64 Value
;     );

        LEAF_ENTRY __ops_writemsr, _TEXT$00
        
        mov eax, edx
        shr rdx, 20h

        wrmsr

        ret

        LEAF_END __ops_writemsr, _TEXT$00
        
; SIZE_T
;     NTAPI
;     __ops_readcr(
;         __in ULONG Register
;     );

        LEAF_ENTRY __ops_readcr, _TEXT$00
        
        test ecx, ecx
        jnz @f

        mov rax, cr0

        jmp __ops_readcr_ret
        
@@ :
        cmp ecx, 2
        jnz @f
        
        mov rax, cr2

        jmp __ops_readcr_ret
        
@@ :
        cmp ecx, 3
        jnz @f
        
        mov rax, cr3

        jmp __ops_readcr_ret
        
@@ :
        cmp ecx, 4
        jnz @f
        
        mov rax, cr4

        jmp __ops_readcr_ret

@@ :
        cmp ecx, 8
        jnz __ops_readcr_ret
        
        mov rax, cr8

__ops_readcr_ret :
        ret

        LEAF_END __ops_readcr, _TEXT$00
        
; VOID
;     NTAPI
;     __ops_writecr(
;         __in ULONG Register,
;         __in SIZE_T Value
;     );

        LEAF_ENTRY __ops_writecr, _TEXT$00
        
        test ecx, ecx
        jnz @f

        mov cr0, rdx

        jmp __ops_writecr_ret
        
@@ :
        cmp ecx, 2
        jnz @f

        mov cr2, rdx

        jmp __ops_writecr_ret
        
@@ :
        cmp ecx, 3
        jnz @f

        mov cr3, rdx

        jmp __ops_writecr_ret
        
@@ :
        cmp ecx, 4
        jnz @f

        mov cr4, rdx

        jmp __ops_writecr_ret

@@ :
        cmp ecx, 8
        jnz __ops_writecr_ret

        mov cr8, rdx

__ops_writecr_ret :
        ret

        LEAF_END __ops_writecr, _TEXT$00
        
; SIZE_T
;     NTAPI
;     __ops_readdr(
;         __in ULONG Register
;     );

        LEAF_ENTRY __ops_readdr, _TEXT$00
        
        test ecx, ecx
        jnz @f

        mov rax, dr0

        jmp __ops_readdr_ret

@@ :
        cmp ecx, 1
        jnz @f
        
        mov rax, dr1

        jmp __ops_readdr_ret
        
@@ :
        cmp ecx, 2
        jnz @f
        
        mov rax, dr2

        jmp __ops_readdr_ret
        
@@ :
        cmp ecx, 3
        jnz @f
        
        mov rax, dr3

        jmp __ops_readdr_ret
        
@@ :
        cmp ecx, 6
        jnz @f
        
        mov rax, dr6

        jmp __ops_readdr_ret
        
@@ :
        cmp ecx, 7
        jnz __ops_readdr_ret
        
        mov rax, dr7
        
__ops_readdr_ret :
        ret

        LEAF_END __ops_readdr, _TEXT$00
        
; VOID
;     NTAPI
;     __ops_writedr(
;         __in ULONG Register,
;         __in SIZE_T Value
;     );

        LEAF_ENTRY __ops_writedr, _TEXT$00
        
        test ecx, ecx
        jnz @f

        mov dr0, rdx

        jmp __ops_writedr_ret

@@ :
        cmp ecx, 1
        jnz @f

        mov dr1, rdx

        jmp __ops_writedr_ret
        
@@ :
        cmp ecx, 2
        jnz @f

        mov dr2, rdx

        jmp __ops_writedr_ret
        
@@ :
        cmp ecx, 3
        jnz @f

        mov dr3, rdx

        jmp __ops_writedr_ret
        
@@ :
        cmp ecx, 6
        jnz @f

        mov dr6, rdx

        jmp __ops_writedr_ret
        
@@ :
        cmp ecx, 7
        jnz __ops_writedr_ret

        mov dr7, rdx
        
__ops_writedr_ret :
        ret

        LEAF_END __ops_writedr, _TEXT$00
        
; VMX_RESULT
;     NTAPI
;     __ops_invept(
;         __in ULONG Type,
;         __in PINVEPT_DESCRIPTOR Descriptor
;     );

        LEAF_ENTRY __ops_invept, _TEXT$00
        
        db 066h, 00fh, 038h, 080h, 00ah ; invept ecx, xmmword ptr [rdx]

        setz al
        setb cl
        adc al, cl
        and rax, 3

        ret

        LEAF_END __ops_invept, _TEXT$00

; VMX_RESULT
;     NTAPI
;     __vmx_on(
;         __in ULONG64 * VmsSupportPhysicalAddress
;     );

        LEAF_ENTRY __vmx_on, _TEXT$00
        
        db 0f3h, 00fh, 0c7h, 031h ; vmxon qword ptr [rcx]

        setz al
        setb cl
        adc al, cl
        and rax, 3

        ret

        LEAF_END __vmx_on, _TEXT$00
        
; VMX_RESULT
;     NTAPI
;     __vmx_off(
;         VOID
;     );

        LEAF_ENTRY __vmx_off, _TEXT$00
        
        db 00fh, 001h, 0c4h ; vmxoff

        setz al
        setb cl
        adc al, cl
        and rax, 3

        ret

        LEAF_END __vmx_off, _TEXT$00
        
; VMX_RESULT
;     NTAPI
;     __vmx_vmclear(
;         __in ULONG64 * VmcsPhysicalAddress
;     );

        LEAF_ENTRY __vmx_vmclear, _TEXT$00
        
        db 066h, 00fh, 0c7h, 031h ; vmclear qword ptr [rcx]

        setz al
        setb cl
        adc al, cl
        and rax, 3

        ret

        LEAF_END __vmx_vmclear, _TEXT$00
        
; VMX_RESULT
;     NTAPI
;     __vmx_vmptrld(
;         __in ULONG64 * VmcsPhysicalAddress
;     );

        LEAF_ENTRY __vmx_vmptrld, _TEXT$00
        
        db 00fh, 0c7h, 031h ; vmptrld qword ptr [rcx]

        setz al
        setb cl
        adc al, cl
        and rax, 3

        ret

        LEAF_END __vmx_vmptrld, _TEXT$00
        
; VMX_RESULT
;     NTAPI
;     __vmx_vmptrst(
;         __in ULONG64 * VmcsPhysicalAddress
;     );

        LEAF_ENTRY __vmx_vmptrst, _TEXT$00
        
        db 00fh, 0c7h, 039h ; vmptrst qword ptr [rcx]

        setz al
        setb cl
        adc al, cl
        and rax, 3

        ret

        LEAF_END __vmx_vmptrst, _TEXT$00
        
; VMX_RESULT
;     NTAPI
;     __vmx_vmread(
;         __in SIZE_T Field,
;         __out SIZE_T * Value
;     );

        LEAF_ENTRY __vmx_vmread, _TEXT$00
        
        xor rax, rax

        db 00fh, 078h, 0c8h ; vmread rax, rcx

        mov [rdx], rax

        setz al
        setb cl
        adc al, cl
        and rax, 3

        ret

        LEAF_END __vmx_vmread, _TEXT$00
        
; VMX_RESULT
;     NTAPI
;     __vmx_vmwrite(
;         __in SIZE_T Field,
;         __in SIZE_T Value
;     );

        LEAF_ENTRY __vmx_vmwrite, _TEXT$00
        
        db 00fh, 079h, 0cah ; vmwrite rcx, rdx

        setz al
        setb cl
        adc al, cl
        and rax, 3

        ret

        LEAF_END __vmx_vmwrite, _TEXT$00
        
;VMX_RESULT
;    NTAPI
;    __vmx_vmlaunch(
;        VOID
;    );

        LEAF_ENTRY __vmx_vmlaunch, _TEXT$00

        db 00fh, 001h, 0c2h ; vmlaunch

        setz al
        setb cl
        adc al, cl
        and rax, 3
        
        ret

        LEAF_END __vmx_vmlaunch, _TEXT$00
    
; VMX_RESULT
;     NTAPI
;     __vmx_vmresume(
;         VOID
;     );

        LEAF_ENTRY __vmx_vmresume, _TEXT$00

        db 00fh, 001h, 0c3h ; vmresume

        setz al
        setb cl
        adc al, cl
        and rax, 3
        
        ret

        LEAF_END __vmx_vmresume, _TEXT$00
    
; VOID
;     NTAPI
;     __vmx_vmentry(
;         VOID
;     );

        LEAF_ENTRY __vmx_vmentry, _TEXT$00
        
        mov RfRax [rsp], rax
        mov RfRcx [rsp], rcx
        mov RfRdx [rsp], rdx
        mov RfRbx [rsp], rbx
        mov RfRsp [rsp], rsp
        mov RfRbp [rsp], rbp
        mov RfRsi [rsp], rsi
        mov RfRdi [rsp], rdi
        mov RfR8 [rsp], r8
        mov RfR9 [rsp], r9
        mov RfR10 [rsp], r10
        mov RfR11 [rsp], r11
        mov RfR12 [rsp], r12
        mov RfR13 [rsp], r13
        mov RfR14 [rsp], r14
        mov RfR15 [rsp], r15
        
        lea rcx, [rsp]

        pushfq
        pop RfEFlags [rcx]
        
        movdqa RfXmm0 [rcx], xmm0
        movdqa RfXmm1 [rcx], xmm1
        movdqa RfXmm2 [rcx], xmm2
        movdqa RfXmm3 [rcx], xmm3
        movdqa RfXmm4 [rcx], xmm4
        movdqa RfXmm5 [rcx], xmm5
        movdqa RfXmm6 [rcx], xmm6
        movdqa RfXmm7 [rcx], xmm7
        movdqa RfXmm8 [rcx], xmm8
        movdqa RfXmm9 [rcx], xmm9
        movdqa RfXmm10 [rcx], xmm10
        movdqa RfXmm11 [rcx], xmm11
        movdqa RfXmm12 [rcx], xmm12
        movdqa RfXmm13 [rcx], xmm13
        movdqa RfXmm14 [rcx], xmm14
        movdqa RfXmm15 [rcx], xmm15

        stmxcsr RfMxCsr [rcx]

        lea rax, __vmx_vmresume
        mov RfRip [rcx], rax
        
        call __vm_exit_dispatch
        
        LEAF_END __vmx_vmentry, _TEXT$00
    
; NTSTATUS
;     NTAPI
;     __vmx_start(
;         __in PREGISTERS_FRAME Registers
;     );

        LEAF_ENTRY __vmx_start, _TEXT$00
        
        sub rsp, (KSTART_FRAME_LENGTH - 8)
        
        pushfq
        push rbx
        push rbp
        push rsi
        push rdi
        push r12
        push r13
        push r14
        push r15
        
        call CaptureRegisters
        
        mov RfRsp [rcx], rsp
        lea rax, resume
        mov RfRip [rcx], rax

        call __vmx_entry

        int 3

        mov rax, -1

resume :
        pop r15
        pop r14
        pop r13
        pop r12
        pop rdi
        pop rsi
        pop rbp
        pop rbx
        popfq

        add rsp,  (KSTART_FRAME_LENGTH - 8)

        ret

        LEAF_END __vmx_start, _TEXT$00
    
; VOID
;     NTAPI
;     __ops_cpuid(
;         __in LONG Function,
;         __in LONG SubFunction,
;         __out PCPUINFO CpuInfo
;     );

        NESTED_ENTRY __ops_cpuid, _TEXT$00

        rex_push_reg rbx

        END_PROLOGUE
        
        mov eax, ecx
        mov ecx, edx

        cpuid

        mov [r8], eax
        mov [r8 + 4], ebx
        mov [r8 + 8], ecx
        mov [r8 + 0ch], edx
        
        pop rbx

        ret

        NESTED_END __ops_cpuid, _TEXT$00
    
; VOID
;     NTAPI
;     __ops_invd(
;         VOID
;     );

        LEAF_ENTRY __ops_invd, _TEXT$00
        
        invd

        ret

        LEAF_END __ops_invd, _TEXT$00
        
; ULONG64
;     NTAPI
;     __ops_rdtsc(
;         VOID
;     );

        LEAF_ENTRY __ops_rdtsc, _TEXT$00
        
        rdtsc
        
        shl rdx, 20h
        or rax, rdx

        ret

        LEAF_END __ops_rdtsc, _TEXT$00
        
; ULONG64
;     NTAPI
;     __ops_rdtscp(
;         __out PULONG Aux
;     );

        LEAF_ENTRY __ops_rdtscp, _TEXT$00
        
        mov r8, rcx

        db 00fh, 001h, 0f9h ; rdtscp
        
        mov [r8], rcx
        shl rdx, 20h
        or rax, rdx

        ret

        LEAF_END __ops_rdtscp, _TEXT$00

; VOID
;     NTAPI
;     __ops_xsetbv(
;         __in ULONG Xcr,
;         __in ULONG64 Value
;     );

        LEAF_ENTRY __ops_xsetbv, _TEXT$00
        
        mov eax, edx
        shr rdx, 20h

        db 00fh, 001h, 0d1h ; xsetbv
        
        ret

        LEAF_END __ops_xsetbv, _TEXT$00

; VOID
;     NTAPI
;     ReadCpuFeature(
;         __in PCPU_FEATURE Feature
;     );

        NESTED_ENTRY ReadCpuFeature, _TEXT$00

        rex_push_reg rbx

        END_PROLOGUE
        
        mov r8, rcx

        mov eax, 1
        xor ecx, ecx

        cpuid

        mov [r8], ecx
        mov [r8 + 4], edx
        
        mov eax, 7
        xor ecx, ecx

        cpuid

        mov [r8 + 8], ebx
        mov [r8 + 0ch], ecx
        
        mov eax, 80000001h
        xor ecx, ecx

        cpuid

        mov [r8 + 10h], ecx
        mov [r8 + 14h], edx

        pop rbx

        ret

        NESTED_END ReadCpuFeature, _TEXT$00
    
; VOID
;     NTAPI
;     CaptureRegisters(
;         __out PREGISTERS_FRAME Registers
;     );

        LEAF_ENTRY CaptureRegisters, _TEXT$00
        
        mov RfSegEs [rcx], es
        mov RfSegCs [rcx], cs
        mov RfSegSs [rcx], ss
        mov RfSegDs [rcx], ds
        mov RfSegFs [rcx], fs
        mov RfSegGs [rcx], gs
        
        mov RfRax [rcx], rax
        mov RfRcx [rcx], rcx
        mov RfRdx [rcx], rdx
        mov RfRbx [rcx], rbx
        mov RfRsp [rcx], rsp
        mov RfRbp [rcx], rbp
        mov RfRsi [rcx], rsi
        mov RfRdi [rcx], rdi
        mov RfR8 [rcx], r8
        mov RfR9 [rcx], r9
        mov RfR10 [rcx], r10
        mov RfR11 [rcx], r11
        mov RfR12 [rcx], r12
        mov RfR13 [rcx], r13
        mov RfR14 [rcx], r14
        mov RfR15 [rcx], r15
        
        mov rax, [rsp]
        mov RfRip [rcx], rax
        
        pushfq
        pop RfEFlags [rcx]
        
        movdqa RfXmm0 [rcx], xmm0
        movdqa RfXmm1 [rcx], xmm1
        movdqa RfXmm2 [rcx], xmm2
        movdqa RfXmm3 [rcx], xmm3
        movdqa RfXmm4 [rcx], xmm4
        movdqa RfXmm5 [rcx], xmm5
        movdqa RfXmm6 [rcx], xmm6
        movdqa RfXmm7 [rcx], xmm7
        movdqa RfXmm8 [rcx], xmm8
        movdqa RfXmm9 [rcx], xmm9
        movdqa RfXmm10 [rcx], xmm10
        movdqa RfXmm11 [rcx], xmm11
        movdqa RfXmm12 [rcx], xmm12
        movdqa RfXmm13 [rcx], xmm13
        movdqa RfXmm14 [rcx], xmm14
        movdqa RfXmm15 [rcx], xmm15

        stmxcsr RfMxCsr [rcx]
        
        ret
        
        LEAF_END CaptureRegisters, _TEXT$00
        
; VOID
;     NTAPI
;     RestoreRegisters(
;         __in PREGISTERS_FRAME Registers
;     );

        LEAF_ENTRY RestoreRegisters, _TEXT$00
        
        mov rax, RfRax [rcx]
        mov rdx, RfRdx [rcx]
        mov r8, RfR8 [rcx]
        mov r9, RfR9 [rcx]
        mov r10, RfR10 [rcx]
        mov r11, RfR11 [rcx]

        movdqa xmm0, RfXmm0 [rcx]
        movdqa xmm1, RfXmm1 [rcx]
        movdqa xmm2, RfXmm2 [rcx]
        movdqa xmm3, RfXmm3 [rcx]
        movdqa xmm4, RfXmm4 [rcx]
        movdqa xmm5, RfXmm5 [rcx]
        
        cli

        mov rbx, RfRbx [rcx]
        mov rbp, RfRbp [rcx]
        mov rsi, RfRsi [rcx]
        mov rdi, RfRdi [rcx]
        mov r12, RfR12 [rcx]
        mov r13, RfR13 [rcx]
        mov r14, RfR14 [rcx]
        mov r15, RfR15 [rcx]

        movdqa xmm6, RfXmm6 [rcx]
        movdqa xmm7, RfXmm7 [rcx]
        movdqa xmm8, RfXmm8 [rcx]
        movdqa xmm9, RfXmm9 [rcx]
        movdqa xmm10, RfXmm10 [rcx]
        movdqa xmm11, RfXmm11 [rcx]
        movdqa xmm12, RfXmm12 [rcx]
        movdqa xmm13, RfXmm13 [rcx]
        movdqa xmm14, RfXmm14 [rcx]
        movdqa xmm15, RfXmm15 [rcx]
        
        push RfEFlags [rcx]
        popfq
        
        ldmxcsr RfMxCsr [rcx]
        
        mov rsp, RfRsp [rcx]
        push RfRip [rcx]
        
        mov rcx, RfRcx [rcx]
        
        ret
        
        LEAF_END RestoreRegisters, _TEXT$00
        
        end
