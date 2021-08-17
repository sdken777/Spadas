
#include "spadas.h"

namespace utility_internal
{
	using namespace spadas;

	template <typename SrcType, typename DstType>
	void srcToDst(SrcType *srcData, DstType *dstData, UInt height, UInt srcRowElements, UInt dstRowElements, UInt copyElements)
	{
		for (UInt v = 0; v < height; v++)
		{
			SrcType *srcRow = &srcData[v * srcRowElements];
			DstType *dstRow = &dstData[v * dstRowElements];
			for (UInt n = 0; n < copyElements; n++)
			{
				dstRow[n] = (DstType)srcRow[n];
			}
		}
	}

	template<typename Type>
	Image matrixRToImage(Type *matData, Type lower, Type upper, UInt matDimAt0, UInt matDimAt1)
	{
		UInt width = matDimAt1;
		UInt height = matDimAt0;
		Image out(Size2D::wh(width, height), PixelFormat::FloatGray);

		Float k = 1.0f / (Float)(upper - lower);

		for (UInt v = 0; v < height; v++)
		{
			Type *srcRow = &matData[matDimAt1];
			Float *dstRow = out[v].f;
			for (UInt u = 0; u < width; u++)
			{
				dstRow[u] = (Float)(math::clamp(srcRow[u], lower, upper) - lower) * k;
			}
		}

		return out;
	}
}

using namespace spadas;
using namespace utility_internal;

IntMat spadas::utility::imageToMatrixInt(Image image)
{
	SPADAS_ERROR_RETURNVAL(image.isNull(), IntMat());

	Array<UInt> dims;
	UInt rowElements;
	switch (image.format().value())
	{
		case PixelFormat::ByteBGR:
			dims = Array<UInt>(3);
			dims[0] = image.height(); dims[1] = image.width(); dims[2] = 3;
			rowElements = image.rowBytes();
			break;
		case PixelFormat::ByteRGB:
			dims = Array<UInt>(3);
			dims[0] = image.height(); dims[1] = image.width(); dims[2] = 3;
			rowElements = image.rowBytes();
			break;
		case PixelFormat::ByteGray:
			dims = Array<UInt>(2);
			dims[0] = image.height(); dims[1] = image.width();
			rowElements = image.rowBytes();
			break;
		case PixelFormat::ByteUYVY:
			dims = Array<UInt>(3);
			dims[0] = image.height(); dims[1] = image.width(); dims[2] = 2;
			rowElements = image.rowBytes();
			break;
		case PixelFormat::ByteYUV:
			dims = Array<UInt>(3);
			dims[0] = image.height(); dims[1] = image.width(); dims[2] = 3;
			rowElements = image.rowBytes();
			break;
		case PixelFormat::ByteBool:
			dims = Array<UInt>(2);
			dims[0] = image.height(); dims[1] = image.width();
			rowElements = image.rowBytes();
			break;
		case PixelFormat::ByteBGRA:
			dims = Array<UInt>(3);
			dims[0] = image.height(); dims[1] = image.width(); dims[2] = 4;
			rowElements = image.rowBytes();
			break;
		case PixelFormat::ByteRGBA:
			dims = Array<UInt>(3);
			dims[0] = image.height(); dims[1] = image.width(); dims[2] = 4;
			rowElements = image.rowBytes();
			break;
		case PixelFormat::WordBGR:
			dims = Array<UInt>(3);
			dims[0] = image.height(); dims[1] = image.width(); dims[2] = 3;
			rowElements = image.rowBytes() / 2;
			break;
		case PixelFormat::WordGray:
			dims = Array<UInt>(2);
			dims[0] = image.height(); dims[1] = image.width();
			rowElements = image.rowBytes() / 2;
			break;
		default:
			SPADAS_ERROR_MSG("Invalid image.format()");
			return IntMat();
	}
	
	IntMat out(dims);
	UInt copyElements = dims[1] * (dims.size() == 3 ? dims[2] : 1);
	UInt imageHeight = image.height();
	
	switch (image.format().value())
	{
		case PixelFormat::ByteBGR:
		case PixelFormat::ByteRGB:
		case PixelFormat::ByteGray:
		case PixelFormat::ByteUYVY:
		case PixelFormat::ByteYUV:
		case PixelFormat::ByteBGRA:
		case PixelFormat::ByteRGBA:
			srcToDst(image.data().b, out.data(), imageHeight, rowElements, copyElements, copyElements);
			break;

		case PixelFormat::WordBGR:
		case PixelFormat::WordGray:
			srcToDst(image.data().w, out.data(), imageHeight, rowElements, copyElements, copyElements);
			break;
			
		case PixelFormat::ByteBool:
			for (UInt v = 0; v < imageHeight; v++)
			{
				Byte *srcRow = image[v].b;
				Int *dstRow = out[v].data();
				for (UInt n = 0; n < copyElements; n++)
				{
					dstRow[n] = srcRow[n] == 0 ? 0 : 1;
				}
			}
			break;
			
		default:
			break;
	}
	
	return out;
}

