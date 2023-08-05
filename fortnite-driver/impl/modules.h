namespace crt
{
    //int get_random_seed( )
    //{
    //    ULONG seed = imports::ke_query_time_increment( );
    //    return imports::rtl_random_ex( &seed );
    //}

    INT kmemcmp( const void* s1, const void* s2, size_t n )
    {
        const unsigned char* p1 = ( const unsigned char* ) s1;
        const unsigned char* end1 = p1 + n;
        const unsigned char* p2 = ( const unsigned char* ) s2;
        int                   d = 0;
        for ( ;;) {
            if ( d || p1 >= end1 ) break;
            d = ( int ) *p1++ - ( int ) *p2++;
            if ( d || p1 >= end1 ) break;
            d = ( int ) *p1++ - ( int ) *p2++;
            if ( d || p1 >= end1 ) break;
            d = ( int ) *p1++ - ( int ) *p2++;
            if ( d || p1 >= end1 ) break;
            d = ( int ) *p1++ - ( int ) *p2++;
        }
        return d;
    }

    //INT kMemcmp( const void* str1, const void* str2, size_t count )
    //{
    //    register const unsigned char* s1 = ( const unsigned char* ) str1;
    //    register const unsigned char* s2 = ( const unsigned char* ) str2;
    //    while ( count-- > 0 )
    //    {
    //        if ( *s1++ != *s2++ )
    //            return s1 [ -1 ] < s2 [ -1 ] ? -1 : 1;
    //    }
    //    return 0;
    //}


    VOID* kmemcpy( void* dest, const void* src, size_t len )
    {
        char* d = ( char* ) dest;
        const char* s = ( const char* ) src;
        while ( len-- )
            *d++ = *s++;
        return dest;
    }

    VOID* kmemset( void* dest, UINT8 c, size_t count )
    {
        size_t blockIdx;
        size_t blocks = count >> 3;
        size_t bytesLeft = count - (blocks << 3);
        UINT64 cUll =
            c
            | ((( UINT64 ) c) << 8)
            | ((( UINT64 ) c) << 16)
            | ((( UINT64 ) c) << 24)
            | ((( UINT64 ) c) << 32)
            | ((( UINT64 ) c) << 40)
            | ((( UINT64 ) c) << 48)
            | ((( UINT64 ) c) << 56);

        UINT64* destPtr8 = ( UINT64* ) dest;
        for ( blockIdx = 0; blockIdx < blocks; blockIdx++ ) destPtr8 [ blockIdx ] = cUll;

        if ( !bytesLeft ) return dest;

        blocks = bytesLeft >> 2;
        bytesLeft = bytesLeft - (blocks << 2);

        UINT32* destPtr4 = ( UINT32* ) &destPtr8 [ blockIdx ];
        for ( blockIdx = 0; blockIdx < blocks; blockIdx++ ) destPtr4 [ blockIdx ] = ( UINT32 ) cUll;

        if ( !bytesLeft ) return dest;

        blocks = bytesLeft >> 1;
        bytesLeft = bytesLeft - (blocks << 1);

        UINT16* destPtr2 = ( UINT16* ) &destPtr4 [ blockIdx ];
        for ( blockIdx = 0; blockIdx < blocks; blockIdx++ ) destPtr2 [ blockIdx ] = ( UINT16 ) cUll;

        if ( !bytesLeft ) return dest;

        UINT8* destPtr1 = ( UINT8* ) &destPtr2 [ blockIdx ];
        for ( blockIdx = 0; blockIdx < bytesLeft; blockIdx++ ) destPtr1 [ blockIdx ] = ( UINT8 ) cUll;

        return dest;
    }
}

namespace modules
{
    void* get_system_information(SYSTEM_INFORMATION_CLASS information_class)
    {
        unsigned long size = 32;
        char buffer[32];

        imports::zw_query_system_information(information_class, buffer, size, &size);

        void* info = imports::ex_allocate_pool(NonPagedPool, size);

        if (!info)
            return nullptr;

        if (!NT_SUCCESS(imports::zw_query_system_information(information_class, info, size, &size)))
        {
            imports::ex_free_pool_with_tag(info, 0);
            return nullptr;
        }

        return info;
    }

