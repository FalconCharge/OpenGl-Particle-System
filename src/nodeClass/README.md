# Octree With Frustum Culling
In this project I created a Octree that holds all the objects within the scene.
We do this to save on computation power; by reducing the amount of checks we need to do.
Instead of going over all the nodes to see if they are in the frustum we instead check if a octant is in the frustum.
If it is we than check each indiviual node to see if it's really in the frustum or not.

## Features
#### Octree
- The Octree recursively subdivides the scene in 3d space into 8 octants
- Octtree Issues: There are a couple of issues related to infinte recursion (The octree keeps dividing even though it doesn't need to). A tried to make a fix to this by adding a max Depth of 30 That number is still high though and I'm not sure what the real number should be. Probably dependant on the game.

### Frustum Culling
- We create a frustum and use it to determine if objects are visible or not
- We only render the objects that are in the view therefore reducing draw calls.
- Frustum Issues: The left and right planes seem to be messed up slightly I think this has to do with the near Plane also

### AABBs
- AABBs are a object that we use to check for collisions, and frustum culling
- We use AABBs for the Octants in the octTree and they are also attached to the DebugCubes.

## Controls

### Camera Movement
- W, A, S, D: Move the camera forward, left, backward, and right.
- Q, E: Move the camera up or down.
- I: Invert the camera

### Debugging Controls
- R: Reload the scene
- O: Toggle the OctTree Rendering
- P: Pause frustum Culling
- SPACE: Move between differnt provided envirometns
- For the DebugCube AABB's you can render them by uncommenting a line at DebugCube.cpp Line: 24
- Note: The Button system seems kinda buggy I think it's with the seed or something. Try pressing R after each SPACE.


## Problems
- The Nodes GetWorldTransform doesn't work I just don't use it
- The Octree can infinitly recurse Limits are on it though to stop this


## Header Files
- aabb.h: The AABB
- camera.h: The Camera
- debugCube.h: The DebugCube that derives from Node to be rendered
- node.h: Basically a empty gameObject
- octTree.h: The OctTree starter
- octTreeNode.h: The octants inside the OctTree
- scene.h: The Game enviroment Scene

- There is other included files but they are not involed with the project.
- Volume used to be an abstract class for Sphere and AABB but I removed that function becuase things were breaking
- Light.h is just an abstract class for the lights which are not used. Lighting is not in this

- Going by memory here: Files in wolf were slightly changed mainly for setting the mouse position and Getting pos on screen (Camera related features)

## Omitted Files when zipped

- samplefw A directory with files I use the Grid found in this file but it's currently comment out. But you need to for a ready to run
- thirdparty is used in this project


#            --------
## Ben Harper \'~~'/
#              ----