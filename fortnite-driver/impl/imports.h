#pragma once

#define VentroAPI inline

namespace imports
{
	struct m_imported
	{
		uintptr_t ex_allocate_pool;
		uintptr_t zw_query_system_information;
		uintptr_t ex_free_pool_with_tag;
		//uintptr_t ke_get_current_thread; //ntoskrnl.exe KeGetCurrentThread -> 
		uintptr_t rtl_init_ansi_string;
		uintptr_t rtl_ansi_string_to_unicode_string;
		uintptr_t mm_copy_virtual_memory;
		uintptr_t io_get_current_process;
		uintptr_t ps_lookup_process_by_process_id;
		uintptr_t ps_get_process_peb;
		uintptr_t ob_reference_object_safe;
		//uintptr_t zw_allocate_virtual_memory;
		uintptr_t rtl_compare_unicode_string;
		uintptr_t rtl_free_unicode_string;
		uintptr_t obf_dereference_object;
		uintptr_t mm_copy_memory;
		uintptr_t ps_get_process_section_base_address;
		//uintptr_t zw_query_virtual_memory;
		//uintptr_t zw_free_virtual_memory;
		uintptr_t io_create_driver;
		uintptr_t io_allocate_mdl;
		uintptr_t mm_probe_and_lock_pages;
		uintptr_t mm_map_locked_pages_specify_cache;
		uintptr_t mm_protect_mdl_system_address;
		uintptr_t mm_unmap_locked_pages;
		uintptr_t mm_unlock_pages;
		uintptr_t io_free_mdl;
		uintptr_t iof_complete_request;
		uintptr_t rtl_init_unicode_string;
		//uintptr_t ex_raise_hard_error;
		uintptr_t io_create_symbolic_link;
		uintptr_t io_delete_device;
		uintptr_t io_create_device;
		uintptr_t rtl_get_version;
		uintptr_t mm_map_io_space_ex;
		uintptr_t mm_unmap_io_space;
		uintptr_t mm_get_virtual_for_physical;
		uintptr_t mm_get_physical_memory_ranges;
		//uintptr_t rtl_random_ex;
		//uintptr_t ke_query_time_increment;
		//uintptr_t ke_stack_attach_process;
		//uintptr_t ke_unstack_detach_process;
	};

	m_imported imported;

	VentroAPI NTSTATUS rtl_get_version( PRTL_OSVERSIONINFOW lpVersionInformation )
	{
		return reinterpret_cast< NTSTATUS( * )(PRTL_OSVERSIONINFOW) > (imported.rtl_get_version)(lpVersionInformation);
	}

	VentroAPI PVOID mm_map_io_space_ex( PHYSICAL_ADDRESS PhysicalAddress, SIZE_T NumberOfBytes, ULONG Protect )
	{
		return reinterpret_cast< PVOID( * )(PHYSICAL_ADDRESS, SIZE_T, ULONG) >(imported.mm_map_io_space_ex)(PhysicalAddress, NumberOfBytes, Protect);
	}

	VentroAPI VOID mm_unmap_io_space( PVOID BaseAddress, SIZE_T NumberOfBytes )
	{
		return reinterpret_cast< VOID( * )(PVOID, SIZE_T) >(imported.mm_unmap_io_space)(BaseAddress, NumberOfBytes);
	}

	VentroAPI PPHYSICAL_MEMORY_RANGE mm_get_physical_memory_ranges( )
	{
		return reinterpret_cast< PPHYSICAL_MEMORY_RANGE( * )( ) >( imported.mm_get_physical_memory_ranges )( );
	}

	VentroAPI NTSTATUS zw_query_system_information( SYSTEM_INFORMATION_CLASS SystemInformationClass, PVOID SystemInformation, ULONG SystemInformationLength, PULONG ReturnLength )
	{
		return reinterpret_cast< NTSTATUS( * )(SYSTEM_INFORMATION_CLASS, PVOID, ULONG, PULONG) > (imported.zw_query_system_information)(SystemInformationClass, SystemInformation, SystemInformationLength, ReturnLength);
	}

