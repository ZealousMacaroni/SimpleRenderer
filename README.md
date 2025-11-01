# SimpleRenderer

## About
SimpleRenderer is a very simple renderer useful for 2d and 3d applications that do not require many complex features.

## What knowledge is required?
- Knowledge of C++ is required.
- Knowledge of GLSL is required
- Knowledge of OpenGL and how it works is very highly recommended as it will make many things easier.

## Dependencies
- GLM 1.01
- GLFW 3.4
- GLEW 2.2.0

## How do I build?
SimpleRenderer is header only, so it is as simple as including the files and using them. If you would like to build the examples, the steps are as follows:
- Make sure g++ is installed.
- Go to the main directory and run the build script, e.g. examples/triangle2d/build.sh. This will build an executable in the main directory.
- Simply run the "main" executable in your folder and voila!
Note: The build scripts only work on Linux and macOS, not on Windows. Use your own build system and compiler on windows.

## Plans for the future
SimpleRenderer is obviously not finished. There are a lot of todos scattered about the files, and those will be cleaned up. New features that will be added eventually include but are not limited to:
- Basic lighting
- Textures
- Greater camera configurability
- Hopefullty much more?

## Things that will not be added
- Complex PBR techniques. Does not fit the scope of this project.
- Support for other graphics APIs. This is meant to be simple, and graphics APIs such as Vulkan and DirectX do not fit in this picture.

## Thanks
This is my first big project, and if anyone even sees this that would be awesome! 
