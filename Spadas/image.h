
#include "spadas.h"

namespace image_internal
{
	using namespace spadas;
	using namespace spadas::math;

	/* constants */
	const UInt MAX_WIDTH = 16384;
	const UInt MAX_HEIGHT = 16384;
	
	/* color conversion table */
    struct ColorConvertTable
    {
        Int *RToY;	//0.29891
        Int *GToY;	//0.58661
        Int *BToY;	//0.11448
	
        Int *VToR;	//1.402
        Int *UToG;	//-0.34414
        Int *VToG;	//-0.71414
        Int *UToB;	//1.772
	
        Int *RToU;	//-0.16874
        Int *GToU;	//-0.33126
        Int *BToU;	//0.5
        Int *RToV;	//0.5
        Int *GToV;	//-0.41869
        Int *BToV;	//-0.08131
        
        Bool inited;
        Lock lock;
        
        ColorConvertTable()
        {
            inited = FALSE;
            RToY = NULL;
            GToY = NULL;
            BToY = NULL;
            VToR = NULL;
            UToG = NULL;
            VToG = NULL;
            UToB = NULL;
            RToU = NULL;
            GToU = NULL;
            BToU = NULL;
            RToV = NULL;
            GToV = NULL;
            BToV = NULL;
        }
        ~ColorConvertTable()
        {
            if (RToY) delete[] RToY;
            if (GToY) delete[] GToY;
            if (BToY) delete[] BToY;
            if (VToR) delete[] VToR;
            if (UToG) delete[] UToG;
            if (VToG) delete[] VToG;
            if (UToB) delete[] UToB;
            if (RToU) delete[] RToU;
            if (GToU) delete[] GToU;
            if (BToU) delete[] BToU;
            if (RToV) delete[] RToV;
            if (GToV) delete[] GToV;
            if (BToV) delete[] BToV;
        }
        void init()
        {
            LockProxy lp(lock);
            if (inited) return;
            
            RToY = new Int[65536];
            GToY = new Int[65536];
            BToY = new Int[65536];
            VToR = new Int[65536];
            UToG = new Int[65536];
            VToG = new Int[65536];
            UToB = new Int[65536];
            RToU = new Int[65536];
            GToU = new Int[65536];
            BToU = new Int[65536];
            RToV = new Int[65536];
            GToV = new Int[65536];
            BToV = new Int[65536];

            for (Int i = 0; i < 65536; i++)
            {
                RToY[i] = math::floor(0.29891f * i);
                GToY[i] = math::floor(0.58661f * i);
                BToY[i] = math::floor(0.11448f * i);
                
                VToR[i] = math::round(1.402f * (i - 32768));
                UToG[i] = math::round(-0.34414f * (i - 32768));
                VToG[i] = math::round(-0.71414f * (i - 32768));
                UToB[i] = math::round(1.772f * (i - 32768));
                
                RToU[i] = math::round(-0.16874f * i);
                GToU[i] = math::round(-0.33126f * i);
                BToU[i] = math::round(0.5f * i);
                RToV[i] = math::round(0.5f * i);
                GToV[i] = math::round(-0.41869f * i);
                BToV[i] = math::round(-0.08131f * i);
            }
            inited = TRUE;
        }
    };
    extern ColorConvertTable table;
	
	/* row io interfaces */
	enum class RowIOType
	{
		WordColorAlpha = 1, // alpha's range: 0~255
		WordMono = 2,
		FloatColor = 3,
		FloatMono = 4,
	};
	
	class IRowIO
	{
	public:
		virtual ~IRowIO() {};
		virtual void io(Pointer row, Pointer data, UInt width) = 0;
	};
	
	class IRowCvt
	{
	public:
		virtual ~IRowCvt() {};
		virtual void cvt(Pointer src, Pointer dst, UInt width) = 0;
	};
	
