using System;
using System.Runtime.InteropServices;
using System.Runtime.InteropServices.Marshalling;

namespace XamlHostTesting;

static unsafe partial class CoreNative
{
    public enum CoreWindowType : int
    {
        IMMERSIVE_BODY = 0,
        IMMERSIVE_DOCK,
        IMMERSIVE_HOSTED,
        IMMERSIVE_TEST,
        IMMERSIVE_BODY_ACTIVE,
        IMMERSIVE_DOCK_ACTIVE,
        NOT_IMMERSIVE
    }

    [LibraryImport("Windows.UI.dll", EntryPoint = "#1500", StringMarshalling = StringMarshalling.Utf16)]
    public static partial int PrivateCreateCoreWindow(
        CoreWindowType coreWindowType,
        string windowTitle,
        int x,
        int y,
        int width,
        int height,
        uint dwAttributes,
        nint hOwnerWindow,
        Guid* riid,
        out nint pCoreWindow);

    [GuidRVAGen.Guid("79b9d5f2-879e-4b89-b798-79e47598030c")]
    public static partial Guid* IID_ICoreWindow { get; }

    [GuidRVAGen.Guid("6090202d-2843-4ba5-9b0d-fc88eecd9ce5")]
    public static partial ref readonly Guid IID_ICoreApplicationPrivate2 { get; }

    [GeneratedComInterface]
    [Guid("45d64a29-a63e-4cb6-b498-5781d298cb4f")]
    public partial interface ICoreWindowInterop
    {
        void GetWindowHandle(out nint hWnd);
    }
}