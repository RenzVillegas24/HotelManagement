#include "pch.h"
#include "Globals.h"

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


            ratio[decoder.PixelHeight() > decoder.PixelWidth()] = mxSz / mnSz;



            encoder.BitmapTransform().ScaledHeight(size * ratio[1]);
            encoder.BitmapTransform().ScaledWidth(size * ratio[0]);


            BitmapBounds bounds = BitmapBounds();
            bounds.Height = size;
            bounds.Width = size;

            int adj = mxSz * (mxSz / mnSz) - mxSz;

            if (decoder.PixelWidth() > decoder.PixelHeight()) {
                bounds.X = (size * adj) / 2;
                bounds.Y = 0;
            }
            else {
                bounds.X = 0;
                bounds.Y = (size * adj) / 2;
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
}