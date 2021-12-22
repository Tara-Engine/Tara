/*
A Tara application, for testing purposes.
*/

#include <random>
#include <any>

#include <Tara.h>

/*
OpenGL include - BAD - used for developing new features ONLY
*/
#include "glad/glad.h"

//testing types
#include "TColorRectEntity.h"
#include "PawnEntity.h"
#include "TOrthoCameraControllerComponent.h"
//#include "BatchTestLayer.h"
#include "EditorCameraControllerComponent.h"
#define SPRITE_MAX 100

#include "DemoLayer.h"
#include "FramebufferBuildLayer.h"
#include "UIBuildLayer.h"
#include "ModelBuildLayer.h"


void LayerSwitch(const std::string& newLayerName, Tara::LayerNoRef currentLayer);


struct TestStruct {
	int32_t a = 77;
	float b = 1.6f;
	bool original = true;
	TestStruct() {
		LOG_S(WARNING) << "Test Struct constructed!";
	}
	TestStruct(const TestStruct& other) {
		LOG_S(WARNING) << "Test Struct copied!";
	}
	TestStruct(TestStruct&& other) {
		other.original = false;
		LOG_S(WARNING) << "Test Struct moved!";
	}
	~TestStruct() {

		LOG_S(WARNING) << "Test Struct destroyed! Original:" << ((original)?"true":"false");
	}
	void operator=(const TestStruct& other) {
		LOG_S(WARNING) << "Test Struct Copied by Operator!";
	}
	void operator=(TestStruct&& other) {
		other.original = false;
		LOG_S(WARNING) << "Test Struct Moved by Operator!";
	}
	std::string ToString() {
		std::stringstream ss;
		ss << "{" << a << ", " << b << "}";
		return ss.str();
	}
};

/// <summary>
/// Testing Layer
/// </summary>
class TestingLayer : public Tara::Layer {
public:
	/// <summary>
	/// Constructor
	/// </summary>
	TestingLayer() 
		: m_Player(nullptr) 
	{}
	
	~TestingLayer() {
		Deactivate();
	}

