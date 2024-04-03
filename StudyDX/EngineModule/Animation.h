#pragma once

struct FrameInfo {
	float Duration;
	std::size_t Texturekey;
};

class Animation

{
public:
	Animation(const std::string& InName, const std::vector<FrameInfo>& InFrameInfo);

public:
	void Update(float InDeltaTime);

	std::size_t GetTextureKey() const { return _vecFrameInfo[_Cnt].Texturekey; }

private:
	std::string _Name;
	std::vector<FrameInfo> _vecFrameInfo;
	
	UINT _Cnt; // 현재 텍스쳐 순서
	float _Time; // 누적 시간
};

