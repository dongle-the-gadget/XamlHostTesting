#pragma once

#include "App.g.h"
#include "App.base.h"

namespace winrt::XamlHostTesting2::implementation
{
    struct App : AppT2<App>
    {
        App()
        { }
    };
}

namespace winrt::XamlHostTesting2::factory_implementation
{
    struct App : AppT<App, implementation::App>
    {
    };
}
