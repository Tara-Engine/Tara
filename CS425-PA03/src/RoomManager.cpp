#include "RoomManager.h"
#include <bitset>

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
		LOG_S(WARNING) << "Unknown door image get attempted: " << name;
		asset = Tara::AssetLibrary::Get()->GetAssetIf<Tara::Sprite>("1-D"); //default is 1-D.png, if that fails, then nullptr
	}
	return asset;
}

glm::ivec2 RoomManager::WorldCoordToRoomCoord(glm::vec2 worldCoord)
{
	//int32_t x = (int32_t)worldCoord.x - (int32_t)worldCoord.x % (int32_t)ROOM_SCALE;
	//int32_t y = (int32_t)worldCoord.y - (int32_t)worldCoord.y % (int32_t)ROOM_SCALE;
	return glm::ivec2(worldCoord.x/ROOM_SCALE, worldCoord.y/ROOM_SCALE-1);
}


glm::vec2 RoomManager::RoomCoordToWorldCoord(glm::ivec2 roomCoord)
{
	glm::vec2 pos = { roomCoord.x * ROOM_SCALE, roomCoord.y * ROOM_SCALE}; //player pos in Map coordinates
	//center the position, and horizontal adjust for cell grid allignment. Y is slightly misalligned for visuals.
	pos += glm::vec2{ ((ROOM_SCALE / 2) - (ROOM_SCALE / 18)), (ROOM_SCALE / 2) };
	return pos;
}

std::pair<bool, bool> RoomManager::IsCentered(glm::vec2 pos)
{
	auto result = std::make_pair(false, false);
	float center = abs(fmod(pos.x, ROOM_SCALE) / ROOM_SCALE);
	if (center > 0.4 && center < 0.6)
		result.first = true;
	center = abs(fmod(pos.y, ROOM_SCALE) / ROOM_SCALE);
	if (center > 0.4 && center < 0.6)
		result.second = true;

	return result;
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
	auto room = Tara::CreateEntity<RoomEntity>(
		m_DefaultParent, m_DefaultLayer, 
		doors, perm,
		Tara::Transform(
			Tara::Vector( x * ROOM_SCALE, y * ROOM_SCALE, 0.0f ), 
			Tara::Rotator(0.0f, 0.0f, 0.0f), 
			Tara::Vector(ROOM_SCALE, ROOM_SCALE, 1.0f)
		),
		"RoomEntity",
		RoomManager::RoomTextureFromDoorsAndPerm(doors, perm)
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

std::list<int32_t> RoomManager::Generate(uint32_t seed, int32_t width, int32_t height, int32_t steps)
{
	LOG_S(INFO) << "Room Generator Seed: " << seed;
	srand(seed);
	m_Width = width;
	m_Height = height;
	auto x = rand() % width;
	auto y = rand() % height;
	auto doorMatrix = std::vector<uint8_t>(width * height, 0);
	//doorMatrix.reserve();
	//uint8_t *doorMatrix = new uint8_t[width * height];
	// initialize the array to all zeroes
	//memset(doorMatrix.data(), 0, width * height);

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
		if (cellX < 0) {cellX = 0;}
		if (doorMatrix[cell] == 0)
		{
			int dir = 1;
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
				auto cell2 = cellX * height + cellY;
				if (cellX >= 0 && cellX < width && cellY >= 0 && cellY < height && doorMatrix[cell2])
				{
					// create a new room and make that the goal
					doorMatrix[cell] |= dir;
					doorMatrix[cell] |= 0x80;
					doorMatrix[cell2] |= InvertDoorState(dir);
					oob = false;
					goal = cell;
				}
				dir <<= 1;
				if (dir > 8)
					oob = false;
			}
		}
		else if (doorMatrix[cell] == DOORSTATE_UP || doorMatrix[cell] == DOORSTATE_DOWN || doorMatrix[cell] == DOORSTATE_LEFT || doorMatrix[cell] == DOORSTATE_RIGHT)
		{
			// turn an existing dead end into the goal
			doorMatrix[cell] |= 0x80;
			goal = cell;
		}
	}
	m_Goal = glm::vec2((goal - (goal % width)) / height, (goal % width) * -1);
	// make a matrix of distance from the goal cell
	//int32_t *distMatrix = new int32_t[width * height];
	//memset(distMatrix, -2, width * height);
	auto distMatrix = std::vector<int32_t>(width * height, 999);
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
		int32_t up = cellX * height + (cellY - 1);
		int32_t down = cellX * height + (cellY + 1);
		int32_t left = (cellX - 1) * height + cellY;
		int32_t right = (cellX + 1) * height + cellY;
		if (cellY > 0 && (doorMatrix[up] & DOORSTATE_DOWN) && (distMatrix[up] > depth))
			distMatrix[up] = depth;
		if (cellY < height - 1 && (doorMatrix[down] & DOORSTATE_UP) && (distMatrix[down] > depth))
			distMatrix[down] = depth;
		if (cellX > 0 && (doorMatrix[left] & DOORSTATE_RIGHT) && (distMatrix[left] > depth))
			distMatrix[left] = depth;
		if (cellX < height - 1 && (doorMatrix[right] & DOORSTATE_LEFT) && (distMatrix[right] > depth))
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
	// clean up after the 999 placeholders
	for (int i = 0; i < width * height; i++)
	{
		if (distMatrix[i] == 999)
			distMatrix[i] = -1;
	}
	for (int i = 0; i < height; i++)
	{
		std::string line = "";
		for (int j = 0; j < width; j++)
		{
			line += std::to_string(distMatrix[j * height + i]);
			line += (std::string)",";
		}
		LOG_S(INFO) << line;
	}
	// collect cells of adequate distance and pick one for the start cell
	cellQueue.clear(); // shouldn't be needed but just in case
	depth = *(std::max_element(std::begin(distMatrix), std::end(distMatrix)));
	if (depth > 3)
		depth = 3;
	for (int i = 0; i < width * height; i++)
	{
		if (distMatrix[i] >= depth && distMatrix[i] != 999)
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
	depth = distMatrix[start] - 1;
	while (depth >= 0)
	{
		// TODO: make this direction random?
		int32_t cell = cellQueue.back();
		int32_t cellY = cell % width;
		int32_t cellX = (cell - cellY) / height;
		int32_t up = cellX * height + (cellY - 1);
		int32_t down = cellX * height + (cellY + 1);
		int32_t left = (cellX - 1) * height + cellY;
		int32_t right = (cellX + 1) * height + cellY;
		if (cellY > 0 && distMatrix[up] == depth && doorMatrix[cell] & DOORSTATE_UP)
			cellQueue.push_back(up);
		else if (cellY < height - 1 && distMatrix[down] == depth && doorMatrix[cell] & DOORSTATE_DOWN)
			cellQueue.push_back(down);
		else if (cellX > 0 && distMatrix[left] == depth && doorMatrix[cell] & DOORSTATE_LEFT)
			cellQueue.push_back(left);
		else if (cellX < height - 1)// process of elimination
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
				auto state = (rand() % 3) + 1;
				if (std::find(cellQueue.begin(), cellQueue.end(), x * height + y) == cellQueue.end() && (rand() % 100 < 25) && (std::bitset<8>(doorMatrix[x * height + y] & DOORSTATE_ALL).count() > 1))
					state = 4;
				if (doorMatrix[x * height + y] & 0x80)
					state = 4;
				RoomManager::AddRoom(x, -y, doorMatrix[x * height + y] & DOORSTATE_ALL, state);
			}
		}
	}
	for (auto cell : cellQueue)
	{
		LOG_S(INFO) << ((cell - (cell % width)) / height) << "," << (cell % width) << " is on the path";
		//LOG_S(INFO) << cell << " is on the path";
	}
	//delete distMatrix;//Don't forget to do this!
	//delete doorMatrix;//
	return cellQueue; // head is start, tail is goal
}

