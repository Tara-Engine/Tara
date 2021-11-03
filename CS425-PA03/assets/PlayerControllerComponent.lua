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
		