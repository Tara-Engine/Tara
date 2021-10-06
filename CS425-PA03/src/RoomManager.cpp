#include "RoomManager.h"

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


void RoomManager::LoadRoomTextures(){
	for (int doors = 0x1; doors < 0x10; doors++) {
		for (int perms = 1; perms < 5; perms++) {
			std::string name = RoomManager::StatesAndPermToString(doors, perms);
			Tara::Texture2D::Create(std::string("assets/") + name + ".png", name);
		}
	}
}

/*************************************************
*				TODO SECTION					 *
**************************************************/


bool RoomManager::AddRoom(int32_t x, int32_t y, uint32_t doors, uint32_t perm)
{
	return false;
}



RoomEntityRef RoomManager::GetRoomAt(int32_t x, int32_t y)
{
	return nullptr;
}

void RoomManager::Generate()
{
}

