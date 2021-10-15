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

std::list<int32_t> RoomManager::Generate(uint32_t seed, int32_t width, int32_t height, int32_t steps)
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
	// pick a goal cell
	// if a dead end cell is selected, convert it to goal
	// if a cell bordering at least one other is selected, pick one of those to connect to and create the goal
	std::vector<int32_t> cellList = std::vector<int32_t>();
	for (int i = 0; i < width * height; i++)
	{
		cellList.push_back(i);
	}
	int32_t goal = -1;
	while (goal == -1)
	{
		int32_t index = rand() % cellList.size();
		int32_t cell = cellList[index];
		cellList.erase(cellList.begin() + index);
		int32_t cellY = cell % width;
		int32_t cellX = (cell - cellY) / height;
		if (doorMatrix[cell] == 0)
		{
			int dir = 1 << (rand() % 4);
			bool oob = true;
			while (oob) {
				switch (dir)
				{
				case DOORSTATE_UP:
					cellY--;
					break;
				case DOORSTATE_DOWN:
					cellY++;
					break;
				case DOORSTATE_LEFT:
					cellX--;
					break;
				case DOORSTATE_RIGHT:
					cellX++;
					break;
				}
				if (doorMatrix[cellX * height + cellY])
				{
					// create a new room and make that the goal
					doorMatrix[cell] |= dir;
					doorMatrix[cell] |= 0x80;
					doorMatrix[cellX * height + cellY] |= InvertDoorState(dir);
					oob = false;
					goal = cell;
				}
				dir <<= 1;
				if (dir > 8)
					dir = 1;
			}
		}
		else if (doorMatrix[cell] == DOORSTATE_UP || doorMatrix[cell] == DOORSTATE_DOWN || doorMatrix[cell] == DOORSTATE_LEFT || doorMatrix[cell] == DOORSTATE_RIGHT)
		{
			// turn an existing dead end into the goal
			doorMatrix[cell] |= 0x80;
			goal = cell;
		}
	}
	// make a matrix of distance from the goal cell
	int32_t *distMatrix = new int32_t[width * height];
	memset(distMatrix, -2, width * height);
	distMatrix[goal] = 0;
	std::list<int32_t> cellQueue = std::list<int32_t>();
	cellQueue.push_back(goal);
	int32_t depth = 1;
	while (!cellQueue.empty() && depth < 25)
	{
		int32_t cell = cellQueue.front();
		cellQueue.pop_front();
		int32_t cellY = cell % width;
		int32_t cellX = (cell - cellY) / height;
		int32_t up = (cellX - 1) * height + cellY;
		int32_t down = (cellX + 1) * height + cellY;
		int32_t left = cellX * height + (cellY - 1);
		int32_t right = cellX * height + (cellY + 1);
		if (doorMatrix[up] && (distMatrix[up] != depth - 1))
			distMatrix[up] = depth;
		if (doorMatrix[down] && (distMatrix[down] != depth - 1))
			distMatrix[down] = depth;
		if (doorMatrix[left] && (distMatrix[left] != depth - 1))
			distMatrix[left] = depth;
		if (doorMatrix[right] && (distMatrix[right] != depth - 1))
			distMatrix[right] = depth;
		if (cellQueue.empty())
		{
			for (int i = 0; i < width * height; i++)
			{
				if (distMatrix[i] == depth)
					cellQueue.push_back(i);
			}
			depth++;
		}
	}
	// collect cells of adequate distance and pick one for the start cell
	cellQueue.clear(); // shouldn't be needed but just in case
	if (depth > 3)
		depth = 3;
	for (int i = 0; i < width * height; i++)
	{
		if (distMatrix[i] >= depth)
			printf("%d is a spawn candidate\n", i);
			cellQueue.push_back(i);
	}
	int clears = rand() % cellQueue.size();
	for (int i = 0; i < clears; i++)
	{
		cellQueue.pop_front();
	}
	int32_t start = cellQueue.front();
	// build a linked list of rooms designating a path from start to goal
	cellQueue.clear();
	cellQueue.push_back(start);
	depth = distMatrix[start];
	while (depth > 0)
	{
		// TODO: make this direction random?
		int32_t cell = cellQueue.back();
		int32_t cellX = cell % width;
		int32_t cellY = (cell - cellX) / height;
		int32_t up = (cellX - 1) * height + cellY;
		int32_t down = (cellX + 1) * height + cellY;
		int32_t left = cellX * height + (cellY - 1);
		int32_t right = cellX * height + (cellY + 1);
		if (distMatrix[up] == depth - 1)
			cellQueue.push_back(up);
		else if (distMatrix[down] == depth - 1)
			cellQueue.push_back(down);
		else if (distMatrix[left] == depth - 1)
			cellQueue.push_back(left);
		else // process of elimination
			cellQueue.push_back(right);
		depth--;
	}
	// push all the rooms to the actual room manager
	for (int x = 0; x < width; x++)
	{
		for (int y = 0; y < height; y++)
		{
			if (doorMatrix[x * height + y])
			{
				RoomManager::AddRoom(x, -y, doorMatrix[x * height + y] & DOORSTATE_ALL, doorMatrix[x * height + y] & 0x80 ? 4 : ((rand() % 3) + 1));
			}
		}
	}
	delete[] distMatrix;//Don't forget to do this!
	delete[] doorMatrix;//
	return cellQueue; // head is start, tail is goal
}