	/// <summary>
	/// Activattion function override
	/// </summary>
	virtual void Activate() override {
		LOG_S(INFO) << "Testing Layer Activated!";

		//Tara::Texture::SetDefaultTextureFiltering(Tara::Texture::Filtering::Linear);
		Tara::Texture::SetDefaultTextureFiltering(Tara::Texture::Filtering::Nearest);


		font = Tara::Font::Create("assets/LiberationSans-Regular.ttf", 1024, 96, "arial");

		//make a texture asset
		auto textureUVChecker = Tara::Texture2D::Create("assets/UV_Checker.png");
		auto textureDirArrows = Tara::Texture2D::Create("assets/DirArrows.png");
		textureDirArrows->SetFiltering(Tara::Texture::Filtering::Linear);

		auto textureParticle4x1 = Tara::Texture2D::Create("assets/Particle_4x1.png");
		//auto textureParticle2x2 = Tara::Texture2D::Create("assets/Particle_2x2.png");
		//auto textureParticle1x4 = Tara::Texture2D::Create("assets/Particle_1x4.png");


		//tileset test
		m_tileset = Tara::Tileset::Create("assets/TestSet.json", "TestTileset");

		auto tiles2Texture = Tara::Texture2D::Create("assets/Tiles2x2.png");

		auto tileset2 = Tara::Tileset::Create(tiles2Texture, 16, 16, 0, 0, "Tiles2");

		m_tileset->SetTileMetadata(0, TestStruct());

		//LOG_S(INFO) << ((TestStruct*)(m_tileset->GetTileMetadata(0)))->a;

		m_tilemap = Tara::CreateEntity<Tara::TilemapEntity>(
			Tara::EntityNoRef(), weak_from_this(),
			std::initializer_list<Tara::TilesetRef>{m_tileset, tileset2}, //unfortunately, must explicitly declare this. Variable Args thing.
			TRANSFORM_2D(0,0,0,1,1), "Tilemap Entity"
		);

		m_tilemap->SetLayerColliding(0, true);

		//m_tilemap->FillFromJson("assets/testMapInf.json");


		m_tilemap->SwapTile(0, 0, 0, 0);



		//m_tilemap->SwapTile(0, 1, 0, 1);
		//m_tilemap->SwapTile(1, 0, 0, 2);
		//m_tilemap->SwapTile(1, 1, 0, 3);
		//m_tilemap->SwapTile(-1, -1, 0, 3);
		//m_tilemap->SwapTile(0, 0, 0, 4);
		//m_tilemap->SwapTile(-1, 0, 0, 0);

		//m_tilemap->PushLayer(); //explicit add new layer
		//m_tilemap->SwapTile(2, 0, 1, 1); //tile on new layer


		//tilemap->SwapTile(10, 9, 0, 3);


		//the number here is width in world units the screen will be wide. Has no effect on window size.
		//to make in 1:1 with pixels, set it to the width of the window.
		
		//m_Camera = Tara::CameraEntity::Create(Tara::EntityNoRef(), weak_from_this(), Tara::Camera::ProjectionType::Ortographic, TRANSFORM_DEFAULT, "camera");
		m_Camera = Tara::CreateEntity<Tara::CameraEntity>(
			Tara::EntityNoRef(), weak_from_this(), 
			Tara::Camera::ProjectionType::Ortographic, 
			Tara::Transform{ {0.0f, 0.0f, 0.0f},{0.0f,0.0f,0.0f},{1.0f} }, 
			"camera"
		);

		//m_Camera->SetPerspectiveFOV(45.0f);


		m_Camera->SetOrthographicExtent(16.0f);
		
		SetLayerCamera(m_Camera);
		
		m_Camera->GetCamera()->SetRenderFilterBits(0x0000000F);
		//auto dmce = Tara::CreateEntity<Tara::DynamicMultiChildEntity>(Tara::EntityNoRef(), weak_from_this());

		
		auto sprite = Tara::Sprite::Create(textureDirArrows, 1, 1, "ParticleSprite");
		
		m_TempSpriteEntity = Tara::CreateEntity<PawnEntity>(m_Camera, weak_from_this(), TRANSFORM_DEFAULT, "sprite", sprite);

		//m_TempSpriteEntity->SetFlip(SPRITE_FLIP_H | SPRITE_FLIP_V);

		

		m_Player = Tara::CreateEntity<Tara::SpriteEntity>(Tara::EntityNoRef(), weak_from_this(), TRANSFORM_DEFAULT, "player");
		m_Player->SetTint({ 0.0f, 1.0f, 0.0f, 0.25f });
		//Tara::CreateComponent<Tara::ClickableComponent>(m_Player, "ClickDetector");

		
		auto textEntity = Tara::CreateEntity<Tara::TextEntity>(Tara::EntityNoRef(), weak_from_this(), font, "Great day, isn't it?\nNEWLINE!", TRANSFORM_DEFAULT);
		textEntity->SetColor({ 1,0,0,1 });

		//textEntity->SetRelativePosition(Tara::Vector{ -1, 0, -1 });

		
		//Controller Component
		//auto controller = Tara::CreateComponent<EditorCameraControllerComponent>(m_Camera, 1.0f, "CameraController");
		auto controller = Tara::CreateComponent<Tara::ScriptComponent>(m_Camera, "assets/PlayerController.lua", "playerController");
		
		
		

		//auto luaComponent1 = Tara::CreateComponent<Tara::ScriptComponent>(m_tilemap, "assets/Component1.lua", "LuaComponent1");
		//auto luaComponent2 = Tara::CreateComponent<Tara::ScriptComponent>(m_Camera, "assets/Component2.lua", "LuaComponent2");




		
	}
	
	virtual void Deactivate() override {
		LOG_S(INFO) << "Testing Layer Deactivated!";
	}

	virtual void Update(float deltaTime) override {
		//SCOPE_PROFILE("Update");
		//Tara::Timer timer("UpdateTimer"); 
		//call super update function
		Tara::Layer::Update(deltaTime);

		//if (Tara::Input::Get()->IsMouseDown(TARA_MOUSE_BUTTON_1)) {
		//	//test the whole camera ray thing
		//	auto ray = m_Camera->GetCamera()->GetRayFromScreenCoordinate(Tara::Input::Get()->GetMouseX(), Tara::Input::Get()->GetMouseY());
		//	//since ortho, only origin is nesecary
		//	//set the m_TempSpriteEntity location
		//	m_TempSpriteEntity->SetWorldPosition(ray.first + (ray.second * 5));
		//}
	}
	
