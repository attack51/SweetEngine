#pragma once

//용도: 모든 .h 에서 공통적으로 사용하는 헤더들 모음. 이 파일내에서는 include만 할것
//접근: SHeader.h, SVkHeader.h

//C++ Include
#include <stdint.h>

//General Include
#include "General/SString.h"
#include "General/SDefines.h"
#include "General/SBuildOptions.h"
#include "General/Math/SSkin.h"
#include "General/Math/SVector.h"
#include "General/Math/SMatrix3x3.h"
#include "General/Math/SMatrix4x3.h"
#include "General/Math/SMatrix.h"
#include "General/Math/SQuaternion.h"
#include "General/Math/SEulerRotator.h"
#include "General/Math/STransform.h"
//#include "General/Math/SHierachyTransform.h"//아직 미완성. include하면 에러남
#include "General/Math/SPlane.h"
#include "General/Math/SMath.h"
#include "General/RenderState/SBlendState.h"
//#include "General/Asset/SAssetManager.h"
#include "General/Asset/SAsset.h"
#include "General/Entity/SEntity.h"
#include "General/Input/SInputState.h"

using namespace std;
