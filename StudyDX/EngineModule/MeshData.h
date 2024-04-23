#pragma once

enum class TextureType : UINT32 {
	albedo = 0,
	emissive,
	normal,
	height,
	ao,
	metallic,
	roughness,
	END,
};

struct MeshData {
    std::vector<Vertex> _Vertices;
    std::vector<uint32_t> _Indices;

	std::string _Textures[(int)TextureType::END];

};
