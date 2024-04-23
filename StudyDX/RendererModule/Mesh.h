#pragma once

struct Mesh {
    ComPtr<ID3D11Buffer> vertexBuffer;
    ComPtr<ID3D11Buffer> indexBuffer;

    ComPtr<ID3D11Buffer> vertexConstantBuffer;
    ComPtr<ID3D11Buffer> pixelConstantBuffer;

    ComPtr<ID3D11Texture2D> albedoTexture;
    ComPtr<ID3D11Texture2D> emissiveTexture; // ºûÀ» ³»´Â ÅØ½ºÃç Ãß°¡
    ComPtr<ID3D11Texture2D> normalTexture;
    ComPtr<ID3D11Texture2D> heightTexture;
    ComPtr<ID3D11Texture2D> aoTexture;
    ComPtr<ID3D11Texture2D> metallicTexture;
    ComPtr<ID3D11Texture2D> roughnessTexture;

    ComPtr<ID3D11ShaderResourceView> albedoSRV;
    ComPtr<ID3D11ShaderResourceView> emissiveSRV;
    ComPtr<ID3D11ShaderResourceView> normalSRV;
    ComPtr<ID3D11ShaderResourceView> heightSRV;
    ComPtr<ID3D11ShaderResourceView> aoSRV;
    ComPtr<ID3D11ShaderResourceView> metallicSRV;
    ComPtr<ID3D11ShaderResourceView> roughnessSRV;

    UINT m_indexCount = 0;
    UINT m_vertexCount = 0;
};