	VentroAPI PVOID ex_allocate_pool( POOL_TYPE PoolType, SIZE_T NumberOfBytes )
	{
		return reinterpret_cast< PVOID( * )(POOL_TYPE, SIZE_T) >(imported.ex_allocate_pool)(PoolType, NumberOfBytes);
	}

	VentroAPI BOOLEAN ob_reference_object_safe( PVOID Object )
	{
		return reinterpret_cast< BOOLEAN(*)(PVOID) > (imported.ob_reference_object_safe )(Object);
	}

	VentroAPI void ex_free_pool_with_tag( PVOID P, ULONG TAG )
	{
		return reinterpret_cast< void(*)(PVOID, ULONG) > (imported.ex_free_pool_with_tag)(P, TAG);
	}

	//VentroAPI PKTHREAD ke_get_current_thread( )
	//{
	//	return reinterpret_cast< PKTHREAD( * )() >(imported.ke_get_current_thread)();
	//}

	VentroAPI VOID rtl_init_ansi_string( PANSI_STRING DestinationString, PCSZ SourceString )
	{
		return reinterpret_cast< VOID( * )(PANSI_STRING, PCSZ) > (imported.rtl_init_ansi_string)(DestinationString, SourceString);
	}

	VentroAPI NTSTATUS rtl_ansi_string_to_unicode_string( PUNICODE_STRING DestinationString, PCANSI_STRING SourceString, BOOLEAN AllocateDestinationString )
	{
		return reinterpret_cast< NTSTATUS( * )(PUNICODE_STRING, PCANSI_STRING, BOOLEAN) > (imported.rtl_ansi_string_to_unicode_string)(DestinationString, SourceString, AllocateDestinationString);
	}

	VentroAPI NTSTATUS mm_copy_virtual_memory( PEPROCESS SourceProcess, PVOID SourceAddress, PEPROCESS TargetProcess, PVOID TargetAddress, SIZE_T BufferSize, KPROCESSOR_MODE PreviousMode, PSIZE_T ReturnSize )
	{
		return reinterpret_cast< NTSTATUS( * )(PEPROCESS, PVOID, PEPROCESS, PVOID, SIZE_T, KPROCESSOR_MODE, PSIZE_T) > (imported.mm_copy_virtual_memory)(SourceProcess, SourceAddress, TargetProcess, TargetAddress, BufferSize, PreviousMode, ReturnSize);
	}

	VentroAPI PEPROCESS io_get_current_process( )
	{
		return reinterpret_cast< PEPROCESS( * )() > (imported.io_get_current_process)();
	}

	//VentroAPI NTSTATUS zw_allocate_virtual_memory( HANDLE ProcessHandle, PVOID* BaseAddress, ULONG_PTR ZeroBits, PSIZE_T RegionSize, ULONG AllocationType, ULONG Protect )
	//{
	//	return reinterpret_cast< NTSTATUS( * )(HANDLE, PVOID*, ULONG_PTR, PSIZE_T, ULONG, ULONG) > (imported.zw_allocate_virtual_memory)(ProcessHandle, BaseAddress, ZeroBits, RegionSize, AllocationType, Protect);
	//}

	VentroAPI NTSTATUS ps_lookup_process_by_process_id( HANDLE ProcessId, PEPROCESS* Process )
	{
		return reinterpret_cast< NTSTATUS( * )(HANDLE, PEPROCESS*) > (imported.ps_lookup_process_by_process_id)(ProcessId, Process);
	}

	VentroAPI PPEB ps_get_process_peb( PEPROCESS Process )
	{
		return reinterpret_cast< PPEB( * )(PEPROCESS) > (imported.ps_get_process_peb)(Process);
	}

	VentroAPI LONG rtl_compare_unicode_string( PCUNICODE_STRING String1, PCUNICODE_STRING String2, BOOLEAN CaseInSensitive )
	{
		return reinterpret_cast< LONG( * )(PCUNICODE_STRING, PCUNICODE_STRING, BOOLEAN) > (imported.rtl_compare_unicode_string)(String1, String2, CaseInSensitive);
	}

