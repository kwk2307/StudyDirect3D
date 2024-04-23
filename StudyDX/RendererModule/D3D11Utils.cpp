#include "Precompiled.h"
#include <directxtk/DDSTextureLoader.h> // ť��� ���� �� �ʿ�

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

void D3D11Utils::CreateDepthBuffer(ComPtr<ID3D11Device>& device, int screenWidth, int screenHeight, UINT numQualityLevels, ComPtr<ID3D11DepthStencilView>& depthStencilView)
{
	D3D11_TEXTURE2D_DESC depthStencilBufferDesc;
	depthStencilBufferDesc.Width = screenWidth;
	depthStencilBufferDesc.Height = screenHeight;
	depthStencilBufferDesc.MipLevels = 1;
	depthStencilBufferDesc.ArraySize = 1;
	depthStencilBufferDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	if (numQualityLevels > 0) {
		depthStencilBufferDesc.SampleDesc.Count = 4; // how many multisamples
		depthStencilBufferDesc.SampleDesc.Quality = numQualityLevels - 1;
	}
	else {
		depthStencilBufferDesc.SampleDesc.Count = 1; // how many multisamples
		depthStencilBufferDesc.SampleDesc.Quality = 0;
	}
	depthStencilBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	depthStencilBufferDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
	depthStencilBufferDesc.CPUAccessFlags = 0;
	depthStencilBufferDesc.MiscFlags = 0;

	ComPtr<ID3D11Texture2D> depthStencilBuffer;

	device->CreateTexture2D(&depthStencilBufferDesc, 0,
		depthStencilBuffer.GetAddressOf());

	device->CreateDepthStencilView(
		depthStencilBuffer.Get(), 0, depthStencilView.GetAddressOf());
}

void D3D11Utils::CreateVertexShaderAndInputLayout(ComPtr<ID3D11Device>& device, const wstring& filename, const vector<D3D11_INPUT_ELEMENT_DESC>& inputElements, ComPtr<ID3D11VertexShader>& m_vertexShader, ComPtr<ID3D11InputLayout>& m_inputLayout)
{
	ComPtr<ID3DBlob> shaderBlob;
	ComPtr<ID3DBlob> errorBlob;

	UINT compileFlags = 0;
#if defined(DEBUG) || defined(_DEBUG)
	compileFlags = D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION;
#endif

	HRESULT hr = D3DCompileFromFile(
		filename.c_str(), 0, D3D_COMPILE_STANDARD_FILE_INCLUDE, "main",
		"vs_5_0", compileFlags, 0, &shaderBlob, &errorBlob);

	CheckResult(hr, errorBlob.Get());

	device->CreateVertexShader(shaderBlob->GetBufferPointer(),
		shaderBlob->GetBufferSize(), NULL,
		&m_vertexShader);

	device->CreateInputLayout(inputElements.data(), UINT(inputElements.size()),
		shaderBlob->GetBufferPointer(),
		shaderBlob->GetBufferSize(), &m_inputLayout);
}

void D3D11Utils::CreateIndexBuffer(ComPtr<ID3D11Device>& device, const vector<uint32_t>& indices, ComPtr<ID3D11Buffer>& indexBuffer)
{
	D3D11_BUFFER_DESC bufferDesc = {};
	bufferDesc.Usage = D3D11_USAGE_IMMUTABLE; // �ʱ�ȭ �� ����X
	bufferDesc.ByteWidth = UINT(sizeof(uint32_t) * indices.size());
	bufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	bufferDesc.CPUAccessFlags = 0; // 0 if no CPU access is necessary.
	bufferDesc.StructureByteStride = sizeof(uint32_t);

	D3D11_SUBRESOURCE_DATA indexBufferData = { 0 };
	indexBufferData.pSysMem = indices.data();
	indexBufferData.SysMemPitch = 0;
	indexBufferData.SysMemSlicePitch = 0;

	device->CreateBuffer(&bufferDesc, &indexBufferData,
		indexBuffer.GetAddressOf());
}

