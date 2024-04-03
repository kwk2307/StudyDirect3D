#pragma once
class Animator
{
public:
	Animator();
	~Animator();

public:
	Animation* CrateAnim(const std::string& InName, const std::vector<FrameInfo>& InFrameInfo);
	Animation* FindAnim(const std::string& InName);

	Animation* GetCurAnim() { return _CurAnim; }
	void SetCurAnim(const std::string& InName);

private:
	std::map<std::string, Animation*> _mapAnim;
	Animation* _CurAnim;
};

