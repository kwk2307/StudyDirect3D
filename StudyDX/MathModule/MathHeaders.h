#pragma once
typedef unsigned char		BYTE;		// 부호 없는 8비트
typedef unsigned int		UINT32;		// 부호 없는 32비트
typedef signed long long	INT64;	// 부호 있는 64비트.

#define SMALL_NUMBER		(1.e-8f)

#include <cassert>

#include <math.h>
#include <intrin.h>

#include <string>
#include <vector>
#include <array>
#include <iterator>
#include <algorithm>

#include <directxtk/SimpleMath.h>
using namespace DirectX::SimpleMath;

#include "MathUtil.h"
#include "ScreenPoint.h"
#include "Rotator.h"
#include "Transform.h"


