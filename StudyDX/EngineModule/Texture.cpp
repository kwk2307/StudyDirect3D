#include "Precompiled.h"
#define STB_IMAGE_IMPLEMENTATION
#pragma warning( push )
#pragma warning( disable : 26451 )
#include "stb_image.h"
#pragma warning( pop )



Texture::Texture(std::string InFileName)
{
	std::string str = fs::current_path().parent_path().string();

	str += "\\Resource";
	str += InFileName;

	FILE* f = NULL;
	if (0 != fopen_s(&f, str.c_str(), "rb"))
	{
		return;
	}
	LoadFromFile(f);
}

Texture::Texture(std::string InFileName, std::pair<UINT32, UINT32> start, std::pair<UINT32, UINT32> size)
{
	std::string str = fs::current_path().parent_path().string();

	str += "\\Resource";
	str += InFileName;

	FILE* f = NULL;
	if (0 != fopen_s(&f, str.c_str(), "rb"))
	{
		return;
	}
	LoadFromFile(f, start, size);
}

Texture::Texture(Color InColor)
{
	_Width = 1;
	_Height = 1;

	for (size_t j = 0; j < _Height; j++)
	{
		for (size_t i = 0; i < _Width; i++)
		{
			size_t ix = (j * _Width + i) * 4;
			_Buffer.push_back(InColor);
		}
	}
}

void Texture::LoadFromFile(FILE* f)
{
	if (f == NULL)
	{
		return;
	}

	Release();
	int width = 0, height = 0, channel = 0;
	stbi_uc* pixelsPtr = stbi_load_from_file(f, &width, &height, &channel, STBI_rgb_alpha);
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
			Color c(pixelsPtr[ix], pixelsPtr[ix + 1], pixelsPtr[ix + 2], pixelsPtr[ix + 3]);
			_Buffer.push_back(c);
		}
	}
	return;
}

Color Texture::GetSample(Vector2 InUV) const
{
	if (!IsIntialized())
	{
		return Color::Error;
	}

	int x = MathUtil::FloorToInt(InUV.X * _Width) % _Width;
	int y = MathUtil::FloorToInt(InUV.Y * _Height) % _Height;


	int index = (_Width * y) + x;
	if (index >= _Buffer.size())
	{
		return Color::Error;
	}

	return _Buffer[index];
}

void Texture::LoadFromFile(FILE* f, std::pair<UINT32, UINT32>start, std::pair<UINT32, UINT32> size)
{
	if (f == NULL)
	{
		return;
	}

	Release();
	int width = 0, height = 0, channel = 0;
	stbi_uc* pixelsPtr = stbi_load_from_file(f, &width, &height, &channel, STBI_rgb_alpha);
	if (pixelsPtr == NULL)
	{
		return;
	}

	_Width = static_cast<UINT32>(size.first);
	_Height = static_cast<UINT32>(size.second);

	size_t pixelNumbers = static_cast<size_t>(_Width) * static_cast<size_t>(_Height);
	_Buffer.reserve(pixelNumbers);
	for (size_t j = 0; j < _Height; j++)
	{
		for (size_t i = 0; i < _Width; i++)
		{
			size_t ix = ((j + start.second) * width  + (i + start.first)) * 4;

			Color c(pixelsPtr[ix], pixelsPtr[ix + 1], pixelsPtr[ix + 2], pixelsPtr[ix + 3]);
			_Buffer.push_back(c);
		}
	}
	return;
}

void Texture::Release()
{
	_Width = 0;
	_Height = 0;
	_Buffer.clear();
}