#pragma once
using namespace std;
class D3D11Utils
{
public:
    static void
        CreateDepthBuffer(ComPtr<ID3D11Device>& device, int screenWidth,
            int screenHeight, UINT numQualityLevels,
            ComPtr<ID3D11DepthStencilView>& depthStencilView);

    static void CreateVertexShaderAndInputLayout(
        ComPtr<ID3D11Device>& device, const wstring& filename,
        const vector<D3D11_INPUT_ELEMENT_DESC>& inputElements,
        ComPtr<ID3D11VertexShader>& m_vertexShader,
        ComPtr<ID3D11InputLayout>& m_inputLayout);

    static void CreateHullShader(ComPtr<ID3D11Device>& device,
        const wstring& filename,
        ComPtr<ID3D11HullShader>& m_hullShader);

    static void CreateDomainShader(ComPtr<ID3D11Device>& device,
        const wstring& filename,
        ComPtr<ID3D11DomainShader>& m_domainShader);

    static void
        CreateGeometryShader(ComPtr<ID3D11Device>& device, const wstring& filename,
            ComPtr<ID3D11GeometryShader>& m_geometryShader);

    static void CreatePixelShader(ComPtr<ID3D11Device>& device,
        const wstring& filename,
        ComPtr<ID3D11PixelShader>& m_pixelShader);

    static void CreateIndexBuffer(ComPtr<ID3D11Device>& device,
        const vector<uint32_t>& indices,
        ComPtr<ID3D11Buffer>& indexBuffer);

    template <typename T_VERTEX>
    static void CreateVertexBuffer(ComPtr<ID3D11Device>& device,
        const vector<T_VERTEX>& vertices,
        ComPtr<ID3D11Buffer>& vertexBuffer) {

        // D3D11_USAGE enumeration (d3d11.h)
        // https://learn.microsoft.com/en-us/windows/win32/api/d3d11/ne-d3d11-d3d11_usage

        D3D11_BUFFER_DESC bufferDesc;
        ZeroMemory(&bufferDesc, sizeof(bufferDesc));
        bufferDesc.Usage = D3D11_USAGE_IMMUTABLE; // �ʱ�ȭ �� ����X
        bufferDesc.ByteWidth = UINT(sizeof(T_VERTEX) * vertices.size());
        bufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
        bufferDesc.CPUAccessFlags = 0; // 0 if no CPU access is necessary.
        bufferDesc.StructureByteStride = sizeof(T_VERTEX);

        D3D11_SUBRESOURCE_DATA vertexBufferData = {
            0 }; // MS �������� �ʱ�ȭ�ϴ� ���
        vertexBufferData.pSysMem = vertices.data();
        vertexBufferData.SysMemPitch = 0;
        vertexBufferData.SysMemSlicePitch = 0;

        ThrowIfFailed(device->CreateBuffer(&bufferDesc, &vertexBufferData,
            vertexBuffer.GetAddressOf()));
    }

    template <typename T_CONSTANT>
    static void CreateConstBuffer(ComPtr<ID3D11Device>& device,
        const T_CONSTANT& constantBufferData,
        ComPtr<ID3D11Buffer>& constantBuffer) {

        static_assert((sizeof(T_CONSTANT) % 16) == 0,
            "Constant Buffer size must be 16-byte aligned");

        D3D11_BUFFER_DESC desc;
        ZeroMemory(&desc, sizeof(desc));
        desc.ByteWidth = sizeof(constantBufferData);
        desc.Usage = D3D11_USAGE_DYNAMIC;
        desc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
        desc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
        desc.MiscFlags = 0;
        desc.StructureByteStride = 0;

        D3D11_SUBRESOURCE_DATA initData;
        ZeroMemory(&initData, sizeof(initData));
        initData.pSysMem = &constantBufferData;
        initData.SysMemPitch = 0;
        initData.SysMemSlicePitch = 0;

        ThrowIfFailed(device->CreateBuffer(&desc, &initData,
            constantBuffer.GetAddressOf()));
    }

    template <typename T_DATA>
    static void UpdateBuffer(ComPtr<ID3D11Device>& device,
        ComPtr<ID3D11DeviceContext>& context,
        const T_DATA& bufferData,
        ComPtr<ID3D11Buffer>& buffer) {

        if (!buffer) {
            std::cout << "UpdateBuffer() buffer was not initialized."
                << std::endl;
        }

        D3D11_MAPPED_SUBRESOURCE ms;
        context->Map(buffer.Get(), NULL, D3D11_MAP_WRITE_DISCARD, NULL, &ms);
        memcpy(ms.pData, &bufferData, sizeof(bufferData));
        context->Unmap(buffer.Get(), NULL);
    }

    static void
        CreateTexture(ComPtr<ID3D11Device>& device,
            ComPtr<ID3D11DeviceContext>& context,
            const TextureData& data, const bool usSRGB,
            ComPtr<ID3D11Texture2D>& texture,
            ComPtr<ID3D11ShaderResourceView>& textureResourceView);

    static void CreateDDSTexture(ComPtr<ID3D11Device>& device,
        const wchar_t* filename, const bool isCubeMap,
        ComPtr<ID3D11ShaderResourceView>& texResView);

};

