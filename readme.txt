<How to play>
1) go to SweetEngine/bin/release
2) execute SweetEngine 0.1.1.exe
It is built for Windows OS.

<How to control>
A: camera move left
D: camera move right
W: camera move forward
S: camera move backward
1 : game slower (minimum 0.05)
2 : game faster (maximum 1)
3 : toggle motion blur (default on)

MOUSE LB DOWN + MOUSE MOVE: look around
 
<What is this?>
1)This is a tech demo that rendering 100 characters that are animation process at high speed.
It used Vulkan, a low-level graphics API.
100 skinning character animation processes are processed quickly with gpu using a compute shader.
Rendering is also drawn using a single Draw Call for same material meshes using Instancing.
Each character has 131 number of bones and the vertex is 8000 more.

2)Object Motion Blur is applied to 100 animated characters.
When the skinning character animation is done with the compute shader, save the vertex of the previous frame,
Calculate Velocity by comparing the position of the current vertex and the previous vertex.
In the geometry shader, extend Mesh in the direction of velocity.
The fragment shader handles Blur in the direction of velocity.

The demo is based on a game framework that has been created on its own.
It uses Vulkan, a low-level graphics API.

In the 2nd gen i7 2600 + GTX 1070 system,
Over 180 frames when MotionBlur is applied,
When MotionBlur is not applied, it works with 290 frames or more.
Most are GPU bottleneck, but they are not yet optimized.
I think that the reason for the bottleneck is that the processing submit is dependent on the order.
Since submit of each CommandBuffer is guaranteed to be processed sequentially using semaphore,
I think that idle time of the GPU is caused.

The character used is unity asset store
https://assetstore.unity.com/packages/3d/characters/humanoids/eri-82607
I use fbx and png files obtained by purchasing here.
png is converted to dds file and used
fbx uses FBX SDK and converts it into json format mesh, skeleton, animation file using FbxConv made by its own.

<Processing order>
A. When updating the characters, calculate the animation Matrix Palette (CPU).
B. Combine Matrix Palette (100 pieces) of all characters and copy to GPU StorageBuffer.
C. Calculate vertex of all characters using Animation Matrix Palette with Compute shader.
  Calculated vertices are stored in Animated Vertex Storage Buffer.
  Not overwritten calculated vertertices to previous frame vertertices. (For MotionBlur)
D. Draw a character on the Geometry Render Target (GeoRT).
  An animated vertex is read from the Animated Vertex Storage Buffer.
  However, UV does not require animation processing, so it reads from static vertex buffer.
E. Copy the GeoRT (that the character is drawing) to PostProcess Render Target (PPRT).
F. Draw characters to PPRT for Motion Blur.
  Read the current & previous frame vertex with the vertex shader and calculate the velocity.
  Calculated Velocity is converted to Clip and NDC coordinate system.
  ClipVel is used when extend Mesh with geometry shader,
  NDCVel is used when MotionBlur with fragment shader.
G. Copy PPRT to PresentSurface.

<Used library>
FBXSDK: Used by FbxConv
RapidJson: json import/export
stb : used when reading png image file
gli: used when reading dds/ktx image files
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

----- UPDATE [2019.05.27] -----
Apply object motion blur

----- UPDATE [2019.05.23] -----
Increase GPU skinning performance

----- UPDATE [2019.05.21] -----
Crowd character gpu skinnging animation with instancing

----- UPDATE [2019.05.02] -----
GPU skinning with compute shader(Improved performance dramatically)
