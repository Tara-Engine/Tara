
local function func1()
	print(CurrentComponent:GetName() ..  " Update (2)!")
end

local function func2()
	func1()
end

local function func3()
	func2()
end

local function func4()
	func3()
end

CurrentComponent:SetOnUpdateCallbackFunction(function(deltaTime)
	--func4() 
end)