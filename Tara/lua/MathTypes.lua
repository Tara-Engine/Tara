--[[
This defines the lua-versions of math types

Vector is the parallel of Tara::Vector and contains much of the same functionality
Rotator is the parallel of Tara::Rotator and contains much of the same functionality
Transform is the parallel of Tara::Transform and contains much of the same functionality

Matrix(...) is a utility to making 3x3 matricies, as expected by the Vector.MatMul3 function.
--]]



Vector = {
	
	new = function(x, y, z)
		local v
		if (Vector.IsVector(x)) then
			--from Vector
			v= {x=x.x,y=x.y,z=x.z}
		elseif (type(x) == "table" and 
			x[1] and x[2] and x[3] and
			type(x[1]) == "number" and
			type(x[2]) == "number" and
			type(x[3]) == "number" 
		)then
			--from list
			v = {x=x[1],y=x[2],z=x[3]}
		elseif (x and not y and not z and type(x) == "number") then
			--from single number
			v = {x=x,y=x,z=x}
		elseif ( x and y and z and
			type(x) == "number" and
			type(y) == "number" and
			type(z) == "number" 
		) then
			--from many numbers
			v = {x=x,y=y,z=z}
		else
			--error("Improper fields for Vector constructor!\nX:" .. 
			--	dump(x) .. "\nY:" ..
			--	dump(y) .. "\nZ:" ..
			--	dump(z)
			--)
			return nil
		end
		setmetatable(v, Vector)
		return v
	end,
	
	IsVector = function(obj)
		if (type(obj) == "table") then
			if (obj.x and obj.y and obj.z and
				type(obj.x) == "number" and
				type(obj.y) == "number" and
				type(obj.z) == "number"
			) then
				--close enough...
				return true
			else
				return false
			end
		else
			return false
		end
	end,
	
	__tostring = function(self)
		return "{" .. self.x .. ", " .. self.y .. ", " .. self.z .. "}"
	end,
	
	__add = function(self, other)
		if Vector.IsVector(other) then
			return Vector.new(self.x+other.x, self.y+other.y, self.z+other.z)
		elseif type(other) == "number" then
			return Vector.new(self.x+other, self.y+other, self.z+other)
		else
			other = Vector.new(other)
			if (other) then
				return self + other
			else
				return nil
			end
		end
	end,
	
	__sub = function(self, other)
		if Vector.IsVector(other) then
			return Vector.new(self.x-other.x, self.y-other.y, self.z-other.z)
		elseif type(other) == "number" then
			return Vector.new(self.x-other, self.y-other, self.z-other)
		else
			other = Vector.new(other)
			if (other) then
				return self - other
			else
				return nil
			end
		end
	end,
	
	__mul = function(self, other)
		if Vector.IsVector(other) then
			return Vector.new(self.x*other.x, self.y*other.y, self.z*other.z)
		elseif type(other) == "number" then
			return Vector.new(self.x*other, self.y*other, self.z*other)
		else
			other = Vector.new(other)
			if (other) then
				return self * other
			else
				return nil
			end
		end
	end,
	
	__div = function(self, other)
		if Vector.IsVector(other) then
			return Vector.new(self.x/other.x, self.y/other.y, self.z/other.z)
		elseif type(other) == "number" then
			return Vector.new(self.x/other, self.y/other, self.z/other)
		else
			other = Vector.new(other)
			if (other) then
				return self / other
			else
				return nil
			end
		end
	end,
	
	__unm = function(self)
		return Vector.new(-self.x, -self.y, -self.z)
	end,
	
	__eq = function(self, other)
		if (Vector.IsVector(other)) then
			return self.x == other.x and self.y == other.y and self.z == other.z
		else
			other = Vector.new(other)
			if (other) then
				return self == other
			else
				return nil
			end
		end
	end,
	
	__metatable = "metaBlock!",
	
	--set this very table to be the fallback table for indexing!
	__index = function(tbl,key)
		return Vector[key]
	end,
	
	__newindex = function(self, k, v)
		print("Ignoring setting of value " .. dump(v) .. " to the key " .. dump(k) .. " in Vector!")
	end,
	
	Set = function(self, a, b, c)
		if (Vector.IsVector(a)) then
			self.x = a.x
			self.y = a.y
			self.z = a.z
			return self
		elseif (a and b and c and
				type(a) == "number" and
				type(b) == "number" and
				type(c) == "number"
		) then
			self.x = a
			self.y = b
			self.z = c
			return self
		else
			return nil
		end
	end,
	
	Dot = function(self, other)
		if (Vector.IsVector(other)) then
			return self.x*other.x + self.y*other.y + self.z * other.z
		else
			other = Vector.new(other)
			if (other) then
				return self:Dot(other)
			else
				return nil
			end
		end
	end,
	
	Cross = function(self, other)
		if (Vector.IsVector(other)) then
			return Vector.new(
				self.y * other.z - other.y * self.z,
				self.z * other.x - other.z * self.x,
				self.x * other.y - other.x * self.y
			)
		else
			other = Vector.new(other)
			if (other) then
				return self:Cross(other)
			else
				return nil
			end
		end
	end,
	
	Length = function(self)
		return math.sqrt(self.x*self.x + self.y*self.y + self.z*self.z)
	end,
	
	LengthSq = function(self)
		return (self.x * self.x) + (self.y * self.y) + (self.z * self.z)
	end,
	
	Distance = function(self, other)
		if (Vector.IsVector(other)) then
			return (self-other):Length()
		else
			other = Vector.new(other)
			if (other) then
				return self:Distance(other)
			else
				return nil
			end
		end
	end,
	
	DistanceSq = function(self, other)
		if (Vector.IsVector(other)) then
			return (self-other):LengthSq()
		else
			other = Vector.new(other)
			if (other) then
				return self:DistanceSq(other)
			else
				return nil
			end
		end
	end,
	
	Normalize = function(self)
		self:Set(self / self:Length())
		return self
	end,
	
	
	--[[
	Matrix multiplication. Assumes matrix is valid.
	Matrix is 3x3, non-homogeonus
	Matrix layout:
	{
	   {a.x, a.y, a.z},
	   {b.x, b.y, b.z}
	   {c.x, c.y, c.z}
	}
	Matrix is NOT a table of three vectors, this is invalid.
	Matrix is assumed to be a table of tables, each 1 indexed, 
		with a length of 3, all entries in inner tables being numbers
	
	--]]
	MatMul3 = function(self, matrix)
		return Vector.new(
			matrix[1][1] * self.x + matrix[1][2] * self.y + matrix[1][3] * self.z,
			matrix[2][1] * self.x + matrix[2][2] * self.y + matrix[2][3] * self.z,
			matrix[3][1] * self.x + matrix[3][2] * self.y + matrix[3][3] * self.z
		)
	end,
}

