
#ifndef SPADAS_CORE_H
#define SPADAS_CORE_H

/// \~English @brief Spadas root namespace
/// \~Chinese @brief Spadas根命名空间
namespace spadas
{
	// Value type / 值类型 //////////////////////////////////////////////////////////////

	/// \~English @brief Boolean
	/// \~Chinese @brief 布尔类型
	typedef bool Bool;

	/// \~English @brief 8-bit unsigned integer
	/// \~Chinese @brief 8位无符号整数
	typedef unsigned char Byte;

	/// \~English @brief 16-bit unsigned integer
	/// \~Chinese @brief 16位无符号整数
	typedef unsigned short Word;

	/// \~English @brief 32-bit unsigned integer
	/// \~Chinese @brief 32位无符号整数
	typedef unsigned int UInt;

	/// \~English @brief 64-bit unsigned integer
	/// \~Chinese @brief 64位无符号整数
	typedef unsigned long long ULong;

	/// \~English @brief 16-bit signed integer
	/// \~Chinese @brief 16位带符号整数
	typedef short Short;

	/// \~English @brief 32-bit signed integer
	/// \~Chinese @brief 32位带符号整数
	typedef int Int;

	/// \~English @brief 64-bit signed integer
	/// \~Chinese @brief 64位带符号整数
	typedef long long Long;
	
	/// \~English @brief Windows: ANSI character set; Linux: UTF-8 character set
	/// \~Chinese @brief Windows: ANSI字符集; Linux: UTF-8字符集
	typedef char Char;
	
	/// \~English @brief Windows: UTF-16 character set, Linux: UTF-32 character set
	/// \~Chinese @brief Windows: UTF-16字符集, Linux: UTF-32字符集
	typedef wchar_t WChar;
	
	/// \~English @brief 32-bit floating point number, single precision
	/// \~Chinese @brief 32位浮点数，单精度
	typedef float Float;

	/// \~English @brief 64-bit floating point number, double precision
	/// \~Chinese @brief 64位浮点数，双精度
	typedef double Double;

	/// \~English @brief Pointer
	/// \~Chinese @brief 指针
	typedef void *Pointer;
	
	/// \~English @brief Integer type compatible with pointers
	/// \~Chinese @brief 兼容指针的整数类型
	typedef ULong PointerInt;

	// Constant / 常数 //////////////////////////////////////////////////////////////
	
	/// \~English @brief Boolean true
	/// \~Chinese @brief 布尔值真
	const Bool TRUE = true;

	/// \~English @brief Boolean false
	/// \~Chinese @brief 布尔值假
	const Bool FALSE = false;

	/// \~English @brief spadas::UInt infinity (can also be used to represent invalid values)
	/// \~Chinese @brief spadas::UInt 类型无限 (亦可用于表示无效数值)
	const UInt UINF = 0xffffffff;

	/// \~English @brief spadas::ULong infinity (can also be used to represent invalid values)
	/// \~Chinese @brief spadas::ULong 类型无限 (亦可用于表示无效数值)
	const ULong ULINF = 0xffffffffffffffffull;

	/// \~English @brief spadas::Int positive infinity (can also be used to represent invalid values)
	/// \~Chinese @brief spadas::Int 类型正无限 (亦可用于表示无效数值)
	const Int INF = 0x7fffffff;

	/// \~English @brief spadas::Int negative infinity (can also be used to represent invalid values)
	/// \~Chinese @brief spadas::Int 类型负无限 (亦可用于表示无效数值)
	const Int NINF = 0x80000000;

	/// \~English @brief spadas::Long positive infinity (can also be used to represent invalid values)
	/// \~Chinese @brief spadas::Long 类型正无限 (亦可用于表示无效数值)
	const Long LINF = 0x7fffffffffffffffll;

	/// \~English @brief spadas::Long negative infinity (can also be used to represent invalid values)
	/// \~Chinese @brief spadas::Long 类型负无限 (亦可用于表示无效数值)
	const Long NLINF = 0x8000000000000000ll;

	// Forward declaration / 前置声明 //////////////////////////////////////////////////////////////

	// String / 字符串
	class SPADAS_API String;

	// Nullable object / 可空对象
	template <typename Type> class Optional;

	// Array / 数组
	template <typename Type> class Array;

	// Linked list node / 链表节点
	template <typename Type> class ListNode;

	// Int type matrix / Int矩阵
	class SPADAS_API IntMat;

	// Float type matrix / Float矩阵
	class SPADAS_API FloatMat;

	// Double type matrix / Double矩阵
	class SPADAS_API DoubleMat;

	// Flag bit / 标志位
	class SPADAS_API Flag;

	// Function template / 函数模板 //////////////////////////////////////////////////////////////

	/// \~English @brief Function template with no parameters and no return value
	/// \~Chinese @brief 无参数无返回值函数模板
	class Void
	{
	public:
		/// \~English @brief Function with no parameters and no return value
		/// \~Chinese @brief 无参数无返回值函数
		typedef void (*Function)();
	};

	/// \~English @brief Function template with no parameters and with return value
	/// \~Chinese @brief 无参数带返回值函数模板
	template <typename RetType> class Out
	{
	public:
		/// \~English @brief Function with no parameters and with return value
		/// \~Chinese @brief 无参数带返回值函数
		typedef RetType (*Function)();
	};

	/// \~English @brief Function template with single parameter and no return value
	/// \~Chinese @brief 单参数无返回值函数模板
	template <typename ArgType> class In
	{
	public:
		/// \~English @brief Function with single parameter and no return value
		/// \~Chinese @brief 单参数无返回值函数
		typedef void (*Function)(ArgType arg);
	};

	/// \~English @brief Function template with single parameter and return value
	/// \~Chinese @brief 单参数带返回值函数模板
	template <typename ArgType, typename RetType> class InOut
	{
	public:
		/// \~English @brief Function with single parameter and return value
		/// \~Chinese @brief 单参数带返回值函数
		typedef RetType (*Function)(ArgType arg);
	};

	/// \~English @brief Function template with two parameters and no return value
	/// \~Chinese @brief 双参数无返回值函数模板
	template <typename ArgType1, typename ArgType2> class In2
	{
	public:
		/// \~English @brief Function with two parameters and no return value
		/// \~Chinese @brief 双参数无返回值函数
		typedef void (*Function)(ArgType1 arg1, ArgType2 arg2);
	};

	/// \~English @brief Function template with two parameters and return value
	/// \~Chinese @brief 双参数带返回值函数模板
	template <typename ArgType1, typename ArgType2, typename RetType> class In2Out
	{
	public:
		/// \~English @brief Function with two parameters and return value
		/// \~Chinese @brief 双参数带返回值函数
		typedef RetType (*Function)(ArgType1 arg1, ArgType2 arg2);
	};

	// Space and Index / 空间与索引 //////////////////////////////////////////////////////////////

	/// \~English @brief 2D size
	/// \~Chinese @brief 2维尺寸
	struct Size2D
	{
		/// \~English @brief The length of the second dimension, or the width of the image
		/// \~Chinese @brief 第2维的长度，或图像宽度
		union { UInt width; UInt dim1; };

		/// \~English @brief The length of the first dimension, or image height
		/// \~Chinese @brief 第1维的长度，或图像高度
		union { UInt height; UInt dim0; };

		/// \~English @brief The default constructor, the value is initialized to 0
		/// \~Chinese @brief 默认构造函数，值初始化为0
		SPADAS_API Size2D();

		/// \~English @brief Whether it is equal to
		/// \~Chinese @brief 是否等于
		SPADAS_API Bool operator ==(Size2D size);

		/// \~English @brief Whether it is not equal to
		/// \~Chinese @brief 是否不等于
		SPADAS_API Bool operator !=(Size2D size);

		/// \~English @brief Convert to string: [width]x[height]
		/// \~Chinese @brief 转字符串: [width]x[height]
		SPADAS_API String toString();

		/// \~English @brief Initialize based on width and height
		/// \~Chinese @brief 基于宽高初始化
		SPADAS_API static Size2D wh(UInt width, UInt height);

		/// \~English @brief Initialize based on dimension
		/// \~Chinese @brief 基于维度初始化
		SPADAS_API static Size2D dims(UInt dim0, UInt dim1);
	};

	/// \~English @brief 3D size
	/// \~Chinese @brief 3维尺寸
	struct Size3D
	{
		/// \~English @brief The length of the second dimension, or the width of the image
		/// \~Chinese @brief 第2维的长度，或图像宽度
		union { UInt width; UInt dim1; };

		/// \~English @brief The length of the first dimension, or image height
		/// \~Chinese @brief 第1维的长度，或图像高度
		union { UInt height; UInt dim0; };

		/// \~English @brief The length of the third dimension, or image depth
		/// \~Chinese @brief 第3维的长度，或图像深度
		union { UInt depth; UInt dim2; };

		/// \~English @brief The default constructor, the value is initialized to 0
		/// \~Chinese @brief 默认构造函数，值初始化为0
		SPADAS_API Size3D();

		/// \~English @brief Whether it is equal to
		/// \~Chinese @brief 是否等于
		SPADAS_API Bool operator ==(Size3D size);

		/// \~English @brief Whether it is not equal to
		/// \~Chinese @brief 是否不等于
		SPADAS_API Bool operator !=(Size3D size);

		/// \~English @brief Convert to string: [width]x[height]x[depth]
		/// \~Chinese @brief 转字符串: [width]x[height]x[depth]
		SPADAS_API String toString();

		/// \~English @brief Initialize based on width, height and depth
		/// \~Chinese @brief 基于宽高及深度初始化
		SPADAS_API static Size3D whd(UInt width, UInt height, UInt depth);

		/// \~English @brief Initialize based on dimension
		/// \~Chinese @brief 基于维度初始化
		SPADAS_API static Size3D dims(UInt dim0, UInt dim1, UInt dim2);
	};

	/// \~English @brief 2D index
	/// \~Chinese @brief 2维序号
	struct Index2D
	{
		/// \~English @brief The index, in direction of the second dimension or image width
		/// \~Chinese @brief 第2维方向或图像宽度方向的序号
		union { Int u; Int j; };

		/// \~English @brief The index, in direction of the first dimension or image height
		/// \~Chinese @brief 第1维方向或图像高度方向的序号
		union { Int v; Int i; };

		/// \~English @brief The default constructor, the value is initialized to 0
		/// \~Chinese @brief 默认构造函数，值初始化为0
		SPADAS_API Index2D();

		/// \~English @brief Whether it is equal to
		/// \~Chinese @brief 是否等于
		SPADAS_API Bool operator ==(Index2D index);

		/// \~English @brief Whether it is not equal to
		/// \~Chinese @brief 是否不等于
		SPADAS_API Bool operator !=(Index2D index);

		/// \~English @brief Convert to string: (u,v)
		/// \~Chinese @brief 转字符串: (u,v)
		SPADAS_API String toString();

		/// \~English @brief Initialize based on image coordination
		/// \~Chinese @brief 基于图像坐标初始化
		SPADAS_API static Index2D uv(Int u, Int v);

		/// \~English @brief Initialize based on indices in 2 directions
		/// \~Chinese @brief 基于两个方向的序号初始化
		SPADAS_API static Index2D ij(Int i, Int j);
	};

	/// \~English @brief 2D integer coordinates
	/// \~Chinese @brief 2维整数型坐标
	typedef Index2D CoordInt2D;

	/// \~English @brief 3D index
	/// \~Chinese @brief 3维序号
	struct Index3D
	{
		/// \~English @brief The index, in direction of the second dimension or image width
		/// \~Chinese @brief 第2维方向或图像宽度方向的序号
		union { Int u; Int j; };

		/// \~English @brief The index, in direction of the first dimension or image height
		/// \~Chinese @brief 第1维方向或图像高度方向的序号
		union { Int v; Int i; };

		/// \~English @brief The index, in direction of the third dimension or image depth
		/// \~Chinese @brief 第3维方向或图像深度方向的序号
		union { Int w; Int k; };

		/// \~English @brief The default constructor, the value is initialized to 0
		/// \~Chinese @brief 默认构造函数，值初始化为0
		SPADAS_API Index3D();

		/// \~English @brief Whether it is equal to
		/// \~Chinese @brief 是否等于
		SPADAS_API Bool operator ==(Index3D index);

		/// \~English @brief Whether it is not equal to
		/// \~Chinese @brief 是否不等于
		SPADAS_API Bool operator !=(Index3D index);

		/// \~English @brief Convert to string: (u,v,w)
		/// \~Chinese @brief 转字符串: (u,v,w)
		SPADAS_API String toString();

		/// \~English @brief Initialize based on image coordination
		/// \~Chinese @brief 基于图像坐标初始化
		SPADAS_API static Index3D uvw(Int u, Int v, Int w);

		/// \~English @brief Initialize based on indices in 3 directions
		/// \~Chinese @brief 基于三个方向的序号初始化
		SPADAS_API static Index3D ijk(Int i, Int j, Int k);
	};

	/// \~English @brief 3D integer coordinates
	/// \~Chinese @brief 3维整数型坐标
	typedef Index3D CoordInt3D;

	/// \~English @brief 1D area (integer)
	/// \~Chinese @brief 1维区域（整型）
	struct Region
	{
		/// \~English @brief The starting position of the area
		/// \~Chinese @brief 区域起始位置
		Int offset;

		/// \~English @brief Size of area
		/// \~Chinese @brief 区域大小
		UInt size;
		
		/// \~English @brief The default constructor, the value is initialized to 0
		/// \~Chinese @brief 默认构造函数，值初始化为0
		SPADAS_API Region();

		/// \~English @brief Initialize based on starting position and size
		/// \~Chinese @brief 基于起始位置和大小初始化
		SPADAS_API Region(Int offset, UInt size);

		/// \~English @brief Whether it is equal to
		/// \~Chinese @brief 是否等于
		SPADAS_API Bool operator ==(Region region);

		/// \~English @brief Whether it is not equal to
		/// \~Chinese @brief 是否不等于
		SPADAS_API Bool operator !=(Region region);

		/// \~English @brief Convert to string: offset->size
		/// \~Chinese @brief 转字符串: offset->size
		SPADAS_API String toString();
	};

	/// \~English @brief 2D area (integer)
	/// \~Chinese @brief 2维区域（整型）
	struct Region2D
	{
		/// \~English @brief The starting position of the area, in direction of the second dimension or image width
		/// \~Chinese @brief 区域起始位置，第2维方向或图像宽度方向
		union { Int offsetU; Int offsetJ; };

		/// \~English @brief The starting position of the area, in direction of the first dimension or image height
		/// \~Chinese @brief 区域起始位置，第1维方向或图像高度方向
		union { Int offsetV; Int offsetI; };

		/// \~English @brief The length of the second dimension, or the width of the image
		/// \~Chinese @brief 第2维的长度，或图像宽度
		union { UInt width; UInt dim1; };

		/// \~English @brief The length of the first dimension, or image height
		/// \~Chinese @brief 第1维的长度，或图像高度
		union { UInt height; UInt dim0; };
		
		/// \~English @brief The default constructor, the value is initialized to 0
		/// \~Chinese @brief 默认构造函数，值初始化为0
		SPADAS_API Region2D();

		/// \~English @brief Initialize based on offset and size
		/// \~Chinese @brief 基于起始位置和大小初始化
		SPADAS_API Region2D(CoordInt2D offset, Size2D size);

		/// \~English @brief Whether it is equal to
		/// \~Chinese @brief 是否等于
		SPADAS_API Bool operator ==(Region2D region);

		/// \~English @brief Whether it is not equal to
		/// \~Chinese @brief 是否不等于
		SPADAS_API Bool operator !=(Region2D region);

		/// \~English @brief Get offset
		/// \~Chinese @brief 获取起始位置
		SPADAS_API CoordInt2D offset();

		/// \~English @brief Get size
		/// \~Chinese @brief 获取大小
		SPADAS_API Size2D size();

		/// \~English @brief Convert to string: (offsetU->width,offsetV->height)
		/// \~Chinese @brief 转字符串: (offsetU->width,offsetV->height)
		SPADAS_API String toString();
	};

	/// \~English @brief 3D area (integer)
	/// \~Chinese @brief 3维区域（整型）
	struct Region3D
	{
		/// \~English @brief The starting position of the area, in direction of the second dimension or image width
		/// \~Chinese @brief 区域起始位置，第2维方向或图像宽度方向
		union { Int offsetU; Int offsetJ; };

		/// \~English @brief The starting position of the area, in direction of the first dimension or image height
		/// \~Chinese @brief 区域起始位置，第1维方向或图像高度方向
		union { Int offsetV; Int offsetI; };

		/// \~English @brief The starting position of the area, in direction of the third dimension or image depth
		/// \~Chinese @brief 区域起始位置，第3维方向或图像深度方向
		union { Int offsetW; Int offsetK; };

		/// \~English @brief The length of the second dimension, or the width of the image
		/// \~Chinese @brief 第2维的长度，或图像宽度
		union { UInt width; UInt dim1; };

		/// \~English @brief The length of the first dimension, or image height
		/// \~Chinese @brief 第1维的长度，或图像高度
		union { UInt height; UInt dim0; };

		/// \~English @brief The length of the third dimension, or image depth
		/// \~Chinese @brief 第1维的长度，或图像高度
		union { UInt depth; UInt dim2; };

		/// \~English @brief The default constructor, the value is initialized to 0
		/// \~Chinese @brief 默认构造函数，值初始化为0
		SPADAS_API Region3D();

		/// \~English @brief Initialize based on offset and size
		/// \~Chinese @brief 基于起始位置和大小初始化
		SPADAS_API Region3D(CoordInt3D offset, Size3D size);

		/// \~English @brief Whether it is equal to
		/// \~Chinese @brief 是否等于
		SPADAS_API Bool operator ==(Region3D region);

		/// \~English @brief Whether it is not equal to
		/// \~Chinese @brief 是否不等于
		SPADAS_API Bool operator !=(Region3D region);

		/// \~English @brief Get offset
		/// \~Chinese @brief 获取起始位置
		SPADAS_API CoordInt3D offset();

		/// \~English @brief Get size
		/// \~Chinese @brief 获取大小
		SPADAS_API Size3D size();

		/// \~English @brief Convert to string: (offsetU->width,offsetV->height,offsetW->depth)
		/// \~Chinese @brief 转字符串: (offsetU->width,offsetV->height,offsetW->depth)
		SPADAS_API String toString();
	};

	/// \~English @brief 2D vector
	/// \~Chinese @brief 2维矢量
	struct Vector2D
	{
		/// \~English @brief The horizontal coordinates
		/// \~Chinese @brief 横向坐标
		union { Float x; Float u; Float j; };

		/// \~English @brief The vertical coordinates
		/// \~Chinese @brief 纵向坐标
		union { Float y; Float v; Float i; };

		/// \~English @brief The default constructor, the value is initialized to 0
		/// \~Chinese @brief 默认构造函数，值初始化为0
		SPADAS_API Vector2D();

		/// \~English @brief Initialize based on horizontal and vertical coordinates
		/// \~Chinese @brief 基于横纵坐标初始化
		SPADAS_API Vector2D(Float x, Float y);

		/// \~English @brief Whether it is equal to
		/// \~Chinese @brief 是否等于
		SPADAS_API Bool operator ==(Vector2D vec);

		/// \~English @brief Whether it is not equal to
		/// \~Chinese @brief 是否不等于
		SPADAS_API Bool operator !=(Vector2D vec);

		/// \~English @brief Vector addition
		/// \~Chinese @brief 矢量相加
		SPADAS_API Vector2D operator +(Vector2D vec);

		/// \~English @brief Vector subtraction
		/// \~Chinese @brief 矢量相减
		SPADAS_API Vector2D operator -(Vector2D vec);

		/// \~English @brief Vector multiplied by scale factor
		/// \~Chinese @brief 矢量乘以比例系数
		SPADAS_API Vector2D operator *(Float k);

		/// \~English @brief Vector inner product (dot product)
		/// \~Chinese @brief 矢量内积（点乘）
		SPADAS_API Float operator *(Vector2D vec);

		/// \~English @brief Calculate the absolute value (length) of the vector
		/// \~Chinese @brief 计算矢量的绝对值（长度）
		SPADAS_API Float absolute();

		/// \~English @brief Convert to string: (x,y)
		/// \~Chinese @brief 转字符串: (x,y)
		SPADAS_API String toString();

		/// \~English @brief Initialize based on image coordinates
		/// \~Chinese @brief 基于图像坐标初始化
		SPADAS_API static Vector2D uv(Float u, Float v);

		/// \~English @brief Initialize based on the position of 2 directions
		/// \~Chinese @brief 基于2个方向的位置初始化
		SPADAS_API static Vector2D ij(Float i, Float j);
	};

	/// \~English @brief 2D point
	/// \~Chinese @brief 2维点
	typedef Vector2D Point2D;

	/// \~English @brief 2D coordinates
	/// \~Chinese @brief 2维坐标
	typedef Vector2D CoordFloat2D;

	/// \~English @brief 3D vector
	/// \~Chinese @brief 3维矢量
	struct Vector3D
	{
		/// \~English @brief The horizontal coordinates
		/// \~Chinese @brief 横向坐标
		union { Float x; Float u; Float j; };

		/// \~English @brief The vertical coordinates
		/// \~Chinese @brief 纵向坐标
		union { Float y; Float v; Float i; };

		/// \~English @brief The depth coordinates
		/// \~Chinese @brief 深度坐标
		union { Float z; Float w; Float k; };

		/// \~English @brief The default constructor, the value is initialized to 0
		/// \~Chinese @brief 默认构造函数，值初始化为0
		SPADAS_API Vector3D();

		/// \~English @brief Initialize based on horizontal, vertical and depth coordinates
		/// \~Chinese @brief 基于横纵深坐标初始化
		SPADAS_API Vector3D(Float x, Float y, Float z);

		/// \~English @brief Whether it is equal to
		/// \~Chinese @brief 是否等于
		SPADAS_API Bool operator ==(Vector3D vec);

		/// \~English @brief Whether it is not equal to
		/// \~Chinese @brief 是否不等于
		SPADAS_API Bool operator !=(Vector3D vec);

		/// \~English @brief Vector addition
		/// \~Chinese @brief 矢量相加
		SPADAS_API Vector3D operator +(Vector3D vec);

		/// \~English @brief Vector subtraction
		/// \~Chinese @brief 矢量相减
		SPADAS_API Vector3D operator -(Vector3D vec);

		/// \~English @brief Vector multiplied by scale factor
		/// \~Chinese @brief 矢量乘以比例系数
		SPADAS_API Vector3D operator *(Float k);

		/// \~English @brief Vector inner product (dot product)
		/// \~Chinese @brief 矢量内积（点乘）
		SPADAS_API Float operator *(Vector3D vec);

		/// \~English @brief Calculate the absolute value (length) of the vector
		/// \~Chinese @brief 计算矢量的绝对值（长度）
		SPADAS_API Float absolute();

		/// \~English @brief Convert to string: (x,y,z)
		/// \~Chinese @brief 转字符串: (x,y,z)
		SPADAS_API String toString();

		/// \~English @brief Initialize based on image coordinates
		/// \~Chinese @brief 基于图像坐标初始化
		SPADAS_API static Vector3D uvw(Float u, Float v, Float w);

		/// \~English @brief Initialize based on the position of 3 directions
		/// \~Chinese @brief 基于3个方向的位置初始化
		SPADAS_API static Vector3D ijk(Float i, Float j, Float k);
	};

	/// \~English @brief 3D point
	/// \~Chinese @brief 3维点
	typedef Vector3D Point3D;

	/// \~English @brief 3D coordinates
	/// \~Chinese @brief 3维坐标
	typedef Vector3D CoordFloat3D;

	/// \~English @brief 1D area (floating point)
	/// \~Chinese @brief 1维区域（浮点）
	struct Range
	{
		/// \~English @brief The lower edge of the area
		/// \~Chinese @brief 区域下沿
		Float lower;

		/// \~English @brief The upper edge of the area
		/// \~Chinese @brief 区域上沿
		Float upper;

		/// \~English @brief Default constructor, initialized to 0
		/// \~Chinese @brief 默认构造函数，初始化为0
		SPADAS_API Range();

		/// \~English @brief Initialization based on the upper and lower edges
		/// \~Chinese @brief 基于上下沿初始化
		SPADAS_API Range(Float lower, Float upper);

		/// \~English @brief Whether it is equal to
		/// \~Chinese @brief 是否等于
		SPADAS_API Bool operator ==(Range range);

		/// \~English @brief Whether it is not equal to
		/// \~Chinese @brief 是否不等于
		SPADAS_API Bool operator !=(Range range);

		/// \~English @brief Convert to string: lower~upper
		/// \~Chinese @brief 转字符串: lower~upper
		SPADAS_API String toString();
	};

	/// \~English @brief 2D area (floating point)
	/// \~Chinese @brief 2维区域（浮点）
	struct Range2D
	{
		/// \~English @brief The lower edge of the area, in direction of the second dimension
		/// \~Chinese @brief 区域下沿，第2维方向
		union { Float lowerX; Float lowerU; Float lowerJ; };

		/// \~English @brief The upper edge of the area, in direction of the second dimension
		/// \~Chinese @brief 区域上沿，第2维方向
		union { Float upperX; Float upperU; Float upperJ; };

		/// \~English @brief The lower edge of the area, in direction of the first dimension
		/// \~Chinese @brief 区域下沿，第1维方向
		union { Float lowerY; Float lowerV; Float lowerI; };

		/// \~English @brief The upper edge of the area, in direction of the first dimension
		/// \~Chinese @brief 区域上沿，第1维方向
		union { Float upperY; Float upperV; Float upperI; };

		/// \~English @brief Default constructor, initialized to 0
		/// \~Chinese @brief 默认构造函数，初始化为0
		SPADAS_API Range2D();

		/// \~English @brief Initialization based on the upper and lower edges
		/// \~Chinese @brief 基于上下沿初始化
		SPADAS_API Range2D(Float lowerX, Float upperX, Float lowerY, Float upperY);

		/// \~English @brief Whether it is equal to
		/// \~Chinese @brief 是否等于
		SPADAS_API Bool operator ==(Range2D range);

		/// \~English @brief Whether it is not equal to
		/// \~Chinese @brief 是否不等于
		SPADAS_API Bool operator !=(Range2D range);

		/// \~English @brief Convert to string: (lowerX~upperX,lowerY~upperY)
		/// \~Chinese @brief 转字符串: (lowerX~upperX,lowerY~upperY)
		SPADAS_API String toString();

		/// \~English @brief Initialization based on the the rectangle position in image
		/// \~Chinese @brief 基于图像中矩形位置初始化
		SPADAS_API static Range2D uv(Float lowerU, Float upperU, Float lowerV, Float upperV);

		/// \~English @brief Initialization based on the the range in 2 dimensions
		/// \~Chinese @brief 基于2个方向上的范围初始化
		SPADAS_API static Range2D ij(Float lowerI, Float upperI, Float lowerJ, Float upperJ);
	};

	/// \~English @brief 3D area (floating point)
	/// \~Chinese @brief 3维区域（浮点）
	struct Range3D
	{
		/// \~English @brief The lower edge of the area, in direction of the second dimension
		/// \~Chinese @brief 区域下沿，第2维方向
		union { Float lowerX; Float lowerU; Float lowerJ; };

		/// \~English @brief The upper edge of the area, in direction of the second dimension
		/// \~Chinese @brief 区域上沿，第2维方向
		union { Float upperX; Float upperU; Float upperJ; };

		/// \~English @brief The lower edge of the area, in direction of the first dimension
		/// \~Chinese @brief 区域下沿，第1维方向
		union { Float lowerY; Float lowerV; Float lowerI; };

		/// \~English @brief The upper edge of the area, in direction of the first dimension
		/// \~Chinese @brief 区域上沿，第1维方向
		union { Float upperY; Float upperV; Float upperI; };

		/// \~English @brief The lower edge of the area, in direction of the third dimension
		/// \~Chinese @brief 区域下沿，第3维方向
		union { Float lowerZ; Float lowerW; Float lowerK; };

		/// \~English @brief The upper edge of the area, in direction of the third dimension
		/// \~Chinese @brief 区域上沿，第3维方向
		union { Float upperZ; Float upperW; Float upperK; };

		/// \~English @brief Default constructor, initialized to 0
		/// \~Chinese @brief 默认构造函数，初始化为0
		SPADAS_API Range3D();

		/// \~English @brief Initialization based on the upper and lower edges
		/// \~Chinese @brief 基于上下沿初始化
		SPADAS_API Range3D(Float lowerX, Float upperX, Float lowerY, Float upperY, Float lowerZ, Float upperZ);

		/// \~English @brief Whether it is equal to
		/// \~Chinese @brief 是否等于
		SPADAS_API Bool operator ==(Range3D range);

		/// \~English @brief Whether it is not equal to
		/// \~Chinese @brief 是否不等于
		SPADAS_API Bool operator !=(Range3D range);

		/// \~English @brief Convert to string: (lowerX~upperX,lowerY~upperY,lowerZ~upperZ)
		/// \~Chinese @brief 转字符串: (lowerX~upperX,lowerY~upperY,lowerZ~upperZ)
		SPADAS_API String toString();

		/// \~English @brief Initialization based on the the rectangle and depth position in image
		/// \~Chinese @brief 基于图像中矩形和深度位置初始化
		SPADAS_API static Range3D uvw(Float lowerU, Float upperU, Float lowerV, Float upperV, Float lowerW, Float upperW);

		/// \~English @brief Initialization based on the the range in 3 dimensions
		/// \~Chinese @brief 基于3个方向上的范围初始化
		SPADAS_API static Range3D ijk(Float lowerI, Float upperI, Float lowerJ, Float upperJ, Float lowerK, Float upperK);
	};

	// Reference count / 引用计数 //////////////////////////////////////////////////////////////

	/// \~English @brief [Multithread safe] An atomic operation class used to ensure multithread safety of counting
	/// \~Chinese @brief [多线程安全] 用于保证计数多线程安全的原子操作类
	class SPADAS_API Atom
	{
	public:
		/// \~English @brief The default constructor, the initial value is 0
		/// \~Chinese @brief 默认构造函数，初始值为0
		Atom();
		
		/// \~English @brief Create based on the specified initial value
		/// \~Chinese @brief 基于指定初始值创建
		Atom(Int val);
		
		/// \~English @brief Increment by 1 and return the new value
		/// \~Chinese @brief 加1并返回新值
		Int increase();
		
		/// \~English @brief Subtract 1 and return the new value
		/// \~Chinese @brief 减1并返回新值
		Int decrease();
		
		/// \~English @brief Get the current value
		/// \~Chinese @brief 获取当前值
		Int get();

		/// \~English @brief Set the current value
		/// \~Chinese @brief 设定当前值
		void set(Int val);

		/// \~English @brief Add/subtract numbers and return the new value
		/// \~Chinese @brief 加/减数并返回新值
		Int add(Int number);

		/// \~English @brief If it is the old value, set it to the new value and return TRUE, otherwise return FALSE
		/// \~Chinese @brief 若为旧值则设定为新值并返回TRUE，否则返回FALSE
		Bool cas(Int oldVal, Int newVal);
		
	private:
		Int val;
		Atom(const Atom& obj);
		Atom& operator =(const Atom& obj) { return *this; }
	};
	
	/// \~English @brief [Multithread safe] The base class of variables, so that it has reference counting availabilities
	/// \~Chinese @brief [多线程安全] 变量数据的基类，使其具有引用计数属性
	class SPADAS_API Vars
	{
	public:
		/// \~English @brief Constructor, the reference count is 1 after creation
		/// \~Chinese @brief 构造函数，创建后引用计数为1
		Vars();
		
		/// \~English @brief Destructor function, automatically called after the reference count drops to 0
		/// \~Chinese @brief 析构函数函数，在引用计数降为0后自动调用
		virtual ~Vars();
		
		/// \~English @brief Increment the reference count by 1
		/// \~Chinese @brief 引用计数增加1
		void retain();
		
		/// \~English @brief Decrement the reference count by 1
		/// \~Chinese @brief 引用计数减1
		void release();
		
		/// \~English @brief Get the current reference count
		/// \~Chinese @brief 取得当前引用计数
		UInt getRefCount();

		/// \~English @brief Get the class name (for type conversion)
		/// \~Chinese @brief 获取类名称（用于实现类型转换）
		virtual String getTypeName();

		/// \~English @brief Get the base class type chain (for type conversion)
		/// \~Chinese @brief 获取基类类型链（用于实现类型转换）
		virtual ListNode<String> getBaseChain();

		/// \~English @brief Get the string description
		/// \~Chinese @brief 获取字符串描述
		virtual String toString();

		/// \~English @brief Get the number of live objects
		/// \~Chinese @brief 取得存活对象数目
		static UInt getObjectCount();

	protected:
		/// \~English @brief Convenient to generate a type chain in the getBaseChain method
		/// \~Chinese @brief 方便在getBaseChain方法中生成类型链
		ListNode<String> genBaseChain(String baseType, ListNode<String> baseBaseChain);

	private:
		Atom nRefs;
	};
	
	/// \~English @brief The base class of all reference counting classes, where VarsType is a derived class of Vars
	/// \~Chinese @brief 所有引用计数类的基类，其中VarsType为Vars的子类
	template <typename VarsType> class Object
	{
	public:
		/// \~English @brief [Multithread safe] The default constructor, the pointer to variables is empty
		/// \~Chinese @brief [多线程安全] 默认构造函数，变量数据指针为空
		Object();

		/// \~English @brief [Multithread safe] Initialize with the specified pointer to variables
		/// \~Chinese @brief [多线程安全] 以指定变量数据指针初始化
		/// \~English @param newVars Pointer to variables
		/// \~Chinese @param newVars 变量数据的指针
		/// \~English @param isNew Should be TRUE for the new pointer, otherwise FALSE (obtained by Object::getVars, etc.)
		/// \~Chinese @param isNew 对于new出来的指针isNew应为TRUE，否则为FALSE（由 Object::getVars 等获得的情况）
		Object(VarsType *newVars, Bool isNew);
		
		/// \~English @brief [Multithread safe] Destructor, the reference of variables is decremented by 1
		/// \~Chinese @brief [多线程安全] 析构函数，变量数据的引用减1
		virtual ~Object();
		
		/// \~English @brief [Multithread safe] Create an object that directly points to the variables of another object (as a base class is called implicitly)
		/// \~Chinese @brief [多线程安全] 创建引用对象，直接指向另一个对象的变量数据（作为基类被隐式调用）
		Object(const Object<VarsType>& obj);
		
		/// \~English @brief Redirection, the reference count of original variables is reduced by 1, and set pointer to the variables of another object (as the base class is called implicitly). Multithread unsafe: All operations on the object must be added with spadas::Lock if the object is shared with multiple threads
		/// \~Chinese @brief 重定向，原变量数据引用减1并指向另一个对象的变量数据（作为基类被隐式调用）。非多线程安全：多线程共享对象有调用此方法的(等号左手边)，必须对该对象的所有操作加 spadas::Lock
		Object& operator =(const Object<VarsType>& obj);
		
		/// \~English @brief [Multithread safe] Is the reference equal
		/// \~Chinese @brief [多线程安全] 是否相等
		Bool operator ==(const Object<VarsType>& obj);

		/// \~English @brief [Multithread safe] Is the reference not equal
		/// \~Chinese @brief [多线程安全] 是否不相等
		Bool operator !=(const Object<VarsType>& obj);

		/// \~English @brief [Multithread safe] Whether the pointer to variables is null
		/// \~Chinese @brief [多线程安全] 变量数据指针是否为空
		Bool isNull();

		/// \~English @brief [Multithread safe] Whether the pointer to variables is valid (not null)
		/// \~Chinese @brief [多线程安全] 变量数据指针是否有效（非空）
		Bool isValid();
		
		/// \~English @brief [Multithread safe] Get the hash value
		/// \~Chinese @brief [多线程安全] 获取哈希值
		Word getHash();

		/// \~English @brief [Multithread safe] Obtain the reference count of variables
		/// \~Chinese @brief [多线程安全] 获得变量数据的引用计数
		UInt getRefCount();

		/// \~English @brief [Multithread safe] Obtain the class name corresponding to the variables
		/// \~Chinese @brief [多线程安全] 获得变量数据对应的类名称
		String getTypeName();

		/// \~English @brief [Multithread safe] Whether it is the variables of this class, if the pointer to variables is empty, it also returns TRUE
		/// \~Chinese @brief [多线程安全] 是否为该类的变量数据，若变量数据指针为空也返回TRUE
		template <typename TargetType>
		Bool is();

		/// \~English @brief [Multithread safe] Convert to base class object, if the pointer to variables is empty or the type does not match, return to the default construction object
		/// \~Chinese @brief [多线程安全] 转换为基类对象，若变量数据指针为空或类型不符合则返回默认构造对象
		template <typename TargetType>
		TargetType as();

		/// \~English @brief [Multithread safe] Convert to a derived class object, if the type does not match, return an invalid object
		/// \~Chinese @brief [多线程安全] 转换为派生类对象，若类型不符合则返回无效对象
		template <typename TargetType>
		Optional<TargetType> cast();

		/// \~English @brief [Unsafe pointer] Obtain the pointer to variables
		/// \~Chinese @brief [非安全指针] 获得变量数据的指针
		VarsType *getVars();

		/// \~English @brief [Unsafe operation] Set pointer to variables. Multithread unsafe: All classes that call this method outside constructor are Multithread-unsafe
		/// \~Chinese @brief [非安全操作] 设置变量数据指针。非多线程安全：在非构造时调用此方法的类都为非多线程安全类
		/// \~English @param newVars Pointer to variables
		/// \~Chinese @param newVars 变量数据的指针
		/// \~English @param isNew Should be TRUE for the new pointer, otherwise FALSE (obtained by Object::getVars, etc.)
		/// \~Chinese @param isNew 对于new出来的指针isNew应为TRUE，否则为FALSE（由 Object::getVars 等获得的情况）
		void setVars(VarsType* newVars, Bool isNew);

		/// \~English @brief [Unsafe operation] Convert the input and set the pointer to variables (castCreate is recommended)
		/// \~Chinese @brief [非安全操作] 转换输入并设置变量数据指针（建议使用castCreate）
		void castVars(Vars* varsToCast);
		
		/// \~English @brief [Unsafe operation] Convert the input pointer to variables to the current type, and create the object
		/// \~Chinese @brief [非安全操作] 以输入的变量数据指针转换为当前类型创建对象
		static Object<VarsType> castCreate(Vars* varsToCast);

		/// \~English @brief Get the string description
		/// \~Chinese @brief 获取字符串描述
		String toString();

	protected:
		/// \~English @brief [Unsafe pointer] Pointer to variables
		/// \~Chinese @brief [非安全指针] 变量数据指针
		VarsType *vars;
	};

	/// \~English @brief Reference counting base class without member variables
	/// \~Chinese @brief 无需成员变量的引用计数基类
	class SPADAS_API EmptyObject : public Object<Vars>
	{
	public:
		/// \~English @brief Create object
		/// \~Chinese @brief 创建对象
		EmptyObject();

	private:
		Bool isNull() { return FALSE; }
		Bool isValid() { return FALSE; }
	};

	/// \~English @brief Convertible base class object
	/// \~Chinese @brief 可转换的基类对象
	class SPADAS_API BaseObject : public Object<Vars>
	{
	public:
		/// \~English @brief Class name
		/// \~Chinese @brief 类名称
		static const String TypeName;

		/// \~English @brief Invalid object
		/// \~Chinese @brief 无效对象
		BaseObject();
	};

	/// \~English @brief Variables of spadas::Interface template class
	/// \~Chinese @brief spadas::Interface 模板类的变量数据
	template <typename Type> class InterfaceVars;

	/// \~English @brief The container of the interface implementation object, where Type is the interface definition type
	/// \~Chinese @brief 接口实现对象的容器，其中Type为接口定义类型
	template <typename Type> class Interface : public Object<InterfaceVars<Type> >
	{
	public:
		/// \~English @brief Invalid object
		/// \~Chinese @brief 无效对象
		Interface();
		
		/// \~English @brief Create a container for input objects, where ImplementType must be the implementation (derived class) of Type
		/// \~Chinese @brief 创建输入对象的容器，其中ImplementType必须为Type的实现（子类）
		template <typename ImplementType>
		Interface(ImplementType implementObj);
		
		/// \~English @brief Whether it is equal to (has the same reference of object as another container)
		/// \~Chinese @brief 是否等于（与另一个容器拥有同一个对象，即指向同一个变量数据）
		Bool operator ==(Interface<Type> target);

		/// \~English @brief Whether it is not equal to (has a different reference of object from another container)
		/// \~Chinese @brief 是否不等于（与另一个容器拥有不同对象）
		Bool operator !=(Interface<Type> target);

		/// \~English @brief Get the hash value
		/// \~Chinese @brief 获取哈希值
		Word getHash();

		/// \~English @brief Get the object in the container
		/// \~Chinese @brief 获取容器中的对象
		BaseObject getObject();

		/// \~English @brief Use the interface of the object in the container
		/// \~Chinese @brief 使用容器中对象的接口
		Type *operator ->();
	};
	
	/// \~English @brief Variables of spadas::Optional template class
	/// \~Chinese @brief spadas::Optional 模板类的变量数据
	template <typename Type> class OptionalVars;

	/// \~English @brief Nullable object, where Type can be any type
	/// \~Chinese @brief 可空对象，其中Type可为任意类型
	template <typename Type> class Optional : public Object<OptionalVars<Type> >
	{
	public:
		/// \~English @brief Invalid object
		/// \~Chinese @brief 无效对象
		Optional();
		
		/// \~English @brief Create an object based on the input
		/// \~Chinese @brief 基于输入创建对象
		Optional(Type val);
		
		/// \~English @brief Value
		/// \~Chinese @brief 取值
		Type value();

		/// \~English @brief Take the value, return the input default value when the value is invalid
		/// \~Chinese @brief 取值，当无效时返回输入的默认值
		Type value(Type defaultValue);

		/// \~English @brief [Modifiable] Value
		/// \~Chinese @brief [可修改] 取值
		Type& refValue();

		/// \~English @brief [Modifiable] Use field or method
		/// \~Chinese @brief [可修改] 使用字段或方法
		Type* operator ->();
	};

	// Data structure template / 数据结构模板 //////////////////////////////////////////////////////////////

	/// \~English @brief Fixed-length array element iterator
	/// \~Chinese @brief 定长数组元素遍历器
	template <typename Type> struct ArrayElem
	{
		Array<Type> arr;
		Type *data;
		UInt size;
		UInt idx;

		/// \~English @brief Initialization function
		/// \~Chinese @brief 初始化函数
		ArrayElem(Array<Type> arr, UInt index);

		/// \~English @brief Is it within the range of the array
		/// \~Chinese @brief 是否在数组范围内
		Bool valid();

		/// \~English @brief Get the index of the current element
		/// \~Chinese @brief 获取当前元素的序号
		UInt index();

		/// \~English @brief [Unsafe operation] [Modifiable] Get the reference of the value (need to ensure that it is within the range of the array)
		/// \~Chinese @brief [非安全操作] [可修改] 取得值的引用（需要先确保在数组范围内）
		Type& value();

		/// \~English @brief [Unsafe operation] [Modifiable] Use the field or method of the value (need to ensure that it is within the range of the array)
		/// \~Chinese @brief [非安全操作] [可修改] 使用值的字段或方法（需要先确保在数组范围内）
		Type* operator ->();

		/// \~English @brief [Unsafe operation] Assign to the current element (need to ensure that it is within the range of the array)
		/// \~Chinese @brief [非安全操作] 赋值给当前元素（需要先确保在数组范围内）
		void operator =(const Type& val);

		/// \~English @brief Move to the previous element
		/// \~Chinese @brief 移动至上个元素
		void operator --();

		/// \~English @brief Move to the next element
		/// \~Chinese @brief 移动至下个元素
		void operator ++();
	};

	/// \~English @brief Variables of spadas::Array template class
	/// \~Chinese @brief spadas::Array 模板类的变量数据
	template <typename Type> class ArrayVars;

	/// \~English @brief Fixed length array
	/// \~Chinese @brief 定长数组
	template <typename Type> class Array : public Object<ArrayVars<Type> >
	{
	public:
		/// \~English @brief Default constructor, create an empty array
		/// \~Chinese @brief 默认构造函数，创建一个空数组
		Array();

		/// \~English @brief Creates an array with the specified size
		/// \~Chinese @brief 创建一个具有指定大小的数组
		/// \~English @param size The size of the array
		/// \~Chinese @param size 创建数组的大小
		Array(UInt size);
		
		/// \~English @brief Create an array with the specified size and set the initial value of all elements
		/// \~Chinese @brief 创建一个具有指定大小的数组并设置所有元素的初始值
		/// \~English @param size The size of the array
		/// \~Chinese @param size 创建数组的大小
		/// \~English @param origin The initial value of all elements in the array
		/// \~Chinese @param origin 数组中所有元素的初始值
		Array(UInt size, Type origin);

		/// \~English @brief Create an object from an array pointer (the source data will be copied when created)
		/// \~Chinese @brief 从一个数组指针创建对象（创建时将拷贝源数据）
		/// \~English @param arr Source data array pointer
		/// \~Chinese @param arr 源数据数组指针
		/// \~English @param size Source data array size
		/// \~Chinese @param size 源数据数组大小
		Array(const Type arr[], UInt size);
		
		/// \~English @brief Copy and create an object from a region of another array
		/// \~Chinese @brief 从另一个数组的子区域拷贝并创建对象
		/// \~English @param input Source data array
		/// \~Chinese @param input 源数据数组
		/// \~English @param region Copy region (when it is spadas::UINF, it will be copied to the end)
		/// \~Chinese @param region 拷贝区域（为 spadas::UINF 时将拷贝至末尾）
		Array(Array<Type> input, Region region);

		/// \~English @brief Get the size of the array
		/// \~Chinese @brief 取得数组大小
		UInt size();

		/// \~English @brief [Unsafe pointer][Modifiable] Get the head pointer of the array
		/// \~Chinese @brief [非安全指针][可修改] 取得数组的头指针
		Type *data();

		/// \~English @brief [Modifiable] Get a reference to an element in the array
		/// \~Chinese @brief [可修改] 取得数组中某个元素的引用
		Type& operator [](UInt index);

		/// \~English @brief [Modifiable] Get a reference to the first element in the array
		/// \~Chinese @brief [可修改] 取得数组中第一个元素的引用
		Type& first();

		/// \~English @brief [Modifiable] Get a reference to the last element in the array
		/// \~Chinese @brief [可修改] 取得数组中最后一个元素的引用
		Type& last();

		/// \~English @brief [Modifiable] Get the iterator from the first element
		/// \~Chinese @brief [可修改] 取得从第一个元素的遍历器
		ArrayElem<Type> firstElem();

		/// \~English @brief [Modifiable] Get the iterator starting from the last element
		/// \~Chinese @brief [可修改] 取得从最后一个元素开始的遍历器
		ArrayElem<Type> lastElem();

		/// \~English @brief [Modifiable] Get the sub-array, which will be bound to the data of this array
		/// \~Chinese @brief [可修改] 取得子数组，其将绑定至本数组的数据
		Array<Type> subArray(UInt index, UInt size = UINF);

		/// \~English @brief Is it an empty array
		/// \~Chinese @brief 是否为空数组
		Bool isEmpty();

		/// \~English @brief Reduce the size of the array (ArrayElem and sub-arrays obtained before reduction will not be available)
		/// \~Chinese @brief 缩减数组大小（缩减前获取的ArrayElem和子数组将不可用）
		void trim(UInt size);
		
		/// \~English @brief Clone a new object (Call operator "=" for all elements)
		/// \~Chinese @brief 克隆出一个新对象 (所有元素调用=号拷贝)
		Array<Type> clone();

		/// \~English @brief Assign and erase data
		/// \~Chinese @brief 赋值擦除数据
		void clear(Type value);
		
		/// \~English @brief Copy data from a sub-area of another array to a certain position of this array
		/// \~Chinese @brief 从另一个数组的某个子区域拷贝数据到本数组的某个位置
		/// \~English @param src Source data array
		/// \~Chinese @param src 源数据数组
		/// \~English @param srcRegion The region copied from the source array
		/// \~Chinese @param srcRegion 从源数组拷贝的区域
		/// \~English @param thisOffset The starting position of this array copied to
		/// \~Chinese @param thisOffset 拷贝至本数组的起始位置
		void copy(Array<Type> src, Region srcRegion, UInt thisOffset);
		
		/// \~English @brief Whether the array contains a certain value
		/// \~Chinese @brief 数组中是否包含某个值
		Bool contain(Type val);

		/// \~English @brief Whether the array contains a value that the specified function returns TRUE
		/// \~Chinese @brief 数组中是否包含指定函数返回TRUE的某个值
		Bool containAs(typename InOut<Type, Bool>::Function func);

		/// \~English @brief Returns the indices of all elements in the array equal to a certain value
		/// \~Chinese @brief 返回数组中所有等于某个值的元素序号
		Array<UInt> search(Type val);

		/// \~English @brief Returns the indices of all elements in the array that the specified function returns TRUE
		/// \~Chinese @brief 返回数组中所有指定函数返回TRUE的元素序号
		Array<UInt> searchAs(typename InOut<Type, Bool>::Function func);

		/// \~English @brief Convert to other types of arrays according to the specified function
		/// \~Chinese @brief 按指定函数转换为其他类型数组
		template <typename TargetType>
		Array<TargetType> convert(typename InOut<Type, TargetType>::Function func);
		
		/// \~English @brief Divide into multiple arrays according to the specified size
		/// \~Chinese @brief 根据指定的大小分割为多个数组
		/// \~English @param sizes The size of each array that will be split
		/// \~Chinese @param sizes 将分割成的每个数组的大小
		/// \~English @returns Split multiple arrays
		/// \~Chinese @returns 分割后的多个数组
		Array<Array<Type> > split(Array<UInt> sizes);

		/// \~English @brief Concatenate another array after this array
		/// \~Chinese @brief 在本数组后方拼接另一个数组
		/// \~English @param arr Another array to be concatenated
		/// \~Chinese @param arr 将拼接的另一个数组
		/// \~English @returns Concatenated array
		/// \~Chinese @returns 拼接后的数组
		Array<Type> operator +(Array<Type> arr);

		/// \~English @brief Create a scalar array (contains only one element)
		/// \~Chinese @brief 创建一个标量数组（只含有一个元素）
		static Array<Type> scalar(Type element);

		/// \~English @brief [Unsafe operation] Create an array based on argument list. ArgType should be Int, UInt, Double, Char* or structure and class type, etc.
		/// \~Chinese @brief [非安全操作] 基于可变参数列表创建数组并赋值，ArgType应为Int，UInt，Double，Char*或结构体和类名等
		/// \~English @param size Array size
		/// \~Chinese @param size 数组大小
		/// \~English @param firstValue The first element value of the array, the following parameters are the element values of the ArgType type, and the total number of elements should be consistent with the size of the array
		/// \~Chinese @param firstValue 数组首个元素值，后面参数都为ArgType类型的元素值，总元素个数应与数组大小一致
		/// \~English @returns Created array
		/// \~Chinese @returns 创建的数组
		template <typename ArgType>
		static Array<Type> create(UInt size, ArgType firstValue, ...);

		/// \~English @brief [Unsafe operation] Create an uninitialized array with the specified size, need to call the "initialize" method afterwards to ensure that each element is initialized once and only once
		/// \~Chinese @brief [非安全操作] 创建一个具有指定大小但未初始化的数组，需要随后调用initialize方法确保对每个元素都初始化了仅一次
		/// \~English @param size The size of the array
		/// \~Chinese @param size 创建数组的大小
		/// \~English @returns Created array
		/// \~Chinese @returns 新创建的数组
		static Array<Type> createUninitialized(UInt size);

		/// \~English @brief [Unsafe operation] Initialize the element, you need to ensure that the index is within the range
		/// \~Chinese @brief [非安全操作] 初始化元素，需要确保序号在范围内
		void initialize(UInt index, const Type& val);
		
		/// \~English @brief Merge multiple arrays
		/// \~Chinese @brief 合并多个数组
		/// \~English @param arrs Multiple arrays to be merged
		/// \~Chinese @param arrs 将合并的多个数组
		/// \~English @returns The merged array
		/// \~Chinese @returns 合并后的数组
		static Array<Type> merge(Array<Array<Type> > arrs);

		/// \~English @brief Convert to an array of base class objects
		/// \~Chinese @brief 转换为基类对象的数组
		template <typename TargetType>
		Array<TargetType> asArray();

		/// \~English @brief Convert to an array of derived class objects
		/// \~Chinese @brief 转换为派生类对象的数组
		template <typename TargetType>
		Array<Optional<TargetType> > castArray();

		/// \~English @brief Sort from small to large, need Type supports ">" operator
		/// \~Chinese @brief 按从小到大排序，需要Type支持>重载符
		void sort();

		/// \~English @brief According to the specified function (returns whether it is greater than), sort from small to large
		/// \~Chinese @brief 根据指定函数(判断是否大于)，按从小到大排序
		void sortAs(typename In2Out<Type&, Type&, Bool>::Function func);

	private:
		Bool isNull() { return FALSE; }
		Bool isValid() { return FALSE; }
	};

	/// \~English @brief Variable data of spadas::ListNode template class
	/// \~Chinese @brief spadas::ListNode 模板类的变量数据
	template <typename Type> class ListNodeVars;

	/// \~English @brief Linked list node
	/// \~Chinese @brief 链表节点
	template <typename Type> class ListNode : public Object<ListNodeVars<Type> >
	{
	public:
		/// \~English @brief Create a node whose data is the default value
		/// \~Chinese @brief 创建一个节点，其数据为默认值
		ListNode();
		
		/// \~English @brief Create a node and assign a specified value to its data
		/// \~Chinese @brief 创建一个节点，并给其数据赋指定值
		ListNode(Type val);

		/// \~English @brief [Modifiable] Get the reference of the data
		/// \~Chinese @brief [可修改] 取得数据的引用
		Type& value();

		/// \~English @brief [Modifiable] Use a field or method of the data object
		/// \~Chinese @brief [可修改] 使用数据的字段或方法
		Type* operator ->();
		
		/// \~English @brief Whether there is a next node
		/// \~Chinese @brief 是否存在下一个节点
		Bool hasNext();

		/// \~English @brief Whether there is a previous node
		/// \~Chinese @brief 是否存在上一个节点
		Bool hasPrevious();

		/// \~English @brief Get the next node (if it does not exist, return this node)
		/// \~Chinese @brief 取得下一个节点 (若不存在返回本节点)
		ListNode<Type> next();
		
		/// \~English @brief Get the previous node (if it does not exist, return this node)
		/// \~Chinese @brief 取得上一个节点 (若不存在返回本节点)
		ListNode<Type> previous();

		/// \~English @brief Go to the next node (if exists)
		/// \~Chinese @brief 赋值为下一个节点（若存在）
		void goNext();

		/// \~English @brief Go to the previous node (if exists)
		/// \~Chinese @brief 赋值为上一个节点（若存在）
		void goPrevious();
		
		/// \~English @brief Cut off the link to the next node
		/// \~Chinese @brief 切断与下一个节点的链接
		void cutNext();
		
		/// \~English @brief Cut off the link with the previous node
		/// \~Chinese @brief 切断与上一个节点的链接
		void cutPrevious();

		/// \~English @brief Connect the node created by the specified value as the next node, and return the created node (this->cutNext will be called automatically)
		/// \~Chinese @brief 连接按指定值创建的节点作为下一个节点，并返回创建的节点 (将自动调用this->cutNext)
		ListNode<Type> joinNext(Type val);

		/// \~English @brief Connect the specified node as the next node (this->cutNext and node.cutPrevious will be called automatically)
		/// \~Chinese @brief 连接指定节点为下一个节点 (将自动调用this->cutNext和node.cutPrevious)
		void joinNext(ListNode<Type> node);
		
		/// \~English @brief Connect the node created by the specified value as the previous node, and return the created node (this->cutPrevious will be called automatically)
		/// \~Chinese @brief 连接按指定值创建的节点作为上一个节点，并返回创建的节点 (将自动调用this->cutPrevious)
		ListNode<Type> joinPrevious(Type val);

		/// \~English @brief Connect the specified node to the previous node (this->cutPrevious and node.cutNext will be called automatically)
		/// \~Chinese @brief 连接指定节点为上一个节点 (将自动调用this->cutPrevious和node.cutNext)
		void joinPrevious(ListNode<Type> node);
		
		/// \~English @brief Insert the node created by the specified value between this node and the next node, and return the node
		/// \~Chinese @brief 在本节点和下一个节点间插入按指定值创建的节点，并返回该节点
		ListNode<Type> insertNext(Type val);

		/// \~English @brief Insert the specified node between this node and the next node (newNode.removeSelf will be called automatically)
		/// \~Chinese @brief 在本节点和下一个节点间插入指定节点 (将自动调用newNode.removeSelf)
		void insertNext(ListNode<Type> newNode);
		
		/// \~English @brief Insert a node created by the specified value between this node and the previous node, and return the node
		/// \~Chinese @brief 在本节点和上一个节点间插入按指定值创建的节点，并返回该节点
		ListNode<Type> insertPrevious(Type val);

		/// \~English @brief Insert the specified node between this node and the previous node (newNode.removeSelf will be called automatically)
		/// \~Chinese @brief 在本节点和上一个节点间插入指定节点 (将自动调用newNode.removeSelf)
		void insertPrevious(ListNode<Type> newNode);
		
		/// \~English @brief Remove the next node (will be connected to the next node)
		/// \~Chinese @brief 移除下一个节点（将与下下个节点相连）
		void removeNext();
		
		/// \~English @brief Remove the previous node (will be connected to the previous node)
		/// \~Chinese @brief 移除上一个节点（将与上上个节点相连）
		void removePrevious();
		
		/// \~English @brief Remove this node from the linked list (the previous node will be connected to the next node)
		/// \~Chinese @brief 从链表中移除本节点（上一个节点将与下一个节点相连）
		void removeSelf();
		
		/// \~English @brief Copy the entire linked list (the operator "=" will be called on all elements in the linked list)
		/// \~Chinese @brief 拷贝整个链表 (将对链表中所有元素调用=号)
		ListNode<Type> cloneList();

		/// \~English @brief Disassemble all links in the linked list
		/// \~Chinese @brief 拆解链表中所有链接
		void collapse();
		
	private:
		ListNode(ListNodeVars<Type> *vars);
		Bool isNull() { return FALSE; }
		Bool isValid() { return FALSE; }
	};

	/// \~English @brief Variables of spadas::TreeNode template class
	/// \~Chinese @brief spadas::TreeNode 模板类的变量数据
	template <typename Type> class TreeNodeVars;

	/// \~English @brief Tree node
	/// \~Chinese @brief 树节点
	template <typename Type> class TreeNode : public Object<TreeNodeVars<Type> >
	{
	public:
		/// \~English @brief Create a node whose data is the default value
		/// \~Chinese @brief 创建一个节点，其数据为默认值
		TreeNode();
		
		/// \~English @brief Create a node and assign a specified value to its data
		/// \~Chinese @brief 创建一个节点，并给其数据赋指定值
		TreeNode(Type val);

		/// \~English @brief [Modifiable] Get the reference of the data
		/// \~Chinese @brief [可修改] 取得数据的引用
		Type& value();

		/// \~English @brief [Modifiable] Use a field or method of the data object
		/// \~Chinese @brief [可修改] 使用数据的字段或方法
		Type* operator ->();

		/// \~English @brief Get the node to the root direction (if it does not exist, return this node)
		/// \~Chinese @brief 取得根方向节点 (若不存在返回本节点)
		TreeNode<Type> root();

		/// \~English @brief Get all child nodes
		/// \~Chinese @brief 取得所有子节点
		Array<TreeNode<Type> > leaves();

		/// \~English @brief Obtain a child node by the index, which starts from 0 (if there are many child nodes and need to traverse all, it'd be better to use the child node array obtained by leaves())
		/// \~Chinese @brief 由序号取得某个子节点，序号从0开始 (若子节点较多且需要全部遍历，最好使用leaves()获取的子节点数组)
		TreeNode<Type> leafAt(UInt index);

		/// \~English @brief Get the number of child nodes
		/// \~Chinese @brief 取得子节点个数
		UInt nLeaves();
		
		/// \~English @brief Whether the input object is a child node of this node
		/// \~Chinese @brief 判断输入对象是否为本节点的子节点
		Bool isLeaf(TreeNode<Type> node);

		/// \~English @brief Add the node created by the specified value at the end of the child node list, and return the node
		/// \~Chinese @brief 在子节点列表末尾添加按指定值创建的节点，并返回该节点
		TreeNode<Type> addLeaf(Type val);

		/// \~English @brief Add a child node at the end of the child node list (will automatically call leafNode.cutRoot)
		/// \~Chinese @brief 在子节点列表末尾添加一个子节点 (将自动调用leafNode.cutRoot)
		void addLeaf(TreeNode<Type> leafNode);

		/// \~English @brief Remove a child node
		/// \~Chinese @brief 移除一个子节点
		void dropLeaf(TreeNode<Type> leafNode);
		
		/// \~English @brief Connect the specified node as the root direction node (the effect is equivalent to rootNode.addLeaf)
		/// \~Chinese @brief 连接指定节点为根方向节点 (效果等同于rootNode.addLeaf)
		void joinRoot(TreeNode<Type> rootNode);
		
		/// \~English @brief Cut off the link with the root direction node
		/// \~Chinese @brief 切断与根方向节点的链接
		void cutRoot();
		
		/// \~English @brief Get the global root node
		/// \~Chinese @brief 取得全局根节点
		TreeNode<Type> globalRoot();

		/// \~English @brief Obtain a list of all nodes from this node to the global root node (starting from this node, its index is 0)
		/// \~Chinese @brief 获得从本节点至全局根节点间所有节点的列表 (从本节点开始，其序号为0)
		Array<TreeNode<Type> > rootChain();
		
		/// \~English @brief Get the depth of this node in the whole tree (the depth of global root is 0)
		/// \~Chinese @brief 取得本节点在整棵树中的深度 (全局根节点深度为0)
		UInt depth();
		
		/// \~English @brief Copy the entire tree with this node as the global root node (the operator "=" will be called on all elements in the tree)
		/// \~Chinese @brief 拷贝以本节点为全局根节点的整棵树 (将对树中所有元素调用=号)
		TreeNode<Type> cloneTree();

		/// \~English @brief Disassemble all links in the tree
		/// \~Chinese @brief 拆解树中的所有链接
		void collapse();
		
	private:
		TreeNode(TreeNodeVars<Type> *vars);
		Bool isNull() { return FALSE; }
		Bool isValid() { return FALSE; }
		void collapseSub();
	};

	/// \~English @brief Variables of spadas::GraphNode template class
	/// \~Chinese @brief spadas::GraphNode 模板类的变量数据
	template <typename NType, typename LType> class GraphNodeVars;

	/// \~English @brief Graph node (NType: node data type, LType: link data type)
	/// \~Chinese @brief 图节点 (NType: 节点数据类型, LType: 链接数据类型)
	template <typename NType, typename LType> class GraphNode : public Object<GraphNodeVars<NType, LType> >
	{
	public:
		/// \~English @brief Create a node whose data is the default value
		/// \~Chinese @brief 创建一个节点，其数据为默认值
		GraphNode();
		
		/// \~English @brief Create a node and assign a specified value to its data
		/// \~Chinese @brief 创建一个节点，并给其数据赋指定值
		GraphNode(NType val);
		
		/// \~English @brief [Modifiable] Get the reference of the data
		/// \~Chinese @brief [可修改] 取得数据的引用
		NType& value();

		/// \~English @brief [Modifiable] Use a field or method of the data object
		/// \~Chinese @brief [可修改] 使用数据的字段或方法
		NType* operator ->();

		/// \~English @brief Get the number of links on this node
		/// \~Chinese @brief 取得本节点上链接的数目
		UInt nLinks();
		
		/// \~English @brief [Modifiable] Get the data on a link by the index, which starts from 0
		/// \~Chinese @brief [可修改] 由序号取得某个链接上的数据 (序号从0开始)
		LType& linkAt(UInt index);
		
		/// \~English @brief [Modifiable] Obtain the data on the link with the specified node, if there is no such link, an error will be reported
		/// \~Chinese @brief [可修改] 取得与指定节点间的链接上的数据，若无此链接则报错
		LType& linkWith(GraphNode<NType, LType> node);
		
		/// \~English @brief Get a linked node by index, which starts from 0
		/// \~Chinese @brief 由序号取得某个链接着的节点 (序号从0开始)
		GraphNode<NType, LType> nodeAt(UInt index);

		/// \~English @brief Obtain a linked node from the index which starts from 0, and output the data on the link at the same time
		/// \~Chinese @brief 由序号取得某个链接着的节点 (序号从0开始)，同时输出链接上的数据
		GraphNode<NType, LType> nodeAt(UInt index, LType& link);
		
		/// \~English @brief Get the index of the link with the specified node (if there is no link, return UINF)
		/// \~Chinese @brief 取得与指定节点的链接的序号 (若无链接返回UINF)
		UInt indexOf(GraphNode<NType, LType> node);
		
		/// \~English @brief Add a link to the specified node, and set the data of the link in both directions (if the link already exists, only the data will be updated)
		/// \~Chinese @brief 添加一个与指定节点的链接，并设置链接在两个方向上的数据 (若已存在链接，将仅更新数据)
		void addLinkWith(GraphNode<NType, LType> dst, LType linkToDst, LType linkFromDst);
		
		/// \~English @brief Cut off the link with the specified node
		/// \~Chinese @brief 切断与指定节点的链接
		void cutLinkWith(GraphNode<NType, LType> node);
		
		/// \~English @brief Disassemble all links in the graph
		/// \~Chinese @brief 拆解图中的所有链接
		void collapse();
		
	private:
		GraphNode(GraphNodeVars<NType, LType> *vars0);
		Bool isNull() { return FALSE; }
		Bool isValid() { return FALSE; }
	};

	/// \~English @brief Variables of spadas::ArrayX template class
	/// \~Chinese @brief spadas::ArrayX 模板类的变量数据
	template <typename Type> class ArrayXVars;

	/// \~English @brief Expandable array
	/// \~Chinese @brief 可扩展数组
	template <typename Type> class ArrayX : public Object<ArrayXVars<Type> >
	{
	public:
		/// \~English @brief Create an empty array, each segment size is 16
		/// \~Chinese @brief 创建一个空数组，每段大小为16
		ArrayX();
		
		/// \~English @brief Create an expandable array with the specified segment size (should be a power of 2, ranging from 1 to 65536)
		/// \~Chinese @brief 以指定段大小（应为2的幂数，范围在1~65536内）创建可扩展数组
		ArrayX(UInt segmentSize);

		/// \~English @brief Create an expandable array with the specified segment size (should be a power of 2, ranging from 1 to 65536) and the default value
		/// \~Chinese @brief 以指定段大小（应为2的幂数，范围在1~65536内）和默认值创建可扩展数组
		ArrayX(UInt segmentSize, Type defaultValue);
		
		/// \~English @brief [Modifiable] Obtain a reference to an element in the array (if it's over the boundary of the array, it will expand the size to index+1 before accessing)
		/// \~Chinese @brief [可修改] 取得数组中某个元素的引用 (若越过数组边界，将先扩展大小至序号+1再访问)
		Type& operator [](UInt index);
		
		/// \~English @brief Is it an empty array
		/// \~Chinese @brief 是否为空数组
		Bool isEmpty();

		/// \~English @brief Get the size
		/// \~Chinese @brief 取得大小
		UInt size();
		
		/// \~English @brief Expand the size to the specified value
		/// \~Chinese @brief 扩展大小至指定值
		void setSize(UInt size);

		/// \~English @brief Expand 1 element at the end of the array
		/// \~Chinese @brief 在数组末尾扩展1个元素
		void append(Type val);

		/// \~English @brief Expand multiple elements at the end of the array
		/// \~Chinese @brief 在数组末尾扩展多个元素
		void append(Array<Type> vals);
		
		/// \~English @brief Copy from a certain sub-area of the fixed-length array object to a certain position in this array (if it's over the boundary of the array, expand the size to meet the condition)
		/// \~Chinese @brief 从定长数组对象的某个子区域拷贝至本数组的某个位置 (若越过了数组边界，则扩展大小至满足条件)
		void copyFromArray(Array<Type> src, Region srcRegion, UInt thisOffset);
		
		/// \~English @brief Convert to a fixed-length array
		/// \~Chinese @brief 转换为定长数组
		Array<Type> toArray();

		/// \~English @brief Convert to a fixed-length array, you can specify a sub-area of this array for conversion
		/// \~Chinese @brief 转换为定长数组，可指定本数组的某个子区域作转换
		Array<Type> toArray(Region thisRegion);

	private:
		Bool isNull() { return FALSE; }
		Bool isValid() { return FALSE; }
		void copyRegion(Pointer, UInt, Array<Type>&, UInt, UInt, UInt, UInt);
		Pointer getSegmentNode(UInt);
	};

	/// \~English @brief Variables of spadas::List template class
	/// \~Chinese @brief spadas::List 模板类的变量数据
	template <typename Type> class ListVars;

	/// \~English @brief Variables of spadas::ListElem template class
	/// \~Chinese @brief spadas::ListElem 模板类的变量数据
	template <typename Type> class ListElemVars;

	// Linked list (Forward declaration) / 链表（前置声明）
	template <typename Type> class List;

	/// \~English @brief Linked list element iterator
	/// \~Chinese @brief 链表元素遍历器
	template <typename Type> class ListElem : public Object<ListElemVars<Type> >
	{
	public:
		/// \~English @brief Constructor, generated by calling List::head and List::tail
		/// \~Chinese @brief 构造函数，由 List::head 和 List::tail 调用生成
		ListElem(ListNode<Type> node, Bool valid, UInt index, ListNode<Type> prevNode, Bool prevValid, ListNode<Type> nextNode, Bool nextValid, List<Type> list);

		/// \~English @brief Whether the current element is in the linked list
		/// \~Chinese @brief 当前元素是否在链表中
		Bool valid();

		/// \~English @brief Whether there is a previous element
		/// \~Chinese @brief 是否存在上一个元素
		Bool hasPrevious();

		/// \~English @brief Whether there is a next element
		/// \~Chinese @brief 是否存在下一个元素
		Bool hasNext();

		/// \~English @brief Get the index of the current element in the linked list (if it is not in the linked list, return UINF)
		/// \~Chinese @brief 获取当前元素在链表中的序号（不在链表中则返回UINF）
		UInt index();

		/// \~English @brief [Modifiable] Get a reference to the value
		/// \~Chinese @brief [可修改] 取得值的引用
		Type& value();

		/// \~English @brief [Modifiable] Use a field or method of the value
		/// \~Chinese @brief [可修改] 使用值的字段或方法
		Type* operator ->();

		/// \~English @brief [Modifiable] Get a reference to the previous element value
		/// \~Chinese @brief [可修改] 取得上一个元素值的引用
		Type& previous();

		/// \~English @brief [Modifiable] Get a reference to the next element value
		/// \~Chinese @brief [可修改] 取得下一个元素值的引用
		Type& next();

		/// \~English @brief Assign to the current element
		/// \~Chinese @brief 赋值给当前元素
		void operator =(const Type& val);

		/// \~English @brief Go to the previous element
		/// \~Chinese @brief 移动至上个元素
		void operator --();

		/// \~English @brief Go to the next element
		/// \~Chinese @brief 移动至下个元素
		void operator ++();

		/// \~English @brief Insert a value before the current element
		/// \~Chinese @brief 在当前元素前插入值
		void insertPrevious(Type val);

		/// \~English @brief Insert value after current element
		/// \~Chinese @brief 在当前元素后插入值
		void insertNext(Type val);

		/// \~English @brief Remove the current element
		/// \~Chinese @brief 移除当前元素
		void remove();
	
	private:
		Bool isNull() { return FALSE; }
		Bool isValid() { return FALSE; }
	};

	/// \~English @brief Linked list
	/// \~Chinese @brief 链表
	template <typename Type> class List : public Object<ListVars<Type> >
	{
	public:
		/// \~English @brief Create an empty linked list
		/// \~Chinese @brief 创建一个空链表
		List();

		/// \~English @brief Create a linked list from a fixed-length array
		/// \~Chinese @brief 从定长数组创建链表
		List(Array<Type> arr);

		/// \~English @brief Copy (Call operator "=" for all elements)
		/// \~Chinese @brief 复制
		List<Type> clone();

		/// \~English @brief Is it an empty linked list
		/// \~Chinese @brief 是否为空链表
		Bool isEmpty();

		/// \~English @brief Get the number of elements
		/// \~Chinese @brief 取得元素个数
		UInt size();

		/// \~English @brief Obtain the iterator starting from the first element (when using this object, you may not call methods such as add/remove/clear of the linked list, or create other ListElem)
		/// \~Chinese @brief 取得从首个元素开始的遍历器（使用该对象时，不可调用链表的add/remove/clear等方法，或再创建其他ListElem）
		ListElem<Type> head();

		/// \~English @brief Obtain the iterator starting from the end element (when using this object, you may not call methods such as add/remove/clear of the linked list, or create other ListElem)
		/// \~Chinese @brief 取得从末尾元素开始的遍历器（使用该对象时，不可调用链表的add/remove/clear等方法，或再创建其他ListElem）
		ListElem<Type> tail();

		/// \~English @brief Add as the first
		/// \~Chinese @brief 添加至首
		void addToHead(Type val);

		/// \~English @brief Add as the last
		/// \~Chinese @brief 添加至尾
		void addToTail(Type val);

		/// \~English @brief Remove the first
		/// \~Chinese @brief 移除首
		void removeHead();

		/// \~English @brief Remove the last
		/// \~Chinese @brief 移除尾
		void removeTail();

		/// \~English @brief Remove all elements equal to a certain value
		/// \~Chinese @brief 移除等于某个值的所有元素
		void remove(Type val);

		/// \~English @brief Clear the linked list
		/// \~Chinese @brief 清空链表
		void clear();

		/// \~English @brief Convert to a fixed-length array
		/// \~Chinese @brief 转换为定长数组
		Array<Type> toArray();

	private:
		Bool isNull() { return FALSE; }
		Bool isValid() { return FALSE; }
	};

	/// \~English @brief Variables of spadas::Stream template class
	/// \~Chinese @brief spadas::Stream 模板类的变量数据
	template <typename Type> class StreamVars;

	/// \~English @brief [Multithread safe] Data stream object (queue), in which elements have first-in-first-out characteristics
	/// \~Chinese @brief [多线程安全] 数据流，即队列或管道，元素具有先入先出特性
	template <typename Type> class Stream : public Object<StreamVars<Type> >
	{
	public:
		/// \~English @brief Create a data stream with a capacity of 1, and its elements can be discarded, and its multithread safety is guaranteed by a non-spin lock
		/// \~Chinese @brief 创建容量为1的数据流，且其元素可丢弃，其多线程安全由非自旋锁保障
		Stream();
		
		/// \~English @brief Create a data stream with a specified capacity, and specify whether its elements can be discarded, and its multithread safety is guaranteed by a non-spin lock
		/// \~Chinese @brief 创建指定容量的数据流，并指定其元素是否可丢弃，其多线程安全由非自旋锁保障
		Stream(UInt capacity, Bool discardable = TRUE);

		/// \~English @brief Create a data stream with a specified capacity, specify whether its elements can be discarded, and whether its multithread safety is guaranteed by a spin lock
		/// \~Chinese @brief 创建指定容量的数据流，并指定其元素是否可丢弃，以及其多线程安全是否由自旋锁保障
		Stream(UInt capacity, Bool discardable, Bool spin);

		/// \~English @brief Update the capacity of the data stream and discard redundant elements
		/// \~Chinese @brief 更新数据流的容量，并舍弃多余的元素
		void setCapacity(UInt capacity);

		/// \~English @brief Get the capacity of the data stream
		/// \~Chinese @brief 取得数据流的容量
		UInt capacity();
		
		/// \~English @brief Get the number of elements enqueued to this data stream
		/// \~Chinese @brief 获得本数据流已推入的元素数目
		UInt nEnqueued();
		
		/// \~English @brief Get the number of elements that have been dequeued from this data stream
		/// \~Chinese @brief 获得本数据流已取出的元素数目
		UInt nDequeued();
		
		/// \~English @brief Get the number of discarded elements in this data stream
		/// \~Chinese @brief 获得本数据流已丢弃的元素数目
		UInt nDiscarded();
		
		/// \~English @brief Get the number of buffered elements in this data stream, the value should be nEnqueued() - nDequeued() - nDiscarded()
		/// \~Chinese @brief 取得本数据流中缓存的元素数目，该值应为nEnqueued() - nDequeued() - nDiscarded()
		UInt nElements();

		/// \~English @brief This stream is not bufferring any elements
		/// \~Chinese @brief 本数据流未缓存任何元素
		Bool isEmpty();

		/// \~English @brief Get the first (earliest enqueued) element
		/// \~Chinese @brief 获取首个(最早推入)元素
		Type first();

		/// \~English @brief Get the last (latest enqueued) element
		/// \~Chinese @brief 获取最后一个(最迟推入)元素
		Type last();

		/// \~English @brief Enqueue new element
		/// \~Chinese @brief 推入新元素
		void enqueue(Type newElement);

		/// \~English @brief Enqueue a new element. If the capacity is exceeded, 1. When the element can be discarded, discard the earliest pushed element and then push the new element; 2. When the element cannot be discarded, wait for an element to be taken out before pushing, or return FALSE after detecting an interrupt
		/// \~Chinese @brief 推入新元素。若超出容量，1.元素可丢弃时，直接舍弃最早推入的元素再推入新元素；2. 元素不可丢弃时，等待有元素取出后再推入，或检测到interrupt后返回FALSE
		Bool enqueue(Type newElement, Flag interrupt); 
		
		/// \~English @brief Enqueue new elements in batches
		/// \~Chinese @brief 批量推入新元素
		void enqueue(Array<Type> newElements);

		/// \~English @brief Enqueue new elements in batches, and return FALSE if an interrupt is detected during the enqueue process (some elements may have been enqueued at this time)
		/// \~Chinese @brief 批量推入新元素，若推入过程中检测到interrupt则返回FALSE(此时可能已推入部分元素)
		Bool enqueue(Array<Type> newElements, Flag interrupt);

		/// \~English @brief Try to dequeue the specified number of elements, the actual dequeued number is subject to the returned object (the element with index 0 in the returned array is the earliest)
		/// \~Chinese @brief 尝试取出指定数量的元素，实际取出数量以返回对象为准 (返回的数组中序号0的元素为最早)
		Array<Type> dequeue(UInt amount = UINF);

		/// \~English @brief Try to dequeue consecutive elements less than the specified value (the element with index 0 in the returned array is the earliest)
		/// \~Chinese @brief 尝试取出小于指定值的连续元素 (返回的数组中序号0的元素为最早)
		template <typename TargetType>
		Array<Type> dequeueLessThan(TargetType target);

		/// \~English @brief Wait for all elements to be dequeued (return FALSE if interrupt is detected)
		/// \~Chinese @brief 等待所有元素取出 (若检测到interrupt则返回FALSE)
		Bool waitAllDequeued(Flag interrupt);

		/// \~English @brief Terminates the stream, making it "terminated". No elements can be enqueued after termination, but can be dequeued
		/// \~Chinese @brief 终止数据流，使其变为“已终止状态”。终止后将无法推入任何元素，但可取出
		void terminate();

		/// \~English @brief Whether this data stream is "terminated"
		/// \~Chinese @brief 本数据流是否为“已终止状态”
		Bool isTerminated();

		/// \~English @brief Reset the data stream: All elements will be cleared, the "terminated" state will be removed, and statistics such as nEnqueued will be cleared. But it will not change the capacity and "discardable" attribute
		/// \~Chinese @brief 重置数据流：将清空所有缓存中的元素，移除“已终止状态”，并清除nEnqueued等统计数据。但不会改变数据流容量和“元素可丢弃属性”
		void reset();

	private:
		Bool isNull() { return FALSE; }
		Bool isValid() { return FALSE; }
	};

	// Mapping template / 映射模板 //////////////////////////////////////////////////////////////

	/// \~English @brief Key-value pair
	/// \~Chinese @brief 键值对
	template <typename KeyType, typename ValueType> struct KeyValue
	{
		/// \~English @brief Key
		/// \~Chinese @brief 键
		KeyType key;

		/// \~English @brief Value
		/// \~Chinese @brief 值
		ValueType value;

		/// \~English @brief Default constructor
		/// \~Chinese @brief 默认构造函数
		KeyValue() {}

		/// \~English @brief Create based on the specified key and value
		/// \~Chinese @brief 基于指定键值创建
		KeyValue(KeyType key0, ValueType value0) : key(key0), value(value0) {}

		/// \~English @brief Whether it is greater than, KeyType needs to support operator ">"
		/// \~Chinese @brief 是否大于，需要KeyType支持>重载符
		inline Bool operator >(KeyValue<KeyType, ValueType> target)
		{ return key > target.key; }

		/// \~English @brief Whether it is less than or not, KeyType needs to support operator "<"
		/// \~Chinese @brief 是否小于，需要KeyType支持<重载符
		inline Bool operator <(KeyValue<KeyType, ValueType> target)
		{ return key < target.key; }
	};

	/// \~English @brief Numeric key, convert the value of the standard layout type into a key
	/// \~Chinese @brief 数值键，将标准布局类型的值转为键
	template <typename Type> class NumericKey
	{
	public:
		/// \~English @brief Default constructor
		/// \~Chinese @brief 默认构造函数
		NumericKey();

		/// \~English @brief Create based on the specified value (if it is a non-compact structure, all bytes should be set to 0 during initialization)
		/// \~Chinese @brief 基于指定值创建（若为非紧凑结构体，则初始化时应将所有字节置为0）
		NumericKey(Type value);

		/// \~English @brief Get the value
		/// \~Chinese @brief 获取数值
		Type value();

		/// \~English @brief Whether it is equal to
		/// \~Chinese @brief 是否等于
		Bool operator ==(NumericKey<Type> decimal);

		/// \~English @brief Whether it is not equal to
		/// \~Chinese @brief 是否不等于
		Bool operator !=(NumericKey<Type> decimal);

		/// \~English @brief Whether it is greater than, the Type needs to support operator ">"
		/// \~Chinese @brief 是否大于，需要Type支持>重载符
		Bool operator >(NumericKey<Type> decimal);

		/// \~English @brief Whether it is less than or not, the Type needs to support operator "<"
		/// \~Chinese @brief 是否小于，需要Type支持<重载符
		Bool operator <(NumericKey<Type> decimal);

		/// \~English @brief Get the hash value
		/// \~Chinese @brief 获取哈希值
		Word getHash();

	private:
		Type val;
	};

	/// \~English @brief Variables of spadas::Map template class
	/// \~Chinese @brief spadas::Map 模板类的变量数据
	template <typename KeyType, typename ValueType> class MapVars;

	/// \~English @brief Mapping table
	/// \~Chinese @brief 映射表
	template <typename KeyType, typename ValueType> class Map : public Object<MapVars<KeyType, ValueType> >
	{
	public:
		/// \~English @brief Create a mapping table, the default number of buckets is 256
		/// \~Chinese @brief 创建映射表，默认bucket个数为256
		Map();

		/// \~English @brief Create a mapping table with the specified number of buckets (should be a power of 2, within the range of 16~65536)
		/// \~Chinese @brief 创建指定bucket个数的映射表（应为2的幂数，范围在16~65536内）
		Map(UInt bucketCount);

		/// \~English @brief [Unsafe operation] Create a mapping table and assign values based on the argument list. KeyArgType and ValueArgType should be Int, UInt, Double, Char* or structure and class type, etc.
		/// \~Chinese @brief [非安全操作] 基于可变参数列表创建映射表并赋值，KeyArgType和ValueArgType应为Int，UInt，Double，Char*或结构体和类名等
		/// \~English @param size The number of key values to be assigned
		/// \~Chinese @param size 待赋值的键值个数
		/// \~English @param firstKey The first key
		/// \~Chinese @param firstKey 首个键
		/// \~English @param firstValue The first value, the following parameters are alternately KeyArgType and ValueArgType types, the total number should be 2x the number of key values to be assigned
		/// \~Chinese @param firstValue 首个值，后面参数交替为KeyArgType和ValueArgType类型，总个数应为待赋值的键值个数x2
		/// \~English @returns The mapping table created
		/// \~Chinese @returns 创建的映射表
		template <typename KeyArgType, typename ValueArgType>
		static Map<KeyType, ValueType> create(UInt size, KeyArgType firstKey, ValueArgType firstValue, ...);

		/// \~English @brief Clear the data
		/// \~Chinese @brief 清空数据
		void clear();

		/// \~English @brief Is there no data
		/// \~Chinese @brief 是否无数据
		Bool isEmpty();

		/// \~English @brief Get the mapping number
		/// \~Chinese @brief 取得映射数目
		UInt size();

		/// \~English @brief Get all keys
		/// \~Chinese @brief 取得所有键
		Array<KeyType> keys();

		/// \~English @brief Get all the keys after sorting, KeyType needs to support operator ">"
		/// \~Chinese @brief 取得排序后的所有键，需要KeyType支持>重载符
		Array<KeyType> keysSorted();

		/// \~English @brief Get all values
		/// \~Chinese @brief 取得所有值
		Array<ValueType> values();

		/// \~English @brief Get all key-value pairs
		/// \~Chinese @brief 取得所有键值对
		Array<KeyValue<KeyType, ValueType> > keyValues();

		/// \~English @brief Obtain all key-value pairs after sorting, KeyType needs to support operator ">"
		/// \~Chinese @brief 取得排序后的所有键值对，需要KeyType支持>重载符
		Array<KeyValue<KeyType, ValueType> > keyValuesSorted();

		/// \~English @brief Get all keys and all values at the same time
		/// \~Chinese @brief 同时取得所有键和所有值
		void pairs(Array<KeyType>& keys, Array<ValueType>& values);

		/// \~English @brief Obtain all sorted keys and all values at the same time, KeyType needs to support operator ">"
		/// \~Chinese @brief 同时取得排序后的所有键和所有值，需要KeyType支持>重载符
		void pairsSorted(Array<KeyType>& keys, Array<ValueType>& values);

		/// \~English @brief Whether the mapping table contains the mapping for the specified key
		/// \~Chinese @brief 是否包含指定键的映射
		Bool contain(KeyType key);

		/// \~English @brief Remove the mapping for the specified key (if it exists)
		/// \~Chinese @brief 移除指定键的映射（若存在）
		void remove(KeyType key);

		/// \~English @brief Get the value corresponding to the specified key, if it does not exist, return the default value
		/// \~Chinese @brief 获取指定键对应的值，若不存在则返回默认值
		ValueType get(KeyType key, ValueType defaultValue);

		/// \~English @brief Try to get the value corresponding to the specified key, if it does not exist, return FALSE
		/// \~Chinese @brief 尝试获得指定键对应的值，若不存在返回FALSE
		Bool tryGet(KeyType key, ValueType& value);

		/// \~English @brief Set the value corresponding to the specified key, if it does not exist, create a new mapping
		/// \~Chinese @brief 设置指定键对应的值，若不存在则新建映射
		void set(KeyType key, ValueType value);

		/// \~English @brief [Modifiable] Get the value reference corresponding to the specified key, if it does not exist, create a new mapping
		/// \~Chinese @brief [可修改] 获得指定键对应的值引用，若不存在则新建映射
		ValueType& operator [](KeyType key);

	private:
		Bool isNull() { return FALSE; }
		Bool isValid() { return FALSE; }
	};

	/// \~English @brief Dictionary, which can be indexed by string
	/// \~Chinese @brief 字典，可通过字符串索引
	template <typename Type> class Dictionary : public Map<String, Type>
	{
	public:
		/// \~English @brief Create a dictionary, the default number of buckets is 256
		/// \~Chinese @brief 创建字典，默认bucket个数为256
		Dictionary();

		/// \~English @brief Create a dictionary with the specified number of buckets (should be a power of 2, within the range of 16~65536)
		/// \~Chinese @brief 创建指定bucket个数的字典（应为2的幂数，范围在16~65536内）
		Dictionary(UInt bucketCount);

		/// \~English @brief [Unsafe operation] Create a dictionary based on argument list. ArgType should be Int, UInt, Double, Char* or structure and class type, etc.
		/// \~Chinese @brief [非安全操作] 基于可变参数列表创建字典并赋值，ArgType应为Int，UInt，Double，Char*或结构体和类名等
		/// \~English @param size The number of key values to be assigned
		/// \~Chinese @param size 待赋值的键值个数
		/// \~English @param firstKey The first key
		/// \~Chinese @param firstKey 首个键
		/// \~English @param firstValue The first value, the following parameters are alternately of Char* and ArgType types, the total number should be 2x the number of key values to be assigned
		/// \~Chinese @param firstValue 首个值，后面参数交替为Char*和ArgType类型，总个数应为待赋值的键值个数x2
		/// \~English @returns Created dictionary
		/// \~Chinese @returns 创建的字典
		template <typename ArgType>
		static Dictionary<Type> create(UInt size, const Char firstKey[], ArgType firstValue, ...);
	};

	/// \~English @brief Boolean key
	/// \~Chinese @brief 布尔键
	typedef NumericKey<Bool> BoolKey;

	/// \~English @brief 8-bit unsigned integer key
	/// \~Chinese @brief 8位无符号整数键
	typedef NumericKey<Byte> ByteKey;

	/// \~English @brief 16-bit unsigned integer key
	/// \~Chinese @brief 16位无符号整数键
	typedef NumericKey<Word> WordKey;

	/// \~English @brief 32-bit unsigned integer key
	/// \~Chinese @brief 32位无符号整数键
	typedef NumericKey<UInt> UIntKey;

	/// \~English @brief 64-bit unsigned integer key
	/// \~Chinese @brief 64位无符号整数键
	typedef NumericKey<ULong> ULongKey;

	/// \~English @brief 16-bit signed integer key
	/// \~Chinese @brief 16位带符号整数键
	typedef NumericKey<Short> ShortKey;

	/// \~English @brief 32-bit signed integer key
	/// \~Chinese @brief 32位带符号整数键
	typedef NumericKey<Int> IntKey;

	/// \~English @brief 64-bit signed integer key
	/// \~Chinese @brief 64位带符号整数键
	typedef NumericKey<Long> LongKey;

	/// \~English @brief Char character key
	/// \~Chinese @brief Char字符键
	typedef NumericKey<Char> CharKey;

	/// \~English @brief WChar character key
	/// \~Chinese @brief WChar字符键
	typedef NumericKey<WChar> WCharKey;

	/// \~English @brief 32-bit floating point key
	/// \~Chinese @brief 32位浮点数键
	typedef NumericKey<Float> FloatKey;

	/// \~English @brief 64-bit floating point key
	/// \~Chinese @brief 64位浮点数键
	typedef NumericKey<Double> DoubleKey;

	/// \~English @brief String key-value pair
	/// \~Chinese @brief 字符串键值对
	typedef KeyValue<String, String> StringKeyValue;

	// Binary data class / 二进制数据类 //////////////////////////////////////////////////////////////

	/// \~English @brief Binary data class
	/// \~Chinese @brief 二进制数据类
	class SPADAS_API Binary : public Object<class BinaryVars>
	{
	public:
		/// \~English @brief Class name
		/// \~Chinese @brief 类名称
		static const String TypeName;

		/// \~English @brief Create an empty data block (length is 0)
		/// \~Chinese @brief 创建一个空数据块（长度为0）
		Binary();

		/// \~English @brief Create a data block of the specified size
		/// \~Chinese @brief 创建指定大小的数据块
		/// \~English @param size The size of the data block (in bytes)
		/// \~Chinese @param size 创建数据块的大小（字节单位）
		Binary(UInt size);
		
		/// \~English @brief Create a data block of the specified size and initialize the value of all bytes
		/// \~Chinese @brief 创建指定大小的数据块，并初始化所有字节的值
		/// \~English @param size The size of the data block (in bytes)
		/// \~Chinese @param size 创建数据块的大小（字节单位）
		/// \~English @param origin The initial value of all bytes in the data block
		/// \~Chinese @param origin 数据块中所有字节的初始值
		Binary(UInt size, Byte origin);

		/// \~English @brief Create an object from a spadas::Byte array pointer, specify the size of the data block (the source data will be copied when creating)
		/// \~Chinese @brief 从一个 spadas::Byte 数组指针创建对象，需指定数据块大小（创建时将拷贝源数据）
		/// \~English @param arr Source data array pointer
		/// \~Chinese @param arr 源数据数组指针
		/// \~English @param size The size of the data block (in bytes)
		/// \~Chinese @param size 源数据数组大小（字节单位）
		Binary(Byte *arr, UInt size);
		
		/// \~English @brief Copy and create an object from a subarea of another data block
		/// \~Chinese @brief 从另一个数据块的子区域拷贝并创建对象
		/// \~English @param input Source data block
		/// \~Chinese @param input 源数据数组
		/// \~English @param region Copy region (when its size is spadas::UINF, it will be copied to the end)
		/// \~Chinese @param region 拷贝区域（为 spadas::UINF 时将拷贝至末尾）
		Binary(Binary input, Region region);

		/// \~English @brief [Unsafe operation] Create a binary data block based on the argument list
		/// \~Chinese @brief [非安全操作] 基于可变参数列表创建二进制数据块并赋值
		/// \~English @param size The size of the data block (in bytes)
		/// \~Chinese @param size 数据块大小（字节单位）
		/// \~English @param firstByte The first byte value, the following parameters are all Byte type values, the total number of bytes should be consistent with the size of the data block
		/// \~Chinese @param firstByte 首个字节值，后面参数都为Byte类型的值，总字节个数应与数据块大小一致
		static Binary create(UInt size, Byte firstByte, ...);

		/// \~English @brief Create binary data from Base64 string
		/// \~Chinese @brief 由Base64字符串创建二进制数据
		/// \~English @param base64 Input base64 string
		/// \~Chinese @param base64 输入的base64字符串
		/// \~English @returns The decoded data block, if it fails, returns an invalid object
		/// \~Chinese @returns 解码后的数据块，若失败则返回无效对象
		static Optional<Binary> createFromBase64(String base64);

		/// \~English @brief Create binary data by DES decryption (encrypted length should be a multiple of 8)
		/// \~Chinese @brief 由DES解密创建二进制数据 (encrypted长度应为8的倍数)
		/// \~English @param encrypted Encrypted DES encrypted data block
		/// \~Chinese @param encrypted DES加密数据块
		/// \~English @param key DES key
		/// \~Chinese @param key DES密钥
		/// \~English @returns The decoded data block, if it fails, returns an invalid object
		/// \~Chinese @returns 解码后的数据块，若失败则返回无效对象
		static Optional<Binary> createFromDES(Binary encrypted, String key);

		/// \~English @brief Get the data block size
		/// \~Chinese @brief 取得数据块大小
		UInt size();

		/// \~English @brief [Unsafe pointer][Modifiable] Get the head pointer of the data block
		/// \~Chinese @brief [非安全指针][可修改] 取得数据块的头指针
		Byte *data();

		/// \~English @brief [Modifiable] Get a reference to a byte
		/// \~Chinese @brief [可修改] 取得某个字节的引用
		Byte& operator [](UInt index);

		/// \~English @brief [Modifiable] Obtain the sub-data block, which will be bound to the data of this data block
		/// \~Chinese @brief [可修改] 取得子数据块，其将绑定至本数据块的数据
		Binary subBinary(UInt index, UInt size = UINF);

		/// \~English @brief Is this object invalid (size is 0)
		/// \~Chinese @brief 本对象是否无效（大小为0）
		Bool isEmpty();
		
		/// \~English @brief Reduce the data block size
		/// \~Chinese @brief 缩减数据块大小
		void trim(UInt size);

		/// \~English @brief Clone a new object
		/// \~Chinese @brief 克隆出一个新对象
		Binary clone();
		
		/// \~English @brief Assign and erase data
		/// \~Chinese @brief 赋值擦除数据
		void clear(Byte val);

		/// \~English @brief Copy data from a sub-area of another data block to a certain position of this data block
		/// \~Chinese @brief 从另一个数据块的某个子区域拷贝数据到本数据块的某个位置
		/// \~English @param src Source data block
		/// \~Chinese @param src 源数据数组
		/// \~English @param srcRegion The area copied from the source
		/// \~Chinese @param srcRegion 从源数组拷贝的区域
		/// \~English @param dstOffset The starting position of this data block copied to
		/// \~Chinese @param dstOffset 拷贝至本数据块的起始位置
		void copy(Binary src, Region srcRegion, UInt dstOffset);
		
		/// \~English @brief Divide into multiple data blocks according to the specified size
		/// \~Chinese @brief 根据指定的大小分割为多个数据块
		/// \~English @param sizes Size of each data block (in bytes)
		/// \~Chinese @param sizes 将分割成的每个数据块的大小（字节单位）
		/// \~English @returns Multiple data blocks after splitted
		/// \~Chinese @returns 分割后的多个数据块
		Array<Binary> split(Array<UInt> sizes);

		/// \~English @brief Merge multiple data blocks
		/// \~Chinese @brief 合并多个数据块
		/// \~English @param binaries Multiple data blocks to be merged
		/// \~Chinese @param binaries 将合并的多个数据块
		/// \~English @returns The merged data block
		/// \~Chinese @returns 合并后的数据块
		static Binary merge(Array<Binary> binaries);

		/// \~English @brief Concatenate another data block after this data block
		/// \~Chinese @brief 在本数据块后拼接另一个数据块
		/// \~English @param bin Another data block to be concatenated
		/// \~Chinese @param bin 将拼接的另一个数据块
		/// \~English @returns Concatenated data block
		/// \~Chinese @returns 拼接后的数据块
		Binary operator +(Binary bin);

		/// \~English @brief Generate reverse data block
		/// \~Chinese @brief 生成逆序数据块
		Binary reverse();

		/// \~English @brief Encrypt to base64 string
		/// \~Chinese @brief 加密为Base64字符串
		String toBase64();

		/// \~English @brief SHA1 encryption
		/// \~Chinese @brief SHA1加密
		Binary toSHA1();

		/// \~English @brief DES encryption (the length of this data block is recommended to be a multiple of 8, if not satisfied, fill it with 0)
		/// \~Chinese @brief DES加密(本数据块长度建议为8的倍数，若不满足则以0填满)
		/// \~English @param key DES key
		/// \~Chinese @param key DES密钥
		/// \~English @returns DES encrypted data block
		/// \~Chinese @returns DES加密后的数据块
		Binary toDES(String key);

	private:
		Bool isNull() { return FALSE; }
		Bool isValid() { return FALSE; }
	};

#if defined(SPADAS_DEBUG)
	class BinaryVars
	{
	public:
		Byte dummy[SPADAS_BINARY_DUMMY_BYTES];
		UInt size;
		Byte* data;
	};
#endif

	// String / 字符串 //////////////////////////////////////////////////////////////

	/// \~English @brief String
	/// \~Chinese @brief 字符串
	class SPADAS_API String : public Object<class StringVars>
	{
	public:
		/// \~English @brief Class name
		/// \~Chinese @brief 类名称
		static const String TypeName;

		/// \~English @brief Create an empty string
		/// \~Chinese @brief 创建空字符串
		String();

		/// \~English @brief Initialize by spadas::Char single character
		/// \~Chinese @brief 由 spadas::Char 单字符初始化
		/// \~English @param character Input character
		/// \~Chinese @param character 输入字符
		String(Char character);

		/// \~English @brief Initialize by spadas::Char single character
		/// \~Chinese @brief 由 spadas::WChar 单字符初始化
		/// \~English @param character Input character
		/// \~Chinese @param character 输入字符
		String(WChar character);

		/// \~English @brief Initialize by spadas::Char character array pointer
		/// \~Chinese @brief 由 spadas::Char 字符数组指针初始化
		/// \~English @param text Character array pointer (terminated with 0)
		/// \~Chinese @param text 字符数组指针（以0结尾）
		String(Char text[]);

		/// \~English @brief Initialize by spadas::Char character array pointer
		/// \~Chinese @brief 由 spadas::Char 字符数组指针初始化
		/// \~English @param text Character array pointer (terminated with 0)
		/// \~Chinese @param text 字符数组指针（以0结尾）
		String(const Char text[]);

		/// \~English @brief Initialize by spadas::WChar character array pointer
		/// \~Chinese @brief 由 spadas::WChar 字符数组指针初始化
		/// \~English @param text Character array pointer (terminated with 0)
		/// \~Chinese @param text 字符数组指针（以0结尾）
		String(WChar text[]);

		/// \~English @brief Initialize by spadas::WChar character array pointer
		/// \~Chinese @brief 由 spadas::WChar 字符数组指针初始化
		/// \~English @param text Character array pointer (terminated with 0)
		/// \~Chinese @param text 字符数组指针（以0结尾）
		String(const WChar text[]);

		/// \~English @brief Initialize by spadas::Char character array
		/// \~Chinese @brief 由 spadas::Char 字符数组初始化
		/// \~English @param text Character array (not required to be terminated with 0)
		/// \~Chinese @param text 字符数组（不要求以0结尾）
		String(Array<Char> text);

		/// \~English @brief Initialize by spadas::WChar character array
		/// \~Chinese @brief 由 spadas::WChar 字符数组初始化
		/// \~English @param text Character array (not required to be terminated with 0)
		/// \~Chinese @param text 字符数组（不要求以0结尾）
		String(Array<WChar> text);

		/// \~English @brief Initialize by spadas::Bool, the string will be "TRUE" or "FALSE"
		/// \~Chinese @brief 由 spadas::Bool 初始化，字符串为"TRUE"或"FALSE"
		/// \~English @param val Input boolean value
		/// \~Chinese @param val 输入布尔值
		String(Bool val);

		/// \~English @brief Initialize by spadas::Byte
		/// \~Chinese @brief 由 spadas::Byte 初始化
		/// \~English @param val Input 8-bit unsigned integer
		/// \~Chinese @param val 输入8位无符号整数
		String(Byte val);

		/// \~English @brief Initialize by spadas::Word
		/// \~Chinese @brief 由 spadas::Word 初始化
		/// \~English @param val Input 16-bit unsigned integer
		/// \~Chinese @param val 输入16位无符号整数
		String(Word val);

		/// \~English @brief Initialize by spadas::UInt
		/// \~Chinese @brief 由 spadas::UInt 初始化
		/// \~English @param val Input 32-bit unsigned integer
		/// \~Chinese @param val 输入32位无符号整数
		String(UInt val);

		/// \~English @brief Initialize by spadas::UInt, and the number of digits can be specified (fill other digits with 0)
		/// \~Chinese @brief 由 spadas::UInt 初始化，可指定位数（不足的用0补齐）
		/// \~English @param val Input 32-bit unsigned integer
		/// \~Chinese @param val 输入32位无符号整数
		/// \~English @param nDigits Specifies the number of digits
		/// \~Chinese @param nDigits 指定位数
		String(UInt val, UInt nDigits);

		/// \~English @brief Initialize by spadas::ULong
		/// \~Chinese @brief 由 spadas::ULong 初始化
		/// \~English @param val Input 64-bit unsigned integer
		/// \~Chinese @param val 输入64位无符号整数
		String(ULong val);

		/// \~English @brief Initialize by spadas::ULong, and the number of digits can be specified (fill other digits with 0)
		/// \~Chinese @brief 由 spadas::ULong 初始化，可指定位数（不足的用0补齐）
		/// \~English @param val Input 64-bit unsigned integer
		/// \~Chinese @param val 输入64位无符号整数
		/// \~English @param nDigits Specifies the number of digits
		/// \~Chinese @param nDigits 指定位数
		String(ULong val, UInt nDigits);

		/// \~English @brief Initialize by spadas::Short
		/// \~Chinese @brief 由 spadas::Short 初始化
		/// \~English @param val Input 16-bit signed integer
		/// \~Chinese @param val 输入16位带符号整数
		String(Short val);

		/// \~English @brief Initialize by spadas::Int
		/// \~Chinese @brief 由 spadas::Int 初始化
		/// \~English @param val Input 32-bit signed integer
		/// \~Chinese @param val 输入32位带符号整数
		String(Int val);

		/// \~English @brief Initialize by spadas::Long
		/// \~Chinese @brief 由 spadas::Long 初始化
		/// \~English @param val Input 64-bit signed integer
		/// \~Chinese @param val 输入64位带符号整数
		String(Long val);

		/// \~English @brief Initialize by spadas::Float, the default decimal place is 3
		/// \~Chinese @brief 由 spadas::Float 初始化，默认小数位数为3
		/// \~English @param val Input 32-bit floating point number
		/// \~Chinese @param val 输入32位浮点数
		String(Float val);

		/// \~English @brief Initialize by spadas::Double, the default decimal place is 3
		/// \~Chinese @brief 由 spadas::Double 初始化，默认小数位数为3
		/// \~English @param val Input 64-bit floating point number
		/// \~Chinese @param val 输入64位浮点数
		String(Double val);

		/// \~English @brief Initialize by spadas::Float, specifying the number of decimal places
		/// \~Chinese @brief 由 spadas::Float 初始化，并指定保留小数位数
		/// \~English @param val Input 32-bit floating point number
		/// \~Chinese @param val 输入32位浮点数
		/// \~English @param nDigits The number of decimal places, the range is 0~10 and UINF (adaptive)
		/// \~Chinese @param nDigits 保留小数位数，范围为0~10以及UINF(自适应)
		String(Float val, UInt nDigits);

		/// \~English @brief Initialize by spadas::Double, specifying the number of decimal places
		/// \~Chinese @brief 由 spadas::Double 初始化，并指定保留小数位数
		/// \~English @param val Input 64-bit floating point number
		/// \~Chinese @param val 输入64位浮点数
		/// \~English @param nDigits The number of decimal places, the range is 0~10 and UINF (adaptive)
		/// \~Chinese @param nDigits 保留小数位数，范围为0~20以及UINF(自适应)
		String(Double val, UInt nDigits);

		/// \~English @brief Create a string object from UTF-8 binary data
		/// \~Chinese @brief 从UTF-8二进制数据创建字符串对象
		/// \~English @param binary UTF-8 binary data (not required to be terminated with 0)
		/// \~Chinese @param binary UTF-8二进制数据（不要求以0结尾）
		String(Binary binary);

		/// \~English @brief Copy and create a string object from a piece of text in another string
		/// \~Chinese @brief 从另一个字符串中的一段文本拷贝并创建字符串对象
		/// \~English @param src Source string
		/// \~Chinese @param src 源字符串
		/// \~English @param region Copy region (when its size is spadas::UINF, it will be copied to the end)
		/// \~Chinese @param region 拷贝区域（为 spadas::UINF 时将拷贝至末尾）
		String(String src, Region region);

		/// \~English @brief Create a string object from any structure or object with toString method
		/// \~Chinese @brief 由任意具有toString方法的结构体或对象创建字符串对象
		template <typename Type>
		String(Type obj);

		/// \~English @brief Whether it is equal to
		/// \~Chinese @brief 是否等于
		Bool operator ==(String string);

		/// \~English @brief Whether it is not equal to
		/// \~Chinese @brief 是否不等于
		Bool operator !=(String string);

		/// \~English @brief Whether it is greater than, according to the system default string sorting order
		/// \~Chinese @brief 是否大于，按照系统默认字符串排序顺序
		Bool operator >(String string);

		/// \~English @brief Whether it is less than, according to the system default string sorting order
		/// \~Chinese @brief 是否小于，按照系统默认字符串排序顺序
		Bool operator <(String string);

		/// \~English @brief Get the hash value
		/// \~Chinese @brief 获取哈希值
		Word getHash();

		/// \~English @brief Clone a new object
		/// \~Chinese @brief 克隆出一个新对象
		String clone();
		
		/// \~English @brief Calculate and update the string length (this method must be called after calling bytes() method and changing the string data)
		/// \~Chinese @brief 计算并更新字符串长度（在调用bytes方法对字符串数据进行更改后必须调用本方法）
		void updateLength();

		/// \~English @brief [Modifiable] Obtain the head pointer of the string UTF-8 data, at least one byte of which should be 0, if the data is modified, updateLength() method must be called
		/// \~Chinese @brief [可修改] 取得字符串UTF-8数据的头指针，其中至少应有一个字节为0，若修改了数据则必须调用updateLength方法
		Byte *bytes();

		/// \~English @brief Get the number of bytes of string UTF-8 data
		/// \~Chinese @brief 取得字符串UTF-8数据的字节数
		UInt byteSize();

		/// \~English @brief Convert to a Char array, ending with 0 (so the effective length is the array length - 1)
		/// \~Chinese @brief 转换为Char数组，以0结尾（因此有效长度为数组长度-1）
		Array<Char> chars();

		/// \~English @brief Convert to a WChar array, terminated with 0 (so the effective length is the array length - 1)
		/// \~Chinese @brief 转换为WChar数组，以0结尾（因此有效长度为数组长度-1）
		Array<WChar> wchars();

		/// \~English @brief Get the string length (UTF-8 bytes)
		/// \~Chinese @brief 取得字符串长度 (UTF-8字节数)
		UInt length();

		/// \~English @brief Is it an empty string
		/// \~Chinese @brief 是否为空字符串
		Bool isEmpty();

		/// \~English @brief Concatenate another string after this string (No new objects created. That is faster, but will change the data of this object, generally used for strings created by createWithSize)
		/// \~Chinese @brief 在本字符串后拼接另一个字符串（可不创建新对象，速度更快，但会更改本对象数据，一般对基于createWithSize创建的字符串使用）
		/// \~English @param string Another string to be concatenated
		/// \~Chinese @param string 将拼接的另一个字符串
		void operator +=(String string);

		/// \~English @brief Concatenate another string after this string (need to create a new object, the speed is slow, but the data of this object will not be changed, and multiple strings can be added in succession)
		/// \~Chinese @brief 在本字符串后拼接另一个字符串（需要创建新对象，速度较慢，但不会更改本对象数据，且可多个字符串连加）
		/// \~English @param string Another string to be concatenated
		/// \~Chinese @param string 将拼接的另一个字符串
		/// \~English @returns Concatenated string
		/// \~Chinese @returns 拼接后的字符串
		String operator +(String string);

		/// \~English @brief Convert to spadas::Int number
		/// \~Chinese @brief 转换为 spadas::Int 数字
		Optional<Int> toInt();

		/// \~English @brief Convert to spadas::Long number
		/// \~Chinese @brief 转换为 spadas::Long 数字
		Optional<Long> toLong();

		/// \~English @brief Convert to spadas::Float value
		/// \~Chinese @brief 转换为 spadas::Float 数值
		Optional<Float> toFloat();

		/// \~English @brief Convert to spadas::Double value
		/// \~Chinese @brief 转换为 spadas::Double 数值
		Optional<Double> toDouble();
		
		/// \~English @brief Convert to UTF-8 binary data (not terminated with 0)
		/// \~Chinese @brief 转换为UTF-8二进制数据（不以0结尾）
		Binary toBinary();

		/// \~English @brief Convert to uppercase string
		/// \~Chinese @brief 转换为全大写字符串
		String toUpper();

		/// \~English @brief Convert to lowercase string
		/// \~Chinese @brief 转换为全小写字符串
		String toLower();

		/// \~English @brief Whether it starts with the specified string
		/// \~Chinese @brief 是否以指定字符串开头
		Bool startsWith(String target);

		/// \~English @brief Whether it ends with the specified string
		/// \~Chinese @brief 是否以指定字符串结尾
		Bool endsWith(String target);

		/// \~English @brief Search the target string and return the first character position of all found targets. For example, searching "nana" in "bananana", returns {2, 4}
		/// \~Chinese @brief 搜索目标字符串，返回所有发现目标的首字符位置。如"bananana"搜"nana"，返回{2, 4}
		Array<UInt> search(String target);
		
		/// \~English @brief Split this string with the specified string. For example, "12 34 56" is separated by spaces, and returns {"12", "34", "56"}. Note that when the string does not contain target, an empty array will be returned if the string is empty, and a scalar array will be returned if it is not empty
		/// \~Chinese @brief 用指定字符串对本字符串进行分割。如"12 34 56"按空格符分割，返回{"12", "34", "56"}。注意，本字符串不含target时，若本字符串为空则返回空数组，非空则返回标量数组
		Array<String> split(String target);

		/// \~English @brief Replace the oldString part of this string with newString, and return the replaced string
		/// \~Chinese @brief 将本字符串中oldString部分替换为newString，并返回替换后的字符串
		String replace(String oldString, String newString);

		/// \~English @brief Create an empty string whose data block size (byte unit) is the specified value
		/// \~Chinese @brief 创建数据块大小（字节单位）为指定值的空字符串
		static String createWithSize(UInt size);
		
		/// \~English @brief Represent 8-bit unsigned integer numbers in binary
		/// \~Chinese @brief 用二进制表示8位无符号整型数字
		static String createBinString(Byte value);

		/// \~English @brief Represents 16-bit unsigned integer numbers in binary
		/// \~Chinese @brief 用二进制表示16位无符号整型数字
		static String createBinString(Word value);

		/// \~English @brief Represents 32-bit unsigned integer numbers in binary
		/// \~Chinese @brief 用二进制表示32位无符号整型数字
		static String createBinString(UInt value);

		/// \~English @brief Represents 64-bit unsigned integer numbers in binary
		/// \~Chinese @brief 用二进制表示64位无符号整型数字
		static String createBinString(ULong value);
		
		/// \~English @brief Use hexadecimal representation of numbers to represent 8-bit unsigned integer numbers
		/// \~Chinese @brief 用十六进制表示数字表示8位无符号整型数字
		static String createHexString(Byte value);

		/// \~English @brief Use hexadecimal representation of numbers to represent 16-bit unsigned integer numbers
		/// \~Chinese @brief 用十六进制表示数字表示16位无符号整型数字
		static String createHexString(Word value);

		/// \~English @brief Use hexadecimal representation of numbers to represent 32-bit unsigned integer numbers
		/// \~Chinese @brief 用十六进制表示数字表示32位无符号整型数字
		static String createHexString(UInt value);

		/// \~English @brief Use hexadecimal representation of numbers to represent 64-bit unsigned integer numbers
		/// \~Chinese @brief 用十六进制表示数字表示64位无符号整型数字
		static String createHexString(ULong value);
		
		/// \~English @brief Represents a block of binary data in hexadecimal (you can specify the number of bytes printed per line)
		/// \~Chinese @brief 用十六进制表示一段二进制数据块（可指定每行打印的字节数）
		/// \~English @param bin Input binary data block
		/// \~Chinese @param bin 输入二进制数据块
		/// \~English @param nBytesPerRow The number of bytes printed in each row
		/// \~Chinese @param nBytesPerRow 每行打印的字节数
		/// \~English @returns The representation string of the binary data block
		/// \~Chinese @returns 二进制数据块的表述字符串
		static String createHexString(Binary bin, UInt nBytesPerRow = 8);
		
		/// \~English @brief Merge multiple strings with the specified separator
		/// \~Chinese @brief 拼接多个字符串，字符串间以指定分割符分割
		/// \~English @param strs Input multiple strings
		/// \~Chinese @param strs 输入的多个字符串
		/// \~English @param separator Separator
		/// \~Chinese @param separator 分隔符
		/// \~English @returns The string concatenated by the specified separator
		/// \~Chinese @returns 按指定分隔符拼接的字符串
		static String mergeStrings(Array<String> strs, String separator = "\n");

		/// \~English @brief Merge multiple strings created by spadas::String constructor with the specified separator
		/// \~Chinese @brief 以String构造函数拼接数组，字符串间以指定分割符分割
		/// \~English @param strs Input array
		/// \~Chinese @param strs 输入数组
		/// \~English @param separator Separator
		/// \~Chinese @param separator 分隔符
		/// \~English @returns The string concatenated by the specified separator
		/// \~Chinese @returns 按指定分隔符拼接的字符串
		template <typename Type>
		static String merge(Array<Type> arr, String separator = "\n");

	private:
		Bool isNull() { return FALSE; }
		Bool isValid() { return FALSE; }
	};

#if defined(SPADAS_DEBUG)
	class StringVars
	{
	public:
		Byte dummy[SPADAS_STRING_DUMMY_BYTES];
		Char* data;
		UInt length;
	};
#endif

	// Enumeration object / 枚举对象 //////////////////////////////////////////////////////////////

	/// \~English @brief [Multithread safe] Enumeration container. If there is no need to convert strings or use them as keys, you may use the enum class directly
	/// \~English @details Type is an enumeration definition class, which needs to implement the following three static methods: \n
	/// - static Bool isValid(Int val); // Tell if the specified value is valid \n
	/// - static String toString(Int val); // Convert to text representation \n
	/// - static Int defaultValue(); // Give the default value
	/// \~Chinese @brief [多线程安全] 枚举类型容器。若无转字符串及作为键的需求，可直接使用enum class
	/// \~Chinese @details 其中Type为枚举定义类，该类中需要实现以下三个静态方法： \n
	/// - static Bool isValid(Int val);		// 告知指定值是否有效 \n
	/// - static String toString(Int val);	// 用文本表述指定值得含义 \n
	/// - static Int defaultValue();			// 给出默认值
	template <typename Type> class Enum
	{
	public:
		/// \~English @brief Create an object with default value
		/// \~Chinese @brief 创建一个对象，使用默认值
		Enum();

		/// \~English @brief Create an object, using the specified value (if the value is invalid, use the default value)
		/// \~Chinese @brief 创建一个对象，使用指定值（若该值无效则使用默认值）
		Enum(const Int val);

		/// \~English @brief Whether it is equal to
		/// \~Chinese @brief 是否等于
		Bool operator ==(Enum<Type> enumeration);

		/// \~English @brief Whether it is not equal to
		/// \~Chinese @brief 是否不等于
		Bool operator !=(Enum<Type> enumeration);

		/// \~English @brief Get the hash value
		/// \~Chinese @brief 获取哈希值
		Word getHash();

		/// \~English @brief Get the value of the enumeration object
		/// \~Chinese @brief 取得枚举对象的值
		Int value();

		/// \~English @brief Get the text representation of the enumeration object
		/// \~Chinese @brief 取得枚举对象的文本表述
		String toString();

	private:
		Int val;
	};

	// Console / 控制台 //////////////////////////////////////////////////////////////

	/// \~English @brief Key enumeration type
	/// \~Chinese @brief 按键枚举类型
	class SPADAS_API Key
	{
	public:
		static const Int None = 0;
		static const Int Enter = 1;
		static const Int Space = 2;
		static const Int Back = 3;
		static const Int Esc = 4;
		static const Int Tab = 5;
		static const Int Shift = 6;
		static const Int Ctrl = 7;
		static const Int Insert = 8;
		static const Int Delete = 9;
		static const Int Home = 10;
		static const Int End = 11;
		static const Int PageUp = 12;
		static const Int PageDown = 13;
		static const Int Up = 14;
		static const Int Down = 15;
		static const Int Left = 16;
		static const Int Right = 17;
		static const Int Comma = 18;
		static const Int Period = 19;
		static const Int Key0 = 20;
		static const Int Key1 = 21;
		static const Int Key2 = 22;
		static const Int Key3 = 23;
		static const Int Key4 = 24;
		static const Int Key5 = 25;
		static const Int Key6 = 26;
		static const Int Key7 = 27;
		static const Int Key8 = 28;
		static const Int Key9 = 29;
		static const Int A = 30;
		static const Int B = 31;
		static const Int C = 32;
		static const Int D = 33;
		static const Int E = 34;
		static const Int F = 35;
		static const Int G = 36;
		static const Int H = 37;
		static const Int I = 38;
		static const Int J = 39;
		static const Int K = 40;
		static const Int L = 41;
		static const Int M = 42;
		static const Int N = 43;
		static const Int O = 44;
		static const Int P = 45;
		static const Int Q = 46;
		static const Int R = 47;
		static const Int S = 48;
		static const Int T = 49;
		static const Int U = 50;
		static const Int V = 51;
		static const Int W = 52;
		static const Int X = 53;
		static const Int Y = 54;
		static const Int Z = 55;
		static const Int F1 = 56;
		static const Int F2 = 57;
		static const Int F3 = 58;
		static const Int F4 = 59;
		static const Int F5 = 60;
		static const Int F6 = 61;
		static const Int F7 = 62;
		static const Int F8 = 63;
		static const Int Unknown = 100;
		
		static Bool isValid(Int val);
		static String toString(Int val);
		static Int defaultValue();
	};

	/// \~English @brief Message level
	/// \~Chinese @brief 消息级别
	class SPADAS_API MessageLevel
	{
	public:
		static const Int Debug = 1;
		static const Int Info = 2;
		static const Int Warning = 3;
		static const Int Error = 4;

		static Bool isValid(Int val);
		static String toString(Int val);
		static Int defaultValue();
	};

	/// \~English @brief Console interface
	/// \~Chinese @brief 控制台接口
	class SPADAS_API IConsole
	{
	public:
		virtual ~IConsole() {};

		/// \~English @brief Print the text with the message level of the information
		/// \~Chinese @brief 打印文本信息，并指定该信息的消息级别
		virtual void print(String text, Enum<MessageLevel> level);

		/// \~English @brief [Optional] Whether it supports scanning a line of text input
		/// \~Chinese @brief [可选] 是否支持扫描一行文本输入
		virtual Bool supportScan();

		/// \~English @brief [Optional] Scan a line of text input
		/// \~Chinese @brief [可选] 扫描一行文本输入
		virtual String scan();

		/// \~English @brief [Optional] Whether it supports scanning key input
		/// \~Chinese @brief [可选] 是否支持扫描按键输入
		virtual Bool supportCheckKey();

		/// \~English @brief [Optional] Scan key input
		/// \~Chinese @brief [可选] 扫描按键输入
		virtual Enum<Key> checkKey();
	};

	/// \~English @brief Debugging information record interface
	/// \~Chinese @brief 记录调试信息接口
	class SPADAS_API ILogger
	{
	public:
		virtual ~ILogger() {}

		/// \~English @brief Record debugging information
		/// \~Chinese @brief 记录调试信息
		/// \~English @param text Debug information
		/// \~Chinese @param text 调试信息
		virtual void print(String text);
	};

	/// \~English @brief Console function namespace
	/// \~Chinese @brief 控制台函数命名空间
	namespace console
	{
		/// \~English @brief Pop up a window to display text information (blocking)
		/// \~Chinese @brief 弹出小窗口显示文本信息（阻塞）
		SPADAS_API void popup(String text);
		
		/// \~English @brief Print debug level text information
		/// \~Chinese @brief 打印调试级别文本信息
		SPADAS_API void print(String text);

		/// \~English @brief Print text information with the specified message level
		/// \~Chinese @brief 按指定消息级别打印文本信息
		SPADAS_API void print(String text, Enum<MessageLevel> level);

		/// \~English @brief Scan a line of text input (higher priority than spadas::console::waitKey )
		/// \~Chinese @brief 扫描一行文本输入 (优先级高于 spadas::console::waitKey )
		SPADAS_API String scan();
		
		/// \~English @brief Wait for a key input (higher priority than spadas::console::checkKey )
		/// \~Chinese @brief 等待一个按键输入 (优先级高于 spadas::console::checkKey )
		SPADAS_API Enum<Key> waitKey();

		/// \~English @brief Whether there is a key input, if there is, return the key, if not, return Key::None
		/// \~Chinese @brief 是否有按键输入，若有则返回该按键，无则返回 Key::None
		SPADAS_API Enum<Key> checkKey();

		/// \~English @brief Redirect the console interface and return the old interface. After redirection, spadas::console::print , spadas::console::scan , spadas::console::waitKey , spadas::console::checkKey will use this interface (the default is invalid interface, use the default console)
		/// \~Chinese @brief 重定向控制台接口并返回旧接口，重定向后 spadas::console::print 、 spadas::console::scan 、 spadas::console::waitKey 、 spadas::console::checkKey 将使用该接口执行 (默认为无效接口，使用默认控制台)
		SPADAS_API Interface<IConsole> useConsole(Interface<IConsole> target);

		/// \~English @brief Redirect the debug information record interface of the current thread and return the old interface. After redirection, spadas::console::print at the debug level will use this interface to print (the default is invalid interface, use the IConsole interface to print)
		/// \~Chinese @brief 重定向当前线程的记录调试信息接口并返回旧接口，重定向后调试级别的 spadas::console::print 将使用该接口打印 (默认为无效接口，使用IConsole接口打印)
		SPADAS_API Interface<ILogger> useThreadLogger(Interface<ILogger> target);
	}

	// File system / 文件系统 //////////////////////////////////////////////////////////////

	/// \~English @brief File path
	/// \~Chinese @brief 文件路径
	class SPADAS_API Path : public Object<class PathVars>
	{
	public:
		/// \~English @brief Class name
		/// \~Chinese @brief 类名称
		static const String TypeName;

		/// \~English @brief Invalid object
		/// \~Chinese @brief 无效对象
		Path();

		/// \~English @brief Initialize based on an absolute or relative path string
		/// \~Chinese @brief 基于一个绝对路径或相对路径的字符串初始化
		/// \~English @param pathString Absolute path or relative path string, the relative path will be based on the current working directory \n
		/// - If it is a folder, it needs to be explicitly terminated with the path separator "/" or "\" \n
		/// - If it is an empty string or ".", it means "./", which is the current working directory
		/// \~Chinese @param pathString 绝对路径或相对路径字符串，相对路径将基于当前工作目录 \n
		/// - 若为文件夹，需显式的以路径分隔符/或\结尾 \n
		/// - 若为空字符串或"."都表示"./"，即当前工作目录
		Path(String pathString);

		/// \~English @brief Get the file or folder name (without suffix)
		/// \~Chinese @brief 取得文件或文件夹名称（不包含后缀）
		String name();

		/// \~English @brief Get file or folder suffix (.xxx)
		/// \~Chinese @brief 取得文件或文件夹后缀 (.xxx)
		String extension();

		/// \~English @brief Get the full name of the file or folder (name and suffix)
		/// \~Chinese @brief 取得文件或文件夹全名 (名称加后缀)
		String fullName();

		/// \~English @brief Get the absolute path string of the file or folder (the folder ends with the path separator "/" or "\")
		/// \~Chinese @brief 取得文件或文件夹的绝对路径字符串 (文件夹以路径分隔符/或\结尾)
		String fullPath();

		/// \~English @brief Compare the two paths to see if they are consistent
		/// \~Chinese @brief 比较两个路径是否一致
		Bool operator ==(Path path);

		/// \~English @brief Compare whether the two paths are inconsistent
		/// \~Chinese @brief 比较两个路径是否不一致
		Bool operator !=(Path path);

		/// \~English @brief Get the hash value
		/// \~Chinese @brief 获取哈希值
		Word getHash();

		/// \~English @brief Check if the file or folder exists
		/// \~Chinese @brief 判断文件或文件夹是否存在
		Bool exist();

		/// \~English @brief Is it a file path
		/// \~Chinese @brief 是否为文件路径
		Bool isFile();

		/// \~English @brief Is it a folder path
		/// \~Chinese @brief 是否为文件夹路径
		Bool isFolder();

		/// \~English @brief Remove the file or folder if it exists
		/// \~Chinese @brief 若存在则移除文件或文件夹
		void remove();

		/// \~English @brief [File] Move the file to the target location
		/// \~Chinese @brief [仅限文件] 移动文件至目标位置
		/// \~English @param dstPath Target file path
		/// \~Chinese @param dstPath 目标文件路径
		void moveTo(Path dstPath);

		/// \~English @brief [Folder] Move the folder to the target location
		/// \~Chinese @brief [仅限文件夹] 移动文件夹至目标位置
		/// \~English @param dstPath Target folder path
		/// \~Chinese @param dstPath 目标文件夹路径
		/// \~English @param mergeDst Whether to merge with the target folder
		/// \~Chinese @param mergeDst 是否与目标文件夹合并
		void moveTo(Path dstPath, Bool mergeDst);

		/// \~English @brief [File] copy the file to the target location
		/// \~Chinese @brief [仅限文件] 拷贝文件至目标位置
		/// \~English @param dstPath Target file path
		/// \~Chinese @param dstPath 目标文件路径
		void copyTo(Path dstPath);

		/// \~English @brief [Folder] copy the folder to the target location
		/// \~Chinese @brief [仅限文件夹] 拷贝文件夹至目标位置
		/// \~English @param dstPath Target folder path
		/// \~Chinese @param dstPath 目标文件夹路径
		/// \~English @param mergeDst Whether to merge with the target folder
		/// \~Chinese @param mergeDst 是否与目标文件夹合并
		void copyTo(Path dstPath, Bool mergeDst);

		/// \~English @brief [File] Get the size of the file (return 0 when the file does not exist)
		/// \~Chinese @brief [仅限文件] 取得文件大小 (当文件不存在返回0)
		ULong fileSize();

		/// \~English @brief [File] Create or recreate an empty file with the specified size, and the data is all 0
		/// \~Chinese @brief [仅限文件] 创建或重新创建一个指定大小的空文件，数据都为0
		void fileCreate(UInt size);

		/// \~English @brief [Folder] create the folder if it does not exist
		/// \~Chinese @brief [仅限文件夹] 若不存在则创建文件夹
		void folderMake();

		/// \~English @brief [Folder] Obtain all files and folder paths in the folder (if includeAllLevels is TRUE, return all levels of sub-files and sub-folders in the folder. If there are lots of contents, the speed may be very slow)
		/// \~Chinese @brief [仅限文件夹] 取得文件夹内所有文件和文件夹路径（若includeAllLevels为TRUE，则返回文件夹内所有层级的子文件和子文件夹。若内容较多则速度可能很慢）
		Array<Path> folderContents(Bool includeAllLevels = FALSE);

		/// \~English @brief [Folder] Generate child file path
		/// \~Chinese @brief [仅限文件夹] 生成子文件路径
		Path childFile(String childFullName);

		/// \~English @brief [Folder] Generate subfolder path
		/// \~Chinese @brief [仅限文件夹] 生成子文件夹路径（无需在末尾加路径分隔符）
		Path childFolder(String childFullName);

		/// \~English @brief [Folder] Generate a child file path or subfolder path based on the input relative path (folders end with path separator "/" or "\")
		/// \~Chinese @brief [仅限文件夹] 输入当前文件夹内的相对路径，生成文件或文件夹路径 (文件夹以路径分隔符/或\结尾)
		Path childPath(String pathString);

		/// \~English @brief [Folder] Check if the target path is contained within the current folder
		/// \~Chinese @brief [仅限文件夹] 检查目标路径是否包含在当前文件夹内
		Bool contain(Path path);

		/// \~English @brief [Folder] Check if the target path is contained within the current folder. If included, output the relative path based on the current folder to pathString
		/// \~Chinese @brief [仅限文件夹] 检查目标路径是否包含在当前文件夹内。若包含，则输出基于当前文件夹的相对路径至pathString
		Bool contain(Path path, String& pathString);

		/// \~English @brief Obtain the parent folder path of the file or folder (if this path is the root directory of the disk, such as c:\, etc., an invalid path will be returned)
		/// \~Chinese @brief 获得文件或文件夹的上层文件夹 (如果本路径为磁盘根目录，如c:\等，则返回无效路径)
		Path parentFolder();

		/// \~English @brief Get the path separator under the current operating system
		/// \~Chinese @brief 获得当前操作系统下的路径分隔符
		static String separator();

		/// \~English @brief Get the working directory
		/// \~Chinese @brief 获得工作目录
		static Path workPath();

		/// \~English @brief Get the directory where the executable program is located
		/// \~Chinese @brief 获得可执行程序所在目录
		static Path executableFolderPath();

		/// \~English @brief Get user root directory
		/// \~Chinese @brief 获得用户根目录
		static Path homePath();

		/// \~English @brief Get the SpadasFiles directory
		/// \~Chinese @brief 获得SpadasFiles目录
		static Path spadasFilesPath();

		/// \~English @brief Set the working directory (note that it should end with a path separator "/" or "\")
		/// \~Chinese @brief 设置工作目录 (注意应以路径分隔符/或\结尾)
		static void setWorkPath(String pathString);

		/// \~English @brief Set the directory where the executable program is located (note that it should end with the path separator "/" or "\")
		/// \~Chinese @brief 设置可执行程序所在目录 (注意应以路径分隔符/或\结尾)
		static void setExecutableFolderPath(String pathString);
	};

	/// \~English @brief File data I/O
	/// \~Chinese @brief 文件数据I/O
	class SPADAS_API File : public Object<class FileVars>
	{
	public:
		/// \~English @brief Class name
		/// \~Chinese @brief 类名称
		static const String TypeName;

		/// \~English @brief Invalid object
		/// \~Chinese @brief 无效对象
		File();

		/// \~English @brief Open the file in binary input mode
		/// \~Chinese @brief 以二进制输入模式打开文件 
		static File openBinary(Path filePath);

		/// \~English @brief Open the file in text input mode (UTF-8 BOM header will be detected)
		/// \~Chinese @brief 以文本输入模式打开文件（将检测UTF-8 BOM头）
		static File openText(Path filePath);

		/// \~English @brief Create or recreate the file in binary output mode
		/// \~Chinese @brief 以二进制输出模式创建或重新创建文件
		static File createBinary(Path filePath);

		/// \~English @brief Create or recreate a file in text output mode (UTF-8 encoding, with BOM header, enter as "\n")
		/// \~Chinese @brief 以文本输出模式创建或重新创建文件（UTF-8编码，带BOM头，回车符\n）
		static File createText(Path filePath);

		/// \~English @brief Close file I/O (in output mode, it will write all data in the buffer to disk before closing)
		/// \~Chinese @brief 关闭文件I/O（在输出模式下，关闭前将缓冲区中所有数据写入磁盘）
		void close();

		/// \~English @brief Whether it is closed (invalid objects also return TRUE)
		/// \~Chinese @brief 是否已关闭（无效对象也返回TRUE）
		Bool isClosed();

		/// \~English @brief Get the current I/O location, byte unit (if closed, return ULINF)
		/// \~Chinese @brief 获得当前的I/O位置，字节单位 (若已关闭则返回ULINF)
		ULong position();

		/// \~English @brief Get the current file size, which will be equal to position in output mode (if closed, return ULINF)
		/// \~Chinese @brief 获得当前文件大小，输出模式下等于position (若已关闭则返回ULINF)
		ULong size();

		/// \~English @brief Get the path of the current file
		/// \~Chinese @brief 获得当前文件的路径
		Path path();

		/// \~English @brief [Text output mode] print a line of text to the buffer
		/// \~Chinese @brief [文本输出模式] 打印一行文本至缓冲区
		void print(String text);

		/// \~English @brief [Output Mode] Output a block of binary data to the buffer
		/// \~Chinese @brief [输出模式] 输出一块二进制数据至缓冲区
		void output(Binary data);

		/// \~English @brief [Output Mode] Write buffer data to disk
		/// \~Chinese @brief [输出模式] 将缓冲区数据写入磁盘
		void flush();

		/// \~English @brief [Text input mode] Scan a line of text from a file
		/// \~Chinese @brief [文本输入模式] 从文件扫描一行文本
		String scan();

		/// \~English @brief [Input Mode] Input the binary data of the specified size from the file, the actual size is subject to the output
		/// \~Chinese @brief [输入模式] 从文件输入指定大小的二进制数据，实际大小以输出为准
		Binary input(UInt size = UINF);

		/// \~English @brief [Input Mode] Move to the specified I/O position, byte unit, and return which position it actually moves to
		/// \~Chinese @brief [输入模式] 移动至指定的I/O位置，字节单位，并返回实际移动到哪个位置
		ULong seek(ULong pos);

		/// \~English @brief [Input mode] Weather it has reached the end of the file
		/// \~Chinese @brief [输入模式] 是否已到文件结尾
		Bool endOfFile();
	};

	/// \~English @brief Print to file
	/// \~Chinese @brief 打印至文件
	class SPADAS_API FileConsole : public Object<class FileConsoleVars>, public IConsole
	{
	public:
		/// \~English @brief Class name
		/// \~Chinese @brief 类名称
		static const String TypeName;

		/// \~English @brief Create an object to ignore all printing
		/// \~Chinese @brief 创建用于忽略所有打印的对象
		FileConsole();

		/// \~English @brief Create an object related to the path, all printing will be written to this file
		/// \~Chinese @brief 根据路径创建对象，所有打印将写入至该文件
		FileConsole(Path filePath);

		/// \~English @brief Implement the print interface
		/// \~Chinese @brief 实现打印接口
		void print(String text, Enum<MessageLevel> level);

	private:
		Bool isNull() { return FALSE; }
		Bool isValid() { return FALSE; }
	};

	// XML //////////////////////////////////////////////////////////////

	/// \~English @brief Attribute of XML tag (the format is name="value")
	/// \~Chinese @brief XML标签的属性（格式为name="value"）
	struct XMLAttribute
	{
		/// \~English @brief Attribute name
		/// \~Chinese @brief 属性名
		String name;

		/// \~English @brief Attribute value
		/// \~Chinese @brief 属性值
		String value;

		SPADAS_API XMLAttribute();
		SPADAS_API XMLAttribute(String name, String value);
	};

	/// \~English @brief XML element
	/// \~Chinese @brief XML元素
	struct XMLElement
	{
		/// \~English @brief Element tag
		/// \~Chinese @brief 元素标签
		String tag;

		/// \~English @brief Attribute list
		/// \~Chinese @brief 元素属性列表
		Array<XMLAttribute> attributes;

		/// \~English @brief Element content (content will be ignored if it contains child nodes)
		/// \~Chinese @brief 元素内容（若含有子节点则content将被忽略）
		String content;

		SPADAS_API XMLElement();
		SPADAS_API XMLElement(String tag);
		SPADAS_API XMLElement(String tag, Array<XMLAttribute> attributes, String content);
	};

	/// \~English @brief XML element node
	/// \~Chinese @brief XML元素节点
	typedef TreeNode<XMLElement> XMLNode;

	/// \~English @brief XML document object
	/// \~Chinese @brief XML文档对象
	class SPADAS_API XML : public Object<class XMLVars>
	{
	public:
		/// \~English @brief Class name
		/// \~Chinese @brief 类名称
		static const String TypeName;

		/// \~English @brief Create an empty document (root node's tag is "root")
		/// \~Chinese @brief 创建一个空文档（根标签为root）
		XML();

		/// \~English @brief Create a document based on XML element node (cloning the entire tree structure with the input node as the root node)
		/// \~Chinese @brief 通过一个XML元素节点创建文档（以输入节点为根节点克隆整个树结构）
		XML(XMLNode xmlRoot);

		/// \~English @brief Clone the XML tree
		/// \~Chinese @brief 复制对象
		XML clone();

		/// \~English @brief [Modifiable] Get the root node of the current document
		/// \~Chinese @brief [可修改] 获得当前文档的根节点
		XMLNode globalRoot();

		/// \~English @brief Save the XML document to the .xml file (UTF-8 encoding with BOM header)
		/// \~Chinese @brief 保存XML文档至.xml文件（UTF-8编码带BOM头）
		void save(Path xmlFilePath);

		/// \~English @brief Get the binary data representation of the current document (UTF-8 encoding)
		/// \~Chinese @brief 获得当前文档的二进制数据表示（UTF-8编码）
		Binary toBinary();

		/// \~English @brief Read the .xml file, which should be encoded in UTF-8, and preferably with a BOM file header
		/// \~Chinese @brief 读取.xml文件，该文件应按UTF-8编码，且最好带BOM文件头
		static Optional<XML> createFromFile(Path xmlFilePath);

		/// \~English @brief Create an XML document from UTF-8 encoded binary data
		/// \~Chinese @brief 由UTF-8编码的二进制数据创建XML文档
		static Optional<XML> createFromBinary(Binary xmlBinary);

		/// \~English @brief Get all child nodes with the specified tag name
		/// \~Chinese @brief 获得输入节点的所有具有指定标签名字的子节点
		static Array<XMLNode> nodeLeavesWithTagName(XMLNode node, String tagName);

		/// \~English @brief Get the first child node with the specified tag name, if it does not exist, return FALSE
		/// \~Chinese @brief 获得输入节点的首个具有指定标签名字的子节点，若不存在返回FALSE
		static Bool firstNodeLeafWithTagName(XMLNode node, String tagName, XMLNode& output);

		/// \~English @brief Convert the XML attribute array to a spadas::String dictionary
		/// \~Chinese @brief XML属性数组转换为字符串型字典
		static Dictionary<String> attributesToDictionary(Array<XMLAttribute> attributes);

		/// \~English @brief Convert the spadas::String dictionary to a XML attribute array
		/// \~Chinese @brief 字符串型字典转换为XML属性数组
		static Array<XMLAttribute> dictionaryToAttributes(Dictionary<String> dict);

	private:
		Bool isNull() { return FALSE; }
		Bool isValid() { return FALSE; }
	};

	// Image / 图像 //////////////////////////////////////////////////////////////

	/// \~English @brief Image data pointer
	/// \~Chinese @brief 图像数据指针
	struct ImagePointer
	{
		/// \~English @brief Image width
		/// \~Chinese @brief 图像宽度
		UInt width;

		/// \~English @brief Image height
		/// \~Chinese @brief 图像高度
		UInt height;

		/// \~English @brief Whether it is a color image, the color image stores data in BGR order
		/// \~Chinese @brief 是否为彩色图像，彩色图像按BGR顺序存储数据
		Bool isColor;

		/// \~English @brief The number of bytes of each line of image, which is a multiple of 8
		/// \~Chinese @brief 每行图像的字节数，为8的倍数
		UInt rowBytes;

		/// \~English @brief Image data, the number of data bytes is a multiple of 8
		/// \~Chinese @brief 图像数据，数据字节数为8的倍数
		Array<ULong> data;

		/// \~English @brief The default constructor, the data is empty
		/// \~Chinese @brief 默认构造函数，数据为空
		SPADAS_API ImagePointer();

		/// \~English @brief Initialize based on the image size and whether it is a color image
		/// \~Chinese @brief 基于图像大小和是否为彩色图像进行初始化
		SPADAS_API ImagePointer(Size2D size, Bool isColor);
	};

	/// \~English @brief Pixel aspect ratio
	/// \~Chinese @brief 像素aspect ratio
	class SPADAS_API AspectRatio
	{
	public:
		static const Int _1_1 = 1;
		static const Int _4_3 = 2;
		static const Int _10_11 = 3;
		static const Int _12_11 = 4;
		static const Int _16_11 = 5;
		static const Int _40_33 = 6;

		static Bool isValid(Int val);
		static String toString(Int val);
		static Int defaultValue();
	};

	/// \~English @brief Frame rate
	/// \~Chinese @brief 帧率 
	class SPADAS_API FrameRate
	{
	public:
		static const Int Unknown = 0;
		static const Int _23_98 = 1; // 24/1001
		static const Int _24 = 2;
		static const Int _25 = 3;
		static const Int _29_97 = 4; // 30/1001
		static const Int _30 = 5;
		static const Int _50 = 6;
		static const Int _59_94 = 7; // 60/1001
		static const Int _60 = 8;

		static Bool isValid(Int val);
		static String toString(Int val);
		static Int defaultValue();

		static Float rate(Enum<FrameRate> frameRate); // [fps]
		static Float period(Enum<FrameRate> frameRate); // [ms]
	};

	/// \~English @brief Image scanning method
	/// \~Chinese @brief 图像扫描方式
	class SPADAS_API InterlaceMode
	{
	public:
		/// \~English @brief Progressive scan
		/// \~Chinese @brief 逐行扫描
		static const Int Progressive = 1;

		/// \~English @brief Interlaced scanning, rows 024... are the field scanned earlier
		/// \~Chinese @brief 隔行扫描，024行...为较早扫描的场
		static const Int UpperFirst = 2;

		/// \~English @brief Interlaced scanning, rows 135... are the field scanned earlier
		/// \~Chinese @brief 隔行扫描，135行...为较早扫描的场
		static const Int LowerFirst = 3;

		static Bool isValid(Int val);
		static String toString(Int val);
		static Int defaultValue();

		static Bool isInterlaced(Enum<InterlaceMode> mode);
	};

	/// \~English @brief 3D merge method
	/// \~Chinese @brief 3D合并方式
	class SPADAS_API MergeMode
	{
	public:
		static const Int Normal = 1; // Not 3D / 非3D
		static const Int LeftEye = 2;
		static const Int RightEye = 3;
		static const Int HalfSideBySide = 4;
		static const Int FullSideBySide = 5;
		static const Int LineByLineLR = 6; // Rows 024... as left image, 135... as right image / 024...行为左机图像, 135...行为右机图像
		static const Int LineByLineRL = 7; // Rows 135... as left image, 024... as right image / 024...行为右机图像, 135...行为左机图像

		static Bool isValid(Int val);
		static String toString(Int val);
		static Int defaultValue();

		static Bool is3DMerged(Enum<MergeMode> mode);
	};

	/// \~English @brief Time code
	/// \~Chinese @brief 时间编码
	struct TimeCode
	{
		UInt hour;
		UInt minute;
		UInt second;
		UInt frame;

		SPADAS_API TimeCode();
		SPADAS_API TimeCode(UInt hour, UInt minute, UInt second, UInt frame);
		SPADAS_API Bool operator ==(TimeCode time);
		SPADAS_API Bool operator !=(TimeCode time);
		SPADAS_API String toString();
	};

	/// \~English @brief The color represented by RGB
	/// \~Chinese @brief 由RGB表述的颜色
	struct ColorRGB
	{
		Byte r;
		Byte g;
		Byte b;

		SPADAS_API ColorRGB();
		SPADAS_API ColorRGB(Byte r, Byte g, Byte b);
		SPADAS_API Bool operator ==(ColorRGB color);
		SPADAS_API Bool operator !=(ColorRGB color);
		SPADAS_API String toString();
	};

	/// \~English @brief Image Standard Resolution
	/// \~Chinese @brief 图像标准分辨率
	class SPADAS_API ImageResolution
	{
	public:
		/// \~ @brief 320 x 240 (4:3)
		static const Int QVGA = 1;

		/// \~ @brief 640 x 480 (4:3)
		static const Int VGA = 2; 

		/// \~ @brief 720 x 576 (5:4)
		static const Int PAL720 = 3; 

		/// \~ @brief 768 x 576 (4:3)
		static const Int PAL768 = 4; 

		/// \~ @brief 800 x 600 (4:3)
		static const Int SVGA = 5; 

		/// \~ @brief 1024 x 768 (4:3)
		static const Int XGA = 6; 

		/// \~ @brief 1280 x 800 (8:5)
		static const Int WXGA = 7; 

		/// \~ @brief 1280 x 1024 (5:4)
		static const Int SXGA = 8; 

		/// \~ @brief 1400 x 1050 (4:3)
		static const Int SXGAPlus = 9; 

		/// \~ @brief 1600 x 1200 (4:3)
		static const Int UXGA = 10; 

		/// \~ @brief 1680 x 1050 (8:5)
		static const Int WSXGAPlus = 11; 

		/// \~ @brief 1280 x 720 (16:9)
		static const Int HD720 = 12; 

		/// \~ @brief 1920 x 1080 (16:9)
		static const Int HD1080 = 13; 

		static Bool isValid(Int val);
		static String toString(Int val);
		static Int defaultValue();

		static Size2D size(Enum<ImageResolution> resolution);
	};

	/// \~English @brief Pixel format
	/// \~Chinese @brief 像素格式
	class SPADAS_API PixelFormat
	{
	public:
		static const Int ByteBGR = 1; // OpenCV compatible / OpenCV兼容
		static const Int ByteRGB = 2; // OpenCV incompatible / OpenCV不兼容
		static const Int ByteGray = 3; // OpenCV compatible / OpenCV兼容
		static const Int ByteUYVY = 4; // YUV(YCbCr)4:2:2, OpenCV incompatible / OpenCV不兼容
		static const Int ByteYUV = 5; // YUV(YCbCr)4:4:4, OpenCV incompatible / OpenCV不兼容
		static const Int ByteBool = 6; // OpenCV incompatible / OpenCV不兼容, TRUE: 255, FALSE: 0
		static const Int ByteBGRA = 7; // OpenCV compatible / OpenCV兼容
		static const Int ByteRGBA = 8; // OpenCV incompatible / OpenCV不兼容
		static const Int WordBGR = 21; // OpenCV compatible / OpenCV兼容, 0~65535
		static const Int WordGray = 22; // OpenCV compatible / OpenCV兼容, 0~65535
		static const Int FloatBGR = 31; // OpenCV compatible / OpenCV兼容, 0.0~1.0
		static const Int FloatGray = 32; // OpenCV compatible / OpenCV兼容, 0.0~1.0
		static const Int FloatHSV = 33; // OpenCV incompatible / OpenCV不兼容, H(0.0~360.0) S(0.0~1.0) V(0.0~1.0)

		static Bool isValid(Int val);
		static String toString(Int val);
		static Int defaultValue();

		static Bool isColor(Enum<PixelFormat> format);
		static Bool hasAlpha(Enum<PixelFormat> format);
		static UInt nChannels(Enum<PixelFormat> format);
		static UInt bytesPerPixel(Enum<PixelFormat> format);
	};

	/// \~English @brief Sampling method for image resizing
	/// \~Chinese @brief 图像大小调整时的采样方式
	class SPADAS_API ResizeMode
	{
	public:
		static const Int Nearest = 1;
		static const Int Bilinear = 2;
		static const Int Area = 3;

		static Bool isValid(Int val);
		static String toString(Int val);
		static Int defaultValue();
	};

	/// \~English @brief Image resizing multiplier
	/// \~Chinese @brief 图像大小的调整倍数
	class SPADAS_API ResizeScale
	{
	public:
		static const Int Quarter = 1;
		static const Int Half = 2;
		static const Int Double = 3;

		static Bool isValid(Int val);
		static String toString(Int val);
		static Int defaultValue();
	};

	/// \~English @brief The type of image rotation
	/// \~Chinese @brief 图像旋转的类型
	class SPADAS_API ImageRotation
	{
	public:
		static const Int CW90 = 1;
		static const Int CW180 = 2;
		static const Int CCW90 = 3;

		static Bool isValid(Int val);
		static String toString(Int val);
		static Int defaultValue();
	};

	/// \~English @brief The value of each channel of the pixel
	/// \~Chinese @brief 像素各通道值
	template <typename Type> struct PixelChannels
	{
		Type ch1;
		Type ch2;
		Type ch3;
		Type ch4;

		PixelChannels() : ch1(0), ch2(0), ch3(0), ch4(0) {}
		PixelChannels(Type ch1) : ch1(ch1), ch2(0), ch3(0), ch4(0) {}
		PixelChannels(Type ch1, Type ch2) : ch1(ch1), ch2(ch2), ch3(0), ch4(0) {}
		PixelChannels(Type ch1, Type ch2, Type ch3) : ch1(ch1), ch2(ch2), ch3(ch3), ch4(0) {}
		PixelChannels(Type ch1, Type ch2, Type ch3, Type ch4) : ch1(ch1), ch2(ch2), ch3(ch3), ch4(ch4) {}
	};

	/// \~English @brief Byte type pixel channel value
	/// \~Chinese @brief Byte型的像素各通道值
	typedef PixelChannels<Byte> BytePixel;

	/// \~English @brief Word type pixel channel value
	/// \~Chinese @brief Word型的像素各通道值
	typedef PixelChannels<Word> WordPixel;

	/// \~English @brief Float type pixel channel value
	/// \~Chinese @brief Float型的像素各通道值
	typedef PixelChannels<Float> FloatPixel;

	/// \~English @brief Pixel data, convenient for image pixel operation
	/// \~Chinese @brief 像素数据，方便进行图像像素操作
	struct PixelData
	{
		/// \~English @brief [Unsafe pointer] Pointer union, can be used according to the type
		/// \~Chinese @brief [非安全指针] 指针联合，可根据类型使用
		union { Pointer ptr; Byte *b; Word *w; Float *f; };

		/// \~English @brief Bytes per pixel
		/// \~Chinese @brief 每像素字节数
		UInt bytesPerPixel;

		/// \~English @brief Invalid pointer
		/// \~Chinese @brief 无效指针
		SPADAS_API PixelData();

		/// \~English @brief Normal creation
		/// \~Chinese @brief 正常创建
		SPADAS_API PixelData(Pointer ptr, UInt bytesPerPixel);

		/// \~English @brief Is it valid
		/// \~Chinese @brief 是否有效
		SPADAS_API Bool isValid();

		/// \~English @brief Get the pixel data on the specified index
		/// \~Chinese @brief 获取指定序号上的像素数据
		SPADAS_API PixelData operator [](UInt index);

		/// \~English @brief Get the value of each channel of Byte type
		/// \~Chinese @brief 获取Byte型的各通道值
		SPADAS_API BytePixel getBytes();

		/// \~English @brief Get the value of each channel of Word type
		/// \~Chinese @brief 获取Word型的各通道值
		SPADAS_API WordPixel getWords();

		/// \~English @brief Get the value of each channel of Float type
		/// \~Chinese @brief 获取Float型的各通道值
		SPADAS_API FloatPixel getFloats();

		/// \~English @brief Set each channel value of Byte type
		/// \~Chinese @brief 设置Byte型的各通道值
		SPADAS_API void set(BytePixel bytes);

		/// \~English @brief Set each channel value of Word type
		/// \~Chinese @brief 设置Word型的各通道值
		SPADAS_API void set(WordPixel words);

		/// \~English @brief Set each channel value of Float type
		/// \~Chinese @brief 设置Float型的各通道值
		SPADAS_API void set(FloatPixel floats);
	};

	// Image pair (Forward declaration) / 图像对（前置声明）
	struct ImagePair;

	/// \~English @brief Image class
	/// \~Chinese @brief 图像类
	class SPADAS_API Image : public Object<class ImageVars>
	{
	public:
		/// \~English @brief Class name
		/// \~Chinese @brief 类名称
		static const String TypeName;

		/// \~English @brief Invalid object
		/// \~Chinese @brief 无效对象
		Image();

		/// \~English @brief Specify the size and format to create an image, if clearPixels is TRUE, clear all pixel values to 0
		/// \~Chinese @brief 指定大小和格式创建图像，若clearPixels为TRUE则清空所有像素值为0
		Image(Size2D size, Enum<PixelFormat> format, Bool clearPixels = TRUE);

		/// \~English @brief Specify the size and format to create an image, if clearPixels is TRUE, clear all pixel values to 0
		/// \~Chinese @brief 指定大小和格式创建图像，若clearPixels为TRUE则清空所有像素值为0
		Image(Enum<ImageResolution> resolution, Enum<PixelFormat> format, Bool clearPixels = TRUE);

		/// \~English @brief Specify the size and format to create an image, and copy data from the data block pointed to by the pointer
		/// \~Chinese @brief 指定大小和格式创建图像，并从指针指向的数据块拷贝数据
		Image(Size2D size, Enum<PixelFormat> format, Pointer raw, UInt rawRowBytes);

		/// \~English @brief Specify the size and format to create an image, and copy data from the data block pointed to by the pointer
		/// \~Chinese @brief 指定大小和格式创建图像，并从指针指向的数据块拷贝数据
		Image(Enum<ImageResolution> resolution, Enum<PixelFormat> format, Pointer raw, UInt rawRowBytes);

		/// \~English @brief Copy and create an image from a subregion of another image
		/// \~Chinese @brief 从另一幅图像的子区域拷贝并创建图像
		Image(Image src, Region2D srcRegion);

		/// \~English @brief Create image from image pointer (without copying data)
		/// \~Chinese @brief 从图像指针创建图像（不拷贝数据）
		Image(ImagePointer pointer);

		/// \~English @brief Read the image from the .bmp file, the format may be ByteBGR, ByteBGRA or ByteGray depending on the content of the file
		/// \~Chinese @brief 从.bmp文件读取图像，根据文件内容不同其格式可能为ByteBGR，ByteBGRA或ByteGray
		Image(Path bmpFilePath);

		/// \~English @brief Create an image from IplImage
		/// \~Chinese @brief 从IplImage创建图像
		Image(IplImage *iplimage);

		/// \~English @brief Clone a new object
		/// \~Chinese @brief 克隆出一个新对象
		Image clone();

		/// \~English @brief Copy data from a sub-area of another image with the same pixel format to a certain position of this image (specified by thisOffset)
		/// \~Chinese @brief 从另一幅具有相同像素格式的图像的某个子区域拷贝数据到本图像的某个位置（通过thisOffset指定）
		void copy(Image src, Region2D srcRegion, CoordInt2D thisOffset);

		/// \~English @brief Draw the layer to a certain position of this image (the layer image must have an Alpha channel)
		/// \~Chinese @brief 绘制图层至本图像的某个位置 (layer图像必须具有Alpha通道)
		void drawLayer(Image layer, CoordInt2D dstOffset);

		/// \~English @brief [Modifiable] Get the sub-image, which will be bound to the data of this image
		/// \~Chinese @brief [可修改] 取得子图像，其将绑定至本图像的数据
		Image subImage(Region2D region);

		/// \~English @brief [Modifiable] Get the image pointer, only supported by ByteGray and ByteBGR
		/// \~Chinese @brief [可修改] 取得图像指针，仅ByteGray和ByteBGR支持
		Optional<ImagePointer> imagePointer();

		/// \~English @brief [Unsafe pointer][Modifiable] Use IplImage to get the image data, which will be bound to the data of this image
		/// \~Chinese @brief [非安全指针][可修改] 以IplImage取得图像数据，其将绑定至本图像的数据
		IplImage *iplImage();

		/// \~English @brief Get image size
		/// \~Chinese @brief 取得图像大小
		Size2D size();

		/// \~English @brief Get image width
		/// \~Chinese @brief 取得图像宽度
		UInt width();

		/// \~English @brief Get image height
		/// \~Chinese @brief 取得图像高度
		UInt height();

		/// \~English @brief Get image's pixel format
		/// \~Chinese @brief 取得图像像素格式
		Enum<PixelFormat> format();

		/// \~English @brief Obtain the number of bytes/step size of a line of the image
		/// \~Chinese @brief 取得图像一行的字节数/步长
		UInt rowBytes();

		/// \~English @brief Get the effective number of bytes in a row (width x bytes per pixel)
		/// \~Chinese @brief 取得一行中有效的字节数（宽度x每像素字节数）
		UInt rowBytesValid();

		/// \~English @brief [Modifiable] Get the first pixel data of the image
		/// \~Chinese @brief [可修改] 取得图像第一个像素数据
		PixelData data();

		/// \~English @brief [Modifiable] Get the first pixel data of a row, v=0 is the first row
		/// \~Chinese @brief [可修改] 取得某一行第一个像素数据，v=0为第一行
		PixelData operator [](UInt v);

		/// \~English @brief Resize the image by the specified factor (faster)
		/// \~Chinese @brief 按照指定倍数调整图像大小 (较快)
		Image resize(Enum<ResizeScale> scale);

		/// \~English @brief Resize the image to any size (slower)
		/// \~Chinese @brief 调整图像大小至任意尺寸 (较慢)
		Image resize(Size2D outputSize, Bool undistort, Enum<ResizeMode> mode = ResizeMode::Bilinear);

		/// \~English @brief Resize the image to any size (slower)
		/// \~Chinese @brief 调整图像大小至任意尺寸 (较慢)
		Image resize(Enum<ImageResolution> outputResolution, Bool undistort, Enum<ResizeMode> mode = ResizeMode::Bilinear);

		/// \~English @brief Convert pixel format (when converting to PixelFormat::ByteBool, 128~255 -> 255, 0~127 -> 0
		/// \~Chinese @brief 转换像素格式 (转换为 PixelFormat::ByteBool 时, 128~255 -> 255, 0~127 -> 0
		Image convert(Enum<PixelFormat> outputFormat);

		/// \~English @brief Flip the image
		/// \~Chinese @brief 反转图像
		Image flip(Bool upDownFlip, Bool leftRightFlip);

		/// \~English @brief Rotate image (supports all pixel formats except ByteUYVY)
		/// \~Chinese @brief 旋转图像 (支持除ByteUYVY以外的所有像素格式)
		Image rotate(Enum<ImageRotation> rotation);

		/// \~English @brief Image field segmentation (image1: rows 024..., image2: rows 135...)
		/// \~Chinese @brief 图像场分割 (image1: 024...行, image2: 135...行)
		ImagePair splitFields();

		/// \~English @brief Merge two fields into one interlaced image (image1: rows 024..., image2: rows 135...)
		/// \~Chinese @brief 合并两场为一幅图像 (image1: 024...行, image2: 135...行)
		static Image mergeFields(ImagePair fields);

		/// \~English @brief Split the 3D image into image pairs (image1: left camera image, image2: right camera image)
		/// \~Chinese @brief 分割3D图像至图像对 (image1: 左机图像, image2: 右机图像)
		ImagePair splitStereo(Enum<MergeMode> srcMergeMode);

		/// \~English @brief Merge into a 3D image (image1: left camera image, image2: right camera image)
		/// \~Chinese @brief 合并为3D图像 (image1: 左机图像, image2: 右机图像)
		static Image mergeStereo(ImagePair src, Enum<MergeMode> dstMergeMode);

		/*
			For splitChannels and merChannels, pixel format will be converted as following:
			对于splitChannels和mergeChannels方法，像素格式将按以下方式转化：
			Multi-channel	Single-channel
			多通道图像        单通道图像
			----------------------------------
			ByteBGR			ByteGray x 3
			ByteRGB			ByteGray x 3
			ByteYUV			ByteGray x 3
			ByteBGRA		ByteGray x 4
			ByteRGBA		ByteGray x 4
			WordBGR			WordGray x 3
			FloatBGR		FloatGray x 3
			FloatHSV		FloatGray x 3 (Hue will be normalized to 0.0~1.0 / 色调数值将归一化至0.0~1.0范围)
		*/

		/// \~English @brief Split multi-channel images into several single-channel images
		/// \~Chinese @brief 分离多通道图像至若干幅单通道图像
		Array<Image> splitChannels();

		/// \~English @brief Merge into a multi-channel image according to the specified pixel format
		/// \~Chinese @brief 按指定像素格式合并为多通道图像
		static Image mergeChannels(Array<Image> channels, Enum<PixelFormat> multiChannelFormat);

		/// \~English @brief Generate mipmap (level 0 is the source image, the higher the level, the smaller the image)
		/// \~Chinese @brief 生成mipmap (级别0为源图像，级别越高图像越小)
		Array<Image> genMipmap(UInt nLevels);

		/// \~English @brief Save to .bmp file
		/// \~Chinese @brief 保存至.bmp文件
		void save(Path bmpFilePath);

		/// \~English @brief Use text to describe information such as image size, pixel format, etc.
		/// \~Chinese @brief 用文本表述图像大小、像素格式等信息
		String getInfoText();
	};

	/// \~English @brief Image pair
	/// \~Chinese @brief 图像对
	struct ImagePair
	{
		Image image1;
		Image image2;

		SPADAS_API ImagePair();
		SPADAS_API ImagePair(Image image1, Image image2);
	};

	// Math / 数学 //////////////////////////////////////////////////////////////

	/// \~English @brief Large integer number
	/// \~Chinese @brief 大数
	class SPADAS_API BigInteger : public Object<class BigIntegerVars>
	{
	public:
		/// \~English @brief Class name
		/// \~Chinese @brief 类名称
		static const String TypeName;

		/// \~English @brief Zero
		/// \~Chinese @brief 零
		BigInteger();

		/// \~English @brief Generate by unsigned binary data (0 if empty) and sign (valid when not 0)
		/// \~Chinese @brief 由无符号二进制数据（若空则为0）以及符号（非0时有效）生成
		BigInteger(Binary integerData, Bool sign = TRUE);

		/// \~English @brief Generate from a decimal number string (0 if empty)
		/// \~Chinese @brief 由10进制数字字符串生成（若空则为0）
		static Optional<BigInteger> createFromString(String decimalString);

		/// \~English @brief Is it positive (0 returns TRUE)
		/// \~Chinese @brief 是否为正（0则返回TRUE）
		Bool isPositive();

		/// \~English @brief Get unsigned binary data
		/// \~Chinese @brief 取得无符号二进制数据
		Binary getData();

		/// \~English @brief Convert to a decimal number string
		/// \~Chinese @brief 转换为10进制数字字符串
		String toString();

		/// \~English @brief Get the hash value
		/// \~Chinese @brief 获取哈希值
		Word getHash();

		/// \~English @brief Whether it is equal to
		/// \~Chinese @brief 是否相等
		Bool operator ==(BigInteger num);

		/// \~English @brief Whether it is not equal to
		/// \~Chinese @brief 是否不等
		Bool operator !=(BigInteger num);

		/// \~English @brief Is less than
		/// \~Chinese @brief 是否小于
		Bool operator <(BigInteger num);

		/// \~English @brief Is greater than
		/// \~Chinese @brief 是否大于
		Bool operator >(BigInteger num);

		/// \~English @brief Add
		/// \~Chinese @brief 加
		BigInteger operator +(BigInteger num);

		/// \~English @brief Minus
		/// \~Chinese @brief 减
		BigInteger operator -(BigInteger num);

		/// \~English @brief Multiple
		/// \~Chinese @brief 乘
		BigInteger operator *(BigInteger num);

		/// \~English @brief Divide
		/// \~Chinese @brief 除
		BigInteger operator /(BigInteger num);

		/// \~English @brief Modulus
		/// \~Chinese @brief 模
		BigInteger operator %(BigInteger num);

		/// \~English @brief Modulus after exponentiation: return = (this ^ exponent) % modulus
		/// \~Chinese @brief 乘幂后模运算：return = (this ^ exponent) % modulus
		BigInteger modPow(BigInteger exponent, BigInteger modulus);

	private:
		Bool isNull() { return FALSE; }
		Bool isValid() { return FALSE; }
	};

	/// \~English @brief Int type matrix
	/// \~Chinese @brief Int型矩阵
	class SPADAS_API IntMat : public Object<class IntMatVars>
	{
	public:
		/// \~English @brief Invalid object
		/// \~Chinese @brief 无效对象
		IntMat();

		/// \~English @brief Create a 2D matrix
		/// \~Chinese @brief 创建一个2维矩阵
		IntMat(UInt dim0, UInt dim1);

		/// \~English @brief Create a 3D matrix
		/// \~Chinese @brief 创建一个3维矩阵
		IntMat(UInt dim0, UInt dim1, UInt dim2);

		/// \~English @brief Create an N-dimensional matrix (N must be at least 1)
		/// \~Chinese @brief 创建一个N维矩阵 (N至少为1)
		IntMat(Array<UInt> dims);

		/// \~English @brief Create an N-dimensional matrix and copy it from the specified data source
		/// \~Chinese @brief 创建一个N维矩阵并从指定数据源拷贝
		IntMat(Array<UInt> dims, Pointer raw);

		/// \~English @brief Create a matrix from an array (the size of the matrix is N x 1 x 1..., the dimension is specified by the parameter)
		/// \~Chinese @brief 由数组创建矩阵 (矩阵大小为N x 1 x 1...，维数由参数指定)
		IntMat(Array<Int> arr, UInt nDims = 2);

		/// \~English @brief Copy and create a matrix from a sub-area of another matrix, src should be a 2D matrix
		/// \~Chinese @brief 从另一个矩阵的子区域拷贝并创建矩阵，src应为2维矩阵
		IntMat(IntMat src, Region2D srcRegion);

		/// \~English @brief Copy and create a matrix from a sub-area of another matrix, src should be a 3D matrix
		/// \~Chinese @brief 从另一个矩阵的子区域拷贝并创建矩阵，src应为3维矩阵
		IntMat(IntMat src, Region3D srcRegion);

		/// \~English @brief Read matrix data from file (created by Matrix::save method)
		/// \~Chinese @brief 从文件读取矩阵数据并创建（由Matrix::save方法创建的文件）
		IntMat(Path filePath);

		/// \~English @brief Create a matrix from CvMat and copy the data
		/// \~Chinese @brief 由CvMat创建矩阵，并拷贝数据
		IntMat(CvMat *cvmat);

		/// \~English @brief Clone a new object
		/// \~Chinese @brief 克隆出一个新对象
		IntMat clone();

		/// \~English @brief Matrix data type conversion
		/// \~Chinese @brief 矩阵数据类型转换
		FloatMat convertToFloatMat();

		/// \~English @brief Matrix data type conversion
		/// \~Chinese @brief 矩阵数据类型转换
		DoubleMat convertToDoubleMat();

		/// \~English @brief Copy data from a sub-area of another matrix to a certain position of this matrix (specified by thisOffset), both the source matrix and this matrix should be 2D matrices
		/// \~Chinese @brief 从另一个矩阵的某个子区域拷贝数据到本矩阵的某个位置（通过thisOffset指定），源矩阵和本矩阵都应为2维矩阵
		void copy(IntMat src, Region2D srcRegion, CoordInt2D thisOffset);

		/// \~English @brief Copy data from a sub-area of another matrix to a certain position of this matrix (specified by thisOffset), both the source matrix and this matrix should be 3D matrices
		/// \~Chinese @brief 从另一个矩阵的某个子区域拷贝数据到本矩阵的某个位置（通过thisOffset指定），源矩阵和本矩阵都应为3维矩阵
		void copy(IntMat src, Region3D srcRegion, CoordInt3D thisOffset);

		/// \~English @brief Reset the value of all elements
		/// \~Chinese @brief 重置所有元素的值
		void clear(Int value);

		/// \~English @brief Get the size of the matrix (the length of each dimension)
		/// \~Chinese @brief 获得矩阵的大小（各维的长度）
		Array<UInt> size();

		/// \~English @brief Get the length of a certain dimension of the matrix (return 0 if the dimension is invalid), index starts from 0
		/// \~Chinese @brief 获得矩阵某个维度的长度 (维度无效时返回0)，序号从0开始
		UInt dimAt(UInt index);

		/// \~English @brief Get matrix dimension
		/// \~Chinese @brief 获得矩阵维数
		UInt nDims();

		/// \~English @brief Get the total number of elements in the matrix
		/// \~Chinese @brief 获得矩阵的总元素个数
		UInt nElems();

		/// \~English @brief Whether the matrix is with the specified size
		/// \~Chinese @brief 判断矩阵是否为指定大小
		Bool isSize(Size2D size);

		/// \~English @brief Whether the matrix is with the specified size
		/// \~Chinese @brief 判断矩阵是否为指定大小
		Bool isSize(Size3D size);

		/// \~English @brief Whether the matrix is with the specified size
		/// \~Chinese @brief 判断矩阵是否为指定大小
		Bool isSize(Array<UInt> dims);

		/// \~English @brief [Unsafe pointer][Modifiable] Get the data pointer
		/// \~Chinese @brief [非安全指针][可修改] 取得数据指针
		Int *data();

		/// \~English @brief [Unsafe pointer][Modifiable] Packed in the form of CvMat, the data will be bound to this matrix
		/// \~Chinese @brief [非安全指针][可修改] 包装成CvMat的形式，数据将绑定至本矩阵
		CvMat *cvMat();

		/// \~English @brief [Modifiable] Obtain the i-th sub-matrix, the dimension of the sub-matrix is reduced by 1, the data is bound to this matrix (index starts from 0)
		/// \~Chinese @brief [可修改] 取得第i个子矩阵，该子矩阵维数减1，数据绑定至本矩阵，序号从0开始
		IntMat operator [](UInt i);

		/// \~English @brief [Modifiable] Obtain a reference to an element of a 1D matrix (scalar) through parentheses
		/// \~Chinese @brief [可修改] 通过圆括号获得某个元素的引用，面向1维矩阵 (标量)
		Int& operator ()(UInt i);

		/// \~English @brief [Modifiable] Obtain a reference to an element of a 2D matrix (scalar) through parentheses
		/// \~Chinese @brief [可修改] 通过圆括号获得某个元素的引用，面向2维矩阵 (标量)
		Int& operator ()(UInt i, UInt j);

		/// \~English @brief [Modifiable] Obtain a reference to an element of a 3D matrix (scalar) through parentheses
		/// \~Chinese @brief [可修改] 通过圆括号获得某个元素的引用，面向3维矩阵 (标量)
		Int& operator ()(UInt i, UInt j, UInt k);

		/// \~English @brief Matrix addition: output = this + matrix
		/// \~Chinese @brief 矩阵加法: output = this + matrix
		IntMat operator +(IntMat matrix);

		/// \~English @brief Matrix subtraction: output = this - matrix
		/// \~Chinese @brief 矩阵减法: output = this - matrix
		IntMat operator -(IntMat matrix);

		/// \~English @brief Matrix multiplied by scalar: output = this * scale
		/// \~Chinese @brief 矩阵乘以标量: output = this * scale
		IntMat operator *(Int scale);

		/// \~English @brief Matrix multiplication: output = this * matrix (only supports 2D matrices)
		/// \~Chinese @brief 矩阵相乘: output = this * matrix (仅支持2维矩阵)
		IntMat operator *(IntMat matrix);

		/// \~English @brief Matrix transpose (only supports 2D matrix)
		/// \~Chinese @brief 矩阵转置 (仅支持2维矩阵)
		IntMat transpose();

		/// \~English @brief Convert matrix data to text string (only supports 2D matrix)
		/// \~Chinese @brief 用字符串表示矩阵数据 (仅支持2维矩阵)
		String toString(UInt nDigits = 1);

		/// \~English @brief Save matrix data to a text file (only supports 2D matrix)
		/// \~Chinese @brief 保存矩阵数据至文本文件 (仅支持2维矩阵)
		void save(Path filePath, UInt nDigits = 1);
	};

	/// \~English @brief Float type matrix
	/// \~Chinese @brief Float型矩阵
	class SPADAS_API FloatMat : public Object<class FloatMatVars>
	{
	public:
		/// \~English @brief Invalid object
		/// \~Chinese @brief 无效对象
		FloatMat();

		/// \~English @brief Create a 2D matrix
		/// \~Chinese @brief 创建一个2维矩阵
		FloatMat(UInt dim0, UInt dim1);

		/// \~English @brief Create a 3D matrix
		/// \~Chinese @brief 创建一个3维矩阵
		FloatMat(UInt dim0, UInt dim1, UInt dim2);

		/// \~English @brief Create an N-dimensional matrix (N must be at least 1)
		/// \~Chinese @brief 创建一个N维矩阵 (N至少为1)
		FloatMat(Array<UInt> dims);

		/// \~English @brief Create an N-dimensional matrix and copy it from the specified data source
		/// \~Chinese @brief 创建一个N维矩阵并从指定数据源拷贝
		FloatMat(Array<UInt> dims, Pointer raw);

		/// \~English @brief Create a matrix from an array (the size of the matrix is N x 1 x 1..., the dimension is specified by the parameter)
		/// \~Chinese @brief 由数组创建矩阵 (矩阵大小为N x 1 x 1...，维数由参数指定)
		FloatMat(Array<Float> arr, UInt nDims = 2);

		/// \~English @brief Copy and create a matrix from a sub-area of another matrix, src should be a 2D matrix
		/// \~Chinese @brief 从另一个矩阵的子区域拷贝并创建矩阵，src应为2维矩阵
		FloatMat(FloatMat src, Region2D srcRegion);

		/// \~English @brief Copy and create a matrix from a sub-area of another matrix, src should be a 3D matrix
		/// \~Chinese @brief 从另一个矩阵的子区域拷贝并创建矩阵，src应为3维矩阵
		FloatMat(FloatMat src, Region3D srcRegion);

		/// \~English @brief Read matrix data from file (created by Matrix::save method)
		/// \~Chinese @brief 从文件读取矩阵数据并创建（由Matrix::save方法创建的文件）
		FloatMat(Path filePath);

		/// \~English @brief Create a matrix from CvMat and copy the data
		/// \~Chinese @brief 由CvMat创建矩阵，并拷贝数据
		FloatMat(CvMat *cvmat);

		/// \~English @brief Clone a new object
		/// \~Chinese @brief 克隆出一个新对象
		FloatMat clone();

		/// \~English @brief Matrix data type conversion
		/// \~Chinese @brief 矩阵数据类型转换
		IntMat convertToIntMat();

		/// \~English @brief Matrix data type conversion
		/// \~Chinese @brief 矩阵数据类型转换
		DoubleMat convertToDoubleMat();

		/// \~English @brief Copy data from a sub-area of another matrix to a certain position of this matrix (specified by thisOffset), both the source matrix and this matrix should be 2D matrices
		/// \~Chinese @brief 从另一个矩阵的某个子区域拷贝数据到本矩阵的某个位置（通过thisOffset指定），源矩阵和本矩阵都应为2维矩阵
		void copy(FloatMat src, Region2D srcRegion, CoordInt2D thisOffset);

		/// \~English @brief Copy data from a sub-area of another matrix to a certain position of this matrix (specified by thisOffset), both the source matrix and this matrix should be 3D matrices
		/// \~Chinese @brief 从另一个矩阵的某个子区域拷贝数据到本矩阵的某个位置（通过thisOffset指定），源矩阵和本矩阵都应为3维矩阵
		void copy(FloatMat src, Region3D srcRegion, CoordInt3D thisOffset);

		/// \~English @brief Reset the value of all elements
		/// \~Chinese @brief 重置所有元素的值
		void clear(Float value);

		/// \~English @brief Get the size of the matrix (the length of each dimension)
		/// \~Chinese @brief 获得矩阵的大小（各维的长度）
		Array<UInt> size();

		/// \~English @brief Get the length of a certain dimension of the matrix (return 0 if the dimension is invalid), index starts from 0
		/// \~Chinese @brief 获得矩阵某个维度的长度 (维度无效时返回0)，序号从0开始
		UInt dimAt(UInt index);

		/// \~English @brief Get matrix dimension
		/// \~Chinese @brief 获得矩阵维数
		UInt nDims();

		/// \~English @brief Get the total number of elements in the matrix
		/// \~Chinese @brief 获得矩阵的总元素个数
		UInt nElems();

		/// \~English @brief Whether the matrix is with the specified size
		/// \~Chinese @brief 判断矩阵是否为指定大小
		Bool isSize(Size2D size);

		/// \~English @brief Whether the matrix is with the specified size
		/// \~Chinese @brief 判断矩阵是否为指定大小
		Bool isSize(Size3D size);

		/// \~English @brief Whether the matrix is with the specified size
		/// \~Chinese @brief 判断矩阵是否为指定大小
		Bool isSize(Array<UInt> dims);

		/// \~English @brief [Unsafe pointer][Modifiable] Get the data pointer
		/// \~Chinese @brief [非安全指针][可修改] 取得数据指针
		Float *data();

		/// \~English @brief [Unsafe pointer][Modifiable] Packed in the form of CvMat, the data will be bound to this matrix
		/// \~Chinese @brief [非安全指针][可修改] 包装成CvMat的形式，数据将绑定至本矩阵
		CvMat *cvMat();

		/// \~English @brief [Modifiable] Obtain the i-th sub-matrix, the dimension of the sub-matrix is reduced by 1, the data is bound to this matrix (index starts from 0)
		/// \~Chinese @brief [可修改] 取得第i个子矩阵，该子矩阵维数减1，数据绑定至本矩阵，序号从0开始
		FloatMat operator [](UInt i);

		/// \~English @brief [Modifiable] Obtain a reference to an element of a 1D matrix (scalar) through parentheses
		/// \~Chinese @brief [可修改] 通过圆括号获得某个元素的引用，面向1维矩阵 (标量)
		Float& operator ()(UInt i);

		/// \~English @brief [Modifiable] Obtain a reference to an element of a 2D matrix (scalar) through parentheses
		/// \~Chinese @brief [可修改] 通过圆括号获得某个元素的引用，面向2维矩阵 (标量)
		Float& operator ()(UInt i, UInt j);

		/// \~English @brief [Modifiable] Obtain a reference to an element of a 3D matrix (scalar) through parentheses
		/// \~Chinese @brief [可修改] 通过圆括号获得某个元素的引用，面向3维矩阵 (标量)
		Float& operator ()(UInt i, UInt j, UInt k);

		/// \~English @brief Matrix addition: output = this + matrix
		/// \~Chinese @brief 矩阵加法: output = this + matrix
		FloatMat operator +(FloatMat matrix);

		/// \~English @brief Matrix subtraction: output = this - matrix
		/// \~Chinese @brief 矩阵减法: output = this - matrix
		FloatMat operator -(FloatMat matrix);

		/// \~English @brief Matrix multiplied by scalar: output = this * scale
		/// \~Chinese @brief 矩阵乘以标量: output = this * scale
		FloatMat operator *(Float scale);

		/// \~English @brief Matrix multiplication: output = this * matrix (only supports 2D matrices)
		/// \~Chinese @brief 矩阵相乘: output = this * matrix (仅支持2维矩阵)
		FloatMat operator *(FloatMat matrix);

		/// \~English @brief Matrix transpose (only supports 2D matrix)
		/// \~Chinese @brief 矩阵转置 (仅支持2维矩阵)
		FloatMat transpose();

		/// \~English @brief Convert matrix data to text string (only supports 2D matrix)
		/// \~Chinese @brief 用字符串表示矩阵数据 (仅支持2维矩阵)
		String toString(UInt nDigits = 1);

		/// \~English @brief Save matrix data to a text file (only supports 2D matrix)
		/// \~Chinese @brief 保存矩阵数据至文本文件 (仅支持2维矩阵)
		void save(Path filePath, UInt nDigits = 1);
	};

	/// \~English @brief Double type matrix
	/// \~Chinese @brief 支持的矩阵类型：Double型矩阵
	class SPADAS_API DoubleMat : public Object<class DoubleMatVars>
	{
	public:
		/// \~English @brief Invalid object
		/// \~Chinese @brief 无效对象
		DoubleMat();

		/// \~English @brief Create a 2D matrix
		/// \~Chinese @brief 创建一个2维矩阵
		DoubleMat(UInt dim0, UInt dim1);

		/// \~English @brief Create a 3D matrix
		/// \~Chinese @brief 创建一个3维矩阵
		DoubleMat(UInt dim0, UInt dim1, UInt dim2);

		/// \~English @brief Create an N-dimensional matrix (N must be at least 1)
		/// \~Chinese @brief 创建一个N维矩阵 (N至少为1)
		DoubleMat(Array<UInt> dims);

		/// \~English @brief Create an N-dimensional matrix and copy it from the specified data source
		/// \~Chinese @brief 创建一个N维矩阵并从指定数据源拷贝
		DoubleMat(Array<UInt> dims, Pointer raw);

		/// \~English @brief Create a matrix from an array (the size of the matrix is N x 1 x 1..., the dimension is specified by the parameter)
		/// \~Chinese @brief 由数组创建矩阵 (矩阵大小为N x 1 x 1...，维数由参数指定)
		DoubleMat(Array<Double> arr, UInt nDims = 2);

		/// \~English @brief Copy and create a matrix from a sub-area of another matrix, src should be a 2D matrix
		/// \~Chinese @brief 从另一个矩阵的子区域拷贝并创建矩阵，src应为2维矩阵
		DoubleMat(DoubleMat src, Region2D srcRegion);

		/// \~English @brief Copy and create a matrix from a sub-area of another matrix, src should be a 3D matrix
		/// \~Chinese @brief 从另一个矩阵的子区域拷贝并创建矩阵，src应为3维矩阵
		DoubleMat(DoubleMat src, Region3D srcRegion);

		/// \~English @brief Read matrix data from file (created by Matrix::save method)
		/// \~Chinese @brief 从文件读取矩阵数据并创建（由Matrix::save方法创建的文件）
		DoubleMat(Path filePath);

		/// \~English @brief Create a matrix from CvMat and copy the data
		/// \~Chinese @brief 由CvMat创建矩阵，并拷贝数据
		DoubleMat(CvMat *cvmat);

		/// \~English @brief Clone a new object
		/// \~Chinese @brief 克隆出一个新对象
		DoubleMat clone();

		/// \~English @brief Matrix data type conversion
		/// \~Chinese @brief 矩阵数据类型转换
		IntMat convertToIntMat();

		/// \~English @brief Matrix data type conversion
		/// \~Chinese @brief 矩阵数据类型转换
		FloatMat convertToFloatMat();

		/// \~English @brief Copy data from a sub-area of another matrix to a certain position of this matrix (specified by thisOffset), both the source matrix and this matrix should be 2D matrices
		/// \~Chinese @brief 从另一个矩阵的某个子区域拷贝数据到本矩阵的某个位置（通过thisOffset指定），源矩阵和本矩阵都应为2维矩阵
		void copy(DoubleMat src, Region2D srcRegion, CoordInt2D thisOffset);

		/// \~English @brief Copy data from a sub-area of another matrix to a certain position of this matrix (specified by thisOffset), both the source matrix and this matrix should be 3D matrices
		/// \~Chinese @brief 从另一个矩阵的某个子区域拷贝数据到本矩阵的某个位置（通过thisOffset指定），源矩阵和本矩阵都应为3维矩阵
		void copy(DoubleMat src, Region3D srcRegion, CoordInt3D thisOffset);

		/// \~English @brief Reset the value of all elements
		/// \~Chinese @brief 重置所有元素的值
		void clear(Double value);

		/// \~English @brief Get the size of the matrix (the length of each dimension)
		/// \~Chinese @brief 获得矩阵的大小（各维的长度）
		Array<UInt> size();

		/// \~English @brief Get the length of a certain dimension of the matrix (return 0 if the dimension is invalid), index starts from 0
		/// \~Chinese @brief 获得矩阵某个维度的长度 (维度无效时返回0)，序号从0开始
		UInt dimAt(UInt index);

		/// \~English @brief Get matrix dimension
		/// \~Chinese @brief 获得矩阵维数
		UInt nDims();

		/// \~English @brief Get the total number of elements in the matrix
		/// \~Chinese @brief 获得矩阵的总元素个数
		UInt nElems();

		/// \~English @brief Whether the matrix is with the specified size
		/// \~Chinese @brief 判断矩阵是否为指定大小
		Bool isSize(Size2D size);

		/// \~English @brief Whether the matrix is with the specified size
		/// \~Chinese @brief 判断矩阵是否为指定大小
		Bool isSize(Size3D size);

		/// \~English @brief Whether the matrix is with the specified size
		/// \~Chinese @brief 判断矩阵是否为指定大小
		Bool isSize(Array<UInt> dims);

		/// \~English @brief [Unsafe pointer][Modifiable] Get the data pointer
		/// \~Chinese @brief [非安全指针][可修改] 取得数据指针
		Double *data();

		/// \~English @brief [Unsafe pointer][Modifiable] Packed in the form of CvMat, the data will be bound to this matrix
		/// \~Chinese @brief [非安全指针][可修改] 包装成CvMat的形式，数据将绑定至本矩阵
		CvMat *cvMat();

		/// \~English @brief [Modifiable] Obtain the i-th sub-matrix, the dimension of the sub-matrix is reduced by 1, the data is bound to this matrix (index starts from 0)
		/// \~Chinese @brief [可修改] 取得第i个子矩阵，该子矩阵维数减1，数据绑定至本矩阵，序号从0开始
		DoubleMat operator [](UInt i);

		/// \~English @brief [Modifiable] Obtain a reference to an element of a 1D matrix (scalar) through parentheses
		/// \~Chinese @brief [可修改] 通过圆括号获得某个元素的引用，面向1维矩阵 (标量)
		Double& operator ()(UInt i);

		/// \~English @brief [Modifiable] Obtain a reference to an element of a 2D matrix (scalar) through parentheses
		/// \~Chinese @brief [可修改] 通过圆括号获得某个元素的引用，面向2维矩阵 (标量)
		Double& operator ()(UInt i, UInt j);

		/// \~English @brief [Modifiable] Obtain a reference to an element of a 3D matrix (scalar) through parentheses
		/// \~Chinese @brief [可修改] 通过圆括号获得某个元素的引用，面向3维矩阵 (标量)
		Double& operator ()(UInt i, UInt j, UInt k);

		/// \~English @brief Matrix addition: output = this + matrix
		/// \~Chinese @brief 矩阵加法: output = this + matrix
		DoubleMat operator +(DoubleMat matrix);

		/// \~English @brief Matrix subtraction: output = this - matrix
		/// \~Chinese @brief 矩阵减法: output = this - matrix
		DoubleMat operator -(DoubleMat matrix);

		/// \~English @brief Matrix multiplied by scalar: output = this * scale
		/// \~Chinese @brief 矩阵乘以标量: output = this * scale
		DoubleMat operator *(Double scale);

		/// \~English @brief Matrix multiplication: output = this * matrix (only supports 2D matrices)
		/// \~Chinese @brief 矩阵相乘: output = this * matrix (仅支持2维矩阵)
		DoubleMat operator *(DoubleMat matrix);

		/// \~English @brief Matrix transpose (only supports 2D matrix)
		/// \~Chinese @brief 矩阵转置 (仅支持2维矩阵)
		DoubleMat transpose();

		/// \~English @brief Convert matrix data to text string (only supports 2D matrix)
		/// \~Chinese @brief 用字符串表示矩阵数据 (仅支持2维矩阵)
		String toString(UInt nDigits = 1);

		/// \~English @brief Save matrix data to a text file (only supports 2D matrix)
		/// \~Chinese @brief 保存矩阵数据至文本文件 (仅支持2维矩阵)
		void save(Path filePath, UInt nDigits = 1);
	};

	/// \~English @brief Mathematical function namespace
	/// \~Chinese @brief 数学函数命名空间
	namespace math
	{
		/// \~English @brief PI
		/// \~Chinese @brief 圆周率
		const Float PI = 3.141593f;

		/// \~English @brief Natural constant
		/// \~Chinese @brief 自然常数
		const Float E = 2.718282f;

		/// \~English @brief Convert angle value to radian value (multiply by the following constant to convert)
		/// \~Chinese @brief 角度数值转为弧度数值 (乘以以下常数进行转换)
		const Float DEG2RAD = 0.01745329f;

		/// \~English @brief Convert radian value to angle value (multiply by the following constants to convert)
		/// \~Chinese @brief 弧度数值转为角度数值 (乘以以下常数进行转换)
		const Float RAD2DEG = 57.29578f;

		/// \~English @brief Single-floating-point arithmetic precision
		/// \~Chinese @brief 单浮点运算精度
		const Float FEPS = 1.4e-45f;

		/// \~English @brief Double-floating-point arithmetic precision
		/// \~Chinese @brief 双浮点运算精度
		const Double DEPS = 4.94065645841247e-324;

		/// \~English @brief Get the minimum of two numbers
		/// \~Chinese @brief 获得两个数中的最小值
		template <typename Type>
		Type min(Type a, Type b);

		/// \~English @brief Get the minimum value in the array
		/// \~Chinese @brief 获得数组中的最小值
		template <typename Type>
		Type min(Array<Type> arr);

		/// \~English @brief Get the maximum of two numbers
		/// \~Chinese @brief 获得两个数中的最大值
		template <typename Type>
		Type max(Type a, Type b);

		/// \~English @brief Get the maximum value in the array
		/// \~Chinese @brief 获得数组中的最大值
		template <typename Type>
		Type max(Array<Type> arr);

		/// \~English @brief Ensure that the number is between lower and upper (if it is greater than upper, it will be upper, if it is less than lower, it will be lower)
		/// \~Chinese @brief 保证number在lower和upper之间（大于upper则为upper，小于lower则为lower）
		template <typename Type>
		Type clamp(Type number, Type lower, Type upper);

		/// \~English @brief Get the absolute value
		/// \~Chinese @brief 获取绝对值
		inline Int abs(Int number) { return number > 0 ? number : -number; }

		/// \~English @brief Get the absolute value
		/// \~Chinese @brief 获取绝对值
		inline Long abs(Long number) { return number > 0 ? number : -number; }

		/// \~English @brief Get the absolute value
		/// \~Chinese @brief 获取绝对值
		inline Float abs(Float number) { return number > 0 ? number : -number; }

		/// \~English @brief Get the absolute value
		/// \~Chinese @brief 获取绝对值
		inline Double abs(Double number) { return number > 0 ? number : -number; }

		/// \~English @brief Round up
		/// \~Chinese @brief 四舍五入
		inline Int round(Float number) { return (Int)(number + (number >= 0 ? 0.5f : -0.5f)); }

		/// \~English @brief Round up
		/// \~Chinese @brief 四舍五入
		inline Int round(Double number) { return (Int)(number + (number >= 0 ? 0.5 : -0.5)); }

		/// \~English @brief Get the largest integer not greater than the input value
		/// \~Chinese @brief 取得不大于输入值的最大整数
		inline Int floor(Float number) { Int i = (Int)number; return i - (i > number); }

		/// \~English @brief Get the largest integer not greater than the input value
		/// \~Chinese @brief 取得不大于输入值的最大整数
		inline Int floor(Double number) { Int i = (Int)number; return i - (i > number); }

		/// \~English @brief Get the smallest integer not less than the input value
		/// \~Chinese @brief 取得不小于输入值的最小整数
		inline Int ceil(Float number) { Int i = (Int)number; return i + (i < number); }

		/// \~English @brief Get the smallest integer not less than the input value
		/// \~Chinese @brief 取得不小于输入值的最小整数
		inline Int ceil(Double number) { Int i = (Int)number; return i + (i < number); }

		/// \~English @brief Calculate "a" to the power of "x" (return infinity if a < 0)
		/// \~Chinese @brief 计算a的x次幂 (返回无穷大若a < 0)
		SPADAS_API Float power(Float a, Float x);

		/// \~English @brief Calculate "a" to the power of "x" (return infinity if a < 0)
		/// \~Chinese @brief 计算a的x次幂 (返回无穷大若a < 0)
		SPADAS_API Double power(Double a, Double x);

		/// \~English @brief Square
		/// \~Chinese @brief 平方
		inline Float square(Float number) { return number * number; }

		/// \~English @brief Square
		/// \~Chinese @brief 平方
		inline Double square(Double number) { return number * number; }

		/// \~English @brief Square root (returns infinity if number < 0)
		/// \~Chinese @brief 平方根 (返回无穷大若number < 0)
		inline Float root(Float number) { return power(number, 0.5f); }

		/// \~English @brief Square root (returns infinity if number < 0)
		/// \~Chinese @brief 平方根 (返回无穷大若number < 0)
		inline Double root(Double number) { return power(number, 0.5); }

		/// \~English @brief Get the positive limit value of single-floating point
		/// \~Chinese @brief 获得单精度浮点的正极限值
		SPADAS_API Float finf();

		/// \~English @brief Get the negative limit value of single-floating point
		/// \~Chinese @brief 获得单精度浮点的负极限值
		SPADAS_API Float nfinf();

		/// \~English @brief Get the positive limit value of double-floating point
		/// \~Chinese @brief 获得双精度浮点的正极限值
		SPADAS_API Double dinf();

		/// \~English @brief Get the negative limit value of double-floating point
		/// \~Chinese @brief 获得双精度浮点的负极限值
		SPADAS_API Double ndinf();

		/// \~English @brief Calculate natural exponent
		/// \~Chinese @brief 计算自然指数
		SPADAS_API Float exp(Float x);

		/// \~English @brief Calculate natural exponent
		/// \~Chinese @brief 计算自然指数
		SPADAS_API Double exp(Double x);

		/// \~English @brief Calculate natural logarithm (return FINF or DINF if x <= 0)
		/// \~Chinese @brief 计算自然对数 (返回FINF或DINF若x <= 0)
		SPADAS_API Float ln(Float x);

		/// \~English @brief Calculate natural logarithm (return FINF or DINF if x <= 0)
		/// \~Chinese @brief 计算自然对数 (返回FINF或DINF若x <= 0)
		SPADAS_API Double ln(Double x);

		/// \~English @brief Trigonometric function: sine
		/// \~Chinese @brief 三角函数: sine
		SPADAS_API Float sin(Float degree);

		/// \~English @brief Trigonometric function: sine
		/// \~Chinese @brief 三角函数: sine
		SPADAS_API Double sin(Double degree);

		/// \~English @brief Trigonometric function: cosine
		/// \~Chinese @brief 三角函数: cosine
		SPADAS_API Float cos(Float degree);

		/// \~English @brief Trigonometric function: cosine
		/// \~Chinese @brief 三角函数: cosine
		SPADAS_API Double cos(Double degree);

		/// \~English @brief Trigonometric function: tangent
		/// \~Chinese @brief 三角函数: tangent
		SPADAS_API Float tan(Float degree);

		/// \~English @brief Trigonometric function: tangent
		/// \~Chinese @brief 三角函数: tangent
		SPADAS_API Double tan(Double degree);

		/// \~English @brief Trigonometric function: arc-sine(output -90~90)
		/// \~Chinese @brief 三角函数: arc-sine(输出-90~90)
		SPADAS_API Float asin(Float x);

		/// \~English @brief Trigonometric function: arc-sine(output -90~90)
		/// \~Chinese @brief 三角函数: arc-sine(输出-90~90)
		SPADAS_API Double asin(Double x);

		/// \~English @brief Trigonometric function: arc-cosine (output 0~180)
		/// \~Chinese @brief 三角函数: arc-cosine(输出0~180)
		SPADAS_API Float acos(Float x);

		/// \~English @brief Trigonometric function: arc-cosine (output 0~180)
		/// \~Chinese @brief 三角函数: arc-cosine(输出0~180)
		SPADAS_API Double acos(Double x);

		/// \~English @brief Trigonometric function: arc-tangent(x/y) (output FINF or DINF if the input parameter is invalid)
		/// \~Chinese @brief 三角函数: arc-tangent(x/y) (输出FINF或DINF若输入参数无效)
		SPADAS_API Float atan(Float x, Float y = 1.0f);

		/// \~English @brief Trigonometric function: arc-tangent(x/y) (output FINF or DINF if the input parameter is invalid)
		/// \~Chinese @brief 三角函数: arc-tangent(x/y) (输出FINF或DINF若输入参数无效)
		SPADAS_API Double atan(Double x, Double y = 1.0);

		/// \~English @brief Obtain the depth of an integer (the depth of 0~1 is 0, the depth of 2~3 is 1, the depth of 4~7 is 2, the depth of 8~15 is 3, ...)
		/// \~Chinese @brief 获取整型数的深度 (0~1的深度为0, 2~3的深度为1, 4~7的深度为2, 8~15的深度为3, ...)
		SPADAS_API UInt depth(UInt number);

		/// \~English @brief Generate a random number between 0.0 and 1.0, the number is specified by the parameter
		/// \~Chinese @brief 生成介于0.0至1.0的随机数，数量由参数指定
		SPADAS_API Array<Float> random(UInt size);

		/// \~English @brief Generate a random number between 0.0 and 1.0, the number is specified by the parameter
		/// \~Chinese @brief 生成介于0.0至1.0的随机数，数量由参数指定
		SPADAS_API Array<Double> randomD(UInt size);

		/// \~English @brief SVD decomposition (src=u*s*v.transpose())
		/// \~Chinese @brief SVD分解 (src=u*s*v.transpose())
		SPADAS_API void decomposeSVD(FloatMat src, FloatMat& u, FloatMat& s, FloatMat& v);

		/// \~English @brief SVD decomposition (src=u*s*v.transpose())
		/// \~Chinese @brief SVD分解 (src=u*s*v.transpose())
		SPADAS_API void decomposeSVD(DoubleMat src, DoubleMat& u, DoubleMat& s, DoubleMat& v);

		/// \~English @brief Calculate the inverse matrix (src*inv=I) or pseudo-inverse matrix (src*inv*src=src)
		/// \~Chinese @brief 计算逆矩阵 (src*inv=I) 或伪逆矩阵 (src*inv*src=src)
		SPADAS_API FloatMat inverse(FloatMat src);

		/// \~English @brief Calculate the inverse matrix (src*inv=I) or pseudo-inverse matrix (src*inv*src=src)
		/// \~Chinese @brief 计算逆矩阵 (src*inv=I) 或伪逆矩阵 (src*inv*src=src)
		SPADAS_API DoubleMat inverse(DoubleMat src);

		/// \~English @brief Least squares method to solve equations
		/// \~Chinese @brief 最小二乘法解方程组
		SPADAS_API Array<Float> solve(FloatMat a, Array<Float> b);

		/// \~English @brief Least squares method to solve equations
		/// \~Chinese @brief 最小二乘法解方程组
		SPADAS_API Array<Double> solve(DoubleMat a, Array<Double> b);
	}

	// Multithreading / 多线程 //////////////////////////////////////////////////////////////

	/// \~English @brief [Multithread safety] Thread lock
	/// \~Chinese @brief [多线程安全] 线程锁
	class SPADAS_API Lock : public Object<class LockVars>
	{
	public:
		/// \~English @brief Class name
		/// \~Chinese @brief 类名称
		static const String TypeName;

		/// \~English @brief Create a non-spinlock object (default is not entered)
		/// \~Chinese @brief 创建非自旋锁对象 (默认为未进入)
		Lock();

		/// \~English @brief Create a spinlock or non-spinlock object (the default is not entered)
		/// \~Chinese @brief 创建自旋锁或非自旋锁对象 (默认为未进入)
		Lock(Bool spin);

		/// \~English @brief Whether it is a spin lock
		/// \~Chinese @brief 是否为自旋锁
		Bool isSpin();

		/// \~English @brief Enter/lock
		/// \~Chinese @brief 进入/加锁
		void enter();

		/// \~English @brief Leave/unlock
		/// \~Chinese @brief 离开/解锁
		void leave();

	private:
		Bool isNull() { return FALSE; }
		Bool isValid() { return FALSE; }
	};

	/// \~English @brief Thread lock proxy, which can secure the use of thread locks and prevent dead locks caused by not calling Lock::leave
	/// \~Chinese @brief 线程锁代理，可方便线程锁的使用，杜绝未调用Lock::leave导致线程锁死的现象
	class SPADAS_API LockProxy
	{
	public:
		/// \~English @brief Bind thread lock (lock.enter will be called automatically)
		/// \~Chinese @brief 绑定线程锁 (将自动调用lock.enter)
		LockProxy(Lock lock);

		/// \~English @brief Destructor (will automatically call lock.leave)
		/// \~Chinese @brief 析构函数 (将自动调用lock.leave)
		~LockProxy();

		/// \~English @brief Call lock.leave in advance
		/// \~Chinese @brief 提前调用lock.leave
		void releaseLock();

	private:
		Lock lock;
		Bool released;
		LockProxy(const LockProxy& obj);
		LockProxy& operator =(const LockProxy& obj) { return *this; }
	};

	/// \~English @brief [Multithread safety] Flag
	/// \~Chinese @brief [多线程安全] 标志位
	class SPADAS_API Flag : public Object<class FlagVars>
	{
	public:
		/// \~English @brief Class name
		/// \~Chinese @brief 类名称
		static const String TypeName;

		/// \~English @brief Create a Flag object (the default is "not set" state)
		/// \~Chinese @brief 创建Flag对象 (默认为not set状态)
		Flag();
		
		/// \~English @brief Create a Flag object and call the Flag::set method automatically after a delay. As a timer, it can be used to wait or notify timeout
		/// \~Chinese @brief 创建Flag对象并延迟一段时间后自动调用 Flag::set 方法。作为定时器，可用于等待或告知超时
		/// \~English @param delaySet Delay time to set, in milliseconds
		/// \~Chinese @param delaySet 延迟set的时间，单位毫秒
		Flag(UInt delaySet);

		/// \~English @brief Change to "set" state
		/// \~Chinese @brief 改为set状态
		void set();

		/// \~English @brief Change to "not set" state
		/// \~Chinese @brief 改为not set状态
		void reset();
		
		/// \~English @brief Automatically calls the Flag::set method after a delay
		/// \~Chinese @brief 延迟一段时间后自动调用 Flag::set 方法
		/// \~English @param time Delay time to set, in milliseconds
		/// \~Chinese @param time 延迟set的时间，单位毫秒
		void delaySet(UInt time);
		
		/// \~English @brief Automatically calls the Flag::reset method after a delay
		/// \~Chinese @brief 延迟一段时间后自动调用 Flag::reset 方法
		/// \~English @param time Delay time to reset, in milliseconds
		/// \~Chinese @param time 延迟reset的时间，单位毫秒
		void delayReset(UInt time);

		/// \~English @brief Whether the current state is "set"
		/// \~Chinese @brief 当前状态是否为set
		Bool check();

		/// \~English @brief Wait for the state to change to "set"
		/// \~Chinese @brief 等待状态变为set
		/// \~English @param waitTime Waiting time in milliseconds
		/// \~Chinese @param waitTime 等待时间，单位毫秒
		/// \~English @returns Returns FALSE if timeout
		/// \~Chinese @returns 若超时则返回FALSE
		Bool waitSet(UInt waitTime = UINF);

		/// \~English @brief Wait for the state to change to "not set"
		/// \~Chinese @brief 等待状态变为not set
		/// \~English @param waitTime Waiting time in milliseconds
		/// \~Chinese @param waitTime 等待时间，单位毫秒
		/// \~English @returns Returns FALSE if timeout
		/// \~Chinese @returns 若超时则返回FALSE
		Bool waitReset(UInt waitTime = UINF);

	private:
		Bool isNull() { return FALSE; }
		Bool isValid() { return FALSE; }
	};

	/// \~English @brief [Multithread safety] Thread synchronizer
	/// \~Chinese @brief [多线程安全] 线程同步器
	class SPADAS_API Barrier : public Object<class BarrierVars>
	{
	public:
		/// \~English @brief Class name
		/// \~Chinese @brief 类名称
		static const String TypeName;

		/// \~English @brief Create a synchronizer object (default strength is 0)
		/// \~Chinese @brief 创建一个同步器对象 (默认强度为0)
		Barrier();

		/// \~English @brief Set strength
		/// \~Chinese @brief 设置强度
		void setStrength(UInt strength);

		/// \~English @brief nter the synchronization state, when the number of threads entering the synchronization state is equal to the strength of the synchronizer, it will return TRUE (return FALSE if it times out)
		/// \~Chinese @brief 进入同步状态，当进入同步状态的线程数等于同步器强度时结束返回TRUE (超时则返回FALSE)
		Bool against(Flag interrupt);

	private:
		Bool isNull() { return FALSE; }
		Bool isValid() { return FALSE; }
	};

	/// \~English @brief Status information of a thread
	/// \~Chinese @brief 一个线程的状态信息
	struct ThreadStatus
	{
		/// \~English @brief Is it running
		/// \~Chinese @brief 是否正在运行
		Bool isActive;

		/// \~English @brief Thread name
		/// \~Chinese @brief 线程名称
		String threadName;

		/// \~English @brief Thread ID
		/// \~Chinese @brief 线程ID
		Optional<UInt> threadID;

		/// \~English @brief Time interval between loops set by the user [ms] (valid only when user-defined time interval is used)
		/// \~Chinese @brief 用户设置的循环时间间隔[ms] (当使用用户自定义的循环间隔时才有效)
		Optional<UInt> userTimeInterval;

		/// \~English @brief Thread loop average processing time [ms]
		/// \~Chinese @brief 线程循环平均处理时间 [ms]
		Optional<Float> averageProcessTime;

		/// \~English @brief The running time of the current loop [ms] (use this value to confirm whether the thread is freezed)
		/// \~Chinese @brief 当前循环已运行时间 [ms] (通过此数值确认线程是否已卡死)
		Float currentLoopTime;
		
		/// \~English @brief Default constructor
		/// \~Chinese @brief 默认构造函数
		SPADAS_API ThreadStatus();
	};
	
	/// \~English @brief Status information of a workflow
	/// \~Chinese @brief 一个流程的状态信息
	struct WorkflowStatus
	{
		/// \~English @brief Workflow ID
		/// \~Chinese @brief 流程ID
		UInt workflowID;

		/// \~English @brief Workflow name
		/// \~Chinese @brief 流程名称
		String workflowName;

		/// \~English @brief Whether the workflow is running (TRUE as long as there is still one thread active)
		/// \~Chinese @brief 流程是否正在运行（只要还有一个线程活动即为TRUE）
		Bool isActive;

		/// \~English @brief Status of all threads in the workflow
		/// \~Chinese @brief 流程中所有线程的状态
		Array<ThreadStatus> threadStatus;
		
		/// \~English @brief Default constructor
		/// \~Chinese @brief 默认构造函数
		SPADAS_API WorkflowStatus();
	};
	
	/// \~English @brief The workflow interface
	/// \~Chinese @brief 流程的标准接口
	class SPADAS_API IWorkflow
	{
	public:
		virtual ~IWorkflow() {};
		
		/// \~English @brief [Optional] Give the name of the workflow (only called when Threads::start)
		/// \~Chinese @brief [可选] 给出流程的名称 (仅在 Threads::start 时被调用)
		virtual String getWorkflowName();
		
		/// \~English @brief Give the names of all threads, and the size of the array is the number of threads. Report an error if it returns empty (only called when Threads::start)
		/// \~Chinese @brief 给出所有线程的名称，数组大小即为线程数。若返回空则报错 (仅在 Threads::start 时被调用)
		virtual Array<String> getThreadNames();
		
		/// \~English @brief [Optional] Give the default time interval between loops of the thread whose index is threadIndex, in milliseconds
		/// \~Chinese @brief [可选] 给出序号为threadIndex的线程默认的循环间隔，单位毫秒
		virtual UInt getTimeInterval(UInt threadIndex);

		/// \~English @brief [Optional] Specifies whether the thread whose index is threadIndex accepts a user-defined time interval
		/// \~Chinese @brief [可选] 指定序号为threadIndex的线程是否接受用户自定义的循环间隔
		virtual Bool supportUserTimeInterval(UInt threadIndex);
		
		/// \~English @brief [Optional] The function that the thread whose index is threadIndex runs at the beginning of the thread (returns FALSE if it fails)
		/// \~Chinese @brief [可选] 给出序号为threadIndex的线程在线程开始处运行的代码 (若失败则返回FALSE)
		virtual Bool onThreadBegin(UInt threadIndex);
		
		/// \~English @brief The function called by the thread whose index is threadIndex in each loop. You'd better not use functions with waiting functions here
		/// \~Chinese @brief 给出序号为threadIndex的线程在每次循环时调用的代码，其中应尽量不使用具有等待功能的函数
		virtual void onThreadLoop(UInt threadIndex, Flag shouldEnd);
		
		/// \~English @brief [Optional] The function that the thread whose index is threadIndex runs when the thread ends
		/// \~Chinese @brief [可选] 给出序号为threadIndex的线程在线程结束时运行的代码
		virtual void onThreadEnd(UInt threadIndex);
	};

	/// \~English @brief Multi-threaded container
	/// \~Chinese @brief 多线程容器
	class SPADAS_API Threads : public Object<class ThreadsVars>
	{
	public:
		/// \~English @brief Class name
		/// \~Chinese @brief 类名称
		static const String TypeName;

		/// \~English @brief Invalid object (should be created by Threads::start or Threads::begin)
		/// \~Chinese @brief 无效对象（应由 Threads::start 或 Threads::begin 创建）
		Threads();
		
		/// \~English @brief Whether the thread is running
		/// \~Chinese @brief 线程是否正在运行
		Bool isRunning();
		
		/// \~English @brief Stop the workflow and wait for all threads to terminate (return FALSE if timeout). If you own an object, it is recommended to execute obj = Threads() after calling this function
		/// \~Chinese @brief 停止流程，并等待所有线程结束 (若超时返回FALSE)。若持有对象，建议在调用此函数后执行 obj = Threads()
		Bool stop(UInt timeout = UINF);

		/// \~English @brief Stop the workflow and return without waiting
		/// \~Chinese @brief 停止流程，不等待即返回
		void stopAsync();

		/// \~English @brief Get the interface of the running workflow
		/// \~Chinese @brief 取得运行中流程的接口
		Interface<IWorkflow> getWorkflow();
		
		/// \~English @brief Get the complete status of the workflow
		/// \~Chinese @brief 取得流程运行的完整状态
		WorkflowStatus getWorkflowStatus();
		
		/// \~English @brief Use a user-defined time interval (requires the IWorkflow::supportUserTimeInterval method to return TRUE)
		/// \~Chinese @brief 使用用户自定义的循环间隔（需要 IWorkflow::supportUserTimeInterval 方法返回TRUE）
		void useUserTimeInterval(UInt threadIndex, UInt interval);
		
		/// \~English @brief Use the default time interval, which is obtained through the IWorkflow::getTimeInterval method of the workflow interface
		/// \~Chinese @brief 使用默认的循环间隔，该值通过流程接口的 IWorkflow::getTimeInterval 方法获得
		void useDefaultTimeInterval(UInt threadIndex);
		
		/// \~English @brief Start the child thread to run the specified workflow, and return the container object of the workflow after all threads finished running IWorkflow::onThreadBegin
		/// \~Chinese @brief 启动子线程运行指定流程，在所有线程运行 IWorkflow::onThreadBegin 完毕后返回该流程的容器对象
		static Threads start(Interface<IWorkflow> workflow);

		/// \~English @brief Start the child thread to run the specified workflow, and return the container object of the workflow after all threads finished running IWorkflow::onThreadBegin, and output whether each thread starts successfully
		/// \~Chinese @brief 启动子线程运行指定流程，在所有线程运行 IWorkflow::onThreadBegin 完毕后返回该流程的容器对象，并输出每个线程是否成功开始
		static Threads start(Interface<IWorkflow> workflow, Array<Bool>& threadsRet);
		
		/// \~English @brief Run the specified workflow in this thread (the number of threads required for this workflow should be 1)
		/// \~Chinese @brief 在本线程中运行指定流程 (该流程所需线程数应为1)
		static void run(Interface<IWorkflow> workflow, Flag interrupt);

		/// \~English @brief Stop all running workflows and wait for all threads to end (return FALSE if timed out)
		/// \~Chinese @brief 停止所有正在运行的流程，并等待所有线程结束 (若超时返回FALSE)
		static Bool stopAllThreads(UInt timeout = UINF);
		
		/// \~English @brief Get the container of the specified process (if not found, return an invalid container)
		/// \~Chinese @brief 取得指定流程的容器 (若未找到则返回无效容器)
		static Threads getThreadsOf(Interface<IWorkflow> workflow);
		
		/// \~English @brief Get all currently valid multi-threaded containers
		/// \~Chinese @brief 取得当前所有有效的多线程容器
		static Array<Threads> getAllThreads();
		
		/// \~English @brief Get the ID of the current thread
		/// \~Chinese @brief 取得当前线程的ID
		static UInt getCurrentThreadID();
	};

	/// \~English @brief Task thread interface
	/// \~Chinese @brief 任务线程的标准接口
	class SPADAS_API ITask
	{
	public:
		virtual ~ITask() {};

		/// \~English @brief Task execution function
		/// \~Chinese @brief 任务的执行函数
		virtual void onRunTask(Flag shouldEnd);
	};

	/// \~English @brief Task thread manager
	/// \~Chinese @brief 任务线程管理器
	class SPADAS_API TaskManager : public Object<class TaskManagerVars>
	{
	public:
		/// \~English @brief Class name
		/// \~Chinese @brief 类名称
		static const String TypeName;

		/// \~English @brief Create task thread manager
		/// \~Chinese @brief 创建任务线程管理器
		TaskManager();

		/// \~English @brief Start a thread to execute the task
		/// \~Chinese @brief 开启线程执行任务
		void start(Interface<ITask> task);

		/// \~English @brief Stop the thread and wait for the end (if timeout returns FALSE)
		/// \~Chinese @brief 停止线程，并等待结束 (若超时返回FALSE)
		Bool stop(Interface<ITask> task, UInt timeout = UINF);

		/// \~English @brief Get the running tasks
		/// \~Chinese @brief 获取正在运行中的任务
		Array<Interface<ITask> > getTasks();

		/// \~English @brief Query whether the task is running
		/// \~Chinese @brief 查询任务是否运行中
		Bool isTaskRunning(Interface<ITask> task);

		/// \~English @brief Wait for all threads to finish running, if interrupted, return FALSE
		/// \~Chinese @brief 等待所有线程运行完成，若被中断则返回FALSE
		Bool waitAll(Flag interrupt);

		/// \~English @brief Stop all threads and wait for execution to complete
		/// \~Chinese @brief 停止所有线程并等待运行完成
		void stopAll(UInt timeout = UINF);

	private:
		Bool isNull() { return FALSE; }
		Bool isValid() { return FALSE; }
	};

	// Utilities / 实用工具 //////////////////////////////////////////////////////////////

	/// \~English @brief Operating system
	/// \~Chinese @brief 运行环境
	class SPADAS_API Environment
	{
	public:
		static const Int Unknown = 0;
		static const Int Linux = 3;
		static const Int MacOS = 4;
		static const Int Windows = 6;

		static Bool isValid(Int val);
		static String toString(Int val);
		static Int defaultValue();
	};

	/// \~English @brief Posix time in milliseconds
	/// \~Chinese @brief Posix时间，毫秒单位
	typedef ULong MilliPosix;

	/// \~English @brief Posix time in nanoseconds
	/// \~Chinese @brief Posix时间，纳秒单位
	typedef ULong NanoPosix;

	/// \~English @brief Date and time
	/// \~Chinese @brief 日期时间
	struct Time
	{
		/// \~English @brief Year
		/// \~Chinese @brief 年
		UInt year;

		/// \~English @brief Month
		/// \~Chinese @brief 月
		UInt month;

		/// \~English @brief Day
		/// \~Chinese @brief 日
		UInt day;

		/// \~English @brief Hour
		/// \~Chinese @brief 时
		UInt hour;

		/// \~English @brief Minute
		/// \~Chinese @brief 分
		UInt minute;

		/// \~English @brief Second
		/// \~Chinese @brief 秒
		UInt second;

		/// \~English @brief The default constructor, the time is January 1, 1900 0:00:00
		/// \~Chinese @brief 默认构造函数，时间为1900年1月1日0时0分0秒
		SPADAS_API Time();

		/// \~English @brief Initialize based on the specified year, month, day, hour, minute, and second
		/// \~Chinese @brief 基于指定年月日时分秒初始化
		SPADAS_API Time(UInt year, UInt month, UInt day, UInt hour, UInt minute, UInt second);

		/// \~English @brief Whether it is equal to
		/// \~Chinese @brief 是否等于
		SPADAS_API Bool operator ==(Time time);

		/// \~English @brief Whether it is not equal to
		/// \~Chinese @brief 是否不等于
		SPADAS_API Bool operator !=(Time time);

		/// \~English @brief Is later than
		/// \~Chinese @brief 是否大于
		SPADAS_API Bool operator >(Time time);

		/// \~English @brief Is earlier than
		/// \~Chinese @brief 是否小于
		SPADAS_API Bool operator <(Time time);

		/// \~English @brief Get the hash value
		/// \~Chinese @brief 获取哈希值
		SPADAS_API Word getHash();

		/// \~English @brief Create a date (year, month, day) string based on the specified separator
		/// \~Chinese @brief 基于指定分隔符创建日期(年月日)字符串
		SPADAS_API String dateString(String separator = "/");

		/// \~English @brief Create a time (hour, minute, second) string based on the specified separator
		/// \~Chinese @brief 基于指定分隔符创建时间(时分秒)字符串
		SPADAS_API String timeString(String separator = ":");

		/// \~English @brief Convert to string based on separators
		/// \~Chinese @brief 基于分隔符转为字符串
		SPADAS_API String toString(String dateSeparator = "/", String timeSeparator = ":", String dateTimeSeparator = " ");
	};

	/// \~English @brief Date and time with millisecond precision
	/// \~Chinese @brief 精度至毫秒的日期时间
	struct TimeWithMS
	{
		/// \~English @brief Date and time, year, month, day, hour, minute, second
		/// \~Chinese @brief 日期与时间，年月日时分秒
		Time dateTime;

		/// \~English @brief Milliseconds
		/// \~Chinese @brief 毫秒数
		UInt milliseconds;

		/// \~English @brief The default constructor, the time is January 1, 1900 0:00:00:00 milliseconds
		/// \~Chinese @brief 默认构造函数，时间为1900年1月1日0时0分0秒0毫秒
		TimeWithMS() : milliseconds(0)
		{}

		/// \~English @brief Initialization based on date and time with millisecond precision
		/// \~Chinese @brief 基于日期和毫秒的初始化
		TimeWithMS(Time dateTime, UInt milliseconds) : dateTime(dateTime), milliseconds(milliseconds)
		{}

		/// \~English @brief Initialization based on components
		/// \~Chinese @brief 基于指定年月日时分秒初始化
		TimeWithMS(UInt year, UInt month, UInt day, UInt hour, UInt minute, UInt second, UInt milliseconds) : dateTime(year, month, day, hour, minute, second), milliseconds(milliseconds)
		{}

		/// \~English @brief Whether it is equal to
		/// \~Chinese @brief 是否等于
		SPADAS_API Bool operator ==(TimeWithMS time);

		/// \~English @brief Whether it is not equal to
		/// \~Chinese @brief 是否不等于
		SPADAS_API Bool operator !=(TimeWithMS time);

		/// \~English @brief Returns a time based on this time plus/minus the specified milliseconds
		/// \~Chinese @brief 返回在该时间基础上加/减指定毫秒的时间
		SPADAS_API TimeWithMS operator +(Long millisecond);

		/// \~English @brief Returns the time difference between two times, in milliseconds
		/// \~Chinese @brief 返回两个时间的时间差，单位毫秒
		SPADAS_API Long operator -(TimeWithMS time);

		/// \~English @brief Returns the time difference between two times, in the form of components
		/// \~Chinese @brief 返回两个时间的时间差，以分量的形式
		SPADAS_API void substract(TimeWithMS time, Int& week, Int& day, Int& hour, Int& minute, Int& second, Int& millisecond);

		/// \~English @brief Convert to posix time as local time
		/// \~Chinese @brief 分量时间（按本地时间）转Posix时间
		SPADAS_API MilliPosix localTimeToPosix();

		/// \~English @brief Convert to posix time as UTC time
		/// \~Chinese @brief 分量时间（按国际协调时间）转Posix时间
		SPADAS_API MilliPosix utcTimeToPosix();

		/// \~English @brief Convert from posix time as local time
		/// \~Chinese @brief Posix时间转分量时间（按本地时间）
		SPADAS_API void localTimeFromPosix(MilliPosix posixTime);

		/// \~English @brief Convert from posix time as UTC time
		/// \~Chinese @brief Posix时间转分量时间（按国际协调时间）
		SPADAS_API void utcTimeFromPosix(MilliPosix posixTime);

		/// \~English @brief Convert to string based on separators
		/// \~Chinese @brief 基于分隔符转为字符串
		SPADAS_API String toString(String dateSeparator = "/", String timeSeparator = ":", String dateTimeSeparator = " ", String msSeparator = ".");
	};

	/// \~English @brief Timer
	/// \~Chinese @brief 计时器
	class SPADAS_API Timer : public Object<class TimerVars>
	{
	public:
		/// \~English @brief Class name
		/// \~Chinese @brief 类名称
		static const String TypeName;

		/// \~English @brief Create a timer object and start it immediately
		/// \~Chinese @brief 创建一个计时器对象并立即启动
		Timer();

		/// \~English @brief Create a timer with the specified CPU tick as the start time, the CPU tick must not be greater than the CPU tick at the time of creation
		/// \~Chinese @brief 以指定的CPU计数作为开始时间创建一个计时器，该计数不得大于创建时的CPU计数
		Timer(ULong startCPUTick);
		
		/// \~English @brief Restart the timer
		/// \~Chinese @brief 重启计时器
		void start();
		
		/// \~English @brief Get the elapsed time in milliseconds
		/// \~Chinese @brief 获得经过时长，单位毫秒
		Double check();

		/// \~English @brief Get the start time (since CPU power on) in milliseconds
		/// \~Chinese @brief 获得开始时间，单位毫秒
		Double getStartTime();

		/// \~English @brief Get the CPU tick at the start time
		/// \~Chinese @brief 获得开始时的CPU计数
		ULong getStartCPUTick();

		/// \~English @brief Get the current CPU tick
		/// \~Chinese @brief 获取当前CPU计数
		static ULong cpuTick();

		/// \~English @brief Get the CPU ticks increased per second
		/// \~Chinese @brief 获取每秒增长的CPU计数
		static ULong cpuTicksPerSecond();

	private:
		Bool isNull() { return FALSE; }
		Bool isValid() { return FALSE; }
	};

	/// \~English @brief Oscillator task interface
	/// \~Chinese @brief 振荡器响应接口
	class SPADAS_API ITickHandler
	{
	public:
		virtual ~ITickHandler() {};

		/// \~English @brief Respond to the oscillator signal
		/// \~Chinese @brief 响应振荡器信号
		virtual void onTick(String name);
	};

	/// \~English @brief Oscillator
	/// \~Chinese @brief 振荡器
	class SPADAS_API Tick : public Object<class TickVars>, public IWorkflow
	{
	public:
		/// \~English @brief Class name
		/// \~Chinese @brief 类名称
		static const String TypeName;

		/// \~English @brief Invalid object
		/// \~Chinese @brief 无效对象
		Tick();

		/// \~English @brief Create a non-spin oscillator, and specify the name, task interface and oscillation period (milliseconds). After the thread is started, the task interface will be called according to this period
		/// \~Chinese @brief 创建一个非自旋振荡器，并指定名称、响应接口和振荡周期（毫秒）。线程启动后将按该周期调用响应接口
		Tick(String name, Interface<ITickHandler> handler, UInt period);

		/// \~English @brief Create an oscillator, and specify the name, task interface, oscillation period (milliseconds), and whether to spin. After the thread is started, the task interface will be called according to this period
		/// \~Chinese @brief 创建一个振荡器，并指定名称、响应接口、振荡周期（毫秒）、以及是否自旋。线程启动后将按该周期调用响应接口
		Tick(String name, Interface<ITickHandler> handler, UInt period, Bool spin);

	private:
		String getWorkflowName() override;
		Array<String> getThreadNames() override;
		UInt getTimeInterval(UInt threadIndex) override;
		Bool onThreadBegin(UInt threadIndex) override;
		void onThreadLoop(UInt threadIndex, Flag shouldEnd) override;
		void onThreadEnd(UInt threadIndex) override;
	};

	/// \~English @brief Memory mapping management, used for hardware drivers and inter-process communication, etc.
	/// \~Chinese @brief 内存映射管理，用于硬件驱动和多进程通讯等
	class SPADAS_API MemoryMap : public Object<class MemoryMapVars>
	{
	public:
		/// \~English @brief Class name
		/// \~Chinese @brief 类名称
		static const String TypeName;

		/// \~English @brief Invalid object
		/// \~Chinese @brief 无效对象
		MemoryMap();

		/// \~English @brief Create a memory mapping object, specify the mapping file path (must be created in advance and the file size is not less than offset+size), mapping start offset and mapping area size. Note that the mapping start offset value should be a multiple of the following N values: N = 4096 @ Linux/MacOS, N = 65536 @ Windows
		/// \~Chinese @brief 创建内存映射对象，指定映射文件路径(必须事先创建且文件大小不小于offset+size)，映射起点偏移以及映射区域大小。注意，映射起点偏移值应为以下N值的倍数：N = 4096 @ Linux/MacOS，N = 65536 @ Windows
		MemoryMap(Path file, PointerInt offset, PointerInt size);

		/// \~English @brief Unmap
		/// \~Chinese @brief 解除映射
		void unmap();

		/// \~English @brief [Unsafe pointer] Get the virtual memory address of the starting point of the memory mapping
		/// \~Chinese @brief [非安全指针] 取得映射起点的虚拟内存地址
		Pointer getPointer();
	};

	/// \~English @brief The sending result to the memory-mapped data stream
	/// \~Chinese @brief 内存映射数据流的发送结果
	enum class MemoryMapSendResult
	{
		/// \~English @brief Sent successfully
		/// \~Chinese @brief 发送成功
		OK = 0,

		/// \~English @brief Unopened
		/// \~Chinese @brief 未开启
		NotOpen = 1,

		/// \~English @brief Incorrect mode
		/// \~Chinese @brief 模式不正确
		WrongMode = 2,

		/// \~English @brief Data size exceeds range (slotSize x slotCount)
		/// \~Chinese @brief 数据大小超过范围（slotSize x slotCount）
		WrongSize = 3,

		/// \~English @brief The queue is full, or the capacity is not enough to send the current data
		/// \~Chinese @brief 队列已满，或容量已不足以发送当前数据
		QueueFull = 4,
	};

	/// \~English @brief Data stream based on memory mapping
	/// \~Chinese @brief 基于内存映射的数据流
	class SPADAS_API MemoryMapStream : public Object<class MemoryMapStreamVars>
	{
	public:
		/// \~English @brief Class name
		/// \~Chinese @brief 类名称
		static const String TypeName;

		/// \~English @brief Create a data stream (unopened state)
		/// \~Chinese @brief 创建一个数据流（未开启状态）
		MemoryMapStream();

		/// \~English @brief Is it in "open" state
		/// \~Chinese @brief 是否为开启状态
		Bool isOpen();

		/// \~English @brief Open data stream
		/// \~Chinese @brief 打开数据流
		/// \~English @param path Mapping file path (no need to create in advance)
		/// \~Chinese @param path 映射文件路径（不需要事先创建）
		/// \~English @param slotSize The size of the slot, preferably larger than the average packet size, in bytes, ranging from 16 to 1048576
		/// \~Chinese @param slotSize 槽的大小，最好大于平均的数据包尺寸，单位为字节, 范围为16~1048576
		/// \~English @param slotCount The number of slots, you need to ensure that the slotSize x slotCount is greater than the maximum size of the data packet, the range is 1~1024
		/// \~Chinese @param slotCount 槽的个数，需要保证slotSize乘slotCount大于数据包的最大尺寸，范围为1~1024
		/// \~English @param sendMode Whether it is sending mode, otherwise it is receiving mode
		/// \~Chinese @param sendMode 是否为发送模式，否则为接收模式
		/// \~English @param host Whether to control the creation and deletion of the mapping file, the object that is TRUE needs to execute open first
		/// \~Chinese @param host 是否控制映射文件的创建与删除，为TRUE的对象需要先执行open
		/// \~English @returns Return success
		/// \~Chinese @returns 返回是否成功
		Bool open(Path path, UInt slotSize, UInt slotCount, Bool sendMode, Bool host);
		
		/// \~English @brief Close the data stream
		/// \~Chinese @brief 关闭数据流
		void close();

		/// \~English @brief Send data (use pointer directly)
		/// \~Chinese @brief 发送数据（直接使用指针）
		/// \~English @param dataPtr The head pointer of the data
		/// \~Chinese @param dataPtr 数据的起始指针
		/// \~English @param byteCount The number of bytes of data
		/// \~Chinese @param byteCount 数据的字节数
		/// \~English @returns The sending result
		/// \~Chinese @returns 返回发送结果
		MemoryMapSendResult send(Pointer dataPtr, UInt byteCount);

		/// \~English @brief Send data
		/// \~Chinese @brief 发送数据
		/// \~English @param data Binary data
		/// \~Chinese @param data 二进制数据
		/// \~English @returns The sending result
		/// \~Chinese @returns 返回发送结果
		MemoryMapSendResult send(Binary data);

		/// \~English @brief Receive all new binary data
		/// \~Chinese @brief 接收所有收到的数据
		Array<Binary> receive();

	private:
		Bool isNull() { return FALSE; }
		Bool isValid() { return FALSE; }
	};

	/// \~English @brief Magic: The four-character identifier forms a 32-bit integer number
	/// \~Chinese @brief Magic：四字符标识构成一个32位整型数字
	class SPADAS_API MagicNumber : public Object<class MagicNumberVars>
	{
	public:
		/// \~English @brief Class name
		/// \~Chinese @brief 类名称
		static const String TypeName;

		/// \~English @brief Create an invalid number (0)
		/// \~Chinese @brief 创建一个无效数字 (0)
		MagicNumber();

		/// \~English @brief Create a magic number (all characters should be between 0~9, A~Z and a~z, a~z will be converted to uppercase)
		/// \~Chinese @brief 创建一个Magic数字 (所有字符应位于0~9, A~Z和a~z之间，a~z将被转换为大写)
		MagicNumber(Char c0, Char c1, Char c2, Char c3);

		/// \~English @brief Is it the same as another magic number
		/// \~Chinese @brief 与另一个Magic数字是否相同
		Bool operator ==(MagicNumber magic);

		/// \~English @brief Is it different from another magic number
		/// \~Chinese @brief 与另一个Magic数字是否不同
		Bool operator !=(MagicNumber magic);

		/// \~English @brief Get four characters
		/// \~Chinese @brief 取得四个字符
		Array<Char> getChars();

		/// \~English @brief Get the magic number
		/// \~Chinese @brief 取得Magic数字
		UInt getNumber();

		/// \~English @brief Get the hash value
		/// \~Chinese @brief 获取哈希值
		Word getHash();

		/// \~English @brief Convert four characters to a string
		/// \~Chinese @brief 四字符转换为字符串
		String toString();

	private:
		Bool isNull() { return FALSE; }
		Bool isValid() { return FALSE; }
	};

	/// \~English @brief Dynamic library loader
	/// \~Chinese @brief 动态库加载器
	class SPADAS_API LibraryLoader : public Object<class LibraryLoaderVars>
	{
	public:
		/// \~English @brief Class name
		/// \~Chinese @brief 类名称
		static const String TypeName;

		/// \~English @brief Create a dynamic library loader
		/// \~Chinese @brief 创建动态库加载器
		LibraryLoader();

		/// \~English @brief Use the library name under the specified folder to open the dynamic library (no need to add a prefix or suffix such as "lib"), libVersion is used to specify the suffix version number of the dynamic library such as so (such as "1.0")
		/// \~Chinese @brief 使用指定文件夹下的库名称打开动态库（无需添加lib等前缀或后缀名），libVersion用于指定so等动态库的后缀版本号（如"1.0"）
		Bool openWithName(Path libDir, String libName, String libVersion = String());

		/// \~English @brief Use the library name under the specified folder to open the dynamic library (no need to add a prefix or suffix such as "lib"), libVersion is used to speify the suffix version number of the dynamic library such as so (such as "1.0"), and output an error message if it fails
		/// \~Chinese @brief 使用指定文件夹下的库名称打开动态库（无需添加lib等前缀或后缀名），libVersion用于指定so等动态库的后缀版本号（如"1.0"），若失败则输出错误信息
		Bool openWithName(Path libDir, String libName, String libVersion, String& errorMessage);

		/// \~English @brief Open the dynamic library using the library file path
		/// \~Chinese @brief 使用库文件路径打开动态库
		Bool openWithPath(Path libPath);

		/// \~English @brief Open the dynamic library using the library file path. If it fails, output an error message
		/// \~Chinese @brief 使用库文件路径打开动态库，若失败则输出错误信息
		Bool openWithPath(Path libPath, String& errorMessage);

		/// \~English @brief Close the dynamic library (generally not executed)
		/// \~Chinese @brief 关闭动态库（一般不执行）
		void close();

		/// \~English @brief Get symbol address
		/// \~Chinese @brief 获取符号地址
		Pointer getSymbol(String symbol);

	private:
		Bool isNull() { return FALSE; }
		Bool isValid() { return FALSE; }
	};

	/// \~English @brief System function namespace
	/// \~Chinese @brief 系统功能函数命名空间
	namespace system
	{
		/// \~English @brief Get the current spadas version
		/// \~Chinese @brief 取得当前Spadas版本
		SPADAS_API String getSpadasVersion();

		/// \~English @brief Get copyright notices of the third party softwares spadas uses
		/// \~Chinese @brief 取得Spadas使用的第三方软件的版权声明
		/// \~English @returns Key is title, value is copyright notice
		/// \~Chinese @returns 键为标题，值为版权声明
		SPADAS_API Dictionary<String> getSpadasThirdPartyNotices();

		/// \~English @brief Get the current running operating system
		/// \~Chinese @brief 取得当前运行环境
		SPADAS_API Enum<Environment> getEnv();

		/// \~English @brief Whether the byte order is Big-Endian
		/// \~Chinese @brief 检查字节序是否Big-Endian
		SPADAS_API Bool isBigEndian();

		/// \~English @brief Get the current host time (local time zone)
		/// \~Chinese @brief 获得当前主机时间（本地时区）
		SPADAS_API Time getTime();

		/// \~English @brief Get the current host time (local time zone), with millisecond information
		/// \~Chinese @brief 获得当前主机时间（本地时区），带毫秒信息
		SPADAS_API TimeWithMS getTimeWithMS();

		/// \~English @brief Wait for the specified time, in milliseconds (the accuracy may drop to 5~10 milliseconds when the system load is high)
		/// \~Chinese @brief 等待指定时间，单位毫秒（系统负载高时精度可能会降至5~10毫秒）
		SPADAS_API void wait(UInt time);

		/// \~English @brief Wait for the specified time, in microseconds
		/// \~Chinese @brief 等待指定时间，单位微秒
		SPADAS_API void waitSpin(UInt timeUS);

		/// \~English @brief Execute a console command
		/// \~Chinese @brief 执行一个控制台命令
		SPADAS_API void command(String cmd);

		/// \~English @brief Forcibly terminate the application
		/// \~Chinese @brief 强制结束应用程序
		SPADAS_API void exit();

		/// \~English @brief Add path environment variables
		/// \~Chinese @brief 添加路径环境变量
		SPADAS_API void addEnvironmentPath(Path path);

		/// \~English @brief Ping, the timeout unit is milliseconds
		/// \~Chinese @brief Ping，超时单位为毫秒
		SPADAS_API Bool ping(String ip, UInt timeout = 1000);
	}

	/// \~English @brief Utility function namespace
	/// \~Chinese @brief 实用功能函数命名空间
	namespace utility
	{
		/// \~English @brief [Unsafe operation] Set the value of a segment of memory in bytes
		/// \~Chinese @brief [非安全操作] 以字节为单位设置一段内存的值
		/// \~English @param value The byte value to set
		/// \~Chinese @param value 希望设置的字节数值
		/// \~English @param dst Memory start address
		/// \~Chinese @param dst 内存起始地址
		/// \~English @param setSize The number of bytes to set
		/// \~Chinese @param setSize 设置的字节数
		SPADAS_API void memorySet(Byte value, Pointer dst, UInt setSize);

		/// \~English @brief [Unsafe operation] memory copy (highest efficiency)
		/// \~Chinese @brief [非安全操作] 内存拷贝（效率最高）
		/// \~English @param src Start address of copy source memory
		/// \~Chinese @param src 拷贝源内存起始地址
		/// \~English @param dst Start address of copy target memory
		/// \~Chinese @param dst 拷贝目标内存起始地址
		/// \~English @param copySize Number of bytes to copy
		/// \~Chinese @param copySize 拷贝字节数
		SPADAS_API void memoryCopy(const Pointer src, Pointer dst, UInt copySize);

		/// \~English @brief [Unsafe operation] Forced conversion of value types (must ensure that the "sizeof" of the two types is consistent, and the structure must ensure that the type and order are consistent)
		/// \~Chinese @brief [非安全操作] 值类型的强制转换（必须保证两个类型sizeof一致，结构体的话必须保证类型与顺序都一致）
		template <typename SrcType, typename DstType>
		DstType valueCast(SrcType val);

		/// \~English @brief [Unsafe operation] Forced conversion of value type arrays (must ensure that the "sizeof" of the two types is consistent, and the structure must ensure that the type and order are consistent)
		/// \~Chinese @brief [非安全操作] 值类型数组的强制转换（必须保证两个类型sizeof一致，结构体的话必须保证类型与顺序都一致）
		template <typename SrcType, typename DstType>
		Array<DstType> valueArrayCast(Array<SrcType> src);

		/// \~English @brief Convert an array of numeric keys to an array of values
		/// \~Chinese @brief 将数值键数组转为数值数组
		template <typename Type>
		Array<Type> unpackKeys(Array<NumericKey<Type> > keys);

		/// \~English @brief Convert the array of numeric key-value pairs to an array of key-value pairs
		/// \~Chinese @brief 将数值键的键值对数组转为数值的键值对数组
		template <typename Type, typename ValueType>
		Array<KeyValue<Type, ValueType> > unpackKeyValues(Array<KeyValue<NumericKey<Type>, ValueType> > keyValues);

		/*
		* ByteBGR: size=(height, width, 3)
		* ByteRGB: size=(height, width, 3)
		* ByteGray: size=(height, width)
		* ByteUYVY: size=(height, width, 2)
		* ByteYUV: size=(height, width, 3)
		* ByteBool: size=(height, width), TRUE's value is 1 / TRUE的值为1
		* ByteBGRA: size=(height, width, 4)
		* ByteRGBA: size=(height, width, 4)
		* WordBGR: size=(height, width, 3)
		* WordGray: size=(height, width)
		*/

		/// \~English @brief Mutual conversion between Int matrix and image
		/// \~Chinese @brief Int型矩阵与图像的相互转换
		SPADAS_API IntMat imageToMatrixInt(Image image);

		/// \~English @brief Mutual conversion between Int matrix and image
		/// \~Chinese @brief Int型矩阵与图像的相互转换
		SPADAS_API Image matrixIntToImage(IntMat matrix, Enum<PixelFormat> format);

		/*
		* FloatBGR: size=(height, width, 3)
		* FloatGray: size=(height, width)
		* FloatHSV: size=(height, width, 3)
		*/

		/// \~English @brief Mutual conversion between Float type matrix and image
		/// \~Chinese @brief Float型矩阵与图像的相互转换
		SPADAS_API FloatMat imageToMatrixFloat(Image image);

		/// \~English @brief Mutual conversion between Float type matrix and image
		/// \~Chinese @brief Float型矩阵与图像的相互转换
		SPADAS_API Image matrixFloatToImage(FloatMat matrix, Enum<PixelFormat> format);

		/// \~English @brief Use the FloatGray image to express the value of the 2D matrix, you need to specify the data range of the matrix, which corresponds to 0.0~1.0 of the image
		/// \~Chinese @brief 用FloatGray图像表现2D矩阵的值，需要指定矩阵的数据范围，对应于图像的0.0~1.0
		SPADAS_API Image matrixRangeToImage(IntMat mat, Int lower, Int upper);

		/// \~English @brief Use the FloatGray image to express the value of the 2D matrix, you need to specify the data range of the matrix, which corresponds to 0.0~1.0 of the image
		/// \~Chinese @brief 用FloatGray图像表现2D矩阵的值，需要指定矩阵的数据范围，对应于图像的0.0~1.0
		SPADAS_API Image matrixRangeToImage(FloatMat mat, Float lower, Float upper);

		/// \~English @brief Use the FloatGray image to express the value of the 2D matrix, you need to specify the data range of the matrix, which corresponds to 0.0~1.0 of the image
		/// \~Chinese @brief 用FloatGray图像表现2D矩阵的值，需要指定矩阵的数据范围，对应于图像的0.0~1.0
		SPADAS_API Image matrixRangeToImage(DoubleMat mat, Double lower, Double upper);
	}

	// Plugin related type definition / 插件相关类型定义 //////////////////////////////////////////////////////////////

	// General sample element (Forward declaration) / 通用样本元素（前置声明）
	struct GeneralElement;

	/// \~English @brief Substitution of Bool type Optional
	/// \~Chinese @brief 代替Bool类型Optional
	struct OptionalBool
	{
		/// \~English @brief Is it valid
		/// \~Chinese @brief 是否有效
		Bool valid;

		/// \~English @brief Boolean value
		/// \~Chinese @brief 布尔值
		Bool value;

		/// \~English @brief Default constructor, initialize to an invalid value
		/// \~Chinese @brief 默认构造函数，初始化为无效值
		OptionalBool() : valid(FALSE), value(FALSE)
		{}

		/// \~English @brief Initialize based on input boolean value
		/// \~Chinese @brief 基于输入的布尔值初始化
		OptionalBool(Bool val) : valid(TRUE), value(val)
		{}

		/// \~English @brief Initialize based on spadas::GeneralElement, all non-zero values are TRUE
		/// \~Chinese @brief 基于 spadas::GeneralElement创建，非0都为TRUE
		SPADAS_API OptionalBool(GeneralElement elem);

		/// \~English @brief Assignment by the input boolean value
		/// \~Chinese @brief 按输入的布尔值赋值
		SPADAS_API OptionalBool& operator =(Bool val);

		/// \~English @brief Is valid and true
		/// \~Chinese @brief 是否有效且为真
		SPADAS_API Bool isValidAndTrue();

		/// \~English @brief Is valid and false
		/// \~Chinese @brief 是否有效且为假
		SPADAS_API Bool isValidAndFalse();

		/// \~English @brief Whether it is equal to
		/// \~Chinese @brief 是否相等
		SPADAS_API Bool operator ==(OptionalBool val);

		/// \~English @brief Whether it is not equal to
		/// \~Chinese @brief 是否不等于
		SPADAS_API Bool operator !=(OptionalBool val);

		/// \~English @brief Convert to string, if invalid, convert to "(invalid)"
		/// \~Chinese @brief 转字符串，若无效则转为(invalid)
		SPADAS_API String toString();
	};

	/// \~English @brief Substitution of Int type Optional
	/// \~Chinese @brief 代替Int类型Optional
	struct OptionalInt
	{
		/// \~English @brief Is it valid
		/// \~Chinese @brief 是否有效
		Bool valid;

		/// \~English @brief Integer value
		/// \~Chinese @brief 整型值
		Int value;

		/// \~English @brief Default constructor, initialize to an invalid value
		/// \~Chinese @brief 默认构造函数，初始化为无效值
		OptionalInt() : valid(FALSE), value(0)
		{}

		/// \~English @brief Initialize based on input integer value
		/// \~Chinese @brief 基于输入的整型值初始化
		OptionalInt(Int val) : valid(TRUE), value(val)
		{}

		/// \~English @brief Initialize based on spadas::GeneralElement
		/// \~Chinese @brief 基于 spadas::GeneralElement创建
		SPADAS_API OptionalInt(GeneralElement elem);

		/// \~English @brief Assignment by the input integer value
		/// \~Chinese @brief 按输入的整型值赋值
		SPADAS_API OptionalInt& operator =(Int val);

		/// \~English @brief Whether it is equal to
		/// \~Chinese @brief 是否相等
		SPADAS_API Bool operator ==(OptionalInt val);

		/// \~English @brief Whether it is not equal to
		/// \~Chinese @brief 是否不等于
		SPADAS_API Bool operator !=(OptionalInt val);

		/// \~English @brief Convert to string, if invalid, convert to "(invalid)"
		/// \~Chinese @brief 转字符串，若无效则转为(invalid)
		SPADAS_API String toString();
	};

	/// \~English @brief Substitution of Double type Optional
	/// \~Chinese @brief 代替Double类型Optional
	struct OptionalDouble
	{
		/// \~English @brief Is it valid
		/// \~Chinese @brief 是否有效
		Bool valid;

		/// \~English @brief Floating point value
		/// \~Chinese @brief 浮点值
		Double value;

		/// \~English @brief Default constructor, initialize to an invalid value
		/// \~Chinese @brief 默认构造函数，初始化为无效值
		OptionalDouble() : valid(FALSE), value(0)
		{}

		/// \~English @brief Initialize based on input floating point value
		/// \~Chinese @brief 基于输入的浮点值初始化
		OptionalDouble(Double val) : valid(TRUE), value(val)
		{}

		/// \~English @brief Initialize based on spadas::GeneralElement
		/// \~Chinese @brief 基于 spadas::GeneralElement创建
		SPADAS_API OptionalDouble(GeneralElement elem);

		/// \~English @brief Assignment by the input floating point value
		/// \~Chinese @brief 按输入的浮点值赋值
		SPADAS_API OptionalDouble& operator =(Double val);

		/// \~English @brief Addition, if any value is invalid, return an invalid value
		/// \~Chinese @brief 加法，若任一值为无效则返回无效值
		SPADAS_API OptionalDouble operator +(OptionalDouble val);

		/// \~English @brief Subtraction, if any value is invalid, return an invalid value
		/// \~Chinese @brief 减法，若任一值为无效则返回无效值
		SPADAS_API OptionalDouble operator -(OptionalDouble val);

		/// \~English @brief Multiplication, if any value is invalid, return an invalid value
		/// \~Chinese @brief 乘法，若任一值为无效则返回无效值
		SPADAS_API OptionalDouble operator *(OptionalDouble val);

		/// \~English @brief Whether it is equal to
		/// \~Chinese @brief 是否相等
		SPADAS_API Bool operator ==(OptionalDouble val);

		/// \~English @brief Whether it is not equal to
		/// \~Chinese @brief 是否不等于
		SPADAS_API Bool operator !=(OptionalDouble val);

		/// \~English @brief Convert string, if invalid, convert to "(invalid)"
		/// \~Chinese @brief 转字符串，若无效则转为(invalid)
		SPADAS_API String toString();
	};

	/// \~English @brief Session identifier
	/// \~Chinese @brief Session标识符
	class SPADAS_API SessionIdentifier
	{
	public:
		/// \~English @brief Create an invalid identifier
		/// \~Chinese @brief 创建无效标识符
		SessionIdentifier();

		/// \~English @brief Initialize based on the specified year, month, day, hour, minute, and second
		/// \~Chinese @brief 基于指定年月日时分秒初始化
		SessionIdentifier(Word year, Byte month, Byte day, Byte hour, Byte minute, Byte second);

		/// \~English @brief Initialize based on date and time
		/// \~Chinese @brief 基于日期初始化
		SessionIdentifier(Time time);

		/// \~English @brief Initialize based on the string "yyyy-MM-dd-HH-mm-ss". If it fails, it will be set to 0
		/// \~Chinese @brief 基于字符串(yyyy-MM-dd-HH-mm-ss)初始化，若失败则置为0
		SessionIdentifier(String idString);

		/// \~English @brief Is it valid
		/// \~Chinese @brief 是否有效
		Bool isValid();

		/// \~English @brief Whether it is equal to
		/// \~Chinese @brief 是否等于
		Bool operator ==(SessionIdentifier id);

		/// \~English @brief Whether it is not equal to
		/// \~Chinese @brief 是否不等于
		Bool operator !=(SessionIdentifier id);

		/// \~English @brief Is greater than
		/// \~Chinese @brief 是否大于
		Bool operator >(SessionIdentifier id);

		/// \~English @brief Is less than
		/// \~Chinese @brief 是否小于
		Bool operator <(SessionIdentifier id);

		/// \~English @brief Get the hash value
		/// \~Chinese @brief 获取哈希值
		Word getHash();

		/// \~English @brief Convert to a string "yyyy-MM-dd-HH-mm-ss"
		/// \~Chinese @brief 转为字符串(yyyy-MM-dd-HH-mm-ss)
		String toString();

		/// \~English @brief Convert to time (convenient for compatibility with old versions)
		/// \~Chinese @brief 转为时间（方便旧版本兼容）
		Time toTime();

	private:
		ULong value;
	};

	/// \~English @brief Time offset synchronization status
	/// \~Chinese @brief 时间偏置同步状态
	enum class TimeOffsetSync
	{
		/// \~English @brief Not synced or sync source unknown
		/// \~Chinese @brief 未同步或同步源未知
		None = 0,

		/// \~English @brief The time has been synchronized with the time server
		/// \~Chinese @brief 已与授时服务器时间同步
		Server = 1,

		/// \~English @brief Synchronized with satellite time
		/// \~Chinese @brief 已与卫星时间同步
		Gnss = 2,
	};

	/// \~English @brief Simple timestamp
	/// \~Chinese @brief 简单时间戳
	struct ShortTimestamp
	{
		/// \~English @brief Session identifier
		/// \~Chinese @brief Session标识符
		SessionIdentifier session;

		/// \~English @brief Time offset, in seconds, valid if it is greater than zero
		/// \~Chinese @brief 时间偏置，单位秒，大于零有效
		Double offset;

		/// \~English @brief Default constructor
		/// \~Chinese @brief 默认构造函数
		ShortTimestamp() : offset(0)
		{}

		/// \~English @brief Initialize based on session identifier and time offset
		/// \~Chinese @brief 基于Session标识符和时间偏置初始化
		ShortTimestamp(SessionIdentifier session, Double offset) : session(session), offset(offset)
		{}

		/// \~English @brief Whether it is equal to
		/// \~Chinese @brief 是否等于
		SPADAS_API Bool operator ==(ShortTimestamp timestamp);

		/// \~English @brief Whether it is not equal to
		/// \~Chinese @brief 是否不等于
		SPADAS_API Bool operator !=(ShortTimestamp timestamp);

		/// \~English @brief Is later than
		/// \~Chinese @brief 是否大于
		SPADAS_API Bool operator >(ShortTimestamp timestamp);

		/// \~English @brief Is earlier than
		/// \~Chinese @brief 是否小于
		SPADAS_API Bool operator <(ShortTimestamp timestamp);

		/// \~English @brief Convert to a string. The format is "SessionYear-Month-Day-Hour-Minute-Second-Offset", such as 2019-01-01-12-30-45-123.456789
		/// \~Chinese @brief 转为字符串显示，格式为"Session年-月-日-时-分-秒-偏置"，如2019-01-01-12-30-45-123.456789
		SPADAS_API String toString();
	};

	/// \~English @brief Full timestamp
	/// \~Chinese @brief 完整时间戳
	struct FullTimestamp
	{
		/// \~English @brief Session identifier
		/// \~Chinese @brief Session标识符
		SessionIdentifier session;

		/// \~English @brief Time offset, in seconds, valid if it is greater than zero
		/// \~Chinese @brief 时间偏置，单位秒，大于零有效
		Double offset;

		/// \~English @brief Time offset source
		/// \~Chinese @brief 时间偏置来源
		TimeOffsetSync offsetSync;

		/// \~English @brief CPU tick when data arrives, 0 means invalid
		/// \~Chinese @brief 到达时CPU计数，0表示无效
		ULong cpuTick;

		/// \~English @brief Host posix time when data arrives, in nanoseconds, 0 means invalid
		/// \~Chinese @brief 到达时主机Posix时间，单位纳秒，0表示无效
		NanoPosix hostPosix;

		/// \~English @brief Guest posix time at data sampling, in nanoseconds, 0 means invalid
		/// \~Chinese @brief 采样时客机Posix时间，单位纳秒，0表示无效
		NanoPosix guestPosix;

		/// \~English @brief posix time of the time server at data sampling, in nanoseconds, 0 means invalid
		/// \~Chinese @brief 采样时授时服务器Posix时间，单位纳秒，0表示无效
		NanoPosix serverPosix;

		/// \~English @brief Satellite posix time at data sampling, in nanoseconds, 0 means invalid
		/// \~Chinese @brief 采样时卫星Posix时间，单位纳秒，0表示无效
		NanoPosix gnssPosix;

		/// \~English @brief Default constructor
		/// \~Chinese @brief 默认构造函数
		FullTimestamp() : offset(0), offsetSync(TimeOffsetSync::None), cpuTick(0), hostPosix(0), guestPosix(0), serverPosix(0), gnssPosix(0)
		{}

		/// \~English @brief Create from a simple timestamp, no synchronization information or session-irrelevant time information
		/// \~Chinese @brief 从简单时间戳构造，无同步信息或Session无关时间信息
		FullTimestamp(ShortTimestamp timestamp) : session(timestamp.session), offset(timestamp.offset), offsetSync(TimeOffsetSync::None), cpuTick(0), hostPosix(0), guestPosix(0), serverPosix(0), gnssPosix(0)
		{}

		/// \~English @brief Whether it is equal to (only compare session identifier and time offset)
		/// \~Chinese @brief 是否等于（仅比较Session标识符和时间偏置）
		SPADAS_API Bool operator ==(FullTimestamp timestamp);

		/// \~English @brief Whether it is not equal to (only compare the session identifier and time offset)
		/// \~Chinese @brief 是否不等于（仅比较Session标识符和时间偏置）
		SPADAS_API Bool operator !=(FullTimestamp timestamp);

		/// \~English @brief Is it later than (only compare session identifier and time offset)
		/// \~Chinese @brief 是否大于（仅比较Session标识符和时间偏置）
		SPADAS_API Bool operator >(FullTimestamp timestamp);

		/// \~English @brief Is it earlier than (only compare session identifier and time offset)
		/// \~Chinese @brief 是否小于（仅比较Session标识符和时间偏置）
		SPADAS_API Bool operator <(FullTimestamp timestamp);

		/// \~English @brief Convert to simple timestamp
		/// \~Chinese @brief 转为简单时间戳
		SPADAS_API ShortTimestamp toShort();

		/// \~English @brief Convert to a string, the format is "SessionYear-Month-Day-Hour-Minute-Second-Offset", such as 2019-01-01-12-30-45-123.456789
		/// \~Chinese @brief 转为字符串显示，格式为"Session年-月-日-时-分-秒-偏置"，如2019-01-01-12-30-45-123.456789
		SPADAS_API String toString();
	};

	/// \~English @brief Session irrelevant time category
	/// \~Chinese @brief Session无关时间的类别
	enum class TimeType
	{
		/// \~English @brief CPU tick when data arrives
		/// \~Chinese @brief 到达时CPU计数
		CPUTick = 0,

		/// \~English @brief Host posix time when data arrives, in nanoseconds
		/// \~Chinese @brief 到达时主机Posix时间，单位纳秒
		HostPosix = 1,

		/// \~English @brief Guest posix time at data sampling, in nanoseconds
		/// \~Chinese @brief 采样时客机Posix时间，单位纳秒
		GuestPosix = 2,

		/// \~English @brief posix time of the time server at data sampling, in nanoseconds
		/// \~Chinese @brief 采样时授时服务器Posix时间，单位纳秒
		ServerPosix = 3,

		/// \~English @brief Satellite posix time at data sampling, in nanoseconds
		/// \~Chinese @brief 采样时卫星Posix时间，单位纳秒
		GnssPosix = 4,
	};

	/// \~English @brief Signal
	/// \~Chinese @brief 信号
	struct SessionSignal
	{
		/// \~English @brief Timestamp
		/// \~Chinese @brief 时间戳
		ShortTimestamp timestamp;

		/// \~English @brief Value
		/// \~Chinese @brief 数值
		Double value;

		/// \~English @brief The default constructor, the initial value is 0
		/// \~Chinese @brief 默认构造函数，初始值为0
		SessionSignal() : value(0)
		{}

		/// \~English @brief Initialize based on timestamp and value
		/// \~Chinese @brief 基于时间戳和数值初始化
		SessionSignal(ShortTimestamp timestamp, Double val) : timestamp(timestamp), value(val)
		{}
	};

	/// \~English @brief Signal data table, the key is the signal ID which format is "category:type:name". For example of bus signal, the category is the protocol file name, such as vehicle.dbc; the type is the message channel ID, such as 123
	/// \~Chinese @brief 信号数据表，key为信号ID，格式为"大类:小类:信号名称"。其中总线信号的情况下，大类为协议文件名，如vehicle.dbc；小类为报文本通道ID，如123
	typedef Dictionary<Array<SessionSignal> > SessionSignalTable;

	/// \~English @brief Raw data from general device
	/// \~Chinese @brief 一般设备输出原始数据
	struct GeneralDeviceData
	{
		/// \~English @brief CPU tick when data arrives
		/// \~Chinese @brief 到达主机时的CPU计数
		ULong cpuTick;

		/// \~English @brief Raw data protocol ID whose format is "xxx-v?", "xxx" indicates the data source, "v?" indicates the version
		/// \~Chinese @brief 原始数据协议ID，一般格式为"xxx-v?"，xxx表示数据来源，v?表示版本
		String protocol;

		/// \~English @brief Numerical array data
		/// \~Chinese @brief 数值数组数据
		Array<Double> vector;

		/// \~English @brief [Optional] Binary data
		/// \~Chinese @brief [可选] 二进制数据
		Binary binary;

		/// \~English @brief Default constructor
		/// \~Chinese @brief 默认构造函数
		GeneralDeviceData()
		{}

		/// \~English @brief Initialize based on timestamp and data
		/// \~Chinese @brief 基于时间戳和数据初始化
		GeneralDeviceData(ULong cpuTick, Array<Double> vector, Binary binary) : cpuTick(cpuTick), vector(vector), binary(binary)
		{}
	};

	/// \~English @brief General raw data
	/// \~Chinese @brief 通用原始数据
	struct SessionGeneralRawData
	{
		/// \~English @brief Timestamp
		/// \~Chinese @brief 时间戳
		FullTimestamp timestamp;

		/// \~English @brief Numerical array data
		/// \~Chinese @brief 数值数组数据
		Array<Double> vector;

		/// \~English @brief [Optional] Binary data
		/// \~Chinese @brief [可选] 二进制数据
		Binary binary;

		/// \~English @brief Default constructor
		/// \~Chinese @brief 默认构造函数
		SessionGeneralRawData()
		{}

		/// \~English @brief Initialize based on timestamp and data
		/// \~Chinese @brief 基于时间戳和数据初始化
		SessionGeneralRawData(FullTimestamp timestamp, Array<Double> vector, Binary binary) : timestamp(timestamp), vector(vector), binary(binary)
		{}
	};

	/// \~English @brief General raw data table, the key is the raw data protocol ID whose format is "xxx-v?", "xxx" indicates the data source, "v?" indicates the version
	/// \~Chinese @brief 通用原始数据表，key为原始数据协议ID，一般格式为"xxx-v?"，xxx表示数据来源，v?表示版本
	typedef Dictionary<Array<SessionGeneralRawData> > SessionGeneralRawDataTable;

	/// \~English @brief Raw data transmit result
	/// \~Chinese @brief 原始数据发送结果
	enum class TransmitResult
	{
		/// \~English @brief Unknown
		/// \~Chinese @brief 未知
		Unknown = 0,

		/// \~English @brief Successfully transmitted
		/// \~Chinese @brief 成功发送
		Transmitted = 1,

		/// \~English @brief Successfully added to the transmitting queue (The actual transmitting may still fail)
		/// \~Chinese @brief 成功添加至发送队列（实际发送时仍有可能失败）
		Added = 2,

		/// \~English @brief Successfully scheduled to transmit (The actual transmitting may still fail)
		/// \~Chinese @brief 成功预约发送（实际发送时仍有可能失败）
		Scheduled = 3,

		/// \~English @brief Not running online mode
		/// \~Chinese @brief 未运行在线模式
		NotRunning = 4,

		/// \~English @brief The protocol of data is supported by no plugin, or no plugin devices are mapped to this channel
		/// \~Chinese @brief 无插件支持该协议数据发送，或无插件设备映射至该通道
		PluginNotFound = 5,

		/// \~English @brief Failed to transmit, or failed to schedule to transmit
		/// \~Chinese @brief 发送失败，或预约发送失败
		Failed = 6,

		/// \~English @brief The guest device or host is not synchronized to the time server
		/// \~Chinese @brief 客机或主机未同步至授时服务器
		NotSync = 7,

		/// \~English @brief Scheduled transmitting is unsupported
		/// \~Chinese @brief 不支持预约发送
		ScheduleUnsupported = 8,

		/// \~English @brief Timestamp out of order
		/// \~Chinese @brief 时间戳乱序
		TimeDisorder = 9,

		/// \~English @brief Invalid channel (should be 1~16 for bus, 0~23 for video)
		/// \~Chinese @brief 无效的通道（总线应为1~16，视频应为0~23）
		InvalidChannel = 10,

		/// \~English @brief Inconsistent with the configured re-injection codec format
		/// \~Chinese @brief 与配置的回注编码格式不一致
		WrongCodec = 11,

		/// \~English @brief Inconsistent with the configured re-injection image size
		/// \~Chinese @brief 与配置的回注视频图像尺寸不一致
		WrongSize = 12,
	};

	/// \~English @brief General raw data transmit interface
	/// \~Chinese @brief 通用原始数据发送接口
	class SPADAS_API IGeneralDataTransmitterX
	{
	public:
		virtual ~IGeneralDataTransmitterX() {};

		/// \~English @brief Transmit data immediately
		/// \~Chinese @brief 立即发送数据
		/// \~English @param protocol Raw data protocol ID whose format is "xxx-v?", "xxx" indicates the data source, "v?" indicates the version
		/// \~Chinese @param protocol 原始数据协议ID，一般格式为"xxx-v?"，xxx表示数据来源，v?表示版本
		/// \~English @param vector Numerical array data
		/// \~Chinese @param vector 数值数组数据
		/// \~English @param binary Binary data
		/// \~Chinese @param binary 二进制数据
		/// \~English @returns Transmit result
		/// \~Chinese @returns 发送结果
		virtual TransmitResult transmitNow(String protocol, Array<Double> vector, Binary binary);

		/// \~English @brief Schedule to transmit data according to server posix time (must be greater than the timestamp of the last frame to transmit with the same protocol)
		/// \~Chinese @brief 指定按服务器Posix时间预约发送视频帧 (必须大于该协议的上一帧预约发送数据的时间戳)
		/// \~English @param protocol Raw data protocol ID whose format is "xxx-v?", "xxx" indicates the data source, "v?" indicates the version
		/// \~Chinese @param protocol 原始数据协议ID，一般格式为"xxx-v?"，xxx表示数据来源，v?表示版本
		/// \~English @param vector Numerical array data
		/// \~Chinese @param vector 数值数组数据
		/// \~English @param binary Binary data
		/// \~Chinese @param binary 二进制数据
		/// \~English @param serverPosix posix time of the time server, in nanoseconds
		/// \~Chinese @param serverPosix 授时服务器Posix时间，单位纳秒
		/// \~English @param tolerance The maximum delayed transmit time allowed, in nanoseconds
		/// \~Chinese @param tolerance 允许的最大延迟发送时间，单位纳秒
		/// \~English @param guestSyncID Guest synchronization ID, based on which it is determined whether the guest device has synchronized with the time server (The format is "xxx.yyy", "xxx" is the plugin type ID, and "yyy" is the guest synchronization channel name)
		/// \~Chinese @param guestSyncID 客机同步ID，将据此确定客机是否已与授时服务器同步（格式为"xxx.yyy"，xxx为插件类型ID，yyy为客机同步通道名称）
		/// \~English @returns Transmit result
		/// \~Chinese @returns 发送结果
		virtual TransmitResult transmitAtServerPosix(String protocol, Array<Double> vector, Binary binary, NanoPosix serverPosix, UInt tolerance, String guestSyncID);
	};

	/// \~English @brief Generic sample element
	/// \~Chinese @brief 通用样本元素
	struct GeneralElement
	{
		/// \~English @brief Is it valid
		/// \~Chinese @brief 是否有效
		Bool valid;

		/// \~English @brief Is it a text element
		/// \~Chinese @brief 是否为文本元素
		Bool isText;

		/// \~English @brief Numerical value
		/// \~Chinese @brief 元素值
		Double value;

		/// \~English @brief Text value
		/// \~Chinese @brief 元素文本
		String text;

		/// \~English @brief Default constructor, create invalid sample element
		/// \~Chinese @brief 默认构造函数，创建无效样本元素
		GeneralElement() : valid(FALSE), isText(FALSE), value(0)
		{}

		/// \~English @brief Create a valid numerical element
		/// \~Chinese @brief 创建有效的值元素
		GeneralElement(Double value) : valid(TRUE), isText(FALSE), value(value)
		{}

		/// \~English @brief Create a valid text element
		/// \~Chinese @brief 创建有效的文本元素
		GeneralElement(String text) : valid(TRUE), isText(TRUE), value(0), text(text)
		{}

		/// \~English @brief Create based on spadas::OptionalBool, converted to 0 or 1
		/// \~Chinese @brief 基于 spadas::OptionalBool 创建，转为0或1
		GeneralElement(OptionalBool value) : valid(value.valid), isText(FALSE), value(value.value ? 1 : 0)
		{}

		/// \~English @brief Create based on spadas::OptionalInt
		/// \~Chinese @brief 基于 spadas::OptionalInt 创建
		GeneralElement(OptionalInt value) : valid(value.valid), isText(FALSE), value((Double)value.value)
		{}

		/// \~English @brief Create based on spadas::OptionalDouble
		/// \~Chinese @brief 基于 spadas::OptionalDouble 创建
		GeneralElement(OptionalDouble value) : valid(value.valid), isText(FALSE), value(value.value)
		{}

		/// \~English @brief Convert to string, if invalid, convert to "(invalid)"
		/// \~Chinese @brief 转字符串，若无效则转为(invalid)
		SPADAS_API String toString();
	};

	/// \~English @brief General sample
	/// \~Chinese @brief 通用样本
	struct SessionGeneralSample
	{
		/// \~English @brief Timestamp
		/// \~Chinese @brief 时间戳
		FullTimestamp timestamp;

		/// \~English @brief Sample element array
		/// \~Chinese @brief 样本数据数组
		Array<GeneralElement> values;

		/// \~English @brief The number of key elements, indicating that the first few in the sample element array are key data, the default is 0
		/// \~Chinese @brief 关键元素个数，表示样本数据数组中前若干个为关键数据，默认为0
		UInt significantCount;

		/// \~English @brief Default constructor
		/// \~Chinese @brief 默认构造函数
		SessionGeneralSample() : significantCount(0)
		{}

		/// \~English @brief Initialize based on timestamp and data
		/// \~Chinese @brief 基于时间戳和数据初始化
		SessionGeneralSample(FullTimestamp timestamp, Array<GeneralElement> values) : timestamp(timestamp), values(values), significantCount(0)
		{}

		/// \~English @brief Initialize based on timestamp, data, and number of key elements
		/// \~Chinese @brief 基于时间戳、数据、关键元素个数初始化
		SessionGeneralSample(FullTimestamp timestamp, Array<GeneralElement> values, UInt significantCount) : timestamp(timestamp), values(values), significantCount(significantCount)
		{}
	};

	/// \~English @brief General sample data table, the key is the sample protocol ID whose format is "xxx-v?" or "xxx-v?@?", "xxx" indicates the sample type, "v?" indicates the version, "@?" indicates the channel index that starts from 0
	/// \~Chinese @brief 通用样本表，key为样本协议ID，一般格式为"xxx-v?"或"xxx-v?@?"，xxx表示样本类型，v?表示版本，@?表示通道序号，序号从0开始
	typedef Dictionary<Array<SessionGeneralSample> > SessionGeneralSampleTable;

	/// \~English @brief General sample interpolation result
	/// \~Chinese @brief 通用样本插值结果
	enum class SampleInterpolationResult
	{
		/// \~English @brief Successful interpolation
		/// \~Chinese @brief 成功插值返回
		OK = 0,

		/// \~English @brief The sample buffer is empty, or the sample range as a whole is later than the interpolation timestamp, or the sample range as a whole is earlier than the interpolation timestamp
		/// \~Chinese @brief 样本缓存为空，或样本范围整体晚于插值时间戳，或样本范围整体过早于插值时间戳
		OutOfRange = 1,

		/// \~English @brief The sample does not support interpolation, directly returns the nearest sample
		/// \~Chinese @brief 样本不支持插值，直接返回最近样本
		NearestInstead = 2,

		/// \~English @brief The sample range is earlier than the interpolation timestamp as a whole, but within the earlyThresh range, it is necessary to wait for new samples before interpolation
		/// \~Chinese @brief 样本范围整体早于插值时间戳，但在阈值earlyThresh范围内，需等待新样本再进行插值
		TooEarly = 3,

		/// \~English @brief General sample parsing error
		/// \~Chinese @brief 通用样本解析错误
		ParseError = 4,

		/// \~English @brief The protocol of the sample buffer is not set. See spadas::SessionSampleBuffer::setProtocol
		/// \~Chinese @brief 未设置样本缓存的协议，详见 spadas::SessionSampleBuffer::setProtocol
		NoProtocol = 5,
	};

	/// \~English @brief General sample buffer
	/// \~Chinese @brief 通用样本缓存
	class SPADAS_API SessionSampleBuffer : public Object<class SessionSampleBufferVars>
	{
	public:
		/// \~English @brief Class name
		/// \~Chinese @brief 类名称
		static const String TypeName;

		/// \~English @brief Create a sample buffer
		/// \~Chinese @brief 创建样本缓存
		SessionSampleBuffer();

		/// \~English @brief Set the maximum buffer duration
		/// \~Chinese @brief 设置缓存最大时长
		/// \~English @param length Maximum duration, in seconds, range 1~60 seconds, 3 seconds as default
		/// \~Chinese @param length 最大时长，单位秒，范围1~60秒，默认3秒
		void setBufferLength(Double length);

		/// \~English @brief Set the corresponding general sample protocol
		/// \~Chinese @brief 设置对应的通用样本协议
		/// \~English @param protocol General sample protocol ID
		/// \~Chinese @param protocol 通用样本协议ID
		void setProtocol(String protocol);

		/// \~English @brief Get the corresponding general sample protocol ID
		/// \~Chinese @brief 获取对应的通用样本协议ID
		/// \~English @param withChannel Whether to carry channel information, that is, "@?" at the end of the protocol ID
		/// \~Chinese @param withChannel 是否带通道信息，即协议ID末尾的"@?"
		/// \~English @returns General sample protocol ID
		/// \~Chinese @returns 通用样本协议ID
		String getProtocol(Bool withChannel);

		/// \~English @brief Add a new sample
		/// \~Chinese @brief 添加新样本
		/// \~English @param sample New general sample
		/// \~Chinese @param sample 新通用样本
		void addSample(SessionGeneralSample sample);

		/// \~English @brief Whether the buffer is empty
		/// \~Chinese @brief 缓存是否为空
		Bool isEmpty();

		/// \~English @brief Get the number of samples in the buffer
		/// \~Chinese @brief 获取缓存中样本个数
		UInt getSampleCount();

		/// \~English @brief Get the identifier of the current session
		/// \~Chinese @brief 获取当前Session的标识符
		SessionIdentifier getCurrentSession();

		/// \~English @brief Get the earliest sample
		/// \~Chinese @brief 获取最早样本
		/// \~English @param sampleEarliest Output the earliest sample
		/// \~Chinese @param sampleEarliest 输出最早样本
		/// \~English @returns FALSE if the sample buffer is empty
		/// \~Chinese @returns 若样本缓存为空则返回FALSE
		Bool getEarliest(SessionGeneralSample& sampleEarliest);

		/// \~English @brief Get the latest sample
		/// \~Chinese @brief 获取最新样本
		/// \~English @param sampleEarliest Output the latest sample
		/// \~Chinese @param sampleEarliest 输出最新样本
		/// \~English @returns FALSE if the sample buffer is empty
		/// \~Chinese @returns 若样本缓存为空则返回FALSE
		Bool getLatest(SessionGeneralSample& sampleLatest);

		/// \~English @brief Find the nearest sample according to the time offset
		/// \~Chinese @brief 根据时间偏置寻找最近样本
		/// \~English @param offset Target time offset
		/// \~Chinese @param offset 目标时间偏置
		/// \~English @param sampleNearest Output the nearest sample to the target time
		/// \~Chinese @param sampleNearest 输出离目标最近样本
		/// \~English @returns FALSE if the sample buffer is empty
		/// \~Chinese @returns 若样本缓存为空，则返回FALSE
		Bool getNearest(Double offset, SessionGeneralSample& sampleNearest);

		/// \~English @brief Find the nearest sample according to the timestamp
		/// \~Chinese @brief 根据时间戳寻找最近样本
		/// \~English @param timeType Session irrelevant time category
		/// \~Chinese @param timeType 时间类别
		/// \~English @param time Target time
		/// \~Chinese @param time 目标时间
		/// \~English @param sampleNearest Output the nearest sample to the target time
		/// \~Chinese @param sampleNearest 输出离目标最近样本
		/// \~English @returns FALSE if the sample buffer is empty
		/// \~Chinese @returns 若样本缓存为空，则返回FALSE
		Bool getNearest(TimeType timeType, ULong time, SessionGeneralSample& sampleNearest);

		/// \~English @brief Find the two samples just before and after the time offset
		/// \~Chinese @brief 根据时间偏置寻找前后两个样本
		/// \~English @param offset Target time offset
		/// \~Chinese @param offset 目标时间偏置
		/// \~English @param sampleBefore Output the sample just before the target time
		/// \~Chinese @param sampleBefore 输出比时间戳早的最近样本
		/// \~English @param sampleAfter Output the sample just after the target time
		/// \~Chinese @param sampleAfter 输出比时间戳晚的最近样本
		/// \~English @returns FALSE if none
		/// \~Chinese @returns 若无则返回FALSE
		Bool search(Double offset, SessionGeneralSample& sampleBefore, SessionGeneralSample& sampleAfter);

		/// \~English @brief Find the two samples just before and after the timestamp
		/// \~Chinese @brief 根据时间戳寻找前后两个样本
		/// \~English @param timeType Session irrelevant time category
		/// \~Chinese @param timeType 时间类别
		/// \~English @param time Target time
		/// \~Chinese @param time 目标时间
		/// \~English @param sampleBefore Output the sample just before the target time
		/// \~Chinese @param sampleBefore 输出比时间戳早的最近样本
		/// \~English @param sampleAfter Output the sample just after the target time
		/// \~Chinese @param sampleAfter 输出比时间戳晚的最近样本
		/// \~English @returns FALSE if none
		/// \~Chinese @returns 若无则返回FALSE
		Bool search(TimeType timeType, ULong time, SessionGeneralSample& sampleBefore, SessionGeneralSample& sampleAfter);

		/// \~English @brief Find the two samples just before and after the time offset, then perform interpolation
		/// \~Chinese @brief 根据时间偏置寻找前后两个样本并插值
		/// \~English @param offset Target time offset
		/// \~Chinese @param offset 目标时间偏置
		/// \~English @param interpolatedSample Output interpolated samples
		/// \~Chinese @param interpolatedSample 输出插值完成的样本
		/// \~English @param earlyThresh The threshold used to determine whether the buffer range is too early, refer to SampleInterpolationResult::TooEarly
		/// \~Chinese @param earlyThresh 用于判断缓存范围是否过早的阈值，参考 SampleInterpolationResult::TooEarly
		/// \~English @details The template type must implement the following functions: \n
		/// \~Chinese @details 该模板类型必须实现以下函数： \n
		/// \~ - Bool fromGeneralSample(String protocol, SessionGeneralSample); \n
		/// \~ - static Bool supportInterpolation(); \n
		/// \~ - static Type interpolate(Type& s1, Double w1, Type& s2, Double w2, FullTimestamp timestamp);
		template <typename Type>
		SampleInterpolationResult interpolate(Double offset, Type& interpolatedSample, UInt earlyThresh = 1000/* ms */);

	private:
		Bool isNull() { return FALSE; }
		Bool isValid() { return FALSE; }
	};

	/// \~English @brief General sample buffer table, the key is the sample protocol ID whose format is "xxx-v?" or "xxx-v?@?", "xxx" indicates the sample type, "v?" indicates the version, "@?" indicates the channel number that starts from 0
	/// \~Chinese @brief 通用样本缓存表，key为样本协议ID，一般格式为"xxx-v?"或"xxx-v?@?"，xxx表示样本类型，v?表示版本，@?表示通道序号，序号从0开始
	typedef Dictionary<SessionSampleBuffer> SessionSampleBufferTable;

	/// \~English @brief Matrix sample
	/// \~Chinese @brief 矩阵样本
	struct SessionMatrixSample
	{
		/// \~English @brief Timestamp
		/// \~Chinese @brief 时间戳
		FullTimestamp timestamp;

		/// \~English @brief Matrix data, stored in the order of rows and columns. For example, the first element is the first row and the first column, the second element is the first row and the second column,...
		/// \~Chinese @brief 矩阵数据，按行、列的顺序存储，如第0元素为第一行第一列，第1元素为第一行第二列，...
		Array<Float> matData;

		/// \~English @brief Number of rows
		/// \~Chinese @brief 行数
		UInt rows;

		/// \~English @brief Number of columns
		/// \~Chinese @brief 列数
		UInt cols;

		/// \~English @brief Default constructor
		/// \~Chinese @brief 默认构造函数
		SessionMatrixSample() : rows(0), cols(0)
		{}

		/// \~English @brief Initialize based on matrix size
		/// \~Chinese @brief 基于矩阵尺寸初始化
		SessionMatrixSample(Size2D size)
		{
			matData = Array<Float>(size.dim0 * size.dim1);
			rows = size.dim0;
			cols = size.dim1;
		}
	};

	/// \~English @brief Matrix sample data table, the key is the sample protocol ID whose format is "xxx-v?" or "xxx-v?@?", "xxx" indicates the sample type, "v?" indicates the version, "@?" indicates the channel number that starts from 0
	/// \~Chinese @brief 矩阵样本表，key为样本协议ID，一般格式为"xxx-v?"或"xxx-v?@?"，xxx表示样本类型，v?表示版本，@?表示通道序号，序号从0开始
	typedef Dictionary<Array<SessionMatrixSample> > SessionMatrixSampleTable;

	/// \~English @brief General device status
	/// \~Chinese @brief 一般设备状态
	enum class GeneralDeviceStatus
	{
		/// \~English @brief Not set to auto-connect mode
		/// \~Chinese @brief 未设定为自动连接模式
		NotConnect = 0,

		/// \~English @brief The connection is normal
		/// \~Chinese @brief 连接正常
		OK = 1,

		/// \~English @brief Connection error
		/// \~Chinese @brief 连接错误
		Error = 2,

		/// \~English @brief There is no error in the connection, but there are certain problems
		/// \~Chinese @brief 连接无错误，但存在一定问题
		Warning = 3,
	};

	/// \~English @brief Bus channel type
	/// \~Chinese @brief 总线通道类型
	enum class BusChannelType
	{
		/// \~English @brief Invalid type
		/// \~Chinese @brief 无效类型
		Invalid = 0,

		/// \~English @brief CAN bus, message data is payload, 1~8 bytes
		/// \~Chinese @brief CAN总线，报文数据即payload，1～8字节
		Can = 1,

		/// \~English @brief CAN-FD bus, message data is payload, 1~64 bytes
		/// \~Chinese @brief CAN-FD总线，报文数据即payload，1～64字节
		CanFD = 2,

		/// \~English @brief LIN bus, message data is payload, 1~8 bytes
		/// \~Chinese @brief LIN总线，报文数据即payload，1～8字节
		Lin = 3,

		/// \~English @brief Flexray bus, message data is composed of flag byte (startup, sync, null from low to high), cycle byte and payload, totally 2~256 bytes, and message ID is Slot ID
		/// \~Chinese @brief Flexray总线，报文数据由标志位字节(从低至高为startup,sync,null)、cycle字节和payload构成(共2～256字节)，报文ID即Slot ID
		Flexray = 4,

		/// \~English @brief Ethernet bus, the message data is the complete Ethernet frame data including the link layer and other protocols, and the message ID is defined as the last four bytes (little endian) of the source MAC
		/// \~Chinese @brief 以太网总线，报文数据为包含链路层等等协议的完整以太网帧数据，报文ID定义为源MAC的后四字节(小字序)
		Ethernet = 5,

		/// \~English @brief SOME/IP automobile Ethernet bus, the message data is the complete Ethernet frame data including the link layer and other protocols, and the message ID is the "SOME/IP Message ID" (composed of Service ID and Method ID)
		/// \~Chinese @brief SOME/IP车载以太网总线，报文数据为包含链路层等等协议的完整以太网帧数据，报文ID即Message ID(由Service ID和Method ID组成)
		SomeIP = 6,
	};

	/// \~English @brief Bus baud rate
	/// \~Chinese @brief 总线波特率
	enum class BusBaudRate
	{
		Invalid = 0,

		CAN_5k = 1,
		CAN_10k = 2,
		CAN_20k = 3,
		CAN_33k = 18,
		CAN_40k = 4,
		CAN_50k = 5,
		CAN_62k = 6,
		CAN_80k = 7,
		CAN_83k = 8,
		CAN_100k = 9,
		CAN_125k = 10,
		CAN_200k = 11,
		CAN_250k = 12,
		CAN_400k = 13,
		CAN_500k = 14, // default
		CAN_666k = 15,
		CAN_800k = 16,
		CAN_1M = 17,

		CANFD_500k = 101,
		CANFD_1M = 102,
		CANFD_2M = 103, // default
		CANFD_4M = 104,
		CANFD_8M = 105,
		CANFD_6M = 106,
		CANFD_2d5M = 107,
		CANFD_5M = 108,

		LIN_1k = 201,
		LIN_9d6k = 202, // default
		LIN_16d5k = 203,
		LIN_19d2k = 204,
		LIN_20k = 205,

		FR_2d5M = 301,
		FR_5M = 302,
		FR_10M = 303, // default
	};

	/// \~English @brief Raw data from bus device
	/// \~Chinese @brief 总线设备输出原始数据
	struct BusDeviceData
	{
		/// \~English @brief CPU tick when data arrives
		/// \~Chinese @brief 到达主机时的CPU计数
		ULong cpuTick;

		/// \~English @brief The time offset on the bus receiver when data arrives, in seconds, 0 means invalid
		/// \~Chinese @brief 到达总线设备时其上的时间偏置，单位秒，0表示无效
		Double bridgeTimeOffset;

		/// \~English @brief The guest posix time on the bus device when data arrives, in nanoseconds, 0 means invalid
		/// \~Chinese @brief 到达总线设备时其上的Posix时间，单位纳秒，0表示无效
		NanoPosix bridgeGuestPosix;

		/// \~English @brief Bus channel, 1~16
		/// \~Chinese @brief 总线通道，1~16
		UInt channel;

		/// \~English @brief Message ID in this channel
		/// \~Chinese @brief 该通道内的报文ID
		UInt id;

		/// \~English @brief Message data
		/// \~Chinese @brief 报文数据
		Binary binary;

		/// \~English @brief Default constructor
		/// \~Chinese @brief 默认构造函数
		BusDeviceData() : cpuTick(0), bridgeTimeOffset(0), bridgeGuestPosix(0), channel(0), id(0)
		{}
	};

	/// \~English @brief Bus raw data
	/// \~Chinese @brief 总线原始数据
	struct SessionBusRawData
	{
		/// \~English @brief Timestamp
		/// \~Chinese @brief 时间戳
		ShortTimestamp timestamp;

		/// \~English @brief Bus channel, 1~16
		/// \~Chinese @brief 总线通道，1~16
		UInt channel;

		/// \~English @brief Message ID in this channel
		/// \~Chinese @brief 该通道内的报文ID
		UInt id;

		/// \~English @brief Message data
		/// \~Chinese @brief 报文数据
		Binary binary;

		/// \~English @brief Default constructor
		/// \~Chinese @brief 默认构造函数
		SessionBusRawData() : channel(0), id(0)
		{}
	};

	/// \~English @brief Bus raw data table (The length is 16, representing bus channels 1~16 respectively)
	/// \~Chinese @brief 总线原始数据表（长度16, 分别表示总线通道1~16）
	typedef Array<Array<SessionBusRawData> > SessionBusRawDataTable;

	/// \~English @brief Bus raw data transmit interface
	/// \~Chinese @brief 总线报文发送接口
	class SPADAS_API IBusMessageTransmitterX
	{
	public:
		virtual ~IBusMessageTransmitterX() {};

		/// \~English @brief Transmit data immediately
		/// \~Chinese @brief 立即发送报文
		/// \~English @param channel Bus channel, 1~16
		/// \~Chinese @param channel 总线通道，1~16
		/// \~English @param id Message ID in this channel
		/// \~Chinese @param id 该通道内的报文ID
		/// \~English @param binary Message data
		/// \~Chinese @param binary 报文数据
		/// \~English @returns Transmit result
		/// \~Chinese @returns 发送结果
		virtual TransmitResult transmitNow(UInt channel, UInt id, Binary binary);

		/// \~English @brief Set to transmit message repeatedly
		/// \~Chinese @brief 设定重复发送报文
		/// \~English @param channel Bus channel, 1~16
		/// \~Chinese @param channel 总线通道，1~16
		/// \~English @param id Message ID in this channel
		/// \~Chinese @param id 该通道内的报文ID
		/// \~English @param binary Message data
		/// \~Chinese @param binary 报文数据
		/// \~English @param interval Transmitting period [ms], effective range is 10~1000ms
		/// \~Chinese @param interval 发送周期[ms]，有效范围10~1000ms
		/// \~English @returns Transmit result
		/// \~Chinese @returns 发送结果
		virtual TransmitResult transmitRepeatedly(UInt channel, UInt id, Binary binary, UInt interval);

		/// \~English @brief Schedule to transmit data according to server posix time (must be greater than the timestamp of the last frame to transmit on the same channel)
		/// \~Chinese @brief 指定按授时服务器Posix时间预约发送报文 (必须大于该通道上一帧预约发送报文的时间)
		/// \~English @param channel Bus channel, 1~16
		/// \~Chinese @param channel 总线通道，1~16
		/// \~English @param id Message ID in this channel
		/// \~Chinese @param id 该通道内的报文ID
		/// \~English @param binary Message data
		/// \~Chinese @param binary 报文数据
		/// \~English @param serverPosix posix time of the time server, in nanoseconds
		/// \~Chinese @param serverPosix 授时服务器Posix时间，单位纳秒
		/// \~English @param tolerance The maximum delayed transmit time allowed, in nanoseconds
		/// \~Chinese @param tolerance 允许的最大延迟发送时间，单位纳秒
		/// \~English @returns Transmit result
		/// \~Chinese @returns 发送结果
		virtual TransmitResult transmitAtServerPosix(UInt channel, UInt id, Binary binary, NanoPosix serverPosix, UInt tolerance);
	};

	/// \~English @brief Bus device ID
	/// \~Chinese @brief 总线设备ID
	struct BusDeviceID
	{
		/// \~English @brief Plugin type ID
		/// \~Chinese @brief 插件类型ID
		String type;

		/// \~English @brief Device serial number
		/// \~Chinese @brief 设备序列号
		ULong deviceSerial;

		/// \~English @brief Device channel index, starting from 0
		/// \~Chinese @brief 设备通道序号，从0开始
		UInt deviceChannelIndex;

		/// \~English @brief Default constructor
		/// \~Chinese @brief 默认构造函数
		BusDeviceID() : deviceSerial(0), deviceChannelIndex(0)
		{}
	};

	/// \~English @brief Bus device information
	/// \~Chinese @brief 总线设备信息
	struct BusDeviceInfoX
	{
		/// \~English @brief Bus device ID
		/// \~Chinese @brief 总线设备ID
		BusDeviceID id;

		/// \~English @brief The bus types supported by this channel
		/// \~Chinese @brief 该通道支持的总线类型
		Array<BusChannelType> types;

		/// \~English @brief Device channel description
		/// \~Chinese @brief 设备通道描述
		String description;

		/// \~English @brief Whether it supports scheduled transmitting
		/// \~Chinese @brief 是否支持按授时服务器Posix时间预约发送报文
		Bool supportTransmitScheduled;

		/// \~English @brief Default constructor
		/// \~Chinese @brief 默认构造函数
		BusDeviceInfoX() : supportTransmitScheduled(FALSE)
		{}
	};

	/// \~English @brief Bus device configuration
	/// \~Chinese @brief 总线设备配置
	struct BusDeviceConfig
	{
		/// \~English @brief Bus device ID
		/// \~Chinese @brief 总线设备ID
		BusDeviceID id;

		/// \~English @brief The bus type of the channel
		/// \~Chinese @brief 该通道的总线类型
		BusChannelType type;

		/// \~English @brief Main baud rate
		/// \~Chinese @brief 主波特率
		BusBaudRate rate;

		/// \~English @brief Secondary baud rate. Currently only used for BusChannelType::CanFD , indicating the arbitration bit-rate
		/// \~Chinese @brief 副波特率。目前仅用于 BusChannelType::CanFD ，表示arbitration bit-rate
		BusBaudRate rate2;

		/// \~English @brief Software channel mapping, 1~16
		/// \~Chinese @brief 软件通道映射，1~16
		UInt mapChannel;

		/// \~English @brief Default constructor
		/// \~Chinese @brief 默认构造函数
		BusDeviceConfig() : type(BusChannelType::Invalid), rate(BusBaudRate::Invalid), rate2(BusBaudRate::Invalid), mapChannel(0)
		{}
	};

	/// \~English @brief Bus data load
	/// \~Chinese @brief 总线数据负载
	struct BusChannelPayload
	{
		/// \~English @brief Software channel, 1~16
		/// \~Chinese @brief 软件通道，1~16
		UInt mapChannel;

		/// \~English @brief Data load, unit %
		/// \~Chinese @brief 数据负载，单位%
		Float payloadPercent;

		/// \~English @brief Default constructor
		/// \~Chinese @brief 默认构造函数
		BusChannelPayload() : mapChannel(0), payloadPercent(0)
		{}
	};

	/// \~English @brief Bus message information
	/// \~Chinese @brief 总线报文信息
	struct BusMessageInfo
	{
		/// \~English @brief Software channel
		/// \~Chinese @brief 软件通道
		UInt channel;

		/// \~English @brief Message ID in this channel
		/// \~Chinese @brief 报文本通道ID
		UInt localID;

		/// \~English @brief Default constructor
		/// \~Chinese @brief 默认构造函数
		BusMessageInfo() : channel(0), localID(0)
		{}

		/// \~English @brief Convert to a message information identifier, the upper 32 bits are the software channel, and the lower 32 bits are the message ID
		/// \~Chinese @brief 转换为报文信息标识符，高32位为软件通道，低32位为本通道ID
		SPADAS_API ULong indicator();
	};

	/// \~English @brief Bus message
	/// \~Chinese @brief 总线报文
	struct SessionBusMessage
	{
		/// \~English @brief Timestamp
		/// \~Chinese @brief 时间戳
		ShortTimestamp timestamp;

		/// \~English @brief Message information
		/// \~Chinese @brief 报文信息
		BusMessageInfo info;

		/// \~English @brief Data
		/// \~Chinese @brief 数据
		Binary data;

		/// \~English @brief Default constructor
		/// \~Chinese @brief 默认构造函数
		SessionBusMessage()
		{}

		/// \~English @brief Initialize based on information, timestamp and data
		/// \~Chinese @brief 基于信息、时间戳和数据初始化
		SessionBusMessage(ShortTimestamp timestamp, BusMessageInfo info, Binary data) : timestamp(timestamp), info(info), data(data)
		{}
	};

	/// \~English @brief Bus message data table, the key is the global ID of the message whose format is "protocol file name:message ID in this channel", such as vehicle.dbc:123
	/// \~Chinese @brief 总线报文数据表，key为报文全局ID，格式为"协议文件名:本通道ID"，如vehicle.dbc:123
	typedef Dictionary<Array<SessionBusMessage> > SessionBusMessageTable;

	/// \~English @brief Video stream codec type
	/// \~Chinese @brief 视频数据流编码方式
	enum class VideoDataCodec
	{
		/// \~English @brief Invalid
		/// \~Chinese @brief 无效
		Invalid = 0,

		/// \~English @brief MJPEG: Lossy encoding, independent frames
		/// \~Chinese @brief MJPEG：有损编码，帧间独立
		MJPEG = 1,

		/// \~English @brief H.264: Lossy encoding, inter-frame dependence
		/// \~Chinese @brief H.264：有损编码，帧间依赖
		H264 = 2,

		/// \~English @brief YUV411: Lossless encoding, independent frames, the format is (U0 Y0 V0 Y1 U4 Y2 V4 Y3 Y4 Y5 Y6 Y7) per 8 pixels, 8 bits per value
		/// \~Chinese @brief YUV411：无损编码，帧间独立，格式为每8像素(U0 Y0 V0 Y1 U4 Y2 V4 Y3 Y4 Y5 Y6 Y7)，每数值8bit
		YUV411 = 3,

		/// \~English @brief YUV420: Lossless encoding, independent frames, the format is (U V Y00 Y01 Y10 Y11) per 2x2 pixels, 8 bits per value
		/// \~Chinese @brief YUV420：无损编码，帧间独立，格式为每2x2像素(U V Y00 Y01 Y10 Y11)，每数值8bit
		YUV420 = 4,

		/// \~English @brief H.265: Lossy encoding, inter-frame dependence
		/// \~Chinese @brief H.265：有损编码，帧间依赖
		H265 = 5,

		/// \~English @brief YUV422: Lossless encoding, independent frames, the format is (Y0 U Y1 V) per 2 pixels , 8 bits per value
		/// \~Chinese @brief YUV422：无损编码，帧间独立，格式为每2像素(Y0 U Y1 V)，每数值8bit
		YUV422 = 6,

		/// \~English @brief RAW: Lossless encoding, independent frames, the format is BG on 024... rows, GR on 135... rows, 8 bits per value
		/// \~Chinese @brief RAW：无损编码，帧间独立，格式为024...行BG，135...行GR，每数值8bit
		RAW = 7,

		/// \~English @brief RAW12: Lossless encoding, independent frames, the format is BG on 024... rows, GR on 135... rows, 12 bits per value
		/// \~Chinese @brief RAW12：无损编码，帧间独立，格式为024...行BG，135...行GR，每数值12bit按小字序依次存储
		RAW12 = 8,

		/// \~English @brief RAW14: Lossless encoding, independent frames, the format is BG on 024... rows, GR on 135... rows, 14 bits per value
		/// \~Chinese @brief RAW14：无损编码，帧间独立，格式为024...行BG，135...行GR，每数值14bit按小字序依次存储
		RAW14 = 9,

		/// \~English @brief RAW16: Lossless encoding, independent frames, the format is BG on 024... rows, GR on 135... rows, 16 bits per value
		/// \~Chinese @brief RAW16：无损编码，帧间独立，格式为024...行BG，135...行GR，每数值16bit按大字序依次存储
		RAW16 = 10,

		/// \~English @brief Y16: Lossless encoding, independent frames, 16 bits (big endian) per value
		/// \~Chinese @brief Y16：无损编码，帧间独立，每数值16bit按大字序依次存储
		Y16 = 11,
	};

	/// \~English @brief Video stream input mode
	/// \~Chinese @brief 视频数据流输入模式
	struct VideoInputMode
	{
		/// \~English @brief Image size
		/// \~Chinese @brief 图像大小
		Size2D size;

		/// \~English @brief Video codec
		/// \~Chinese @brief 编码方式
		VideoDataCodec codec;

		/// \~English @brief Default constructor
		/// \~Chinese @brief 默认构造函数
		VideoInputMode() : codec(VideoDataCodec::Invalid)
		{}
	};

	/// \~English @brief Video stream re-injection mode
	/// \~Chinese @brief 视频数据流回注模式
	struct VideoOutputMode
	{
		/// \~English @brief Image size
		/// \~Chinese @brief 图像大小
		Size2D size;

		/// \~English @brief Video codec
		/// \~Chinese @brief 编码方式
		VideoDataCodec codec;

		/// \~English @brief Default constructor
		/// \~Chinese @brief 默认构造函数
		VideoOutputMode() : codec(VideoDataCodec::Invalid)
		{}
	};

	/// \~English @brief Raw data from video device
	/// \~Chinese @brief 视频设备输出原始数据
	struct VideoDeviceData
	{
		/// \~English @brief CPU tick when data arrives
		/// \~Chinese @brief 到达主机时的CPU计数
		ULong cpuTick;

		/// \~English @brief Guest posix time at data sampling, in nanoseconds, 0 means invalid
		/// \~Chinese @brief 采样时客机Posix时间，单位纳秒，0表示无效
		NanoPosix guestPosix;

		/// \~English @brief Satellite posix time at data sampling, in nanoseconds, 0 means invalid
		/// \~Chinese @brief 采样时卫星Posix时间，单位纳秒，0表示无效
		NanoPosix gnssPosix;

		/// \~English @brief Video channel, 0~23, corresponding to A~X
		/// \~Chinese @brief 视频通道，0~23，对应A~X
		UInt channel;

		/// \~English @brief Video stream input mode
		/// \~Chinese @brief 视频数据流输入模式
		VideoInputMode inputMode;

		/// \~English @brief The data of the original frame of the video
		/// \~Chinese @brief 视频原始帧的数据
		Binary data;

		/// \~English @brief Whether it includes a preview image
		/// \~Chinese @brief 是否包含预览图像
		Bool hasPreview;

		/// \~English @brief Preview image, BGR image with 640x(360-480) resolution
		/// \~Chinese @brief 预览图像，640x(360-480)分辨率的BGR图像
		ImagePointer preview;

		/// \~English @brief Default constructor
		/// \~Chinese @brief 默认构造函数
		VideoDeviceData() : cpuTick(0), guestPosix(0), gnssPosix(0), channel(0), hasPreview(FALSE)
		{}
	};

	/// \~English @brief Video raw data
	/// \~Chinese @brief 视频原始数据
	struct SessionVideoRawData
	{
		/// \~English @brief Timestamp
		/// \~Chinese @brief 时间戳
		FullTimestamp timestamp;

		/// \~English @brief Video channel, 0~23, corresponding to A~X
		/// \~Chinese @brief 视频通道，0~23，对应A~X
		UInt channel;

		/// \~English @brief Video stream input mode
		/// \~Chinese @brief 视频数据流输入模式
		VideoInputMode inputMode;

		/// \~English @brief The data of the original frame of the video
		/// \~Chinese @brief 视频原始帧的数据
		Binary data;

		/// \~English @brief Whether it includes a preview image
		/// \~Chinese @brief 是否包含预览图像
		Bool hasPreview;

		/// \~English @brief Preview image, BGR image with 640x(360-480) resolution
		/// \~Chinese @brief 预览图像，640x(360-480)分辨率的BGR图像
		ImagePointer preview;

		/// \~English @brief Default constructor
		/// \~Chinese @brief 默认构造函数
		SessionVideoRawData() : channel(0), hasPreview(FALSE)
		{}
	};

	/// \~English @brief Video raw data table (The length is 24, representing video channels A~X respectively)
	/// \~Chinese @brief 视频原始数据表（长度24, 分别表示视频通道A~X）
	typedef Array<Array<SessionVideoRawData> > SessionVideoRawDataTable;

	/// \~English @brief Video device ID
	/// \~Chinese @brief 视频设备ID
	struct VideoDeviceID
	{
		/// \~English @brief Plugin type ID
		/// \~Chinese @brief 插件类型ID
		String type;

		/// \~English @brief Device ID
		/// \~Chinese @brief 设备标识ID
		String localID;
	};

	/// \~English @brief Preview image support capability
	/// \~Chinese @brief 预览图像支持能力
	enum class VideoPreviewCapability
	{
		/// \~English @brief No preview image
		/// \~Chinese @brief 无预览图像
		NoPreview = 0,

		/// \~English @brief Preview image can be output together with the spadas::VideoDeviceData
		/// \~Chinese @brief 支持预览图像随 spadas::VideoDeviceData 一起输出
		WithPreview = 1,

		/// \~English @brief Besides the capability of WithPreview, preview image can also be output directly through spadas::IVideoPreviewExpressX
		/// \~Chinese @brief 在WithPreview的基础上，还支持通过 spadas::IVideoPreviewExpressX 直接输出
		PreviewExpress = 2,
	};

	/// \~English @brief Video device information
	/// \~Chinese @brief 视频设备信息
	struct VideoDeviceInfoX2
	{
		/// \~English @brief Video device ID
		/// \~Chinese @brief 视频设备ID
		VideoDeviceID id;

		/// \~English @brief Video device channel description
		/// \~Chinese @brief 视频设备通道描述
		String description;

		/// \~English @brief The video stream input modes supported by the video device channel
		/// \~Chinese @brief 该视频设备通道支持的视频数据流输入模式
		Array<VideoInputMode> inputModes;

		/// \~English @brief Preview image support capability for input video stream
		/// \~Chinese @brief 输入视频数据流的预览图像支持能力
		VideoPreviewCapability previewCapability;

		/// \~English @brief The video stream re-injection modes supported by the video device channel
		/// \~Chinese @brief 该视频设备通道支持的视频数据流回注模式
		Array<VideoOutputMode> outputModes;

		/// \~English @brief Whether it supports scheduled transmitting
		/// \~Chinese @brief 是否支持按授时服务器Posix时间预约发送视频帧
		Bool supportTransmitScheduled;

		/// \~English @brief Default constructor
		/// \~Chinese @brief 默认构造函数
		VideoDeviceInfoX2() : previewCapability(VideoPreviewCapability::NoPreview), supportTransmitScheduled(FALSE)
		{}
	};

	/// \~English @brief Video device configuration
	/// \~Chinese @brief 视频设备配置
	struct VideoDeviceConfigX
	{
		/// \~English @brief Video device ID
		/// \~Chinese @brief 视频设备ID
		VideoDeviceID id;

		/// \~English @brief Software channel mapping, 0~23, corresponding to A~X
		/// \~Chinese @brief 软件通道映射，0~23，对应A~X
		UInt mapChannel;

		/// \~English @brief The video stream input mode of this channel
		/// \~Chinese @brief 该通道的视频数据流输入模式
		VideoInputMode inputMode;

		/// \~English @brief The video stream re-injection mode of this channel
		/// \~Chinese @brief 该通道的视频数据流回注模式
		VideoOutputMode outputMode;

		/// \~English @brief Input reference frame rate, 10~120Hz
		/// \~Chinese @brief 输入参考帧率，10~120Hz
		UInt frameRate;

		/// \~English @brief Default constructor
		/// \~Chinese @brief 默认构造函数
		VideoDeviceConfigX() : mapChannel(0), frameRate(0)
		{}
	};

	/// \~English @brief Video data for image processing
	/// \~Chinese @brief 用于数据处理的视频数据
	struct SessionVideoProcData
	{
		/// \~English @brief Timestamp
		/// \~Chinese @brief 时间戳
		FullTimestamp timestamp;

		/// \~English @brief Software channel mapping, 0~23, corresponding to A~X
		/// \~Chinese @brief 软件通道，0~23，对应A~X
		UInt channel;

		/// \~English @brief Data for image processing
		/// \~Chinese @brief 用于数据处理的图像数据
		ImagePointer data;

		/// \~English @brief The meta protocol of the image
		/// \~Chinese @brief 图像的meta协议
		String metaProtocol;

		/// \~English @brief Meta information of the image
		/// \~Chinese @brief 图像的meta信息
		Array<Double> meta;

		/// \~English @brief Default constructor
		/// \~Chinese @brief 默认构造函数
		SessionVideoProcData() : channel(0)
		{}
	};

	/// \~English @brief Video data table for image processing (The length is 24, representing video channels A~X respectively)
	/// \~Chinese @brief 用于数据处理的视频数据表（长度24, 分别表示视频通道A~X）
	typedef Array<Array<SessionVideoProcData> > SessionVideoProcDataTable;

	/// \~English @brief Express output interface of video preview image
	/// \~Chinese @brief 视频预览图像的快速输出接口
	class SPADAS_API IVideoPreviewExpressX
	{
	public:
		virtual ~IVideoPreviewExpressX() {};

		/// \~English @brief Output preview image
		/// \~Chinese @brief 输出预览图像
		/// \~English @param cpuTick CPU tick when data arrives
		/// \~Chinese @param cpuTick 视频帧的到达时CPU计数
		/// \~English @param channel Video channel, 0~23, corresponding to A~X
		/// \~Chinese @param channel 视频通道，0~23，对应A~X
		/// \~English @param preview Preview image, BGR image with 640x(360-480) resolution
		/// \~Chinese @param preview 预览图像，640x(360-480)分辨率的BGR图像
		/// \~English @param guestPosix Guest posix time at data sampling, in nanoseconds, 0 means invalid
		/// \~Chinese @param guestPosix 视频帧的采样时客机Posix时间，单位纳秒，0表示无效
		/// \~English @param gnssPosix Satellite posix time at data sampling, in nanoseconds, 0 means invalid
		/// \~Chinese @param gnssPosix 视频帧的采样时卫星Posix时间，单位纳秒，0表示无效
		virtual void outputPreview(ULong cpuTick, UInt channel, ImagePointer preview, NanoPosix guestPosix = 0, NanoPosix gnssPosix = 0);
	};

	/// \~English @brief Video raw data transmit interface
	/// \~Chinese @brief 视频帧回注接口
	class SPADAS_API IVideoFrameTransmitterX
	{
	public:
		virtual ~IVideoFrameTransmitterX() {};

		/// \~English @brief Transmit data immediately
		/// \~Chinese @brief 立即发送视频帧
		/// \~English @param channel Video channel, 0~23
		/// \~Chinese @param channel 视频通道，0~23
		/// \~English @param codec Video codec
		/// \~Chinese @param codec 视频帧的编码格式
		/// \~English @param size The size of the video frame, in pixels
		/// \~Chinese @param size 视频帧的大小，像素单位
		/// \~English @param data Video frame data
		/// \~Chinese @param data 视频帧数据
		/// \~English @returns Transmit result
		/// \~Chinese @returns 发送结果
		virtual TransmitResult transmitNow(UInt channel, VideoDataCodec codec, Size2D size, Binary data);

		/// \~English @brief Schedule to transmit data according to server posix time (must be greater than the timestamp of the last frame to transmit on the same channel)
		/// \~Chinese @brief 指定按服务器Posix时间预约发送视频帧 (必须大于该通道上一帧预约发送报文的时间)
		/// \~English @param channel Video channel, 0~23
		/// \~Chinese @param channel 视频通道，0~23
		/// \~English @param codec Video codec
		/// \~Chinese @param codec 视频帧的编码格式
		/// \~English @param size The size of the video frame, in pixels
		/// \~Chinese @param size 视频帧的大小，像素单位
		/// \~English @param data Video frame data
		/// \~Chinese @param data 视频帧数据
		/// \~English @param serverPosix posix time of the time server, in nanoseconds
		/// \~Chinese @param serverPosix 授时服务器Posix时间，单位纳秒
		/// \~English @param tolerance The maximum delayed transmit time allowed, in nanoseconds
		/// \~Chinese @param tolerance 允许的最大延迟发送时间，单位纳秒
		/// \~English @returns Transmit result
		/// \~Chinese @returns 发送结果
		virtual TransmitResult transmitAtServerPosix(UInt channel, VideoDataCodec codec, Size2D size, Binary data, NanoPosix serverPosix, UInt tolerance);
	};

	/// \~English @brief All input data tables
	/// \~Chinese @brief 所有输入数据表
	struct InputTablesX
	{
		/// \~English @brief Generic raw data table
		/// \~Chinese @brief 通用原始数据表
		SessionGeneralRawDataTable rawDatas;

		/// \~English @brief Bus raw data table
		/// \~Chinese @brief 总线原始数据表
		SessionBusRawDataTable busRawDatas;

		/// \~English @brief Video raw data table
		/// \~Chinese @brief 视频原始数据表
		SessionVideoRawDataTable videoRawDatas;

		/// \~English @brief Video data table for image processing
		/// \~Chinese @brief 用于数据处理的视频数据表
		SessionVideoProcDataTable videoProcDatas;

		/// \~English @brief Bus message data table
		/// \~Chinese @brief 总线报文数据表
		SessionBusMessageTable busMessages;

		/// \~English @brief Signal data table
		/// \~Chinese @brief 信号数据表
		SessionSignalTable signals;

		/// \~English @brief General sample data table
		/// \~Chinese @brief 样本数据表
		SessionGeneralSampleTable samples;

		/// \~English @brief Matrix sample data table
		/// \~Chinese @brief 矩阵数据表
		SessionMatrixSampleTable matrices;

		/// \~English @brief Default constructor
		/// \~Chinese @brief 默认构造函数
		InputTablesX() : busRawDatas(BC_NUM), videoRawDatas(VC_NUM), videoProcDatas(VC_NUM)
		{}

		/// \~English @brief Clear all data
		/// \~Chinese @brief 清空所有数据
		inline void clear()
		{
			rawDatas.clear();
			busMessages.clear();
			signals.clear();
			samples.clear();
			matrices.clear();
			busRawDatas = SessionBusRawDataTable(BC_NUM);
			videoRawDatas = SessionVideoRawDataTable(VC_NUM);
			videoProcDatas = SessionVideoProcDataTable(VC_NUM);
		}
	};

	/// \~English @brief All output data tables
	/// \~Chinese @brief 所有输出数据表
	struct OutputTablesX
	{
		/// \~English @brief Signal data table
		/// \~Chinese @brief 信号数据表
		SessionSignalTable signals;

		/// \~English @brief General sample data table
		/// \~Chinese @brief 样本数据表
		SessionGeneralSampleTable samples;

		/// \~English @brief Matrix sample data table
		/// \~Chinese @brief 矩阵数据表
		SessionMatrixSampleTable matrices;

		/// \~English @brief Default constructor
		/// \~Chinese @brief 默认构造函数
		OutputTablesX()
		{}

		/// \~English @brief Clear all data
		/// \~Chinese @brief 清空所有数据
		inline void clear()
		{
			signals.clear();
			samples.clear();
			matrices.clear();
		}
	};

	/// \~English @brief The state of standalone task
	/// \~Chinese @brief 独立处理任务的状态
	enum class StandaloneTaskState
	{
		/// \~English @brief The task has not started yet
		/// \~Chinese @brief 任务还未开始
		Unknown = 0,

		/// \~English @brief Mission completed
		/// \~Chinese @brief 任务完成
		Finished = 1,

		/// \~English @brief The task failed or was aborted
		/// \~Chinese @brief 任务失败或被中止
		Failed = 2,

		/// \~English @brief Task in progress
		/// \~Chinese @brief 任务进行中
		Progressing = 3,
	};

	/// \~English @brief Feedback interface for standalone task
	/// \~Chinese @brief 独立处理任务的反馈接口
	class SPADAS_API IStandaloneTaskCallback
	{
	public:
		virtual ~IStandaloneTaskCallback() {};

		/// \~English @brief Set task status and progress
		/// \~Chinese @brief 设置任务处理状态和进度
		/// \~English @param state Task status
		/// \~Chinese @param state 任务状态
		/// \~English @param description The text describing the completion / failure / abort / progressing status, empty means disabled
		/// \~Chinese @param description 描述完成状态、失败或中止状态、任务进行状态的文本，空表示不启用
		/// \~English @param progress Task progress, unit %, which only needs to be set when the state is StandaloneTaskState::Progressing
		/// \~Chinese @param progress 任务进度，单位%，仅当状态为 StandaloneTaskState::Progressing 时需要设置
		virtual void setTaskProgress(StandaloneTaskState state, String description, Double progress = 0);

		/// \~English @brief Set the return value of standalone task
		/// \~Chinese @brief 设置任务返回值信息
		virtual void setTaskReturnValue(String value);
	};

	/// \~English @brief Data picking parameters
	/// \~Chinese @brief 数据截取任务参数
	struct PickConfig
	{
		/// \~English @brief The range (time offset) of the source session to pick
		/// \~Chinese @brief 截取源Session的时间偏置范围
		Range srcRange;

		/// \~English @brief The identifier of the output session (compatible with older versions, equivalent to SessionIdentifier)
		/// \~Chinese @brief 目标Session的标识符 (与旧版本兼容，等效于SessionIdentifier)
		Time dstSession;

		/// \~English @brief The input folder path of the output session
		/// \~Chinese @brief 目标Session的input文件夹路径
		Path dstInputRoot;

		/// \~English @brief The generation folder path of the output Session
		/// \~Chinese @brief 目标Session的generation文件夹路径
		Path dstGenerationRoot;
	};

	/// \~English @brief File writing mode
	/// \~Chinese @brief 文件写入模式
	enum class FileWriteMode
	{
		/// \~English @brief No special mode
		/// \~Chinese @brief 无特殊模式
		Normal = 0,

		/// \~English @brief Online mode
		/// \~Chinese @brief 采集模式
		OnlineMode = 1,

		/// \~English @brief Offline processing mode
		/// \~Chinese @brief 离线处理模式
		OfflineMode = 2,
	};

	/// \~English @brief File reading mode
	/// \~Chinese @brief 文件读取模式
	enum class FileReadMode
	{
		/// \~English @brief No special mode
		/// \~Chinese @brief 无特殊模式
		Normal = 0,

		/// \~English @brief In offline processing or offline replay mode, reading from raw data
		/// \~Chinese @brief 离线处理或离线回放模式下，从原始数据读取
		FromRaw = 1,

		/// \~English @brief In offline processing mode, reading from generation data
		/// \~Chinese @brief 离线处理模式下，从generation读取
		OfflineModeFromGeneration = 2,

		/// \~English @brief In offline replay mode, reading from generation data
		/// \~Chinese @brief 离线回放模式下，从generation读取
		ReplayModeFromGeneration = 3,

		/// \~English @brief In the remote mode, reading from the remote host
		/// \~Chinese @brief 远程采集模式下，从远程主机读取
		FromRemote = 4,
	};

	/// \~English @brief File R/W filter
	/// \~Chinese @brief 文件读写筛选
	enum class FileIOFilter
	{
		/// \~English @brief Signal data
		/// \~Chinese @brief 信号数据
		Signal = 1,

		/// \~English @brief General sample data
		/// \~Chinese @brief 样本数据
		Sample = 2,

		/// \~English @brief Matrix sample data
		/// \~Chinese @brief 矩阵数据
		Matrix = 3,

		/// \~English @brief Bus channel 1 data, channel 2 and other data are sequentially added from 101 upwards
		/// \~Chinese @brief 总线通道1数据，通道2等数据依次从101往上加
		BusCH1 = 100,

		/// \~English @brief Video channel A data, channel B and other data are sequentially added from 201 upwards
		/// \~Chinese @brief 视频通道A数据，通道B等数据依次从201往上加
		VideoChannelA = 200,
	};

	/// \~English @brief File R/W basic information
	/// \~Chinese @brief 文件读写基本信息
	struct FileIOBasicInfo
	{
		/// \~English @brief Session identifier (compatible with older versions, equivalent to SessionIdentifier)
		/// \~Chinese @brief Session标识符 (与旧版本兼容，等效于SessionIdentifier)
		Time session;

		/// \~English @brief Filter items, empty means no filter
		/// \~Chinese @brief 筛选项，空表示不进行筛选
		Array<FileIOFilter> filter;

		/// \~English @brief File writing mode
		/// \~Chinese @brief 文件写入模式
		FileWriteMode writeMode;

		/// \~English @brief File reading mode
		/// \~Chinese @brief 文件读取模式
		FileReadMode readMode;

		/// \~English @brief Data password (unnecessary for non-encrypted files)
		/// \~Chinese @brief 数据密码（非加密文件无需使用）
		String password;

		/// \~English @brief Default constructor
		/// \~Chinese @brief 默认构造函数
		FileIOBasicInfo() : writeMode(FileWriteMode::Normal), readMode(FileReadMode::Normal)
		{}
	};

	/// \~English @brief File R/W extended information
	/// \~Chinese @brief 文件读写扩展信息
	struct FileIOExtInfo
	{
		/// \~English @brief Type of each bus channel, empty means no bus data reading or writing
		/// \~Chinese @brief 各总线通道类型，空表示无总线数据读写
		Array<BusChannelType> busChannelTypes;

		/// \~English @brief Bus message name table, key is the bus message ID
		/// \~Chinese @brief 总线报文名称表，key为总线报文ID字符串
		Dictionary<String> busMessageNameTable;

		/// \~English @brief The input mode of each video channel, empty means no video data reading or writing
		/// \~Chinese @brief 各视频通道输入模式，空表示无视频数据读写
		Array<VideoInputMode> videoChannelModes;

		/// \~English @brief The frame rate of each video channel to write to the file, empty means that the frame rate is not considered
		/// \~Chinese @brief 各视频通道写入文件的帧率，空表示不考虑帧率
		Array<UInt> videoChannelFPS;

		/// \~English @brief Whether each video channel is aligned to the actual frame rate according to the frame rate of the written file (only for writing), empty means not aligned
		/// \~Chinese @brief 各视频通道是否按写入文件帧率对齐实际帧率（仅用于写入），空表示不对齐
		Array<Bool> videoChannelFPSAligned;

		/// \~English @brief Sample title table, key is sample protocol ID
		/// \~Chinese @brief 样本标题表，key为样本协议ID
		Dictionary<String> sampleTitles;
	};

	/// \~English @brief Cross-module data sending interface
	/// \~Chinese @brief 跨模块数据发送接口
	class SPADAS_API ICrossTransmitter
	{
	public:
		virtual ~ICrossTransmitter() {}

		/// \~English @brief Send to the application layer module
		/// \~Chinese @brief 发送至应用层模块
		/// \~English @param id Data ID
		/// \~Chinese @param id 数据ID
		/// \~English @param data Data content, can be empty
		/// \~Chinese @param data 数据内容，可为空
		virtual void sendToApp(String id, Binary data);

		/// \~English @brief Send to the native layer module
		/// \~Chinese @brief 发送至原生层模块
		/// \~English @param pluginType Target plugin type ID
		/// \~Chinese @param pluginType 模块的插件类型ID
		/// \~English @param id Data ID
		/// \~Chinese @param id 数据ID
		/// \~English @param data Data content, can be empty
		/// \~Chinese @param data 数据内容，可为空
		/// \~English @returns Returns FALSE if no module matching the specified plugin type ID is found
		/// \~Chinese @returns 若未找到符合指定插件类型ID的模块则返回FALSE
		virtual Bool sendToNative(String pluginType, String id, Binary data);
	};

	/// \~English @brief Cross-module function call interface
	/// \~Chinese @brief 跨模块函数调用接口
	class SPADAS_API ICrossCaller
	{
	public:
		virtual ~ICrossCaller() {}

		/// \~English @brief Call the application layer function
		/// \~Chinese @brief 调用应用层函数
		/// \~English @param id Function ID, which can be used to distinguish different functions
		/// \~Chinese @param id 调用ID，可用于区分不同功能或函数
		/// \~English @param input Input data
		/// \~Chinese @param input 输入数据
		/// \~English @param output Output data
		/// \~Chinese @param output 输出数据
		/// \~English @returns Returns FALSE if it is not responded by the application layer program
		/// \~Chinese @returns 若未被应用层程序响应则返回FALSE
		virtual Bool callAppFunction(String id, Binary input, Binary& output);

		/// \~English @brief Call the native layer function
		/// \~Chinese @brief 调用原生层函数
		/// \~English @param pluginType Target plugin type ID
		/// \~Chinese @param pluginType 模块的插件类型ID
		/// \~English @param id Function ID, which can be used to distinguish different functions
		/// \~Chinese @param id 调用ID，可用于区分不同功能或函数
		/// \~English @param context Context, which stores input and output and temporary variables, etc.
		/// \~Chinese @param context 调用上下文，存储输入输出和临时变量等
		/// \~English @returns Returns FALSE if no module matching the specified plugin type ID is found, or is not responded by any module
		/// \~Chinese @returns 若未找到符合制定插件类型ID的模块，或未被任何模块响应则返回FALSE
		virtual Bool callNativeFunction(String pluginType, String id, BaseObject context);
	};

	/// \~English @brief Time related service interface
	/// \~Chinese @brief 时间相关服务接口
	class SPADAS_API ITimeServer
	{
	public:
		virtual ~ITimeServer() {}

		/// \~English @brief Create a timestamp (the priority of calculating the time offset from high to low is: time server posix time, satellite posix time, host posix time at arrival, CPU tick at arrival)
		/// \~Chinese @brief 创建时间戳（计算时间偏置的优先级从高至低为：授时服务器Posix时间、卫星Posix时间、到达时主机Posix时间、到达时CPU计数）
		/// \~English @param outputTimestamp Output timestamp
		/// \~Chinese @param outputTimestamp 输出的时间戳
		/// \~English @param session The identifier of the timestamp's related session
		/// \~Chinese @param session 时间戳所在Session的标识符
		/// \~English @param cpuTick CPU tick when data arrives, 0 means invalid
		/// \~Chinese @param cpuTick 到达时CPU计数，0表示无效
		/// \~English @param hostPosix Host posix time when data arrives, in nanoseconds, 0 means invalid
		/// \~Chinese @param hostPosix 到达时主机Posix时间，单位纳秒，0表示无效
		/// \~English @param guestPosix Guest posix time at data sampling, in nanoseconds, 0 means invalid
		/// \~Chinese @param guestPosix 客机Posix时间，单位纳秒，0表示无效
		/// \~English @param gnssPosix Satellite posix time at data sampling, in nanoseconds, 0 means invalid
		/// \~Chinese @param gnssPosix 卫星Posix时间，单位纳秒，0表示无效
		/// \~English @param guestSyncID Guest synchronization ID, based on which it is determined whether the guest device has synchronized with the time server (The format is "xxx.yyy", "xxx" is the plugin type ID, and "yyy" is the guest synchronization channel name)
		/// \~Chinese @param guestSyncID 客机同步ID，将据此确定客机是否已与授时服务器同步（视频通道格式为video@x，x为通道序号0~23；插件相关通道格式为"xxx.yyy"，xxx为插件类型ID，yyy为客机同步通道名称）
		/// \~English @returns Whether successfully created
		/// \~Chinese @returns 是否成功
		virtual Bool createTimestamp(FullTimestamp& outputTimestamp, SessionIdentifier session, ULong cpuTick = 0, NanoPosix hostPosix = 0, NanoPosix guestPosix = 0, NanoPosix gnssPosix = 0, String guestSyncID = String());

		/// \~English @brief Perform re-synchronization based on the reference timestamp (the priority of recalculating the time offset is from high to low: time server posix time, satellite posix time)
		/// \~Chinese @brief 根据基准时间戳进行二次同步（重新计算时间偏置的优先级从高至低为：授时服务器Posix时间、卫星Posix时间）
		/// \~English @param srcTimestamp Base timestamp
		/// \~Chinese @param srcTimestamp 基准时间戳
		/// \~English @param guestPosix If it is not 0, use this input as the guest posix time of the reference timestamp, in nanoseconds
		/// \~Chinese @param guestPosix 非0则使用该输入作为基准时间戳的客机Posix时间，单位纳秒
		/// \~English @param gnssPosix If it is not 0, use this input as the satellite posix time of the reference timestamp, in nanoseconds
		/// \~Chinese @param gnssPosix 非0则使用该输入作为基准时间戳的卫星Posix时间，单位纳秒
		/// \~English @param guestSyncID Guest synchronization ID, based on which it is determined whether the guest device has synchronized with the time server (The format is "xxx.yyy", "xxx" is the plugin type ID, and "yyy" is the guest synchronization channel name)
		/// \~Chinese @param guestSyncID 客机同步ID，将据此确定客机是否已与授时服务器同步（视频通道格式为video@x，x为通道序号0~23；插件相关通道格式为"xxx.yyy"，xxx为插件类型ID，yyy为客机同步通道名称）
		/// \~English @returns Output timestamp
		/// \~Chinese @returns 输出的时间戳
		virtual FullTimestamp resyncTimestamp(FullTimestamp srcTimestamp, NanoPosix guestPosix = 0, NanoPosix gnssPosix = 0, String guestSyncID = String());

		/// \~English @brief Recalculate session-irrelevant time such as CPU tick, host posix time, time server posix time, satellite posix time, based on the input time offset and related session
		/// \~Chinese @brief 根据基准时间戳的时间偏置反算CPU计数、主机Posix时间、授时服务器Posix时间、卫星Posix时间等
		/// \~English @param srcTimestamp Input simple timestamp
		/// \~Chinese @param srcTimestamp 基准时间戳
		/// \~English @param timeType Time type of output. spadas::TimeType::GuestPosix is unsupported
		/// \~Chinese @param timeType 输出的时间类型，不支持 spadas::TimeType::GuestPosix
		/// \~English @returns The session-irrelevant time, 0 means failure
		/// \~Chinese @returns 输出的Session无关时间，0表示失败
		virtual ULong calcTime(ShortTimestamp srcTimestamp, TimeType timeType);
	};

	// Plugin related utility functions / 插件相关实用功能 //////////////////////////////////////////////////////////////

	/// \~English @brief Bus protocol ID (like xxx.dbc)
	/// \~Chinese @brief 总线协议ID (形如XXX.dbc)
	typedef String BusProtocolID;

	/// \~English @brief Bus message ID
	/// \~Chinese @brief 总线报文ID
	struct BusMessageID
	{
		/// \~English @brief Bus protocol ID
		/// \~Chinese @brief 总线协议ID
		BusProtocolID protocol;

		/// \~English @brief Message ID in this channel
		/// \~Chinese @brief 本通道报文ID
		UInt message;

		/// \~English @brief Bus message ID string
		/// \~Chinese @brief 总线报文ID字符串
		String text;

		/// \~English @brief Default constructor
		/// \~Chinese @brief 默认构造函数
		SPADAS_API BusMessageID();

		/// \~English @brief Initialize with the bus protocol ID and the message ID in this channel
		/// \~Chinese @brief 按总线协议ID和本通道报文ID初始化
		SPADAS_API BusMessageID(BusProtocolID protocol, UInt message);

		/// \~English @brief Initialize from the bus message ID string
		/// \~Chinese @brief 按总线报文ID字符串初始化
		SPADAS_API BusMessageID(String text);
	};

	/// \~English @brief Bus signal ID
	/// \~Chinese @brief 总线信号ID
	struct BusSignalID
	{
		/// \~English @brief Bus protocol ID
		/// \~Chinese @brief 总线协议ID
		BusProtocolID protocol;

		/// \~English @brief Message ID in this channel
		/// \~Chinese @brief 本通道报文ID
		UInt message;

		/// \~English @brief Signal name
		/// \~Chinese @brief 信号名称
		String signal;

		/// \~English @brief Bus signal ID string
		/// \~Chinese @brief 总线信号ID字符串
		String text;

		/// \~English @brief Default constructor
		/// \~Chinese @brief 默认构造函数
		SPADAS_API BusSignalID();

		/// \~English @brief Initialize with the bus protocol ID, the message ID in this channel, and the signal name
		/// \~Chinese @brief 按总线协议ID，本通道报文ID，以及信号名称初始化
		SPADAS_API BusSignalID(BusProtocolID protocol, UInt message, String signal);

		/// \~English @brief Initialize from bus signal ID string
		/// \~Chinese @brief 按总线信号ID字符串初始化
		SPADAS_API BusSignalID(String text);
	};

	/// \~English @brief Configuration for signal with both numerical signal and sign bit signal
	/// \~Chinese @brief 带符号位信号配置
	struct BusSignedSignalConfig
	{
		/// \~English @brief Numerical signal ID string
		/// \~Chinese @brief 数值信号ID字符串
		String valueSignalID;

		/// \~English @brief Sign bit signal ID string
		/// \~Chinese @brief 符号位信号ID字符串
		String signSignalID;

		/// \~English @brief Whether it indicates a positive value when the sign bit signal value is 0 
		/// \~Chinese @brief 符号位信号值为0时是否表示正值
		Bool positiveAsZero;

		/// \~English @brief Default constructor
		/// \~Chinese @brief 默认构造函数
		SPADAS_API BusSignedSignalConfig();

		/// \~English @brief Is sign bit signal not configured
		/// \~Chinese @brief 符号位信号配置是否有效
		SPADAS_API Bool isSignOptional();
	};

	/// \~English @brief OptionalDouble with sign bit
	/// \~Chinese @brief 带符号OptionalDouble
	struct OptionalSignedDouble
	{
		/// \~English @brief Sign bit
		/// \~Chinese @brief 符号位
		Bool positive;

		/// \~English @brief Positive floating point value (including 0)
		/// \~Chinese @brief 正浮点值（含0）
		Double value;

		/// \~English @brief Whether the sign bit is valid
		/// \~Chinese @brief 符号位是否有效
		Bool signValid;

		/// \~English @brief Weather the positive floating point value is valid
		/// \~Chinese @brief 正浮点值是否有效
		Bool valueValid;

		/// \~English @brief Default constructor, all invalid
		/// \~Chinese @brief 默认构造函数，初始化为无效值
		OptionalSignedDouble() : positive(TRUE), value(0), signValid(FALSE), valueValid(FALSE)
		{}

		/// \~English @brief Set positive floating point value
		/// \~Chinese @brief 设置正浮点值
		SPADAS_API void setValue(Double value);

		/// \~English @brief Set the sign bit
		/// \~Chinese @brief 设置符号位
		SPADAS_API void setSign(Bool positive);

		/// \~English @brief Convert to spadas::OptionalDouble
		/// \~Chinese @brief 转为 spadas::OptionalDouble 类型
		/// \~English @param signOptional Whether the sign bit is optional, if it is FALSE, the sign bit must be valid to return a valid value
		/// \~Chinese @param signOptional 符号位是否可选，若为FALSE则符号位必需有效才能返回有效值
		/// \~English @returns Return spadas::OptionalDouble value
		/// \~Chinese @returns 返回的 spadas::OptionalDouble 类型数值
		SPADAS_API OptionalDouble toOptionalDouble(Bool signOptional);
	};

	/// \~English @brief General raw data output interface
	/// \~Chinese @brief 通用原始数据输出接口
	class SPADAS_API IGeneralDeviceDataOutput
	{
	public:
		virtual ~IGeneralDeviceDataOutput() {};

		/// \~English @brief Output data of general device
		/// \~Chinese @brief 输出一般设备原始数据
		/// \~English @param data Data of general device
		/// \~Chinese @param data 一般设备原始数据
		virtual void outputGeneralDeviceData(GeneralDeviceData data);
	};

	/// \~English @brief Timestamp search interface
	/// \~Chinese @brief 时间戳搜索接口
	class SPADAS_API ITimestampSearch
	{
	public:
		virtual ~ITimestampSearch() {};

		/// \~English @brief Search the next frame of data from the current position of the file
		/// \~Chinese @brief 以文件当前位置搜索下一帧数据
		/// \~English @param file File object
		/// \~Chinese @param file 数据文件对象
		/// \~English @param time Output the timestamp of the next frame of data
		/// \~Chinese @param time 输出下一帧数据的时间戳
		/// \~English @param pos Output the position in file of next frame data
		/// \~Chinese @param pos 输出下一帧数据的开始位置
		/// \~English @returns Whether the search is successful, if there is no data, return FALSE
		/// \~Chinese @returns 搜索是否成功，若无数据返回FALSE
		virtual Bool readNextTimestamp(File file, Double& time, ULong& pos);

		/// \~English @brief [Optional] Get the tail search range, the default is Range(100, 10000)
		/// \~Chinese @brief [可选] 获取尾部搜索范围，默认为Range(100, 10000)
		virtual Range getTailSearchRange();
	};

	/// \~English @brief Timestamp search
	/// \~Chinese @brief 时间戳搜索
	class SPADAS_API TimestampSearch
	{
	public:
		/// \~English @brief Search the file according to the timestamp, jump to the location, so that the next frame of data is the earliest frame whose timestamp is not less than the specified timestamp
		/// \~Chinese @brief 按时间戳搜索数据文件，跳转文件位置，使得下一帧数据为时间戳不小于指定时间戳的最早帧
		/// \~English @param path Data file path
		/// \~Chinese @param path 数据文件路径
		/// \~English @param time Target timestamp
		/// \~Chinese @param time 目标时间戳
		/// \~English @param searcher Search interface
		/// \~Chinese @param searcher 搜索接口
		/// \~English @returns File object, if there is no data or the timestamp of all data is less than the target, an invalid object will be returned
		/// \~Chinese @returns 数据文件对象，若无数据或所有数据的时间戳都小于目标则返回无效对象
		static File search(Path path, Double time, Interface<ITimestampSearch> searcher);

		/// \~English @brief Get the timestamp range in the data file
		/// \~Chinese @brief 获取数据文件中的时间戳范围
		/// \~English @param path Data file path
		/// \~Chinese @param path 数据文件路径
		/// \~English @param searcher Search interface
		/// \~Chinese @param searcher 搜索接口
		/// \~English @returns Timestamp range
		/// \~Chinese @returns 时间戳范围
		static Optional<Range> getTimeRange(Path path, Interface<ITimestampSearch> searcher);
	};

	/// \~English @brief Interface for constructing and deconstructing data in spadas::FlexVars
	/// \~Chinese @brief 用于在 spadas::FlexVars 中构建和析构数据的接口
	class SPADAS_API IFlexHandler
	{
	public:
		virtual ~IFlexHandler() {};
		virtual void createData(Pointer data);
		virtual void destroyData(Pointer data);
	};

	/// \~English @brief Support variables with different definitions in multiple modules
	/// \~Chinese @brief 支持在多个模块中使用不同定义的变量数据基类
	template <typename Type> class FlexVars : public Vars
	{
	public:
		/// \~English @brief Initialization, input the index list of supported fields under the current definition (starting from 0)
		/// \~Chinese @brief 初始化，输入当前定义下支持字段的序号列表（从0开始）
		FlexVars(Array<Int> validFlagIndices);

		/// \~English @brief Destructor (No need to implement a destructor for derived class)
		/// \~Chinese @brief 析构函数(子类无需实现析构函数)
		~FlexVars();

		/// \~English @brief Is it valid
		/// \~Chinese @brief 是否有效
		Bool valid();

		/// \~English @brief Whether the definition of variables supports the field with the specified index
		/// \~Chinese @brief 此数据是否支持指定序号的字段
		Bool has(Int flagIndex);

		/// \~English @brief [Unsafe operation] [Modifiable] Obtain the field reference under the current definition (The "has" method should be called before using the field to see whether it is supported)
		/// \~Chinese @brief [非安全操作] [可修改] 获取当前定义下的数据类型引用（使用字段前应先调用has方法确定是否支持）
		Type& cast();

	private:
		Binary data;
		Array<ULong> flags;
		Interface<IFlexHandler> handler;
	};

	/// \~English @brief General function IO object
	/// \~Chinese @brief 通用的函数IO用对象
	class SPADAS_API GeneralIOObject : public Object<class GeneralIOObjectVars>
	{
	public:
		/// \~English @brief Class name
		/// \~Chinese @brief 类名称
		static const String TypeName;

		/// \~English @brief Create object
		/// \~Chinese @brief 创建对象
		GeneralIOObject();

		/// \~English @brief Get input data
		/// \~Chinese @brief 获取输入数据
		Binary getInput();

		/// \~English @brief Set input data
		/// \~Chinese @brief 设置输入数据
		void setInput(Binary data);

		/// \~English @brief Get output data
		/// \~Chinese @brief 获取输出数据
		Binary getOutput();

		/// \~English @brief Set the output data
		/// \~Chinese @brief 设置输出数据
		void setOutput(Binary data);

	private:
		Bool isNull() { return FALSE; }
		Bool isValid() { return FALSE; }
	};

	/// \~English @brief Object container for raw data exchange
	/// \~Chinese @brief 用于原始数据交换的对象容器
	class SPADAS_API GeneralRawObject : public Object<class GeneralRawObjectVars>
	{
	public:
		/// \~English @brief Class name
		/// \~Chinese @brief 类名称
		static const String TypeName;

		/// \~English @brief Invalid object
		/// \~Chinese @brief 无效对象
		GeneralRawObject();

		/// \~English @brief Create by object
		/// \~Chinese @brief 基于对象创建
		GeneralRawObject(BaseObject obj);

		/// \~English @brief Create by general raw data
		/// \~Chinese @brief 基于通用原始数据创建
		GeneralRawObject(SessionGeneralRawData data);

		/// \~English @brief Get the object inside
		/// \~Chinese @brief 获取内部的对象
		BaseObject getObject();

		/// \~English @brief Convert to general device data
		/// \~Chinese @brief 转换为一般设备原始数据
		GeneralDeviceData toGeneralDeviceData(ULong cpuTick, String protocol);

		/// \~English @brief Convert to general raw data
		/// \~Chinese @brief 转换为通用原始数据
		SessionGeneralRawData toSessionGeneralRawData(FullTimestamp timestamp);
	};

	// Plugin API / 插件API //////////////////////////////////////////////////////////////

	/// \~English @brief General function plugin interface 1.6
	/// \~Chinese @brief 通用功能插件接口 1.6
	class SPADAS_API IPluginV106
	{
	public:
		virtual ~IPluginV106() {};

		/// \~English @brief Get the plugin type ID
		/// \~Chinese @brief 获取插件类型ID
		/// \~English @returns The plugin type ID
		/// \~Chinese @returns 插件类型ID
		virtual String getPluginType();

		/// \~English @brief Get plugin version
		/// \~Chinese @brief 获取插件版本
		/// \~English @returns Plugin version, the format is x.x.x, respectively representing the major version, minor version, and build version. Generally speaking, the major version corresponds to the architecture update, the minor version corresponds to the function update, and the build version corresponds to the bug fix.
		/// \~Chinese @returns 插件版本，格式为x.x.x，分别表示主版本、次版本、build版本。一般而言主版本对应架构更新，次版本对应功能更新，build版本对应bug修复
		virtual String getPluginVersion();

		/// \~English @brief [Optional] Called before exiting the application, used to stop the background thread, etc.
		/// \~Chinese @brief [可选] 在程序结束前被调用，用于停止背景线程等
		virtual void closePlugin();

		/// \~English @brief [Optional] Get copyright notices of the third party softwares the plugin uses
		/// \~Chinese @brief [可选] 取得插件使用的第三方软件的版权声明
		/// \~English @returns Key is title, value is copyright notice
		/// \~Chinese @returns 键为标题，值为版权声明
		virtual Dictionary<String> getThirdPartyNotices();

		/// \~English @brief [Optional] Called when the locale config is being initialized
		/// \~Chinese @brief [可选] 在初始化本土化配置时被调用
		/// \~English @param languageCode Language code, "en" means English, "ch" means Chinese
		/// \~Chinese @param languageCode 语言代号，"en"表示英文，"ch"表示中文
		/// \~English @param preferPRCWeb Prefer to use web service in the People's Republic of China
		/// \~Chinese @param preferPRCWeb 使用国内的网络服务
		virtual void initLocaleInfo(String languageCode, Bool preferPRCWeb);

		/// \~English @brief [Optional] Set to use the specified interface to log debugging information (mainly used to call spadas::console::useThreadLogger in child threads)
		/// \~Chinese @brief [可选] 设置使用指定的接口记录调试信息（主要用于在子线程中调用 spadas::console::useThreadLogger ）
		/// \~English @param logger Interface for recording debugging information
		/// \~Chinese @param logger 记录调试信息接口
		virtual void useLogger(Interface<ILogger> logger);

		/// \~English @brief [Optional] Set the root path where the application's external files (such as plugin library files, etc. are stored in the "plugin" subfolder)
		/// \~Chinese @brief [可选] 设置应用程序外部文件(如插件库文件等在plugin子文件夹)存放的根路径
		/// \~English @param path The root path where the external files of the application are stored
		/// \~Chinese @param path 应用程序外部文件存放的根路径
		virtual void setAppFilesPath(Path path);

		/// \~English @brief [Optional] Called when data sent by other modules is received (Ensure that this should be handled at the microsecond level)
		/// \~Chinese @brief [可选] 在收到其他模块发送的数据时被调用（应确保微秒级别的运行时间）
		/// \~English @param id Data ID
		/// \~Chinese @param id 数据ID
		/// \~English @param data Data content, can be empty
		/// \~Chinese @param data 数据内容，可为空
		virtual void onCrossData(String id, Binary data);

		/// \~English @brief [Optional] Set to use the specified cross-module data sending interface
		/// \~Chinese @brief [可选] 设置使用指定的跨模块数据发送接口
		/// \~English @param transmitter Cross-module data sending interface
		/// \~Chinese @param transmitter 跨模块数据发送接口
		virtual void useCrossTransmitter(Interface<ICrossTransmitter> transmitter);

		/// \~English @brief [Optional] Called when receiving a function request from other modules (should ensure millisecond-level running time, longer processing should be implemented through independent tasks)
		/// \~Chinese @brief [可选] 在收到其他模块调用函数请求时被调用（应确保毫秒级别的运行时间，更长的处理应通过独立任务实现）
		/// \~English @param id Function ID, which can be used to distinguish different functions
		/// \~Chinese @param id 调用ID，可用于区分不同功能或函数
		/// \~English @param context Context, which stores input and output and temporary variables, etc.
		/// \~Chinese @param context 调用上下文，存储输入输出和临时变量等
		/// \~English @returns Whether it has responded
		/// \~Chinese @returns 是否已响应
		virtual Bool onCrossCall(String id, BaseObject context);

		/// \~English @brief [Optional] Set to use the specified cross-module function call interface
		/// \~Chinese @brief [可选] 设置使用指定的跨模块函数调用接口
		/// \~English @param caller Cross-module function call interface
		/// \~Chinese @param caller 跨模块函数调用接口
		virtual void useCrossCaller(Interface<ICrossCaller> caller);

		/// \~English @brief [Optional] Called while starting an online session
		/// \~Chinese @brief [可选] 在开始在线模式Session时被调用
		/// \~English @param session The identifier of the online mode session
		/// \~Chinese @param session 该在线模式Session的标识符
		/// \~English @param startCPUTick The CPU tick at when the online mode session starts
		/// \~Chinese @param startCPUTick 该在线模式Session启动时的基准CPU计数
		virtual void onStartOnlineSession(SessionIdentifier session, ULong startCPUTick);

		/// \~English @brief [Optional] Called when the session in online mode ends
		/// \~Chinese @brief [可选] 在结束在线模式Session时被调用
		virtual void onStopOnlineSession();

		/// \~English @brief [Optional] Get a list of supported standalone task names
		/// \~Chinese @brief [可选] 获取支持的独立任务名称列表
		/// \~English @returns Supported standalone task name list
		/// \~Chinese @returns 支持的独立任务名称列表
		virtual Array<String> getStandaloneTaskNames();

		/// \~English @brief [Optional] Start an standalone task (usually only called while idle)
		/// \~Chinese @brief [可选] 启动独立的处理任务（一般仅在非Session时段被调用）
		/// \~English @param taskName Task name, used to distinguish different tasks
		/// \~Chinese @param taskName 任务名称，用于区分不同任务
		/// \~English @param config The configuration string of this task
		/// \~Chinese @param config 该任务的配置信息
		/// \~English @param shouldEnd Whether the task should be terminated, usually from user operation
		/// \~Chinese @param shouldEnd 任务是否应该中止，一般来自用户操作
		/// \~English @param callback The interface used to feedback task processing status, progress, and return value
		/// \~Chinese @param callback 用于反馈任务处理状态、进度，以及返回值的接口
		virtual void runStandaloneTask(String taskName, String config, Flag shouldEnd, Interface<IStandaloneTaskCallback> callback);

		/// \~English @brief [Optional] Get the name list of guest synchronization channels
		/// \~Chinese @brief [可选] 获取客机同步通道名称列表
		/// \~English @returns Guest synchronization channel name list
		/// \~Chinese @returns 客机同步通道名称列表
		virtual Array<String> getGuestSyncChannelNames();
	};

	/// \~English @brief Function definition of getting the general function plugin interface, the function name should be get_plugin_v106
	/// \~Chinese @brief 获取通用功能插件接口的全局函数定义，函数名应为get_plugin_v106
	typedef Interface<IPluginV106>(*GetPluginV106)();

	/// \~English @brief Latest version of general function plugin interface
	/// \~Chinese @brief 最新版本通用功能插件接口
	typedef IPluginV106 IPluginLatest;

	/// \~English @brief General device plugin interface 2.2
	/// \~Chinese @brief 一般设备插件接口 2.2
	class SPADAS_API IDevicePluginV202
	{
	public:
		virtual ~IDevicePluginV202() {};

		/// \~English @brief Configure the device, connect, disconnect, or reconnect according to the configuration (only called while idle)
		/// \~Chinese @brief 配置设备，在函数内部根据配置实现连接、断开、或重连（仅在非Session时段被调用）
		/// \~English @param config Configuration string, which includes the configuration of whether to connect the device
		/// \~Chinese @param config 配置信息，应包含是否连接设备的配置
		virtual void setDeviceConnection(String config);

		/// \~English @brief Disconnect (called before the application exited)
		/// \~Chinese @brief 断开连接（在程序关闭前被调用）
		virtual void disconnectDevice();

		/// \~English @brief Get device connection status
		/// \~Chinese @brief 获取设备连接状态
		/// \~English @param info Output device connection information
		/// \~Chinese @param info 输出设备连接信息
		/// \~English @returns Return device status information
		/// \~Chinese @returns 返回设备状态信息
		virtual GeneralDeviceStatus getDeviceStatus(String& info);

		/// \~English @brief [Optional] Get the connection status of each sub-device
		/// \~Chinese @brief [可选] 获取各子设备的连接状态
		/// \~English @returns The connection status of each sub-device, the length of the array is the number of sub-device
		/// \~Chinese @returns 返回各子设备的连接状态，数组长度即子设备数量
		virtual Array<GeneralDeviceStatus> getChildDeviceStatus();

		/// \~English @brief [Optional] Get the new data of the device
		/// \~Chinese @brief [可选] 获取设备新数据
		/// \~English @returns All the newly acquired data
		/// \~Chinese @returns 返回新获取的数据
		virtual Array<GeneralDeviceData> getDeviceNewData();

		/// \~English @brief [Optional] Get the list of data protocols that can be transmitted by transmitGeneralData
		/// \~Chinese @brief [可选] 获取可通过transmitGeneralData立即发送的数据协议列表
		/// \~English @returns The list of data protocols
		/// \~Chinese @returns 可发送的数据协议列表
		virtual Array<String> getTransmittableProtocols();

		/// \~English @brief [Optional] Get the list of data protocols that can be scheduled transmitted by transmitGeneralDataScheduled
		/// \~Chinese @brief [可选] 获取可通过transmitGeneralDataScheduled预约发送的数据协议列表
		/// \~English @returns The list of data protocols
		/// \~Chinese @returns 可发送的数据协议列表
		virtual Array<String> getScheduledTransmittableProtocols();

		/// \~English @brief [Optional] Immediately transmit a frame of data
		/// \~Chinese @brief [可选] 立即发送一帧数据
		/// \~English @param protocol Raw data protocol ID whose format is "xxx-v?", "xxx" indicates the data source, "v?" indicates the version
		/// \~Chinese @param protocol 原始数据协议ID，一般格式为"xxx-v?"，xxx表示数据来源，v?表示版本
		/// \~English @param vector Numerical array data
		/// \~Chinese @param vector 数值数组数据
		/// \~English @param binary Binary data
		/// \~Chinese @param binary 二进制数据
		/// \~English @returns Returns whether a frame of data is successfully transmitted, FALSE if the protocol is not in the list
		/// \~Chinese @returns 返回是否成功发送一帧数据，若协议未在可发送的协议列表内则返回FALSE
		virtual Bool transmitGeneralData(String protocol, Array<Double> vector, Binary binary);

		/// \~English @brief [Optional] Schedule to transmit a frame of data (the timestamps of the same protocol are guaranteed to be monotonically increasing)
		/// \~Chinese @brief [可选] 预约发送一帧数据（相同协议的预约发送时间已确保递增）
		/// \~English @param protocol Raw data protocol ID whose format is "xxx-v?", "xxx" indicates the data source, "v?" indicates the version
		/// \~Chinese @param protocol 原始数据协议ID，一般格式为"xxx-v?"，xxx表示数据来源，v?表示版本
		/// \~English @param vector Numerical array data
		/// \~Chinese @param vector 数值数组数据
		/// \~English @param binary Binary data
		/// \~Chinese @param binary 二进制数据
		/// \~English @param serverPosix Posix time of the time server for scheduled transmitting, in nanoseconds
		/// \~Chinese @param serverPosix 预约发送的授时服务器Posix时间，单位纳秒
		/// \~English @param tolerance The maximum delayed transmit time allowed, in nanoseconds
		/// \~Chinese @param tolerance 允许的最大延迟发送时间，单位纳秒
		/// \~English @returns Whether the scheduled transmitting is successful, FALSE if the protocol is not in the list
		/// \~Chinese @returns 返回是否成功预约发送一帧数据，若协议未在可发送的协议列表内则返回FALSE
		virtual Bool transmitGeneralDataScheduled(String protocol, Array<Double> vector, Binary binary, NanoPosix serverPosix, UInt tolerance);
	};

	/// \~English @brief Function definition of getting the general device plugin interface, the function name should be get_device_plugin_v202
	/// \~Chinese @brief 获取一般设备插件接口的全局函数定义，函数名应为get_device_plugin_v202
	typedef Interface<IDevicePluginV202>(*GetDevicePluginV202)();

	/// \~English @brief Latest version of general device plugin interface
	/// \~Chinese @brief 最新版本一般设备插件接口
	typedef IDevicePluginV202 IDevicePluginLatest;

	/// \~English @brief Bus device plugin interface 2.1
	/// \~Chinese @brief 总线设备插件接口 2.1
	class SPADAS_API IBusPluginV201
	{
	public:
		virtual ~IBusPluginV201() {};

		/// \~English @brief Get the bus device list
		/// \~Chinese @brief 获取总线设备列表
		/// \~English @returns The list of bus devices
		/// \~Chinese @returns 返回总线设备列表
		virtual Array<BusDeviceInfoX> getBusDeviceList();

		/// \~English @brief Open the bus device (called while starting the session)
		/// \~Chinese @brief 打开总线设备（在开始Session时被调用）
		/// \~English @param configs The list of bus device channels and related configurations
		/// \~Chinese @param configs 希望打开的总线设备通道列表及相关配置
		virtual Bool openBusDevice(Array<BusDeviceConfig> configs);

		/// \~English @brief Close the bus device (called when the session ends)
		/// \~Chinese @brief 关闭总线设备（在结束Session时被调用）
		virtual void closeBusDevice();

		/// \~English @brief [Optional] Get a frame of data
		/// \~Chinese @brief [可选] 获取一帧数据
		/// \~English @param rxData Output the frame of data (if any)
		/// \~Chinese @param rxData 输出一帧数据（若有）
		/// \~English @returns Whether a frame of data is successfully obtained
		/// \~Chinese @returns 返回是否成功获取一帧数据
		virtual Bool receiveBusMessage(BusDeviceData& rxData);

		/// \~English @brief [Optional] Immediately transmit a frame of data
		/// \~Chinese @brief [可选] 立即发送一帧数据
		/// \~English @param channel Bus channel, 1~16
		/// \~Chinese @param channel 总线通道，1~16
		/// \~English @param id The message ID in this channel
		/// \~Chinese @param id 该通道内的报文ID
		/// \~English @param binary Message data
		/// \~Chinese @param binary 报文数据
		/// \~English @returns Whether the frame of data is transmitted successfully
		/// \~Chinese @returns 返回是否成功发送一帧数据
		virtual Bool transmitBusMessage(UInt channel, UInt id, Binary binary);

		/// \~English @brief [Optional] Schedule to transmit a frame of data (the timestamps of the same channel are guaranteed to be monotonically increasing)
		/// \~Chinese @brief [可选] 预约发送一帧数据（相同通道的预约发送时间已确保递增）
		/// \~English @param channel Bus channel, 1~16
		/// \~Chinese @param channel 总线通道，1~16
		/// \~English @param id The message ID in this channel
		/// \~Chinese @param id 该通道内的报文ID
		/// \~English @param binary Message data
		/// \~Chinese @param binary 报文数据
		/// \~English @param serverPosix Posix time of the time server for scheduled transmitting, in nanoseconds
		/// \~Chinese @param serverPosix 预约发送的授时服务器Posix时间，单位纳秒
		/// \~English @param tolerance The maximum delayed transmit time allowed, in nanoseconds
		/// \~Chinese @param tolerance 允许的最大延迟发送时间，单位纳秒
		/// \~English @returns Whether the scheduled transmitting is successful
		/// \~Chinese @returns 返回是否成功预约发送一帧数据
		virtual Bool transmitBusMessageScheduled(UInt channel, UInt id, Binary binary, NanoPosix serverPosix, UInt tolerance);

		/// \~English @brief [Optional] Additional settings for the bus device (called before openBusDevice)
		/// \~Chinese @brief [可选] 对总线设备进行额外设置（在openBusDevice前被调用）
		/// \~English @param extra Extra configuration information
		/// \~Chinese @param extra 配置信息
		virtual void setBusExtraConfig(String extra);

		/// \~English @brief [Optional] Get the data load of each bus channel
		/// \~Chinese @brief [可选] 获取各总线通道的数据负载情况
		/// \~English @returns The data load percentage of each bus channel
		/// \~Chinese @returns 返回各总线通道的数据负载百分比
		virtual Array<BusChannelPayload> getBusPayload();
	};

	/// \~English @brief Function definition of getting the bus device plugin interface, the function name should be get_bus_plugin_v201
	/// \~Chinese @brief 获取总线设备插件接口的全局函数定义，函数名应为get_bus_plugin_v201
	typedef Interface<IBusPluginV201>(*GetBusPluginV201)();

	/// \~English @brief Latest version of bus device plugin interface
	/// \~Chinese @brief 最新版本总线设备插件接口
	typedef IBusPluginV201 IBusPluginLatest;

	/// \~English @brief Video device plugin interface 4.3
	/// \~Chinese @brief 视频设备插件接口 4.3
	class SPADAS_API IVideoPluginV403
	{
	public:
		virtual ~IVideoPluginV403() {};

		/// \~English @brief Get video device list
		/// \~Chinese @brief 获取视频设备列表
		/// \~English @returns Video device list
		/// \~Chinese @returns 视频设备列表
		virtual Array<VideoDeviceInfoX2> getVideoDeviceList();

		/// \~English @brief Open the video device (called while starting the session)
		/// \~Chinese @brief 打开视频设备（在开始Session时被调用）
		/// \~English @param configs The list of video device channels and related configurations
		/// \~Chinese @param configs 希望打开的视频设备通道列表及相关配置
		virtual Bool openVideoDevice(Array<VideoDeviceConfigX> configs);

		/// \~English @brief Close the bus device (called when the session ends)
		/// \~Chinese @brief 关闭总线设备（在结束Session时被调用）
		virtual void closeVideoDevice();

		/// \~English @brief [Optional] Get a frame of data
		/// \~Chinese @brief [可选] 获取一帧数据
		/// \~English @param frame Output a frame of data (if any)
		/// \~Chinese @param frame 输出一帧数据（若有）
		/// \~English @returns Whether a frame of data is successfully output
		/// \~Chinese @returns 返回是否成功输出一帧数据
		virtual Bool queryVideoFrame(VideoDeviceData& frame);

		/// \~English @brief [Optional] Immediately transmit a frame of data
		/// \~Chinese @brief [可选] 立即发送一帧数据
		/// \~English @param channel Video channel, 0~23, corresponding to A~X
		/// \~Chinese @param channel 视频通道，0~23，对应A~X
		/// \~English @param codec Video codec
		/// \~Chinese @param codec 视频帧编码方式
		/// \~English @param size Video frame size, pixel unit
		/// \~Chinese @param size 视频帧大小，像素单位
		/// \~English @param data Video frame data
		/// \~Chinese @param data 视频帧数据
		/// \~English @returns Whether the frame of data is transmitted successfully
		/// \~Chinese @returns 返回是否成功发送一帧数据
		virtual Bool transmitVideoFrame(UInt channel, VideoDataCodec codec, Size2D size, Binary data);

		/// \~English @brief [Optional] Schedule to transmit a frame of data (the timestamps of the same channel are guaranteed to be monotonically increasing)
		/// \~Chinese @brief [可选] 预约发送一帧数据（相同通道的预约发送时间已确保递增）
		/// \~English @param channel Video channel, 0~23, corresponding to A~X
		/// \~Chinese @param channel 视频通道，0~23，对应A~X
		/// \~English @param codec Video codec
		/// \~Chinese @param codec 视频帧编码方式
		/// \~English @param size Video frame size, pixel unit
		/// \~Chinese @param size 视频帧大小，像素单位
		/// \~English @param data Video frame data
		/// \~Chinese @param data 视频帧数据
		/// \~English @param serverPosix Posix time of the time server for scheduled transmitting, in nanoseconds
		/// \~Chinese @param serverPosix 预约发送的授时服务器Posix时间，单位纳秒
		/// \~English @param tolerance The maximum delayed transmit time allowed, in nanoseconds
		/// \~Chinese @param tolerance 允许的最大延迟发送时间，单位纳秒
		/// \~English @returns Whether the scheduled transmitting is successful
		/// \~Chinese @returns 返回是否成功预约发送一帧数据
		virtual Bool transmitVideoFrameScheduled(UInt channel, VideoDataCodec codec, Size2D size, Binary data, NanoPosix serverPosix, UInt tolerance);

		/// \~English @brief [Optional] Additional settings for the video device (called before openVideoDevice)
		/// \~Chinese @brief [可选] 对视频设备进行额外设置（在openVideoDevice前被调用）
		/// \~English @param extra Extra configuration information
		/// \~Chinese @param extra 配置信息
		virtual void setVideoExtraConfig(String extra);

		/// \~English @brief [Optional] Get the new general raw data of the video device
		/// \~Chinese @brief [可选] 获取视频设备新原始数据
		/// \~English @returns All the newly acquired data
		/// \~Chinese @returns 返回新获取的数据
		virtual Array<GeneralDeviceData> getVideoDeviceNewData();

		/// \~English @brief [Optional] Set the express output interface for preview images
		/// \~Chinese @brief [可选] 设置使用指定的视频预览图像的快速输出接口
		/// \~English @param previewExpress Express output interface for preview images
		/// \~Chinese @param previewExpress 视频预览图像的快速输出接口
		virtual void useVideoPreviewExpress(Interface<IVideoPreviewExpressX> previewExpress);

		/// \~English @brief [Optional] Get the video device exclusive keyword, other video devices whose description matching this keyword will be disabled
		/// \~Chinese @brief [可选] 获取视频设备独占关键字，其他插件匹配此关键字的视频设备将被禁用
		/// \~English @returns Video device exclusive keywords
		/// \~Chinese @returns 返回视频设备独占关键字
		virtual Array<String> getExclusiveKeywords();
	};

	/// \~English @brief Function definition of getting the video device plugin interface, the function name should be get_video_plugin_v403
	/// \~Chinese @brief 获取视频设备插件接口的全局函数定义，函数名应为get_video_plugin_v403
	typedef Interface<IVideoPluginV403>(*GetVideoPluginV403)();

	/// \~English @brief Latest version of video device plugin interface
	/// \~Chinese @brief 最新版本视频设备插件接口
	typedef IVideoPluginV403 IVideoPluginLatest;

	/// \~English @brief Native data processing plugin interface 6.3
	/// \~Chinese @brief 原生数据处理插件接口 6.3
	class SPADAS_API IProcessorPluginV603
	{
	public:
		virtual ~IProcessorPluginV603() {};

		/// \~English @brief [Optional] Whether it is online mode limited data processing (default is FALSE)
		/// \~Chinese @brief [可选] 是否为在线限定的数据处理（默认为FALSE）
		virtual Bool isProcessorOnlineOnly();

		/// \~English @brief [Optional] Whether it is offline mode limited data processing (default is FALSE)
		/// \~Chinese @brief [可选] 是否为离线限定的数据处理（默认为FALSE）
		virtual Bool isProcessorOfflineOnly();

		/// \~English @brief Configure data processing (called while starting the session)
		/// \~Chinese @brief 配置数据处理（在开始Session时被调用）
		/// \~English @param config Configuration string, which includes the configuration of whether to enable the function
		/// \~Chinese @param config 配置信息，应包含是否启用功能的配置
		/// \~English @param onlineMode Whether it is online mode, otherwise it is offline mode
		/// \~Chinese @param onlineMode 是否为在线模式，否则为离线模式或回放模式
		virtual void setProcessorConfig(String config, Bool onlineMode);

		/// \~English @brief Disable data processing function (called when session ends)
		/// \~Chinese @brief 禁用数据处理功能（在结束Session时被调用）
		virtual void disableProcessor();

		/// \~English @brief Process data, the function should only modify the output data table
		/// \~Chinese @brief 处理数据，函数内部应该只对输出数据表作更改
		/// \~English @param inputs Input data table
		/// \~Chinese @param inputs 输入数据表
		/// \~English @param sampleBuffers Sample buffer table
		/// \~Chinese @param sampleBuffers 样本缓存表
		/// \~English @param outputs Output data table
		/// \~Chinese @param outputs 输出数据表
		virtual void processData(InputTablesX inputs, SessionSampleBufferTable sampleBuffers, OutputTablesX outputs);

		/// \~English @brief [Optional] Set to use the specified time-related service interface
		/// \~Chinese @brief [可选] 设置使用指定的时间相关服务接口
		/// \~English @param timeServer Time related service interface
		/// \~Chinese @param timeServer 时间相关服务接口
		virtual void useTimeServer(Interface<ITimeServer> timeServer);

		/// \~English @brief [Optional] Set to use the specified general raw data transmitting interface
		/// \~Chinese @brief [可选] 设置使用指定的通用原始数据发送接口
		/// \~English @param generalTransmitter General raw data transmitting interface
		/// \~Chinese @param generalTransmitter 通用原始数据发送接口
		virtual void useGeneralTransmitter(Interface<IGeneralDataTransmitterX> generalTransmitter);

		/// \~English @brief [Optional] Set to use the specified bus message transmitting interface
		/// \~Chinese @brief [可选] 设置使用指定的总线报文发送接口
		/// \~English @param busTransmitter Bus message transmitting interface
		/// \~Chinese @param busTransmitter 总线报文发送接口
		virtual void useBusTransmitter(Interface<IBusMessageTransmitterX> busTransmitter);

		/// \~English @brief [Optional] Set to use the specified video frame re-injection interface
		/// \~Chinese @brief [可选] 设置使用指定的视频帧回注接口
		/// \~English @param videoTransmitter Video frame re-injection interface
		/// \~Chinese @param videoTransmitter 视频帧回注接口
		virtual void useVideoTransmitter(Interface<IVideoFrameTransmitterX> videoTransmitter);
	};

	/// \~English @brief Function definition of getting the native data processing plugin interface, the function name should be get_processor_plugin_v603
	/// \~Chinese @brief 获取原生数据处理插件接口的全局函数定义，函数名应为get_processor_plugin_v603
	typedef Interface<IProcessorPluginV603>(*GetProcessorPluginV603)();

	/// \~English @brief Latest version of native data processing plugin interface
	/// \~Chinese @brief 最新版本原生数据处理插件接口
	typedef IProcessorPluginV603 IProcessorPluginLatest;

	/// \~English @brief File R/W plugin interface 1.3
	/// \~Chinese @brief 文件读写插件接口 1.3
	class SPADAS_API IFilePluginV103
	{
	public:
		virtual ~IFilePluginV103() {};

		/// \~English @brief [Optional] Get the maximum duration of all files applicable to the specified reader
		/// \~Chinese @brief [可选] 获取适用于指定读取器的所有文件的最大时长
		/// \~English @param readerName Reader name
		/// \~Chinese @param readerName 读取器名称
		/// \~English @param inputRoot Session's "input" folder path
		/// \~Chinese @param inputRoot Session的input文件夹路径
		/// \~English @param subInputRoots Session's "input" subfolder paths
		/// \~Chinese @param subInputRoots Session的input子文件夹路径
		/// \~English @param generationRoots Session's all generation folder paths
		/// \~Chinese @param generationRoots Session的所有generation文件夹路径
		/// \~English @param basicInfo Basic information for file reading
		/// \~Chinese @param basicInfo 文件读写基本信息
		/// \~English @param timeServer Time-related service interface (only for current reading session)
		/// \~Chinese @param timeServer 时间相关服务接口（仅限当前读取Session）
		/// \~English @returns The maximum duration of all files, in seconds, if there is no file or no data, return 0
		/// \~Chinese @returns 所有文件的最大时长，单位秒，若无文件或无数据则返回0
		virtual Double getFilesDuration(String readerName, Path inputRoot, Array<Path> subInputRoots, Array<Path> generationRoots, FileIOBasicInfo basicInfo, Interface<ITimeServer> timeServer);

		/// \~English @brief [Optional] Initialize to read raw data file
		/// \~Chinese @brief [可选] 初始化读取原始数据文件
		/// \~English @param readerName Reader name
		/// \~Chinese @param readerName 读取器名称
		/// \~English @param inputRoot Session's "input" folder path
		/// \~Chinese @param inputRoot Session的input文件夹路径
		/// \~English @param subInputRoots Session's "input" subfolder paths
		/// \~Chinese @param subInputRoots Session的input子文件夹路径
		/// \~English @param generationRoot Generation folder path
		/// \~Chinese @param generationRoot Generation的文件夹路径
		/// \~English @param jumpOffset Jump to this time offset to start reading
		/// \~Chinese @param jumpOffset 跳转至该时间偏置开始读取
		/// \~English @param basicInfo Basic information for file reading
		/// \~Chinese @param basicInfo 文件读写基本信息
		/// \~English @param extInfo Output file extension information
		/// \~Chinese @param extInfo 输出文件扩展信息
		/// \~English @param timeServer Time-related service interface (only for current reading session)
		/// \~Chinese @param timeServer 时间相关服务接口（仅限当前读取Session）
		/// \~English @returns Whether the initialization is successful, or returns FALSE if there is no data file
		/// \~Chinese @returns 返回是否成功初始化，无数据文件的情况也返回FALSE
		virtual Bool openReadFiles(String readerName, Path inputRoot, Array<Path> subInputRoots, Path generationRoot, Double jumpOffset, FileIOBasicInfo basicInfo, FileIOExtInfo& extInfo, Interface<ITimeServer> timeServer);

		/// \~English @brief [Optional] Read file data
		/// \~Chinese @brief [可选] 读取文件数据
		/// \~English @param readerName Reader name
		/// \~Chinese @param readerName 读取器名称
		/// \~English @param inputs Input data table, the read data is written into this table (the timestamp of all dependent frames of the first frame of the video is 0)
		/// \~Chinese @param inputs 输入数据表，读取的数据写入该表（其中视频首帧图像的所有依赖帧时间戳为0）
		/// \~English @param targetOffset Read target time offset, in seconds
		/// \~Chinese @param targetOffset 读取的目标时间偏置，单位秒
		/// \~English @param shouldEnd Whether to stop reading
		/// \~Chinese @param shouldEnd 是否准备关闭
		/// \~English @returns Whether there is data following. If all files have been read to the end, return FALSE
		/// \~Chinese @returns 后续是否还有数据，若所有文件已读取至末尾则返回FALSE
		virtual Bool readFilesData(String readerName, InputTablesX inputs, Double targetOffset, Flag shouldEnd);

		/// \~English @brief [Optional] Close reading file
		/// \~Chinese @brief [可选] 关闭读取文件
		/// \~English @param readerName Reader name
		/// \~Chinese @param readerName 读取器名称
		virtual void closeReadFiles(String readerName);

		/// \~English @brief [Optional] Initialize to write data file
		/// \~Chinese @brief [可选] 初始化写入数据文件
		/// \~English @param writerName Writer name
		/// \~Chinese @param writerName 写入器名称
		/// \~English @param inputRoot Session's "input" folder path
		/// \~Chinese @param inputRoot Session的input文件夹路径
		/// \~English @param subInputRoots Session's "input" subfolder paths
		/// \~Chinese @param subInputRoots Session的input子文件夹路径
		/// \~English @param generationRoot Generation folder path
		/// \~Chinese @param generationRoot Generation的文件夹路径
		/// \~English @param basicInfo Basic information for file writing
		/// \~Chinese @param basicInfo 文件读写基本信息
		/// \~English @param extInfo File extension information
		/// \~Chinese @param extInfo 文件扩展信息
		/// \~English @param timeServer Time-related service interface (only for current writing session)
		/// \~Chinese @param timeServer 时间相关服务接口（仅限当前写入Session）
		/// \~English @returns Whether the initialization is successful
		/// \~Chinese @returns 返回是否成功初始化
		virtual Bool openWriteFiles(String writerName, Path inputRoot, Array<Path> subInputRoots, Path generationRoot, FileIOBasicInfo basicInfo, FileIOExtInfo extInfo, Interface<ITimeServer> timeServer);

		/// \~English @brief [Optional] Write file data
		/// \~Chinese @brief [可选] 写入文件数据
		/// \~English @param writerName Writer name
		/// \~Chinese @param writerName 写入器名称
		/// \~English @param inputs Input data table, get the data from the table and write it to the files (the timestamp of all dependent frames of the first frame of the video is 0)
		/// \~Chinese @param inputs 输入数据表，从表中获取数据写入文件（其中视频首帧图像的所有依赖帧时间戳为0）
		/// \~English @param busMessages Bus data of all channels sorted by timestamp
		/// \~Chinese @param busMessages 按时间戳排序的所有通道总线数据
		/// \~English @param shouldEnd Whether to stop writing
		/// \~Chinese @param shouldEnd 是否准备关闭
		virtual void writeFilesData(String writerName, InputTablesX inputs, Array<SessionBusRawData> busMessages, Flag shouldEnd);

		/// \~English @brief Close writing to file
		/// \~Chinese @brief [可选] 关闭写入文件
		/// \~English @param writerName Writer name
		/// \~Chinese @param writerName 写入器名称
		virtual void closeWriteFiles(String writerName);

		/// \~English @brief [Optional] Get whether there is any data files capable for data picking
		/// \~Chinese @brief [可选] 获取是否有适用于数据截取器的数据文件
		/// \~English @param pickerName Data picker name
		/// \~Chinese @param pickerName 数据截取器名称
		/// \~English @param inputRoot The "input" folder of the source session
		/// \~Chinese @param inputRoot 源Session的input文件夹
		/// \~English @param subInputRoots The "input" subfolder paths of the source session
		/// \~Chinese @param subInputRoots 源Session的input子文件夹路径
		/// \~English @param generationRoot The generation folder of the source session
		/// \~Chinese @param generationRoot 源Session的generation文件夹
		/// \~English @param basicInfo Basic information for file reading
		/// \~Chinese @param basicInfo 文件读写基本信息
		/// \~English @returns Whether there is any data files capable for data picking
		/// \~Chinese @returns 是否有适用于数据截取器的数据文件
		virtual Bool hasDataFiles(String pickerName, Path inputRoot, Array<Path> subInputRoots, Path generationRoot, FileIOBasicInfo basicInfo);

		/// \~English @brief [Optional] Pick data, called in the standalone task of data picking
		/// \~Chinese @brief [可选] 离线截取数据，在数据截取独立任务中被调用
		/// \~English @param pickerName Data picker name
		/// \~Chinese @param pickerName 数据截取器名称
		/// \~English @param inputRoot The "input" folder of the source session
		/// \~Chinese @param inputRoot 源Session的input文件夹
		/// \~English @param subInputRoots The "input" subfolder paths of the source session
		/// \~Chinese @param subInputRoots 源Session的input子文件夹路径
		/// \~English @param generationRoot The generation folder of the source session
		/// \~Chinese @param generationRoot 源Session的generation文件夹
		/// \~English @param pick Picking task parameters
		/// \~Chinese @param pick 截取任务参数
		/// \~English @param basicInfo Basic information for file reading
		/// \~Chinese @param basicInfo 文件读写基本信息
		/// \~English @param shouldEnd Weather to stop picking
		/// \~Chinese @param shouldEnd 是否已被取消
		/// \~English @param callback The feedback interface of the task, mainly used to notify the progress of the task
		/// \~Chinese @param callback 任务的反馈接口，主要用于通知任务进度
		/// \~English @param timeServer Time-related service interface (only for the current source session and target session)
		/// \~Chinese @param timeServer 时间相关服务接口（仅限当前源Session和目标Session）
		virtual void pickSession(String pickerName, Path inputRoot, Array<Path> subInputRoots, Path generationRoot, PickConfig pick, FileIOBasicInfo basicInfo, Flag shouldEnd, Interface<IStandaloneTaskCallback> callback, Interface<ITimeServer> timeServer);

		/// \~English @brief [Optional] Additional settings for file R/W (called before each openXXXFiles function)
		/// \~Chinese @brief [可选] 对文件读写进行额外设置（在各openXXXFiles函数前被调用）
		/// \~English @param extra Extra configuration information
		/// \~Chinese @param extra 配置信息
		virtual void setFileExtraConfig(String extra);
	};

	/// \~English @brief Function definition of getting the file R/W plugin interface, the function name should be get_file_plugin_v103
	/// \~Chinese @brief 获取文件读写插件接口的全局函数定义，函数名应为get_file_plugin_v103
	typedef Interface<IFilePluginV103>(*GetFilePluginV103)();

	/// \~English @brief Latest version of file R/W plugin interface
	/// \~Chinese @brief 最新版本文件读写插件接口
	typedef IFilePluginV103 IFilePluginLatest;
}

#endif
