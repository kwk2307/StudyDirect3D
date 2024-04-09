#include "Precompiled.h"

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>


DXRenderer::DXRenderer() :
    _ScreenWidth(1280),
    _ScreenHeight(960), 
    _MainWindow(0),
    _ScreenViewport(D3D11_VIEWPORT()),
    _BasicPixelConstantBufferData(),
    _BasicVertexConstantBufferData()
{
}

bool DXRenderer::Init(const ScreenPoint& InSize)
{
    _MainWindow = GetActiveWindow();
    // 실패한 경우 종료
    if (_MainWindow == 0) {
        return false;
    }

    OnReSize(InSize);

    const D3D_DRIVER_TYPE driverType = D3D_DRIVER_TYPE_HARDWARE;

    UINT createDeviceFlags = 0;
#if defined(DEBUG) || defined(_DEBUG)
    createDeviceFlags |= D3D11_CREATE_DEVICE_DEBUG;
#endif

    ComPtr<ID3D11Device> device;
    ComPtr<ID3D11DeviceContext> context;

    const D3D_FEATURE_LEVEL featureLevels[2] = {
        D3D_FEATURE_LEVEL_11_0, // 더 높은 버전이 먼저 오도록 설정
        D3D_FEATURE_LEVEL_9_3 };
    D3D_FEATURE_LEVEL featureLevel;

    if (FAILED(D3D11CreateDevice(
        nullptr,    // Specify nullptr to use the default adapter.
        driverType, // Create a device using the hardware graphics driver.
        0, // Should be 0 unless the driver is D3D_DRIVER_TYPE_SOFTWARE.
        createDeviceFlags, // Set debug and Direct2D compatibility flags.
        featureLevels,     // List of feature levels this app can support.
        ARRAYSIZE(featureLevels), // Size of the list above.
        D3D11_SDK_VERSION,     // Always set this to D3D11_SDK_VERSION for
        // Microsoft Store apps.
        device.GetAddressOf(), // Returns the Direct3D device created.
        &featureLevel,         // Returns feature level of device created.
        context.GetAddressOf() // Returns the device immediate context.
    ))) {
        cout << "D3D11CreateDevice() failed." << endl;
        return false;
    }

    /* 참고: 오류가 있을 경우 예외 발생 방법

    // MS 예제
    inline void ThrowIfFailed(HRESULT hr)
    {
        if (FAILED(hr))
        {
            // Set a breakpoint on this line to catch Win32 API errors.
            throw Platform::Exception::CreateException(hr);
        }
    }

    // Luna DX12 교재
    #ifndef ThrowIfFailed
    #define ThrowIfFailed(x)                                              \
    {                                                                     \
        HRESULT hr__ = (x);                                               \
        std::wstring wfn = AnsiToWString(__FILE__);                       \
        if(FAILED(hr__)) { throw DxException(hr__, L#x, wfn, __LINE__); } \
    }
    #endif
    */

    if (featureLevel != D3D_FEATURE_LEVEL_11_0) {
        cout << "D3D Feature Level 11 unsupported." << endl;
        return false;
    }

    // 참고: Immediate vs deferred context
    // A deferred context is primarily used for multithreading and is not
    // necessary for a single-threaded application.
    // https://learn.microsoft.com/en-us/windows/win32/direct3d11/overviews-direct3d-11-devices-intro#deferred-context

    // 4X MSAA 지원하는지 확인
    // UINT numQualityLevels; // Resize 처리를 위해 멤버 변수로 변경
    device->CheckMultisampleQualityLevels(DXGI_FORMAT_R8G8B8A8_UNORM, 4,
        &_NumQualityLevels);
    if (_NumQualityLevels <= 0) {
        cout << "MSAA not supported." << endl;
    }

    // _NumQualityLevels = 0; // MSAA 강제로 끄기

    if (FAILED(device.As(&_Device))) {
        cout << "device.AS() failed." << endl;
        return false;
    }

    if (FAILED(context.As(&_Context))) {
        cout << "context.As() failed." << endl;
        return false;
    }

    DXGI_SWAP_CHAIN_DESC sd;
    ZeroMemory(&sd, sizeof(sd));
    sd.BufferDesc.Width = InSize.X;   // set the back buffer width
    sd.BufferDesc.Height = InSize.Y; // set the back buffer height
    sd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM; // use 32-bit color
    sd.BufferCount = 2;                                // Double-buffering
    sd.BufferDesc.RefreshRate.Numerator = 60;
    sd.BufferDesc.RefreshRate.Denominator = 1;
    sd.BufferUsage =
        DXGI_USAGE_RENDER_TARGET_OUTPUT; // how swap chain is to be used
    sd.OutputWindow = _MainWindow;      // the window to be used
    sd.Windowed = TRUE;                  // windowed/full-screen mode
    sd.Flags =
        DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH; // allow full-screen switching
    sd.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;
    if (_NumQualityLevels > 0) {
        sd.SampleDesc.Count = 4; // how many multisamples
        sd.SampleDesc.Quality = _NumQualityLevels - 1;
    }
    else {
        sd.SampleDesc.Count = 1; // how many multisamples
        sd.SampleDesc.Quality = 0;
    }

    if (FAILED(D3D11CreateDeviceAndSwapChain(
        0, // Default adapter
        driverType,
        0, // No software device
        createDeviceFlags, featureLevels, 1, D3D11_SDK_VERSION, &sd,
        _SwapChain.GetAddressOf(), _Device.GetAddressOf(), &featureLevel,
        _Context.GetAddressOf()))) {
        cout << "D3D11CreateDeviceAndSwapChain() failed." << endl;
        return false;
    }

    // 참고: IDXGIFactory를 이용한 CreateSwapChain()
    /*
    ComPtr<IDXGIDevice3> dxgiDevice;
    _Device.As(&dxgiDevice);

    ComPtr<IDXGIAdapter> dxgiAdapter;
    dxgiDevice->GetAdapter(&dxgiAdapter);

    ComPtr<IDXGIFactory> dxgiFactory;
    dxgiAdapter->GetParent(IID_PPV_ARGS(&dxgiFactory));

    ComPtr<IDXGISwapChain> swapChain;
    dxgiFactory->CreateSwapChain(_Device.Get(), &sd, &swapChain);

    swapChain.As(&m_swapChain);
    */

    // 참고: IDXGIFactory4를 이용한 CreateSwapChainForHwnd()
    /*
    ComPtr<IDXGIFactory4> dxgiFactory;
    dxgiAdapter->GetParent(IID_PPV_ARGS(&dxgiFactory));

    DXGI_SWAP_CHAIN_DESC1 swapChainDesc = {0};
    swapChainDesc.Width = lround(m_screenWidth); // Match the size of the
    window. swapChainDesc.Height = lround(m_screenHeight); swapChainDesc.Format
    = DXGI_FORMAT_B8G8R8A8_UNORM; // This is the most common swap chain format.
    swapChainDesc.Stereo = false;
    swapChainDesc.SampleDesc.Count = 1; // Don't use multi-sampling.
    swapChainDesc.SampleDesc.Quality = 0;
    swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
    swapChainDesc.BufferCount = 2; // Use double-buffering to minimize latency.
    swapChainDesc.SwapEffect =
        DXGI_SWAP_EFFECT_FLIP_SEQUENTIAL; // All Microsoft Store apps must use
    this SwapEffect. swapChainDesc.Flags = 0; swapChainDesc.Scaling =
    DXGI_SCALING_NONE; swapChainDesc.AlphaMode = DXGI_ALPHA_MODE_IGNORE;

    ComPtr<IDXGISwapChain1> swapChain;
    dxgiFactory->CreateSwapChainForHwnd(_Device.Get(), m_mainWindow,
    &swapChainDesc, nullptr, nullptr, swapChain.GetAddressOf());
    */

    CreateRenderTargetView();

    SetViewport();

    // Create a rasterizer state
    D3D11_RASTERIZER_DESC rastDesc;
    ZeroMemory(&rastDesc, sizeof(D3D11_RASTERIZER_DESC)); // Need this
    rastDesc.FillMode = D3D11_FILL_MODE::D3D11_FILL_SOLID;
    // rastDesc.FillMode = D3D11_FILL_MODE::D3D11_FILL_WIREFRAME;
    rastDesc.CullMode = D3D11_CULL_MODE::D3D11_CULL_NONE;
    rastDesc.FrontCounterClockwise = false;
    rastDesc.DepthClipEnable = true; // <- zNear, zFar 확인에 필요

    _Device->CreateRasterizerState(&rastDesc,
        _SolidRasterizerSate.GetAddressOf());

    rastDesc.FillMode = D3D11_FILL_MODE::D3D11_FILL_WIREFRAME;

    _Device->CreateRasterizerState(&rastDesc,
        _WireRasterizerSate.GetAddressOf());

    CreateDepthBuffer();

    // Create depth stencil state
    D3D11_DEPTH_STENCIL_DESC depthStencilDesc;
    ZeroMemory(&depthStencilDesc, sizeof(D3D11_DEPTH_STENCIL_DESC));
    depthStencilDesc.DepthEnable = true; // false
    depthStencilDesc.DepthWriteMask =
        D3D11_DEPTH_WRITE_MASK::D3D11_DEPTH_WRITE_MASK_ALL;
    depthStencilDesc.DepthFunc =
        D3D11_COMPARISON_FUNC::D3D11_COMPARISON_LESS_EQUAL;
    if (FAILED(_Device->CreateDepthStencilState(
        &depthStencilDesc, _DepthStencilState.GetAddressOf()))) {
        cout << "CreateDepthStencilState() failed." << endl;
    }
    
    _Texture = std::make_shared<Texture>();

    CreateTexture("ojwD8.jpg", _Texture.get()->_Texture2D, _Texture.get()->_TextureResourceView);

    // Texture sampler 만들기
    D3D11_SAMPLER_DESC sampDesc;
    ZeroMemory(&sampDesc, sizeof(sampDesc));
    sampDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
    sampDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
    sampDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
    sampDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
    sampDesc.ComparisonFunc = D3D11_COMPARISON_NEVER;
    sampDesc.MinLOD = 0;
    sampDesc.MaxLOD = D3D11_FLOAT32_MAX;

    // Create the Sample State
    _Device->CreateSamplerState(&sampDesc, _SamplerState.GetAddressOf());
    
    // POSITION에 float3를 보낼 경우 내부적으로 마지막에 1을 덧붙여서 float4를 만듭니다.
    // https://learn.microsoft.com/en-us/windows-hardware/drivers/display/supplying-default-values-for-texture-coordinates-in-vertex-declaration
    std::vector<D3D11_INPUT_ELEMENT_DESC> basicInputElements = {
        {"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0,
         D3D11_INPUT_PER_VERTEX_DATA, 0},
        {"NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 4 * 3,
         D3D11_INPUT_PER_VERTEX_DATA, 0},
        {"TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 4 * 3 + 4 * 3,
         D3D11_INPUT_PER_VERTEX_DATA, 0},
    };

    CreateVertexShaderAndInputLayout(
        L"BasicVertexShader.hlsl", basicInputElements, _BasicVertexShader,
        _BasicInputLayout);

    CreatePixelShader(L"BasicPixelShader.hlsl", _BasicPixelShader);

    _Initailized = true;
    return true;
}

