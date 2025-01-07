#pragma once

namespace winrt::XamlHostTesting2::implementation
{
	template<typename D, typename... I>
	struct App_baseWithProvider : public App_base<D, Windows::UI::Xaml::Markup::IXamlMetadataProvider> {
		using IXamlType = Windows::UI::Xaml::Markup::IXamlType;


		IXamlType GetXamlType(const Windows::UI::Xaml::Interop::TypeName& type)
		{
			return AppProvider()->GetXamlType(type);
		}


		IXamlType GetXamlType(const hstring& fullName)
		{
			return AppProvider()->GetXamlType(fullName);
		}


		com_array<Windows::UI::Xaml::Markup::XmlnsDefinition> GetXmlnsDefinitions()
		{
			return AppProvider()->GetXmlnsDefinitions();
		}


	private:
		bool _contentLoaded{ false };
		com_ptr<XamlMetaDataProvider> _appProvider;
		com_ptr<XamlMetaDataProvider> AppProvider()
		{
			if (!_appProvider)
			{
				_appProvider = make_self<XamlMetaDataProvider>();
			}
			return _appProvider;
		}
	};


	template<typename D, typename... I>
	using AppT2 = App_baseWithProvider<D, I...>;
}