	class IRowRsz
	{
	public:
		virtual ~IRowRsz() {};
		virtual void rsz(Pointer* src, Pointer dst, UInt srcWidth, UInt dstWidth, UInt srcHeight, UInt dstHeight, Enum<ResizeMode> mode, Float v) = 0;
	};
	
	/* row io implementations */
#define rowio_class(type)							\
class Load##type : public IRowIO				\
{												\
public:											\
void io(Pointer row, Pointer data, UInt width);		\
};												\
class Write##type : public IRowIO				\
{												\
public:											\
void io(Pointer row, Pointer data, UInt width);		\
};
	
#define rowcvt_class(type1, type2)					\
class Cvt##type1##To##type2 : public IRowCvt	\
{												\
public:											\
void cvt(Pointer src, Pointer dst, UInt width);		\
};												\
class Cvt##type2##To##type1 : public IRowCvt	\
{												\
public:											\
void cvt(Pointer src, Pointer dst, UInt width);		\
};
	
#define rowflip_class(type)							\
class Flip##type : public IRowCvt				\
{												\
public:											\
void cvt(Pointer src, Pointer dst, UInt width);		\
};
	
#define rowrsz_class(type)							\
class Resize##type : public IRowRsz				\
{												\
public:											\
void rsz(Pointer* src, Pointer dst, UInt srcWidth, UInt dstWidth, UInt srcHeight, UInt dstHeight, Enum<ResizeMode> mode, Float v);		\
};
	
	rowio_class(ByteBGR)
	rowio_class(ByteGray)
    rowio_class(ByteRGB)
	rowio_class(ByteUYVY)
	rowio_class(ByteYUV)
    rowio_class(ByteBool)
    rowio_class(ByteBGRA)
    rowio_class(ByteRGBA)
	rowio_class(WordBGR)
	rowio_class(WordGray)
	rowio_class(FloatBGR)
	rowio_class(FloatGray)
	rowio_class(FloatHSV)
	
	rowcvt_class(WordColorAlpha, WordMono)
	rowcvt_class(WordColorAlpha, FloatColor)
	rowcvt_class(WordColorAlpha, FloatMono)
	rowcvt_class(WordMono, FloatColor)
	rowcvt_class(WordMono, FloatMono)
	rowcvt_class(FloatColor, FloatMono)
	
	rowflip_class(WordColorAlpha)
	rowflip_class(WordMono)
	rowflip_class(FloatColor)
	rowflip_class(FloatMono)
	
	rowrsz_class(WordColorAlpha)
	rowrsz_class(WordMono)
	rowrsz_class(FloatColor)
	rowrsz_class(FloatMono)
	
	/* row io functions */
	RowIOType getRowIOType(Enum<PixelFormat> format);
	
	Pointer createRowData(RowIOType type, UInt width);
	void destroyRowData(Pointer data, RowIOType type);
	
	IRowIO *createRowLoader(Enum<PixelFormat> format);
	IRowIO *createRowWriter(Enum<PixelFormat> format);
	IRowCvt *createRowCvt(RowIOType srcType, RowIOType dstType);
	IRowCvt *createRowFlip(RowIOType type);
	IRowRsz* createRowRsz(RowIOType type);

	/* resize (area, scale) */
	void areaResizeColorByte(Image src, Image& dst, Float widthRatio, Float heightRatio, UInt nChannels);
	void areaResizeColorWord(Image src, Image& dst, Float widthRatio, Float heightRatio);
	void areaResizeColorFloat(Image src, Image& dst, Float widthRatio, Float heightRatio);
	void areaResizeGrayByte(Image src, Image& dst, Float widthRatio, Float heightRatio);
	void areaResizeGrayWord(Image src, Image& dst, Float widthRatio, Float heightRatio);
	void areaResizeGrayFloat(Image src, Image& dst, Float widthRatio, Float heightRatio);

