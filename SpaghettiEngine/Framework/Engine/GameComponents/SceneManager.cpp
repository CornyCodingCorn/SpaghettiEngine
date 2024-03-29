﻿#include "SceneManager.h"
#include "SpaghettiEnginePath.h"
#include "json.hpp"
#include "CornException.h"
#include "Animation.h"
#include "Graphics.h"
#include "Path.h"
#include "Prefabs.h"
#include "Extra.h"
#include "Game.h"
#include "TextMap.h"
#include "MaterialContainer.h"
#include "TileSet.h"
#include "Mixer.h"

#include <fstream>
#include <future>

PSceneManager SceneManager::__instance = nullptr;

SceneManager::SceneManagerException::SceneManagerException(int line, const char* file, std::string description) noexcept
	:
	CornException(line, file),
	_description(description)
{}

const wchar_t* SceneManager::SceneManagerException::GetType() const noexcept
{
	return L"Scene manager exception |ʘ‿ʘ)╯";
}

const wchar_t* SceneManager::SceneManagerException::What() const noexcept
{
	std::wostringstream os;
	os << GetOriginString() << std::endl;
	os << _description.c_str();
	whatBuffer = os.str();
	return whatBuffer.c_str();
}

void SceneManager::Update()
{
	auto SM = GetInstance();

	// Wil implement the async stuff in the future for now let's just keep it single thread :D
	//if (!_isLoading)
	//{
	//	static std::future<void> loadingSceneJob;
	//	if (_startedLoadNewScene)
	//	{
	//		if (loadingSceneJob.wait_for(std::chrono::nanoseconds(1)) == std::future_status::ready)
	//		{
	//			try
	//			{
	//				loadingSceneJob.get();
	//
	//				scenes[sceneIndex]->LoadComponent();
	//				scenes[sceneIndex]->Start();
	//
	//				CleanUpAfterLoad();
	//				_startedLoadNewScene = false;
	//			}
	//			catch (const CornException&)
	//			{
	//				throw;
	//			}
	//			catch (const std::exception&)
	//			{
	//				throw;
	//			}
	//		}
	//	}
	//	else if (callLoadSceneIndex != sceneIndex)
	//	{
	//		_isLoading = true;
	//		_startedLoadNewScene = true;
	//
	//
	//		//Would try async stuff but this game engine have never designed around async so this is just a bad implementation.
	//		loadingSceneJob = std::async(std::launch::async, &SceneManager::StartLoadScene, this, scenes[sceneIndex], scenes[callLoadSceneIndex]);
	//	}
	//	else
	//	{
	//		SM->scenes[sceneIndex]->Update();
	//	}
	//}

	if (callLoadSceneIndex != sceneIndex || _forceReload)
	{
		SM->scenes[sceneIndex]->Unload();

		sceneIndex = callLoadSceneIndex;

		SM->scenes[sceneIndex]->Load();
		SM->scenes[sceneIndex]->LoadComponent();
		SM->scenes[sceneIndex]->Start();

		CleanUpAfterLoad();
		Game::ResetGameTimer();
		_forceReload = false;
	}

	SM->scenes[sceneIndex]->Update();
	SM->constScene->Update();
}

void SceneManager::FixedUpdate()
{
	if (!(_isLoading || _startedLoadNewScene))
	{
		scenes[sceneIndex]->FixedUpdate();
	}
	constScene->FixedUpdate();
}

void SceneManager::LateUpdate()
{
	__instance->scenes[sceneIndex]->LateUpdate();
	__instance->constScene->LateUpdate();
}

void SceneManager::CleanUp()
{
	__instance->scenes[sceneIndex]->EraseTrashBin();
	__instance->constScene->EraseTrashBin();
}

PSceneManager SceneManager::GetInstance()
{
	if (!__instance)
		__instance = new SceneManager();
	return __instance;
}

