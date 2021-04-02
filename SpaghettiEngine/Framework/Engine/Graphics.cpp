﻿#include "Graphics.h"

PGraphics Graphics::__instance = nullptr;

Graphics::GraphicException::GraphicException(int line, const char* file, const wchar_t* discription) noexcept : CornException(line, file)
{
	this->discription = discription;
}

const wchar_t* Graphics::GraphicException::GetType() const noexcept
{
	return L"∑(O_O;) Graphic Exception";
}

const wchar_t* Graphics::GraphicException::What() const noexcept
{
	std::wostringstream oss;
	oss << GetType() << std::endl
		<< "[Description] " << GetErrorString() << std::endl
		<< GetOriginString();
	whatBuffer = oss.str();
	return whatBuffer.c_str();
}

std::wstring Graphics::GraphicException::GetErrorString() const noexcept
{
	return discription;
}

PGraphics Graphics::GetInstance()
{
	if (!__instance)
		__instance = new Graphics();
	return __instance;
}

void Graphics::ToFullScreenMode()
{
	if (__instance->isFullScreen)
		return;

	__instance->isFullScreen = true;
	__instance->ReleaseResource();
	__instance->dxpp.Windowed = FALSE;
	__instance->CreateResource();
}

void Graphics::ToWindowMode()
{
	if (!__instance->isFullScreen)
		return;

	__instance->isFullScreen = false;
	__instance->ReleaseResource();
	__instance->dxpp.Windowed = TRUE;
	__instance->CreateResource();
}

void Graphics::CreateResource()
{
	dx = Direct3DCreate9(D3D_SDK_VERSION);
	dx->CreateDevice(
		D3DADAPTER_DEFAULT,
		D3DDEVTYPE_HAL,
		wnd->GetHwnd(),
		D3DCREATE_SOFTWARE_VERTEXPROCESSING,
		&dxpp,
		&dxdev
	);

	if (!dx)
		throw GRAPHICS_EXCEPT(L"Can't initialize directX properly");
	if (!dxdev)
		throw GRAPHICS_EXCEPT(L"Can't initialize driectXDev, most likely cause is that your window type is difference than what is in d3dPresent_parameters");
}

void Graphics::ReleaseResource()
{
	if (dx)
		dx->Release();
	dx = nullptr;
	if (dxdev)
		dxdev->Release();
	dxdev = nullptr;
}

SWindow Graphics::GetCurrentWindow() const noexcept
{
	return wnd;
}

void Graphics::Init(STimer timer, int fps)
{
	if (fps <= 0)
		delayPerFrame = 0;
	else
		delayPerFrame = 1 / static_cast<double>(fps);

	this->timer = timer;

	ZeroMemory(&dxpp, sizeof(dxpp));

	wnd = SWindow(Window::Create(800, 600, Window::WindowMode::Window, L"SpaghettiE"));
	Size size = wnd->GetSize();

	dxpp.Windowed = TRUE; // thể hiện ở chế độ cửa sổ
	dxpp.SwapEffect = D3DSWAPEFFECT_DISCARD;
	dxpp.BackBufferFormat = D3DFMT_X8R8G8B8;
	dxpp.BackBufferCount = 1;
	dxpp.BackBufferWidth = size.width;
	dxpp.BackBufferHeight = size.height;
	dxpp.hDeviceWindow = wnd->GetHwnd();

	CreateResource();
}


void Graphics::Render()
{
	timeSinceLastFrame += timer->GetDeltaTime();
	if (timeSinceLastFrame < delayPerFrame)
	{
		while (!buffer.empty())
			buffer.pop();
		return;
	}
	else
	{
		timeSinceLastFrame -= delayPerFrame * static_cast<int>(timeSinceLastFrame / delayPerFrame);

		rgb[index] += delta;
		if (!(rgb[index] & 0xFF))
		{
			if (jump)
			{
				index += 2;
				index %= 3;
				rgb[index] += 1;
				delta = 1;
				jump = false;
			}
			else
			{
				rgb[index] -= 1;
				index -= 1;
				if (index < 0)
					index = 2;
				delta = -1;
				jump = true;
			}
		}
	}

	dxdev->Clear(0, NULL, D3DCLEAR_TARGET, D3DCOLOR_XRGB(rgb[0], rgb[1], rgb[2]), 1.0f, 0);

	if (Begin() != 0)
	{
		/// <summary>
		/// Render here
		/// </summary>

		if (!End())
		{
			Reset();
		}
	}

	dxdev->Present(NULL, NULL, NULL, NULL);
}

HRESULT Graphics::Begin() noexcept
{
	return dxdev->BeginScene();
}

bool Graphics::End()
{
	// device alive
	if (!isDeviceLost)
	{
		dxdev->EndScene();
	}
	HRESULT hr = dxdev->TestCooperativeLevel();
	if (hr != D3D_OK)
	{
		if (hr == D3DERR_DEVICELOST)
		{
			isDeviceLost = true;
		}
		else if (hr == D3DERR_DRIVERINTERNALERROR)
		{
			PostQuitMessage(0);
			throw GRAPHICS_EXCEPT(L"directX driver internal error");
		}
		return false;
	}
	return true;
}

bool Graphics::Reset()
{
	HRESULT hr = dxdev->TestCooperativeLevel();
	if (hr == D3DERR_DEVICENOTRESET)
	{
		if (SUCCEEDED(dxdev->Reset(&dxpp)))
		{
			// reset success
			isDeviceLost = false;
			return true;
		}
	}
	// failed to reset
	return false;
}

Graphics::Graphics() noexcept
{
	ZeroMemory(&dxpp, sizeof(dxpp));
}

Graphics::~Graphics() noexcept
{
	ReleaseResource();
}
