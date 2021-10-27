--Main file to load lua libraries
--these two lines MUST come first!
print("Lua Library Loading! Path:" . LibraryPath)
dofile(LibraryPath .. "/require.lua")

--library for dumping tables to strings. Handy for debugging
require("inspect")
