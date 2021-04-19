#pragma once
#include "ScriptBase.h"
#include "SceneManager.h"
#include "Scene.h"
#include "CornDirectX.h"
#include <memory>
#include <list>

typedef class Scene* PScene;

typedef class GameObj* PGameObj;
typedef std::shared_ptr<GameObj> SGameObj;
typedef std::unique_ptr<GameObj> UGameObjl;

typedef class ScriptBase* PScriptBase;
typedef class std::shared_ptr<ScriptBase> SScriptBase;
typedef class std::unique_ptr<ScriptBase> UScriptBase;

class GameObj
{
	friend class Scene;
public:
	GameObj(const GameObj& obj);
	GameObj(const std::string path, const PScene ownerScene = nullptr);
	void Load();
	void Destroy();

	void Start();
	void Update();
	void End();

	const SScriptBase GetScript(UINT index) noexcept;
	const SScriptBase GetScript(std::string name) noexcept;
	const std::list<SScriptBase> GetAllScripts(std::string name) noexcept;

	const PGameObj GetParent();
	const PGameObj GetChild(UINT index);
	const Vector3* GetPosition();
	const char* GetTag();
	const char* GetPath();

	bool IsDisabled();
	void Disable();
	void Enable();

	void Translate(const Vector3* vector);
	void Move(const Vector3* newPosition);

	void RemoveParent();
	void AddParent(const PGameObj& gameObj);

	void BecomeCurrentSceneObj();
	void BecomeConstSceneObj();

	void SetTag(const char* tag);
protected:
	~GameObj();

	void AddScript(const std::string& scriptName, const std::string& arg);
	void AddScript(const PScriptBase script);
	void AddChild(PGameObj obj);

	void RemoveChild(PGameObj obj);
protected:
	PScene ownerScene;
	PGameObj parent;
	std::list<PGameObj> children;

	bool isDisabled = false;
	bool loaded = false;
	std::string path;
	std::string tag;

	Vector3 position;

	std::list<SScriptBase> scripts;
};