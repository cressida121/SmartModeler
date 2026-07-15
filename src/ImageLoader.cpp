#include <vector>
#include <string>

#include <windows.h>
#include <objidl.h>
#include <gdiplus.h>

#pragma comment (lib,"Gdiplus.lib")

using namespace Gdiplus;

static int GetEncoderClsid(const WCHAR *format, CLSID *pClsid)
{
	UINT  num = 0;          // number of image encoders
	UINT  size = 0;         // size of the image encoder array in bytes

	ImageCodecInfo *pImageCodecInfo = NULL;

	GetImageEncodersSize(&num, &size);
	if (size == 0)
		return -1;  // Failure

	pImageCodecInfo = (ImageCodecInfo *)(malloc(size));
	if (pImageCodecInfo == NULL)
		return -1;  // Failure

	GetImageEncoders(num, size, pImageCodecInfo);

	for (UINT j = 0; j < num; ++j)
	{
		if (wcscmp(pImageCodecInfo[j].MimeType, format) == 0)
		{
			*pClsid = pImageCodecInfo[j].Clsid;
			free(pImageCodecInfo);
			return j;  // Success
		}
	}

	free(pImageCodecInfo);
	return -1;  // Failure
}

static uint32_t PixelBytesCount(PixelFormat pixelFormat) {

	switch (pixelFormat)
	{
	case PixelFormat8bppIndexed:		return 1u;
	case PixelFormat16bppARGB1555:		return 2u;
	case PixelFormat16bppGrayScale:		return 2u;
	case PixelFormat16bppRGB555:		return 2u;
	case PixelFormat16bppRGB565:		return 2u;
	case PixelFormat24bppRGB:			return 3u;
	case PixelFormat32bppARGB:			return 4u;
	case PixelFormat32bppCMYK:			return 4u;
	case PixelFormat32bppPARGB:			return 4u;
	case PixelFormat32bppRGB:			return 4u;
	case PixelFormat48bppRGB:			return 6u;
	case PixelFormat64bppARGB:			return 8u;
	case PixelFormat64bppPARGB:			return 8u;
	default: 							return 0u;
	}
}


/*
image/bmp
image/jpeg
image/gif
image/tiff
image/png
*/
std::vector<uint8_t> GetImagePixels(std::wstring file, uint32_t &width, uint32_t &height, uint32_t &pixelByteSize) {

	GdiplusStartupInput gdiplusStartupInput;
	ULONG_PTR gdiplusToken;
	GdiplusStartup(&gdiplusToken, &gdiplusStartupInput, NULL);

	Image *pImage = Image::FromFile(file.c_str());
	std::vector<uint8_t> result;

	if (pImage) {

		width = pImage->GetWidth();
		height = pImage->GetHeight();
		pixelByteSize = PixelBytesCount(pImage->GetPixelFormat());

		const size_t imageByteSize = width * height * pixelByteSize;
		HGLOBAL hGlobal = nullptr;

		hGlobal = GlobalAlloc(GHND, imageByteSize);
		if (hGlobal) {

			IStream *pStream = nullptr;
			HRESULT hresult = CreateStreamOnHGlobal(hGlobal,TRUE, &pStream);

			if (SUCCEEDED(hresult)) {

				auto delimIter = file.rbegin();
				while (*delimIter != L'.') {
					delimIter++;
				}

				const std::wstring fileFormat(delimIter.base(), file.end());
				std::wstring encoderFormat(L"image/");

				encoderFormat.append(fileFormat);

				CLSID clsid;
				if (GetEncoderClsid(encoderFormat.c_str(), &clsid) == -1) {
					return result;
				}

				pImage->Save(pStream, &clsid);

				Bitmap *pBitmap = Bitmap::FromStream(pStream);

				if (pBitmap) {

					const Rect rect{ 0, 0, static_cast<int>(width), static_cast<int>(height) };
					BitmapData lockedBitmapData = {};
					Status lockStatus = pBitmap->LockBits(&rect, ImageLockModeRead, pImage->GetPixelFormat(), &lockedBitmapData);

					if (lockStatus == Status::Ok) {
						result.resize(imageByteSize);
						memcpy(result.data(), lockedBitmapData.Scan0, imageByteSize);
					}

					pBitmap->UnlockBits(&lockedBitmapData);
					DeleteObject(pBitmap);
				}
			}
			pStream->Release();
		}
		DeleteObject(pImage);
	}
	return result;
}
