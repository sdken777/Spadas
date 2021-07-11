
#include "spadas.h"

namespace spadas
{
	class ImageMetaVars : public Vars
	{
    public:
		Enum<AspectRatio> aspectRatio;
		Enum<FrameRate> frameRate;
		Enum<InterlaceMode> interlaceMode;
		Enum<MergeMode> mergeMode;
		TimeCode timeCode;
	};
}

using namespace spadas;
using namespace spadas::math;

// AspectRatio
Bool AspectRatio::isValid(Int val)
{
    return val >= AspectRatio::_1_1 && val <= AspectRatio::_40_33;
}
String AspectRatio::toString(Int val)
{
    switch (val)
	{
		case AspectRatio::_1_1:
			return "1:1";
		case AspectRatio::_4_3:
			return "4:3";
		case AspectRatio::_10_11:
			return "10:11";
		case AspectRatio::_12_11:
			return "12:11";
		case AspectRatio::_16_11:
			return "16:11";
		case AspectRatio::_40_33:
			return "40:33";
        default:
            return String();
	}
}
Int AspectRatio::defaultValue()
{
	return AspectRatio::_1_1;
}

// FrameRate
Bool FrameRate::isValid(Int val)
{
    return val >= FrameRate::Unknown && val <= FrameRate::_60;
}
String FrameRate::toString(Int val)
{
    switch (val)
	{
		case FrameRate::Unknown:
			return "Unknown frame rate";
		case FrameRate::_23_98:
			return "23.98[fps]";
		case FrameRate::_24:
			return "24[fps]";
		case FrameRate::_25:
			return "25[fps]";
		case FrameRate::_29_97:
			return "29.97[fps]";
		case FrameRate::_30:
			return "30[fps]";
		case FrameRate::_50:
			return "50[fps]";
		case FrameRate::_59_94:
			return "59.94[fps]";
		case FrameRate::_60:
			return "60[fps]";
        default:
            return String();
	}
}
Int FrameRate::defaultValue()
{
	return FrameRate::Unknown;
}
Float FrameRate::rate(Enum<FrameRate> frameRate)
{
    switch (frameRate.value())
	{
		case FrameRate::Unknown:
			return FINF;
		case FrameRate::_23_98:
			return 24.0f / 1001;
		case FrameRate::_24:
			return 24.0f;
		case FrameRate::_25:
			return 25.0f;
		case FrameRate::_29_97:
			return 30.0f / 1001;
		case FrameRate::_30:
			return 30.0f;
		case FrameRate::_50:
			return 50.0f;
		case FrameRate::_59_94:
			return 60.0f / 1001;
		case FrameRate::_60:
			return 60.0f;
        default:
            return FINF;
	}
}
Float FrameRate::period(Enum<FrameRate> frameRate)
{
    Float rate = FrameRate::rate(frameRate);
    if (rate == FINF) return FINF;
    else return 1000.0f / rate;
}

// InterlaceMode
Bool InterlaceMode::isValid(Int val)
{
    return val >= InterlaceMode::Progressive && val <= InterlaceMode::LowerFirst;
}
String InterlaceMode::toString(Int val)
{
    switch (val)
	{
		case InterlaceMode::Progressive:
			return "Progressive";
		case InterlaceMode::UpperFirst:
			return "Interlaced (Upper field first)";
		case InterlaceMode::LowerFirst:
			return "Interlaced (Lower field first)";
        default:
            return String();
	}
}
Int InterlaceMode::defaultValue()
{
	return InterlaceMode::Progressive;
}
Bool InterlaceMode::isInterlaced(Enum<InterlaceMode> mode)
{
    switch (mode.value())
	{
		case InterlaceMode::Progressive:
			return FALSE;
		case InterlaceMode::UpperFirst:
			return TRUE;
		case InterlaceMode::LowerFirst:
			return TRUE;
        default:
            return FALSE;
	}
}

