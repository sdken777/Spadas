
#include "spadas.h"

using namespace spadas;
using namespace spadas::math;

// ImageResolution
Size2D ImageResolution::size(Enum<spadas::ImageResolution> resolution)
{
    switch (resolution.value())
    {
        case ImageResolution::Value::QVGA:
            return Size2D::wh(320, 240);
        case ImageResolution::Value::VGA:
            return Size2D::wh(640, 480);
        case ImageResolution::Value::PAL720:
            return Size2D::wh(720, 576);
        case ImageResolution::Value::PAL768:
            return Size2D::wh(768, 576);
        case ImageResolution::Value::SVGA:
            return Size2D::wh(800, 600);
        case ImageResolution::Value::XGA:
            return Size2D::wh(1024, 768);
        case ImageResolution::Value::WXGA:
            return Size2D::wh(1280, 800);
        case ImageResolution::Value::SXGA:
            return Size2D::wh(1280, 1024);
        case ImageResolution::Value::SXGAPlus:
            return Size2D::wh(1400, 1050);
        case ImageResolution::Value::UXGA:
            return Size2D::wh(1600, 1200);
        case ImageResolution::Value::WSXGAPlus:
            return Size2D::wh(1680, 1050);
        case ImageResolution::Value::HD720:
            return Size2D::wh(1280, 720);
        case ImageResolution::Value::HD1080:
            return Size2D::wh(1920, 1080);
		case ImageResolution::Value::HD2K:
			return Size2D::wh(2560, 1440);
		case ImageResolution::Value::HD4K:
			return Size2D::wh(3840, 2160);
        default:
            return Size2D();
    }
}

// PixelFormat
Bool PixelFormat::isColor(Enum<spadas::PixelFormat> format)
{
    switch (format.value())
	{
		case PixelFormat::Value::ByteBGR:
			return TRUE;
        case PixelFormat::Value::ByteRGB:
			return TRUE;
		case PixelFormat::Value::ByteUYVY:
			return TRUE;
		case PixelFormat::Value::ByteYUV:
			return TRUE;
		case PixelFormat::Value::ByteGray:
			return FALSE;
        case PixelFormat::Value::ByteBool:
			return FALSE;
        case PixelFormat::Value::ByteBGRA:
            return TRUE;
        case PixelFormat::Value::ByteRGBA:
            return TRUE;
		case PixelFormat::Value::WordBGR:
			return TRUE;
		case PixelFormat::Value::WordGray:
			return FALSE;
		case PixelFormat::Value::FloatBGR:
			return TRUE;
		case PixelFormat::Value::FloatGray:
			return FALSE;
		case PixelFormat::Value::FloatHSV:
			return TRUE;
        default:
            return FALSE;
	}
}
Bool PixelFormat::hasAlpha(Enum<spadas::PixelFormat> format)
{
    switch (format.value())
	{
		case PixelFormat::Value::ByteBGR:
			return FALSE;
        case PixelFormat::Value::ByteRGB:
			return FALSE;
		case PixelFormat::Value::ByteUYVY:
			return FALSE;
		case PixelFormat::Value::ByteYUV:
			return FALSE;
		case PixelFormat::Value::ByteGray:
			return FALSE;
        case PixelFormat::Value::ByteBool:
			return FALSE;
        case PixelFormat::Value::ByteBGRA:
            return TRUE;
        case PixelFormat::Value::ByteRGBA:
            return TRUE;
		case PixelFormat::Value::WordBGR:
			return FALSE;
		case PixelFormat::Value::WordGray:
			return FALSE;
		case PixelFormat::Value::FloatBGR:
			return FALSE;
		case PixelFormat::Value::FloatGray:
			return FALSE;
		case PixelFormat::Value::FloatHSV:
			return FALSE;
        default:
            return FALSE;
	}
}
UInt PixelFormat::nChannels(Enum<PixelFormat> format)
{
    switch (format.value())
	{
		case PixelFormat::Value::ByteBGR:
			return 3;
        case PixelFormat::Value::ByteRGB:
			return 3;
		case PixelFormat::Value::ByteUYVY:
			return 2;
		case PixelFormat::Value::ByteYUV:
			return 3;
		case PixelFormat::Value::ByteGray:
			return 1;
        case PixelFormat::Value::ByteBool:
			return 1;
        case PixelFormat::Value::ByteBGRA:
            return 4;
        case PixelFormat::Value::ByteRGBA:
            return 4;
		case PixelFormat::Value::WordBGR:
			return 3;
		case PixelFormat::Value::WordGray:
			return 1;
		case PixelFormat::Value::FloatBGR:
			return 3;
		case PixelFormat::Value::FloatGray:
			return 1;
		case PixelFormat::Value::FloatHSV:
			return 3;
        default:
            return 0;
	}
}
UInt PixelFormat::bytesPerPixel(Enum<PixelFormat> format)
{
    switch (format.value())
	{
		case PixelFormat::Value::ByteBGR:
			return 3;
        case PixelFormat::Value::ByteRGB:
			return 3;
		case PixelFormat::Value::ByteUYVY:
			return 2;
		case PixelFormat::Value::ByteYUV:
			return 3;
		case PixelFormat::Value::ByteGray:
			return 1;
        case PixelFormat::Value::ByteBool:
			return 1;
        case PixelFormat::Value::ByteBGRA:
            return 4;
        case PixelFormat::Value::ByteRGBA:
            return 4;
		case PixelFormat::Value::WordBGR:
			return 6;
		case PixelFormat::Value::WordGray:
			return 2;
		case PixelFormat::Value::FloatBGR:
			return 12;
		case PixelFormat::Value::FloatGray:
			return 4;
		case PixelFormat::Value::FloatHSV:
			return 12;
        default:
            return 0;
	}
}