void RoomManager::ClearAll()
{
	m_Rooms.clear();
	m_DefaultParent.reset();
	m_DefaultLayer.reset();
}

void RoomManager::RegisterLuaType(sol::state& lua)
{
	sol::usertype<RoomManager> type = lua.new_usertype<RoomManager>("RoomManager");
	CONNECT_METHOD(RoomManager, GetRoom);
	CONNECT_STATIC(RoomManager, Get);
	CONNECT_STATIC_OVERRIDE(RoomManager, WorldCoordToRoomCoord);
	CONNECT_STATIC_OVERRIDE(RoomManager, IsCentered);

}

sol::table RoomManager::__SCRIPT__WorldCoordToRoomCoord(sol::table worldCoord)
{
	//must parse the worldCoord table to vec2, run through WorldCoordToRoomCoord, then put into lua table
	sol::table table = NEW_TABLE;
	table["x"] = (int)(worldCoord.get_or("x", 0.0f) / ROOM_SCALE);
	table["y"] = (int)(worldCoord.get_or("y", 0.0f) / ROOM_SCALE - 1);
	return table;
}

std::pair<bool, bool> RoomManager::__SCRIPT__IsCentered(sol::table pos)
{
	glm::vec2 vpos(0,0);
	vpos.x = pos.get_or("x", -1.0f);
	vpos.y = pos.get_or("y", -1.0f);
	LOG_S(INFO) << "{" << vpos.x << "," << vpos.y << "}";
	auto p = IsCentered(vpos);
	return std::make_pair(p.first, p.second);
}