	/*
	virtual void Draw(float deltaTime) override{
		Tara::Renderer::BeginScene(m_Camera->GetCamera());
		
		Tara::Layer::Draw(deltaTime);

		//auto uvs = m_tileset->GetTileUVs(2);
		//auto tex = m_tileset->GetTexture();
		//Tara::Renderer::Quad(TRANSFORM_2D(-1, 0, 0, 1, 1), { 1,1,1,1 }, tex, uvs.first, uvs.second);

		//Tara::Renderer::Text(TRANSFORM_2D(1, 1, 0, 1, 1), "Hello World", font);
		//Tara::Renderer::Quad(TRANSFORM_2D(1, 1, 0, 2, 2), { 1,1,1,1 }, font->GetTexture());
		
		
		//Tara::RenderCommand::PushDrawType(Tara::RenderDrawType::LineStrip, false);
		//Tara::Renderer::Draw(m_VertexArray, m_SimpleShader, TRANSFORM_2D(-1, -1, 0, 1, 1));
		//Tara::RenderCommand::PopDrawType();

		Tara::Renderer::EndScene();
	}
	*/
	
	
	virtual void OnEvent(Tara::Event& e) override {
		//LOG_S(INFO) << "TestingLayer::OnEvent!";


		Tara::EventFilter filter(e);
		filter.Call<Tara::MouseButtonPressEvent>(TARA_BIND_FN(TestingLayer::OnMousePressedEvent));
		filter.Call<Tara::KeyPressEvent>(TARA_BIND_FN(TestingLayer::OnKeyPressedEvent));
		///LOG_S(INFO) << "LAYER CALLING PARENT EVENT";
		Layer::OnEvent(e); //parent call
	}

	bool OnKeyPressedEvent(Tara::KeyPressEvent& e) {
		//LOG_S(INFO) << "Key pressed called!";
		//Tara::After(std::bind(&TestingLayer::test, this, m_Player), 1.0f);
		
		//if (m_TempSpriteEntity){
		//	m_TempSpriteEntity->Destroy();
		//	m_TempSpriteEntity.reset();
		//}
		//if (e.GetKey() == TARA_KEY_F1) {
		//	//load new layer
		//	LOG_S(INFO) << "LAYER SWITCH";
		//	LayerSwitch("basic", weak_from_this());
		//}
		m_tilemap->SwapTile(-1, 0, 0, Tara::TilemapEntity::NO_TILE);
		return false;
	}

	void test(std::shared_ptr<TColorRectEntity> player) {
		player->SetColor({ 0.0f, 0.0f, 1.0f, 1.0f });
		LOG_S(INFO) << "After function executed";
	}