void DXRenderer::Shutdown()
{

}

bool DXRenderer::OnReSize(const ScreenPoint& InSize)
{
    _ScreenWidth = InSize.X;
    _ScreenHeight = InSize.Y;
    _GuiWidth = 0;

    if (_SwapChain) {
        _RenderTargetView.Reset();
        _SwapChain->ResizeBuffers(0, // 현재 개수 유지
            InSize.X, // 해상도 변경
            InSize.Y,
            DXGI_FORMAT_UNKNOWN, // 현재 포맷 유지
            0);
        CreateRenderTargetView();
        CreateDepthBuffer();
        SetViewport();
    }
    return true;
}

void DXRenderer::Clear(const Color& InClearColor)
{
}

void DXRenderer::BeginFrame()
{
}

void DXRenderer::EndFrame()
{
    _SwapChain->Present(1, 0);
}



void DXRenderer::SetViewport()
{
    static int previousGuiWidth = _GuiWidth;

    if (previousGuiWidth != _GuiWidth) {

        previousGuiWidth = _GuiWidth;

        // Set the viewport
        ZeroMemory(&_ScreenViewport, sizeof(D3D11_VIEWPORT));
        _ScreenViewport.TopLeftX = float(_GuiWidth);
        _ScreenViewport.TopLeftY = 0;
        _ScreenViewport.Width = float(_ScreenWidth - _GuiWidth);
        _ScreenViewport.Height = float(_ScreenHeight);
        // m_screenViewport.Width = static_cast<float>(m_screenHeight);
        _ScreenViewport.MinDepth = 0.0f;
        _ScreenViewport.MaxDepth = 1.0f; // Note: important for depth buffering

        _Context->RSSetViewports(1, &_ScreenViewport);
    }
}

