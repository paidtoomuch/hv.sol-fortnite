
typedef enum _requests
{
	invoke_start,

	invoke_base,
	invoke_read,
	//invoke_read_large,
	invoke_write,
	invoke_resolve_dtb,
	invoke_peb,

	invoke_success,
	invoke_unique,
	invoke_unload,
}requests, * prequests;

struct m_read_info
{
	uintptr_t address;
	void* buffer;
	size_t size;
};

typedef struct _read_invoke
{
	//bool pool;
	uint32_t pid;
	uintptr_t address;
	void* buffer;
	size_t size;
} read_invoke, * pread_invoke;

typedef struct _write_invoke
{
	uint32_t pid;
	uintptr_t address;
	void* buffer;
	size_t size;
} write_invoke, * pwrite_invoke;

typedef struct _dtb_invoke
{
	uint32_t pid;
	bool operation;
} dtb_invoke, * pdtb_invoke;

typedef struct _base_invoke
{
	uint32_t pid;
	uintptr_t handle;
	const char* name;
	size_t size;
} base_invoke, * pbase_invoke;

typedef struct _peb_invoke
{
	uint32_t pid;
	ULONGLONG handle;
} peb_invoke, * ppeb_invoke;

typedef struct _invoke_data
{
	uint32_t unique;
	requests code;
	void* data;
}invoke_data, * pinvoke_data;

