#pragma once

struct Rotator
{
public:
	constexpr Rotator() = default;
	constexpr Rotator(float InYaw, float InRoll, float InPitch) : Yaw(InYaw), Roll(InRoll), Pitch(InPitch) { }
	constexpr Rotator(const Vector3& InVector) : Yaw(MathUtil::Rad2Deg(InVector.y)), Roll(MathUtil::Rad2Deg(InVector.z)), Pitch(MathUtil::Rad2Deg(InVector.x)) { }

	void Clamp()
	{
		Yaw = GetAxisClampedValue(Yaw);
		Roll = GetAxisClampedValue(Roll);
		Pitch = GetAxisClampedValue(Pitch);
	}

	float GetAxisClampedValue(float InRotatorValue)
	{
		float angle = MathUtil::FMod(InRotatorValue, 360.f);
		if (angle < 0.f)
		{
			angle += 360.f;
		}

		return angle;
	}

	void GetLocalAxes(Vector3& OutRight, Vector3& OutUp, Vector3& OutForward)
	{
		float cy = 0.f, sy = 0.f, cp = 0.f, sp = 0.f, cr = 0.f, sr = 0.f;
		MathUtil::GetSinCos(sy, cy, Yaw);
		MathUtil::GetSinCos(sp, cp, Pitch);
		MathUtil::GetSinCos(sr, cr, Roll);

		OutRight = Vector3(cy * cr + sy * sp * sr, cp * sr, -sy * cr + cy * sp * sr);
		OutUp = Vector3(-cy * sr + sy * sp * cr, cp * cr, sy * sr + cy * sp * cr);
		OutForward = Vector3(sy * cp, -sp, cy * cp);
	}

	std::string ToString() const;
	static const Rotator Identity;

public:
	float Yaw = 0.f;
	float Roll = 0.f;
	float Pitch = 0.f;
};