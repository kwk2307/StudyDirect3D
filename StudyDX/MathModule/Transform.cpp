#include "Precompiled.h"

Transform::Transform(const Matrix& InMatrix)
{
	DirectX::XMVECTOR scale, rotation, position;
	DirectX::XMMatrixDecompose(&scale, &rotation, &position, InMatrix);

	Scale = scale;
	Rotation = rotation;
	Position = position;
}