Image spadas::utility::matrixIntToImage(IntMat matrix, Enum<PixelFormat> format)
{
	SPADAS_ERROR_RETURNVAL(matrix.isNull(), Image());

	UInt nDims = 0, nChannels = 1;
	switch (format.value())
	{
	case PixelFormat::ByteBGRA:
	case PixelFormat::ByteRGBA:
		nDims = 3;
		nChannels = 4;
		break;
	case PixelFormat::ByteBGR:
	case PixelFormat::ByteRGB:
	case PixelFormat::ByteYUV:
	case PixelFormat::WordBGR:
		nDims = 3;
		nChannels = 3;
		break;
	case PixelFormat::ByteUYVY:
		nDims = 3;
		nChannels = 2;
		break;
	case PixelFormat::ByteGray:
	case PixelFormat::ByteBool:
	case PixelFormat::WordGray:
		nDims = 2;
		break;
	default:
		SPADAS_ERROR_MSG("Invalid format");
		return Image();
	}
	SPADAS_ERROR_RETURNVAL(matrix.nDims() != nDims, Image());
	SPADAS_ERROR_RETURNVAL(nDims == 3 && matrix.dimAt(2) != nChannels, Image());

	Image out(Size2D::wh(matrix.dimAt(1), matrix.dimAt(0)), format, FALSE);
	UInt copyElements = matrix.dimAt(1) * (nDims == 3 ? matrix.dimAt(2) : 1);
	UInt imageHeight = matrix.dimAt(0);

	switch (format.value())
	{
	case PixelFormat::ByteBGR:
	case PixelFormat::ByteRGB:
	case PixelFormat::ByteGray:
	case PixelFormat::ByteUYVY:
	case PixelFormat::ByteYUV:
	case PixelFormat::ByteBGRA:
	case PixelFormat::ByteRGBA:
		srcToDst(matrix.data(), out.data().b, imageHeight, copyElements, out.rowBytes(), copyElements);
		break;

	case PixelFormat::WordBGR:
	case PixelFormat::WordGray:
		srcToDst(matrix.data(), out.data().w, imageHeight, copyElements, out.rowBytes() / 2, copyElements);
		break;

	case PixelFormat::ByteBool:
		for (UInt v = 0; v < imageHeight; v++)
		{
			Int *srcRow = matrix[v].data();
			Byte *dstRow = out[v].b;
			for (UInt n = 0; n < copyElements; n++)
			{
				dstRow[n] = srcRow[n] == 0 ? 0 : 255;
			}
		}
		break;

	default:
		break;
	}

	return out;
}

FloatMat spadas::utility::imageToMatrixFloat(Image image)
{
	SPADAS_ERROR_RETURNVAL(image.isNull(), FloatMat());

	Array<UInt> dims;
	UInt rowElements;
	switch (image.format().value())
	{
		case PixelFormat::FloatBGR:
			dims = Array<UInt>(3);
			dims[0] = image.height(); dims[1] = image.width(); dims[2] = 3;
			rowElements = image.rowBytes() / 4;
			break;
		case PixelFormat::FloatGray:
			dims = Array<UInt>(2);
			dims[0] = image.height(); dims[1] = image.width();
			rowElements = image.rowBytes() / 4;
			break;
		case PixelFormat::FloatHSV:
			dims = Array<UInt>(3);
			dims[0] = image.height(); dims[1] = image.width(); dims[2] = 3;
			rowElements = image.rowBytes() / 4;
			break;
		default:
			SPADAS_ERROR_MSG("Invalid image.format()");
			return FloatMat();
	}
	
	FloatMat out(dims);
	UInt copyElements = dims[1] * (dims.size() == 3 ? dims[2] : 1);
	UInt imageHeight = image.height();
	
	srcToDst(image.data().f, out.data(), imageHeight, rowElements, copyElements, copyElements);
	
	return out;
}

Image spadas::utility::matrixFloatToImage(FloatMat matrix, Enum<PixelFormat> format)
{
	SPADAS_ERROR_RETURNVAL(matrix.isNull(), Image());

	UInt nDims = 0, nChannels = 1;
	switch (format.value())
	{
	case PixelFormat::FloatBGR:
	case PixelFormat::FloatHSV:
		nChannels = 3;
		nDims = 3;
		break;
	case PixelFormat::FloatGray:
		nDims = 2;
		break;
	default:
		SPADAS_ERROR_MSG("Invaild format");
		return Image();
	}
	SPADAS_ERROR_RETURNVAL(matrix.nDims() != nDims, Image());
	SPADAS_ERROR_RETURNVAL(nDims == 3 && matrix.dimAt(2) != nChannels, Image());

	Image out(Size2D::wh(matrix.dimAt(1), matrix.dimAt(0)), format, FALSE);
	UInt copyElements = matrix.dimAt(1) * (nDims == 3 ? matrix.dimAt(2) : 1);
	UInt imageHeight = matrix.dimAt(0);

	srcToDst(matrix.data(), out.data().f, imageHeight, copyElements, out.rowBytes() / 4, copyElements);

	return out;
}

Image spadas::utility::matrixRangeToImage(IntMat mat, Int lower, Int upper)
{
	SPADAS_ERROR_RETURNVAL(mat.isNull(), Image());
	SPADAS_ERROR_RETURNVAL(mat.nDims() != 2, Image());
	SPADAS_ERROR_RETURNVAL(lower >= upper, Image());

	return matrixRToImage(mat.data(), lower, upper, mat.dimAt(0), mat.dimAt(1));
}

Image spadas::utility::matrixRangeToImage(FloatMat mat, Float lower, Float upper)
{
	SPADAS_ERROR_RETURNVAL(mat.isNull(), Image());
	SPADAS_ERROR_RETURNVAL(mat.nDims() != 2, Image());
	SPADAS_ERROR_RETURNVAL(lower >= upper, Image());

	return matrixRToImage(mat.data(), lower, upper, mat.dimAt(0), mat.dimAt(1));
}

Image spadas::utility::matrixRangeToImage(DoubleMat mat, Double lower, Double upper)
{
	SPADAS_ERROR_RETURNVAL(mat.isNull(), Image());
	SPADAS_ERROR_RETURNVAL(mat.nDims() != 2, Image());
	SPADAS_ERROR_RETURNVAL(lower >= upper, Image());

	return matrixRToImage(mat.data(), lower, upper, mat.dimAt(0), mat.dimAt(1));
}
