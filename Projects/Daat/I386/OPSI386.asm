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

.686p

        .xlist
include ks386.inc
include callconv.inc
        .list
        
        EXTRNP ___vmx_entry, 1
        EXTRNP ___vm_exit_dispatch, 1

_TEXT$00    SEGMENT PAGE 'CODE'

RfReg EQU 00000h
RfEax EQU 00000h
RfAx EQU 00000h
RfAl EQU 00000h
RfAh EQU 00001h
RfEcx EQU 00004h
RfCx EQU 00004h
RfCl EQU 00004h
RfCh EQU 00005h
RfEdx EQU 00008h
RfDx EQU 00008h
RfDl EQU 00008h
RfDh EQU 00009h
RfEbx EQU 000ch
RfBx EQU 000ch
RfBl EQU 000ch
RfBh EQU 000dh
RfEsp EQU 00010h
RfSp EQU 00010h
RfStackPointer EQU 00010h
RfEbp EQU 00014h
RfBp EQU 00014h
RfEsi EQU 00018h
RfSi EQU 00018h
RfEdi EQU 0001ch
RfDi EQU 0001ch
RfEip EQU 00020h
RfProgramCounter EQU 00020h
RfEFlags EQU 00024h
RfDr EQU 00028h
RfDr0 EQU 00028h
RfDr1 EQU 0002ch
RfDr2 EQU 00030h
RfDr3 EQU 00034h
RfDr6 EQU 00040h
RfDr7 EQU 00044h
RfSegEs EQU 00048h
RfSegCs EQU 0004ah
RfSegSs EQU 0004ch
RfSegDs EQU 0004eh
RfSegFs EQU 00050h
RfSegGs EQU 00052h
RfLdtr EQU 00054h
RfTr EQU 00056h
RfGdtr EQU 0005ah
RfIdtr EQU 00062h
RfCr EQU 00068h
RfCr0 EQU 00068h
RfCr2 EQU 00070h
RfCr3 EQU 00074h
RfCr4 EQU 00078h

REGISTERS_FRAME_LENGTH EQU 00080h

; ULONG
;     NTAPI
;     __ops_sldt(
;         __in PUSHORT Selector
;     );

    cPublicProc ___ops_sldt, 1
    
        mov ecx, [esp + 4]
        
        sldt word ptr [ecx]
        
        stdRET ___ops_sldt

    stdENDP ___ops_sldt
    
; ULONG
;     NTAPI
;     __ops_str(
;         __in PUSHORT Selector
;     );

    cPublicProc ___ops_str, 1
    
        mov ecx, [esp + 4]
        
        str word ptr [ecx]
        
        stdRET ___ops_str

    stdENDP ___ops_str
    
; ULONG
;     NTAPI
;     __ops_sgdt(
;         __in PUSHORT Limit
;     );

    cPublicProc ___ops_sgdt, 1
    
        mov ecx, [esp + 4]
        
        sgdt fword ptr [ecx] ; &Descriptor->Limit
        
        stdRET ___ops_sgdt

    stdENDP ___ops_sgdt
    
; ULONG
;     NTAPI
;     __ops_lgdt(
;         __in PUSHORT Limit
;     );

    cPublicProc ___ops_lgdt, 1
    
        mov ecx, [esp + 4]
        
        lgdt fword ptr [ecx] ; &Descriptor->Limit
        
        stdRET ___ops_lgdt

    stdENDP ___ops_lgdt
    
; ULONG
;     NTAPI
;     __ops_sidt(
;         __in PUSHORT Limit
;     );

    cPublicProc ___ops_sidt, 1
    
        mov ecx, [esp + 4]
        
        sidt fword ptr [ecx] ; &Descriptor->Limit
        
        stdRET ___ops_sidt

    stdENDP ___ops_sidt
    
; ULONG
;     NTAPI
;     __ops_lidt(
;         __in PUSHORT Limit
;     );

    cPublicProc ___ops_lidt, 1
    
        mov ecx, [esp + 4]
        
        lidt fword ptr [ecx] ; &Descriptor->Limit
        
        stdRET ___ops_lidt

    stdENDP ___ops_lidt
    
