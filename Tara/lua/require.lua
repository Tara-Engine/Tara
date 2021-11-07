--[[
Simple clone of "Require" from lua's package library, for use with Tara
For security reasons, the package library is not loaded.
However, some lua packages are still worth using, so, this exists
Not it ONLY checks in the current library directory, as defined by Tara
--]]

--make a copy
local libDir = LibraryPath

local loaded = {}

function require(modname, usename)
	if (usename == nil) then
		usename = modname
	end
	
	--try several permutations of possible locations
	local status
	local ret
	
	local msg = ""
	
	local paths = {
		modname,
		modname .. ".lua",
		modname .. "/init.lua",
		modname .. "/" .. modname .. ".lua",
	}
	
	for i, path in ipairs(paths) do
		status, ret = xpcall(function()
			return dofile(libDir .. "/" .. path)
		end, 
		function(err)
			local traceback = debug.traceback(err)
			local s, e = string.find(err, "cannot open")
			if s~=1 then
				print(err .. "\n" .. traceback)
			end
			return traceback
		end)
		
		if (status) then
			if (ret) then
				--add to global namespace
				_G[usename]  = ret
			end
			return ret --return returned value
		else
			msg = msg .. "Not found at: " ..libDir .. "/" .. path .. "\n"
		end
	end
	
	error("Could not load module: " .. modname .. "\nLocations searched:\n" .. msg)
	
end