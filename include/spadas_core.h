
#ifndef SPADAS_CORE_H
#define SPADAS_CORE_H

/// Spadas根命名空间
namespace spadas
{
	// 值类型 //////////////////////////////////////////////////////////////

	/// 布尔类型
	typedef bool Bool;

	/// 8位无符号整数
	typedef unsigned char Byte;

	/// 16位无符号整数
	typedef unsigned short Word;

	/// 32位无符号整数
	typedef unsigned int UInt;

	/// 64位无符号整数
	typedef unsigned long long ULong;

	/// 16位带符号整数
	typedef short Short;

	/// 32位带符号整数
	typedef int Int;

	/// 64位带符号整数
	typedef long long Long;
	
	/// UTF-8字符
	typedef char Char;
	
	/// Windows: UTF-16字符, Linux: UTF-32字符
	typedef wchar_t WChar;
	
	/// 32位浮点数，单精度
	typedef float Float;

	/// 64位浮点数，双精度
	typedef double Double;

	/// 读写数据指针
	typedef void *Pointer;

	/// 只读数据指针
	typedef const void *ConstPointer;
	
	/// 兼容指针的整数类型
	typedef ULong PointerInt;

	// 常数 //////////////////////////////////////////////////////////////
	
	/// 布尔值真
	const Bool TRUE = true;

	/// 布尔值假
	const Bool FALSE = false;

	/// spadas::UInt 类型无限 (亦可用于表示无效数值)
	const UInt UINF = 0xffffffff;

	/// spadas::ULong 类型无限 (亦可用于表示无效数值)
	const ULong ULINF = 0xffffffffffffffffull;

	/// spadas::Int 类型正无限 (亦可用于表示无效数值)
	const Int INF = 0x7fffffff;

	/// spadas::Int 类型负无限 (亦可用于表示无效数值)
	const Int NINF = 0x80000000;

	/// spadas::Long 类型正无限 (亦可用于表示无效数值)
	const Long LINF = 0x7fffffffffffffffll;

	/// spadas::Long 类型负无限 (亦可用于表示无效数值)
	const Long NLINF = 0x8000000000000000ll;

	// 前置声明 //////////////////////////////////////////////////////////////

	// 字符串
	class String;

	// 字符串片段
	class StringSpan;

	/// 字符串拼接器
	class StringAppender;

	// 可空对象
	template <typename Type> class Optional;

	// 数组
	template <typename Type> class Array;

	// 数组片段
	template <typename Type> class ArraySpan;

	// 链表节点
	template <typename Type> class ListNode;

	// 二进制数据
	class Binary;

	// 二进制数据片段
	class BinarySpan;

	// 图像对
	struct ImagePair;

	// 标志位
	class Flag;

	// 空间与索引 //////////////////////////////////////////////////////////////

	/// 2维尺寸
	struct Size2D
	{
		/// 第2维的长度，或图像宽度
		union { UInt width; UInt dim1; };

		/// 第1维的长度，或图像高度
		union { UInt height; UInt dim0; };

		/// 默认构造函数，值初始化为0
		SPADAS_API Size2D();

		/// 是否等于
		SPADAS_API Bool operator ==(Size2D size);

		/// 是否不等于
		SPADAS_API Bool operator !=(Size2D size);

		/// 转字符串: [width]x[height]
		SPADAS_API String toString();

		/// 基于宽高初始化
		SPADAS_API static Size2D wh(UInt width, UInt height);

		/// 基于维度初始化
		SPADAS_API static Size2D dims(UInt dim0, UInt dim1);
	};

	/// 3维尺寸
	struct Size3D
	{
		union { UInt width; UInt dim1; };
		union { UInt height; UInt dim0; };
		union { UInt depth; UInt dim2; };

		SPADAS_API Size3D();
		SPADAS_API Bool operator ==(Size3D size);
		SPADAS_API Bool operator !=(Size3D size);
		SPADAS_API String toString(); // [width]x[height]x[depth]
		SPADAS_API static Size3D whd(UInt width, UInt height, UInt depth);
		SPADAS_API static Size3D dims(UInt dim0, UInt dim1, UInt dim2);
	};

	/// 1维区域（整型）
	struct Region
	{
		/// 区域起始位置
		Int offset;

		/// 区域大小
		UInt size;
		
		/// 默认构造函数，值初始化为0
		SPADAS_API Region();

		/// 基于起始位置和大小初始化
		SPADAS_API Region(Int offset, UInt size);

		/// 是否等于
		SPADAS_API Bool operator ==(Region region);

		/// 是否不等于
		SPADAS_API Bool operator !=(Region region);

		/// 转字符串: offset->size
		SPADAS_API String toString();
	};

	/// 2维序号
	struct Index2D
	{
		union { Int u; Int j; };
		union { Int v; Int i; };

		SPADAS_API Index2D();
		SPADAS_API Bool operator ==(Index2D index);
		SPADAS_API Bool operator !=(Index2D index);
		SPADAS_API String toString(); // (u,v)
		SPADAS_API static Index2D uv(Int u, Int v);
		SPADAS_API static Index2D ij(Int i, Int j);
	};

	/// 2维整数型坐标
	typedef Index2D CoordInt2D;

	/// 3维序号
	struct Index3D
	{
		union { Int u; Int j; };
		union { Int v; Int i; };
		union { Int w; Int k; };

		SPADAS_API Index3D();
		SPADAS_API Bool operator ==(Index3D index);
		SPADAS_API Bool operator !=(Index3D index);
		SPADAS_API String toString(); // (u,v,w)
		SPADAS_API static Index3D uvw(Int u, Int v, Int w);
		SPADAS_API static Index3D ijk(Int i, Int j, Int k);
	};

	/// 3维整数型坐标
	typedef Index3D CoordInt3D;

	/// 2维区域（整型）
	struct Region2D
	{
		union { Int offsetU; Int offsetJ; };
		union { Int offsetV; Int offsetI; };
		union { UInt width; UInt dim1; };
		union { UInt height; UInt dim0; };

		SPADAS_API Region2D();
		SPADAS_API Region2D(CoordInt2D offset, Size2D size);
		SPADAS_API Bool operator ==(Region2D region);
		SPADAS_API Bool operator !=(Region2D region);
		SPADAS_API CoordInt2D offset();
		SPADAS_API Size2D size();
		SPADAS_API String toString(); // (offsetU->width,offsetV->height)
	};

	/// 3维区域（整型）
	struct Region3D
	{
		union { Int offsetU; Int offsetJ; };
		union { Int offsetV; Int offsetI; };
		union { Int offsetW; Int offsetK; };
		union { UInt width; UInt dim1; };
		union { UInt height; UInt dim0; };
		union { UInt depth; UInt dim2; };

		SPADAS_API Region3D();
		SPADAS_API Region3D(CoordInt3D offset, Size3D size);
		SPADAS_API Bool operator ==(Region3D region);
		SPADAS_API Bool operator !=(Region3D region);
		SPADAS_API CoordInt3D offset();
		SPADAS_API Size3D size();
		SPADAS_API String toString(); // (offsetU->width,offsetV->height,offsetW->depth)
	};

	/// 2维矢量
	struct Vector2D
	{
		/// 第1维坐标
		union { Float x; Float u; Float j; };

		/// 第2维坐标
		union { Float y; Float v; Float i; };

		/// 默认构造函数，值初始化为0
		SPADAS_API Vector2D();

		/// 基于2维坐标初始化
		SPADAS_API Vector2D(Float x, Float y);

		/// 是否等于
		SPADAS_API Bool operator ==(Vector2D vec);

		/// 是否不等于
		SPADAS_API Bool operator !=(Vector2D vec);

		/// 矢量相加
		SPADAS_API Vector2D operator +(Vector2D vec);

		/// 矢量相减
		SPADAS_API Vector2D operator -(Vector2D vec);

		/// 矢量乘以比例系数
		SPADAS_API Vector2D operator *(Float k);

		/// 矢量内积（点乘）
		SPADAS_API Float operator *(Vector2D vec);

		/// 计算矢量的绝对值（长度）
		SPADAS_API Float absolute();

		/// 转字符串: (x,y)
		SPADAS_API String toString();

		/// @brief 基于图像坐标初始化
		/// @param u 图像横坐标
		/// @param v 图像纵坐标
		SPADAS_API static Vector2D uv(Float u, Float v);

		/// @brief 基于矩阵元素位置初始化
		/// @param i 矩阵第1维坐标
		/// @param j 矩阵第2维坐标
		SPADAS_API static Vector2D ij(Float i, Float j);
	};

	/// 2维点
	typedef Vector2D Point2D;

	/// 2维坐标
	typedef Vector2D CoordFloat2D;

	/// 3维矢量
	struct Vector3D
	{
		union { Float x; Float u; Float j; };
		union { Float y; Float v; Float i; };
		union { Float z; Float w; Float k; };

		SPADAS_API Vector3D();
		SPADAS_API Vector3D(Float x, Float y, Float z);
		SPADAS_API Bool operator ==(Vector3D vec);
		SPADAS_API Bool operator !=(Vector3D vec);
		SPADAS_API Vector3D operator +(Vector3D vec);
		SPADAS_API Vector3D operator -(Vector3D vec);
		SPADAS_API Vector3D operator *(Float k);		// 比例
		SPADAS_API Float operator *(Vector3D vec);	// 内积（点乘）
		SPADAS_API Float absolute();
		SPADAS_API String toString(); // (x,y,z)
		SPADAS_API static Vector3D uvw(Float u, Float v, Float w);
		SPADAS_API static Vector3D ijk(Float i, Float j, Float k);
	};

	/// 3维点
	typedef Vector3D Point3D;

	/// 3维坐标
	typedef Vector3D CoordFloat3D;

	/// 1维区域（浮点）
	struct Range
	{
		/// 区域下沿
		Float lower;

		/// 区域上沿
		Float upper;

		/// 默认构造函数，初始化为0
		SPADAS_API Range();

		/// 基于上下沿初始化
		SPADAS_API Range(Float lower, Float upper);

		/// 是否等于
		SPADAS_API Bool operator ==(Range range);

		/// 是否不等于
		SPADAS_API Bool operator !=(Range range);

		/// 转字符串: (lower~upper)
		SPADAS_API String toString();
	};

	/// 2维区域（浮点）
	struct Range2D
	{
		union { Float lowerX; Float lowerU; Float lowerJ; };
		union { Float upperX; Float upperU; Float upperJ; };
		union { Float lowerY; Float lowerV; Float lowerI; };
		union { Float upperY; Float upperV; Float upperI; };

		SPADAS_API Range2D();
		SPADAS_API Range2D(Float lowerX, Float upperX, Float lowerY, Float upperY);
		SPADAS_API Bool operator ==(Range2D range);
		SPADAS_API Bool operator !=(Range2D range);
		SPADAS_API String toString(); // (lowerX~upperX,lowerY~upperY)
		SPADAS_API static Range2D uv(Float lowerU, Float upperU, Float lowerV, Float upperV);
		SPADAS_API static Range2D ij(Float lowerI, Float upperI, Float lowerJ, Float upperJ);
	};

	/// 3维区域（浮点）
	struct Range3D
	{
		union { Float lowerX; Float lowerU; Float lowerJ; };
		union { Float upperX; Float upperU; Float upperJ; };
		union { Float lowerY; Float lowerV; Float lowerI; };
		union { Float upperY; Float upperV; Float upperI; };
		union { Float lowerZ; Float lowerW; Float lowerK; };
		union { Float upperZ; Float upperW; Float upperK; };

		SPADAS_API Range3D();
		SPADAS_API Range3D(Float lowerX, Float upperX, Float lowerY, Float upperY, Float lowerZ, Float upperZ);
		SPADAS_API Bool operator ==(Range3D range);
		SPADAS_API Bool operator !=(Range3D range);
		SPADAS_API String toString(); // (lowerX~upperX,lowerY~upperY,lowerZ~upperZ)
		SPADAS_API static Range3D uvw(Float lowerU, Float upperU, Float lowerV, Float upperV, Float lowerW, Float upperW);
		SPADAS_API static Range3D ijk(Float lowerI, Float upperI, Float lowerJ, Float upperJ, Float lowerK, Float upperK);
	};

	// 引用计数 //////////////////////////////////////////////////////////////

	/// 原子操作类
	class SPADAS_API Atom
	{
	public:
		/// 默认构造函数，初始值为0
		Atom();
		
		/// 基于指定初始值创建
		Atom(Int val);
		
		/// 加1并返回新值
		Int increase() const;
		
		/// 减1并返回新值
		Int decrease() const;
		
		/// 获取当前值
		Int get() const;

		/// 设定当前值
		void set(Int val) const;

		/// 加/减数并返回新值
		Int add(Int number) const;

		/// 若为旧值则设定为新值并返回TRUE，否则返回FALSE
		Bool cas(Int oldVal, Int newVal) const;
		
	private:
		Int val;
		Atom(const Atom& obj);
		Atom& operator =(const Atom& obj) { return *this; }
	};
	
	/// 变量数据的基类，使其具有引用计数属性
	class SPADAS_API Vars
	{
	public:
		/// 构造函数，创建后引用计数为1
		Vars();
		
		/// 析构函数函数，在引用计数降为0后自动调用
		virtual ~Vars();
		
		/// 引用计数增加1
		void retain();
		
		/// 引用计数减1
		void release();

		/// 取得当前引用计数
		UInt getRefCount();

		/// 加自旋锁
		void spinEnter();

		/// 解自旋锁
		void spinLeave();

		/// 获取类名称
		virtual String getTypeName();

		/// 按类ID判断是否为该类型
		virtual Bool isType(ULong typeID);

		/// 按类名称判断是否为该类型
		virtual Bool isType(String typeName);

		/// 获取字符串描述
		virtual String toString();

		/// 自旋锁是否已被管理，若是则 spadas::SpinLocked 将不加锁
		virtual Bool isSpinLockManaged();

		/// 取得存活对象数目
		static UInt getObjectCount();

	private:
		Atom nRefs;
		Atom varSpinLock;
		Vars(const Vars& obj);
		Vars& operator =(const Vars& obj) { return *this; }
	};
	
	/// 所有引用计数类的基类，其中VarsType为Vars的子类
	template <typename VarsType> class Object
	{
	public:
		/// 默认构造函数，变量数据指针为空
		Object();

		/// @brief 以指定变量数据指针初始化
		/// @param newVars 变量数据的指针
		/// @param isNew 对于new出来的指针isNew应为TRUE，否则为FALSE（由 Object::getVars 等获得的情况）
		Object(VarsType *newVars, Bool isNew);
		
		/// 析构函数，变量数据的引用减1
		~Object();
		
		/// 创建引用对象，直接指向另一个对象的变量数据（作为基类被隐式调用）
		Object(const Object<VarsType>& obj);
		
		/// 重定向，原变量数据引用减1并指向另一个对象的变量数据（作为基类被隐式调用）
		Object& operator =(const Object<VarsType>& obj);
		
		/// 是否相等
		Bool operator ==(const Object<VarsType>& obj);

		/// 是否不相等
		Bool operator !=(const Object<VarsType>& obj);

		/// 变量数据指针是否为空
		Bool isNull();

		/// 变量数据指针是否有效（非空）
		Bool isValid();
		
		/// 获取哈希值
		Word getHash();

		/// 作为非空静态对象时可获取程序运行周期间的唯一ID
		ULong getID() const;

		/// 获得变量数据的引用计数
		UInt getRefCount();

		/// 获得变量数据对应的类名称
		String getTypeName();

		/// 是否为该类的变量数据，若变量数据指针为空也返回TRUE
		template <typename TargetType>
		Bool is();

		/// 转换为基类或派生类对象，若变量数据指针为空或类型不符合则返回默认构造对象
		template <typename TargetType>
		TargetType as();

		/// 转换为基类或派生类对象，若变量数据指针为空或类型不符合则返回默认构造对象，并仅当类型不符合时输出FALSE
		template <typename TargetType>
		TargetType as(Bool& ok);

		/// [非安全] 获得变量数据的指针
		VarsType *getVars();

		/// @brief [非安全] 重定向，设置变量数据指针
		/// @param newVars 变量数据的指针
		/// @param isNew 对于new出来的指针isNew应为TRUE，否则为FALSE（由 Object::getVars 等获得的情况）
		void setVars(VarsType* newVars, Bool isNew);
		
		/// [非安全] 以输入的变量数据指针转换为当前类型创建对象
		static Object<VarsType> castCreate(Vars* varsToCast);

		/// 获取字符串描述
		String toString();

	protected:
		/// [非安全] 变量数据指针
		VarsType *vars;
	};

	/// 无需成员变量的引用计数基类
	class SPADAS_API EmptyObject : public Object<Vars>
	{
	public:
		/// 创建对象
		EmptyObject();

	private:
		Bool isNull() { return FALSE; }
		Bool isValid() { return FALSE; }
	};

	/// 可转换的基类对象
	class SPADAS_API BaseObject : public Object<Vars>
	{
	public:
		/// 类名称
		static const String TypeName;

		/// 无效对象
		BaseObject();
	};

	/// 实现Type类重定向操作的多线程安全
	template <typename Type> class Safe
	{
	public:
		/// 创建Type对象
		Safe();
		
		/// 等于号重定向
		void operator =(Type target);

		/// @brief [非安全] 重定向，设置变量数据指针
		/// @param newVars 变量数据的指针
		/// @param isNew 对于new出来的指针isNew应为TRUE，否则为FALSE（由 Object::getVars 等获得的情况）
		template <typename VarsType>
		void setVars(VarsType* newVars, Bool isNew);

		/// 获取对象
		Type get();

	private:
		Type obj;
		Atom objSpinLock;
		Safe(const Safe<Type>& obj);
		Safe<Type>& operator =(const Safe<Type>& obj) { return *this; }
	};

	/// 对Type对象进行多线程安全操作的容器
	template <typename Type> class SpinLocked
	{
	public:
		/// 构造函数，输入需要进行多线程安全操作的对象，自动加锁
		SpinLocked(Safe<Type>& safeObj);

		// 析构，自动解锁
		~SpinLocked();

		/// 使用容器中的对象
		Type *operator ->();

	private:
		Type obj;
		SpinLocked(const SpinLocked& obj);
		SpinLocked& operator =(const SpinLocked& obj) { return *this; }
	};

	/// spadas::Interface 模板类的变量数据
	template <typename Type> class InterfaceVars;

	/// 接口实现对象的容器，其中Type为接口定义类型
	template <typename Type> class Interface : public Object<InterfaceVars<Type> >
	{
	public:
		/// 无效对象
		Interface();
		
		/// 创建输入对象的容器，其中ImplementType必须为Type的实现（子类）
		template <typename ImplementType>
		Interface(ImplementType implementObj);
		
		/// 是否等于（与另一个容器拥有同一个对象，即指向同一个变量数据）
		Bool operator ==(Interface<Type> target);

		/// 是否不等于（与另一个容器拥有不同对象）
		Bool operator !=(Interface<Type> target);

		/// 获取哈希值
		Word getHash();

		/// 获取容器中的对象
		BaseObject getObject();

		/// 使用容器中对象的接口
		Type *operator ->();
	};
	
	/// spadas::Optional 模板类的变量数据
	template <typename Type> class OptionalVars;

	/// 可空对象，其中Type可为任意类型
	template <typename Type> class Optional : public Object<OptionalVars<Type> >
	{
	public:
		/// 无效对象
		Optional();
		
		/// 基于输入创建对象
		Optional(Type val);
		
		/// 取值
		Type value();

		/// 取值，当无效时返回输入的默认值
		Type value(Type defaultValue);

		/// 取得值的引用
		Type& refValue();

		/// 使用字段或方法
		Type* operator ->();
	};

	// 数据结构模板 //////////////////////////////////////////////////////////////

	/// 定长数组元素遍历器
	template <typename Type> class ArrayElem
	{
	public:
		/// 初始化函数
		ArrayElem(Array<Type> arr, UInt index);

		/// 是否在数组范围内
		Bool valid();

		/// 获取当前元素的序号
		UInt index();

		/// [非安全] 取得值的引用（需要先确保在数组范围内）
		Type& value();

		/// [非安全] 使用值的字段或方法（需要先确保在数组范围内）
		Type* operator ->();

		/// [非安全] 赋值给当前元素（需要先确保在数组范围内）
		void operator =(const Type& val);

		/// 移动至上个元素
		void operator --();

		/// 移动至下个元素
		void operator ++();

	private:
		Array<Type> arr;
		Type *data;
		UInt size;
		UInt idx;
	};

	/// spadas::Array 模板类的变量数据
	template <typename Type> class ArrayVars;

	/// 定长数组
	template <typename Type> class Array : public Object<ArrayVars<Type> >
	{
	public:
		/// 默认构造函数，创建一个空数组
		Array();

		/// @brief 创建一个具有指定大小的数组
		/// @param size 创建数组的大小
		Array(UInt size);
		
		/// @brief 创建一个具有指定大小的数组并设置所有元素的初始值
		/// @param size 创建数组的大小
		/// @param origin 数组中所有元素的初始值
		Array(UInt size, Type origin);

		/// @brief 从一个数组指针创建对象（创建时将拷贝源数据）
		/// @param arr 源数据数组指针
		/// @param size 源数据数组大小
		Array(const Type arr[], UInt size);

		/// [非安全] 取得数组的头指针
		Type *data();

		/// 取得数组大小
		UInt size();

		/// 是否为空数组
		Bool isEmpty();

		/// 克隆出一个新对象 (所有元素调用=号拷贝)
		Array<Type> clone();

		/// 取得数组中某个元素的引用
		Type& operator [](UInt index);

		/// 取得数组中第一个元素的引用
		Type& first();

		/// 取得数组中最后一个元素的引用
		Type& last();

		/// @brief 在本数组后方拼接另一个数组
		/// @param arr 将拼接的另一个数组
		/// @returns 拼接后的数组
		Array<Type> operator +(Array<Type> arr);

		/// 缩减数组大小
		void trim(UInt size);
		
		/// 对所有元素赋同一个值
		void set(Type value);

		/// 数组中是否包含某个值
		Bool contain(Type val);

		/// 数组中是否包含指定函数返回TRUE的某个值
		Bool containAs(Func<Bool(Type&)> func);

		/// 返回数组中所有等于某个值的元素序号
		Array<UInt> search(Type val);

		/// 返回数组中所有指定函数返回TRUE的元素序号
		Array<UInt> searchAs(Func<Bool(Type&)> func);

		/// 按从小到大排序，需要Type支持>重载符
		void sort();

		/// 根据指定函数(判断是否大于)，按从小到大排序
		void sortAs(Func<Bool(Type&, Type&)> func);

		/// 按指定函数转换为其他类型数组
		template <typename TargetType>
		Array<TargetType> convert(Func<TargetType(Type&)> func);
		
		/// @brief 根据指定的大小分割为多个数组
		/// @param sizes 将分割成的每个数组的大小
		/// @returns 分割后的多个数组
		Array<ArraySpan<Type> > split(Array<UInt> sizes);

		/// 转换为基类或派生类对象的数组（具体行为参考 spadas::Object::as ）
		template <typename TargetType>
		Array<TargetType> asArray();

		/// 转换为基类或派生类对象的数组，并输出每个元素是否转换成功的数组（具体行为参考 spadas::Object::as ）
		template <typename TargetType>
		Array<TargetType> asArray(Array<Bool>& ok);

		/// 取得子数组，其数据绑定至本数组的数据
		ArraySpan<Type> sub(UInt index, UInt size = UINF);

		/// 取得从第一个元素的遍历器，可修改元素
		ArrayElem<Type> firstElem();

		/// 取得从最后一个元素开始的遍历器，可修改元素
		ArrayElem<Type> lastElem();

		/// @brief 从另一个数组的某个子区域拷贝数据到本数组的某个位置
		/// @param src 源数据数组
		/// @param srcRegion 从源数组拷贝的区域
		/// @param thisOffset 拷贝至本数组的起始位置
		void copy(Array<Type> src, Region srcRegion, UInt thisOffset);

		/// 创建一个标量数组（只含有一个元素）
		static Array<Type> scalar(Type element);

		/// @brief [非安全] 基于可变参数列表创建数组并赋值，ArgType应为Int，UInt，Double，Char*或结构体和类名等
		/// @param size 数组大小
		/// @param firstValue 数组首个元素值，后面参数都为ArgType类型的元素值，总元素个数应与数组大小一致
		/// @returns 创建的数组
		template <typename ArgType>
		static Array<Type> create(UInt size, ArgType firstValue, ...);

		/// @brief [非安全] 创建一个具有指定大小但未初始化的数组，需要随后调用init方法确保对每个元素都初始化了仅一次
		/// @param size 创建数组的大小
		/// @returns 新创建的数组
		static Array<Type> createUninitialized(UInt size);

		/// [非安全] 初始化元素，需要确保序号在范围内
		void initialize(UInt index, const Type& val);
		
		/// @brief 合并多个数组
		/// @param arrs 将合并的多个数组
		/// @returns 合并后的数组
		static Array<Type> merge(Array<Array<Type> > arrs);

		/// @brief 合并多个数组片段
		/// @param spans 将合并的多个数组片段
		/// @returns 合并后的数组
		static Array<Type> merge(Array<ArraySpan<Type> > spans);

	private:
		Bool isNull() { return FALSE; }
		Bool isValid() { return FALSE; }
	};

