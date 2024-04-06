#include "Precompiled.h"

bool DXRenderer::Init(const ScreenPoint& InSize)
{
    _MainWindow = GetActiveWindow();
    // ������ ��� ����
    if (_MainWindow == 0) {
        return false;
    }

    const D3D_DRIVER_TYPE driverType = D3D_DRIVER_TYPE_HARDWARE;

    UINT createDeviceFlags = 0;
#if defined(DEBUG) || defined(_DEBUG)
    createDeviceFlags |= D3D11_CREATE_DEVICE_DEBUG;
#endif

    ComPtr<ID3D11Device> device;
    ComPtr<ID3D11DeviceContext> context;

    const D3D_FEATURE_LEVEL featureLevels[2] = {
        D3D_FEATURE_LEVEL_11_0, // �� ���� ������ ���� ������ ����
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

    /* ����: ������ ���� ��� ���� �߻� ���

    // MS ����
    inline void ThrowIfFailed(HRESULT hr)
    {
        if (FAILED(hr))
        {
            // Set a breakpoint on this line to catch Win32 API errors.
            throw Platform::Exception::CreateException(hr);
        }
    }

    // Luna DX12 ����
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

    // ����: Immediate vs deferred context
    // A deferred context is primarily used for multithreading and is not
    // necessary for a single-threaded application.
    // https://learn.microsoft.com/en-us/windows/win32/direct3d11/overviews-direct3d-11-devices-intro#deferred-context

    // 4X MSAA �����ϴ��� Ȯ��
    // UINT numQualityLevels; // Resize ó���� ���� ��� ������ ����
    device->CheckMultisampleQualityLevels(DXGI_FORMAT_R8G8B8A8_UNORM, 4,
        &_NumQualityLevels);
    if (_NumQualityLevels <= 0) {
        cout << "MSAA not supported." << endl;
    }

    // _NumQualityLevels = 0; // MSAA ������ ����

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

    // ����: IDXGIFactory�� �̿��� CreateSwapChain()
    /*
    ComPtr<IDXGIDevice3> dxgiDevice;
    m_device.As(&dxgiDevice);

    ComPtr<IDXGIAdapter> dxgiAdapter;
    dxgiDevice->GetAdapter(&dxgiAdapter);

    ComPtr<IDXGIFactory> dxgiFactory;
    dxgiAdapter->GetParent(IID_PPV_ARGS(&dxgiFactory));

    ComPtr<IDXGISwapChain> swapChain;
    dxgiFactory->CreateSwapChain(m_device.Get(), &sd, &swapChain);

    swapChain.As(&m_swapChain);
    */

    // ����: IDXGIFactory4�� �̿��� CreateSwapChainForHwnd()
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
    dxgiFactory->CreateSwapChainForHwnd(m_device.Get(), m_mainWindow,
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
    rastDesc.DepthClipEnable = true; // <- zNear, zFar Ȯ�ο� �ʿ�

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

    return true;
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
    if (FAILED(_Device->CreateDepthStencilView(_DepthStencilBuffer.Get(), 0,
        &_DepthStencilView))) {
        std::cout << "CreateDepthStencilView() failed." << std::endl;
    }
}

void CheckResult(HRESULT hr, ID3DBlob* errorBlob) {
    if (FAILED(hr)) {
        // ������ ���� ���
        if ((hr & D3D11_ERROR_FILE_NOT_FOUND) != 0) {
            cout << "File not found." << endl;
        }

        // ���� �޽����� ������ ���
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

    // ���̴��� �������� �̸��� "main"�� �Լ��� ����
    // D3D_COMPILE_STANDARD_FILE_INCLUDE �߰�: ���̴����� include ���
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

    // ���̴��� �������� �̸��� "main"�� �Լ��� ����
    // D3D_COMPILE_STANDARD_FILE_INCLUDE �߰�: ���̴����� include ���
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
    bufferDesc.Usage = D3D11_USAGE_IMMUTABLE; // �ʱ�ȭ �� ����X
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
    const std::string filename, ComPtr<ID3D11Texture2D>& texture,
    ComPtr<ID3D11ShaderResourceView>& textureResourceView)
{
    int width, height, channels;

    unsigned char* img =
        stbi_load(filename.c_str(), &width, &height, &channels, 0);

    // assert(channels == 4);

    // 4ä�η� ���� ����
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
