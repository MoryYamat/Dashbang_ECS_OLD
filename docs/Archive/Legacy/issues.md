# **Identifying issues in the development process and consolidating solutions**

* Challenge/Problem (Solution)
> Next issues/problems etc.

## **Project**
* Convert the local character code from `Shift-JIS` to `UTF-8` in bulk
* **Organizing the folder structure**

## **Architecture
* ~~Separation of game logic and system (engine) logic~~ (Initialization of `logical data (2D)` based on `drawing data (3D)`) (Update of `drawing data (3D)` by (updating) `logical data (2D)` based on input)
> Enhancing game editor-like functions (more intuitive and flexible)

## **ECS**
* Defining hierarchical relationships between `Entity` and implementing semantics based on names
    * Processing to limit duplicate `Entity` names
    * Name search processing -> Improved efficiency
* ** Consider a good way to manage the lifespan of `Entity` (even if you delete an `Entity`, the `Component` will remain. If there is a process that references the `Entity`, you need to manage the deletion carefully) **
* Consider restricting the acquisition and reference of non-existent `Entity` and good exception handling methods to improve safety.

## **Physical Layer**
* Physics layer (collision) implementation
    * ~~Ground implementation~~ (automatic initialization of logical tilemap by TerrainMesh)
    * ~~Obstacle implementation~~ (automatic initialization of logical tilemap Walkable attribute by static obstacle object by ObstacleActor/Component)
> Room for processing optimization and improvement
* Implement flexible and intuitive integration of drawing data and physics data
    * Consider how to match the apparent size with meaningful collision detection/action range in logical space (logic) and visual space (visual).
* When initializing logical data using drawing data, if you try to take rotation into account, there is a problem where misalignment occurs due to scale, center of rotation, and various other errors.

## **Camera**
* Separation of system camera and game camera
* Implementing a logical camera (taking pictures from directly above the XZ plane)
* The camera offset Z axis value caused the drawing to not work properly (e.g. 0).

## **input**
* Improved `Input` data/processing structure
    * Separation of `System Input` and `Game Input`
* Strict separation of key state management and side effects caused by state changes
* Improved operability (allowing players to choose what to base their character movement on and how to rotate their viewpoint)

## **RenderSystem**
* ~~Accurately extracting OpenGL buffers such as VAO and VBO from `MeshComponent` and coordinating `ModelData`~~(2025/04/26)
    * The solution is to add a for loop and handle intermediate data.
* Implementing Texture Shader and Rendering Pipeline
* Lighting shader and drawing pipeline implementation
* ~~Implementing camera movement and rotation~~(2025/04/28)
    * Figure out what to do about the one-frame delay in `RenderContext`.

## **3D Model**
* implementing Mixamo animation playback function
* Implementing a process to unify the `origin position` of 3D models
* How to `get` or `define` the `front` of a 3D model

## **Game Objects(Actor)**
* Considering the architecture for easily creating Actor object classes
* Consider how to create an `Entity` for each `Mesh` instead of creating an `Entity` for each actor and manage them
    * Give `Entity` a hierarchical structure
    * Give `Entity` a name
    * Link `Entity` and `Actor` based on `name` (proposal)
* Implemented a simplified actor creation process by templating `Entity` and `Component`



## **System**
* Implementing a Texture Importer
* Implementing load log acquisition and display functions
* ~~WindowManager implementation~~(2025/04/29)
* Implementing the engine (development) camera and the game camera, and separating them
* **Implementation of meaningful data lists and search logic (naming entities and efficiently searching by name)**/**Separation from mechanical processing**
* Implemented a process to improve the safety of vector normalization (safely normalizing 0 vectors)

## **FUTURE**
* Implementing game editing functionality via UI (engine)
* Simplify prototyping with game templates
* **Scene-oriented prototyping support engine**