	VentroAPI VOID rtl_free_unicode_string( PUNICODE_STRING UnicodeString )
	{
		return reinterpret_cast< VOID( * )(PUNICODE_STRING) > (imported.rtl_free_unicode_string)(UnicodeString);
	}

	VentroAPI LONG_PTR obf_dereference_object( PVOID Object )
	{
		return reinterpret_cast< LONG_PTR( * )(PVOID) >(imported.obf_dereference_object)(Object);
	}

	VentroAPI NTSTATUS mm_copy_memory( PVOID TargetAddress, MM_COPY_ADDRESS SourceAddress, SIZE_T NumberOfBytes, ULONG Flags, PSIZE_T NumberOfBytesTransferred )
	{
		return reinterpret_cast< NTSTATUS( * )(PVOID, MM_COPY_ADDRESS, SIZE_T, ULONG, PSIZE_T) >(imported.mm_copy_memory)(TargetAddress, SourceAddress, NumberOfBytes, Flags, NumberOfBytesTransferred);
	}

	VentroAPI PVOID ps_get_process_section_base_address( PEPROCESS Process )
	{
		return reinterpret_cast< PVOID( * )(PEPROCESS) >(imported.ps_get_process_section_base_address)(Process);
	}

	//VentroAPI NTSTATUS zw_query_virtual_memory( HANDLE ProcessHandle, PVOID BaseAddress, MEMORY_INFORMATION_CLASS MemoryInformationClass, PVOID MemoryInformation, SIZE_T MemoryInformationLength, PSIZE_T ReturnLength )
	//{
	//	return reinterpret_cast< NTSTATUS( * )(HANDLE, PVOID, MEMORY_INFORMATION_CLASS, PVOID, SIZE_T, PSIZE_T) >(imported.zw_query_virtual_memory)(ProcessHandle, BaseAddress, MemoryInformationClass, MemoryInformation, MemoryInformationLength, ReturnLength);
	//}

	//VentroAPI NTSTATUS zw_free_virtual_memory( HANDLE ProcessHandle, PVOID* BaseAddress, PSIZE_T RegionSize, ULONG FreeType )
	//{
	//	return reinterpret_cast< NTSTATUS( * )(HANDLE, PVOID*, PSIZE_T, ULONG) >(imported.zw_free_virtual_memory)(ProcessHandle, BaseAddress, RegionSize, FreeType);
	//}

	VentroAPI NTSTATUS io_create_driver( PUNICODE_STRING Driver, PDRIVER_INITIALIZE INIT )
	{
		return reinterpret_cast< NTSTATUS( * )(PUNICODE_STRING, PDRIVER_INITIALIZE) >(imported.io_create_driver)(Driver, INIT);
	}

	VentroAPI PMDL io_allocate_mdl( PVOID VirtualAddress, ULONG Length, BOOLEAN SecondaryBuffer, BOOLEAN ChargeQuota, PIRP Irp )
	{
		return reinterpret_cast< PMDL( * )(PVOID, ULONG, BOOLEAN, BOOLEAN, PIRP) >(imported.io_allocate_mdl)(VirtualAddress, Length, SecondaryBuffer, ChargeQuota, Irp);
	}

	VentroAPI VOID mm_probe_and_lock_pages( PMDL MemoryDescriptorList, KPROCESSOR_MODE AccessMode, LOCK_OPERATION Operation )
	{
		return reinterpret_cast< VOID( * )(PMDL, KPROCESSOR_MODE, LOCK_OPERATION) >(imported.mm_probe_and_lock_pages)(MemoryDescriptorList, AccessMode, Operation);
	}

	VentroAPI PVOID mm_map_locked_pages_specify_cache( PMDL MemoryDescriptorList, KPROCESSOR_MODE AccessMode, MEMORY_CACHING_TYPE CacheType, PVOID RequestedAddress, ULONG BugCheckOnFailure, ULONG Priority )
	{
		return reinterpret_cast< PVOID( * )(PMDL, KPROCESSOR_MODE, MEMORY_CACHING_TYPE, PVOID, ULONG, ULONG) >(imported.mm_map_locked_pages_specify_cache)(MemoryDescriptorList, AccessMode, CacheType, RequestedAddress, BugCheckOnFailure, Priority);
	}

