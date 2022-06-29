// driver_template.cpp : Defines the entry point for the console application.
//


#include "bugcheck.h"

void BugCheckUnload(PDRIVER_OBJECT DriverObject)
{
   UNICODE_STRING SymbolicLinkName = RTL_CONSTANT_STRING(L"\\??\\BugCheckDriver");

   IoDeleteSymbolicLink(&SymbolicLinkName);
   IoDeleteDevice(DriverObject->DeviceObject);
}

NTSTATUS BugCheckCreateClose(PDEVICE_OBJECT DeviceObject, PIRP Irp)
{
	UNREFERENCED_PARAMETER(DeviceObject);
	Irp->IoStatus.Status = STATUS_SUCCESS;
	Irp->IoStatus.Information = 0;
	IoCompleteRequest(Irp, IO_NO_INCREMENT);
	return STATUS_SUCCESS;
}

NTSTATUS DoBugCheck(PDEVICE_OBJECT DeviceObject, PIRP Irp)
{
	UNREFERENCED_PARAMETER(DeviceObject);

	NTSTATUS Status = STATUS_SUCCESS;
	ULONG_PTR Information = 0;

	PIO_STACK_LOCATION IrpSp = IoGetCurrentIrpStackLocation(Irp);

	while(1)
	{
		if(IrpSp->Parameters.Write.Length < sizeof(BugCheckData))
		{
			Status = STATUS_BUFFER_TOO_SMALL;
		      break;
		}
		BugCheckData* bcd = (BugCheckData*) Irp->UserBuffer;


		KeBugCheckEx(bcd->BugCheckCode, bcd->BugCheckParameter1, bcd->BugCheckParameter2, bcd->BugCheckParameter3, bcd->BugCheckParameter4);
	}


	return Status;
}


NTSTATUS DriverEntry(PDRIVER_OBJECT DriverObject, PUNICODE_STRING RegistryPath)
{
	UNREFERENCED_PARAMETER(RegistryPath);
	PDEVICE_OBJECT DeviceObject;
	NTSTATUS Status;

    DriverObject->DriverUnload = BugCheckUnload;
	DriverObject->MajorFunction[IRP_MJ_CREATE] = BugCheckCreateClose;
	DriverObject->MajorFunction[IRP_MJ_CLOSE] = BugCheckCreateClose;
	DriverObject->MajorFunction[IRP_MJ_WRITE] = DoBugCheck;

	UNICODE_STRING DeviceName = RTL_CONSTANT_STRING(L"\\Device\\BugCheckDriver");
	UNICODE_STRING SymbolicLinkName = RTL_CONSTANT_STRING(L"\\??\\BugCheckDriver");
 
    Status = IoCreateDevice(DriverObject, 0, &DeviceName, FILE_DEVICE_UNKNOWN, 0, FALSE, &DeviceObject);
	if(Status < STATUS_SUCCESS)
	{
		KdPrint(("Failed to create device object, error %d\n", Status));
		return Status;
	}

	Status = IoCreateSymbolicLink(&SymbolicLinkName, &DeviceName);
	if(Status < STATUS_SUCCESS)
	{
		KdPrint(("Failed to create symbolic link to driver, error %d\n", Status));
		IoDeleteDevice(DeviceObject);
		return Status;
	}

	return STATUS_SUCCESS;
}

