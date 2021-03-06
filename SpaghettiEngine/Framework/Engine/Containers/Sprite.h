#pragma once
#include "Plane2D.h"
#include "Texture.h"
#include "Rect.h"
#include <memory>

typedef class Sprite* PSprite;
typedef std::shared_ptr<Sprite> SSprite;
typedef std::unique_ptr<Sprite> USprite;

using Plane2D::Size;
using Plane2D::Point;

class Sprite
{
	friend class Texture;
public:
	[[nodiscard]]PTexture	GetSource() const;
	[[nodiscard]]int		GetWidth() const;
	[[nodiscard]]int		GetHeight() const;
	[[nodiscard]]Vector3	GetCenter() const;
	[[nodiscard]]tagRECT	GetSourceRect() const;
protected:
	Sprite(const PTexture& source, const int& x, const int& y, const int& w, const int& h);
	Sprite(const PTexture& source, const Size& size, const Point& position);
protected:
	Rect srcRect;
	PTexture source;
	Vector3 center;
};