void SceneManager::StartLoadScene(SScene current, SScene toLoad)
{
	//DestructSetFlagThreadSafe setFlag (_isLoading, false);
	//
	//if (callLoadSceneIndex >= scenes.size() - 1)
	//{
	//	std::ostringstream os;
	//	os << "LoadScene called with index ";
	//	os << callLoadSceneIndex << std::endl;
	//	os << "But the number of scenes is ";
	//	os << scenes.size() - 1 << std::endl;
	//	throw SCENEMANAGER_EXCEPT(os.str());
	//}
	//
	//try
	//{
	//	current->Unload();
	//	toLoad->Load();
	//
	//	sceneIndex = callLoadSceneIndex.load();
	//}
	//catch (const CornException&)
	//{
	//	throw;
	//}
	//catch (const std::exception&)
	//{
	//	std::ostringstream os;
	//	os << "[Error] Standard exception has occurred while loading and unloading scene\n";
	//	os << "[Scene] " << callLoadSceneIndex << std::endl;
	//	throw SCENEMANAGER_EXCEPT(os.str());
	//}
	//return;
}

void SceneManager::CleanUpAfterLoad()
{
	TextMapContainer::GetInstance()->UnloadUnusedResources();
	PrefabsContainer::GetInstance()->UnloadUnusedResources();
	AnimationContainer::GetInstance()->UnloadUnusedResources();
	TileSetContainer::GetInstance()->UnloadUnusedResources();
	TextureContainer::GetInstance()->UnloadUnusedResources();
	MaterialContainer::GetInstance()->UnloadUnusedResources();
	MixerContainer::GetInstance()->UnloadUnusedResources();
}

void SceneManager::CallLoadNextScene()
{
	GetInstance()->callLoadSceneIndex = GetInstance()->sceneIndex + 1;
}

void SceneManager::CallLoadScene(unsigned index)
{
	GetInstance()->callLoadSceneIndex = index;
}

SScene& SceneManager::GetCurrentScene()
{
	return __instance->scenes[__instance->sceneIndex];
}

int SceneManager::GetCurrentSceneIndex()
{
	return __instance->sceneIndex;
}

int SceneManager::GetNumberOfScene()
{
	return static_cast<int>(__instance->scenes.size());
}

void SceneManager::CallLoadPreviousScene()
{
	GetInstance()->callLoadSceneIndex = GetInstance()->sceneIndex - 1;
}

void SceneManager::CallReloadScene()
{
	GetInstance()->_forceReload = true;
}

SScene& SceneManager::GetConstScene()
{
	return __instance->constScene;
}

SceneManager::SceneManager()
	:
	sceneIndex(-1),
	callLoadSceneIndex(-1)
{}

void SceneManager::Load()
{
	using namespace nlohmann;

	std::ifstream jsonStream(SystemPath::SceneManagerPath);
	if (!jsonStream.is_open())
	{
		std::ostringstream oss;
		oss << "[Exception] File ";
		oss << SystemPath::SceneManagerPath;
		oss << " doesn't exist";
		throw SCENEMANAGER_EXCEPT(oss.str());
	}
	try
	{
		static constexpr const char* StaticScene = "StaticScene";
		static constexpr const char* Scenes = "Scenes";
		static constexpr const char* Start = "Start";
		
		json file;
		jsonStream >> file;
		for (const auto& scene : file[Scenes])
			scenes.push_back(SScene(new Scene(CLib::ConvertPath(SystemPath::SceneManagerPath, scene.get<std::string>()))));
		scenes.push_back(SScene(new Scene("decoy"))); //Add decoy scene

		callLoadSceneIndex = file[Start].get<int>();
		sceneIndex = static_cast<unsigned>(scenes.size()) - 1u;
		constScene = SScene(new Scene(file[StaticScene].get<std::string>()));
	}
	catch (...)
	{
		std::ostringstream oss;
		oss << "[Exception] File ";
		oss << SystemPath::SceneManagerPath;
		oss << " is in the wrong format";
		throw SCENEMANAGER_EXCEPT(oss.str());
	}
}

void SceneManager::Init()
{
	Load();
	constScene->Load();
	constScene->LoadComponent();
	constScene->Start();
}

void SceneManager::Unload()
{
	scenes[sceneIndex]->Unload();
	constScene->Unload();

	CleanUpAfterLoad();
}
