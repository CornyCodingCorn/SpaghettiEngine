#pragma once
#include "GameWnd.h"

typedef class Graphics* PGraphics;
typedef class App* PApp;

class App
{
	friend class WinMain;
public:
	static void CallQuit();
	static PApp GetInstance() noexcept;
protected:
	BOOL Go();
	void DoFrame() const;
	void Quit() const;
protected:
	SGameWnd wnd = nullptr;
	STimer timer = nullptr;

	Size resolution;
	std::wstring appName;

	bool running = false;
	float _accumulation = 0.0f;

	static constexpr float _loopCap = 0.0001f;
	static PApp __instance;
};