setmetatable(Vector, { __call = function(_, ...) return Vector.new(...) end })

--[[
A function to make a 3x3 matrix
THIS IS NOT A TYPE
--]]
Matrix3 = function(a,b,c, d,e,f, g,h,i)
	if (type(a) == "table" and not b) then
		if (a[1] and a[2] and a[3] and
			type(a[1]) == "table" and
			type(a[2]) == "table" and
			type(a[3]) == "table"
		) then
			--just turn to vectors 
			local v1 = Vector(a[1])
			local v2 = Vector(a[1])
			local v3 = Vector(a[1])
			if (v1 and v2 and v3) then
				--table of tables
				return {
					{v1.x, v1.y, v1.z},
					{v2.x, v2.y, v2.z},
					{v3.x, v3.y, v3.z}
				}
			else 
				--bad params, return identity
				return {
					{1, 0, 0},
					{0, 1, 0},
					{0, 0, 1}
				}
			end
		elseif (#a == 9 and type(a[1]) == "number" and
			type(a[2]) == "number" and type(a[3]) == "number" and
			type(a[4]) == "number" and type(a[5]) == "number" and
			type(a[6]) == "number" and type(a[7]) == "number" and
			type(a[8]) == "number" and type(a[9]) == "number"
		)then
			--one table of all 9 values
			return {
					{a[1], a[2], a[3]},
					{a[4], a[5], a[6]},
					{a[7], a[8], a[9]}
				}
		else
			--bad params, return identity
			return {
				{1, 0, 0},
				{0, 1, 0},
				{0, 0, 1}
			}
		end
	elseif (b and type(b) == "table" and c and type(c) == "table") then
		--just turn to vectors
		local v1 = Vector(a)
		local v2 = Vector(b)
		local v3 = Vector(c)
		if (v1 and v2 and v3) then
			--three tables/vectors
			return {
				{v1.x, v1.y, v1.z},
				{v2.x, v2.y, v2.z},
				{v3.x, v3.y, v3.z}
			}
		else 
			--bad params, return identity
			return {
				{1, 0, 0},
				{0, 1, 0},
				{0, 0, 1}
			}
		end
	elseif (type(a) == "number") then
		if (b and c and d and e and f and g and h and i and
			type(b) == "number" and type(c) == "number" and
			type(d) == "number" and type(e) == "number" and
			type(f) == "number" and type(g) == "number" and
			type(h) == "number" and type(i) == "number"
		) then
			--9 numbers
			return {
				{a, b, c},
				{d, e, f},
				{g, h, i}
			}
		else
			--single number
			return {
				{a, 0, 0},
				{0, a, 0},
				{0, 0, a}
			}
		end
	else
		--bad params, return identity
		return {
			{1, 0, 0},
			{0, 1, 0},
			{0, 0, 1}
		}
	end
