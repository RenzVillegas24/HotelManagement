
#include <winrt/Windows.Graphics.Imaging.h>
#include <winrt/Windows.Graphics.h>
#include "winrt/Windows.Security.Cryptography.h"

using namespace winrt;
using namespace Windows;
using namespace UI::Xaml::Media::Imaging;
using namespace Graphics::Imaging;
using namespace Storage;
using namespace Streams;
using namespace Foundation;
using namespace std;


namespace winrt::HotelManagement::implementation
{
    IAsyncOperation<hstring> ImageToBase64(StorageFile sourceFile, int size);
    IAsyncOperation<BitmapImage> Base64ToImage(hstring sourceFile);

}