// MergeMode
Bool MergeMode::isValid(Int val)
{
    return val >= MergeMode::Normal && val <= MergeMode::LineByLineRL;
}
String MergeMode::toString(Int val)
{
    switch (val)
	{
		case MergeMode::Normal:
			return "Normal";
		case MergeMode::LeftEye:
			return "Left image";
		case MergeMode::RightEye:
			return "Right image";
		case MergeMode::HalfSideBySide:
			return "Side by side (Half)";
		case MergeMode::FullSideBySide:
			return "Side by side (Full)";
		case MergeMode::LineByLineLR:
			return (String)"Line by line (Upper field is left image)";
		case MergeMode::LineByLineRL:
			return (String)"Line by line (Upper field is right image)";
        default:
            return String();
	}
}
Int MergeMode::defaultValue()
{
	return MergeMode::Normal;
}
Bool MergeMode::is3DMerged(Enum<MergeMode> mode)
{
    switch (mode.value())
	{
		case MergeMode::Normal:
			return FALSE;
		case MergeMode::LeftEye:
			return FALSE;
		case MergeMode::RightEye:
			return FALSE;
		case MergeMode::HalfSideBySide:
			return TRUE;
		case MergeMode::FullSideBySide:
			return TRUE;
		case MergeMode::LineByLineLR:
			return TRUE;
		case MergeMode::LineByLineRL:
			return TRUE;
        default:
            return FALSE;
	}
}

// TimeCode
TimeCode::TimeCode() : hour(0), minute(0), second(0), frame(0)
{
}

TimeCode::TimeCode(UInt hour0, UInt minute0, UInt second0, UInt frame0) : hour(hour0), minute(minute0), second(second0), frame(frame0)
{
}

Bool TimeCode::operator ==(TimeCode time)
{
	return hour == time.hour && minute == time.minute && second == time.second && frame == time.frame;
}

Bool TimeCode::operator !=(TimeCode time)
{
	return !(hour == time.hour && minute == time.minute && second == time.second && frame == time.frame);
}

String TimeCode::toString()
{
	return String(hour, 2) + ":" + String(minute, 2) + ":" + String(second, 2) + "." + String(frame, 3);
}

// ColorRGB
ColorRGB::ColorRGB() : r(0), g(0), b(0)
{
}

ColorRGB::ColorRGB(Byte r0, Byte g0, Byte b0) : r(r0), g(g0), b(b0)
{
}

Bool ColorRGB::operator ==(ColorRGB color)
{
	return r == color.r && g == color.g && b == color.b;
}

Bool ColorRGB::operator !=(ColorRGB color)
{
	return !(r == color.r && g == color.g && b == color.b);
}

String ColorRGB::toString()
{
	return (String)r + "," + g + "," + b;
}

// ImageResolution
Bool ImageResolution::isValid(Int val)
{
    return val >= ImageResolution::QVGA && val <= ImageResolution::HD1080;
}

String ImageResolution::toString(Int val)
{
    switch (val)
    {
        case ImageResolution::QVGA:
            return "QVGA (320x240)";
        case ImageResolution::VGA:
            return "VGA (640x480)";
        case ImageResolution::PAL720:
            return "PAL (720x576)";
        case ImageResolution::PAL768:
            return "PAL (768x576)";
        case ImageResolution::SVGA:
            return "SVGA (800x600)";
        case ImageResolution::XGA:
            return "XGA (1024x768)";
        case ImageResolution::WXGA:
            return "WXGA (1280x800)";
        case ImageResolution::SXGA:
            return "SXGA (1280x1024)";
        case ImageResolution::SXGAPlus:
            return "SXGA+ (1400x1050)";
        case ImageResolution::UXGA:
            return "UXGA (1600x1200)";
        case ImageResolution::WSXGAPlus:
            return "WSXGA+ (1680x1050)";
        case ImageResolution::HD720:
            return "HD720 (1280x720)";
        case ImageResolution::HD1080:
            return "HD1080 (1920x1080)";
        default:
            return String();
    }
}
Int ImageResolution::defaultValue()
{
	return ImageResolution::VGA;
}
Size2D ImageResolution::size(Enum<spadas::ImageResolution> resolution)
{
    switch (resolution.value())
    {
        case ImageResolution::QVGA:
            return Size2D::wh(320, 240);
        case ImageResolution::VGA:
            return Size2D::wh(640, 480);
        case ImageResolution::PAL720:
            return Size2D::wh(720, 576);
        case ImageResolution::PAL768:
            return Size2D::wh(768, 576);
        case ImageResolution::SVGA:
            return Size2D::wh(800, 600);
        case ImageResolution::XGA:
            return Size2D::wh(1024, 768);
        case ImageResolution::WXGA:
            return Size2D::wh(1280, 800);
        case ImageResolution::SXGA:
            return Size2D::wh(1280, 1024);
        case ImageResolution::SXGAPlus:
            return Size2D::wh(1400, 1050);
        case ImageResolution::UXGA:
            return Size2D::wh(1600, 1200);
        case ImageResolution::WSXGAPlus:
            return Size2D::wh(1680, 1050);
        case ImageResolution::HD720:
            return Size2D::wh(1280, 720);
        case ImageResolution::HD1080:
            return Size2D::wh(1920, 1080);
        default:
            return Size2D();
    }
}

