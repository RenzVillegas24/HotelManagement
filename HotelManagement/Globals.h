
#include <winrt/Windows.Graphics.Imaging.h>
#include <winrt/Windows.Graphics.h>
#include "winrt/Windows.Security.Cryptography.h"

//for debugging
//#define DEBUG


namespace winrt::HotelManagement::implementation
{
  
    Windows::Foundation::IAsyncOperation<hstring> ImageToBase64(Windows::Storage::StorageFile sourceFile, int size);
    Windows::Foundation::IAsyncOperation<Windows::UI::Xaml::Media::Imaging::BitmapImage> Base64ToImage(hstring sourceFile);

    Windows::Foundation::IAsyncOperation<Windows::UI::Xaml::Controls::ContentDialogResult> Dialog(hstring titleText, hstring contentText, hstring closeButtonText);
    Windows::Foundation::IAsyncOperation<Windows::UI::Xaml::Controls::ContentDialogResult> Dialog(hstring titleText, hstring contentText, hstring closeButtonText, hstring primaryButtonText);
    Windows::Foundation::IAsyncOperation<Windows::UI::Xaml::Controls::ContentDialogResult> Dialog(hstring titleText, hstring contentText, hstring closeButtonText, hstring primaryButtonText, hstring secondaryButtonText);

    char* strptime(const char* s, const char* f, struct tm* tm);
    std::string serializeTimePoint(const std::chrono::system_clock::time_point& time, const std::string& format);
    std::tm to_tm(const std::string_view& date);
}
