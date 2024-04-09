#pragma once

struct Texture
{
	ComPtr<ID3D11Texture2D> _Texture2D;
	ComPtr<ID3D11ShaderResourceView> _TextureResourceView;
};