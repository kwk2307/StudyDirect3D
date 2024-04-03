#include "Precompiled.h"

Animator::Animator():
	_mapAnim{},
	_CurAnim(nullptr)
{
}

Animator::~Animator()
{
	Safe_Erase_Map(_mapAnim);
}

Animation* Animator::CrateAnim(const std::string& InName, const std::vector<FrameInfo>& InFrameInfo)
{
    Animation* ResultAnim = FindAnim(InName);
    if (ResultAnim != nullptr) {
        return ResultAnim;
    }

    Animation* Anim = new Animation(InName, InFrameInfo);
    _mapAnim.insert(std::make_pair(InName, Anim));

    return Anim;
}

Animation* Animator::FindAnim(const std::string& InName)
{
    std::map<std::string, Animation*>::iterator iter = _mapAnim.find(InName);

    if (iter != _mapAnim.end()) {
        return iter->second;
    }

    return nullptr;

}

void Animator::SetCurAnim(const std::string& InName)
{
    Animation* ResultAnim = FindAnim(InName);
    if (ResultAnim != _CurAnim) {
        _CurAnim = ResultAnim;
    }
}
