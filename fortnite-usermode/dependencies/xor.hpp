/*
 * Copyright 2018-2020 Justas Masiulis
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

 // documentation is available at https://github.com/JustasMasiulis/lazy_importer

#ifndef FORCEINLINE_HPP
#define FORCEINLINE_HPP

#define I(name) \
    ::li::detail::lazy_function<::li::detail::khash(#name), decltype(&name)>()

#define LI_FN_DEF(name) ::li::detail::lazy_function<::li::detail::khash(#name), name>()

#define LI_MODULE(name) ::li::detail::lazy_module<::li::detail::khash(name)>()

// NOTE only std::forward is used from this header.
// If there is a need to eliminate this dependency the function itself is very small.

#include <utility>
#include <cstddef>
#include <intrin.h>

#ifndef LAZY_IMPORTER_NO_FORCEINLINE
#if defined(_MSC_VER)
#define LAZY_IMPORTER_FORCEINLINE __forceinline
#elif defined(__GNUC__) && __GNUC__ > 3
#define LAZY_IMPORTER_FORCEINLINE inline __attribute__((__always_inline__))
#else
#define LAZY_IMPORTER_FORCEINLINE inline
#endif
#else
#define LAZY_IMPORTER_FORCEINLINE inline
#endif

#ifdef LAZY_IMPORTER_CASE_INSENSITIVE
#define LAZY_IMPORTER_TOLOWER(c) (c >= 'A' && c <= 'Z' ? (c | (1 << 5)) : c)
#else
#define LAZY_IMPORTER_TOLOWER(c) (c)
#endif

namespace li {
    namespace detail {

        template<class First, class Second>
        struct pair {
            First  first;
            Second second;
        };

        namespace win {

            struct LIST_ENTRY_T {
                const char* Flink;
                const char* Blink;
            };

            struct UNICODE_STRING_T {
                unsigned short Length;
                unsigned short MaximumLength;
                wchar_t* Buffer;
            };

            struct PEB_LDR_DATA_T {
                unsigned long Length;
                unsigned long Initialized;
                const char* SsHandle;
                LIST_ENTRY_T  InLoadOrderModuleList;
            };

            struct PEB_T {
                unsigned char   Reserved1 [ 2 ];
                unsigned char   BeingDebugged;
                unsigned char   Reserved2 [ 1 ];
                const char* Reserved3 [ 2 ];
                PEB_LDR_DATA_T* Ldr;
            };

            struct LDR_DATA_TABLE_ENTRY_T {
                LIST_ENTRY_T InLoadOrderLinks;
                LIST_ENTRY_T InMemoryOrderLinks;
                LIST_ENTRY_T InInitializationOrderLinks;
                const char* DllBase;
                const char* EntryPoint;
                union {
                    unsigned long SizeOfImage;
                    const char* _dummy;
                };
                UNICODE_STRING_T FullDllName;
                UNICODE_STRING_T BaseDllName;

                LAZY_IMPORTER_FORCEINLINE const LDR_DATA_TABLE_ENTRY_T*
                    load_order_next( ) const noexcept
                {
                    return reinterpret_cast< const LDR_DATA_TABLE_ENTRY_T* >(
                        InLoadOrderLinks.Flink);
                }
            };

            struct IMAGE_DOS_HEADER { // DOS .EXE header
                unsigned short e_magic; // Magic number
                unsigned short e_cblp; // Bytes on last page of file
                unsigned short e_cp; // Pages in file
                unsigned short e_crlc; // Relocations
                unsigned short e_cparhdr; // Size of header in paragraphs
                unsigned short e_minalloc; // Minimum extra paragraphs needed
                unsigned short e_maxalloc; // Maximum extra paragraphs needed
                unsigned short e_ss; // Initial (relative) SS value
                unsigned short e_sp; // Initial SP value
                unsigned short e_csum; // Checksum
                unsigned short e_ip; // Initial IP value
                unsigned short e_cs; // Initial (relative) CS value
                unsigned short e_lfarlc; // File address of relocation table
                unsigned short e_ovno; // Overlay number
                unsigned short e_res [ 4 ]; // Reserved words
                unsigned short e_oemid; // OEM identifier (for e_oeminfo)
                unsigned short e_oeminfo; // OEM information; e_oemid specific
                unsigned short e_res2 [ 10 ]; // Reserved words
                long           e_lfanew; // File address of new exe header
            };

            struct IMAGE_FILE_HEADER {
                unsigned short Machine;
                unsigned short NumberOfSections;
                unsigned long  TimeDateStamp;
                unsigned long  PointerToSymbolTable;
                unsigned long  NumberOfSymbols;
                unsigned short SizeOfOptionalHeader;
                unsigned short Characteristics;
            };

            struct IMAGE_EXPORT_DIRECTORY {
                unsigned long  Characteristics;
                unsigned long  TimeDateStamp;
                unsigned short MajorVersion;
                unsigned short MinorVersion;
                unsigned long  Name;
                unsigned long  Base;
                unsigned long  NumberOfFunctions;
                unsigned long  NumberOfNames;
                unsigned long  AddressOfFunctions; // RVA from base of image
                unsigned long  AddressOfNames; // RVA from base of image
                unsigned long  AddressOfNameOrdinals; // RVA from base of image
            };

            struct IMAGE_DATA_DIRECTORY {
                unsigned long VirtualAddress;
                unsigned long Size;
            };

            struct IMAGE_OPTIONAL_HEADER64 {
                unsigned short       Magic;
                unsigned char        MajorLinkerVersion;
                unsigned char        MinorLinkerVersion;
                unsigned long        SizeOfCode;
                unsigned long        SizeOfInitializedData;
                unsigned long        SizeOfUninitializedData;
                unsigned long        AddressOfEntryPoint;
                unsigned long        BaseOfCode;
                unsigned long long   ImageBase;
                unsigned long        SectionAlignment;
                unsigned long        FileAlignment;
                unsigned short       MajorOperatingSystemVersion;
                unsigned short       MinorOperatingSystemVersion;
                unsigned short       MajorImageVersion;
                unsigned short       MinorImageVersion;
                unsigned short       MajorSubsystemVersion;
                unsigned short       MinorSubsystemVersion;
                unsigned long        Win32VersionValue;
                unsigned long        SizeOfImage;
                unsigned long        SizeOfHeaders;
                unsigned long        CheckSum;
                unsigned short       Subsystem;
                unsigned short       DllCharacteristics;
                unsigned long long   SizeOfStackReserve;
                unsigned long long   SizeOfStackCommit;
                unsigned long long   SizeOfHeapReserve;
                unsigned long long   SizeOfHeapCommit;
                unsigned long        LoaderFlags;
                unsigned long        NumberOfRvaAndSizes;
                IMAGE_DATA_DIRECTORY DataDirectory [ 16 ];
            };

            struct IMAGE_OPTIONAL_HEADER32 {
                unsigned short       Magic;
                unsigned char        MajorLinkerVersion;
                unsigned char        MinorLinkerVersion;
                unsigned long        SizeOfCode;
                unsigned long        SizeOfInitializedData;
                unsigned long        SizeOfUninitializedData;
                unsigned long        AddressOfEntryPoint;
                unsigned long        BaseOfCode;
                unsigned long        BaseOfData;
                unsigned long        ImageBase;
                unsigned long        SectionAlignment;
                unsigned long        FileAlignment;
                unsigned short       MajorOperatingSystemVersion;
                unsigned short       MinorOperatingSystemVersion;
                unsigned short       MajorImageVersion;
                unsigned short       MinorImageVersion;
                unsigned short       MajorSubsystemVersion;
                unsigned short       MinorSubsystemVersion;
                unsigned long        Win32VersionValue;
                unsigned long        SizeOfImage;
                unsigned long        SizeOfHeaders;
                unsigned long        CheckSum;
                unsigned short       Subsystem;
                unsigned short       DllCharacteristics;
                unsigned long        SizeOfStackReserve;
                unsigned long        SizeOfStackCommit;
                unsigned long        SizeOfHeapReserve;
                unsigned long        SizeOfHeapCommit;
                unsigned long        LoaderFlags;
                unsigned long        NumberOfRvaAndSizes;
                IMAGE_DATA_DIRECTORY DataDirectory [ 16 ];
            };

            struct IMAGE_NT_HEADERS {
                unsigned long     Signature;
                IMAGE_FILE_HEADER FileHeader;
#ifdef _WIN64
                IMAGE_OPTIONAL_HEADER64 OptionalHeader;
#else
                IMAGE_OPTIONAL_HEADER32 OptionalHeader;
#endif
            };

        } // namespace win

        // hashing stuff
        struct hash_t {
            using value_type = unsigned long;
            constexpr static value_type         offset = 2166136261;
            constexpr static value_type         prime = 16777619;
            constexpr static unsigned long long prime64 = prime;

            LAZY_IMPORTER_FORCEINLINE constexpr static value_type single( value_type value,
                char c ) noexcept
            {
                return static_cast< hash_t::value_type >(
                    (value ^ LAZY_IMPORTER_TOLOWER( c )) *
                    static_cast< unsigned long long >(prime));
            }
        };

        template<class CharT = char>
        LAZY_IMPORTER_FORCEINLINE constexpr hash_t::value_type
            khash( const CharT* str, hash_t::value_type value = hash_t::offset ) noexcept
        {
            return (*str ? khash( str + 1, hash_t::single( value, *str ) ) : value);
        }

        template<class CharT = char>
        LAZY_IMPORTER_FORCEINLINE hash_t::value_type hash( const CharT* str ) noexcept
        {
            hash_t::value_type value = hash_t::offset;

            for ( ;;) {
                char c = *str++;
                if ( !c )
                    return value;
                value = hash_t::single( value, c );
            }
        }

        LAZY_IMPORTER_FORCEINLINE hash_t::value_type hash(
            const win::UNICODE_STRING_T& str ) noexcept
        {
            auto       first = str.Buffer;
            const auto last = first + (str.Length / sizeof( wchar_t ));
            auto       value = hash_t::offset;
            for ( ; first != last; ++first )
                value = hash_t::single( value, static_cast< char >(*first) );

            return value;
        }

        LAZY_IMPORTER_FORCEINLINE pair<hash_t::value_type, hash_t::value_type> hash_forwarded(
            const char* str ) noexcept
        {
            pair<hash_t::value_type, hash_t::value_type> module_and_function {
                hash_t::offset, hash_t::offset
            };

            for ( ; *str != '.'; ++str )
                module_and_function.first = hash_t::single( module_and_function.first, *str );

            ++str;

            for ( ; *str; ++str )
                module_and_function.second = hash_t::single( module_and_function.second, *str );

            return module_and_function;
        }


        // some helper functions
        LAZY_IMPORTER_FORCEINLINE const win::PEB_T* peb( ) noexcept
        {
#if defined(_M_X64) || defined(__amd64__)
            return reinterpret_cast< const win::PEB_T* >(__readgsqword( 0x60 ));
#elif defined(_M_IX86) || defined(__i386__)
            return reinterpret_cast< const win::PEB_T* >(__readfsdword( 0x30 ));
#elif defined(_M_ARM) || defined(__arm__)
            return *reinterpret_cast< const win::PEB_T** >(_MoveFromCoprocessor( 15, 0, 13, 0, 2 ) + 0x30);
#elif defined(_M_ARM64) || defined(__aarch64__)
            return *reinterpret_cast< const win::PEB_T** >(__getReg( 18 ) + 0x60);
#elif defined(_M_IA64) || defined(__ia64__)
            return *reinterpret_cast< const win::PEB_T** >(static_cast< char* >(_rdteb( )) + 0x60);
#else
#error Unsupported platform. Open an issue and I'll probably add support.
#endif
        }

        LAZY_IMPORTER_FORCEINLINE const win::PEB_LDR_DATA_T* ldr( )
        {
            return reinterpret_cast< const win::PEB_LDR_DATA_T* >(peb( )->Ldr);
        }

        LAZY_IMPORTER_FORCEINLINE const win::IMAGE_NT_HEADERS* nt_headers(
            const char* base ) noexcept
        {
            return reinterpret_cast< const win::IMAGE_NT_HEADERS* >(
                base + reinterpret_cast< const win::IMAGE_DOS_HEADER* >(base)->e_lfanew);
        }

        LAZY_IMPORTER_FORCEINLINE const win::IMAGE_EXPORT_DIRECTORY* image_export_dir(
            const char* base ) noexcept
        {
            return reinterpret_cast< const win::IMAGE_EXPORT_DIRECTORY* >(
                base + nt_headers( base )->OptionalHeader.DataDirectory->VirtualAddress);
        }

        LAZY_IMPORTER_FORCEINLINE const win::LDR_DATA_TABLE_ENTRY_T* ldr_data_entry( ) noexcept
        {
            return reinterpret_cast< const win::LDR_DATA_TABLE_ENTRY_T* >(
                ldr( )->InLoadOrderModuleList.Flink);
        }

        struct exports_directory {
            const char* _base;
            const win::IMAGE_EXPORT_DIRECTORY* _ied;
            unsigned long                      _ied_size;

        public:
            using size_type = unsigned long;

            LAZY_IMPORTER_FORCEINLINE
                exports_directory( const char* base ) noexcept : _base( base )
            {
                const auto ied_data_dir = nt_headers( base )->OptionalHeader.DataDirectory [ 0 ];
                _ied = reinterpret_cast< const win::IMAGE_EXPORT_DIRECTORY* >(
                    base + ied_data_dir.VirtualAddress);
                _ied_size = ied_data_dir.Size;
            }

            LAZY_IMPORTER_FORCEINLINE explicit operator bool( ) const noexcept
            {
                return reinterpret_cast< const char* >(_ied) != _base;
            }

            LAZY_IMPORTER_FORCEINLINE size_type size( ) const noexcept
            {
                return _ied->NumberOfNames;
            }

            LAZY_IMPORTER_FORCEINLINE const char* base( ) const noexcept { return _base; }
            LAZY_IMPORTER_FORCEINLINE const win::IMAGE_EXPORT_DIRECTORY* ied( ) const noexcept
            {
                return _ied;
            }

            LAZY_IMPORTER_FORCEINLINE const char* name( size_type index ) const noexcept
            {
                return reinterpret_cast< const char* >(
                    _base + reinterpret_cast< const unsigned long* >(
                        _base + _ied->AddressOfNames) [ index ]);
            }

            LAZY_IMPORTER_FORCEINLINE const char* address( size_type index ) const noexcept
            {
                const auto* const rva_table =
                    reinterpret_cast< const unsigned long* >(_base + _ied->AddressOfFunctions);

                const auto* const ord_table = reinterpret_cast< const unsigned short* >(
                    _base + _ied->AddressOfNameOrdinals);

                return _base + rva_table [ ord_table [ index ] ];
            }

            LAZY_IMPORTER_FORCEINLINE bool is_forwarded(
                const char* export_address ) const noexcept
            {
                const auto ui_ied = reinterpret_cast< const char* >(_ied);
                return (export_address > ui_ied && export_address < ui_ied + _ied_size);
            }
        };

        struct safe_module_enumerator {
            using value_type = const detail::win::LDR_DATA_TABLE_ENTRY_T;
            value_type* value;
            value_type* head;

            LAZY_IMPORTER_FORCEINLINE safe_module_enumerator( ) noexcept
                : safe_module_enumerator( ldr_data_entry( ) )
            {}

            LAZY_IMPORTER_FORCEINLINE
                safe_module_enumerator( const detail::win::LDR_DATA_TABLE_ENTRY_T* ldr ) noexcept
                : value( ldr->load_order_next( ) ), head( value )
            {}

            LAZY_IMPORTER_FORCEINLINE void reset( ) noexcept
            {
                value = head->load_order_next( );
            }

            LAZY_IMPORTER_FORCEINLINE bool next( ) noexcept
            {
                value = value->load_order_next( );

                return value != head && value->DllBase;
            }
        };

        struct unsafe_module_enumerator {
            using value_type = const detail::win::LDR_DATA_TABLE_ENTRY_T*;
            value_type value;

            LAZY_IMPORTER_FORCEINLINE unsafe_module_enumerator( ) noexcept
                : value( ldr_data_entry( ) )
            {}

            LAZY_IMPORTER_FORCEINLINE void reset( ) noexcept { value = ldr_data_entry( ); }

            LAZY_IMPORTER_FORCEINLINE bool next( ) noexcept
            {
                value = value->load_order_next( );
                return true;
            }
        };

        // provides the cached functions which use Derive classes methods
        template<class Derived, class DefaultType = void*>
        class lazy_base {
        protected:
            // This function is needed because every templated function
            // with different args has its own static buffer
            LAZY_IMPORTER_FORCEINLINE static void*& _cache( ) noexcept
            {
                static void* value = nullptr;
                return value;
            }

        public:
            template<class T = DefaultType>
            LAZY_IMPORTER_FORCEINLINE static T safe( ) noexcept
            {
                return Derived::template get<T, safe_module_enumerator>( );
            }

            template<class T = DefaultType, class Enum = unsafe_module_enumerator>
            LAZY_IMPORTER_FORCEINLINE static T cached( ) noexcept
            {
                auto& cached = _cache( );
                if ( !cached )
                    cached = Derived::template get<void*, Enum>( );

                return ( T ) (cached);
            }

            template<class T = DefaultType>
            LAZY_IMPORTER_FORCEINLINE static T safe_cached( ) noexcept
            {
                return cached<T, safe_module_enumerator>( );
            }
        };

        template<hash_t::value_type Hash>
        struct lazy_module : lazy_base<lazy_module<Hash>> {
            template<class T = void*, class Enum = unsafe_module_enumerator>
            LAZY_IMPORTER_FORCEINLINE static T get( ) noexcept
            {
                Enum e;
                do {
                    if ( hash( e.value->BaseDllName ) == Hash )
                        return ( T ) (e.value->DllBase);
                } while ( e.next( ) );
                return {};
            }

            template<class T = void*, class Ldr>
            LAZY_IMPORTER_FORCEINLINE static T in( Ldr ldr ) noexcept
            {
                safe_module_enumerator e( ( const detail::win::LDR_DATA_TABLE_ENTRY_T* ) (ldr) );
                do {
                    if ( hash( e.value->BaseDllName ) == Hash )
                        return ( T ) (e.value->DllBase);
                } while ( e.next( ) );
                return {};
            }

            template<class T = void*, class Ldr>
            LAZY_IMPORTER_FORCEINLINE static T in_cached( Ldr ldr ) noexcept
            {
                auto& cached = lazy_base<lazy_module<Hash>>::_cache( );
                if ( !cached )
                    cached = in( ldr );

                return ( T ) (cached);
            }
        };

        template<hash_t::value_type Hash, class T>
        struct lazy_function : lazy_base<lazy_function<Hash, T>, T> {
            using base_type = lazy_base<lazy_function<Hash, T>, T>;

            template<class... Args>
            LAZY_IMPORTER_FORCEINLINE decltype(auto) operator()( Args&&... args ) const
            {
#ifndef LAZY_IMPORTER_CACHE_OPERATOR_PARENS
                return get( )(std::forward<Args>( args )...);
#else
                return this->cached( )(std::forward<Args>( args )...);
#endif
            }

            template<class F = T, class Enum = unsafe_module_enumerator>
            LAZY_IMPORTER_FORCEINLINE static F get( ) noexcept
            {
                // for backwards compatability.
                // Before 2.0 it was only possible to resolve forwarded exports when
                // this macro was enabled
#ifdef LAZY_IMPORTER_RESOLVE_FORWARDED_EXPORTS
                return forwarded<F, Enum>( );
#else

                Enum e;

                do {
#ifdef LAZY_IMPORTER_HARDENED_MODULE_CHECKS
                    if ( !e.value->DllBase || !e.value->FullDllName.Length )
                        continue;
#endif

                    const exports_directory exports( e.value->DllBase );

                    if ( exports ) {
                        auto export_index = exports.size( );
                        while ( export_index-- )
                            if ( hash( exports.name( export_index ) ) == Hash )
                                return ( F ) (exports.address( export_index ));
                    }
                } while ( e.next( ) );
                return {};
#endif
            }

            template<class F = T, class Enum = unsafe_module_enumerator>
            LAZY_IMPORTER_FORCEINLINE static F forwarded( ) noexcept
            {
                detail::win::UNICODE_STRING_T name;
                hash_t::value_type            module_hash = 0;
                auto                          function_hash = Hash;

                Enum e;
                do {
                    name = e.value->BaseDllName;
                    name.Length -= 8; // get rid of .dll extension

                    if ( !module_hash || hash( name ) == module_hash ) {
                        const exports_directory exports( e.value->DllBase );

                        if ( exports ) {
                            auto export_index = exports.size( );
                            while ( export_index-- )
                                if ( hash( exports.name( export_index ) ) == function_hash ) {
                                    const auto addr = exports.address( export_index );

                                    if ( exports.is_forwarded( addr ) ) {
                                        auto hashes = hash_forwarded(
                                            reinterpret_cast< const char* >(addr) );

                                        function_hash = hashes.second;
                                        module_hash = hashes.first;

                                        e.reset( );
                                        break;
                                    }
                                    return ( F ) (addr);
                                }
                        }
                    }
                } while ( e.next( ) );
                return {};
            }

            template<class F = T>
            LAZY_IMPORTER_FORCEINLINE static F forwarded_safe( ) noexcept
            {
                return forwarded<F, safe_module_enumerator>( );
            }

            template<class F = T, class Enum = unsafe_module_enumerator>
            LAZY_IMPORTER_FORCEINLINE static F forwarded_cached( ) noexcept
            {
                auto& value = base_type::_cache( );
                if ( !value )
                    value = forwarded<void*, Enum>( );
                return ( F ) (value);
            }

            template<class F = T>
            LAZY_IMPORTER_FORCEINLINE static F forwarded_safe_cached( ) noexcept
            {
                return forwarded_cached<F, safe_module_enumerator>( );
            }

            template<class F = T, bool IsSafe = false, class Module>
            LAZY_IMPORTER_FORCEINLINE static F in( Module m ) noexcept
            {
                if ( IsSafe && !m )
                    return {};

                const exports_directory exports( ( const char* ) (m) );
                if ( IsSafe && !exports )
                    return {};

                for ( unsigned long i {};; ++i ) {
                    if ( IsSafe && i == exports.size( ) )
                        break;

                    if ( hash( exports.name( i ) ) == Hash )
                        return ( F ) (exports.address( i ));
                }
                return {};
            }

            template<class F = T, class Module>
            LAZY_IMPORTER_FORCEINLINE static F in_safe( Module m ) noexcept
            {
                return in<F, true>( m );
            }

            template<class F = T, bool IsSafe = false, class Module>
            LAZY_IMPORTER_FORCEINLINE static F in_cached( Module m ) noexcept
            {
                auto& value = base_type::_cache( );
                if ( !value )
                    value = in<void*, IsSafe>( m );
                return ( F ) (value);
            }

            template<class F = T, class Module>
            LAZY_IMPORTER_FORCEINLINE static F in_safe_cached( Module m ) noexcept
            {
                return in_cached<F, true>( m );
            }

            template<class F = T>
            LAZY_IMPORTER_FORCEINLINE static F nt( ) noexcept
            {
                return in<F>( ldr_data_entry( )->load_order_next( )->DllBase );
            }

            template<class F = T>
            LAZY_IMPORTER_FORCEINLINE static F nt_safe( ) noexcept
            {
                return in_safe<F>( ldr_data_entry( )->load_order_next( )->DllBase );
            }

            template<class F = T>
            LAZY_IMPORTER_FORCEINLINE static F nt_cached( ) noexcept
            {
                return in_cached<F>( ldr_data_entry( )->load_order_next( )->DllBase );
            }

            template<class F = T>
            LAZY_IMPORTER_FORCEINLINE static F nt_safe_cached( ) noexcept
            {
                return in_safe_cached<F>( ldr_data_entry( )->load_order_next( )->DllBase );
            }
        };

    }
} // namespace li::detail

#endif // include guard

/*
 * Copyright 2017 - 2020 Justas Masiulis
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#ifndef JM_XORSTR_HPP
#define JM_XORSTR_HPP
#define JM_XORSTR_DISABLE_AVX_INTRINSICS

#include <immintrin.h>
#include <cstdint>
#include <cstddef>
#include <utility>

#define xorstr(str)                                              \
    ::jm::make_xorstr(                                           \
        []() { return str; },                                    \
        std::make_index_sequence<sizeof(str) / sizeof(*str)>{},  \
        std::make_index_sequence<::jm::detail::_buffer_size<sizeof(str)>()>{})
#define xorstr_(str) xorstr(str).crypt_get()
#define _(str) xorstr(str).crypt_get()

#ifdef _MSC_VER
#define XORSTR_FORCEINLINE __forceinline
#else
#define XORSTR_FORCEINLINE __attribute__((always_inline))
#endif

 // you can define this macro to get possibly faster code on gcc/clang
 // at the expense of constants being put into data section.
#if !defined(XORSTR_ALLOW_DATA)
// MSVC - no volatile
// GCC and clang - volatile everywhere
#if defined(__clang__) || defined(__GNUC__)
#define XORSTR_VOLATILE volatile
#endif

#endif
#ifndef XORSTR_VOLATILE
#define XORSTR_VOLATILE
#endif

namespace jm
{

    namespace detail
    {

        template<std::size_t S>
        struct unsigned_;

        template<>
        struct unsigned_<1>
        {
            using type = std::uint8_t;
        };
        template<>
        struct unsigned_<2>
        {
            using type = std::uint16_t;
        };
        template<>
        struct unsigned_<4>
        {
            using type = std::uint32_t;
        };

        template<auto C, auto...>
        struct pack_value_type
        {
            using type = decltype(C);
        };

        template<std::size_t Size>
        constexpr std::size_t _buffer_size( )
        {
            return ((Size / 16) + (Size % 16 != 0)) * 2;
        }

        template<auto... Cs>
        struct tstring_
        {
            using value_type = typename pack_value_type<Cs...>::type;
            constexpr static std::size_t size = sizeof...(Cs);
            constexpr static value_type  str [ size ] = { Cs... };

            constexpr static std::size_t buffer_size = _buffer_size<sizeof( str )>( );
            constexpr static std::size_t buffer_align =
#ifndef JM_XORSTR_DISABLE_AVX_INTRINSICS
            ( (sizeof( str ) > 16) ? 32 : 16 );
#else
                16;
#endif
        };

        template<std::size_t I, std::uint64_t K>
        struct _ki
        {
            constexpr static std::size_t   idx = I;
            constexpr static std::uint64_t key = K;
        };

        template<std::uint32_t Seed>
        constexpr std::uint32_t key4( ) noexcept
        {
            std::uint32_t value = Seed;
            for ( char c : __TIMESTAMP__ )
                value = static_cast< std::uint32_t >((value ^ c) * 78920519ull);
            return value;
        }

        template<std::size_t S>
        constexpr std::uint64_t key8( )
        {
            constexpr auto first_part = key4<4784258102 + S>( );
            constexpr auto second_part = key4<first_part>( );
            return (static_cast< std::uint64_t >(first_part) << 32) | second_part;
        }

        // clang and gcc try really hard to place the constants in data
        // sections. to counter that there was a need to create an intermediate
        // constexpr string and then copy it into a non constexpr container with
        // volatile storage so that the constants would be placed directly into
        // code.
        template<class T, std::uint64_t... Keys>
        struct string_storage
        {
            std::uint64_t storage [ T::buffer_size ];

            XORSTR_FORCEINLINE constexpr string_storage( ) noexcept : storage { Keys... }
            {
                using cast_type =
                    typename unsigned_<sizeof( typename T::value_type )>::type;
                constexpr auto value_size = sizeof( typename T::value_type );
                // puts the string into 64 bit integer blocks in a constexpr
                // fashion
                for ( std::size_t i = 0; i < T::size; ++i )
                    storage [ i / (8 / value_size) ] ^=
                    (std::uint64_t { static_cast< cast_type >(T::str [ i ]) }
                << ((i % (8 / value_size)) * 8 * value_size));
            }
        };

    } // namespace detail

    template<class T, class... Keys>
    class xor_string
    {
        alignas(T::buffer_align) std::uint64_t _storage [ T::buffer_size ];

        // _single functions needed because MSVC crashes without them
        XORSTR_FORCEINLINE void _crypt_256_single( const std::uint64_t* keys,
            std::uint64_t* storage ) noexcept

        {
            _mm256_store_si256(
                reinterpret_cast< __m256i* >(storage),
                _mm256_xor_si256(
                    _mm256_load_si256( reinterpret_cast< const __m256i* >(storage) ),
                    _mm256_load_si256( reinterpret_cast< const __m256i* >(keys) ) ) );
        }

        template<std::size_t... Idxs>
        XORSTR_FORCEINLINE void _crypt_256( const std::uint64_t* keys,
            std::index_sequence<Idxs...> ) noexcept
        {
            (_crypt_256_single( keys + Idxs * 4, _storage + Idxs * 4 ), ...);
        }

        XORSTR_FORCEINLINE void _crypt_128_single( const std::uint64_t* keys,
            std::uint64_t* storage ) noexcept
        {
            _mm_store_si128(
                reinterpret_cast< __m128i* >(storage),
                _mm_xor_si128( _mm_load_si128( reinterpret_cast< const __m128i* >(storage) ),
                    _mm_load_si128( reinterpret_cast< const __m128i* >(keys) ) ) );
        }

        template<std::size_t... Idxs>
        XORSTR_FORCEINLINE void _crypt_128( const std::uint64_t* keys,
            std::index_sequence<Idxs...> ) noexcept
        {
            (_crypt_128_single( keys + Idxs * 2, _storage + Idxs * 2 ), ...);
        }

        // loop generates vectorized code which places constants in data dir
        XORSTR_FORCEINLINE constexpr void _copy( ) noexcept
        {
            constexpr detail::string_storage<T, Keys::key...> storage;
            static_cast< void >(std::initializer_list<std::uint64_t>{
                (const_cast< XORSTR_VOLATILE std::uint64_t* >(_storage)) [ Keys::idx ] =
                    storage.storage [ Keys::idx ]... });
        }

    public:
        using value_type = typename T::value_type;
        using size_type = std::size_t;
        using pointer = value_type*;
        using const_pointer = const pointer;

        XORSTR_FORCEINLINE xor_string( ) noexcept {
            _copy( );
        }

        XORSTR_FORCEINLINE constexpr size_type size( ) const noexcept
        {
            return T::size - 1;
        }

        XORSTR_FORCEINLINE void crypt( ) noexcept
        {
            alignas(T::buffer_align) std::uint64_t keys [ T::buffer_size ];
            static_cast< void >(std::initializer_list<std::uint64_t>{
                (const_cast< XORSTR_VOLATILE std::uint64_t* >(keys)) [ Keys::idx ] =
                    Keys::key... });

            _copy( );

#ifndef JM_XORSTR_DISABLE_AVX_INTRINSICS
            _crypt_256( keys, std::make_index_sequence<T::buffer_size / 4>{} );
            if constexpr ( T::buffer_size % 4 != 0 )
                _crypt_128( keys, std::index_sequence<T::buffer_size / 2 - 1>{} );
#else
            _crypt_128( keys, std::make_index_sequence<T::buffer_size / 2>{} );
#endif
        }

        XORSTR_FORCEINLINE const_pointer get( ) const noexcept
        {
            return reinterpret_cast< const_pointer >(_storage);
        }

        XORSTR_FORCEINLINE const_pointer crypt_get( ) noexcept
        {
            crypt( );
            return reinterpret_cast< const_pointer >(_storage);
        }
    };

    template<class Tstr, std::size_t... StringIndices, std::size_t... KeyIndices>
    XORSTR_FORCEINLINE constexpr auto
        make_xorstr( Tstr str_lambda,
            std::index_sequence<StringIndices...>,
            std::index_sequence<KeyIndices...> ) noexcept
    {
        return xor_string<detail::tstring_<str_lambda( ) [ StringIndices ]...>,
            detail::_ki<KeyIndices, detail::key8<KeyIndices>( )>...>{};
    }

} // namespace jm

#endif // include guard


#ifndef DEFS_H
#define DEFS_H

class VMatrix
{
public:

    VMatrix(
        double m00, double m01, double m02, double m03,
        double m10, double m11, double m12, double m13,
        double m20, double m21, double m22, double m23,
        double m30, double m31, double m32, double m33
    ) {
        m [ 0 ][ 0 ] = m00;
        m [ 0 ][ 1 ] = m01;
        m [ 0 ][ 2 ] = m02;
        m [ 0 ][ 3 ] = m03;

        m [ 1 ][ 0 ] = m10;
        m [ 1 ][ 1 ] = m11;
        m [ 1 ][ 2 ] = m12;
        m [ 1 ][ 3 ] = m13;

        m [ 2 ][ 0 ] = m20;
        m [ 2 ][ 1 ] = m21;
        m [ 2 ][ 2 ] = m22;
        m [ 2 ][ 3 ] = m23;

        m [ 3 ][ 0 ] = m30;
        m [ 3 ][ 1 ] = m31;
        m [ 3 ][ 2 ] = m32;
        m [ 3 ][ 3 ] = m33;
    }



    // array access
    inline double* operator[]( int i )
    {
        return m [ i ];
    }

    inline const double* operator[]( int i ) const
    {
        return m [ i ];
    }

    // Get a pointer to m[0][0]
    inline double* Base( )
    {
        return &m [ 0 ][ 0 ];
    }

    inline const double* Base( ) const
    {
        return &m [ 0 ][ 0 ];
    }


public:
    // The matrix.
    double		m [ 4 ][ 4 ];
};


#if defined(__GNUC__)
typedef          long long ll;
typedef unsigned long long ull;
#define __int64 long long
#define __int32 int
#define __int16 short
#define __int8  char
#define MAKELL(num) num ## LL
#define FMT_64 "ll"
#elif defined(_MSC_VER)
typedef          __int64 ll;
typedef unsigned __int64 ull;
#define MAKELL(num) num ## i64
#define FMT_64 "I64"
#elif defined (__BORLANDC__)
typedef          __int64 ll;
typedef unsigned __int64 ull;
#define MAKELL(num) num ## i64
#define FMT_64 "L"
#else
#error "unknown compiler"
#endif

typedef unsigned int uint;
typedef unsigned char uchar;
typedef unsigned short ushort;
typedef unsigned long ulong;

typedef          char   int8;
typedef   signed char   sint8;
typedef unsigned char   uint8;
typedef          short  int16;
typedef   signed short  sint16;
typedef unsigned short  uint16;
typedef          int    int32;
typedef   signed int    sint32;
typedef unsigned int    uint32;
typedef ll              int64;
typedef ll              sint64;
typedef ull             uint64;

// Partially defined types. They are used when the decompiler does not know
// anything about the type except its size.
#define _BYTE  uint8
#define _WORD  uint16
#define _DWORD uint32
#define _QWORD uint64
#if !defined(_MSC_VER)
#define _LONGLONG __int128
#endif

// Non-standard boolean types. They are used when the decompiler can not use
// the standard "bool" type because of the size mistmatch but the possible
// values are only 0 and 1. See also 'BOOL' type below.
typedef int8 _BOOL1;
typedef int16 _BOOL2;
typedef int32 _BOOL4;

//#ifndef _WINDOWS_
//typedef int8 BYTE;
//typedef int16 WORD;
//typedef int32 DWORD;
//typedef int32 LONG;
//typedef int BOOL;       // uppercase BOOL is usually 4 bytes
//#endif
//typedef int64 QWORD;
#ifndef __cplusplus
typedef int bool;       // we want to use bool in our C programs
#endif

#define __pure          // pure function: always returns the same value, has no
// side effects

// Non-returning function
#if defined(__GNUC__)
#define __noreturn  __attribute__((noreturn))
#else
#define __noreturn  __declspec(noreturn)
#endif


#ifndef NULL
#define NULL 0
#endif

// Some convenience macros to make partial accesses nicer
#define LAST_IND(x,part_type)    (sizeof(x)/sizeof(part_type) - 1)
#if defined(__BYTE_ORDER) && __BYTE_ORDER == __BIG_ENDIAN
#  define LOW_IND(x,part_type)   LAST_IND(x,part_type)
#  define HIGH_IND(x,part_type)  0
#else
#  define HIGH_IND(x,part_type)  LAST_IND(x,part_type)
#  define LOW_IND(x,part_type)   0
#endif
// first unsigned macros:
//#define LODWORD(x)  (*((DWORD*)&(x)))  // low dword
//#define HIDWORD(x)  (*((DWORD*)&(x)+1))

#define BYTEn(x, n)   (*((_BYTE*)&(x)+n))
#define WORDn(x, n)   (*((_WORD*)&(x)+n))
#define DWORDn(x, n)  (*((_DWORD*)&(x)+n))

#define LOBYTE(x)  BYTEn(x,LOW_IND(x,_BYTE))
#define LOWORD(x)  WORDn(x,LOW_IND(x,_WORD))
#define LODWORD(x) DWORDn(x,LOW_IND(x,_DWORD))
#define HIBYTE(x)  BYTEn(x,HIGH_IND(x,_BYTE))
#define HIWORD(x)  WORDn(x,HIGH_IND(x,_WORD))
#define HIDWORD(x) DWORDn(x,HIGH_IND(x,_DWORD))
#define BYTE1(x)   BYTEn(x,  1)         // byte 1 (counting from 0)
#define BYTE2(x)   BYTEn(x,  2)
#define BYTE3(x)   BYTEn(x,  3)
#define BYTE4(x)   BYTEn(x,  4)
#define BYTE5(x)   BYTEn(x,  5)
#define BYTE6(x)   BYTEn(x,  6)
#define BYTE7(x)   BYTEn(x,  7)
#define BYTE8(x)   BYTEn(x,  8)
#define BYTE9(x)   BYTEn(x,  9)
#define BYTE10(x)  BYTEn(x, 10)
#define BYTE11(x)  BYTEn(x, 11)
#define BYTE12(x)  BYTEn(x, 12)
#define BYTE13(x)  BYTEn(x, 13)
#define BYTE14(x)  BYTEn(x, 14)
#define BYTE15(x)  BYTEn(x, 15)
#define WORD1(x)   WORDn(x,  1)
#define WORD2(x)   WORDn(x,  2)         // third word of the object, unsigned
#define WORD3(x)   WORDn(x,  3)
#define WORD4(x)   WORDn(x,  4)
#define WORD5(x)   WORDn(x,  5)
#define WORD6(x)   WORDn(x,  6)
#define WORD7(x)   WORDn(x,  7)

// now signed macros (the same but with sign extension)
#define SBYTEn(x, n)   (*((int8*)&(x)+n))
#define SWORDn(x, n)   (*((int16*)&(x)+n))
#define SDWORDn(x, n)  (*((int32*)&(x)+n))

#define SLOBYTE(x)  SBYTEn(x,LOW_IND(x,int8))
#define SLOWORD(x)  SWORDn(x,LOW_IND(x,int16))
#define SLODWORD(x) SDWORDn(x,LOW_IND(x,int32))
#define SHIBYTE(x)  SBYTEn(x,HIGH_IND(x,int8))
#define SHIWORD(x)  SWORDn(x,HIGH_IND(x,int16))
#define SHIDWORD(x) SDWORDn(x,HIGH_IND(x,int32))
#define SBYTE1(x)   SBYTEn(x,  1)
#define SBYTE2(x)   SBYTEn(x,  2)
#define SBYTE3(x)   SBYTEn(x,  3)
#define SBYTE4(x)   SBYTEn(x,  4)
#define SBYTE5(x)   SBYTEn(x,  5)
#define SBYTE6(x)   SBYTEn(x,  6)
#define SBYTE7(x)   SBYTEn(x,  7)
#define SBYTE8(x)   SBYTEn(x,  8)
#define SBYTE9(x)   SBYTEn(x,  9)
#define SBYTE10(x)  SBYTEn(x, 10)
#define SBYTE11(x)  SBYTEn(x, 11)
#define SBYTE12(x)  SBYTEn(x, 12)
#define SBYTE13(x)  SBYTEn(x, 13)
#define SBYTE14(x)  SBYTEn(x, 14)
#define SBYTE15(x)  SBYTEn(x, 15)
#define SWORD1(x)   SWORDn(x,  1)
#define SWORD2(x)   SWORDn(x,  2)
#define SWORD3(x)   SWORDn(x,  3)
#define SWORD4(x)   SWORDn(x,  4)
#define SWORD5(x)   SWORDn(x,  5)
#define SWORD6(x)   SWORDn(x,  6)
#define SWORD7(x)   SWORDn(x,  7)


// Helper functions to represent some assembly instructions.

#ifdef __cplusplus

// compile time assertion
#define __CASSERT_N0__(l) COMPILE_TIME_ASSERT_ ## l
#define __CASSERT_N1__(l) __CASSERT_N0__(l)
#define CASSERT(cnd) typedef char __CASSERT_N1__(__LINE__) [(cnd) ? 1 : -1]

// check that unsigned multiplication does not overflow
template<class T> bool is_mul_ok( T count, T elsize )
{
    CASSERT( ( T ) (-1) > 0 ); // make sure T is unsigned
    if ( elsize == 0 || count == 0 )
        return true;
    return count <= (( T ) (-1)) / elsize;
}

// multiplication that saturates (yields the biggest value) instead of overflowing
// such a construct is useful in "operator new[]"
template<class T> bool saturated_mul( T count, T elsize )
{
    return is_mul_ok( count, elsize ) ? count * elsize : T( -1 );
}

#include <stddef.h> // for size_t
#include <cstdint>

// memcpy() with determined behavoir: it always copies
// from the start to the end of the buffer
// note: it copies byte by byte, so it is not equivalent to, for example, rep movsd
inline void* qmemcpy( void* dst, const void* src, size_t cnt )
{
    char* out = ( char* ) dst;
    const char* in = ( const char* ) src;
    while ( cnt > 0 )
    {
        *out++ = *in++;
        --cnt;
    }
    return dst;
}

// Generate a reference to pair of operands
template<class T>  int16 __PAIR__( int8  high, T low ) { return ((( int16 ) high) << sizeof( high ) * 8) | uint8( low ); }
template<class T>  int32 __PAIR__( int16 high, T low ) { return ((( int32 ) high) << sizeof( high ) * 8) | uint16( low ); }
template<class T>  int64 __PAIR__( int32 high, T low ) { return ((( int64 ) high) << sizeof( high ) * 8) | uint32( low ); }
template<class T> uint16 __PAIR__( uint8  high, T low ) { return ((( uint16 ) high) << sizeof( high ) * 8) | uint8( low ); }
template<class T> uint32 __PAIR__( uint16 high, T low ) { return ((( uint32 ) high) << sizeof( high ) * 8) | uint16( low ); }
template<class T> uint64 __PAIR__( uint32 high, T low ) { return ((( uint64 ) high) << sizeof( high ) * 8) | uint32( low ); }

// rotate left
template<class T> T __ROL__( T value, int count )
{
    const uint nbits = sizeof( T ) * 8;

    if ( count > 0 )
    {
        count %= nbits;
        T high = value >> (nbits - count);
        if ( T( -1 ) < 0 )
            high &= ~((T( -1 ) << count));
        value <<= count;
        value |= high;
    }
    else
    {
        count = -count % nbits;
        T low = value << (nbits - count);
        value >>= count;
        value |= low;
    }
    return value;
}

inline uint8  __ROL1__( uint8  value, int count ) { return __ROL__( ( uint8 ) value, count ); }
inline uint16 __ROL2__( uint16 value, int count ) { return __ROL__( ( uint16 ) value, count ); }
inline uint32 __ROL4__( uint32 value, int count ) { return __ROL__( ( uint32 ) value, count ); }
inline uint64 __ROL8__( uint64 value, int count ) { return __ROL__( ( uint64 ) value, count ); }
inline uint8  __ROR1__( uint8  value, int count ) { return __ROL__( ( uint8 ) value, -count ); }
inline uint16 __ROR2__( uint16 value, int count ) { return __ROL__( ( uint16 ) value, -count ); }
inline uint32 __ROR4__( uint32 value, int count ) { return __ROL__( ( uint32 ) value, -count ); }
inline uint64 __ROR8__( uint64 value, int count ) { return __ROL__( ( uint64 ) value, -count ); }

// carry flag of left shift
template<class T> int8 __MKCSHL__( T value, uint count )
{
    const uint nbits = sizeof( T ) * 8;
    count %= nbits;

    return (value >> (nbits - count)) & 1;
}

// carry flag of right shift
template<class T> int8 __MKCSHR__( T value, uint count )
{
    return (value >> (count - 1)) & 1;
}

// sign flag
template<class T> int8 __SETS__( T x )
{
    if ( sizeof( T ) == 1 )
        return int8( x ) < 0;
    if ( sizeof( T ) == 2 )
        return int16( x ) < 0;
    if ( sizeof( T ) == 4 )
        return int32( x ) < 0;
    return int64( x ) < 0;
}

// overflow flag of subtraction (x-y)
template<class T, class U> int8 __OFSUB__( T x, U y )
{
    if ( sizeof( T ) < sizeof( U ) )
    {
        U x2 = x;
        int8 sx = __SETS__( x2 );
        return (sx ^ __SETS__( y )) & (sx ^ __SETS__( x2 - y ));
    }
    else
    {
        T y2 = y;
        int8 sx = __SETS__( x );
        return (sx ^ __SETS__( y2 )) & (sx ^ __SETS__( x - y2 ));
    }
}

// overflow flag of addition (x+y)
template<class T, class U> int8 __OFADD__( T x, U y )
{
    if ( sizeof( T ) < sizeof( U ) )
    {
        U x2 = x;
        int8 sx = __SETS__( x2 );
        return ((1 ^ sx) ^ __SETS__( y )) & (sx ^ __SETS__( x2 + y ));
    }
    else
    {
        T y2 = y;
        int8 sx = __SETS__( x );
        return ((1 ^ sx) ^ __SETS__( y2 )) & (sx ^ __SETS__( x + y2 ));
    }
}

// carry flag of subtraction (x-y)
template<class T, class U> int8 __CFSUB__( T x, U y )
{
    int size = sizeof( T ) > sizeof( U ) ? sizeof( T ) : sizeof( U );
    if ( size == 1 )
        return uint8( x ) < uint8( y );
    if ( size == 2 )
        return uint16( x ) < uint16( y );
    if ( size == 4 )
        return uint32( x ) < uint32( y );
    return uint64( x ) < uint64( y );
}

// carry flag of addition (x+y)
template<class T, class U> int8 __CFADD__( T x, U y )
{
    int size = sizeof( T ) > sizeof( U ) ? sizeof( T ) : sizeof( U );
    if ( size == 1 )
        return uint8( x ) > uint8( x + y );
    if ( size == 2 )
        return uint16( x ) > uint16( x + y );
    if ( size == 4 )
        return uint32( x ) > uint32( x + y );
    return uint64( x ) > uint64( x + y );
}



#else
// The following definition is not quite correct because it always returns
// uint64. The above C++ functions are good, though.
#define __PAIR__(high, low) (((uint64)(high)<<sizeof(high)*8) | low)
// For C, we just provide macros, they are not quite correct.
#define __ROL__(x, y) __rotl__(x, y)      // Rotate left
#define __ROR__(x, y) __rotr__(x, y)      // Rotate right
#define __CFSHL__(x, y) invalid_operation // Generate carry flag for (x<<y)
#define __CFSHR__(x, y) invalid_operation // Generate carry flag for (x>>y)
#define __CFADD__(x, y) invalid_operation // Generate carry flag for (x+y)
#define __CFSUB__(x, y) invalid_operation // Generate carry flag for (x-y)
#define __OFADD__(x, y) invalid_operation // Generate overflow flag for (x+y)
#define __OFSUB__(x, y) invalid_operation // Generate overflow flag for (x-y)
#endif

// No definition for rcl/rcr because the carry flag is unknown
#define __RCL__(x, y)    invalid_operation // Rotate left thru carry
#define __RCR__(x, y)    invalid_operation // Rotate right thru carry
#define __MKCRCL__(x, y) invalid_operation // Generate carry flag for a RCL
#define __MKCRCR__(x, y) invalid_operation // Generate carry flag for a RCR
#define __SETP__(x, y)   invalid_operation // Generate parity flag for (x-y)

// In the decompilation listing there are some objects declarared as _UNKNOWN
// because we could not determine their types. Since the C compiler does not
// accept void item declarations, we replace them by anything of our choice,
// for example a char:

#define _UNKNOWN char

//#ifdef _MSC_VER
//#define snprintf _snprintf
//#define vsnprintf _vsnprintf
#endif