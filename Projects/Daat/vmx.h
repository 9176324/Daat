/*
*
* Copyright (c) 2018 by blindtiger. All rights reserved.
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
* The Initial Developer of the Original e is blindtiger.
*
*/

#ifndef _VMX_H_
#define _VMX_H_

#ifdef __cplusplus
/* Assume byte packing throughout */
extern "C" {
#endif	/* __cplusplus */

    typedef struct _CCB * PCCB;
    typedef struct _REGISTERS_FRAME * PREGISTERS_FRAME;

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
        VMX_EXIT_VMCALL = 18,
        VMX_EXIT_VMCLEAR = 19,
        VMX_EXIT_VMLAUNCH = 20,
        VMX_EXIT_VMPTRLD = 21,
        VMX_EXIT_VMPTRST = 22,
        VMX_EXIT_VMREAD = 23,
        VMX_EXIT_VMRESUME = 24,
        VMX_EXIT_VMWRITE = 25,
        VMX_EXIT_VMXOFF = 26,
        VMX_EXIT_VMXON = 27,
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
        VMX_EXIT_GDT_IDT_ACCESS = 46,
        VMX_EXIT_LDT_TR_ACCESS = 47,
        VMX_EXIT_EPT_VIOLATION = 48,
        VMX_EXIT_EPT_MISCONFIG = 49,
        VMX_EXIT_INVEPT = 50,
        VMX_EXIT_RDTSCP = 51,
        VMX_EXIT_VMX_TIMER_EXIT = 52,
        VMX_EXIT_INVVPID = 53,
        VMX_EXIT_WBINVD = 54,
        VMX_EXIT_XSETBV = 55,
        VMX_EXIT_APIC_WRITE = 56,
        VMX_EXIT_RDRAND = 57,
        VMX_EXIT_INVPCID = 58,
        VMX_EXIT_VMFUNC = 59,
        VMX_EXIT_ENCLS = 60,
        VMX_EXIT_RDSEED = 61,
        VMX_EXIT_XSAVES = 63,
        VMX_EXIT_XRSTORS = 64
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
        CR0_PE = (1ULL << 0),
        CR0_MP = (1ULL << 1),
        CR0_EM = (1ULL << 2),
        CR0_TS = (1ULL << 3),
        CR0_ET = (1ULL << 4),
        CR0_NE = (1ULL << 5),
        CR0_WP = (1ULL << 16),
        CR0_AM = (1ULL << 18),
        CR0_NW = (1ULL << 29),
        CR0_CD = (1ULL << 30),
        CR0_PG = (1ULL << 31)
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
        CR4_VME = (1ULL << 0),
        CR4_PVI = (1ULL << 1),
        CR4_TSD = (1ULL << 2),
        CR4_DE = (1ULL << 3),
        CR4_PSE = (1ULL << 4),
        CR4_PAE = (1ULL << 5),
        CR4_MCE = (1ULL << 6),
        CR4_PGE = (1ULL << 7),
        CR4_PCE = (1ULL << 8),
        CR4_OSFXSR = (1ULL << 9),
        CR4_OSXMMEXCPT = (1ULL << 10),
        CR4_VMXE = (1ULL << 13),
        CR4_SMXE = (1ULL << 14)
    };

    enum {
        DR6_BD = (1ULL << 13),
        DR7_L0 = (1ULL << 0),
        DR7_G0 = (1ULL << 1),
        DR7_L1 = (1ULL << 2),
        DR7_G1 = (1ULL << 3),
        DR7_L2 = (1ULL << 4),
        DR7_G2 = (1ULL << 5),
        DR7_L3 = (1ULL << 6),
        DR7_G3 = (1ULL << 7),
        DR7_GD = (1ULL << 13),
    };
#define DR6_SETBITS 0xFFFF0FF0
#define DR7_SETBITS (1ULL << 10)
#define HBREAK_ENABLED_MASK (DR7_L0 | DR7_G0 | \
                            DR7_L1 | DR7_G1 | \
                            DR7_L2 | DR7_G2 | \
                            DR7_L3 | DR7_G3)

    /*
    * According to SDM Vol 3B 17.2.6, DR6/7 high 32 bits should only be set to
    * 0 in 64 bits mode. Reserved bits should be 1.
    */

