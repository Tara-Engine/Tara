
function Cast(typename, object)
	local sucess
	local result
	if (typename.__type.is(object)) then
		--print("already that!")
		return object
	end
	--the CastToX::XX functions DO NOT accept anything but Component and Entity.
	if (Component.__type.is(object)) then
		--print("fixing Component ref")
		object = Cast(Component, object)
	end
	if (Entity.__type.is(object)) then
		--print("fixing Entity ref")
		object = Cast(Entity, object)
	end
	
	sucess, result = xpcall(function() 
		local castName = "CastTo" .. typename.__type.name
		--print("Casting func: " .. castName)
		local v = _G[castName](object)
		--print(v)
		return v
	end, debug.traceback)
	
	if (sucess) then
		--print(result)
		return result
	else
		--print(result)
		--print("nil is returned")
		return nil
	end
end