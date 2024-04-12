#pragma once

struct Mesh {
    ComPtr<ID3D11Buffer> m_vertexBuffer;
    ComPtr<ID3D11Buffer> m_indexBuffer;

    //ComPtr<ID3D11Buffer> vertexConstantBuffer;
    //ComPtr<ID3D11Buffer> pixelConstantBuffer;
    UINT m_indexCount = 0;

    ComPtr<ID3D11Texture2D> texture;
    ComPtr<ID3D11ShaderResourceView> textureResourceView;
};