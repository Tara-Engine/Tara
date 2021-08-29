#pragma once

/*Tara Main Header File, for external includes*/
#include "Tara/Core/Base.h"

//input
#include "Tara/Input/Event.h"
#include "Tara/Input/Mappings.h"
#include "Tara/Input/Input.h"

//renderer
#include "Tara/Renderer/Renderer.h"
#include "Tara/Renderer/Buffer.h"
#include "Tara/Renderer/VertexArray.h"
#include "Tara/Renderer/Shader.h"
#include "Tara/Renderer/RenderCommand.h"

//Core
#include "Tara/Core/Scene.h"
#include "Tara/Core/Layer.h"
#include "Tara/Core/Window.h"
#include "Tara/Core/Application.h"


#define LOGURU_WITH_STREAMS 1
#include "loguru/loguru.hpp"