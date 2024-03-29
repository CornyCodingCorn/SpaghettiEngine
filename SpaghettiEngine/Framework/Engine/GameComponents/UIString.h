#pragma once 
#include "UIComponent.h"
#include "Sprite.h"
#include "TextMap.h"
#include <map>

// Render string to sprite with predefined map

class UIString : public UIComponent
{
public:
	void Draw(Vector3 pos) override;
	void Load(nlohmann::json& input) override;

	void SetText(const std::string& text);
	[[nodiscard]] const std::string& GetText() const noexcept; 
private:
	void GetSprites();
private:
	// Update first time
	bool _textChanged = true;
	std::string _text = "";
	std::list<SSprite> _sprites;
	STextMap _map;

	// -1 for unlimited size
	int _size = 0;
	char _fill = 0;
	SSprite _fillSprite;

	REGISTER_UI_COMPONENT_START(UIString);
};