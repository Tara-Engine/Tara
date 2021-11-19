CurrentComponent:ListenForEvents(true)

CurrentComponent:SetOnEventCallbackFunction(function(event) 
	local parent = Cast(TilemapEntity, CurrentComponent:GetParent())
	if (parent) then
		if (event.Type == "MouseButtonPress" and event.Button == TARA_MOUSE_BUTTON_1) then
			local x = math.random(0, 32)
			local y = math.random(0, 32)
			--parent:SwapTile(x, y, 0, 2)
		elseif (event.Type == "MouseButtonPress" and event.Button == TARA_MOUSE_BUTTON_2) then
			local x = math.random(0, 32)
			local y = math.random(0, 32)
			print(parent:GetTile(x, y, 0))
		end
	end
end)