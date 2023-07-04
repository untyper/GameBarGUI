﻿#include "pch.h"
#include "Overlay.xaml.h"

#include "Input.h"


using namespace Cheat;
using namespace Platform;
using namespace Windows::Foundation;
using namespace Windows::Foundation::Collections;
using namespace Windows::UI::Xaml;
using namespace Windows::UI::Xaml::Controls;
using namespace Windows::UI::Xaml::Controls::Primitives;
using namespace Windows::UI::Xaml::Data;
using namespace Windows::UI::Xaml::Input;
using namespace Windows::UI::Xaml::Media;
using namespace Windows::UI::Xaml::Navigation;
using namespace Windows::UI;
using namespace Microsoft::Graphics::Canvas::UI::Xaml;
using namespace Microsoft::Graphics::Canvas;

using namespace Microsoft::Graphics::Canvas;
using namespace Microsoft::Graphics::Canvas::Text;
using namespace Microsoft::Graphics::Canvas::UI::Xaml;
using namespace Microsoft::Gaming::XboxGameBar;

using namespace Windows::Foundation;
using namespace Windows::UI::Core;
using namespace Windows::System;
using namespace Windows::UI::ViewManagement;
using namespace Windows::ApplicationModel::Core;

Microsoft::Graphics::Canvas::UI::Xaml::CanvasSwapChainPanel^ CanvasObject;

namespace sdk
{
	float WindowWidth;
	float WindowHeight;
}


Overlay::Overlay()
{
	InitializeComponent();

	// Hide Title Bar
	auto  coretitlebar = CoreApplication::GetCurrentView()->TitleBar;
	coretitlebar->ExtendViewIntoTitleBar = true;
	auto titlebar = ApplicationView::GetForCurrentView()->TitleBar;
	titlebar->ButtonBackgroundColor = Colors::Transparent;
	titlebar->ButtonInactiveBackgroundColor = Colors::Transparent;
	titlebar->ButtonPressedBackgroundColor = Colors::Transparent;
	titlebar->ButtonHoverBackgroundColor = Colors::Transparent;
}
POINT MousePosition;
void Overlay::PointerMoved(Platform::Object^ sender, Windows::UI::Xaml::Input::PointerRoutedEventArgs^ e)
{
	// Get the current mouse position
	Windows::Foundation::Point mp = e->GetCurrentPoint(nullptr)->Position;
	MousePosition = {(long) mp.X,(long)mp.Y };
}
void Overlay::PointerPressed(Platform::Object^ sender, Windows::UI::Xaml::Input::PointerRoutedEventArgs^ e)
{
	// Check the state of the mouse buttons
	Windows::UI::Input::PointerPoint^ pointer = e->GetCurrentPoint(nullptr);
	auto button = pointer->Properties->PointerUpdateKind;
	if (button == Windows::UI::Input::PointerUpdateKind::LeftButtonPressed)
	{
		UpdateKeyState(VK_LBUTTON, true);
	}
	if (button == Windows::UI::Input::PointerUpdateKind::RightButtonPressed)
	{
		UpdateKeyState(VK_RBUTTON, true);
	}
	if (button == Windows::UI::Input::PointerUpdateKind::MiddleButtonPressed)
	{
		UpdateKeyState(VK_MBUTTON, true);
	}
	if (button == Windows::UI::Input::PointerUpdateKind::XButton1Pressed)
	{
		UpdateKeyState(VK_XBUTTON1, true);
	}
	if (button == Windows::UI::Input::PointerUpdateKind::XButton2Pressed)
	{
		UpdateKeyState(VK_XBUTTON2, true);
	}
}
void Overlay::PointerReleased(Platform::Object^ sender, Windows::UI::Xaml::Input::PointerRoutedEventArgs^ e)
{
	// Check the state of the mouse buttons
	Windows::UI::Input::PointerPoint^ pointer = e->GetCurrentPoint(nullptr);
	auto button = pointer->Properties->PointerUpdateKind;
	if (button == Windows::UI::Input::PointerUpdateKind::LeftButtonReleased)
	{
		UpdateKeyState(VK_LBUTTON, false);
	}
	if (button == Windows::UI::Input::PointerUpdateKind::RightButtonReleased)
	{
		UpdateKeyState(VK_RBUTTON, false);
	}
	if (button == Windows::UI::Input::PointerUpdateKind::MiddleButtonReleased)
	{
		UpdateKeyState(VK_MBUTTON, false);
	}
	if (button == Windows::UI::Input::PointerUpdateKind::XButton1Released)
	{
		UpdateKeyState(VK_XBUTTON1, false);
	}
	if (button == Windows::UI::Input::PointerUpdateKind::XButton2Released)
	{
		UpdateKeyState(VK_XBUTTON2, false);
	}
}
int ValTest;
void Overlay::KeyDown(Windows::UI::Core::CoreWindow^ sender, Windows::UI::Core::KeyEventArgs^ e)
{
	// Get the key that was pressed down
	Windows::System::VirtualKey key = e->VirtualKey;
	int keyvalue = static_cast<int>(key);
	ValTest = 1;
	UpdateKeyState(keyvalue, true);
	
}
void Overlay::KeyUp(Windows::UI::Core::CoreWindow^ sender, Windows::UI::Core::KeyEventArgs^ e)
{
	// Get the key that was released
	Windows::System::VirtualKey key = e->VirtualKey;
	int keyvalue = static_cast<int>(key);
	ValTest = 1;
	UpdateKeyState(keyvalue, false);
}
//You can just pass the CanvasObject directly into this but I used it in other places also
void RenderingThread()
{
	static auto ds = CanvasObject->SwapChain->CreateDrawingSession(Colors::Transparent);

	while (true) 
	{
		
		ds->Clear(Colors::Transparent);
		/* RENDER*/

		std::string test = std::to_string(MousePosition.x) + "x" + std::to_string(MousePosition.y) +"|" + std::to_string(IsKeyDown(VK_LSHIFT));
		std::wstring wideText(test.begin(), test.end());
		Platform::String^ text = ref new Platform::String(wideText.c_str());
	
		ds->DrawText(text, 0, 0, Colors::Red);

		/*END OF RENDERING*/
		ds->Flush();
		CanvasObject->SwapChain->Present();
	

	
	}
}




void Overlay::SwapChainPanel_Loaded(Platform::Object^ sender, Windows::UI::Xaml::RoutedEventArgs^ e)
{
	canvasSwapChainPanel->SwapChain = ref new CanvasSwapChain(CanvasDevice::GetSharedDevice(), (float)Window::Current->CoreWindow->Bounds.Width, 
		(float)Window::Current->CoreWindow->Bounds.Height, 96);

	CanvasObject = canvasSwapChainPanel;
	CanvasObject->AllowFocusWhenDisabled = true;
	//lets use this it is way better for what we want
	sdk::WindowWidth = (float)Window::Current->CoreWindow->Bounds.Width;
	sdk::WindowHeight = (float)Window::Current->CoreWindow->Bounds.Height;
	Windows::UI::Core::CoreWindow::GetForCurrentThread()->KeyDown += ref new Windows::Foundation::TypedEventHandler<Windows::UI::Core::CoreWindow^, Windows::UI::Core::KeyEventArgs^>(this, &Overlay::KeyDown);
	Windows::UI::Core::CoreWindow::GetForCurrentThread()->KeyUp += ref new Windows::Foundation::TypedEventHandler<Windows::UI::Core::CoreWindow^, Windows::UI::Core::KeyEventArgs^>(this, &Overlay::KeyUp);

	SetInput();

	std::thread renderthread(RenderingThread);
	renderthread.detach();

}
