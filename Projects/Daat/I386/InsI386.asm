;
;
; Copyright (c) 2015 - 2021 by blindtiger. All rights reserved.
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

RfFrameLength EQU 00080h

; u32
;     NTAPI
;     __ins_sldt(
;         __in u16ptr Selector
;     );

    cPublicProc ___ins_sldt, 1
    
        mov ecx, [esp + 4]
        
        sldt word ptr [ecx]
        
        stdRET ___ins_sldt

    stdENDP ___ins_sldt
    
; u32
;     NTAPI
;     __ins_str(
;         __in u16ptr Selector
;     );

    cPublicProc ___ins_str, 1
    
        mov ecx, [esp + 4]
        
        str word ptr [ecx]
        
        stdRET ___ins_str

    stdENDP ___ins_str
    
; u32
;     NTAPI
;     __ins_sgdt(
;         __in u16ptr Limit
;     );

    cPublicProc ___ins_sgdt, 1
    
        mov ecx, [esp + 4]
        
        sgdt fword ptr [ecx] ; &Descriptor->Limit
        
        stdRET ___ins_sgdt

    stdENDP ___ins_sgdt
    
; u32
;     NTAPI
;     __ins_lgdt(
;         __in u16ptr Limit
;     );

    cPublicProc ___ins_lgdt, 1
    
        mov ecx, [esp + 4]
        
        lgdt fword ptr [ecx] ; &Descriptor->Limit
        
        stdRET ___ins_lgdt

    stdENDP ___ins_lgdt
    
; u32
;     NTAPI
;     __ins_sidt(
;         __in u16ptr Limit
;     );

    cPublicProc ___ins_sidt, 1
    
        mov ecx, [esp + 4]
        
        sidt fword ptr [ecx] ; &Descriptor->Limit
        
        stdRET ___ins_sidt

    stdENDP ___ins_sidt
    
; u32
;     NTAPI
;     __ins_lidt(
;         __in u16ptr Limit
;     );

    cPublicProc ___ins_lidt, 1
    
        mov ecx, [esp + 4]
        
        lidt fword ptr [ecx] ; &Descriptor->Limit
        
        stdRET ___ins_lidt

    stdENDP ___ins_lidt
    
; u32
;     NTAPI
;     __ins_segment_limit(
;         __in u32 Selector
;     );

    cPublicProc ___ins_segment_limit, 1

        mov eax, [esp + 4]

        lsl eax, eax
        
        stdRET ___ins_segment_limit

    stdENDP ___ins_segment_limit
    
; u32
;     NTAPI
;     __ins_segment_ar(
;         __in u32 Selector
;     );

    cPublicProc ___ins_segment_ar, 1

        mov eax, [esp + 4]

        lar eax, eax
        
        shr eax, 8
        and eax, 0f0ffh

        test eax, eax
        jnz @f

        mov eax, 10000h

@@ :
        stdRET ___ins_segment_ar

    stdENDP ___ins_segment_ar
    
; u64
;     NTAPI
;     __ins_readmsr(
;         __in u32 Register
;     );

    cPublicProc ___ins_readmsr, 1

        mov ecx, [esp + 4]

        rdmsr
        
        stdRET ___ins_readmsr

    stdENDP ___ins_readmsr
    
; void
;     NTAPI
;     __ins_writemsr(
;         __in u32 Register,
;         __in u64 Value
;     );

    cPublicProc ___ins_writemsr, 3

        mov ecx, [esp + 4]
        mov eax, [esp + 8]
        mov edx, [esp + 0ch]

        wrmsr
        
        stdRET ___ins_writemsr

    stdENDP ___ins_writemsr
    
; u
;     NTAPI
;     __ins_readcr(
;         __in u32 Register
;     );

    cPublicProc ___ins_readcr, 1
    
        mov ecx, [esp + 4]
        
        test ecx, ecx
        jnz @f

        mov eax, cr0

        jmp __ins_readcr_ret
        
@@ :
        cmp ecx, 2
        jnz @f
        
        mov eax, cr2

        jmp __ins_readcr_ret
        
@@ :
        cmp ecx, 3
        jnz @f
        
        mov eax, cr3

        jmp __ins_readcr_ret
        
