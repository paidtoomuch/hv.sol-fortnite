#pragma once

typedef unsigned __int64 ULONGLONG;

namespace physical {
    ULONGLONG m_stored_dtb;
    PEPROCESS save_process;
    uint64_t eac_module;
    uint64_t eac_cr3;
    
    auto read_physical(
        std::uintptr_t address,
        PVOID buffer,
        size_t size,
        size_t* bytes ) -> NTSTATUS
    {
        MM_COPY_ADDRESS target_address = { 0 };
        target_address.PhysicalAddress.QuadPart = address;
        return imports::mm_copy_memory( buffer, target_address, size, MM_COPY_MEMORY_PHYSICAL, bytes );
    }

    auto write_physical( std::uintptr_t address,
        PVOID buffer,
        size_t size,
        size_t* bytes ) -> NTSTATUS
    {
        if ( !address )
            return STATUS_UNSUCCESSFUL;

        PHYSICAL_ADDRESS AddrToWrite = { 0 };
        AddrToWrite.QuadPart = ( LONGLONG ) address;

        PVOID pmapped_mem = imports::mm_map_io_space_ex( AddrToWrite, size, PAGE_READWRITE );

        if ( !pmapped_mem )
            return STATUS_UNSUCCESSFUL;

        crt::kmemcpy( pmapped_mem, buffer, size );

        *bytes = size;
        imports::mm_unmap_io_space( pmapped_mem, size );
        return STATUS_SUCCESS;
    }

    auto translate_linear(
        std::uintptr_t directory_base,
        std::uintptr_t address ) -> std::uintptr_t {

        directory_base &= ~0xf;

        auto virt_addr = address & ~( ~0ul << 12 );
        auto pte = ( ( address >> 12 ) & ( 0x1ffll ) );
        auto pt = ( ( address >> 21 ) & ( 0x1ffll ) );
        auto pd = ( ( address >> 30 ) & ( 0x1ffll ) );
        auto pdp = ( ( address >> 39 ) & ( 0x1ffll ) );
        auto p_mask = ( ( ~0xfull << 8 ) & 0xfffffffffull );

        size_t readsize = 0;
        std::uintptr_t pdpe = 0;
        read_physical( directory_base + 8 * pdp, &pdpe, sizeof( pdpe ), &readsize );
        if ( ~pdpe & 1 ) {
            return 0;
        }

        std::uintptr_t pde = 0;
        read_physical( ( pdpe & p_mask ) + 8 * pd, &pde, sizeof( pde ), &readsize );
        if ( ~pde & 1 ) {
            return 0;
        }

        /* 1GB large page, use pde's 12-34 bits */
        if ( pde & 0x80 )
            return ( pde & ( ~0ull << 42 >> 12 ) ) + ( address & ~( ~0ull << 30 ) );

        std::uintptr_t pteAddr = 0;
        read_physical( ( pde & p_mask ) + 8 * pt, &pteAddr, sizeof( pteAddr ), &readsize );
        if ( ~pteAddr & 1 ) {
            return 0;
        }

        /* 2MB large page */
        if ( pteAddr & 0x80 ) {
            return ( pteAddr & p_mask ) + ( address & ~( ~0ull << 21 ) );
        }

        address = 0;
        read_physical( ( pteAddr & p_mask ) + 8 * pte, &address, sizeof( address ), &readsize );
        address &= p_mask;

        if ( !address ) {
            return 0;
        }

        return address + virt_addr;
    }

    auto find_min( INT32 g, SIZE_T f ) -> ULONG64
    {
        INT32 h = ( INT32 ) f;
        ULONG64 result = 0;

        result = ( ( ( g ) < ( h ) ) ? ( g ) : ( h ) );

        return result;
    }
}