	bool OnMousePressedEvent(Tara::MouseButtonPressEvent& e) {
		if (e.GetButton() == TARA_MOUSE_BUTTON_3) {
			for (auto entity : GetEntityList()) {
				LOG_S(INFO) << entity->GetName() << "{" << (entity->GetParent().lock() == nullptr) << "}";
				entity->DebugLogAllChildren(true, 1);
			}
		}

		if (e.GetButton() == TARA_MOUSE_BUTTON_2) {
			glm::vec2 mousepos = Tara::Input::Get()->GetMousePos();

			//try and get a camera
			Tara::CameraEntityRef camEntity = m_tilemap->GetOwningLayer().lock()->GetLayerCamera().lock();
			if (camEntity) {

				//get the mouse pos in the world using the camera
				std::pair<Tara::Vector, Tara::Vector> ray = camEntity->GetCamera()->GetRayFromScreenCoordinate(mousepos.x, mousepos.y);

				int32_t tileX = (int32_t)floorf(ray.first.x);
				int32_t tileY = (int32_t)floorf(ray.first.y);

				//get the tile and cell metadata, and if not nullptr, print the data
				std::any cellMeta = m_tilemap->GetCellMetadata(tileX, tileY, 0);
				std::any tileMeta = m_tilemap->GetTileMetadata(tileX, tileY, 0);

				LOG_S(INFO) << "Cell Meta Type: " << cellMeta.type().name();
				LOG_S(INFO) << "Tile Meta Type: " << tileMeta.type().name();

				if (tileMeta.type() != typeid(nullptr)) {
					uint32_t tile = m_tilemap->GetTile(tileX, tileY, 0);
					m_tileset->WipeTileMetadata(tile);
				}
				if (TestStruct* tt = std::any_cast<TestStruct>(&tileMeta)) {
					LOG_S(INFO) << "Tile Meta: " << tt->ToString();
				}

				if (TestStruct* tt = std::any_cast<TestStruct>(&cellMeta)) {
					LOG_S(INFO) << "Cell Meta: " << tt->ToString();
				}
			}
		}

		if (e.GetButton() == TARA_MOUSE_BUTTON_1) {
			//Tilemap stuff
			glm::vec2 mousepos = Tara::Input::Get()->GetMousePos();

			//try and get a camera
			Tara::CameraEntityRef camEntity = m_tilemap->GetOwningLayer().lock()->GetLayerCamera().lock();
			if (camEntity) {

				//get the mouse pos in the world using the camera
				std::pair<Tara::Vector, Tara::Vector> ray = camEntity->GetCamera()->GetRayFromScreenCoordinate(mousepos.x, mousepos.y);

				int32_t tileX = (int32_t)floorf(ray.first.x);
				int32_t tileY = (int32_t)floorf(ray.first.y);

				uint32_t newIndex = rand() % 4;

				//if there is no tile, set one. Otherwise, remove it!
				uint32_t oldTile = m_tilemap->GetTile(tileX, tileY, 0);
				if (oldTile == Tara::TilemapEntity::NO_TILE) {
					LOG_S(INFO) << "Tile is empty! " << oldTile;
					m_tilemap->SwapTile(tileX, tileY, 0, newIndex);
					if ((rand() % 3) == 0) {
						m_tilemap->SetCellMetadata(tileX, tileY, 0, TestStruct());
					}
				}
				else {
					LOG_S(INFO) << "Tile is not empty! " << oldTile;
					//set the cell to empty.
					m_tilemap->SetTile(tileX, tileY, 0, Tara::TilemapEntity::NO_TILE);
				}

			}


		}

		return false;
	}

private:
	//TESTING STUFF
	//Tara::Texture2DRef m_Texture;
	//Tara::Texture2DRef m_Texture2;
	Tara::TilesetRef m_tileset;
	Tara::ShaderRef m_SimpleShader;
	Tara::VertexArrayRef m_VertexArray;
	
	Tara::FontRef font;
	//Tara::CameraRef m_Camera;
	
	//float m_PlayerSpeed = 1.0f;

	Tara::CameraEntityRef m_Camera;
	Tara::SpriteEntityRef m_Player;
	Tara::SpriteEntityRef m_TempSpriteEntity;
	//float m_TimeCounter = 0;

	//Tara::ShaderRef m_GShader;
	//Tara::VertexArrayRef m_QuadPoints;

	Tara::TilemapEntityRef m_tilemap;
};


int main(int argc, char** argv) {
	Tara::Script::Get()->SetDefaultLibraryPath("../Tara/lua");
	Tara::Application::Get()->Init(1200, 700, "Tara Playground Application!");
	//init stuff we have to do
	Tara::Script::RegisterType<PawnEntity>("PawnEntity"); //register PawnEntity

	//add layers to scene...
	//Tara::Application::Get()->GetScene()->PushLayer(std::make_shared<DemoLayer>());
	//Tara::Application::Get()->GetScene()->PushLayer(std::make_shared<TestingLayer>());
	Tara::Application::Get()->GetScene()->PushLayer(std::make_shared<FramebufferBuildLayer>());
	//Tara::Application::Get()->GetScene()->PushLayer(std::make_shared<UIBuildLayer>());
	//Tara::Application::Get()->GetScene()->PushLayer(std::make_shared<ModelBuildLayer>());
	//run
	Tara::Application::Get()->Run();

	//_CrtDumpMemoryLeaks();
	return 0;
}

void LayerSwitch(const std::string& newLayerName, Tara::LayerNoRef currentLayer)
{
	if (newLayerName == "basic") {
		Tara::Application::Get()->GetScene()->PushLayer(std::make_shared<DemoLayer>());
	}
	else {
		LOG_S(WARNING) << "Attempted to switch to unknown layer: " << newLayerName;
		return; //make sure not to unload current layer
	}
	if (currentLayer.lock()) {
		Tara::Application::Get()->GetScene()->RemoveLayer(currentLayer.lock());
	}
}