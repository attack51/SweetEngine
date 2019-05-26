<実行方法>
1)go to SweetEngine_exe/bin/release
2)execute SweetEngine0.1.1.exe
Windows用にビルドされています。参考お願いします。

<操作方法>
A : camera move left
D : camera move right
W : camera move forward
S : camera move backward 
1 : game slower (minimum 0.05)
2 : game faster (maximum 1)
3 : toggle motion blur (default on)

MOUSE LB DOWN + MOUSE MOVE : look around
 
<何をする技術デモか？>
1)アニメーション処理されている100人のキャラクターを高速でレンダリングされています。
100人のスキニングキャラクターアニメーション処理は、compute shaderを利用してgpuで急速に処理され、
レンダリングもInstancingを利用して同じMaterialのMesh達を一度のDraw Callで描かれています。
100人のアニメーションはフレームに一度のDispatch Callで処理されたいます。
キャラクターのbone数は131個でありvertexは8000以上です。

2)アニメーション処理されている100人のキャラクターにObject Motion Blurが適用されています。
スキニングキャラクターアニメーションするとき、以前フレームのvertexを保管して、
今のvertexと以前のvertexの位置を比較してVelocityを計算します。
geometry shaderではVelocity方向でMeshを拡張して、
fragment shaderではVelocity方向でBlurの処理をします。

処理プロセスは


ローレベルグラフィックスAPIであるVulkanを使用しました。

第2世代のi7 2600 + GTX1070から250フレーム以上で動作します。

使用されているキャラクターは、unity asset store
https://assetstore.unity.com/packages/3d/characters/humanoids/eri-82607
ここで購入して得られたfbx、pngファイルを利用しています。
pngはddsファイルに変換して使用しており、
fbxはFBXSDKを利用して、独自の製作したFbxConvを利用して、json形式のmesh、skeleton、animationファイルに変換して使用しています。

<使用したライブラリ>
FBXSDK：FbxConvで使用
RapidJson：json import / export
stb：pngイメージファイルを読み取るときに使用
gli：dds、ktxイメージファイルを読み取るときに使用
vulkan：レンダリンググラフィックスAPI
spirv-tool：コンパイルされたシェーダファイルspvではなく、テキストベースのシェーダファイルをリアルタイムで使用

<コードをビルドする方法>
1)install Vulkan SDK -https://vulkan.lunarg.com/sdk/home
2)install Python 3.x -https://www.python.org/ (require for build spirv-tools)
3)cmake VulkanSDK/[version]/glslang and build
4)cmake VulkanSDK/[version]/spirv-tools and build
5)cmake SweetEngine_project and build
6)cmake SweetEngine_project/tools/FBXConv build  (optionable)
