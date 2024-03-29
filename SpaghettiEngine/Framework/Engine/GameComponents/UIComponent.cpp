#include "UIComponent.h"
#include "Canvas.h"
#include "Graphics.h"
#include "UIFields.h"

#include "UIString.h"
#include "RectangleUI.h"

void UI_COMPONENT_REGISTER()
{
	RectangleUI shit1;
	UIString shit2;
}

std::shared_ptr<UIComponent> UIFactory::Create(const std::string& className)
{
	auto _uiComponent = _createFunction[className]();
	_uiComponent->SetType(className);
	return _uiComponent;
}

const Vector3& UIComponent::GetPosition() const noexcept
{
	return _position;
}

const float& UIComponent::GetWidth() const noexcept
{
	return _width;
}

const float& UIComponent::GetHeight() const noexcept
{
	return _height;
}

const Color& UIComponent::GetColor() const noexcept
{
	return _color;
}

void UIComponent::SetPosition(const Vector3& pos) noexcept
{
	_position = pos;
}

void UIComponent::SetWidth(const float& width) noexcept
{
	_width = width;
}

void UIComponent::SetHeight(const float& height) noexcept
{
	_height = height;
}

void UIComponent::SetColor(const Color& color) noexcept
{
	_color = color;
}

void UIComponent::AddChild(UIComponent* component)
{
	_children.emplace(component);
}

void UIComponent::RemoveChild(UIComponent* component)
{
	_children.erase(component);
}

void UIComponent::Load(nlohmann::json& input)
{
	if (!input[UIField::Component::_color].empty())
		_color = input[UIField::Component::_color].get<unsigned>();
	else
		_color = Color(255, 255, 255, 255);

	_position = input[UIField::Component::_position].empty() ? Vector3() : Vector3(input[UIField::Component::_position]);

	for (auto& componentJson : input[UIField::Component::_components])
	{
		auto newComponent = UIFactory::Create(componentJson[UIField::Component::_type].get<std::string>());
		newComponent->SetName(componentJson[UIField::Component::_name].get<std::string>());
		_canvas->AddComponent(newComponent);

		AddChild(newComponent.get());

		newComponent->SetCanvas(_canvas);
		newComponent->Load(componentJson);
	}
}

const std::set<UIComponent*>& UIComponent::GetChildren() const noexcept
{
	return _children;
}

const std::string& UIComponent::GetType() const noexcept
{
	return *_typeStr;
}

const std::string& UIComponent::GetName() const noexcept
{
	return _componentName;
}

UIComponent* UIComponent::Clone(PCanvas canvas)
{
	auto clone = UIFactory::Create(GetType());
	canvas->AddComponent(clone);

	clone->_canvas = canvas;
	clone->_position = _position;
	clone->_height = _height;
	clone->_width = _width;
	clone->_componentName = _componentName;
	
	for (auto& child : _children)
	{
		auto childClone = child->Clone(canvas);
		clone->AddChild(childClone);
	}

	return clone.get();
}

void UIComponent::Draw(Vector3 position)
{
	auto drawPos = position + _position;

	for (auto& component : _children)
		component->Draw(drawPos);
}

void UIComponent::SetType(const std::string& className) noexcept
{
	_typeStr = &className;
}

void UIComponent::SetName(const std::string& componentName) noexcept
{
	_componentName = componentName;
}

void UIComponent::SetCanvas(PCanvas canvas)
{
	_canvas = canvas;
}
