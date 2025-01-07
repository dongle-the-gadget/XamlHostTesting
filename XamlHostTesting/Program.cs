using System;
using Windows.Win32.Foundation;
using Windows.Win32.UI.WindowsAndMessaging;
using WinRT;
using Windows.UI.Core;
using Windows.ApplicationModel.Core;
using Windows.UI.Xaml;
using System.Runtime.InteropServices;
using System.Runtime.CompilerServices;
using Windows.UI.Xaml.Controls;

using static XamlHostTesting.CoreNative;
using static Windows.Win32.PInvoke;
using Microsoft.UI.Xaml.Controls;

namespace XamlHostTesting;

static unsafe class Program
{
    static CoreWindow coreWindow;
    static FrameworkView frameworkView;
    static HWND coreWindowHwnd = HWND.Null;
    static Application app = null;

    [STAThread]
    static void Main()
    {
        ComWrappersSupport.InitializeComWrappers();

        app = new App();

        HINSTANCE hInstance = GetModuleHandle(new PCWSTR());

        const string className = "XamlHostTestingDotNet";

        fixed (char* lpClassName = className)
        {
            WNDCLASSEXW wc = new()
            {
                cbSize = (uint)sizeof(WNDCLASSEXW),
                hInstance = hInstance,
                lpfnWndProc = &WndProc,
                lpszClassName = lpClassName,
            };

            if (RegisterClassEx(&wc) == 0)
            {
                return;
            }
        }

        HWND hWnd = CreateWindowEx(
            WINDOW_EX_STYLE.WS_EX_NOREDIRECTIONBITMAP,
            className,
            "XamlHost Testing (.NET)",
            WINDOW_STYLE.WS_OVERLAPPEDWINDOW,
            CW_USEDEFAULT,
            CW_USEDEFAULT,
            CW_USEDEFAULT,
            CW_USEDEFAULT,
            HWND.Null,
            HMENU.Null,
            hInstance,
            null);

        if (hWnd == HWND.Null)
        {
            return;
        }

        SWCA.ACCENTPOLICY accent = new()
        {
            accentFlags = 0,
            accentState = SWCA.AccentState.ACCENT_ENABLE_HOSTBACKDROP,
            gradientColor = 0,
            animationId = 0
        };

        SWCA.WINCOMPATTRDATA data = new()
        {
            attribute = SWCA.WindowCompositionAttribute.WCA_ACCENT_POLICY,
            pData = &accent,
            cbSize = (uint)sizeof(SWCA.ACCENTPOLICY)
        };

        SWCA.SetWindowCompositionAttribute(hWnd, &data);

        ShowWindow(hWnd, SHOW_WINDOW_CMD.SW_SHOWNORMAL);
        UpdateWindow(hWnd);

        frameworkView.Run();
    }

    [UnmanagedCallersOnly(CallConvs = [typeof(CallConvStdcall)])]
    static LRESULT WndProc(HWND hWnd, uint msg, WPARAM wParam, LPARAM lParam)
    {
        switch (msg)
        {
            case WM_CREATE:
                {
                    CREATESTRUCTW* createStruct = (CREATESTRUCTW*)lParam.Value;

                    ExceptionHelpers.ThrowExceptionForHR(
                        PrivateCreateCoreWindow(
                            CoreWindowType.IMMERSIVE_HOSTED,
                            "",
                            0, 0, 0, 0,
                            0,
                            hWnd,
                            IID_ICoreWindow,
                            out nint pCoreWindow));

                    coreWindow = CoreWindow.FromAbi(pCoreWindow);

                    Marshal.Release(pCoreWindow);

                    nint pCoreApplicationView;
                    using IObjectReference coreAppPrivate2 = ActivationFactory.Get("Windows.ApplicationModel.Core.CoreApplication", IID_ICoreApplicationPrivate2);
                    {
                        ExceptionHelpers.ThrowExceptionForHR(((delegate* unmanaged<nint, nint*, int>)(*(void***)coreAppPrivate2.ThisPtr)[8])(coreAppPrivate2.ThisPtr, &pCoreApplicationView));
                    }
                    CoreApplicationView coreApplicationView = CoreApplicationView.FromAbi(pCoreApplicationView);

                    frameworkView = new FrameworkView();
                    frameworkView.Initialize(coreApplicationView);
                    frameworkView.SetWindow(coreWindow);

                    ICoreWindowInterop coreWindowInterop = (ICoreWindowInterop)(object)coreWindow;
                    coreWindowInterop.GetWindowHandle(out nint cwHwnd);
                    coreWindowHwnd = new HWND(cwHwnd);

                    SetParent(coreWindowHwnd, hWnd);
                    SetWindowLong(coreWindowHwnd, WINDOW_LONG_PTR_INDEX.GWL_STYLE, (int)(WINDOW_STYLE.WS_CHILD | WINDOW_STYLE.WS_VISIBLE));
                    SetWindowPos(coreWindowHwnd, HWND.Null, 0, 0, createStruct->cx, createStruct->cy, SET_WINDOW_POS_FLAGS.SWP_SHOWWINDOW);

                    Frame frame = new();
                    frame.Navigate(typeof(MainPage));
                    BackdropMaterial.SetApplyToRootOrPageBackground(frame, true);

                    Window.Current.Content = frame;

                    break;
                }
            case WM_SIZE:
            case WM_DPICHANGED:
                {
                    if (coreWindowHwnd == HWND.Null)
                        break;

                    RECT rect;
                    GetClientRect(hWnd, &rect);

                    SetWindowPos(coreWindowHwnd, HWND.Null, rect.left, rect.top, rect.right - rect.left, rect.bottom - rect.top, SET_WINDOW_POS_FLAGS.SWP_SHOWWINDOW);
                    SendMessage(coreWindowHwnd, msg, wParam, lParam);
                    break;
                }
            case WM_ACTIVATE:
            case WM_SETTINGCHANGE:
            case WM_THEMECHANGED:
                {
                    if (coreWindowHwnd == HWND.Null)
                        break;
                    SendMessage(coreWindowHwnd, msg, wParam, lParam);
                    break;
                }
            case WM_CLOSE:
                DestroyWindow(hWnd);
                break;
            case WM_DESTROY:
                PostQuitMessage(0);
                break;
        }
        return DefWindowProc(hWnd, msg, wParam, lParam);
    }
}