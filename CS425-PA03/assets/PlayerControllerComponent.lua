local m_SendingToNextLevel = false
CurrentComponent:ListenForEvents(true)
CurrentComponent:SetOnUpdateCallbackFunction(function()
	if (not m_SendingToNextLevel) then
		local parent = Cast(PawnEntity, CurrentComponent:GetParent())
		if (parent and not parent:GetTraveling()) then
			pos = parent:GetWorldPosition()
			roomPos = RoomManager:WorldCoordToRoomCoord(pos)
			centX, centY = RoomManager:IsCentered(pos)
			room = RoomManager:Get():GetRoom(roomPos.x, roomPos.y)
			if (room and centX and centY and room:GetPerm() == 4 --[[and PowOfTwo(room:GetDoorState())]]) then
				m_SendingToNextLevel = true
				-- regen here
			end
		end
	end
end)

CurrentComponent:SetOnEventCallbackFunction(function(event)
	if (event.Type == "KeyPress") then
		local parent = Cast(PawnEntity, CurrentComponent:GetEvent())
		if (not parent) then return false end
		pos = parent:GetWorldPosition()
		key = event.GetKey()
		dir = {0,0,0}
		roomPos = RoomManager:WorldCoordToRoomCoord(pos)
		centX, centY = RoomManager:IsCentered(pos)
		room = RoomManager:Get():GetRoom(roomPos.x, roomPosy)
		if (key == 69) then -- E
			print("CURRENT ROOM: {" .. roomPos.x .. "," .. roomPos.y .. "}")
		end
		if (key == 83 or key == 264) then -- S, down
			if (centX and (not room or not centY or room:GetDoorState() & 2)) then
				dir[2] = -1
			end
		elseif (key == 87 or key == 265) then -- W, up
			if (centX and (not room or not centY or room:GetDoorState() & 1)) then
				dir[2] = 1
			end
		elseif (key == 65 or key == 263) then -- A, left
			if (centY and (not room or not centX or room:GetDoorState() & 4)) then
				dir[1] = -1
			end
		elseif (key == 68 or key == 262) then -- D, right
			if (centY and (not room or not centX or room:GetDoorState() & 8)) then
				dir[1] = 1
			end
		else return false end
		if (dir[1] == 0 and dir[2] == 0) then return true end
		dir = dir * 64 * 4.5
		parent:SetTarget(parent:GetWorldPosition() + dir, 0.25 * 4.5)
	end
end)