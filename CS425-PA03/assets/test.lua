
print(inspect(RoomManager))

print(RoomManager:Get())

After(function()
	print("lua after")
end, 3)