; ULONG
;     NTAPI
;     __ops_segment_limit(
;         __in ULONG Selector
;     );

    cPublicProc ___ops_segment_limit, 1

        mov eax, [esp + 4]

        lsl eax, eax
        
        stdRET ___ops_segment_limit

    stdENDP ___ops_segment_limit
    
; ULONG
;     NTAPI
;     __ops_segment_ar(
;         __in ULONG Selector
;     );

    cPublicProc ___ops_segment_ar, 1

        mov eax, [esp + 4]

        lar eax, eax
        
        shr eax, 8
        and eax, 0f0ffh

        test eax, eax
        jnz @f

        mov eax, 10000h

@@ :
        stdRET ___ops_segment_ar

    stdENDP ___ops_segment_ar
    
; ULONG64
;     NTAPI
;     __ops_readmsr(
;         __in ULONG Register
;     );

    cPublicProc ___ops_readmsr, 1

        mov ecx, [esp + 4]

        rdmsr
        
        stdRET ___ops_readmsr

    stdENDP ___ops_readmsr
    
; VOID
;     NTAPI
;     __ops_writemsr(
;         __in ULONG Register,
;         __in ULONG64 Value
;     );

    cPublicProc ___ops_writemsr, 3

        mov ecx, [esp + 4]
        mov eax, [esp + 8]
        mov edx, [esp + 0ch]

        wrmsr
        
        stdRET ___ops_writemsr

    stdENDP ___ops_writemsr
    
; SIZE_T
;     NTAPI
;     __ops_readcr(
;         __in ULONG Register
;     );

    cPublicProc ___ops_readcr, 1
    
        mov ecx, [esp + 4]
        
        test ecx, ecx
        jnz @f

        mov eax, cr0

        jmp __ops_readcr_ret
        
@@ :
        cmp ecx, 2
        jnz @f
        
        mov eax, cr2

        jmp __ops_readcr_ret
        
@@ :
        cmp ecx, 3
        jnz @f
        
        mov eax, cr3

        jmp __ops_readcr_ret
        
@@ :
        cmp ecx, 4
        jnz __ops_readcr_ret
        
        mov eax, cr4

__ops_readcr_ret :
        stdRET ___ops_readcr

    stdENDP ___ops_readcr
    
; VOID
;     NTAPI
;     __ops_writecr(
;         __in ULONG Register,
;         __in SIZE_T Value
;     );

    cPublicProc ___ops_writecr, 2
    
        mov ecx, [esp + 4]
        mov edx, [esp + 8]
        
        test ecx, ecx
        jnz @f

        mov cr0, edx

        jmp __ops_writecr_ret
        
@@ :
        cmp ecx, 2
        jnz @f

        mov cr2, edx

        jmp __ops_writecr_ret
        
@@ :
        cmp ecx, 3
        jnz @f

        mov cr3, edx

        jmp __ops_writecr_ret
        
@@ :
        cmp ecx, 4
        jnz __ops_writecr_ret

        mov cr4, edx

__ops_writecr_ret :
        stdRET ___ops_writecr

    stdENDP ___ops_writecr
    
; SIZE_T
;     NTAPI
;     __ops_readdr(
;         __in ULONG Register
;     );

    cPublicProc ___ops_readdr, 1
    
        mov ecx, [esp + 4]
        
        test ecx, ecx
        jnz @f

        mov eax, dr0

        jmp __ops_readdr_ret

@@ :
        cmp ecx, 1
        jnz @f
        
        mov eax, dr1

        jmp __ops_readdr_ret
        
@@ :
        cmp ecx, 2
        jnz @f
        
        mov eax, dr2

        jmp __ops_readdr_ret
        
@@ :
        cmp ecx, 3
        jnz @f
        
        mov eax, dr3

        jmp __ops_readdr_ret
        
@@ :
        cmp ecx, 6
        jnz @f
        
        mov eax, dr6

        jmp __ops_readdr_ret
        
@@ :
        cmp ecx, 7
        jnz __ops_readdr_ret
        
        mov eax, dr7
        
__ops_readdr_ret :
        stdRET ___ops_readdr

    stdENDP ___ops_readdr
    
