#pragma once
#include "pch.h"

#define AUTO_DETECT -1
#define AUTO_RESIZE -1, -1

using namespace std;
using Json = nlohmann::json;

enum OutputSizeMode { DISABLE = -1, ORIGIN_SIZE = -2 };

enum class OutputSizeMode2 { DEFAULT = 0, DISABLE = -1, ORIGIN_SIZE = -2 };
enum class OutputSetting { DEFAULT = 0, AUTO = -1, ONE_BY_ONE = -2 };