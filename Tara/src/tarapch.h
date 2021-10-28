//#pragma once //this is technically a main file, so this is silly to have

//c++ standard stuff

//data structures
#include <string>
#include <sstream>
#include <array>
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
#include <numeric>

#include <exception>
#include <stdexcept>


//include Windows.h if we are on windows
#ifdef _WIN32
#include <Windows.h>
#endif

//library stuff
#define LOGURU_WITH_STREAMS 1
#include "loguru.hpp"
#include "glm/glm.hpp"
#include <sol/sol.hpp>

//tara stuff
#include "Tara/Core/Base.h"