// PixelFormat
Bool PixelFormat::isValid(Int val)
{
    return (val >= PixelFormat::ByteBGR && val <= PixelFormat::ByteRGBA) ||
    (val >= PixelFormat::WordBGR && val <= PixelFormat::WordGray) ||
    (val >= PixelFormat::FloatBGR && val <= PixelFormat::FloatHSV);
}
String PixelFormat::toString(Int val)
{
    switch (val)
	{
		case PixelFormat::ByteBGR:
			return "ByteBGR";
        case PixelFormat::ByteRGB:
			return "ByteRGB";
		case PixelFormat::ByteUYVY:
			return "ByteUYVY";
		case PixelFormat::ByteYUV:
			return "ByteYUV";
		case PixelFormat::ByteGray:
			return "ByteGray";
        case PixelFormat::ByteBool:
			return "ByteBool";
        case PixelFormat::ByteBGRA:
            return "ByteBGRA";
        case PixelFormat::ByteRGBA:
            return "ByteRGBA";
		case PixelFormat::WordBGR:
			return "WordBGR";
		case PixelFormat::WordGray:
			return "WordGray";
		case PixelFormat::FloatBGR:
			return "FloatBGR";
		case PixelFormat::FloatGray:
			return "FloatGray";
		case PixelFormat::FloatHSV:
			return "FloatHSV";
        default:
            return String();
	}
}
Int PixelFormat::defaultValue()
{
	return PixelFormat::ByteBGR;
}
Bool PixelFormat::isColor(Enum<spadas::PixelFormat> format)
{
    switch (format.value())
	{
		case PixelFormat::ByteBGR:
			return TRUE;
        case PixelFormat::ByteRGB:
			return TRUE;
		case PixelFormat::ByteUYVY:
			return TRUE;
		case PixelFormat::ByteYUV:
			return TRUE;
		case PixelFormat::ByteGray:
			return FALSE;
        case PixelFormat::ByteBool:
			return FALSE;
        case PixelFormat::ByteBGRA:
            return TRUE;
        case PixelFormat::ByteRGBA:
            return TRUE;
		case PixelFormat::WordBGR:
			return TRUE;
		case PixelFormat::WordGray:
			return FALSE;
		case PixelFormat::FloatBGR:
			return TRUE;
		case PixelFormat::FloatGray:
			return FALSE;
		case PixelFormat::FloatHSV:
			return TRUE;
        default:
            return FALSE;
	}
}
Bool PixelFormat::hasAlpha(Enum<spadas::PixelFormat> format)
{
    switch (format.value())
	{
		case PixelFormat::ByteBGR:
			return FALSE;
        case PixelFormat::ByteRGB:
			return FALSE;
		case PixelFormat::ByteUYVY:
			return FALSE;
		case PixelFormat::ByteYUV:
			return FALSE;
		case PixelFormat::ByteGray:
			return FALSE;
        case PixelFormat::ByteBool:
			return FALSE;
        case PixelFormat::ByteBGRA:
            return TRUE;
        case PixelFormat::ByteRGBA:
            return TRUE;
		case PixelFormat::WordBGR:
			return FALSE;
		case PixelFormat::WordGray:
			return FALSE;
		case PixelFormat::FloatBGR:
			return FALSE;
		case PixelFormat::FloatGray:
			return FALSE;
		case PixelFormat::FloatHSV:
			return FALSE;
        default:
            return FALSE;
	}
}
UInt PixelFormat::nChannels(Enum<PixelFormat> format)
{
    switch (format.value())
	{
		case PixelFormat::ByteBGR:
			return 3;
        case PixelFormat::ByteRGB:
			return 3;
		case PixelFormat::ByteUYVY:
			return 2;
		case PixelFormat::ByteYUV:
			return 3;
		case PixelFormat::ByteGray:
			return 1;
        case PixelFormat::ByteBool:
			return 1;
        case PixelFormat::ByteBGRA:
            return 4;
        case PixelFormat::ByteRGBA:
            return 4;
		case PixelFormat::WordBGR:
			return 3;
		case PixelFormat::WordGray:
			return 1;
		case PixelFormat::FloatBGR:
			return 3;
		case PixelFormat::FloatGray:
			return 1;
		case PixelFormat::FloatHSV:
			return 3;
        default:
            return 0;
	}
}
UInt PixelFormat::bytesPerPixel(Enum<PixelFormat> format)
{
    switch (format.value())
	{
		case PixelFormat::ByteBGR:
			return 3;
        case PixelFormat::ByteRGB:
			return 3;
		case PixelFormat::ByteUYVY:
			return 2;
		case PixelFormat::ByteYUV:
			return 3;
		case PixelFormat::ByteGray:
			return 1;
        case PixelFormat::ByteBool:
			return 1;
        case PixelFormat::ByteBGRA:
            return 4;
        case PixelFormat::ByteRGBA:
            return 4;
		case PixelFormat::WordBGR:
			return 6;
		case PixelFormat::WordGray:
			return 2;
		case PixelFormat::FloatBGR:
			return 12;
		case PixelFormat::FloatGray:
			return 4;
		case PixelFormat::FloatHSV:
			return 12;
        default:
            return 0;
	}
}

