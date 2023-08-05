#pragma once

#define WIN32_LEAN_AND_MEAN  
#define _CRT_SECURE_NO_WARNINGS_


#pragma warning(disable  : 4996).
#pragma warning(disable  : 4530).

#include <Windows.h>
#include <tlhelp32.h>
#include <iostream>
#include <dwmapi.h>
#include <winioctl.h>
#include <bcrypt.h>
#include <vector>
#include <array>
#include <thread>
#include <mutex>
#include <string>
#include <list>
#include <d3d11.h>
#include <algorithm>
#include <TlHelp32.h>

#pragma comment(lib, "d3d11.lib")

#define m_log(fmt, ...) printf("[%s] " fmt, __FUNCTION__, ##__VA_ARGS__)

#include <dependencies/cfg/cfg.h>
#include <dependencies/xor.hpp>

#include "defs.h"

#include <dependencies/gui/imgui.h>
#include <dependencies/gui/imgui_impl_win32.h>
#include <dependencies/gui/imgui_impl_dx11.h>

#include <dependencies/driver/driver.h>
#include <dependencies/util.h>

#include <dependencies/ue/implementation.hpp>
#include <dependencies/ue/structures.h>

#include <impl/overwolf.h>

