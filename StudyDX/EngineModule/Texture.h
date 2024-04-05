#pragma once
class Texture
{
public:
	Texture() = default;
	Texture(std::string InFileName);
	Texture(std::wstring InFileName);
	~Texture() { }

public:
	void Release();
	bool IsIntialized() const { return (_Buffer.size() > 0); }
	UINT32 GetWidth() const { return _Width; }
	UINT32 GetHeight() const { return _Height; }
	UINT32 GetSize() const { return _Width * _Height; }

private:
	void LoadFromFile(FILE* f);

private:
	static constexpr BYTE _Channel = 4;
	std::vector<Color> _Buffer;
	UINT32 _Width = 0;
	UINT32 _Height = 0;
};

