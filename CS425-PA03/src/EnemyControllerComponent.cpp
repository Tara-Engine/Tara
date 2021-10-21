#include "EnemyControllerComponent.h"
#include "PawnEntity.h"
#include "RoomManager.h"
#include <queue>

const static float MOVEMENT_DISTANCE = 16 * 4; //size of 1 tile

EnemyControllerComponent::EnemyControllerComponent(Tara::EntityNoRef parent, const std::string& name)
	: Tara::Component(parent, name)
{}

void EnemyControllerComponent::OnUpdate(float deltaTime)
{
	auto parent = std::dynamic_pointer_cast<PawnEntity>(GetParent().lock()); //OK becase will be cleaned up and end of func call
	if (parent && !parent->GetTraveling()) {
		//choose a new destination
		auto roomCoords = RoomManager::WorldCoordToRoomCoord(glm::vec2(parent->GetWorldPosition()));
		auto room = RoomManager::Get()->GetRoom(roomCoords.x, roomCoords.y);
		auto doorState = room->GetDoorState();
		Tara::Vector dir = GetAStarDirection(roomCoords.x, roomCoords.y);
		//LOG_S(INFO) << dir.x << ", " << dir.y;
		/*uint32_t dirBit = 0;
		while (!(dirBit & doorState)) {
			dirBit = 1 << (rand() % 4); //should pick a random bit
		}
		Tara::Vector dir = {0,0,0};
		switch (dirBit) {
		case DOORSTATE_UP: {
			dir.y = 1;
			break;
		}
		case DOORSTATE_DOWN: {
			dir.y = -1;
			break;
		}
		case DOORSTATE_LEFT: {
			dir.x = -1;
			break;
		}
		case DOORSTATE_RIGHT: {
			dir.x = 1;
			break;
		}
		}*/
		if (dir.x != 0 || dir.y != 0) {
			dir *= MOVEMENT_DISTANCE * 9; //move 9 tiles, room center to room center
			parent->SetTarget(parent->GetWorldPosition() + dir, 0.25 * 9);
		}
	}
}

Tara::Vector EnemyControllerComponent::GetAStarDirection(int32_t x, int32_t y)
{
	auto manager = RoomManager::Get();
	int32_t width = manager->GetWidth();
	int32_t height = manager->GetHeight();
	// TODO: make these be player positions instead
	int32_t targetX = manager->GetGoal().x;
	int32_t targetY = manager->GetGoal().y;
	glm::vec2 target = glm::vec2(targetX, targetY);
	std::priority_queue<glm::vec3, std::vector<glm::vec3>, ZComparator> queue = std::priority_queue<glm::vec3, std::vector<glm::vec3>, ZComparator>();
	std::unordered_map<glm::vec2, std::list<glm::vec2>, vec_hash> visited = std::unordered_map<glm::vec2, std::list<glm::vec2>, vec_hash>();
	queue.push(glm::vec3(x, y, 0));
	visited[glm::vec2(x, y)] = std::list<glm::vec2>();
	while (true)
	{
		if (visited.count(target) != 0)
		{
			auto iter = visited[target].begin();
			// TODO: somehow this always stops one cell short of its destination
			if (iter != visited[target].end())
			{
				iter++;
				if (iter != visited[target].end())
				{
					auto front = *iter;
					return Tara::Vector(front.x - x, front.y - y, 0);
				}
				else
				{
					auto front = visited[target].front();
					return Tara::Vector(front.x - x, front.y - y, 0);
				}
			}
			else
			{
				return Tara::Vector(0, 0, 0); // already at target so don't move
			}
		}
		glm::vec2 current = queue.top();
		queue.pop();
		auto room = manager->GetRoom(current.x, current.y);
		if (room->GetDoorState() & DOORSTATE_UP && visited.count(glm::vec2(current.x, current.y+1)) == 0)
		{
			int32_t h = visited[current].size() + abs(current.x - targetX) + abs(current.y + 1 - targetY) + 1; // +1 to account for traceback length being one short
			queue.push(glm::vec3(current.x, current.y + 1, h));
			glm::vec2 next = glm::vec2(current.x, current.y + 1);
			visited[next] = std::list<glm::vec2>();
			for (auto v : visited[current])
			{
				visited[next].push_back(v);
			}
			visited[next].push_back(current);
		}
		if (room->GetDoorState() & DOORSTATE_DOWN && visited.count(glm::vec2(current.x, current.y - 1)) == 0)
		{
			int32_t h = visited[current].size() + abs(current.x - targetX) + abs(current.y - 1 - targetY) + 1;
			queue.push(glm::vec3(current.x, current.y - 1, h));
			glm::vec2 next = glm::vec2(current.x, current.y - 1);
			visited[next] = std::list<glm::vec2>();
			for (auto v : visited[current])
			{
				visited[next].push_back(v);
			}
			visited[next].push_back(current);
		}
		if (room->GetDoorState() & DOORSTATE_LEFT && visited.count(glm::vec2(current.x - 1, current.y)) == 0)
		{
			int32_t h = visited[current].size() + abs(current.x - 1 - targetX) + abs(current.y - targetY) + 1;
			queue.push(glm::vec3(current.x - 1, current.y, h));
			glm::vec2 next = glm::vec2(current.x - 1, current.y);
			visited[next] = std::list<glm::vec2>();
			for (auto v : visited[current])
			{
				visited[next].push_back(v);
			}
			visited[next].push_back(current);
		}
		if (room->GetDoorState() & DOORSTATE_RIGHT && visited.count(glm::vec2(current.x + 1, current.y)) == 0)
		{
			int32_t h = visited[current].size() + abs(current.x + 1 - targetX) + abs(current.y - targetY) + 1;
			queue.push(glm::vec3(current.x + 1, current.y, h));
			glm::vec2 next = glm::vec2(current.x + 1, current.y);
			visited[next] = std::list<glm::vec2>();
			for (auto v : visited[current])
			{
				visited[next].push_back(v);
			}
			visited[next].push_back(current);
		}
	}
}

