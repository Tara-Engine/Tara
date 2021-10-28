
local function UpCast(object)
	local sucess
	local result
	sucess, result = xpcall(function() 
		local castName = "UpCast" .. getmetatable(object).__type.name
		local v = _G[castName](object)
		return v
	end, debug.traceback)
	if (sucess) then
		--print(result)
		return result
	else
		--print(result)
		return nil
	end
	
end

function Cast(typename, object)
	if (not typename or not object) then
		return nil
	end
	local sucess
	local result
	
	object = UpCast(object)
	if (object == nil) then
		--print("Nil from upcast")
		return nil
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
		print(result)
		--print("nil is returned")
		return nil
	end
end