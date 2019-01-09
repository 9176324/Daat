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
#include <devicedefs.h>

#include "Daat.h"

#include "handler.h"
#include "vcpu.h"
#include "vmx.h"

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

    CurrentBlock = DeferredContext;

#ifndef PUBLIC
    DbgPrint(
        "[Sefirot] [Daat] < %p : %p > current processor\n",
        KeGetCurrentProcessorNumber(),
        CurrentBlock);
#endif // !PUBLIC

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

VOID
NTAPI
DriverUnload(
    __in PDRIVER_OBJECT DriverObject
);

NTSTATUS
NTAPI
DeviceCreate(
    __in PDEVICE_OBJECT DeviceObject,
    __in PIRP Irp
);

NTSTATUS
NTAPI
DeviceClose(
    __in PDEVICE_OBJECT DeviceObject,
    __in PIRP Irp
);

NTSTATUS
NTAPI
DeviceWrite(
    __in PDEVICE_OBJECT DeviceObject,
    __in PIRP Irp
);

NTSTATUS
NTAPI
DeviceRead(
    __in PDEVICE_OBJECT DeviceObject,
    __in PIRP Irp
);

NTSTATUS
NTAPI
DeviceControl(
    __in PDEVICE_OBJECT DeviceObject,
    __in PIRP Irp
);

NTSTATUS
NTAPI
DriverEntry(
    __in PDRIVER_OBJECT DriverObject,
    __in PUNICODE_STRING RegistryPath
)
{
    NTSTATUS Status = STATUS_SUCCESS;
    PDEVICE_OBJECT DeviceObject = NULL;
    UNICODE_STRING DeviceName = { 0 };
    UNICODE_STRING SymbolicLinkName = { 0 };
    WORK_QUEUE_ITEM WorkerItem = { 0 };
    KEVENT Notify = { 0 };

    RtlInitUnicodeString(&DeviceName, LOADER_DEVICE_STRING);

    Status = IoCreateDevice(
        DriverObject,
        0,
        &DeviceName,
        FILE_DEVICE_UNKNOWN,
        FILE_DEVICE_SECURE_OPEN,
        FALSE,
        &DeviceObject);

    if (NT_SUCCESS(Status)) {
        DriverObject->MajorFunction[IRP_MJ_CREATE] = (PDRIVER_DISPATCH)DeviceCreate;
        DriverObject->MajorFunction[IRP_MJ_CLOSE] = (PDRIVER_DISPATCH)DeviceClose;
        DriverObject->MajorFunction[IRP_MJ_WRITE] = (PDRIVER_DISPATCH)DeviceWrite;
        DriverObject->MajorFunction[IRP_MJ_READ] = (PDRIVER_DISPATCH)DeviceRead;
        DriverObject->MajorFunction[IRP_MJ_DEVICE_CONTROL] = (PDRIVER_DISPATCH)DeviceControl;

        RtlInitUnicodeString(&SymbolicLinkName, LOADER_SYMBOLIC_STRING);

        Status = IoCreateSymbolicLink(&SymbolicLinkName, &DeviceName);

        if (TRACE(Status)) {
            DriverObject->DriverUnload = (PDRIVER_UNLOAD)DriverUnload;

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
        }
        else {
            IoDeleteDevice(DeviceObject);
        }
    }

    return Status;
}

VOID
NTAPI
DriverUnload(
    __in PDRIVER_OBJECT DriverObject
)
{
    UNICODE_STRING SymbolicLinkName = { 0 };

    RtlInitUnicodeString(&SymbolicLinkName, LOADER_SYMBOLIC_STRING);
    IoDeleteSymbolicLink(&SymbolicLinkName);
    IoDeleteDevice(DriverObject->DeviceObject);

#ifndef VMP
    DbgPrint("Shark - unload\n");
#endif // !VMP
}

NTSTATUS
NTAPI
DeviceCreate(
    __in PDEVICE_OBJECT DeviceObject,
    __in PIRP Irp
)
{
    NTSTATUS Status = STATUS_SUCCESS;

    Irp->IoStatus.Information = 0;
    Irp->IoStatus.Status = Status;

    IoCompleteRequest(
        Irp,
        IO_NO_INCREMENT);

    return Status;
}

NTSTATUS
NTAPI
DeviceClose(
    __in PDEVICE_OBJECT DeviceObject,
    __in PIRP Irp
)
{
    NTSTATUS Status = STATUS_SUCCESS;

    Irp->IoStatus.Information = 0;
    Irp->IoStatus.Status = Status;

    IoCompleteRequest(
        Irp,
        IO_NO_INCREMENT);

    return Status;
}

NTSTATUS
NTAPI
DeviceWrite(
    __in PDEVICE_OBJECT DeviceObject,
    __in PIRP Irp
)
{
    NTSTATUS Status = STATUS_SUCCESS;

    Irp->IoStatus.Information = 0;
    Irp->IoStatus.Status = Status;

    IoCompleteRequest(
        Irp,
        IO_NO_INCREMENT);

    return Status;
}

NTSTATUS
NTAPI
DeviceRead(
    __in PDEVICE_OBJECT DeviceObject,
    __in PIRP Irp
)
{
    NTSTATUS Status = STATUS_SUCCESS;

    Irp->IoStatus.Information = 0;
    Irp->IoStatus.Status = Status;

    IoCompleteRequest(
        Irp,
        IO_NO_INCREMENT);

    return Status;
}

NTSTATUS
NTAPI
DeviceControl(
    __in PDEVICE_OBJECT DeviceObject,
    __in PIRP Irp
)
{
    NTSTATUS Status = STATUS_SUCCESS;
    PIO_STACK_LOCATION IrpSp = NULL;

    IrpSp = IoGetCurrentIrpStackLocation(Irp);

    switch (IrpSp->Parameters.DeviceIoControl.IoControlCode) {
    default: {
        Irp->IoStatus.Information = 0;
        Status = STATUS_INVALID_DEVICE_REQUEST;

        break;
    }
    }

    Irp->IoStatus.Status = Status;

    IoCompleteRequest(
        Irp,
        IO_NO_INCREMENT);

    return Status;
}
