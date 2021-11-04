
--set up some Component member variables
local SPEED = 5

--attach a function to the update callback
--this function will run every frame
CurrentComponent:SetOnUpdateCallbackFunction(function(deltaTime)
	--get and validate parent
	local parent = CurrentComponent:GetParent();
	if (not parent) then return end
	--get input controls and offset
	local offset = {x=0,y=0}
	local input = Input.Get()
	if (input:IsKeyPressed(TARA_KEY_W)) then
		offset.y = offset.y + 1
	end
	if (input:IsKeyPressed(TARA_KEY_S)) then
		offset.y = offset.y - 1
	end
	if (input:IsKeyPressed(TARA_KEY_A)) then
		offset.x = offset.x - 1
	end
	if (input:IsKeyPressed(TARA_KEY_D)) then
		offset.x = offset.x + 1
	end
	
	--scale offset by the speed and the deltaTime (frame time)
	offset.x = offset.x * deltaTime * SPEED
	offset.y = offset.y * deltaTime * SPEED
	
	--get the parent entity position, offset it by the offset, and set it back
	local pos = parent:GetRelativePosition()
	pos.x = pos.x + offset.x
	pos.y = pos.y + offset.y
	parent:SetRelativePosition(pos)
	
end)