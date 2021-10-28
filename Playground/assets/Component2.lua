
CurrentComponent:ListenForEvents(true)
CurrentComponent:SetOnEventCallbackFunction(function(event)
	--print(event.Type)
	if (event.Type == "MouseButtonPress") then
		local parent = Cast(SpriteEntity, CurrentComponent:GetParent())
		if (parent) then
			print("Tinting!")
			color = {1,1,1,1}
			color[1] = math.random()
			color[2] = math.random()
			color[3] = math.random()
			parent:SetTint(color)
			
			--attempt an upcast
			local pawnRef = Cast(PawnEntity, parent)
			if (pawnRef) then
				print("Success!")
				print(inspect(getmetatable(pawnRef).__type.name))
			else
				print("Problem!")
			end
			
		else
			print("Parent is not a Sprite! Parent:" .. CurrentComponent:GetParent():GetName())
		end
	end
end)