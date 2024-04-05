#pragma once
typedef unsigned char		BYTE;		// ��ȣ ���� 8��Ʈ
typedef unsigned int		UINT32;		// ��ȣ ���� 32��Ʈ
typedef signed long long	INT64;	// ��ȣ �ִ� 64��Ʈ.

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