void DXRenderer::Render()
{
    SetViewport();

    float clearColor[4] = { 0.0f, 0.0f, 0.0f, 1.0f };
    _Context->ClearRenderTargetView(_RenderTargetView.Get(), clearColor);
    _Context->ClearDepthStencilView(_DepthStencilView.Get(),
        D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL,
        1.0f, 0);
    _Context->OMSetRenderTargets(1, _RenderTargetView.GetAddressOf(),
        _DepthStencilView.Get());
    _Context->OMSetDepthStencilState(_DepthStencilState.Get(), 0);
}

bool DXRenderer::CreateRenderTargetView()
{
    ComPtr<ID3D11Texture2D> backBuffer;
    _SwapChain->GetBuffer(0, IID_PPV_ARGS(backBuffer.GetAddressOf()));
    if (backBuffer) {
        _Device->CreateRenderTargetView(backBuffer.Get(), NULL,
            _RenderTargetView.GetAddressOf());
    }
    else {
        std::cout << "CreateRenderTargetView() failed." << std::endl;
        return false;
    }

    return true;
}

bool DXRenderer::CreateDepthBuffer()
{
    D3D11_TEXTURE2D_DESC depthStencilBufferDesc;
    depthStencilBufferDesc.Width = _ScreenWidth;
    depthStencilBufferDesc.Height = _ScreenHeight;
    depthStencilBufferDesc.MipLevels = 1;
    depthStencilBufferDesc.ArraySize = 1;
    depthStencilBufferDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
    if (_NumQualityLevels > 0) {
        depthStencilBufferDesc.SampleDesc.Count = 4; // how many multisamples
        depthStencilBufferDesc.SampleDesc.Quality = _NumQualityLevels - 1;
    }
    else {
        depthStencilBufferDesc.SampleDesc.Count = 1; // how many multisamples
        depthStencilBufferDesc.SampleDesc.Quality = 0;
    }
    depthStencilBufferDesc.Usage = D3D11_USAGE_DEFAULT;
    depthStencilBufferDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
    depthStencilBufferDesc.CPUAccessFlags = 0;
    depthStencilBufferDesc.MiscFlags = 0;

    if (FAILED(_Device->CreateTexture2D(
        &depthStencilBufferDesc, 0, _DepthStencilBuffer.GetAddressOf()))) {
        std::cout << "CreateTexture2D() failed." << std::endl;
    }
    if (FAILED(_Device->CreateDepthStencilView(_DepthStencilBuffer.Get(), 0,&_DepthStencilView))) {
        std::cout << "CreateDepthStencilView() failed." << std::endl;
    }
    return true;
}


