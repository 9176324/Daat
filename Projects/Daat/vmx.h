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

#ifndef _VMX_H_
#define _VMX_H_

#ifdef __cplusplus
/* Assume byte packing throughout */
extern "C" {
#endif	/* __cplusplus */

#define CPUID_CACHE_SIZE 6

    typedef union _CPU_FEATURE {
        u32 Data[CPUID_CACHE_SIZE];
    } CPU_FEATURE, *PCPU_FEATURE;

#define FEAT(feature, index, bit) ((((feature)->Data[index]) >> (bit)) & 1)

    /* Intel SDM Vol. 2A: Table 3-8.
    * Information Returned by CPUID Instruction */

    /*
    * Intel SDM Vol. 2A: Table 3-10.
    * Feature Information Returned in the ECX Register
    * Features for CPUID with EAX=01h stored in ECX
    */

#define FEATURE_SSE3(feature) FEAT(feature, 0, 0) /* 0x00000001 Streaming SIMD Extensions 3 */
#define FEATURE_PCLMULQDQ(feature) FEAT(feature, 0, 1) /* 0x00000002 PCLMULQDQ Instruction */
#define FEATURE_DTES64(feature) FEAT(feature, 0, 2) /* 0x00000004 64-bit DS Area */
#define FEATURE_MONITOR(feature) FEAT(feature, 0, 3) /* 0x00000008 MONITOR/MWAIT Instructions */
#define FEATURE_DS_CPL(feature) FEAT(feature, 0, 4) /* 0x00000010 CPL Qualified Debug Store */
#define FEATURE_VMX(feature) FEAT(feature, 0, 5) /* 0x00000020 Virtual Machine Extensions */
#define FEATURE_SMX(feature) FEAT(feature, 0, 6) /* 0x00000040 Safer Mode Extensions */
#define FEATURE_EIST(feature) FEAT(feature, 0, 7) /* 0x00000080 Enhanced Intel SpeedStep technology */
#define FEATURE_TM2(feature) FEAT(feature, 0, 8) /* 0x00000100 Thermal Monitor 2 */
#define FEATURE_SSSE3(feature) FEAT(feature, 0, 9) /* 0x00000200 Supplemental Streaming SIMD Extensions 3 */
#define FEATURE_CNXT_ID(feature) FEAT(feature, 0, 10) /* 0x00000400 L1 Context ID */
#define FEATURE_SDBG(feature) FEAT(feature, 0, 11) /* 0x00000800 Silicon Debug Interface */
#define FEATURE_FMA(feature) FEAT(feature, 0, 12) /* 0x00001000 Fused Multiply-Add */
#define FEATURE_CMPXCHG16B(feature) FEAT(feature, 0, 13) /* 0x00002000 CMPXCHG16B Instruction */
#define FEATURE_XTPR_UPDATE(feature) FEAT(feature, 0, 14) /* 0x00004000 xTPR Update Control */
#define FEATURE_PDCM(feature) FEAT(feature, 0, 15) /* 0x00008000 Perfmon and Debug Capability */
#define FEATURE_PCID(feature) FEAT(feature, 0, 17) /* 0x00020000 Process-context identifiers */
#define FEATURE_DCA(feature) FEAT(feature, 0, 18) /* 0x00040000 Direct feature access for DMA writes */
#define FEATURE_SSE41(feature) FEAT(feature, 0, 19) /* 0x00080000 Streaming SIMD Extensions 4.1 */
#define FEATURE_SSE42(feature) FEAT(feature, 0, 20) /* 0x00100000 Streaming SIMD Extensions 4.2 */
#define FEATURE_X2APIC(feature) FEAT(feature, 0, 21) /* 0x00200000 x2APIC support */
#define FEATURE_MOVBE(feature) FEAT(feature, 0, 22) /* 0x00400000 MOVBE Instruction */
#define FEATURE_POPCNT(feature) FEAT(feature, 0, 23) /* 0x00800000 POPCNT Instruction */
#define FEATURE_TSC_DEADLINE(feature) FEAT(feature, 0, 24) /* 0x01000000 APIC supports one-shot operation using TSC deadline */
#define FEATURE_AESNI(feature) FEAT(feature, 0, 25) /* 0x02000000 AESNI Extension */
#define FEATURE_XSAVE(feature) FEAT(feature, 0, 26) /* 0x04000000 XSAVE/XRSTOR/XSETBV/XGETBV Instructions and XCR0 */
#define FEATURE_OSXSAVE(feature) FEAT(feature, 0, 27) /* 0x08000000 XSAVE enabled by OS */
#define FEATURE_AVX(feature) FEAT(feature, 0, 28) /* 0x10000000 Advanced Vector Extensions */
#define FEATURE_F16C(feature) FEAT(feature, 0, 29) /* 0x20000000 16-bit Floating-Point Instructions */
#define FEATURE_RDRAND(feature) FEAT(feature, 0, 30) /* 0x40000000 RDRAND Instruction */
#define FEATURE_HYPERVISOR(feature) FEAT(feature, 0, 31) /* 0x80000000 Hypervisor Running */

    /*
    * Intel SDM Vol. 2A: Table 3-11.
    * More on Feature Information Returned in the EDX Register
    * Features for CPUID with EAX=01h stored in EDX
    */

#define FEATURE_FPU(feature) FEAT(feature, 1, 0) /* 0x00000001 Floating Point Unit On-Chip */
#define FEATURE_VME(feature) FEAT(feature, 1, 1) /* 0x00000002 Virtual 8086 Mode Enhancements */
#define FEATURE_DE(feature) FEAT(feature, 1, 2) /* 0x00000004 Debugging Extensions */
#define FEATURE_PSE(feature) FEAT(feature, 1, 3) /* 0x00000008 Page Size Extension */
#define FEATURE_TSC(feature) FEAT(feature, 1, 4) /* 0x00000010 Time Stamp Counter */
#define FEATURE_MSR(feature) FEAT(feature, 1, 5) /* 0x00000020 RDMSR/WRMSR Instructions */
#define FEATURE_PAE(feature) FEAT(feature, 1, 6) /* 0x00000040 Physical Address Extension */
#define FEATURE_MCE(feature) FEAT(feature, 1, 7) /* 0x00000080 Machine Check Exception */
#define FEATURE_CX8(feature) FEAT(feature, 1, 8) /* 0x00000100 CMPXCHG8B Instruction */
#define FEATURE_APIC(feature) FEAT(feature, 1, 9) /* 0x00000200 APIC On-Chip */
#define FEATURE_SEP(feature) FEAT(feature, 1, 11) /* 0x00000800 SYSENTER/SYSEXIT Instructions */
#define FEATURE_MTRR(feature) FEAT(feature, 1, 12) /* 0x00001000 Memory Type Range Registers */
#define FEATURE_PGE(feature) FEAT(feature, 1, 13) /* 0x00002000 Page Global Bit */
#define FEATURE_MCA(feature) FEAT(feature, 1, 14) /* 0x00004000 Machine Check Architecture */
#define FEATURE_CMOV(feature) FEAT(feature, 1, 15) /* 0x00008000 Conditional Move Instructions */
#define FEATURE_PAT(feature) FEAT(feature, 1, 16) /* 0x00010000 Page Attribute Table */
#define FEATURE_PSE36(feature) FEAT(feature, 1, 17) /* 0x00020000 36-Bit Page Size Extension */
#define FEATURE_PSN(feature) FEAT(feature, 1, 18) /* 0x00040000 Processor Serial Number */
#define FEATURE_CLFSH(feature) FEAT(feature, 1, 19) /* 0x00080000 CLFLUSH Instruction */
#define FEATURE_DS(feature) FEAT(feature, 1, 21) /* 0x00200000 Debug Store */
#define FEATURE_ACPI(feature) FEAT(feature, 1, 22) /* 0x00400000 Thermal Monitor and Software Controlled Clock Facilities */
#define FEATURE_MMX(feature) FEAT(feature, 1, 23) /* 0x00800000 Intel MMX Technology */
#define FEATURE_FXSR(feature) FEAT(feature, 1, 24) /* 0x01000000 FXSAVE and FXRSTOR Instructions */
#define FEATURE_SSE(feature) FEAT(feature, 1, 25) /* 0x02000000 Streaming SIMD Extensions */
#define FEATURE_SSE2(feature) FEAT(feature, 1, 26) /* 0x04000000 Streaming SIMD Extensions 2 */
#define FEATURE_SS(feature) FEAT(feature, 1, 27) /* 0x08000000 Self Snoop */
#define FEATURE_HTT(feature) FEAT(feature, 1, 28) /* 0x10000000 Max APIC IDs reserved field is Valid */
#define FEATURE_TM(feature) FEAT(feature, 1, 29) /* 0x20000000 Thermal Monitor */
#define FEATURE_PBE(feature) FEAT(feature, 1, 31) /* 0x80000000 Pending Break Enable */

    /*
    * Intel SDM Vol. 2A: Table 3-8. Information Returned by CPUID Instruction
    * Structured Extended Feature Flags Enumeration Leaf
    * Features for CPUID with EAX=07h, ECX=00h stored in EBX
    */

#define FEATURE_FSGSBASE(feature) FEAT(feature, 2, 0) /* 0x00000001 RDFSBASE/RDGSBASE/WRFSBASE/WRGSBASE supported */
#define FEATURE_TSC_ADJUST(feature) FEAT(feature, 2, 1) /* 0x00000002 MSR IA32_TSC_ADJUST supported */
#define FEATURE_SGX(feature) FEAT(feature, 2, 2) /* 0x00000004 Software Guard Extensions */
#define FEATURE_BMI1(feature) FEAT(feature, 2, 3) /* 0x00000008 Bit Manipulation Instruction Set 1 */
#define FEATURE_HLE(feature) FEAT(feature, 2, 4) /* 0x00000010 Transactional Synchronization Extensions */
#define FEATURE_AVX2(feature) FEAT(feature, 2, 5) /* 0x00000020 Advanced Vector Extensions 2 */
#define FEATURE_SMEP(feature) FEAT(feature, 2, 7) /* 0x00000080 Supervisor-Mode Execution Prevention */
#define FEATURE_BMI2(feature) FEAT(feature, 2, 8) /* 0x00000100 Bit Manipulation Instruction Set 2 */
#define FEATURE_INVPCID(feature) FEAT(feature, 2, 10) /* 0x00000400 INVPCID instruction */
#define FEATURE_RTM(feature) FEAT(feature, 2, 11) /* 0x00000800 Transactional Synchronization Extensions */
#define FEATURE_RDT_M(feature) FEAT(feature, 2, 12) /* 0x00001000 Resource Director Technology Monitoring */
#define FEATURE_MPX(feature) FEAT(feature, 2, 14) /* 0x00004000 Memory Protection Extensions */
#define FEATURE_RDT_A(feature) FEAT(feature, 2, 15) /* 0x00008000 Resource Director Technology Allocation */
#define FEATURE_AVX512F(feature) FEAT(feature, 2, 16) /* 0x00010000 AVX-512 Foundation */
#define FEATURE_AVX512DQ(feature) FEAT(feature, 2, 17) /* 0x00020000 AVX-512 Doubleword and Quadword Instructions */
#define FEATURE_RDSEED(feature) FEAT(feature, 2, 18) /* 0x00040000 RDSEED Instruction */
#define FEATURE_ADX(feature) FEAT(feature, 2, 19) /* 0x00080000 Multi-Precision Add-Carry Instruction Extensions */
#define FEATURE_SMAP(feature) FEAT(feature, 2, 20) /* 0x00100000 Supervisor Mode Access Prevention */
#define FEATURE_AVX512_IFMA(feature) FEAT(feature, 2, 21) /* 0x00200000 AVX-512 Integer Fused Multiply-Add Instructions */
#define FEATURE_CLFLUSHOPT(feature) FEAT(feature, 2, 23) /* 0x00800000 CLFLUSHOPT Instruction */
#define FEATURE_CLWB(feature) FEAT(feature, 2, 24) /* 0x01000000 CLWB Instruction */
#define FEATURE_AVX512PF(feature) FEAT(feature, 2, 26) /* 0x04000000 AVX-512 Prefetch Instructions */
#define FEATURE_AVX512ER(feature) FEAT(feature, 2, 27) /* 0x08000000 AVX-512 Exponential and Reciprocal Instructions */
#define FEATURE_AVX512CD(feature) FEAT(feature, 2, 28) /* 0x10000000 AVX-512 Conflict Detection Instructions*/
#define FEATURE_SHA(feature) FEAT(feature, 2, 29) /* 0x20000000 SHA Extension */
#define FEATURE_AVX512BW(feature) FEAT(feature, 2, 30) /* 0x40000000 AVX-512 Byte and Word Instructions */
#define FEATURE_AVX512VL(feature) FEAT(feature, 2, 31) /* 0x80000000 AVX-512 Vector Length Extensions */

    /*
    * Intel SDM Vol. 2A: Table 3-8. Information Returned by CPUID Instruction
    * Structured Extended Feature Flags Enumeration Leaf
    * Features for CPUID with EAX=07h, ECX=00h stored in ECX
    */

#define FEATURE_PREFETCHWT1(feature) FEAT(feature, 3, 0) /* 0x00000001 PREFETCHWT1 Instruction */
#define FEATURE_AVX512_VBMI(feature) FEAT(feature, 3, 1) /* 0x00000002 AVX-512 Vector Bit Manipulation Instructions */
#define FEATURE_UMIP(feature) FEAT(feature, 3, 2) /* 0x00000004 User-Mode Instruction Prevention */
#define FEATURE_PKU(feature) FEAT(feature, 3, 3) /* 0x00000008 Protection Keys for User-mode pages */
#define FEATURE_OSPKE(feature) FEAT(feature, 3, 4) /* 0x00000010 PKU enabled by OS */
#define FEATURE_RDPID(feature) FEAT(feature, 3, 22) /* 0x00400000 RDPID Instruction and IA32_TSC_AUX MSR */

    /*
    * Intel SDM Vol. 2A: Table 3-8. Information Returned by CPUID Instruction
    * Extended Function CPUID Information
    * Features for CPUID with EAX=80000001h stored in ECX
    */

#define FEATURE_LAHF(feature) FEAT(feature, 4, 0) /* 0x00000001 LAHF/SAHF Instructions */
#define FEATURE_PREFETCHW(feature) FEAT(feature, 4, 8) /* 0x00000100 PREFETCH/PREFETCHW instructions */

    /*
    * Intel SDM Vol. 2A: Table 3-8. Information Returned by CPUID Instruction
    * Extended Function CPUID Information
    * Features for CPUID with EAX=80000001h stored in EDX
    */

#define FEATURE_SYSCALL(feature) FEAT(feature, 5, 11) /* 0x00000800 SYSCALL/SYSRET Instructions */
#define FEATURE_NX(feature) FEAT(feature, 5, 20) /* 0x00100000 No-Execute Bit */
#define FEATURE_PDPE1GB(feature) FEAT(feature, 5, 26) /* 0x04000000 Gibibyte pages */
#define FEATURE_RDTSCP(feature) FEAT(feature, 5, 27) /* 0x08000000 RDTSCP Instruction */
#define FEATURE_EM64T(feature) FEAT(feature, 5, 29) /* 0x20000000 Long Mode */

#ifndef _WIN64
    typedef struct DECLSPEC_ALIGN(16) _REGISTERS_FRAME {
        //
        // Integer registers.
        //

        union {
            u32 Reg[8];

            struct {
                union {
                    u32 Eax;
                    u16 Ax;

                    struct {
                        u8 Al;
                        u8 Ah;
                    };
                };

                union {
                    u32 Ecx;
                    u16 Cx;

                    struct {
                        u8 Cl;
                        u8 Ch;
                    };
                };

                union {
                    u32 Edx;
                    u16 Dx;

                    struct {
                        u8 Dl;
                        u8 Dh;
                    };
                };

                union {
                    u32 Ebx;
                    u16 Bx;

                    struct {
                        u8 Bl;
                        u8 Bh;
                    };
                };

                union {
                    u32 Esp;
                    u16 Sp;
                    u32 StackPointer;
                };

                union {
                    u32 Ebp;
                    u16 Bp;
                };

                union {
                    u32 Esi;
                    u16 Si;
                };

                union {
                    u32 Edi;
                    u16 Di;
                };
            };
        };

        //
        // Program counter.
        //

        union {
            u32 Eip;
            u32 ProgramCounter;
        };

        //
        // Control flags.
        //

        u32 EFlags;

        //
        // Debug registers
        //

        union {
            u32 Dr[8];

            struct {
                u32 Dr0;
                u32 Dr1;
                u32 Dr2;
                u32 Dr3;
                u32 NOTHING : 32;
                u32 NOTHING : 32;
                u32 Dr6;
                u32 Dr7;
            };
        };

        //
        // Segment Registers and processor flags.
        //

        u16 SegEs;
        u16 SegCs;
        u16 SegSs;
        u16 SegDs;
        u16 SegFs;
        u16 SegGs;

        u16 Ldtr;
        u16 Tr;

        KDESCRIPTOR Gdtr;
        KDESCRIPTOR Idtr;

        //
        // Special registers.
        //

        union {
            u32 Cr[5];

            struct {
                u32 Cr0;
                u32 NOTHING : 32;
                u32 Cr2;
                u32 Cr3;
                u32 Cr4;
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

        u64 P1Home;
        u64 P2Home;
        u64 P3Home;
        u64 P4Home;
        u64 P5Home;
        u64 P6Home;

        //
        // Integer registers.
        //

        union {
            u64 Reg[16];

            struct {
                union {
                    u64 Rax;
                    u32 Eax;
                    u16 Ax;

                    struct {
                        u8 Al;
                        u8 Ah;
                    };
                };

                union {
                    u64 Rcx;
                    u32 Ecx;
                    u16 Cx;

                    struct {
                        u8 Cl;
                        u8 Ch;
                    };
                };

                union {
                    u64 Rdx;
                    u32 Edx;
                    u16 Dx;

                    struct {
                        u8 Dl;
                        u8 Dh;
                    };
                };

                union {
                    u64 Rbx;
                    u32 Ebx;
                    u16 Bx;

                    struct {
                        u8 Bl;
                        u8 Bh;
                    };
                };

                union {
                    u64 Rsp;
                    u32 Esp;
                    u16 Sp;
                    u64 StackPointer;
                };

                union {
                    u64 Rbp;
                    u32 Ebp;
                    u16 Bp;
                };

                union {
                    u64 Rsi;
                    u32 Esi;
                    u16 Si;
                };

                union {
                    u64 Rdi;
                    u32 Edi;
                    u16 Di;
                };

                union {
                    u64 R8;
                    u32 R8d;
                };

                union {
                    u64 R9;
                    u32 R9d;
                };

                union {
                    u64 R10;
                    u32 R10d;
                };

                union {
                    u64 R11;
                    u32 R11d;
                };

                union {
                    u64 R12;
                    u32 R12d;
                };

                union {
                    u64 R13;
                    u32 R13d;
                };

                union {
                    u64 R14;
                    u32 R14d;
                };

                union {
                    u64 R15;
                    u32 R15d;
                };
            };
        };

        //
        // Program counter.
        //

        union {
            u64 Rip;
            u64 ProgramCounter;
        };

        //
        // Control flags.
        //

        u32 EFlags;

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

        u32 MxCsr;

        //
        // Debug registers
        //

        union {
            u64 Dr[8];

            struct {
                u64 Dr0;
                u64 Dr1;
                u64 Dr2;
                u64 Dr3;
                u64 NOTHING : 64;
                u64 NOTHING : 64;
                u64 Dr6;
                u64 Dr7;
            };
        };

        //
        // Segment Registers and processor flags.
        //

        u16 SegEs;
        u16 SegCs;
        u16 SegSs;
        u16 SegDs;
        u16 SegFs;
        u16 SegGs;

        u16 Ldtr;
        u16 Tr;

        KDESCRIPTOR Gdtr;
        KDESCRIPTOR Idtr;

        //
        // Special registers.
        //

        union {
            u64 Cr[9];

            struct {
                u64 Cr0;
                u64 NOTHING : 64;
                u64 Cr2;
                u64 Cr3;
                u64 Cr4;
                u64 NOTHING : 64;
                u64 NOTHING : 64;
                u64 NOTHING : 64;
                u64 Cr8;
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

    void
        NTAPI
        ReadCpuFeature(
            __in PCPU_FEATURE Feature
        );

    void
        NTAPI
        CaptureRegisters(
            __out PREGISTERS_FRAME Registers
        );

    void
        NTAPI
        RestoreRegisters(
            __in PREGISTERS_FRAME Registers
        );

    u32
        NTAPI
        __ops_sldt(
            __in u16ptr Selector
        );

    u32
        NTAPI
        __ops_str(
            __in u16ptr Selector
        );

    u32
        NTAPI
        __ops_sgdt(
            __in u16ptr Limit
        );

    u32
        NTAPI
        __ops_lgdt(
            __in u16ptr Limit
        );

    u32
        NTAPI
        __ops_sidt(
            __in u16ptr Limit
        );

    u32
        NTAPI
        __ops_lidt(
            __in u16ptr Limit
        );

    u64
        NTAPI
        __ops_readmsr(
            __in u32 Register
        );

    void
        NTAPI
        __ops_writemsr(
            __in u32 Register,
            __in u64 Value
        );

    u
        NTAPI
        __ops_readcr(
            __in u32 Register
        );

    void
        NTAPI
        __ops_writecr(
            __in u32 Register,
            __in u Value
        );

    u
        NTAPI
        __ops_readdr(
            __in u32 Register
        );

    void
        NTAPI
        __ops_writedr(
            __in u32 Register,
            __in u Value
        );

#define VMCS_NONE 0xFFFFFFFFFFFFFFFF

    // Size of VMCS structure
#define IA32_VMX_VMCS_SIZE 4096

    // Intel SDM Vol. 3D: Table C-1. Basic Exit Reasons
    enum {
        VMX_EXIT_INT_EXCEPTION_NMI = 0, // An SW interrupt, exception or NMI has occurred
        VMX_EXIT_EXT_INTERRUPT = 1, // An external interrupt has occurred
        VMX_EXIT_TRIPLE_FAULT = 2, // Triple fault occurred
        VMX_EXIT_INIT_EVENT = 3, // INIT signal arrived
        VMX_EXIT_SIPI_EVENT = 4, // SIPI signal arrived
        VMX_EXIT_SMI_IO_EVENT = 5,
        VMX_EXIT_SMI_OTHER_EVENT = 6,
        VMX_EXIT_PENDING_INTERRUPT = 7,
        VMX_EXIT_PENDING_NMI = 8,
        VMX_EXIT_TASK_SWITCH = 9, // Guest attempted a task switch
        VMX_EXIT_CPUID = 10, // Guest executed CPUID instruction
        VMX_EXIT_GETSEC = 11, // Guest executed GETSEC instruction
        VMX_EXIT_HLT = 12, // Guest executed HLT instruction
        VMX_EXIT_INVD = 13, // Guest executed INVD instruction
        VMX_EXIT_INVLPG = 14, // Guest executed INVLPG instruction
        VMX_EXIT_RDPMC = 15, // Guest executed RDPMC instruction
        VMX_EXIT_RDTSC = 16, // Guest executed RDTSC instruction
        VMX_EXIT_RSM = 17, // Guest executed RSM instruction in SMM
        VMX_EXIT_VMCALL = 18, // Guest executed VMCALL instruction
        VMX_EXIT_VMCLEAR = 19, // Guest executed VMCLEAR instruction
        VMX_EXIT_VMLAUNCH = 20, // Guest executed VMLAUNCH instruction
        VMX_EXIT_VMPTRLD = 21, // Guest executed VMPTRLD instruction
        VMX_EXIT_VMPTRST = 22, // Guest executed VMPTRST instruction
        VMX_EXIT_VMREAD = 23, // Guest executed VMREAD instruction
        VMX_EXIT_VMRESUME = 24, // Guest executed VMRESUME instruction
        VMX_EXIT_VMWRITE = 25, // Guest executed VMWRITE instruction
        VMX_EXIT_VMXOFF = 26, // Guest executed VMXON instruction
        VMX_EXIT_VMXON = 27, // Guest executed VMXOFF instruction
        VMX_EXIT_CR_ACCESS = 28, // Guest accessed a control register
        VMX_EXIT_DR_ACCESS = 29, // Guest attempted access to debug register
        VMX_EXIT_IO = 30, // Guest attempted I/O
        VMX_EXIT_MSR_READ = 31, // Guest attempted to read an MSR
        VMX_EXIT_MSR_WRITE = 32, // Guest attempted to write an MSR
        VMX_EXIT_FAILED_VMENTER_GS = 33, // VMENTER failed due to guest state
        VMX_EXIT_FAILED_VMENTER_MSR = 34, // VMENTER failed due to MSR loading
        VMX_EXIT_MWAIT = 36,
        VMX_EXIT_MTF_EXIT = 37,
        VMX_EXIT_MONITOR = 39,
        VMX_EXIT_PAUSE = 40,
        VMX_EXIT_MACHINE_CHECK = 41,
        VMX_EXIT_TPR_BELOW_THRESHOLD = 43,
        VMX_EXIT_APIC_ACCESS = 44,
        VMX_EXIT_VIRTUALIZED_EOI = 45,
        VMX_EXIT_GDT_IDT_ACCESS = 46,
        VMX_EXIT_LDT_TR_ACCESS = 47,
        VMX_EXIT_EPT_VIOLATION = 48,
        VMX_EXIT_EPT_MISCONFIG = 49,
        VMX_EXIT_INVEPT = 50,
        VMX_EXIT_RDTSCP = 51,
        VMX_EXIT_VMX_TIMER_EXIT = 52,
        VMX_EXIT_INVVPID = 53,
        VMX_EXIT_WBINVD = 54,
        VMX_EXIT_XSETBV = 55, // Guest executed XSETBV instruction
        VMX_EXIT_APIC_WRITE = 56,
        VMX_EXIT_RDRAND = 57,
        VMX_EXIT_INVPCID = 58,
        VMX_EXIT_VMFUNC = 59,
        VMX_EXIT_ENCLS = 60,
        VMX_EXIT_RDSEED = 61,
        VMX_EXIT_PML_FULL = 62,
        VMX_EXIT_XSAVES = 63,
        VMX_EXIT_XRSTORS = 64,
        VMX_EXIT_SPP_RELATED = 66,
        VMX_EXIT_UMWAIT = 67,
        VMX_EXIT_TPAUSE = 68
    };

    enum {
        VMX_PIN_CONTROLS = 0x00004000,
        VMX_PRIMARY_PROCESSOR_CONTROLS = 0x00004002,
        VMX_SECONDARY_PROCESSOR_CONTROLS = 0x0000401e,
        VMX_EXCEPTION_BITMAP = 0x00004004,
        VMX_PAGE_FAULT_ERROR_CODE_MASK = 0x00004006,
        VMX_PAGE_FAULT_ERROR_CODE_MATCH = 0x00004008,
        VMX_EXIT_CONTROLS = 0x0000400c,
        VMX_EXIT_MSR_STORE_COUNT = 0x0000400e,
        VMX_EXIT_MSR_LOAD_COUNT = 0x00004010,
        VMX_ENTRY_CONTROLS = 0x00004012,
        VMX_ENTRY_MSR_LOAD_COUNT = 0x00004014,
        VMX_ENTRY_INTERRUPT_INFO = 0x00004016,
        VMX_ENTRY_EXCEPTION_ERROR_CODE = 0x00004018,
        VMX_ENTRY_INSTRUCTION_LENGTH = 0x0000401a,
        VMX_TPR_THRESHOLD = 0x0000401c,

        VMX_CR0_MASK = 0x00006000,
        VMX_CR4_MASK = 0x00006002,
        VMX_CR0_READ_SHADOW = 0x00006004,
        VMX_CR4_READ_SHADOW = 0x00006006,
        VMX_CR3_TARGET_COUNT = 0x0000400a,
        VMX_CR3_TARGET_VAL_BASE = 0x00006008, // x6008-x6206

        VMX_VPID = 0x00000000,
        VMX_IO_BITMAP_A = 0x00002000,
        VMX_IO_BITMAP_B = 0x00002002,
        VMX_MSR_BITMAP = 0x00002004,
        VMX_EXIT_MSR_STORE_ADDRESS = 0x00002006,
        VMX_EXIT_MSR_LOAD_ADDRESS = 0x00002008,
        VMX_ENTRY_MSR_LOAD_ADDRESS = 0x0000200a,
        VMX_TSC_OFFSET = 0x00002010,
        VMX_VAPIC_PAGE = 0x00002012,
        VMX_APIC_ACCESS_PAGE = 0x00002014,
        VMX_EPTP = 0x0000201a,
        VMX_PREEMPTION_TIMER = 0x0000482e,

        VMX_INSTRUCTION_ERROR_CODE = 0x00004400,

        VM_EXIT_INFO_REASON = 0x00004402,
        VM_EXIT_INFO_INTERRUPT_INFO = 0x00004404,
        VM_EXIT_INFO_EXCEPTION_ERROR_CODE = 0x00004406,
        VM_EXIT_INFO_IDT_VECTORING = 0x00004408,
        VM_EXIT_INFO_IDT_VECTORING_ERROR_CODE = 0x0000440a,
        VM_EXIT_INFO_INSTRUCTION_LENGTH = 0x0000440c,
        VM_EXIT_INFO_INSTRUCTION_INFO = 0x0000440e,
        VM_EXIT_INFO_QUALIFICATION = 0x00006400,
        VM_EXIT_INFO_IO_ECX = 0x00006402,
        VM_EXIT_INFO_IO_ESI = 0x00006404,
        VM_EXIT_INFO_IO_EDI = 0x00006406,
        VM_EXIT_INFO_IO_EIP = 0x00006408,
        VM_EXIT_INFO_GUEST_LINEAR_ADDRESS = 0x0000640a,
        VM_EXIT_INFO_GUEST_PHYSICAL_ADDRESS = 0x00002400,

        HOST_RIP = 0x00006c16,
        HOST_RSP = 0x00006c14,
        HOST_CR0 = 0x00006c00,
        HOST_CR3 = 0x00006c02,
        HOST_CR4 = 0x00006c04,

        HOST_CS_SELECTOR = 0x00000c02,
        HOST_DS_SELECTOR = 0x00000c06,
        HOST_ES_SELECTOR = 0x00000c00,
        HOST_FS_SELECTOR = 0x00000c08,
        HOST_GS_SELECTOR = 0x00000c0a,
        HOST_SS_SELECTOR = 0x00000c04,
        HOST_TR_SELECTOR = 0x00000c0c,
        HOST_FS_BASE = 0x00006c06,
        HOST_GS_BASE = 0x00006c08,
        HOST_TR_BASE = 0x00006c0a,
        HOST_GDTR_BASE = 0x00006c0c,
        HOST_IDTR_BASE = 0x00006c0e,

        HOST_SYSENTER_CS = 0x00004c00,
        HOST_SYSENTER_ESP = 0x00006c10,
        HOST_SYSENTER_EIP = 0x00006c12,

        HOST_PAT = 0x00002c00,
        HOST_EFER = 0x00002c02,
        HOST_PERF_GLOBAL_CTRL = 0x00002c04,

        GUEST_RIP = 0x0000681e,
        GUEST_RFLAGS = 0x00006820,
        GUEST_RSP = 0x0000681c,
        GUEST_CR0 = 0x00006800,
        GUEST_CR3 = 0x00006802,
        GUEST_CR4 = 0x00006804,

        GUEST_ES_SELECTOR = 0x00000800,
        GUEST_CS_SELECTOR = 0x00000802,
        GUEST_SS_SELECTOR = 0x00000804,
        GUEST_DS_SELECTOR = 0x00000806,
        GUEST_FS_SELECTOR = 0x00000808,
        GUEST_GS_SELECTOR = 0x0000080a,
        GUEST_LDTR_SELECTOR = 0x0000080c,
        GUEST_TR_SELECTOR = 0x0000080e,

        GUEST_ES_AR = 0x00004814,
        GUEST_CS_AR = 0x00004816,
        GUEST_SS_AR = 0x00004818,
        GUEST_DS_AR = 0x0000481a,
        GUEST_FS_AR = 0x0000481c,
        GUEST_GS_AR = 0x0000481e,
        GUEST_LDTR_AR = 0x00004820,
        GUEST_TR_AR = 0x00004822,

        GUEST_ES_BASE = 0x00006806,
        GUEST_CS_BASE = 0x00006808,
        GUEST_SS_BASE = 0x0000680a,
        GUEST_DS_BASE = 0x0000680c,
        GUEST_FS_BASE = 0x0000680e,
        GUEST_GS_BASE = 0x00006810,
        GUEST_LDTR_BASE = 0x00006812,
        GUEST_TR_BASE = 0x00006814,
        GUEST_GDTR_BASE = 0x00006816,
        GUEST_IDTR_BASE = 0x00006818,

        GUEST_ES_LIMIT = 0x00004800,
        GUEST_CS_LIMIT = 0x00004802,
        GUEST_SS_LIMIT = 0x00004804,
        GUEST_DS_LIMIT = 0x00004806,
        GUEST_FS_LIMIT = 0x00004808,
        GUEST_GS_LIMIT = 0x0000480a,
        GUEST_LDTR_LIMIT = 0x0000480c,
        GUEST_TR_LIMIT = 0x0000480e,
        GUEST_GDTR_LIMIT = 0x00004810,
        GUEST_IDTR_LIMIT = 0x00004812,

        GUEST_VMCS_LINK_PTR = 0x00002800,
        GUEST_DEBUGCTL = 0x00002802,
        GUEST_PAT = 0x00002804,
        GUEST_EFER = 0x00002806,
        GUEST_PERF_GLOBAL_CTRL = 0x00002808,
        GUEST_PDPTE0 = 0x0000280a,
        GUEST_PDPTE1 = 0x0000280c,
        GUEST_PDPTE2 = 0x0000280e,
        GUEST_PDPTE3 = 0x00002810,

        GUEST_DR7 = 0x0000681a,
        GUEST_PENDING_DBE = 0x00006822,
        GUEST_SYSENTER_CS = 0x0000482a,
        GUEST_SYSENTER_ESP = 0x00006824,
        GUEST_SYSENTER_EIP = 0x00006826,
        GUEST_SMBASE = 0x00004828,
        GUEST_INTERRUPTIBILITY = 0x00004824,
        GUEST_ACTIVITY_STATE = 0x00004826,

        /* Invalid encoding */
        VMCS_NO_COMPONENT = 0x0000ffff
    };

    // PIN-BASED CONTROLS
#define EXT_INTERRUPT_EXITING                  0x00000001
#define NMI_EXITING                            0x00000008
#define VIRTUAL_NMIS                           0x00000020
#define VMX_TIMER_EXITING                      0x00000040
#define PIN_CONTROLS_DEFINED                   0x00000069

    // Primary CPU Exit CONTROLS
#define INTERRUPT_WINDOW_EXITING               0x00000004
#define USE_TSC_OFFSETTING                     0x00000008
#define HLT_EXITING                            0x00000080
#define INVLPG_EXITING                         0x00000200
#define MWAIT_EXITING                          0x00000400
#define RDPMC_EXITING                          0x00000800
#define RDTSC_EXITING                          0x00001000
#define CR3_LOAD_EXITING                       0x00008000
#define CR3_STORE_EXITING                      0x00010000
#define CR8_LOAD_EXITING                       0x00080000
#define CR8_STORE_EXITING                      0x00100000
#define USE_TPR_SHADOW                         0x00200000
#define NMI_WINDOW_EXITING                     0x00400000
#define DR_EXITING                             0x00800000
#define IO_EXITING                             0x01000000
#define IO_BITMAP_ACTIVE                       0x02000000
#define MONITOR_TRAP_FLAG                      0x08000000
#define MSR_BITMAP_ACTIVE                      0x10000000
#define MONITOR_EXITING                        0x20000000
#define PAUSE_EXITING                          0x40000000
#define SECONDARY_CONTROLS                     0x80000000
#define PRIMARY_CONTROLS_DEFINED               0xfbf99e8c

    // Secondary CPU Exit CONTROLS
#define VIRTUALIZE_APIC_ACCESSES               0x00000001
#define ENABLE_EPT                             0x00000002
#define DESCTAB_EXITING                        0x00000004
#define ENABLE_RDTSCP                          0x00000008
#define VIRTUALIZE_X2APIC                      0x00000010
#define ENABLE_VPID                            0x00000020
#define WBINVD_EXITING                         0x00000040
#define UNRESTRICTED_GUEST                     0x00000080
#define PAUSE_LOOP_EXITING                     0x00000400
#define SECONDARY_CONTROLS_DEFINED             0x000004ff

    // Exit Controls
#define EXIT_CONTROL_SAVE_DEBUG_CONTROLS       0x00000004
#define EXIT_CONTROL_HOST_ADDR_SPACE_SIZE      0x00000200
#define EXIT_CONTROL_LOAD_PERF_GLOBAL_CTRL     0x00001000
#define EXIT_CONTROL_ACKNOWLEDGE_INTERRUPT     0x00008000
#define EXIT_CONTROL_SAVE_PAT                  0x00040000
#define EXIT_CONTROL_LOAD_PAT                  0x00080000
#define EXIT_CONTROL_SAVE_EFER                 0x00100000
#define EXIT_CONTROL_LOAD_EFER                 0x00200000
#define EXIT_CONTROL_SAVE_VMX_TIMER            0x00400000
#define EXIT_CONTROLS_DEFINED                  0x007c9204

    // Entry Controls
#define ENTRY_CONTROL_LOAD_DEBUG_CONTROLS      0x00000004
#define ENTRY_CONTROL_LONG_MODE_GUEST          0x00000200
#define ENTRY_CONTROL_ENTRY_TO_SMM             0x00000400
#define ENTRY_CONTROL_TEAR_DOWN_SMM_MONITOR    0x00000800
#define ENTRY_CONTROL_LOAD_PERF_GLOBAL_CTRL    0x00002000
#define ENTRY_CONTROL_LOAD_PAT                 0x00004000
#define ENTRY_CONTROL_LOAD_EFER                0x00008000
#define ENTRY_CONTROLS_DEFINED                 0x0000ee04

 // Intel SDM Vol. 3C: 30.2 Conventions
    typedef enum VMX_RESULT {
        /* VMsucceed
        * Operation succeeded (OSZPAC flags are 0) */
        VMX_SUCCEED = 0,

        /* VMfailInvalid
        * Operation failed and VCMS pointer is invalid (CF=1) */
        VMX_FAIL_INVALID = 1,

        /* VMfailValid(ErrorNumber)
        * Operation failed and VCMS pointer is valid (ZF=1) */
        VMX_FAIL_VALID = 2,
    } VMX_RESULT;

    // Intel SDM Vol. 3C: 30.4 VM Instruction Error Numbers
    enum {
        VMX_ERROR_VMCALL_ROOT = 1, // VMCALL executed in VMX root operation
        VMX_ERROR_VMCLEAR_PADDR_INVALID = 2, // VMCLEAR with invalid physical address
        VMX_ERROR_VMCLEAR_VMXON_PTR = 3, // VMCLEAR with VMXON pointer
        VMX_ERROR_VMLAUNCH_VMCS_UNCLEAR = 4, // VMLAUNCH with non-clear VMCS
        VMX_ERROR_VMRESUME_VMCS_UNLAUNCHED = 5, // VMRESUME with non-launched VMCS
        VMX_ERROR_VMRESUME_AFTER_VMXOFF = 6, // VMRESUME after VMXOFF
        VMX_ERROR_ENTRY_CTRL_FIELDS_INVALID = 7, // VM entry with invalid control field(s)
        VMX_ERROR_ENTRY_HOST_FIELDS_INVALID = 8, // VM entry with invalid host-state field(s)
        VMX_ERROR_VMPTRLD_PADDR_INVALID = 9, // VMPTRLD with invalid physical address
        VMX_ERROR_VMPTRLD_VMXON_PTR = 10, // VMPTRLD with VMXON pointer
        VMX_ERROR_VMPTRLD_VMCSREV_INVALID = 11, // VMPTRLD with incorrect VMCS revision identifier
        VMX_ERROR_VMREAD_VMWRITE_INVALID = 12, // VMREAD/VMWRITE from/to unsupported VMCS component
        VMX_ERROR_VMWRITE_READONLY = 13, // VMWRITE to read-only VMCS component
        VMX_ERROR_VMXON_ROOT = 15, // VMXON executed in VMX root operation
        VMX_ERROR_ENTRY_VMCS_INVALID = 16, // VM entry with invalid executive-VMCS pointer
        VMX_ERROR_ENTRY_VMCS_UNLAUNCHED = 17, // VM entry with non-launched executive VMCS
        VMX_ERROR_ENTRY_VMCS_NOT_VMXON = 18, // VM entry with executive-VMCS pointer not VMXON pointer
        VMX_ERROR_VMCALL_VMCS_UNCLEAR = 19, // VMCALL with non-clear VMCS
        VMX_ERROR_VMCALL_EXIT_INVALID = 20, // VMCALL with invalid VM-exit control fields
        VMX_ERROR_VMCALL_MSEG_INVALID = 22, // VMCALL with incorrect MSEG revision identifier
        VMX_ERROR_VMXOFF_SMM_DUALMONITOR = 23, // VMXOFF under dual-monitor treatment of SMIs and SMM
        VMX_ERROR_VMCALL_SMM_INVALID = 24, // VMCALL with invalid SMM-monitor features
        VMX_ERROR_ENTRY_EXECCTRL_INVALID = 25, // VM entry with invalid VM-execution control fields in executive VMCS
        VMX_ERROR_ENTRY_MOV_SS = 26, // VM entry with events blocked by MOV SS
        VMX_ERROR_INVEPT_INVALID = 28, // Invalid operand to INVEPT/INVVPID
    };

#define IA32_FXSAVE_SIZE 512

#undef CR0_PE
#undef CR0_MP
#undef CR0_EM
#undef CR0_TS
#undef CR0_ET
#undef CR0_NE
#undef CR0_WP
#undef CR0_AM
#undef CR0_NW
#undef CR0_CD
#undef CR0_PG

    enum {
        CR0_PE = (1 << 0),
        CR0_MP = (1 << 1),
        CR0_EM = (1 << 2),
        CR0_TS = (1 << 3),
        CR0_ET = (1 << 4),
        CR0_NE = (1 << 5),
        CR0_WP = (1 << 16),
        CR0_AM = (1 << 18),
        CR0_NW = (1 << 29),
        CR0_CD = (1 << 30),
        CR0_PG = (1U << 31)
    };

#undef CR4_VME
#undef CR4_PVI
#undef CR4_TSD
#undef CR4_DE
#undef CR4_PSE
#undef CR4_PAE
#undef CR4_MCE
#undef CR4_PGE

    enum {
        CR4_VME = (1 << 0),
        CR4_PVI = (1 << 1),
        CR4_TSD = (1 << 2),
        CR4_DE = (1 << 3),
        CR4_PSE = (1 << 4),
        CR4_PAE = (1 << 5),
        CR4_MCE = (1 << 6),
        CR4_PGE = (1 << 7),
        CR4_PCE = (1 << 8),
        CR4_OSFXSR = (1 << 9),
        CR4_OSXMMEXCPT = (1 << 10),
        CR4_VMXE = (1 << 13),
        CR4_SMXE = (1 << 14)
    };

    enum {
        DR6_BD = (1 << 13),
        DR7_L0 = (1 << 0),
        DR7_G0 = (1 << 1),
        DR7_L1 = (1 << 2),
        DR7_G1 = (1 << 3),
        DR7_L2 = (1 << 4),
        DR7_G2 = (1 << 5),
        DR7_L3 = (1 << 6),
        DR7_G3 = (1 << 7),
        DR7_GD = (1 << 13),
    };

#define DR6_SETBITS 0xFFFF0FF0
#define DR7_SETBITS (1 << 10)

#define HBREAK_ENABLED_MASK (DR7_L0 | DR7_G0 | \
                                DR7_L1 | DR7_G1 | \
                                DR7_L2 | DR7_G2 | \
                                DR7_L3 | DR7_G3)

    /*
    * According to SDM Vol 3B 17.2.6, DR6/7 high 32 bits should only be set to
    * 0 in 64 bits mode. Reserved bits should be 1.
    */

#define FIX_DR6(val) ((val & 0xFFFFFFFFUL) | DR6_SETBITS)
#define FIX_DR7(val) ((val & 0xFFFFFFFFUL) | DR7_SETBITS)

#undef MSR_BPU_COUNTER0

    enum {
        IA32_P5_MC_ADDR = 0x0,
        IA32_P5_MC_TYPE = 0x1,
        IA32_TSC = 0x10,
        IA32_PLATFORM_ID = 0x17,
        IA32_APIC_BASE = 0x1b,
        IA32_EBC_HARD_POWERON = 0x2a,
        IA32_EBC_SOFT_POWERON = 0x2b,
        IA32_EBC_FREQUENCY_ID = 0x2c,
        IA32_FEATURE_CONTROL = 0x3a,
        IA32_THERM_DIODE_OFFSET = 0x3f,
        IA32_BIOS_UPDT_TRIG = 0x79,
        IA32_BIOS_SIGN_ID = 0x8b,
        IA32_SMM_MONITOR_CTL = 0x9b,
        IA32_PMC0 = 0xc1,
        IA32_PMC1 = 0xc2,
        IA32_PMC2 = 0xc3,
        IA32_PMC3 = 0xc4,
        IA32_FSB_FREQ = 0xcd,
        IA32_MPERF = 0xe7,
        IA32_APERF = 0xe8,
        IA32_TEMP_TARGET = 0xee,
        IA32_MTRRCAP = 0xfe,
        IA32_BBL_CR_CTL3 = 0x11e,
        IA32_SYSENTER_CS = 0x174,
        IA32_SYSENTER_ESP = 0x175,
        IA32_SYSENTER_EIP = 0x176,
        IA32_MCG_CAP = 0x179,
        IA32_MCG_STATUS = 0x17a,
        IA32_MCG_CTL = 0x17b,
        IA32_PERFEVTSEL0 = 0x186,
        IA32_PERFEVTSEL1 = 0x187,
        IA32_PERFEVTSEL2 = 0x188,
        IA32_PERFEVTSEL3 = 0x189,
        IA32_PERF_CTL = 0x199,
        IA32_MISC_ENABLE = 0x1a0,
        IA32_DEBUGCTL = 0x1d9,
        IA32_MTRR_PHYSBASE0 = 0x200,
        IA32_MTRR_PHYSMASK0 = 0x201,
        IA32_MTRR_PHYSBASE1 = 0x202,
        IA32_MTRR_PHYSMASK1 = 0x203,
        IA32_MTRR_PHYSBASE2 = 0x204,
        IA32_MTRR_PHYSMASK2 = 0x205,
        IA32_MTRR_PHYSBASE3 = 0x206,
        IA32_MTRR_PHYSMASK3 = 0x207,
        IA32_MTRR_PHYSBASE4 = 0x208,
        IA32_MTRR_PHYSMASK4 = 0x209,
        IA32_MTRR_PHYSBASE5 = 0x20a,
        IA32_MTRR_PHYSMASK5 = 0x20b,
        IA32_MTRR_PHYSBASE6 = 0x20c,
        IA32_MTRR_PHYSMASK6 = 0x20d,
        IA32_MTRR_PHYSBASE7 = 0x20e,
        IA32_MTRR_PHYSMASK7 = 0x20f,
        IA32_MTRR_PHYSBASE8 = 0x210,
        IA32_MTRR_PHYSMASK8 = 0x211,
        IA32_MTRR_PHYSBASE9 = 0x212,
        IA32_MTRR_PHYSMASK9 = 0x213,
        MTRRFIX64K_00000 = 0x250,
        MTRRFIX16K_80000 = 0x258,
        MTRRFIX16K_A0000 = 0x259,
        MTRRFIX4K_C0000 = 0x268,
        MTRRFIX4K_F8000 = 0x26f,
        IA32_CR_PAT = 0x277,
        IA32_MC0_CTL2 = 0x280,
        IA32_MC1_CTL2 = 0x281,
        IA32_MC2_CTL2 = 0x282,
        IA32_MC3_CTL2 = 0x283,
        IA32_MC4_CTL2 = 0x284,
        IA32_MC5_CTL2 = 0x285,
        IA32_MC6_CTL2 = 0x286,
        IA32_MC7_CTL2 = 0x287,
        IA32_MC8_CTL2 = 0x288,
        IA32_MTRR_DEF_TYPE = 0x2ff,
        MSR_BPU_COUNTER0 = 0x300,
        IA32_FIXED_CTR0 = 0x309,
        IA32_FIXED_CTR1 = 0x30a,
        IA32_FIXED_CTR2 = 0x30b,
        IA32_PERF_CAPABILITIES = 0x345,
        MSR_PEBS_MATRIX_VERT = 0x3f2,
        IA32_FIXED_CTR_CTRL = 0x38d,
        IA32_PERF_GLOBAL_STATUS = 0x38e,
        IA32_PERF_GLOBAL_CTRL = 0x38f,
        IA32_PERF_GLOBAL_OVF_CTRL = 0x390,
        IA32_MC0_CTL = 0x400,
        IA32_MC0_STATUS = 0x401,
        IA32_MC0_ADDR = 0x402,
        IA32_MC0_MISC = 0x403,
        IA32_CPUID_FEATURE_MASK = 0x478,
        IA32_VMX_BASIC = 0x480,
        IA32_VMX_PINBASED_CTLS = 0x481,
        IA32_VMX_PROCBASED_CTLS = 0x482,
        IA32_VMX_EXIT_CTLS = 0x483,
        IA32_VMX_ENTRY_CTLS = 0x484,
        IA32_VMX_MISC = 0x485,
        IA32_VMX_CR0_FIXED0 = 0x486,
        IA32_VMX_CR0_FIXED1 = 0x487,
        IA32_VMX_CR4_FIXED0 = 0x488,
        IA32_VMX_CR4_FIXED1 = 0x489,
        IA32_VMX_VMCS_ENUM = 0x48a,
        IA32_VMX_SECONDARY_CTLS = 0x48b,
        IA32_VMX_EPT_VPID_CAP = 0x48c,
        IA32_VMX_TRUE_PINBASED_CTLS = 0x48d,
        IA32_VMX_TRUE_PROCBASED_CTLS = 0x48e,
        IA32_VMX_TRUE_EXIT_CTLS = 0x48f,
        IA32_VMX_TRUE_ENTRY_CTLS = 0x490,
        IA32_EFER = 0xc0000080,
        IA32_STAR = 0xc0000081,
        IA32_LSTAR = 0xc0000082,
        IA32_CSTAR = 0xc0000083,
        IA32_SF_MASK = 0xc0000084,
        IA32_FS_BASE = 0xc0000100,
        IA32_GS_BASE = 0xc0000101,
        IA32_KERNEL_GS_BASE = 0xc0000102,
        IA32_TSC_AUX = 0xc0000103
    };

    // EFER bits
    enum {
        IA32_EFER_SCE = (1u << 0),
        IA32_EFER_LME = (1u << 8),
        IA32_EFER_LMA = (1u << 10),
        IA32_EFER_XD = (1u << 11)
    };

    // Feature control MSR bits
    enum {
        FC_LOCKED = 0x00001,
        FC_VMXON_INSMX = 0x00002,
        FC_VMXON_OUTSMX = 0x00004
    };

#undef EFLAGS_TF
#undef EFLAGS_NT

    enum {
        EFLAGS_CF = (1u << 0),
        EFLAGS_PF = (1u << 2),
        EFLAGS_AF = (1u << 4),
        EFLAGS_ZF = (1u << 6),
        EFLAGS_TF = (1u << 8),
        EFLAGS_IF = (1u << 9),
        EFLAGS_DF = (1u << 10),
        EFLAGS_OF = (1u << 11),
        EFLAGS_IOPL = (3u << 12),
        EFLAGS_NT = (1u << 14),
        EFLAGS_VM = (1u << 17),

        EFLAGS_SETBITS = (1u << 1)
    };

    // Intel SDM Vol. 2A: Table 3-4. Intel 64 and IA-32 General Exceptions
    enum {
        VECTOR_DE = 0,  // Divide Error
        VECTOR_DB = 1,  // Debug
        VECTOR_NMI = 2,  // NMI Interrupt
        VECTOR_BP = 3,  // Breakpoint
        VECTOR_OF = 4,  // Overflow
        VECTOR_BR = 5,  // BOUND Range Exceeded
        VECTOR_UD = 6,  // Undefined Opcode
        VECTOR_NM = 7,  // Device Not Available (No Math Coprocessor)
        VECTOR_DF = 8,  // Double Fault
        VECTOR_TS = 10,  // Invalid TSS
        VECTOR_NP = 11,  // Segment Not Present
        VECTOR_SS = 12,  // Stack Segment Fault
        VECTOR_GP = 13,  // General Protection
        VECTOR_PF = 14,  // Page Fault
        VECTOR_MF = 16,  // Floating-Point Error (Math Error)
        VECTOR_AC = 17,  // Alignment Check
        VECTOR_MC = 18,  // Machine Check
        VECTOR_XM = 19,  // SIMD Floating-Point Numeric Error
        VECTOR_VE = 20   // Virtualization Exception
    };

    // For IA32_APIC_BASE MSR (see IASDM Vol. 3A 10.4.4)
#define APIC_BASE_BSP (1ULL << 8)
#define APIC_BASE_ENABLE  (1ULL << 11)
#define APIC_BASE_DEFAULT_ADDR  0xfee00000
#define APIC_BASE_ADDR_MASK (((1ULL << 24) - 1) << 12)
    // Reserve bits 0 through 7, bits 9 and 10, and
    // bits MAXPHYADDR1 through 63 , assuming MAXPHYADDR == 36.
    // TODO: Use CPUID to obtain the true MAXPHYADDR
#define APIC_BASE_MASK (APIC_BASE_BSP | APIC_BASE_ENABLE | APIC_BASE_ADDR_MASK)

#define IA32_VMX_MISC_UG_AVAILABLE (0x0000000000000020)

    typedef struct _VMX_INFO {
        union {
            u64 BaseInfo;

            struct {
                u32 Identifier;

                struct {
                    u32 NumberOfBytes : 16;
                    u32 Limit : 1;
                    u32 DualMonitor : 1;
                    u32 Type : 4;
                    u32 NOTHING : 10;
                };
            };
        };

        ULARGE_INTEGER Pin;
        ULARGE_INTEGER PinFixed;

        ULARGE_INTEGER Primary;
        ULARGE_INTEGER PrimaryFixed;

        ULARGE_INTEGER Secondary;
        ULARGE_INTEGER SecondaryFixed;

        ULARGE_INTEGER Exit;
        ULARGE_INTEGER ExitFixed;

        ULARGE_INTEGER Entry;
        ULARGE_INTEGER EntryFixed;

        ULARGE_INTEGER Cr0;
        ULARGE_INTEGER Cr0Fixed;
        ULARGE_INTEGER Cr0Mask;
        ULARGE_INTEGER Cr0ReadShadow;

        ULARGE_INTEGER Cr4;
        ULARGE_INTEGER Cr4Fixed;
        ULARGE_INTEGER Cr4Mask;
        ULARGE_INTEGER Cr4ReadShadow;
    }VMX_INFO, *PVMX_INFO;

    enum {
        ENCODE_16,
        ENCODE_64,
        ENCODE_32,
        ENCODE_NATURAL
    };

#define ENCODE_MASK 3
#define ENCODE_SHIFT 13

    typedef struct _VMX_VMCS {
        u32 Identifier;
        u32 Abort;
        u8 Data[PAGE_SIZE - 8];
    } VMX_VMCS, *PVMX_VMCS;

    typedef struct _INVEPT_DESCRIPTOR {
        u64 EptPointer;
        u64 Reserved;
    }INVEPT_DESCRIPTOR, *PINVEPT_DESCRIPTOR;

    VMX_RESULT
        NTAPI
        __ops_invept(
            __in u32 Type,
            __in PINVEPT_DESCRIPTOR Descriptor
        );

    VMX_RESULT
        NTAPI
        __vmx_on(
            __in u64 * VmsSupportPhysicalAddress
        );

    VMX_RESULT
        NTAPI
        __vmx_off(
            void
        );

    VMX_RESULT
        NTAPI
        __vmx_vmclear(
            __in u64 * VmcsPhysicalAddress
        );

    VMX_RESULT
        NTAPI
        __vmx_vmptrld(
            __in u64 * VmcsPhysicalAddress
        );

    VMX_RESULT
        NTAPI
        __vmx_vmptrst(
            __in u64 * VmcsPhysicalAddress
        );

    VMX_RESULT
        NTAPI
        __vmx_vmread(
            __in u Field,
            __out u * Value
        );

    VMX_RESULT
        NTAPI
        __vmx_vmwrite(
            __in u Field,
            __in u Value
        );

    VMX_RESULT
        NTAPI
        __vmx_vmlaunch(
            void
        );

    VMX_RESULT
        NTAPI
        __vmx_vmresume(
            void
        );

    VMX_RESULT
        NTAPI
        __vmx_vmread_common(
            __in u Field,
            __out u64 * Value
        );

    VMX_RESULT
        NTAPI
        __vmx_vmwrite_common(
            __in u Field,
            __in u64 Value
        );

    typedef struct _SEGMENT_DESCRIPTOR {
        u16 Selector;

        union {
            struct {
                u32 LimitLow : 16;
                u32 LimitHigh : 4;
            };

            u32 Limit;
        };

        union {
            struct {
                u64 BaseLow : 16;
                u64 BaseMiddle : 8;
                u64 BaseHigh : 8;
#ifdef _WIN64
                u64 BaseUpper : 32;
#endif // _WIN64
            };

            u Base;
        };

        union {
            struct {
                u32 Type : 4; //Segment type
                u32 S : 1; //Descriptor type (0 = system; 1 = code or data)
                u32 DPL : 2; //Descriptor privilege level
                u32 P : 1; //Segment present
                u32 NOTHING : 4; //Reserved
                u32 AVL : 1; //Available for use by system software
                u32 L : 1; //64-bit mode active (for CS only)
                u32 DB : 1; //Default operation size (0 = 16-bit segment; 1 = 32-bit segment)
                u32 G : 1; //Granularity
                u32 Unusable : 1; //Segment unusable (0 = usable; 1 = unusable)
                u32 NOTHING : 15; //Reserved
            };

            u32 AccessRights;
        };
    } SEGMENT_DESCRIPTOR, *PSEGMENT_DESCRIPTOR;

    void
        NTAPI
        __vmx_prepare_segment(
            __in PKDESCRIPTOR Descriptor,
            __in UINT16 Selector,
            __out PSEGMENT_DESCRIPTOR SegmentDescriptor
        );

    enum {
        INTERRUPT = 0,
        NMI = 2,
        EXCEPTION = 3,
        SWINT = 4,
        PRIV_TRAP = 5,
        UNPRIV_TRAP = 6,
        OTHER = 7
    };

#define NO_ERROR_CODE ~0U

    typedef union _ENTRY_INTERRUPTION {
        u64 Information;

        struct {
            u64 Vector : 8;
            u64 Type : 3;
            u64 DeliverErrorCode : 1;
            u64 NOTHING : 19;
            u64 Valid : 1;
        };
    }ENTRY_INTERRUPTION, *PENTRY_INTERRUPTION;

    typedef union _REASON {
        struct {
            u64 BasicReason : 16;
            u64 NOTHING : 11;

            // A VM exit saves this bit as 1 to indicate that the VM exit was incident to enclave mode.
            u64 EnclaveMode : 1;
            u64 PendingMTF : 1;
            u64 ExitRoot : 1; // VM exit from VMX root operation
            u64 NOTHING : 1;
            u64 EnterFail : 1; // 0 = true VM exit; 1 = VM-entry failure
        };
    }REASON, *PREASON;

    typedef union _EXIT_INTERRUPTION {
        u64 Information;

        struct {
            u64 Vector : 8;
            u64 Type : 3;
            u64 ErrorCode : 1;
            u64 NmiUnmasking : 1;
            u64 NOTHING : 18;
            u64 Valid : 1;
        };
    }EXIT_INTERRUPTION, *PEXIT_INTERRUPTION;

    typedef union _IDT_VECTORING {
        u64 Information;

        struct {
            u64 Vector : 8;
            u64 Type : 3;
            u64 ErrorCode : 1;
            u64 Undefined : 1;
            u64 NOTHING : 18;
            u64 Valid : 1;
        };
    }IDT_VECTORING, *PIDT_VECTORING;

    typedef union _INSTRUCTION {
        u64 Information;

        struct {
            union {
                struct {
                    u64 Scaling : 2;

                    union {
                        struct {
                            u64 NOTHING : 1;
                            u64 Reg1 : 4;
                        };

                        u64 DestReg : 4;
                    };
                };

                u64 NOTHING : 7;
            };

            u64 AddrSize : 3;

            union {
                struct {
                    u64 MemOrReg : 1;
                    u64 OpSize : 2;
                };

                u64 NOTHING : 5;
            };

            u64 Segment : 3;

            union {
                struct {
                    u64 IndexReg : 4;
                    u64 IndexRegInvalid : 1;
                    u64 BaseReg : 4;
                    u64 BaseRegInvalid : 1;

                    union {
                        struct {
                            u64 InstructionIdentity : 2;
                            u64 NOTHING : 2;
                        };

                        u64 Reg2 : 4;
                    };
                };

                u64 NOTHING : 14;
            };
        };
    }INSTRUCTION, *PINSTRUCTION;

    typedef union _QUALIFICATION {
        u64 Address;

        struct {
            u64 Size : 3;
            u64 Direction : 1;
            u64 String : 1;
            u64 Rep : 1;
            u64 Encoding : 1;
            u64 NOTHING : 9;
            u64 Port : 16;
        } IO;

        struct {
            u64 Number : 4;
            u64 Type : 2;
            u64 LMSW : 2;
            u64 GpReg : 4;
            u64 NOTHING : 4;
            u64 LMSWSource : 16;
        } CR;

        struct {
            u64 Number : 3;
            u64 NOTHING : 1;
            u64 Direction : 1;
            u64 NOTHING : 3;
            u64 GpReg : 4;
            u64 NOTHING : 20;
        } DR;

        struct {
            u64 Selector : 16;
            u64 NOTHING : 14;
            u64 Source : 2;
        }TS;

        struct {
            u64 Offset : 12;
            u64 Access : 3;
        } APIC;

        struct {
            u64 Vector;
        } VAPIC_EOI;

        struct {
            u64 Read : 1;
            u64 Write : 1;
            u64 Execute : 1;
            u64 Readable : 1;
            u64 Writeable : 1;
            u64 Executable : 1;
            u64 NOTHING : 1;
            u64 Gla1 : 1;
            u64 Gla2 : 1;
            u64 NOTHING : 3;  /* bits 11:9 */
            u64 NmiBlock : 1;
            u64 NOTHING : 19;
            u64 NOTHING : 32;
        } EPT;
    }QUALIFICATION, *PQUALIFICATION;

    typedef struct _GUEST_STATE {
        u64 GuestRip;
        u64 GuestRsp;
        u64 GuestRFlags;
        ENTRY_INTERRUPTION EntryInterruption;
        REASON Reason;
        EXIT_INTERRUPTION ExitInterruption;
        IDT_VECTORING IdtVectoring;
        INSTRUCTION Instruction;
        u64 InstructionLength;
        QUALIFICATION Qualification;
        b Injected;
    } GUEST_STATE, *PGUEST_STATE;

    typedef struct _CCB {
        DECLSPEC_ALIGN(PAGE_SIZE) struct {
            u8 KernelStack[KERNEL_LARGE_STACK_SIZE - sizeof(REGISTERS_FRAME)];
            REGISTERS_FRAME Registers;
        };

        DECLSPEC_ALIGN(PAGE_SIZE) struct {
            VMX_VMCS VmsSupport;
            VMX_VMCS Vmcs;
            u8 Bitmap[PAGE_SIZE];
        }Region;

        ULARGE_INTEGER ExceptionBitmap;

        CPU_FEATURE Feature;
        VMX_INFO VmxInfo;

        PHYSICAL_ADDRESS VmsSupport;
        PHYSICAL_ADDRESS Vmcs;
        PHYSICAL_ADDRESS Bitmap;

        GUEST_STATE GuestState;
    }CCB, *PCCB;

    typedef
        void
        (NTAPI * PEXIT_HANDLER)(
            __inout PCCB Block
            );

    u32
        NTAPI
        __ops_segment_limit(
            __in u32 Selector
        );

    u32
        NTAPI
        __ops_segment_ar(
            __in u32 Selector
        );

    void
        NTAPI
        __vmx_vmentry(
            void
        );

    status
        NTAPI
        __vmx_start(
            __in PCCB Block
        );

    status
        NTAPI
        __vmx_entry(
            __in PREGISTERS_FRAME Registers
        );

    void
        NTAPI
        VmxStartAllProcessors(
            __in PCCB * Block
        );

#ifdef __cplusplus
}
#endif	/* __cplusplus */

#endif // !_VMX_H_
