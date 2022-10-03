#include "pch.h"
#include "Globals.h"


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

    IAsyncOperation<hstring> ImageToBase64(StorageFile sourceFile, int size) {

        BitmapDecoder decoder = co_await BitmapDecoder::CreateAsync(
            co_await sourceFile.OpenAsync(FileAccessMode::Read)
        );



        InMemoryRandomAccessStream ras = InMemoryRandomAccessStream();
        {
            BitmapEncoder encoder = co_await BitmapEncoder::CreateForTranscodingAsync(
                ras, decoder
            );


            double mxSz = max(decoder.PixelWidth(), decoder.PixelHeight()),
                    mnSz = min(decoder.PixelWidth(), decoder.PixelHeight());

            double ratio[] = { 1,1 };

            double ratioMxMn = mxSz / mnSz;


            ratio[decoder.PixelHeight() > decoder.PixelWidth()] = ratioMxMn;



            encoder.BitmapTransform().ScaledHeight(size * ratio[1]);
            encoder.BitmapTransform().ScaledWidth(size * ratio[0]);


            BitmapBounds bounds = BitmapBounds();
            bounds.Height = (uint32_t)size;
            bounds.Width = (uint32_t)size;

            double adj = (size * ratioMxMn - size) / 2;

            if (decoder.PixelWidth() > decoder.PixelHeight()) {
                bounds.X = uint32_t(adj);
                bounds.Y = uint32_t(0);
            }
            else {
                bounds.X = uint32_t(0);
                bounds.Y = uint32_t(adj);
            }

            encoder.BitmapTransform().Bounds(bounds);

            co_await encoder.FlushAsync();

        }


        Buffer buff(ras.Size());

        co_await ras.ReadAsync(buff, ras.Size(), InputStreamOptions::None);

        co_return Security::Cryptography::CryptographicBuffer::EncodeToBase64String(buff);
    }

    IAsyncOperation<BitmapImage> Base64ToImage(hstring sourceFile) {
        auto buff = Security::Cryptography::CryptographicBuffer::DecodeFromBase64String(sourceFile);

        InMemoryRandomAccessStream ras = InMemoryRandomAccessStream();

        ras.WriteAsync(buff);


        BitmapImage img = BitmapImage();
        img.SetSourceAsync(ras);

        co_return img;
    }

    


    winrt::Windows::Foundation::IAsyncOperation<winrt::Windows::UI::Xaml::Controls::ContentDialogResult> Dialog(hstring titleText, hstring contentText, hstring closeButtonText) {
        auto title = winrt::Windows::UI::Xaml::Controls::TextBlock(); 
        title.Text(titleText);

        auto content = winrt::Windows::UI::Xaml::Controls::TextBlock(); 
        content.Text(contentText);

        winrt::Windows::UI::Xaml::Controls::ContentDialog dialog; 
        dialog.Title(title); 
        dialog.Content(content);
        dialog.CloseButtonText(closeButtonText);

        co_return co_await dialog.ShowAsync();
    }

    winrt::Windows::Foundation::IAsyncOperation<winrt::Windows::UI::Xaml::Controls::ContentDialogResult> Dialog(hstring titleText, hstring contentText, hstring closeButtonText, hstring primaryButtonText) {
        auto title = winrt::Windows::UI::Xaml::Controls::TextBlock(); 
        title.Text(titleText);

        auto content = winrt::Windows::UI::Xaml::Controls::TextBlock(); 
        content.Text(contentText);

        winrt::Windows::UI::Xaml::Controls::ContentDialog dialog; 
        dialog.Title(title); 
        dialog.Content(content);
        dialog.CloseButtonText(closeButtonText);
        dialog.PrimaryButtonText(primaryButtonText);

        co_return co_await dialog.ShowAsync();
    }

    winrt::Windows::Foundation::IAsyncOperation<winrt::Windows::UI::Xaml::Controls::ContentDialogResult> Dialog(hstring titleText, hstring contentText, hstring closeButtonText, hstring primaryButtonText, hstring secondaryButtonText) {
        auto title = winrt::Windows::UI::Xaml::Controls::TextBlock();
        title.Text(titleText);

        auto content = winrt::Windows::UI::Xaml::Controls::TextBlock();
        content.Text(contentText);

        winrt::Windows::UI::Xaml::Controls::ContentDialog dialog;
        dialog.Title(title);
        dialog.Content(content);
        dialog.CloseButtonText(closeButtonText);
        dialog.PrimaryButtonText(primaryButtonText);
        dialog.SecondaryButtonText(secondaryButtonText);

        co_return co_await dialog.ShowAsync();
    }
}