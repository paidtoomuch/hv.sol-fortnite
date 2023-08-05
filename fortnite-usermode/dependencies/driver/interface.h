#pragma once
#include "../fortnite-driver/impl/communication/structs.h"

/* typedefs */


typedef struct _IO_STATUS_BLOCK {
	union {
		NTSTATUS Status;
		PVOID    Pointer;
	};
	ULONG_PTR Information;
} IO_STATUS_BLOCK, * PIO_STATUS_BLOCK;


/* typedefs */

typedef
VOID
( NTAPI* PIO_APC_ROUTINE ) (
	IN PVOID ApcContext,
	IN PIO_STATUS_BLOCK IoStatusBlock,
	IN ULONG Reserved
	);

extern "C" __int64 DirectIO(
	HANDLE FileHandle,
	HANDLE Event,
	PIO_APC_ROUTINE
	ApcRoutine,
	PVOID ApcContext,
	PIO_STATUS_BLOCK IoStatusBlock,
	ULONG IoControlCode,
	PVOID InputBuffer,
	ULONG InputBufferLength,
	PVOID OutputBuffer,
	ULONG OutputBufferLength );