void CheckResult(HRESULT hr, ID3DBlob* errorBlob) {
    if (FAILED(hr)) {
        // 파일이 없을 경우
        if ((hr & D3D11_ERROR_FILE_NOT_FOUND) != 0) {
            cout << "File not found." << endl;
        }

        // 에러 메시지가 있으면 출력
        if (errorBlob) {
            cout << "Shader compile error\n"
                << (char*)errorBlob->GetBufferPointer() << endl;
        }
    }
}

void DXRenderer::CreateVertexShaderAndInputLayout(
    const std::wstring& filename, 
    const std::vector<D3D11_INPUT_ELEMENT_DESC>& inputElements, 
    ComPtr<ID3D11VertexShader>& vertexShader, 
    ComPtr<ID3D11InputLayout>& inputLayout)
{
    ComPtr<ID3DBlob> shaderBlob;
    ComPtr<ID3DBlob> errorBlob;

    UINT compileFlags = 0;
#if defined(DEBUG) || defined(_DEBUG)
    compileFlags = D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION;
#endif

    // 쉐이더의 시작점의 이름이 "main"인 함수로 지정
    // D3D_COMPILE_STANDARD_FILE_INCLUDE 추가: 쉐이더에서 include 사용
    HRESULT hr = D3DCompileFromFile(
        filename.c_str(), 0, D3D_COMPILE_STANDARD_FILE_INCLUDE, "main",
        "vs_5_0", compileFlags, 0, &shaderBlob, &errorBlob);

    CheckResult(hr, errorBlob.Get());

    _Device->CreateVertexShader(shaderBlob->GetBufferPointer(),
        shaderBlob->GetBufferSize(), NULL,
        &vertexShader);

    _Device->CreateInputLayout(inputElements.data(),
        UINT(inputElements.size()),
        shaderBlob->GetBufferPointer(),
        shaderBlob->GetBufferSize(), &inputLayout);
}

