#pragma once
#include <string>
#include <memory>
#include <vector>
#include "Texture.h"
#include "Sprite.h"
#include "GameObj.h"

typedef class GameObj* PGameObj;

typedef class Scene* PScene;
typedef std::shared_ptr<Scene> SScene;
typedef std::unique_ptr<Scene> UScene;

class Scene
{
	friend class SceneManager;
	friend class GameObj;
public:
	bool GetGameObj(PGameObj& gameObj, const std::string& path);
protected:
	Scene(std::string path);
	void RemoveGameObject(PGameObj gameObj);
	void AddGameObject(PGameObj gameObj);
	bool Load();
	bool Unload();
protected:
	std::string path = "";

	std::list<PGameObj> instances;
};