    auto find_section( uintptr_t ModuleBase, char* SectionName ) -> uintptr_t
    {
        PIMAGE_NT_HEADERS NtHeaders = ( PIMAGE_NT_HEADERS ) ( ModuleBase + ( ( PIMAGE_DOS_HEADER ) ModuleBase )->e_lfanew );
        PIMAGE_SECTION_HEADER Sections = IMAGE_FIRST_SECTION( NtHeaders );

        for ( DWORD i = 0; i < NtHeaders->FileHeader.NumberOfSections; i++ )
        {
            PIMAGE_SECTION_HEADER Section = &Sections [ i ];
            if ( crt::kmemcmp( Section->Name, SectionName, 5 ) == 0 )
            {
                return ModuleBase + Section->VirtualAddress;
            }
        }

        return 0;
    }

    uintptr_t get_kernel_module(const char* name)
    {
        const auto to_lower = [](char* string) -> const char*
        {
            for (char* pointer = string; *pointer != '\0'; ++pointer)
            {
                *pointer = (char)(short)tolower(*pointer);
            }

            return string;
        };

        const PRTL_PROCESS_MODULES info = (PRTL_PROCESS_MODULES)get_system_information(SystemModuleInformation);

        if (!info)
            return NULL;

        for (size_t i = 0; i < info->NumberOfModules; ++i)
        {
            const auto& mod = info->Modules[i];

            printf( "%s\n", mod.FullPathName + mod.OffsetToFileName );

            if (crt::strcmp(to_lower_c((char*)mod.FullPathName + mod.OffsetToFileName), name) == 0)
            {
                const void* address = mod.ImageBase;
                imports::ex_free_pool_with_tag(info, 0);
                return (uintptr_t)address;
            }
        }

        imports::ex_free_pool_with_tag(info, 0);
        return NULL;
    }

    bool safe_copy( void* dst, void* src, size_t size )
    {
        SIZE_T bytes = 0;

        if (imports::mm_copy_virtual_memory( imports::io_get_current_process( ), src, imports::io_get_current_process( ), dst, size, KernelMode, &bytes ) == STATUS_SUCCESS && bytes == size)
        {
            return true;
        }

        return false;
    }
}

namespace ctx
{
    auto write_protected_address( void* address, void* buffer, SIZE_T size, bool Restore ) -> BOOLEAN
    {
        NTSTATUS Status = { STATUS_SUCCESS };

        auto Mdl = imports::io_allocate_mdl(address, size, FALSE, FALSE, NULL);

        imports::mm_probe_and_lock_pages(Mdl, KernelMode, IoReadAccess);

        auto Mapping = imports::mm_map_locked_pages_specify_cache(Mdl, KernelMode, MmNonCached, NULL, FALSE, NormalPagePriority);

        Status = imports::mm_protect_mdl_system_address(Mdl, PAGE_READWRITE);

        if ( Status != STATUS_SUCCESS )
        {
            printf( "change page protection.\n" );
            imports::mm_unmap_locked_pages(Mapping, Mdl);
            imports::mm_unlock_pages(Mdl);
            imports::io_free_mdl(Mdl);
        }

        crt::kmemcpy( Mapping, buffer, size );

        if ( Restore )
        {
            Status = imports::mm_protect_mdl_system_address(Mdl, PAGE_READONLY);

            if ( Status != STATUS_SUCCESS )
            {
                printf( "restore page.\n" );

                imports::mm_unmap_locked_pages( Mapping, Mdl );
                imports::mm_unlock_pages( Mdl );
                imports::io_free_mdl( Mdl );
            }
        }

        imports::mm_unmap_locked_pages( Mapping, Mdl );
        imports::mm_unlock_pages( Mdl );
        imports::io_free_mdl( Mdl );

        return Status;
    }
}