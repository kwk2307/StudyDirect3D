#pragma once

#include <iostream>

#include <memory>
#include <functional>
#include <unordered_map>

#include <filesystem>
#include <DirectXMesh.h>
#include <DirectXTexEXR.h> // EXR 형식 HDRI 읽기
#include <fp16.h>

#define STB_IMAGE_IMPLEMENTATION
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image.h"
#include "stb_image_write.h"

#include "PathMng.h"
#include "InputManager.h"

#include "TransformComponent.h"

#include "TextureData.h"
#include "MeshData.h"
#include "modelLoader.h"
#include "GeometryGenerator.h"

#include "GameObject.h"
#include "CameraObject.h"

#include "EngineInterface.h"
#include "GameEngine.h"

