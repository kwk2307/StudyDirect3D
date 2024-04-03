#pragma once
enum class KeyState
{
	NONE,	// �ȴ���
	TAP,	// �ѹ�����
	HOLD,	// ������ ������
	AWAY,	// �� ������ �����
};

enum class Key {

	LEFT,
	RIGHT,
	UP,
	DOWN,
	Q,
	W,
	E,
	R,
	A,
	S,
	D,
	F,
	ALT,
	CTRL,
	SPACE,
	ENTER,
	ESC,
	LBUTTON,
	RBUTTON,
	LAST,
};

struct keyinfo {
	bool prev = false;
	KeyState state = KeyState::NONE;
};

class InputManager
{
	//input Manager ���� �ʿ� �� �� ���� 
public:
	InputManager();
	~InputManager();

public:

	void Update();
	KeyState GetKeyState(Key InKey) { return vecKey[static_cast<int>(InKey)].state; }
	float GetXAxis();
	float GetYAxis();

private:
	std::vector<keyinfo> vecKey;
};