	void resizeColorByteWithScale(Image src, Image& dst, Enum<ResizeScale> scale, UInt nChannels);
	void resizeGrayByteWithScale(Image src, Image& dst, Enum<ResizeScale> scale);
	void resizeColorWordWithScale(Image src, Image& dst, Enum<ResizeScale> scale);
	void resizeGrayWordWithScale(Image src, Image& dst, Enum<ResizeScale> scale);
	
    /* fastcvt */
	void fastCvtByteBGR2ByteGray(Image src, Image& dst);
	void fastCvtByteBGR2FloatBGR(Image src, Image& dst);
	void fastCvtByteBGR2FloatGray(Image src, Image& dst);
	void fastCvtByteBGR2WordBGR(Image src, Image& dst);
	void fastCvtByteBGR2WordGray(Image src, Image& dst);
	void fastCvtByteBGR2ByteRGB(Image src, Image& dst);
	void fastCvtByteBGR2ByteUYVY(Image src, Image& dst);
	void fastCvtByteBGR2ByteYUV(Image src, Image& dst);
    void fastCvtByteBGR2ByteBGRA(Image src, Image& dst);
    void fastCvtByteBGR2ByteRGBA(Image src, Image& dst);
	
	void fastCvtByteGray2ByteBGR(Image src, Image& dst);
	void fastCvtByteGray2FloatGray(Image src, Image& dst);
	void fastCvtByteGray2WordGray(Image src, Image& dst);
	void fastCvtByteGray2ByteUYVY(Image src, Image& dst);
	void fastCvtByteGray2ByteYUV(Image src, Image& dst);
	
	void fastCvtFloatBGR2ByteBGR(Image src, Image& dst);
	
	void fastCvtFloatGray2ByteBGR(Image src, Image& dst);
	void fastCvtFloatGray2ByteGray(Image src, Image& dst);
	void fastCvtFloatGray2WordGray(Image src, Image& dst);
	
	void fastCvtWordBGR2ByteBGR(Image src, Image& dst);
	void fastCvtWordBGR2WordGray(Image src, Image& dst);
	
	void fastCvtWordGray2ByteBGR(Image src, Image& dst);
	void fastCvtWordGray2ByteGray(Image src, Image& dst);
	
	void fastCvtByteRGB2ByteBGR(Image src, Image& dst);
    void fastCvtByteRGB2ByteRGBA(Image src, Image& dst);
	
	void fastCvtByteUYVY2ByteBGR(Image src, Image& dst);
	void fastCvtByteUYVY2ByteGray(Image src, Image& dst);
	
	void fastCvtByteYUV2ByteBGR(Image src, Image& dst);
	void fastCvtByteYUV2ByteGray(Image src, Image& dst);
    
    void fastCvtByteBGRA2ByteBGR(Image src, Image& dst);
    void fastCvtByteBGRA2ByteRGBA(Image src, Image& dst);
    
    void fastCvtByteRGBA2ByteBGR(Image src, Image& dst);
    void fastCvtByteRGBA2ByteRGB(Image src, Image& dst);
    void fastCvtByteRGBA2ByteBGRA(Image src, Image& dst);

    /* rotate */
	void rotateByte(Image src, Image& dst, UInt nChannels, Enum<ImageRotation> rotation);
	void rotateWord(Image src, Image& dst, Bool isColor, Enum<ImageRotation> rotation);
	void rotateInt(Image src, Image& dst, Bool isColor, Enum<ImageRotation> rotation);
    
    /* for image data construction */
    inline UInt calcImageByteSize(Size2D size, Enum<PixelFormat> format)
    {
        return size.height * (((size.width * PixelFormat::bytesPerPixel(format) - 1) / 8) + 1) * 8;
    }
}

namespace spadas
{
	class ImageVars : public Vars
	{
    public:
		SPADAS_VARS(Image, Vars)

		UInt width;
		UInt height;
		Enum<PixelFormat> format;
		UInt rowBytes;
        UInt bytesPerPixel;
		BinarySpan span;
	};
}
