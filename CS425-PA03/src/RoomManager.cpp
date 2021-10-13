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
	if (perm > 0 && perm < 5) {
		ss << perm;
	}
	else {
		ss << 1; 
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


Tara::SpriteRef RoomManager::RoomTextureFromDoorsAndPerm(uint32_t doorState, uint32_t perm)
{
	std::string name = RoomManager::StatesAndPermToString(doorState, perm);
	auto asset = Tara::AssetLibrary::Get()->GetAssetIf<Tara::Sprite>(name);
	if (!asset) {
		LOG_S(WARNING) << "Unknown door image get apptempted: " << name;
		asset = Tara::AssetLibrary::Get()->GetAssetIf<Tara::Sprite>("1-D"); //default is 1-D.png, if that fails, then nullptr
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
			Tara::Sprite::Create(Tara::Texture2D::Create(std::string("assets/") + name + ".png", name + "_Texture"), 1, 1, name);
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

void RoomManager::Generate(uint32_t seed, int32_t width, int32_t height, int32_t steps)
{
	srand(seed);
	auto x = rand() % width;
	auto y = rand() % height;
	uint8_t *doorMatrix = new uint8_t[width * height];
	// initialize the array to all zeroes
	memset(doorMatrix, 0, width * height);

	for (int i = 0; i < steps; i++)
	{
		// pick a random direction
		auto dir = 1 << (rand() % 4);
		auto oob = true;
		while (oob)
		{
			// make sure it won't take us outside the established region
			if ((dir == DOORSTATE_UP && y != 0) || (dir == DOORSTATE_DOWN && y != width - 1) || (dir == DOORSTATE_LEFT && x != 0) || (dir == DOORSTATE_RIGHT && x != height - 1))
			{
				oob = false;
			}
			if (oob)
			{
				dir <<= 1;
				if (dir > 8)
					dir = 1;
			}
		}
		// make the active cell link in that direction
		doorMatrix[x * height + y] |= dir;
		// mirror the link and move to that cell
		switch (dir)
		{
		case DOORSTATE_UP:
			y--;
			break;
		case DOORSTATE_DOWN:
			y++;
			break;
		case DOORSTATE_LEFT:
			x--;
			break;
		case DOORSTATE_RIGHT:
			x++;
			break;
		}
		doorMatrix[x * height + y] |= InvertDoorState(dir);
	}
	// TODO: goal tile, spawn tile
	// push all the rooms
	for (int x = 0; x < width; x++)
	{
		for (int y = 0; y < height; y++)
		{
			if (doorMatrix[x * height + y] != 0)
			{
				RoomManager::AddRoom(x, -y, doorMatrix[x * height + y] & DOORSTATE_ALL, doorMatrix[x * height + y] > 64 ? 4 : ((rand() % 3) + 1));
			}
		}
	}
}

