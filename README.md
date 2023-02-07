# Simple RayTracing Engine

A cross-platform raytracer using cmake

Following the [Cherno's RayTracing Series](https://youtube.com/playlist?list=PLlrATfBNZ98edc5GshdBtREv5asFW3yXl) as an engine base

## Building
#### UNIX:
In the root folder run the `unixBuild.sh` script with `./scripts/unixBuild.sh` 

To open the application, run `./scripts/run.sh`

#### WINDOWS:
In Visual Studio Code chose the desired kit, `ie. Release - amd64` and build the project 

## Installation
Included in the project are the necessary submodules

Run `git clone --recursive-submodules https://github.com/DeanKJones/RayTracer.git` to fetch the submodules when cloning the project

In the `core/external/` subdirectory are the `vulkanSDK/include` files. This is because I haven't figured out how to include these without causing linker errors yet. You can delete them if they are not needed in your project folder. 

## Resources
Using [Walnut](https://github.com/TheCherno/Walnut) as an application template. 

[ImGui](https://github.com/thegeeko/imgui), [Vulkan](https://www.vulkan.org/), [GLFW](https://github.com/glfw/glfw), [GLM](https://github.com/g-truc/glm) and [stb_image](https://github.com/nothings/stb) 
