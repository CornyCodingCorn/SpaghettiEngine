#include "Material.h"
#include "SpaghettiEnginePath.h"

Material::Material()
{
	this->_staticFriction = _defaultMaterial->_staticFriction;
	this->_dynamicFriction = _defaultMaterial->_dynamicFriction;
	this->_restitution = _defaultMaterial->_restitution;
}

Material::Material(const float staticFriction, const float dynamicFriction, const float restitution)
	:
	_staticFriction(staticFriction),
	_dynamicFriction(dynamicFriction),
	_restitution(restitution)
{}

const float& Material::GetStaticFriction() const noexcept
{
	return _staticFriction;
}

const float& Material::GetDynamicFriction() const noexcept
{
	return _dynamicFriction;
}

const float& Material::GetRestitution() const noexcept
{
	return _restitution;
}

void Material::Load(const std::string& path)
{
	using namespace nlohmann;

	std::ifstream jsonFile(path);
	if (!jsonFile.is_open())
	{
		std::ostringstream os;
		os << "[Exception] File ";
		os << path.c_str();
		os << " doesn't exist";
		throw RESOURCE_LOAD_EXCEPTION(os.str(), Texture);
	}

	try
	{
		constexpr auto restitutionField = "Restitution";
		constexpr auto dynamicFrictionField = "DynamicFriction";
		constexpr auto staticFrictionField = "StaticFriction";

		json file;
		jsonFile >> file;

		_restitution = file[restitutionField].get<float>();
		_dynamicFriction = file[dynamicFrictionField].get<float>();
		_staticFriction = file[staticFrictionField].get<float>();
	}
	catch (...)
	{
		std::ostringstream os;
		os << "File ";
		os << path.c_str();
		os << " doesn't have the right format";
		throw RESOURCE_LOAD_EXCEPTION(os.str(), Texture);
	}
}

WMaterial Material::GetDefaultMaterial()
{
	return _defaultMaterial;
}

void Material::SetStaticFriction(const float f) noexcept
{
	_staticFriction = f;
}

void Material::SetDynamicFriction(const float f) noexcept
{
	_dynamicFriction = f;
}

void Material::SetRestitution(const float f) noexcept
{
	_restitution = f;
}
