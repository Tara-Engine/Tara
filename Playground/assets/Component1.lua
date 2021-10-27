
---[[
--local v = 0
CurrentComponent:SetOnUpdateCallbackFunction(function(deltaTime)
	--print(CurrentComponent:GetName() ..  " Update!")
	--v = v + 1
end)
--]]

CurrentComponent:ListenForEvents(true)
CurrentComponent:SetOnEventCallbackFunction(function(event)
	print(event)
	print(event.Type)
	print(event.String)
	print(event.Categories)
end)