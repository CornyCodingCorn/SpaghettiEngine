﻿#include "InputSystem.h"
#include "json.hpp"
#include "SpaghettiEnginePath.h"
#include "KeyBoard.h"
#include "Mouse.h"
#include "StringConverter.h"

#include <sstream>
#include <fstream>

constexpr const char* InputText = "Inputs";
constexpr const char* TypeText = "Type";
constexpr const char* NameText = "Name";
constexpr const char* CodeText = "KeyCode";

PInputSystem InputSystem::__instance = nullptr;

int InputSystem::GetFirstKeyPressCode()
{
	return KeyCode();
}

SInput InputSystem::GetInput(const string& name) noexcept
{
	for (const auto& input : __instance->inputs)
		if (input->CheckName(name))
			return input;

	return SInput();
}

std::string InputSystem::GetStrInput() noexcept
{
	return StringConverter::WStrToStr(__instance->charInput);
}

std::wstring InputSystem::GetWideStrInput() noexcept
{
	return __instance->charInput;
}

PInputSystem InputSystem::GetInstance()
{
	if (!__instance)
		__instance = new InputSystem();
	return __instance;
}

InputSystem::InputSystem()
{
	kb = KeyBoard::GetInstance();
	mouse = Mouse::GetInstance();

	Load();
}

void InputSystem::Update()
{
	charInput = L"";
	for (const auto& input : inputs)
		input->Reset();

	KeyBoardEvent e;
	while (!kb->IsKeyEmpty())
	{
		e = kb->ReadKey();
		if (!e.IsValid())
		{
			for (const auto& input : inputs)
				input->LostFocus();
		}
		else
		{
			for (const auto& input : inputs)
				input->Update(e);
		}
	}

	while (!kb->IsCharEmpty())
	{
		charInput += kb->ReadChar();
	}
}

void InputSystem::Load()
{
	using namespace nlohmann;

	std::ifstream jsonStream(SystemPath::InputPath);
	if (!jsonStream.is_open())
	{
		std::wostringstream oss;
		oss << L"File ";
		oss << SystemPath::InputPath;
		oss << L" doesn't exist";
		throw INPUTSYS_EXCEPT(oss.str());
	}
	try
	{
		json file;
		jsonStream >> file;
		
		for (const auto& input : file[InputText])
		{
			auto type = input[TypeText].get<Input::Type>();
			auto name = input[NameText].get<std::string>();
			auto code = input[CodeText].get<KeyCode>();
			CreateInput(type, name, code);
		}
	}
	catch (...)
	{
		std::wostringstream oss;
		oss << L"File ";
		oss << SystemPath::InputPath;
		oss << L" is in the wrong format";
		throw INPUTSYS_EXCEPT(oss.str());
	}
}

void InputSystem::Unload()
{
	inputs.clear();
}

NLOHMANN_JSON_SERIALIZE_ENUM(Input::Type, {
	{Input::Type::KeyPress, "KeyPress"},
	{Input::Type::KeyUp, "KeyUp"},
	{Input::Type::KeyDown, "KeyDown"},
	{Input::Type::KeyRelease, "KeyRelease"},
	{Input::Type::All, "All"}
	})

void InputSystem::Save()
{
	using namespace nlohmann;

	json j;
	for (const auto& loadedInput : inputs)
	{
		j[InputText].push_back({});
		auto input = j[InputText].back();

		input[CodeText] = loadedInput->keyCode;
		input[NameText] = loadedInput->name;
		input[TypeText] = loadedInput->GetType();
	}

	std::ofstream oFile;
	oFile.open(SystemPath::InputPath, std::ios_base::trunc);
	oFile << j;
	oFile.close();
}

void InputSystem::CreateInput(const Input::Type& type, const std::string& name, const int& code)
{
	inputs.push_back(Input::Create(code, name, type));
}

InputSystem::InputSysException::InputSysException(int line, const char* file, std::wstring description) noexcept
	:
	CornDescriptionException(line, file, std::move(description))
{}

const wchar_t* InputSystem::InputSysException::GetType() const noexcept
{
	return L"Σ(°△°|||) Input system exception";
}
