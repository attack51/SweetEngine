<How to play>
1) go to SweetEngine/bin/release
2) execute SweetEngine 0.1.1.exe
It is built for Windows OS.

<How to control>
A: camera move left
D: camera move right
W: camera move forward
S: camera move backward

MOUSE LB DOWN + MOUSE MOVE: look around
 
<What is this?>
This is a tech demo that rendering 100 characters that are animation process at high speed.
It used Vulkan, a low-level graphics API.

100 skinning character animation processes are processed quickly with gpu using a compute shader.
Rendering is also drawn in one Draw Call using Instancing.

Each character has 131 number of bones and the vertex is 8000 more.
It works with more than 250 frames from the 2nd gen i7 2600 + GTX 1070.

The character used is unity asset store
https://assetstore.unity.com/packages/3d/characters/humanoids/eri-82607
We use fbx and png files obtained by purchasing here.
png is converted to dds file and used
fbx uses FBX SDK and converts it into json format mesh, skeleton, animation file using FbxConv made by its own.

<Used library>
FBXSDK: Used by FbxConv
RapidJson: json import/export
stb : used when reading png image file
gli: dds, used when reading ktx image files
vulkan: rendering graphics API
spirv-tool: use text based shader files in real time instead of compiled shader files spv

<How to build code>
1) install Vulkan SDK-https://vulkan.lunarg.com/sdk/home
2) install Python 3.x-https://www.python.org/ (require for build spirv-tools)
3) cmake VulkanSDK/[version]/glslang and build
4) cmake VulkanSDK/[version]/spirv-tools and build
5) cmake SweetEngine and build
6) cmake SweetEngine/tools/FBXConv build (optionable)

FBXConv is import tool from .fbx to json format mesh/skeleton/animation data

----- UPDATE [2019.05.32] -----
Crowd character gpu skinnging animation with instancing

----- UPDATE [2019.05.02] -----
GPU skinning with compute shader(Improved performance dramatically)
