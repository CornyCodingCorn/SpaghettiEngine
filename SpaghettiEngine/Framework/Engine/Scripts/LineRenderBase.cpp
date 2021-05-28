#include "LineRenderBase.h"
#include "SMath.h"
#include "LoadingJson.h"

REGISTER_FINISH(LineRendererBase);

void LineRendererBase::Draw(PCamera camera)
{
	auto matrix = camera->GetMatrix(_ownerObj->GetWorldMatrix());
	auto transformedVectexes = _vertexes;
	for (auto& vectex : transformedVectexes)
		vectex = vectex * matrix;
	Graphics::Draw2DPolygon(transformedVectexes, _color);
}

void LineRendererBase::SetVertexes(const std::vector<Vector3>& vertextes)
{
	_vertexes = vertextes;
}

void LineRendererBase::SetCircle(float radius, const Vector3& center)
{
	if (radius <= 0)
		return;
	_vertexes = std::vector<Vector3>(_vertextesForCircle);
	_vertexes[0] = center;
	_vertexes[0].y += radius;

	Matrix4 rotationZ = SMath::GetZAxisRotateMatrix(360 / _vertextesForCircle);
	for (int i = 1; i < _vertextesForCircle; i++)
	{
		_vertexes[i] = _vertexes[i - 1] * rotationZ;
	}
}

void LineRendererBase::SetBox(const float width, const float height)
{
	_vertexes.clear();
	_vertexes.push_back(Vector3(-width / 2, -height / 2, 0));
	_vertexes.push_back(Vector3(width / 2, -height / 2, 0));
	_vertexes.push_back(Vector3(width / 2, height / 2, 0));
	_vertexes.push_back(Vector3(-width / 2, height / 2, 0));
}

void LineRendererBase::SetColor(Color color)
{
	_color = color;
}

void LineRendererBase::OnEnabled()
{
	Graphics::AddLineRender(this);
}

void LineRendererBase::OnDisabled()
{
	Graphics::RemoveLineRender(this);
}

void LineRendererBase::Load(nlohmann::json& inputObject)
{
	constexpr auto vertexesField = "Vertexes";
	constexpr auto colorField = "Color";
	constexpr auto widthField = "Width";
	constexpr auto heightField = "Height";
	constexpr auto radiusField = "Radius";
	constexpr auto centerField = "Center";

	std::string type = inputObject[LoadingJson::Field::typeField].get<std::string>();
	if (type == "Polygon")
	{
		_vertexes.reserve(inputObject[vertexesField].size());
		for (int i = 0; i < _vertexes.size(); i++)
		{
			auto vertexJson = inputObject[vertexesField][i];
			_vertexes[i].x = vertexJson[0];
			_vertexes[i].y = vertexJson[1];
			_vertexes[i].z = vertexJson[2];
		}
	}
	else if (type == "Rectangle")
	{
		SetBox(inputObject[widthField], inputObject[heightField]);
	}
	else
	{
		Vector3 center;
		if (inputObject[centerField] != nullptr)
		{
			center.x = inputObject[centerField][0];
			center.y = inputObject[centerField][1];
			center.z = inputObject[centerField][2];
		}

		SetCircle(inputObject[radiusField], center);
	}

	if (inputObject[colorField] != nullptr)
		_color = XRGB(inputObject[colorField][0], inputObject[colorField][1], inputObject[colorField][2]);
}
