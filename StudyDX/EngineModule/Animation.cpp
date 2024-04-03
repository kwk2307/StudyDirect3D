#include "Precompiled.h"

Animation::Animation(const std::string& InName, const std::vector<FrameInfo>& InFrameInfo):
	_Name(InName),
	_vecFrameInfo(InFrameInfo),
	_Cnt(0),
	_Time(0.f)
{
}

void Animation::Update(float InDeltaTime)
{
	_Time += InDeltaTime;

	if (_vecFrameInfo[_Cnt].Duration < _Time) {
		_Time -= _vecFrameInfo[_Cnt].Duration;
		
		_Cnt++;
		if (_Cnt == _vecFrameInfo.size()) {
			_Cnt = 0;
		}
	}

}