; VOID
;     NTAPI
;     __ops_writedr(
;         __in ULONG Register,
;         __in SIZE_T Value
;     );

    cPublicProc ___ops_writedr, 2
    
        mov ecx, [esp + 4]
        mov edx, [esp + 8]
        
        test ecx, ecx
        jnz @f

        mov dr0, edx

        jmp __ops_writedr_ret

@@ :
        cmp ecx, 1
        jnz @f

        mov dr1, edx

        jmp __ops_writedr_ret
        
@@ :
        cmp ecx, 2
        jnz @f

        mov dr2, edx

        jmp __ops_writedr_ret
        
@@ :
        cmp ecx, 3
        jnz @f

        mov dr3, edx

        jmp __ops_writedr_ret
        
@@ :
        cmp ecx, 6
        jnz @f

        mov dr6, edx

        jmp __ops_writedr_ret
        
@@ :
        cmp ecx, 7
        jnz __ops_writedr_ret

        mov dr7, edx
        
__ops_writedr_ret :
        stdRET ___ops_writedr

    stdENDP ___ops_writedr
    
; VMX_RESULT
;     NTAPI
;     __ops_invept(
;         __in ULONG Type,
;         __in PINVEPT_DESCRIPTOR Descriptor
;     );

    cPublicProc ___ops_invept, 1
    
        mov ecx, [esp + 4]
        mov edx, [esp + 8]

        db 066h, 00fh, 038h, 080h, 00ah ; invept ecx, xmmword ptr [edx]

        setz al
        setb cl
        adc al, cl
        and eax, 3
        
        stdRET ___ops_invept

    stdENDP ___ops_invept
    
; VMX_RESULT
;     NTAPI
;     __vmx_on(
;         __in ULONG64 * VmsSupportPhysicalAddress
;     );

    cPublicProc ___vmx_on, 1
    
        mov ecx, [esp + 4]

        db 0f3h, 00fh, 0c7h, 031h ; vmxon qword ptr [ecx]

        setz al
        setb cl
        adc al, cl
        and eax, 3
        
        stdRET ___vmx_on

    stdENDP ___vmx_on
    
; VMX_RESULT
;     NTAPI
;     __vmx_off(
;         VOID
;     );

    cPublicProc ___vmx_off
    
        db 00fh, 001h, 0c4h ; vmxoff

        setz al
        setb cl
        adc al, cl
        and eax, 3
        
        stdRET ___vmx_off

    stdENDP ___vmx_off
    
; VMX_RESULT
;     NTAPI
;     __vmx_vmclear(
;         __in ULONG64 * VmcsPhysicalAddress
;     );

    cPublicProc ___vmx_vmclear, 1
    
        mov ecx, [esp + 4]
        
        db 066h, 00fh, 0c7h, 031h ; vmxclear qword ptr [ecx]
        
        setz al
        setb cl
        adc al, cl
        and eax, 3
        
        stdRET ___vmx_vmclear

    stdENDP ___vmx_vmclear
    
; VMX_RESULT
;     NTAPI
;     __vmx_vmptrld(
;         __in ULONG64 * VmcsPhysicalAddress
;     );

    cPublicProc ___vmx_vmptrld, 1

        mov ecx, [esp + 4]
        
        db 00fh, 0c7h, 031h ; vmptrld qword ptr [ecx]

        setz al
        setb cl
        adc al, cl
        and eax, 3
        
        stdRET ___vmx_vmptrld

    stdENDP ___vmx_vmptrld
    
; VMX_RESULT
;     NTAPI
;     __vmx_vmptrst(
;         __in ULONG64 * VmcsPhysicalAddress
;     );

    cPublicProc ___vmx_vmptrst, 1

        mov ecx, [esp + 4]
        
        db 00fh, 0c7h, 039h ; vmptrst qword ptr [ecx]

        setz al
        setb cl
        adc al, cl
        and eax, 3
        
        stdRET ___vmx_vmptrst

    stdENDP ___vmx_vmptrst
    
