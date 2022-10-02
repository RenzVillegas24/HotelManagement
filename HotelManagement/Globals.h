
#include <winrt/Windows.Graphics.Imaging.h>
#include <winrt/Windows.Graphics.h>
#include "winrt/Windows.Security.Cryptography.h"


namespace winrt::HotelManagement::implementation
{
    Windows::Foundation::IAsyncOperation<hstring> ImageToBase64(Windows::Storage::StorageFile sourceFile, int size);
    Windows::Foundation::IAsyncOperation<Windows::UI::Xaml::Media::Imaging::BitmapImage> Base64ToImage(hstring sourceFile);

    Windows::Foundation::IAsyncOperation<Windows::UI::Xaml::Controls::ContentDialogResult> Dialog(hstring titleText, hstring contentText, hstring closeButtonText);
    Windows::Foundation::IAsyncOperation<Windows::UI::Xaml::Controls::ContentDialogResult> Dialog(hstring titleText, hstring contentText, hstring closeButtonText, hstring primaryButtonText);
    Windows::Foundation::IAsyncOperation<Windows::UI::Xaml::Controls::ContentDialogResult> Dialog(hstring titleText, hstring contentText, hstring closeButtonText, hstring primaryButtonText, hstring secondaryButtonText);

}
