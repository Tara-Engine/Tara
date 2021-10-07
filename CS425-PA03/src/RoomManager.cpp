#include "RoomManager.h"

static const uint32_t ROOMSIZE_PX = 144; //pixel size of original room images
static const float ROOM_SCALE = ROOMSIZE_PX * 4.0f;


RoomManager::RoomManager()
{
	LoadRoomTextures();
}

RoomManager* RoomManager::Get()
{
	static RoomManager manager;
	return &manager;
}

std::string RoomManager::StatesAndPermToString(uint32_t doorStates, uint32_t perm)
{
	static char IDs[] = { 'U', 'D', 'L', 'R'};
	std::stringstream ss;
	if (perm > 0) {
		ss << perm;
	}
	else {
		ss << (rand() % MAX_ROOM_PERMS + 1); //could use a better random engine.
	}
	ss << '-';
	for (int i = 0; i < 4; i++) {
		if (doorStates & BIT(i)) { //BIT(x) is a macro provided by Tara to get the xth bit in a bitfield. IE, it is literally just (1 << x).
			ss << IDs[i];
		}
	}
	
	return ss.str();
}

uint32_t RoomManager::InvertDoorState(uint32_t state)
{
	switch (state) {
	case DOORSTATE_UP: return DOORSTATE_DOWN;
	case DOORSTATE_DOWN: return DOORSTATE_UP;
	case DOORSTATE_LEFT: return DOORSTATE_RIGHT;
	case DOORSTATE_RIGHT: return DOORSTATE_LEFT;
	default: return 0;
	}
}


Tara::Texture2DRef RoomManager::RoomTextureFromDoorsAndPerm(uint32_t doorState, uint32_t perm)
{
	auto asset = Tara::AssetLibrary::Get()->GetAssetIf<Tara::Texture2D>(RoomManager::StatesAndPermToString(doorState, perm));
	if (!asset) {
		asset = Tara::AssetLibrary::Get()->GetAssetIf<Tara::Texture2D>("1-D"); //default is 1-D.png, if that fails, then nullptr
	}
	return asset;
}


void RoomManager::Init(Tara::EntityNoRef defaultParent, Tara::LayerNoRef defaultLayer)
{
	m_DefaultParent = defaultParent;
	m_DefaultLayer = defaultLayer;
}

void RoomManager::LoadRoomTextures(){
	for (int doors = 0x1; doors < 0x10; doors++) {
		for (int perms = 1; perms < 5; perms++) {
			std::string name = RoomManager::StatesAndPermToString(doors, perms);
			Tara::Texture2D::Create(std::string("assets/") + name + ".png", name);
		}
	}
}




bool RoomManager::AddRoom(int32_t x, int32_t y, uint32_t doors, uint32_t perm)
{
	auto loc = std::make_pair(x, y);
	//auto loc = x;
	if (m_Rooms.find(loc) != m_Rooms.end()) {
		//the room already exists
		return false;
	}
	auto room = RoomEntity::Create(
		m_DefaultParent, m_DefaultLayer, 
		doors, perm, 
		Tara::Transform(
			Tara::Vector( x * ROOM_SCALE, y * ROOM_SCALE, 0.0f ), 
			Tara::Rotator(0.0f, 0.0f, 0.0f), 
			Tara::Vector(ROOM_SCALE, ROOM_SCALE, 1.0f)
		)
	);
	m_Rooms[loc] = room;
	return true;
}

RoomEntityRef RoomManager::GetRoom(int32_t x, int32_t y)
{
	auto loc = std::make_pair(x, y);
	//auto loc = x;
	auto f = m_Rooms.find(loc);
	if (f != m_Rooms.end()) {
		return f->second;
	}
	return nullptr;
}

/*************************************************
*				TODO SECTION					 *
**************************************************/

void RoomManager::Generate()
{
}

