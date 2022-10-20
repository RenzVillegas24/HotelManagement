#include "pch.h"
#include "Globals.h"

#include <time.h>
#include <iomanip>
#include <sstream>



#define _CRT
#define _CRT_SECURE_NO_WARNINGS

using namespace winrt;
using namespace Windows;
using namespace UI::Xaml::Media::Imaging;
using namespace Graphics::Imaging;
using namespace Storage;
using namespace Streams;
using namespace Foundation;
using namespace std;
using time_point = std::chrono::system_clock::time_point;

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




            if (mnSz >= 250)
            {
                encoder.BitmapTransform().ScaledHeight(size * ratio[1]);
                encoder.BitmapTransform().ScaledWidth(size * ratio[0]);
            } else 
                size = mnSz;
                


            BitmapBounds bounds = BitmapBounds();
            bounds.Height = (uint32_t)size;
            bounds.Width = (uint32_t)size;

            double adj = (size * ratioMxMn - size) / 2;

            //cropped ssss
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
        co_await ras.WriteAsync(buff);
        ras.Seek(0);    //reset to read the buffer

        BitmapImage img = BitmapImage();
        img.SetSourceAsync(ras);

        co_return img;
    }

    


    winrt::Windows::Foundation::IAsyncOperation<winrt::Windows::UI::Xaml::Controls::ContentDialogResult> Dialog(hstring titleText, hstring contentText, hstring closeButtonText) {
        auto title = winrt::Windows::UI::Xaml::Controls::TextBlock(); 
        title.Text(titleText);

        auto content = winrt::Windows::UI::Xaml::Controls::TextBlock(); 
        content.Text(contentText);
        content.Width(400);
        content.TextWrapping(winrt::Windows::UI::Xaml::TextWrapping::WrapWholeWords);

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
        content.Width(400);
        content.TextWrapping(winrt::Windows::UI::Xaml::TextWrapping::WrapWholeWords);

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
        content.Width(400);
        content.TextWrapping(winrt::Windows::UI::Xaml::TextWrapping::WrapWholeWords);

        winrt::Windows::UI::Xaml::Controls::ContentDialog dialog;
        dialog.Title(title);
        dialog.Content(content);
        dialog.CloseButtonText(closeButtonText);
        dialog.PrimaryButtonText(primaryButtonText);
        dialog.SecondaryButtonText(secondaryButtonText);

        co_return co_await dialog.ShowAsync();
    }


    //source: https://stackoverflow.com/questions/321849/strptime-equivalent-on-windows
    char* strptime(const char* s,
        const char* f,
        struct tm* tm) {
        // Isn't the C++ standard lib nice? std::get_time is defined such that its
        // format parameters are the exact same as strptime. Of course, we have to
        // create a string stream first, and imbue it with the current C locale, and
        // we also have to make sure we return the right things if it fails, or
        // if it succeeds, but this is still far simpler an implementation than any
        // of the versions in any of the C standard libraries.
        std::istringstream input(s);
        input.imbue(std::locale(setlocale(LC_ALL, nullptr)));
        input >> std::get_time(tm, f);
        if (input.eof()) return (char*)(s + strlen(s));
        if (input.fail()) {
            return nullptr;
        }
        return (char*)(s + input.tellg());

    }


    std::tm to_tm(const std::string_view& date) {
        std::tm t{};

        if (sscanf_s(date.data(), "%d/%d/%d",
            &t.tm_mon,
            &t.tm_mday,
            &t.tm_year
            ) != 3)
            throw std::runtime_error("Invalid date format: " + std::string(date));

        t.tm_year -= 1900;
        --t.tm_mon;
        t.tm_isdst = -1;  // guess if DST should be in effect when calling mktime
        errno = 0;
        std::mktime(&t);

        return t;
    }

    //source: https://stackoverflow.com/questions/34857119/how-to-convert-stdchronotime-point-to-string
    std::string serializeTimePoint(const time_point& time, const std::string& format)
    {
        std::time_t tt = std::chrono::system_clock::to_time_t(time);
        std::tm tm;
        localtime_s(&tm, &tt);

        char buff[70];
        if (strftime(buff, sizeof buff, "%B %e, %Y", &tm)) {
            puts(buff);
        }
        else {
            puts("strftime failed");
        }

        return buff;
    }


  
}