@@ :
        cmp ecx, 4
        jnz __ins_readcr_ret
        
        mov eax, cr4

__ins_readcr_ret :
        stdRET ___ins_readcr

    stdENDP ___ins_readcr
    
; void
;     NTAPI
;     __ins_writecr(
;         __in u32 Register,
;         __in u Value
;     );

    cPublicProc ___ins_writecr, 2
    
        mov ecx, [esp + 4]
        mov edx, [esp + 8]
        
        test ecx, ecx
        jnz @f

        mov cr0, edx

        jmp __ins_writecr_ret
        
@@ :
        cmp ecx, 2
        jnz @f

        mov cr2, edx

        jmp __ins_writecr_ret
        
@@ :
        cmp ecx, 3
        jnz @f

        mov cr3, edx

        jmp __ins_writecr_ret
        
@@ :
        cmp ecx, 4
        jnz __ins_writecr_ret

        mov cr4, edx

__ins_writecr_ret :
        stdRET ___ins_writecr

    stdENDP ___ins_writecr
    
; u
;     NTAPI
;     __ins_readdr(
;         __in u32 Register
;     );

    cPublicProc ___ins_readdr, 1
    
        mov ecx, [esp + 4]
        
        test ecx, ecx
        jnz @f

        mov eax, dr0

        jmp __ins_readdr_ret

@@ :
        cmp ecx, 1
        jnz @f
        
        mov eax, dr1

        jmp __ins_readdr_ret
        
@@ :
        cmp ecx, 2
        jnz @f
        
        mov eax, dr2

        jmp __ins_readdr_ret
        
@@ :
        cmp ecx, 3
        jnz @f
        
        mov eax, dr3

        jmp __ins_readdr_ret
        
@@ :
        cmp ecx, 6
        jnz @f
        
        mov eax, dr6

        jmp __ins_readdr_ret
        
@@ :
        cmp ecx, 7
        jnz __ins_readdr_ret
        
        mov eax, dr7
        
__ins_readdr_ret :
        stdRET ___ins_readdr

    stdENDP ___ins_readdr
    
; void
;     NTAPI
;     __ins_writedr(
;         __in u32 Register,
;         __in u Value
;     );

    cPublicProc ___ins_writedr, 2
    
        mov ecx, [esp + 4]
        mov edx, [esp + 8]
        
        test ecx, ecx
        jnz @f

        mov dr0, edx

        jmp __ins_writedr_ret

@@ :
        cmp ecx, 1
        jnz @f

        mov dr1, edx

        jmp __ins_writedr_ret
        
@@ :
        cmp ecx, 2
        jnz @f

        mov dr2, edx

        jmp __ins_writedr_ret
        
@@ :
        cmp ecx, 3
        jnz @f

        mov dr3, edx

        jmp __ins_writedr_ret
        
@@ :
        cmp ecx, 6
        jnz @f

        mov dr6, edx

        jmp __ins_writedr_ret
        
@@ :
        cmp ecx, 7
        jnz __ins_writedr_ret

        mov dr7, edx
        
__ins_writedr_ret :
        stdRET ___ins_writedr

    stdENDP ___ins_writedr
    
; VMX_RESULT
;     NTAPI
;     __ins_invept(
;         __in u32 Type,
;         __in PEPT_DESCRIPTOR Descriptor
;     );

    cPublicProc ___ins_invept, 2
    
        mov ecx, [esp + 4]
        mov edx, [esp + 8]

        db 066h, 00fh, 038h, 080h, 00ah ; invept ecx, xmmword ptr [edx]

        setz al
        setb cl
        adc al, cl
        and eax, 3
        
        stdRET ___ins_invept

    stdENDP ___ins_invept
                      
; VMX_RESULT
;     NTAPI
;     __ins_invvpid(
;         __in u32 Type,
;         __in PEPT_DESCRIPTOR Descriptor
;     );

    cPublicProc ___ins_invvpid, 2
    
        mov ecx, [esp + 4]
        mov edx, [esp + 8]

        db 066h, 00fh, 038h, 081h, 00ah ; invept ecx, xmmword ptr [edx]

        setz al
        setb cl
        adc al, cl
        and eax, 3
        
        stdRET ___ins_invvpid

    stdENDP ___ins_invvpid
    
