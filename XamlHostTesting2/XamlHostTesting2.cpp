// XamlHostTesting2.cpp : Defines the entry point for the application.
//

#include "pch.h"
#include "XamlHostTesting2.h"
#include "App.g.cpp"

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
                     _In_opt_ HINSTANCE hPrevInstance,
                     _In_ LPWSTR    lpCmdLine,
                     _In_ int       nCmdShow)
{
    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);

    // TODO: Place code here.
    winrt::init_apartment(winrt::apartment_type::single_threaded);

    app = XamlHostTesting2::App();

    // Initialize global strings
    MyRegisterClass(hInstance);

    // Perform application initialization:
    if (!InitInstance (hInstance, nCmdShow))
    {
        return FALSE;
    }

    frameworkView.Run();
}

//
//  FUNCTION: MyRegisterClass()
//
//  PURPOSE: Registers the window class.
//
ATOM MyRegisterClass(HINSTANCE hInstance)
{
    WNDCLASSEXW wcex = { 0 };

    wcex.cbSize = sizeof(WNDCLASSEX);

    wcex.lpfnWndProc    = WndProc;
    wcex.hInstance      = hInstance;
    wcex.hIcon          = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_XAMLHOSTTESTING2));
    wcex.lpszClassName  = L"XamlHostTestingCpp";
    wcex.hCursor        = LoadCursor(nullptr, IDC_ARROW);
    wcex.hIconSm        = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

    return RegisterClassExW(&wcex);
}

//
//   FUNCTION: InitInstance(HINSTANCE, int)
//
//   PURPOSE: Saves instance handle and creates main window
//
//   COMMENTS:
//
//        In this function, we save the instance handle in a global variable and
//        create and display the main program window.
//
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
   hInst = hInstance; // Store instance handle in our global variable

   HWND hWnd = CreateWindowEx(WS_EX_NOREDIRECTIONBITMAP, L"XamlHostTestingCpp", L"XamlHost Testing (C++)", WS_OVERLAPPEDWINDOW,
      CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, nullptr, nullptr, hInstance, nullptr);

   if (!hWnd)
   {
      return FALSE;
   }

   ShowWindow(hWnd, nCmdShow);
   UpdateWindow(hWnd);

   return TRUE;
}

//
//  FUNCTION: WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  PURPOSE: Processes messages for the main window.
//
//  WM_DESTROY  - post a quit message and return
//
//
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    switch (message)
    {
    case WM_CREATE:
    {
        HMODULE windowsUILibrary = LoadLibrary(L"Windows.UI.dll");
        fnPrivateCreateCoreWindow createCoreWindow = (fnPrivateCreateCoreWindow)GetProcAddress(windowsUILibrary, MAKEINTRESOURCEA(1500));
        CREATESTRUCT* pCreateStruct = (CREATESTRUCT*)lParam;
        winrt::check_hresult(createCoreWindow(IMMERSIVE_HOSTED, L"", 0, 0, pCreateStruct->cx, pCreateStruct->cy, 0, hWnd, winrt::guid_of<Windows::UI::Core::ICoreWindow>(), winrt::put_abi(coreWindow)));
        
        frameworkView = Windows::UI::Xaml::FrameworkView();
        frameworkView.Initialize(winrt::get_activation_factory<Windows::ApplicationModel::Core::CoreApplication, XamlHostTesting2::ICoreApplicationPrivate2>().CreateNonImmersiveView());
        frameworkView.SetWindow(coreWindow);

        app.Resources(Microsoft::UI::Xaml::Controls::XamlControlsResources());

        com_ptr<ICoreWindowInterop> coreWindowInterop = coreWindow.as<ICoreWindowInterop>();
        winrt::check_hresult(coreWindowInterop->get_WindowHandle(&coreWindowHwnd));

        SetParent(coreWindowHwnd, hWnd);
        SetWindowLongPtr(coreWindowHwnd, GWL_STYLE, WS_CHILD | WS_VISIBLE);
        SetWindowPos(coreWindowHwnd, nullptr, 0, 0, pCreateStruct->cx, pCreateStruct->cy, SWP_SHOWWINDOW);

        Windows::UI::Xaml::Controls::Button button;
		button.Content(winrt::box_value(L"Hello, World!"));
		button.HorizontalAlignment(Windows::UI::Xaml::HorizontalAlignment::Center);
		button.VerticalAlignment(Windows::UI::Xaml::VerticalAlignment::Center);

        Windows::UI::Xaml::Controls::Page page;
		page.Content(button);

        Windows::UI::Xaml::Window::Current().Content(page);
		Microsoft::UI::Xaml::Controls::BackdropMaterial::SetApplyToRootOrPageBackground(page, true);
        break;
    }
    case WM_SIZE:
    case WM_DPICHANGED:
    {
        if (!coreWindowHwnd)
            break;
        RECT rect;
        GetClientRect(hWnd, &rect);
        SetWindowPos(coreWindowHwnd, nullptr, rect.left, rect.top, rect.right - rect.left, rect.bottom - rect.top, SWP_SHOWWINDOW);
        SendMessage(coreWindowHwnd, message, wParam, lParam);
        break;
    }
    case WM_ACTIVATE:
    case WM_SETTINGCHANGE:
    case WM_THEMECHANGED:
    {
        if (!coreWindowHwnd)
            break;
        SendMessage(coreWindowHwnd, message, wParam, lParam);
        break;
    }
    case WM_DESTROY:
        PostQuitMessage(0);
        break;
    default:
        return DefWindowProc(hWnd, message, wParam, lParam);
    }
    return 0;
}

namespace winrt::XamlHostTesting2::implementation
{
    App::App()
    {
        m_providers.Append(Microsoft::UI::Xaml::XamlTypeInfo::XamlControlsXamlMetaDataProvider());
    }

    Windows::UI::Xaml::Markup::IXamlType App::GetXamlType(winrt::hstring const& fullName)
    {
        for (const auto& provider : m_providers)
        {
            const auto type = provider.GetXamlType(fullName);
            if (type)
            {
                return type;
            }
        }
        return nullptr;
    }

	Windows::UI::Xaml::Markup::IXamlType App::GetXamlType(Windows::UI::Xaml::Interop::TypeName const& type)
	{
		for (const auto& provider : m_providers)
		{
			const auto xamlType = provider.GetXamlType(type);
			if (xamlType)
			{
				return xamlType;
			}
		}
		return nullptr;
	}

	winrt::com_array<Windows::UI::Xaml::Markup::XmlnsDefinition> App::GetXmlnsDefinitions()
	{
		std::list<Windows::UI::Xaml::Markup::XmlnsDefinition> definitions;
		for (const auto& provider : m_providers)
		{
			const auto xmlnsDefinitions = provider.GetXmlnsDefinitions();
			for (const auto& definition : xmlnsDefinitions)
			{
				definitions.insert(definitions.begin(), definition);
			}
		}
		return winrt::com_array<Windows::UI::Xaml::Markup::XmlnsDefinition>(definitions.begin(), definitions.end());
	}
}