#define FIX_DR6(val) ((val & 0xffffffff) | DR6_SETBITS)
#define FIX_DR7(val) ((val & 0xffffffff) | DR7_SETBITS)

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
            ULONG64 BaseInfo;

            struct {
                ULONG Identifier;

                struct {
                    ULONG NumberOfBytes : 16;
                    ULONG Limit : 1;
                    ULONG DualMonitor : 1;
                    ULONG Type : 4;
                    ULONG NOTHING : 10;
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
        ULONG Identifier;
        ULONG Abort;
        UCHAR Data[PAGE_SIZE - 8];
    } VMX_VMCS, *PVMX_VMCS;

    VOID
        NTAPI
        __ops_writecr0(
            __in SIZE_T Value
        );

    VOID
        NTAPI
        __ops_writecr3(
            __in SIZE_T Value
        );

    VOID
        NTAPI
        __ops_writecr4(
            __in SIZE_T Value
        );

#ifdef _WIN64
    VOID
        NTAPI
        __ops_writecr8(
            __in SIZE_T Value
        );
#endif // _WIN64

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

    typedef struct _INVEPT_DESCRIPTOR {
        ULONG64 EptPointer;
        ULONG64 Reserved;
    }INVEPT_DESCRIPTOR, *PINVEPT_DESCRIPTOR;

    VMX_RESULT
        NTAPI
        __ops_invept(
            __in ULONG Type,
            __in PINVEPT_DESCRIPTOR Descriptor
        );

    VMX_RESULT
        NTAPI
        __vmx_on(
            __in ULONG64 * VmsSupportPhysicalAddress
        );

    VMX_RESULT
        NTAPI
        __vmx_off(
            VOID
        );

    VMX_RESULT
        NTAPI
        __vmx_vmclear(
            __in ULONG64 * VmcsPhysicalAddress
        );

    VMX_RESULT
        NTAPI
        __vmx_vmptrld(
            __in ULONG64 * VmcsPhysicalAddress
        );

    VMX_RESULT
        NTAPI
        __vmx_vmptrst(
            __in ULONG64 * VmcsPhysicalAddress
        );

    VMX_RESULT
        NTAPI
        __vmx_vmread(
            __in SIZE_T Field,
            __out SIZE_T * Value
        );

    VMX_RESULT
        NTAPI
        __vmx_vmwrite(
            __in SIZE_T Field,
            __in SIZE_T Value
        );

    VMX_RESULT
        NTAPI
        __vmx_vmlaunch(
            VOID
        );

    VMX_RESULT
        NTAPI
        __vmx_vmresume(
            VOID
        );

    VMX_RESULT
        NTAPI
        __vmx_vmread_common(
            __in SIZE_T Field,
            __out ULONG64 * Value
        );

    VMX_RESULT
        NTAPI
        __vmx_vmwrite_common(
            __in SIZE_T Field,
            __in ULONG64 Value
        );

    typedef struct _SEGMENT_DESCRIPTOR {
        USHORT Selector;

        union {
            struct {
                ULONG LimitLow : 16;
                ULONG LimitHigh : 4;
            };

            ULONG Limit;
        };

        union {
            struct {
                ULONG64 BaseLow : 16;
                ULONG64 BaseMiddle : 8;
                ULONG64 BaseHigh : 8;
#ifdef _WIN64
                ULONG64 BaseUpper : 32;
#endif // _WIN64
            };

            ULONG_PTR Base;
        };

        union {
            struct {
                ULONG Type : 4; //Segment type
                ULONG S : 1; //Descriptor type (0 = system; 1 = code or data)
                ULONG DPL : 2; //Descriptor privilege level
                ULONG P : 1; //Segment present
                ULONG NOTHING : 4; //Reserved
                ULONG AVL : 1; //Available for use by system software
                ULONG L : 1; //64-bit mode active (for CS only)
                ULONG DB : 1; //Default operation size (0 = 16-bit segment; 1 = 32-bit segment)
                ULONG G : 1; //Granularity
                ULONG Unusable : 1; //Segment unusable (0 = usable; 1 = unusable)
                ULONG NOTHING : 15; //Reserved
            };

            ULONG AccessRights;
        };
    } SEGMENT_DESCRIPTOR, *PSEGMENT_DESCRIPTOR;

    ULONG
        NTAPI
        __ops_segment_limit(
            __in ULONG Selector
        );

    ULONG
        NTAPI
        __ops_segment_ar(
            __in ULONG Selector
        );

    VOID
        NTAPI
        __vmx_vmentry(
            VOID
        );

    NTSTATUS
        NTAPI
        __vmx_start(
            __in PCCB Block
        );

    NTSTATUS
        NTAPI
        __vmx_entry(
            __in PREGISTERS_FRAME Registers
        );

#ifdef __cplusplus
}
#endif	/* __cplusplus */

#endif // !_VMX_H_