; VMX_RESULT
;     NTAPI
;     __vmx_vmread(
;         __in SIZE_T Field,
;         __out SIZE_T * Value
;     );

    cPublicProc ___vmx_vmread, 2

        mov ecx, [esp + 4]
        
        xor eax, eax

        db 00fh, 078h, 0c8h ; vmread eax, ecx
        
        mov edx, [esp + 8]
        mov [edx], eax

        setz al
        setb cl
        adc al, cl
        and eax, 3
        
        stdRET ___vmx_vmread

    stdENDP ___vmx_vmread
    
; VMX_RESULT
;     NTAPI
;     __vmx_vmwrite(
;         __in SIZE_T Field,
;         __in SIZE_T Value
;     );

    cPublicProc ___vmx_vmwrite, 2

        mov ecx, [esp + 4]
        mov edx, [esp + 8]
        
        db 00fh, 079h, 0cah ; vmwrite ecx, edx

        setz al
        setb cl
        adc al, cl
        and eax, 3
        
        stdRET ___vmx_vmwrite

    stdENDP ___vmx_vmwrite
    
;VMX_RESULT
;    NTAPI
;    __vmx_vmlaunch(
;        VOID
;    );

    cPublicProc ___vmx_vmlaunch

        db 00fh, 001h, 0c2h ; vmlaunch

        setz al
        setb cl
        adc al, cl
        and eax, 3
        
        stdRET ___vmx_vmlaunch

    stdENDP ___vmx_vmlaunch
    
; VMX_RESULT
;     NTAPI
;     __vmx_vmresume(
;         VOID
;     );

    cPublicProc ___vmx_vmresume

        db 00fh, 001h, 0c3h ; vmresume

        setz al
        setb cl
        adc al, cl
        and eax, 3
        
        stdRET ___vmx_vmresume

    stdENDP ___vmx_vmresume
    
; VOID
;     NTAPI
;     __vmx_vmentry(
;         VOID
;     );

    cPublicProc ___vmx_vmentry
    
        mov [esp].RfEax, eax
        mov [esp].RfEcx, ecx
        mov [esp].RfEdx, edx
        mov [esp].RfEbx, ebx
        mov [esp].RfEsp, esp
        mov [esp].RfEbp, ebp
        mov [esp].RfEsi, esi
        mov [esp].RfEdi, edi

        lea ecx, [esp]

        pushfd
        pop RfEFlags [ecx]

        lea eax, ___vmx_vmresume
        mov [ecx].RfEip, eax
        
        stdCall ___vm_exit_dispatch, < ecx >

    stdENDP ___vmx_vmentry
    
; NTSTATUS
;     NTAPI
;     __vmx_start(
;         __in PREGISTERS_FRAME Registers
;     );

    cPublicProc ___vmx_start, 1

        mov edi, edi

        push ebp
        mov ebp, esp
        
        pushfd
        push ebx
        push esi
        push edi
        push ebp
        
        mov ecx, [ebp + 8]

        stdCall _CaptureRegisters, < ecx >
        
        mov ecx, [ebp + 8]
        mov RfEsp [ecx], esp
        lea eax, resume
        mov RfEip [ecx], eax

        stdCall ___vmx_entry, < ecx >

        mov eax, -1

resume :
        pop ebp
        pop edi
        pop esi
        pop ebx
        popfd
        
        mov esp, ebp
        pop ebp

        stdRET ___vmx_start

    stdENDP ___vmx_start
    
; VOID
;     NTAPI
;     __ops_cpuid(
;         __in LONG Function,
;         __in LONG SubFunction,
;         __out PCPUINFO CpuInfo
;     );

    cPublicProc ___ops_cpuid, 3

        mov edi, edi

        push ebp
        mov ebp, esp

        push ebx
        push esi
        
        mov eax, [ebp + 8]
        mov ecx, [ebp + 0ch]

        cpuid
        
        mov esi, [ebp + 10h]

        mov [esi], eax
        mov [esi + 4], ebx
        mov [esi + 8], ecx
        mov [esi + 0ch], edx
        
        pop esi
        pop ebx

        mov esp, ebp
        pop ebp

        stdRET ___ops_cpuid

    stdENDP ___ops_cpuid
    