void DXRenderer::CreatePixelShader(const std::wstring& filename, ComPtr<ID3D11PixelShader>& pixelShader)
{
    ComPtr<ID3DBlob> shaderBlob;
    ComPtr<ID3DBlob> errorBlob;

    UINT compileFlags = 0;
#if defined(DEBUG) || defined(_DEBUG)
    compileFlags = D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION;
#endif

    // 쉐이더의 시작점의 이름이 "main"인 함수로 지정
    // D3D_COMPILE_STANDARD_FILE_INCLUDE 추가: 쉐이더에서 include 사용
    HRESULT hr = D3DCompileFromFile(
        filename.c_str(), 0, D3D_COMPILE_STANDARD_FILE_INCLUDE, "main",
        "ps_5_0", compileFlags, 0, &shaderBlob, &errorBlob);

    CheckResult(hr, errorBlob.Get());

    _Device->CreatePixelShader(shaderBlob->GetBufferPointer(),
        shaderBlob->GetBufferSize(), NULL,
        &pixelShader);
}

void DXRenderer::CreateIndexBuffer(const std::vector<uint16_t>& indices, ComPtr<ID3D11Buffer>& m_indexBuffer)
{
    D3D11_BUFFER_DESC bufferDesc = {};
    bufferDesc.Usage = D3D11_USAGE_IMMUTABLE; // 초기화 후 변경X
    bufferDesc.ByteWidth = UINT(sizeof(uint16_t) * indices.size());
    bufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
    bufferDesc.CPUAccessFlags = 0; // 0 if no CPU access is necessary.
    bufferDesc.StructureByteStride = sizeof(uint16_t);

    D3D11_SUBRESOURCE_DATA indexBufferData = { 0 };
    indexBufferData.pSysMem = indices.data();
    indexBufferData.SysMemPitch = 0;
    indexBufferData.SysMemSlicePitch = 0;

    _Device->CreateBuffer(&bufferDesc, &indexBufferData,
        m_indexBuffer.GetAddressOf());
}

void DXRenderer::CreateTexture(
    const std::string filename, 
    ComPtr<ID3D11Texture2D>& texture,
    ComPtr<ID3D11ShaderResourceView>& textureResourceView)
{
    int width, height, channels;

    unsigned char* img =
        stbi_load(filename.c_str(), &width, &height, &channels, 0);

    // assert(channels == 4);

    // 4채널로 만들어서 복사
    std::vector<uint8_t> image;
    image.resize(width * height * 4);
    for (size_t i = 0; i < width * height; i++) {
        for (size_t c = 0; c < 3; c++) {
            image[4 * i + c] = img[i * channels + c];
        }
        image[4 * i + 3] = 255;
    }
    // Create texture.
    D3D11_TEXTURE2D_DESC txtDesc = {};
    txtDesc.Width = width;
    txtDesc.Height = height;
    txtDesc.MipLevels = txtDesc.ArraySize = 1;
    txtDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
    txtDesc.SampleDesc.Count = 1;
    txtDesc.Usage = D3D11_USAGE_IMMUTABLE;
    txtDesc.BindFlags = D3D11_BIND_SHADER_RESOURCE;

    // Fill in the subresource data.
    D3D11_SUBRESOURCE_DATA InitData;
    InitData.pSysMem = image.data();
    InitData.SysMemPitch = txtDesc.Width * sizeof(uint8_t) * 4;
    // InitData.SysMemSlicePitch = 0;

    _Device->CreateTexture2D(&txtDesc, &InitData, texture.GetAddressOf());
    _Device->CreateShaderResourceView(texture.Get(), nullptr,
        textureResourceView.GetAddressOf());
}

