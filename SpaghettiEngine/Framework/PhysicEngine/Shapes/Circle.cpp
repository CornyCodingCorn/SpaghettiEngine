#include "Circle.h"
#include "Collision.h"

Shape::Type Circle::GetType() const
{
	return Shape::Type::Circle;
}

void Circle::SetRadius(const float& radius)
{
	_radius = radius;
}

void Circle::UpdateParameter()
{
	Matrix4 matrix = _body.lock()->GetWorldMatrix();
	if (matrix == _worldMatrix)
		return;

	_worldMatrix = matrix;
	_center.x = _worldMatrix._41;
	_center.y = _worldMatrix._42;
	_center.z = 0;
	_center = _center * _offSetMatrix;
}

bool Circle::CircleCircle(Collision* collision)
{
	auto shapeA = dynamic_cast<Circle*>(collision->GetShapeA());
	auto shapeB = dynamic_cast<Circle*>(collision->GetShapeB());

	Vector3 normal = shapeB->GetCenter() - shapeA->GetCenter();
	float sumRadius = shapeA->_radius + shapeB->_radius;
	sumRadius *= sumRadius;

	if (normal.GetPow2Magnitude() > sumRadius)
		return false;

	sumRadius = shapeA->_radius + shapeB->_radius;
	float distance = normal.GetMagnitude();
	if (distance != 0)
	{
		collision->SetPenetration(sumRadius - distance);
		collision->SetNormal(normal / distance);
		return true;
	}
	else
	{
		collision->SetPenetration(shapeA->_radius);
		collision->SetNormal(Vector3(0, 1, 0));
		return true;
	}
}

bool Circle::CirclePolygon(Collision* collision)
{
	return false;
}