; VOID
;     NTAPI
;     __ops_invd(
;         VOID
;     );

    cPublicProc ___ops_invd

        invd
        
        stdRET ___ops_invd

    stdENDP ___ops_invd
    
; ULONG64
;     NTAPI
;     __ops_rdtsc(
;         VOID
;     );

    cPublicProc ___ops_rdtsc

        rdtsc
        
        stdRET ___ops_rdtsc

    stdENDP ___ops_rdtsc
    
; ULONG64
;     NTAPI
;     __ops_rdtscp(
;         __out PULONG Aux
;     );

    cPublicProc ___ops_rdtscp, 1
        
        mov edi, edi

        push ebp
        mov ebp, esp

        push esi

        db 00fh, 001h, 0f9h ; rdtscp
        
        mov esi, [ebp + 8]
        mov [esi], ecx

        pop esi

        mov esp, ebp
        pop ebp

        stdRET ___ops_rdtscp

    stdENDP ___ops_rdtscp
    
; VOID
;     NTAPI
;     __ops_xsetbv(
;         __in ULONG Xcr,
;         __in ULONG64 Value
;     );

    cPublicProc ___ops_xsetbv, 3

        mov ecx, [esp + 4]
        mov eax, [esp + 8]
        mov edx, [esp + 0ch]
        
        db 00fh, 001h, 0d1h ; xsetbv
        
        stdRET ___ops_xsetbv

    stdENDP ___ops_xsetbv
    
; VOID
;     NTAPI
;     ReadCpuFeature(
;         __in PCPU_FEATURE Feature
;     );

    cPublicProc _ReadCpuFeature, 1

        mov edi, edi

        push ebp
        mov ebp, esp

        push ebx
        push esi
        
        mov esi, [ebp + 8]

        mov eax, 1
        xor ecx, ecx

        cpuid

        mov [esi], ecx
        mov [esi + 4], edx
        
        mov eax, 7
        xor ecx, ecx

        cpuid

        mov [esi + 8], ebx
        mov [esi + 0ch], ecx
        
        mov eax, 80000001h
        xor ecx, ecx

        cpuid

        mov [esi + 10h], ecx
        mov [esi + 14h], edx

        pop esi
        pop ebx

        mov esp, ebp
        pop ebp

        stdRET _ReadCpuFeature

    stdENDP _ReadCpuFeature
    
; VOID
;     NTAPI
;     CaptureRegisters(
;         __out PREGISTERS_FRAME Registers
;     );

    cPublicProc _CaptureRegisters, 1
    
        push ecx

        mov ecx, [esp + 8]
        
        pop [ecx].RfEcx
        
        mov RfSegEs [ecx], es
        mov RfSegCs [ecx], cs
        mov RfSegSs [ecx], ss
        mov RfSegDs [ecx], ds
        mov RfSegFs [ecx], fs
        mov RfSegGs [ecx], gs
        
        mov [ecx].RfEax, eax
        mov [ecx].RfEdx, edx
        mov [ecx].RfEbx, ebx
        mov [ecx].RfEsp, esp
        mov [ecx].RfEbp, ebp
        mov [ecx].RfEsi, esi
        mov [ecx].RfEdi, edi

        mov eax, [esp]
        mov RfEip [ecx], eax
        
        pushfd
        pop RfEFlags [ecx]
        
        stdRET _CaptureRegisters

    stdENDP _CaptureRegisters
    
; VOID
;     NTAPI
;     RestoreRegisters(
;         __in PREGISTERS_FRAME Registers
;     );

    cPublicProc _RestoreRegisters, 1
    
        mov ecx, [esp + 4]

        mov eax, RfEax [ecx]
        mov edx, RfEdx [ecx]
        
        cli

        mov ebx, RfEbx [ecx]
        mov ebp, RfEbp [ecx]
        mov esi, RfEsi [ecx]
        mov edi, RfEdi [ecx]
        
        push RfEFlags [ecx]
        popfd
        
        mov esp, RfEsp [ecx]
        push RfEip [ecx]

        mov ecx, RfEcx [ecx]
        
        stdRET _RestoreRegisters

    stdENDP _RestoreRegisters
    
_TEXT$00    ends

        end
