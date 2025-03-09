# Particle System Assignment (Assignment 3)
## 1. Introduction

### Overview
- This project implments a way to render several Particles at the same time, which is also data driven for ease of use
- You are able to make any effect you wish if your creative enough with just the XML

### Objectives
- Demonstrate an understanding of particle systems.
- Implement a system where effects, emitters, and affectors are defined via XML.
- Create a flexible, reusable framework for various particle effects.

## 2. Assignment Requirements

### Effect Class
- **Purpose** Act as a container for the particle emitters.
- **Features:**
    - Contains a list of emitters.
    - Can receieve a parent Transform (pos, scale, rotation) that applies to all contained children (emitters)
    - Collects all the particles from the emitters, sorts em from front to back, Renders in a single draw call
- **XML Example:**
    ```xml
    <effect name="effectName" position="x,y,z" scale="x,y,z" rotation="x,y,z" texture="Full_Path_to_Texture">
        <emitter file="Full_File_Path_to_Emitter" offset="x,y,z"/>   
        <emitter file="Full_File_Path_to_Emitter" offset="x,y,z"/>   
        ...
    </effect>
    ```


### Emitter Class (core)
- **Purpose** Responsible for particle Management.
- **Features:**
    - Manages a constant Pool of particles
    - Spawns particles with initial properties (color, size, velocity, lifetime, fade, etc.).
    - Supports both continuous and burst emission modes.
    - If there isn't a particle to Spawn we recycle is by grabbing the LRU Particle
    - Uses Linked List For the Active Pool, and Free Pool

- **XML Example:**
    - Go Check out the bottom of the README for all possible affectors definitions

### PointBB Class (particle)
- **Purpose** Holds the data for each particle
- **Features**
    - Has Velcity and changes it's position based on that

### Controls
- W, A, S, D: Move the camera forward, left, backward, and right.
- Q, E: Move the camera up or down.
- I: Invert the camera
- R: Reloads the particle Effect
- Space: Switches to another particle effectd

### Sample Demo
- There is 4 Samples
- Stress Test: Spawns a bunch of particles in burst (How to use: adjust the amount of particles to burst)
- Fire: Has 3 emitters For the Fire, FireBase, Smoke. 
- Lighting: Lights up the sky with some stars and flashing lighting.
- Ben's World: Planned to make a world with a bunch of effects for different textures but quit and just combined my result

### Build and Run
- DownLoad the files
- Cmake to generate the build files
- Run the File

### Known Issue 
 - We are massively CPU Bound: Due to Sorting the Pixels and Passing PointBBs to Point For I think no reason (I didn't look into it)
 - Random Crashing: There are times when we reload the file it will crash the program. I have no clue how to reproduce this therefore can't fix it. (Prop memory issue)
 - Textures: Emitters can only have 1 texture Best thing to do is create a texture Atlas to avoid this
 - Code Quality: The code organization could be improved; apologies for the messy sections due to time constraints.
 - Emitter Positions: The Offset is Off due how the emitter is made (It's not center with the object its 0, 0, 0 Not 0.5, 0.5, 0.5).
 
### Conclusion
 - I'm not proud of the performance at all
 - This system isn't that great for creating particles with XML
 - The reloading XML is bad (Pressing "R"). All we are donig is deleting the particles and creating em again
 - But It's fun working on larger projects It feels like im learning alot about creating systems from scratch.


# Emitter XML (ChatGPT Generated)
- type = CONTINUOUS, BURST
- spawn_property = type = random | constant. They values it wants are dependant on that
```xml

<emitter name="emitterName" num_particles="1000" emitter_size="x,y,z" duration="-1" type="continuous" birthrate="100" burst_count="0" burst_interval="0.0">
    <!-- Spawn Properties -->
    <spawn_property name="velocity" type="random">
        <property name="min" value="-10.0,-10.0,-10.0" />
        <property name="max" value="10.0,10.0,10.0" />
    </spawn_property>
    <spawn_property name="size" type="random">
        <property name="min" value="0.1" />
        <property name="max" value="1.0" />
    </spawn_property>
    <spawn_property name="lifeTime" type="constant">
        <property name="value" value="5.0" />
    </spawn_property>
    <spawn_property name="color" type="random">
        <property name="min" value="0.0,0.0,0.0" />
        <property name="max" value="1.0,1.0,1.0" />
    </spawn_property>

    <!-- Affectors -->
    <affector type="scale">
        <property name="mode" value="Breath" />
        <property name="start" value="1.0" />
        <property name="end" value="0.1" />
    </affector>
    <affector type="color">
        <property name="mode" value="OverLife" />
        <property name="start" value="1.0,0.0,0.0" />
        <property name="end" value="0.0,0.0,1.0" />
    </affector>
    <affector type="gravity">
        <property name="strength" value="9.81" />
    </affector>
    <affector type="fade">
        <property name="mode" value="OverLife" />
        <property name="start" value="1.0" />
        <property name="mid" value="0.5" />
        <property name="end" value="0.0" />
    </affector>
    <affector type="turbulence">
        <property name="strength" value="5.0" />
    </affector>
    <affector type="directional">
        <property name="direction" value="1.0,0.0,0.0" />
        <property name="speed" value="2.0" />
    </affector>
</emitter>

```

# The End!