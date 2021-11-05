local m_SendingToNextLevel = false

--todo: make this part of lua library
local function PowOfTwo(val)
	return ((val == 0) or (val & (val-1)) == 0)
end

CurrentComponent:ListenForEvents(true)
CurrentComponent:SetOnUpdateCallbackFunction(function()
	if (not m_SendingToNextLevel) then
		local parent = Cast(PawnEntity, CurrentComponent:GetParent())
		if (parent and not parent:GetTraveling() and parent:GetAlive()) then
			local pos = parent:GetWorldPosition()
			local roomPos = RoomManager.WorldCoordToRoomCoord(pos)
			local centX, centY = RoomManager.IsCentered(pos)
			local room = RoomManager.Get():GetRoom(roomPos.x, roomPos.y)
			local door = room:GetDoorState()
			if (room and centX and centY and room:GetPerm() == 4) then
				if PowOfTwo(door)then
					m_SendingToNextLevel = true
					local layer = parent:GetOwningLayer()
					After(function() 
						Regenerate(layer) 
					end, 1)
				else
					--player needs to be stopped here
					parent:Kill()
				end
			end
		end
	end
end)

CurrentComponent:SetOnEventCallbackFunction(function(event)
	if (event.Type == "KeyPress") then
		local parent = Cast(PawnEntity, CurrentComponent:GetParent())
		if (not parent) then return false end
		if (parent:GetTraveling() or not parent:GetAlive()) then
			return
		end
		local pos = parent:GetWorldPosition()
		local key = event.Key
		local dir = {x = 0, y = 0}
		local roomPos = RoomManager.WorldCoordToRoomCoord(pos)
		local centX, centY = RoomManager.IsCentered(pos)
		local room = RoomManager.Get():GetRoom(roomPos.x, roomPos.y)
		if (key == TARA_KEY_E) then
			print("CURRENT ROOM: {" .. roomPos.x .. "," .. roomPos.y .. "}")
		end
		if (key == TARA_KEY_S or key == TARA_KEY_DOWN) then
			if (centX and (room == nil or not centY or (room and room:GetDoorState() & 2 > 0))) then
				dir.y = -1
			end
		elseif (key == TARA_KEY_W or key == TARA_KEY_UP) then
			if (centX and (room == nil or not centY or (room and room:GetDoorState() & 1 > 0))) then
				dir.y = 1
			end
		elseif (key == TARA_KEY_A or key == TARA_KEY_LEFT) then
			if (centY and (room == nil or not centX or (room and room:GetDoorState() & 4 > 0))) then
				dir.x = -1
			end
		elseif (key == TARA_KEY_D or key == TARA_KEY_RIGHT) then
			if (centY and (room == nil or not centX or (room and room:GetDoorState() & 8 > 0))) then
				dir.x = 1
			end
		else return false end
		if (dir[1] == 0 and dir[2] == 0) then return true end
		dir.x = dir.x * 64 * 4.5
		dir.y = dir.y * 64 * 4.5
		target = parent:GetWorldPosition()
		target.x = target.x + dir.x
		target.y = target.y + dir.y
		parent:SetTarget(target, 0.25 * 4.5)
		return true
	end
end)