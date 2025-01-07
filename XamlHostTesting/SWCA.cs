using System.Runtime.InteropServices;
using Windows.Win32.Foundation;

namespace XamlHostTesting;

internal unsafe class SWCA
{
    internal enum WindowCompositionAttribute
    {
        WCA_ACCENT_POLICY = 19,
    }

    internal enum AccentState
    {
        ACCENT_ENABLE_HOSTBACKDROP = 5, // RS5 1809
    }

    [StructLayout(LayoutKind.Sequential)]
    internal struct WINCOMPATTRDATA
    {
        public WindowCompositionAttribute attribute;
        public void* pData;
        public uint cbSize;
    }

    internal struct ACCENTPOLICY
    {
        public AccentState accentState;
        public uint accentFlags;
        public uint gradientColor;
        public int animationId;
    }

    [DllImport("user32")]
    internal static extern int SetWindowCompositionAttribute(HWND hwnd, WINCOMPATTRDATA* pAttrData);
}
