#pragma once

//c++ standard stuff

//data structures
#include <string>
#include <sstream>
#include <list>
#include <vector>
#include <unordered_map>
#include <unordered_set>
#include <initializer_list>
#include <tuple>

#include <algorithm>
#include <memory>
#include <functional>
#include <utility>

#include <exception>
#include <stdexcept>


//include Windows.h if we are on windows
#ifdef _WIN32
#include <Windows.h>
#endif

//library stuff
#define LOGURU_WITH_STREAMS 1
#include "loguru/loguru.hpp"

#include "glm/glm.hpp"

//tara stuff
#include "Tara/Core/Base.h"