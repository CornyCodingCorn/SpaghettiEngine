#pragma once
#include "CornDirectX.h"
#include "CornException.h"
#include "GameWnd.h"
#include "Texture.h"
#include <vector>

/// <summary>
/// Singleton directx9 wrapper
/// </summary>

typedef class Render2DScriptBase* PRender2DScriptBase;
typedef class Camera* PCamera;
typedef class Graphics* PGraphics;

class Graphics
{
	friend class App;
	friend class SceneManager;
public:
	enum class ColorFormat
	{
		DEFAULT = 0,
		ARGB32Bit = 21,
		RGB32Bit = 22
	};

	class GraphicException : public CornDescriptionException
	{
	public:
		GraphicException(int line, const char* file, std::wstring description) noexcept;
		virtual const wchar_t* GetType() const noexcept override;
	};
	class GraphicCodeException : public CornException
	{
	public:
		GraphicCodeException(const int line, const char* file, HRESULT code) noexcept;
		virtual const wchar_t* GetType() const noexcept override;
		virtual const wchar_t* What() const noexcept override;
		virtual const wchar_t* Translate() const noexcept;
		virtual HRESULT GetErrorCode() noexcept;
	private:
		HRESULT code;
	};
public:
	Graphics(const Graphics&) = delete;
	Graphics& operator=(const Graphics&) = delete;
	
	static PGraphics GetInstance();
	static void ToFullScreenMode();
	static void ToWindowMode();
	static void LoadTexture(PDx9Texture& rTexture, const std::string& path, const Color& keyColor);

	static void AddRender2D(PRender2DScriptBase renderScript);
	static void RemoveRender2D(PRender2DScriptBase renderScript);
	
	static void AddCamera(PCamera camera);
	static void RemoveCamera(PCamera camera);

	static void SetActiveCamera(PCamera setCamera);
	[[nodiscard]] static PCamera GetActiveCamera();
protected:
	void ClearRenderBuffer2D();
	void ClearRenderBuffer(); // for later if we want to add a 3D renderer

	void CreateResource();
	void ReleaseResource();

	void FullScreen();
	void Window();
	[[nodiscard]] SGameWnd GetCurrentWindow() const noexcept;
	
	void Init(const STimer& timer, const ColorFormat& colorFormat);

	HRESULT Begin() const noexcept;
	void Render();
	bool End();
	bool Reset();

	void UpdateCurrentVideoAdapter();

	Graphics() noexcept;
	~Graphics() noexcept;
protected:
	SGameWnd wnd;
	RECT restoreRec;
	bool isFullScreen = false;
	Size resolution;

	Renderer renderer = nullptr;
	RenderDevice renderDevice = nullptr;
	PresentParam presentParam;
	SpriteHandler spriteHandler = nullptr;
	ColorFormat colorFormat = ColorFormat::RGB32Bit;
	UINT videoAdapter = D3DADAPTER_DEFAULT;
	std::vector<DisplayMode> adapterMode;

	bool isDeviceLost = false;

	STimer timer;
	double delayPerFrame;
	double timeSinceLastFrame;

	bool isPixelPerfect = false;
	std::list<PCamera> cameraList;
	std::vector<std::list<PRender2DScriptBase>> _renderBuffer2D;

	static PGraphics __instance;
private:
#ifdef _DEBUG
#include "Timer.h"
	float fps = 0;
	PTimer fpsTimer = Timer::Create();
	RECT fpsRect;
	FontHandler fpsFont = nullptr;

	int index = 2;
	int delta = -1;
	int rgb[3] = { 255, 0, 1 };
	bool jump = true;

	void UpdateFPS() {
		fpsTimer->Mark();
		fps = fps * 0.8 + 0.2 * (1 / fpsTimer->GetDeltaTime());
	}
#endif
};

#define GRAPHICS_EXCEPT(description) Graphics::GraphicException(__LINE__,__FILE__,description)
#define GRAPHICS_EXCEPT_CODE(code) Graphics::GraphicCodeException(__LINE__,__FILE__,code)