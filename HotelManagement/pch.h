#pragma once
#include <windows.h>
#include <unknwn.h>
#include <restrictederrorinfo.h>
#include <hstring.h>

#include <winrt/Windows.Foundation.h>
#include <winrt/Windows.Foundation.Collections.h>
#include <winrt/Windows.ApplicationModel.Activation.h>
#include "winrt/Windows.Storage.h"
#include "winrt/Windows.Storage.Streams.h"

#include <winrt/Windows.UI.Xaml.h>
#include <winrt/Windows.UI.Xaml.Controls.h>
#include <winrt/Windows.UI.Xaml.Controls.Primitives.h>
#include <winrt/Windows.UI.Xaml.Data.h>
#include <winrt/Windows.UI.Xaml.Interop.h>
#include <winrt/Windows.UI.Xaml.Markup.h>
#include <winrt/Windows.UI.Xaml.Navigation.h>

#include "winrt/Windows.UI.Xaml.Input.h"
#include <winrt/Windows.UI.Xaml.Shapes.h>
#include "winrt/Windows.UI.Popups.h"

#include "winrt/Windows.UI.Xaml.Media.Animation.h"
#include <winrt/Windows.UI.Core.h>
#include "winrt/Windows.UI.Input.h"
#include <winrt/Windows.UI.Xaml.Media.Imaging.h>
#include <winrt/Windows.Graphics.h>
#include <winrt/Windows.Graphics.Imaging.h>
#include <winrt/Windows.UI.Xaml.Documents.h>

#include "winrt/Microsoft.UI.Xaml.Automation.Peers.h"
#include "winrt/Microsoft.UI.Xaml.Controls.h"
#include "winrt/Microsoft.UI.Xaml.Controls.Primitives.h"
#include "winrt/Microsoft.UI.Xaml.Media.h"
#include "winrt/Microsoft.UI.Xaml.XamlTypeInfo.h"





//Tutorial installing libaries: https://stackoverflow.com/questions/23882112/how-to-add-static-libraries-to-a-visual-studio-project
//Source:   https://stackoverflow.com/questions/56993480/how-to-connect-to-sql-server-from-c-winrt-uwp-app
#include "sqlite_modern_cpp.h"

namespace winrt::HotelManagement::implementation
{
    std::string dbPath(std::string);
    std::string dbPath();
    winrt::Windows::Storage::IStorageFolder localPath();
    winrt::hstring dbPathH();

    bool isLoggedIn(void);
    bool initDatabase(void);
    bool isHeaderSticky(bool value);
    bool isHeaderSticky();

    std::string dataPath;
}