	VentroAPI NTSTATUS mm_protect_mdl_system_address( PMDL MemoryDescriptorList, ULONG NewProtect )
	{
		return reinterpret_cast< NTSTATUS( * )(PMDL, ULONG) >(imported.mm_protect_mdl_system_address)(MemoryDescriptorList, NewProtect);
	}

	VentroAPI VOID mm_unmap_locked_pages( PVOID BaseAddress, PMDL MemoryDescriptorList )
	{
		return reinterpret_cast< VOID( * )(PVOID, PMDL) >(imported.mm_unmap_locked_pages)(BaseAddress, MemoryDescriptorList);
	}

	VentroAPI VOID mm_unlock_pages( PMDL MemoryDescriptorList )
	{
		return reinterpret_cast< VOID( * )(PMDL) >(imported.mm_unlock_pages)(MemoryDescriptorList);
	}

	VentroAPI VOID io_free_mdl( PMDL Mdl )
	{
		return reinterpret_cast< VOID( * )(PMDL) >(imported.io_free_mdl)(Mdl);
	}
	
	VentroAPI VOID iof_complete_request( PIRP Irp, CCHAR PriorityBoost )
	{
		return reinterpret_cast< VOID( * )(PIRP, CCHAR) >(imported.iof_complete_request)(Irp, PriorityBoost);
	}

	VentroAPI VOID rtl_init_unicode_string( PUNICODE_STRING DestinationString, PCWSTR SourceString )
	{
		return reinterpret_cast< VOID( * )(PUNICODE_STRING, PCWSTR) >(imported.rtl_init_unicode_string)(DestinationString, SourceString);
	}	
	
	//VentroAPI NTSTATUS ex_raise_hard_error( NTSTATUS ErrorStatus, ULONG NumberOfParameters,
	//	ULONG UnicodeStringParameterMask, PULONG_PTR Parameters,
	//	ULONG ValidResponseOptions, PULONG Response )
	//{
	//	return reinterpret_cast< NTSTATUS( * )( NTSTATUS, ULONG, ULONG, PULONG_PTR, ULONG, PULONG ) >(imported.ex_raise_hard_error )( ErrorStatus, NumberOfParameters , UnicodeStringParameterMask, Parameters, ValidResponseOptions, Response);
	//}

	VentroAPI NTSTATUS io_create_symbolic_link( PUNICODE_STRING SymbolicLinkName, PUNICODE_STRING DeviceName )
	{
		return reinterpret_cast< NTSTATUS( * )(PUNICODE_STRING, PUNICODE_STRING) >(imported.io_create_symbolic_link)(SymbolicLinkName, DeviceName);
	}

	VentroAPI VOID io_delete_device( PDEVICE_OBJECT DeviceObject )
	{
		return reinterpret_cast< VOID( * )(PDEVICE_OBJECT) >(imported.io_delete_device)(DeviceObject);
	}

	VentroAPI NTSTATUS io_create_device( PDRIVER_OBJECT DriverObject, ULONG DeviceExtensionSize, PUNICODE_STRING DeviceName, DEVICE_TYPE DeviceType, ULONG DeviceCharacteristics, BOOLEAN Exclusive, PDEVICE_OBJECT* DeviceObject )
	{
		return reinterpret_cast< NTSTATUS( * )(PDRIVER_OBJECT, ULONG, PUNICODE_STRING, DEVICE_TYPE, ULONG, BOOLEAN, PDEVICE_OBJECT*) >(imported.io_create_device)(DriverObject, DeviceExtensionSize, DeviceName, DeviceType, DeviceCharacteristics, Exclusive, DeviceObject);
	}

	//VentroAPI ULONG ke_query_time_increment(  )
	//{
	//	return reinterpret_cast< ULONG( * )( ) >( imported.ke_query_time_increment )( );
	//}

	//VentroAPI ULONG rtl_random_ex( PULONG Seed )
	//{
	//	return reinterpret_cast< ULONG( * )( PULONG ) >( imported.rtl_random_ex )( Seed );
	//}
}