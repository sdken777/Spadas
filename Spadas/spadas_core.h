
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
	
	/// \~English @brief UTF-8 character
	/// \~Chinese @brief UTF-8字符
	typedef char Char;
	
	/// \~English @brief Windows: UTF-16 character, Linux: UTF-32 character
	/// \~Chinese @brief Windows: UTF-16字符, Linux: UTF-32字符
	typedef wchar_t WChar;
	
	/// \~English @brief 32-bit floating point number, single precision
	/// \~Chinese @brief 32位浮点数，单精度
	typedef float Float;

	/// \~English @brief 64-bit floating point number, double precision
	/// \~Chinese @brief 64位浮点数，双精度
	typedef double Double;

	/// \~English @brief Pointer for reading and writing
	/// \~Chinese @brief 读写数据指针
	typedef void *Pointer;

	/// \~English @brief Read-only pointer for reading and writing
	/// \~Chinese @brief 只读数据指针
	typedef const void *ConstPointer;
	
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
	class String;

	// String span / 字符串片段
	class StringSpan;

	// String appender / 字符串拼接器
	class StringAppender;

	// Nullable object / 可空对象
	template <typename Type> class Optional;

	// Array / 数组
	template <typename Type> class Array;

	// Array span / 数组片段
	template <typename Type> class ArraySpan;

	// Linked list node / 链表节点
	template <typename Type> class ListNode;

	// Linked list / 链表
	template <typename Type> class List;
	
	// Binary data / 二进制数据
	class Binary;

	// Binary data span / 二进制数据片段
	class BinarySpan;

	// XML element node / XML元素节点
	class XMLNode;

	// Flag bit / 标志位
	class Flag;

	// Sample / 样本
	class SessionSample;

	// General sample element / 通用样本元素
	struct GeneralElement;

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
		Size2D() : width(0), height(0) {}

		/// \~English @brief Whether it is equal to
		/// \~Chinese @brief 是否等于
		Bool operator ==(Size2D size) { return width == size.width && height == size.height; }

		/// \~English @brief Whether it is not equal to
		/// \~Chinese @brief 是否不等于
		Bool operator !=(Size2D size) { return !(width == size.width && height == size.height); }

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
		Size3D() : width(0), height(0), depth(0) {}

		/// \~English @brief Whether it is equal to
		/// \~Chinese @brief 是否等于
		Bool operator ==(Size3D size) { return width == size.width && height == size.height && depth == size.depth; }

		/// \~English @brief Whether it is not equal to
		/// \~Chinese @brief 是否不等于
		Bool operator !=(Size3D size) { return !(width == size.width && height == size.height && depth == size.depth); }

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
		Index2D() : u(0), v(0) {}

		/// \~English @brief Whether it is equal to
		/// \~Chinese @brief 是否等于
		Bool operator ==(Index2D index) { return u == index.u && v == index.v; }

		/// \~English @brief Whether it is not equal to
		/// \~Chinese @brief 是否不等于
		Bool operator !=(Index2D index) { return !(u == index.u && v == index.v); }

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
		Index3D() : u(0), v(0), w(0) {}

		/// \~English @brief Whether it is equal to
		/// \~Chinese @brief 是否等于
		Bool operator ==(Index3D index) { return u == index.u && v == index.v && w == index.w; }

		/// \~English @brief Whether it is not equal to
		/// \~Chinese @brief 是否不等于
		Bool operator !=(Index3D index) { return !(u == index.u && v == index.v && w == index.w); }

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
		Region() : offset(0), size(0) {}

		/// \~English @brief Initialize based on starting position and size
		/// \~Chinese @brief 基于起始位置和大小初始化
		Region(Int offset, UInt size) : offset(offset), size(size) {}

		/// \~English @brief Whether it is equal to
		/// \~Chinese @brief 是否等于
		Bool operator ==(Region region) { return offset == region.offset && size == region.size; }

		/// \~English @brief Whether it is not equal to
		/// \~Chinese @brief 是否不等于
		Bool operator !=(Region region) { return !(offset == region.offset && size == region.size); }

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
		Region2D() : offsetU(0), offsetV(0), width(0), height(0) {}

		/// \~English @brief Initialize based on offset and size
		/// \~Chinese @brief 基于起始位置和大小初始化
		Region2D(Int offsetI, Int offsetJ, UInt dim0, UInt dim1) : offsetJ(offsetJ), offsetI(offsetI), dim1(dim1), dim0(dim0) {}

		/// \~English @brief Initialize based on offset and size
		/// \~Chinese @brief 基于起始位置和大小初始化
		Region2D(CoordInt2D offset, Size2D size) : offsetU(offset.u), offsetV(offset.v), width(size.width), height(size.height) {}

		/// \~English @brief Whether it is equal to
		/// \~Chinese @brief 是否等于
		Bool operator ==(Region2D region) { return offsetU == region.offsetU && width == region.width && offsetV == region.offsetV && height == region.height; }

		/// \~English @brief Whether it is not equal to
		/// \~Chinese @brief 是否不等于
		Bool operator !=(Region2D region) { return !(offsetU == region.offsetU && width == region.width && offsetV == region.offsetV && height == region.height); }

		/// \~English @brief Get offset
		/// \~Chinese @brief 获取起始位置
		CoordInt2D offset() { CoordInt2D out; out.u = offsetU; out.v = offsetV; return out; }

		/// \~English @brief Get size
		/// \~Chinese @brief 获取大小
		Size2D size() { Size2D out; out.width = width; out.height = height; return out; }

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
		Region3D() : offsetU(0), offsetV(0), offsetW(0), width(0), height(0), depth(0) {}

		/// \~English @brief Initialize based on offset and size
		/// \~Chinese @brief 基于起始位置和大小初始化
		Region3D(Int offsetI, Int offsetJ, Int offsetK, UInt dim0, UInt dim1, UInt dim2) : offsetJ(offsetJ), offsetI(offsetI), offsetK(offsetK), dim1(dim1), dim0(dim0), dim2(dim2) {}

		/// \~English @brief Initialize based on offset and size
		/// \~Chinese @brief 基于起始位置和大小初始化
		Region3D(CoordInt3D offset, Size3D size) : offsetU(offset.u), offsetV(offset.v), offsetW(offset.w), width(size.width), height(size.height), depth(size.depth) {}

		/// \~English @brief Whether it is equal to
		/// \~Chinese @brief 是否等于
		Bool operator ==(Region3D region) { return offsetU == region.offsetU && width == region.width && offsetV == region.offsetV && height == region.height && offsetW == region.offsetW && depth == region.depth; }

		/// \~English @brief Whether it is not equal to
		/// \~Chinese @brief 是否不等于
		Bool operator !=(Region3D region) { return !(offsetU == region.offsetU && width == region.width && offsetV == region.offsetV && height == region.height && offsetW == region.offsetW && depth == region.depth); }

		/// \~English @brief Get offset
		/// \~Chinese @brief 获取起始位置
		CoordInt3D offset() { CoordInt3D out; out.u = offsetU; out.v = offsetV; out.w = offsetW; return out; }

		/// \~English @brief Get size
		/// \~Chinese @brief 获取大小
		Size3D size() { Size3D out; out.width = width; out.height = height; out.depth = depth; return out; }

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
		Vector2D() : x(0), y(0) {}

		/// \~English @brief Initialize based on horizontal and vertical coordinates
		/// \~Chinese @brief 基于横纵坐标初始化
		Vector2D(Float x, Float y) : x(x), y(y) {}

		/// \~English @brief Whether it is equal to
		/// \~Chinese @brief 是否等于
		Bool operator ==(Vector2D vec) { return x == vec.x && y == vec.y; }

		/// \~English @brief Whether it is not equal to
		/// \~Chinese @brief 是否不等于
		Bool operator !=(Vector2D vec) { return !(x == vec.x && y == vec.y); }

		/// \~English @brief Vector addition
		/// \~Chinese @brief 矢量相加
		Vector2D operator +(Vector2D vec) { return Vector2D(x + vec.x, y + vec.y); }

		/// \~English @brief Vector subtraction
		/// \~Chinese @brief 矢量相减
		Vector2D operator -(Vector2D vec) { return Vector2D(x - vec.x, y - vec.y); }

		/// \~English @brief Vector multiplied by scale factor
		/// \~Chinese @brief 矢量乘以比例系数
		Vector2D operator *(Float k) { return Vector2D(x * k, y * k); }

		/// \~English @brief Vector inner product (dot product)
		/// \~Chinese @brief 矢量内积（点乘）
		Float operator *(Vector2D vec) { return x * vec.x + y * vec.y; }

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
		Vector3D() : x(0), y(0), z(0) {}

		/// \~English @brief Initialize based on horizontal, vertical and depth coordinates
		/// \~Chinese @brief 基于横纵深坐标初始化
		Vector3D(Float x, Float y, Float z) : x(x), y(y), z(z) {}

		/// \~English @brief Whether it is equal to
		/// \~Chinese @brief 是否等于
		Bool operator ==(Vector3D vec) { return x == vec.x && y == vec.y && z == vec.z; }

		/// \~English @brief Whether it is not equal to
		/// \~Chinese @brief 是否不等于
		Bool operator !=(Vector3D vec) { return !(x == vec.x && y == vec.y && z == vec.z); }

		/// \~English @brief Vector addition
		/// \~Chinese @brief 矢量相加
		Vector3D operator +(Vector3D vec) { return Vector3D(x + vec.x, y + vec.y, z + vec.z); }

		/// \~English @brief Vector subtraction
		/// \~Chinese @brief 矢量相减
		Vector3D operator -(Vector3D vec) { return Vector3D(x - vec.x, y - vec.y, z - vec.z); }

		/// \~English @brief Vector multiplied by scale factor
		/// \~Chinese @brief 矢量乘以比例系数
		Vector3D operator *(Float k) { return Vector3D(x * k, y * k, z * k); }

		/// \~English @brief Vector inner product (dot product)
		/// \~Chinese @brief 矢量内积（点乘）
		Float operator *(Vector3D vec) { return x * vec.x + y * vec.y + z * vec.z; }

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
		Range() : lower(0), upper(0) {}

		/// \~English @brief Initialization based on the upper and lower edges
		/// \~Chinese @brief 基于上下沿初始化
		Range(Float lower, Float upper) : lower(lower), upper(upper) {}

		/// \~English @brief Whether it is equal to
		/// \~Chinese @brief 是否等于
		Bool operator ==(Range range) { return lower == range.lower && upper == range.upper; }

		/// \~English @brief Whether it is not equal to
		/// \~Chinese @brief 是否不等于
		Bool operator !=(Range range) { return !(lower == range.lower && upper == range.upper); }

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
		Range2D() : lowerX(0), upperX(0), lowerY(0), upperY(0) {}

		/// \~English @brief Initialization based on the upper and lower edges
		/// \~Chinese @brief 基于上下沿初始化
		Range2D(Float lowerX, Float upperX, Float lowerY, Float upperY) : lowerX(lowerX), upperX(upperX), lowerY(lowerY), upperY(upperY) {}

		/// \~English @brief Whether it is equal to
		/// \~Chinese @brief 是否等于
		Bool operator ==(Range2D range) { return lowerX == range.lowerX && upperX == range.upperX && lowerY == range.lowerY && upperY == range.upperY; }

		/// \~English @brief Whether it is not equal to
		/// \~Chinese @brief 是否不等于
		Bool operator !=(Range2D range) { return !(lowerX == range.lowerX && upperX == range.upperX && lowerY == range.lowerY && upperY == range.upperY); }

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
		Range3D() : lowerX(0), upperX(0), lowerY(0), upperY(0), lowerZ(0), upperZ(0) {}

		/// \~English @brief Initialization based on the upper and lower edges
		/// \~Chinese @brief 基于上下沿初始化
		Range3D(Float lowerX, Float upperX, Float lowerY, Float upperY, Float lowerZ, Float upperZ) : lowerX(lowerX), upperX(upperX), lowerY(lowerY), upperY(upperY), lowerZ(lowerZ), upperZ(upperZ) {}

		/// \~English @brief Whether it is equal to
		/// \~Chinese @brief 是否等于
		Bool operator ==(Range3D range) { return lowerX == range.lowerX && upperX == range.upperX && lowerY == range.lowerY && upperY == range.upperY && lowerZ == range.lowerZ && upperZ == range.upperZ; }

		/// \~English @brief Whether it is not equal to
		/// \~Chinese @brief 是否不等于
		Bool operator !=(Range3D range) { return !(lowerX == range.lowerX && upperX == range.upperX && lowerY == range.lowerY && upperY == range.upperY && lowerZ == range.lowerZ && upperZ == range.upperZ); }

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

	/// \~English @brief Atomic operation
	/// \~Chinese @brief 原子操作类
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
		Int increase() const;
		
		/// \~English @brief Subtract 1 and return the new value
		/// \~Chinese @brief 减1并返回新值
		Int decrease() const;
		
		/// \~English @brief Get the current value
		/// \~Chinese @brief 获取当前值
		Int get() const;

		/// \~English @brief Set the current value
		/// \~Chinese @brief 设定当前值
		void set(Int val) const;

		/// \~English @brief Add/subtract numbers and return the new value
		/// \~Chinese @brief 加/减数并返回新值
		Int add(Int number) const;

		/// \~English @brief If it is the old value, set it to the new value and return TRUE, otherwise return FALSE
		/// \~Chinese @brief 若为旧值则设定为新值并返回TRUE，否则返回FALSE
		Bool cas(Int oldVal, Int newVal) const;

		/// \~English @brief Keep calling cas until it returns TRUE
		/// \~Chinese @brief 不断调用cas，直到返回TRUE
		void casSpin(Int oldVal, Int newVal) const;
		
	private:
		Int val;
		Atom(const Atom& obj) {}
		Atom& operator =(const Atom& obj) { return *this; }
	};
	
	/// \~English @brief The base class of variables, so that it has reference counting availabilities
	/// \~Chinese @brief 变量数据的基类，使其具有引用计数属性
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

		/// \~English @brief Spin lock
		/// \~Chinese @brief 加自旋锁
		void spinEnter();

		/// \~English @brief Spin unlock
		/// \~Chinese @brief 解自旋锁
		void spinLeave();

		/// \~English @brief Get the class name
		/// \~Chinese @brief 获取类名称
		virtual String getTypeName();

		/// \~English @brief Whether it is the type (by class type ID)
		/// \~Chinese @brief 按类ID判断是否为该类型
		virtual Bool isType(ULong id);

		/// \~English @brief Whether it is the type (by class type name)
		/// \~Chinese @brief 按类名称判断是否为该类型
		virtual Bool isType(String name);

		/// \~English @brief Get the string description
		/// \~Chinese @brief 获取字符串描述
		virtual String toString();

		/// \~English @brief Whether the spin lock is managed, if so, spadas::SpinLocked will not lock
		/// \~Chinese @brief 自旋锁是否已被管理，若是则 spadas::SpinLocked 将不加锁
		virtual Bool isSpinLockManaged();

		/// \~English @brief Get the number of live objects
		/// \~Chinese @brief 取得存活对象数目
		static UInt getObjectCount();

	private:
		Atom nRefs;
		Atom varSpinLock;
		Vars(const Vars& obj) {}
		Vars& operator =(const Vars& obj) { return *this; }
	};
	
	/// \~English @brief The base class of all reference counting classes, where VarsType is a derived class of Vars
	/// \~Chinese @brief 所有引用计数类的基类，其中VarsType为Vars的子类
	template <typename VarsType> class Object
	{
	public:
		/// \~English @brief The default constructor, the pointer to variables is empty
		/// \~Chinese @brief 默认构造函数，变量数据指针为空
		Object();

		/// \~English @brief Initialize with the specified pointer to variables
		/// \~Chinese @brief 以指定变量数据指针初始化
		/// \~English @param newVars Pointer to variables
		/// \~Chinese @param newVars 变量数据的指针
		/// \~English @param isNew Should be TRUE for the new pointer, otherwise FALSE (obtained by Object::getVars, etc.)
		/// \~Chinese @param isNew 对于new出来的指针isNew应为TRUE，否则为FALSE（由 Object::getVars 等获得的情况）
		Object(VarsType *newVars, Bool isNew);
		
		/// \~English @brief Destructor, the reference of variables is decremented by 1
		/// \~Chinese @brief 析构函数，变量数据的引用减1
		~Object();
		
		/// \~English @brief Create an object that directly points to the variables of another object (as a base class is called implicitly)
		/// \~Chinese @brief 创建引用对象，直接指向另一个对象的变量数据（作为基类被隐式调用）
		Object(const Object<VarsType>& obj);
		
		/// \~English @brief Redirection, the reference count of original variables is reduced by 1, and set pointer to the variables of another object (as the base class is called implicitly)
		/// \~Chinese @brief 重定向，原变量数据引用减1并指向另一个对象的变量数据（作为基类被隐式调用）
		Object& operator =(const Object<VarsType>& obj);
		
		/// \~English @brief Is the reference equal
		/// \~Chinese @brief 是否相等
		Bool operator ==(const Object<VarsType>& obj);

		/// \~English @brief Is the reference not equal
		/// \~Chinese @brief 是否不相等
		Bool operator !=(const Object<VarsType>& obj);

		/// \~English @brief Whether the pointer to variables is null
		/// \~Chinese @brief 变量数据指针是否为空
		Bool isNull();

		/// \~English @brief Whether the pointer to variables is valid (not null)
		/// \~Chinese @brief 变量数据指针是否有效（非空）
		Bool isValid();
		
		/// \~English @brief Get the hash value
		/// \~Chinese @brief 获取哈希值
		Word getHash();

		/// \~English @brief As a non-null static object, the unique ID during the program life cycle can be obtained
		/// \~Chinese @brief 作为非空静态对象时可获取程序运行周期间的唯一ID
		ULong getID() const;

		/// \~English @brief Obtain the reference count of variables
		/// \~Chinese @brief 获得变量数据的引用计数
		UInt getRefCount();

		/// \~English @brief Obtain the class name corresponding to the variables
		/// \~Chinese @brief 获得变量数据对应的类名称
		String getTypeName();

		/// \~English @brief Whether it is the variables of this class, if the pointer to variables is empty, it also returns TRUE
		/// \~Chinese @brief 是否为该类的变量数据，若变量数据指针为空也返回TRUE
		template <typename TargetType>
		Bool is();

		/// \~English @brief Convert to object of base class or derived class, if the pointer to variables is empty or the type does not match, return the default construction object
		/// \~Chinese @brief 转换为基类或派生类对象，若变量数据指针为空或类型不符合则返回默认构造对象
		template <typename TargetType>
		TargetType as();

		/// \~English @brief Convert to object of base class or derived class, if the pointer to variables is empty or the type does not match, output the default construction object. Return FALSE while the type does not match
		/// \~Chinese @brief 转换为基类或派生类对象，若变量数据指针为空或类型不符合则输出默认构造对象，并仅当类型不符合时输出FALSE
		template <typename TargetType>
		TargetType as(Bool& ok);

		/// \~English @brief [Unsafe] Obtain the pointer to variables
		/// \~Chinese @brief [非安全] 获得变量数据的指针
		VarsType *getVars();

		/// \~English @brief @brief [Unsafe] Redirection, set pointer to variables
		/// \~Chinese @brief @brief [非安全] 重定向，设置变量数据指针
		/// \~English @param newVars Pointer to variables
		/// \~Chinese @param newVars 变量数据的指针
		/// \~English @param isNew Should be TRUE for the new pointer, otherwise FALSE (obtained by Object::getVars, etc.)
		/// \~Chinese @param isNew 对于new出来的指针isNew应为TRUE，否则为FALSE（由 Object::getVars 等获得的情况）
		void setVars(VarsType* newVars, Bool isNew);
		
		/// \~English @brief [Unsafe] Convert the input pointer to variables to the current type, and create the object
		/// \~Chinese @brief [非安全] 以输入的变量数据指针转换为当前类型创建对象
		static Object<VarsType> castCreate(Vars* varsToCast);

		/// \~English @brief Get the string description
		/// \~Chinese @brief 获取字符串描述
		String toString();

	protected:
		/// \~English @brief [Unsafe] Pointer to variables
		/// \~Chinese @brief [非安全] 变量数据指针
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
		static String typeName();

		/// \~English @brief Invalid object
		/// \~Chinese @brief 无效对象
		BaseObject();
	};

	/// \~English @brief Container to realize the multithread safety for redirection operation
	/// \~Chinese @brief 实现Type类重定向操作的多线程安全
	template <typename Type> class Safe
	{
	public:
		/// \~English @brief Create an object of Type
		/// \~Chinese @brief 创建Type对象
		Safe();
		
		/// \~English @brief Redirection by equal operator
		/// \~Chinese @brief 等于号重定向
		void operator =(Type target);

		/// \~English @brief [Unsafe] Redirection, set pointer to variables
		/// \~Chinese @brief [非安全] 重定向，设置变量数据指针
		/// \~English @param newVars Pointer to variables
		/// \~Chinese @param newVars 变量数据的指针
		/// \~English @param isNew Should be TRUE for the new pointer, otherwise FALSE (obtained by Object::getVars, etc.)
		/// \~Chinese @param isNew 对于new出来的指针isNew应为TRUE，否则为FALSE（由 Object::getVars 等获得的情况）
		template <typename VarsType>
		void setVars(VarsType* newVars, Bool isNew);

		/// \~English @brief Get object
		/// \~Chinese @brief 获取对象
		Type get();

	private:
		Type obj;
		Atom objSpinLock;
		Safe(const Safe<Type>& obj) {}
		Safe<Type>& operator =(const Safe<Type>& obj) { return *this; }
	};

	/// \~English @brief A container for multithread safe operations
	/// \~Chinese @brief 对Type对象进行多线程安全操作的容器
	template <typename Type> class SpinLocked
	{
	public:
		/// \~English @brief Constructor, input the object that needs to be multithread-safely operated, then automatically lock it
		/// \~Chinese @brief 构造函数，输入需要进行多线程安全操作的对象，自动加锁
		SpinLocked(Safe<Type>& safeObj);

		/// \~English @brief Destructor, automatic unlock
		/// \~Chinese @brief 析构，自动解锁
		~SpinLocked();

		/// \~English @brief Use the object in the container
		/// \~Chinese @brief 使用容器中的对象
		Type *operator ->();

	private:
		Type obj;
		SpinLocked(const SpinLocked& obj) {}
		SpinLocked& operator =(const SpinLocked& obj) { return *this; }
	};

	/// \~English @brief Variables of spadas::Interface template class
	/// \~Chinese @brief spadas::Interface 模板类的变量数据
	template <typename Type> class InterfaceVars;

	/// \~English @brief The container of the interface implementation object, where Type is the interface definition type
	/// \~Chinese @brief 接口实现对象的容器，其中Type为接口定义类型
	template <typename Type> class Interface : public Object<InterfaceVars<Type> >
	{
	public:
		static String typeName();

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
		static String typeName();

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

		/// \~English @brief Get reference
		/// \~Chinese @brief 取得值的引用
		Type& refValue();

		/// \~English @brief Use field or method
		/// \~Chinese @brief 使用字段或方法
		Type* operator ->();
	};

	// Data structure template / 数据结构模板 //////////////////////////////////////////////////////////////

	/// \~English @brief Fixed-length array element iterator
	/// \~Chinese @brief 定长数组元素遍历器
	template <typename Type> class ArrayElem
	{
	public:
		/// \~English @brief Initialization function
		/// \~Chinese @brief 初始化函数
		ArrayElem(Array<Type> arr, UInt index);

		/// \~English @brief Is it within the range of the array
		/// \~Chinese @brief 是否在数组范围内
		Bool valid();

		/// \~English @brief Get the index of the current element
		/// \~Chinese @brief 获取当前元素的序号
		UInt index();

		/// \~English @brief [Unsafe] Get the reference of the value (need to ensure that it is within the range of the array)
		/// \~Chinese @brief [非安全] 取得值的引用（需要先确保在数组范围内）
		Type& value();

		/// \~English @brief [Unsafe] Use the field or method of the value (need to ensure that it is within the range of the array)
		/// \~Chinese @brief [非安全] 使用值的字段或方法（需要先确保在数组范围内）
		Type* operator ->();

		/// \~English @brief [Unsafe] Assign to the current element (need to ensure that it is within the range of the array)
		/// \~Chinese @brief [非安全] 赋值给当前元素（需要先确保在数组范围内）
		void operator =(const Type& val);

		/// \~English @brief Move to the previous element
		/// \~Chinese @brief 移动至上个元素
		void operator --();

		/// \~English @brief Move to the next element
		/// \~Chinese @brief 移动至下个元素
		void operator ++();

	private:
		Array<Type> arr;
		Type *data;
		UInt size;
		UInt idx;
		ArrayElem<Type>& operator =(const ArrayElem<Type>& elem) { return *this; }
	};

	/// \~English @brief Variables of spadas::Array template class
	/// \~Chinese @brief spadas::Array 模板类的变量数据
	template <typename Type> class ArrayVars;

	/// \~English @brief Fixed length array
	/// \~Chinese @brief 定长数组
	template <typename Type> class Array : public Object<ArrayVars<Type> >
	{
	public:
		static String typeName();

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

		/// \~English @brief [Unsafe] Get the head pointer of the array
		/// \~Chinese @brief [非安全] 取得数组的头指针
		Type *data();

		/// \~English @brief Get the size of the array
		/// \~Chinese @brief 取得数组大小
		UInt size();

		/// \~English @brief Is it an empty array
		/// \~Chinese @brief 是否为空数组
		Bool isEmpty();

		/// \~English @brief Clone a new object (Call operator "=" for all elements)
		/// \~Chinese @brief 克隆出一个新对象 (所有元素调用=号拷贝)
		Array<Type> clone();

		/// \~English @brief Get a reference to an element in the array
		/// \~Chinese @brief 取得数组中某个元素的引用
		Type& operator [](UInt index);

		/// \~English @brief Get a reference to the first element in the array
		/// \~Chinese @brief 取得数组中第一个元素的引用
		Type& first();

		/// \~English @brief Get a reference to the last element in the array
		/// \~Chinese @brief 取得数组中最后一个元素的引用
		Type& last();

		/// \~English @brief Concatenate another array after this array
		/// \~Chinese @brief 在本数组后方拼接另一个数组
		/// \~English @param arr Another array to be concatenated
		/// \~Chinese @param arr 将拼接的另一个数组
		/// \~English @returns Concatenated array
		/// \~Chinese @returns 拼接后的数组
		Array<Type> operator +(Array<Type> arr);

		/// \~English @brief Reduce the size of the array, and return this array object
		/// \~Chinese @brief 缩减数组大小，并返回本数组对象
		Array<Type>& trim(UInt size);
		
		/// \~English @brief Assign the same value to all elements
		/// \~Chinese @brief 对所有元素赋同一个值
		void set(Type value);

		/// \~English @brief Whether the array contains a certain value
		/// \~Chinese @brief 数组中是否包含某个值
		Bool contain(Type val);

		/// \~English @brief Whether the array contains a value that the specified function returns TRUE
		/// \~Chinese @brief 数组中是否包含指定函数返回TRUE的某个值
		Bool containAs(Func<Bool(Type&)> func);

		/// \~English @brief Returns the indices of all elements in the array equal to a certain value
		/// \~Chinese @brief 返回数组中所有等于某个值的元素序号
		Array<UInt> search(Type val);

		/// \~English @brief Returns the indices of all elements in the array that the specified function returns TRUE
		/// \~Chinese @brief 返回数组中所有指定函数返回TRUE的元素序号
		Array<UInt> searchAs(Func<Bool(Type&)> func);

		/// \~English @brief Sort from small to large, need Type supports ">" operator
		/// \~Chinese @brief 按从小到大排序，需要Type支持>重载符
		void sort();

		/// \~English @brief According to the specified function (returns whether it is greater than), sort from small to large
		/// \~Chinese @brief 根据指定函数(判断是否大于)，按从小到大排序
		void sortAs(Func<Bool(Type&, Type&)> func);

		/// \~English @brief Convert to other types of arrays according to the specified function
		/// \~Chinese @brief 按指定函数转换为其他类型数组
		template <typename TargetType>
		Array<TargetType> convert(Func<TargetType(Type&)> func);
		
		/// \~English @brief Divide into multiple arrays according to the specified size
		/// \~Chinese @brief 根据指定的大小分割为多个数组
		/// \~English @param sizes The size of each array that will be split
		/// \~Chinese @param sizes 将分割成的每个数组的大小
		/// \~English @returns Split multiple arrays
		/// \~Chinese @returns 分割后的多个数组
		Array<ArraySpan<Type> > split(Array<UInt> sizes);

		/// \~English @brief Convert to an array of base class or derived class objects (See spadas::Object::as )
		/// \~Chinese @brief 转换为基类或派生类对象的数组（具体行为参考 spadas::Object::as ）
		template <typename TargetType>
		Array<TargetType> asArray();

		/// \~English @brief Convert to an array of base class or derived class objects, and output an array of whether each element is converted successfully (See spadas::Object::as )
		/// \~Chinese @brief 转换为基类或派生类对象的数组，并输出每个元素是否转换成功的数组（具体行为参考 spadas::Object::as ）
		template <typename TargetType>
		Array<TargetType> asArray(Array<Bool>& ok);

		/// \~English @brief Get the sub-array, which will be bound to the data of this array
		/// \~Chinese @brief 取得子数组，其数据绑定至本数组的数据
		ArraySpan<Type> span(UInt index = 0, UInt size = UINF);
		
		/// \~English @brief Get the iterator from the first element, then the elements can be modified
		/// \~Chinese @brief 取得从第一个元素的遍历器，可修改元素
		ArrayElem<Type> firstElem();

		/// \~English @brief Get the iterator starting from the last element, then the elements can be modified
		/// \~Chinese @brief 取得从最后一个元素开始的遍历器，可修改元素
		ArrayElem<Type> lastElem();

		/// \~English @brief Copy data from a sub-area of another array to a certain position of this array
		/// \~Chinese @brief 从另一个数组的某个子区域拷贝数据到本数组的某个位置
		/// \~English @param src Source data array
		/// \~Chinese @param src 源数据数组
		/// \~English @param srcRegion The region copied from the source array
		/// \~Chinese @param srcRegion 从源数组拷贝的区域
		/// \~English @param thisOffset The starting position of this array copied to
		/// \~Chinese @param thisOffset 拷贝至本数组的起始位置
		void copy(Array<Type> src, Region srcRegion, UInt thisOffset);

		/// \~English @brief Create a scalar array (contains only one element)
		/// \~Chinese @brief 创建一个标量数组（只含有一个元素）
		static Array<Type> scalar(Type element);

		/// \~English @brief [Unsafe] Create an array based on argument list. ArgType should be Int, UInt, Double, Char* or structure and class type, etc.
		/// \~Chinese @brief [非安全] 基于可变参数列表创建数组并赋值，ArgType应为Int，UInt，Double，Char*或结构体和类名等
		/// \~English @param size Array size
		/// \~Chinese @param size 数组大小
		/// \~English @param firstValue The first element value of the array, the following parameters are the element values of the ArgType type, and the total number of elements should be consistent with the size of the array
		/// \~Chinese @param firstValue 数组首个元素值，后面参数都为ArgType类型的元素值，总元素个数应与数组大小一致
		/// \~English @returns Created array
		/// \~Chinese @returns 创建的数组
		template <typename ArgType>
		static Array<Type> create(UInt size, ArgType firstValue, ...);

		/// \~English @brief [Unsafe] Create an uninitialized array with the specified size, need to call the "initialize" method afterwards to ensure that each element is initialized once and only once
		/// \~Chinese @brief [非安全] 创建一个具有指定大小但未初始化的数组，需要随后调用initialize方法确保对每个元素都初始化了仅一次
		/// \~English @param size The size of the array
		/// \~Chinese @param size 创建数组的大小
		/// \~English @returns Created array
		/// \~Chinese @returns 新创建的数组
		static Array<Type> createUninitialized(UInt size);

		/// \~English @brief [Unsafe] Initialize the element, you need to ensure that the index is within the range
		/// \~Chinese @brief [非安全] 初始化元素，需要确保序号在范围内
		void initialize(UInt index, const Type& val);
		
		/// \~English @brief Merge multiple arrays
		/// \~Chinese @brief 合并多个数组
		/// \~English @param arrs Multiple arrays to be merged
		/// \~Chinese @param arrs 将合并的多个数组
		/// \~English @returns The merged array
		/// \~Chinese @returns 合并后的数组
		static Array<Type> merge(Array<Array<Type> > arrs);

		/// \~English @brief Merge multiple array spans
		/// \~Chinese @brief 合并多个数组片段
		/// \~English @param spans Multiple array spans to be merged
		/// \~Chinese @param spans 将合并的多个数组片段
		/// \~English @returns The merged array
		/// \~Chinese @returns 合并后的数组
		static Array<Type> merge(Array<ArraySpan<Type> > spans);

	private:
		Bool isNull() { return FALSE; }
		Bool isValid() { return FALSE; }
	};

	/// \~English @brief Fixed-length array span, whose data bound to the external array data
	/// \~Chinese @brief 定长数组片段，数据绑定至外部数组数据
	template <typename Type> class ArraySpan
	{
	public:
		/// \~English @brief Create an empty span
		/// \~Chinese @brief 创建空片段
		ArraySpan();

		/// \~English @brief [Unsafe] Bind to external array data
		/// \~Chinese @brief [非安全] 绑定至外部数组数据
		ArraySpan(const Type *ptr, UInt size);

		/// \~English @brief Bind to data of spadas::Array object
		/// \~Chinese @brief 绑定至 spadas::Array 对象的数据
		ArraySpan(Array<Type>& sourceArray, UInt offset, UInt size);

		/// \~English @brief Bind to data of spadas::ArraySpan object
		/// \~Chinese @brief 绑定至 spadas::ArraySpan 对象的数据
		ArraySpan(ArraySpan<Type>& sourceSpan, UInt offset, UInt size);

		/// \~English @brief Copy constructor
		/// \~Chinese @brief 拷贝构造函数
		ArraySpan(const ArraySpan<Type>& span);
		
		/// \~English @brief Destructor
		/// \~Chinese @brief 析构函数
		~ArraySpan();

		/// \~English @brief Redirection
		/// \~Chinese @brief 重定向
		ArraySpan<Type>& operator =(const ArraySpan<Type>& span);

		/// \~English @brief Whether the source of the array span is a reference counting object
		/// \~Chinese @brief 定长数组片段的源是否为一个引用计数对象
		Bool isRefCounting();

		/// \~English @brief [Unsafe] Get the head pointer of the array span
		/// \~Chinese @brief [非安全] 取得数组片段的头指针
		Type *data();

		/// \~English @brief Get the size of the array span
		/// \~Chinese @brief 取得数组片段大小
		UInt size();

		/// \~English @brief Is it an empty array span
		/// \~Chinese @brief 是否为空数组片段
		Bool isEmpty();

		/// \~English @brief Clone a new object (Call operator "=" for all elements)
		/// \~Chinese @brief 克隆出一个新对象 (所有元素调用=号拷贝)
		Array<Type> clone();

		/// \~English @brief Get a reference to an element in the array span
		/// \~Chinese @brief 取得数组片段中某个元素的引用
		Type& operator [](UInt index);

		/// \~English @brief Get a reference to the first element in the array span
		/// \~Chinese @brief 取得数组片段中第一个元素的引用
		Type& first();

		/// \~English @brief Get a reference to the last element in the array span
		/// \~Chinese @brief 取得数组片段中最后一个元素的引用
		Type& last();

		/// \~English @brief Concatenate another array after this array span
		/// \~Chinese @brief 在本数组片段后方拼接另一个数组
		/// \~English @param arr Another array to be concatenated
		/// \~Chinese @param arr 将拼接的另一个数组
		/// \~English @returns Concatenated array
		/// \~Chinese @returns 拼接后的数组
		Array<Type> operator +(Array<Type> arr);

		/// \~English @brief Reduce the size of the array span, and return this span object
		/// \~Chinese @brief 缩减数组片段大小，并返回本片段对象
		ArraySpan<Type>& trim(UInt size);
		
		/// \~English @brief Assign the same value to all elements
		/// \~Chinese @brief 对所有元素赋同一个值
		void set(Type value);
		
		/// \~English @brief Whether the array contains a certain value
		/// \~Chinese @brief 数组中是否包含某个值
		Bool contain(Type val);

		/// \~English @brief Whether the array contains a value that the specified function returns TRUE
		/// \~Chinese @brief 数组中是否包含指定函数返回TRUE的某个值
		Bool containAs(Func<Bool(Type&)> func);

		/// \~English @brief Returns the indices of all elements in the array equal to a certain value
		/// \~Chinese @brief 返回数组中所有等于某个值的元素序号
		Array<UInt> search(Type val);

		/// \~English @brief Returns the indices of all elements in the array that the specified function returns TRUE
		/// \~Chinese @brief 返回数组中所有指定函数返回TRUE的元素序号
		Array<UInt> searchAs(Func<Bool(Type&)> func);

		/// \~English @brief Sort from small to large, need Type supports ">" operator
		/// \~Chinese @brief 按从小到大排序，需要Type支持>重载符
		void sort();

		/// \~English @brief According to the specified function (returns whether it is greater than), sort from small to large
		/// \~Chinese @brief 根据指定函数(判断是否大于)，按从小到大排序
		void sortAs(Func<Bool(Type&, Type&)> func);

		/// \~English @brief Convert to other types of arrays according to the specified function
		/// \~Chinese @brief 按指定函数转换为其他类型数组
		template <typename TargetType>
		Array<TargetType> convert(Func<TargetType(Type&)> func);
		
		/// \~English @brief Divide into multiple arrays according to the specified size
		/// \~Chinese @brief 根据指定的大小分割为多个数组
		/// \~English @param sizes The size of each array that will be split
		/// \~Chinese @param sizes 将分割成的每个数组的大小
		/// \~English @returns Split multiple arrays
		/// \~Chinese @returns 分割后的多个数组
		Array<ArraySpan<Type> > split(Array<UInt> sizes);

		/// \~English @brief Convert to an array of base class or derived class objects (See spadas::Object::as )
		/// \~Chinese @brief 转换为基类或派生类对象的数组（具体行为参考 spadas::Object::as ）
		template <typename TargetType>
		Array<TargetType> asArray();

		/// \~English @brief Convert to an array of base class or derived class objects, and output an array of whether each element is converted successfully (See spadas::Object::as )
		/// \~Chinese @brief 转换为基类或派生类对象的数组，并输出每个元素是否转换成功的数组（具体行为参考 spadas::Object::as ）
		template <typename TargetType>
		Array<TargetType> asArray(Array<Bool>& ok);

		/// \~English @brief Get the sub-array, which will be bound to the data of source array
		/// \~Chinese @brief 取得子数组，其数据绑定至原数组的数据
		ArraySpan<Type> span(UInt index = 0, UInt size = UINF);

	private:
		ULong source;
		UInt idx;
		UInt siz;
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

		/// \~English @brief Get the reference of the data
		/// \~Chinese @brief 取得数据的引用
		Type& value();

		/// \~English @brief Use a field or method of the data object
		/// \~Chinese @brief 使用数据的字段或方法
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

		/// \~English @brief Get the reference of the data
		/// \~Chinese @brief 取得数据的引用
		Type& value();

		/// \~English @brief Use a field or method of the data object
		/// \~Chinese @brief 使用数据的字段或方法
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
		
		/// \~English @brief Get the reference of the data
		/// \~Chinese @brief 取得数据的引用
		NType& value();

		/// \~English @brief Use a field or method of the data object
		/// \~Chinese @brief 使用数据的字段或方法
		NType* operator ->();

		/// \~English @brief Get the number of links on this node
		/// \~Chinese @brief 取得本节点上链接的数目
		UInt nLinks();
		
		/// \~English @brief Get the data reference on a link by the index, which starts from 0
		/// \~Chinese @brief 由序号取得某个链接上的数据引用 (序号从0开始)
		LType& linkAt(UInt index);
		
		/// \~English @brief Obtain the data reference on the link with the specified node, if there is no such link, an error will be reported
		/// \~Chinese @brief 取得与指定节点间的链接上的数据引用，若无此链接则报错
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
		static String typeName();

		/// \~English @brief Create an empty array, each segment size is 16
		/// \~Chinese @brief 创建一个空数组，每段大小为16
		ArrayX();
		
		/// \~English @brief Create an expandable array with the specified segment size (should be a power of 2, ranging from 1 to 65536)
		/// \~Chinese @brief 以指定段大小（应为2的幂数，范围在1~65536内）创建可扩展数组
		ArrayX(UInt segmentSize);

		/// \~English @brief Create an expandable array with the specified segment size (should be a power of 2, ranging from 1 to 65536) and the default value
		/// \~Chinese @brief 以指定段大小（应为2的幂数，范围在1~65536内）和默认值创建可扩展数组
		ArrayX(UInt segmentSize, Type defaultValue);
		
		/// \~English @brief Obtain a reference to an element in the array (if it's over the boundary of the array, it will expand the size to index+1 before accessing)
		/// \~Chinese @brief 取得数组中某个元素的引用 (若越过数组边界，将先扩展大小至序号+1再访问)
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

		/// \~English @brief Expand 1 element at the end of the array, and return reference of the new element
		/// \~Chinese @brief 在数组末尾扩展1个元素，并返回数组中该元素的引用
		Type& append(Type val);

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

	/// \~English @brief Linked list element iterator
	/// \~Chinese @brief 链表元素遍历器
	template <typename Type> class ListElem
	{
	public:
		/// \~English @brief Constructor, generated by calling List::head and List::tail
		/// \~Chinese @brief 构造函数，由 List::head 和 List::tail 调用生成
		ListElem(List<Type> list, Pointer cell, UInt index);

		/// \~English @brief Copy constructor
		/// \~Chinese @brief 拷贝构造函数
		ListElem(const ListElem<Type>& elem);

		/// \~English @brief Destructor
		/// \~Chinese @brief 析构函数
		~ListElem();

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

		/// \~English @brief Get a reference to the value
		/// \~Chinese @brief 取得值的引用
		Type& value();

		/// \~English @brief Use a field or method of the value
		/// \~Chinese @brief 使用值的字段或方法
		Type* operator ->();

		/// \~English @brief Get a reference to the previous element value
		/// \~Chinese @brief 取得上一个元素值的引用
		Type& previous();

		/// \~English @brief Get a reference to the next element value
		/// \~Chinese @brief 取得下一个元素值的引用
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

		/// \~English @brief Insert a value before the current element, and return reference of the new element
		/// \~Chinese @brief 在当前元素前插入新元素，并返回链表中该元素的引用
		Type& insertPrevious(Type val);

		/// \~English @brief Insert value after current element, and return reference of the new element
		/// \~Chinese @brief 在当前元素后插入新元素，并返回链表中该元素的引用
		Type& insertNext(Type val);

		/// \~English @brief Remove the current element
		/// \~Chinese @brief 移除当前元素
		void remove();
	
	private:
		List<Type> list;
		Pointer cell;
		Pointer prevCell;
		Pointer nextCell;
		UInt idx;
		UInt prevIndex;
		UInt nextIndex;
		ListElem<Type>& operator =(const ListElem<Type>& elem) { return *this; }
	};

	/// \~English @brief Variables of spadas::List template class
	/// \~Chinese @brief spadas::List 模板类的变量数据
	template <typename Type> class ListVars;
	
	/// \~English @brief Linked list
	/// \~Chinese @brief 链表
	template <typename Type> class List : public Object<ListVars<Type> >
	{
	public:
		static String typeName();

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

		/// \~English @brief Add as the first, and return reference of the new element
		/// \~Chinese @brief 添加为首个元素，并返回链表中该元素的引用
		Type& addToHead(Type val);

		/// \~English @brief Add as the last, and return reference of the new element
		/// \~Chinese @brief 添加为末尾元素，并返回链表中该元素的引用
		Type& addToTail(Type val);

		/// \~English @brief Remove the first
		/// \~Chinese @brief 移除首个元素
		void removeHead();

		/// \~English @brief Remove the last
		/// \~Chinese @brief 移除末尾元素
		void removeTail();

		/// \~English @brief Remove all elements equal to a certain value
		/// \~Chinese @brief 移除等于某个值的所有元素
		void remove(Type val);

		/// \~English @brief Remove all elements that satisfy the condition (the specified function returns TRUE)
		/// \~Chinese @brief 移除满足条件(指定函数返回TRUE)的所有元素
		void removeAs(Func<Bool(Type&)> func);

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
		static String typeName();

		/// \~English @brief Create a data stream with a capacity of 1, and its elements can be discarded
		/// \~Chinese @brief 创建容量为1的数据流，且其元素可丢弃
		Stream();
		
		/// \~English @brief Create a data stream with a specified capacity, and specify whether its elements can be discarded
		/// \~Chinese @brief 创建指定容量的数据流，并指定其元素是否可丢弃
		Stream(UInt capacity, Bool discardable = TRUE);

		/// \~English @brief Get the capacity of the data stream
		/// \~Chinese @brief 取得数据流的容量
		UInt capacity();

		/// \~English @brief Get the number of buffered elements in this data stream
		/// \~Chinese @brief 取得本数据流中缓存的元素数目
		UInt nElements();

		/// \~English @brief This stream is not bufferring any elements
		/// \~Chinese @brief 本数据流未缓存任何元素
		Bool isEmpty();

		/// \~English @brief Get the number of discarded elements in this data stream
		/// \~Chinese @brief 获得本数据流已丢弃的元素数目
		UInt nDiscarded();

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

		/// \~English @brief Dequeue one element, and return whether successfully dequeued
		/// \~Chinese @brief 取出一个元素，返回是否成功
		Bool dequeueOne(Type& elem);

		/// \~English @brief Try to dequeue the specified number of elements, the actual dequeued number is subject to the returned object (the element with index 0 in the returned array is the earliest)
		/// \~Chinese @brief 尝试取出指定数量的元素，实际取出数量以返回对象为准 (返回的数组中序号0的元素为最早)
		Array<Type> dequeue(UInt amount = UINF);

		/// \~English @brief Try to dequeue all consecutive elements that satisfy the condition (the element with index 0 in the returned array is the earliest)
		/// \~Chinese @brief 尝试取出所有满足条件(指定函数返回TRUE)的连续元素 (返回的数组中序号0的元素为最早)
		Array<Type> dequeueAs(Func<Bool(Type&)> func);

		/// \~English @brief Wait for all elements to be dequeued (return FALSE if interrupt is detected)
		/// \~Chinese @brief 等待所有元素取出 (若检测到interrupt则返回FALSE)
		Bool waitAllDequeued(Flag interrupt, Bool spin);

		/// \~English @brief Terminates the stream, making it "terminated". No elements can be enqueued after termination, but can be dequeued
		/// \~Chinese @brief 终止数据流，使其变为"已终止状态"。终止后将无法推入任何元素，但可取出
		void terminate();

		/// \~English @brief Whether this data stream is "terminated"
		/// \~Chinese @brief 本数据流是否为"已终止状态"
		Bool isTerminated();

		/// \~English @brief Reset the data stream: All elements will be cleared, the "terminated" state will be removed, and statistics such as nEnqueued will be cleared. But it will not change the capacity and "discardable" attribute
		/// \~Chinese @brief 重置数据流：将清空所有缓存中的元素，移除"已终止状态"，并清除nEnqueued等统计数据。但不会改变数据流容量和"元素可丢弃属性"
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
		Bool operator >(KeyValue<KeyType, ValueType> target) { return key > target.key; }

		/// \~English @brief Whether it is less than or not, KeyType needs to support operator "<"
		/// \~Chinese @brief 是否小于，需要KeyType支持<重载符
		Bool operator <(KeyValue<KeyType, ValueType> target) { return key < target.key; }
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
		static String typeName();

		/// \~English @brief Create an empty mapping table, the default number of buckets is 256
		/// \~Chinese @brief 创建空的映射表，默认bucket个数为256
		Map();

		/// \~English @brief Create a mapping table with the specified number of buckets (should be a power of 2, within the range of 16~65536)
		/// \~Chinese @brief 创建指定bucket个数的映射表（应为2的幂数，范围在16~65536内）
		Map(UInt bucketCount);

		/// \~English @brief [Unsafe] Create a mapping table and assign values based on the argument list. KeyArgType and ValueArgType should be Int, UInt, Double, Char* or structure and class type, etc.
		/// \~Chinese @brief [非安全] 基于可变参数列表创建映射表并赋值，KeyArgType和ValueArgType应为Int，UInt，Double，Char*或结构体和类名等
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

		/// \~English @brief Weather it's an empty mapping table
		/// \~Chinese @brief 是否为空的映射表
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

		/// \~English @brief Get the value reference corresponding to the specified key, if it does not exist, create a new mapping
		/// \~Chinese @brief 获得指定键对应的值引用，若不存在则新建映射
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

		/// \~English @brief [Unsafe] Create a dictionary based on argument list. ArgType should be Int, UInt, Double, Char* or structure and class type, etc.
		/// \~Chinese @brief [非安全] 基于可变参数列表创建字典并赋值，ArgType应为Int，UInt，Double，Char*或结构体和类名等
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
		static String typeName();

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
		Binary(const Byte *arr, UInt size);

		/// \~English @brief [Unsafe] Create a binary data block based on the argument list
		/// \~Chinese @brief [非安全] 基于可变参数列表创建二进制数据块并赋值
		/// \~English @param size The size of the data block (in bytes)
		/// \~Chinese @param size 数据块大小（字节单位）
		/// \~English @param firstByte The first byte value, the following parameters are all values ranging from 0 to 255, the total number of bytes should be consistent with the size of the data block
		/// \~Chinese @param firstByte 首个字节值，后面参数都为范围在0～255的值，总字节个数应与数据块大小一致
		static Binary create(UInt size, UInt firstByte, ...);

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

		/// \~English @brief [Unsafe] Get the head pointer of the data block
		/// \~Chinese @brief [非安全] 取得数据块的头指针
		Byte *data();

		/// \~English @brief Get the data block size
		/// \~Chinese @brief 取得数据块大小
		UInt size();

		/// \~English @brief Whether it's an empty data block
		/// \~Chinese @brief 数据是否为空
		Bool isEmpty();

		/// \~English @brief Clone a new object
		/// \~Chinese @brief 克隆出一个新对象
		Binary clone();

		/// \~English @brief Get a reference to a byte
		/// \~Chinese @brief 取得某个字节的引用
		Byte& operator [](UInt index);

		/// \~English @brief Concatenate another data block after this data block
		/// \~Chinese @brief 在本数据块后拼接另一个数据块
		/// \~English @param bin Another data block to be concatenated
		/// \~Chinese @param bin 将拼接的另一个数据块
		/// \~English @returns Concatenated data block
		/// \~Chinese @returns 拼接后的数据块
		Binary operator +(Binary bin);

		/// \~English @brief Reduce the data block size, and return this object
		/// \~Chinese @brief 缩减数据块大小，并返回本对象
		Binary& trim(UInt size);

		/// \~English @brief Assign the same value to all bytes
		/// \~Chinese @brief 对所有字节赋同一个值
		void set(Byte val);

		/// \~English @brief Divide into multiple data spans according to the specified size
		/// \~Chinese @brief 根据指定的大小分割为多个数据片段
		/// \~English @param sizes Size of each data span (in bytes)
		/// \~Chinese @param sizes 将分割成的每个数据片段的大小（字节单位）
		/// \~English @returns Multiple data spans after splitted
		/// \~Chinese @returns 分割后的多个数据片段
		Array<BinarySpan> split(Array<UInt> sizes);
		
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

		/// \~English @brief Obtain a sub-data block whose data is bound to the data of this data block
		/// \~Chinese @brief 取得子数据块，其数据绑定至本数据块的数据
		BinarySpan span(UInt index = 0, UInt size = UINF);

		/// \~English @brief Copy data from a sub-area of another data block to a certain position of this data block
		/// \~Chinese @brief 从另一个数据块的某个子区域拷贝数据到本数据块的某个位置
		/// \~English @param src Source data block
		/// \~Chinese @param src 源数据数组
		/// \~English @param srcRegion The area copied from the source
		/// \~Chinese @param srcRegion 从源数组拷贝的区域
		/// \~English @param dstOffset The starting position of this data block copied to
		/// \~Chinese @param dstOffset 拷贝至本数据块的起始位置
		void copy(Binary src, Region srcRegion, UInt dstOffset);

		/// \~English @brief Merge multiple data blocks
		/// \~Chinese @brief 合并多个数据块
		/// \~English @param binaries Multiple data blocks to be merged
		/// \~Chinese @param binaries 将合并的多个数据块
		/// \~English @returns The merged data block
		/// \~Chinese @returns 合并后的数据块
		static Binary merge(Array<Binary> binaries);

	private:
		Bool isNull() { return FALSE; }
		Bool isValid() { return FALSE; }
	};

	/// \~English @brief Span of binary data, bound to the external byte array
	/// \~Chinese @brief 二进制数据片段，绑定至外部字节数组
	class SPADAS_API BinarySpan
	{
	public:
		/// \~English @brief Create empty span
		/// \~Chinese @brief 创建空片段
		BinarySpan();

		/// \~English @brief [Unsafe] Bind to external byte array
		/// \~Chinese @brief [非安全] 绑定至外部字节数组
		BinarySpan(const Byte *ptr, UInt size);

		/// \~English @brief Bind to data of spadas::Binary object
		/// \~Chinese @brief 绑定至 spadas::Binary 对象的数据
		BinarySpan(Binary& sourceBinary, UInt offset, UInt size);

		/// \~English @brief Bind to data of spadas::BinarySpan object
		/// \~Chinese @brief 绑定至 spadas::BinarySpan 对象的数据
		BinarySpan(BinarySpan& sourceSpan, UInt offset, UInt size);

		/// \~English @brief Copy constructor
		/// \~Chinese @brief 拷贝构造函数
		BinarySpan(const BinarySpan& span);
		
		/// \~English @brief Destructor
		/// \~Chinese @brief 析构函数
		~BinarySpan();

		/// \~English @brief Redirection
		/// \~Chinese @brief 重定向
		BinarySpan& operator =(const BinarySpan& span);

		/// \~English @brief Whether the source of the span is a reference counting object
		/// \~Chinese @brief 数据片段的源是否为一个引用计数对象
		Bool isRefCounting();

		/// \~English @brief [Unsafe] Get the head pointer of the data span
		/// \~Chinese @brief [非安全] 取得数据片段的头指针
		Byte *data();

		/// \~English @brief Get the data span size
		/// \~Chinese @brief 取得数据片段大小
		UInt size();

		/// \~English @brief Whether it's an empty data span
		/// \~Chinese @brief 数据片段是否为空
		Bool isEmpty();

		/// \~English @brief Clone a new object
		/// \~Chinese @brief 克隆出一个新对象
		Binary clone();

		/// \~English @brief Get a reference to a byte
		/// \~Chinese @brief 取得某个字节的引用
		Byte& operator [](UInt index);

		/// \~English @brief Concatenate another data block after this data span
		/// \~Chinese @brief 在本数据片段后拼接另一个数据块
		/// \~English @param bin Another data block to be concatenated
		/// \~Chinese @param bin 将拼接的另一个数据块
		/// \~English @returns Concatenated data block
		/// \~Chinese @returns 拼接后的数据块
		Binary operator +(Binary bin);

		/// \~English @brief Reduce the data span size, and return this object
		/// \~Chinese @brief 缩减数据片段大小，并返回本对象
		BinarySpan& trim(UInt size);

		/// \~English @brief Assign the same value to all bytes
		/// \~Chinese @brief 对所有字节赋同一个值
		void set(Byte val);

		/// \~English @brief Divide into multiple data spans according to the specified size
		/// \~Chinese @brief 根据指定的大小分割为多个数据片段
		/// \~English @param sizes Size of each data span (in bytes)
		/// \~Chinese @param sizes 将分割成的每个数据片段的大小（字节单位）
		/// \~English @returns Multiple data spans after splitted
		/// \~Chinese @returns 分割后的多个数据片段
		Array<BinarySpan> split(Array<UInt> sizes);
		
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

		/// \~English @brief Get the sub-data span whose data is bound to the data of the original data block
		/// \~Chinese @brief 取得子数据片段，其数据绑定至原数据块的数据
		BinarySpan span(UInt index = 0, UInt size = UINF);

	private:
		ULong source;
		UInt idx;
		UInt siz;
	};

	// String / 字符串 //////////////////////////////////////////////////////////////

	/// \~English @brief String
	/// \~Chinese @brief 字符串
	class SPADAS_API String : public Object<class StringVars>
	{
	public:
		SPADAS_TYPE("spadas.String")

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
		/// \~English @param nDigits Specifies the number of digits, ranging from 0 to 10
		/// \~Chinese @param nDigits 指定位数，范围为0~10
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
		/// \~English @param nDigits Specifies the number of digits, ranging from 0 to 20
		/// \~Chinese @param nDigits 指定位数，范围为0~20
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
		/// \~English @param nDigits The number of decimal places, ranging from 0 to 9
		/// \~Chinese @param nDigits 保留小数位数，范围为0~9
		String(Float val, UInt nDigits);

		/// \~English @brief Initialize by spadas::Double, specifying the number of decimal places
		/// \~Chinese @brief 由 spadas::Double 初始化，并指定保留小数位数
		/// \~English @param val Input 64-bit floating point number
		/// \~Chinese @param val 输入64位浮点数
		/// \~English @param nDigits The number of decimal places, ranging from 0 to 18
		/// \~Chinese @param nDigits 保留小数位数，范围为0~18
		String(Double val, UInt nDigits);

		/// \~English @brief Create a string object from UTF-8 binary data
		/// \~Chinese @brief 从UTF-8二进制数据创建字符串对象
		/// \~English @param binary UTF-8 binary data (not required to be terminated with 0)
		/// \~Chinese @param binary UTF-8二进制数据（不要求以0结尾）
		String(Binary binary);

		/// \~English @brief Create a string object from UTF-8 binary span
		/// \~Chinese @brief 从UTF-8二进制数据片段创建字符串对象
		/// \~English @param span UTF-8 binary span (not required to be terminated with 0)
		/// \~Chinese @param span UTF-8二进制数据片段（不要求以0结尾）
		String(BinarySpan span);

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

		/// \~English @brief [Unsafe] Obtain the head pointer of the string UTF-8 data (read-only and NOT terminated by 0)
		/// \~Chinese @brief [非安全] 取得字符串UTF-8数据的头指针 (只读且不以0结尾)
		const Byte *bytes();

		/// \~English @brief Get the string length (UTF-8 bytes)
		/// \~Chinese @brief 取得字符串长度 (UTF-8字节数)
		UInt length();

		/// \~English @brief Is it an empty string
		/// \~Chinese @brief 是否为空字符串
		Bool isEmpty();

		/// \~English @brief Get the hash value
		/// \~Chinese @brief 获取哈希值
		Word getHash();

		/// \~English @brief Clone a new object
		/// \~Chinese @brief 克隆出一个新对象
		String clone();

		/// \~English @brief Convert to a Char array, ending with 0 (so the effective length is the array length - 1)
		/// \~Chinese @brief 转换为Char数组，以0结尾（因此有效长度为数组长度-1）
		Array<Char> chars();

		/// \~English @brief Convert to a WChar array, terminated with 0 (so the effective length is the array length - 1)
		/// \~Chinese @brief 转换为WChar数组，以0结尾（因此有效长度为数组长度-1）
		Array<WChar> wchars();

		/// \~English @brief Concatenate another string after this string (The data of this object will not be changed, and multiple strings can be added in succession)
		/// \~Chinese @brief 在本字符串后拼接另一个字符串（不会更改本对象数据，且可多个字符串连加）
		/// \~English @param string Another string to be concatenated
		/// \~Chinese @param string 将拼接的另一个字符串
		/// \~English @returns String appender object
		/// \~Chinese @returns 字符串拼接器
		StringAppender operator +(String string);

		/// \~English @brief Convert to spadas::Int number and return it
		/// \~Chinese @brief 转换并返回 spadas::Int 数字
		Optional<Int> toInt();

		/// \~English @brief Convert to spadas::Long number and return it
		/// \~Chinese @brief 转换并返回 spadas::Long 数字
		Optional<Long> toLong();

		/// \~English @brief Convert to spadas::Float value and return it
		/// \~Chinese @brief 转换并返回 spadas::Float 数值
		Optional<Float> toFloat();

		/// \~English @brief Convert to spadas::Double value and return it
		/// \~Chinese @brief 转换并返回 spadas::Double 数值
		Optional<Double> toDouble();

		/// \~English @brief Convert and output spadas::Int number, return whether the conversion is successful
		/// \~Chinese @brief 转换并输出 spadas::Int 数字，返回是否转换成功
		Bool toNumber(Int& number);

		/// \~English @brief Convert and output spadas::Long number, return whether the conversion is successful
		/// \~Chinese @brief 转换并输出 spadas::Long 数字，返回是否转换成功
		Bool toNumber(Long& number);

		/// \~English @brief Convert and output spadas::Float number, return whether the conversion is successful
		/// \~Chinese @brief 转换并输出 spadas::Float 数值，返回是否转换成功
		Bool toNumber(Float& number);

		/// \~English @brief Convert and output spadas::Double number, return whether the conversion is successful
		/// \~Chinese @brief 转换并输出 spadas::Double 数值，返回是否转换成功
		Bool toNumber(Double& number);
		
		/// \~English @brief Convert to UTF-8 binary data (not terminated with 0)
		/// \~Chinese @brief 转换为UTF-8二进制数据块（不以0结尾）
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

		/// \~English @brief Search the target string from the first character, and return the index of the first character of the first found target
		/// \~Chinese @brief 从前往后搜索目标字符串，返回首个发现目标的首字符位置
		UInt searchFirst(String target);

		/// \~English @brief Search the target string from the last character, and return the index of the first character of the first found target
		/// \~Chinese @brief 从后往前搜索目标字符串，返回首个发现目标的首字符位置
		UInt searchLast(String target);

		/// \~English @brief Remove all spaces, tabs, returns, and newlines before and after, and return a new string
		/// \~Chinese @brief 移除前后所有空格、制表符、回车、换行，并返回新字符串
		String trim();
		
		/// \~English @brief Split this string with the specified string. For example, "12 34 56" is separated by spaces, and returns {"12", "34", "56"}. Note that when the string does not contain target, an empty array will be returned if the string is empty, and a scalar array will be returned if it is not empty
		/// \~Chinese @brief 用指定字符串对本字符串进行分割。如"12 34 56"按空格符分割，返回{"12", "34", "56"}。注意，本字符串不含target时，若本字符串为空则返回空数组，非空则返回标量数组
		Array<StringSpan> split(String target);

		/// \~English @brief Replace the oldString part of this string with newString, and return the replaced string
		/// \~Chinese @brief 将本字符串中oldString部分替换为newString，并返回替换后的字符串
		String replace(String oldString, String newString);

		/// \~English @brief Get a substring whose data is bound to the data of this string
		/// \~Chinese @brief 取得子字符串，其数据绑定至本字符串的数据
		/// \~English @param index The starting position of the substring in this string
		/// \~Chinese @param index 子字符串在本字符串的起始位置
		/// \~English @param length Substring length
		/// \~Chinese @param length 子字符串长度
		/// \~English @returns Substring
		/// \~Chinese @returns 子字符串
		StringSpan span(UInt index = 0, UInt length = UINF);

		/// \~English @brief Concatenate another string after this string (This will change the data of this object)
		/// \~Chinese @brief 在本字符串后拼接另一个字符串(将更改本对象数据)
		/// \~English @param string Another string to be concatenated
		/// \~Chinese @param string 拼接的另一个字符串
		void operator +=(String string);

		/// \~English @brief Concatenate a string span after this string (This will change the data of this object)
		/// \~Chinese @brief 在本字符串后拼接另一个字符串片段(将更改本对象数据)
		/// \~English @param string String span to be concatenated
		/// \~Chinese @param string 拼接的另一个字符串片段
		void operator +=(StringSpan span);

		/// \~English @brief Concatenate a spadas::Char single character after this string (This will change the data of this object)
		/// \~Chinese @brief 在本字符串后拼接 spadas::Char 单字符 (将更改本对象数据)
		/// \~English @param character Character to be concatenated
		/// \~Chinese @param character 拼接的字符
		void operator +=(Char character);

		/// \~English @brief Concatenate a spadas::WChar single character after this string (This will change the data of this object)
		/// \~Chinese @brief 在本字符串后拼接 spadas::WChar 单字符 (将更改本对象数据)
		/// \~English @param character Character to be concatenated
		/// \~Chinese @param character 拼接的字符
		void operator +=(WChar character);

		/// \~English @brief Concatenate spadas::Char characters after this string (This will change the data of this object)
		/// \~Chinese @brief 在本字符串后拼接 spadas::Char 字符数组指针 (将更改本对象数据)
		/// \~English @param text Characters to be concatenated
		/// \~Chinese @param text 拼接的字符数组指针（以0结尾）
		void operator +=(Char text[]);

		/// \~English @brief Concatenate spadas::Char characters after this string (This will change the data of this object)
		/// \~Chinese @brief 在本字符串后拼接 spadas::Char 字符数组指针 (将更改本对象数据)
		/// \~English @param text Characters to be concatenated
		/// \~Chinese @param text 拼接的字符数组指针（以0结尾）
		void operator +=(const Char text[]);

		/// \~English @brief Concatenate spadas::WChar characters after this string (This will change the data of this object)
		/// \~Chinese @brief 在本字符串后拼接 spadas::WChar 字符数组指针 (将更改本对象数据)
		/// \~English @param text Characters to be concatenated
		/// \~Chinese @param text 拼接的字符数组指针（以0结尾）
		void operator +=(WChar text[]);

		/// \~English @brief Concatenate spadas::WChar characters after this string (This will change the data of this object)
		/// \~Chinese @brief 在本字符串后拼接 spadas::WChar 字符数组指针 (将更改本对象数据)
		/// \~English @param text Characters to be concatenated
		/// \~Chinese @param text 拼接的字符数组指针（以0结尾）
		void operator +=(const WChar text[]);

		/// \~English @brief Concatenate spadas::Char array after this string (This will change the data of this object)
		/// \~Chinese @brief 在本字符串后拼接 spadas::Char 字符数组 (将更改本对象数据)
		/// \~English @param text Character array to be concatenated
		/// \~Chinese @param text 拼接的字符数组（不要求以0结尾）
		void operator +=(Array<Char> text);

		/// \~English @brief Concatenate spadas::WChar character array after this string (This will change the data of this object)
		/// \~Chinese @brief 在本字符串后拼接 spadas::WChar 字符数组 (将更改本对象数据)
		/// \~English @param text Character array to be concatenated
		/// \~Chinese @param text 拼接的字符数组（不要求以0结尾）
		void operator +=(Array<WChar> text);

		/// \~English @brief Concatenate a spadas::Bool value after this string (This will change the data of this object)
		/// \~Chinese @brief 在本字符串后拼接 spadas::Bool (将更改本对象数据)
		/// \~English @param val Character to be concatenated
		/// \~Chinese @param val 拼接的布尔值
		void operator +=(Bool val);

		/// \~English @brief Concatenate a spadas::Byte value after this string (This will change the data of this object)
		/// \~Chinese @brief 在本字符串后拼接 spadas::Byte (将更改本对象数据)
		/// \~English @param val 8-bit unsigned integer to be concatenated
		/// \~Chinese @param val 拼接的8位无符号整数
		void operator +=(Byte val);

		/// \~English @brief Concatenate a spadas::Word value after this string (This will change the data of this object)
		/// \~Chinese @brief 在本字符串后拼接 spadas::Word (将更改本对象数据)
		/// \~English @param val 16-bit unsigned integer to be concatenated
		/// \~Chinese @param val 拼接的16位无符号整数
		void operator +=(Word val);

		/// \~English @brief Concatenate a spadas::UInt value after this string (This will change the data of this object)
		/// \~Chinese @brief 在本字符串后拼接 spadas::UInt (将更改本对象数据)
		/// \~English @param val 32-bit unsigned integer to be concatenated
		/// \~Chinese @param val 拼接的32位无符号整数
		void operator +=(UInt val);

		/// \~English @brief Concatenate a spadas::ULong value after this string (This will change the data of this object)
		/// \~Chinese @brief 在本字符串后拼接 spadas::ULong (将更改本对象数据)
		/// \~English @param val 64-bit unsigned integer to be concatenated
		/// \~Chinese @param val 拼接的64位无符号整数
		void operator +=(ULong val);

		/// \~English @brief Concatenate a spadas::Short value after this string (This will change the data of this object)
		/// \~Chinese @brief 在本字符串后拼接 spadas::Short (将更改本对象数据)
		/// \~English @param val 16-bit signed integer to be concatenated
		/// \~Chinese @param val 拼接的16位带符号整数
		void operator +=(Short val);

		/// \~English @brief Concatenate a spadas::Int value after this string (This will change the data of this object)
		/// \~Chinese @brief 在本字符串后拼接 spadas::Int (将更改本对象数据)
		/// \~English @param val 32-bit signed integer to be concatenated
		/// \~Chinese @param val 拼接的32位带符号整数
		void operator +=(Int val);

		/// \~English @brief Concatenate a spadas::Long value after this string (This will change the data of this object)
		/// \~Chinese @brief 在本字符串后拼接 spadas::Long (将更改本对象数据)
		/// \~English @param val 64-bit signed integer to be concatenated
		/// \~Chinese @param val 拼接的64位带符号整数
		void operator +=(Long val);

		/// \~English @brief Concatenate a spadas::Float value after this string (This will change the data of this object)
		/// \~Chinese @brief 在本字符串后拼接 spadas::Float (将更改本对象数据)
		/// \~English @param val 32-bit floating point number to be concatenated
		/// \~Chinese @param val 拼接的32位浮点数
		void operator +=(Float val);

		/// \~English @brief Concatenate a spadas::Double value after this string (This will change the data of this object)
		/// \~Chinese @brief 在本字符串后拼接 spadas::Double (将更改本对象数据)
		/// \~English @param val 64-bit floating point number to be concatenated
		/// \~Chinese @param val 拼接的64位浮点数
		void operator +=(Double val);

		/// \~English @brief Concatenate UTF-8 binary data after this string (This will change the data of this object)
		/// \~Chinese @brief 在本字符串后拼接UTF-8二进制数据 (将更改本对象数据)
		/// \~English @param binary UTF-8 binary data to be concatenated (not required to be terminated with 0)
		/// \~Chinese @param binary 拼接的UTF-8二进制数据（不要求以0结尾）
		void operator +=(Binary binary);

		/// \~English @brief Concatenate UTF-8 binary span after this string (This will change the data of this object)
		/// \~Chinese @brief 在本字符串后拼接UTF-8二进制数据片段 (将更改本对象数据)
		/// \~English @param span UTF-8 binary span to be concatenated (not required to be terminated with 0)
		/// \~Chinese @param span 拼接的UTF-8二进制数据片段（不要求以0结尾）
		void operator +=(BinarySpan span);

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

		/// \~English @brief Merge multiple string spans with the specified separator
		/// \~Chinese @brief 拼接多个字符串片段，片段间以指定分割符分割
		/// \~English @param strs Input multiple string spans
		/// \~Chinese @param strs 输入的多个字符串片段
		/// \~English @param separator Separator
		/// \~Chinese @param separator 分隔符
		/// \~English @returns The string concatenated by the specified separator
		/// \~Chinese @returns 按指定分隔符拼接的字符串
		static String merge(Array<StringSpan> spans, String separator = ", ");

		/// \~English @brief Merge multiple strings created by spadas::String constructor with the specified separator
		/// \~Chinese @brief 以String构造函数拼接数组，字符串间以指定分割符分割
		/// \~English @param strs Input array
		/// \~Chinese @param strs 输入数组
		/// \~English @param separator Separator
		/// \~Chinese @param separator 分隔符
		/// \~English @returns The string concatenated by the specified separator
		/// \~Chinese @returns 按指定分隔符拼接的字符串
		template <typename Type>
		static String merge(Array<Type> arr, String separator = ", ");

	private:
		Bool isNull() { return FALSE; }
		Bool isValid() { return FALSE; }
		void initBuffer(UInt dataSize);
		void ensureBuffer(UInt appendSize);
		static String mergeStrings(Array<String> strs, String separator);
	};

	/// \~English @brief String span, whose data bound to the external string data
	/// \~Chinese @brief 字符串片段，数据绑定至外部字符串数据
	class SPADAS_API StringSpan
	{
	public:
		/// \~English @brief Create empty span
		/// \~Chinese @brief 创建空片段
		StringSpan();

		/// \~English @brief [Unsafe] Bind to external string data (not required to be terminated with 0)
		/// \~Chinese @brief [非安全] 绑定至外部字符串数据（不要求以0结尾）
		StringSpan(const Byte *ptr, UInt length);

		/// \~English @brief Bind to data of spadas::String object
		/// \~Chinese @brief 绑定至 spadas::String 对象的数据
		StringSpan(String& sourceString, UInt offset, UInt length);

		/// \~English @brief Bind to data of spadas::StringSpan object
		/// \~Chinese @brief 绑定至 spadas::StringSpan 对象的数据
		StringSpan(StringSpan& sourceSpan, UInt offset, UInt length);

		/// \~English @brief Copy constructor
		/// \~Chinese @brief 拷贝构造函数
		StringSpan(const StringSpan& span);
		
		/// \~English @brief Destructor
		/// \~Chinese @brief 析构函数
		~StringSpan();

		/// \~English @brief Redirection
		/// \~Chinese @brief 重定向
		StringSpan& operator =(const StringSpan& span);

		/// \~English @brief Whether the source of the string span is a reference counting object
		/// \~Chinese @brief 字符串片段的源是否为一个引用计数对象
		Bool isRefCounting();

		/// \~English @brief Whether it is equal to
		/// \~Chinese @brief 是否等于
		Bool operator ==(StringSpan span);

		/// \~English @brief Whether it is not equal to
		/// \~Chinese @brief 是否不等于
		Bool operator !=(StringSpan span);

		/// \~English @brief Whether it is equal to
		/// \~Chinese @brief 是否等于
		Bool operator ==(String string);

		/// \~English @brief Whether it is not equal to
		/// \~Chinese @brief 是否不等于
		Bool operator !=(String string);

		/// \~English @brief Whether it is greater than, according to the system default string sorting order
		/// \~Chinese @brief 是否大于，按照系统默认字符串排序顺序
		Bool operator >(StringSpan span);

		/// \~English @brief Whether it is less than, according to the system default string sorting order
		/// \~Chinese @brief 是否小于，按照系统默认字符串排序顺序
		Bool operator <(StringSpan span);

		/// \~English @brief [Unsafe] Obtain the head pointer of the string span UTF-8 data (read-only and NOT terminated by 0)
		/// \~Chinese @brief [非安全] 取得字符串片段UTF-8数据的头指针 (只读且不以0结尾)
		const Byte *bytes();

		/// \~English @brief Get the string span length (UTF-8 bytes)
		/// \~Chinese @brief 取得字符串片段长度 (UTF-8字节数)
		UInt length();

		/// \~English @brief Is it an empty string span
		/// \~Chinese @brief 是否为空字符串片段
		Bool isEmpty();

		/// \~English @brief Get the hash value
		/// \~Chinese @brief 获取哈希值
		Word getHash();

		/// \~English @brief Clone a new string object
		/// \~Chinese @brief 克隆出一个新字符串对象
		String clone();

		/// \~English @brief Convert to a Char array, ending with 0 (so the effective length is the array length - 1)
		/// \~Chinese @brief 转换为Char数组，以0结尾（因此有效长度为数组长度-1）
		Array<Char> chars();

		/// \~English @brief Convert to a WChar array, terminated with 0 (so the effective length is the array length - 1)
		/// \~Chinese @brief 转换为WChar数组，以0结尾（因此有效长度为数组长度-1）
		Array<WChar> wchars();

		/// \~English @brief Concatenate another string after this string span (The data of this object will not be changed, and multiple strings can be added in succession)
		/// \~Chinese @brief 在本字符串片段后拼接另一个字符串（不会更改本对象数据，且可多个字符串连加）
		/// \~English @param string Another string to be concatenated
		/// \~Chinese @param string 将拼接的另一个字符串
		/// \~English @returns String appender object
		/// \~Chinese @returns 字符串拼接器
		StringAppender operator +(String string);

		/// \~English @brief Convert to spadas::Int number and return it
		/// \~Chinese @brief 转换并返回 spadas::Int 数字
		Optional<Int> toInt();

		/// \~English @brief Convert to spadas::Long number and return it
		/// \~Chinese @brief 转换并返回 spadas::Long 数字
		Optional<Long> toLong();

		/// \~English @brief Convert to spadas::Float value and return it
		/// \~Chinese @brief 转换并返回 spadas::Float 数值
		Optional<Float> toFloat();

		/// \~English @brief Convert to spadas::Double value and return it
		/// \~Chinese @brief 转换并返回 spadas::Double 数值
		Optional<Double> toDouble();

		/// \~English @brief Convert and output spadas::Int number, return whether the conversion is successful
		/// \~Chinese @brief 转换并输出 spadas::Int 数字，返回是否转换成功
		Bool toNumber(Int& number);

		/// \~English @brief Convert and output spadas::Long number, return whether the conversion is successful
		/// \~Chinese @brief 转换并输出 spadas::Long 数字，返回是否转换成功
		Bool toNumber(Long& number);

		/// \~English @brief Convert and output spadas::Float number, return whether the conversion is successful
		/// \~Chinese @brief 转换并输出 spadas::Float 数值，返回是否转换成功
		Bool toNumber(Float& number);

		/// \~English @brief Convert and output spadas::Double number, return whether the conversion is successful
		/// \~Chinese @brief 转换并输出 spadas::Double 数值，返回是否转换成功
		Bool toNumber(Double& number);
		
		/// \~English @brief Convert to UTF-8 binary data (not terminated with 0)
		/// \~Chinese @brief 转换为UTF-8二进制数据块（不以0结尾）
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
		
		/// \~English @brief Search the target string from the first character, and return the index of the first character of the first found target
		/// \~Chinese @brief 从前往后搜索目标字符串，返回首个发现目标的首字符位置
		UInt searchFirst(String target);

		/// \~English @brief Search the target string from the last character, and return the index of the first character of the first found target
		/// \~Chinese @brief 从后往前搜索目标字符串，返回首个发现目标的首字符位置
		UInt searchLast(String target);
		
		/// \~English @brief Remove all spaces, tabs, returns, and newlines before and after, and return a new string
		/// \~Chinese @brief 移除前后所有空格、制表符、回车、换行，并返回新字符串
		String trim();

		/// \~English @brief Split this string span with the specified string. For example, "12 34 56" is separated by spaces, and returns {"12", "34", "56"}. Note that when the string span does not contain target, an empty array will be returned if the string span is empty, and a scalar array will be returned if it is not empty
		/// \~Chinese @brief 用指定字符串对本字符串片段进行分割。如"12 34 56"按空格符分割，返回{"12", "34", "56"}。注意，本字符串片段不含target时，若本字符串片段为空则返回空数组，非空则返回标量数组
		Array<StringSpan> split(String target);

		/// \~English @brief Replace the oldString part of this string span with newString, and return the replaced string
		/// \~Chinese @brief 将本字符串片段中oldString部分替换为newString，并返回替换后的字符串
		String replace(String oldString, String newString);

		/// \~English @brief Get a substring whose data is bound to the data of this span's original string
		/// \~Chinese @brief 取得子字符串，其数据绑定至本字符串片段的原字符串数据
		/// \~English @param index The starting position of the substring in this string span
		/// \~Chinese @param index 子字符串在本字符串片段的起始位置
		/// \~English @param length Substring length
		/// \~Chinese @param length 子字符串长度
		/// \~English @returns Substring
		/// \~Chinese @returns 子字符串
		StringSpan span(UInt index = 0, UInt length = UINF);

	private:
		ULong source;
		UInt idx;
		UInt len;
	};

	/// \~English @brief string appender, used to speed up the "+" operator
	/// \~Chinese @brief 字符串拼接器，用于加速+运算符
	class SPADAS_API StringAppender
	{
	public:
		/// \~English @brief Default constructor
		/// \~Chinese @brief 默认构造函数
		StringAppender(String origin);

		/// \~English @brief Concatenate string
		/// \~Chinese @brief 拼接字符串
		template <typename Type>
		StringAppender& operator +(Type target);

		/// \~English @brief Convert to string object
		/// \~Chinese @brief 转字符串
		String toString();

	private:
		String str;
	};

	/// \~English @brief Convenience for l-value to concatenate string, see macro "cat"
	/// ~\Chinese @brief 方便左值拼接字符串，详见宏"cat"
	SPADAS_API StringAppender& operator +(StringAppender appender, String str);

	/// \~English @brief Convenience for l-value to concatenate string, see macro "cat"
	/// ~\Chinese @brief 方便左值拼接字符串，详见宏"cat"
	template <typename Type>
	StringAppender operator +(Type obj, String str);

	// Enumeration object / 枚举对象 //////////////////////////////////////////////////////////////

	/// \~English @brief [Multithread safe] Enumeration container. If you have high performance requirements, you should use Type::Value directly
	/// \~English @details Type is an enumeration class, which needs to be defined as follows: \n
	/// - With a definition of enum class "Value", which contains a "Default" enumeration value \n
	/// - With a macro definition SPADAS_ENUM_VALUES, passing in all enumeration values except "Default"
	/// \~Chinese @brief [多线程安全] 枚举类型容器。若有高性能要求，应直接使用Type::Value
	/// \~Chinese @details 其中Type为枚举类，该类需要定义如下： \n
	/// - 具有定义enum class Value，其中包含Default枚举值 \n
	/// - 具有宏定义SPADAS_ENUM_VALUES，传入Default以外所有枚举值
	template <typename Type> class Enum
	{
	public:
		/// \~English @brief Create an object with default value
		/// \~Chinese @brief 创建一个对象，使用默认值
		Enum();

		/// \~English @brief Create an object, using the specified value (if the value is invalid, use the default value)
		/// \~Chinese @brief 创建一个对象，使用指定值（若该值无效则使用默认值）
		Enum(typename Type::Value val);

		/// \~English @brief Whether it is equal to
		/// \~Chinese @brief 是否等于
		Bool operator ==(typename Type::Value val);

		/// \~English @brief Whether it is equal to
		/// \~Chinese @brief 是否等于
		Bool operator ==(Enum<Type> enumeration);

		/// \~English @brief Whether it is not equal to
		/// \~Chinese @brief 是否不等于
		Bool operator !=(typename Type::Value val);

		/// \~English @brief Whether it is not equal to
		/// \~Chinese @brief 是否不等于
		Bool operator !=(Enum<Type> enumeration);

		/// \~English @brief Get the hash value
		/// \~Chinese @brief 获取哈希值
		Word getHash();

		/// \~English @brief Get the value of the enumeration object
		/// \~Chinese @brief 取得枚举对象的值
		typename Type::Value value();

		/// \~English @brief Get the text representation of the enumeration object
		/// \~Chinese @brief 取得枚举对象的文本表述
		String toString();

	private:
		typename Type::Value val;
		const Char* str;
	};

	// Console / 控制台 //////////////////////////////////////////////////////////////

	/// \~English @brief Key enumeration type
	/// \~Chinese @brief 按键枚举类型
	class SPADAS_API Key
	{
	public:
		enum class Value
		{
			Default = 0,
			None = 0,
			Enter = 1,
			Space = 2,
			Back = 3,
			Esc = 4,
			Tab = 5,
			Shift = 6,
			Ctrl = 7,
			Insert = 8,
			Delete = 9,
			Home = 10,
			End = 11,
			PageUp = 12,
			PageDown = 13,
			Up = 14,
			Down = 15,
			Left = 16,
			Right = 17,
			Comma = 18,
			Period = 19,
			Key0 = 20,
			Key1 = 21,
			Key2 = 22,
			Key3 = 23,
			Key4 = 24,
			Key5 = 25,
			Key6 = 26,
			Key7 = 27,
			Key8 = 28,
			Key9 = 29,
			A = 30,
			B = 31,
			C = 32,
			D = 33,
			E = 34,
			F = 35,
			G = 36,
			H = 37,
			I = 38,
			J = 39,
			K = 40,
			L = 41,
			M = 42,
			N = 43,
			O = 44,
			P = 45,
			Q = 46,
			R = 47,
			S = 48,
			T = 49,
			U = 50,
			V = 51,
			W = 52,
			X = 53,
			Y = 54,
			Z = 55,
			F1 = 56,
			F2 = 57,
			F3 = 58,
			F4 = 59,
			F5 = 60,
			F6 = 61,
			F7 = 62,
			F8 = 63,
			Unknown = 100,
		};
		SPADAS_ENUM_VALUES(None, Enter, Space, Back, Esc, Tab, Shift, Ctrl, Insert, Delete, Home, End, PageUp, PageDown, Up, Down, Left, Right, Comma, Period, Key0, Key1, Key2, Key3, Key4, Key5, Key6, Key7, Key8, Key9, A, B, C, D, E, F, G, H, I, J, K, L, M, N, O, P, Q, R, S, T, U, V, W, X, Y, Z, F1, F2, F3, F4, F5, F6, F7, F8, Unknown);
	};

	/// \~English @brief Message level
	/// \~Chinese @brief 消息级别
	class SPADAS_API MessageLevel
	{
	public:
		enum class Value
		{
			Default = 1,
			Debug = 1,
			Info = 2,
			Warning = 3,
			Error = 4,
		};
		SPADAS_ENUM_VALUES(Debug, Info, Warning, Error);
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

		/// \~English @brief Whether there is a key input, if there is, return the key, if not, return Key::Value::None
		/// \~Chinese @brief 是否有按键输入，若有则返回该按键，无则返回 Key::Value::None
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
		SPADAS_TYPE("spadas.Path")

		/// \~English @brief Invalid object
		/// \~Chinese @brief 无效对象
		Path();

		/// \~English @brief Initialize based on an absolute or relative path string
		/// \~Chinese @brief 基于一个绝对路径或相对路径的字符串初始化
		/// \~English @param pathString Absolute path or relative path string, the relative path will be based on entryFolderPath() \n
		/// - If it is a folder, it needs to be explicitly terminated with the path separator "/" or "\" \n
		/// - You can add "../" or "..\" to indicate the parent folder
		/// \~Chinese @param pathString 绝对路径或相对路径字符串，相对路径将基于entryFolderPath() \n
		/// - 若为文件夹，需显式的以路径分隔符"/"或"\"结尾 \n
		/// - 可加入"../"或"..\"表示上层文件夹
		Path(const Char pathString[]);

		/// \~English @brief Initialize based on an absolute or relative path string
		/// \~Chinese @brief 基于一个绝对路径或相对路径的字符串初始化
		/// \~English @param pathString Absolute path or relative path string, the relative path will be based on entryFolderPath() \n
		/// - If it is a folder, it needs to be explicitly terminated with the path separator "/" or "\" \n
		/// - You can add "../" or "..\" to indicate the parent folder
		/// \~Chinese @param pathString 绝对路径或相对路径字符串，相对路径将基于entryFolderPath() \n
		/// - 若为文件夹，需显式的以路径分隔符"/"或"\"结尾 \n
		/// - 可加入"../"或"..\"表示上层文件夹
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
		/// \~Chinese @brief 取得文件或文件夹的绝对路径字符串 (文件夹以路径分隔符"/"或"\"结尾)
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

		/// \~English @brief (File-only) Move the file to the target location
		/// \~Chinese @brief (仅限文件) 移动文件至目标位置
		/// \~English @param dstPath Target file path
		/// \~Chinese @param dstPath 目标文件路径
		void moveTo(Path dstPath);

		/// \~English @brief (Folder-only) Move the folder to the target location
		/// \~Chinese @brief (仅限文件夹) 移动文件夹至目标位置
		/// \~English @param dstPath Target folder path
		/// \~Chinese @param dstPath 目标文件夹路径
		/// \~English @param mergeDst Whether to merge with the target folder
		/// \~Chinese @param mergeDst 是否与目标文件夹合并
		void moveTo(Path dstPath, Bool mergeDst);

		/// \~English @brief (File-only) copy the file to the target location
		/// \~Chinese @brief (仅限文件) 拷贝文件至目标位置
		/// \~English @param dstPath Target file path
		/// \~Chinese @param dstPath 目标文件路径
		void copyTo(Path dstPath);

		/// \~English @brief (Folder-only) copy the folder to the target location
		/// \~Chinese @brief (仅限文件夹) 拷贝文件夹至目标位置
		/// \~English @param dstPath Target folder path
		/// \~Chinese @param dstPath 目标文件夹路径
		/// \~English @param mergeDst Whether to merge with the target folder
		/// \~Chinese @param mergeDst 是否与目标文件夹合并
		void copyTo(Path dstPath, Bool mergeDst);

		/// \~English @brief (File-only) Get the size of the file (return 0 when the file does not exist)
		/// \~Chinese @brief (仅限文件) 取得文件大小 (当文件不存在返回0)
		ULong fileSize();

		/// \~English @brief (File-only) Create or recreate an empty file with the specified size, and the data is all 0
		/// \~Chinese @brief (仅限文件) 创建或重新创建一个指定大小的空文件，数据都为0
		void fileCreate(UInt size);

		/// \~English @brief (Folder-only) create the folder if it does not exist
		/// \~Chinese @brief (仅限文件夹) 若不存在则创建文件夹
		void folderMake();

		/// \~English @brief (Folder-only) Obtain all files and folder paths in the folder (if includeAllLevels is TRUE, return all levels of sub-files and sub-folders in the folder. If there are lots of contents, the speed may be very slow)
		/// \~Chinese @brief (仅限文件夹) 取得文件夹内所有文件和文件夹路径（若includeAllLevels为TRUE，则返回文件夹内所有层级的子文件和子文件夹。若内容较多则速度可能很慢）
		Array<Path> folderContents(Bool includeAllLevels = FALSE);

		/// \~English @brief (Folder-only) Generate child file path
		/// \~Chinese @brief (仅限文件夹) 生成子文件路径
		Path childFile(String childFullName);

		/// \~English @brief (Folder-only) Generate subfolder path
		/// \~Chinese @brief (仅限文件夹) 生成子文件夹路径（无需在末尾加路径分隔符）
		Path childFolder(String childFullName);

		/// \~English @brief (Folder-only) Generate a child file path or subfolder path based on the input relative path (folders end with path separator "/" or "\")
		/// \~Chinese @brief (仅限文件夹) 输入本文件夹内的相对路径，生成文件或文件夹路径 (文件夹以路径分隔符/或\结尾)
		Path childPath(String pathString);

		/// \~English @brief (Folder-only) Check if the target path is contained within this folder
		/// \~Chinese @brief (仅限文件夹) 检查目标路径是否包含在本文件夹内
		Bool contain(Path path);

		/// \~English @brief (Folder-only) Check if the target path is contained within this folder. If included, output the relative path based on this folder to pathString
		/// \~Chinese @brief (仅限文件夹) 检查目标路径是否包含在本文件夹内。若包含，则输出基于本文件夹的相对路径至pathString
		Bool contain(Path path, String& pathString);

		/// \~English @brief Obtain the parent folder path of the file or folder (if this path is the root directory of the disk, such as c:\, etc., an invalid path will be returned)
		/// \~Chinese @brief 获得文件或文件夹的上层文件夹 (如果本路径为磁盘根目录，如c:\等，则返回无效路径)
		Path parentFolder();

		/// \~English @brief Get the path separator under the current operating system
		/// \~Chinese @brief 获得当前操作系统下的路径分隔符
		static String separator();

		/// \~English @brief Get the directory where the entry module resides (On MacOS, forApp can be set to TRUE to obtain the directory where the .app bundle resides)
		/// \~Chinese @brief 获得入口模块所在目录（MacOS下可设置forApp为TRUE以取得.app文件夹所在目录）
		static Path entryFolderPath(Bool forApp = FALSE);

		/// \~English @brief Get system's current directory
		/// \~Chinese @brief 获得系统当前目录
		static Path currentPath();

		/// \~English @brief Get user root directory
		/// \~Chinese @brief 获得用户根目录
		static Path homePath();

		/// \~English @brief Get the SpadasFiles directory
		/// \~Chinese @brief 获得SpadasFiles目录
		static Path spadasFilesPath();

		/// \~English @brief Change system's current directory
		/// \~Chinese @brief 改变系统当前目录
		static void setCurrentPath(Path path);

		/// \~English @brief Manually set the directory where the entry module resides
		/// \~Chinese @brief 手动设置入口模块所在目录
		static void setEntryFolderPath(Path path);

	private:
		static void addFolderContents(ArrayX<Path>& contents, String folderPathString, Array<StringSpan>& folderComponents);
	};

	/// \~English @brief File data I/O
	/// \~Chinese @brief 文件数据I/O
	class SPADAS_API File : public Object<class FileVars>
	{
	public:
		SPADAS_TYPE("spadas.File")

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

		/// \~English @brief Open the file in binary output mode (create it if it does not exist) and append the content at the end
		/// \~Chinese @brief 以二进制输出模式打开文件（若不存在则创建）并在尾部追加内容
		static File appendBinary(Path filePath);

		/// \~English @brief Open the file in text output mode (create it if it does not exist) and append content at the end (UTF-8 BOM header will be detected)
		/// \~Chinese @brief 以文本输出模式打开文件（若不存在则创建）并在尾部追加内容（将检测UTF-8 BOM头）
		static File appendText(Path filePath);

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

		/// \~English @brief (Text output mode) print a line of text to the buffer
		/// \~Chinese @brief (文本输出模式) 打印一行文本至缓冲区
		void print(String text);

		/// \~English @brief (Output Mode) Output a block of binary data to the buffer
		/// \~Chinese @brief (输出模式) 输出一块二进制数据至缓冲区
		void output(Binary data);

		/// \~English @brief (Output Mode) Write buffer data to disk
		/// \~Chinese @brief (输出模式) 将缓冲区数据写入磁盘
		void flush();

		/// \~English @brief (Text input mode) Scan a line of text from a file
		/// \~Chinese @brief (文本输入模式) 从文件扫描一行文本
		String scan();

		/// \~English @brief (Input Mode) Input the binary data of the specified size from the file, the actual size is subject to the output
		/// \~Chinese @brief (输入模式) 从文件输入指定大小的二进制数据，实际大小以输出为准
		Binary input(UInt size = UINF);

		/// \~English @brief (Input Mode) Move to the specified I/O position, byte unit, and return which position it actually moves to
		/// \~Chinese @brief (输入模式) 移动至指定的I/O位置，字节单位，并返回实际移动到哪个位置
		ULong seek(ULong pos);

		/// \~English @brief (Input mode) Weather it has reached the end of the file
		/// \~Chinese @brief (输入模式) 是否已到文件结尾
		Bool endOfFile();
	};

	/// \~English @brief Print to file
	/// \~Chinese @brief 打印至文件
	class SPADAS_API FileConsole : public Object<class FileConsoleVars>, public IConsole
	{
	public:
		SPADAS_TYPE("spadas.FileConsole")

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

		XMLAttribute() {}
		XMLAttribute(String name, String value) : name(name), value(value) {}
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

	/// \~English @brief XML document object
	/// \~Chinese @brief XML文档对象
	class SPADAS_API XML : public Object<class XMLVars>
	{
	public:
		SPADAS_TYPE("spadas.XML")

		/// \~English @brief Create an empty document (root node's tag is "root")
		/// \~Chinese @brief 创建一个空文档（根标签为root）
		XML();

		/// \~English @brief Create a document based on XML element node (cloning the entire tree structure with the input node as the root node)
		/// \~Chinese @brief 通过一个XML元素节点创建文档（以输入节点为根节点克隆整个树结构）
		XML(XMLNode xmlRoot);

		/// \~English @brief Clone the XML tree
		/// \~Chinese @brief 复制对象
		XML clone();

		/// \~English @brief Get the root node of the current document, to modify the XML data
		/// \~Chinese @brief 获得当前文档的根节点，从而可修改XML数据
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

	private:
		Bool isNull() { return FALSE; }
		Bool isValid() { return FALSE; }
	};

	/// \~English @brief XML element node
	/// \~Chinese @brief XML元素节点
	class SPADAS_API XMLNode
	{
	public:
		/// \~English @brief Create invalid node
		/// \~Chinese @brief 创建无效节点
		XMLNode();

		/// \~English @brief Constructor, if the input cell is empty, it is also an invalid node
		/// \~Chinese @brief 构造函数，输入cell为空也为无效节点
		XMLNode(XML xml, Pointer cell);

		/// \~English @brief Whether is is a valid node
		/// \~Chinese @brief 是否为有效节点
		Bool valid();

		/// \~English @brief Get the reference of XML element data
		/// \~Chinese @brief 取得XML元素数据的引用
		XMLElement& value();

		/// \~English @brief Use field or method of the XML element
		/// \~Chinese @brief 使用XML元素数据的字段或方法
		XMLElement* operator ->();

		/// \~English @brief Get the node in root direction (if it does not exist, return an invalid node)
		/// \~Chinese @brief 取得根方向节点 (若不存在则返回无效节点)
		XMLNode root();

		/// \~English @brief Get the number of child nodes
		/// \~Chinese @brief 取得子节点个数
		UInt nLeaves();

		/// \~English @brief Get all child nodes
		/// \~Chinese @brief 取得所有子节点
		Array<XMLNode> leaves();

		/// \~English @brief Get all child nodes with the specified tag name
		/// \~Chinese @brief 获得所有具有指定标签名字的子节点
		Array<XMLNode> leavesWithTagName(String tagName);

		/// \~English @brief Get the first child node with the specified tag name, if it does not exist, return FALSE
		/// \~Chinese @brief 获得首个具有指定标签名字的子节点，若不存在返回FALSE
		Bool firstLeafWithTagName(String tagName, XMLNode& output);

		/// \~English @brief Convert the XML attribute array to a spadas::String dictionary
		/// \~Chinese @brief XML属性数组转换为字符串型字典
		Dictionary<String> attributesToDictionary();

		/// \~English @brief Set the XML attribute array by spadas::String type dictionary (This will replace the original attribute array)
		/// \~Chinese @brief 按字符串型字典设置XML属性数组 (将替换原有的属性数组)
		void dictionaryToAttributes(Dictionary<String> dict);

		/// \~English @brief Add child node with the specified XML element data, and return the node
		/// \~Chinese @brief 按指定XML元素数据添加子节点，并返回该节点
		XMLNode addLeaf(XMLElement val);

	private:
		XML xml;
		Pointer cell;
	};

	// Image / 图像 //////////////////////////////////////////////////////////////

	/// \~English @brief Image data pointer
	/// \~Chinese @brief 图像数据指针
	class SPADAS_API ImagePointer
	{
	public:
		/// \~English @brief Create invalid object
		/// \~Chinese @brief 创建无效对象
		ImagePointer();

		/// \~English @brief Initialize based on the image size and whether it is a color image. If setPixels is TRUE, assign zero to all pixel values
		/// \~Chinese @brief 基于图像大小和是否为彩色图像进行初始化，若setPixels为TRUE则所有像素赋值为0
		ImagePointer(Size2D size, Bool isColor, Bool setPixels = FALSE);

		/// \~English @brief Initialize based on external data
		/// \~Chinese @brief 基于外部数据初始化
		ImagePointer(Size2D size, Bool isColor, UInt rowBytes, BinarySpan data);

		/// \~English @brief Whether the data is valid
		/// \~Chinese @brief 数据是否有效
		Bool isValid();

		/// \~English @brief Whether the source of data is a reference counting object
		/// \~Chinese @brief 数据源是否为一个引用计数对象
		Bool isRefCounting();

		/// \~English @brief Image width
		/// \~Chinese @brief 图像宽度
		UInt getWidth();

		/// \~English @brief Image height
		/// \~Chinese @brief 图像高度
		UInt getHeight();

		/// \~English @brief Whether it is a color image, the color image stores data in BGR order
		/// \~Chinese @brief 是否为彩色图像，彩色图像按BGR顺序存储数据
		Bool isColor();

		/// \~English @brief The number of bytes of each line of image, which is a multiple of 8
		/// \~Chinese @brief 每行图像的字节数，为8的倍数
		UInt getRowBytes();

		/// \~English @brief Image data
		/// \~Chinese @brief 图像数据
		BinarySpan getData();

	private:
		UInt width, height, step;
		Bool color;
		BinarySpan span;
	};

	/// \~English @brief The color represented by RGB
	/// \~Chinese @brief 由RGB表述的颜色
	struct ColorRGB
	{
		Byte r;
		Byte g;
		Byte b;

		ColorRGB() : r(0), g(0), b(0) {}
		ColorRGB(Byte r, Byte g, Byte b) : r(r), g(g), b(b) {}
		Bool operator ==(ColorRGB color) { return r == color.r && g == color.g && b == color.b; }
		Bool operator !=(ColorRGB color) { return !(r == color.r && g == color.g && b == color.b); }
		String toString() { return (String)r + "," + g + "," + b; }
	};

	/// \~English @brief Image Standard Resolution
	/// \~Chinese @brief 图像标准分辨率
	class SPADAS_API ImageResolution
	{
	public:
		enum class Value
		{
			Default = 2,

			/// \~ @brief 320 x 240 (4:3)
			QVGA = 1,

			/// \~ @brief 640 x 480 (4:3)
			VGA = 2,

			/// \~ @brief 720 x 576 (5:4)
			PAL720 = 3,

			/// \~ @brief 768 x 576 (4:3)
			PAL768 = 4,

			/// \~ @brief 800 x 600 (4:3)
			SVGA = 5,

			/// \~ @brief 1024 x 768 (4:3)
			XGA = 6,

			/// \~ @brief 1280 x 800 (8:5)
			WXGA = 7,

			/// \~ @brief 1280 x 1024 (5:4)
			SXGA = 8,

			/// \~ @brief 1400 x 1050 (4:3)
			SXGAPlus = 9,

			/// \~ @brief 1600 x 1200 (4:3)
			UXGA = 10,

			/// \~ @brief 1680 x 1050 (8:5)
			WSXGAPlus = 11,

			/// \~ @brief 1280 x 720 (16:9)
			HD720 = 12,

			/// \~ @brief 1920 x 1080 (16:9)
			HD1080 = 13,

			/// \~ @brief 2560 x 1440 (16:9)
			HD2K = 14,

			/// \~ @brief 3840 x 2160 (16:9)
			HD4K = 15,
		};
		SPADAS_ENUM_VALUES(QVGA, VGA, PAL720, PAL768, SVGA, XGA, WXGA, SXGA, SXGAPlus, UXGA, WSXGAPlus, HD720, HD1080, HD2K, HD4K);

		static Size2D size(Enum<ImageResolution> resolution);
	};

	/// \~English @brief Pixel format
	/// \~Chinese @brief 像素格式
	class SPADAS_API PixelFormat
	{
	public:
		enum class Value
		{
			Default = 1,
			ByteBGR = 1,
			ByteRGB = 2,
			ByteGray = 3,
			ByteUYVY = 4, // YUV(YCbCr)4:2:2
			ByteYUV = 5, // YUV(YCbCr)4:4:4
			ByteBool = 6, // TRUE: 255, FALSE: 0
			ByteBGRA = 7,
			ByteRGBA = 8,
			WordBGR = 21, // 0~65535
			WordGray = 22, // 0~65535
			FloatBGR = 31, // 0.0~1.0
			FloatGray = 32, // 0.0~1.0
			FloatHSV = 33, // H(0.0~360.0) S(0.0~1.0) V(0.0~1.0)
		};
		SPADAS_ENUM_VALUES(ByteBGR, ByteRGB, ByteGray, ByteUYVY, ByteYUV, ByteBool, ByteBGRA, ByteRGBA, WordBGR, WordGray, FloatBGR, FloatGray, FloatHSV);

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
		enum class Value
		{
			Default = 1,
			Nearest = 1,
			Bilinear = 2,
			Area = 3,
		};
		SPADAS_ENUM_VALUES(Nearest, Bilinear, Area);
	};

	/// \~English @brief Image resizing multiplier
	/// \~Chinese @brief 图像大小的调整倍数
	class SPADAS_API ResizeScale
	{
	public:
		enum class Value
		{
			Default = 0,
			None = 0,
			Quarter = 1,
			Half = 2,
			Double = 3,
		};
		SPADAS_ENUM_VALUES(None, Quarter, Half, Double);
	};

	/// \~English @brief The type of image rotation
	/// \~Chinese @brief 图像旋转的类型
	class SPADAS_API ImageRotation
	{
	public:
		enum class Value
		{
			Default = 0,
			None = 0,
			CW90 = 1,
			CW180 = 2,
			CCW90 = 3,
		};
		SPADAS_ENUM_VALUES(None, CW90, CW180, CCW90);
	};

	/// \~English @brief The format of image file
	/// \~Chinese @brief 图像文件格式
	class SPADAS_API ImageFileFormat
	{
	public:
		enum class Value
		{
			Default = 0,
			BMP = 0,
			JPG = 1,
			PNG = 2,
		};
		SPADAS_ENUM_VALUES(BMP, JPG, PNG);
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
		/// \~English @brief [Unsafe] Pointer union, can be used according to the type
		/// \~Chinese @brief [非安全] 指针联合，可根据类型使用
		union { Pointer ptr; Byte *b; Word *w; Float *f; };

		/// \~English @brief Bytes per pixel
		/// \~Chinese @brief 每像素字节数
		UInt bytesPerPixel;

		/// \~English @brief Invalid pointer
		/// \~Chinese @brief 无效指针
		PixelData() : ptr(NULL), bytesPerPixel(0) {}

		/// \~English @brief Normal creation
		/// \~Chinese @brief 正常创建
		PixelData(Pointer ptr, UInt bytesPerPixel) : ptr(ptr), bytesPerPixel(bytesPerPixel) {}

		/// \~English @brief Is it valid
		/// \~Chinese @brief 是否有效
		Bool isValid() { return ptr != NULL; }

		/// \~English @brief Get the pixel data on the specified index
		/// \~Chinese @brief 获取指定序号上的像素数据
		PixelData operator [](UInt index) { return ptr ? PixelData((Pointer)((PointerInt)ptr + index * bytesPerPixel), bytesPerPixel) : PixelData(); }

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

	/// \~English @brief Image class
	/// \~Chinese @brief 图像类
	class SPADAS_API Image : public Object<class ImageVars>
	{
	public:
		SPADAS_TYPE("spadas.Image")

		/// \~English @brief Invalid object
		/// \~Chinese @brief 无效对象
		Image();

		/// \~English @brief Specify the size and format to create an image. If setPixels is TRUE, assign zero to all pixel values
		/// \~Chinese @brief 指定大小和格式创建图像，若setPixels为TRUE则所有像素赋值为0
		Image(Size2D size, Enum<PixelFormat> format, Bool setPixels = TRUE);

		/// \~English @brief Specify the size and format to create an image. If setPixels is TRUE, assign zero to all pixel values
		/// \~Chinese @brief 指定大小和格式创建图像，若setPixels为TRUE则所有像素赋值为0
		Image(Enum<ImageResolution> resolution, Enum<PixelFormat> format, Bool setPixels = TRUE);

		/// \~English @brief Create image from image pointer (without copying data)
		/// \~Chinese @brief 从图像指针创建图像（不拷贝数据）
		Image(ImagePointer pointer);

		/// \~English @brief Read the image from the .bmp, .jpg or .png file data, the format may be ByteBGR, ByteBGRA or ByteGray depending on the data
		/// \~Chinese @brief 从.bmp, .jpg或.png文件数据读取图像，根据数据不同其格式可能为ByteBGR，ByteBGRA或ByteGray
		Image(Enum<ImageFileFormat> format, Binary fileData);

		/// \~English @brief Read the image from the .bmp, .jpg or .png file, the format may be ByteBGR, ByteBGRA or ByteGray depending on the content of the file
		/// \~Chinese @brief 从.bmp, .jpg或.png文件读取图像，根据文件内容不同其格式可能为ByteBGR，ByteBGRA或ByteGray
		Image(Path filePath);

		/// \~English @brief Whether the source of data is a reference counting object
		/// \~Chinese @brief 数据源是否为一个引用计数对象
		Bool isRefCounting();

		/// \~English @brief Clone a new object
		/// \~Chinese @brief 克隆出一个新对象
		Image clone();

		/// \~English @brief Copy data from a sub-area of another image with the same pixel format to a certain position of this image (specified by thisOffset)
		/// \~Chinese @brief 从另一幅具有相同像素格式的图像的某个子区域拷贝数据到本图像的某个位置（通过thisOffset指定）
		void copy(Image src, Region2D srcRegion, CoordInt2D thisOffset);

		/// \~English @brief Draw the layer to a certain position of this image (the layer image must have an Alpha channel)
		/// \~Chinese @brief 绘制图层至本图像的某个位置 (layer图像必须具有Alpha通道)
		void drawLayer(Image layer, CoordInt2D dstOffset);

		/// \~English @brief Get the sub-image, whose data will be bound to the data of this image
		/// \~Chinese @brief 取得子图像，其数据绑定至本图像的数据
		Image subImage(Region2D region);

		/// \~English @brief Get the image pointer (without copying data), only supported by ByteGray and ByteBGR
		/// \~Chinese @brief 取得图像指针（不拷贝数据），仅ByteGray和ByteBGR支持
		ImagePointer imagePointer();

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

		/// \~English @brief Get the first pixel data of the image
		/// \~Chinese @brief 取得图像第一个像素数据
		PixelData data();

		/// \~English @brief Get the first pixel data of a row, v=0 is the first row
		/// \~Chinese @brief 取得某一行第一个像素数据，v=0为第一行
		PixelData operator [](UInt v);

		/// \~English @brief Call the specified function for each pixel (The function parameters are the row index starting from 0, column index starting from 0, and pixel data)
		/// \~Chinese @brief 对每个像素调用指定函数（该函数参数分别为从0起算的行序号、列序号、像素数据）
		void forPixels(Func<void(UInt, UInt, PixelData)> func);

		/// \~English @brief Resize the image by the specified factor (faster)
		/// \~Chinese @brief 按照指定倍数调整图像大小 (较快)
		Image resize(Enum<ResizeScale> scale);

		/// \~English @brief Resize the image to any size (slower)
		/// \~Chinese @brief 调整图像大小至任意尺寸 (较慢)
		Image resize(Size2D outputSize, Bool undistort, Enum<ResizeMode> mode = ResizeMode::Value::Bilinear);

		/// \~English @brief Resize the image to any size (slower)
		/// \~Chinese @brief 调整图像大小至任意尺寸 (较慢)
		Image resize(Enum<ImageResolution> outputResolution, Bool undistort, Enum<ResizeMode> mode = ResizeMode::Value::Bilinear);

		/// \~English @brief Convert pixel format (when converting to PixelFormat::Value::ByteBool, 128~255 -> 255, 0~127 -> 0
		/// \~Chinese @brief 转换像素格式 (转换为 PixelFormat::Value::ByteBool 时, 128~255 -> 255, 0~127 -> 0
		Image convert(Enum<PixelFormat> outputFormat);

		/// \~English @brief Flip the image
		/// \~Chinese @brief 反转图像
		Image flip(Bool upDownFlip, Bool leftRightFlip);

		/// \~English @brief Rotate image (supports all pixel formats except ByteUYVY)
		/// \~Chinese @brief 旋转图像 (支持除ByteUYVY以外的所有像素格式)
		Image rotate(Enum<ImageRotation> rotation);

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

		/// \~English @brief Encode to .bmp, .jpg or .png file data. You can set jpgQuality while encoding to .jpg (ranges 0~100, 0 for smallest file, 100 for highest quality)
		/// \~Chinese @brief 编码至.bmp, .jpg或.png文件数据，编码为.jpg时可通过jpgQuality指定编码质量（0~100，0则文件最小，100则图像质量最高）
		Binary toBinary(Enum<ImageFileFormat> format, UInt jpgQuality = UINF);

		/// \~English @brief Save to .bmp, .jpg or .png file. You can set jpgQuality while saving to .jpg file (ranges 0~100, 0 for smallest file, 100 for highest quality)
		/// \~Chinese @brief 保存至.bmp, .jpg或.png文件，存储为.jpg时可通过jpgQuality指定编码质量（0~100，0则文件最小，100则图像质量最高）
		void save(Path filePath, UInt jpgQuality = UINF);

		/// \~English @brief Use text to describe information such as image size, pixel format, etc.
		/// \~Chinese @brief 用文本表述图像大小、像素格式等信息
		String getInfoText();
	};

	// Math / 数学 //////////////////////////////////////////////////////////////

	/// \~English @brief Large integer number
	/// \~Chinese @brief 大数
	class SPADAS_API BigInteger : public Object<class BigIntegerVars>
	{
	public:
		SPADAS_TYPE("spadas.BigInteger")

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

	/// \~English @brief Variables of spadas::Matrix template class
	/// \~Chinese @brief spadas::Matrix 模板类的变量数据
	template <typename Type> class MatrixVars;

	/// \~English @brief Matrix
	/// \~Chinese @brief 矩阵
	template <typename Type> class Matrix : public Object<class MatrixVars<Type> >
	{
	public:
		SPADAS_TYPE("spadas.Matrix<" cat typeid(Type).name() cat ">")

		/// \~English @brief Invalid object
		/// \~Chinese @brief 无效对象
		Matrix();

		/// \~English @brief Create a 2D matrix
		/// \~Chinese @brief 创建一个2维矩阵
		Matrix(UInt dim0, UInt dim1);

		/// \~English @brief Create a 3D matrix
		/// \~Chinese @brief 创建一个3维矩阵
		Matrix(UInt dim0, UInt dim1, UInt dim2);

		/// \~English @brief Create an N-dimensional matrix (N must be at least 1)
		/// \~Chinese @brief 创建一个N维矩阵 (N至少为1)
		Matrix(Array<UInt> dims);

		/// \~English @brief Create an N-dimensional matrix and copy it from the specified data source
		/// \~Chinese @brief 创建一个N维矩阵并从指定数据源拷贝
		Matrix(Array<UInt> dims, Pointer raw);

		/// \~English @brief Create a matrix from an array (the size of the matrix is N x 1 x 1..., the dimension is specified by the parameter)
		/// \~Chinese @brief 由数组创建矩阵 (矩阵大小为N x 1 x 1...，维数由参数指定)
		Matrix(Array<Type> arr, UInt nDims = 2);

		/// \~English @brief Read matrix data from file (created by Matrix::save method)
		/// \~Chinese @brief 从文件读取矩阵数据并创建（由Matrix::save方法创建的文件）
		Matrix(Path filePath);

		/// \~English @brief Clone a new object
		/// \~Chinese @brief 克隆出一个新对象
		Matrix<Type> clone();

		/// \~English @brief Matrix data type conversion
		/// \~Chinese @brief 矩阵数据类型转换
		template <typename TargetType>
		void convertTo(Matrix<TargetType>& target);

		/// \~English @brief Copy data from a sub-area of another matrix to a certain position of this matrix (specified by thisOffset), both the source matrix and this matrix should be 2D matrices
		/// \~Chinese @brief 从另一个矩阵的某个子区域拷贝数据到本矩阵的某个位置（通过thisOffset指定），源矩阵和本矩阵都应为2维矩阵
		void copy(Matrix<Type> src, Region2D srcRegion, CoordInt2D thisOffset);

		/// \~English @brief Copy data from a sub-area of another matrix to a certain position of this matrix (specified by thisOffset), both the source matrix and this matrix should be 3D matrices
		/// \~Chinese @brief 从另一个矩阵的某个子区域拷贝数据到本矩阵的某个位置（通过thisOffset指定），源矩阵和本矩阵都应为3维矩阵
		void copy(Matrix<Type> src, Region3D srcRegion, CoordInt3D thisOffset);

		/// \~English @brief Assign the same value to all elements
		/// \~Chinese @brief 对所有元素赋同一个值
		void set(Type value);

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

		/// \~English @brief [Unsafe] Get the data pointer
		/// \~Chinese @brief [非安全] 取得数据指针
		Type *data();

		/// \~English @brief Obtain the i-th sub-matrix, the dimension of the sub-matrix is reduced by 1, the data is bound to this matrix (index starts from 0)
		/// \~Chinese @brief 取得第i个子矩阵，该子矩阵维数减1，数据绑定至本矩阵，序号从0开始
		Matrix<Type> operator [](UInt i);

		/// \~English @brief Obtain a reference to an element of a 1D matrix (scalar) through parentheses
		/// \~Chinese @brief 通过圆括号获得某个元素的引用，面向1维矩阵 (标量)
		Type& operator ()(UInt i);

		/// \~English @brief Obtain a reference to an element of a 2D matrix (scalar) through parentheses
		/// \~Chinese @brief 通过圆括号获得某个元素的引用，面向2维矩阵 (标量)
		Type& operator ()(UInt i, UInt j);

		/// \~English @brief Obtain a reference to an element of a 3D matrix (scalar) through parentheses
		/// \~Chinese @brief 通过圆括号获得某个元素的引用，面向3维矩阵 (标量)
		Type& operator ()(UInt i, UInt j, UInt k);

		/// \~English @brief Matrix addition: output = this + matrix
		/// \~Chinese @brief 矩阵加法: output = this + matrix
		Matrix<Type> operator +(Matrix<Type> matrix);

		/// \~English @brief Matrix subtraction: output = this - matrix
		/// \~Chinese @brief 矩阵减法: output = this - matrix
		Matrix<Type> operator -(Matrix<Type> matrix);

		/// \~English @brief Matrix multiplied by scalar: output = this * scale
		/// \~Chinese @brief 矩阵乘以标量: output = this * scale
		Matrix<Type> operator *(Type scale);

		/// \~English @brief Matrix multiplication: output = this * matrix (only supports 2D matrices)
		/// \~Chinese @brief 矩阵相乘: output = this * matrix (仅支持2维矩阵)
		Matrix<Type> operator *(Matrix<Type> matrix);

		/// \~English @brief Matrix transpose (only supports 2D matrix)
		/// \~Chinese @brief 矩阵转置 (仅支持2维矩阵)
		Matrix<Type> transpose();

		/// \~English @brief Convert matrix data to text string (only supports 2D matrix)
		/// \~Chinese @brief 用字符串表示矩阵数据 (仅支持2维矩阵)
		String toString();

		/// \~English @brief Save matrix data to a text file (only supports 2D matrix)
		/// \~Chinese @brief 保存矩阵数据至文本文件 (仅支持2维矩阵)
		void save(Path filePath);
	};

	/// \~English @brief Supported matrix types: Int matrix
	/// \~Chinese @brief 支持的矩阵类型: Int型矩阵
	typedef Matrix<Int> IntMat;

	/// \~English @brief Supported matrix types: Float matrix
	/// \~Chinese @brief 支持的矩阵类型: Float型矩阵
	typedef Matrix<Float> FloatMat;

	/// \~English @brief Supported matrix types: Double matrix
	/// \~Chinese @brief 支持的矩阵类型: Double型矩阵
	typedef Matrix<Double> DoubleMat;

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
		inline Long round(Float number) { return (Long)(number + (number >= 0 ? 0.5f : -0.5f)); }

		/// \~English @brief Round up
		/// \~Chinese @brief 四舍五入
		inline Long round(Double number) { return (Long)(number + (number >= 0 ? 0.5 : -0.5)); }

		/// \~English @brief Get the largest integer not greater than the input value
		/// \~Chinese @brief 取得不大于输入值的最大整数
		inline Long floor(Float number) { Long i = (Long)number; return i - (i > number); }

		/// \~English @brief Get the largest integer not greater than the input value
		/// \~Chinese @brief 取得不大于输入值的最大整数
		inline Long floor(Double number) { Long i = (Long)number; return i - (i > number); }

		/// \~English @brief Get the smallest integer not less than the input value
		/// \~Chinese @brief 取得不小于输入值的最小整数
		inline Long ceil(Float number) { Long i = (Long)number; return i + (i < number); }

		/// \~English @brief Get the smallest integer not less than the input value
		/// \~Chinese @brief 取得不小于输入值的最小整数
		inline Long ceil(Double number) { Long i = (Long)number; return i + (i < number); }

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

	/// \~English @brief [Multithread safe] Thread lock
	/// \~Chinese @brief [多线程安全] 线程锁
	class SPADAS_API Lock : public Object<class LockVars>
	{
	public:
		SPADAS_TYPE("spadas.Lock")

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
		LockProxy(Lock& lock);

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

	/// \~English @brief [Multithread safe] Flag
	/// \~Chinese @brief [多线程安全] 标志位
	class SPADAS_API Flag : public Object<class FlagVars>
	{
	public:
		SPADAS_TYPE("spadas.Flag")

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

	/// \~English @brief [Multithread safe] Thread synchronizer
	/// \~Chinese @brief [多线程安全] 线程同步器
	class SPADAS_API Barrier : public Object<class BarrierVars>
	{
	public:
		SPADAS_TYPE("spadas.Barrier")

		/// \~English @brief Create a synchronizer object (default strength is 0)
		/// \~Chinese @brief 创建一个同步器对象 (默认强度为0)
		Barrier();

		/// \~English @brief Set strength
		/// \~Chinese @brief 设置强度
		void setStrength(UInt strength);

		/// \~English @brief Enter the synchronization state, when the number of threads entering the synchronization state is equal to the strength of the synchronizer, it will return TRUE (return FALSE if it times out)
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
		/// \~Chinese @brief 用户设置的循环时间间隔 [ms] (当使用用户自定义的循环间隔时才有效)
		Optional<UInt> userTimeInterval;

		/// \~English @brief Thread loop average processing time [ms]
		/// \~Chinese @brief 线程循环平均处理时间 [ms]
		Optional<Float> averageProcessTime;

		/// \~English @brief The running time of the current loop [ms] (use this value to confirm whether the thread is freezed)
		/// \~Chinese @brief 当前循环已运行时间 [ms] (通过此数值确认线程是否已卡死)
		Float currentLoopTime;
		
		/// \~English @brief Default constructor
		/// \~Chinese @brief 默认构造函数
		ThreadStatus() : isActive(FALSE), currentLoopTime(0.0f) {}
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
		WorkflowStatus() : workflowID(0), isActive(FALSE) {}
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
		
		/// \~English @brief [Optional] The function called by the thread whose index is threadIndex in each loop. You'd better not use functions with waiting functions here
		/// \~Chinese @brief [可选] 给出序号为threadIndex的线程在每次循环时调用的代码，其中应尽量不使用具有等待功能的函数
		virtual void onThreadLoop(UInt threadIndex, Flag shouldEnd);
		
		/// \~English @brief [Optional] The function that the thread whose index is threadIndex runs when the thread ends
		/// \~Chinese @brief [可选] 给出序号为threadIndex的线程在线程结束时运行的代码
		virtual void onThreadEnd(UInt threadIndex);
	};

	/// \~English @brief [Multithread safe] Multi-threaded container
	/// \~Chinese @brief [多线程安全] 多线程容器
	class SPADAS_API Threads : public Object<class ThreadsVars>
	{
	public:
		SPADAS_TYPE("spadas.Threads")

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

	/// \~English @brief [Multithread safe] Task thread manager
	/// \~Chinese @brief [多线程安全] 任务线程管理器
	class SPADAS_API TaskManager : public Object<class TaskManagerVars>
	{
	public:
		SPADAS_TYPE("spadas.TaskManager")

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
		enum class Value
		{
			Default = 0,
			Unknown = 0,
			Linux = 3,
			MacOS = 4,
			Windows = 6,
		};
		SPADAS_ENUM_VALUES(Unknown, Linux, MacOS, Windows);
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
		Time() : year(1900), month(1), day(1), hour(0), minute(0), second(0) {}

		/// \~English @brief Initialize based on the specified year, month, day, hour, minute, and second
		/// \~Chinese @brief 基于指定年月日时分秒初始化
		Time(UInt year, UInt month, UInt day, UInt hour, UInt minute, UInt second) : year(year), month(month), day(day), hour(hour), minute(minute), second(second) {}

		/// \~English @brief Whether it is equal to
		/// \~Chinese @brief 是否等于
		Bool operator ==(Time time) { return year == time.year && month == time.month && day == time.day && hour == time.hour && minute == time.minute && second == time.second; }

		/// \~English @brief Whether it is not equal to
		/// \~Chinese @brief 是否不等于
		Bool operator !=(Time time) { return !(year == time.year && month == time.month && day == time.day && hour == time.hour && minute == time.minute && second == time.second); }

		/// \~English @brief Get the hash value
		/// \~Chinese @brief 获取哈希值
		Word getHash() { return (Word)((((((UInt)second * 3 + (UInt)minute) * 3 + (UInt)hour) * 3 + (UInt)day) * 3 + (UInt)month) * 3 + (UInt)year); }

		/// \~English @brief Create a date (year, month, day) string based on the specified separator
		/// \~Chinese @brief 基于指定分隔符创建日期(年月日)字符串
		String dateString(String separator = "/") { return String(this->year, 4) + separator + String(this->month, 2) + separator + String(this->day, 2); }

		/// \~English @brief Create a time (hour, minute, second) string based on the specified separator
		/// \~Chinese @brief 基于指定分隔符创建时间(时分秒)字符串
		String timeString(String separator = ":") { return String(this->hour, 2) + separator + String(this->minute, 2) + separator + String(this->second, 2); }

		/// \~English @brief Convert to string based on separators
		/// \~Chinese @brief 基于分隔符转为字符串
		String toString(String dateSeparator = "/", String timeSeparator = ":", String dateTimeSeparator = " ") { return dateString(dateSeparator) + dateTimeSeparator + timeString(timeSeparator); }

		/// \~English @brief Is later than
		/// \~Chinese @brief 是否大于
		SPADAS_API Bool operator >(Time time);

		/// \~English @brief Is earlier than
		/// \~Chinese @brief 是否小于
		SPADAS_API Bool operator <(Time time);
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
		TimeWithMS() : milliseconds(0) {}

		/// \~English @brief Initialization based on date and time with millisecond precision
		/// \~Chinese @brief 基于日期和毫秒的初始化
		TimeWithMS(Time dateTime, UInt milliseconds) : dateTime(dateTime), milliseconds(milliseconds) {}

		/// \~English @brief Initialization based on components
		/// \~Chinese @brief 基于指定年月日时分秒初始化
		TimeWithMS(UInt year, UInt month, UInt day, UInt hour, UInt minute, UInt second, UInt milliseconds) : dateTime(year, month, day, hour, minute, second), milliseconds(milliseconds) {}

		/// \~English @brief Whether it is equal to
		/// \~Chinese @brief 是否等于
		Bool operator ==(TimeWithMS time) { return dateTime == time.dateTime && milliseconds == time.milliseconds; }

		/// \~English @brief Whether it is not equal to
		/// \~Chinese @brief 是否不等于
		Bool operator !=(TimeWithMS time) { return dateTime != time.dateTime || milliseconds != time.milliseconds; }

		/// \~English @brief Convert to string based on separators
		/// \~Chinese @brief 基于分隔符转为字符串
		String toString(String dateSeparator = "/", String timeSeparator = ":", String dateTimeSeparator = " ", String msSeparator = ".") { return dateTime.toString(dateSeparator, timeSeparator, dateTimeSeparator) + msSeparator + String(milliseconds, 3); }

		/// \~English @brief Returns a time based on this time plus/minus the specified milliseconds
		/// \~Chinese @brief 返回在该时间基础上加/减指定毫秒的时间
		SPADAS_API TimeWithMS operator +(Long millisecond);

		/// \~English @brief Returns the time difference between two times, in milliseconds
		/// \~Chinese @brief 返回两个时间的时间差，单位毫秒
		SPADAS_API Long operator -(TimeWithMS time);

		/// \~English @brief Returns the time difference between two times, in the form of components
		/// \~Chinese @brief 返回两个时间的时间差，以分量的形式
		SPADAS_API void minus(TimeWithMS time, Int& week, Int& day, Int& hour, Int& minute, Int& second, Int& millisecond);

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
	};

	/// \~English @brief Timer
	/// \~Chinese @brief 计时器
	class SPADAS_API Timer : public Object<class TimerVars>
	{
	public:
		SPADAS_TYPE("spadas.Timer")

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
		SPADAS_TYPE("spadas.Tick")

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
		SPADAS_TYPE("spadas.MemoryMap")

		/// \~English @brief Invalid object
		/// \~Chinese @brief 无效对象
		MemoryMap();

		/// \~English @brief Create a memory mapping object, specify the mapping file path (must be created in advance and the file size is not less than offset+size), mapping start offset and mapping area size. Note that the mapping start offset value should be a multiple of the following N values: N = 4096 @ Linux/MacOS, N = 65536 @ Windows
		/// \~Chinese @brief 创建内存映射对象，指定映射文件路径(必须事先创建且文件大小不小于offset+size)，映射起点偏移以及映射区域大小。注意，映射起点偏移值应为以下N值的倍数：N = 4096 @ Linux/MacOS，N = 65536 @ Windows
		MemoryMap(Path file, PointerInt offset, PointerInt size);

		/// \~English @brief Unmap
		/// \~Chinese @brief 解除映射
		void unmap();

		/// \~English @brief [Unsafe] Get the virtual memory address of the starting point of the memory mapping
		/// \~Chinese @brief [非安全] 取得映射起点的虚拟内存地址
		Pointer getPointer();
	};

	/// \~English @brief The sending result to the memory-mapped data stream
	/// \~Chinese @brief 内存映射数据流的发送结果
	class SPADAS_API MemoryMapSendResult
	{
	public:
		enum class Value
		{
			Default = 0,

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
		SPADAS_ENUM_VALUES(OK, NotOpen, WrongMode, WrongSize, QueueFull);
	};


	/// \~English @brief [Multithread safe] Data stream based on memory mapping
	/// \~Chinese @brief [多线程安全] 基于内存映射的数据流
	class SPADAS_API MemoryMapStream : public Object<class MemoryMapStreamVars>
	{
	public:
		SPADAS_TYPE("spadas.MemoryMapStream")

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
		Enum<MemoryMapSendResult> send(Pointer dataPtr, UInt byteCount);

		/// \~English @brief Send data
		/// \~Chinese @brief 发送数据
		/// \~English @param data Binary data
		/// \~Chinese @param data 二进制数据
		/// \~English @returns The sending result
		/// \~Chinese @returns 返回发送结果
		Enum<MemoryMapSendResult> send(Binary data);

		/// \~English @brief Receive all new binary data
		/// \~Chinese @brief 接收所有收到的数据
		Array<Binary> receive();

	private:
		Bool isNull() { return FALSE; }
		Bool isValid() { return FALSE; }
	};

	/// \~English @brief Dynamic library loader
	/// \~Chinese @brief 动态库加载器
	class SPADAS_API LibraryLoader : public Object<class LibraryLoaderVars>
	{
	public:
		SPADAS_TYPE("spadas.LibraryLoader")

		/// \~English @brief Create a dynamic library loader
		/// \~Chinese @brief 创建动态库加载器
		LibraryLoader();

		/// \~English @brief Use the library name under the specified folder to open the dynamic library (no need to add a prefix or suffix such as "lib"), libVersion is used to specify the suffix version number of the dynamic library such as so (such as "1.0")
		/// \~Chinese @brief 使用指定文件夹下的库名称打开动态库（无需添加lib等前缀或后缀名），libVersion用于指定so等动态库的后缀版本号（如"1.0"）
		Bool openWithName(Path libDir, String libName, String libVersion = String());

		/// \~English @brief Use the library name under the specified folder to open the dynamic library (no need to add a prefix or suffix such as "lib"), libVersion is used to specify the suffix version number of the dynamic library such as so (such as "1.0"), and output an error message if it fails
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

		/// \~English @brief Get the current host time (local time zone)
		/// \~Chinese @brief 获得当前主机时间（本地时区）
		SPADAS_API Time getTime();

		/// \~English @brief Get the current host time (local time zone), with millisecond information
		/// \~Chinese @brief 获得当前主机时间（本地时区），带毫秒信息
		SPADAS_API TimeWithMS getTimeWithMS();

		/// \~English @brief Wait for the specified time, in milliseconds (the accuracy may drop to 5~10 milliseconds when the system load is high)
		/// \~Chinese @brief 等待指定时间，单位毫秒（系统负载高时精度可能会降至5~10毫秒）
		SPADAS_API void wait(UInt time);

		/// \~English @brief Spin to wait for the specified time, in microseconds
		/// \~Chinese @brief 以自旋方式等待指定时间，单位微秒
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
		/// \~English @brief [Unsafe] Set the value of a segment of memory in bytes
		/// \~Chinese @brief [非安全] 以字节为单位设置一段内存的值
		/// \~English @param value The byte value to set
		/// \~Chinese @param value 希望设置的字节数值
		/// \~English @param dst Memory start address
		/// \~Chinese @param dst 内存起始地址
		/// \~English @param setSize The number of bytes to set
		/// \~Chinese @param setSize 设置的字节数
		SPADAS_API void memorySet(Byte value, Pointer dst, UInt setSize);

		/// \~English @brief [Unsafe] memory copy (highest efficiency)
		/// \~Chinese @brief [非安全] 内存拷贝（效率最高）
		/// \~English @param src Start address of copy source memory
		/// \~Chinese @param src 拷贝源内存起始地址
		/// \~English @param dst Start address of copy target memory
		/// \~Chinese @param dst 拷贝目标内存起始地址
		/// \~English @param copySize Number of bytes to copy
		/// \~Chinese @param copySize 拷贝字节数
		SPADAS_API void memoryCopy(ConstPointer src, Pointer dst, UInt copySize);

		/// \~English @brief [Unsafe] Forced conversion of value types (must ensure that the "sizeof" of the two types is consistent, and the structure must ensure that the type and order are consistent)
		/// \~Chinese @brief [非安全] 值类型的强制转换（必须保证两个类型sizeof一致，结构体的话必须保证类型与顺序都一致）
		template <typename SrcType, typename DstType>
		DstType valueCast(SrcType val);

		/// \~English @brief [Unsafe] Forced conversion of value type arrays (must ensure that the "sizeof" of the two types is consistent, and the structure must ensure that the type and order are consistent)
		/// \~Chinese @brief [非安全] 值类型数组的强制转换（必须保证两个类型sizeof一致，结构体的话必须保证类型与顺序都一致）
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
		OptionalBool() : valid(FALSE), value(FALSE) {}

		/// \~English @brief Initialize based on input boolean value
		/// \~Chinese @brief 基于输入的布尔值初始化
		OptionalBool(Bool val) : valid(TRUE), value(val) {}

		/// \~English @brief Assignment by the input boolean value
		/// \~Chinese @brief 按输入的布尔值赋值
		OptionalBool& operator =(Bool val) { valid = TRUE; value = val; return *this; }

		/// \~English @brief Is valid and true
		/// \~Chinese @brief 是否有效且为真
		Bool isValidAndTrue() { return valid && value; }

		/// \~English @brief Is valid and false
		/// \~Chinese @brief 是否有效且为假
		Bool isValidAndFalse() { return valid && !value; }

		/// \~English @brief Whether it is equal to
		/// \~Chinese @brief 是否相等
		Bool operator ==(OptionalBool val) { if (!valid && !val.valid) return TRUE; if (!valid || !val.valid) return FALSE; return value == val.value; }

		/// \~English @brief Whether it is not equal to
		/// \~Chinese @brief 是否不等于
		Bool operator !=(OptionalBool val) { if (!valid && !val.valid) return FALSE; if (!valid || !val.valid) return TRUE; return value != val.value; }

		/// \~English @brief Convert to string, if invalid, convert to "(invalid)"
		/// \~Chinese @brief 转字符串，若无效则转为(invalid)
		String toString() { return valid ? String(value) : "(invalid)"; }

		/// \~English @brief Initialize based on spadas::GeneralElement, all non-zero values are TRUE
		/// \~Chinese @brief 基于 spadas::GeneralElement创建，非0都为TRUE
		SPADAS_API OptionalBool(GeneralElement elem);
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
		OptionalInt() : valid(FALSE), value(0) {}

		/// \~English @brief Initialize based on input integer value
		/// \~Chinese @brief 基于输入的整型值初始化
		OptionalInt(Int val) : valid(TRUE), value(val) {}

		/// \~English @brief Assignment by the input integer value
		/// \~Chinese @brief 按输入的整型值赋值
		OptionalInt& operator =(Int val) { valid = TRUE; value = val; return *this; }

		/// \~English @brief Whether it is equal to
		/// \~Chinese @brief 是否相等
		Bool operator ==(OptionalInt val) { if (!valid && !val.valid) return TRUE; if (!valid || !val.valid) return FALSE; return value == val.value; }

		/// \~English @brief Whether it is not equal to
		/// \~Chinese @brief 是否不等于
		Bool operator !=(OptionalInt val) { if (!valid && !val.valid) return FALSE; if (!valid || !val.valid) return TRUE; return value != val.value; }

		/// \~English @brief Convert to string, if invalid, convert to "(invalid)"
		/// \~Chinese @brief 转字符串，若无效则转为(invalid)
		String toString() { return valid ? String(value) : "(invalid)"; }

		/// \~English @brief Initialize based on spadas::GeneralElement
		/// \~Chinese @brief 基于 spadas::GeneralElement创建
		SPADAS_API OptionalInt(GeneralElement elem);
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
		OptionalDouble() : valid(FALSE), value(0) {}

		/// \~English @brief Initialize based on input floating point value
		/// \~Chinese @brief 基于输入的浮点值初始化
		OptionalDouble(Double val) : valid(TRUE), value(val) {}

		/// \~English @brief Assignment by the input floating point value
		/// \~Chinese @brief 按输入的浮点值赋值
		OptionalDouble& operator =(Double val) { valid = TRUE; value = val; return *this; }

		/// \~English @brief Addition, if any value is invalid, return an invalid value
		/// \~Chinese @brief 加法，若任一值为无效则返回无效值
		OptionalDouble operator +(OptionalDouble val) { return (valid && val.valid) ? (value + val.value) : OptionalDouble(); }

		/// \~English @brief Subtraction, if any value is invalid, return an invalid value
		/// \~Chinese @brief 减法，若任一值为无效则返回无效值
		OptionalDouble operator -(OptionalDouble val) { return (valid && val.valid) ? (value - val.value) : OptionalDouble(); }

		/// \~English @brief Multiplication, if any value is invalid, return an invalid value
		/// \~Chinese @brief 乘法，若任一值为无效则返回无效值
		OptionalDouble operator *(OptionalDouble val) { return (valid && val.valid) ? (value * val.value) : OptionalDouble(); }

		/// \~English @brief Whether it is equal to
		/// \~Chinese @brief 是否相等
		Bool operator ==(OptionalDouble val) { if (!valid && !val.valid) return TRUE; if (!valid || !val.valid) return FALSE; return value == val.value; }

		/// \~English @brief Whether it is not equal to
		/// \~Chinese @brief 是否不等于
		Bool operator !=(OptionalDouble val) { if (!valid && !val.valid) return FALSE; if (!valid || !val.valid) return TRUE; return value != val.value; }

		/// \~English @brief Convert string, if invalid, convert to "(invalid)"
		/// \~Chinese @brief 转字符串，若无效则转为(invalid)
		String toString() { return valid ? String(value) : "(invalid)"; }

		/// \~English @brief Initialize based on spadas::GeneralElement
		/// \~Chinese @brief 基于 spadas::GeneralElement创建
		SPADAS_API OptionalDouble(GeneralElement elem);
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

		/// \~English @brief Convert to time
		/// \~Chinese @brief 转为时间
		Time toTime();

	private:
		ULong value;
	};

	/// \~English @brief Time offset synchronization status
	/// \~Chinese @brief 时间偏置同步状态
	class SPADAS_API TimeOffsetSync
	{
	public:
		enum class Value
		{
			Default = 0,

			/// \~English @brief Host arrival time, not synchronized
			/// \~Chinese @brief 到达主机的时间，未同步
			HostArrival = 0,

			/// \~English @brief Sampling time, synchronized with the time server
			/// \~Chinese @brief 采样时间，已与授时服务器同步
			Server = 1,

			/// \~English @brief Sampling time, synchronized with satellite
			/// \~Chinese @brief 采样时间，已与卫星同步
			Gnss = 2,

			/// \~English @brief Bus receiver arrival time, synchronized with the time server
			/// \~Chinese @brief 到达总线设备的时间，已与授时服务器同步
			BusReceiverArrival = 3,

			/// \~English @brief The time obtained by interpolating two samples with different synchronization status
			/// \~Chinese @brief 同步状态不同的两个样本插值得到的时间
			Interpolated = 4,
		};
		SPADAS_ENUM_VALUES(HostArrival, Server, Gnss, BusReceiverArrival, Interpolated);
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

		/// \~English @brief Time offset source
		/// \~Chinese @brief 时间偏置来源
		TimeOffsetSync::Value offsetSync;

		/// \~English @brief Default constructor
		/// \~Chinese @brief 默认构造函数
		ShortTimestamp() : offset(0), offsetSync(TimeOffsetSync::Value::HostArrival) {}

		/// \~English @brief Initialize based on session identifier and time offset
		/// \~Chinese @brief 基于Session标识符和时间偏置初始化
		ShortTimestamp(SessionIdentifier session, Double offset, TimeOffsetSync::Value offsetSync = TimeOffsetSync::Value::HostArrival) : session(session), offset(offset), offsetSync(offsetSync) {}

		/// \~English @brief Whether it is equal to
		/// \~Chinese @brief 是否等于
		Bool operator ==(ShortTimestamp timestamp) { return session == timestamp.session && offset == timestamp.offset; }

		/// \~English @brief Whether it is not equal to
		/// \~Chinese @brief 是否不等于
		Bool operator !=(ShortTimestamp timestamp) { return session != timestamp.session || offset != timestamp.offset; }

		/// \~English @brief Is later than
		/// \~Chinese @brief 是否大于
		Bool operator >(ShortTimestamp timestamp) { if (session == timestamp.session) return offset > timestamp.offset; else return session > timestamp.session; }

		/// \~English @brief Is earlier than
		/// \~Chinese @brief 是否小于
		Bool operator <(ShortTimestamp timestamp) { if (session == timestamp.session) return offset < timestamp.offset; else return session < timestamp.session; }

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
		TimeOffsetSync::Value offsetSync;

		/// \~English @brief CPU tick when data arrives, 0 means invalid
		/// \~Chinese @brief 到达时CPU计数，0表示无效
		ULong cpuTick;

		/// \~English @brief Host posix time when data arrives, in nanoseconds, 0 means invalid
		/// \~Chinese @brief 到达时主机Posix时间，单位纳秒，0表示无效
		NanoPosix hostPosix;

		/// \~English @brief Guest posix time at data sampling, in nanoseconds, 0 means invalid
		/// \~Chinese @brief 采样时客机Posix时间，单位纳秒，0表示无效
		NanoPosix guestPosix;

		/// \~English @brief Satellite posix time at data sampling, in nanoseconds, 0 means invalid
		/// \~Chinese @brief 采样时卫星Posix时间，单位纳秒，0表示无效
		NanoPosix gnssPosix;

		/// \~English @brief Default constructor
		/// \~Chinese @brief 默认构造函数
		FullTimestamp() : offset(0), offsetSync(TimeOffsetSync::Value::HostArrival), cpuTick(0), hostPosix(0), guestPosix(0), gnssPosix(0) {}

		/// \~English @brief Create from a simple timestamp, no synchronization information or session-irrelevant time information
		/// \~Chinese @brief 从简单时间戳构造，无同步信息或Session无关时间信息
		FullTimestamp(ShortTimestamp timestamp) : session(timestamp.session), offset(timestamp.offset), offsetSync(timestamp.offsetSync), cpuTick(0), hostPosix(0), guestPosix(0), gnssPosix(0) {}

		/// \~English @brief Whether it is equal to (only compare session identifier and time offset)
		/// \~Chinese @brief 是否等于（仅比较Session标识符和时间偏置）
		Bool operator ==(FullTimestamp timestamp) { return session == timestamp.session && offset == timestamp.offset; }

		/// \~English @brief Whether it is not equal to (only compare the session identifier and time offset)
		/// \~Chinese @brief 是否不等于（仅比较Session标识符和时间偏置）
		Bool operator !=(FullTimestamp timestamp) { return session != timestamp.session || offset != timestamp.offset; }

		/// \~English @brief Is it later than (only compare session identifier and time offset)
		/// \~Chinese @brief 是否大于（仅比较Session标识符和时间偏置）
		Bool operator >(FullTimestamp timestamp) { if (session == timestamp.session) return offset > timestamp.offset; else return session > timestamp.session; }

		/// \~English @brief Is it earlier than (only compare session identifier and time offset)
		/// \~Chinese @brief 是否小于（仅比较Session标识符和时间偏置）
		Bool operator <(FullTimestamp timestamp) { if (session == timestamp.session) return offset < timestamp.offset; else return session < timestamp.session; }

		/// \~English @brief Convert to simple timestamp
		/// \~Chinese @brief 转为简单时间戳
		ShortTimestamp toShort() { return ShortTimestamp(session, offset, offsetSync); }

		/// \~English @brief Convert to a string, the format is "SessionYear-Month-Day-Hour-Minute-Second-Offset", such as 2019-01-01-12-30-45-123.456789
		/// \~Chinese @brief 转为字符串显示，格式为"Session年-月-日-时-分-秒-偏置"，如2019-01-01-12-30-45-123.456789
		SPADAS_API String toString();
	};

	/// \~English @brief Session irrelevant time category
	/// \~Chinese @brief Session无关时间的类别
	class SPADAS_API TimeType
	{
	public:
		enum class Value
		{
			Default = 0,

			/// \~English @brief CPU tick when data arrives
			/// \~Chinese @brief 到达时CPU计数
			CPUTick = 0,

			/// \~English @brief Host posix time when data arrives, in nanoseconds
			/// \~Chinese @brief 到达时主机Posix时间，单位纳秒
			HostPosix = 1,

			/// \~English @brief Guest posix time at data sampling, in nanoseconds
			/// \~Chinese @brief 采样时客机Posix时间，单位纳秒
			GuestPosix = 2,

			/// \~English @brief Satellite posix time at data sampling, in nanoseconds
			/// \~Chinese @brief 采样时卫星Posix时间，单位纳秒
			GnssPosix = 3,
		};
		SPADAS_ENUM_VALUES(CPUTick, HostPosix, GuestPosix, GnssPosix);
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
		SessionSignal() : value(0) {}

		/// \~English @brief Initialize based on timestamp and value
		/// \~Chinese @brief 基于时间戳和数值初始化
		SessionSignal(ShortTimestamp timestamp, Double val) : timestamp(timestamp), value(val) {}
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

		/// \~English @brief Numerical array data, which can be empty
		/// \~Chinese @brief 数值数组数据，可为空
		Array<Double> vector;

		/// \~English @brief Binary data, which can be empty
		/// \~Chinese @brief 二进制数据，可为空
		Binary binary;

		/// \~English @brief Default constructor
		/// \~Chinese @brief 默认构造函数
		GeneralDeviceData() {}

		/// \~English @brief Initialize based on timestamp and data
		/// \~Chinese @brief 基于时间戳和数据初始化
		GeneralDeviceData(ULong cpuTick, Array<Double> vector, Binary binary) : cpuTick(cpuTick), vector(vector), binary(binary) {}
	};

	/// \~English @brief General raw data
	/// \~Chinese @brief 通用原始数据
	struct SessionGeneralRawData
	{
		/// \~English @brief Timestamp
		/// \~Chinese @brief 时间戳
		FullTimestamp timestamp;

		/// \~English @brief Numerical array data, which can be empty
		/// \~Chinese @brief 数值数组数据，可为空
		Array<Double> vector;

		/// \~English @brief Binary data, which can be empty
		/// \~Chinese @brief 二进制数据，可为空
		Binary binary;

		/// \~English @brief Default constructor
		/// \~Chinese @brief 默认构造函数
		SessionGeneralRawData() {}

		/// \~English @brief Initialize based on timestamp and data
		/// \~Chinese @brief 基于时间戳和数据初始化
		SessionGeneralRawData(FullTimestamp timestamp, Array<Double> vector, Binary binary) : timestamp(timestamp), vector(vector), binary(binary) {}
	};

	/// \~English @brief General raw data table, the key is the raw data protocol ID whose format is "xxx-v?", "xxx" indicates the data source, "v?" indicates the version
	/// \~Chinese @brief 通用原始数据表，key为原始数据协议ID，一般格式为"xxx-v?"，xxx表示数据来源，v?表示版本
	typedef Dictionary<Array<SessionGeneralRawData> > SessionGeneralRawDataTable;

	/// \~English @brief Raw data transmit result
	/// \~Chinese @brief 原始数据发送结果
	class SPADAS_API TransmitResult
	{
	public:
		enum class Value
		{
			Default = 0,

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
		SPADAS_ENUM_VALUES(Unknown, Transmitted, Added, Scheduled, NotRunning, PluginNotFound, Failed, NotSync, ScheduleUnsupported, TimeDisorder, InvalidChannel, WrongCodec, WrongSize);
	};

	/// \~English @brief General raw data transmit interface
	/// \~Chinese @brief 通用原始数据发送接口
	class SPADAS_API IGeneralDataTransmitter
	{
	public:
		virtual ~IGeneralDataTransmitter() {};

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
		virtual TransmitResult::Value transmitNow(String protocol, Array<Double> vector, Binary binary);

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
		/// \~English @param guestSyncID Guest synchronization ID, based on which it is determined whether the guest device has synchronized with the time server (The format is "xxx.yyy", "xxx" is the plugin type ID, and "yyy" is the guest synchronization channel name)
		/// \~Chinese @param guestSyncID 客机同步ID，将据此确定客机是否已与授时服务器同步（格式为"xxx.yyy"，xxx为插件类型ID，yyy为客机同步通道名称）
		/// \~English @returns Transmit result
		/// \~Chinese @returns 发送结果
		virtual TransmitResult::Value transmitAtServerPosix(String protocol, Array<Double> vector, Binary binary, NanoPosix serverPosix, String guestSyncID);
	};

	/// \~English @brief Variables of sample
	/// \~Chinese @brief 样本的变量数据
	class SPADAS_API SessionSampleVars : public Vars
	{
	public:		
		/// \~English @brief Timestamp
		/// \~Chinese @brief 时间戳
		FullTimestamp timestamp;

		/// \~English @brief Specialized from general sample or matrix sample
		/// \~Chinese @brief 从通用样本或矩阵样本特化
		virtual Bool fromSample(String protocol, SessionSample sample);

		/// \~English @brief Whether specialized sample interpolation is supported
		/// \~Chinese @brief 是否支持特化样本插值
		virtual Bool supportInterpolation();

		/// \~English @brief Specialized sample interpolation
		/// \~Chinese @brief 特化样本插值
		virtual SessionSample interpolate(SessionSample& s1, Double w1, SessionSample& s2, Double w2, FullTimestamp timestamp);

		// SPADAS_VARS
		virtual String getTypeName() override;
		virtual Bool isType(ULong id) override;
		virtual Bool isType(String name) override;
	};

	/// \~English @brief Sample
	/// \~Chinese @brief 样本
	class SPADAS_API SessionSample : public Object<SessionSampleVars>
	{
	public:
		SPADAS_TYPE("spadas.SessionSample")

		/// \~English @brief Default constructor
		/// \~Chinese @brief 默认构造函数
		SessionSample();

		/// \~English @brief Get the reference of the timestamp
		/// \~Chinese @brief 取得时间戳的引用
		FullTimestamp& timestamp();

		/// \~English @brief Specialized from general sample or matrix sample
		/// \~Chinese @brief 从通用样本或矩阵样本特化
		Bool fromSample(String protocol, SessionSample sample);

		/// \~English @brief Whether specialized sample interpolation is supported
		/// \~Chinese @brief 是否支持特化样本插值
		Bool supportInterpolation();

		/// \~English @brief Specialized sample interpolation
		/// \~Chinese @brief 特化样本插值
		SessionSample interpolate(SessionSample& s1, Double w1, SessionSample& s2, Double w2, FullTimestamp timestamp);
	};

	/// \~English @brief General sample element
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
		GeneralElement() : valid(FALSE), isText(FALSE), value(0) {}

		/// \~English @brief Create a valid numerical element
		/// \~Chinese @brief 创建有效的值元素
		GeneralElement(Double value) : valid(TRUE), isText(FALSE), value(value) {}

		/// \~English @brief Create a valid text element
		/// \~Chinese @brief 创建有效的文本元素
		GeneralElement(String text) : valid(TRUE), isText(TRUE), value(0), text(text) {}

		/// \~English @brief Create based on spadas::OptionalBool, converted to 0 or 1
		/// \~Chinese @brief 基于 spadas::OptionalBool 创建，转为0或1
		GeneralElement(OptionalBool value) : valid(value.valid), isText(FALSE), value(value.value ? 1 : 0) {}

		/// \~English @brief Create based on spadas::OptionalInt
		/// \~Chinese @brief 基于 spadas::OptionalInt 创建
		GeneralElement(OptionalInt value) : valid(value.valid), isText(FALSE), value((Double)value.value) {}

		/// \~English @brief Create based on spadas::OptionalDouble
		/// \~Chinese @brief 基于 spadas::OptionalDouble 创建
		GeneralElement(OptionalDouble value) : valid(value.valid), isText(FALSE), value(value.value) {}

		/// \~English @brief Convert to string, if invalid, convert to "(invalid)"
		/// \~Chinese @brief 转字符串，若无效则转为(invalid)
		String toString() { return valid ? (isText ? text : String(value)) : "(invalid)"; }
	};

	/// \~English @brief General sample
	/// \~Chinese @brief 通用样本
	class SPADAS_API SessionGeneralSample : public Object<class SessionGeneralSampleVars>
	{
	public:
		SPADAS_TYPE("spadas.SessionGeneralSample")

		/// \~English @brief Default constructor
		/// \~Chinese @brief 默认构造函数
		SessionGeneralSample();

		/// \~English @brief Initialize based on timestamp and data
		/// \~Chinese @brief 基于时间戳和数据初始化
		SessionGeneralSample(FullTimestamp timestamp, Array<GeneralElement> values);

		/// \~English @brief Initialize based on timestamp, data, and number of key elements
		/// \~Chinese @brief 基于时间戳、数据、关键元素个数初始化
		SessionGeneralSample(FullTimestamp timestamp, Array<GeneralElement> values, UInt significantCount);

		/// \~English @brief Get the reference of the timestamp
		/// \~Chinese @brief 取得时间戳的引用
		FullTimestamp& timestamp();

		/// \~English @brief Get the reference of the element array
		/// \~Chinese @brief 取得样本数据数组的引用
		Array<GeneralElement>& values();

		/// \~English @brief Get the reference of the key element number, indicating that the first few in the sample element array are key data, the default is 0
		/// \~Chinese @brief 取得关键元素个数的引用，表示样本数据数组中前若干个为关键数据，默认为0
		UInt& significantCount();
	};

	/// \~English @brief Matrix sample
	/// \~Chinese @brief 矩阵样本
	class SPADAS_API SessionMatrixSample : public Object<class SessionMatrixSampleVars>
	{
	public:
		SPADAS_TYPE("spadas.SessionMatrixSample")

		/// \~English @brief Default constructor
		/// \~Chinese @brief 默认构造函数
		SessionMatrixSample();

		/// \~English @brief Initialize based on matrix size
		/// \~Chinese @brief 基于矩阵尺寸初始化
		SessionMatrixSample(Size2D size);

		/// \~English @brief Get the reference of the timestamp
		/// \~Chinese @brief 取得时间戳的引用
		FullTimestamp& timestamp();

		/// \~English @brief Get the reference of the matrix data, stored in the order of rows and columns. For example, the first element is the first row and the first column, the second element is the first row and the second column,...
		/// \~Chinese @brief 取得矩阵数据的引用，按行、列的顺序存储，如第0元素为第一行第一列，第1元素为第一行第二列，...
		Array<Float>& matData();

		/// \~English @brief Get the reference of the row number
		/// \~Chinese @brief 取得行数的引用
		UInt& rows();

		/// \~English @brief Get the reference of the column number
		/// \~Chinese @brief 取得列数的引用
		UInt& cols();
	};

	/// \~English @brief General sample data table, the key is the sample protocol ID whose format is "xxx-v?" or "xxx-v?@?", "xxx" indicates the sample type, "v?" indicates the version, "@?" indicates the channel index that starts from 0
	/// \~Chinese @brief 通用样本表，key为样本协议ID，一般格式为"xxx-v?"或"xxx-v?@?"，xxx表示样本类型，v?表示版本，@?表示通道序号，序号从0开始
	typedef Dictionary<Array<SessionGeneralSample> > SessionGeneralSampleTable;

	/// \~English @brief Matrix sample data table, the key is the sample protocol ID whose format is "xxx-v?" or "xxx-v?@?", "xxx" indicates the sample type, "v?" indicates the version, "@?" indicates the channel index that starts from 0
	/// \~Chinese @brief 矩阵样本表，key为样本协议ID，一般格式为"xxx-v?"或"xxx-v?@?"，xxx表示样本类型，v?表示版本，@?表示通道序号，序号从0开始
	typedef Dictionary<Array<SessionMatrixSample> > SessionMatrixSampleTable;

	/// \~English @brief Sample interpolation result
	/// \~Chinese @brief 样本插值结果
	class SPADAS_API SampleInterpolationResult
	{
	public:
		enum class Value
		{
			Default = 0,

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

			/// \~English @brief Sample parsing error
			/// \~Chinese @brief 样本解析错误
			ParseError = 4,

			/// \~English @brief The protocol of the sample buffer is not set. See spadas::SessionSampleBuffer::setProtocol
			/// \~Chinese @brief 未设置样本缓存的协议，详见 spadas::SessionSampleBuffer::setProtocol
			NoProtocol = 5,

			/// \~English @brief The output Type is not sub class of SessionSample
			/// \~Chinese @brief 输出Type类型非SessionSample子类
			InvalidType = 6,
		};
		SPADAS_ENUM_VALUES(OK, OutOfRange, NearestInstead, TooEarly, ParseError, NoProtocol, InvalidType);
	};

	/// \~English @brief [Multithread safe] Sample buffer
	/// \~Chinese @brief [多线程安全] 样本缓存
	class SPADAS_API SessionSampleBuffer : public Object<class SessionSampleBufferVars>
	{
	public:
		SPADAS_TYPE("spadas.SessionSampleBuffer")

		/// \~English @brief Create a sample buffer
		/// \~Chinese @brief 创建样本缓存
		SessionSampleBuffer();

		/// \~English @brief Set the maximum buffer duration
		/// \~Chinese @brief 设置缓存最大时长
		/// \~English @param length Maximum duration, in seconds, range 1~60 seconds, 3 seconds as default
		/// \~Chinese @param length 最大时长，单位秒，范围1~60秒，默认3秒
		void setBufferLength(Double length);

		/// \~English @brief Set the corresponding sample protocol
		/// \~Chinese @brief 设置对应的样本协议
		/// \~English @param protocol Sample protocol ID
		/// \~Chinese @param protocol 样本协议ID
		void setProtocol(String protocol);

		/// \~English @brief Get the corresponding sample protocol ID
		/// \~Chinese @brief 获取对应的样本协议ID
		/// \~English @param withChannel Whether to carry channel information, that is, "@?" at the end of the protocol ID
		/// \~Chinese @param withChannel 是否带通道信息，即协议ID末尾的"@?"
		/// \~English @returns Sample protocol ID
		/// \~Chinese @returns 样本协议ID
		String getProtocol(Bool withChannel);

		/// \~English @brief Add a new sample
		/// \~Chinese @brief 添加新样本
		/// \~English @param sample New sample, should be general sample or matrix sample
		/// \~Chinese @param sample 新样本，必须为通用样本或矩阵样本
		void addSample(SessionSample sample);

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
		Bool getEarliest(SessionSample& sampleEarliest);

		/// \~English @brief Get the latest sample
		/// \~Chinese @brief 获取最新样本
		/// \~English @param sampleEarliest Output the latest sample
		/// \~Chinese @param sampleEarliest 输出最新样本
		/// \~English @returns FALSE if the sample buffer is empty
		/// \~Chinese @returns 若样本缓存为空则返回FALSE
		Bool getLatest(SessionSample& sampleLatest);

		/// \~English @brief Find the nearest sample according to the time offset
		/// \~Chinese @brief 根据时间偏置寻找最近样本
		/// \~English @param offset Target time offset
		/// \~Chinese @param offset 目标时间偏置
		/// \~English @param sampleNearest Output the nearest sample to the target time
		/// \~Chinese @param sampleNearest 输出离目标最近样本
		/// \~English @returns FALSE if the sample buffer is empty
		/// \~Chinese @returns 若样本缓存为空，则返回FALSE
		Bool getNearest(Double offset, SessionSample& sampleNearest);

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
		Bool getNearest(Enum<TimeType> timeType, ULong time, SessionSample& sampleNearest);

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
		Bool search(Double offset, SessionSample& sampleBefore, SessionSample& sampleAfter);

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
		Bool search(Enum<TimeType> timeType, ULong time, SessionSample& sampleBefore, SessionSample& sampleAfter);

		/// \~English @brief Find the two samples just before and after the time offset, then perform interpolation
		/// \~Chinese @brief 根据时间偏置寻找前后两个样本并插值
		/// \~English @param offset Target time offset
		/// \~Chinese @param offset 目标时间偏置
		/// \~English @param interpolatedSample Output interpolated samples, while Type must be sub class of SessionSample
		/// \~Chinese @param interpolatedSample 输出插值完成的样本，Type必须为SessionSample的子类
		/// \~English @param earlyThresh The threshold used to determine whether the buffer range is too early, refer to spadas::SampleInterpolationResult::Value::TooEarly
		/// \~Chinese @param earlyThresh 用于判断缓存范围是否过早的阈值，参考 spadas::SampleInterpolationResult::Value::TooEarly
		template <typename Type>
		Enum<SampleInterpolationResult> interpolate(Double offset, Type& interpolatedSample, UInt earlyThresh = 1000/* ms */);

	private:
		Bool isNull() { return FALSE; }
		Bool isValid() { return FALSE; }
	};

	/// \~English @brief Sample buffer table, the key is the sample protocol ID whose format is "xxx-v?" or "xxx-v?@?", "xxx" indicates the sample type, "v?" indicates the version, "@?" indicates the channel number that starts from 0
	/// \~Chinese @brief 样本缓存表，key为样本协议ID，一般格式为"xxx-v?"或"xxx-v?@?"，xxx表示样本类型，v?表示版本，@?表示通道序号，序号从0开始
	typedef Dictionary<SessionSampleBuffer> SessionSampleBufferTable;

	/// \~English @brief General device status
	/// \~Chinese @brief 一般设备状态
	class SPADAS_API GeneralDeviceStatus
	{
	public:
		enum class Value
		{
			Default = 0,

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
		SPADAS_ENUM_VALUES(NotConnect, OK, Error, Warning);
	};

	/// \~English @brief Bus channel type
	/// \~Chinese @brief 总线通道类型
	class SPADAS_API BusChannelType
	{
	public:
		enum class Value
		{
			Default = 0,

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
		SPADAS_ENUM_VALUES(Invalid, Can, CanFD, Lin, Flexray, Ethernet, SomeIP);
	};

	/// \~English @brief Bus baud rate
	/// \~Chinese @brief 总线波特率
	class SPADAS_API BusBaudRate
	{
	public:
		enum class Value
		{
			Default = 0,
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
		SPADAS_ENUM_VALUES(Invalid, CAN_5k, CAN_10k, CAN_20k, CAN_33k, CAN_40k, CAN_50k, CAN_62k, CAN_80k, CAN_83k, CAN_100k, CAN_125k, CAN_200k, CAN_250k, CAN_400k, CAN_500k, CAN_666k, CAN_800k, CAN_1M, CANFD_500k, CANFD_1M, CANFD_2M, CANFD_4M, CANFD_8M, CANFD_6M, CANFD_2d5M, CANFD_5M, LIN_1k, LIN_9d6k, LIN_16d5k, LIN_19d2k, LIN_20k, FR_2d5M, FR_5M, FR_10M);
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
		BusDeviceData() : cpuTick(0), bridgeTimeOffset(0), bridgeGuestPosix(0), channel(0), id(0) {}
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
		Word channel;

		/// \~English @brief Whether it's TX message
		/// \~Chinese @brief 是否为发送报文
		Bool txFlag;

		/// \~English @brief Message ID in this channel
		/// \~Chinese @brief 该通道内的报文ID
		UInt id;

		/// \~English @brief Message data
		/// \~Chinese @brief 报文数据
		Binary binary;

		/// \~English @brief Default constructor
		/// \~Chinese @brief 默认构造函数
		SessionBusRawData() : channel(0), txFlag(FALSE), id(0) {}
	};

	/// \~English @brief Bus raw data table (The length is 16, representing bus channels 1~16 respectively)
	/// \~Chinese @brief 总线原始数据表（长度16, 分别表示总线通道1~16）
	typedef Array<Array<SessionBusRawData> > SessionBusRawDataTable;

	/// \~English @brief Bus raw data transmit interface
	/// \~Chinese @brief 总线报文发送接口
	class SPADAS_API IBusMessageTransmitter
	{
	public:
		virtual ~IBusMessageTransmitter() {};

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
		virtual TransmitResult::Value transmitNow(UInt channel, UInt id, Binary binary);

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
		virtual TransmitResult::Value transmitRepeatedly(UInt channel, UInt id, Binary binary, UInt interval);

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
		/// \~English @returns Transmit result
		/// \~Chinese @returns 发送结果
		virtual TransmitResult::Value transmitAtServerPosix(UInt channel, UInt id, Binary binary, NanoPosix serverPosix);
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
		BusDeviceID() : deviceSerial(0), deviceChannelIndex(0) {}
	};

	/// \~English @brief Bus device information
	/// \~Chinese @brief 总线设备信息
	struct BusDeviceInfo
	{
		/// \~English @brief Bus device ID
		/// \~Chinese @brief 总线设备ID
		BusDeviceID id;

		/// \~English @brief The bus types supported by this channel
		/// \~Chinese @brief 该通道支持的总线类型
		Array<Enum<BusChannelType> > types;

		/// \~English @brief Device channel description
		/// \~Chinese @brief 设备通道描述
		String description;

		/// \~English @brief Whether it supports scheduled transmitting
		/// \~Chinese @brief 是否支持按授时服务器Posix时间预约发送报文
		Bool supportTransmitScheduled;

		/// \~English @brief Default constructor
		/// \~Chinese @brief 默认构造函数
		BusDeviceInfo() : supportTransmitScheduled(FALSE) {}
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
		Enum<BusChannelType> type;

		/// \~English @brief Main baud rate
		/// \~Chinese @brief 主波特率
		Enum<BusBaudRate> rate;

		/// \~English @brief Secondary baud rate. Currently only used for spadas::BusChannelType::Value::CanFD , indicating the arbitration bit-rate
		/// \~Chinese @brief 副波特率。目前仅用于 spadas::BusChannelType::Value::CanFD ，表示arbitration bit-rate
		Enum<BusBaudRate> rate2;

		/// \~English @brief Software channel mapping, 1~16
		/// \~Chinese @brief 软件通道映射，1~16
		UInt mapChannel;

		/// \~English @brief Default constructor
		/// \~Chinese @brief 默认构造函数
		BusDeviceConfig() : mapChannel(0) {}
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
		BusChannelPayload() : mapChannel(0), payloadPercent(0) {}
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
		BusMessageInfo() : channel(0), localID(0) {}

		/// \~English @brief Convert to a message information identifier, the upper 32 bits are the software channel, and the lower 32 bits are the message ID
		/// \~Chinese @brief 转换为报文信息标识符，高32位为软件通道，低32位为本通道ID
		ULong indicator() { return ((ULong)channel << 32) | (ULong)localID; }
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
		SessionBusMessage() {}

		/// \~English @brief Initialize based on information, timestamp and data
		/// \~Chinese @brief 基于信息、时间戳和数据初始化
		SessionBusMessage(ShortTimestamp timestamp, BusMessageInfo info, Binary data) : timestamp(timestamp), info(info), data(data) {}
	};

	/// \~English @brief Bus message data table, the key is the global ID of the message whose format is "protocol file name:message ID in this channel", such as vehicle.dbc:123
	/// \~Chinese @brief 总线报文数据表，key为报文全局ID，格式为"协议文件名:本通道ID"，如vehicle.dbc:123
	typedef Dictionary<Array<SessionBusMessage> > SessionBusMessageTable;

	/// \~English @brief Video stream codec type
	/// \~Chinese @brief 视频数据流编码方式
	class SPADAS_API VideoDataCodec
	{
	public:
		enum class Value
		{
			Default = 0,

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
		SPADAS_ENUM_VALUES(Invalid, MJPEG, H264, YUV411, YUV420, H265, YUV422, RAW, RAW12, RAW14, RAW16, Y16);
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
		Enum<VideoDataCodec> codec;
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
		Enum<VideoDataCodec> codec;
	};

	/// \~English @brief Extra data of a video frame, such as hardware information, etc
	/// \~Chinese @brief 视频帧的额外数据，如硬件信息等
	struct VideoExtraData
	{
		/// \~English @brief ID of extra data
		/// \~Chinese @brief 额外数据的ID
		String id;

		/// \~English @brief Content of extra data
		/// \~Chinese @brief 额外数据内容
		Binary data;
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

		/// \~English @brief Optional preview image, BGR image with 640x(360-480) resolution
		/// \~Chinese @brief 可选的预览图像，640x(360-480)分辨率的BGR图像
		Optional<ImagePointer> preview;

		/// \~English @brief Optional extra data
		/// \~Chinese @brief 可选的额外数据
		Optional<VideoExtraData> extraData;

		/// \~English @brief Default constructor
		/// \~Chinese @brief 默认构造函数
		VideoDeviceData() : cpuTick(0), guestPosix(0), gnssPosix(0), channel(0) {}
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

		/// \~English @brief Optional preview image, BGR image with 640x(360-480) resolution
		/// \~Chinese @brief 可选的预览图像，640x(360-480)分辨率的BGR图像
		Optional<ImagePointer> preview;

		/// \~English @brief Optional extra data
		/// \~Chinese @brief 可选的额外数据
		Optional<VideoExtraData> extraData;

		/// \~English @brief Frame index in the session, which starts from 0, while -1 means invalid
		/// \~Chinese @brief 在Session内的帧序号，从0起算，-1表示无效
		Int frameIndex;

		/// \~English @brief Default constructor
		/// \~Chinese @brief 默认构造函数
		SessionVideoRawData() : channel(0), frameIndex(-1) {}
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
	class SPADAS_API VideoPreviewCapability
	{
	public:
		enum class Value
		{
			Default = 0,

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
		SPADAS_ENUM_VALUES(NoPreview, WithPreview, PreviewExpress);
	};

	/// \~English @brief Video device information
	/// \~Chinese @brief 视频设备信息
	struct VideoDeviceInfo
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
		Enum<VideoPreviewCapability> previewCapability;

		/// \~English @brief The video stream re-injection modes supported by the video device channel
		/// \~Chinese @brief 该视频设备通道支持的视频数据流回注模式
		Array<VideoOutputMode> outputModes;

		/// \~English @brief Whether it supports scheduled transmitting
		/// \~Chinese @brief 是否支持按授时服务器Posix时间预约发送视频帧
		Bool supportTransmitScheduled;

		/// \~English @brief Default constructor
		/// \~Chinese @brief 默认构造函数
		VideoDeviceInfo() : supportTransmitScheduled(FALSE) {}
	};

	/// \~English @brief Video device configuration
	/// \~Chinese @brief 视频设备配置
	struct VideoDeviceConfig
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
		VideoDeviceConfig() : mapChannel(0), frameRate(0) {}
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

		/// \~English @brief Extra data
		/// \~Chinese @brief 额外数据
		Optional<VideoExtraData> extraData;

		/// \~English @brief Frame index in the session, which starts from 0, while -1 means invalid
		/// \~Chinese @brief 在Session内的帧序号，从0起算，-1表示无效
		Int frameIndex;

		/// \~English @brief Default constructor
		/// \~Chinese @brief 默认构造函数
		SessionVideoProcData() : channel(0), frameIndex(-1) {}
	};

	/// \~English @brief Video data table for image processing (The length is 24, representing video channels A~X respectively)
	/// \~Chinese @brief 用于数据处理的视频数据表（长度24, 分别表示视频通道A~X）
	typedef Array<Array<SessionVideoProcData> > SessionVideoProcDataTable;

	/// \~English @brief Express output interface of video preview image
	/// \~Chinese @brief 视频预览图像的快速输出接口
	class SPADAS_API IVideoPreviewExpress
	{
	public:
		virtual ~IVideoPreviewExpress() {};

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
		/// \~English @param extraData Optional extra data
		/// \~Chinese @param extraData 可选的额外数据
		virtual void outputPreview(ULong cpuTick, UInt channel, ImagePointer preview, NanoPosix guestPosix = 0, NanoPosix gnssPosix = 0, Optional<VideoExtraData> extraData = Optional<VideoExtraData>());
	};

	/// \~English @brief Video raw data transmit interface
	/// \~Chinese @brief 视频帧回注接口
	class SPADAS_API IVideoFrameTransmitter
	{
	public:
		virtual ~IVideoFrameTransmitter() {};

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
		/// \~English @param extraData Optional extra data
		/// \~Chinese @param extraData 可选的额外数据
		/// \~English @returns Transmit result
		/// \~Chinese @returns 发送结果
		virtual TransmitResult::Value transmitNow(UInt channel, Enum<VideoDataCodec> codec, Size2D size, Binary data, Optional<VideoExtraData> extraData = Optional<VideoExtraData>());

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
		/// \~English @param extraData Optional extra data
		/// \~Chinese @param extraData 可选的额外数据
		/// \~English @returns Transmit result
		/// \~Chinese @returns 发送结果
		virtual TransmitResult::Value transmitAtServerPosix(UInt channel, Enum<VideoDataCodec> codec, Size2D size, Binary data, NanoPosix serverPosix, Optional<VideoExtraData> extraData = Optional<VideoExtraData>());
	};

	/// \~English @brief All input data tables
	/// \~Chinese @brief 所有输入数据表
	struct InputTables
	{
		/// \~English @brief Generic raw data table
		/// \~Chinese @brief 通用原始数据表
		SessionGeneralRawDataTable generalRawDatas;

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
		/// \~Chinese @brief 通用样本数据表
		SessionGeneralSampleTable generalSamples;

		/// \~English @brief Matrix sample data table
		/// \~Chinese @brief 矩阵样本数据表
		SessionMatrixSampleTable matrixSamples;

		/// \~English @brief Default constructor
		/// \~Chinese @brief 默认构造函数
		InputTables() : busRawDatas(BC_NUM), videoRawDatas(VC_NUM), videoProcDatas(VC_NUM) {}

		/// \~English @brief Clear all data
		/// \~Chinese @brief 清空所有数据
		void clear() { generalRawDatas.clear(); busMessages.clear(); signals.clear(); generalSamples.clear(); matrixSamples.clear(); busRawDatas = SessionBusRawDataTable(BC_NUM); videoRawDatas = SessionVideoRawDataTable(VC_NUM); videoProcDatas = SessionVideoProcDataTable(VC_NUM); }
	};

	/// \~English @brief All output data tables
	/// \~Chinese @brief 所有输出数据表
	struct OutputTables
	{
		/// \~English @brief Signal data table
		/// \~Chinese @brief 信号数据表
		SessionSignalTable signals;

		/// \~English @brief General sample data table
		/// \~Chinese @brief 通用样本数据表
		SessionGeneralSampleTable generalSamples;

		/// \~English @brief Matrix sample data table
		/// \~Chinese @brief 矩阵样本数据表
		SessionMatrixSampleTable matrixSamples;

		/// \~English @brief Default constructor
		/// \~Chinese @brief 默认构造函数
		OutputTables() {}

		/// \~English @brief Clear all data
		/// \~Chinese @brief 清空所有数据
		void clear() { signals.clear(); generalSamples.clear(); matrixSamples.clear(); }
	};

	/// \~English @brief The state of standalone task
	/// \~Chinese @brief 独立处理任务的状态
	class SPADAS_API StandaloneTaskState
	{
	public:
		enum class Value
		{
			Default = 0,

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
		SPADAS_ENUM_VALUES(Unknown, Finished, Failed, Progressing);
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
		/// \~English @param progress Task progress, unit %, which only needs to be set when the state is spadas::StandaloneTaskState::Value::Progressing
		/// \~Chinese @param progress 任务进度，单位%，仅当状态为 spadas::StandaloneTaskState::Value::Progressing 时需要设置
		virtual void setTaskProgress(Enum<StandaloneTaskState> state, String description, Double progress = 0);

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

		/// \~English @brief The identifier of the output session
		/// \~Chinese @brief 目标Session的标识符
		SessionIdentifier dstSession;

		/// \~English @brief The input folder path of the output session
		/// \~Chinese @brief 目标Session的input文件夹路径
		Path dstInputRoot;

		/// \~English @brief The generation folder path of the output Session
		/// \~Chinese @brief 目标Session的generation文件夹路径
		Path dstGenerationRoot;
	};

	/// \~English @brief File writing mode
	/// \~Chinese @brief 文件写入模式
	class SPADAS_API FileWriteMode
	{
	public:
		enum class Value
		{
			Default = 0,

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
		SPADAS_ENUM_VALUES(Normal, OnlineMode, OfflineMode);
	};

	/// \~English @brief File reading mode
	/// \~Chinese @brief 文件读取模式
	class SPADAS_API FileReadMode
	{
	public:
		enum class Value
		{
			Default = 0,

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
		SPADAS_ENUM_VALUES(Normal, FromRaw, OfflineModeFromGeneration, ReplayModeFromGeneration, FromRemote);
	};

	/// \~English @brief File R/W filter
	/// \~Chinese @brief 文件读写筛选
	class SPADAS_API FileIOFilter
	{
	public:
		enum class Value
		{
			Default = 0,

			/// \~English @brief Invalid
			/// \~Chinese @brief 无效值
			Invalid = 0,
			
			/// \~English @brief Signal data
			/// \~Chinese @brief 信号数据
			Signal = 1,

			/// \~English @brief General sample data
			/// \~Chinese @brief 通用样本数据
			GeneralSample = 2,

			/// \~English @brief Matrix sample data
			/// \~Chinese @brief 矩阵样本数据
			MatrixSample = 3,

			/// \~English @brief Bus channel 1 data
			/// \~Chinese @brief 总线通道1数据
			BusCH1 = 100,
			BusCH2 = 101,
			BusCH3 = 102,
			BusCH4 = 103,
			BusCH5 = 104,
			BusCH6 = 105,
			BusCH7 = 106,
			BusCH8 = 107,
			BusCH9 = 108,
			BusCH10 = 109,
			BusCH11 = 110,
			BusCH12 = 111,
			BusCH13 = 112,
			BusCH14 = 113,
			BusCH15 = 114,
			BusCH16 = 115,

			/// \~English @brief Video channel A data
			/// \~Chinese @brief 视频通道A数据
			VideoChannelA = 200,
			VideoChannelB = 201,
			VideoChannelC = 202,
			VideoChannelD = 203,
			VideoChannelE = 204,
			VideoChannelF = 205,
			VideoChannelG = 206,
			VideoChannelH = 207,
			VideoChannelI = 208,
			VideoChannelJ = 209,
			VideoChannelK = 210,
			VideoChannelL = 211,
			VideoChannelM = 212,
			VideoChannelN = 213,
			VideoChannelO = 214,
			VideoChannelP = 215,
			VideoChannelQ = 216,
			VideoChannelR = 217,
			VideoChannelS = 218,
			VideoChannelT = 219,
			VideoChannelU = 220,
			VideoChannelV = 221,
			VideoChannelW = 222,
			VideoChannelX = 223,
		};
		SPADAS_ENUM_VALUES(Invalid, Signal, GeneralSample, MatrixSample, BusCH1, BusCH2, BusCH3, BusCH4, BusCH5, BusCH6, BusCH7, BusCH8, BusCH9, BusCH10, BusCH11, BusCH12, BusCH13, BusCH14, BusCH15, BusCH16, VideoChannelA, VideoChannelB, VideoChannelC, VideoChannelD, VideoChannelE, VideoChannelF, VideoChannelG, VideoChannelH, VideoChannelI, VideoChannelJ, VideoChannelK, VideoChannelL, VideoChannelM, VideoChannelN, VideoChannelO, VideoChannelP, VideoChannelQ, VideoChannelR, VideoChannelS, VideoChannelT, VideoChannelU, VideoChannelV, VideoChannelW, VideoChannelX);
	};

	/// \~English @brief File R/W basic information
	/// \~Chinese @brief 文件读写基本信息
	struct FileIOBasicInfo
	{
		/// \~English @brief Session identifier
		/// \~Chinese @brief Session标识符
		SessionIdentifier session;

		/// \~English @brief Filter items, empty means no filter
		/// \~Chinese @brief 筛选项，空表示不进行筛选
		Array<Enum<FileIOFilter> > filter;

		/// \~English @brief File writing mode
		/// \~Chinese @brief 文件写入模式
		Enum<FileWriteMode> writeMode;

		/// \~English @brief File reading mode
		/// \~Chinese @brief 文件读取模式
		Enum<FileReadMode> readMode;

		/// \~English @brief Data password (unnecessary for non-encrypted files)
		/// \~Chinese @brief 数据密码（非加密文件无需使用）
		String password;
	};

	/// \~English @brief File R/W extended information
	/// \~Chinese @brief 文件读写扩展信息
	struct FileIOExtInfo
	{
		/// \~English @brief Type of each bus channel, empty means no bus data reading or writing
		/// \~Chinese @brief 各总线通道类型，空表示无总线数据读写
		Array<Enum<BusChannelType> > busChannelTypes;

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

		/// \~English @brief Recalculate session-irrelevant posix time based on the input time offset and synchronization status
		/// \~Chinese @brief 根据输入时间戳的时间偏置和同步状态反算对应的Session无关Posix时间
		/// \~English @param srcTimestamp Input timestamp
		/// \~Chinese @param srcTimestamp 输入时间戳
		/// \~English @returns The session-irrelevant posix time, in nanoseconds, 0 means failure
		/// \~Chinese @returns 输出的Session无关Posix时间，单位纳秒，0表示失败
		virtual NanoPosix calcPosixTime(ShortTimestamp srcTimestamp);

		/// \~English @brief Recalculate CPU tick based on the input time offset
		/// \~Chinese @brief 根据输入时间偏置反算对应的CPU计数
		/// \~English @param session Related session
		/// \~Chinese @param session 对应的session
		/// \~English @param timeOffset Input time offset
		/// \~Chinese @param timeOffset 输入的时间偏置
		/// \~English @returns The CPU tick, 0 means failure
		/// \~Chinese @returns 输出的CPU计数，0表示失败
		virtual ULong calcCPUTick(SessionIdentifier session, Double timeOffset);
	};

	// Plugin related utility functions / 插件相关实用功能 //////////////////////////////////////////////////////////////

	/// \~English @brief Bus protocol ID (like xxx.dbc)
	/// \~Chinese @brief 总线协议ID (形如xxx.dbc)
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
		BusMessageID() : message(0) {}

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
		BusSignalID() : message(0) {}

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
		BusSignedSignalConfig() : positiveAsZero(FALSE) {}

		/// \~English @brief Is sign bit signal not configured
		/// \~Chinese @brief 符号位信号配置是否有效
		Bool isSignOptional() { return signSignalID.isEmpty() || signSignalID == "null"; }
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
		OptionalSignedDouble() : positive(TRUE), value(0), signValid(FALSE), valueValid(FALSE) {}

		/// \~English @brief Set positive floating point value
		/// \~Chinese @brief 设置正浮点值
		void setValue(Double value) { this->value = value; this->valueValid = TRUE; }

		/// \~English @brief Set the sign bit
		/// \~Chinese @brief 设置符号位
		void setSign(Bool positive) { this->positive = positive; this->signValid = TRUE; }

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

		/// \~English @brief [Unsafe] Obtain the field reference under the current definition (The "has" method should be called before using the field to see whether it is supported)
		/// \~Chinese @brief [非安全] 获取当前定义下的数据类型引用（使用字段前应先调用has方法确定是否支持）
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
		SPADAS_TYPE("spadas.GeneralIOObject")

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

	/// \~English @brief Variables of spadas::SampleParser template class
	/// \~Chinese @brief spadas::SampleParser 模板类的变量数据
    template <typename Type> class SampleParserVars;

	/// \~English @brief Convenience for sample parsing
	/// \~Chinese @brief 方便解析样本
    template <typename Type>
    class SampleParser : Object<SampleParserVars<Type> >
    {
    public:
        static String typeName();

		/// \~English @brief Create invalid object
		/// \~Chinese @brief 创建无效对象
        SampleParser();

		/// \~English @brief Create sample parser, bound to the list of protocol IDs
		/// \~Chinese @brief 创建样本解析器，绑定至指定协议ID列表
        SampleParser(Array<String> protocols);

		/// \~English @brief You should call this method to reset when a session starts
		/// \~Chinese @brief 开始Session时应调用此方法重置
        void reset();

		/// \~English @brief All specialized samples are parsed from the newly input general sample table
		/// \~Chinese @brief 从新输入的通用样本表解析出所有特化样本
        Array<Type> parse(SessionGeneralSampleTable table);

		/// \~English @brief All specialized samples are parsed from the newly input matrix sample table
		/// \~Chinese @brief 从新输入的矩阵样本表解析出所有特化样本
        Array<Type> parse(SessionMatrixSampleTable table);
    };

	/// \~English @brief Variables of spadas::SampleSync template class
	/// \~Chinese @brief spadas::SampleSync 模板类的变量数据
    template <typename Type> class SampleSyncVars;

	/// \~English @brief Convenience for sample synchronization
	/// \~Chinese @brief 方便同步样本
    template <typename Type>
    class SampleSync : Object<SampleSyncVars<Type> >
    {
    public:
        static String typeName();

		/// \~English @brief Create invalid object
		/// \~Chinese @brief 创建无效对象
        SampleSync();

		/// \~English @brief Create sample synchronizer, bound to the list of protocol IDs
		/// \~Chinese @brief 创建样本同步器，绑定至指定协议ID列表
        SampleSync(Array<String> protocols);

		/// \~English @brief You should call this method to reset when a session starts
		/// \~Chinese @brief 开始Session时应调用此方法重置
        void reset();

		/// \~English @brief Find the two samples just before and after the timestamp in the sample buffer, then perform interpolation. See spadas::SessionSampleBuffer::interpolate for arguments' description
		/// \~Chinese @brief 根据时间戳在输入的样本缓存中寻找前后两个样本并插值，参数详见 spadas::SessionSampleBuffer::interpolate
        Enum<SampleInterpolationResult> interpolate(SessionSampleBufferTable table, SessionIdentifier session, Double timeOffset, Type& interpolatedSample, UInt earlyThresh = 1000/* ms */);
    };

	/// \~English @brief Object container for raw data exchange
	/// \~Chinese @brief 用于原始数据交换的对象容器
	class SPADAS_API GeneralRawObject : public Object<class GeneralRawObjectVars>
	{
	public:
		SPADAS_TYPE("spadas.GeneralRawObject")

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

	/// \~English @brief General function plugin interface 2.0
	/// \~Chinese @brief 通用功能插件接口 2.0
	class SPADAS_API IPluginV200
	{
	public:
		virtual ~IPluginV200() {};

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
		/// \~English @param languageCode Language code, "en" means English, "zh" means Chinese
		/// \~Chinese @param languageCode 语言代号，"en"表示英文，"zh"表示中文
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

	/// \~English @brief Function definition of getting the general function plugin interface, the function name should be get_plugin_v200
	/// \~Chinese @brief 获取通用功能插件接口的全局函数定义，函数名应为get_plugin_v200
	typedef Interface<IPluginV200>(*GetPluginV200)();

	/// \~English @brief General device plugin interface 3.0
	/// \~Chinese @brief 一般设备插件接口 3.0
	class SPADAS_API IDevicePluginV300
	{
	public:
		virtual ~IDevicePluginV300() {};

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
		virtual Enum<GeneralDeviceStatus> getDeviceStatus(String& info);

		/// \~English @brief [Optional] Get the connection status of each sub-device
		/// \~Chinese @brief [可选] 获取各子设备的连接状态
		/// \~English @returns The connection status of each sub-device, the length of the array is the number of sub-device
		/// \~Chinese @returns 返回各子设备的连接状态，数组长度即子设备数量
		virtual Array<Enum<GeneralDeviceStatus> > getChildDeviceStatus();

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
		/// \~English @returns Whether the scheduled transmitting is successful, FALSE if the protocol is not in the list
		/// \~Chinese @returns 返回是否成功预约发送一帧数据，若协议未在可发送的协议列表内则返回FALSE
		virtual Bool transmitGeneralDataScheduled(String protocol, Array<Double> vector, Binary binary, NanoPosix serverPosix);
	};

	/// \~English @brief Function definition of getting the general device plugin interface, the function name should be get_device_plugin_v300
	/// \~Chinese @brief 获取一般设备插件接口的全局函数定义，函数名应为get_device_plugin_v300
	typedef Interface<IDevicePluginV300>(*GetDevicePluginV300)();

	/// \~English @brief Bus device plugin interface 3.0
	/// \~Chinese @brief 总线设备插件接口 3.0
	class SPADAS_API IBusPluginV300
	{
	public:
		virtual ~IBusPluginV300() {};

		/// \~English @brief Get the bus device list
		/// \~Chinese @brief 获取总线设备列表
		/// \~English @returns The list of bus devices
		/// \~Chinese @returns 返回总线设备列表
		virtual Array<BusDeviceInfo> getBusDeviceList();

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
		/// \~English @returns Whether the scheduled transmitting is successful
		/// \~Chinese @returns 返回是否成功预约发送一帧数据
		virtual Bool transmitBusMessageScheduled(UInt channel, UInt id, Binary binary, NanoPosix serverPosix);

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

	/// \~English @brief Function definition of getting the bus device plugin interface, the function name should be get_bus_plugin_v300
	/// \~Chinese @brief 获取总线设备插件接口的全局函数定义，函数名应为get_bus_plugin_v300
	typedef Interface<IBusPluginV300>(*GetBusPluginV300)();

	/// \~English @brief Video device plugin interface 5.0
	/// \~Chinese @brief 视频设备插件接口 5.0
	class SPADAS_API IVideoPluginV500
	{
	public:
		virtual ~IVideoPluginV500() {};

		/// \~English @brief Get video device list
		/// \~Chinese @brief 获取视频设备列表
		/// \~English @returns Video device list
		/// \~Chinese @returns 视频设备列表
		virtual Array<VideoDeviceInfo> getVideoDeviceList();

		/// \~English @brief Open the video device (called while starting the session)
		/// \~Chinese @brief 打开视频设备（在开始Session时被调用）
		/// \~English @param configs The list of video device channels and related configurations
		/// \~Chinese @param configs 希望打开的视频设备通道列表及相关配置
		virtual Bool openVideoDevice(Array<VideoDeviceConfig> configs);

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
		virtual Bool transmitVideoFrame(UInt channel, Enum<VideoDataCodec> codec, Size2D size, Binary data);

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
		/// \~English @returns Whether the scheduled transmitting is successful
		/// \~Chinese @returns 返回是否成功预约发送一帧数据
		virtual Bool transmitVideoFrameScheduled(UInt channel, Enum<VideoDataCodec> codec, Size2D size, Binary data, NanoPosix serverPosix);

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
		virtual void useVideoPreviewExpress(Interface<IVideoPreviewExpress> previewExpress);

		/// \~English @brief [Optional] Get the video device exclusive keyword, other video devices whose description matching this keyword will be disabled
		/// \~Chinese @brief [可选] 获取视频设备独占关键字，其他插件匹配此关键字的视频设备将被禁用
		/// \~English @returns Video device exclusive keywords
		/// \~Chinese @returns 返回视频设备独占关键字
		virtual Array<String> getExclusiveKeywords();
	};

	/// \~English @brief Function definition of getting the video device plugin interface, the function name should be get_video_plugin_v500
	/// \~Chinese @brief 获取视频设备插件接口的全局函数定义，函数名应为get_video_plugin_v500
	typedef Interface<IVideoPluginV500>(*GetVideoPluginV500)();

	/// \~English @brief Native data processing plugin interface 7.0
	/// \~Chinese @brief 原生数据处理插件接口 7.0
	class SPADAS_API IProcessorPluginV700
	{
	public:
		virtual ~IProcessorPluginV700() {};

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
		virtual void processData(InputTables inputs, SessionSampleBufferTable sampleBuffers, OutputTables outputs);

		/// \~English @brief [Optional] Set to use the specified time-related service interface
		/// \~Chinese @brief [可选] 设置使用指定的时间相关服务接口
		/// \~English @param timeServer Time related service interface
		/// \~Chinese @param timeServer 时间相关服务接口
		virtual void useTimeServer(Interface<ITimeServer> timeServer);

		/// \~English @brief [Optional] Set to use the specified general raw data transmitting interface
		/// \~Chinese @brief [可选] 设置使用指定的通用原始数据发送接口
		/// \~English @param generalTransmitter General raw data transmitting interface
		/// \~Chinese @param generalTransmitter 通用原始数据发送接口
		virtual void useGeneralTransmitter(Interface<IGeneralDataTransmitter> generalTransmitter);

		/// \~English @brief [Optional] Set to use the specified bus message transmitting interface
		/// \~Chinese @brief [可选] 设置使用指定的总线报文发送接口
		/// \~English @param busTransmitter Bus message transmitting interface
		/// \~Chinese @param busTransmitter 总线报文发送接口
		virtual void useBusTransmitter(Interface<IBusMessageTransmitter> busTransmitter);

		/// \~English @brief [Optional] Set to use the specified video frame re-injection interface
		/// \~Chinese @brief [可选] 设置使用指定的视频帧回注接口
		/// \~English @param videoTransmitter Video frame re-injection interface
		/// \~Chinese @param videoTransmitter 视频帧回注接口
		virtual void useVideoTransmitter(Interface<IVideoFrameTransmitter> videoTransmitter);
	};

	/// \~English @brief Function definition of getting the native data processing plugin interface, the function name should be get_processor_plugin_v700
	/// \~Chinese @brief 获取原生数据处理插件接口的全局函数定义，函数名应为get_processor_plugin_v700
	typedef Interface<IProcessorPluginV700>(*GetProcessorPluginV700)();

	/// \~English @brief File R/W plugin interface 2.0
	/// \~Chinese @brief 文件读写插件接口 2.0
	class SPADAS_API IFilePluginV200
	{
	public:
		virtual ~IFilePluginV200() {};

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
		virtual Bool readFilesData(String readerName, InputTables inputs, Double targetOffset, Flag shouldEnd);

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
		virtual void writeFilesData(String writerName, InputTables inputs, Array<SessionBusRawData> busMessages, Flag shouldEnd);

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

		/// \~English @brief [Optional] Additional settings for file R/W (called before each openRead/WriteFiles function)
		/// \~Chinese @brief [可选] 对文件读写进行额外设置（在各openRead/WriteFiles函数前被调用）
		/// \~English @param extra Extra configuration information
		/// \~Chinese @param extra 配置信息
		virtual void setFileExtraConfig(String extra);
	};

	/// \~English @brief Function definition of getting the file R/W plugin interface, the function name should be get_file_plugin_v200
	/// \~Chinese @brief 获取文件读写插件接口的全局函数定义，函数名应为get_file_plugin_v200
	typedef Interface<IFilePluginV200>(*GetFilePluginV200)();
}

#endif
