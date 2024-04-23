#pragma once

#include <d3d11.h>
#include <d3dcompiler.h>

#include <imgui.h>
#include <imgui_impl_dx11.h>
#include <imgui_impl_win32.h>

#include <wrl.h> // ComPtr

#include <iostream>

using Microsoft::WRL::ComPtr;

using std::cout;
using std::endl;

#include "BasicConstantData.h"
#include "Light.h"
#include "Material.h"
#include "Mesh.h"

#include "RenderObject.h" 

#include "D3D11Utils.h"

#include "RendererInterface.h"

#include "Mesh.h"
#include "Texture.h"

#include "DXRenderer.h"

