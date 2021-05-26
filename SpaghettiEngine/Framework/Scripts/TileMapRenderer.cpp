#include "TileMapRenderer.h"
#include "json.hpp"
#include "GameTimer.h"
#include "Animation.h"
#include "ExMath.h"
#include "GraphicsMath.h"
#include "LoadingJson.h"
#include <fstream>
#include <algorithm>
#include <Setting.h>

#include "Graphics.h"

using namespace nlohmann;
using namespace std;

REGISTER_FINISH(TileMapRenderer);

TileMapRenderer::TileMapRenderer(PScene owner) : Render2DScriptBase(owner), width(0), height(0), tileWidth(0), tileHeight(0)
{
	_name = TYPE_NAME(TileMapRenderer);
}

void TileMapRenderer::Load(nlohmann::json& inputObject)
{
	json copy = inputObject;
	std::string tileMapFilePath;

	constexpr const char* TileMapPath = "TileMapPath";
	try
	{
		tileMapFilePath = copy[TileMapPath].get<std::string>();
	}
	catch (const std::exception&)
	{
		std::wostringstream os;
		os << "\n[Error field] " << TileMapPath;
		throw SCRIPT_FORMAT_EXCEPT(this, os.str());
	}

	ifstream file(tileMapFilePath);
	if (!file.is_open())
	{
		std::wostringstream os;
		os << "[Error field] " << TileMapPath << std::endl;
		os << "[Error Info] " << "Tile map " << tileMapFilePath.c_str() << " doesn't exist" << std::endl;
		throw SCRIPT_FORMAT_EXCEPT(this, os.str());
	}

	try
	{
		json jsonFile;
		file >> jsonFile;

		width = jsonFile["Width"].get<int>();
		height = jsonFile["Height"].get<int>();
		tileWidth = jsonFile["TileWidth"].get<int>();
		tileHeight = jsonFile["TileHeight"].get<int>();
		_pixelWidth = width * tileWidth;
		_pixelHeight = height * tileHeight;
		
		_tileSet = TileSetContainer::GetInstance()->GetResource(jsonFile[LoadingJson::Field::idField]);

		int row = 0;
		int col = 0;
		_tiles.reserve(height);
		_tiles.emplace_back(std::vector<WTile>());
		_tiles[0].reserve(width);
		for (WTile tile; int index : jsonFile["Data"])
		{
			if (col == width)
			{
				_tiles.emplace_back(std::vector<WTile>());
				col = 0;
				row++;
				_tiles[row].reserve(width);
			}

			if (index > 0)
			{
				tile = _tileSet->GetTile(index - 1);
				_tiles[row].emplace_back(tile);
			}
			else
				_tiles[row].emplace_back(WTile());
			col++;
		}

		file.close();
	}
	catch (const std::exception& e)
	{
		std::wostringstream os;
		os << "[Error field] " << TileMapPath << std::endl;
		os << "[Error Info] " << "Tile map " << tileMapFilePath.c_str() << " doesn't have the right format" << std::endl;
		os << "[Exception] " << e.what() << std::endl;
		throw SCRIPT_FORMAT_EXCEPT(this, os.str());
	}
	Render2DScriptBase::Load(inputObject);
}

void TileMapRenderer::Draw(PCamera camera)
{
	using CLib::ToFloat;
	using CLib::ToInt;
	using CLib::ceili;
	using CLib::floori;

	const float halfHeightPx = (ToFloat(height) / 2.0f) * ToFloat(tileHeight);
	const float halfWidthPx = (ToFloat(width) / 2.0f) * ToFloat(tileWidth);

	//Set tiles map transform
	Matrix transform = GetWorldMatrix();
	transform._42 += halfHeightPx;
	transform._41 -= halfWidthPx;
	transform = camera->GetMatrix(transform);
	Graphics::SetSpriteTransform(transform);

	//get screen height and width then divide them by 2;
	Size viewPort = Setting::GetHalfResolution();
	Matrix tileMapMatrix;
	GraphicsMath::Inverse(GetWorldMatrix(), tileMapMatrix);
	tileMapMatrix = camera->GetWorldMatrix() * tileMapMatrix;

	Vector3 points[4];
	points[0] = { -static_cast<float>(viewPort.width), -static_cast<float>(viewPort.height), 0 };
	points[1] = { -static_cast<float>(viewPort.width),  static_cast<float>(viewPort.height), 0 };
	points[2] = { static_cast<float>(viewPort.width), -static_cast<float>(viewPort.height), 0 };
	points[3] = { static_cast<float>(viewPort.width),  static_cast<float>(viewPort.height), 0 };

	GraphicsMath::TransformVector3(points[0], points[0], tileMapMatrix);
	GraphicsMath::TransformVector3(points[1], points[1], tileMapMatrix);
	GraphicsMath::TransformVector3(points[2], points[2], tileMapMatrix);
	GraphicsMath::TransformVector3(points[3], points[3], tileMapMatrix);

	float maxX = points[0].x, maxY = points[0].y, minX = points[0].x, minY = points[0].y;
	for (const auto& point : points)
	{
		if (point.x > maxX)
			maxX = point.x;
		else if (point.x < minX)
			minX = point.x;

		if (point.y > maxY)
			maxY = point.y;
		else if (point.y < minY)
			minY = point.y;
	}

	std::swap(maxY, minY);
	maxY = _pixelHeight - (maxY + _pixelHeight / 2.0f);
	minY = _pixelHeight - (minY + _pixelHeight / 2.0f);
	maxX = maxX + ToFloat(_pixelWidth) / 2.0f;
	minX = minX + ToFloat(_pixelWidth) / 2.0f;
	int startRow = ceili(minY / ToFloat(tileWidth)) - 1;
	int endRow = ceili(maxY / ToFloat(tileWidth)) + 1;
	int startCol = ceili(minX / ToFloat(tileWidth)) - 1;
	int endCol = ceili(maxX / ToFloat(tileWidth)) + 1;

	startRow = startRow < 0 ? 0 : startRow;
	endRow = endRow < height ? endRow : height;
	startCol = startCol < 0 ? 0 : startCol;
	endCol = endCol < width ? endCol : width;

	Vector3 position;
	position.z = 0;
	for (int row = startRow; row < endRow; row++)
	{
		position.y = static_cast<float>(row * tileHeight);
		for (int col = startCol; col < endCol; col++)
		{
			if (!_tiles[row][col].expired())
			{
				position.x = static_cast<float>(col * tileWidth);
				_tiles[row][col].lock()->Draw(position);
			}
		}
	}
}

TileMapRenderer::~TileMapRenderer()
{
	_tiles.clear();
}