; VMX_RESULT
;     NTAPI
;     __ins_vmx_on(
;         __in u64 * SupportPhysicalAddress
;     );

    cPublicProc ___ins_vmx_on, 1
    
        mov ecx, [esp + 4]

        db 0f3h, 00fh, 0c7h, 031h ; vmxon qword ptr [ecx]

        setz al
        setb cl
        adc al, cl
        and eax, 3
        
        stdRET ___ins_vmx_on

    stdENDP ___ins_vmx_on
    
; VMX_RESULT
;     NTAPI
;     __ins_vmx_off(
;         void
;     );

    cPublicProc ___ins_vmx_off
    
        db 00fh, 001h, 0c4h ; vmxoff

        setz al
        setb cl
        adc al, cl
        and eax, 3
        
        stdRET ___ins_vmx_off

    stdENDP ___ins_vmx_off
    
; VMX_RESULT
;     NTAPI
;     __ins_vmx_vmclear(
;         __in u64 * VmcsPhysicalAddress
;     );

    cPublicProc ___ins_vmx_vmclear, 1
    
        mov ecx, [esp + 4]
        
        db 066h, 00fh, 0c7h, 031h ; vmxclear qword ptr [ecx]
        
        setz al
        setb cl
        adc al, cl
        and eax, 3
        
        stdRET ___ins_vmx_vmclear

    stdENDP ___ins_vmx_vmclear
    
; VMX_RESULT
;     NTAPI
;     __ins_vmx_vmptrld(
;         __in u64 * VmcsPhysicalAddress
;     );

    cPublicProc ___ins_vmx_vmptrld, 1

        mov ecx, [esp + 4]
        
        db 00fh, 0c7h, 031h ; vmptrld qword ptr [ecx]

        setz al
        setb cl
        adc al, cl
        and eax, 3
        
        stdRET ___ins_vmx_vmptrld

    stdENDP ___ins_vmx_vmptrld
    
; VMX_RESULT
;     NTAPI
;     __ins_vmx_vmptrst(
;         __in u64 * VmcsPhysicalAddress
;     );

    cPublicProc ___ins_vmx_vmptrst, 1

        mov ecx, [esp + 4]
        
        db 00fh, 0c7h, 039h ; vmptrst qword ptr [ecx]

        setz al
        setb cl
        adc al, cl
        and eax, 3
        
        stdRET ___ins_vmx_vmptrst

    stdENDP ___ins_vmx_vmptrst
    
; VMX_RESULT
;     NTAPI
;     __ins_vmx_vmread(
;         __in u Field,
;         __out u * Value
;     );

    cPublicProc ___ins_vmx_vmread, 2

        mov ecx, [esp + 4]
        
        xor eax, eax

        db 00fh, 078h, 0c8h ; vmread eax, ecx
        
        mov edx, [esp + 8]
        mov [edx], eax

        setz al
        setb cl
        adc al, cl
        and eax, 3
        
        stdRET ___ins_vmx_vmread

    stdENDP ___ins_vmx_vmread
    
; VMX_RESULT
;     NTAPI
;     __ins_vmx_vmwrite(
;         __in u Field,
;         __in u Value
;     );

    cPublicProc ___ins_vmx_vmwrite, 2

        mov ecx, [esp + 4]
        mov edx, [esp + 8]
        
        db 00fh, 079h, 0cah ; vmwrite ecx, edx

        setz al
        setb cl
        adc al, cl
        and eax, 3
        
        stdRET ___ins_vmx_vmwrite

    stdENDP ___ins_vmx_vmwrite
    
;VMX_RESULT
;    NTAPI
;    __ins_vmx_vmlaunch(
;        void
;    );

    cPublicProc ___ins_vmx_vmlaunch

        db 00fh, 001h, 0c2h ; vmlaunch

        setz al
        setb cl
        adc al, cl
        and eax, 3
        
        stdRET ___ins_vmx_vmlaunch

    stdENDP ___ins_vmx_vmlaunch
    