std::shared_ptr<Mesh>  DXRenderer::CreateMesh(const MeshData& InMeshData)
{
    std::shared_ptr<Mesh> mesh = std::make_shared<Mesh>();

    CreateVertexBuffer(InMeshData.GetVertices(), mesh->m_vertexBuffer);
    mesh->m_indexCount = UINT(InMeshData.GetIndices().size());
    CreateIndexBuffer(InMeshData.GetIndices(), mesh->m_indexBuffer);

    CreateConstantBuffer(_BasicVertexConstantBufferData,
        mesh->m_vertexConstantBuffer);

    CreateConstantBuffer(_BasicPixelConstantBufferData,
        mesh->m_pixelConstantBuffer);

    return mesh;
}

void DXRenderer::OnUpdateEvnet(std::shared_ptr<Mesh> InMesh, const Matrix& InTransform,const Matrix& InView,const Matrix& InProj )
{
    _BasicVertexConstantBufferData.model = InTransform;
    _BasicVertexConstantBufferData.model = _BasicVertexConstantBufferData.model.Transpose();

    _BasicVertexConstantBufferData.invTranspose = _BasicVertexConstantBufferData.model;
    _BasicVertexConstantBufferData.invTranspose.Translation(Vector3(0.0f));
    _BasicVertexConstantBufferData.invTranspose = _BasicVertexConstantBufferData.invTranspose.Transpose().Invert();

    
    //뷰
    _BasicVertexConstantBufferData.view = InView;
    
    _BasicPixelConstantBufferData.eyeWorld = Vector3::Transform(
        Vector3(0.0f), _BasicVertexConstantBufferData.view.Invert());

    _BasicVertexConstantBufferData.view = _BasicVertexConstantBufferData.view.Transpose();

    // 프로젝션
    _BasicVertexConstantBufferData.projection = InProj;
    _BasicVertexConstantBufferData.projection = _BasicVertexConstantBufferData.projection.Transpose();

    UpdateBuffer(_BasicVertexConstantBufferData,
        InMesh->m_vertexConstantBuffer);

    _BasicPixelConstantBufferData.useTexture = false;

    _BasicPixelConstantBufferData.material.diffuse = Vector3(0.7f);
    _BasicPixelConstantBufferData.material.specular = Vector3(0.2f);

    // 여러 개 조명 사용 예시
    for (int i = 0; i < MAX_LIGHTS; i++) {
        // 다른 조명 끄기
        if (i != 0) {
            _BasicPixelConstantBufferData.lights[i].strength *= 0.0f;
        }
        else {

            _BasicPixelConstantBufferData.lights[i] = _Light;
        }
    }
    UpdateBuffer(_BasicPixelConstantBufferData,
        InMesh->m_pixelConstantBuffer);
}
void DXRenderer::OnRenderEvent(std::shared_ptr<Mesh> InMesh)
{
    // 버텍스/인덱스 버퍼 설정
    UINT stride = sizeof(Vertex);
    UINT offset = 0;

    _Context->VSSetShader(_BasicVertexShader.Get(), 0, 0);
    _Context->VSSetConstantBuffers(0, 1, InMesh->m_vertexConstantBuffer.GetAddressOf());

    _Context->PSSetShaderResources(0, 1, _Texture->_TextureResourceView.GetAddressOf());
    _Context->PSSetSamplers(0, 1, _SamplerState.GetAddressOf());

    _Context->PSSetShader(_BasicPixelShader.Get(), 0, 0);
    _Context->PSSetConstantBuffers(
        0, 1, InMesh->m_pixelConstantBuffer.GetAddressOf());

    _Context->RSSetState(_SolidRasterizerSate.Get());

    _Context->IASetInputLayout(_BasicInputLayout.Get());

    _Context->IASetVertexBuffers(
        0, 1, InMesh->m_vertexBuffer.GetAddressOf(), &stride, &offset);

    _Context->IASetIndexBuffer(InMesh->m_indexBuffer.Get(),
        DXGI_FORMAT_R16_UINT, 0);

    _Context->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_LINELIST);

    _Context->DrawIndexed(InMesh->m_indexCount, 0, 0);
}