using System;
using System.Collections.Generic;
using System.IO;
using System.Linq;
using System.Runtime.CompilerServices;
using System.Runtime.InteropServices;
using System.Runtime.InteropServices.WindowsRuntime;
using Windows.ApplicationModel.AppService;
using Windows.Foundation;
using Windows.Foundation.Collections;
using Windows.Storage.Pickers;
using Windows.UI.Xaml;
using Windows.UI.Xaml.Controls;
using Windows.UI.Xaml.Controls.Primitives;
using Windows.UI.Xaml.Data;
using Windows.UI.Xaml.Input;
using Windows.UI.Xaml.Media;
using Windows.UI.Xaml.Navigation;
using WinRT;

// The Blank Page item template is documented at https://go.microsoft.com/fwlink/?LinkId=402352&clcid=0x409

namespace XamlHostTesting
{
    /// <summary>
    /// An empty page that can be used on its own or navigated to within a Frame.
    /// </summary>
    public sealed partial class MainPage : Page
    {
        public MainPage()
        {
            InitializeComponent();
        }

        private void Button_Click(object sender, RoutedEventArgs e)
        {
            ContentDialog contentDialog = new()
            {
                Title = "Hello World!",
                Content = $"""
                .NET Runtime: {RuntimeInformation.FrameworkDescription}
                IsDynamicCodeSupported: {RuntimeFeature.IsDynamicCodeSupported}
                """,
                CloseButtonText = "OK",
                DefaultButton = ContentDialogButton.Close,
                Style = (Style)Application.Current.Resources["DefaultContentDialogStyle"]
            };

            _ = contentDialog.ShowAsync();
        }
    }
}
