# Tara Game Engine
The Tara Game Engine is designed to be a game engine for procedurally generated and/or experimental games. It is designed to have a simple, yet powerful API that gives several levels of access. A good example of this is the very main application loop. It is possible to let Tara handle it for you, and it does a good job of it, but it is also possible to handle it yourself, calling various functions in the Application to do so.

## Cloning
When cloning the repository, using the `--recursive` option. There are a number of submodules. A game engine is generally a pretty large project, and using libraries and tools made by other excellent people helps the process a lot!


## Compilation Guide
Tara uses [Premake](https://premake.github.io/) as its build configuration manager. Premake is similar to cmake, but much simpler and easier to use. It can generate project files for pretty much all platforms.

### Windows
Compiling on windows should be very straightforward. All that is required is some version of Visual Studios that supports C++17. For this example, VS 2019 will be assumed. If using a different version, then replace the year number in the commands with the appropriate one. Tara has not been tested on versions of Visual Studios older than 2019.

#### Step 1: Generate project files.
To generate project files, use [Premake](https://premake.github.io/).

	cd root/of/Tara
	path/to/premake/premake5.exe vs2019
	
After this, Visual Studios project files are generated.

#### Step 2: Build.
Using Visual Studios, build the project as normal. All required libraries are in the project (under the ```Vendor``` folder)
To do this from the command line (as debug): 

	msbuild Tara.sln /p:Configuration=Debug /p:Platform="x64"

### Linux
Compiling on Linux uses clang. Not that not all pushes may support Linux, or compile error-free. Building should be straightforward, as most dependencies are included. 

It should be noted that the Linux build has *not* been thoroughly tested. It assumes an OpenGL version >= 4.3. If this is not installed, it is likely to segfault when run. Yes, this is a bug. Linux is currently low-priority. 

#### Step 1: Dependancies
The dependencies are similar to those of GLFW, as GLFW is used, and compiled for the project. Check [GLFW's Page](https://www.glfw.org/docs/latest/compile_guide.html#compile_deps_x11) for more information. 

##### Ubuntu:
To install dependencies on Ubuntu, run the following:

	sudo apt install xorg-dev clang

xorg-dev is used to compile glfw, while clang is the compiler for Tara itself.


#### Step 2: Generate project files.
To generate project files, use [Premake](https://premake.github.io/).

	cd root/of/Tara
	path/to/premake/premake5 gmake2
	
After this, Makefiles for each project are generated.

#### Step 3: Build.
Using make, it is easy to build:

	make -j8


### Other Platforms
Currently, Tara only supports Windows. Support for other platforms planned, but not for a while.

## Credit for other parts
A lot of my knowledge about game engine design comes from [The Cherno's Youtube series on the topic](https://www.youtube.com/watch?v=JxIZbV_XjAs&list=PLlrATfBNZ98dC-V-N3m0Go4deliWHPFwT&index=1), and I referenced some of the code I wrote following those tutorials while making this engine. Some things, (like window abstraction) are likely to be very similar, but others, like some of how I design input and layers, will be very different.

Anything in the "Vendor" folder is third-party, and the license, if available, is in the folder of each individual part. As far as possible, these are also git submodules. The `premake5.lua` file directly in "Vendor" is used for the various projects for these submodules.

