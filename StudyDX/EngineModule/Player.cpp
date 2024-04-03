#include "Precompiled.h"

Player::Player(const ObjectInfo& Info):
	Object(Info)
{
	std::vector<FrameInfo> Info_idle = { 
		{0.1f, std::hash<std::string>()("Warrior_Idle_1")},
		{0.1f, std::hash<std::string>()("Warrior_Idle_2")},
		{0.1f, std::hash<std::string>()("Warrior_Idle_3")},
		{0.1f, std::hash<std::string>()("Warrior_Idle_4")},
		{0.1f, std::hash<std::string>()("Warrior_Idle_5")},
		{0.1f, std::hash<std::string>()("Warrior_Idle_6")}
	};
	GetAnimator()->CrateAnim("Anim_idle",Info_idle);
	

	std::vector<FrameInfo> Info_run = {
		{0.1f, std::hash<std::string>()("Warrior_Run_1")},
		{0.1f, std::hash<std::string>()("Warrior_Run_2")},
		{0.1f, std::hash<std::string>()("Warrior_Run_3")},
		{0.1f, std::hash<std::string>()("Warrior_Run_4")},
		{0.1f, std::hash<std::string>()("Warrior_Run_5")},
		{0.1f, std::hash<std::string>()("Warrior_Run_6")},
		{0.1f, std::hash<std::string>()("Warrior_Run_7")},
		{0.1f, std::hash<std::string>()("Warrior_Run_8")},
	};
	GetAnimator()->CrateAnim("Anim_run", Info_run);

	std::vector<FrameInfo> Info_Jump = {
		{0.1f, std::hash<std::string>()("Warrior_Jump_1")},
		{0.1f, std::hash<std::string>()("Warrior_Jump_2")},
		{0.1f, std::hash<std::string>()("Warrior_Jump_3")}
	};
	GetAnimator()->CrateAnim("Anim_jump", Info_Jump);

	std::vector<FrameInfo> Info_Fall = {
		{0.1f, std::hash<std::string>()("Warrior_Fall_1")},
		{0.1f, std::hash<std::string>()("Warrior_Fall_2")},
		{0.1f, std::hash<std::string>()("Warrior_Fall_3")},
	};
	GetAnimator()->CrateAnim("Anim_fall", Info_Fall);

	GetAnimator()->SetCurAnim("Anim_idle");
}

void Player::Update(float InDeltaSeconds)
{

	//ÁÂ¿ì ¿òÁ÷ÀÓ
	float XAixs = InputManager::GetInstanc().GetXAxis();
	if (MathUtil::Abs(XAixs) > 0.1f ){
		if (XAixs > 0) {
			GetTransform().SetRotation(Rotator(0.f, 0.f, 0.f));
		}
		else {
			GetTransform().SetRotation(Rotator(180.f, 0.f, 0.f));
		}

		if (MathUtil::Abs(XAixs) < MAX_SPEED) {
			_Speed += Vector3(XAixs, 0.f, 0.f);
		}
	}
	else {
		if (_Speed.X < 0) {
			_Speed += Vector3::UnitX;
		}
		else if (_Speed.X > 0) {
			_Speed -= Vector3::UnitX;
		}
	}
	
 	if (InputManager::GetInstanc().GetKeyState(Key::SPACE) == KeyState::TAP) {
		if (isLand) {
			_Speed.Y = 800.f;
		}
	}
	if (!isLand) {
		_Speed += Vector3(0.f, -10.f, 0.f);
	}
	GetTransform().AddPosition(_Speed * InDeltaSeconds);

	if (isLand) {
		if (MathUtil::Abs(_Speed.X) > 0.1f) {
			GetAnimator()->SetCurAnim("Anim_run");
		}
		else {
			GetAnimator()->SetCurAnim("Anim_idle");
		}
	}
	else {
		if (_Speed.Y > 0.f) {
			GetAnimator()->SetCurAnim("Anim_jump");
		}
		else {
			GetAnimator()->SetCurAnim("Anim_fall");
		}	
	}

	GetAnimator()->GetCurAnim()->Update(InDeltaSeconds);
	SetTextureKey(GetAnimator()->GetCurAnim()->GetTextureKey());
}

void Player::OnCollision(const Collider& InOther)
{
}

void Player::BeginCollision(const Collider& InOther)
{
	if (InOther.GetOwner().GetType() == ObjectType::Block) {
		isLand = true;
		_Speed.Y = 0;
	}
}

void Player::EndCollision(const Collider& InOther)
{
	isLand = false;
}