void D3D11Utils::CreateHullShader(ComPtr<ID3D11Device>& device, const wstring& filename, ComPtr<ID3D11HullShader>& m_hullShader)
{

	ComPtr<ID3DBlob> shaderBlob;
	ComPtr<ID3DBlob> errorBlob;

	UINT compileFlags = 0;
#if defined(DEBUG) || defined(_DEBUG)
	compileFlags = D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION;
#endif
	HRESULT hr = D3DCompileFromFile(
		filename.c_str(), 0, D3D_COMPILE_STANDARD_FILE_INCLUDE, "main",
		"hs_5_0", compileFlags, 0, &shaderBlob, &errorBlob);

	CheckResult(hr, errorBlob.Get());

	device->CreateHullShader(shaderBlob->GetBufferPointer(),
		shaderBlob->GetBufferSize(), NULL, &m_hullShader);
}

void D3D11Utils::CreateDomainShader(ComPtr<ID3D11Device>& device, const wstring& filename, ComPtr<ID3D11DomainShader>& m_domainShader)
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
		"ds_5_0", compileFlags, 0, &shaderBlob, &errorBlob);

	CheckResult(hr, errorBlob.Get());

	device->CreateDomainShader(shaderBlob->GetBufferPointer(),
		shaderBlob->GetBufferSize(), NULL,
		&m_domainShader);
}

void D3D11Utils::CreateGeometryShader(ComPtr<ID3D11Device>& device, const wstring& filename, ComPtr<ID3D11GeometryShader>& m_geometryShader)
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
		"gs_5_0", compileFlags, 0, &shaderBlob, &errorBlob);

	// CheckResult(hr, errorBlob.Get());

	device->CreateGeometryShader(shaderBlob->GetBufferPointer(),
		shaderBlob->GetBufferSize(), NULL,
		&m_geometryShader);
}

void D3D11Utils::CreatePixelShader(ComPtr<ID3D11Device>& device, const wstring& filename, ComPtr<ID3D11PixelShader>& m_pixelShader)
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

	device->CreatePixelShader(shaderBlob->GetBufferPointer(),
		shaderBlob->GetBufferSize(), NULL,
		&m_pixelShader);
}

ComPtr<ID3D11Texture2D>
CreateStagingTexture(ComPtr<ID3D11Device>& device,
	ComPtr<ID3D11DeviceContext>& context, const int width,
	const int height, const std::vector<uint8_t>& image,
	const DXGI_FORMAT pixelFormat = DXGI_FORMAT_R8G8B8A8_UNORM,
	const int mipLevels = 1, const int arraySize = 1) {

	// ������¡ �ؽ��� �����
	D3D11_TEXTURE2D_DESC txtDesc;
	ZeroMemory(&txtDesc, sizeof(txtDesc));
	txtDesc.Width = width;
	txtDesc.Height = height;
	txtDesc.MipLevels = mipLevels;
	txtDesc.ArraySize = arraySize;
	txtDesc.Format = pixelFormat;
	txtDesc.SampleDesc.Count = 1;
	txtDesc.Usage = D3D11_USAGE_STAGING;
	txtDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE | D3D11_CPU_ACCESS_READ;

	ComPtr<ID3D11Texture2D> stagingTexture;
	if (FAILED(device->CreateTexture2D(&txtDesc, NULL,
		stagingTexture.GetAddressOf()))) {
		cout << "Failed()" << endl;
	}

	// CPU���� �̹��� ������ ����
	size_t pixelSize = sizeof(uint8_t) * 4;
	if (pixelFormat == DXGI_FORMAT_R16G16B16A16_FLOAT) {
		pixelSize = sizeof(uint16_t) * 4;
	}

	D3D11_MAPPED_SUBRESOURCE ms;
	context->Map(stagingTexture.Get(), NULL, D3D11_MAP_WRITE, NULL, &ms);
	uint8_t* pData = (uint8_t*)ms.pData;
	for (UINT h = 0; h < UINT(height); h++) { // ������ �� �پ� ����
		memcpy(&pData[h * ms.RowPitch], &image[h * width * pixelSize],
			width * pixelSize);
	}
	context->Unmap(stagingTexture.Get(), NULL);

	return stagingTexture;
}


