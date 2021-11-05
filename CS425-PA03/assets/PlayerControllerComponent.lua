local m_SendingToNextLevel = false
CurrentComponent:ListenForEvents(true)
CurrentComponent:SetOnUpdateCallbackFunction(function()
	if (not m_SendingToNextLevel) then
		local parent = Cast(PawnEntity, CurrentComponent:GetParent())
		if (parent and not parent:GetTraveling()) then
			pos = parent:GetWorldPosition()
			roomPos = RoomManager.WorldCoordToRoomCoord(pos)
			centX, centY = RoomManager.IsCentered(pos)
			room = RoomManager.Get():GetRoom(roomPos.x, roomPos.y)
			if (room and centX and centY and room:GetPerm() == 4 --[[and PowOfTwo(room:GetDoorState())]]) then
				m_SendingToNextLevel = true
				-- regen here
			end
		end
	end
end)

CurrentComponent:SetOnEventCallbackFunction(function(event)
	if (event.Type == "KeyPress") then
		local parent = Cast(PawnEntity, CurrentComponent:GetParent())
		if (not parent) then return false end
		pos = parent:GetWorldPosition()
		key = event.Key
		dir = {x = 0, y = 0}
		roomPos = RoomManager.WorldCoordToRoomCoord(pos)
		centX, centY = RoomManager.IsCentered(pos)
		room = RoomManager.Get():GetRoom(roomPos.x, roomPos.y)
		print(centX)
		print(centY)
		if (key == TARA_KEY_E) then
			print("CURRENT ROOM: {" .. roomPos.x .. "," .. roomPos.y .. "}")
		end
		if (key == TARA_KEY_S or key == TARA_KEY_DOWN) then
			if (centX and (room == nil or not centY or (room:GetDoorState() & 2 > 0))) then
				dir.y = -1
			end
		elseif (key == TARA_KEY_W or key == TARA_KEY_UP) then
			if (centX and (room == nil or not centY or (room:GetDoorState() & 1 > 0))) then
				dir.y = 1
			end
		elseif (key == TARA_KEY_A or key == TARA_KEY_LEFT) then
			if (centY and (room == nil or not centX or (room:GetDoorState() & 4 > 0))) then
				dir.x = -1
			end
		elseif (key == TARA_KEY_S or key == TARA_KEY_RIGHT) then
			if (centY and (room == nil or not centX or (room:GetDoorState() & 8 > 0))) then
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