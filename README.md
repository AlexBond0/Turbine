# Turbine

## Tools

### GLEW
The OpenGL Extention Wrangler, handles the OpenGL importing.

### GLFW
An API ho handle creation of windows to render OpenGL in, along with event handelling of key presses and resizing. Should support cross platform functionality.

### ImGui
Lightweight Debug UI which can be rendered within GLFW.

### GLM
Maths library, speciffically providing data structures and tools for 3D math manipulation. Loaded in via NuGet

## Project Setup

#### C/C++ -> General -> Additional Include Libraries
This requires references to the GLEW and GLFW include directories (for. dll amd .lib files), using the $(SolutionDir)/ reference to make paths dynamic 

#### Linker -> General -> Additional Library Directories
This requires references to the GLEW and GLFW implementation files (.h files), using the $(SolutionDir)/ reference to make paths dynamic 

#### Linker -> Input -> Additional Dependancies
This requires references to :
 - opengl32.lib
 - glew32s.lib
 - glfw3.lib
 