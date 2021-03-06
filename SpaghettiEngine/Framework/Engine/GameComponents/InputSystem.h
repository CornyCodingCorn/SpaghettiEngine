#pragma once
#include "Input.h"
#include "CornException.h"

#include <vector>
#include <string>

class InputSystem;
typedef InputSystem* PInputSystem;

class InputSystem
{
	friend class Game;
public:
	class InputSysException : public CornDescriptionException
	{
	public:
		InputSysException(int line, const char* file, std::wstring description) noexcept;
		virtual const wchar_t* GetType() const noexcept override;
	};
public:
	InputSystem(const InputSystem&) = delete;
	InputSystem& operator=(const InputSystem&) = delete;
	
	static int GetFirstKeyPressCode();
	static SInput GetInput(const string& name) noexcept;
	static std::string GetStrInput() noexcept;
	static std::wstring GetWideStrInput() noexcept;
protected:
	~InputSystem() = default;
	InputSystem();

	void Update();

	void Load();
	void Unload();
	void Save();

	void CreateInput(const Input::Type& type, const std::string& name, const int& code);

	static PInputSystem GetInstance();
protected:
	KeyBoard* kb;
	Mouse* mouse;

	std::wstring charInput;
	std::vector<SInput> inputs;

	static PInputSystem __instance;
};

#define INPUTSYS_EXCEPT( description ) InputSystem::InputSysException(__LINE__,__FILE__,description)