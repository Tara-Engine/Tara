# Additions/Things Completed
Work done by Stephen Kelly and ____________

If you are interested in accessing the git repository, please contact Stephen Kelly.

## Stephen's Work
Note that the engine being used (Tara) was developed by Stephen, entirely since the first day of this class. Some of the major structures, like the Renderer, Event, Application, and Window classes were made before this assignment. Others, like Layer, were heavily modified during the assignment, and still others, like Entity, where created entirely during the assignment. 

* Entire Entity code (including belonging to a layer, parent-child structures, and Event listening)
* Entire Layer code, for this project specifically, everything that deals with entities. Layers exited before, but only had stub functions.
* Entity overlap detection code, via AABB testing and Manifold generation
	* Optimization 1: Using the parent-child structure of entities to exclude children if you don't overlap the wide area of the parent.
	* Optimization 2: A specific entity (`DynamicMultiChildEntity`) uses an AABB tree to optimize overlap detection among and against its children
* Simplified Quad Rendering
* Porting the Noise Implementation. As noted in the file, Noise.cpp, the actual implementation was created by Stefan Gustavson, and is available at [https://github.com/stegu/perlin-noise](https://github.com/stegu/perlin-noise). I just ported it to C++, and added seeding, controlling octaves, frequency, amplitude, and persistence.
* Particle Interactions - using the overlap detection, get all entities in a radius, check if they are another particle, and add acceleration based on distance. use deltaTime from last frame to make the acceleration and velocity scale correctly to the framerate. 

## Matthew's Work

*Detail your stuff here*