#pragma once

/*Tara Main Header File, for external includes*/
#include "Tara/Core/Base.h"

//input
#include "Tara/Input/Event.h"
#include "Tara/Input/Mappings.h"
#include "Tara/Input/Input.h"
#include "Tara/Input/ApplicationEvents.h"
#include "Tara/Input/EventListener.h"

//renderer
#include "Tara/Renderer/Renderer.h"
#include "Tara/Renderer/Buffer.h"
#include "Tara/Renderer/VertexArray.h"
#include "Tara/Renderer/Shader.h"
#include "Tara/Renderer/RenderCommand.h"
#include "Tara/Renderer/Texture.h"
#include "Tara/Renderer/Camera.h"
#include "Tara/Renderer/MaterialBase.h"

//Core
#include "Tara/Core/Scene.h"
#include "Tara/Core/Layer.h"
#include "Tara/Core/Window.h"
#include "Tara/Core/Application.h"
#include "Tara/Core/Entity.h"
#include "Tara/Core/Script.h"

//Assets
#include "Tara/Asset/AssetLibrary.h"
#include "Tara/Asset/Asset.h"
#include "Tara/Asset/Font.h"
#include "Tara/Asset/Tileset.h"
#include "Tara/Asset/Patch.h"
#include "Tara/Asset/StaticMesh.h"
#include "Tara/Asset/Material.h"

//math
#include "Tara/Math/Types.h"
#include "Tara/Math/BoundingBox.h"
#include "Tara/Math/Functions.h"
#include "Tara/Math/Noise.h"

//Entites
#include "Tara/Entities/SpriteEntity.h"
#include "Tara/Entities/CameraEntity.h"
#include "Tara/Entities/DynamicMultiChildEntity.h"
#include "Tara/Entities/TextEntity.h"
#include "Tara/Entities/TilemapEntity.h"
#include "Tara/Entities/StaticMeshEntity.h"

//Components
#include "Tara/Components/ScriptComponent.h"
#include "Tara/Components/ClickableComponent.h"
#include "Tara/Components/LambdaComponent.h"

//UI
#include "Tara/UI/UIBaseEntity.h"
#include "Tara/UI/UIVisualEntity.h"
#include "Tara/UI/UIListEntity.h"
#include "Tara/UI/UISpacerEntity.h"
#include "Tara/UI/UITextEntity.h"
#include "Tara/UI/UIButtonEntity.h"
#include "Tara/UI/UIFrameEntity.h"

//Utilities
#include "Tara/Utility/After.h"
#include "Tara/Utility/Timer.h"
#include "Tara/Utility/Profiler.h"
#include "Tara/Utility/MeshPart.h"
#include "Tara/Utility/MeshMaker.h"

#define LOGURU_WITH_STREAMS 1
#include "loguru.hpp"
//sol
#include <sol/sol.hpp>