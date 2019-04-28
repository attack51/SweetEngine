simple character animation sample with Vulkan API 

==How To Build==
1)install Vulkan SDK -https://vulkan.lunarg.com/sdk/home
2)install Python 3.x -https://www.python.org/ (require for build spirv-tools)
3)cmake VulkanSDK/[version]/glslang and build debug and release
4)cmake VulkanSDK/[version]/spirv-tools and build debug and release
5)cmake SweetEngine and build debug and release
6)cmake SweetEngine/tools/FBXConv debug and release (optionable)

FBXConv is import tool from .fbx to json format mesh/skeleton/animation data