end





Rotator = {
	new = function(roll, pitch, yaw)
		local r
		if (Rotator.IsRotator(roll)) then
			r = {Roll = roll.Roll, Pitch = roll.Pitch, Yaw = roll.Yaw}
		elseif (type(roll) == "table" and 
			roll[1] and roll[2] and roll[3] and
			type(roll[1]) == "number" and
			type(roll[2]) == "number" and
			type(roll[3]) == "number" 
		)then
			r = {Roll=roll[1], Pitch=roll[2], Yaw=roll[3]}
		elseif ( roll and pitch and yaw and
			type(roll) == "number" and
			type(pitch) == "number" and
			type(yaw) == "number" 
		) then
			r = {Roll = roll, Pitch = pitch, Yaw = yaw}
		else
			--error("Improper fields for Rotator constructor!")
			return nil
		end
		setmetatable(r, Rotator)
		r:Clamp()
		return r
	end,
	
	IsRotator = function(obj)
		if (type(obj) == "table") then
			if (obj.Roll and obj.Pitch and obj.Yaw and
				type(obj.Roll) == "number" and
				type(obj.Pitch) == "number" and
				type(obj.Yaw) == "number"
			) then
				--close enough...
				return true
			else
				return false
			end
		else
			return false
		end
	end,
	
	__tostring = function(self)
		return "{Roll:" .. self.Roll .. ", Pitch:" .. self.Pitch .. ", Yaw:" .. self.Yaw .. "}"
	end,
	
	__add = function(self, other)
		if Rotator.IsRotator(other) then
			return Rotator.new(self.Roll + other.Roll, self.Pitch + other.Pitch, self.Yaw + other.Yaw)
		else
			other = Rotator.new(other)
			if (other) then
				return self + other
			else
				return nil
			end
		end
	end,
	
	__sub = function(self, other)
		if Rotator.IsRotator(other) then
			return Rotator.new(self.Roll - other.Roll, self.Pitch - other.Pitch, self.Yaw - other.Yaw)
		else
			other = Rotator.new(other)
			if (other) then
				return self + other
			else
				return nil
			end
		end
	end,
	
	__mul = function(self, other)
		if type(other) == "number" then
			return Rotator.new(self.Roll * other, self.Pitch * other, self.Yaw * other)
		else
			return nil
		end
	end,
	
	__div = function(self, other)
		if type(other) == "number" then
			return Rotator.new(self.Roll / other, self.Pitch / other, self.Yaw / other)
		else
			return nil
		end
	end,
	
	__unm = function(self)
		return Rotator.new(-self.Roll, -self.Pitch, -self.Yaw)
	end,
	
	__eq = function(self, other)
		if (Rotator.IsRotator(other)) then
			return self.Roll == other.Roll and self.Pitch == other.Pitch and self.Yaw == other.Yaw
		else
			other = Rotator.new(other)
			if (other) then
				return self == other
			else
				return nil
			end
		end
	end,
	
	__metatable = "metaBlock!",
	
	--set this very table to be the fallback table for indexing!
	__index = function(tbl,key)
		return Rotator[key]
	end,
	
	__newindex = function(self, k, v)
		print("Ignoring setting of value " .. dump(v) .. " to the key " .. dump(k) .. " in Rotator!")
	end,
	
	Clamp = function(self)
		self.Roll = self.Roll % 360
		if (self.Roll > 180) then self.Roll = self.Roll - 360 end
		if (self.Roll < -180) then self.Roll = self.Roll + 360 end
		
		self.Pitch = self.Pitch % 180
		if (self.Pitch > 90) then self.Pitch = self.Pitch - 180 end
		if (self.Pitch < -90) then self.Pitch = self.Pitch + 180 end
		
		self.Yaw = self.Yaw % 360
		if (self.Yaw > 180) then self.Yaw = self.Yaw - 360 end
		if (self.Yaw < -180) then self.Yaw = self.Yaw + 360 end
	end,
	
	Set = function(self, a, b, c)
		if (Rotator.IsRotator(a)) then
			self.Roll = a.Roll
			self.Pitch = a.Pitch
			self.Yaw = a.Yaw
			self:Clamp()
			return self
		elseif (a and b and c and
				type(a) == "number" and
				type(b) == "number" and
				type(c) == "number"
		) then
			self.Roll = a
			self.Pitch = b
			self.Yaw = c
			self:Clamp()
			return self
		else
			return nil
		end
	end,
	
	Inverse = function(self)
		return -self
	end,
	
	RotateVector = function(self, vec)
		--Z, then X, then Y
		local roll = math.rad(self.Roll)
		local pitch = math.rad(self.Pitch)
		local yaw = math.rad(self.Yaw)
		
		local rotX = Matrix3( --pitch
			{1,0,0},
			{0,math.cos(pitch),-math.sin(pitch)}, 
			{0,math.sin(pitch),math.cos(pitch)} 
		)
		
		local rotY = Matrix3( --yaw
			{math.cos(yaw),0,math.sin(yaw)},
			{0,1,0}, 
			{-math.sin(yaw),0,math.cos(yaw)} 
		)
		
		local rotZ = Matrix3( --roll
			{math.cos(roll),-math.sin(roll),0}, 
			{math.sin(roll),math.cos(roll),0},
			{0,0,1}
		)
		--print("X ("..pitch.."):" .. dump(rotX))
		--print("Y ("..yaw.."):" .. dump(rotY))
		--print("Z ("..roll.."):" .. dump(rotZ))
		
		local v  = vec:MatMul3(rotZ)
		v = v:MatMul3(rotX)
		v = v:MatMul3(rotY)
		return v
	end,
}