	/// 定长数组片段，数据绑定至原定长数组的数据
	template <typename Type> class ArraySpan
	{
	public:
		/// 创建空片段
		ArraySpan();

		/// [非安全] 绑定至原定长数组
		ArraySpan(Array<Type>& sourceArray, UInt index, UInt size);

		/// [非安全] 取得数组片段的头指针
		Type *data();

		/// 取得数组片段大小
		UInt size();

		/// 是否为空数组片段
		Bool isEmpty();

		/// 克隆出一个新对象 (所有元素调用=号拷贝)
		Array<Type> clone();

		/// 取得数组片段中某个元素的引用
		Type& operator [](UInt index);

		/// 取得数组片段中第一个元素的引用
		Type& first();

		/// 取得数组片段中最后一个元素的引用
		Type& last();

		/// @brief 在本数组片段后方拼接另一个数组
		/// @param arr 将拼接的另一个数组
		/// @returns 拼接后的数组
		Array<Type> operator +(Array<Type> arr);

		/// 缩减数组片段大小
		void trim(UInt size);
		
		/// 对所有元素赋同一个值
		void set(Type value);
		
		/// 数组中是否包含某个值
		Bool contain(Type val);

		/// 数组中是否包含指定函数返回TRUE的某个值
		Bool containAs(Func<Bool(Type&)> func);

		/// 返回数组中所有等于某个值的元素序号
		Array<UInt> search(Type val);

		/// 返回数组中所有指定函数返回TRUE的元素序号
		Array<UInt> searchAs(Func<Bool(Type&)> func);

		/// 按从小到大排序，需要Type支持>重载符
		void sort();

		/// 根据指定函数(判断是否大于)，按从小到大排序
		void sortAs(Func<Bool(Type&, Type&)> func);

		/// 按指定函数转换为其他类型数组
		template <typename TargetType>
		Array<TargetType> convert(Func<TargetType(Type&)> func);
		
		/// @brief 根据指定的大小分割为多个数组
		/// @param sizes 将分割成的每个数组的大小
		/// @returns 分割后的多个数组
		Array<ArraySpan<Type> > split(Array<UInt> sizes);

		/// 转换为基类或派生类对象的数组（具体行为参考 spadas::Object::as ）
		template <typename TargetType>
		Array<TargetType> asArray();

		/// 转换为基类或派生类对象的数组，并输出每个元素是否转换成功的数组（具体行为参考 spadas::Object::as ）
		template <typename TargetType>
		Array<TargetType> asArray(Array<Bool>& ok);

		/// 取得子数组，其数据绑定至原数组的数据
		ArraySpan<Type> sub(UInt index, UInt size = UINF);

	private:
		Array<Type> source;
		UInt idx;
		UInt siz;
	};

	/// spadas::ListNode 模板类的变量数据
	template <typename Type> class ListNodeVars;

	/// 链表节点
	template <typename Type> class ListNode : public Object<ListNodeVars<Type> >
	{
	public:
		/// 创建一个节点，其数据为默认值
		ListNode();
		
		/// 创建一个节点，并给其数据赋指定值
		ListNode(Type val);

		/// 取得数据的引用
		Type& value();

		/// 使用数据的字段或方法
		Type* operator ->();
		
		/// 是否存在下一个节点
		Bool hasNext();

		/// 是否存在上一个节点
		Bool hasPrevious();

		/// 取得下一个节点 (若不存在返回本节点)
		ListNode<Type> next();
		
		/// 取得上一个节点 (若不存在返回本节点)
		ListNode<Type> previous();

		/// 赋值为下一个节点（若存在）
		void goNext();

		/// 赋值为上一个节点（若存在）
		void goPrevious();
		
		/// 切断与下一个节点的链接
		void cutNext();
		
		/// 切断与上一个节点的链接
		void cutPrevious();

		/// 连接按指定值创建的节点作为下一个节点，并返回创建的节点 (将自动调用this->cutNext)
		ListNode<Type> joinNext(Type val);

		/// 连接指定节点为下一个节点 (将自动调用this->cutNext和node.cutPrevious)
		void joinNext(ListNode<Type> node);
		
		/// 连接按指定值创建的节点作为上一个节点，并返回创建的节点 (将自动调用this->cutPrevious)
		ListNode<Type> joinPrevious(Type val);

		/// 连接指定节点为上一个节点 (将自动调用this->cutPrevious和node.cutNext)
		void joinPrevious(ListNode<Type> node);
		
		/// 在本节点和下一个节点间插入按指定值创建的节点，并返回该节点
		ListNode<Type> insertNext(Type val);

		/// 在本节点和下一个节点间插入指定节点 (将自动调用newNode.removeSelf)
		void insertNext(ListNode<Type> newNode);
		
		/// 在本节点和上一个节点间插入按指定值创建的节点，并返回该节点
		ListNode<Type> insertPrevious(Type val);

		/// 在本节点和上一个节点间插入指定节点 (将自动调用newNode.removeSelf)
		void insertPrevious(ListNode<Type> newNode);
		
		/// 移除下一个节点（将与下下个节点相连）
		void removeNext();
		
		/// 移除上一个节点（将与上上个节点相连）
		void removePrevious();
		
		/// 从链表中移除本节点（上一个节点将与下一个节点相连）
		void removeSelf();
		
		/// 拷贝整个链表 (将对链表中所有元素调用=号)
		ListNode<Type> cloneList();

		/// 拆解链表中所有链接
		void collapse();
		
	private:
		ListNode(ListNodeVars<Type> *vars);
		Bool isNull() { return FALSE; }
		Bool isValid() { return FALSE; }
	};

	/// spadas::TreeNode 模板类的变量数据
	template <typename Type> class TreeNodeVars;

	/// 树节点
	template <typename Type> class TreeNode : public Object<TreeNodeVars<Type> >
	{
	public:
		/// 创建一个节点，其数据为默认值
		TreeNode();
		
		/// 创建一个节点，并给其数据赋指定值
		TreeNode(Type val);

		/// 取得数据的引用
		Type& value();

		/// 使用数据的字段或方法
		Type* operator ->();

		/// 取得根方向节点 (若不存在返回本节点)
		TreeNode<Type> root();

		/// 取得所有子节点
		Array<TreeNode<Type> > leaves();

		/// 由序号取得某个子节点，序号从0开始 (若子节点较多且需要全部遍历，最好使用leaves()获取的子节点数组)
		TreeNode<Type> leafAt(UInt index);

		/// 取得子节点个数
		UInt nLeaves();
		
		/// 判断输入对象是否为本节点的子节点
		Bool isLeaf(TreeNode<Type> node);

		/// 在子节点列表末尾添加按指定值创建的节点，并返回该节点
		TreeNode<Type> addLeaf(Type val);

		/// 在子节点列表末尾添加一个子节点 (将自动调用leafNode.cutRoot)
		void addLeaf(TreeNode<Type> leafNode);

		/// 移除一个子节点
		void dropLeaf(TreeNode<Type> leafNode);
		
		/// 连接指定节点为根方向节点 (效果等同于rootNode.addLeaf)
		void joinRoot(TreeNode<Type> rootNode);
		
		/// 切断与根方向节点的链接
		void cutRoot();
		
		/// 取得全局根节点
		TreeNode<Type> globalRoot();

		/// 获得从本节点至全局根节点间所有节点的列表 (从本节点开始，其序号为0)
		Array<TreeNode<Type> > rootChain();
		
		/// 取得本节点在整棵树中的深度 (全局根节点深度为0)
		UInt depth();
		
		/// 拷贝以本节点为全局根节点的整棵树 (将对树中所有元素调用=号)
		TreeNode<Type> cloneTree();

		/// 拆解树中的所有链接
		void collapse();
		
	private:
		TreeNode(TreeNodeVars<Type> *vars);
		Bool isNull() { return FALSE; }
		Bool isValid() { return FALSE; }
		void collapseSub();
	};

	/// spadas::GraphNode 模板类的变量数据
	template <typename NType, typename LType> class GraphNodeVars;

	/// 图节点 (NType: 节点数据类型, LType: 链接数据类型)
	template <typename NType, typename LType> class GraphNode : public Object<GraphNodeVars<NType, LType> >
	{
	public:
		/// 创建一个节点，其数据为默认值
		GraphNode();
		
		/// 创建一个节点，并给其数据赋指定值
		GraphNode(NType val);
		
		/// 取得数据的引用
		NType& value();

		/// 使用数据的字段或方法
		NType* operator ->();

		/// 取得本节点上链接的数目
		UInt nLinks();
		
		/// 由序号取得某个链接上的数据引用 (序号从0开始)
		LType& linkAt(UInt index);
		
		/// 取得与指定节点间的链接上的数据引用，若无此链接则报错
		LType& linkWith(GraphNode<NType, LType> node);
		
		/// 由序号取得某个链接着的节点 (序号从0开始)
		GraphNode<NType, LType> nodeAt(UInt index);

		/// 由序号取得某个链接着的节点 (序号从0开始)，同时输出链接上的数据
		GraphNode<NType, LType> nodeAt(UInt index, LType& link);
		
		/// 取得与指定节点的链接的序号 (若无链接返回UINF)
		UInt indexOf(GraphNode<NType, LType> node);
		
		/// 添加一个与指定节点的链接，并设置链接在两个方向上的数据 (若已存在链接，将仅更新数据)
		void addLinkWith(GraphNode<NType, LType> dst, LType linkToDst, LType linkFromDst);
		
		/// 切断与指定节点的链接
		void cutLinkWith(GraphNode<NType, LType> node);
		
		/// 拆解图中的所有链接
		void collapse();
		
	private:
		GraphNode(GraphNodeVars<NType, LType> *vars0);
		Bool isNull() { return FALSE; }
		Bool isValid() { return FALSE; }
	};

	/// spadas::ArrayX 模板类的变量数据
	template <typename Type> class ArrayXVars;

	/// 可扩展数组
	template <typename Type> class ArrayX : public Object<ArrayXVars<Type> >
	{
	public:
		/// 创建一个空数组，每段大小为16
		ArrayX();
		
		/// 以指定段大小（应为2的幂数，范围在1~65536内）创建可扩展数组
		ArrayX(UInt segmentSize);

		/// 以指定段大小（应为2的幂数，范围在1~65536内）和默认值创建可扩展数组
		ArrayX(UInt segmentSize, Type defaultValue);
		
		/// 取得数组中某个元素的引用 (若越过数组边界，将先扩展大小至序号+1再访问)
		Type& operator [](UInt index);
		
		/// 是否为空数组
		Bool isEmpty();

		/// 取得大小
		UInt size();
		
		/// 扩展大小至指定值
		void setSize(UInt size);

		/// 在数组末尾扩展1个元素
		void append(Type val);

		/// 在数组末尾扩展多个元素
		void append(Array<Type> vals);
		
		/// 从定长数组对象的某个子区域拷贝至本数组的某个位置 (若越过了数组边界，则扩展大小至满足条件)
		void copyFromArray(Array<Type> src, Region srcRegion, UInt thisOffset);
		
		/// 转换为定长数组
		Array<Type> toArray();

		/// 转换为定长数组，可指定本数组的某个子区域作转换
		Array<Type> toArray(Region thisRegion);

	private:
		Bool isNull() { return FALSE; }
		Bool isValid() { return FALSE; }
		void copyRegion(Pointer, UInt, Array<Type>&, UInt, UInt, UInt, UInt);
		Pointer getSegmentNode(UInt);
	};

	/// spadas::List 模板类的变量数据
	template <typename Type> class ListVars;

	/// 链表元素遍历器
	template <typename Type> class List;
	template <typename Type> class ListElem
	{
	public:
		/// 构造函数，由 List::head 和 List::tail 调用生成
		ListElem(ListNode<Type> node, Bool valid, UInt index, ListNode<Type> prevNode, Bool prevValid, ListNode<Type> nextNode, Bool nextValid, List<Type> list);

		/// 当前元素是否在链表中
		Bool valid();

		/// 是否存在上一个元素
		Bool hasPrevious();

		/// 是否存在下一个元素
		Bool hasNext();

		/// 获取当前元素在链表中的序号（不在链表中则返回UINF）
		UInt index();

		/// 取得值的引用
		Type& value();

		/// 使用值的字段或方法
		Type* operator ->();

		/// 取得上一个元素值的引用
		Type& previous();

		/// 取得下一个元素值的引用
		Type& next();

		/// 赋值给当前元素
		void operator =(const Type& val);

		/// 移动至上个元素
		void operator --();

		/// 移动至下个元素
		void operator ++();

		/// 在当前元素前插入值
		void insertPrevious(Type val);

		/// 在当前元素后插入值
		void insertNext(Type val);

		/// 移除当前元素
		void remove();
	
	private:
		ListNode<Type> node;
		Bool vld;
		UInt idx;
		ListNode<Type> prevNode;
		Bool prevValid;
		UInt prevIndex;
		ListNode<Type> nextNode;
		Bool nextValid;
		UInt nextIndex;
		List<Type> list;
	};

	/// 链表
	template <typename Type> class List : public Object<ListVars<Type> >
	{
	public:
		/// 创建一个空链表
		List();

		/// 从定长数组创建链表
		List(Array<Type> arr);

		/// 复制
		List<Type> clone();

		/// 是否为空链表
		Bool isEmpty();

		/// 取得元素个数
		UInt size();

		/// 取得从首个元素开始的遍历器（使用该对象时，不可调用链表的add/remove/clear等方法，或再创建其他ListElem）
		ListElem<Type> head();

		/// 取得从末尾元素开始的遍历器（使用该对象时，不可调用链表的add/remove/clear等方法，或再创建其他ListElem）
		ListElem<Type> tail();

		/// 添加至首
		void addToHead(Type val);

		/// 添加至尾
		void addToTail(Type val);

		/// 移除首
		void removeHead();

		/// 移除尾
		void removeTail();

		/// 移除等于某个值的所有元素
		void remove(Type val);

		/// 清空链表
		void clear();

		/// 转换为定长数组
		Array<Type> toArray();

	private:
		Bool isNull() { return FALSE; }
		Bool isValid() { return FALSE; }
	};

	/// spadas::Stream 模板类的变量数据
	template <typename Type> class StreamVars;

	/// [多线程安全] 数据流，即队列或管道，元素具有先入先出特性
	template <typename Type> class Stream : public Object<StreamVars<Type> >
	{
	public:
		/// 创建容量为1的数据流，且其元素可丢弃
		Stream();
		
		/// 创建指定容量的数据流，并指定其元素是否可丢弃
		Stream(UInt capacity, Bool discardable = TRUE);

		/// 更新数据流的容量，并舍弃多余的元素
		void setCapacity(UInt capacity);

		/// 取得数据流的容量
		UInt capacity();
		
		/// 获得本数据流已推入的元素数目
		UInt nEnqueued();
		
		/// 获得本数据流已取出的元素数目
		UInt nDequeued();
		
		/// 获得本数据流已丢弃的元素数目
		UInt nDiscarded();
		
		/// 取得本数据流中缓存的元素数目，该值应为nEnqueued() - nDequeued() - nDiscarded()
		UInt nElements();

		/// 本数据流未缓存任何元素
		Bool isEmpty();

		/// 获取首个(最早推入)元素
		Type first();

		/// 获取最后一个(最迟推入)元素
		Type last();

		/// 推入新元素
		void enqueue(Type newElement);

		/// 推入新元素。若超出容量，1.元素可丢弃时，直接舍弃最早推入的元素再推入新元素；2. 元素不可丢弃时，等待有元素取出后再推入，或检测到interrupt后返回FALSE
		Bool enqueue(Type newElement, Flag interrupt); 
		
		/// 批量推入新元素
		void enqueue(Array<Type> newElements);

		/// 批量推入新元素，若推入过程中检测到interrupt则返回FALSE(此时可能已推入部分元素)
		Bool enqueue(Array<Type> newElements, Flag interrupt);

		/// 尝试取出指定数量的元素，实际取出数量以返回对象为准 (返回的数组中序号0的元素为最早)
		Array<Type> dequeue(UInt amount = UINF);

		/// 尝试取出小于指定值的连续元素 (返回的数组中序号0的元素为最早)
		template <typename TargetType>
		Array<Type> dequeueLessThan(TargetType target);

		/// 等待所有元素取出 (若检测到interrupt则返回FALSE)
		Bool waitAllDequeued(Flag interrupt, Bool spin);

		/// 终止数据流，使其变为“已终止状态”。终止后将无法推入任何元素，但可取出
		void terminate();

		/// 本数据流是否为“已终止状态”
		Bool isTerminated();

		/// 重置数据流：将清空所有缓存中的元素，移除“已终止状态”，并清除nEnqueued等统计数据。但不会改变数据流容量和“元素可丢弃属性”
		void reset();

	private:
		Bool isNull() { return FALSE; }
		Bool isValid() { return FALSE; }
	};

	// 映射模板 //////////////////////////////////////////////////////////////

	/// 键值对
	template <typename KeyType, typename ValueType> struct KeyValue
	{
		/// 键
		KeyType key;

		/// 值
		ValueType value;

		/// 默认构造函数
		KeyValue() {}

		/// 基于指定键值创建
		KeyValue(KeyType key0, ValueType value0) : key(key0), value(value0) {}

		/// 是否大于，需要KeyType支持>重载符
		inline Bool operator >(KeyValue<KeyType, ValueType> target)
		{ return key > target.key; }

		/// 是否小于，需要KeyType支持<重载符
		inline Bool operator <(KeyValue<KeyType, ValueType> target)
		{ return key < target.key; }
	};

	/// 数值键，将标准布局类型的值转为键
	template <typename Type> class NumericKey
	{
	public:
		/// 默认构造函数
		NumericKey();

		/// 基于指定值创建（若为非紧凑结构体，则初始化时应将所有字节置为0）
		NumericKey(Type value);

		/// 获取数值
		Type value();

		/// 是否等于
		Bool operator ==(NumericKey<Type> decimal);

		/// 是否不等于
		Bool operator !=(NumericKey<Type> decimal);

		/// 是否大于，需要Type支持>重载符
		Bool operator >(NumericKey<Type> decimal);

		/// 是否小于，需要Type支持<重载符
		Bool operator <(NumericKey<Type> decimal);

		/// 获取哈希值
		Word getHash();

	private:
		Type val;
	};

	/// spadas::Map 模板类的变量数据
	template <typename KeyType, typename ValueType> class MapVars;

	/// 映射表
	template <typename KeyType, typename ValueType> class Map : public Object<MapVars<KeyType, ValueType> >
	{
	public:
		/// 创建空的映射表，默认bucket个数为256
		Map();

		/// 创建指定bucket个数的映射表（应为2的幂数，范围在16~65536内）
		Map(UInt bucketCount);

		/// @brief [非安全] 基于可变参数列表创建映射表并赋值，KeyArgType和ValueArgType应为Int，UInt，Double，Char*或结构体和类名等
		/// @param size 待赋值的键值个数
		/// @param firstKey 首个键
		/// @param firstValue 首个值，后面参数交替为KeyArgType和ValueArgType类型，总个数应为待赋值的键值个数x2
		/// @returns 创建的映射表
		template <typename KeyArgType, typename ValueArgType>
		static Map<KeyType, ValueType> create(UInt size, KeyArgType firstKey, ValueArgType firstValue, ...);

		/// 清空数据
		void clear();

		/// 是否为空的映射表
		Bool isEmpty();

		/// 取得映射数目
		UInt size();

		/// 取得所有键
		Array<KeyType> keys();

		/// 取得排序后的所有键，需要KeyType支持>重载符
		Array<KeyType> keysSorted();

		/// 取得所有值
		Array<ValueType> values();

		/// 取得所有键值对
		Array<KeyValue<KeyType, ValueType> > keyValues();

		/// 取得排序后的所有键值对，需要KeyType支持>重载符
		Array<KeyValue<KeyType, ValueType> > keyValuesSorted();

		/// 同时取得所有键和所有值
		void pairs(Array<KeyType>& keys, Array<ValueType>& values);

		/// 同时取得排序后的所有键和所有值，需要KeyType支持>重载符
		void pairsSorted(Array<KeyType>& keys, Array<ValueType>& values);

		/// 是否包含指定键的映射
		Bool contain(KeyType key);

		/// 移除指定键的映射（若存在）
		void remove(KeyType key);

		/// 获取指定键对应的值，若不存在则返回默认值
		ValueType get(KeyType key, ValueType defaultValue);

		/// 尝试获得指定键对应的值，若不存在返回FALSE
		Bool tryGet(KeyType key, ValueType& value);

		/// 设置指定键对应的值，若不存在则新建映射
		void set(KeyType key, ValueType value);

		/// 获得指定键对应的值引用，若不存在则新建映射
		ValueType& operator [](KeyType key);

	private:
		Bool isNull() { return FALSE; }
		Bool isValid() { return FALSE; }
	};

	/// 字典，可通过字符串索引
	template <typename Type> class Dictionary : public Map<String, Type>
	{
	public:
		/// 创建字典，默认bucket个数为256
		Dictionary();

		/// 创建指定bucket个数的字典（应为2的幂数，范围在16~65536内）
		Dictionary(UInt bucketCount);

		/// @brief [非安全] 基于可变参数列表创建字典并赋值，ArgType应为Int，UInt，Double，Char*或结构体和类名等
		/// @param size 待赋值的键值个数
		/// @param firstKey 首个键
		/// @param firstValue 首个值，后面参数交替为Char*和ArgType类型，总个数应为待赋值的键值个数x2
		/// @returns 创建的字典
		template <typename ArgType>
		static Dictionary<Type> create(UInt size, const Char firstKey[], ArgType firstValue, ...);
	};

	/// 布尔键
	typedef NumericKey<Bool> BoolKey;

	/// 8位无符号整数键
	typedef NumericKey<Byte> ByteKey;

	/// 16位无符号整数键
	typedef NumericKey<Word> WordKey;

	/// 32位无符号整数键
	typedef NumericKey<UInt> UIntKey;

	/// 64位无符号整数键
	typedef NumericKey<ULong> ULongKey;

	/// 16位带符号整数键
	typedef NumericKey<Short> ShortKey;

	/// 32位带符号整数键
	typedef NumericKey<Int> IntKey;

	/// 64位带符号整数键
	typedef NumericKey<Long> LongKey;

	/// Char字符键
	typedef NumericKey<Char> CharKey;

	/// WChar字符键
	typedef NumericKey<WChar> WCharKey;

	/// 32位浮点数键
	typedef NumericKey<Float> FloatKey;

	/// 64位浮点数键
	typedef NumericKey<Double> DoubleKey;

	/// 字符串键值对
	typedef KeyValue<String, String> StringKeyValue;

	// 二进制数据类 //////////////////////////////////////////////////////////////

	/// 二进制数据类
	class SPADAS_API Binary : public Object<class BinaryVars>
	{
	public:
		/// 类名称
		static const String TypeName;

		/// 创建一个空数据块（长度为0）
		Binary();

		/// @brief 创建指定大小的数据块
		/// @param size 创建数据块的大小（字节单位）
		Binary(UInt size);
		
		/// @brief 创建指定大小的数据块，并初始化所有字节的值
		/// @param size 创建数据块的大小（字节单位）
		/// @param origin 数据块中所有字节的初始值
		Binary(UInt size, Byte origin);

		/// @brief 从一个 spadas::Byte 数组指针创建对象，需指定数据块大小（创建时将拷贝源数据）
		/// @param arr 源数据数组指针
		/// @param size 源数据数组大小（字节单位）
		Binary(const Byte *arr, UInt size);

		/// @brief [非安全] 基于可变参数列表创建二进制数据块并赋值
		/// @param size 数据块大小（字节单位）
		/// @param firstByte 首个字节值，后面参数都为范围在0～255的值，总字节个数应与数据块大小一致
		static Binary create(UInt size, UInt firstByte, ...);

		/// @brief 由Base64字符串创建二进制数据
		/// @param base64 输入的base64字符串
		/// @returns 解码后的数据块，若失败则返回无效对象
		static Optional<Binary> createFromBase64(String base64);

		/// @brief 由DES解密创建二进制数据 (encrypted长度应为8的倍数)
		/// @param encrypted DES加密数据块
		/// @param key DES密钥
		/// @returns 解码后的数据块，若失败则返回无效对象
		static Optional<Binary> createFromDES(Binary encrypted, String key);

		/// [非安全] 取得数据块的头指针
		Byte *data();

		/// 取得数据块大小
		UInt size();

		/// 数据是否为空
		Bool isEmpty();

		/// 克隆出一个新对象
		Binary clone();

		/// 取得某个字节的引用
		Byte& operator [](UInt index);

		/// @brief 在本数据块后拼接另一个数据块
		/// @param bin 将拼接的另一个数据块
		/// @returns 拼接后的数据块
		Binary operator +(Binary bin);

		/// 缩减数据块大小
		void trim(UInt size);

		/// 对所有字节赋同一个值
		void set(Byte val);

		/// @brief 根据指定的大小分割为多个数据块
		/// @param sizes 将分割成的每个数据块的大小（字节单位）
		/// @returns 分割后的多个数据块
		Array<BinarySpan> split(Array<UInt> sizes);
		
		/// 生成逆序数据块
		Binary reverse();

		/// 加密为Base64字符串
		String toBase64();

		/// SHA1加密
		Binary toSHA1();

		/// @brief DES加密(本数据块长度建议为8的倍数，若不满足则以0填满)
		/// @param key DES密钥
		/// @returns DES加密后的数据块
		Binary toDES(String key);

		/// 取得子数据块，其数据绑定至本数据块的数据
		BinarySpan sub(UInt index, UInt size = UINF);

		/// @brief 从另一个数据块的某个子区域拷贝数据到本数据块的某个位置
		/// @param src 源数据数组
		/// @param srcRegion 从源数组拷贝的区域
		/// @param dstOffset 拷贝至本数据块的起始位置
		void copy(Binary src, Region srcRegion, UInt dstOffset);

		/// @brief 合并多个数据块
		/// @param binaries 将合并的多个数据块
		/// @returns 合并后的数据块
		static Binary merge(Array<Binary> binaries);

	private:
		Bool isNull() { return FALSE; }
		Bool isValid() { return FALSE; }
	};

	/// 二进制数据片段，绑定至原二进制数据
	class SPADAS_API BinarySpan
	{
	public:
		/// 创建空片段
		BinarySpan();

		/// [非安全] 绑定至原二进制数据
		BinarySpan(Binary& sourceBinary, UInt index, UInt size);

		/// [非安全] 取得数据片段的头指针
		Byte *data();

		/// 取得数据片段大小
		UInt size();