void D3D11Utils::CreateTexture(ComPtr<ID3D11Device>& device, ComPtr<ID3D11DeviceContext>& context, const TextureData& data, const bool usSRGB, ComPtr<ID3D11Texture2D>& texture, ComPtr<ID3D11ShaderResourceView>& textureResourceView)
{
	int width = data.width, height = data.height;

	DXGI_FORMAT pixelFormat =
		usSRGB ? DXGI_FORMAT_R8G8B8A8_UNORM_SRGB : DXGI_FORMAT_R8G8B8A8_UNORM;
	  // ������¡ �ؽ��� ����� CPU���� �̹����� �����մϴ�.
    ComPtr<ID3D11Texture2D> stagingTexture = CreateStagingTexture(
        device, context, width, height, data.image, pixelFormat);

    // ������ ����� �ؽ��� ����
    D3D11_TEXTURE2D_DESC txtDesc;
    ZeroMemory(&txtDesc, sizeof(txtDesc));
    txtDesc.Width = width;
    txtDesc.Height = height;
    txtDesc.MipLevels = 0; // �Ӹ� ���� �ִ�
    txtDesc.ArraySize = 1;
    txtDesc.Format = pixelFormat;
    txtDesc.SampleDesc.Count = 1;
    txtDesc.Usage = D3D11_USAGE_DEFAULT; // ������¡ �ؽ���κ��� ���� ����
    txtDesc.BindFlags = D3D11_BIND_SHADER_RESOURCE | D3D11_BIND_RENDER_TARGET;
    txtDesc.MiscFlags = D3D11_RESOURCE_MISC_GENERATE_MIPS; // �Ӹ� ���
    txtDesc.CPUAccessFlags = 0;

    // �ʱ� ������ ���� �ؽ��� ���� (���� ������)
    device->CreateTexture2D(&txtDesc, NULL, texture.GetAddressOf());

    // ������ ������ MipLevels�� Ȯ���غ��� ���� ���
    // texture->GetDesc(&txtDesc);
    // cout << txtDesc.MipLevels << endl;

    // ������¡ �ؽ���κ��� ���� �ػ󵵰� ���� �̹��� ����
    context->CopySubresourceRegion(texture.Get(), 0, 0, 0, 0,
                                   stagingTexture.Get(), 0, NULL);

    // ResourceView �����
    device->CreateShaderResourceView(texture.Get(), 0,
                                     textureResourceView.GetAddressOf());

    // �ػ󵵸� ���簡�� �Ӹ� ����
    context->GenerateMips(textureResourceView.Get());

    // HLSL ���̴� �ȿ����� SampleLevel() ���
}


void D3D11Utils::CreateDDSTexture(ComPtr<ID3D11Device>& device, const wchar_t* filename, const bool isCubeMap, ComPtr<ID3D11ShaderResourceView>& texResView)
{

	ComPtr<ID3D11Texture2D> texture;

	UINT miscFlags = 0;
	if (isCubeMap) {
		miscFlags |= D3D11_RESOURCE_MISC_TEXTURECUBE;
	}

	// https://github.com/microsoft/DirectXTK/wiki/DDSTextureLoader
	DirectX::CreateDDSTextureFromFileEx(
		device.Get(), filename, 0, D3D11_USAGE_DEFAULT,
		D3D11_BIND_SHADER_RESOURCE, 0, miscFlags, DirectX::DDS_LOADER_FLAGS(false),
		(ID3D11Resource**)texture.GetAddressOf(),
		texResView.GetAddressOf(), NULL);
}
