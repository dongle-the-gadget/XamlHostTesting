#pragma once
#include <windef.h>

enum ACCENT_STATE : INT
{
	ACCENT_ENABLE_HOSTBACKDROP = 5
};

struct ACCENT_POLICY
{
	ACCENT_STATE    AccentState;
	UINT            AccentFlags;
	COLORREF        GradientColor;
	LONG            AnimationId;
};

enum WINDOWCOMPOSITIONATTRIB : INT
{
	WCA_ACCENT_POLICY = 19
};

struct WINDOWCOMPOSITIONATTRIBDATA
{
	WINDOWCOMPOSITIONATTRIB    Attrib;
	LPVOID                     pvData;
	UINT                       cbData;
};

typedef BOOL(WINAPI* pfnSetWindowCompositionAttribute)(HWND, const WINDOWCOMPOSITIONATTRIBDATA*);