// PixelData
BytePixel PixelData::getBytes()
{
	SPADAS_ERROR_RETURNVAL(!ptr, BytePixel());

	switch (bytesPerPixel)
	{
	case 1:
		return BytePixel(b[0]);
	case 2:
		return BytePixel(b[0], b[1]);
	case 3:
		return BytePixel(b[0], b[1], b[2]);
	case 4:
		return BytePixel(b[0], b[1], b[2], b[3]);
	default:
		SPADAS_ERROR_MSG("Invalid bytesPerPixel");
		return BytePixel();
	}
}
WordPixel PixelData::getWords()
{
	SPADAS_ERROR_RETURNVAL(!ptr, WordPixel());

	switch (bytesPerPixel >> 1)
	{
	case 1:
		return WordPixel(w[0]);
	case 2:
		return WordPixel(w[0], w[1]);
	case 3:
		return WordPixel(w[0], w[1], w[2]);
	case 4:
		return WordPixel(w[0], w[1], w[2], w[3]);
	default:
		SPADAS_ERROR_MSG("Invalid bytesPerPixel");
		return WordPixel();
	}
}
FloatPixel PixelData::getFloats()
{
	SPADAS_ERROR_RETURNVAL(!ptr, FloatPixel());

	switch (bytesPerPixel >> 2)
	{
	case 1:
		return FloatPixel(f[0]);
	case 2:
		return FloatPixel(f[0], f[1]);
	case 3:
		return FloatPixel(f[0], f[1], f[2]);
	case 4:
		return FloatPixel(f[0], f[1], f[2], f[3]);
	default:
		SPADAS_ERROR_MSG("Invalid bytesPerPixel");
		return FloatPixel();
	}
}
void PixelData::set(BytePixel bytes)
{
	SPADAS_ERROR_RETURN(!ptr);

	switch (bytesPerPixel)
	{
	case 1:
		b[0] = bytes.ch1;
		break;
	case 2:
		b[0] = bytes.ch1;
		b[1] = bytes.ch2;
		break;
	case 3:
		b[0] = bytes.ch1;
		b[1] = bytes.ch2;
		b[2] = bytes.ch3;
		break;
	case 4:
		b[0] = bytes.ch1;
		b[1] = bytes.ch2;
		b[2] = bytes.ch3;
		b[3] = bytes.ch4;
		break;
	default:
		SPADAS_ERROR_MSG("Invalid bytesPerPixel");
		break;
	}
}
void PixelData::set(WordPixel words)
{
	SPADAS_ERROR_RETURN(!ptr);

	switch (bytesPerPixel >> 1)
	{
	case 1:
		w[0] = words.ch1;
		break;
	case 2:
		w[0] = words.ch1;
		w[1] = words.ch2;
		break;
	case 3:
		w[0] = words.ch1;
		w[1] = words.ch2;
		w[2] = words.ch3;
		break;
	case 4:
		w[0] = words.ch1;
		w[1] = words.ch2;
		w[2] = words.ch3;
		w[3] = words.ch4;
		break;
	default:
		SPADAS_ERROR_MSG("Invalid bytesPerPixel");
		break;
	}
}
void PixelData::set(FloatPixel floats)
{
	SPADAS_ERROR_RETURN(!ptr);

	switch (bytesPerPixel >> 2)
	{
	case 1:
		f[0] = floats.ch1;
		break;
	case 2:
		f[0] = floats.ch1;
		f[1] = floats.ch2;
		break;
	case 3:
		f[0] = floats.ch1;
		f[1] = floats.ch2;
		f[2] = floats.ch3;
		break;
	case 4:
		f[0] = floats.ch1;
		f[1] = floats.ch2;
		f[2] = floats.ch3;
		f[3] = floats.ch4;
		break;
	default:
		SPADAS_ERROR_MSG("Invalid bytesPerPixel");
		break;
	}
}