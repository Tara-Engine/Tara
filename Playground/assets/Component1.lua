
--[[
--local v = 0
CurrentComponent:SetOnUpdateCallbackFunction(function(deltaTime)
	--print(CurrentComponent:GetName() ..  " Update!")
	--v = v + 1
end)
--]]

--print(inspect(Entity))

--[[
print(Component.__type.is(CurrentComponent))

local script = Cast(ScriptComponent, CurrentComponent)
print(CurrentComponent:GetName())
if (script) then
	print("Yay!")
	print(script:GetName())
else
	print("Boooo!")
end

print("Illegal cast!")
local e = Cast(Entity, CurrentComponent)
if (e) then
	print("boo!")
else
	print("yay")
end
--]]

--[[
CurrentComponent:ListenForEvents(true)
CurrentComponent:SetOnEventCallbackFunction(function(event)
	--print(inspect(event))
	--print(event)
	--print(event.Type)
	--print(event.String)
	--print(event.Categories)
end)
--]]