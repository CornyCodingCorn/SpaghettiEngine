#include "TileMapRenderer.h"

REGISTER_FINISH(TileMapRenderer);

TileMapRenderer::TileMapRenderer() : height(0), width(0)
{
	name = TYPE_NAME(SpriteRenderer);
}

void TileMapRenderer::Update()
{
	time += GameTimer::GetDeltaTime();
	for (auto ani : animations)
	{
		ani->animation->Advance(&ani->frame, &time);
	}
}

void TileMapRenderer::Load(const string* inputArg)
{
	ifstream file(inputArg->c_str());
	if (!file.is_open())
	{
		wostringstream os;
		os << L"File ";
		os << inputArg->c_str();
		os << L" Doesn't exist";
		throw CORN_EXCEPT_WITH_DESCRIPTION(os.str());
	}

	try
	{
		json jsonFile;
		file >> jsonFile;

		Texture::GetTexture(&texture,jsonFile["Texture"]);

		width = jsonFile["Width"];
		height = jsonFile["Height"];
		
		for (int index : jsonFile["Data"])
		{
			data.push_back(index);
		}
		
		for (string aniPath : jsonFile["Animations"])
		{
			SAnimation ani = Animation::GetAnimation(&aniPath);
			AnimationTile* a = new AnimationTile();
			a->animation = ani;
			a->frame = 0;
			animations.push_back(a);
		}
		file.close();
	}
	catch (...)
	{
		wostringstream os;
		os << L"File ";
		os << inputArg->c_str();
		os << L" doesn't have the right format";
		throw CORN_EXCEPT_WITH_DESCRIPTION(os.str());
	}
}

void TileMapRenderer::Draw(SpriteHandler handler, PCamera camera)
{
	Matrix transform = camera->GetMatrix(GetWorldMatrix());
	if (Setting::IsWorldPointPixelPerfect())
	{
		transform._41 = std::round(transform._41);
		transform._42 = std::round(transform._42);
	}
	handler->SetTransform(&transform);

	int k = 0;

	for (int index : data)
	{
		int x = k % height;
		int y = (k / height);
		SSprite sprite;
		if (index > 0)
		{
			texture->GetSprite(&sprite, index - 1);
		}
		else if (index < 0)
		{
			SAnimation a = animations[abs(index + 1)]->animation;
			UINT f = animations[abs(index + 1)]->frame;
			sprite = a->GetSpriteOfFrame(&f);
		}

		RECT srcRect = sprite->GetSourceRect();
		Vector3 center = sprite->GetCenter();
		int px = x * sprite->GetWidth();
		int py = y * sprite->GetHeight();
		Vector3 pos(px, py, 0);

		handler->Draw(
			texture->GetImage(),
			&srcRect,
			&center,
			&pos,
			WHITE
		);

		k++;
	}

}