setmetatable(Rotator, { __call = function(_, ...) return Rotator.new(...) end })




Transform = {
	new = function(pos, rot, scale)
		local vPos = Vector(pos)
		local rRot = Rotator(rot)
		local vScale = Vector(scale)
		if (vPos and rRot and vScale) then
			local t = {Position = vPos, Rotation = rRot, Scale = vScale}
			setmetatable(t, Transform)
			return t
		else
			--error("Improper fields for a Transform Constructor!\nPos: " .. 
			--	dump(pos) .. "\nRot: " ..
			--	dump(rot) .. "\nScale: " ..
			--	dump(scale)
			--)
			
			return nil
		end
	end,
	
	IsTransform = function(obj)
		if (type(obj) == "table") then
			if (obj.Position and obj.Rotation and obj.Scale and
				Vector.IsVector(obj.Position) and
				Rotator.IsRotator(obj.Rotation) and
				Vector.IsVector(obj.Scale)
			)then
				return true
			else
				return false
			end
		else
			return false
		end
	end,
	
	__tostring = function(self)
		return "{Position:" .. tostring(self.Position) .. 
			", Rotation:" .. tostring(self.Rotation) .. 
			", Scale:" .. tostring(self.Scale) .. "}"
	end,
	
	__add = function(self, other)
		if Transform.IsTransform(other) then
			return Transform.new(
				self.Position + (self.Rotation:Inverse():RotateVector(other.Position)) * self.Scale,
				self.Rotation + other.Rotation,
				self.Scale * other.Scale
			)
		else
			other = Transform.new(other)
			if (other) then
				return self + other
			else
				return nil
			end
		end
	end,
	
	__sub = function(self, other)
		if Transform.IsTransform(other) then
			return Transform.new(
				self.Position - (self.Rotation:Inverse():RotateVector(other.Position)) / self.Scale,
				self.Rotation - other.Rotation,
				self.Scale / other.Scale
			)
		else
			other = Transform.new(other)
			if (other) then
				return self - other
			else
				return nil
			end
		end
	end,
	
	__eq = function(self, other)
		if Transform.IsTransform(other) then
			return self.Position == other.Position and self.Rotation == other.Rotation and self.Scale == other.Scale
		else
			other = Transform.new(other)
			if (other) then
				return self == other
			else
				return nil
			end
		end
	end,
	
	__metatable = "metaBlock!",
	
	--set this very table to be the fallback table for indexing!
	__index = function(tbl,key)
		return Rotator[key]
	end,
	
	__newindex = function(self, k, v)
		print("Ignoring setting of value " .. dump(v) .. " to the key " .. dump(k) .. " in Transform!")
	end,
}

setmetatable(Transform, { __call = function(_, ...) return Transform.new(...) end })

