#ifndef TK_ENGINE_TK_ENGINE_H
#define TK_ENGINE_TK_ENGINE_H

#include <iostream>
#include <cassert>
#include <memory>
#include <algorithm>
#include <functional>

#include <string>
#include <sstream>
#include <fstream>
#include <filesystem>
#include <vector>
#include <stack>
#include <queue>
#include <deque>
#include <set>
#include <unordered_set>

#ifdef TK_ENGINE_PLATFORM_WIN32
#define VK_USE_PLATFORM_WIN32_KHR
#include <Windows.h>
#elif TK_ENGINE_PLATFORM_MACOS
#define VK_USE_PLATFORM_MACOS_MVK
#elif TK_ENGINE_PLATFORM_LINUX
#define VK_USE_PLATFORM_XCB_KHR
#else
#error Unsupports this Platform
#endif

#define TK_ENGINE_GRAPHIC_API_VULKAN

#endif //TK_ENGINE_TK_ENGINE_H
