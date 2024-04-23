#include "Precompiled.h"


const std::size_t GameEngine::RectangleMesh = std::hash<std::string>()("SM_Rectangle");
const std::size_t GameEngine::BoxMesh = std::hash<std::string>()("SM_Box");
const std::size_t GameEngine::SphereMesh = std::hash<std::string>() ("SM_Sphere");

struct GameObjectCompare
{
	bool operator()(const std::unique_ptr<GameObject>& lhs, std::size_t rhs)
	{
		return lhs->GetHash() < rhs;
	}
};

GameObject& GameEngine::GetGameObject(const std::string& InName)
{
	std::size_t targetHash = std::hash<std::string>()(InName);
	const auto it = std::lower_bound(SceneBegin(), SceneEnd(), targetHash, GameObjectCompare());

	return (it != _Scene.end()) ? *(*it).get() : GameObject::Invalid;
}

GameObject& GameEngine::CreateNewGameObject(const std::string& InName)
{
	std::size_t inHash = std::hash<std::string>()(InName);
	const auto it = std::lower_bound(SceneBegin(), SceneEnd(), inHash, GameObjectCompare());

	auto newGameObject = std::make_unique<GameObject>(InName);
	if (it != _Scene.end())
	{
		std::size_t targetHash = (*it)->GetHash();
		if (targetHash == inHash)
		{
			// 중복된 키 발생. 무시.
			assert(false);
			return GameObject::Invalid;
		}
		else if (targetHash < inHash)
		{
			_Scene.insert(it + 1, std::move(newGameObject));
		}
		else
		{
			_Scene.insert(it, std::move(newGameObject));
		}
	}
	else
	{
		_Scene.push_back(std::move(newGameObject));
	}

	return GetGameObject(InName);
}

std::vector<std::shared_ptr<MeshData>>& GameEngine::CreateMesh(const std::size_t& InKey, const std::vector<MeshData>& InMeshes)
{
	std::vector<std::shared_ptr<MeshData>> vec;
	for (const auto& mesh : InMeshes) {
		vec.push_back(std::make_shared<MeshData>(mesh));

		for (const auto& texture : mesh._Textures) {
			CreateTexture(std::hash<std::string>()(texture), texture);
		}
	}
	_Meshes.insert({ InKey, vec });
	
	return _Meshes.at(InKey);
}

std::vector<MeshData> GameEngine::ReadFromFile(std::string InBasePath, std::string Infilename)
{
	using namespace DirectX;

	ModelLoader modelLoader;
	modelLoader.Load(InBasePath, Infilename);
	std::vector<MeshData>& meshes = modelLoader.meshes;

	// Normalize vertices
	Vector3 vmin(1000, 1000, 1000);
	Vector3 vmax(-1000, -1000, -1000);
	for (auto& mesh : meshes) {
		for (auto& v : mesh._Vertices) {
			vmin.x = XMMin(vmin.x, v.position.x);
			vmin.y = XMMin(vmin.y, v.position.y);
			vmin.z = XMMin(vmin.z, v.position.z);
			vmax.x = XMMax(vmax.x, v.position.x);
			vmax.y = XMMax(vmax.y, v.position.y);
			vmax.z = XMMax(vmax.z, v.position.z);
		}
	}

	float dx = vmax.x - vmin.x, dy = vmax.y - vmin.y, dz = vmax.z - vmin.z;
	float dl = XMMax(XMMax(dx, dy), dz);
	float cx = (vmax.x + vmin.x) * 0.5f, cy = (vmax.y + vmin.y) * 0.5f,
		cz = (vmax.z + vmin.z) * 0.5f;

	for (auto& mesh : meshes) {
		for (auto& v : mesh._Vertices) {
			v.position.x = (v.position.x - cx) / dl;
			v.position.y = (v.position.y - cy) / dl;
			v.position.z = (v.position.z - cz) / dl;
		}
	}

	return meshes;
}

std::shared_ptr<TextureData>& GameEngine::CreateTexture(const std::size_t& InKey, const std::string& InfileName, const bool isSRGB)
{
	std::shared_ptr<TextureData> texture = std::make_shared<TextureData>();

	std::string ext(InfileName.end() - 3, InfileName.end());
	std::transform(ext.begin(), ext.end(), ext.begin(), std::tolower);

	if (ext == "exr") {
		ReadEXRImage(InfileName, texture.get()->image, texture.get()->width, texture.get()->height, texture.get()->_PixelFormat);
	}
	else {
		ReadImage(InfileName, texture.get()->image, texture.get()->width, texture.get()->height);
	}
	_Textures.insert({ InKey,texture });

	return _Textures.at(InKey);
}

