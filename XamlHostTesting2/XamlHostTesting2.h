#pragma once

#include "resource.h"

#include <windows.ui.core.h>
#include <corewindow.h>

#include "winrt/XamlHostTesting2.h"

#include "App.g.h"

// Global Variables:
HINSTANCE hInst;                                // current instance

// Forward declarations of functions included in this code module:
ATOM                MyRegisterClass(HINSTANCE hInstance);
BOOL                InitInstance(HINSTANCE, int);
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);

// Defines the window types for core windows
enum WINDOW_TYPE
{
    IMMERSIVE_BODY = 0x0,
    IMMERSIVE_DOCK = 0x1,
    IMMERSIVE_HOSTED = 0x2,
    IMMERSIVE_TEST = 0x3,
    IMMERSIVE_BODY_ACTIVE = 0x4,
    IMMERSIVE_DOCK_ACTIVE = 0x5,
    NOT_IMMERSIVE = 0x6,
};

// Function signature for PrivateCreateCoreWindow
typedef HRESULT(CDECL* fnPrivateCreateCoreWindow)(WINDOW_TYPE WindowType, LPCWSTR pWindowTitle, INT X, INT Y, UINT uWidth, UINT uHeight, DWORD dwAttributes, HWND hOwnerWindow, REFIID riid, void** ppv);

// Stores the handle of the parent native window
HWND parentWindow = nullptr;

// Stores the core window for the UI thread
Windows::UI::Core::CoreWindow coreWindow = nullptr;

// Stores the handle of the core window
HWND coreWindowHwnd = nullptr;

// Stores the xaml framework view for the UI thread
Windows::UI::Xaml::FrameworkView frameworkView = nullptr;

Windows::UI::Xaml::Application app = nullptr;

namespace winrt::XamlHostTesting2::implementation
{
    struct App : AppT<App>
    {
        App();

		Windows::UI::Xaml::Markup::IXamlType GetXamlType(winrt::hstring const& fullName);
        Windows::UI::Xaml::Markup::IXamlType GetXamlType(Windows::UI::Xaml::Interop::TypeName const& type);
        winrt::com_array<Windows::UI::Xaml::Markup::XmlnsDefinition> GetXmlnsDefinitions();

    private:
        Windows::Foundation::Collections::IVector<Windows::UI::Xaml::Markup::IXamlMetadataProvider> m_providers = winrt::single_threaded_vector<Windows::UI::Xaml::Markup::IXamlMetadataProvider>();
    };
}

namespace winrt::XamlHostTesting2::factory_implementation
{
	struct App : AppT<App, implementation::App>
	{
	};
}