#include "Material.h"
#include "SpaghettiEnginePath.h"

Material::Material(const float staticFriction, const float dynamicFriction, const float restitution)
	:
	_staticFriction(staticFriction),
	_dynamicFriction(dynamicFriction),
	_restitution(restitution)
{}

WMaterial Material::CreateMaterial(const unsigned id, const float staticFriction, const float dynamicFriction, const float restitution)
{
	SMaterial newMaterial = std::make_shared<Material>(staticFriction, dynamicFriction, restitution);
	_materials.emplace(id, newMaterial);
	return newMaterial;
}

WMaterial Material::GetMaterial(const unsigned id)
{
	return _materials[id];
}

bool Material::RemoveMaterial(const unsigned id)
{
	auto it = _materials.find(id);
	if (it != _materials.end())
	{
		_materials.erase(id);
		return true;
	}
	else
		return false;
}

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