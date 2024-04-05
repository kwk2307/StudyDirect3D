#include "Precompiled.h"

#define STB_IMAGE_IMPLEMENTATION
#pragma warning( push )
#pragma warning( disable : 26451 )
#include "stb_image.h"
#pragma warning( pop )

Texture::Texture(std::string InFileName)
{
	FILE* f = NULL;
	if (0 != fopen_s(&f, InFileName.c_str(), "rb"))
	{
		return;
	}
	LoadFromFile(f);
}

Texture::Texture(std::wstring InFileName)
{
	FILE* f = NULL;
	if (0 != _wfopen_s(&f, InFileName.c_str(), L"rb"))
	{
		return;
	}
	LoadFromFile(f);
}

void Texture::Release()
{
	_Width = 0;
	_Height = 0;
	_Buffer.clear();
}

void Texture::LoadFromFile(FILE* InFilePtr)
{
	if (InFilePtr == NULL)
	{
		return;
	}

	Release();
	int width = 0, height = 0, channel = 0;
	stbi_uc* pixelsPtr = stbi_load_from_file(InFilePtr, &width, &height, &channel, STBI_rgb_alpha);
	if (pixelsPtr == NULL)
	{
		return;
	}

	_Width = static_cast<UINT32>(width);
	_Height = static_cast<UINT32>(height);
	size_t pixelNumbers = static_cast<size_t>(_Width) * static_cast<size_t>(_Height);
	_Buffer.reserve(pixelNumbers);
	for (size_t j = 0; j < _Height; j++)
	{
		for (size_t i = 0; i < _Width; i++)
		{
			size_t ix = (j * _Width + i) * 4;
			DirectX::PackedVector::XMCOLOR c(pixelsPtr[ix], pixelsPtr[ix + 1], pixelsPtr[ix + 2], pixelsPtr[ix + 3]);
			_Buffer.push_back(Color(c));
		}
	}

	return;
}