		/// 数据片段是否为空
		Bool isEmpty();

		/// 克隆出一个新对象
		Binary clone();

		/// 取得某个字节的引用
		Byte& operator [](UInt index);

		/// @brief 在本数据片段后拼接另一个数据块
		/// @param bin 将拼接的另一个数据块
		/// @returns 拼接后的数据块
		Binary operator +(Binary bin);

		/// 缩减数据片段大小
		void trim(UInt size);

		/// 对所有字节赋同一个值
		void set(Byte val);

		/// @brief 根据指定的大小分割为多个数据片段
		/// @param sizes 将分割成的每个数据片段的大小（字节单位）
		/// @returns 分割后的多个数据片段
		Array<BinarySpan> split(Array<UInt> sizes);
		
		/// 生成逆序数据块
		Binary reverse();

		/// 加密为Base64字符串
		String toBase64();

		/// SHA1加密
		Binary toSHA1();

		/// @brief DES加密(本数据块长度建议为8的倍数，若不满足则以0填满)
		/// @param key DES密钥
		/// @returns DES加密后的数据块
		Binary toDES(String key);

		/// 取得子数据片段，其数据绑定至原数据块的数据
		BinarySpan sub(UInt index, UInt size = UINF);

	private:
		Binary source;
		UInt idx;
		UInt siz;
	};

	// 字符串 //////////////////////////////////////////////////////////////

	/// 字符串
	class SPADAS_API String : public Object<class StringVars>
	{
	public:
		/// 类名称
		static const String TypeName;

		/// 创建空字符串
		String();

		/// @brief 由 spadas::Char 单字符初始化
		/// @param character 输入字符
		String(Char character);

		/// @brief 由 spadas::WChar 单字符初始化
		/// @param character 输入字符
		String(WChar character);

		/// @brief 由 spadas::Char 字符数组指针初始化
		/// @param text 字符数组指针（以0结尾）
		String(Char text[]);

		/// @brief 由 spadas::Char 字符数组指针初始化
		/// @param text 字符数组指针（以0结尾）
		String(const Char text[]);

		/// @brief 由 spadas::WChar 字符数组指针初始化
		/// @param text 字符数组指针（以0结尾）
		String(WChar text[]);

		/// @brief 由 spadas::WChar 字符数组指针初始化
		/// @param text 字符数组指针（以0结尾）
		String(const WChar text[]);

		/// @brief 由 spadas::Char 字符数组初始化
		/// @param text 字符数组（不要求以0结尾）
		String(Array<Char> text);

		/// @brief 由 spadas::WChar 字符数组初始化
		/// @param text 字符数组（不要求以0结尾）
		String(Array<WChar> text);

		/// @brief 由 spadas::Bool 初始化，字符串为"TRUE"或"FALSE"
		/// @param val 输入布尔值
		String(Bool val);

		/// @brief 由 spadas::Byte 初始化
		/// @param val 输入8位无符号整数
		String(Byte val);

		/// @brief 由 spadas::Word 初始化
		/// @param val 输入16位无符号整数
		String(Word val);

		/// @brief 由 spadas::UInt 初始化
		/// @param val 输入32位无符号整数
		String(UInt val);

		/// @brief 由 spadas::UInt 初始化，可指定位数（不足的用0补齐）
		/// @param val 输入32位无符号整数
		/// @param nDigits 指定位数，范围为0~10
		String(UInt val, UInt nDigits);

		/// @brief 由 spadas::ULong 初始化
		/// @param val 输入64位无符号整数
		String(ULong val);

		/// @brief 由 spadas::ULong 初始化，可指定位数（不足的用0补齐）
		/// @param val 输入64位无符号整数
		/// @param nDigits 指定位数，范围为0~20
		String(ULong val, UInt nDigits);

		/// @brief 由 spadas::Short 初始化
		/// @param val 输入16位带符号整数
		String(Short val);

		/// @brief 由 spadas::Int 初始化
		/// @param val 输入32位带符号整数
		String(Int val);

		/// @brief 由 spadas::Long 初始化
		/// @param val 输入64位带符号整数
		String(Long val);

		/// @brief 由 spadas::Float 初始化
		/// @param val 输入32位浮点数
		String(Float val);

		/// @brief 由 spadas::Double 初始化
		/// @param val 输入64位浮点数
		String(Double val);

		/// @brief 由 spadas::Float 初始化，并指定保留小数位数
		/// @param val 输入32位浮点数
		/// @param nDigits 保留小数位数，范围为0~9
		String(Float val, UInt nDigits);

		/// @brief 由 spadas::Double 初始化，并指定保留小数位数
		/// @param val 输入64位浮点数
		/// @param nDigits 保留小数位数，范围为0~18
		String(Double val, UInt nDigits);

		/// @brief 从UTF-8二进制数据创建字符串对象
		/// @param binary UTF-8二进制数据（不要求以0结尾）
		String(Binary binary);

		/// @brief 由任意具有toString方法的结构体或对象创建字符串对象
		template <typename Type>
		String(Type obj);

		/// 是否等于
		Bool operator ==(String string);

		/// 是否不等于
		Bool operator !=(String string);

		/// 是否大于，按照系统默认字符串排序顺序
		Bool operator >(String string);

		/// 是否小于，按照系统默认字符串排序顺序
		Bool operator <(String string);

		/// [非安全] 取得字符串UTF-8数据的头指针 (只读且不以0结尾)
		const Byte *bytes();

		/// 取得字符串长度 (UTF-8字节数)
		UInt length();

		/// 是否为空字符串
		Bool isEmpty();

		/// 获取哈希值
		Word getHash();

		/// 克隆出一个新对象
		String clone();

		/// 转换为Char数组，以0结尾（因此有效长度为数组长度-1）
		Array<Char> chars();

		/// 转换为WChar数组，以0结尾（因此有效长度为数组长度-1）
		Array<WChar> wchars();

		/// @brief 在本字符串后拼接另一个字符串（不会更改本对象数据，且可多个字符串连加）
		/// @param string 将拼接的另一个字符串
		/// @returns 字符串拼接器
		StringAppender operator +(String string);

		/// 转换并返回 spadas::Int 数字
		Optional<Int> toInt();

		/// 转换并返回 spadas::Long 数字
		Optional<Long> toLong();

		/// 转换并返回 spadas::Float 数值
		Optional<Float> toFloat();

		/// 转换并返回 spadas::Double 数值
		Optional<Double> toDouble();

		/// 转换并输出 spadas::Int 数字，返回是否转换成功
		Bool toNumber(Int& number);

		/// 转换并输出 spadas::Long 数字，返回是否转换成功
		Bool toNumber(Long& number);

		/// 转换并输出 spadas::Float 数字，返回是否转换成功
		Bool toNumber(Float& number);

		/// 转换并输出 spadas::Double 数字，返回是否转换成功
		Bool toNumber(Double& number);
		
		/// 转换为UTF-8二进制数据块（不以0结尾）
		Binary toBinary();

		/// 转换为全大写字符串
		String toUpper();

		/// 转换为全小写字符串
		String toLower();

		/// 是否以指定字符串开头
		Bool startsWith(String target);

		/// 是否以指定字符串结尾
		Bool endsWith(String target);

		/// 搜索目标字符串，返回所有发现目标的首字符位置。如"bananana"搜"nana"，返回{2, 4}
		Array<UInt> search(String target);
		
		/// 用指定字符串对本字符串进行分割。如"12 34 56"按空格符分割，返回{"12", "34", "56"}。注意，本字符串不含target时，若本字符串为空则返回空数组，非空则返回标量数组
		Array<StringSpan> split(String target);

		/// 将本字符串中oldString部分替换为newString，并返回替换后的字符串
		String replace(String oldString, String newString);

		/// @brief 取得子字符串，其数据绑定至本字符串的数据
		/// @param index 子字符串在本字符串的起始位置
		/// @param length 字符串长度
		/// @param trimStart 是否裁剪掉开始处的空格
		/// @param trimEnd 是否裁剪掉结尾处的空格
		/// @return 子字符串
		StringSpan sub(UInt index, UInt length = UINF, Bool trimStart = FALSE, Bool trimEnd = FALSE);

		/// @brief 在本字符串后拼接另一个字符串(将更改本对象数据)
		/// @param string 拼接的另一个字符串
		void operator +=(String string);

		/// @brief 在本字符串后拼接另一个字符串片段(将更改本对象数据)
		/// @param string 拼接的另一个字符串片段
		void operator +=(StringSpan span);

		/// @brief 在本字符串后拼接 spadas::Char 单字符 (将更改本对象数据)
		/// @param character 拼接的字符
		void operator +=(Char character);

		/// @brief 在本字符串后拼接 spadas::WChar 单字符 (将更改本对象数据)
		/// @param character 拼接的字符
		void operator +=(WChar character);

		/// @brief 在本字符串后拼接 spadas::Char 字符数组指针 (将更改本对象数据)
		/// @param text 拼接的字符数组指针（以0结尾）
		void operator +=(Char text[]);

		/// @brief 在本字符串后拼接 spadas::Char 字符数组指针 (将更改本对象数据)
		/// @param text 拼接的字符数组指针（以0结尾）
		void operator +=(const Char text[]);

		/// @brief 在本字符串后拼接 spadas::WChar 字符数组指针 (将更改本对象数据)
		/// @param text 拼接的字符数组指针（以0结尾）
		void operator +=(WChar text[]);

		/// @brief 在本字符串后拼接 spadas::WChar 字符数组指针 (将更改本对象数据)
		/// @param text 拼接的字符数组指针（以0结尾）
		void operator +=(const WChar text[]);

		/// @brief 在本字符串后拼接 spadas::Char 字符数组 (将更改本对象数据)
		/// @param text 拼接的字符数组（不要求以0结尾）
		void operator +=(Array<Char> text);

		/// @brief 在本字符串后拼接 spadas::WChar 字符数组 (将更改本对象数据)
		/// @param text 拼接的字符数组（不要求以0结尾）
		void operator +=(Array<WChar> text);

		/// @brief 在本字符串后拼接 spadas::Bool (将更改本对象数据)
		/// @param val 拼接的布尔值
		void operator +=(Bool val);

		/// @brief 在本字符串后拼接 spadas::Byte (将更改本对象数据)
		/// @param val 拼接的8位无符号整数
		void operator +=(Byte val);

		/// @brief 在本字符串后拼接 spadas::Word (将更改本对象数据)
		/// @param val 拼接的16位无符号整数
		void operator +=(Word val);

		/// @brief 在本字符串后拼接 spadas::UInt (将更改本对象数据)
		/// @param val 拼接的32位无符号整数
		void operator +=(UInt val);

		/// @brief 在本字符串后拼接 spadas::ULong (将更改本对象数据)
		/// @param val 拼接的64位无符号整数
		void operator +=(ULong val);

		/// @brief 在本字符串后拼接 spadas::Short (将更改本对象数据)
		/// @param val 拼接的16位带符号整数
		void operator +=(Short val);

		/// @brief 在本字符串后拼接 spadas::Int (将更改本对象数据)
		/// @param val 拼接的32位带符号整数
		void operator +=(Int val);

		/// @brief 在本字符串后拼接 spadas::Long (将更改本对象数据)
		/// @param val 拼接的64位带符号整数
		void operator +=(Long val);

		/// @brief 在本字符串后拼接 spadas::Float (将更改本对象数据)
		/// @param val 拼接的32位浮点数
		void operator +=(Float val);

		/// @brief 在本字符串后拼接 spadas::Double (将更改本对象数据)
		/// @param val 拼接的64位浮点数
		void operator +=(Double val);

		/// @brief 在本字符串后拼接UTF-8二进制数据 (将更改本对象数据)
		/// @param binary 拼接的UTF-8二进制数据（不要求以0结尾）
		void operator +=(Binary binary);

		/// 创建数据块大小（字节单位）为指定值的空字符串
		static String createWithSize(UInt size);
		
		/// 用二进制表示8位无符号整型数字
		static String createBinString(Byte value);

		/// 用二进制表示16位无符号整型数字
		static String createBinString(Word value);

		/// 用二进制表示32位无符号整型数字
		static String createBinString(UInt value);

		/// 用二进制表示64位无符号整型数字
		static String createBinString(ULong value);
		
		/// 用十六进制表示数字表示8位无符号整型数字
		static String createHexString(Byte value);

		/// 用十六进制表示数字表示16位无符号整型数字
		static String createHexString(Word value);

		/// 用十六进制表示数字表示32位无符号整型数字
		static String createHexString(UInt value);

		/// 用十六进制表示数字表示64位无符号整型数字
		static String createHexString(ULong value);
		
		/// @brief 用十六进制表示一段二进制数据块（可指定每行打印的字节数）
		/// @param bin 输入二进制数据块
		/// @param nBytesPerRow 每行打印的字节数
		/// @returns 二进制数据块的表述字符串
		static String createHexString(Binary bin, UInt nBytesPerRow = 8);

		/// @brief 拼接多个字符串片段，片段间以指定分割符分割
		/// @param strs 输入的多个字符串片段
		/// @param separator 分隔符
		/// @returns 按指定分隔符拼接的字符串
		static String merge(Array<StringSpan> spans, String separator = ", ");

		/// @brief 以String构造函数拼接数组，字符串间以指定分割符分割
		/// @param strs 输入数组
		/// @param separator 分隔符
		/// @returns 按指定分隔符拼接的字符串
		template <typename Type>
		static String merge(Array<Type> arr, String separator = ", ");

	private:
		Bool isNull() { return FALSE; }
		Bool isValid() { return FALSE; }
		void initBuffer(UInt dataSize);
		void ensureBuffer(UInt appendSize);
		static String mergeStrings(Array<String> strs, String separator);
	};

	/// 字符串片段，数据绑定至原字符串的数据
	class SPADAS_API StringSpan
	{
	public:
		/// 创建空片段
		StringSpan();

		/// [非安全] 绑定至原字符串
		StringSpan(String& sourceString, UInt index, UInt length);

		/// 是否等于
		Bool operator ==(StringSpan span);

		/// 是否不等于
		Bool operator !=(StringSpan span);

		/// 是否等于
		Bool operator ==(String string);

		/// 是否不等于
		Bool operator !=(String string);

		/// 是否大于，按照系统默认字符串排序顺序
		Bool operator >(StringSpan span);

		/// 是否小于，按照系统默认字符串排序顺序
		Bool operator <(StringSpan span);

		/// [非安全] 取得字符串片段UTF-8数据的头指针 (只读且不以0结尾)
		const Byte *bytes();

		/// 取得字符串片段长度 (UTF-8字节数)
		UInt length();

		/// 是否为空字符串片段
		Bool isEmpty();

		/// 获取哈希值
		Word getHash();

		/// 克隆出一个新字符串对象
		String clone();

		/// 转换为Char数组，以0结尾（因此有效长度为数组长度-1）
		Array<Char> chars();

		/// 转换为WChar数组，以0结尾（因此有效长度为数组长度-1）
		Array<WChar> wchars();

		/// @brief 在本字符串片段后拼接另一个字符串（不会更改本对象数据，且可多个字符串连加）
		/// @param string 将拼接的另一个字符串
		/// @returns 字符串拼接器
		StringAppender operator +(String string);

		/// 转换并返回 spadas::Int 数字
		Optional<Int> toInt();

		/// 转换并返回 spadas::Long 数字
		Optional<Long> toLong();

		/// 转换并返回 spadas::Float 数值
		Optional<Float> toFloat();

		/// 转换并返回 spadas::Double 数值
		Optional<Double> toDouble();

		/// 转换并输出 spadas::Int 数字，返回是否转换成功
		Bool toNumber(Int& number);

		/// 转换并输出 spadas::Long 数字，返回是否转换成功
		Bool toNumber(Long& number);

		/// 转换并输出 spadas::Float 数字，返回是否转换成功
		Bool toNumber(Float& number);

		/// 转换并输出 spadas::Double 数字，返回是否转换成功
		Bool toNumber(Double& number);
		
		/// 转换为UTF-8二进制数据块（不以0结尾）
		Binary toBinary();

		/// 转换为全大写字符串
		String toUpper();

		/// 转换为全小写字符串
		String toLower();

		/// 是否以指定字符串开头
		Bool startsWith(String target);

		/// 是否以指定字符串结尾
		Bool endsWith(String target);

		/// 搜索目标字符串，返回所有发现目标的首字符位置。如"bananana"搜"nana"，返回{2, 4}
		Array<UInt> search(String target);
		
		/// 用指定字符串对本字符串片段进行分割。如"12 34 56"按空格符分割，返回{"12", "34", "56"}。注意，本字符串片段不含target时，若本字符串片段为空则返回空数组，非空则返回标量数组
		Array<StringSpan> split(String target);

		/// 将本字符串片段中oldString部分替换为newString，并返回替换后的字符串
		String replace(String oldString, String newString);

		/// @brief 取得子字符串，其数据绑定至本字符串片段的原字符串数据
		/// @param index 子字符串在本字符串片段的起始位置
		/// @param length 字符串长度
		/// @param trimStart 是否裁剪掉开始处的空格
		/// @param trimEnd 是否裁剪掉结尾处的空格
		/// @return 子字符串
		StringSpan sub(UInt index, UInt length = UINF, Bool trimStart = FALSE, Bool trimEnd = FALSE);

	private:
		String source;
		UInt idx;
		UInt len;
	};

	/// 字符串拼接器，用于加速+运算符
	class SPADAS_API StringAppender
	{
	public:
		/// 默认构造函数
		StringAppender(String origin);

		/// 拼接字符串
		template <typename Type>
		StringAppender operator +(Type target);

		/// 转字符串
		String toString();

	private:
		String str;
	};

	// 枚举对象 //////////////////////////////////////////////////////////////

	/// @brief [多线程安全] 枚举类型容器。若有高性能要求，应直接使用Type::Value
	/// @details 其中Type为枚举类，该类需要定义如下：\n
	/// 具有定义enum class Value，其中包含Default枚举值 \n
	/// 具有函数static const Char* toString(Value)，返回各枚举值的字符串，默认返回空指针0 \n
	template <typename Type> class Enum
	{
	public:
		/// 创建一个对象，使用默认值
		Enum();

		/// 创建一个对象，使用指定值（若该值无效则使用默认值）
		Enum(typename Type::Value val);

		/// 是否等于
		Bool operator ==(typename Type::Value val);

		/// 是否等于
		Bool operator ==(Enum<Type> enumeration);

		/// 是否不等于
		Bool operator !=(typename Type::Value val);

		/// 是否不等于
		Bool operator !=(Enum<Type> enumeration);

		/// 获取哈希值
		Word getHash();

		/// 取得枚举对象的值
		typename Type::Value value();

		/// 取得枚举对象的文本表述
		String toString();

	private:
		typename Type::Value val;
		const Char* str;
	};

	// 控制台 //////////////////////////////////////////////////////////////

