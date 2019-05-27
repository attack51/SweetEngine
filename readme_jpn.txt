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
100人のスキニングキャラクターアニメーション処理は、compute shaderを利用してGPUで急速に処理され、
レンダリングもInstancingを利用して同じMaterialのMesh達を一度のDraw Callで描かれています。
100人のアニメーションはフレームに一度のDispatch Callで処理されています。
キャラクターのbone数は131個でありvertexは8000以上です。

2)アニメーション処理されている100人のキャラクターにObject Motion Blurが適用されています。
compute shaderでスキニングキャラクターアニメーションする時、以前フレームのvertexを保管して、
今のvertexと以前のvertexの位置を比較してVelocityを計算します。
geometry shaderではVelocityの方向でMeshを拡張して、
fragment shaderではVelocityの方向でBlurの処理をします。

デモは独自で作成されているゲームフレームワークを元で作りました。
ローレベルグラフィックスAPIであるVulkanを使用しています。

第2世代のi7 2600 + GTX1070のシステムで
MotionBlurが適用された時180フレーム以上、
MotionBlurが適用されなかった時290フレーム以上で動作します。
殆どがGPUの負荷ですが、まだ最適化が終わった状態ではありません。
負荷の理由は全般的な処理のプロセスが順序の依存があるからだと思っています。
各CommandBufferのsubmitはsemaphoreを利用して順次的な処理を保障されているので、
GPUのidle timeが起こすと思います。
依存関係の処理をする時に、他の(依存関係がない)GPUの処理をするのも最適化の方法だと思います。


<処理プロセス>
A.キャラクター達をupdateする時、アニメーションMatrix Paletteを計算(CPU)。
B.全てのキャラクターのMatrix Palette(100個)を合わせてGPUのStorageBufferにコピー。
C.Compute shaderでAnimation Matrix Paletteを利用して全てのキャラクターのvertexを計算。
  計算されたvertexはAnimated Vertex Storage Bufferに保管。
  以前のフレームに計算されたvertexは上書きさせない。(MotionBlurをために)
D.Geometry Render Target(GeoRT)にキャラクターを描く。
  アニメーションされたvertexはAnimated Vertex Storage Bufferから読み出す。
  ただし、UVはアニメーション処理が必要ないのでvertex bufferから読み出す。
E.キャラクターが描いているGeoRTをPostProcess Render Target(PPRT)にコピー。
F.MotionBlurをため、PPRTにキャラクター達を描く。
  Vertex shaderで以前フレームと今のvertexを読み出してVelocityを計算。
  計算されたVelocityはClipとNDC座標系で変換。
  ClipVelはgeometry shaderでMeshを拡張する時、
  NDCVelはfragment shaderでMotionBlurする時使用。
G.PPRTをPresentSurfaceにコピー。


<使用されているリソース>
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
