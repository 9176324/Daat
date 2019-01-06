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

#include "Daat.h"

#include "handler.h"
#include "vcpu.h"
#include "vmx.h"

#include "..\Tiferet\Reload.h"

PRELOADER_PARAMETER_BLOCK ReloaderBlock;

VOID
NTAPI
VmxStartProcessors(
    __in struct _KDPC * Dpc,
    __in PVOID DeferredContext,
    __in PVOID SystemArgument1,
    __in PVOID SystemArgument2
)
{
    PCCB DpcNotify = NULL;
    PCCB CurrentBlock = NULL;

#ifndef PUBLIC
    DbgPrint(
        "[Sefirot] [Daat] < %p > current processor number\n",
        KeGetCurrentProcessorNumber());
#endif // !PUBLIC

    CurrentBlock = DeferredContext;

    __vmx_start(&CurrentBlock->Registers);

    DpcNotify = SystemArgument1;

    KeSetEvent(DpcNotify, LOW_PRIORITY, FALSE);
}

VOID
NTAPI
VmxStartAllProcessors(
    __in PKEVENT Notify
)
{
    UNICODE_STRING RoutineString = { 0 };
    PCCHAR NumberProcessors = NULL;
    CHAR Index = 0;
    KEVENT DpcNotify = { 0 };
    KDPC Dpc = { 0 };
    PCCB ControlBlock = NULL;
    PCCB CurrentBlock = NULL;

    RtlInitUnicodeString(&RoutineString, L"KeNumberProcessors");

    NumberProcessors = MmGetSystemRoutineAddress(&RoutineString);

    ControlBlock = ExAllocatePool(
        NonPagedPool,
        *NumberProcessors * sizeof(CCB));

    if (NULL != ControlBlock) {
        RtlZeroMemory(
            ControlBlock,
            *NumberProcessors * sizeof(CCB));

        ReloaderBlock->CpuControlBlock = ControlBlock;

        for (Index = 0;
            Index < *NumberProcessors;
            Index++) {
            CurrentBlock = ControlBlock + Index;

            KeInitializeEvent(
                &DpcNotify,
                SynchronizationEvent,
                FALSE);

            KeInitializeDpc(
                &Dpc,
                VmxStartProcessors,
                CurrentBlock);

            KeSetTargetProcessorDpc(&Dpc, Index);
            KeSetImportanceDpc(&Dpc, HighImportance);

            if (FALSE != KeInsertQueueDpc(
                &Dpc,
                &DpcNotify,
                NULL)) {
                KeWaitForSingleObject(
                    &DpcNotify,
                    Executive,
                    KernelMode,
                    FALSE,
                    NULL);
            }
        }
    }

    KeSetEvent(Notify, LOW_PRIORITY, FALSE);
}

NTSTATUS
NTAPI
DriverEntry(
    __in PRELOADER_PARAMETER_BLOCK Block,
    __in PVOID Reserved
)
{
    NTSTATUS Status = STATUS_SUCCESS;
    WORK_QUEUE_ITEM WorkerItem = { 0 };
    KEVENT Notify = { 0 };

    ReloaderBlock = Block;

    KeInitializeEvent(
        &Notify,
        SynchronizationEvent,
        FALSE);

    ExInitializeWorkItem(
        &WorkerItem,
        VmxStartAllProcessors,
        &Notify);

    ExQueueWorkItem(&WorkerItem, CriticalWorkQueue);

    Status = KeWaitForSingleObject(
        &Notify,
        Executive,
        KernelMode,
        FALSE,
        NULL);

#ifndef PUBLIC
    DbgPrint(
        "[Sefirot] [Daat] load\n");
#endif // !PUBLIC

    return Status;
}
