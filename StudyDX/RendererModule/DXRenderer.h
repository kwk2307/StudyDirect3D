#pragma once

struct BasicVertexConstantBuffer {
    Matrix model;
    Matrix invTranspose;
    Matrix view;
    Matrix projection;
};

static_assert((sizeof(BasicVertexConstantBuffer) % 16) == 0,
    "Constant Buffer size must be 16-byte aligned");

#define MAX_LIGHTS 3

struct BasicPixelConstantBuffer {
    Vector3 eyeWorld;         // 12
    bool useTexture;          // 4
    //Material material;        // 48
    //Light lights[MAX_LIGHTS]; // 48 * MAX_LIGHTS
};

static_assert((sizeof(BasicPixelConstantBuffer) % 16) == 0,
    "Constant Buffer size must be 16-byte aligned");

class DXRenderer : public RendererInterface
{
public:
	virtual bool Init(const ScreenPoint& InSize) override;
    virtual void Shutdown()override;
    virtual bool IsInitialized() const override { return _Initailized; }

    virtual bool OnReSize(const ScreenPoint& InSize) override;

	virtual void Clear(const Color& InClearColor) override;
	virtual void BeginFrame() override;
	virtual void EndFrame() override;

    virtual std::shared_ptr<Mesh> CreateMesh(const MeshData& InMeshData) override;

    virtual void OnUpdateEvnet(std::shared_ptr<Mesh> InMesh, const Matrix& InTransform, const Matrix& InView, const Matrix& InProj) override;
    virtual void OnRenderEvent(std::shared_ptr<Mesh> InMesh) override;

    virtual void Render() override;
private:
    void SetViewport();
    bool CreateRenderTargetView();
    bool CreateDepthBuffer();
    void CreateVertexShaderAndInputLayout(
        const std::wstring& filename,
        const std::vector<D3D11_INPUT_ELEMENT_DESC>& inputElements,
        ComPtr<ID3D11VertexShader>& vertexShader,
        ComPtr<ID3D11InputLayout>& inputLayout);
    void CreatePixelShader(const std::wstring& filename,
        ComPtr<ID3D11PixelShader>& pixelShader);
    void CreateIndexBuffer(const std::vector<uint16_t>& indices,
        ComPtr<ID3D11Buffer>& m_indexBuffer);

    template <typename T_VERTEX>
    void CreateVertexBuffer(const std::vector<T_VERTEX>& vertices,
        ComPtr<ID3D11Buffer>& vertexBuffer) {

        // D3D11_USAGE enumeration (d3d11.h)
        // https://learn.microsoft.com/en-us/windows/win32/api/d3d11/ne-d3d11-d3d11_usage

        D3D11_BUFFER_DESC bufferDesc;
        ZeroMemory(&bufferDesc, sizeof(bufferDesc));
        bufferDesc.Usage = D3D11_USAGE_IMMUTABLE; // 초기화 후 변경X
        bufferDesc.ByteWidth = UINT(sizeof(T_VERTEX) * vertices.size());
        bufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
        bufferDesc.CPUAccessFlags = 0; // 0 if no CPU access is necessary.
        bufferDesc.StructureByteStride = sizeof(T_VERTEX);

        D3D11_SUBRESOURCE_DATA vertexBufferData = {
            0 }; // MS 예제에서 초기화하는 방식
        vertexBufferData.pSysMem = vertices.data();
        vertexBufferData.SysMemPitch = 0;
        vertexBufferData.SysMemSlicePitch = 0;

        const HRESULT hr = _Device->CreateBuffer(
            &bufferDesc, &vertexBufferData, vertexBuffer.GetAddressOf());
        if (FAILED(hr)) {
            std::cout << "CreateBuffer() failed. " << std::hex << hr
                << std::endl;
        };
    }

    template <typename T_CONSTANT>
    void CreateConstantBuffer(const T_CONSTANT& constantBufferData,
        ComPtr<ID3D11Buffer>& constantBuffer) {
        // 주의:
        // For a constant buffer (BindFlags of D3D11_BUFFER_DESC set to
        // D3D11_BIND_CONSTANT_BUFFER), you must set the ByteWidth value of
        // D3D11_BUFFER_DESC in multiples of 16, and less than or equal to
        // D3D11_REQ_CONSTANT_BUFFER_ELEMENT_COUNT.

        D3D11_BUFFER_DESC cbDesc;
        cbDesc.ByteWidth = sizeof(constantBufferData);
        cbDesc.Usage = D3D11_USAGE_DYNAMIC;
        cbDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
        cbDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
        cbDesc.MiscFlags = 0;
        cbDesc.StructureByteStride = 0;

        // Fill in the subresource data.
        D3D11_SUBRESOURCE_DATA initData;
        initData.pSysMem = &constantBufferData;
        initData.SysMemPitch = 0;
        initData.SysMemSlicePitch = 0;

        auto hr = _Device->CreateBuffer(&cbDesc, &initData,
            constantBuffer.GetAddressOf());
        if (FAILED(hr)) {
            std::cout << "CreateConstantBuffer() CreateBuffer failed()."
                << std::endl;
        }
    }

    template <typename T_DATA>
    void UpdateBuffer(const T_DATA& bufferData, ComPtr<ID3D11Buffer>& buffer) {

        if (!buffer) {
            std::cout << "UpdateBuffer() buffer was not initialized."
                << std::endl;
        }

        D3D11_MAPPED_SUBRESOURCE ms;
        _Context->Map(buffer.Get(), NULL, D3D11_MAP_WRITE_DISCARD, NULL, &ms);
        memcpy(ms.pData, &bufferData, sizeof(bufferData));
        _Context->Unmap(buffer.Get(), NULL);
    }

    void CreateTexture(
        const std::string filename,
        ComPtr<ID3D11Texture2D>& texture,
        ComPtr<ID3D11ShaderResourceView>& textureResourceView);

private:
    bool _Initailized =false;

    int _ScreenWidth; // 렌더링할 최종 화면의 해상도
    int _ScreenHeight;
    int _GuiWidth = 0;
    HWND _MainWindow;
    UINT _NumQualityLevels = 0;

    ComPtr<ID3D11VertexShader> _BasicVertexShader;
    ComPtr<ID3D11PixelShader> _BasicPixelShader;
    ComPtr<ID3D11InputLayout> _BasicInputLayout;

    std::shared_ptr<Texture> _Texture;
    ComPtr<ID3D11SamplerState> _SamplerState;

    BasicVertexConstantBuffer _BasicVertexConstantBufferData;
    BasicPixelConstantBuffer _BasicPixelConstantBufferData;

    ComPtr<ID3D11Device> _Device;
    ComPtr<ID3D11DeviceContext> _Context;
    ComPtr<ID3D11RenderTargetView> _RenderTargetView;
    ComPtr<IDXGISwapChain> _SwapChain;

    ComPtr<ID3D11RasterizerState> _SolidRasterizerSate;
    ComPtr<ID3D11RasterizerState> _WireRasterizerSate;
    bool m_DrawAsWire = false;

    // Depth buffer 관련
    ComPtr<ID3D11Texture2D> _DepthStencilBuffer;
    ComPtr<ID3D11DepthStencilView> _DepthStencilView;
    ComPtr<ID3D11DepthStencilState> _DepthStencilState;

    D3D11_VIEWPORT _ScreenViewport;
};