; VMX_RESULT
;     NTAPI
;     __vmx_vmresume(
;         void
;     );

    cPublicProc ___vmx_vmresume

        db 00fh, 001h, 0c3h ; vmresume

        setz al
        setb cl
        adc al, cl
        and eax, 3
        
        stdRET ___vmx_vmresume

    stdENDP ___vmx_vmresume
    
; void
;     NTAPI
;     __vmx_trap(
;         void
;     );

    cPublicProc ___vmx_trap
    
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
        pop [ecx].RfEFlags

        lea eax, ___vmx_vmresume
        mov [ecx].RfEip, eax
        
        stdCall ___vm_exit_dispatch, < ecx >

    stdENDP ___vmx_trap
    
; status
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
        mov [ecx].RfEsp, esp
        lea eax, resume
        mov [ecx].RfEip, eax

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
    
; void
;     NTAPI
;     __ins_cpuid(
;         __in s32 Function,
;         __in s32 SubFunction,
;         __out PCPUINFO CpuInfo
;     );

    cPublicProc ___ins_cpuid, 3

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

        stdRET ___ins_cpuid

    stdENDP ___ins_cpuid
    
; void
;     NTAPI
;     __ins_invd(
;         void
;     );

    cPublicProc ___ins_invd

        invd
        
        stdRET ___ins_invd

    stdENDP ___ins_invd
    
; u64
;     NTAPI
;     __ins_rdtsc(
;         void
;     );

    cPublicProc ___ins_rdtsc

        rdtsc
        
        stdRET ___ins_rdtsc

    stdENDP ___ins_rdtsc
    
; u64
;     NTAPI
;     __ins_rdtscp(
;         __out u32ptr Aux
;     );

    cPublicProc ___ins_rdtscp, 1
        
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

        stdRET ___ins_rdtscp

    stdENDP ___ins_rdtscp
    
; void
;     NTAPI
;     __ins_xsetbv(
;         __in u32 Xcr,
;         __in u64 Value
;     );

    cPublicProc ___ins_xsetbv, 3

        mov ecx, [esp + 4]
        mov eax, [esp + 8]
        mov edx, [esp + 0ch]
        
        db 00fh, 001h, 0d1h ; xsetbv
        
        stdRET ___ins_xsetbv

    stdENDP ___ins_xsetbv
    
; void
;     NTAPI
;     ReadCpuFeature(
;         __in PCPU_FEATURE CpuFeature
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
    
; void
;     NTAPI
;     CaptureRegisters(
;         __out PREGISTERS_FRAME Registers
;     );

    cPublicProc _CaptureRegisters, 1
    
        push ecx

        mov ecx, [esp + 8]
        
        pop [ecx].RfEcx
        
        mov [ecx].RfSegEs, es
        mov [ecx].RfSegCs, cs
        mov [ecx].RfSegSs, ss
        mov [ecx].RfSegDs, ds
        mov [ecx].RfSegFs, fs
        mov [ecx].RfSegGs, gs
        
        mov [ecx].RfEax, eax
        mov [ecx].RfEdx, edx
        mov [ecx].RfEbx, ebx
        mov [ecx].RfEsp, esp
        mov [ecx].RfEbp, ebp
        mov [ecx].RfEsi, esi
        mov [ecx].RfEdi, edi

        mov eax, [esp]
        mov [ecx].RfEip, eax
        
        pushfd
        pop [ecx].RfEFlags
        
        stdRET _CaptureRegisters

    stdENDP _CaptureRegisters
    
; void
;     NTAPI
;     RestoreRegisters(
;         __in PREGISTERS_FRAME Registers
;     );

    cPublicProc _RestoreRegisters, 1
    
        mov ecx, [esp + 4]

        mov eax, [ecx].RfEax
        mov edx, [ecx].RfEdx
        
        mov ebx, [ecx].RfEbx
        mov ebp, [ecx].RfEbp
        mov esi, [ecx].RfEsi
        mov edi, [ecx].RfEdi
        
        push [ecx].RfEFlags
        popfd
        
        mov esp, [ecx].RfEsp
        push [ecx].RfEip

        mov ecx, [ecx].RfEcx
        
        ret

    stdENDP _RestoreRegisters
    
_TEXT$00    ends

        end