void GameEngine::ReadEXRImage(const std::string filename, std::vector<uint8_t>& image, int& width, int& height, DXGI_FORMAT& pixelFormat)
{
	using namespace DirectX;
	const std::wstring wFilename(filename.begin(), filename.end());

	TexMetadata metadata;
	GetMetadataFromEXRFile(wFilename.c_str(), metadata);

	ScratchImage scratchImage;
	LoadFromEXRFile(wFilename.c_str(), NULL, scratchImage);

	width = static_cast<int>(metadata.width);
	height = static_cast<int>(metadata.height);
	pixelFormat = metadata.format;

	std::cout << filename << " " << metadata.width << " " << metadata.height << metadata.format << std::endl;

	image.resize(scratchImage.GetPixelsSize());
	memcpy(image.data(), scratchImage.GetPixels(), image.size());

	// 데이터 범위 확인해보기
	std::vector<float> f32(image.size() / 2);
	uint16_t* f16 = (uint16_t*)image.data();
	for (int i = 0; i < image.size() / 2; i++) {
		f32[i] = fp16_ieee_to_fp32_value(f16[i]);
	}

	const float minValue = *std::min_element(f32.begin(), f32.end());
	const float maxValue = *std::max_element(f32.begin(), f32.end());

	std::cout << minValue << " " << maxValue << std::endl;

	// f16 = (uint16_t *)image.data();
	// for (int i = 0; i < image.size() / 2; i++) {
	//     f16[i] = fp16_ieee_from_fp32_value(f32[i] * 2.0f);
	// }
}

void GameEngine::ReadImage(const std::string filename, std::vector<uint8_t>& image, int& width, int& height)
{
	int channels;

	unsigned char* img =
		stbi_load(filename.c_str(), &width, &height, &channels, 0);

	// assert(channels == 4);

	std::cout << filename << " " << width << " " << height << " " << channels
		<< std::endl;

	// 4채널로 만들어서 복사
	image.resize(width * height * 4);

	if (channels == 1) {
		for (size_t i = 0; i < width * height; i++) {
			uint8_t g = img[i * channels + 0];
			for (size_t c = 0; c < 4; c++) {
				image[4 * i + c] = g;
			}
		}
	}
	else if (channels == 2) {
		for (size_t i = 0; i < width * height; i++) {
			for (size_t c = 0; c < 2; c++) {
				image[4 * i + c] = img[i * channels + c];
			}
			image[4 * i + 2] = 255;
			image[4 * i + 3] = 255;
		}
	}
	else if (channels == 3) {
		for (size_t i = 0; i < width * height; i++) {
			for (size_t c = 0; c < 3; c++) {
				image[4 * i + c] = img[i * channels + c];
			}
			image[4 * i + 3] = 255;
		}
	}
	else if (channels == 4) {
		for (size_t i = 0; i < width * height; i++) {
			for (size_t c = 0; c < 4; c++) {
				image[4 * i + c] = img[i * channels + c];
			}
		}
	}
	else {
		std::cout << "Cannot read " << channels << " channels" << std::endl;
	}
}

bool GameEngine::Init()
{
	// 이미 초기화되어 있으면 초기화 진행하지 않음.
	if (_IsInitialized)
	{
		return true;
	}

	// 화면 크기가 올바로 설정되어 있는지 확인
	if (_ScreenSize.HasZero())
	{
		return false;
	}

	//if (!_InputManager.IsInputReady())
	//{
	//	return false;
	//}

	if (!LoadResources())
	{
		return false;
	}

	_IsInitialized = true;
	return _IsInitialized;
}

void GameEngine::OnScreenResize(const ScreenPoint& InScreenSize)
{
	// 화면 크기의 설정
	_ScreenSize = InScreenSize;
	_MainCamera.SetViewportSize(_ScreenSize);
}

// 여기서 필요 리소스를 로드 해준다. 
bool GameEngine::LoadResources()
{
	std::vector<MeshData> boxMesh = { GeometryGenerator::MakeBox(), };
	CreateMesh(GameEngine::BoxMesh, boxMesh);

	return true;
}