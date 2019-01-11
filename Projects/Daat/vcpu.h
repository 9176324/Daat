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

#ifndef _VCPU_H_
#define _VCPU_H_

#include "vmx.h"
#include "handler.h"

#ifdef __cplusplus
/* Assume byte packing throughout */
extern "C" {
#endif	/* __cplusplus */

#define CPUID_CACHE_SIZE 6

    typedef union _CPU_FEATURE {
        ULONG Data[CPUID_CACHE_SIZE];
    } CPU_FEATURE, *PCPU_FEATURE;

#define FEAT(feature, index, bit) ((((feature)->Data[index]) >> (bit)) & 1)

    /* Intel SDM Vol. 2A: Table 3-8.
    * Information Returned by CPUID Instruction */

    /*
    * Intel SDM Vol. 2A: Table 3-10.
    * Feature Information Returned in the ECX Register
    * Features for CPUID with EAX=01h stored in ECX
    */

#define X86_FEATURE_SSE3(feature) FEAT(feature, 0, 0) /* 0x00000001 Streaming SIMD Extensions 3 */
#define X86_FEATURE_PCLMULQDQ(feature) FEAT(feature, 0, 1) /* 0x00000002 PCLMULQDQ Instruction */
#define X86_FEATURE_DTES64(feature) FEAT(feature, 0, 2) /* 0x00000004 64-bit DS Area */
#define X86_FEATURE_MONITOR(feature) FEAT(feature, 0, 3) /* 0x00000008 MONITOR/MWAIT Instructions */
#define X86_FEATURE_DS_CPL(feature) FEAT(feature, 0, 4) /* 0x00000010 CPL Qualified Debug Store */
#define X86_FEATURE_VMX(feature) FEAT(feature, 0, 5) /* 0x00000020 Virtual Machine Extensions */
#define X86_FEATURE_SMX(feature) FEAT(feature, 0, 6) /* 0x00000040 Safer Mode Extensions */
#define X86_FEATURE_EIST(feature) FEAT(feature, 0, 7) /* 0x00000080 Enhanced Intel SpeedStep technology */
#define X86_FEATURE_TM2(feature) FEAT(feature, 0, 8) /* 0x00000100 Thermal Monitor 2 */
#define X86_FEATURE_SSSE3(feature) FEAT(feature, 0, 9) /* 0x00000200 Supplemental Streaming SIMD Extensions 3 */
#define X86_FEATURE_CNXT_ID(feature) FEAT(feature, 0, 10) /* 0x00000400 L1 Context ID */
#define X86_FEATURE_SDBG(feature) FEAT(feature, 0, 11) /* 0x00000800 Silicon Debug Interface */
#define X86_FEATURE_FMA(feature) FEAT(feature, 0, 12) /* 0x00001000 Fused Multiply-Add */
#define X86_FEATURE_CMPXCHG16B(feature) FEAT(feature, 0, 13) /* 0x00002000 CMPXCHG16B Instruction */
#define X86_FEATURE_XTPR_UPDATE(feature) FEAT(feature, 0, 14) /* 0x00004000 xTPR Update Control */
#define X86_FEATURE_PDCM(feature) FEAT(feature, 0, 15) /* 0x00008000 Perfmon and Debug Capability */
#define X86_FEATURE_PCID(feature) FEAT(feature, 0, 17) /* 0x00020000 Process-context identifiers */
#define X86_FEATURE_DCA(feature) FEAT(feature, 0, 18) /* 0x00040000 Direct feature access for DMA writes */
#define X86_FEATURE_SSE41(feature) FEAT(feature, 0, 19) /* 0x00080000 Streaming SIMD Extensions 4.1 */
#define X86_FEATURE_SSE42(feature) FEAT(feature, 0, 20) /* 0x00100000 Streaming SIMD Extensions 4.2 */
#define X86_FEATURE_X2APIC(feature) FEAT(feature, 0, 21) /* 0x00200000 x2APIC support */
#define X86_FEATURE_MOVBE(feature) FEAT(feature, 0, 22) /* 0x00400000 MOVBE Instruction */
#define X86_FEATURE_POPCNT(feature) FEAT(feature, 0, 23) /* 0x00800000 POPCNT Instruction */
#define X86_FEATURE_TSC_DEADLINE(feature) FEAT(feature, 0, 24) /* 0x01000000 APIC supports one-shot operation using TSC deadline */
#define X86_FEATURE_AESNI(feature) FEAT(feature, 0, 25) /* 0x02000000 AESNI Extension */
#define X86_FEATURE_XSAVE(feature) FEAT(feature, 0, 26) /* 0x04000000 XSAVE/XRSTOR/XSETBV/XGETBV Instructions and XCR0 */
#define X86_FEATURE_OSXSAVE(feature) FEAT(feature, 0, 27) /* 0x08000000 XSAVE enabled by OS */
#define X86_FEATURE_AVX(feature) FEAT(feature, 0, 28) /* 0x10000000 Advanced Vector Extensions */
#define X86_FEATURE_F16C(feature) FEAT(feature, 0, 29) /* 0x20000000 16-bit Floating-Point Instructions */
#define X86_FEATURE_RDRAND(feature) FEAT(feature, 0, 30) /* 0x40000000 RDRAND Instruction */
#define X86_FEATURE_HYPERVISOR(feature) FEAT(feature, 0, 31) /* 0x80000000 Hypervisor Running */

    /*
    * Intel SDM Vol. 2A: Table 3-11.
    * More on Feature Information Returned in the EDX Register
    * Features for CPUID with EAX=01h stored in EDX
    */

#define X86_FEATURE_FPU(feature) FEAT(feature, 1, 0) /* 0x00000001 Floating Point Unit On-Chip */
#define X86_FEATURE_VME(feature) FEAT(feature, 1, 1) /* 0x00000002 Virtual 8086 Mode Enhancements */
#define X86_FEATURE_DE(feature) FEAT(feature, 1, 2) /* 0x00000004 Debugging Extensions */
#define X86_FEATURE_PSE(feature) FEAT(feature, 1, 3) /* 0x00000008 Page Size Extension */
#define X86_FEATURE_TSC(feature) FEAT(feature, 1, 4) /* 0x00000010 Time Stamp Counter */
#define X86_FEATURE_MSR(feature) FEAT(feature, 1, 5) /* 0x00000020 RDMSR/WRMSR Instructions */
#define X86_FEATURE_PAE(feature) FEAT(feature, 1, 6) /* 0x00000040 Physical Address Extension */
#define X86_FEATURE_MCE(feature) FEAT(feature, 1, 7) /* 0x00000080 Machine Check Exception */
#define X86_FEATURE_CX8(feature) FEAT(feature, 1, 8) /* 0x00000100 CMPXCHG8B Instruction */
#define X86_FEATURE_APIC(feature) FEAT(feature, 1, 9) /* 0x00000200 APIC On-Chip */
#define X86_FEATURE_SEP(feature) FEAT(feature, 1, 11) /* 0x00000800 SYSENTER/SYSEXIT Instructions */
#define X86_FEATURE_MTRR(feature) FEAT(feature, 1, 12) /* 0x00001000 Memory Type Range Registers */
#define X86_FEATURE_PGE(feature) FEAT(feature, 1, 13) /* 0x00002000 Page Global Bit */
#define X86_FEATURE_MCA(feature) FEAT(feature, 1, 14) /* 0x00004000 Machine Check Architecture */
#define X86_FEATURE_CMOV(feature) FEAT(feature, 1, 15) /* 0x00008000 Conditional Move Instructions */
#define X86_FEATURE_PAT(feature) FEAT(feature, 1, 16) /* 0x00010000 Page Attribute Table */
#define X86_FEATURE_PSE36(feature) FEAT(feature, 1, 17) /* 0x00020000 36-Bit Page Size Extension */
#define X86_FEATURE_PSN(feature) FEAT(feature, 1, 18) /* 0x00040000 Processor Serial Number */
#define X86_FEATURE_CLFSH(feature) FEAT(feature, 1, 19) /* 0x00080000 CLFLUSH Instruction */
#define X86_FEATURE_DS(feature) FEAT(feature, 1, 21) /* 0x00200000 Debug Store */
#define X86_FEATURE_ACPI(feature) FEAT(feature, 1, 22) /* 0x00400000 Thermal Monitor and Software Controlled Clock Facilities */
#define X86_FEATURE_MMX(feature) FEAT(feature, 1, 23) /* 0x00800000 Intel MMX Technology */
#define X86_FEATURE_FXSR(feature) FEAT(feature, 1, 24) /* 0x01000000 FXSAVE and FXRSTOR Instructions */
#define X86_FEATURE_SSE(feature) FEAT(feature, 1, 25) /* 0x02000000 Streaming SIMD Extensions */
#define X86_FEATURE_SSE2(feature) FEAT(feature, 1, 26) /* 0x04000000 Streaming SIMD Extensions 2 */
#define X86_FEATURE_SS(feature) FEAT(feature, 1, 27) /* 0x08000000 Self Snoop */
#define X86_FEATURE_HTT(feature) FEAT(feature, 1, 28) /* 0x10000000 Max APIC IDs reserved field is Valid */
#define X86_FEATURE_TM(feature) FEAT(feature, 1, 29) /* 0x20000000 Thermal Monitor */
#define X86_FEATURE_PBE(feature) FEAT(feature, 1, 31) /* 0x80000000 Pending Break Enable */

    /*
    * Intel SDM Vol. 2A: Table 3-8. Information Returned by CPUID Instruction
    * Structured Extended Feature Flags Enumeration Leaf
    * Features for CPUID with EAX=07h, ECX=00h stored in EBX
    */

#define X86_FEATURE_FSGSBASE(feature) FEAT(feature, 2, 0) /* 0x00000001 RDFSBASE/RDGSBASE/WRFSBASE/WRGSBASE supported */
#define X86_FEATURE_TSC_ADJUST(feature) FEAT(feature, 2, 1) /* 0x00000002 MSR IA32_TSC_ADJUST supported */
#define X86_FEATURE_SGX(feature) FEAT(feature, 2, 2) /* 0x00000004 Software Guard Extensions */
#define X86_FEATURE_BMI1(feature) FEAT(feature, 2, 3) /* 0x00000008 Bit Manipulation Instruction Set 1 */
#define X86_FEATURE_HLE(feature) FEAT(feature, 2, 4) /* 0x00000010 Transactional Synchronization Extensions */
#define X86_FEATURE_AVX2(feature) FEAT(feature, 2, 5) /* 0x00000020 Advanced Vector Extensions 2 */
#define X86_FEATURE_SMEP(feature) FEAT(feature, 2, 7) /* 0x00000080 Supervisor-Mode Execution Prevention */
#define X86_FEATURE_BMI2(feature) FEAT(feature, 2, 8) /* 0x00000100 Bit Manipulation Instruction Set 2 */
#define X86_FEATURE_INVPCID(feature) FEAT(feature, 2, 10) /* 0x00000400 INVPCID instruction */
#define X86_FEATURE_RTM(feature) FEAT(feature, 2, 11) /* 0x00000800 Transactional Synchronization Extensions */
#define X86_FEATURE_RDT_M(feature) FEAT(feature, 2, 12) /* 0x00001000 Resource Director Technology Monitoring */
#define X86_FEATURE_MPX(feature) FEAT(feature, 2, 14) /* 0x00004000 Memory Protection Extensions */
#define X86_FEATURE_RDT_A(feature) FEAT(feature, 2, 15) /* 0x00008000 Resource Director Technology Allocation */
#define X86_FEATURE_AVX512F(feature) FEAT(feature, 2, 16) /* 0x00010000 AVX-512 Foundation */
#define X86_FEATURE_AVX512DQ(feature) FEAT(feature, 2, 17) /* 0x00020000 AVX-512 Doubleword and Quadword Instructions */
#define X86_FEATURE_RDSEED(feature) FEAT(feature, 2, 18) /* 0x00040000 RDSEED Instruction */
#define X86_FEATURE_ADX(feature) FEAT(feature, 2, 19) /* 0x00080000 Multi-Precision Add-Carry Instruction Extensions */
#define X86_FEATURE_SMAP(feature) FEAT(feature, 2, 20) /* 0x00100000 Supervisor Mode Access Prevention */
#define X86_FEATURE_AVX512_IFMA(feature) FEAT(feature, 2, 21) /* 0x00200000 AVX-512 Integer Fused Multiply-Add Instructions */
#define X86_FEATURE_CLFLUSHOPT(feature) FEAT(feature, 2, 23) /* 0x00800000 CLFLUSHOPT Instruction */
#define X86_FEATURE_CLWB(feature) FEAT(feature, 2, 24) /* 0x01000000 CLWB Instruction */
#define X86_FEATURE_AVX512PF(feature) FEAT(feature, 2, 26) /* 0x04000000 AVX-512 Prefetch Instructions */
#define X86_FEATURE_AVX512ER(feature) FEAT(feature, 2, 27) /* 0x08000000 AVX-512 Exponential and Reciprocal Instructions */
#define X86_FEATURE_AVX512CD(feature) FEAT(feature, 2, 28) /* 0x10000000 AVX-512 Conflict Detection Instructions*/
#define X86_FEATURE_SHA(feature) FEAT(feature, 2, 29) /* 0x20000000 SHA Extension */
#define X86_FEATURE_AVX512BW(feature) FEAT(feature, 2, 30) /* 0x40000000 AVX-512 Byte and Word Instructions */
#define X86_FEATURE_AVX512VL(feature) FEAT(feature, 2, 31) /* 0x80000000 AVX-512 Vector Length Extensions */

    /*
    * Intel SDM Vol. 2A: Table 3-8. Information Returned by CPUID Instruction
    * Structured Extended Feature Flags Enumeration Leaf
    * Features for CPUID with EAX=07h, ECX=00h stored in ECX
    */

#define X86_FEATURE_PREFETCHWT1(feature) FEAT(feature, 3, 0) /* 0x00000001 PREFETCHWT1 Instruction */
#define X86_FEATURE_AVX512_VBMI(feature) FEAT(feature, 3, 1) /* 0x00000002 AVX-512 Vector Bit Manipulation Instructions */
#define X86_FEATURE_UMIP(feature) FEAT(feature, 3, 2) /* 0x00000004 User-Mode Instruction Prevention */
#define X86_FEATURE_PKU(feature) FEAT(feature, 3, 3) /* 0x00000008 Protection Keys for User-mode pages */
#define X86_FEATURE_OSPKE(feature) FEAT(feature, 3, 4) /* 0x00000010 PKU enabled by OS */
#define X86_FEATURE_RDPID(feature) FEAT(feature, 3, 22) /* 0x00400000 RDPID Instruction and IA32_TSC_AUX MSR */

    /*
    * Intel SDM Vol. 2A: Table 3-8. Information Returned by CPUID Instruction
    * Extended Function CPUID Information
    * Features for CPUID with EAX=80000001h stored in ECX
    */

#define X86_FEATURE_LAHF(feature) FEAT(feature, 4, 0) /* 0x00000001 LAHF/SAHF Instructions */
#define X86_FEATURE_PREFETCHW(feature) FEAT(feature, 4, 8) /* 0x00000100 PREFETCH/PREFETCHW instructions */

    /*
    * Intel SDM Vol. 2A: Table 3-8. Information Returned by CPUID Instruction
    * Extended Function CPUID Information
    * Features for CPUID with EAX=80000001h stored in EDX
    */

#define X86_FEATURE_SYSCALL(feature) FEAT(feature, 5, 11) /* 0x00000800 SYSCALL/SYSRET Instructions */
#define X86_FEATURE_NX(feature) FEAT(feature, 5, 20) /* 0x00100000 No-Execute Bit */
#define X86_FEATURE_PDPE1GB(feature) FEAT(feature, 5, 26) /* 0x04000000 Gibibyte pages */
#define X86_FEATURE_RDTSCP(feature) FEAT(feature, 5, 27) /* 0x08000000 RDTSCP Instruction */
#define X86_FEATURE_EM64T(feature) FEAT(feature, 5, 29) /* 0x20000000 Long Mode */

#ifndef _WIN64
    typedef struct DECLSPEC_ALIGN(16) _REGISTERS_FRAME {
        //
        // Integer registers.
        //

        union {
            ULONG Reg[8];

            struct {
                union {
                    ULONG Eax;
                    USHORT Ax;

                    struct {
                        UCHAR Al;
                        UCHAR Ah;
                    };
                };

                union {
                    ULONG Ecx;
                    USHORT Cx;

                    struct {
                        UCHAR Cl;
                        UCHAR Ch;
                    };
                };

                union {
                    ULONG Edx;
                    USHORT Dx;

                    struct {
                        UCHAR Dl;
                        UCHAR Dh;
                    };
                };

                union {
                    ULONG Ebx;
                    USHORT Bx;

                    struct {
                        UCHAR Bl;
                        UCHAR Bh;
                    };
                };

                union {
                    ULONG Esp;
                    USHORT Sp;
                    ULONG StackPointer;
                };

                union {
                    ULONG Ebp;
                    USHORT Bp;
                };

                union {
                    ULONG Esi;
                    USHORT Si;
                };

                union {
                    ULONG Edi;
                    USHORT Di;
                };
            };
        };

        //
        // Program counter.
        //

        union {
            ULONG Eip;
            ULONG ProgramCounter;
        };

        //
        // Control flags.
        //

        ULONG EFlags;

        //
        // Debug registers
        //

        union {
            ULONG Dr[8];

            struct {
                ULONG Dr0;
                ULONG Dr1;
                ULONG Dr2;
                ULONG Dr3;
                ULONG Dr4;
                ULONG Dr5;
                ULONG Dr6;
                ULONG Dr7;
            };
        };

        //
        // Segment Registers and processor flags.
        //

        USHORT SegEs;
        USHORT SegCs;
        USHORT SegSs;
        USHORT SegDs;
        USHORT SegFs;
        USHORT SegGs;

        USHORT Ldtr;
        USHORT Tr;

        KDESCRIPTOR Gdtr;
        KDESCRIPTOR Idtr;

        //
        // Special registers.
        //

        union {
            ULONG Cr[5];

            struct {
                ULONG Cr0;
                ULONG NOTHING : 32;
                ULONG Cr2;
                ULONG Cr3;
                ULONG Cr4;
            };
        };
    } REGISTERS_FRAME, *PREGISTERS_FRAME;

    C_ASSERT(FIELD_OFFSET(REGISTERS_FRAME, Reg) == 0);
    C_ASSERT(FIELD_OFFSET(REGISTERS_FRAME, Eax) == 0);
    C_ASSERT(FIELD_OFFSET(REGISTERS_FRAME, Ax) == 0);
    C_ASSERT(FIELD_OFFSET(REGISTERS_FRAME, Al) == 0);
    C_ASSERT(FIELD_OFFSET(REGISTERS_FRAME, Ah) == 1);
    C_ASSERT(FIELD_OFFSET(REGISTERS_FRAME, Ecx) == 4);
    C_ASSERT(FIELD_OFFSET(REGISTERS_FRAME, Cx) == 4);
    C_ASSERT(FIELD_OFFSET(REGISTERS_FRAME, Cl) == 4);
    C_ASSERT(FIELD_OFFSET(REGISTERS_FRAME, Ch) == 5);
    C_ASSERT(FIELD_OFFSET(REGISTERS_FRAME, Edx) == 0x08);
    C_ASSERT(FIELD_OFFSET(REGISTERS_FRAME, Dx) == 8);
    C_ASSERT(FIELD_OFFSET(REGISTERS_FRAME, Dl) == 8);
    C_ASSERT(FIELD_OFFSET(REGISTERS_FRAME, Dh) == 9);
    C_ASSERT(FIELD_OFFSET(REGISTERS_FRAME, Ebx) == 0xc);
    C_ASSERT(FIELD_OFFSET(REGISTERS_FRAME, Bx) == 0xc);
    C_ASSERT(FIELD_OFFSET(REGISTERS_FRAME, Bl) == 0xc);
    C_ASSERT(FIELD_OFFSET(REGISTERS_FRAME, Bh) == 0xd);
    C_ASSERT(FIELD_OFFSET(REGISTERS_FRAME, Esp) == 0x10);
    C_ASSERT(FIELD_OFFSET(REGISTERS_FRAME, Sp) == 0x10);
    C_ASSERT(FIELD_OFFSET(REGISTERS_FRAME, StackPointer) == 0x10);
    C_ASSERT(FIELD_OFFSET(REGISTERS_FRAME, Ebp) == 0x14);
    C_ASSERT(FIELD_OFFSET(REGISTERS_FRAME, Bp) == 0x14);
    C_ASSERT(FIELD_OFFSET(REGISTERS_FRAME, Esi) == 0x18);
    C_ASSERT(FIELD_OFFSET(REGISTERS_FRAME, Si) == 0x18);
    C_ASSERT(FIELD_OFFSET(REGISTERS_FRAME, Edi) == 0x1c);
    C_ASSERT(FIELD_OFFSET(REGISTERS_FRAME, Di) == 0x1c);
    C_ASSERT(FIELD_OFFSET(REGISTERS_FRAME, Eip) == 0x20);
    C_ASSERT(FIELD_OFFSET(REGISTERS_FRAME, ProgramCounter) == 0x20);
    C_ASSERT(FIELD_OFFSET(REGISTERS_FRAME, EFlags) == 0x24);
    C_ASSERT(FIELD_OFFSET(REGISTERS_FRAME, Dr) == 0x28);
    C_ASSERT(FIELD_OFFSET(REGISTERS_FRAME, Dr0) == 0x28);
    C_ASSERT(FIELD_OFFSET(REGISTERS_FRAME, Dr1) == 0x2c);
    C_ASSERT(FIELD_OFFSET(REGISTERS_FRAME, Dr2) == 0x30);
    C_ASSERT(FIELD_OFFSET(REGISTERS_FRAME, Dr3) == 0x34);
    C_ASSERT(FIELD_OFFSET(REGISTERS_FRAME, Dr4) == 0x38);
    C_ASSERT(FIELD_OFFSET(REGISTERS_FRAME, Dr5) == 0x3c);
    C_ASSERT(FIELD_OFFSET(REGISTERS_FRAME, Dr6) == 0x40);
    C_ASSERT(FIELD_OFFSET(REGISTERS_FRAME, Dr7) == 0x44);
    C_ASSERT(FIELD_OFFSET(REGISTERS_FRAME, SegEs) == 0x48);
    C_ASSERT(FIELD_OFFSET(REGISTERS_FRAME, SegCs) == 0x4a);
    C_ASSERT(FIELD_OFFSET(REGISTERS_FRAME, SegSs) == 0x4c);
    C_ASSERT(FIELD_OFFSET(REGISTERS_FRAME, SegDs) == 0x4e);
    C_ASSERT(FIELD_OFFSET(REGISTERS_FRAME, SegFs) == 0x50);
    C_ASSERT(FIELD_OFFSET(REGISTERS_FRAME, SegGs) == 0x52);
    C_ASSERT(FIELD_OFFSET(REGISTERS_FRAME, Ldtr) == 0x54);
    C_ASSERT(FIELD_OFFSET(REGISTERS_FRAME, Tr) == 0x56);
    C_ASSERT(FIELD_OFFSET(REGISTERS_FRAME, Gdtr.Limit) == 0x5a);
    C_ASSERT(FIELD_OFFSET(REGISTERS_FRAME, Idtr.Limit) == 0x62);
    C_ASSERT(FIELD_OFFSET(REGISTERS_FRAME, Cr) == 0x68);
    C_ASSERT(FIELD_OFFSET(REGISTERS_FRAME, Cr0) == 0x68);
    C_ASSERT(FIELD_OFFSET(REGISTERS_FRAME, Cr2) == 0x70);
    C_ASSERT(FIELD_OFFSET(REGISTERS_FRAME, Cr3) == 0x74);
    C_ASSERT(FIELD_OFFSET(REGISTERS_FRAME, Cr4) == 0x78);

    C_ASSERT(sizeof(REGISTERS_FRAME) == 0x80);
#else
    typedef struct DECLSPEC_ALIGN(16) _REGISTERS_FRAME {
        //
        // Register parameter home addresses.
        //
        // N.B. These fields are for convenience - they could be used to extend the
        // context record in the future.
        //

        ULONG64 P1Home;
        ULONG64 P2Home;
        ULONG64 P3Home;
        ULONG64 P4Home;
        ULONG64 P5Home;
        ULONG64 P6Home;

        //
        // Integer registers.
        //

        union {
            ULONG64 Reg[16];

            struct {
                union {
                    ULONG64 Rax;
                    ULONG Eax;
                    USHORT Ax;

                    struct {
                        UCHAR Al;
                        UCHAR Ah;
                    };
                };

                union {
                    ULONG64 Rcx;
                    ULONG Ecx;
                    USHORT Cx;

                    struct {
                        UCHAR Cl;
                        UCHAR Ch;
                    };
                };

                union {
                    ULONG64 Rdx;
                    ULONG Edx;
                    USHORT Dx;

                    struct {
                        UCHAR Dl;
                        UCHAR Dh;
                    };
                };

                union {
                    ULONG64 Rbx;
                    ULONG Ebx;
                    USHORT Bx;

                    struct {
                        UCHAR Bl;
                        UCHAR Bh;
                    };
                };

                union {
                    ULONG64 Rsp;
                    ULONG Esp;
                    USHORT Sp;
                    ULONG64 StackPointer;
                };

                union {
                    ULONG64 Rbp;
                    ULONG Ebp;
                    USHORT Bp;
                };

                union {
                    ULONG64 Rsi;
                    ULONG Esi;
                    USHORT Si;
                };

                union {
                    ULONG64 Rdi;
                    ULONG Edi;
                    USHORT Di;
                };

                union {
                    ULONG64 R8;
                    ULONG R8d;
                };

                union {
                    ULONG64 R9;
                    ULONG R9d;
                };

                union {
                    ULONG64 R10;
                    ULONG R10d;
                };

                union {
                    ULONG64 R11;
                    ULONG R11d;
                };

                union {
                    ULONG64 R12;
                    ULONG R12d;
                };

                union {
                    ULONG64 R13;
                    ULONG R13d;
                };

                union {
                    ULONG64 R14;
                    ULONG R14d;
                };

                union {
                    ULONG64 R15;
                    ULONG R15d;
                };
            };
        };

        //
        // Program counter.
        //

        union {
            ULONG64 Rip;
            ULONG64 ProgramCounter;
        };

        //
        // Control flags.
        //

        ULONG EFlags;

        //
        // Floating point state.
        //

        union {
            M128A Xmm[16];

            struct {
                M128A Xmm0;
                M128A Xmm1;
                M128A Xmm2;
                M128A Xmm3;
                M128A Xmm4;
                M128A Xmm5;
                M128A Xmm6;
                M128A Xmm7;
                M128A Xmm8;
                M128A Xmm9;
                M128A Xmm10;
                M128A Xmm11;
                M128A Xmm12;
                M128A Xmm13;
                M128A Xmm14;
                M128A Xmm15;
            };
        };

        //
        // Control flags.
        //

        ULONG MxCsr;

        //
        // Debug registers
        //

        union {
            ULONG64 Dr[8];

            struct {
                ULONG64 Dr0;
                ULONG64 Dr1;
                ULONG64 Dr2;
                ULONG64 Dr3;
                ULONG64 Dr4;
                ULONG64 Dr5;
                ULONG64 Dr6;
                ULONG64 Dr7;
            };
        };

        //
        // Segment Registers and processor flags.
        //

        USHORT SegEs;
        USHORT SegCs;
        USHORT SegSs;
        USHORT SegDs;
        USHORT SegFs;
        USHORT SegGs;

        USHORT Ldtr;
        USHORT Tr;

        KDESCRIPTOR Gdtr;
        KDESCRIPTOR Idtr;

        //
        // Special registers.
        //

        union {
            ULONG64 Cr[9];

            struct {
                ULONG64 Cr0;
                ULONG64 NOTHING : 64;
                ULONG64 Cr2;
                ULONG64 Cr3;
                ULONG64 Cr4;
                ULONG64 NOTHING : 64;
                ULONG64 NOTHING : 64;
                ULONG64 NOTHING : 64;
                ULONG64 Cr8;
            };
        };
    } REGISTERS_FRAME, *PREGISTERS_FRAME;

    C_ASSERT(FIELD_OFFSET(REGISTERS_FRAME, P1Home) == 0);
    C_ASSERT(FIELD_OFFSET(REGISTERS_FRAME, P2Home) == 8);
    C_ASSERT(FIELD_OFFSET(REGISTERS_FRAME, P3Home) == 0x10);
    C_ASSERT(FIELD_OFFSET(REGISTERS_FRAME, P4Home) == 0x18);
    C_ASSERT(FIELD_OFFSET(REGISTERS_FRAME, P5Home) == 0x20);
    C_ASSERT(FIELD_OFFSET(REGISTERS_FRAME, P6Home) == 0x28);
    C_ASSERT(FIELD_OFFSET(REGISTERS_FRAME, Reg) == 0x30);
    C_ASSERT(FIELD_OFFSET(REGISTERS_FRAME, Rax) == 0x30);
    C_ASSERT(FIELD_OFFSET(REGISTERS_FRAME, Eax) == 0x30);
    C_ASSERT(FIELD_OFFSET(REGISTERS_FRAME, Ax) == 0x30);
    C_ASSERT(FIELD_OFFSET(REGISTERS_FRAME, Al) == 0x30);
    C_ASSERT(FIELD_OFFSET(REGISTERS_FRAME, Ah) == 0x31);
    C_ASSERT(FIELD_OFFSET(REGISTERS_FRAME, Rcx) == 0x38);
    C_ASSERT(FIELD_OFFSET(REGISTERS_FRAME, Ecx) == 0x38);
    C_ASSERT(FIELD_OFFSET(REGISTERS_FRAME, Cx) == 0x38);
    C_ASSERT(FIELD_OFFSET(REGISTERS_FRAME, Cl) == 0x38);
    C_ASSERT(FIELD_OFFSET(REGISTERS_FRAME, Ch) == 0x39);
    C_ASSERT(FIELD_OFFSET(REGISTERS_FRAME, Rdx) == 0x40);
    C_ASSERT(FIELD_OFFSET(REGISTERS_FRAME, Edx) == 0x40);
    C_ASSERT(FIELD_OFFSET(REGISTERS_FRAME, Dx) == 0x40);
    C_ASSERT(FIELD_OFFSET(REGISTERS_FRAME, Dl) == 0x40);
    C_ASSERT(FIELD_OFFSET(REGISTERS_FRAME, Dh) == 0x41);
    C_ASSERT(FIELD_OFFSET(REGISTERS_FRAME, Rbx) == 0x48);
    C_ASSERT(FIELD_OFFSET(REGISTERS_FRAME, Ebx) == 0x48);
    C_ASSERT(FIELD_OFFSET(REGISTERS_FRAME, Bx) == 0x48);
    C_ASSERT(FIELD_OFFSET(REGISTERS_FRAME, Bl) == 0x48);
    C_ASSERT(FIELD_OFFSET(REGISTERS_FRAME, Bh) == 0x49);
    C_ASSERT(FIELD_OFFSET(REGISTERS_FRAME, Rsp) == 0x50);
    C_ASSERT(FIELD_OFFSET(REGISTERS_FRAME, Esp) == 0x50);
    C_ASSERT(FIELD_OFFSET(REGISTERS_FRAME, Sp) == 0x50);
    C_ASSERT(FIELD_OFFSET(REGISTERS_FRAME, StackPointer) == 0x50);
    C_ASSERT(FIELD_OFFSET(REGISTERS_FRAME, Rbp) == 0x58);
    C_ASSERT(FIELD_OFFSET(REGISTERS_FRAME, Ebp) == 0x58);
    C_ASSERT(FIELD_OFFSET(REGISTERS_FRAME, Bp) == 0x58);
    C_ASSERT(FIELD_OFFSET(REGISTERS_FRAME, Rsi) == 0x60);
    C_ASSERT(FIELD_OFFSET(REGISTERS_FRAME, Esi) == 0x60);
    C_ASSERT(FIELD_OFFSET(REGISTERS_FRAME, Si) == 0x60);
    C_ASSERT(FIELD_OFFSET(REGISTERS_FRAME, Rdi) == 0x68);
    C_ASSERT(FIELD_OFFSET(REGISTERS_FRAME, Edi) == 0x68);
    C_ASSERT(FIELD_OFFSET(REGISTERS_FRAME, Di) == 0x68);
    C_ASSERT(FIELD_OFFSET(REGISTERS_FRAME, R8) == 0x70);
    C_ASSERT(FIELD_OFFSET(REGISTERS_FRAME, R8d) == 0x70);
    C_ASSERT(FIELD_OFFSET(REGISTERS_FRAME, R9) == 0x78);
    C_ASSERT(FIELD_OFFSET(REGISTERS_FRAME, R9d) == 0x78);
    C_ASSERT(FIELD_OFFSET(REGISTERS_FRAME, R10) == 0x80);
    C_ASSERT(FIELD_OFFSET(REGISTERS_FRAME, R10d) == 0x80);
    C_ASSERT(FIELD_OFFSET(REGISTERS_FRAME, R11) == 0x88);
    C_ASSERT(FIELD_OFFSET(REGISTERS_FRAME, R11d) == 0x88);
    C_ASSERT(FIELD_OFFSET(REGISTERS_FRAME, R12) == 0x90);
    C_ASSERT(FIELD_OFFSET(REGISTERS_FRAME, R12d) == 0x90);
    C_ASSERT(FIELD_OFFSET(REGISTERS_FRAME, R13) == 0x98);
    C_ASSERT(FIELD_OFFSET(REGISTERS_FRAME, R13d) == 0x98);
    C_ASSERT(FIELD_OFFSET(REGISTERS_FRAME, R14) == 0xa0);
    C_ASSERT(FIELD_OFFSET(REGISTERS_FRAME, R14d) == 0xa0);
    C_ASSERT(FIELD_OFFSET(REGISTERS_FRAME, R15) == 0xa8);
    C_ASSERT(FIELD_OFFSET(REGISTERS_FRAME, R15d) == 0xa8);
    C_ASSERT(FIELD_OFFSET(REGISTERS_FRAME, Rip) == 0xb0);
    C_ASSERT(FIELD_OFFSET(REGISTERS_FRAME, ProgramCounter) == 0xb0);
    C_ASSERT(FIELD_OFFSET(REGISTERS_FRAME, EFlags) == 0xb8);
    C_ASSERT(FIELD_OFFSET(REGISTERS_FRAME, Xmm0) == 0xc0);
    C_ASSERT(FIELD_OFFSET(REGISTERS_FRAME, Xmm1) == 0xd0);
    C_ASSERT(FIELD_OFFSET(REGISTERS_FRAME, Xmm2) == 0xe0);
    C_ASSERT(FIELD_OFFSET(REGISTERS_FRAME, Xmm3) == 0xf0);
    C_ASSERT(FIELD_OFFSET(REGISTERS_FRAME, Xmm4) == 0x100);
    C_ASSERT(FIELD_OFFSET(REGISTERS_FRAME, Xmm5) == 0x110);
    C_ASSERT(FIELD_OFFSET(REGISTERS_FRAME, Xmm6) == 0x120);
    C_ASSERT(FIELD_OFFSET(REGISTERS_FRAME, Xmm7) == 0x130);
    C_ASSERT(FIELD_OFFSET(REGISTERS_FRAME, Xmm8) == 0x140);
    C_ASSERT(FIELD_OFFSET(REGISTERS_FRAME, Xmm9) == 0x150);
    C_ASSERT(FIELD_OFFSET(REGISTERS_FRAME, Xmm10) == 0x160);
    C_ASSERT(FIELD_OFFSET(REGISTERS_FRAME, Xmm11) == 0x170);
    C_ASSERT(FIELD_OFFSET(REGISTERS_FRAME, Xmm12) == 0x180);
    C_ASSERT(FIELD_OFFSET(REGISTERS_FRAME, Xmm13) == 0x190);
    C_ASSERT(FIELD_OFFSET(REGISTERS_FRAME, Xmm14) == 0x1a0);
    C_ASSERT(FIELD_OFFSET(REGISTERS_FRAME, Xmm15) == 0x1b0);
    C_ASSERT(FIELD_OFFSET(REGISTERS_FRAME, MxCsr) == 0x1c0);
    C_ASSERT(FIELD_OFFSET(REGISTERS_FRAME, Dr) == 0x1c8);
    C_ASSERT(FIELD_OFFSET(REGISTERS_FRAME, Dr0) == 0x1c8);
    C_ASSERT(FIELD_OFFSET(REGISTERS_FRAME, Dr1) == 0x1d0);
    C_ASSERT(FIELD_OFFSET(REGISTERS_FRAME, Dr2) == 0x1d8);
    C_ASSERT(FIELD_OFFSET(REGISTERS_FRAME, Dr3) == 0x1e0);
    C_ASSERT(FIELD_OFFSET(REGISTERS_FRAME, Dr4) == 0x1e8);
    C_ASSERT(FIELD_OFFSET(REGISTERS_FRAME, Dr5) == 0x1f0);
    C_ASSERT(FIELD_OFFSET(REGISTERS_FRAME, Dr6) == 0x1f8);
    C_ASSERT(FIELD_OFFSET(REGISTERS_FRAME, Dr7) == 0x200);
    C_ASSERT(FIELD_OFFSET(REGISTERS_FRAME, SegEs) == 0x208);
    C_ASSERT(FIELD_OFFSET(REGISTERS_FRAME, SegCs) == 0x20a);
    C_ASSERT(FIELD_OFFSET(REGISTERS_FRAME, SegSs) == 0x20c);
    C_ASSERT(FIELD_OFFSET(REGISTERS_FRAME, SegDs) == 0x20e);
    C_ASSERT(FIELD_OFFSET(REGISTERS_FRAME, SegFs) == 0x210);
    C_ASSERT(FIELD_OFFSET(REGISTERS_FRAME, SegGs) == 0x212);
    C_ASSERT(FIELD_OFFSET(REGISTERS_FRAME, Ldtr) == 0x214);
    C_ASSERT(FIELD_OFFSET(REGISTERS_FRAME, Tr) == 0x216);
    C_ASSERT(FIELD_OFFSET(REGISTERS_FRAME, Gdtr.Limit) == 0x21e);
    C_ASSERT(FIELD_OFFSET(REGISTERS_FRAME, Idtr.Limit) == 0x22e);
    C_ASSERT(FIELD_OFFSET(REGISTERS_FRAME, Cr) == 0x238);
    C_ASSERT(FIELD_OFFSET(REGISTERS_FRAME, Cr0) == 0x238);
    C_ASSERT(FIELD_OFFSET(REGISTERS_FRAME, Cr2) == 0x248);
    C_ASSERT(FIELD_OFFSET(REGISTERS_FRAME, Cr3) == 0x250);
    C_ASSERT(FIELD_OFFSET(REGISTERS_FRAME, Cr4) == 0x258);
    C_ASSERT(FIELD_OFFSET(REGISTERS_FRAME, Cr8) == 0x278);

    C_ASSERT(sizeof(REGISTERS_FRAME) == 0x280);
#endif // !_WIN64

    typedef struct _INTERRUPTION_FRAME {
        ULONG_PTR ProgramCounter;
        ULONG_PTR SegCs;
        ULONG_PTR Eflags;
    }INTERRUPTION_FRAME, *PINTERRUPTION_FRAME;

#define DivideErrorFault        0  
#define DebugTrapOrFault        1  
#define NmiInterrupt            2  
#define BreakpointTrap          3  
#define OverflowTrap            4  
#define BoundFault              5  
#define InvalidOpcodeFault      6  
#define NpxNotAvailableFault    7  
#define DoubleFaultAbort        8  
#define NpxSegmentOverrunAbort  9  
#define InvalidTssFault         10 
#define SegmentNotPresentFault  11 
#define StackFault              12 
#define GeneralProtectionFault  13 
#define PageFault               14 
#define FloatingErrorFault      16 
#define AlignmentFault          17 
#define McheckAbort             18 
#define XmmException            19 
#define ApcInterrupt            31 
#define RaiseAssertion          44 
#define DebugServiceTrap        45 
#define DpcInterrupt            47 
#define IpiInterrupt            225

    typedef
        VOID
        (NTAPI * PIDT_HANDLER)(
            VOID
            );

    typedef union _KIDT_HANDLER {
        struct {
#ifndef _WIN64
            USHORT Offset;
            USHORT ExtendedOffset;
#else
            USHORT OffsetLow;
            USHORT OffsetMiddle;
            ULONG OffsetHigh;
#endif // !_WIN64
        };

        ULONG_PTR Address;
    } KIDT_HANDLER, *PKIDT_HANDLER;

    typedef struct _CCB {
        DECLSPEC_ALIGN(PAGE_SIZE) struct {
            UCHAR KernelStack[KERNEL_LARGE_STACK_SIZE - sizeof(REGISTERS_FRAME)];
            REGISTERS_FRAME Registers;
        };

        DECLSPEC_ALIGN(PAGE_SIZE) struct {
            VMX_VMCS VmsSupport;
            VMX_VMCS Vmcs;
            UCHAR Bitmap[PAGE_SIZE];
        }Region;

        CPU_FEATURE Feature;
        VMX_INFO VmxInfo;

        PHYSICAL_ADDRESS VmsSupport;
        PHYSICAL_ADDRESS Vmcs;
        PHYSICAL_ADDRESS Bitmap;

        GUEST_STATE GuestState;
    }CCB, *PCCB;

    VOID
        NTAPI
        ReadCpuFeature(
            __in PCPU_FEATURE Feature
        );

    VOID
        NTAPI
        CaptureRegisters(
            __out PREGISTERS_FRAME Registers
        );

    VOID
        NTAPI
        RestoreRegisters(
            __in PREGISTERS_FRAME Registers
        );

    ULONG
        NTAPI
        __ops_sldt(
            __in PUSHORT Selector
        );

    ULONG
        NTAPI
        __ops_str(
            __in PUSHORT Selector
        );

    ULONG
        NTAPI
        __ops_sgdt(
            __in PUSHORT Limit
        );

    ULONG
        NTAPI
        __ops_sidt(
            __in PUSHORT Limit
        );

    ULONG64
        NTAPI
        __ops_readmsr(
            __in ULONG Register
        );

    VOID
        NTAPI
        __ops_writemsr(
            __in ULONG Register,
            __in ULONG64 Value
        );

    SIZE_T
        NTAPI
        __ops_readcr(
            __in ULONG Register
        );

    VOID
        NTAPI
        __ops_writecr(
            __in ULONG Register,
            __in SIZE_T Value
        );

    SIZE_T
        NTAPI
        __ops_readdr(
            __in ULONG Register
        );

    VOID
        NTAPI
        __ops_writedr(
            __in ULONG Register,
            __in SIZE_T Value
        );

#ifdef __cplusplus
}
#endif	/* __cplusplus */

#endif // !_VCPU_H_
