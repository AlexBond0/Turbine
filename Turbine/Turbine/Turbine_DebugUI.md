# Debugging UI with ImGui

## ImGui setup

As this project uses GLEW and GLFW to handle the behind-the-scenes OpenGL API stuff, the ImGui needs speciffic headers to work alongside these tools. they are:

~~~~
#include "ImGui//imgui.h"
#include "ImGui//imgui_impl_glfw.h"
#include "ImGui//imgui_impl_opengl3.h"
~~~~

These includes use the `ImGui//` filepath as ImGui headres are stored seperately to the Turbine code in the file structure of the project. These headers do all the hard work of initialising ImGui, provided that the GLEW and GLFW have been setup properly. Refer to the project readme for help linking the project up.

## DebugUI

The DebugUI class is a container/manager for ImGui elements in the Turbine scene. UI items are created in heap memory, and are disposed of on the deletion of the DebugUI container.

## DebugUIContainer

The container class is a purely abstract class, which UI elements can inherit from. It desxcribes render functionality, so any class can be creatued using this DebugUIContainer as a parent class, and should be able to add it to the DebugUI manager without problem.