	/// 按键枚举类型
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
		static const Char* toString(Value val)
		{
			switch (val)
			{
				ES(None);
				ES(Enter);
				ES(Space);
				ES(Back);
				ES(Esc);
				ES(Tab);
				ES(Shift);
				ES(Ctrl);
				ES(Insert);
				ES(Delete);
				ES(Home);
				ES(End);
				ES(PageUp);
				ES(PageDown);
				ES(Up);
				ES(Down);
				ES(Left);
				ES(Right);
				ES(Comma);
				ES(Period);
				ES(Key0);
				ES(Key1);
				ES(Key2);
				ES(Key3);
				ES(Key4);
				ES(Key5);
				ES(Key6);
				ES(Key7);
				ES(Key8);
				ES(Key9);
				ES(A);
				ES(B);
				ES(C);
				ES(D);
				ES(E);
				ES(F);
				ES(G);
				ES(H);
				ES(I);
				ES(J);
				ES(K);
				ES(L);
				ES(M);
				ES(N);
				ES(O);
				ES(P);
				ES(Q);
				ES(R);
				ES(S);
				ES(T);
				ES(U);
				ES(V);
				ES(W);
				ES(X);
				ES(Y);
				ES(Z);
				ES(F1);
				ES(F2);
				ES(F3);
				ES(F4);
				ES(F5);
				ES(F6);
				ES(F7);
				ES(F8);
				ES(Unknown);
				default: return 0;
			}
		}
	};

	/// 消息级别
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
		static const Char* toString(Value val)
		{
			switch (val)
			{
				ES(Debug);
				ES(Info);
				ES(Warning);
				ES(Error);
				default: return 0;
			}
		}
	};

	/// 控制台接口
	class SPADAS_API IConsole
	{
	public:
		virtual ~IConsole() {};

		/// 打印文本信息，并指定该信息的消息级别
		virtual void print(String text, Enum<MessageLevel> level);

		/// [可选] 是否支持扫描一行文本输入
		virtual Bool supportScan();

		/// [可选] 扫描一行文本输入
		virtual String scan();

		/// [可选] 是否支持扫描按键输入
		virtual Bool supportCheckKey();

		/// [可选] 扫描按键输入
		virtual Enum<Key> checkKey();
	};

	/// 记录调试信息接口
	class SPADAS_API ILogger
	{
	public:
		virtual ~ILogger() {}

		/// @brief 记录调试信息
		/// @param text 调试信息
		virtual void print(String text);
	};

	/// 控制台函数命名空间
	namespace console
	{
		/// 弹出小窗口显示文本信息（阻塞）
		SPADAS_API void popup(String text);
		
		/// 打印调试级别文本信息
		SPADAS_API void print(String text);

		/// 按指定消息级别打印文本信息
		SPADAS_API void print(String text, Enum<MessageLevel> level);

		/// 扫描一行文本输入 (优先级高于 spadas::console::waitKey )
		SPADAS_API String scan();
		
		/// 等待一个按键输入 (优先级高于 spadas::console::checkKey )
		SPADAS_API Enum<Key> waitKey();

		/// 是否有按键输入，若有则返回该按键，无则返回 Key::Value::None
		SPADAS_API Enum<Key> checkKey();

		/// 重定向控制台接口并返回旧接口，重定向后 spadas::console::print 、 spadas::console::scan 、 spadas::console::waitKey 、 spadas::console::checkKey 将使用该接口执行 (默认为无效接口，使用默认控制台)
		SPADAS_API Interface<IConsole> useConsole(Interface<IConsole> target);

		/// 重定向当前线程的记录调试信息接口并返回旧接口，重定向后调试级别的 spadas::console::print 将使用该接口打印 (默认为无效接口，使用IConsole接口打印)
		SPADAS_API Interface<ILogger> useThreadLogger(Interface<ILogger> target);
	}

	// 文件系统 //////////////////////////////////////////////////////////////

	/// 文件路径
	class SPADAS_API Path : public Object<class PathVars>
	{
	public:
		/// 类名称
		static const String TypeName;

		/// 无效对象
		Path();

		/// @brief 基于一个绝对路径或相对路径的字符串初始化
		/// @param pathString 绝对路径或相对路径字符串，相对路径将基于当前工作目录 \n
		/// - 若为文件夹，需显式的以路径分隔符"/"或"\"结尾 \n
		/// - 可加入"../"或"..\"表示上层文件夹
		Path(String pathString);

		/// 取得文件或文件夹名称（不包含后缀）
		String name();

		/// 取得文件或文件夹后缀 (.xxx)
		String extension();

		/// 取得文件或文件夹全名 (名称加后缀)
		String fullName();

		/// 取得文件或文件夹的绝对路径字符串 (文件夹以路径分隔符"/"或"\"结尾)
		String fullPath();

		/// 比较两个路径是否一致
		Bool operator ==(Path path);

		/// 比较两个路径是否不一致
		Bool operator !=(Path path);

		/// 获取哈希值
		Word getHash();

		/// 判断文件或文件夹是否存在
		Bool exist();

		/// 是否为文件路径
		Bool isFile();

		/// 是否为文件夹路径
		Bool isFolder();

		/// 若存在则移除文件或文件夹
		void remove();

		/// @brief (仅限文件) 移动文件至目标位置
		/// @param dstPath 目标文件路径
		void moveTo(Path dstPath);

		/// @brief (仅限文件夹) 移动文件夹至目标位置
		/// @param dstPath 目标文件夹路径
		/// @param mergeDst 是否与目标文件夹合并
		void moveTo(Path dstPath, Bool mergeDst);

		/// @brief (仅限文件) 拷贝文件至目标位置
		/// @param dstPath 目标文件路径
		void copyTo(Path dstPath);

		/// @brief (仅限文件夹) 拷贝文件夹至目标位置
		/// @param dstPath 目标文件夹路径
		/// @param mergeDst 是否与目标文件夹合并
		void copyTo(Path dstPath, Bool mergeDst);

		/// (仅限文件) 取得文件大小 (当文件不存在返回0)
		ULong fileSize();

		/// (仅限文件) 创建或重新创建一个指定大小的空文件，数据都为0
		void fileCreate(UInt size);

		/// (仅限文件夹) 若不存在则创建文件夹
		void folderMake();

		/// (仅限文件夹) 取得文件夹内所有文件和文件夹路径（若includeAllLevels为TRUE，则返回文件夹内所有层级的子文件和子文件夹。若内容较多则速度可能很慢）
		Array<Path> folderContents(Bool includeAllLevels = FALSE);

		/// (仅限文件夹) 生成子文件路径
		Path childFile(String childFullName);

		/// (仅限文件夹) 生成子文件夹路径（无需在末尾加路径分隔符）
		Path childFolder(String childFullName);

		/// (仅限文件夹) 输入当前文件夹内的相对路径，生成文件或文件夹路径 (文件夹以路径分隔符"/"或"\"结尾)
		Path childPath(String pathString);

		/// (仅限文件夹) 检查目标路径是否包含在当前文件夹内
		Bool contain(Path path);

		/// (仅限文件夹) 检查目标路径是否包含在当前文件夹内。若包含，则输出基于当前文件夹的相对路径至pathString
		Bool contain(Path path, String& pathString);

		/// 获得文件或文件夹的上层文件夹 (如果本路径为磁盘根目录，如c:\等，则返回无效路径)
		Path parentFolder();

		/// 获得当前操作系统下的路径分隔符
		static String separator();

		/// 获得工作目录
		static Path workPath();

		/// 获得可执行程序所在目录
		static Path executableFolderPath();

		/// 获得用户根目录
		static Path homePath();

		/// 获得SpadasFiles目录
		static Path spadasFilesPath();

		/// 设置工作目录 (注意应以路径分隔符"/"或"\"结尾)
		static void setWorkPath(String pathString);

		/// 设置可执行程序所在目录 (注意应以路径分隔符"/"或"\"结尾)
		static void setExecutableFolderPath(String pathString);
	};

	/// 文件数据I/O
	class SPADAS_API File : public Object<class FileVars>
	{
	public:
		/// 类名称
		static const String TypeName;

		/// 无效对象
		File();

		/// 以二进制输入模式打开文件 
		static File openBinary(Path filePath);

		/// 以文本输入模式打开文件（将检测UTF-8 BOM头）
		static File openText(Path filePath);

		/// 以二进制输出模式创建或重新创建文件
		static File createBinary(Path filePath);

		/// 以文本输出模式创建或重新创建文件（UTF-8编码，带BOM头，回车符\n）
		static File createText(Path filePath);

		/// 关闭文件I/O（在输出模式下，关闭前将缓冲区中所有数据写入磁盘）
		void close();

		/// 是否已关闭（无效对象也返回TRUE）
		Bool isClosed();

		/// 获得当前的I/O位置，字节单位 (若已关闭则返回ULINF)
		ULong position();

		/// 获得当前文件大小，输出模式下等于position (若已关闭则返回ULINF)
		ULong size();

		/// 获得当前文件的路径
		Path path();

		/// (文本输出模式) 打印一行文本至缓冲区
		void print(String text);

		/// (输出模式) 输出一块二进制数据至缓冲区
		void output(Binary data);

		/// (输出模式) 将缓冲区数据写入磁盘
		void flush();

		/// (文本输入模式) 从文件扫描一行文本
		String scan();

		/// (输入模式) 从文件输入指定大小的二进制数据，实际大小以输出为准
		Binary input(UInt size = UINF);

		/// (输入模式) 移动至指定的I/O位置，字节单位，并返回实际移动到哪个位置
		ULong seek(ULong pos);

		/// (输入模式) 是否已到文件结尾
		Bool endOfFile();
	};

	/// 打印至文件
	class SPADAS_API FileConsole : public Object<class FileConsoleVars>, public IConsole
	{
	public:
		/// 类名称
		static const String TypeName;

		/// 创建用于忽略所有打印的对象
		FileConsole();

		/// 根据路径创建对象，所有打印将写入至该文件
		FileConsole(Path filePath);

		/// 实现打印接口
		void print(String text, Enum<MessageLevel> level);

	private:
		Bool isNull() { return FALSE; }
		Bool isValid() { return FALSE; }
	};

	// XML //////////////////////////////////////////////////////////////

	/// XML标签的属性（格式为name="value"）
	struct XMLAttribute
	{
		/// 属性名
		String name;

		/// 属性值
		String value;

		SPADAS_API XMLAttribute();
		SPADAS_API XMLAttribute(String name, String value);
	};

	/// XML元素
	struct XMLElement
	{
		/// 元素标签
		String tag;

		/// 元素属性列表
		Array<XMLAttribute> attributes;

		/// 元素内容（若含有子节点则content将被忽略）
		String content;

		SPADAS_API XMLElement();
		SPADAS_API XMLElement(String tag);
		SPADAS_API XMLElement(String tag, Array<XMLAttribute> attributes, String content);
	};

	/// XML元素节点
	typedef TreeNode<XMLElement> XMLNode;

	/// XML文档对象
	class SPADAS_API XML : public Object<class XMLVars>
	{
	public:
		/// 类名称
		static const String TypeName;

		/// 创建一个空文档（根标签为root）
		XML();

		/// 通过一个XML元素节点创建文档（以输入节点为根节点克隆整个树结构）
		XML(XMLNode xmlRoot);

		/// 复制对象
		XML clone();

		/// 获得当前文档的根节点，从而可修改XML数据
		XMLNode globalRoot();

		/// 保存XML文档至.xml文件（UTF-8编码带BOM头）
		void save(Path xmlFilePath);

		/// 获得当前文档的二进制数据表示（UTF-8编码）
		Binary toBinary();

		/// 读取.xml文件，该文件应按UTF-8编码，且最好带BOM文件头
		static Optional<XML> createFromFile(Path xmlFilePath);

		/// 由UTF-8编码的二进制数据创建XML文档
		static Optional<XML> createFromBinary(Binary xmlBinary);

		/// 获得输入节点的所有具有指定标签名字的子节点
		static Array<XMLNode> nodeLeavesWithTagName(XMLNode node, String tagName);

		/// 获得输入节点的首个具有指定标签名字的子节点，若不存在返回FALSE
		static Bool firstNodeLeafWithTagName(XMLNode node, String tagName, XMLNode& output);

		/// XML属性数组转换为字符串型字典（可指定输出字典的容量，但以实际输出为准）
		static Dictionary<String> attributesToDictionary(Array<XMLAttribute> attributes);

		/// 字符串型字典转换为XML属性数组
		static Array<XMLAttribute> dictionaryToAttributes(Dictionary<String> dict);

	private:
		Bool isNull() { return FALSE; }
		Bool isValid() { return FALSE; }
	};

	// 图像 //////////////////////////////////////////////////////////////

	/// 图像数据指针
	class SPADAS_API ImagePointer
	{
	public:
		/// 默认构造函数，创建16x16灰度图像
		ImagePointer();

		/// 基于图像大小和是否为彩色图像进行初始化
		ImagePointer(Size2D size, Bool isColor);

		/// 基于已有数据初始化
		ImagePointer(Size2D size, Bool isColor, UInt rowBytes, Array<ULong> data);

		/// 图像宽度
		UInt getWidth();

		/// 图像高度
		UInt getHeight();

		/// 是否为彩色图像，彩色图像按BGR顺序存储数据
		Bool isColor();

		/// 每行图像的字节数，为8的倍数
		UInt getRowBytes();

		/// 图像数据，数据字节数为8的倍数
		Array<ULong> getData();

	private:
		UInt width, height, rowBytes;
		Bool color;
		Array<ULong> data;
	};

	/// 像素aspect ratio
	class SPADAS_API AspectRatio
	{
	public:
		enum class Value
		{
			Default = 1,
			_1_1 = 1,
			_4_3 = 2,
			_10_11 = 3,
			_12_11 = 4,
			_16_11 = 5,
			_40_33 = 6,
		};
		static const Char* toString(Value val)
		{
			switch (val)
			{
				ES(_1_1);
				ES(_4_3);
				ES(_10_11);
				ES(_12_11);
				ES(_16_11);
				ES(_40_33);
				default: return 0;
			}
		}
	};

	/// 帧率 
	class SPADAS_API FrameRate
	{
	public:
		enum class Value
		{
			Default = 0,
			Unknown = 0,
			_23_98 = 1, // 24/1001
			_24 = 2,
			_25 = 3,
			_29_97 = 4, // 30/1001
			_30 = 5,
			_50 = 6,
			_59_94 = 7, // 60/1001
			_60 = 8,
		};
		static const Char* toString(Value val)
		{
			switch (val)
			{
				ES(Unknown);
				ES(_23_98);
				ES(_24);
				ES(_25);
				ES(_29_97);
				ES(_30);
				ES(_50);
				ES(_59_94);
				ES(_60);
				default: return 0;
			}
		}
		static Float rate(Enum<FrameRate> frameRate); // [fps]
		static Float period(Enum<FrameRate> frameRate); // [毫秒]
	};

	/// 图像扫描方式
	class SPADAS_API InterlaceMode
	{
	public:
		enum class Value
		{
			Default = 1,

			/// 逐行扫描
			Progressive = 1,

			/// 隔行扫描，偶数行024...为较早扫描的场
			UpperFirst = 2,

			/// 隔行扫描，奇数行135...为较早扫描的场
			LowerFirst = 3,
		};
		static const Char* toString(Value val)
		{
			switch (val)
			{
				ES(Progressive);
				ES(UpperFirst);
				ES(LowerFirst);
				default: return 0;
			}
		}
		static Bool isInterlaced(Enum<InterlaceMode> mode);
	};

	/// 3D合并方式
	class SPADAS_API MergeMode
	{
	public:
		enum class Value
		{
			Default = 1,
			Normal = 1, // 非3D
			LeftEye = 2,
			RightEye = 3,
			HalfSideBySide = 4,
			FullSideBySide = 5,
			LineByLineLR = 6, // 偶数行024...为左机图像, 奇数行135...为右机图像
			LineByLineRL = 7, // 偶数行024...为右机图像, 奇数行135...为左机图像
		};
		static const Char* toString(Value val)
		{
			switch (val)
			{
				ES(Normal);
				ES(LeftEye);
				ES(RightEye);
				ES(HalfSideBySide);
				ES(FullSideBySide);
				ES(LineByLineLR);
				ES(LineByLineRL);
				default: return 0;
			}
		}
		static Bool is3DMerged(Enum<MergeMode> mode);
	};

	/// 时间编码
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

	/// 由RGB表述的颜色
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

	/// 图像标准分辨率
	class SPADAS_API ImageResolution
	{
	public:
		enum class Value
		{
			Default = 2,

			/// 320 x 240 (4:3)
			QVGA = 1,

			/// 640 x 480 (4:3)
			VGA = 2,

			/// 720 x 576 (5:4)
			PAL720 = 3,

			/// 768 x 576 (4:3)
			PAL768 = 4,

			/// 800 x 600 (4:3)
			SVGA = 5,

			/// 1024 x 768 (4:3)
			XGA = 6,

			/// 1280 x 800 (8:5)
			WXGA = 7,

			/// 1280 x 1024 (5:4)
			SXGA = 8,

			/// 1400 x 1050 (4:3)
			SXGAPlus = 9,

			/// 1600 x 1200 (4:3)
			UXGA = 10,

			/// 1680 x 1050 (8:5)
			WSXGAPlus = 11,

			/// 1280 x 720 (16:9)
			HD720 = 12,

			/// 1920 x 1080 (16:9)
			HD1080 = 13,
		};
		static const Char* toString(Value val)
		{
			switch (val)
			{
				ES(QVGA);
				ES(VGA);
				ES(PAL720);
				ES(PAL768);
				ES(SVGA);
				ES(XGA);
				ES(WXGA);
				ES(SXGA);
				ES(SXGAPlus);
				ES(UXGA);
				ES(WSXGAPlus);
				ES(HD720);
				ES(HD1080);
				default: return 0;
			}
		}
		static Size2D size(Enum<ImageResolution> resolution);
	};

	/// 像素格式
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
			ByteBool = 6, // 值: TRUE: 255, FALSE: 0
			ByteBGRA = 7,
			ByteRGBA = 8,
			WordBGR = 21, // 数值范围: 0~65535
			WordGray = 22, // 数值范围: 0~65535
			FloatBGR = 31, // 数值范围: 0.0~1.0
			FloatGray = 32, // 数值范围: 0.0~1.0
			FloatHSV = 33, // 数值范围: H(0.0~360.0) S(0.0~1.0) V(0.0~1.0)
		};
		static const Char* toString(Value val)
		{
			switch (val)
			{
				ES(ByteBGR);
				ES(ByteRGB);
				ES(ByteGray);
				ES(ByteUYVY);
				ES(ByteYUV);
				ES(ByteBool);
				ES(ByteBGRA);
				ES(ByteRGBA);
				ES(WordBGR);
				ES(WordGray);
				ES(FloatBGR);
				ES(FloatGray);
				ES(FloatHSV);
				default: return 0;
			}
		}
		static Bool isColor(Enum<PixelFormat> format);
		static Bool hasAlpha(Enum<PixelFormat> format);
		static UInt nChannels(Enum<PixelFormat> format);
		static UInt bytesPerPixel(Enum<PixelFormat> format);
	};

	/// 图像大小调整时的采样方式
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
		static const Char* toString(Value val)
		{
			switch (val)
			{
				ES(Nearest);
				ES(Bilinear);
				ES(Area);
				default: return 0;
			}
		}
	};

	/// 图像大小的调整倍数
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
		static const Char* toString(Value val)
		{
			switch (val)
			{
				ES(None);
				ES(Quarter);
				ES(Half);
				ES(Double);
				default: return 0;
			}
		}
	};

	/// 图像旋转的类型
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
		static const Char* toString(Value val)
		{
			switch (val)
			{
				ES(None);
				ES(CW90);
				ES(CW180);
				ES(CCW90);
				default: return 0;
			}
		}
	};

	/// 像素各通道值
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

	/// Byte型的像素各通道值
	typedef PixelChannels<Byte> BytePixel;

	/// Word型的像素各通道值
	typedef PixelChannels<Word> WordPixel;

	/// Float型的像素各通道值
	typedef PixelChannels<Float> FloatPixel;

	/// 像素数据，方便进行图像像素操作
	struct PixelData
	{
		/// [非安全] 指针联合，可根据类型使用
		union { Pointer ptr; Byte *b; Word *w; Float *f; };

		/// 每像素字节数
		UInt bytesPerPixel;

		/// 无效指针
		SPADAS_API PixelData();

		/// 正常创建
		SPADAS_API PixelData(Pointer ptr, UInt bytesPerPixel);

		/// 是否有效
		SPADAS_API Bool isValid();

		/// 获取指定序号上的像素数据
		SPADAS_API PixelData operator [](UInt index);

		/// 获取Byte型的各通道值
		SPADAS_API BytePixel getBytes();

		/// 获取Word型的各通道值
		SPADAS_API WordPixel getWords();

		/// 获取Float型的各通道值
		SPADAS_API FloatPixel getFloats();

		/// 设置Byte型的各通道值
		SPADAS_API void set(BytePixel bytes);

		/// 设置Word型的各通道值
		SPADAS_API void set(WordPixel words);

		/// 设置Float型的各通道值
		SPADAS_API void set(FloatPixel floats);
	};

	/// 图像类
	class SPADAS_API Image : public Object<class ImageVars>
	{
	public:
		/// 类名称
		static const String TypeName;

		/// 无效对象
		Image();

		/// 指定大小和格式创建图像，若setPixels为TRUE则所有像素赋值为0
		Image(Size2D size, Enum<PixelFormat> format, Bool setPixels = TRUE);

		/// 指定大小和格式创建图像，若setPixels为TRUE则所有像素赋值为0
		Image(Enum<ImageResolution> resolution, Enum<PixelFormat> format, Bool setPixels = TRUE);

		/// 指定大小和格式创建图像，并从指针指向的数据块拷贝数据
		Image(Size2D size, Enum<PixelFormat> format, Pointer raw, UInt rawRowBytes);

		/// 指定大小和格式创建图像，并从指针指向的数据块拷贝数据
		Image(Enum<ImageResolution> resolution, Enum<PixelFormat> format, Pointer raw, UInt rawRowBytes);

		/// 从图像指针创建图像（不拷贝数据）
		Image(ImagePointer pointer);

		/// 从.bmp文件读取图像，根据文件内容不同其格式可能为ByteBGR，ByteBGRA或ByteGray
		Image(Path bmpFilePath);

		/// 克隆出一个新对象
		Image clone();

		/// 从另一幅具有相同像素格式的图像的某个子区域拷贝数据到本图像的某个位置（通过thisOffset指定）
		void copy(Image src, Region2D srcRegion, CoordInt2D thisOffset);

		/// 绘制图层至本图像的某个位置 (layer图像必须具有Alpha通道)
		void drawLayer(Image layer, CoordInt2D dstOffset);

		/// 取得子图像，其数据绑定至本图像的数据
		Image sub(Region2D region);

		/// 取得图像指针，仅ByteGray和ByteBGR支持
		Optional<ImagePointer> imagePointer();

		/// 取得图像大小
		Size2D size();

		/// 取得图像宽度
		UInt width();

		/// 取得图像高度
		UInt height();

		/// 取得图像像素格式
		Enum<PixelFormat> format();

		/// 取得图像一行的字节数/步长
		UInt rowBytes();

		/// 取得一行中有效的字节数（宽度x通道数）
		UInt rowBytesValid();

		/// 取得图像第一个像素数据
		PixelData data();

		/// 取得某一行第一个像素数据，v=0为第一行
		PixelData operator [](UInt v);

		/// 按照指定倍数调整图像大小 (较快)
		Image resize(Enum<ResizeScale> scale);

		/// 调整图像大小至任意尺寸 (较慢)
		Image resize(Size2D outputSize, Bool undistort, Enum<ResizeMode> mode = ResizeMode::Value::Bilinear);

		/// 调整图像大小至任意尺寸 (较慢)
		Image resize(Enum<ImageResolution> outputResolution, Bool undistort, Enum<ResizeMode> mode = ResizeMode::Value::Bilinear);

		/// 转换像素格式 (转换为 PixelFormat::Value::ByteBool 时, 128~255 -> 255, 0~127 -> 0
		Image convert(Enum<PixelFormat> outputFormat);

		/// 反转图像
		Image flip(Bool upDownFlip, Bool leftRightFlip);

		/// 旋转图像 (支持除ByteUYVY以外的所有像素格式)
		Image rotate(Enum<ImageRotation> rotation);

		/// 图像场分割 (image1: 偶数行024..., image2: 奇数行135...)
		ImagePair splitFields();

		/// 合并两场为一幅图像 (image1: 偶数行024..., image2: 奇数行135...)
		static Image mergeFields(ImagePair fields);

		/// 分割3D图像至图像对 (image1: 左机图像, image2: 右机图像)
		ImagePair splitStereo(Enum<MergeMode> srcMergeMode);

		/// 合并为3D图像 (image1: 左机图像, image2: 右机图像)
		static Image mergeStereo(ImagePair src, Enum<MergeMode> dstMergeMode);

		/*
			对于splitChannels和mergeChannels方法，像素格式将按以下方式转化：
			多通道图像		单通道图像
			----------------------------------
			ByteBGR			ByteGray x 3
			ByteRGB			ByteGray x 3
			ByteYUV			ByteGray x 3
			ByteBGRA		ByteGray x 4
			ByteRGBA		ByteGray x 4
			WordBGR			WordGray x 3
			FloatBGR		FloatGray x 3
			FloatHSV		FloatGray x 3 (色调数值将归一化至0.0~1.0范围)
		*/

		/// 分离多通道图像至若干幅单通道图像
		Array<Image> splitChannels();

		/// 按指定像素格式合并为多通道图像
		static Image mergeChannels(Array<Image> channels, Enum<PixelFormat> multiChannelFormat);

		/// 生成mipmap (级别0为源图像，级别越高图像越小)
		Array<Image> genMipmap(UInt nLevels);

		/// 保存至.bmp文件
		void save(Path bmpFilePath);

		/// 用文本表述图像大小、像素格式等信息
		String getInfoText();
	};

	/// 图像对
	struct ImagePair
	{
		Image image1;
		Image image2;

		SPADAS_API ImagePair();
		SPADAS_API ImagePair(Image image1, Image image2);
	};

	// 数学 //////////////////////////////////////////////////////////////

	/// 大数
	class SPADAS_API BigInteger : public Object<class BigIntegerVars>
	{
	public:
		/// 类名称
		static const String TypeName;

		/// 零
		BigInteger();

		/// 由无符号二进制数据（若空则为0）以及符号（非0时有效）生成
		BigInteger(Binary integerData, Bool sign = TRUE);

		/// 由10进制数字字符串生成（若空则为0）
		static Optional<BigInteger> createFromString(String decimalString);

		/// 是否为正（0则返回TRUE）
		Bool isPositive();

		/// 取得无符号二进制数据
		Binary getData();

		/// 获取哈希值
		Word getHash();

		/// 是否相等
		Bool operator ==(BigInteger num);

		/// 是否不等
		Bool operator !=(BigInteger num);

		/// 是否小于
		Bool operator <(BigInteger num);

		/// 是否大于
		Bool operator >(BigInteger num);

		/// 加
		BigInteger operator +(BigInteger num);

		/// 减
		BigInteger operator -(BigInteger num);

		/// 乘
		BigInteger operator *(BigInteger num);

		/// 除
		BigInteger operator /(BigInteger num);

		/// 模
		BigInteger operator %(BigInteger num);

		/// 乘幂后模运算：return = (this ^ exponent) % modulus
		BigInteger modPow(BigInteger exponent, BigInteger modulus);

	private:
		Bool isNull() { return FALSE; }
		Bool isValid() { return FALSE; }
	};

	/// spadas::Matrix 模板类的变量数据
	template <typename Type> class MatrixVars;

	/// Int型矩阵
	template <typename Type> class Matrix : public Object<class MatrixVars<Type> >
	{
	public:
		/// 无效对象
		Matrix();

		/// 创建一个2维矩阵
		Matrix(UInt dim0, UInt dim1);

		/// 创建一个3维矩阵
		Matrix(UInt dim0, UInt dim1, UInt dim2);

		/// 创建一个N维矩阵 (N至少为1)
		Matrix(Array<UInt> dims);

		/// 创建一个N维矩阵并从指定数据源拷贝
		Matrix(Array<UInt> dims, Pointer raw);

		/// 由数组创建矩阵 (矩阵大小为N x 1 x 1...，维数由参数指定)
		Matrix(Array<Type> arr, UInt nDims = 2);

		/// 从文件读取矩阵数据并创建（由Matrix::save方法创建的文件）
		Matrix(Path filePath);

		/// 克隆出一个新对象
		Matrix<Type> clone();

		/// 矩阵数据类型转换
		template <typename TargetType>
		void convertTo(Matrix<TargetType>& target);

		/// 从另一个矩阵的某个子区域拷贝数据到本矩阵的某个位置（通过thisOffset指定），源矩阵和本矩阵都应为2维矩阵
		void copy(Matrix<Type> src, Region2D srcRegion, CoordInt2D thisOffset);

		/// 从另一个矩阵的某个子区域拷贝数据到本矩阵的某个位置（通过thisOffset指定），源矩阵和本矩阵都应为3维矩阵
		void copy(Matrix<Type> src, Region3D srcRegion, CoordInt3D thisOffset);

		/// 对所有元素赋同一个值
		void set(Type value);

		/// 获得矩阵的大小（各维的长度）
		Array<UInt> size();

		/// 获得矩阵某个维度的长度 (维度无效时返回0)，序号从0开始
		UInt dimAt(UInt index);

		/// 获得矩阵维数
		UInt nDims();

		/// 获得矩阵的总元素个数
		UInt nElems();

		/// 判断矩阵是否为指定大小
		Bool isSize(Size2D size);

		/// 判断矩阵是否为指定大小
		Bool isSize(Size3D size);

		/// 判断矩阵是否为指定大小
		Bool isSize(Array<UInt> dims);

		/// [非安全] 取得数据指针
		Type *data();

		/// 取得第i个子矩阵，该子矩阵维数减1，数据可绑定至本矩阵数据，序号从0开始
		Matrix<Type> operator [](UInt i);

		/// 通过圆括号获得某个元素的引用，面向1维矩阵 (标量)
		Type& operator ()(UInt i);

		/// 通过圆括号获得某个元素的引用，面向2维矩阵 (标量)
		Type& operator ()(UInt i, UInt j);

		/// 通过圆括号获得某个元素的引用，面向3维矩阵 (标量)
		Type& operator ()(UInt i, UInt j, UInt k);

		/// 矩阵加法: output = this + matrix
		Matrix<Type> operator +(Matrix<Type> matrix);

		/// 矩阵减法: output = this - matrix
		Matrix<Type> operator -(Matrix<Type> matrix);

		/// 矩阵乘以标量: output = this * scale
		Matrix<Type> operator *(Type scale);

		/// 矩阵相乘: output = this * matrix (仅支持2维矩阵)
		Matrix<Type> operator *(Matrix<Type> matrix);

		/// 矩阵转置 (仅支持2维矩阵)
		Matrix<Type> transpose();

		/// 用字符串表示矩阵数据 (仅支持2维矩阵)
		String toString();

		/// 保存矩阵数据至文本文件 (仅支持2维矩阵)
		void save(Path filePath);
	};

	/// 支持的矩阵类型: Int型矩阵
	typedef Matrix<Int> IntMat;

	/// 支持的矩阵类型: Float型矩阵
	typedef Matrix<Float> FloatMat;

	/// 支持的矩阵类型: Double型矩阵
	typedef Matrix<Double> DoubleMat;

	/// 数学函数命名空间
	namespace math
	{
		/// 圆周率
		const Float PI = 3.141593f;

		/// 自然常数
		const Float E = 2.718282f;

		/// 角度数值转为弧度数值 (乘以以下常数进行转换)
		const Float DEG2RAD = 0.01745329f;

		/// 弧度数值转为角度数值 (乘以以下常数进行转换)
		const Float RAD2DEG = 57.29578f;

		/// 浮点运算精度
		const Float FEPS = 1.4e-45f;

		/// 浮点运算精度
		const Double DEPS = 4.94065645841247e-324;

		/// 获得两个数中的最小值
		template <typename Type>
		Type min(Type a, Type b);

		/// 获得数组中的最小值
		template <typename Type>
		Type min(Array<Type> arr);

		/// 获得两个数中的最大值
		template <typename Type>
		Type max(Type a, Type b);

		/// 获得数组中的最大值
		template <typename Type>
		Type max(Array<Type> arr);

		/// 保证number在lower和upper之间（大于upper则为upper，小于lower则为lower）
		template <typename Type>
		Type clamp(Type number, Type lower, Type upper);

		/// 获取绝对值
		inline Int abs(Int number) { return number > 0 ? number : -number; }

		/// 获取绝对值
		inline Long abs(Long number) { return number > 0 ? number : -number; }

		/// 获取绝对值
		inline Float abs(Float number) { return number > 0 ? number : -number; }

		/// 获取绝对值
		inline Double abs(Double number) { return number > 0 ? number : -number; }

		/// 四舍五入
		inline Long round(Float number) { return (Long)(number + (number >= 0 ? 0.5f : -0.5f)); }

		/// 四舍五入
		inline Long round(Double number) { return (Long)(number + (number >= 0 ? 0.5 : -0.5)); }

		/// 取得不大于输入值的最大整数
		inline Long floor(Float number) { Long i = (Long)number; return i - (i > number); }

		/// 取得不大于输入值的最大整数
		inline Long floor(Double number) { Long i = (Long)number; return i - (i > number); }

		/// 取得不小于输入值的最小整数
		inline Long ceil(Float number) { Long i = (Long)number; return i + (i < number); }

		/// 取得不小于输入值的最小整数
		inline Long ceil(Double number) { Long i = (Long)number; return i + (i < number); }

		/// 计算a的x次幂 (返回无穷大若a < 0)
		SPADAS_API Float power(Float a, Float x);

		/// 计算a的x次幂 (返回无穷大若a < 0)
		SPADAS_API Double power(Double a, Double x);

		/// 平方
		inline Float square(Float number) { return number * number; }

		/// 平方
		inline Double square(Double number) { return number * number; }

		/// 平方根 (返回无穷大若number < 0)
		inline Float root(Float number) { return power(number, 0.5f); }

		/// 平方根 (返回无穷大若number < 0)
		inline Double root(Double number) { return power(number, 0.5); }

		/// 获得浮点的极限值
		SPADAS_API Float finf();

		/// 获得浮点的极限值
		SPADAS_API Float nfinf();

		/// 获得浮点的极限值
		SPADAS_API Double dinf();

		/// 获得浮点的极限值
		SPADAS_API Double ndinf();

		/// 计算自然指数
		SPADAS_API Float exp(Float x);

		/// 计算自然指数
		SPADAS_API Double exp(Double x);

		/// 计算自然对数 (返回FINF或DINF若x <= 0)
		SPADAS_API Float ln(Float x);

		/// 计算自然对数 (返回FINF或DINF若x <= 0)
		SPADAS_API Double ln(Double x);

		/// 三角函数: sine
		SPADAS_API Float sin(Float degree);

		/// 三角函数: sine
		SPADAS_API Double sin(Double degree);

		/// 三角函数: cosine
		SPADAS_API Float cos(Float degree);

		/// 三角函数: cosine
		SPADAS_API Double cos(Double degree);

		/// 三角函数: tangent
		SPADAS_API Float tan(Float degree);

		/// 三角函数: tangent
		SPADAS_API Double tan(Double degree);

		/// 三角函数: arc-sine(输出-90~90)
		SPADAS_API Float asin(Float x);

		/// 三角函数: arc-sine(输出-90~90)
		SPADAS_API Double asin(Double x);

		/// 三角函数: arc-cosine(输出0~180)
		SPADAS_API Float acos(Float x);

		/// 三角函数: arc-cosine(输出0~180)
		SPADAS_API Double acos(Double x);

		/// 三角函数: arc-tangent(x/y) (输出FINF或DINF若输入参数无效)
		SPADAS_API Float atan(Float x, Float y = 1.0f);

		/// 三角函数: arc-tangent(x/y) (输出FINF或DINF若输入参数无效)
		SPADAS_API Double atan(Double x, Double y = 1.0);

		/// 获取整型数的深度 (0~1的深度为0, 2~3的深度为1, 4~7的深度为2, 8~15的深度为3, ...)
		SPADAS_API UInt depth(UInt number);

		/// 生成介于0.0至1.0的随机数，数量由参数指定
		SPADAS_API Array<Float> random(UInt size);

		/// 生成介于0.0至1.0的随机数，数量由参数指定
		SPADAS_API Array<Double> randomD(UInt size);

		/// SVD分解 (src=u*s*v.transpose())
		SPADAS_API void decomposeSVD(FloatMat src, FloatMat& u, FloatMat& s, FloatMat& v);

		/// SVD分解 (src=u*s*v.transpose())
		SPADAS_API void decomposeSVD(DoubleMat src, DoubleMat& u, DoubleMat& s, DoubleMat& v);

		/// 计算逆矩阵 (src*inv=I) 或伪逆矩阵 (src*inv*src=src)
		SPADAS_API FloatMat inverse(FloatMat src);

		/// 计算逆矩阵 (src*inv=I) 或伪逆矩阵 (src*inv*src=src)
		SPADAS_API DoubleMat inverse(DoubleMat src);

		/// 最小二乘法解方程组
		SPADAS_API Array<Float> solve(FloatMat a, Array<Float> b);

		/// 最小二乘法解方程组
		SPADAS_API Array<Double> solve(DoubleMat a, Array<Double> b);
	}

	// 多线程 //////////////////////////////////////////////////////////////

	/// [多线程安全] 线程锁
	class SPADAS_API Lock : public Object<class LockVars>
	{
	public:
		/// 类名称
		static const String TypeName;

		/// 创建非自旋锁对象 (默认为未进入)
		Lock();

		/// 创建自旋锁或非自旋锁对象 (默认为未进入)
		Lock(Bool spin);

		/// 是否为自旋锁
		Bool isSpin();

		/// 进入/加锁
		void enter();

		/// 离开/解锁
		void leave();

	private:
		Bool isNull() { return FALSE; }
		Bool isValid() { return FALSE; }
	};

	/// 线程锁代理，可方便线程锁的使用，杜绝未调用Lock::leave导致线程锁死的现象
	class SPADAS_API LockProxy
	{
	public:
		/// 绑定线程锁 (将自动调用lock.enter)
		LockProxy(Lock& lock);

		/// 析构函数 (将自动调用lock.leave)
		~LockProxy();

		/// 提前调用lock.leave
		void releaseLock();

	private:
		Lock lock;
		Bool released;
		LockProxy(const LockProxy& obj);
		LockProxy& operator =(const LockProxy& obj) { return *this; }
	};

	/// [多线程安全] 标志位
	class SPADAS_API Flag : public Object<class FlagVars>
	{
	public:
		/// 类名称
		static const String TypeName;

		/// 创建Flag对象 (默认为not set状态)
		Flag();
		
		/// @brief 创建Flag对象并延迟一段时间后自动调用 Flag::set 方法。作为定时器，可用于等待或告知超时
		/// @param delaySet 延迟set的时间，单位毫秒
		Flag(UInt delaySet);

		/// 改为set状态
		void set();

		/// 改为not set状态
		void reset();
		
		/// @brief 延迟一段时间后自动调用 Flag::set 方法
		/// @param time 延迟set的时间，单位毫秒
		void delaySet(UInt time);
		
		/// @brief 延迟一段时间后自动调用 Flag::reset 方法
		/// @param time 延迟reset的时间，单位毫秒
		void delayReset(UInt time);

		/// 当前状态是否为set
		Bool check();

		/// @brief 等待状态变为set
		/// @param waitTime 等待时间，单位毫秒
		/// @returns 若超时则返回FALSE
		Bool waitSet(UInt waitTime = UINF);

		/// @brief 等待状态变为not set
		/// @param waitTime 等待时间，单位毫秒
		/// @returns 若超时则返回FALSE
		Bool waitReset(UInt waitTime = UINF);

	private:
		Bool isNull() { return FALSE; }
		Bool isValid() { return FALSE; }
	};

	/// [多线程安全] 线程同步器
	class SPADAS_API Barrier : public Object<class BarrierVars>
	{
	public:
		/// 类名称
		static const String TypeName;

		/// 创建一个同步器对象 (默认强度为0)
		Barrier();

		/// 设置强度
		void setStrength(UInt strength);

		/// 进入同步状态，当进入同步状态的线程数等于同步器强度时结束返回TRUE (超时则返回FALSE)
		Bool against(Flag interrupt);

	private:
		Bool isNull() { return FALSE; }
		Bool isValid() { return FALSE; }
	};

	/// 一个线程的状态信息
	struct ThreadStatus
	{
		/// 是否正在运行
		Bool isActive;

		/// 线程名称
		String threadName;

		/// 线程ID
		Optional<UInt> threadID;

		/// 用户设置的循环时间间隔 [ms] (当使用用户自定义的循环间隔时才有效)
		Optional<UInt> userTimeInterval;

		/// 线程循环平均处理时间 [ms]
		Optional<Float> averageProcessTime;

		/// 当前循环已运行时间 [ms] (通过此数值确认线程是否已卡死)
		Float currentLoopTime;
		
		/// 默认构造函数
		SPADAS_API ThreadStatus();
	};
	
	/// 一个流程的状态信息
	struct WorkflowStatus
	{
		/// 流程ID
		UInt workflowID;

		/// 流程名称
		String workflowName;

		/// 流程是否正在运行（只要还有一个线程活动即为TRUE）
		Bool isActive;

		/// 流程中所有线程的状态
		Array<ThreadStatus> threadStatus;
		
		/// 默认构造函数
		SPADAS_API WorkflowStatus();
	};
	
	/// 流程的标准接口
	class SPADAS_API IWorkflow
	{
	public:
		virtual ~IWorkflow() {};
		
		/// [可选] 给出流程的名称 (仅在 Threads::start 时被调用)
		virtual String getWorkflowName();
		
		/// 给出所有线程的名称，数组大小即为线程数。若返回空则报错 (仅在 Threads::start 时被调用)
		virtual Array<String> getThreadNames();
		
		/// [可选] 给出序号为threadIndex的线程默认的循环间隔，单位毫秒
		virtual UInt getTimeInterval(UInt threadIndex);

		/// [可选] 指定序号为threadIndex的线程是否接受用户自定义的循环间隔
		virtual Bool supportUserTimeInterval(UInt threadIndex);
		
		/// [可选] 给出序号为threadIndex的线程在线程开始处运行的代码 (若失败则返回FALSE)
		virtual Bool onThreadBegin(UInt threadIndex);
		
		/// 给出序号为threadIndex的线程在每次循环时调用的代码，其中应尽量不使用具有等待功能的函数
		virtual void onThreadLoop(UInt threadIndex, Flag shouldEnd);
		
		/// [可选] 给出序号为threadIndex的线程在线程结束时运行的代码
		virtual void onThreadEnd(UInt threadIndex);
	};

	/// [多线程安全] 多线程容器
	class SPADAS_API Threads : public Object<class ThreadsVars>
	{
	public:
		/// 类名称
		static const String TypeName;

		/// 无效对象（应由 Threads::start 或 Threads::begin 创建）
		Threads();
		
		/// 线程是否正在运行
		Bool isRunning();
		
		/// 停止流程，并等待所有线程结束 (若超时返回FALSE)。若持有对象，建议在调用此函数后执行 obj = Threads()
		Bool stop(UInt timeout = UINF);

		/// 停止流程，不等待即返回
		void stopAsync();

		/// 取得运行中流程的接口
		Interface<IWorkflow> getWorkflow();
		
		/// 取得流程运行的完整状态
		WorkflowStatus getWorkflowStatus();
		
		/// 使用用户自定义的循环间隔（需要 IWorkflow::supportUserTimeInterval 方法返回TRUE）
		void useUserTimeInterval(UInt threadIndex, UInt interval);
		
		/// 使用默认的循环间隔，该值通过流程接口的 IWorkflow::getTimeInterval 方法获得
		void useDefaultTimeInterval(UInt threadIndex);
		
		/// 启动子线程运行指定流程，在所有线程运行 IWorkflow::onThreadBegin 完毕后返回该流程的容器对象
		static Threads start(Interface<IWorkflow> workflow);

		/// 启动子线程运行指定流程，在所有线程运行 IWorkflow::onThreadBegin 完毕后返回该流程的容器对象，并输出每个线程是否成功开始
		static Threads start(Interface<IWorkflow> workflow, Array<Bool>& threadsRet);
		
		/// 在本线程中运行指定流程 (该流程所需线程数应为1)
		static void run(Interface<IWorkflow> workflow, Flag interrupt);

		/// 停止所有正在运行的流程，并等待所有线程结束 (若超时返回FALSE)
		static Bool stopAllThreads(UInt timeout = UINF);
		
		/// 取得指定流程的容器 (若未找到则返回无效容器)
		static Threads getThreadsOf(Interface<IWorkflow> workflow);
		
		/// 取得当前所有有效的多线程容器
		static Array<Threads> getAllThreads();
		
		/// 取得当前线程的ID
		static UInt getCurrentThreadID();
	};

	/// 任务的标准接口
	class SPADAS_API ITask
	{
	public:
		virtual ~ITask() {};

		/// 任务的执行函数
		virtual void onRunTask(Flag shouldEnd);
	};

	/// [多线程安全] 任务管理器
	class SPADAS_API TaskManager : public Object<class TaskManagerVars>
	{
	public:
		/// 类名称
		static const String TypeName;

		/// 创建任务管理器
		TaskManager();

		/// 开启线程执行任务
		void start(Interface<ITask> task);

		/// 停止线程，并等待结束 (若超时返回FALSE)
		Bool stop(Interface<ITask> task, UInt timeout = UINF);

		/// 获取正在运行中的任务
		Array<Interface<ITask> > getTasks();

		/// 查询任务是否运行中
		Bool isTaskRunning(Interface<ITask> task);

		/// 等待所有线程运行完成，若被中断则返回FALSE
		Bool waitAll(Flag interrupt);

		/// 停止所有线程并等待运行完成
		void stopAll(UInt timeout = UINF);

	private:
		Bool isNull() { return FALSE; }
		Bool isValid() { return FALSE; }
	};

	// 实用工具 //////////////////////////////////////////////////////////////

	/// 运行环境
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
			NILRT = 7,
		};
		static const Char* toString(Value val)
		{
			switch (val)
			{
				ES(Unknown);
				ES(Linux);
				ES(MacOS);
				ES(Windows);
				ES(NILRT);
				default: return 0;
			}
		}
	};

	/// Posix时间，毫秒单位
	typedef ULong MilliPosix;

	/// Posix时间，纳秒单位
	typedef ULong NanoPosix;

	/// 日期时间
	struct Time
	{
		/// 年
		UInt year;

		/// 月
		UInt month;

		/// 日
		UInt day;

		/// 时
		UInt hour;

		/// 分
		UInt minute;

		/// 秒
		UInt second;

		/// 默认构造函数，时间为1900年1月1日0时0分0秒
		SPADAS_API Time();

		/// 基于指定年月日时分秒初始化
		SPADAS_API Time(UInt year, UInt month, UInt day, UInt hour, UInt minute, UInt second);

		/// 是否等于
		SPADAS_API Bool operator ==(Time time);

		/// 是否不等于
		SPADAS_API Bool operator !=(Time time);

		/// 是否大于
		SPADAS_API Bool operator >(Time time);

		/// 是否小于
		SPADAS_API Bool operator <(Time time);

		/// 获取哈希值
		SPADAS_API Word getHash();

		/// 基于指定分隔符创建日期(年月日)字符串
		SPADAS_API String dateString(String separator = "/");

		/// 基于指定分隔符创建时间(时分秒)字符串
		SPADAS_API String timeString(String separator = ":");

		/// 基于分隔符转为字符串
		SPADAS_API String toString(String dateSeparator = "/", String timeSeparator = ":", String dateTimeSeparator = " ");
	};

	/// 精度至毫秒的日期时间
	struct TimeWithMS
	{
		/// 日期与时间，年月日时分秒
		Time dateTime;

		/// 毫秒数
		UInt milliseconds;

		/// 默认构造函数，时间为1900年1月1日0时0分0秒0毫秒
		TimeWithMS() : milliseconds(0)
		{}

		/// 基于日期和毫秒的初始化
		TimeWithMS(Time dateTime, UInt milliseconds) : dateTime(dateTime), milliseconds(milliseconds)
		{}

		/// 基于指定年月日时分秒初始化
		TimeWithMS(UInt year, UInt month, UInt day, UInt hour, UInt minute, UInt second, UInt milliseconds) : dateTime(year, month, day, hour, minute, second), milliseconds(milliseconds)
		{}

		/// 是否等于
		SPADAS_API Bool operator ==(TimeWithMS time);

		/// 是否不等于
		SPADAS_API Bool operator !=(TimeWithMS time);

		/// 返回在该时间基础上加/减指定毫秒的时间
		SPADAS_API TimeWithMS operator +(Long millisecond);

		/// 返回两个时间的时间差，单位毫秒
		SPADAS_API Long operator -(TimeWithMS time);

		/// 返回两个时间的时间差，以分量的形式
		SPADAS_API void minus(TimeWithMS time, Int& week, Int& day, Int& hour, Int& minute, Int& second, Int& millisecond);

		/// 分量时间（按本地时间）转Posix时间
		SPADAS_API MilliPosix localTimeToPosix();

		/// 分量时间（按国际协调时间）转Posix时间
		SPADAS_API MilliPosix utcTimeToPosix();

		/// Posix时间转分量时间（按本地时间）
		SPADAS_API void localTimeFromPosix(MilliPosix posixTime);

		/// Posix时间转分量时间（按国际协调时间）
		SPADAS_API void utcTimeFromPosix(MilliPosix posixTime);

		/// 基于分隔符转为字符串
		SPADAS_API String toString(String dateSeparator = "/", String timeSeparator = ":", String dateTimeSeparator = " ", String msSeparator = ".");
	};

	/// 计时器
	class SPADAS_API Timer : public Object<class TimerVars>
	{
	public:
		/// 类名称
		static const String TypeName;

		/// 创建一个计时器对象并立即启动
		Timer();

		/// 以指定的CPU计数作为开始时间创建一个计时器，该计数不得大于创建时的CPU计数
		Timer(ULong startCPUTick);
		
		/// 重启计时器
		void start();
		
		/// 获得经过时长，单位毫秒
		Double check();

		/// 获得开始时间，单位毫秒
		Double getStartTime();

		/// 获得开始时的CPU计数
		ULong getStartCPUTick();

		/// 获取当前CPU计数
		static ULong cpuTick();

		/// 获取每秒增长的CPU计数
		static ULong cpuTicksPerSecond();

	private:
		Bool isNull() { return FALSE; }
		Bool isValid() { return FALSE; }
	};

	/// 振荡器响应接口
	class SPADAS_API ITickHandler
	{
	public:
		virtual ~ITickHandler() {};

		/// 响应振荡器信号
		virtual void onTick(String name);
	};

	/// 振荡器
	class SPADAS_API Tick : public Object<class TickVars>, public IWorkflow
	{
	public:
		/// 类名称
		static const String TypeName;

		/// 无效对象
		Tick();

		/// 创建一个非自旋振荡器，并指定名称、响应接口和振荡周期（毫秒）。线程启动后将按该周期调用响应接口
		Tick(String name, Interface<ITickHandler> handler, UInt period);

		/// 创建一个振荡器，并指定名称、响应接口、振荡周期（毫秒）、以及是否自旋。线程启动后将按该周期调用响应接口
		Tick(String name, Interface<ITickHandler> handler, UInt period, Bool spin);

	private:
		String getWorkflowName() override;
		Array<String> getThreadNames() override;
		UInt getTimeInterval(UInt threadIndex) override;
		Bool onThreadBegin(UInt threadIndex) override;
		void onThreadLoop(UInt threadIndex, Flag shouldEnd) override;
		void onThreadEnd(UInt threadIndex) override;
	};

	/// 内存映射管理，用于硬件驱动和多进程通讯等
	class SPADAS_API MemoryMap : public Object<class MemoryMapVars>
	{
	public:
		/// 类名称
		static const String TypeName;

		/// 无效对象
		MemoryMap();

		/// 创建内存映射对象，指定映射文件路径(必须事先创建且文件大小不小于offset+size)，映射起点偏移以及映射区域大小。注意，映射起点偏移值应为以下N值的倍数：N = 4096 @ Linux/MacOS，N = 65536 @ Windows
		MemoryMap(Path file, PointerInt offset, PointerInt size);

		/// 解除映射
		void unmap();

		/// [非安全] 取得映射起点的虚拟内存地址
		Pointer getPointer();
	};

	/// 内存映射数据流的发送结果
	class SPADAS_API MemoryMapSendResult
	{
	public:
		enum class Value
		{
			Default = 0,

			/// 发送成功
			OK = 0,

			/// 未开启
			NotOpen = 1,

			/// 模式不正确
			WrongMode = 2,

			/// 数据大小超过范围（slotSize x slotCount）
			WrongSize = 3,

			/// 队列已满，或容量已不足以发送当前数据
			QueueFull = 4,
		};
		static const Char* toString(Value val)
		{
			switch (val)
			{
				ES(OK);
				ES(NotOpen);
				ES(WrongMode);
				ES(WrongSize);
				ES(QueueFull);
				default: return 0;
			}
		}
	};

	/// [多线程安全] 基于内存映射的数据流
	class SPADAS_API MemoryMapStream : public Object<class MemoryMapStreamVars>
	{
	public:
		/// 类名称
		static const String TypeName;

		/// 创建一个数据流（未开启状态）
		MemoryMapStream();

		/// 是否为开启状态
		Bool isOpen();

		/// @brief 打开数据流
		/// @param path 映射文件路径（不需要事先创建）
		/// @param slotSize 槽的大小，最好大于平均的数据包尺寸，单位为字节, 范围为16~1048576
		/// @param slotCount 槽的个数，需要保证slotSize乘slotCount大于数据包的最大尺寸，范围为1~1024
		/// @param sendMode 是否为发送模式，否则为接收模式
		/// @param host 是否控制映射文件的创建与删除，为TRUE的对象需要先执行open
		/// @returns 返回是否成功
		Bool open(Path path, UInt slotSize, UInt slotCount, Bool sendMode, Bool host);
		
		/// 关闭数据流
		void close();

		/// @brief 发送数据（直接使用指针）
		/// @param dataPtr 数据的起始指针
		/// @param byteCount 数据的字节数
		/// @returns 返回发送结果
		Enum<MemoryMapSendResult> send(Pointer dataPtr, UInt byteCount);

		/// @brief 发送数据
		/// @param data 二进制数据
		/// @returns 返回发送结果
		Enum<MemoryMapSendResult> send(Binary data);

		/// 接收所有收到的数据
		Array<Binary> receive();

	private:
		Bool isNull() { return FALSE; }
		Bool isValid() { return FALSE; }
	};

	/// Magic：四字符标识构成一个32位整型数字
	class SPADAS_API MagicNumber : public Object<class MagicNumberVars>
	{
	public:
		/// 类名称
		static const String TypeName;

		/// 创建一个无效数字 (0)
		MagicNumber();

		/// 创建一个Magic数字 (所有字符应位于0~9, A~Z和a~z之间，a~z将被转换为大写)
		MagicNumber(Char c0, Char c1, Char c2, Char c3);

		/// 与另一个Magic数字是否相同
		Bool operator ==(MagicNumber magic);

		/// 与另一个Magic数字是否相同
		Bool operator !=(MagicNumber magic);

		/// 取得四个字符
		Array<Char> getChars();

		/// 取得Magic数字
		UInt getNumber();

		/// 获取哈希值
		Word getHash();

	private:
		Bool isNull() { return FALSE; }
		Bool isValid() { return FALSE; }
	};

	/// 动态库加载器
	class SPADAS_API LibraryLoader : public Object<class LibraryLoaderVars>
	{
	public:
		/// 类名称
		static const String TypeName;

		/// 创建动态库加载器
		LibraryLoader();

		/// 使用指定文件夹下的库名称打开动态库（无需添加lib等前缀或后缀名），libVersion用于指定so等动态库的后缀版本号（如"1.0"）
		Bool openWithName(Path libDir, String libName, String libVersion = String());

		/// 使用指定文件夹下的库名称打开动态库（无需添加lib等前缀或后缀名），libVersion用于指定so等动态库的后缀版本号（如"1.0"），若失败则输出错误信息
		Bool openWithName(Path libDir, String libName, String libVersion, String& errorMessage);

		/// 使用库文件路径打开动态库
		Bool openWithPath(Path libPath);

		/// 使用库文件路径打开动态库，若失败则输出错误信息
		Bool openWithPath(Path libPath, String& errorMessage);

		/// 关闭动态库（一般不执行）
		void close();

		/// 获取符号地址
		Pointer getSymbol(String symbol);

	private:
		Bool isNull() { return FALSE; }
		Bool isValid() { return FALSE; }
	};

	/// 系统功能函数命名空间
	namespace system
	{
		/// 取得当前Spadas版本
		SPADAS_API String getSpadasVersion();

		/// 取得当前运行环境
		SPADAS_API Enum<Environment> getEnv();

		/// 检查字节序是否Big-Endian
		SPADAS_API Bool isBigEndian();

		/// 获得当前主机时间（本地时区）
		SPADAS_API Time getTime();

		/// 获得当前主机时间（本地时区），带毫秒信息
		SPADAS_API TimeWithMS getTimeWithMS();

		/// 等待指定时间，单位毫秒（系统负载高时精度可能会降至5~10毫秒）
		SPADAS_API void wait(UInt time);

		/// 以自旋方式等待指定时间，单位微秒
		SPADAS_API void waitSpin(UInt timeUS);

		/// 执行一个控制台命令
		SPADAS_API void command(String cmd);

		/// 强制结束应用程序
		SPADAS_API void exit();

		/// 添加环境变量
		SPADAS_API void addEnvironmentPath(Path path);

		/// Ping，超时单位为毫秒
		SPADAS_API Bool ping(String ip, UInt timeout = 1000);
	}

	/// 实用功能函数命名空间
	namespace utility
	{
		/// @brief [非安全] 以字节为单位设置一段内存的值
		/// @param value 希望设置的字节数值
		/// @param dst 内存起始地址
		/// @param setSize 设置的字节数
		SPADAS_API void memorySet(Byte value, Pointer dst, UInt setSize);

		/// @brief [非安全] 内存拷贝（效率最高）
		/// @param src 拷贝源内存起始地址
		/// @param dst 拷贝目标内存起始地址
		/// @param copySize 拷贝字节数
		SPADAS_API void memoryCopy(ConstPointer src, Pointer dst, UInt copySize);

		/// [非安全] 值类型的强制转换（必须保证两个类型sizeof一致，结构体的话必须保证类型与顺序都一致）
		template <typename SrcType, typename DstType>
		DstType valueCast(SrcType val);

		/// [非安全] 值类型数组的强制转换（必须保证两个类型sizeof一致，结构体的话必须保证类型与顺序都一致）
		template <typename SrcType, typename DstType>
		Array<DstType> valueArrayCast(Array<SrcType> src);

		/// 将数值键数组转为数值数组
		template <typename Type>
		Array<Type> unpackKeys(Array<NumericKey<Type> > keys);

		/// 将数值键的键值对数组转为数值的键值对数组
		template <typename Type, typename ValueType>
		Array<KeyValue<Type, ValueType> > unpackKeyValues(Array<KeyValue<NumericKey<Type>, ValueType> > keyValues);

		/*
		* ByteBGR: size=(height, width, 3)
		* ByteRGB: size=(height, width, 3)
		* ByteGray: size=(height, width)
		* ByteUYVY: size=(height, width, 2)
		* ByteYUV: size=(height, width, 3)
		* ByteBool: size=(height, width), TRUE的值为1
		* ByteBGRA: size=(height, width, 4)
		* ByteRGBA: size=(height, width, 4)
		* WordBGR: size=(height, width, 3)
		* WordGray: size=(height, width)
		*/

		/// Int型矩阵与图像的相互转换
		SPADAS_API IntMat imageToMatrixInt(Image image);

		/// Int型矩阵与图像的相互转换
		SPADAS_API Image matrixIntToImage(IntMat matrix, Enum<PixelFormat> format);

		/*
		* FloatBGR: size=(height, width, 3)
		* FloatGray: size=(height, width)
		* FloatHSV: size=(height, width, 3)
		*/

		/// Float型矩阵与图像的相互转换
		SPADAS_API FloatMat imageToMatrixFloat(Image image);

		/// Float型矩阵与图像的相互转换
		SPADAS_API Image matrixFloatToImage(FloatMat matrix, Enum<PixelFormat> format);

		/// 用FloatGray图像表现2D矩阵的值，需要指定矩阵的数据范围，对应于图像的0.0~1.0
		SPADAS_API Image matrixRangeToImage(IntMat mat, Int lower, Int upper);

		/// 用FloatGray图像表现2D矩阵的值，需要指定矩阵的数据范围，对应于图像的0.0~1.0
		SPADAS_API Image matrixRangeToImage(FloatMat mat, Float lower, Float upper);

		/// 用FloatGray图像表现2D矩阵的值，需要指定矩阵的数据范围，对应于图像的0.0~1.0
		SPADAS_API Image matrixRangeToImage(DoubleMat mat, Double lower, Double upper);
	}

	// 插件相关类型定义 //////////////////////////////////////////////////////////////

	/// 代替Bool类型Optional
	struct GeneralElement;
	struct OptionalBool
	{
		/// 是否有效
		Bool valid;

		/// 布尔值
		Bool value;

		/// 默认构造函数，初始化为无效值
		OptionalBool() : valid(FALSE), value(FALSE)
		{}

		/// 基于输入的布尔值初始化
		OptionalBool(Bool val) : valid(TRUE), value(val)
		{}

		/// 基于 spadas::GeneralElement创建，非0都为TRUE
		SPADAS_API OptionalBool(GeneralElement elem);

		/// 按输入的布尔值赋值
		SPADAS_API OptionalBool& operator =(Bool val);

		/// 是否有效且为真
		SPADAS_API Bool isValidAndTrue();

		/// 是否有效且为假
		SPADAS_API Bool isValidAndFalse();

		/// 是否相等
		SPADAS_API Bool operator ==(OptionalBool val);

		/// 是否不等于
		SPADAS_API Bool operator !=(OptionalBool val);

		/// 转字符串，若无效则转为(invalid)
		SPADAS_API String toString();
	};

	/// 代替Int类型Optional
	struct OptionalInt
	{
		/// 是否有效
		Bool valid;

		/// 整型值
		Int value;

		/// 默认构造函数，初始化为无效值
		OptionalInt() : valid(FALSE), value(0)
		{}

		/// 基于输入的整型值初始化
		OptionalInt(Int val) : valid(TRUE), value(val)
		{}

		/// 基于 spadas::GeneralElement创建
		SPADAS_API OptionalInt(GeneralElement elem);

		/// 按输入的整型值赋值
		SPADAS_API OptionalInt& operator =(Int val);

		/// 是否相等
		SPADAS_API Bool operator ==(OptionalInt val);

		/// 是否不等于
		SPADAS_API Bool operator !=(OptionalInt val);

		/// 转字符串，若无效则转为(invalid)
		SPADAS_API String toString();
	};

	/// 代替Double类型Optional
	struct OptionalDouble
	{
		/// 是否有效
		Bool valid;

		/// 浮点值
		Double value;

		/// 默认构造函数，初始化为无效值
		OptionalDouble() : valid(FALSE), value(0)
		{}

		/// 基于输入的浮点值初始化
		OptionalDouble(Double val) : valid(TRUE), value(val)
		{}

		/// 基于 spadas::GeneralElement创建
		SPADAS_API OptionalDouble(GeneralElement elem);

		/// 按输入的浮点值赋值
		SPADAS_API OptionalDouble& operator =(Double val);

		/// 加法，若任一值为无效则返回无效值
		SPADAS_API OptionalDouble operator +(OptionalDouble val);

		/// 减法，若任一值为无效则返回无效值
		SPADAS_API OptionalDouble operator -(OptionalDouble val);

		/// 乘法，若任一值为无效则返回无效值
		SPADAS_API OptionalDouble operator *(OptionalDouble val);

		/// 是否相等
		SPADAS_API Bool operator ==(OptionalDouble val);

		/// 是否不等于
		SPADAS_API Bool operator !=(OptionalDouble val);

		/// 转字符串，若无效则转为(invalid)
		SPADAS_API String toString();
	};

	/// Session标识符
	class SPADAS_API SessionIdentifier
	{
	public:
		/// 创建无效标识符
		SessionIdentifier();

		/// 基于指定年月日时分秒初始化
		SessionIdentifier(Word year, Byte month, Byte day, Byte hour, Byte minute, Byte second);

		/// 基于日期初始化
		SessionIdentifier(Time time);

		/// 基于字符串(yyyy-MM-dd-HH-mm-ss)初始化，若失败则置为0
		SessionIdentifier(String idString);

		/// 是否有效
		Bool isValid();

		/// 是否等于
		Bool operator ==(SessionIdentifier id);

		/// 是否不等于
		Bool operator !=(SessionIdentifier id);

		/// 是否大于
		Bool operator >(SessionIdentifier id);

		/// 是否小于
		Bool operator <(SessionIdentifier id);

		/// 获取哈希值
		Word getHash();

		/// 转为字符串(yyyy-MM-dd-HH-mm-ss)
		String toString();

		/// 转为时间
		Time toTime();

	private:
		ULong value;
	};

	/// 时间偏置同步状态
	class SPADAS_API TimeOffsetSync
	{
	public:
		enum class Value
		{
			Default = 0,

			/// 未同步或同步源未知
			None = 0,

			/// 已与授时服务器时间同步
			Server = 1,

			/// 已与卫星时间同步
			Gnss = 2,
		};
		static const Char* toString(Value val)
		{
			switch (val)
			{
				ES(None);
				ES(Server);
				ES(Gnss);
				default: return 0;
			}
		}
	};

	/// 简单时间戳
	struct ShortTimestamp
	{
		/// Session标识符
		SessionIdentifier session;

		/// 时间偏置，单位秒，大于零有效
		Double offset;

		/// 默认构造函数
		ShortTimestamp() : offset(0)
		{}

		/// 基于Session标识符和时间偏置初始化
		ShortTimestamp(SessionIdentifier session, Double offset) : session(session), offset(offset)
		{}

		/// 是否等于
		SPADAS_API Bool operator ==(ShortTimestamp timestamp);

		/// 是否不等于
		SPADAS_API Bool operator !=(ShortTimestamp timestamp);

		/// 是否大于
		SPADAS_API Bool operator >(ShortTimestamp timestamp);

		/// 是否小于
		SPADAS_API Bool operator <(ShortTimestamp timestamp);

		/// 转为字符串显示，格式为"Session年月日-时-分-秒-偏置"，如2019-01-01-12-30-45-123.456789
		SPADAS_API String toString();
	};

	/// 完整时间戳
	struct FullTimestamp
	{
		/// Session标识符
		SessionIdentifier session;

		/// 时间偏置，单位秒，大于零有效
		Double offset;

		/// 时间偏置来源
		TimeOffsetSync::Value offsetSync;

		/// 到达时CPU计数，0表示无效
		ULong cpuTick;

		/// 到达时主机Posix时间，单位纳秒，0表示无效
		NanoPosix hostPosix;

		/// 采样时客机Posix时间，单位纳秒，0表示无效
		NanoPosix guestPosix;

		/// 采样时授时服务器Posix时间，单位纳秒，0表示无效
		NanoPosix serverPosix;

		/// 采样时卫星Posix时间，单位纳秒，0表示无效
		NanoPosix gnssPosix;

		/// 默认构造函数
		FullTimestamp() : offset(0), offsetSync(TimeOffsetSync::Value::None), cpuTick(0), hostPosix(0), guestPosix(0), serverPosix(0), gnssPosix(0)
		{}

		/// 从简单时间戳构造，无同步信息或Session无关时间信息
		FullTimestamp(ShortTimestamp timestamp) : session(timestamp.session), offset(timestamp.offset), offsetSync(TimeOffsetSync::Value::None), cpuTick(0), hostPosix(0), guestPosix(0), serverPosix(0), gnssPosix(0)
		{}

		/// 是否等于（仅比较Session标识符和时间偏置）
		SPADAS_API Bool operator ==(FullTimestamp timestamp);

		/// 是否不等于（仅比较Session标识符和时间偏置）
		SPADAS_API Bool operator !=(FullTimestamp timestamp);

		/// 是否大于（仅比较Session标识符和时间偏置）
		SPADAS_API Bool operator >(FullTimestamp timestamp);

		/// 是否小于（仅比较Session标识符和时间偏置）
		SPADAS_API Bool operator <(FullTimestamp timestamp);

		/// 转为简单时间戳
		SPADAS_API ShortTimestamp toShort();

		/// 转为字符串显示，格式为"Session年月日-时-分-秒-偏置"，如2019-01-01-12-30-45-123.456789
		SPADAS_API String toString();
	};

	/// Session无关时间的类别
	class SPADAS_API TimeType
	{
	public:
		enum class Value
		{
			Default = 0,

			/// 到达时CPU计数
			CPUTick = 0,

			/// 到达时主机Posix时间，单位纳秒
			HostPosix = 1,

			/// 采样时客机Posix时间，单位纳秒
			GuestPosix = 2,

			/// 采样时授时服务器Posix时间，单位纳秒
			ServerPosix = 3,

			/// 采样时卫星Posix时间，单位纳秒
			GnssPosix = 4,
		};
		static const Char* toString(Value val)
		{
			switch (val)
			{
				ES(CPUTick);
				ES(HostPosix);
				ES(GuestPosix);
				ES(ServerPosix);
				ES(GnssPosix);
				default: return 0;
			}
		}
	};

	/// 信号
	struct SessionSignal
	{
		/// 时间戳
		ShortTimestamp timestamp;

		/// 数值
		Double value;

		/// 默认构造函数，初始值为0
		SessionSignal() : value(0)
		{}

		/// 基于时间戳和数值初始化
		SessionSignal(ShortTimestamp timestamp, Double val) : timestamp(timestamp), value(val)
		{}
	};

	/// 信号数据表，key为信号ID，格式为"大类:小类:信号名称"。其中总线信号的情况下，大类为协议文件名，如vehicle.dbc；小类为报文本通道ID，如123
	typedef Dictionary<Array<SessionSignal> > SessionSignalTable;

	/// 一般设备输出原始数据
	struct GeneralDeviceData
	{
		/// 到达主机时的CPU计数
		ULong cpuTick;

		/// 原始数据协议ID，一般格式为"xxx-v?"，xxx表示数据来源，v?表示版本
		String protocol;

		/// 数值数组数据，可为空
		Array<Double> vector;

		/// 二进制数据，可为空
		Binary binary;

		/// 默认构造函数
		GeneralDeviceData()
		{}

		/// 基于时间戳和数据初始化
		GeneralDeviceData(ULong cpuTick, Array<Double> vector, Binary binary) : cpuTick(cpuTick), vector(vector), binary(binary)
		{}
	};

	/// 通用原始数据
	struct SessionGeneralRawData
	{
		/// 时间戳
		FullTimestamp timestamp;

		/// 数值数组数据，可为空
		Array<Double> vector;

		/// 二进制数据，可为空
		Binary binary;

		/// 默认构造函数
		SessionGeneralRawData()
		{}

		/// 基于时间戳和数据初始化
		SessionGeneralRawData(FullTimestamp timestamp, Array<Double> vector, Binary binary) : timestamp(timestamp), vector(vector), binary(binary)
		{}
	};

	/// 通用原始数据表，key为原始数据协议ID，一般格式为"xxx-v?"，xxx表示数据来源，v?表示版本
	typedef Dictionary<Array<SessionGeneralRawData> > SessionGeneralRawDataTable;

	/// 通用原始数据发送接口
	class SPADAS_API IGeneralDataTransmitter
	{
	public:
		virtual ~IGeneralDataTransmitter() {};

		/// @brief 立即发送数据
		/// @param protocol 原始数据协议ID，一般格式为"xxx-v?"，xxx表示数据来源，v?表示版本
		/// @param vector 数值数组数据
		/// @param binary 二进制数据
		/// @returns 是否成功发送，无效表示不确定
		virtual OptionalBool transmitNow(String protocol, Array<Double> vector, Binary binary);

		/// @brief 按时间偏置预约发送数据（将优先按服务器Posix时间预约发送，不满足条件则按CPU计数时间发送）
		/// @param protocol 原始数据协议ID，一般格式为"xxx-v?"，xxx表示数据来源，v?表示版本
		/// @param vector 数值数组数据
		/// @param binary 二进制数据
		/// @param offset 时间偏置，单位秒 (必须大于该协议的上一帧预约发送数据的时间戳)
		/// @param tolerance 允许的最大延迟发送时间，单位纳秒
		/// @returns 是否成功预约发送，无效表示不确定
		virtual OptionalBool transmitAtTimeOffset(String protocol, Array<Double> vector, Binary binary, Double offset, UInt tolerance);

		/// @brief 指定按服务器Posix时间预约发送视频帧 (必须大于该协议的上一帧预约发送数据的时间戳)
		/// @param protocol 原始数据协议ID，一般格式为"xxx-v?"，xxx表示数据来源，v?表示版本
		/// @param vector 数值数组数据
		/// @param binary 二进制数据
		/// @param serverPosix 授时服务器Posix时间，单位纳秒
		/// @param tolerance 允许的最大延迟发送时间，单位纳秒
		/// @returns 是否成功预约发送，无效表示不确定
		virtual OptionalBool transmitAtServerPosix(String protocol, Array<Double> vector, Binary binary, NanoPosix serverPosix, UInt tolerance);
	};

	/// 通用样本元素
	struct GeneralElement
	{
		/// 是否有效
		Bool valid;

		/// 是否为文本元素
		Bool isText;

		/// 元素值
		Double value;

		/// 元素文本
		String text;

		/// 默认构造函数，创建无效样本元素
		GeneralElement() : valid(FALSE), isText(FALSE), value(0)
		{}

		/// 创建有效的值元素
		GeneralElement(Double value) : valid(TRUE), isText(FALSE), value(value)
		{}

		/// 创建有效的文本元素
		GeneralElement(String text) : valid(TRUE), isText(TRUE), value(0), text(text)
		{}

		/// 基于 spadas::OptionalBool 创建，转为0或1
		GeneralElement(OptionalBool value) : valid(value.valid), isText(FALSE), value(value.value ? 1 : 0)
		{}

		/// 基于 spadas::OptionalInt 创建，转为0或1
		GeneralElement(OptionalInt value) : valid(value.valid), isText(FALSE), value((Double)value.value)
		{}

		/// 基于 spadas::OptionalDouble 创建，转为0或1
		GeneralElement(OptionalDouble value) : valid(value.valid), isText(FALSE), value(value.value)
		{}

		/// 转字符串，若无效则转为(invalid)
		SPADAS_API String toString();
	};

	/// 通用样本
	struct SessionGeneralSample
	{
		/// 时间戳
		FullTimestamp timestamp;

		/// 样本数据数组
		Array<GeneralElement> values;

		/// 关键元素个数，表示样本数据数组中前若干个为关键数据，默认为0
		UInt significantCount;

		/// 默认构造函数
		SessionGeneralSample() : significantCount(0)
		{}

		/// 基于时间戳和数据初始化
		SessionGeneralSample(FullTimestamp timestamp, Array<GeneralElement> values) : timestamp(timestamp), values(values), significantCount(0)
		{}

		/// 基于时间戳、数据、关键元素个数初始化
		SessionGeneralSample(FullTimestamp timestamp, Array<GeneralElement> values, UInt significantCount) : timestamp(timestamp), values(values), significantCount(significantCount)
		{}
	};

	/// 通用样本表，key为样本协议ID，一般格式为"xxx-v?"或"xxx-v?@?"，xxx表示样本类型，v?表示版本，@?表示通道序号，序号从0开始
	typedef Dictionary<Array<SessionGeneralSample> > SessionGeneralSampleTable;

	/// 通用样本插值结果
	class SPADAS_API SampleInterpolationResult
	{
	public:
		enum class Value
		{
			Default = 0,

			/// 成功插值返回
			OK = 0,

			/// 样本缓存为空，或样本范围整体晚于插值时间戳，或样本范围整体过早于插值时间戳
			OutOfRange = 1,

			/// 样本不支持插值，直接返回最近样本
			NearestInstead = 2,

			/// 样本范围整体早于插值时间戳，但在阈值earlyThresh范围内，需等待新样本再进行插值
			TooEarly = 3,

			/// 通用样本解析错误
			ParseError = 4,

			/// 未设置样本缓存的协议
			NoProtocol = 5,
		};
		static const Char* toString(Value val)
		{
			switch (val)
			{
				ES(OK);
				ES(OutOfRange);
				ES(NearestInstead);
				ES(TooEarly);
				ES(ParseError);
				ES(NoProtocol);
				default: return 0;
			}
		}
	};

	/// [多线程安全] 通用样本缓存
	class SPADAS_API SessionSampleBuffer : public Object<class SessionSampleBufferVars>
	{
	public:
		/// 类名称
		static const String TypeName;

		/// 创建样本缓存
		SessionSampleBuffer();

		/// @brief 设置缓存最大时长
		/// @param length 最大时长，单位秒，范围1~60秒，默认3秒
		void setBufferLength(Double length);

		/// @brief 设置对应的通用样本协议
		/// @param protocol 通用样本协议ID
		void setProtocol(String protocol);

		/// @brief 获取对应的通用样本协议ID
		/// @param withChannel 是否带通道信息，即协议ID末尾的"@?"
		/// @returns 通用样本协议ID
		String getProtocol(Bool withChannel);

		/// @brief 添加新样本
		/// @param sample 新通用样本
		void addSample(SessionGeneralSample sample);

		/// 缓存是否为空
		Bool isEmpty();

		/// 获取缓存中样本个数
		UInt getSampleCount();

		/// 获取当前Session的标识符
		SessionIdentifier getCurrentSession();

		/// @brief 获取最早样本
		/// @param sampleEarliest 输出最早样本
		/// @returns 若样本缓存为空则返回FALSE
		Bool getEarliest(SessionGeneralSample& sampleEarliest);

		/// @brief 获取最新样本
		/// @param sampleEarliest 输出最新样本
		/// @returns 若样本缓存为空则返回FALSE
		Bool getLatest(SessionGeneralSample& sampleLatest);

		/// @brief 根据时间偏置寻找最近样本
		/// @param offset 目标时间偏置
		/// @param sampleNearest 输出离目标最近样本
		/// @returns 若样本缓存为空，则返回FALSE
		Bool getNearest(Double offset, SessionGeneralSample& sampleNearest);

		/// @brief 根据时间戳寻找最近样本
		/// @param timeType 时间类别
		/// @param time 目标时间
		/// @param sampleNearest 输出离目标最近样本
		/// @returns 若样本缓存为空，则返回FALSE
		Bool getNearest(Enum<TimeType> timeType, ULong time, SessionGeneralSample& sampleNearest);

		/// @brief 根据时间偏置寻找前后两个样本
		/// @param offset 目标时间偏置
		/// @param sampleBefore 输出比时间戳早的最近样本
		/// @param sampleAfter 输出比时间戳晚的最近样本
		/// @returns 若无则返回FALSE
		Bool search(Double offset, SessionGeneralSample& sampleBefore, SessionGeneralSample& sampleAfter);

		/// @brief 根据时间戳寻找前后两个样本
		/// @param timeType 时间类别
		/// @param time 目标时间
		/// @param sampleBefore 输出比时间戳早的最近样本
		/// @param sampleAfter 输出比时间戳晚的最近样本
		/// @returns 若无则返回FALSE
		Bool search(Enum<TimeType> timeType, ULong time, SessionGeneralSample& sampleBefore, SessionGeneralSample& sampleAfter);

		/// @brief 根据时间偏置寻找前后两个样本并插值
		/// @param offset 目标时间偏置
		/// @param interpolatedSample 输出插值完成的样本
		/// @param earlyThresh 用于判断缓存范围是否过早的阈值，参考 spadas::SampleInterpolationResult::Value::TooEarly
		/// @details 该模板类型必须实现以下函数：\n
		/// - Bool fromGeneralSample(String protocol, SessionGeneralSample); \n
		/// - static Bool supportInterpolation(); \n
		/// - static Type interpolate(Type& s1, Double w1, Type& s2, Double w2, FullTimestamp timestamp);
		template <typename Type>
		Enum<SampleInterpolationResult> interpolate(Double offset, Type& interpolatedSample, UInt earlyThresh = 1000/* ms */);

	private:
		Bool isNull() { return FALSE; }
		Bool isValid() { return FALSE; }
	};

	/// 通用样本缓存表，key为样本协议ID，一般格式为"xxx-v?"或"xxx-v?@?"，xxx表示样本类型，v?表示版本，@?表示通道序号，序号从0开始
	typedef Dictionary<SessionSampleBuffer> SessionSampleBufferTable;

	/// 矩阵样本
	struct SessionMatrixSample
	{
		/// 时间戳
		FullTimestamp timestamp;

		/// 矩阵数据，按行、列的顺序存储，如第0元素为第一行第一列，第1元素为第一行第二列，...
		Array<Float> matData;

		/// 行数
		UInt rows;

		/// 列数
		UInt cols;

		/// 默认构造函数
		SessionMatrixSample() : rows(0), cols(0)
		{}

		/// 基于矩阵尺寸初始化
		SessionMatrixSample(Size2D size)
		{
			matData = Array<Float>(size.dim0 * size.dim1);
			rows = size.dim0;
			cols = size.dim1;
		}
	};

	/// 矩阵样本表，key为样本协议ID，一般格式为"xxx-v?"或"xxx-v?@?"，xxx表示样本类型，v?表示版本，@?表示通道序号，序号从0开始
	typedef Dictionary<Array<SessionMatrixSample> > SessionMatrixSampleTable;

	/// 一般设备状态
	class SPADAS_API GeneralDeviceStatus
	{
	public:
		enum class Value
		{
			Default = 0,

			/// 未设定为自动连接模式
			NotConnect = 0,

			/// 连接正常
			OK = 1,

			/// 连接错误
			Error = 2,

			/// 连接无错误，但存在一定问题
			Warning = 3,
		};
		static const Char* toString(Value val)
		{
			switch (val)
			{
				ES(NotConnect);
				ES(OK);
				ES(Error);
				ES(Warning);
				default: return 0;
			}
		}
	};

	/// 总线通道类型
	class SPADAS_API BusChannelType
	{
	public:
		enum class Value
		{
			Default = 0,

			/// 无效类型
			Invalid = 0,

			/// CAN总线，报文数据即payload，1～8字节
			Can = 1,

			/// CAN-FD总线，报文数据即payload，1～64字节
			CanFD = 2,

			/// LIN总线，报文数据即payload，1～8字节
			Lin = 3,

			/// Flexray总线，报文数据由标志位字节(从低至高为startup,sync,null)、cycle字节和payload构成(共2～256字节)，报文ID即Slot ID
			Flexray = 4,

			/// 以太网总线，报文数据为包含链路层等等协议的完整以太网帧数据，报文ID定义为源MAC的后四字节(小字序)
			Ethernet = 5,

			/// SOME/IP车载以太网总线，报文数据为包含链路层等等协议的完整以太网帧数据，报文ID即Message ID(由Service ID和Method ID组成)
			SomeIP = 6,
		};
		static const Char* toString(Value val)
		{
			switch (val)
			{
				ES(Invalid);
				ES(Can);
				ES(CanFD);
				ES(Lin);
				ES(Flexray);
				ES(Ethernet);
				ES(SomeIP);
				default: return 0;
			}
		}
	};

	/// 总线波特率
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
			CAN_500k = 14, // CAN默认值
			CAN_666k = 15,
			CAN_800k = 16,
			CAN_1M = 17,

			CANFD_500k = 101,
			CANFD_1M = 102,
			CANFD_2M = 103, // CAN-FD默认值
			CANFD_4M = 104,
			CANFD_8M = 105,
			CANFD_6M = 106,
			CANFD_2d5M = 107,
			CANFD_5M = 108,

			LIN_1k = 201,
			LIN_9d6k = 202, // LIN默认值
			LIN_16d5k = 203,
			LIN_19d2k = 204,
			LIN_20k = 205,

			FR_2d5M = 301,
			FR_5M = 302,
			FR_10M = 303, // Flexray默认值
		};
		static const Char* toString(Value val)
		{
			switch (val)
			{
				ES(Invalid);
				ES(CAN_5k);
				ES(CAN_10k);
				ES(CAN_20k);
				ES(CAN_33k);
				ES(CAN_40k);
				ES(CAN_50k);
				ES(CAN_62k);
				ES(CAN_80k);
				ES(CAN_83k);
				ES(CAN_100k);
				ES(CAN_125k);
				ES(CAN_200k);
				ES(CAN_250k);
				ES(CAN_400k);
				ES(CAN_500k);
				ES(CAN_666k);
				ES(CAN_800k);
				ES(CAN_1M);
				ES(CANFD_500k);
				ES(CANFD_1M);
				ES(CANFD_2M);
				ES(CANFD_4M);
				ES(CANFD_8M);
				ES(CANFD_6M);
				ES(CANFD_2d5M);
				ES(CANFD_5M);
				ES(LIN_1k);
				ES(LIN_9d6k);
				ES(LIN_16d5k);
				ES(LIN_19d2k);
				ES(LIN_20k);
				ES(FR_2d5M);
				ES(FR_5M);
				ES(FR_10M);
				default: return 0;
			}
		}
	};

	/// 总线设备输出原始数据
	struct BusDeviceData
	{
		/// 到达主机时的CPU计数
		ULong cpuTick;

		/// 到达总线设备时其上的时间偏置，单位秒，0表示无效
		Double bridgeTimeOffset;

		/// 到达总线设备时其上的Posix时间，单位纳秒，0表示无效
		NanoPosix bridgeGuestPosix;

		/// 总线通道，1~16
		UInt channel;

		/// 该通道内的报文ID
		UInt id;

		/// 报文数据
		Binary binary;

		/// 默认构造函数
		BusDeviceData() : cpuTick(0), bridgeTimeOffset(0), bridgeGuestPosix(0), channel(0), id(0)
		{}
	};

	/// 总线原始数据
	struct SessionBusRawData
	{
		/// 时间戳
		ShortTimestamp timestamp;

		/// 总线通道，1~16
		UInt channel;

		/// 该通道内的报文ID
		UInt id;

		/// 报文数据
		Binary binary;

		/// 默认构造函数
		SessionBusRawData() : channel(0), id(0)
		{}
	};

	/// 总线原始数据表（长度16, 分别表示总线通道1~16）
	typedef Array<Array<SessionBusRawData> > SessionBusRawDataTable;

	/// 总线报文发送接口
	class SPADAS_API IBusMessageTransmitter
	{
	public:
		virtual ~IBusMessageTransmitter() {};

		/// @brief 立即发送报文
		/// @param channel 总线通道，1~16
		/// @param id 该通道内的报文ID
		/// @param binary 报文数据
		/// @returns 是否成功发送，无效表示不确定
		virtual OptionalBool transmitNow(UInt channel, UInt id, Binary binary);

		/// @brief 设定重复发送报文
		/// @param channel 总线通道，1~16
		/// @param id 该通道内的报文ID
		/// @param binary 报文数据
		/// @param interval 发送周期[ms]，有效范围10~1000ms
		/// @returns 是否成功设定重复发送，无效表示不确定
		virtual OptionalBool transmitRepeatedly(UInt channel, UInt id, Binary binary, UInt interval);

		/// @brief 按时间偏置预约发送报文（将优先按授时服务器Posix时间预约发送，不满足条件则按CPU计数时间发送）
		/// @param channel 总线通道，1~16
		/// @param id 该通道内的报文ID
		/// @param binary 报文数据
		/// @param offset 时间偏置，单位秒 (必须大于该通道上一帧预约发送报文的时间戳)
		/// @param tolerance 允许的最大延迟发送时间，单位纳秒
		/// @returns 是否成功预约发送，无效表示不确定
		virtual OptionalBool transmitAtTimeOffset(UInt channel, UInt id, Binary binary, Double offset, UInt tolerance);

		/// @brief 指定按授时服务器Posix时间预约发送报文 (必须大于该通道上一帧预约发送报文的时间)
		/// @param channel 总线通道，1~16
		/// @param id 该通道内的报文ID
		/// @param binary 报文数据
		/// @param serverPosix 授时服务器Posix时间，单位纳秒
		/// @param tolerance 允许的最大延迟发送时间，单位纳秒
		/// @returns 是否成功预约发送，无效表示不确定
		virtual OptionalBool transmitAtServerPosix(UInt channel, UInt id, Binary binary, NanoPosix serverPosix, UInt tolerance);
	};

	/// 总线设备ID
	struct BusDeviceID
	{
		/// 插件类型ID
		String type;

		/// 设备序列号
		ULong deviceSerial;

		/// 设备通道序号，从0开始
		UInt deviceChannelIndex;

		/// 默认构造函数
		BusDeviceID() : deviceSerial(0), deviceChannelIndex(0)
		{}
	};

	/// 总线设备信息
	struct BusDeviceInfo
	{
		/// 总线设备ID
		BusDeviceID id;

		/// 该通道支持的总线类型
		Array<Enum<BusChannelType> > types;

		/// 设备通道描述
		String description;

		/// 是否支持按授时服务器Posix时间预约发送报文
		Bool supportTransmitScheduled;

		/// 默认构造函数
		BusDeviceInfo() : supportTransmitScheduled(FALSE)
		{}
	};

	/// 总线设备配置
	struct BusDeviceConfig
	{
		/// 总线设备ID
		BusDeviceID id;

		/// 该通道的总线类型
		Enum<BusChannelType> type;

		/// 主波特率
		Enum<BusBaudRate> rate;

		/// 副波特率。目前仅用于 spadas::BusChannelType::Value::CanFD ，表示arbitration bit-rate
		Enum<BusBaudRate> rate2;

		/// 软件通道映射，1~16
		UInt mapChannel;

		/// 默认构造函数
		BusDeviceConfig() : mapChannel(0)
		{}
	};

	/// 总线数据负载
	struct BusChannelPayload
	{
		/// 软件通道，1~16
		UInt mapChannel;

		/// 数据负载，单位%
		Float payloadPercent;

		/// 默认构造函数
		BusChannelPayload() : mapChannel(0), payloadPercent(0)
		{}
	};

	/// 总线报文信息
	struct BusMessageInfo
	{
		/// 软件通道
		UInt channel;

		/// 报文本通道ID
		UInt localID;

		/// 默认构造函数
		BusMessageInfo() : channel(0), localID(0)
		{}

		/// 转换为报文信息标识符，高32位为软件通道，低32位为本通道ID
		SPADAS_API ULong indicator();
	};

	/// 总线报文
	struct SessionBusMessage
	{
		/// 时间戳
		ShortTimestamp timestamp;

		/// 报文信息
		BusMessageInfo info;

		/// 数据
		Binary data;

		/// 默认构造函数
		SessionBusMessage()
		{}

		/// 基于信息、时间戳和数据初始化
		SessionBusMessage(ShortTimestamp timestamp, BusMessageInfo info, Binary data) : timestamp(timestamp), info(info), data(data)
		{}
	};

	/// 总线报文数据表，key为报文全局ID，格式为"协议文件名:本通道ID"，如vehicle.dbc:123
	typedef Dictionary<Array<SessionBusMessage> > SessionBusMessageTable;

	/// 视频数据流编码方式
	class SPADAS_API VideoDataCodec
	{
	public:
		enum class Value
		{
			Default = 0,

			/// 无效
			Invalid = 0,

			/// MJPEG：有损编码，帧间独立
			MJPEG = 1,

			/// H.264：有损编码，帧间依赖
			H264 = 2,

			/// YUV411：无损编码，帧间独立，格式为每8像素(U0 Y0 V0 Y1 U4 Y2 V4 Y3 Y4 Y5 Y6 Y7)，每数值8bit
			YUV411 = 3,

			/// YUV420：无损编码，帧间独立，格式为每2x2像素(U V Y00 Y01 Y10 Y11)，每数值8bit
			YUV420 = 4,

			/// H.265：有损编码，帧间依赖
			H265 = 5,

			/// YUV422：无损编码，帧间独立，格式为每2像素(Y0 U Y1 V)，每数值8bit
			YUV422 = 6,

			/// RAW：无损编码，帧间独立，格式为奇数行BG，偶数行GR，每数值8bit
			RAW = 7,

			/// RAW12：无损编码，帧间独立，格式为奇数行BG，偶数行GR，每数值12bit按小字序依次存储
			RAW12 = 8,

			/// RAW14：无损编码，帧间独立，格式为奇数行BG，偶数行GR，每数值14bit按小字序依次存储
			RAW14 = 9,

			/// RAW16：无损编码，帧间独立，格式为奇数行BG，偶数行GR，每数值16bit按大字序依次存储
			RAW16 = 10,

			/// Y16：无损编码，帧间独立，每数值16bit按大字序依次存储
			Y16 = 11,
		};
		static const Char* toString(Value val)
		{
			switch (val)
			{
				ES(Invalid);
				ES(MJPEG);
				ES(H264);
				ES(YUV411);
				ES(YUV420);
				ES(H265);
				ES(YUV422);
				ES(RAW);
				ES(RAW12);
				ES(RAW14);
				ES(RAW16);
				ES(Y16);
				default: return 0;
			}
		}
	};

	/// 视频数据流输入模式
	struct VideoInputMode
	{
		/// 图像大小
		Size2D size;

		/// 编码方式
		Enum<VideoDataCodec> codec;
	};

	/// 视频数据流回注模式
	struct VideoOutputMode
	{
		/// 图像大小
		Size2D size;

		/// 编码方式
		Enum<VideoDataCodec> codec;
	};

	/// 视频设备输出原始数据
	struct VideoDeviceData
	{
		/// 到达主机时的CPU计数
		ULong cpuTick;

		/// 采样时客机Posix时间，单位纳秒，0表示无效
		NanoPosix guestPosix;

		/// 采样时卫星Posix时间，单位纳秒，0表示无效
		NanoPosix gnssPosix;

		/// 视频通道，0~23，对应A~X
		UInt channel;

		/// 视频数据流输入模式
		VideoInputMode inputMode;

		/// 视频原始帧的数据
		Binary data;

		/// 可选的预览图像，640x(360-480)分辨率的BGR图像
		Optional<ImagePointer> preview;

		/// 默认构造函数
		VideoDeviceData() : cpuTick(0), guestPosix(0), gnssPosix(0), channel(0)
		{}
	};

	/// 视频原始数据
	struct SessionVideoRawData
	{
		/// 时间戳
		FullTimestamp timestamp;

		/// 视频通道，0~23，对应A~X
		UInt channel;

		/// 视频数据流输入模式
		VideoInputMode inputMode;

		/// 视频原始帧的数据
		Binary data;

		/// 可选的预览图像，640x(360-480)分辨率的BGR图像
		Optional<ImagePointer> preview;

		/// 默认构造函数
		SessionVideoRawData() : channel(0)
		{}
	};

	/// 视频原始数据表（长度24, 分别表示视频通道A~X）
	typedef Array<Array<SessionVideoRawData> > SessionVideoRawDataTable;

	/// 视频设备ID
	struct VideoDeviceID
	{
		/// 插件类型ID
		String type;

		/// 设备标识ID
		String localID;
	};

	/// 视频设备信息
	struct VideoDeviceInfo
	{
		/// 视频设备ID
		VideoDeviceID id;

		/// 视频设备通道描述
		String description;

		/// 该视频设备通道支持的视频数据流输入模式
		Array<VideoInputMode> inputModes;

		/// 该视频设备通道支持的视频数据流回注模式
		Array<VideoOutputMode> outputModes;

		/// 是否支持按授时服务器Posix时间预约发送视频帧
		Bool supportTransmitScheduled;

		/// 默认构造函数
		VideoDeviceInfo() : supportTransmitScheduled(FALSE)
		{}
	};

	/// 视频设备配置
	struct VideoDeviceConfig
	{
		/// 视频设备ID
		VideoDeviceID id;

		/// 软件通道映射，0~23，对应A~X
		UInt mapChannel;

		/// 该通道的视频数据流输入模式
		VideoInputMode inputMode;

		/// 该通道的视频数据流回注模式
		VideoOutputMode outputMode;

		/// 输入参考帧率，10~120Hz
		UInt frameRate;

		/// 默认构造函数
		VideoDeviceConfig() : mapChannel(0), frameRate(0)
		{}
	};

	/// 用于数据处理的视频数据
	struct SessionVideoProcData
	{
		/// 时间戳
		FullTimestamp timestamp;

		/// 软件通道，0~23，对应A~X
		UInt channel;

		/// 用于数据处理的图像数据
		ImagePointer data;

		/// 图像的meta协议
		String metaProtocol;

		/// 图像的meta信息
		Array<Double> meta;

		/// 默认构造函数
		SessionVideoProcData() : channel(0)
		{}
	};

	/// 用于数据处理的视频数据表（长度24, 分别表示视频通道A~X）
	typedef Array<Array<SessionVideoProcData> > SessionVideoProcDataTable;

	/// 视频预览图像的快速输出接口
	class SPADAS_API IVideoPreviewExpress
	{
	public:
		virtual ~IVideoPreviewExpress() {};

		/// @brief 输出预览图像
		/// @param cpuTick 视频帧的到达时CPU计数
		/// @param channel 视频通道，0~23，对应A~X
		/// @param preview 预览图像，640x(360-480)分辨率的BGR图像
		/// @param guestPosix 视频帧的采样时客机Posix时间，单位纳秒，0表示无效
		/// @param gnssPosix 视频帧的采样时卫星Posix时间，单位纳秒，0表示无效
		virtual void outputPreview(ULong cpuTick, UInt channel, ImagePointer preview, NanoPosix guestPosix = 0, NanoPosix gnssPosix = 0);
	};

	/// 视频帧回注接口
	class SPADAS_API IVideoFrameTransmitter
	{
	public:
		virtual ~IVideoFrameTransmitter() {};

		/// @brief 立即发送视频帧
		/// @param channel 视频通道，0~23
		/// @param codec 视频帧的编码格式
		/// @param size 视频帧的大小，像素单位
		/// @param data 视频帧数据
		/// @returns 是否成功发送，无效表示不确定
		virtual OptionalBool transmitNow(UInt channel, Enum<VideoDataCodec> codec, Size2D size, Binary data);

		/// @brief 按时间偏置预约发送视频帧（将优先按服务器Posix时间预约发送，不满足条件则按CPU计数时间发送）
		/// @param channel 视频通道，0~23
		/// @param codec 视频帧的编码格式
		/// @param size 视频帧的大小，像素单位
		/// @param data 视频帧数据
		/// @param offset 时间偏置，单位秒 (必须大于该通道上一帧预约发送报文的时间戳)
		/// @param tolerance 允许的最大延迟发送时间，单位纳秒
		/// @returns 是否成功预约发送，无效表示不确定
		virtual OptionalBool transmitAtTimeOffset(UInt channel, Enum<VideoDataCodec> codec, Size2D size, Binary data, Double offset, UInt tolerance);

		/// @brief 指定按服务器Posix时间预约发送视频帧 (必须大于该通道上一帧预约发送报文的时间)
		/// @param channel 视频通道，0~23
		/// @param codec 视频帧的编码格式
		/// @param size 视频帧的大小，像素单位
		/// @param data 视频帧数据
		/// @param serverPosix 授时服务器Posix时间，单位纳秒
		/// @param tolerance 允许的最大延迟发送时间，单位纳秒
		/// @returns 是否成功预约发送，无效表示不确定
		virtual OptionalBool transmitAtServerPosix(UInt channel, Enum<VideoDataCodec> codec, Size2D size, Binary data, NanoPosix serverPosix, UInt tolerance);
	};

	/// 所有输入数据表
	struct InputTables
	{
		/// 通用原始数据表
		SessionGeneralRawDataTable rawDatas;

		/// 总线原始数据表
		SessionBusRawDataTable busRawDatas;

		/// 视频原始数据表
		SessionVideoRawDataTable videoRawDatas;

		/// 用于数据处理的视频数据表
		SessionVideoProcDataTable videoProcDatas;

		/// 总线报文数据表
		SessionBusMessageTable busMessages;

		/// 信号数据表
		SessionSignalTable signals;

		/// 样本数据表
		SessionGeneralSampleTable samples;

		/// 矩阵数据表
		SessionMatrixSampleTable matrices;

		/// 默认构造函数
		InputTables() : busRawDatas(BC_NUM), videoRawDatas(VC_NUM), videoProcDatas(VC_NUM)
		{}

		/// 清空所有数据
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

	/// 所有输出数据表
	struct OutputTables
	{
		/// 信号数据表
		SessionSignalTable signals;

		/// 样本数据表
		SessionGeneralSampleTable samples;

		/// 矩阵数据表
		SessionMatrixSampleTable matrices;

		/// 默认构造函数
		OutputTables()
		{}

		/// 清空所有数据
		inline void clear()
		{
			signals.clear();
			samples.clear();
			matrices.clear();
		}
	};

	/// 独立处理任务的状态
	class SPADAS_API StandaloneTaskState
	{
	public:
		enum class Value
		{
			Default = 0,

			/// 任务还未开始
			Unknown = 0,

			/// 任务完成
			Finished = 1,

			/// 任务失败或被中止
			Failed = 2,

			/// 任务进行中
			Progressing = 3,
		};
		static const Char* toString(Value val)
		{
			switch (val)
			{
				ES(Unknown);
				ES(Finished);
				ES(Failed);
				ES(Progressing);
				default: return 0;
			}
		}
	};

	/// 独立处理任务的反馈接口
	class SPADAS_API IStandaloneTaskCallback
	{
	public:
		virtual ~IStandaloneTaskCallback() {};

		/// @brief 设置任务处理状态和进度
		/// @param state 任务状态
		/// @param description 描述完成状态、失败或中止状态、任务进行状态的文本，空表示不启用
		/// @param progress 任务进度，单位%，仅当状态为 spadas::StandaloneTaskState::Value::Progressing 时需要设置
		virtual void setTaskProgress(Enum<StandaloneTaskState> state, String description, Double progress = 0);

		/// <summary>
		///  @brief 设置任务返回值信息
		/// </summary>
		/// <param name="value">返回值信息</param>
		virtual void setTaskReturnValue(String value);
	};

	/// 数据截取任务参数
	struct PickConfig
	{
		/// 截取源Session的时间偏置范围
		Range srcRange;

		/// 目标Session的标识符
		SessionIdentifier dstSession;

		/// 目标Session的input文件夹路径
		Path dstInputRoot;

		/// 目标Session的generation文件夹路径
		Path dstGenerationRoot;
	};

	/// 文件写入模式
	class SPADAS_API FileWriteMode
	{
	public:
		enum class Value
		{
			Default = 0,

			/// 无特殊模式
			Normal = 0,

			/// 采集模式
			OnlineMode = 1,

			/// 离线处理模式
			OfflineMode = 2,
		};
		static const Char* toString(Value val)
		{
			switch (val)
			{
				ES(Normal);
				ES(OnlineMode);
				ES(OfflineMode);
				default: return 0;
			}
		}
	};

	/// 文件读取模式
	class SPADAS_API FileReadMode
	{
	public:
		enum class Value
		{
			Default = 0,

			/// 无特殊模式
			Normal = 0,

			/// 离线处理或离线回放模式下，从原始数据读取
			FromRaw = 1,

			/// 离线处理模式下，从generation读取
			OfflineModeFromGeneration = 2,

			/// 离线回放模式下，从generation读取
			ReplayModeFromGeneration = 3,

			/// 远程采集模式下，从远程主机读取
			FromRemote = 4,
		};
		static const Char* toString(Value val)
		{
			switch (val)
			{
				ES(Normal);
				ES(FromRaw);
				ES(OfflineModeFromGeneration);
				ES(ReplayModeFromGeneration);
				ES(FromRemote);
				default: return 0;
			}
		}
	};

	/// 文件读写筛选
	class SPADAS_API FileIOFilter
	{
	public:
		enum class Value
		{
			Default = 0,

			/// 无效值
			Invalid = 0,

			/// 信号数据
			Signal = 1,

			/// 样本数据
			Sample = 2,

			/// 矩阵数据
			Matrix = 3,

			/// 总线通道1~16数据
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

			/// 视频通道A~X数据
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
		static const Char* toString(Value val)
		{
			switch (val)
			{
				ES(Invalid);
				ES(Signal);
				ES(Sample);
				ES(Matrix);
				case (Value)100: return "BusCH1";
				case (Value)101: return "BusCH2";
				case (Value)102: return "BusCH3";
				case (Value)103: return "BusCH4";
				case (Value)104: return "BusCH5";
				case (Value)105: return "BusCH6";
				case (Value)106: return "BusCH7";
				case (Value)107: return "BusCH8";
				case (Value)108: return "BusCH9";
				case (Value)109: return "BusCH10";
				case (Value)110: return "BusCH11";
				case (Value)111: return "BusCH12";
				case (Value)112: return "BusCH13";
				case (Value)113: return "BusCH14";
				case (Value)114: return "BusCH15";
				case (Value)115: return "BusCH16";
				case (Value)200: return "VideoChannelA";
				case (Value)201: return "VideoChannelB";
				case (Value)202: return "VideoChannelC";
				case (Value)203: return "VideoChannelD";
				case (Value)204: return "VideoChannelE";
				case (Value)205: return "VideoChannelF";
				case (Value)206: return "VideoChannelG";
				case (Value)207: return "VideoChannelH";
				case (Value)208: return "VideoChannelI";
				case (Value)209: return "VideoChannelJ";
				case (Value)210: return "VideoChannelK";
				case (Value)211: return "VideoChannelL";
				case (Value)212: return "VideoChannelM";
				case (Value)213: return "VideoChannelN";
				case (Value)214: return "VideoChannelO";
				case (Value)215: return "VideoChannelP";
				case (Value)216: return "VideoChannelQ";
				case (Value)217: return "VideoChannelR";
				case (Value)218: return "VideoChannelS";
				case (Value)219: return "VideoChannelT";
				case (Value)220: return "VideoChannelU";
				case (Value)221: return "VideoChannelV";
				case (Value)222: return "VideoChannelW";
				case (Value)223: return "VideoChannelX";
				default: return 0;
			}
		}
	};

	/// 文件读写基本信息
	struct FileIOBasicInfo
	{
		/// Session标识符
		SessionIdentifier session;

		/// 筛选项，空表示不进行筛选
		Array<Enum<FileIOFilter> > filter;

		/// 文件写入模式
		Enum<FileWriteMode> writeMode;

		/// 文件读取模式
		Enum<FileReadMode> readMode;

		/// 数据密码（非加密文件无需使用）
		String password;
	};

	/// 文件读写扩展信息
	struct FileIOExtInfo
	{
		/// 各总线通道类型，空表示无总线数据读写
		Array<Enum<BusChannelType> > busChannelTypes;

		/// 总线报文名称表，key为总线报文ID字符串
		Dictionary<String> busMessageNameTable;

		/// 各视频通道输入模式，空表示无视频数据读写
		Array<VideoInputMode> videoChannelModes;

		/// 各视频通道写入文件的帧率，空表示不考虑帧率
		Array<UInt> videoChannelFPS;

		/// 各视频通道是否按写入文件帧率对齐实际帧率（仅用于写入），空表示不对齐
		Array<Bool> videoChannelFPSAligned;

		/// 样本标题表，key为样本协议ID
		Dictionary<String> sampleTitles;
	};

	/// 跨模块数据发送接口
	class SPADAS_API ICrossTransmitter
	{
	public:
		virtual ~ICrossTransmitter() {}

		/// @brief 发送至应用层模块
		/// @param id 数据ID
		/// @param data 数据内容，可为空
		virtual void sendToApp(String id, Binary data);

		/// @brief 发送至原生层模块
		/// @param pluginType 模块的插件类型ID
		/// @param id 数据ID
		/// @param data 数据内容，可为空
		/// @returns 若未找到符合指定插件类型ID的模块则返回FALSE
		virtual Bool sendToNative(String pluginType, String id, Binary data);
	};

	/// 跨模块函数调用接口
	class SPADAS_API ICrossCaller
	{
	public:
		virtual ~ICrossCaller() {}

		/// @brief 调用应用层函数
		/// @param id 调用ID，可用于区分不同功能或函数
		/// @param input 输入数据
		/// @param output 输出数据
		/// @returns 若未被应用层程序响应则返回FALSE
		virtual Bool callAppFunction(String id, Binary input, Binary& output);

		/// @brief 调用原生层函数
		/// @param pluginType 模块的插件类型ID
		/// @param id 调用ID，可用于区分不同功能或函数
		/// @param context 调用上下文，存储输入输出和临时变量等
		/// @returns 若未找到符合制定插件类型ID的模块，或未被任何模块响应则返回FALSE
		virtual Bool callNativeFunction(String pluginType, String id, BaseObject context);
	};

	/// 时间相关服务接口
	class SPADAS_API ITimeServer
	{
	public:
		virtual ~ITimeServer() {}

		/// @brief 创建时间戳（计算时间偏置的优先级从高至低为：授时服务器Posix时间、卫星Posix时间、到达时主机Posix时间、到达时CPU计数）
		/// @param outputTimestamp 输出的时间戳
		/// @param session 时间戳所在Session的标识符
		/// @param cpuTick 到达时CPU计数，0表示无效
		/// @param hostPosix 到达时主机Posix时间，单位纳秒，0表示无效
		/// @param guestPosix 客机Posix时间，单位纳秒，0表示无效
		/// @param gnssPosix 卫星Posix时间，单位纳秒，0表示无效
		/// @param guestSyncID 客机同步ID，将据此确定客机是否已与授时服务器同步（视频通道格式为video@x，x为通道序号0~23；插件相关通道格式为"xxx.yyy"，xxx为插件类型ID，yyy为客机同步通道名称）
		/// @return 是否成功
		virtual Bool createTimestamp(FullTimestamp& outputTimestamp, SessionIdentifier session, ULong cpuTick = 0, NanoPosix hostPosix = 0, NanoPosix guestPosix = 0, NanoPosix gnssPosix = 0, String guestSyncID = String());

		/// @brief 根据基准时间戳进行二次同步（重新计算时间偏置的优先级从高至低为：授时服务器Posix时间、卫星Posix时间）
		/// @param srcTimestamp 基准时间戳
		/// @param guestPosix 非0则使用该输入作为基准时间戳的客机Posix时间，单位纳秒
		/// @param gnssPosix 非0则使用该输入作为基准时间戳的卫星Posix时间，单位纳秒
		/// @param guestSyncID 客机同步ID，将据此确定客机是否已与授时服务器同步（视频通道格式为video@x，x为通道序号0~23；插件相关通道格式为"xxx.yyy"，xxx为插件类型ID，yyy为客机同步通道名称）
		/// @return 输出的时间戳
		virtual FullTimestamp resyncTimestamp(FullTimestamp srcTimestamp, NanoPosix guestPosix = 0, NanoPosix gnssPosix = 0, String guestSyncID = String());

		/// @brief 根据基准时间戳的时间偏置反算CPU计数、主机Posix时间、授时服务器Posix时间、卫星Posix时间等
		/// @param srcTimestamp 基准时间戳
		/// @param timeType 输出的时间类型，不支持 spadas::TimeType::Value::GuestPosix
		/// @return 输出的Session无关时间，0表示失败
		virtual ULong calcTime(ShortTimestamp srcTimestamp, Enum<TimeType> timeType);
	};

	// 插件相关实用功能 //////////////////////////////////////////////////////////////

	/// 总线协议ID (形如XXX.dbc)
	typedef String BusProtocolID;

	/// 总线报文ID
	struct BusMessageID
	{
		/// 总线协议ID
		BusProtocolID protocol;

		/// 本通道报文ID
		UInt message;

		/// 总线报文ID字符串
		String text;

		/// 默认构造函数
		SPADAS_API BusMessageID();

		/// 按总线协议ID和本通道报文ID初始化
		SPADAS_API BusMessageID(BusProtocolID protocol, UInt message);

		/// 按总线报文ID字符串初始化
		SPADAS_API BusMessageID(String text);
	};

	/// 总线信号ID
	struct BusSignalID
	{
		/// 总线协议ID
		BusProtocolID protocol;

		/// 本通道报文ID
		UInt message;

		/// 信号名称
		String signal;

		/// 总线信号ID字符串
		String text;

		/// 默认构造函数
		SPADAS_API BusSignalID();

		/// 按总线协议ID，本通道报文ID，以及信号名称初始化
		SPADAS_API BusSignalID(BusProtocolID protocol, UInt message, String signal);

		/// 按总线信号ID字符串初始化
		SPADAS_API BusSignalID(String text);
	};

	/// 带符号位信号配置
	struct BusSignedSignalConfig
	{
		/// 数值信号ID字符串
		String valueSignalID;

		/// 符号位信号ID字符串
		String signSignalID;

		/// 符号位信号值为0时是否表示正值
		Bool positiveAsZero;

		/// 默认构造函数
		SPADAS_API BusSignedSignalConfig();

		/// 符号位信号配置是否有效
		SPADAS_API Bool isSignOptional();
	};

	/// 带符号OptionalDouble
	struct OptionalSignedDouble
	{
		/// 符号位
		Bool positive;

		/// 正浮点值（含0）
		Double value;

		/// 符号位是否有效
		Bool signValid;

		/// 正浮点值是否有效
		Bool valueValid;

		/// 默认构造函数，初始化为无效值
		OptionalSignedDouble() : positive(TRUE), value(0), signValid(FALSE), valueValid(FALSE)
		{}

		/// 设置正浮点值
		SPADAS_API void setValue(Double value);

		/// 设置符号位
		SPADAS_API void setSign(Bool positive);

		/// @brief 转为 spadas::OptionalDouble 类型
		/// @param signOptional 符号位是否可选，若为FALSE则符号位必需有效才能返回有效值
		/// @returns 返回的 spadas::OptionalDouble 类型数值
		SPADAS_API OptionalDouble toOptionalDouble(Bool signOptional);
	};

	/// 通用原始数据输出接口
	class SPADAS_API IGeneralDeviceDataOutput
	{
	public:
		virtual ~IGeneralDeviceDataOutput() {};

		/// @brief 输出一般设备原始数据
		/// @param data 一般设备原始数据
		virtual void outputGeneralDeviceData(GeneralDeviceData data);
	};

	/// 时间戳搜索接口
	class SPADAS_API ITimestampSearch
	{
	public:
		virtual ~ITimestampSearch() {};

		/// @brief 以文件当前位置搜索下一帧数据
		/// @param file 数据文件对象
		/// @param time 输出下一帧数据的时间戳
		/// @param pos 输出下一帧数据的开始位置
		/// @returns 搜索是否成功，若无数据返回FALSE
		virtual Bool readNextTimestamp(File file, Double& time, ULong& pos);

		/// [可选] 获取尾部搜索范围，默认为Range(100, 10000)
		virtual Range getTailSearchRange();
	};

	/// 时间戳搜索
	class SPADAS_API TimestampSearch
	{
	public:
		/// @brief 按时间戳搜索数据文件，跳转文件位置，使得下一帧数据为时间戳不小于指定时间戳的最早帧
		/// @param path 数据文件路径
		/// @param time 目标时间戳
		/// @param searcher 搜索接口
		/// @returns 数据文件对象，若无数据或所有数据的时间戳都小于目标则返回无效对象
		static File search(Path path, Double time, Interface<ITimestampSearch> searcher);

		/// @brief 获取数据文件中的时间戳范围
		/// @param path 数据文件路径
		/// @param searcher 搜索接口
		/// @returns 时间戳范围
		static Optional<Range> getTimeRange(Path path, Interface<ITimestampSearch> searcher);
	};

	/// 用于在 spadas::FlexVars 中构建和析构数据的接口
	class SPADAS_API IFlexHandler
	{
	public:
		virtual ~IFlexHandler() {};
		virtual void createData(Pointer data);
		virtual void destroyData(Pointer data);
	};

	/// 支持在多个模块中使用不同定义的变量数据基类
	template <typename Type> class FlexVars : public Vars
	{
	public:
		/// 初始化，输入当前定义下支持字段的序号列表（从0开始）
		FlexVars(Array<Int> validFlagIndices);

		/// 析构函数(子类无需实现析构函数)
		~FlexVars();

		/// 是否有效
		Bool valid();

		/// 此数据是否支持指定序号的字段
		Bool has(Int flagIndex);

		/// [非安全] 获取当前定义下的数据类型引用（使用字段前应先调用has方法确定是否支持）
		Type& cast();

	private:
		Binary data;
		Array<ULong> flags;
		Interface<IFlexHandler> handler;
	};

	/// 通用的函数IO用对象
	class SPADAS_API GeneralIOObject : public Object<class GeneralIOObjectVars>
	{
	public:
		/// 类名称
		static const String TypeName;

		/// 创建对象
		GeneralIOObject();

		/// 获取输入数据
		Binary getInput();

		/// 设置输入数据
		void setInput(Binary data);

		/// 获取输出数据
		Binary getOutput();

		/// 设置输出数据
		void setOutput(Binary data);

	private:
		Bool isNull() { return FALSE; }
		Bool isValid() { return FALSE; }
	};

	// 插件API //////////////////////////////////////////////////////////////

	/// 通用功能插件接口 2.0
	class SPADAS_API IPluginV200
	{
	public:
		virtual ~IPluginV200() {};

		/// @brief 获取插件类型ID
		/// @returns 插件类型ID
		virtual String getPluginType();

		/// @brief 获取插件版本
		/// @returns 插件版本，格式为x.x.x，分别表示主版本、次版本、build版本。一般而言主版本对应架构更新，次版本对应功能更新，build版本对应bug修复
		virtual String getPluginVersion();

		/// [可选] 在程序结束前被调用，用于停止背景线程等
		virtual void closePlugin();

		/// @brief [可选] 在初始化语言时被调用
		/// @param languageCode 语言代号，"en"表示英文，"ch"表示中文
		virtual void initLanguage(String languageCode);

		/// @brief [可选] 设置使用指定的接口记录调试信息（主要用于在子线程中调用 spadas::console::useThreadLogger ）
		/// @param logger 记录调试信息接口
		virtual void useLogger(Interface<ILogger> logger);

		/// @brief [可选] 设置应用程序外部文件(如插件库文件等在plugin子文件夹)存放的根路径
		/// @param path 应用程序外部文件存放的根路径
		virtual void setAppFilesPath(Path path);

		/// @brief [可选] 在收到其他模块发送的数据时被调用（应确保微秒级别的运行时间）
		/// @param id 数据ID
		/// @param data 数据内容，可为空
		virtual void onCrossData(String id, Binary data);

		/// @brief [可选] 设置使用指定的跨模块数据发送接口
		/// @param transmitter 跨模块数据发送接口
		virtual void useCrossTransmitter(Interface<ICrossTransmitter> transmitter);

		/// @brief [可选] 在收到其他模块调用函数请求时被调用（应确保毫秒级别的运行时间，更长的处理应通过独立任务实现）
		/// @param id 调用ID，可用于区分不同功能或函数
		/// @param context 调用上下文，存储输入输出和临时变量等
		/// @returns 是否已响应
		virtual Bool onCrossCall(String id, BaseObject context);

		/// @brief [可选] 设置使用指定的跨模块函数调用接口
		/// @param caller 跨模块函数调用接口
		virtual void useCrossCaller(Interface<ICrossCaller> caller);

		/// @brief [可选] 在开始在线模式Session时被调用
		/// @param session 该在线模式Session的标识符
		/// @param startCPUTick 该在线模式Session启动时的基准CPU计数
		virtual void onStartOnlineSession(SessionIdentifier session, ULong startCPUTick);

		/// [可选] 在结束在线模式Session时被调用
		virtual void onStopOnlineSession();

		/// @brief [可选] 获取支持的独立任务名称列表
		/// @return 支持的独立任务名称列表
		virtual Array<String> getStandaloneTaskNames();

		/// @brief [可选] 启动独立的处理任务（一般仅在非Session时段被调用）
		/// @param taskName 任务名称，用于区分不同任务
		/// @param config 该任务的配置信息
		/// @param shouldEnd 任务是否应该中止，一般来自用户操作
		/// @param callback 用于反馈任务处理状态、进度，以及返回值的接口
		virtual void runStandaloneTask(String taskName, String config, Flag shouldEnd, Interface<IStandaloneTaskCallback> callback);

		/// @brief [可选] 获取客机同步通道名称列表
		/// @return 客机同步通道名称列表
		virtual Array<String> getGuestSyncChannelNames();
	};

	/// 获取通用功能插件接口的全局函数定义，函数名应为get_plugin_v200
	typedef Interface<IPluginV200>(*GetPluginV200)();

	/// 一般设备插件接口 3.0
	class SPADAS_API IDevicePluginV300
	{
	public:
		virtual ~IDevicePluginV300() {};

		/// @brief 配置设备，在函数内部根据配置实现连接、断开、或重连（仅在非Session时段被调用）
		/// @param config 配置信息，应包含是否连接设备的配置
		virtual void setDeviceConnection(String config);

		/// 断开连接（在程序关闭前被调用）
		virtual void disconnectDevice();

		/// @brief 获取设备连接状态
		/// @param info 输出设备连接信息
		/// @returns 返回设备状态信息
		virtual Enum<GeneralDeviceStatus> getDeviceStatus(String& info);

		/// @brief [可选] 获取各子设备的连接状态
		/// @returns 返回各子设备的连接状态，数组长度即子设备数量
		virtual Array<Enum<GeneralDeviceStatus> > getChildDeviceStatus();

		/// @brief [可选] 获取设备新数据
		/// @returns 返回新获取的数据
		virtual Array<GeneralDeviceData> getDeviceNewData();

		/// @brief [可选] 获取可立即发送的数据协议列表
		/// @return 可发送的数据协议列表
		virtual Array<String> getTransmittableProtocols();

		/// @brief [可选] 获取可预约发送的数据协议列表
		/// @return 可发送的数据协议列表
		virtual Array<String> getScheduledTransmittableProtocols();

		/// @brief [可选] 立即发送一帧数据
		/// @param protocol 原始数据协议ID，一般格式为"xxx-v?"，xxx表示数据来源，v?表示版本
		/// @param vector 数值数组数据
		/// @param binary 二进制数据
		/// @returns 返回是否成功发送一帧数据，若协议未在可发送的协议列表内则返回FALSE
		virtual Bool transmitGeneralData(String protocol, Array<Double> vector, Binary binary);

		/// @brief [可选] 预约发送一帧数据（相同协议的预约发送时间已确保递增）
		/// @param protocol 原始数据协议ID，一般格式为"xxx-v?"，xxx表示数据来源，v?表示版本
		/// @param vector 数值数组数据
		/// @param binary 二进制数据
		/// @param serverPosix 预约发送的授时服务器Posix时间，单位纳秒
		/// @param tolerance 允许的最大延迟发送时间，单位纳秒
		/// @returns 返回是否成功预约发送一帧数据，若协议未在可发送的协议列表内则返回FALSE
		virtual Bool transmitGeneralDataScheduled(String protocol, Array<Double> vector, Binary binary, NanoPosix serverPosix, UInt tolerance);
	};

	/// 获取一般设备插件接口的全局函数定义，函数名应为get_device_plugin_v300
	typedef Interface<IDevicePluginV300>(*GetDevicePluginV300)();

	/// 总线设备插件接口 3.0
	class SPADAS_API IBusPluginV300
	{
	public:
		virtual ~IBusPluginV300() {};

		/// @brief 获取总线设备列表
		/// @returns 返回总线设备列表
		virtual Array<BusDeviceInfo> getBusDeviceList();

		/// @brief 打开总线设备（在开始Session时被调用）
		/// @param configs 希望打开的总线设备通道列表及相关配置
		virtual Bool openBusDevice(Array<BusDeviceConfig> configs);

		/// 关闭总线设备（在结束Session时被调用）
		virtual void closeBusDevice();

		/// @brief [可选] 获取一帧数据
		/// @param rxData 输出一帧数据（若有）
		/// @returns 返回是否成功获取一帧数据
		virtual Bool receiveBusMessage(BusDeviceData& rxData);

		/// @brief [可选] 立即发送一帧数据
		/// @param channel 总线通道，1~16
		/// @param id 该通道内的报文ID
		/// @param binary 报文数据
		/// @returns 返回是否成功发送一帧数据
		virtual Bool transmitBusMessage(UInt channel, UInt id, Binary binary);

		/// @brief [可选] 预约发送一帧数据（相同通道的预约发送时间已确保递增）
		/// @param channel 总线通道，1~16
		/// @param id 该通道内的报文ID
		/// @param binary 报文数据
		/// @param serverPosix 预约发送的授时服务器Posix时间，单位纳秒
		/// @param tolerance 允许的最大延迟发送时间，单位纳秒
		/// @returns 返回是否成功预约发送一帧数据
		virtual Bool transmitBusMessageScheduled(UInt channel, UInt id, Binary binary, NanoPosix serverPosix, UInt tolerance);

		/// @brief [可选] 对总线设备进行额外设置（在openBusDevice前被调用）
		/// @param extra 配置信息
		virtual void setBusExtraConfig(String extra);

		/// @brief [可选] 获取各总线通道的数据负载情况
		/// @returns 返回各总线通道的数据负载百分比
		virtual Array<BusChannelPayload> getBusPayload();
	};

	/// 获取总线设备插件接口的全局函数定义，函数名应为get_bus_plugin_v300
	typedef Interface<IBusPluginV300>(*GetBusPluginV300)();

	/// 视频设备插件接口 5.0
	class SPADAS_API IVideoPluginV500
	{
	public:
		virtual ~IVideoPluginV500() {};

		/// @brief 获取视频设备列表
		/// @returns 视频设备列表
		virtual Array<VideoDeviceInfo> getVideoDeviceList();

		/// @brief 打开视频设备（在开始Session时被调用）
		/// @param configs 希望打开的视频设备通道列表及相关配置
		virtual Bool openVideoDevice(Array<VideoDeviceConfig> configs);

		/// 关闭总线设备（在结束Session时被调用）
		virtual void closeVideoDevice();

		/// @brief [可选] 获取一帧数据
		/// @param frame 输出一帧数据（若有）
		/// @returns 返回是否成功输出一帧数据
		virtual Bool queryVideoFrame(VideoDeviceData& frame);

		/// @brief [可选] 立即发送一帧数据
		/// @param channel 视频通道，0~23，对应A~X
		/// @param codec 视频帧编码方式
		/// @param size 视频帧大小，像素单位
		/// @param data 视频帧数据
		/// @returns 返回是否成功发送一帧数据
		virtual Bool transmitVideoFrame(UInt channel, Enum<VideoDataCodec> codec, Size2D size, Binary data);

		/// @brief [可选] 预约发送一帧数据（相同通道的预约发送时间已确保递增）
		/// @param channel 视频通道，0~23，对应A~X
		/// @param codec 视频帧编码方式
		/// @param size 视频帧大小，像素单位
		/// @param data 视频帧数据
		/// @param serverPosix 预约发送的授时服务器Posix时间，单位纳秒
		/// @param tolerance 允许的最大延迟发送时间，单位纳秒
		/// @returns 返回是否成功预约发送一帧数据
		virtual Bool transmitVideoFrameScheduled(UInt channel, Enum<VideoDataCodec> codec, Size2D size, Binary data, NanoPosix serverPosix, UInt tolerance);

		/// @brief [可选] 对视频设备进行额外设置（在openVideoDevice前被调用）
		/// @param extra 配置信息
		virtual void setVideoExtraConfig(String extra);

		/// @brief [可选] 获取视频设备新原始数据
		/// @returns 返回新获取的数据
		virtual Array<GeneralDeviceData> getVideoDeviceNewData();

		/// @brief [可选] 设置使用指定的视频预览图像的快速输出接口
		/// @param previewExpress 视频预览图像的快速输出接口
		virtual void useVideoPreviewExpress(Interface<IVideoPreviewExpress> previewExpress);

		/// @brief [可选] 获取视频设备独占关键字，其他插件匹配此关键字的视频设备将被禁用
		/// @returns 返回视频设备独占关键字
		virtual Array<String> getExclusiveKeywords();
	};

	/// 获取视频设备插件接口的全局函数定义，函数名应为get_video_plugin_v500
	typedef Interface<IVideoPluginV500>(*GetVideoPluginV500)();

	/// 原生数据处理插件接口 7.0
	class SPADAS_API IProcessorPluginV700
	{
	public:
		virtual ~IProcessorPluginV700() {};

		/// [可选] 是否为在线限定的数据处理（默认为FALSE）
		virtual Bool isProcessorOnlineOnly();

		/// [可选] 是否为离线限定的数据处理（默认为FALSE）
		virtual Bool isProcessorOfflineOnly();

		/// @brief 配置数据处理及杂项数据路径表（在开始Session时被调用）
		/// @param config 配置信息，应包含是否启用功能的配置
		/// @param onlineMode 是否为在线模式，否则为离线模式或回放模式
		virtual void setProcessorConfig(String config, Bool onlineMode);

		/// 禁用数据处理功能（在结束Session时被调用）
		virtual void disableProcessor();

		/// @brief 处理数据，函数内部应该只对输出数据表作更改
		/// @param inputs 输入数据表
		/// @param sampleBuffers 样本缓存表
		/// @param outputs 输出数据表
		virtual void processData(InputTables inputs, SessionSampleBufferTable sampleBuffers, OutputTables outputs);

		/// @brief [可选] 设置使用指定的时间相关服务接口
		/// @param timeServer 时间相关服务接口
		virtual void useTimeServer(Interface<ITimeServer> timeServer);

		/// @brief [可选] 设置使用指定的通用原始数据发送接口
		/// @param generalTransmitter 通用原始数据发送接口
		virtual void useGeneralTransmitter(Interface<IGeneralDataTransmitter> generalTransmitter);

		/// @brief [可选] 设置使用指定的总线报文发送接口
		/// @param busTransmitter 总线报文发送接口
		virtual void useBusTransmitter(Interface<IBusMessageTransmitter> busTransmitter);

		/// @brief [可选] 设置使用指定的视频帧回注接口
		/// @param videoTransmitter 视频帧回注接口
		virtual void useVideoTransmitter(Interface<IVideoFrameTransmitter> videoTransmitter);
	};

	/// 获取原生数据处理插件接口的全局函数定义，函数名应为get_processor_plugin_v700
	typedef Interface<IProcessorPluginV700>(*GetProcessorPluginV700)();

	/// 文件读写插件接口 2.0
	class SPADAS_API IFilePluginV200
	{
	public:
		virtual ~IFilePluginV200() {};

		/// @brief [可选] 获取适用于指定读取器的所有文件的最大时长
		/// @param readerName 读取器名称
		/// @param inputRoot Session的input文件夹路径
		/// @param subInputRoots Session的input子文件夹路径
		/// @param generationRoots Session的所有generation文件夹路径
		/// @param basicInfo 文件读写基本信息
		/// @param timeServer 时间相关服务接口（仅限当前读取Session）
		/// @returns 所有文件的最大时长，单位秒，若无文件或无数据则返回0
		virtual Double getFilesDuration(String readerName, Path inputRoot, Array<Path> subInputRoots, Array<Path> generationRoots, FileIOBasicInfo basicInfo, Interface<ITimeServer> timeServer);

		/// @brief [可选] 初始化读取原始数据文件
		/// @param readerName 读取器名称
		/// @param inputRoot Session的input文件夹路径
		/// @param subInputRoots Session的input子文件夹路径
		/// @param generationRoot Generation的文件夹路径
		/// @param jumpOffset 跳转至该时间偏置开始读取
		/// @param basicInfo 文件读写基本信息
		/// @param extInfo 输出文件扩展信息
		/// @param timeServer 时间相关服务接口（仅限当前读取Session）
		/// @returns 返回是否成功初始化，无数据文件的情况也返回FALSE
		virtual Bool openReadFiles(String readerName, Path inputRoot, Array<Path> subInputRoots, Path generationRoot, Double jumpOffset, FileIOBasicInfo basicInfo, FileIOExtInfo& extInfo, Interface<ITimeServer> timeServer);

		/// @brief [可选] 读取文件数据
		/// @param readerName 读取器名称
		/// @param inputs 输入数据表，读取的数据写入该表（其中视频首帧图像的所有依赖帧时间戳为0）
		/// @param targetOffset 读取的目标时间偏置，单位秒
		/// @param shouldEnd 是否准备关闭
		/// @returns 后续是否还有数据，若所有文件已读取至末尾则返回FALSE
		virtual Bool readFilesData(String readerName, InputTables inputs, Double targetOffset, Flag shouldEnd);

		/// @brief [可选] 关闭读取文件
		/// @param readerName 读取器名称
		virtual void closeReadFiles(String readerName);

		/// @brief [可选] 初始化写入数据文件
		/// @param writerName 写入器名称
		/// @param inputRoot Session的input文件夹路径
		/// @param subInputRoots Session的input子文件夹路径
		/// @param generationRoot Generation的文件夹路径
		/// @param basicInfo 文件读写基本信息
		/// @param extInfo 文件扩展信息
		/// @param timeServer 时间相关服务接口（仅限当前写入Session）
		/// @returns 返回是否成功初始化
		virtual Bool openWriteFiles(String writerName, Path inputRoot, Array<Path> subInputRoots, Path generationRoot, FileIOBasicInfo basicInfo, FileIOExtInfo extInfo, Interface<ITimeServer> timeServer);

		/// @brief [可选] 写入文件数据
		/// @param writerName 写入器名称
		/// @param inputs 输入数据表，从表中获取数据写入文件（其中视频首帧图像的所有依赖帧时间戳为0）
		/// @param busMessages 按时间戳排序的所有通道总线数据
		/// @param shouldEnd 是否准备关闭
		virtual void writeFilesData(String writerName, InputTables inputs, Array<SessionBusRawData> busMessages, Flag shouldEnd);

		/// @brief [可选] 关闭写入文件
		/// @param writerName 写入器名称
		virtual void closeWriteFiles(String writerName);

		/// @brief [可选] 获取是否有适用于数据截取器的数据文件
		/// @param pickerName 数据截取器名称
		/// @param inputRoot 源Session的input文件夹
		/// @param subInputRoots 源Session的input子文件夹路径
		/// @param generationRoot 源Session的generation文件夹
		/// @param basicInfo 文件读写基本信息
		/// @returns 是否有适用于数据截取器的数据文件
		virtual Bool hasDataFiles(String pickerName, Path inputRoot, Array<Path> subInputRoots, Path generationRoot, FileIOBasicInfo basicInfo);

		/// @brief [可选] 离线截取数据，在数据截取独立任务中被调用
		/// @param pickerName 数据截取器名称
		/// @param inputRoot 源Session的input文件夹
		/// @param subInputRoots 源Session的input子文件夹路径
		/// @param generationRoot 源Session的generation文件夹
		/// @param pick 截取任务参数
		/// @param basicInfo 文件读写基本信息
		/// @param shouldEnd 是否已被取消
		/// @param callback 任务的反馈接口，主要用于通知任务进度
		/// @param timeServer 时间相关服务接口（仅限当前源Session和目标Session）
		virtual void pickSession(String pickerName, Path inputRoot, Array<Path> subInputRoots, Path generationRoot, PickConfig pick, FileIOBasicInfo basicInfo, Flag shouldEnd, Interface<IStandaloneTaskCallback> callback, Interface<ITimeServer> timeServer);

		/// @brief [可选] 对文件读写进行额外设置（在各openXXXFiles函数前被调用）
		/// @param extra 配置信息
		virtual void setFileExtraConfig(String extra);
	};

	/// 获取文件读写插件接口的全局函数定义，函数名应为get_file_plugin_v200
	typedef Interface<IFilePluginV200>(*GetFilePluginV200)();
}

#endif
