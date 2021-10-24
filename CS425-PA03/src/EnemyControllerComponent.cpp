#include "EnemyControllerComponent.h"
#include "PawnEntity.h"
#include "RoomManager.h"
#include <queue>
#include <bitset>

const static float MOVEMENT_DISTANCE = 16 * 4; //size of 1 tile

EnemyControllerComponent::EnemyControllerComponent(Tara::EntityNoRef parent, const std::string& name, PawnEntityNoRef player)
	: Tara::Component(parent, name), m_Player(player), m_LastDir(Tara::Vector(0,0,0))
{}

void EnemyControllerComponent::OnUpdate(float deltaTime)
{
	auto parent = std::dynamic_pointer_cast<PawnEntity>(GetParent().lock()); //OK because will be cleaned up and end of func call
	if (parent && !parent->GetTraveling()) {
		//choose a new destination
		auto roomCoords = RoomManager::WorldCoordToRoomCoord(glm::vec2(parent->GetWorldPosition()));
		auto room = RoomManager::Get()->GetRoom(roomCoords.x, roomCoords.y);
		auto doorState = room->GetDoorState();
		Tara::Vector dir = GetAStarDirection(roomCoords.x, roomCoords.y);
		//LOG_S(INFO) << dir.x << ", " << dir.y;
		if (dir.x != 0 || dir.y != 0) {
			dir *= MOVEMENT_DISTANCE * 9; //move 9 tiles, room center to room center
			parent->SetTarget(parent->GetWorldPosition() + dir, 0.25 * 9);
		}
		if (abs(dir.x) > 0.5f || abs(dir.y) > 0.5f)
			m_LastDir = dir / (MOVEMENT_DISTANCE * 9);
	}
}

Tara::Vector EnemyControllerComponent::GetAStarDirection(int32_t x, int32_t y)
{
	auto manager = RoomManager::Get();
	int32_t width = manager->GetWidth();
	int32_t height = manager->GetHeight();
	// TODO: make these be player positions instead
	auto target = RoomManager::WorldCoordToRoomCoord(m_Player.lock()->GetWorldPosition());
	std::priority_queue<glm::vec3, std::vector<glm::vec3>, ZComparator> queue = std::priority_queue<glm::vec3, std::vector<glm::vec3>, ZComparator>();
	std::unordered_map<glm::vec2, std::list<glm::vec2>, vec_hash> visited = std::unordered_map<glm::vec2, std::list<glm::vec2>, vec_hash>();
	queue.push(glm::vec3(x, y, 0));
	visited[glm::vec2(x, y)] = std::list<glm::vec2>();
	while (true)
	{
		if (visited.count(target) != 0)
		{
			auto targetDoor = manager->GetRoom(target.x, target.y)->GetDoorState();
			if (std::bitset<8>(targetDoor).count() == 1)
			{
				// impossible to reach player; use opposite direction
				return Tara::Vector(m_LastDir.x * -1, m_LastDir.y * -1, 0);
			}
			visited[target].push_back(target); // needed since traceback doesn't include target cell
			auto iter = visited[target].begin();
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
		if (room->GetDoorState() & DOORSTATE_UP && visited.count(glm::vec2(current.x, current.y + 1)) == 0)
		{
			int32_t h = visited[current].size() + abs(current.x - target.x) + abs(current.y + 1 - target.y) + 1; // +1 to account for traceback length being one short
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
			int32_t h = visited[current].size() + abs(current.x - target.x) + abs(current.y - 1 - target.y) + 1;
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
			int32_t h = visited[current].size() + abs(current.x - 1 - target.x) + abs(current.y - target.y) + 1;
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
			int32_t h = visited[current].size() + abs(current.x + 1 - target.x) + abs(current.y - target.y) + 1;
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