// ResizeMode
Bool ResizeMode::isValid(Int val)
{
	return val >= ResizeMode::Nearest && val <= ResizeMode::Area;
}
String ResizeMode::toString(Int val)
{
	switch (val)
	{
		case ResizeMode::Nearest:
			return "Nearest";
		case ResizeMode::Bilinear:
			return "Bilinear";
		case ResizeMode::Area:
			return "Area";
		default:
			return String();
	}
}
Int ResizeMode::defaultValue()
{
	return ResizeMode::Bilinear;
}

// ResizeScale
Bool ResizeScale::isValid(Int val)
{
	return val >= ResizeScale::Quarter && val <= ResizeScale::Double;
}
String ResizeScale::toString(Int val)
{
	switch (val)
	{
		case ResizeScale::Quarter:
			return "Quarter";
		case ResizeScale::Half:
			return "Half";
		case ResizeScale::Double:
			return "Double";
		default:
			return String();
	}
}
Int ResizeScale::defaultValue()
{
	return ResizeScale::Half;
}

/* ImageRotation */
Bool ImageRotation::isValid(Int val)
{
	return val >= ImageRotation::CW90 && val <= ImageRotation::CCW90;
}
String ImageRotation::toString(Int val)
{
	switch (val)
	{
		case ImageRotation::CW90:
			return "Clockwise 90 degree";
		case ImageRotation::CW180:
			return "Clockwise 180 degree";
		case ImageRotation::CCW90:
			return "Counter-clockwise 90 degree";
		default:
			return String();
	}
}
Int ImageRotation::defaultValue()
{
	return ImageRotation::CW90;
}

/* PixelData */
PixelData::PixelData() : ptr(NULL), bytesPerPixel(0)
{
}
PixelData::PixelData(Pointer ptr0, UInt bytesPerPixel0) : ptr(ptr0), bytesPerPixel(bytesPerPixel0)
{
}
Bool PixelData::isValid()
{
	return ptr != NULL;
}
PixelData PixelData::operator[](UInt index)
{
    return ptr ? PixelData((Pointer)((PointerInt)ptr + index * bytesPerPixel), bytesPerPixel) : PixelData();
}
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

/* ImagePair */
ImagePair::ImagePair()
{

}
ImagePair::ImagePair(Image image10, Image image20) : image1(image10), image2(image20)
{

}
