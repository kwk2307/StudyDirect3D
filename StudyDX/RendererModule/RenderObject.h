#pragma once
class RenderObject
{

public:
	void Initialize(ComPtr<ID3D11Device>& device,
		ComPtr<ID3D11DeviceContext>& context,
		const std::vector<MeshData>& meshes);

	void UpdateConstantBuffers(ComPtr<ID3D11Device>& device,
		ComPtr<ID3D11DeviceContext>& context);

	void Render(ComPtr<ID3D11DeviceContext>& context);

	void UpdateModelWorld(const Matrix& modelToWorldRow);

public:
	// Model/World ���: ������ ���ǵ� ���� ���� ��ǥ�迡�� �׷��� ��ġ��
	// ��ȯ���ִ� ���
	// ���� ��ü�����δ� Model to World ��ȯ ����̶�� �� �� �ֽ��ϴ�.
	// DirectX(�𸮾� ����)������ World ����̶�� �θ��ϴ�.
	// http://www.opengl-tutorial.org/beginners-tutorials/tutorial-3-matrices/
	// https://gamedev.stackexchange.com/questions/194929/difference-between-world-matrix-and-model-matrix
	// https://jsantell.com/model-view-projection/
	Matrix m_modelWorldRow = Matrix();
	Matrix m_invTransposeRow = Matrix();

	// ExampleApp::Update()���� ����
	BasicVertexConstData m_basicVertexConstData;
	BasicPixelConstData m_basicPixelConstData;

	// ExampleApp:Initialize()���� ����
	ComPtr<ID3D11ShaderResourceView> m_irradianceSRV;
	ComPtr<ID3D11ShaderResourceView> m_specularSRV;
	ComPtr<ID3D11ShaderResourceView> m_brdfSRV;

	// GUI���� ������Ʈ �� �� ���
	NormalVertexConstantData m_normalVertexConstData;
	bool m_drawNormalsDirtyFlag = true;
	bool m_drawNormals = false;

	std::vector<shared_ptr<Mesh>> m_meshes; // Mipmaps ������ ���� �ӽ÷�

private:
	ComPtr<ID3D11VertexShader> m_basicVertexShader;
	ComPtr<ID3D11PixelShader> m_basicPixelShader;
	ComPtr<ID3D11InputLayout> m_basicInputLayout;

	ComPtr<ID3D11SamplerState> m_samplerState;
	ComPtr<ID3D11SamplerState> m_clampSamplerState;

	ComPtr<ID3D11Buffer> m_vertexConstantBuffer;
	ComPtr<ID3D11Buffer> m_pixelConstantBuffer;

	// �޽��� ��� ���� �׸���
	ComPtr<ID3D11GeometryShader> m_normalGeometryShader;
	ComPtr<ID3D11VertexShader> m_normalVertexShader;
	ComPtr<ID3D11PixelShader> m_normalPixelShader;

	// shared_ptr<Mesh> m_normalLines;

	ComPtr<ID3D11Buffer> m_normalVertexConstantBuffer;
	ComPtr<ID3D11Buffer> m_normalPixelConstantBuffer;
};

