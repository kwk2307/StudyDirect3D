#pragma once
struct TextureData
{
	int width = 0;
	int height = 0;
	std::vector<uint8_t> image;
	bool IsHDRI;
};

