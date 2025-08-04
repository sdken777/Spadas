
#ifndef SPADAS_TEMPLATE_H
#define SPADAS_TEMPLATE_H

namespace spadas
{
	// Constant / 相关常数 ///////////////////////////////////////////////////////////
	const UInt ARRAY_SIZE_LIMIT = 1000000000;		// Billion / 十亿
	const UInt ARRAYX_SIZE_LIMIT = 134217728;		// More than 100 millions / 一亿多 (2^27)
	const UInt ARRAYX_SEGMENT_LIMIT = 65536;
	const UInt MAP_INDEX_LIMIT = 65536;
	const ULong MATRIX_MAX_NELEMS = 100000000;		// 100 millions / 一亿

	// Implementation of reference count / 引用计数实现 ///////////////////////////////////////////////////////////
	template <typename VarsType>
	Object<VarsType>::Object() : vars(0)
	{}

	template <typename VarsType>
	Object<VarsType>::Object(VarsType *initVars, Bool isNew) : vars(initVars)
	{
		if (!isNew && vars) ((Vars*)vars)->retain();
	}

	template <typename VarsType>
	Object<VarsType>::~Object()
	{
		if (vars) ((Vars*)vars)->release();
	}

	template <typename VarsType>
	Object<VarsType>::Object(const Object<VarsType>& obj) : vars(obj.vars)
	{
		if (vars) ((Vars*)vars)->retain();
	}

	template <typename VarsType>
	Object<VarsType>& Object<VarsType>::operator =(const Object<VarsType>& obj)
	{
		if (obj.vars) ((Vars*)obj.vars)->retain();
		if (vars) ((Vars*)vars)->release();
		vars = obj.vars;
		return *this;
	}

	template <typename VarsType>
	Bool Object<VarsType>::operator ==(const Object<VarsType>& obj)
	{
		return vars == obj.vars;
	}

	template <typename VarsType>
	Bool Object<VarsType>::operator !=(const Object<VarsType>& obj)
	{
		return vars != obj.vars;
	}

	template <typename VarsType>
	Bool Object<VarsType>::isNull()
	{
		return vars == 0;
	}

	template <typename VarsType>
	Bool Object<VarsType>::isValid()
	{
		return vars != 0;
	}

	template <typename VarsType>
	Word Object<VarsType>::getHash()
	{
		return (Word)(((PointerInt)vars & 0x000ffff0) >> 4);
	}

	template <typename VarsType>
	ULong Object<VarsType>::getID() const
	{
		return (ULong)vars;
	}

	template <typename VarsType>
	UInt Object<VarsType>::getRefCount()
	{
		return vars ? ((Vars*)vars)->getRefCount() : 0;
	}

	template <typename VarsType>
	String Object<VarsType>::getTypeName()
	{
		return vars ? ((Vars*)vars)->getTypeName() : String();
	}

	template <typename VarsType>
	template <typename TargetType>
	Bool Object<VarsType>::is()
	{
		return !vars || ((Vars*)vars)->isType(TargetType::typeName().getID()) || ((Vars*)vars)->isType(TargetType::typeName());
	}

	template <typename VarsType>
	template <typename TargetType>
	TargetType Object<VarsType>::as()
	{
		Bool dummy;
		return as<TargetType>(dummy);
	}

	template <typename VarsType>
	template <typename TargetType>
	TargetType Object<VarsType>::as(Bool& ok)
	{
		if (!vars)
		{
			ok = TRUE;
			return TargetType();
		}
		else if (((Vars*)vars)->isType(TargetType::typeName().getID()) || ((Vars*)vars)->isType(TargetType::typeName()))
		{
			auto objBase = TargetType::castCreate((Vars*)vars);
			ok = TRUE;
			return *(TargetType*)(&objBase);
		}
		else
		{
			ok = FALSE;
			return TargetType();
		}
	}

	template <typename VarsType>
	VarsType *Object<VarsType>::getVars()
	{
		return vars;
	}

	template <typename VarsType>
	void Object<VarsType>::setVars(VarsType *targetVars, Bool isNew)
	{
		SPADAS_ERROR_RETURN(!targetVars);
		if (!isNew) ((Vars*)targetVars)->retain();
		if (vars) ((Vars*)vars)->release();
		vars = targetVars;
	}

	template <typename VarsType>
	Object<VarsType> Object<VarsType>::castCreate(Vars* varsToCast)
	{
		Object<VarsType> obj;
		obj.vars = (VarsType*)varsToCast;
		if (obj.vars) ((Vars*)obj.vars)->retain();
		return obj;
	}

	template <typename VarsType>
	String Object<VarsType>::toString()
	{
		return vars ? ((Vars*)vars)->toString() : String();
	}

	template <typename Type>
	Safe<Type>::Safe()
	{}

	template <typename Type>
	void Safe<Type>::operator =(Type target)
	{
		this->objSpinLock.casSpin(0, 1);
		this->obj = target;
		this->objSpinLock.decrease();
	}

	template <typename Type>
	template <typename VarsType>
	void Safe<Type>::setVars(VarsType* targetVars, Bool isNew)
	{
		this->objSpinLock.casSpin(0, 1);
		this->obj.setVars(targetVars, isNew);
		this->objSpinLock.decrease();
	}

	template <typename Type>
	Type Safe<Type>::get()
	{
		this->objSpinLock.casSpin(0, 1);
		Type output = this->obj;
		this->objSpinLock.decrease();
		return output;
	}

	template <typename Type>
	SpinLocked<Type>::SpinLocked(Safe<Type>& safeObj) : obj(safeObj.get())
	{
		Vars *localVars = (Vars*)this->obj.getVars();
		if (localVars && !localVars->isSpinLockManaged()) localVars->spinEnter();
	}

	template <typename Type>
	SpinLocked<Type>::~SpinLocked()
	{
		Vars *localVars = (Vars*)this->obj.getVars();
		if (localVars && !localVars->isSpinLockManaged()) localVars->spinLeave();
	}

	template <typename Type>
	Type *SpinLocked<Type>::operator ->()
	{
		return &this->obj;
	}

	template <typename Type> class InterfaceVars : public Vars
	{
	public:
		SPADAS_VARS(Interface<Type>, Vars)
		Type *implement;
		Vars *implementVars;
		InterfaceVars(Type *implement0, Vars *implementVars0) : implement(implement0), implementVars(implementVars0)
		{}
		~InterfaceVars()
		{
			delete implement;
		}
	};

	template <typename Type>
	String Interface<Type>::typeName() { static String name = (String)"spadas.Interface<" + typeid(Type).name() + ">"; return name; }

	template <typename Type>
	InterfaceVars<Type>* Interface<Type>::var() { return (InterfaceVars<Type>*)this->vars; }

	template <typename Type>
	Interface<Type>::Interface()
	{ }

	template <typename Type>
	template <typename ImplementType>
	Interface<Type>::Interface(ImplementType implementObj)
	{
		Vars *implementObjVars = (Vars*)implementObj.getVars();
		if (implementObjVars)
		{
			this->setVars(new InterfaceVars<Type>(new ImplementType(implementObj), implementObjVars), TRUE);
		}
	}

	template <typename Type>
	Bool Interface<Type>::operator ==(Interface<Type> target)
	{
		if (!this->vars && !target.vars) return TRUE;
		else if (!this->vars || !target.vars) return FALSE;
		else return this->var()->implementVars == target.var()->implementVars;
	}

	template <typename Type>
	Bool Interface<Type>::operator !=(Interface<Type> target)
	{
		return !(operator ==(target));
	}

	template <typename Type>
	Word Interface<Type>::getHash()
	{
		if (this->vars) return (Word)(((UInt)(PointerInt)this->var()->implementVars & 0x000ffff0) >> 4);
		else return 0;
	}

	template <typename Type>
	Type *Interface<Type>::operator ->()
	{
		SPADAS_ERROR_RETURNVAL(!this->vars, new Type());
		return this->var()->implement;
	}

	template <typename Type>
	BaseObject Interface<Type>::getObject()
	{
		BaseObject obj;
		if (!this->vars) return obj;
		obj.setVars(this->var()->implementVars, FALSE);
		return obj;
	}

	template <typename Type> class OptionalVars : public Vars
	{
	public:
		SPADAS_VARS(Optional<Type>, Vars)
		Type val;
		OptionalVars(Type& v) : val(v)
		{}
	};

	template <typename Type>
	String Optional<Type>::typeName() { static String name = (String)"spadas.Optional<" + typeid(Type).name() + ">"; return name; }

	template <typename Type>
	OptionalVars<Type>* Optional<Type>::var() { return (OptionalVars<Type>*)this->vars; }

	template <typename Type>
	Optional<Type>::Optional()
	{ }

	template <typename Type>
	Optional<Type>::Optional(Type val) : BaseObject(new OptionalVars<Type>(val))
	{ }

	template <typename Type>
	Type Optional<Type>::value()
	{
		SPADAS_ERROR_RETURNVAL(!this->vars, Type());
		return this->var()->val;
	}

	template <typename Type>
	Type Optional<Type>::value(Type defaultValue)
	{
		if (this->vars) return this->var()->val;
		else return defaultValue;
	}

	template <typename Type>
	Type& Optional<Type>::refValue()
	{
		SPADAS_ERROR_RETURNVAL(!this->vars, *(new Type()));
		return this->var()->val;
	}

	template <typename Type>
	Type* Optional<Type>::operator ->()
	{
		SPADAS_ERROR_RETURNVAL(!this->vars, new Type());
		return &this->var()->val;
	}

	// Implementation of array / 数组实现 ///////////////////////////////////////////////////////
	namespace internal
	{
		template <typename Type>
		Array<Type> arrayCommonPlus(Type* data, UInt size, Array<Type>& arr)
		{
			if (size > 0)
			{
				UInt arrSize = arr.size();
				if (size > 0)
				{
					Type *arrData = arr.data();
					Array<Type> out = Array<Type>::createUninitialized(size + arrSize);
					for (UInt i = 0; i < size; i++) out.initialize(i, data[i]);
					for (UInt i = size, j = 0; j < arrSize; i++, j++) out.initialize(i, arrData[j]);
					return out;
				}
				else return Array<Type>(data, size);
			}
			else return arr.clone();
		}

		template <typename Type>
		void arrayCommonSet(Type* data, UInt size, Type& value)
		{
			for (UInt i = 0; i < size; i++)
			{
				data[i] = value;
			}
		}

		template <typename Type>
		Bool arrayCommonContain(Type* data, UInt size, Func<Bool(Type&)>& func)
		{
			for (UInt i = 0; i < size; i++)
			{
				if (func(data[i])) return TRUE;
			}
			return FALSE;
		}

		template <typename Type>
		Array<UInt> arrayCommonSearch(Type* data, UInt size, Func<Bool(Type&)>& func)
		{
			if (size >= 64)
			{
				ArrayX<UInt> bufIndices(size >= 8192 ? (size >= 1048576 ? 256 : 64) : 16);
				for (UInt i = 0; i < size; i++)
				{
					if (func(data[i])) bufIndices.append(i);
				}
				return bufIndices.toArray();
			}
			else
			{
				Array<UInt> bufIndices(size);
				UInt nBufIndices = 0;
				for (UInt i = 0; i < size; i++)
				{
					if (func(data[i])) bufIndices[nBufIndices++] = i;
				}
				if (nBufIndices == 0) return Array<UInt>();
				bufIndices.trim(nBufIndices);
				return bufIndices;
			}
		}

		template <typename Type>
		void arrayCommonSort(Type* data, UInt size, Func<Bool(Type&, Type&)>& func)
		{
			const Int n = (Int)size;
			if (n < 2) return;
			if (__is_trivial(Type) && sizeof(Type) <= 8)
			{
				for (Int i = 0; i < n - 1; i++)
				{
					for (Int j = 0; j < n - i - 1; j++)
					{
						if (func(data[j], data[j + 1]))
						{
							Type tmp = data[j];
							data[j] = data[j + 1];
							data[j + 1] = tmp;
						}
					}
				}
			}
			else
			{
				Array<UInt> indexArr(n);
				UInt* indices = indexArr.data();
				for (Int i = 0; i < n; i++)
				{
					indices[i] = i;
				}
				for (Int i = 0; i < n - 1; i++)
				{
					for (Int j = 0; j < n - i - 1; j++)
					{
						if (func(data[indices[j]], data[indices[j + 1]]))
						{
							UInt tmp = indices[j];
							indices[j] = indices[j + 1];
							indices[j + 1] = tmp;
						}
					}
				}
				Array<Type> buffer(data, n);
				Type* bufferData = buffer.data();
				for (Int i = 0; i < n; i++)
				{
					data[i] = bufferData[indices[i]];
				}
			}
		}

		template <typename Type, typename TargetType>
		Array<TargetType> arrayCommonConvert(Type* data, UInt size, Func<TargetType(Type&)>& func)
		{
			Array<TargetType> output = Array<TargetType>::createUninitialized(size);
			for (UInt i = 0; i < size; i++)
			{
				output.initialize(i, func(data[i]));
			}
			return output;
		}

		template <typename Type>
		Array<ArraySpan<Type> > arrayCommonSplit(Type* data, UInt size, Array<UInt>& sizes, Vars *arrayVars)
		{
			UInt totalSize = 0;
			for (UInt i = 0; i < sizes.size(); i++) totalSize += sizes[i];
			SPADAS_ERROR_RETURNVAL(totalSize != size, Array<ArraySpan<Type> >());
			Array<Type> sourceArray;
			UInt curIndex = 0;
			if (arrayVars)
			{
				auto objBase = Object<ArrayVars<Type> >::castCreate(arrayVars);
				sourceArray = *(Array<Type>*)(&objBase);
				curIndex = (UInt)(data - sourceArray.data());
			}
			Array<ArraySpan<Type> > out = Array<ArraySpan<Type> >::createUninitialized(sizes.size());
			for (UInt i = 0; i < sizes.size(); i++)
			{
				UInt subArraySize = sizes[i];
				if (subArraySize == 0)
				{
					out.initialize(i, ArraySpan<Type>());
				}
				else
				{
					out.initialize(i, arrayVars ? ArraySpan<Type>(sourceArray, curIndex, subArraySize) : ArraySpan<Type>(data + curIndex, subArraySize));
					curIndex += subArraySize;
				}
			}
			return out;
		}

		template <typename Type, typename TargetType>
		Array<TargetType> arrayCommonAsArray(Type* data, UInt size)
		{
			Array<TargetType> output = Array<TargetType>::createUninitialized(size);
			for (UInt i = 0; i < size; i++) output.initialize(i, data[i].template as<TargetType>());
			return output;
		}

		template <typename Type, typename TargetType>
		Array<TargetType> arrayCommonAsArray(Type* data, UInt size, Array<Bool>& ok)
		{
			Array<TargetType> output = Array<TargetType>::createUninitialized(size);
			ok = Array<Bool>(size);
			for (UInt i = 0; i < size; i++) output.initialize(i, data[i].template as<TargetType>(ok[i]));
			return output;
		}
	}

	template <typename Type> class ArrayVars : public Vars
	{
	public:
		SPADAS_VARS(Array<Type>, Vars)
		UInt size;
		UInt reserveSize;
		Type *data;
		ArrayVars(UInt size0, Type* data0) : size(size0), reserveSize(0), data(data0)
		{ }
		~ArrayVars()
		{
			if (!__is_trivial(Type))
			{
				UInt count = math::max(size, reserveSize);
				for (UInt i = 0; i < count; i++)
				{
					(&data[i])->~Type();
				}
			}
		}
	};

	template <typename Type>
	String Array<Type>::typeName() { static String name = (String)"spadas.Array<" + typeid(Type).name() + ">"; return name; }

	template <typename Type>
	ArrayVars<Type>* Array<Type>::var() { return (ArrayVars<Type>*)this->vars; }

	template <typename Type>
	Array<Type>::Array()
	{ }

	template <typename Type>
	Array<Type>::Array(UInt size)
	{
		if (size == 0) return;
		size = math::min(size, ARRAY_SIZE_LIMIT);
		Byte* newVarsRaw = new Byte[sizeof(ArrayVars<Type>) + sizeof(Type) * size];
		ArrayVars<Type>* newVars = new (newVarsRaw)ArrayVars<Type>(size, (Type*)&newVarsRaw[sizeof(ArrayVars<Type>)]);
		if (!__is_trivial(Type))
		{
			for (UInt i = 0; i < size; i++)
			{
				new (&newVars->data[i])Type();
			}
		}
		this->setVars(newVars, TRUE);
	}

	template <typename Type>
	Array<Type>::Array(UInt size, Type originValue)
	{
		if (size == 0) return;
		size = math::min(size, ARRAY_SIZE_LIMIT);
		Byte* newVarsRaw = new Byte[sizeof(ArrayVars<Type>) + sizeof(Type) * size];
		ArrayVars<Type>* newVars = new (newVarsRaw)ArrayVars<Type>(size, (Type*)&newVarsRaw[sizeof(ArrayVars<Type>)]);
		for (UInt i = 0; i < size; i++)
		{
			new (&newVars->data[i])Type(originValue);
		}
		this->setVars(newVars, TRUE);
	}

	template <typename Type>
	Array<Type>::Array(const Type arr[], UInt size)
	{
		if (arr == 0 || size == 0) return;
		size = math::min(size, ARRAY_SIZE_LIMIT);
		Byte* newVarsRaw = new Byte[sizeof(ArrayVars<Type>) + sizeof(Type) * size];
		ArrayVars<Type>* newVars = new (newVarsRaw)ArrayVars<Type>(size, (Type*)&newVarsRaw[sizeof(ArrayVars<Type>)]);
		for (UInt i = 0; i < size; i++)
		{
			new (&newVars->data[i])Type(arr[i]);
		}
		this->setVars(newVars, TRUE);
	}

	template <typename Type>
	Type *Array<Type>::data()
	{
		return this->vars ? this->var()->data : 0;
	}

	template <typename Type>
	UInt Array<Type>::size()
	{
		return this->vars ? this->var()->size : 0;
	}

	template <typename Type>
	Bool Array<Type>::isEmpty()
	{
		return this->vars == 0;
	}

	template <typename Type>
	Array<Type> Array<Type>::clone()
	{
		if (!this->vars) return Array<Type>();
		else return Array<Type>(this->var()->data, this->var()->size);
	}

	template <typename Type>
	Type& Array<Type>::operator [](UInt index)
	{
		SPADAS_ERROR_RETURNVAL(!this->vars || index >= this->var()->size, *(new Type));
		return this->var()->data[index];
	}

	template <typename Type>
	Type& Array<Type>::first()
	{
		SPADAS_ERROR_RETURNVAL(!this->vars, *(new Type));
		return *this->var()->data;
	}

	template <typename Type>
	Type& Array<Type>::last()
	{
		SPADAS_ERROR_RETURNVAL(!this->vars, *(new Type));
		return this->var()->data[this->var()->size - 1];
	}

	template <typename Type>
	Array<Type> Array<Type>::operator +(Array<Type> arr)
	{
		if (!this->vars) return arr.clone();
		else return internal::arrayCommonPlus<Type>(this->var()->data, this->var()->size, arr);
	}

	template <typename Type>
	Array<Type>& Array<Type>::trim(UInt size)
	{
		SPADAS_ERROR_RETURNVAL(size == 0, *this);
		if (this->vars && size < this->var()->size)
		{
			if (!__is_trivial(Type))
			{
				for (UInt i = math::max(size, this->var()->reserveSize); i < this->var()->size; i++)
				{
					(&this->var()->data[i])->~Type();
				}
			}
			this->var()->size = size;
		}
		return *this;
	}

	template <typename Type>
	void Array<Type>::set(Type value)
	{
		if (!this->vars) return;
		else internal::arrayCommonSet<Type>(this->var()->data, this->var()->size, value);
	}

	template <typename Type>
	Bool Array<Type>::contain(Type val)
	{
		if (!this->vars) return FALSE;
		else
		{
			Func<Bool(Type&)> func = [val](Type& v){ return v == val; };
			return internal::arrayCommonContain<Type>(this->var()->data, this->var()->size, func);
		}
	}

	template <typename Type>
	Bool Array<Type>::containAs(Func<Bool(Type&)> func)
	{
		if (!this->vars) return FALSE;
		SPADAS_ERROR_RETURNVAL(!func, FALSE);
		return internal::arrayCommonContain<Type>(this->var()->data, this->var()->size, func);
	}

	template <typename Type>
	Array<UInt> Array<Type>::search(Type val)
	{
		if (!this->vars) return Array<UInt>();
		else
		{
			Func<Bool(Type&)> func = [val](Type& v){ return v == val; };
			return internal::arrayCommonSearch<Type>(this->var()->data, this->var()->size, func);
		}
	}

	template <typename Type>
	Array<UInt> Array<Type>::searchAs(Func<Bool(Type&)> func)
	{
		if (!this->vars) return Array<UInt>();
		SPADAS_ERROR_RETURNVAL(!func, Array<UInt>());
		return internal::arrayCommonSearch<Type>(this->var()->data, this->var()->size, func);
	}

	template <typename Type>
	void Array<Type>::sort()
	{
		if (!this->vars) return;
		else
		{
			Func<Bool(Type&, Type&)> func = [](Type& a, Type& b){ return a > b; };
			internal::arrayCommonSort<Type>(this->var()->data, this->var()->size, func);
		}
	}

	template <typename Type>
	void Array<Type>::sortAs(Func<Bool(Type&, Type&)> func)
	{
		if (!this->vars) return;
		SPADAS_ERROR_RETURN(!func);
		internal::arrayCommonSort<Type>(this->var()->data, this->var()->size, func);
	}

	template <typename Type>
	template <typename TargetType>
	Array<TargetType> Array<Type>::convert(Func<TargetType(Type&)> func)
	{
		if (!this->vars) return Array<TargetType>();
		SPADAS_ERROR_RETURNVAL(!func, Array<TargetType>());
		return internal::arrayCommonConvert<Type, TargetType>(this->var()->data, this->var()->size, func);
	}

	template <typename Type>
	Array<ArraySpan<Type> > Array<Type>::split(Array<UInt> sizes)
	{
		SPADAS_ERROR_RETURNVAL(!this->vars, Array<ArraySpan<Type> >());
		this->var()->reserveSize = math::max(this->var()->reserveSize, this->var()->size);
		return internal::arrayCommonSplit(this->var()->data, this->var()->size, sizes, this->vars);
	}

	template <typename Type>
	template <typename TargetType>
	Array<TargetType> Array<Type>::asArray()
	{
		if (!this->vars) return Array<TargetType>();
		else return internal::arrayCommonAsArray<Type, TargetType>(this->var()->data, this->var()->size);
	}

	template <typename Type>
	template <typename TargetType>
	Array<TargetType> Array<Type>::asArray(Array<Bool>& ok)
	{
		if (!this->vars)
		{
			ok = Array<Bool>();
			return Array<TargetType>();
		}
		else return internal::arrayCommonAsArray<Type, TargetType>(this->var()->data, this->var()->size, ok);
	}

	template <typename Type>
	ArraySpan<Type> Array<Type>::span(UInt index, UInt size)
	{
		if (this->vars) this->var()->reserveSize = math::max(this->var()->reserveSize, this->var()->size);
		return ArraySpan<Type>(*this, index, size);
	}

	template <typename Type>
	ArrayElem<Type> Array<Type>::firstElem()
	{
		if (this->vars) this->var()->reserveSize = math::max(this->var()->reserveSize, this->var()->size);
		return ArrayElem<Type>(*this, 0);
	}

	template <typename Type>
	ArrayElem<Type> Array<Type>::lastElem()
	{
		if (this->vars) this->var()->reserveSize = math::max(this->var()->reserveSize, this->var()->size);
		return ArrayElem<Type>(*this, this->vars ? (this->var()->size - 1) : UINF);
	}

	template <typename Type>
	void Array<Type>::copy(Array<Type> src, Region srcRegion, UInt thisOffset)
	{
		SPADAS_ERROR_RETURN(!this->vars || !src.vars || this->vars == src.vars);
		UInt srcSize = src.var()->size;
		SPADAS_ERROR_RETURN(srcRegion.offset < 0 || srcRegion.offset + srcRegion.size > srcSize);
		SPADAS_ERROR_RETURN(thisOffset + srcRegion.size > this->var()->size);
		Type *srcData = src.var()->data;
		Type *dstData = this->var()->data;
		for (UInt i = 0, srcI = srcRegion.offset, dstI = thisOffset; i < srcRegion.size; i++, srcI++, dstI++)
		{
			dstData[dstI] = srcData[srcI];
		}
	}

	template <typename Type>
	Array<Type> Array<Type>::scalar(Type element)
	{
		return Array<Type>(&element, 1);
	}

	template <typename Type>
	template <typename ArgType>
	Array<Type> Array<Type>::create(UInt size, ArgType firstValue, ...)
	{
		if (size == 0) return Array<Type>();
		Array<Type> out = Array<Type>::createUninitialized(size);
		out.initialize(0, (Type)firstValue);
		if (size == 1) return out;
		va_list list;
		va_start(list, firstValue);
		for (UInt i = 1; i < size; i++)
		{
			Type val = (Type)va_arg(list, ArgType);
			out.initialize(i, val);
		}
		va_end(list);
		return out;
	}

	template <typename Type>
	Array<Type> Array<Type>::createUninitialized(UInt size)
	{
		if (size == 0) return Array<Type>();
		size = math::min(size, ARRAY_SIZE_LIMIT);
		Byte* newVarsRaw = new Byte[sizeof(ArrayVars<Type>) + sizeof(Type) * size];
		ArrayVars<Type>* newVars = new (newVarsRaw)ArrayVars<Type>(size, (Type*)&newVarsRaw[sizeof(ArrayVars<Type>)]);
		Array<Type> out;
		out.setVars(newVars, TRUE);
		return out;
	}

	template <typename Type>
	void Array<Type>::initialize(UInt index, const Type& val)
	{
		new (&this->var()->data[index])Type(val);
	}

	template <typename Type>
	Array<Type> Array<Type>::merge(Array<Array<Type> > arrs)
	{
		UInt totalSize = 0;
		for (UInt i = 0; i < arrs.size(); i++) totalSize += arrs[i].size();
		if (totalSize == 0) return Array<Type>();
		SPADAS_ERROR_RETURNVAL(totalSize > ARRAY_SIZE_LIMIT, Array<Type>());
		Array<Type> out = Array<Type>::createUninitialized(totalSize);
		UInt n = 0;
		for (UInt i = 0; i < arrs.size(); i++)
		{
			Array<Type>& arr = arrs[i];
			if (!arr.vars) continue;
			Type *srcData = arr.var()->data;
			UInt copySize = arr.var()->size;
			for (UInt j = 0; j < copySize; j++) out.initialize(n++, srcData[j]);
		}
		return out;
	}

	template <typename Type>
	Array<Type> Array<Type>::merge(Array<ArraySpan<Type> > spans)
	{
		UInt totalSize = 0;
		for (UInt i = 0; i < spans.size(); i++) totalSize += spans[i].size();
		if (totalSize == 0) return Array<Type>();
		SPADAS_ERROR_RETURNVAL(totalSize > ARRAY_SIZE_LIMIT, Array<Type>());
		Array<Type> out = Array<Type>::createUninitialized(totalSize);
		UInt n = 0;
		for (UInt i = 0; i < spans.size(); i++)
		{
			ArraySpan<Type>& span = spans[i];
			Type *srcData = span.data();
			UInt copySize = span.size();
			for (UInt j = 0; j < copySize; j++) out.initialize(n++, srcData[j]);
		}
		return out;
	}

	template <typename Type>
	ArraySpan<Type>::ArraySpan() : source(0), idx(0), siz(0)
	{}

	template <typename Type>
	ArraySpan<Type>::ArraySpan(const Type *ptr, UInt size) : source(0), idx(0), siz(0)
	{
		if (size == 0) return;
		SPADAS_ERROR_RETURN(ptr == NULL);
		this->source = (ULong)ptr;
		this->siz = size;
	}

	template <typename Type>
	ArraySpan<Type>::ArraySpan(Array<Type>& sourceArray, UInt offset, UInt size) : source(0), idx(0), siz(0)
	{
		if (size == 0) return;
		ArrayVars<Type> *localVars = sourceArray.var();
		SPADAS_ERROR_RETURN(!localVars);
		SPADAS_ERROR_RETURN(offset >= localVars->size);
		localVars->retain();
		this->source = (ULong)localVars;
		this->idx = (UInt)((ULong)(localVars->data + offset) - this->source);
		this->siz = math::min(size, localVars->size - offset);
	}

	template <typename Type>
	ArraySpan<Type>::ArraySpan(ArraySpan<Type>& sourceSpan, UInt offset, UInt size) : source(0), idx(0), siz(0)
	{
		if (size == 0) return;
		SPADAS_ERROR_RETURN(sourceSpan.source == 0);
		SPADAS_ERROR_RETURN(offset >= sourceSpan.siz);
		if (sourceSpan.idx)
		{
			((Vars*)sourceSpan.source)->retain();
			this->source = sourceSpan.source;
			this->idx = sourceSpan.idx + offset * sizeof(Type);
		}
		else
		{
			this->source = sourceSpan.source + offset * sizeof(Type);
		}
		this->siz = math::min(size, sourceSpan.siz - offset);
	}

	template <typename Type>
	ArraySpan<Type>::ArraySpan(const ArraySpan<Type>& span) : source(span.source), idx(span.idx), siz(span.siz)
	{
		if (this->idx) ((Vars*)this->source)->retain();
	}

	template <typename Type>
	ArraySpan<Type>::~ArraySpan()
	{
		if (this->idx) ((Vars*)this->source)->release();
	}

	template <typename Type>
	ArraySpan<Type>& ArraySpan<Type>::operator =(const ArraySpan<Type>& span)
	{
		if (this->idx) ((Vars*)this->source)->release();
		this->source = span.source;
		this->idx = span.idx;
		this->siz = span.siz;
		if (this->idx) ((Vars*)this->source)->retain();
		return *this;
	}

	template <typename Type>
	Bool ArraySpan<Type>::isRefCounting()
	{
		return this->idx != 0;
	}

	template <typename Type>
	Type* ArraySpan<Type>::data()
	{
		return (Type*)(this->source + this->idx);
	}

	template <typename Type>
	UInt ArraySpan<Type>::size()
	{
		return this->siz;
	}

	template <typename Type>
	Bool ArraySpan<Type>::isEmpty()
	{
		return this->siz == 0;
	}

	template <typename Type>
	Array<Type> ArraySpan<Type>::clone()
	{
		if (this->source)
		{
			Array<Type> output = Array<Type>::createUninitialized(this->siz);
			Type *src = (Type*)(this->source + this->idx);
			for (UInt i = 0; i < this->siz; i++) output.initialize(i, src[i]);
			return output;
		}
		else return Array<Type>();
	}

	template <typename Type>
	Type& ArraySpan<Type>::operator [](UInt index)
	{
		SPADAS_ERROR_RETURNVAL(index >= this->siz, *(new Type));
		return *(Type*)(source + idx + index * sizeof(Type));
	}

	template <typename Type>
	Type& ArraySpan<Type>::first()
	{
		SPADAS_ERROR_RETURNVAL(this->siz == 0, *(new Type));
		return *(Type*)(source + idx);
	}

	template <typename Type>
	Type& ArraySpan<Type>::last()
	{
		SPADAS_ERROR_RETURNVAL(this->siz == 0, *(new Type));
		return *(Type*)(source + idx + (this->siz - 1) * sizeof(Type));
	}

	template <typename Type>
	Array<Type> ArraySpan<Type>::operator +(Array<Type> arr)
	{
		if (this->source) return internal::arrayCommonPlus<Type>((Type*)(this->source + this->idx), this->siz, arr);
		else return arr.clone();
	}

	template <typename Type>
	ArraySpan<Type>& ArraySpan<Type>::trim(UInt size)
	{
		SPADAS_ERROR_RETURNVAL(size == 0, *this);
		this->siz = math::min(this->siz, size);
		return *this;
	}

	template <typename Type>
	void ArraySpan<Type>::set(Type value)
	{
		if (this->source) internal::arrayCommonSet<Type>((Type*)(this->source + this->idx), this->siz, value);
	}

	template <typename Type>
	Bool ArraySpan<Type>::contain(Type val)
	{
		if (this->source)
		{
			Func<Bool(Type&)> func = [val](Type& v){ return v == val; };
			return internal::arrayCommonContain<Type>((Type*)(this->source + this->idx), this->siz, func);
		}
		else return FALSE;
	}

	template <typename Type>
	Bool ArraySpan<Type>::containAs(Func<Bool(Type&)> func)
	{
		if (this->source)
		{
			SPADAS_ERROR_RETURNVAL(!func, FALSE);
			return internal::arrayCommonContain<Type>((Type*)(this->source + this->idx), this->siz, func);
		}
		else return FALSE;
	}

	template <typename Type>
	Array<UInt> ArraySpan<Type>::search(Type val)
	{
		if (this->source)
		{
			Func<Bool(Type&)> func = [val](Type& v){ return v == val; };
			return internal::arrayCommonSearch<Type>((Type*)(this->source + this->idx), this->siz, func);
		}
		else return Array<UInt>();
	}

	template <typename Type>
	Array<UInt> ArraySpan<Type>::searchAs(Func<Bool(Type&)> func)
	{
		if (this->source)
		{
			SPADAS_ERROR_RETURNVAL(!func, Array<UInt>());
			return internal::arrayCommonSearch<Type>((Type*)(this->source + this->idx), this->siz, func);
		}
		else return Array<UInt>();
	}

	template <typename Type>
	void ArraySpan<Type>::sort()
	{
		if (this->source)
		{
			Func<Bool(Type&, Type&)> func = [](Type& a, Type& b){ return a > b; };
			internal::arrayCommonSort<Type>((Type*)(this->source + this->idx), this->siz, func);
		}
	}

	template <typename Type>
	void ArraySpan<Type>::sortAs(Func<Bool(Type&, Type&)> func)
	{
		if (this->source)
		{
			SPADAS_ERROR_RETURN(!func);
			internal::arrayCommonSort<Type>((Type*)(this->source + this->idx), this->siz, func);
		}
	}

	template <typename Type>
	template <typename TargetType>
	Array<TargetType> ArraySpan<Type>::convert(Func<TargetType(Type&)> func)
	{
		if (this->source)
		{
			SPADAS_ERROR_RETURNVAL(!func, Array<TargetType>());
			return internal::arrayCommonConvert<Type, TargetType>((Type*)(this->source + this->idx), this->siz, func);
		}
		return Array<TargetType>();
	}

	template <typename Type>
	Array<ArraySpan<Type> > ArraySpan<Type>::split(Array<UInt> sizes)
	{
		SPADAS_ERROR_RETURNVAL(!this->source, Array<ArraySpan<Type> >());
		return internal::arrayCommonSplit((Type*)(this->source + this->idx), this->siz, sizes, idx ? (Vars*)source : NULL);
	}

	template <typename Type>
	template <typename TargetType>
	Array<TargetType> ArraySpan<Type>::asArray()
	{
		if (this->source) return internal::arrayCommonAsArray<Type, TargetType>((Type*)(this->source + this->idx), this->siz);
		else return Array<TargetType>();
	}

	template <typename Type>
	template <typename TargetType>
	Array<TargetType> ArraySpan<Type>::asArray(Array<Bool>& ok)
	{
		if (this->source) return internal::arrayCommonAsArray<Type, TargetType>((Type*)(this->source + this->idx), this->siz, ok);
		else
		{
			ok = Array<Bool>();
			return Array<TargetType>();
		}
	}

	template <typename Type>
	ArraySpan<Type> ArraySpan<Type>::span(UInt index, UInt size)
	{
		return ArraySpan<Type>(*this, index, size);
	}

	template <typename Type>
	ArrayElem<Type>::ArrayElem(Array<Type> arr0, UInt index0) : arr(arr0), data(arr.data()), size(arr.size()), idx(index0)
	{}

	template <typename Type>
	Bool ArrayElem<Type>::valid()
	{
		return this->idx < this->size;
	}

	template <typename Type>
	UInt ArrayElem<Type>::index()
	{
		return this->idx;
	}

	template <typename Type>
	Type& ArrayElem<Type>::value()
	{
		return this->data[this->idx];
	}

	template <typename Type>
	Type* ArrayElem<Type>::operator ->()
	{
		return &this->data[this->idx];
	}

	template <typename Type>
	void ArrayElem<Type>::operator =(const Type& val)
	{
		this->data[this->idx] = val;
	}

	template <typename Type>
	void ArrayElem<Type>::operator --()
	{
		this->idx--;
	}

	template <typename Type>
	void ArrayElem<Type>::operator ++()
	{
		this->idx++;
	}

	// Implementation of linked list node / 链表节点实现 ///////////////////////////////////////////////////////
	template <typename Type> class ListNodeVars : public Vars
	{
	public:
		Type val;
		ListNodeVars<Type> *nextVars;
		ListNodeVars<Type> *previousVars;
		ListNodeVars() : nextVars(0), previousVars(0)
		{ }
		ListNodeVars(Type& origin) : val(origin), nextVars(0), previousVars(0)
		{ }
	};

	template <typename Type>
	String ListNode<Type>::typeName() { static String name = (String)"spadas.ListNode<" + typeid(Type).name() + ">"; return name; }

	template <typename Type>
	ListNodeVars<Type>* ListNode<Type>::var() { return (ListNodeVars<Type>*)this->vars; }

	template <typename Type>
	ListNode<Type>::ListNode() : BaseObject(new ListNodeVars<Type>())
	{ }

	template <typename Type>
	ListNode<Type>::ListNode(Type val) : BaseObject(new ListNodeVars<Type>(val))
	{ }

	template <typename Type>
	ListNode<Type>::ListNode(ListNodeVars<Type> *vars0)
	{
		setVars(vars0, FALSE);
	}

	template <typename Type>
	Type& ListNode<Type>::value()
	{
		return this->var()->val;
	}

	template <typename Type>
	Type* ListNode<Type>::operator ->()
	{
		return &this->var()->val;
	}

	template <typename Type>
	Bool ListNode<Type>::hasNext()
	{
		return this->var()->nextVars != 0;
	}

	template <typename Type>
	Bool ListNode<Type>::hasPrevious()
	{
		return this->var()->previousVars != 0;
	}

	template <typename Type>
	ListNode<Type> ListNode<Type>::next()
	{
		if (this->var()->nextVars == 0) return *this;
		else return ListNode<Type>(this->var()->nextVars);
	}

	template <typename Type>
	ListNode<Type> ListNode<Type>::previous()
	{
		if (this->var()->previousVars == 0) return *this;
		else return ListNode<Type>(this->var()->previousVars);
	}

	template <typename Type>
	void ListNode<Type>::goNext()
	{
		if (this->var()->nextVars != 0) this->setVars(this->var()->nextVars, FALSE);
	}

	template <typename Type>
	void ListNode<Type>::goPrevious()
	{
		if (this->var()->previousVars != 0) this->setVars(this->var()->previousVars, FALSE);
	}

	template <typename Type>
	void ListNode<Type>::cutNext()
	{
		if (this->var()->nextVars != 0)
		{
			ListNode<Type> nextNode(this->var()->nextVars);
			this->var()->nextVars = 0;
			this->var()->release();
			nextNode.var()->previousVars = 0;
			nextNode.var()->release();
		}
	}

	template <typename Type>
	void ListNode<Type>::cutPrevious()
	{
		if (this->var()->previousVars != 0)
		{
			ListNode<Type> previousNode(this->var()->previousVars);
			this->var()->previousVars = 0;
			this->var()->release();
			previousNode.var()->nextVars = 0;
			previousNode.var()->release();
		}
	}

	template <typename Type>
	ListNode<Type> ListNode<Type>::joinNext(Type val)
	{
		cutNext();
		ListNode<Type> node(val);
		this->var()->nextVars = node.var();
		this->var()->retain();
		node.var()->previousVars = this->var();
		node.var()->retain();
		return node;
	}

	template <typename Type>
	void ListNode<Type>::joinNext(ListNode<Type> node)
	{
		cutNext();
		node.cutPrevious();
		this->var()->nextVars = node.var();
		this->var()->retain();
		node.var()->previousVars = this->var();
		node.var()->retain();
	}

	template <typename Type>
	ListNode<Type> ListNode<Type>::joinPrevious(Type val)
	{
		cutPrevious();
		ListNode<Type> node(val);
		this->var()->previousVars = node.var();
		this->var()->retain();
		node.var()->nextVars = this->var();
		node.var()->retain();
		return node;
	}

	template <typename Type>
	void ListNode<Type>::joinPrevious(ListNode<Type> node)
	{
		cutPrevious();
		node.cutNext();
		this->var()->previousVars = node.var();
		this->var()->retain();
		node.var()->nextVars = this->var();
		node.var()->retain();
	}

	template <typename Type>
	ListNode<Type> ListNode<Type>::insertNext(Type val)
	{
		ListNode<Type> node(val);
		if (this->var()->nextVars == 0)
		{
			this->var()->nextVars = node.var();
			this->var()->retain();
			node.var()->previousVars = this->var();
			node.var()->retain();
		}
		else
		{
			ListNodeVars<Type> *nextNextVars = this->var()->nextVars;
			nextNextVars->previousVars = node.var();
			this->var()->nextVars = node.var();
			node.var()->nextVars = nextNextVars;
			node.var()->previousVars = this->var();
			node.var()->retain();
			node.var()->retain();
		}
		return node;
	}

	template <typename Type>
	void ListNode<Type>::insertNext(ListNode<Type> node)
	{
		node.cutPrevious();
		node.cutNext();
		if (this->var()->nextVars == 0)
		{
			this->var()->nextVars = node.var();
			this->var()->retain();
			node.var()->previousVars = this->var();
			node.var()->retain();
		}
		else
		{
			ListNodeVars<Type> *nextNextVars = this->var()->nextVars;
			nextNextVars->previousVars = node.var();
			this->var()->nextVars = node.var();
			node.var()->nextVars = nextNextVars;
			node.var()->previousVars = this->var();
			node.var()->retain();
			node.var()->retain();
		}
	}

	template <typename Type>
	ListNode<Type> ListNode<Type>::insertPrevious(Type val)
	{
		ListNode<Type> node(val);
		if (this->var()->previousVars == 0)
		{
			this->var()->previousVars = node.var();
			this->var()->retain();
			node.var()->nextVars = this->var();
			node.var()->retain();
		}
		else
		{
			ListNodeVars<Type> *prevPrevVars = this->var()->previousVars;
			prevPrevVars->nextVars = node.var();
			this->var()->previousVars = node.var();
			node.var()->previousVars = prevPrevVars;
			node.var()->nextVars = this->var();
			node.var()->retain();
			node.var()->retain();
		}
		return node;
	}

	template <typename Type>
	void ListNode<Type>::insertPrevious(ListNode<Type> node)
	{
		node.cutPrevious();
		node.cutNext();
		if (this->var()->previousVars == 0)
		{
			this->var()->previousVars = node.var();
			this->var()->retain();
			node.var()->nextVars = this->var();
			node.var()->retain();
		}
		else
		{
			ListNodeVars<Type> *prevPrevVars = this->var()->previousVars;
			prevPrevVars->nextVars = node.var();
			this->var()->previousVars = node.var();
			node.var()->previousVars = prevPrevVars;
			node.var()->nextVars = this->var();
			node.var()->retain();
			node.var()->retain();
		}
	}

	template <typename Type>
	void ListNode<Type>::removeNext()
	{
		if (this->var()->nextVars != 0)
		{
			ListNode<Type> nextNode(this->var()->nextVars);
			nextNode.removeSelf();
		}
	}

	template <typename Type>
	void ListNode<Type>::removePrevious()
	{
		if (this->var()->previousVars != 0)
		{
			ListNode<Type> previousNode(this->var()->previousVars);
			previousNode.removeSelf();
		}
	}

	template <typename Type>
	void ListNode<Type>::removeSelf()
	{
		if (this->var()->nextVars == 0)
		{
			if (this->var()->previousVars != 0)
			{
				ListNode<Type> previousNode(this->var()->previousVars);
				this->var()->previousVars = 0;
				this->var()->release();
				previousNode.var()->nextVars = 0;
				previousNode.var()->release();
			}
		}
		else //var()->nextVars != 0
		{
			if (this->var()->previousVars == 0)
			{
				ListNode<Type> nextNode(this->var()->nextVars);
				this->var()->nextVars = 0;
				this->var()->release();
				nextNode.var()->previousVars = 0;
				nextNode.var()->release();
			}
			else //var()->previousVars != 0
			{
				ListNode<Type> previousNode(this->var()->previousVars), nextNode(this->var()->nextVars);
				previousNode.var()->nextVars = nextNode.var();
				nextNode.var()->previousVars = previousNode.var();
				this->var()->nextVars = 0;
				this->var()->previousVars = 0;
				this->var()->release();
				this->var()->release();
			}
		}
	}

	template <typename Type>
	ListNode<Type> ListNode<Type>::cloneList()
	{
		ListNode<Type> dst(this->var()->val);
		if (this->var()->nextVars == this->var())
		{
			dst.joinNext(dst);
			return dst;
		}
		ListNode<Type> srcCurrent = *this, dstCurrent = dst;
		while (TRUE)
		{
			ListNode<Type> srcPrevious = srcCurrent.previous();
			if (srcPrevious == srcCurrent) break;
			if (srcPrevious == *this)
			{
				dstCurrent.joinPrevious(dst);
				return dst;
			}
			srcCurrent = srcPrevious;
			dstCurrent = dstCurrent.insertPrevious(srcCurrent.value());
		}
		srcCurrent = *this;
		dstCurrent = dst;
		while (TRUE)
		{
			if (srcCurrent.next() == srcCurrent) break;
			srcCurrent = srcCurrent.next();
			dstCurrent = dstCurrent.insertNext(srcCurrent.value());
		}
		return dst;
	}

	template <typename Type>
	void ListNode<Type>::collapse()
	{
		ListNode<Type> currentNode(*this);
		while (TRUE)
		{
			if (currentNode.var()->nextVars == 0) break;
			ListNode<Type> nextNode(currentNode.var()->nextVars);
			nextNode.var()->previousVars = 0;
			nextNode.var()->release();
			currentNode.var()->nextVars = 0;
			currentNode.var()->release();
			currentNode = nextNode;
		}
		currentNode = ListNode<Type>(*this);
		while (TRUE)
		{
			if (currentNode.var()->previousVars == 0) break;
			ListNode<Type> prevNode(currentNode.var()->previousVars);
			prevNode.var()->nextVars = 0;
			prevNode.var()->release();
			currentNode.var()->previousVars = 0;
			currentNode.var()->release();
			currentNode = prevNode;
		}
	}

	// Implementation of tree node / 树节点实现 ///////////////////////////////////////////////////////
	template <typename Type> class TreeNodeVars : public Vars
	{
	public:
		Type val;
		TreeNodeVars<Type> *rootVars;
		TreeNodeVars<Type> *leafFirstVars;
		TreeNodeVars<Type> *leafLastVars;
		TreeNodeVars<Type> *prevVars;
		TreeNodeVars<Type> *nextVars;
		UInt nLeaves;
		TreeNodeVars() : rootVars(0), leafFirstVars(0), leafLastVars(0), prevVars(0), nextVars(0), nLeaves(0)
		{ }
		TreeNodeVars(Type& origin) : val(origin), rootVars(0), leafFirstVars(0), leafLastVars(0), prevVars(0), nextVars(0), nLeaves(0)
		{ }
	};

	template <typename Type>
	String TreeNode<Type>::typeName() { static String name = (String)"spadas.TreeNode<" + typeid(Type).name() + ">"; return name; }

	template <typename Type>
	TreeNodeVars<Type>* TreeNode<Type>::var() { return (TreeNodeVars<Type>*)this->vars; }

	template <typename Type>
	TreeNode<Type>::TreeNode() : BaseObject(new TreeNodeVars<Type>())
	{ }

	template <typename Type>
	TreeNode<Type>::TreeNode(Type val) : BaseObject(new TreeNodeVars<Type>(val))
	{ }

	template <typename Type>
	TreeNode<Type>::TreeNode(TreeNodeVars<Type> *vars0)
	{
		setVars(vars0, FALSE);
	}

	template <typename Type>
	Type& TreeNode<Type>::value()
	{
		return this->var()->val;
	}

	template <typename Type>
	Type* TreeNode<Type>::operator ->()
	{
		return &this->var()->val;
	}

	template <typename Type>
	TreeNode<Type> TreeNode<Type>::root()
	{
		if (this->var()->rootVars == 0) return *this;
		else return TreeNode<Type>(this->var()->rootVars);
	}

	template <typename Type>
	Array<TreeNode<Type> > TreeNode<Type>::leaves()
	{
		Array<TreeNode<Type> > out = Array<TreeNode<Type> >::createUninitialized(this->var()->nLeaves);
		TreeNodeVars<Type> *leafVars = this->var()->leafFirstVars;
		for (UInt i = 0; i < this->var()->nLeaves; i++)
		{
			out.initialize(i, TreeNode<Type>(leafVars));
			leafVars = leafVars->nextVars;
		}
		return out;
	}

	template <typename Type>
	TreeNode<Type> TreeNode<Type>::leafAt(UInt index)
	{
		SPADAS_ERROR_RETURNVAL(index >= this->var()->nLeaves, *this);
		TreeNodeVars<Type>* leafVars = this->var()->leafFirstVars;
		for (UInt i = 0; i < index; i++) leafVars = leafVars->nextVars;
		return TreeNode<Type>(leafVars);
	}

	template <typename Type>
	UInt TreeNode<Type>::nLeaves()
	{
		return this->var()->nLeaves;
	}

	template <typename Type>
	Bool TreeNode<Type>::isLeaf(TreeNode<Type> node)
	{
		TreeNodeVars<Type> *leafVars = this->var()->leafFirstVars;
		while (leafVars)
		{
			if (leafVars == node.var()) return TRUE;
			leafVars = leafVars->nextVars;
		}
		return FALSE;
	}

	template <typename Type>
	TreeNode<Type> TreeNode<Type>::addLeaf(Type val)
	{
		TreeNode<Type> leafNode(val);
		if (this->var()->leafFirstVars == 0)
		{
			this->var()->leafFirstVars = leafNode.var();
		}
		else
		{
			this->var()->leafLastVars->nextVars = leafNode.var();
			leafNode.var()->prevVars = this->var()->leafLastVars;
		}
		this->var()->leafLastVars = leafNode.var();
		leafNode.var()->rootVars = this->var();
		leafNode.var()->retain();
		leafNode.var()->retain();
		this->var()->retain();
		this->var()->nLeaves++;
		return leafNode;
	}

	template <typename Type>
	void TreeNode<Type>::addLeaf(TreeNode<Type> leafNode)
	{
		TreeNodeVars<Type> *currentVars = this->var();
		while (TRUE)
		{
			SPADAS_ERROR_RETURN(currentVars == leafNode.var());
			if (currentVars->rootVars == 0) break;
			else currentVars = currentVars->rootVars;
		}
		leafNode.cutRoot();
		if (this->var()->leafFirstVars == 0)
		{
			this->var()->leafFirstVars = leafNode.var();
		}
		else
		{
			this->var()->leafLastVars->nextVars = leafNode.var();
			leafNode.var()->prevVars = this->var()->leafLastVars;
		}
		this->var()->leafLastVars = leafNode.var();
		leafNode.var()->rootVars = this->var();
		leafNode.var()->retain();
		leafNode.var()->retain();
		this->var()->retain();
		this->var()->nLeaves++;
	}

	template <typename Type>
	void TreeNode<Type>::dropLeaf(TreeNode<Type> leafNode)
	{
		if (this->var()->nLeaves == 0) return;
		Bool found = FALSE;
		TreeNodeVars<Type>* targetVars = this->var()->leafFirstVars;
		while (TRUE)
		{
			if (targetVars == leafNode.var())
			{
				found = TRUE;
				break;
			}
			if (!targetVars->nextVars) break;
			targetVars = targetVars->nextVars;
		}
		SPADAS_ERROR_RETURN(!found);
		if (targetVars->nextVars == 0)
		{
			if (targetVars->prevVars == 0)
			{
				this->var()->leafFirstVars = this->var()->leafLastVars = 0;
			}
			else //targetVars->prevVars != 0
			{
				this->var()->leafLastVars = targetVars->prevVars;
				targetVars->prevVars->nextVars = 0;
				targetVars->prevVars = 0;
			}
		}
		else //targetVars->nextVars != 0
		{
			if (this->var()->prevVars == 0)
			{
				this->var()->leafFirstVars = targetVars->nextVars;
				targetVars->nextVars->prevVars = 0;
				targetVars->nextVars = 0;
			}
			else //targetVars->prevVars != 0
			{
				targetVars->prevVars->nextVars = targetVars->nextVars;
				targetVars->nextVars->prevVars = targetVars->prevVars;
				targetVars->prevVars = targetVars->nextVars = 0;
			}
		}
		targetVars->rootVars = 0;
		this->var()->release();
		targetVars->release();
		targetVars->release();
		this->var()->nLeaves--;
	}

	template <typename Type>
	void TreeNode<Type>::joinRoot(TreeNode<Type> rootNode)
	{
		rootNode.addLeaf(*this);
	}

	template <typename Type>
	void TreeNode<Type>::cutRoot()
	{
		if (this->var()->rootVars)
		{
			TreeNode<Type> rootNode(this->var()->rootVars);
			rootNode.dropLeaf(*this);
		}
	}

	template <typename Type>
	TreeNode<Type> TreeNode<Type>::globalRoot()
	{
		TreeNodeVars<Type> *currentVars = this->var();
		while (currentVars->rootVars) currentVars = currentVars->rootVars;
		return TreeNode<Type>(currentVars);
	}

	template <typename Type>
	Array<TreeNode<Type> > TreeNode<Type>::rootChain()
	{
		UInt depth0 = depth();
		TreeNodeVars<Type> *currentVars = this->var();
		Array<TreeNode<Type> > out = Array<TreeNode<Type> >::createUninitialized(depth0 + 1);
		for (UInt i = 0; i <= depth0; i++)
		{
			out.initialize(i, TreeNode<Type>(currentVars));
			currentVars = currentVars->rootVars;
		}
		return out;
	}

	template <typename Type>
	UInt TreeNode<Type>::depth()
	{
		UInt depth0 = 0;
		TreeNodeVars<Type> *currentVars = this->var();
		while (currentVars->rootVars)
		{
			currentVars = currentVars->rootVars;
			depth0++;
		}
		return depth0;
	}

	template <typename Type>
	TreeNode<Type> TreeNode<Type>::cloneTree()
	{
		TreeNode<Type> node(this->var()->val);
		Array<TreeNode<Type> > srcLeaves = leaves();
		for (UInt i = 0; i < this->var()->nLeaves; i++)
		{
			node.addLeaf(srcLeaves[i].cloneTree());
		}
		return node;
	}

	template <typename Type>
	void TreeNode<Type>::collapse()
	{
		globalRoot().collapseSub();
	}

	template <typename Type>
	void TreeNode<Type>::collapseSub()
	{
		if (this->var()->nLeaves == 0) return;
		Array<TreeNode<Type> > leafNodes = leaves();
		this->var()->leafFirstVars->release();
		this->var()->leafLastVars->release();
		this->var()->leafFirstVars = this->var()->leafLastVars = 0;
		for (UInt i = 0; i < this->var()->nLeaves; i++)
		{
			TreeNode<Type> leafNode = leafNodes[i];
			leafNode.collapseSub();
			if (leafNode.var()->nextVars)
			{
				leafNode.var()->nextVars->release();
				leafNode.var()->nextVars = 0;
			}
			if (leafNode.var()->prevVars)
			{
				leafNode.var()->prevVars->release();
				leafNode.var()->prevVars = 0;
			}
			leafNode.var()->rootVars = 0;
			this->var()->release();
		}
	}
	
	// Implementation of graph node / 图节点实现 ///////////////////////////////////////////////////////
	namespace internal
	{
		template <typename NType, typename LType> struct GraphLink
		{
			GraphNodeVars<NType, LType> *target;
			LType link;
			GraphLink<NType, LType> *next;
			GraphLink(GraphNodeVars<NType, LType> *target0, LType link0) : target(target0), link(link0), next(0)
			{ }
			void release()
			{
				if (next)
				{
					next->release();
					delete next;
				}
			}
		};
	}

	template <typename NType, typename LType> class GraphNodeVars : public Vars
	{
	public:
		NType val;
		UInt nLinks;
		internal::GraphLink<NType, LType> *links;
		GraphNodeVars() : nLinks(0), links(0)
		{ }
		GraphNodeVars(NType& origin) : val(origin), nLinks(0), links(0)
		{ }
		~GraphNodeVars()
		{
			if (links)
			{
				links->release();
				delete links;
			}
		}
	};

	template <typename NType, typename LType>
	String GraphNode<NType, LType>::typeName() { static String name = (String)"spadas.GraphNode<" + typeid(NType).name() + "," + typeid(LType).name() + ">"; return name; }

	template <typename NType, typename LType>
	GraphNodeVars<NType, LType>* GraphNode<NType, LType>::var() { return (GraphNodeVars<NType, LType>*)this->vars; }

	template <typename NType, typename LType>
	GraphNode<NType, LType>::GraphNode() : BaseObject(new GraphNodeVars<NType, LType>())
	{ }

	template <typename NType, typename LType>
	GraphNode<NType, LType>::GraphNode(NType val) : BaseObject(new GraphNodeVars<NType, LType>(val))
	{ }

	template <typename NType, typename LType>
	GraphNode<NType, LType>::GraphNode(GraphNodeVars<NType, LType> *vars0)
	{
		setVars(vars0, FALSE);
	}

	template <typename NType, typename LType>
	NType& GraphNode<NType, LType>::value()
	{
		return this->var()->val;
	}

	template <typename NType, typename LType>
	NType* GraphNode<NType, LType>::operator ->()
	{
		return &this->var()->val;
	}

	template <typename NType, typename LType>
	UInt GraphNode<NType, LType>::nLinks()
	{
		return this->var()->nLinks;
	}

	template <typename NType, typename LType>
	LType& GraphNode<NType, LType>::linkAt(UInt index)
	{
		SPADAS_ERROR_RETURNVAL(index >= this->var()->nLinks, *(new LType));
		internal::GraphLink<NType, LType> *linkInfo = this->var()->links;
		for (UInt i = 0; i < index; i++) linkInfo = linkInfo->next;
		return linkInfo->link;
	}

	template <typename NType, typename LType>
	LType& GraphNode<NType, LType>::linkWith(GraphNode<NType, LType> node)
	{
		internal::GraphLink<NType, LType> *linkInfo = this->var()->links;
		if (linkInfo != 0)
		{
			for (UInt i = 0; i < this->var()->nLinks; i++)
			{
				if (linkInfo->target == node.var()) return linkInfo->link;
				linkInfo = linkInfo->next;
			}
		}
		SPADAS_ERROR_MSG("No linkInfo->target == node.var()");
		return *(new LType);
	}

	template <typename NType, typename LType>
	GraphNode<NType, LType> GraphNode<NType, LType>::nodeAt(UInt index)
	{
		LType dummy;
		return nodeAt(index, dummy);
	}

	template <typename NType, typename LType>
	GraphNode<NType, LType> GraphNode<NType, LType>::nodeAt(UInt index, LType& link)
	{
		if (index >= this->var()->nLinks)
		{
			SPADAS_ERROR_MSG("index >= this->var()->nLinks");
			return GraphNode<NType, LType>();
		}
		internal::GraphLink<NType, LType> *linkInfo = this->var()->links;
		for (UInt i = 0; i < index; i++) linkInfo = linkInfo->next;
		link = linkInfo->link;
		return GraphNode<NType, LType>(linkInfo->target);
	}

	template <typename NType, typename LType>
	UInt GraphNode<NType, LType>::indexOf(GraphNode<NType, LType> node)
	{
		internal::GraphLink<NType, LType> *linkInfo = this->var()->links;
		if (linkInfo == 0) return UINF;
		for (UInt i = 0; i < this->var()->nLinks; i++)
		{
			if (linkInfo->target == node.var()) return i;
			linkInfo = linkInfo->next;
		}
		return UINF;
	}

	template <typename NType, typename LType>
	void GraphNode<NType, LType>::addLinkWith(GraphNode<NType, LType> dst, LType linkToDst, LType linkFromDst)
	{
		internal::GraphLink<NType, LType> *linkInfo = this->var()->links, *previousLinkInfo = 0;
		while (linkInfo)
		{
			if (linkInfo->target == dst.var())
			{
				linkInfo->link = linkToDst;
				linkInfo = dst.var()->links;
				while (linkInfo)
				{
					if (linkInfo->target == this->var())
					{
						linkInfo->link = linkFromDst;
						break;
					}
					linkInfo = linkInfo->next;
				}
				return;
			}
			previousLinkInfo = linkInfo;
			linkInfo = linkInfo->next;
		}
		internal::GraphLink<NType, LType> *newLink = new internal::GraphLink<NType, LType>(dst.var(), linkToDst);
		dst.var()->retain();
		if (!previousLinkInfo) this->var()->links = newLink;
		else previousLinkInfo->next = newLink;
		this->var()->nLinks++;
		linkInfo = dst.var()->links;
		previousLinkInfo = 0;
		while (linkInfo)
		{
			previousLinkInfo = linkInfo;
			linkInfo = linkInfo->next;
		}
		newLink = new internal::GraphLink<NType, LType>(this->var(), linkFromDst);
		this->var()->retain();
		if (!previousLinkInfo) dst.var()->links = newLink;
		else previousLinkInfo->next = newLink;
		dst.var()->nLinks++;
	}

	template <typename NType, typename LType>
	void GraphNode<NType, LType>::cutLinkWith(GraphNode<NType, LType> node)
	{
		internal::GraphLink<NType, LType> *linkInfo = this->var()->links, *previousLinkInfo = 0;
		if (linkInfo == 0) return;
		for (UInt i = 0; i < this->var()->nLinks; i++)
		{
			if (linkInfo->target == node.var())
			{
				if (previousLinkInfo) previousLinkInfo->next = linkInfo->next;
				else this->var()->links = linkInfo->next;
				this->var()->nLinks--;
				delete linkInfo;
				node.var()->release();
				linkInfo = node.var()->links;
				previousLinkInfo = 0;
				for (UInt j = 0; j < node.var()->nLinks; j++)
				{
					if (linkInfo->target == this->var())
					{
						if (previousLinkInfo) previousLinkInfo->next = linkInfo->next;
						else node.var()->links = linkInfo->next;
						node.var()->nLinks--;
						delete linkInfo;
						this->var()->release();
						break;
					}
					previousLinkInfo = linkInfo;
					linkInfo = linkInfo->next;
				}
				return;
			}
			previousLinkInfo = linkInfo;
			linkInfo = linkInfo->next;
		}
	}

	template <typename NType, typename LType>
	void GraphNode<NType, LType>::collapse()
	{
		UInt nLinks = this->var()->nLinks;
		if (nLinks == 0) return;
		GraphNode<NType, LType> **linkedNodes = new GraphNode<NType, LType>*[nLinks];
		for (UInt i = 0; i < nLinks; i++)
		{
			linkedNodes[i] = new GraphNode<NType, LType>(this->var()->links->target);
			cutLinkWith(*linkedNodes[i]);
		}
		for (UInt i = 0; i < nLinks; i++)
		{
			linkedNodes[i]->collapse();
			delete linkedNodes[i];
		}
		delete[] linkedNodes;
	}

	// Implementation of expandable array / 自动变长数组实现 ///////////////////////////////////////////////////////
	namespace internal
	{
		template <typename Type> struct ArrayXNode
		{
			Type* buffer;
			UInt initialized;
			ArrayXNode<Type>* children[2];
			ArrayXNode(Type* bufferUninitialized, UInt bufferSize) : buffer(bufferUninitialized), initialized(bufferSize * (UInt)(Bool)__is_trivial(Type))
			{
				children[0] = 0;
				children[1] = 0;
			}
			ArrayXNode(Type* bufferUninitialized, UInt bufferSize, Type& defaultValue) : buffer(bufferUninitialized), initialized(bufferSize)
			{
				children[0] = 0;
				children[1] = 0;
				for (UInt i = 0; i < bufferSize; i++) new (&buffer[i])Type(defaultValue);
			}
			void release()
			{
				if (!__is_trivial(Type))
				{
					for (UInt i = 0; i < initialized; i++) (&buffer[i])->~Type();
				}
			}
			void reset()
			{
				if (!__is_trivial(Type))
				{
					for (UInt i = 0; i < initialized; i++) (&buffer[i])->~Type();
					initialized = 0;
				}
				children[0] = 0;
				children[1] = 0;
			}
		};
	}

	template <typename Type> class ArrayXVars : public Vars
	{
	public:
		SPADAS_VARS(ArrayX<Type>, Vars)
		UInt size;
		UInt segmentSize;
		Bool withDefault;
		Byte segmentSizePower;
		Word segmentSizeMask;
		UInt accessingSegment;
		internal::ArrayXNode<Type>* accessingNode;
		UInt lastSegment;
		UInt lastNextIndex;
		internal::ArrayXNode<Type>* lastNode;
		Type* defaultValue;
		internal::ArrayXNode<Type> root;
		ArrayXVars(UInt segSize, UInt power, Type* rootBufferUninitialized) : size(0), segmentSize(segSize), withDefault(FALSE), segmentSizePower((Byte)power), segmentSizeMask((Word)(segSize - 1)), accessingSegment(1), lastSegment(1), lastNextIndex(0), defaultValue(0), root(rootBufferUninitialized, segSize)
		{
			accessingNode = lastNode = &root;
		}
		ArrayXVars(UInt segSize, UInt power, Type* defaultVal, Type* rootBufferUninitialized) : size(0), segmentSize(segSize), withDefault(TRUE), segmentSizePower((Byte)power), segmentSizeMask((Word)(segSize - 1)), accessingSegment(1), lastSegment(1), lastNextIndex(0), defaultValue(defaultVal), root(rootBufferUninitialized, segSize, *defaultVal)
		{
			accessingNode = lastNode = &root;
		}
		~ArrayXVars()
		{
			if (withDefault) defaultValue->~Type();
			if (root.children[0]) releaseNode(root.children[0]);
			if (root.children[1]) releaseNode(root.children[1]);
			root.release();
		}
		void releaseNode(internal::ArrayXNode<Type>* node)
		{
			if (node->children[0]) releaseNode(node->children[0]);
			if (node->children[1]) releaseNode(node->children[1]);
			node->release();
			delete node;
		}
	};

	template <typename Type>
	String ArrayX<Type>::typeName() { static String name = (String)"spadas.ArrayX<" + typeid(Type).name() + ">"; return name; }

	template <typename Type>
	ArrayXVars<Type>* ArrayX<Type>::var() { return (ArrayXVars<Type>*)this->vars; }

	template <typename Type>
	ArrayX<Type>::ArrayX()
	{ }

	template <typename Type>
	ArrayX<Type>::ArrayX(UInt segmentSize)
	{
		UInt size, power;
		if (segmentSize <= 1)
		{
			power = 0;
			size = 1;
		}
		else
		{
			power = math::depth(math::min(segmentSize, ARRAYX_SEGMENT_LIMIT) - 1) + 1;
			size = 1 << power;
		}
		Byte* newVarsRaw = new Byte[sizeof(ArrayXVars<Type>) + sizeof(Type) * size];
		ArrayXVars<Type>* newVars = new (newVarsRaw)ArrayXVars<Type>(size, power, (Type*)&newVarsRaw[sizeof(ArrayXVars<Type>)]);
		this->setVars(newVars, TRUE);
	}

	template <typename Type>
	ArrayX<Type>::ArrayX(UInt segmentSize, Type defaultValue)
	{
		UInt size, power;
		if (segmentSize <= 1)
		{
			power = 0;
			size = 1;
		}
		else
		{
			power = math::depth(math::min(segmentSize, ARRAYX_SEGMENT_LIMIT) - 1) + 1;
			size = 1 << power;
		}
		Byte* newVarsRaw = new Byte[sizeof(ArrayXVars<Type>) + sizeof(Type) * (1 + size)];
		Type* defaultVal = new (&newVarsRaw[sizeof(ArrayXVars<Type>)])Type(defaultValue);
		ArrayXVars<Type>* newVars = new (newVarsRaw)ArrayXVars<Type>(size, power, defaultVal, (Type*)&newVarsRaw[sizeof(ArrayXVars<Type>) + sizeof(Type)]);
		this->setVars(newVars, TRUE);
	}

	template <typename Type>
	Pointer ArrayX<Type>::getSegmentNode(UInt segment)
	{
		Int depth = (Int)math::depth(segment);
		internal::ArrayXNode<Type>* currentNode = &this->var()->root;
		for (Int i = depth - 1; i >= 0; i--)
		{
			if (!currentNode->children[0])
			{
				Byte* leaf0Data = new Byte[sizeof(internal::ArrayXNode<Type>) + sizeof(Type) * this->var()->segmentSize];
				Byte* leaf1Data = new Byte[sizeof(internal::ArrayXNode<Type>) + sizeof(Type) * this->var()->segmentSize];
				internal::ArrayXNode<Type>* leaf0Node = 0;
				internal::ArrayXNode<Type>* leaf1Node = 0;
				if (this->var()->withDefault)
				{
					leaf0Node = new (leaf0Data)internal::ArrayXNode<Type>((Type*)&leaf0Data[sizeof(internal::ArrayXNode<Type>)], this->var()->segmentSize, *this->var()->defaultValue);
					leaf1Node = new (leaf1Data)internal::ArrayXNode<Type>((Type*)&leaf1Data[sizeof(internal::ArrayXNode<Type>)], this->var()->segmentSize, *this->var()->defaultValue);
				}
				else
				{
					leaf0Node = new (leaf0Data)internal::ArrayXNode<Type>((Type*)&leaf0Data[sizeof(internal::ArrayXNode<Type>)], this->var()->segmentSize);
					leaf1Node = new (leaf1Data)internal::ArrayXNode<Type>((Type*)&leaf1Data[sizeof(internal::ArrayXNode<Type>)], this->var()->segmentSize);
				}
				currentNode->children[0] = leaf0Node;
				currentNode->children[1] = leaf1Node;
			}
			currentNode = currentNode->children[(UInt)(Bool)(segment & (1 << i))];
		}
		return currentNode;
	}

	template <typename Type>
	Type& ArrayX<Type>::operator [](UInt index)
	{
		if (!this->vars)
		{
			Byte* newVarsRaw = new Byte[sizeof(ArrayXVars<Type>) + sizeof(Type) * 16];
			ArrayXVars<Type>* newVars = new (newVarsRaw)ArrayXVars<Type>(16, 4, (Type*)&newVarsRaw[sizeof(ArrayXVars<Type>)]);
			this->setVars(newVars, TRUE);
		}
		UInt segment = (index >> this->var()->segmentSizePower) + 1;
		UInt localIndex = index & this->var()->segmentSizeMask;
		if (segment != this->var()->accessingSegment)
		{
			SPADAS_ERROR_RETURNVAL(index >= ARRAYX_SIZE_LIMIT, *(new Type));
			this->var()->accessingSegment = segment;
			this->var()->accessingNode = (internal::ArrayXNode<Type>*)getSegmentNode(segment);
		}
		if (index >= this->var()->size)
		{
			this->var()->size = index + 1;
			this->var()->lastSegment = segment;
			this->var()->lastNextIndex = localIndex + 1;
			this->var()->lastNode = this->var()->accessingNode;
		}
		internal::ArrayXNode<Type>& targetNode = *this->var()->accessingNode;
		if (!__is_trivial(Type) && localIndex >= targetNode.initialized)
		{
			for (UInt i = targetNode.initialized; i < this->var()->segmentSize; i++)
			{
				new (&targetNode.buffer[i])Type();
			}
			targetNode.initialized = this->var()->segmentSize;
		}
		return targetNode.buffer[localIndex];
	}

	template <typename Type>
	Bool ArrayX<Type>::isEmpty()
	{
		return !this->vars || this->var()->size == 0;
	}

	template <typename Type>
	UInt ArrayX<Type>::size()
	{
		return this->vars ? this->var()->size : 0;
	}

	template <typename Type>
	void ArrayX<Type>::setSize(UInt size)
	{
		if (!this->vars)
		{
			Byte* newVarsRaw = new Byte[sizeof(ArrayXVars<Type>) + sizeof(Type) * 16];
			ArrayXVars<Type>* newVars = new (newVarsRaw)ArrayXVars<Type>(16, 4, (Type*)&newVarsRaw[sizeof(ArrayXVars<Type>)]);
			this->setVars(newVars, TRUE);
		}
		if (size == this->var()->size) return;
		SPADAS_ERROR_RETURN(size < this->var()->size || size > ARRAYX_SIZE_LIMIT);
		UInt index = size - 1;
		UInt segment = (index >> this->var()->segmentSizePower) + 1;
		UInt localIndex = index & this->var()->segmentSizeMask;
		this->var()->lastSegment = segment;
		if (segment == this->var()->accessingSegment)
		{
			this->var()->lastNode = this->var()->accessingNode;
		}
		else
		{
			this->var()->accessingSegment = segment;
			this->var()->lastNode = this->var()->accessingNode = (internal::ArrayXNode<Type>*)getSegmentNode(segment);
		}
		this->var()->lastNextIndex = localIndex + 1;
		this->var()->size = size;
	}

	template <typename Type>
	void ArrayX<Type>::clear()
	{
		if (!this->vars) return;
		ArrayXVars<Type> *thisVars = this->var();
		if (thisVars->root.children[0]) thisVars->releaseNode(thisVars->root.children[0]);
		if (thisVars->root.children[1]) thisVars->releaseNode(thisVars->root.children[1]);
		thisVars->root.reset();
		thisVars->size = 0;
		thisVars->accessingSegment = 1;
		thisVars->accessingNode = thisVars->lastNode = &thisVars->root;
		thisVars->lastSegment = 1;
		thisVars->lastNextIndex = 0;
	}

	template <typename Type>
	Type& ArrayX<Type>::append(Type val)
	{
		if (!this->vars)
		{
			Byte* newVarsRaw = new Byte[sizeof(ArrayXVars<Type>) + sizeof(Type) * 16];
			ArrayXVars<Type>* newVars = new (newVarsRaw)ArrayXVars<Type>(16, 4, (Type*)&newVarsRaw[sizeof(ArrayXVars<Type>)]);
			this->setVars(newVars, TRUE);
		}
		if (this->var()->lastNextIndex >= this->var()->segmentSize)
		{
			SPADAS_ERROR_RETURNVAL(this->var()->size >= ARRAYX_SIZE_LIMIT, *(new Type));
			UInt segment = (this->var()->size >> this->var()->segmentSizePower) + 1;
			this->var()->lastSegment = this->var()->accessingSegment = segment;
			this->var()->lastNode = this->var()->accessingNode = (internal::ArrayXNode<Type>*)getSegmentNode(segment);
			this->var()->lastNextIndex = 0;
		}
		internal::ArrayXNode<Type>& targetNode = *this->var()->lastNode;
		if (__is_trivial(Type) || this->var()->withDefault)
		{
			this->var()->size++;
			Type& valRef = targetNode.buffer[this->var()->lastNextIndex++];
			valRef = val;
			return valRef;
		}
		else
		{
			this->var()->size++;
			Type& valRef = targetNode.buffer[this->var()->lastNextIndex++];
			new (&valRef)Type(val);
			targetNode.initialized++;
			return valRef;
		}
	}

	template <typename Type>
	void ArrayX<Type>::append(Array<Type> vals)
	{
		for (auto e = vals.firstElem(); e.valid(); ++e)
		{
			append(e.value());
		}
	}

	template <typename Type>
	void ArrayX<Type>::copyFromArray(Array<Type> src, Region srcRegion, UInt thisOffset)
	{
		UInt srcSize = src.size();
		if (srcSize == 0) return;
		SPADAS_ERROR_RETURN(srcRegion.offset < 0 || srcRegion.offset + srcRegion.size > srcSize);
		Type *srcData = src.data();
		for (UInt i = 0, srcI = srcRegion.offset, dstI = thisOffset; i < srcRegion.size; i++, srcI++, dstI++)
		{
			operator[](dstI) = srcData[srcI];
		}
	}

	template <typename Type>
	Array<Type> ArrayX<Type>::toArray(Region thisRegion)
	{
		if (thisRegion.size == 0) return Array<Type>();
		SPADAS_ERROR_RETURNVAL(thisRegion.offset < 0, Array<Type>());
		if (!this->vars) return Array<Type>();
		Array<Type> out = Array<Type>::createUninitialized(thisRegion.size);
		UInt lastIndex = thisRegion.offset + thisRegion.size - 1;
		copyRegion(&this->var()->root, 1, out, thisRegion.offset, lastIndex, (thisRegion.offset >> this->var()->segmentSizePower) + 1, (lastIndex >> this->var()->segmentSizePower) + 1);
		return out;
	}

	template <typename Type>
	void ArrayX<Type>::copyRegion(Pointer nodePtr, UInt segment, Array<Type>& dst, UInt firstIndex, UInt lastIndex, UInt firstSegment, UInt lastSegment)
	{
		internal::ArrayXNode<Type>* node = (internal::ArrayXNode<Type>*)nodePtr;
		if (segment >= firstSegment && segment <= lastSegment)
		{
			UInt startIndex = 0, endIndex = this->var()->segmentSize - 1;
			if (segment == firstSegment) startIndex = firstIndex & this->var()->segmentSizeMask;
			if (segment == lastSegment) endIndex = lastIndex & this->var()->segmentSizeMask;
			Int indexOffset = (Int)(segment - 1) * (Int)this->var()->segmentSize - (Int)firstIndex;
			if (node)
			{
				if (!__is_trivial(Type) && node->initialized < this->var()->segmentSize)
				{
					for (UInt i = startIndex; i < node->initialized; i++) dst.initialize((UInt)(indexOffset + i), node->buffer[i]);
					for (UInt i = node->initialized; i <= endIndex; i++) dst.initialize((UInt)(indexOffset + i), Type());
				}
				else
				{
					for (UInt i = startIndex; i <= endIndex; i++) dst.initialize((UInt)(indexOffset + i), node->buffer[i]);
				}
			}
			else if (this->var()->withDefault)
			{
				for (UInt i = startIndex; i <= endIndex; i++) dst.initialize((UInt)(indexOffset + i), *this->var()->defaultValue);
			}
			else if (!__is_trivial(Type))
			{
				for (UInt i = startIndex; i <= endIndex; i++) dst.initialize((UInt)(indexOffset + i), Type());
			}
		}
		if (segment < lastSegment)
		{
			if (node && node->children[0])
			{
				copyRegion(node->children[0], segment * 2, dst, firstIndex, lastIndex, firstSegment, lastSegment);
				copyRegion(node->children[1], segment * 2 + 1, dst, firstIndex, lastIndex, firstSegment, lastSegment);
			}
			else
			{
				copyRegion(0, segment * 2, dst, firstIndex, lastIndex, firstSegment, lastSegment);
				copyRegion(0, segment * 2 + 1, dst, firstIndex, lastIndex, firstSegment, lastSegment);
			}
		}
	}

	template <typename Type>
	Array<Type> ArrayX<Type>::toArray()
	{
		return toArray(Region(0, size()));
	}

	// Implementation of linked list / 链表实现 ///////////////////////////////////////////////////////
	namespace internal
	{
		const UInt LIST_SEGMENT_SIZE = 16;

		template <typename Type> struct ListSegment;

		template <typename Type> struct ListCell
		{
			Type val;
			ListCell<Type> *prev;
			ListCell<Type> *next;
			ListSegment<Type> *owner;
			ListCell(Type& val, ListSegment<Type> *owner) : val(val), prev(0), next(0), owner(owner)
			{ }
		};

		template <typename Type> struct ListSegment
		{
			ListCell<Type> *buffer;
			ListSegment<Type> *prev;
			ListSegment<Type> *next;
			UInt used;
			UInt unused;
			ListSegment(ListCell<Type> *buffer) : buffer(buffer), prev(0), next(0), used(0), unused(0)
			{ }
		};
	}

	template <typename Type> class ListVars : public Vars
	{
	public:
		SPADAS_VARS(List<Type>, Vars)
		UInt size;
		volatile Int elemRefs;
		internal::ListSegment<Type> *firstSeg;
		internal::ListSegment<Type> *lastSeg;
		internal::ListCell<Type> *head;
		internal::ListCell<Type> *tail;
		ListVars() : size(0), elemRefs(0), head(0), tail(0)
		{
			firstSeg = lastSeg = createSegment();
		}
		~ListVars()
		{
			releaseSegments();
		}
		internal::ListSegment<Type> *createSegment()
		{
			Byte* newBytes = new Byte[sizeof(internal::ListSegment<Type>) + sizeof(internal::ListCell<Type>) * internal::LIST_SEGMENT_SIZE];
			return new (newBytes)internal::ListSegment<Type>((internal::ListCell<Type>*)&newBytes[sizeof(internal::ListSegment<Type>)]);
		}
		void releaseSegments()
		{
			if (!__is_trivial(Type))
			{
				if (head)
				{
					internal::ListCell<Type> *curCell = head;
					while (TRUE)
					{
						internal::ListCell<Type> *nextCell = curCell->next;
						curCell->~ListCell();
						if (curCell == tail) break;
						else curCell = nextCell;
					}
				}
			}
			internal::ListSegment<Type> *curSeg = firstSeg;
			while (TRUE)
			{
				internal::ListSegment<Type> *nextSeg = curSeg->next;
				delete[] (Byte*)curSeg;
				if (curSeg == lastSeg) break;
				else curSeg = nextSeg;
			}
			size = 0;
			firstSeg = lastSeg = 0;
			head = tail = 0;
		}
		void removeSegment(internal::ListSegment<Type> *seg)
		{
			if (seg == firstSeg) firstSeg = seg->next;
			if (seg == lastSeg) lastSeg = seg->prev;
			if (seg->prev) seg->prev->next = seg->next;
			if (seg->next) seg->next->prev = seg->prev;
			delete[] (Byte*)seg;
			if (!firstSeg) firstSeg = lastSeg = createSegment();
		}
	};

	template <typename Type>
	String List<Type>::typeName() { static String name = (String)"spadas.List<" + typeid(Type).name() + ">"; return name; }
 
	template <typename Type>
	ListVars<Type>* List<Type>::var() { return (ListVars<Type>*)this->vars; }

	template <typename Type>
	List<Type>::List()
	{}

	template <typename Type>
	List<Type>::List(Array<Type> arr)
	{
		for (auto e = arr.firstElem(); e.valid(); ++e) this->addToTail(e.value());
	}

	template <typename Type>
	List<Type> List<Type>::clone()
	{
		return List<Type>(this->toArray());
	}

	template <typename Type>
	Bool List<Type>::isEmpty()
	{
		return !this->vars || this->var()->size == 0;
	}

	template <typename Type>
	UInt List<Type>::size()
	{
		return this->vars ? this->var()->size : 0;
	}

	template <typename Type>
	Type& List<Type>::head()
	{
		SPADAS_ERROR_RETURNVAL(!this->var()->head, *(new Type));
		return this->var()->head->val;
	}

	template <typename Type>
	Type& List<Type>::tail()
	{
		SPADAS_ERROR_RETURNVAL(!this->var()->tail, *(new Type));
		return this->var()->tail->val;
	}

	template <typename Type>
	ListElem<Type> List<Type>::headElem()
	{
		if (this->vars && this->var()->size > 0)
		{
			SPADAS_ERROR_RETURNVAL(this->var()->elemRefs > 0, ListElem<Type>(List<Type>(), 0, UINF));
			return ListElem<Type>(*this, this->var()->head, 0);
		}
		return ListElem<Type>(List<Type>(), 0, UINF);
	}

	template <typename Type>
	ListElem<Type> List<Type>::tailElem()
	{
		if (this->vars && this->var()->size > 0)
		{
			SPADAS_ERROR_RETURNVAL(this->var()->elemRefs > 0, ListElem<Type>(List<Type>(), 0, UINF));
			return ListElem<Type>(*this, this->var()->tail, this->var()->size - 1);
		}
		return ListElem<Type>(List<Type>(), 0, UINF);
	}

	template <typename Type>
	Type& List<Type>::addToHead(Type val)
	{
		if (!this->vars) this->setVars(new ListVars<Type>(), TRUE);
		else SPADAS_ERROR_RETURNVAL(this->var()->elemRefs > 0, *(new Type));
		if (this->var()->lastSeg->used == internal::LIST_SEGMENT_SIZE)
		{
			internal::ListSegment<Type> *newSeg = this->var()->createSegment();
			newSeg->prev = this->var()->lastSeg;
			this->var()->lastSeg->next = newSeg;
			this->var()->lastSeg = newSeg;
		}
		internal::ListCell<Type> *targetCell = this->var()->lastSeg->buffer + this->var()->lastSeg->used++;
		new (targetCell)internal::ListCell<Type>(val, this->var()->lastSeg);
		if (this->var()->size > 0)
		{
			targetCell->next = this->var()->head;
			this->var()->head->prev = targetCell;
			this->var()->head = targetCell;
		}
		else
		{
			this->var()->head = this->var()->tail = targetCell;
		}
		this->var()->size++;
		return targetCell->val;
	}

	template <typename Type>
	Type& List<Type>::addToTail(Type val)
	{
		if (!this->vars) this->setVars(new ListVars<Type>(), TRUE);
		else SPADAS_ERROR_RETURNVAL(this->var()->elemRefs > 0, *(new Type));
		if (this->var()->lastSeg->used == internal::LIST_SEGMENT_SIZE)
		{
			internal::ListSegment<Type> *newSeg = this->var()->createSegment();
			newSeg->prev = this->var()->lastSeg;
			this->var()->lastSeg->next = newSeg;
			this->var()->lastSeg = newSeg;
		}
		internal::ListCell<Type> *targetCell = this->var()->lastSeg->buffer + this->var()->lastSeg->used++;
		new (targetCell)internal::ListCell<Type>(val, this->var()->lastSeg);
		if (this->var()->size > 0)
		{
			targetCell->prev = this->var()->tail;
			this->var()->tail->next = targetCell;
			this->var()->tail = targetCell;
		}
		else
		{
			this->var()->head = this->var()->tail = targetCell;
		}
		this->var()->size++;
		return targetCell->val;
	}

	template <typename Type>
	void List<Type>::removeHead()
	{
		if (!this->vars || this->var()->size == 0) return;
		SPADAS_ERROR_RETURN(this->var()->elemRefs > 0);
		internal::ListCell<Type> *targetCell = this->var()->head;
		internal::ListSegment<Type> *targetSeg = targetCell->owner;
		if (this->var()->size > 1)
		{
			this->var()->head = targetCell->next;
			this->var()->head->prev = 0;
		}
		else
		{
			this->var()->head = this->var()->tail = 0;
		}
		if (!__is_trivial(Type)) targetCell->~ListCell();
		if (++targetSeg->unused == internal::LIST_SEGMENT_SIZE) this->var()->removeSegment(targetSeg);
		this->var()->size--;
	}

	template <typename Type>
	void List<Type>::removeTail()
	{
		if (!this->vars || this->var()->size == 0) return;
		SPADAS_ERROR_RETURN(this->var()->elemRefs > 0);
		internal::ListCell<Type> *targetCell = this->var()->tail;
		internal::ListSegment<Type> *targetSeg = targetCell->owner;
		if (this->var()->size > 1)
		{
			this->var()->tail = targetCell->prev;
			this->var()->tail->next = 0;
		}
		else
		{
			this->var()->head = this->var()->tail = 0;
		}
		if (!__is_trivial(Type)) targetCell->~ListCell();
		if (++targetSeg->unused == internal::LIST_SEGMENT_SIZE) this->var()->removeSegment(targetSeg);
		this->var()->size--;
	}

	template <typename Type>
	void List<Type>::remove(Type val)
	{
		if (!this->vars || this->var()->size == 0) return;
		SPADAS_ERROR_RETURN(this->var()->elemRefs > 0);
		internal::ListCell<Type> *curCell = this->var()->head;
		while (TRUE)
		{
			Bool isHead = curCell == this->var()->head;
			Bool isTail = curCell == this->var()->tail;
			internal::ListCell<Type> *prevCell = curCell->prev;
			internal::ListCell<Type> *nextCell = curCell->next;
			internal::ListSegment<Type> *curOwner = curCell->owner;
			if (curCell->val == val)
			{
				if (isHead) this->var()->head = nextCell;
				if (isTail) this->var()->tail = prevCell;
				if (prevCell) prevCell->next = nextCell;
				if (nextCell) nextCell->prev = prevCell;
				if (!__is_trivial(Type)) curCell->~ListCell();
				if (++curOwner->unused == internal::LIST_SEGMENT_SIZE) this->var()->removeSegment(curOwner);
				this->var()->size--;
			}
			if (isTail) break;
			else curCell = nextCell;
		}
	}

	template <typename Type>
	void List<Type>::removeAs(Func<Bool(Type&)> func)
	{
		if (!this->vars || this->var()->size == 0) return;
		SPADAS_ERROR_RETURN(this->var()->elemRefs > 0);
		SPADAS_ERROR_RETURN(!func);
		internal::ListCell<Type> *curCell = this->var()->head;
		while (TRUE)
		{
			Bool isHead = curCell == this->var()->head;
			Bool isTail = curCell == this->var()->tail;
			internal::ListCell<Type> *prevCell = curCell->prev;
			internal::ListCell<Type> *nextCell = curCell->next;
			internal::ListSegment<Type> *curOwner = curCell->owner;
			if (func(curCell->val))
			{
				if (isHead) this->var()->head = nextCell;
				if (isTail) this->var()->tail = prevCell;
				if (prevCell) prevCell->next = nextCell;
				if (nextCell) nextCell->prev = prevCell;
				if (!__is_trivial(Type)) curCell->~ListCell();
				if (++curOwner->unused == internal::LIST_SEGMENT_SIZE) this->var()->removeSegment(curOwner);
				this->var()->size--;
			}
			if (isTail) break;
			else curCell = nextCell;
		}
	}

	template <typename Type>
	void List<Type>::clear()
	{
		if (!this->vars) return;
		SPADAS_ERROR_RETURN(this->var()->elemRefs > 0);
		this->var()->releaseSegments();
		this->var()->firstSeg = this->var()->lastSeg = this->var()->createSegment();
	}

	template <typename Type>
	Array<Type> List<Type>::toArray()
	{
		if (!this->vars) return Array<Type>();
		Array<Type> arr = Array<Type>::createUninitialized(this->var()->size);
		internal::ListCell<Type> *curCell = this->var()->head;
		for (UInt i = 0; i < this->var()->size; i++)
		{
			arr.initialize(i, curCell->val);
			curCell = curCell->next;
		}
		return arr;
	}

	template <typename Type>
	ListElem<Type>::ListElem(List<Type> list, Pointer cell, UInt index) : list(list), cell(cell), idx(index)
	{
		internal::ListCell<Type> *curCell = (internal::ListCell<Type>*)this->cell;
		if (curCell && curCell->prev)
		{
			this->prevCell = curCell->prev;
			this->prevIndex = index - 1;
		}
		else
		{
			this->prevCell = 0;
			this->prevIndex = UINF;
		}
		if (curCell && curCell->next)
		{
			this->nextCell = curCell->next;
			this->nextIndex = index + 1;
		}
		else
		{
			this->nextCell = 0;
			this->nextIndex = UINF;
		}
		ListVars<Type> *listVars = list.var();
		if (listVars) listVars->elemRefs++;
	}

	template <typename Type>
	ListElem<Type>::ListElem(const ListElem<Type>& elem) : list(elem.list), cell(elem.cell), prevCell(elem.prevCell), nextCell(elem.nextCell), idx(elem.idx), prevIndex(elem.prevIndex), nextIndex(elem.nextIndex)
	{
		ListVars<Type> *listVars = list.var();
		if (listVars) listVars->elemRefs++;
	}

	template <typename Type>
	ListElem<Type>::~ListElem()
	{
		ListVars<Type> *listVars = list.var();
		if (listVars) listVars->elemRefs--;
	}

	template <typename Type>
	Bool ListElem<Type>::valid()
	{
		return this->cell != 0;
	}

	template <typename Type>
	Bool ListElem<Type>::hasPrevious()
	{
		return this->prevCell != 0;
	}

	template <typename Type>
	Bool ListElem<Type>::hasNext()
	{
		return this->nextCell != 0;
	}

	template <typename Type>
	UInt ListElem<Type>::index()
	{
		return this->idx;
	}

	template <typename Type>
	Type& ListElem<Type>::value()
	{
		SPADAS_ERROR_RETURNVAL(!this->cell, *(new Type));
		internal::ListCell<Type> *curCell = (internal::ListCell<Type>*)this->cell;
		return curCell->val;
	}

	template <typename Type>
	Type* ListElem<Type>::operator ->()
	{
		SPADAS_ERROR_RETURNVAL(!this->cell, new Type());
		internal::ListCell<Type> *curCell = (internal::ListCell<Type>*)this->cell;
		return &curCell->val;
	}

	template <typename Type>
	Type& ListElem<Type>::previous()
	{
		SPADAS_ERROR_RETURNVAL(!this->prevCell, *(new Type));
		internal::ListCell<Type> *targetCell = (internal::ListCell<Type>*)this->prevCell;
		return targetCell->val;
	}

	template <typename Type>
	Type& ListElem<Type>::next()
	{
		SPADAS_ERROR_RETURNVAL(!this->nextCell, *(new Type));
		internal::ListCell<Type> *targetCell = (internal::ListCell<Type>*)this->nextCell;
		return targetCell->val;
	}

	template <typename Type>
	void ListElem<Type>::operator =(const Type& val)
	{
		SPADAS_ERROR_RETURN(!this->cell);
		internal::ListCell<Type> *curCell = (internal::ListCell<Type>*)this->cell;
		curCell->val = val;
	}

	template <typename Type>
	void ListElem<Type>::operator --()
	{
		if (this->cell)
		{
			this->nextCell = this->cell;
			this->nextIndex = this->idx;
		}
		this->cell = this->prevCell;
		this->idx = this->prevIndex;
		if (this->cell)
		{
			this->prevCell = ((internal::ListCell<Type>*)this->cell)->prev;
			this->prevIndex = this->prevCell ? (this->idx - 1) : UINF;
		}
	}

	template <typename Type>
	void ListElem<Type>::operator ++()
	{
		if (this->cell)
		{
			this->prevCell = this->cell;
			this->prevIndex = this->idx;
		}
		this->cell = this->nextCell;
		this->idx = this->nextIndex;
		if (this->cell)
		{
			this->nextCell = ((internal::ListCell<Type>*)this->cell)->next;
			this->nextIndex = this->nextCell ? (this->idx + 1) : UINF;
		}
	}

	template <typename Type>
	Type& ListElem<Type>::insertPrevious(Type val)
	{
		SPADAS_ERROR_RETURNVAL(!this->cell, *(new Type));
		internal::ListCell<Type> *curCell = (internal::ListCell<Type>*)this->cell;
		ListVars<Type> *listVars = this->list.var();
		if (listVars->lastSeg->used == internal::LIST_SEGMENT_SIZE)
		{
			internal::ListSegment<Type> *newSeg = listVars->createSegment();
			newSeg->prev = listVars->lastSeg;
			listVars->lastSeg->next = newSeg;
			listVars->lastSeg = newSeg;
		}
		internal::ListCell<Type> *targetCell = listVars->lastSeg->buffer + listVars->lastSeg->used++;
		new (targetCell)internal::ListCell<Type>(val, listVars->lastSeg);
		targetCell->prev = curCell->prev;
		targetCell->next = curCell;
		if (curCell->prev) curCell->prev->next = targetCell;
		curCell->prev = targetCell;
		if (listVars->head == curCell) listVars->head = targetCell;
		listVars->size++;
		this->prevCell = targetCell;
		this->prevIndex = this->idx;
		this->idx++;
		if (this->nextCell) this->nextIndex++;
		return targetCell->val;
	}

	template <typename Type>
	Type& ListElem<Type>::insertNext(Type val)
	{
		SPADAS_ERROR_RETURNVAL(!this->cell, *(new Type));
		internal::ListCell<Type> *curCell = (internal::ListCell<Type>*)this->cell;
		ListVars<Type> *listVars = this->list.var();
		if (listVars->lastSeg->used == internal::LIST_SEGMENT_SIZE)
		{
			internal::ListSegment<Type> *newSeg = listVars->createSegment();
			newSeg->prev = listVars->lastSeg;
			listVars->lastSeg->next = newSeg;
			listVars->lastSeg = newSeg;
		}
		internal::ListCell<Type> *targetCell = listVars->lastSeg->buffer + listVars->lastSeg->used++;
		new (targetCell)internal::ListCell<Type>(val, listVars->lastSeg);
		targetCell->next = curCell->next;
		targetCell->prev = curCell;
		if (curCell->next) curCell->next->prev = targetCell;
		curCell->next = targetCell;
		if (listVars->tail == curCell) listVars->tail = targetCell;
		listVars->size++;
		this->nextCell = targetCell;
		this->nextIndex = this->idx + 1;
		return targetCell->val;
	}

	template <typename Type>
	void ListElem<Type>::remove()
	{
		if (!this->cell) return;
		internal::ListCell<Type> *curCell = (internal::ListCell<Type>*)this->cell;
		internal::ListSegment<Type> *curSeg = curCell->owner;
		ListVars<Type> *listVars = this->list.var();
		if (curCell->prev) curCell->prev->next = curCell->next;
		else listVars->head = curCell->next;
		if (curCell->next) curCell->next->prev = curCell->prev;
		else listVars->tail = curCell->prev;
		if (!__is_trivial(Type)) curCell->~ListCell();
		if (++curSeg->unused == internal::LIST_SEGMENT_SIZE) listVars->removeSegment(curSeg);
		listVars->size--;
		this->cell = 0;
		this->idx = UINF;
		if (this->nextCell) this->nextIndex--;
	}

	// Implementation of data stream / 数据流实现 ///////////////////////////////////////////////////////
	template <typename Type> class StreamVars : public Vars
	{
	public:
		SPADAS_VARS(Stream<Type>, Vars)
		UInt capacity;
		volatile UInt nElements;
		UInt nDiscarded;
		Bool discardable;
		volatile Bool terminated;
		Type *buffer;
		Type *toEnqueue;
		Type *toDequeue;
		StreamVars(UInt capacity, Type *buffer, Bool discardable) : capacity(capacity), nElements(0), nDiscarded(0), discardable(discardable), terminated(FALSE), buffer(buffer), toEnqueue(buffer), toDequeue(0)
		{ }
		~StreamVars()
		{
			if (!__is_trivial(Type))
			{
				Type *bufferEnd = buffer + capacity;
				while (toDequeue)
				{
					toDequeue->~Type();
					if (++toDequeue == bufferEnd) toDequeue = buffer;
					if (toDequeue == toEnqueue) toDequeue = 0;
				}
			}
		}
		Bool isSpinLockManaged() override
		{
			return TRUE;
		}
	};

	template <typename Type>
	String Stream<Type>::typeName() { static String name = (String)"spadas.Stream<" + typeid(Type).name() + ">"; return name; }

	template <typename Type>
	StreamVars<Type>* Stream<Type>::var() { return (StreamVars<Type>*)this->vars; }

	template <typename Type>
	Stream<Type>::Stream()
	{
		Byte* newVarsRaw = new Byte[sizeof(StreamVars<Type>) + sizeof(Type)];
		StreamVars<Type>* newVars = new (newVarsRaw)StreamVars<Type>(1, (Type*)&newVarsRaw[sizeof(StreamVars<Type>)], TRUE);
		this->setVars(newVars, TRUE);
	}

	template <typename Type>
	Stream<Type>::Stream(UInt capacity, Bool discardable)
	{
		capacity = math::max(1u, capacity);
		Byte* newVarsRaw = new Byte[sizeof(StreamVars<Type>) + sizeof(Type) * capacity];
		StreamVars<Type>* newVars = new (newVarsRaw)StreamVars<Type>(capacity, (Type*)&newVarsRaw[sizeof(StreamVars<Type>)], discardable);
		this->setVars(newVars, TRUE);
	}

	template <typename Type>
	UInt Stream<Type>::capacity()
	{
		return this->var()->capacity;
	}

	template <typename Type>
	UInt Stream<Type>::nElements()
	{
		return this->var()->nElements;
	}

	template <typename Type>
	Bool Stream<Type>::isEmpty()
	{
		return this->var()->nElements == 0;
	}

	template <typename Type>
	UInt Stream<Type>::nDiscarded()
	{
		return this->var()->nDiscarded;
	}

	template <typename Type>
	Type Stream<Type>::first()
	{
		this->var()->spinEnter();
		if (!this->var()->toDequeue)
		{
			this->var()->spinLeave();
			return Type();
		}
		else
		{
			Type output = *this->var()->toDequeue;
			this->var()->spinLeave();
			return output;
		}
	}

	template <typename Type>
	Type Stream<Type>::last()
	{
		this->var()->spinEnter();
		if (this->var()->nElements == 0)
		{
			this->var()->spinLeave();
			return Type();
		}
		else
		{
			Type output = *((this->var()->toEnqueue == this->var()->buffer ? (this->var()->buffer + this->var()->capacity) : this->var()->toEnqueue) - 1);
			this->var()->spinLeave();
			return output;
		}
	}

	template <typename Type>
	void Stream<Type>::enqueue(Type newElement)
	{
		this->var()->spinEnter();
		if (!this->var()->discardable)
		{
			while (this->var()->nElements == this->var()->capacity)
			{
				this->var()->spinLeave();
				while (this->var()->nElements == this->var()->capacity)
				{
					SPADAS_WARNING_RETURN(this->var()->terminated);
				}
				this->var()->spinEnter();
			}
		}
		if (this->var()->terminated)
		{
			this->var()->spinLeave();
			SPADAS_WARNING_MSG("this->var()->terminated");
			return;
		}
		Type *bufferEnd = this->var()->buffer + this->var()->capacity;
		if (this->var()->nElements == this->var()->capacity)
		{
			if (!__is_trivial(Type)) this->var()->toEnqueue->~Type();
			if (++this->var()->toDequeue == bufferEnd) this->var()->toDequeue = this->var()->buffer;
			this->var()->nDiscarded++;
		}
		else this->var()->nElements++;
		new (this->var()->toEnqueue)Type(newElement);
		if (!this->var()->toDequeue) this->var()->toDequeue = this->var()->toEnqueue;
		if (++this->var()->toEnqueue == bufferEnd) this->var()->toEnqueue = this->var()->buffer;
		this->var()->spinLeave();
	}

	template <typename Type>
	Bool Stream<Type>::enqueue(Type newElement, Flag interrupt)
	{
		this->var()->spinEnter();
		if (!this->var()->discardable)
		{
			while (this->var()->nElements == this->var()->capacity)
			{
				this->var()->spinLeave();
				while (this->var()->nElements == this->var()->capacity)
				{
					if (this->var()->terminated || interrupt.check()) return FALSE;
				}
				this->var()->spinEnter();
			}
		}
		if (this->var()->terminated)
		{
			this->var()->spinLeave();
			return FALSE;
		}
		Type *bufferEnd = this->var()->buffer + this->var()->capacity;
		if (this->var()->nElements == this->var()->capacity)
		{
			if (!__is_trivial(Type)) this->var()->toEnqueue->~Type();
			if (++this->var()->toDequeue == bufferEnd) this->var()->toDequeue = this->var()->buffer;
			this->var()->nDiscarded++;
		}
		else this->var()->nElements++;
		new (this->var()->toEnqueue)Type(newElement);
		if (!this->var()->toDequeue) this->var()->toDequeue = this->var()->toEnqueue;
		if (++this->var()->toEnqueue == bufferEnd) this->var()->toEnqueue = this->var()->buffer;
		this->var()->spinLeave();
		return TRUE;
	}

	template <typename Type>
	void Stream<Type>::enqueue(Array<Type> newElements)
	{
		if (newElements.isEmpty()) return;
		UInt elemSize = newElements.size();
		if (!this->var()->discardable)
		{
			for (UInt i = 0; i < elemSize; i++)
			{
				SPADAS_WARNING_RETURN(this->var()->terminated);
				enqueue(newElements[i]);
			}
		}
		else
		{
			this->var()->spinEnter();
			if (this->var()->terminated)
			{
				this->var()->spinLeave();
				SPADAS_WARNING_MSG("this->var()->terminated");
				return;
			}
			Type *bufferEnd = this->var()->buffer + this->var()->capacity;
			for (UInt i = 0; i < elemSize; i++)
			{
				if (this->var()->nElements == this->var()->capacity)
				{
					if (!__is_trivial(Type)) this->var()->toEnqueue->~Type();
					if (++this->var()->toDequeue == bufferEnd) this->var()->toDequeue = this->var()->buffer;
					this->var()->nDiscarded++;
				}
				else this->var()->nElements++;
				new (this->var()->toEnqueue)Type(newElements[i]);
				if (!this->var()->toDequeue) this->var()->toDequeue = this->var()->toEnqueue;
				if (++this->var()->toEnqueue == bufferEnd) this->var()->toEnqueue = this->var()->buffer;
			}
			this->var()->spinLeave();
		}
	}

	template <typename Type>
	Bool Stream<Type>::enqueue(Array<Type> newElements, Flag interrupt)
	{
		if (newElements.isEmpty()) return TRUE;
		UInt elemSize = newElements.size();
		if (!this->var()->discardable)
		{
			for (UInt i = 0; i < elemSize; i++)
			{
				if (!enqueue(newElements[i], interrupt)) return FALSE;
			}
			return TRUE;
		}
		else
		{
			this->var()->spinEnter();
			if (this->var()->terminated)
			{
				this->var()->spinLeave();
				return FALSE;
			}
			Type *bufferEnd = this->var()->buffer + this->var()->capacity;
			for (UInt i = 0; i < elemSize; i++)
			{
				if (this->var()->nElements == this->var()->capacity)
				{
					if (!__is_trivial(Type)) this->var()->toEnqueue->~Type();
					if (++this->var()->toDequeue == bufferEnd) this->var()->toDequeue = this->var()->buffer;
					this->var()->nDiscarded++;
				}
				else this->var()->nElements++;
				new (this->var()->toEnqueue)Type(newElements[i]);
				if (!this->var()->toDequeue) this->var()->toDequeue = this->var()->toEnqueue;
				if (++this->var()->toEnqueue == bufferEnd) this->var()->toEnqueue = this->var()->buffer;
			}
			this->var()->spinLeave();
			return TRUE;
		}
	}

	template <typename Type>
	Bool Stream<Type>::dequeueOne(Type& elem)
	{
		Bool ret = FALSE;
		this->var()->spinEnter();
		if (this->var()->nElements > 0)
		{
			elem = *this->var()->toDequeue;
			if (!__is_trivial(Type)) this->var()->toDequeue->~Type();
			Type *bufferEnd = this->var()->buffer + this->var()->capacity;
			if (++this->var()->toDequeue == bufferEnd) this->var()->toDequeue = this->var()->buffer;
			if (--this->var()->nElements == 0) this->var()->toDequeue = 0;
			ret = TRUE;
		}
		this->var()->spinLeave();
		return ret;
	}

	template <typename Type>
	Array<Type> Stream<Type>::dequeue(UInt num)
	{
		this->var()->spinEnter();
		UInt nOuts = math::min(this->var()->nElements, num);
		Array<Type> out = Array<Type>::createUninitialized(nOuts);
		Type *bufferEnd = this->var()->buffer + this->var()->capacity;
		for (UInt i = 0; i < nOuts; i++)
		{
			out.initialize(i, *this->var()->toDequeue);
			if (!__is_trivial(Type)) this->var()->toDequeue->~Type();
			if (++this->var()->toDequeue == bufferEnd) this->var()->toDequeue = this->var()->buffer;
		}
		this->var()->nElements -= nOuts;
		if (this->var()->nElements == 0) this->var()->toDequeue = 0;
		this->var()->spinLeave();
		return out;
	}

	template <typename Type>
	Array<Type> Stream<Type>::dequeueAs(Func<Bool(Type&)> func)
	{
		SPADAS_ERROR_RETURNVAL(!func, Array<Type>());
		this->var()->spinEnter();
		ArrayX<Type> out;
		Type *bufferEnd = this->var()->buffer + this->var()->capacity;
		for (UInt i = 0; i < this->var()->nElements; i++)
		{
			if (func(*this->var()->toDequeue))
			{
				out.append(*this->var()->toDequeue);
				if (!__is_trivial(Type)) this->var()->toDequeue->~Type();
				if (++this->var()->toDequeue == bufferEnd) this->var()->toDequeue = this->var()->buffer;
			}
			else break;
		}
		this->var()->nElements -= out.size();
		if (this->var()->nElements == 0) this->var()->toDequeue = 0;
		this->var()->spinLeave();
		return out.toArray();
	}

	template <typename Type>
	Bool Stream<Type>::waitAllDequeued(Flag interrupt, Bool spin)
	{
		while (this->var()->nElements != 0)
		{
			if (interrupt.check()) return FALSE;
			if (!spin) system::wait(1);
		}
		return TRUE;
	}

	template <typename Type>
	void Stream<Type>::terminate()
	{
		this->var()->spinEnter();
		this->var()->terminated = TRUE;
		this->var()->spinLeave();
	}

	template <typename Type>
	Bool Stream<Type>::isTerminated()
	{
		return this->var()->terminated;
	}

	template <typename Type>
	void Stream<Type>::reset()
	{
		this->var()->spinEnter();
		if (!__is_trivial(Type))
		{
			Type *bufferEnd = this->var()->buffer + this->var()->capacity;
			while (this->var()->toDequeue)
			{
				this->var()->toDequeue->~Type();
				if (++this->var()->toDequeue == bufferEnd) this->var()->toDequeue = this->var()->buffer;
				if (this->var()->toDequeue == this->var()->toEnqueue) this->var()->toDequeue = 0;
			}
		}
		else this->var()->toDequeue = 0;
		this->var()->toEnqueue = this->var()->buffer;
		this->var()->nElements = 0;
		this->var()->nDiscarded = 0;
		this->var()->terminated = FALSE;
		this->var()->spinLeave();
	}

	// Implementation of mapping and dictionary / 映射与字典实现 ///////////////////////////////////////////////////////
	namespace internal
	{
		template <typename Type> struct MapKey
		{
			Type key;
			UInt index;
			Bool operator >(const MapKey<Type>& target)
			{
				return key > target.key;
			}
			Bool operator <(const MapKey<Type>& target)
			{
				return key < target.key;
			}
		};
	}

	template <typename Type>
	NumericKey<Type>::NumericKey()
	{}

	template <typename Type>
	NumericKey<Type>::NumericKey(Type value) : val(value)
	{
		if (!__is_standard_layout(Type))
		{
			SPADAS_ERROR_MSG("!__is_standard_layout(Type)");
		}
	}

	template <typename Type>
	Type NumericKey<Type>::value()
	{
		return val;
	}

	template <typename Type>
	Bool NumericKey<Type>::operator ==(NumericKey<Type> decimal)
	{
		UInt typeSize = sizeof(Type);
		Byte *srcBytes = (Byte*)&val;
		Byte *dstBytes = (Byte*)&decimal.val;
		for (UInt i = 0; i < typeSize; i++)
		{
			if (srcBytes[i] != dstBytes[i]) return FALSE;
		}
		return TRUE;
	}

	template <typename Type>
	Bool NumericKey<Type>::operator !=(NumericKey<Type> decimal)
	{
		return !(operator ==(decimal));
	}

	template <typename Type>
	Bool NumericKey<Type>::operator >(NumericKey<Type> decimal)
	{
		return val > decimal.val;
	}

	template <typename Type>
	Bool NumericKey<Type>::operator <(NumericKey<Type> decimal)
	{
		return val < decimal.val;
	}

	template <typename Type>
	Word NumericKey<Type>::getHash()
	{
		UInt typeSize = sizeof(Type);
		UInt typeSizeWord = typeSize >> 1;
		Byte *bytes = (Byte*)&val;
		Word *words = (Word*)&val;
		Word hash = 0;
		for (UInt i = 0; i < typeSizeWord; i++)
		{
			hash += words[i];
			hash *= 3;
		}
		if (typeSize & 0x01)
		{
			hash += (Word)bytes[typeSize - 1];
			hash *= 3;
		}
		return hash;
	}

	template <typename KeyType, typename ValueType> class MapVars : public Vars
	{
	public:
		SPADAS_VARS(Map<KeyType SPADAS_MACRO_COMMA ValueType>, Vars)
		Word mask;
		UInt size;
		Array<ListNode<KeyValue<KeyType, ValueType> >* > table;
		MapVars(UInt bucketCount) : mask((Word)(bucketCount - 1)), size(0), table(bucketCount, 0)
		{}
		~MapVars()
		{
			if (size == 0) return;
			UInt tableSize = table.size();
			ListNode<KeyValue<KeyType, ValueType> >** tableData = table.data();
			for (UInt i = 0; i < tableSize; i++)
			{
				if (tableData[i])
				{
					tableData[i]->collapse();
					delete tableData[i];
				}
			}
		}
	};

	template <typename KeyType, typename ValueType>
	String Map<KeyType, ValueType>::typeName() { static String name = (String)"spadas.Map<" + typeid(KeyType).name() + "," + typeid(ValueType).name() + ">"; return name; }

	template <typename KeyType, typename ValueType>
	MapVars<KeyType, ValueType>* Map<KeyType, ValueType>::var() { return (MapVars<KeyType, ValueType>*)this->vars; }

	template <typename KeyType, typename ValueType>
	Map<KeyType, ValueType>::Map()
	{ }

	template <typename KeyType, typename ValueType>
	Map<KeyType, ValueType>::Map(UInt bucketCount)
	{
		UInt size = bucketCount <= 16 ? 16 : (1 << (math::depth(math::min(bucketCount, MAP_INDEX_LIMIT) - 1) + 1));
		this->setVars(new MapVars<KeyType, ValueType>(size), TRUE);
	}

	template <typename KeyType, typename ValueType>
	template <typename KeyArgType, typename ValueArgType>
	Map<KeyType, ValueType> Map<KeyType, ValueType>::create(UInt size, KeyArgType firstKey, ValueArgType firstValue, ...)
	{
		Map<KeyType, ValueType> map;
		if (size == 0) return map;
		map[(KeyType)firstKey] = (ValueType)firstValue;
		if (size == 1) return map;
		va_list list;
		va_start(list, firstValue);
		for (UInt i = 1; i < size; i++)
		{
			KeyType key = (KeyType)va_arg(list, KeyArgType);
			map[key] = (ValueType)va_arg(list, ValueArgType);
		}
		va_end(list);
		return map;
	}

	template <typename KeyType, typename ValueType>
	void Map<KeyType, ValueType>::clear()
	{
		if (!this->vars) return;
		UInt tableSize = this->var()->table.size();
		ListNode<KeyValue<KeyType, ValueType> >** tableData = this->var()->table.data();
		for (UInt i = 0; i < tableSize; i++)
		{
			if (tableData[i])
			{
				tableData[i]->collapse();
				delete tableData[i];
				tableData[i] = 0;
			}
		}
		this->var()->size = 0;
	}

	template <typename KeyType, typename ValueType>
	Bool Map<KeyType, ValueType>::isEmpty()
	{
		return !this->vars || this->var()->size == 0;
	}

	template <typename KeyType, typename ValueType>
	UInt Map<KeyType, ValueType>::size()
	{
		return this->vars ? this->var()->size : 0;
	}

	template <typename KeyType, typename ValueType>
	Array<KeyType> Map<KeyType, ValueType>::keys()
	{
		if (!this->vars || this->var()->size == 0) return Array<KeyType>();
		Array<KeyType> out = Array<KeyType>::createUninitialized(this->var()->size);
		UInt count = 0;
		UInt tableSize = this->var()->table.size();
		ListNode<KeyValue<KeyType, ValueType> >** tableData = this->var()->table.data();
		for (UInt i = 0; i < tableSize; i++)
		{
			if (!tableData[i]) continue;
			ListNode<KeyValue<KeyType, ValueType> > node = *tableData[i];
			while (TRUE)
			{
				out.initialize(count++, node->key);
				if (node.hasNext()) node.goNext();
				else break;
			}
		}
		return out;
	}

	template <typename KeyType, typename ValueType>
	Array<KeyType> Map<KeyType, ValueType>::keysSorted()
	{
		Array<KeyType> output = keys();
		output.sort();
		return output;
	}

	template <typename KeyType, typename ValueType>
	Array<ValueType> Map<KeyType, ValueType>::values()
	{
		if (!this->vars || this->var()->size == 0) return Array<ValueType>();
		Array<ValueType> out = Array<ValueType>::createUninitialized(this->var()->size);
		UInt count = 0;
		UInt tableSize = this->var()->table.size();
		ListNode<KeyValue<KeyType, ValueType> >** tableData = this->var()->table.data();
		for (UInt i = 0; i < tableSize; i++)
		{
			if (!tableData[i]) continue;
			ListNode<KeyValue<KeyType, ValueType> > node = *tableData[i];
			while (TRUE)
			{
				out.initialize(count++, node->value);
				if (node.hasNext()) node.goNext();
				else break;
			}
		}
		return out;
	}

	template <typename KeyType, typename ValueType>
	Array<KeyValue<KeyType, ValueType> > Map<KeyType, ValueType>::keyValues()
	{
		if (!this->vars || this->var()->size == 0) return Array<KeyValue<KeyType, ValueType> >();
		Array<KeyValue<KeyType, ValueType> > out = Array<KeyValue<KeyType, ValueType> >::createUninitialized(this->var()->size);
		UInt count = 0;
		UInt tableSize = this->var()->table.size();
		ListNode<KeyValue<KeyType, ValueType> >** tableData = this->var()->table.data();
		for (UInt i = 0; i < tableSize; i++)
		{
			if (!tableData[i]) continue;
			ListNode<KeyValue<KeyType, ValueType> > node = *tableData[i];
			while (TRUE)
			{
				out.initialize(count++, node.value());
				if (node.hasNext()) node.goNext();
				else break;
			}
		}
		return out;
	}

	template <typename KeyType, typename ValueType>
	Array<KeyValue<KeyType, ValueType> > Map<KeyType, ValueType>::keyValuesSorted()
	{
		Array<KeyValue<KeyType, ValueType> > output = keyValues();
		output.sort();
		return output;
	}

	template <typename KeyType, typename ValueType>
	void Map<KeyType, ValueType>::pairs(Array<KeyType>& keys, Array<ValueType>& values)
	{
		if (!this->vars || this->var()->size == 0)
		{
			keys = Array<KeyType>();
			values = Array<ValueType>();
			return;
		}
		keys = Array<KeyType>::createUninitialized(this->var()->size);
		values = Array<ValueType>::createUninitialized(this->var()->size);
		UInt count = 0;
		UInt tableSize = this->var()->table.size();
		ListNode<KeyValue<KeyType, ValueType> >** tableData = this->var()->table.data();
		for (UInt i = 0; i < tableSize; i++)
		{
			if (!tableData[i]) continue;
			ListNode<KeyValue<KeyType, ValueType> > node = *tableData[i];
			while (TRUE)
			{
				keys.initialize(count, node->key);
				values.initialize(count++, node->value);
				if (node.hasNext()) node.goNext();
				else break;
			}
		}
	}

	template <typename KeyType, typename ValueType>
	void Map<KeyType, ValueType>::pairsSorted(Array<KeyType>& keys, Array<ValueType>& values)
	{
		Array<KeyType> keysRaw;
		Array<ValueType> valuesRaw;
		pairs(keysRaw, valuesRaw);
		if (keysRaw.isEmpty())
		{
			keys = keysRaw;
			values = valuesRaw;
			return;
		}
		Array<internal::MapKey<KeyType> > mapKeys(keysRaw.size());
		for (UInt i = 0; i < keysRaw.size(); i++)
		{
			mapKeys[i].key = keysRaw[i];
			mapKeys[i].index = i;
		}
		mapKeys.sort();
		keys = Array<KeyType>::createUninitialized(mapKeys.size());
		values = Array<ValueType>::createUninitialized(mapKeys.size());
		for (UInt i = 0; i < mapKeys.size(); i++)
		{
			keys.initialize(i, mapKeys[i].key);
			values.initialize(i, valuesRaw[mapKeys[i].index]);
		}
	}

	template <typename KeyType, typename ValueType>
	Bool Map<KeyType, ValueType>::contain(KeyType key)
	{
		if (!this->vars) return FALSE;
		Word index = key.getHash() & this->var()->mask;
		ListNode<KeyValue<KeyType, ValueType> >* target = this->var()->table[index];
		if (!target) return FALSE;
		ListNode<KeyValue<KeyType, ValueType> > node = *target;
		while (TRUE)
		{
			if (node->key == key) return TRUE;
			else if (!node.hasNext()) return FALSE;
			else node.goNext();
		}
	}

	template <typename KeyType, typename ValueType>
	void Map<KeyType, ValueType>::remove(KeyType key)
	{
		if (!this->vars) return;
		Word index = key.getHash() & this->var()->mask;
		ListNode<KeyValue<KeyType, ValueType> >** tableData = this->var()->table.data();
		if (!tableData[index]) return;
		ListNode<KeyValue<KeyType, ValueType> > node = *tableData[index];
		if (node->key == key)
		{
			if (node.hasNext())
			{
				*tableData[index] = node.next();
				node.removeSelf();
			}
			else
			{
				delete tableData[index];
				tableData[index] = 0;
			}
			this->var()->size--;
			return;
		}
		else
		{
			while (node.hasNext())
			{
				node.goNext();
				if (node->key == key)
				{
					node.removeSelf();
					this->var()->size--;
					return;
				}
			}
		}
	}

	template <typename KeyType, typename ValueType>
	ValueType Map<KeyType, ValueType>::get(KeyType key, ValueType defaultValue)
	{
		if (!this->vars) return defaultValue;
		Word index = key.getHash() & this->var()->mask;
		ListNode<KeyValue<KeyType, ValueType> >* target = this->var()->table[index];
		if (!target) return defaultValue;
		ListNode<KeyValue<KeyType, ValueType> > node = *target;
		while (TRUE)
		{
			KeyValue<KeyType, ValueType>& pair = node.value();
			if (pair.key == key) return pair.value;
			else if (!node.hasNext()) return defaultValue;
			else node.goNext();
		}
	}

	template <typename KeyType, typename ValueType>
	Bool Map<KeyType, ValueType>::tryGet(KeyType key, ValueType& value)
	{
		if (!this->vars) return FALSE;
		Word index = key.getHash() & this->var()->mask;
		ListNode<KeyValue<KeyType, ValueType> >* target = this->var()->table[index];
		if (!target) return FALSE;
		ListNode<KeyValue<KeyType, ValueType> > node = *target;
		while (TRUE)
		{
			KeyValue<KeyType, ValueType>& pair = node.value();
			if (pair.key == key)
			{
				value = pair.value;
				return TRUE;
			}
			else if (!node.hasNext()) return FALSE;
			else node.goNext();
		}
	}

	template <typename KeyType, typename ValueType>
	void Map<KeyType, ValueType>::set(KeyType key, ValueType value)
	{
		if (!this->vars)
		{
			this->setVars(new MapVars<KeyType, ValueType>(256), TRUE);
		}
		Word index = key.getHash() & this->var()->mask;
		ListNode<KeyValue<KeyType, ValueType> >** tableData = this->var()->table.data();
		if (!tableData[index])
		{
			KeyValue<KeyType, ValueType> pair(key, value);
			tableData[index] = new ListNode<KeyValue<KeyType, ValueType> >(pair);
			this->var()->size++;
			return;
		}
		else
		{
			ListNode<KeyValue<KeyType, ValueType> > node = *tableData[index];
			while (TRUE)
			{
				KeyValue<KeyType, ValueType>& pair = node.value();
				if (pair.key == key)
				{
					pair.value = value;
					return;
				}
				else if (!node.hasNext())
				{
					node.joinNext(KeyValue<KeyType, ValueType>(key, value));
					this->var()->size++;
					return;
				}
				else node.goNext();
			}
		}
	}

	template <typename KeyType, typename ValueType>
	ValueType& Map<KeyType, ValueType>::operator [](KeyType key)
	{
		if (!this->vars)
		{
			this->setVars(new MapVars<KeyType, ValueType>(256), TRUE);
		}
		Word index = key.getHash() & this->var()->mask;
		ListNode<KeyValue<KeyType, ValueType> >** tableData = this->var()->table.data();
		if (!tableData[index])
		{
			KeyValue<KeyType, ValueType> pair(key, ValueType());
			tableData[index] = new ListNode<KeyValue<KeyType, ValueType> >(pair);
			this->var()->size++;
			return tableData[index]->value().value;
		}
		else
		{
			ListNode<KeyValue<KeyType, ValueType> > node = *tableData[index];
			while (TRUE)
			{
				KeyValue<KeyType, ValueType>& pair = node.value();
				if (pair.key == key) return pair.value;
				else if (!node.hasNext())
				{
					KeyValue<KeyType, ValueType> pair(key, ValueType());
					ListNode<KeyValue<KeyType, ValueType> > newNode = node.joinNext(pair);
					this->var()->size++;
					return newNode->value;
				}
				else node.goNext();
			}
		}
	}

	template <typename Type>
	Dictionary<Type>::Dictionary()
	{ }

	template <typename Type>
	Dictionary<Type>::Dictionary(UInt bucketCount) : Map<String, Type>(bucketCount)
	{ }

	template <typename Type>
	template <typename ArgType>
	Dictionary<Type> Dictionary<Type>::create(UInt size, const Char firstKey[], ArgType firstValue, ...)
	{
		Dictionary<Type> map;
		if (size == 0) return map;
		map[firstKey] = (Type)firstValue;
		if (size == 1) return map;
		va_list list;
		va_start(list, firstValue);
		for (UInt i = 1; i < size; i++)
		{
			String key(va_arg(list, Char*));
			map[key] = (Type)va_arg(list, ArgType);
		}
		va_end(list);
		return map;
	}

	// Implementation of String template functions / 字符串模板函数实现 ///////////////////////////////////////////////////////
	class StringBufferVars : public Vars
	{
	public:
		SPADAS_VARS(StringBuffer, Vars)
		String target;
	};

	template <typename Type>
	String::String(Type obj)
	{
		String objString = obj.toString();
		if (!objString.isEmpty()) setVars(objString.vars, FALSE);
	}

	template <typename Type>
	String String::merge(Array<Type> arr, String separator)
	{
		if (arr.isEmpty()) return String();
		UInt size = arr.size();
		Array<String> strs = Array<String>::createUninitialized(size);
		for (UInt i = 0; i < size; i++) strs.initialize(i, String(arr[i]));
		return mergeStrings(strs, separator);
	}

	template <typename Type>
	StringAppender& StringAppender::operator +(Type target)
	{
		this->str += target;
		return *this;
	}

	template <typename Type>
	StringAppender operator +(Type obj, String str)
	{
		return String(obj) + str;
	}

	template <typename Type>
	void StringBuffer::append(Type obj)
	{
		this->var()->target += obj;
	}

	// Implementation of enumeration class / 枚举类实现 ///////////////////////////////////////////////////////
	template <typename Type>
	Enum<Type>::Enum()
	{
		this->val = Type::Value::Default;
		this->str = Type::toString(this->val);
	}

	template <typename Type>
	Enum<Type>::Enum(typename Type::Value val)
	{
		this->str = Type::toString(val);
		if (this->str) this->val = val;
		else
		{
			this->val = Type::Value::Default;
			this->str = Type::toString(this->val);
		}
	}

	template <typename Type>
	Bool Enum<Type>::operator ==(typename Type::Value val)
	{
		return this->val == val;
	}

	template <typename Type>
	Bool Enum<Type>::operator ==(Enum<Type> enumeration)
	{
		return this->val == enumeration.val;
	}

	template <typename Type>
	Bool Enum<Type>::operator !=(typename Type::Value val)
	{
		return this->val != val;
	}

	template <typename Type>
	Bool Enum<Type>::operator !=(Enum<Type> enumeration)
	{
		return this->val != enumeration.val;
	}

	template <typename Type>
	Word Enum<Type>::getHash()
	{
		return (Word)(UInt)this->val;
	}

	template <typename Type>
	typename Type::Value Enum<Type>::value()
	{
		return this->val;
	}

	template <typename Type>
	String Enum<Type>::toString()
	{
		return this->str;
	}

	// Implementation of matrix class / 矩阵类实现 ///////////////////////////////////////////////////////
	template <typename Type> class MatrixVars : public Vars
	{
	public:
		SPADAS_VARS(Matrix<Type>, Vars)
		Array<UInt> dims;
		UInt nElems;
		Array<Type> data0;
		Type *data;
		MatrixVars(Array<UInt> dims0/* 不能为空 */) : dims(dims0.clone())
        {
            UInt *dimsData = dims.data();
            UInt nDims = dims.size();
            for (UInt i = 0; i < nDims; i++)
            {
                dimsData[i] += (dimsData[i] == 0);
            }
            ULong nElems0 = 1;
            for (UInt i = 0; i < nDims; i++)
            {
                nElems0 *= (ULong)dimsData[i];
                if (nElems0 > MATRIX_MAX_NELEMS)
                {
					SPADAS_ERROR_MSG("nElems0 > MATRIX_MAX_NELEMS");
                    nElems0 /= (ULong)dimsData[i];
                    for (UInt n = i; n < nDims; n++) dimsData[n] = 1;
					break;
                }
            }
            nElems = (UInt)nElems0;
            data0 = Array<Type>(nElems, 0);
            data = data0.data();
        }
		MatrixVars(Array<Type> data00, UInt nDims) : dims(Array<UInt>(nDims)), data0(data00)
		{ }
	};

	template <typename Type>
	Matrix<Type>::Matrix()
	{ }

	template <typename Type>
	Matrix<Type>::Matrix(UInt nRows, UInt nCols)
	{
		nRows += (nRows == 0);
		nCols += (nCols == 0);
		Array<UInt> dims(2);
		dims[0] = nRows; dims[1] = nCols;
		this->setVars(new MatrixVars<Type>(dims), TRUE);
	}

	template <typename Type>
	Matrix<Type>::Matrix(UInt nRows, UInt nCols, UInt nDepths)
	{
		nRows += (nRows == 0);
		nCols += (nCols == 0);
		nDepths += (nDepths == 0);
		Array<UInt> dims(3);
		dims[0] = nRows; dims[1] = nCols; dims[2] = nDepths;
		this->setVars(new MatrixVars<Type>(dims), TRUE);
	}

	template <typename Type>
	Matrix<Type>::Matrix(Array<UInt> dims)
	{
		if (dims.isEmpty()) dims = Array<UInt>::scalar(1);
		this->setVars(new MatrixVars<Type>(dims), TRUE);
	}

	template <typename Type>
	Matrix<Type>::Matrix(Array<UInt> dims, Pointer raw)
	{
		SPADAS_ERROR_RETURN(dims.isEmpty() || !raw);
		for (UInt i = 0; i < dims.size(); i++)
		{
			SPADAS_ERROR_RETURN (dims[i] == 0);
		}
		this->setVars(new MatrixVars<Type>(dims), TRUE);
		utility::memoryCopy(raw, this->var()->data, this->var()->nElems * sizeof(Type));
	}

	template <typename Type>
	Matrix<Type>::Matrix(Array<Type> arr, UInt nDims)
	{
		SPADAS_ERROR_RETURN(nDims == 0 || arr.size() == 0);
		Array<UInt> dims(nDims, 1);
		dims[0] = arr.size();
		this->setVars(new MatrixVars<Type>(dims), TRUE);
		utility::memoryCopy(arr.data(), this->var()->data, this->var()->nElems * sizeof(Type));
	}

	template <typename Type>
	Matrix<Type>::Matrix(Path filePath)
	{
		SPADAS_ERROR_RETURN(filePath.isNull() || !filePath.exist())
		File file = File::openText(filePath);
		if (file.isNull()) return;
		ArrayX<String> rowStringsBuf(256, String());
		while (TRUE)
		{
			String newString = file.scan();
			if (newString.isEmpty()) break;
			else rowStringsBuf.append(newString);
		}
		Array<String> rowStrings = rowStringsBuf.toArray();
		UInt nRows = rowStrings.size();
		SPADAS_ERROR_RETURN(nRows == 0);
		UInt nCols = 0;
		Array<StringSpan> firstRowElements = rowStrings[0].split('\t');
		for (UInt i = 0; i < firstRowElements.size(); i++)
		{
			if (!firstRowElements[i].isEmpty()) nCols++;
		}
		SPADAS_ERROR_RETURN(nCols == 0);
		Matrix<Type> out(nRows, nCols);
		Type *outData = out.var()->data;
		for (UInt i = 0; i < nRows; i++)
		{
			Array<StringSpan> elems = rowStrings[i].split('\t');
			Type *matRow = &outData[i * nCols];
			UInt curCols = math::min(nCols, elems.size());
			for (UInt n = 0; n < curCols; n++)
			{
				elems[n].toNumber(matRow[n]);
			}
		}
		this->setVars(out.vars, FALSE);
	}

	template <typename Type>
	Matrix<Type> Matrix<Type>::clone()
	{
		SPADAS_ERROR_RETURNVAL(!this->vars, Matrix<Type>());
		Matrix<Type> out(this->var()->dims);
		utility::memoryCopy(this->var()->data, out.var()->data, this->var()->nElems * sizeof(Type));
		return out;
	}

	template <typename Type>
	template <typename TargetType>
	void Matrix<Type>::convertTo(Matrix<TargetType>& target)
	{
		SPADAS_ERROR_RETURN(!this->vars);
		target = Matrix<TargetType>(this->var()->dims);
		UInt nElements = nElems();
		Type *srcData = this->var()->data;
		TargetType *dstData = target.data();
		for (UInt i = 0; i < nElements; i++)
		{
			dstData[i] = (TargetType)srcData[i];
		}
	}

	template <typename Type>
	void Matrix<Type>::copy(Matrix<Type> src, Region2D srcRegion, CoordInt2D thisOffset)
	{
		SPADAS_ERROR_RETURN(!this->vars);
		SPADAS_ERROR_RETURN(!src.vars || this->vars == src.vars);
		srcRegion.dim0 = math::max(srcRegion.dim0, 1u);
		srcRegion.dim1 = math::max(srcRegion.dim1, 1u);
		SPADAS_ERROR_RETURN(srcRegion.offsetI < 0 || srcRegion.offsetJ < 0 || srcRegion.offsetI + srcRegion.dim0 > src.dimAt(0) || srcRegion.offsetJ + srcRegion.dim1 > src.dimAt(1));
		SPADAS_ERROR_RETURN(thisOffset.i < 0 || thisOffset.j < 0 || thisOffset.i + srcRegion.dim0 > this->var()->dims[0] || thisOffset.j + srcRegion.dim1 > this->var()->dims[1]);
		UInt srcDim1 = src.dimAt(1);
		UInt dstDim1 = this->var()->dims[1];
		Type *srcData = &src(srcRegion.offsetI, srcRegion.offsetJ);
		Type *dstData = &operator()(thisOffset.i, thisOffset.j);
		for (UInt i = 0; i < srcRegion.dim0; i++)
		{
			Type *srcRow = &srcData[i * srcDim1];
			Type *dstRow = &dstData[i * dstDim1];
			for (UInt j = 0; j < srcRegion.dim1; j++)
			{
				dstRow[j] = srcRow[j];
			}
		}
	}

	template <typename Type>
	void Matrix<Type>::copy(Matrix<Type> src, Region3D srcRegion, CoordInt3D thisOffset)
	{
		SPADAS_ERROR_RETURN(!this->vars);
		SPADAS_ERROR_RETURN(!src.vars || this->vars == src.vars);
		srcRegion.dim0 = math::max(srcRegion.dim0, 1u);
		srcRegion.dim1 = math::max(srcRegion.dim1, 1u);
		srcRegion.dim2 = math::max(srcRegion.dim2, 1u);
		SPADAS_ERROR_RETURN(srcRegion.offsetI < 0 || srcRegion.offsetJ < 0 || srcRegion.offsetK < 0 || srcRegion.offsetI + srcRegion.dim0 > src.dimAt(0) || srcRegion.offsetJ + srcRegion.dim1 > src.dimAt(1) || srcRegion.offsetK + srcRegion.dim2 > src.dimAt(2));
		SPADAS_ERROR_RETURN(thisOffset.i < 0 || thisOffset.j < 0 || thisOffset.k < 0 || thisOffset.i + srcRegion.dim0 > this->var()->dims[0] || thisOffset.j + srcRegion.dim1 > this->var()->dims[1] || thisOffset.k + srcRegion.dim2 > this->var()->dims[2]);
		UInt srcDim1 = src.dimAt(1);
		UInt srcDim2 = src.dimAt(2);
		UInt dstDim1 = this->var()->dims[1];
		UInt dstDim2 = this->var()->dims[2];
		Type *srcData = &src(srcRegion.offsetI, srcRegion.offsetJ, srcRegion.offsetK);
		Type *dstData = &operator()(thisOffset.i, thisOffset.j, thisOffset.k);
		for (UInt i = 0; i < srcRegion.dim0; i++)
		{
			Type *srcSlice = &srcData[i * srcDim1 * srcDim2];
			Type *dstSlice = &dstData[i * dstDim1 * dstDim2];
			for (UInt j = 0; j < srcRegion.dim1; j++)
			{
				Type *srcRow = &srcSlice[j * srcDim2];
				Type *dstRow = &dstSlice[j * dstDim2];
				for (UInt k = 0; k < srcRegion.dim2; k++)
				{
					dstRow[k] = srcRow[k];
				}
			}
		}
	}

	template <typename Type>
	void Matrix<Type>::set(Type value)
	{
		SPADAS_ERROR_RETURN(!this->vars);
		Type *data0 = this->var()->data;
		for (UInt i = 0; i < this->var()->nElems; i++)
		{
			data0[i] = value;
		}
	}

	template <typename Type>
	Array<UInt> Matrix<Type>::size()
	{
		SPADAS_ERROR_RETURNVAL(!this->vars, Array<UInt>());
		return this->var()->dims.clone();
	}

	template <typename Type>
	UInt Matrix<Type>::dimAt(UInt index)
	{
		SPADAS_ERROR_RETURNVAL(!this->vars, 0);
		UInt nDims = this->var()->dims.size();
		SPADAS_ERROR_RETURNVAL(index >= nDims, 0);
		return this->var()->dims[index];
	}

	template <typename Type>
	UInt Matrix<Type>::nDims()
	{
		SPADAS_ERROR_RETURNVAL(!this->vars, 0);
		return this->var()->dims.size();
	}

	template <typename Type>
	UInt Matrix<Type>::nElems()
	{
		SPADAS_ERROR_RETURNVAL(!this->vars, 0);
		return this->var()->nElems;
	}

	template <typename Type>
	Bool Matrix<Type>::isSize(Size2D size2D)
	{
		SPADAS_ERROR_RETURNVAL(!this->vars, FALSE);
		if (this->var()->dims.size() != 2) return FALSE;
		return this->var()->dims[0] == size2D.height && this->var()->dims[1] == size2D.width;
	}

	template <typename Type>
	Bool Matrix<Type>::isSize(Size3D size3D)
	{
		SPADAS_ERROR_RETURNVAL(!this->vars, FALSE);
		if (this->var()->dims.size() != 3) return FALSE;
		return this->var()->dims[0] == size3D.height && this->var()->dims[1] == size3D.width && this->var()->dims[2] == size3D.depth;
	}

	template <typename Type>
	Bool Matrix<Type>::isSize(Array<UInt> dims)
	{
		SPADAS_ERROR_RETURNVAL(!this->vars, FALSE);
		if (this->var()->dims.size() != dims.size()) return FALSE;
		UInt nDims = this->var()->dims.size();
		for (UInt i = 0; i < nDims; i++)
		{
			if (this->var()->dims[i] != dims[i]) return FALSE;
		}
		return TRUE;
	}

	template <typename Type>
	Type *Matrix<Type>::data()
	{
		SPADAS_ERROR_RETURNVAL(!this->vars, 0);
		return this->var()->data;
	}

	template <typename Type>
	Matrix<Type> Matrix<Type>::operator [](UInt i)
	{
		SPADAS_ERROR_RETURNVAL(!this->vars, Matrix<Type>());
		UInt nDims = this->var()->dims.size();
		SPADAS_ERROR_RETURNVAL(nDims == 0, *this);
		if (i >= this->var()->dims[0])
		{
			SPADAS_ERROR_MSG("i >= this->var()->dims[0]");
			i = 0;
		}
		MatrixVars<Type> *newVars = new MatrixVars<Type>(this->var()->data0, nDims - 1);
		UInt newVarsNDims = newVars->dims.size();
		for (UInt j = 0; j < newVarsNDims; j++)
		{
			newVars->dims[j] = this->var()->dims[j + 1];
		}
		newVars->nElems = this->var()->nElems / this->var()->dims[0];
		newVars->data = &this->var()->data[i * newVars->nElems];
		Matrix<Type> out;
		out.setVars(newVars, TRUE);
		return out;
	}

	template <typename Type>
	Type& Matrix<Type>::operator ()(UInt i)
	{
		SPADAS_ERROR_RETURNVAL(!this->vars, *(new Type));
		SPADAS_ERROR_RETURNVAL(this->var()->dims.size() != 1, *(new Type));
		SPADAS_ERROR_RETURNVAL (i >= this->var()->dims[0], *(new Type));
		return this->var()->data[i];
	}

	template <typename Type>
	Type& Matrix<Type>::operator ()(UInt i, UInt j)
	{
		SPADAS_ERROR_RETURNVAL(!this->vars, *(new Type));
		SPADAS_ERROR_RETURNVAL(this->var()->dims.size() != 2, *(new Type));
		UInt *dimsData = this->var()->dims.data();
		SPADAS_ERROR_RETURNVAL(i >= dimsData[0] || j >= dimsData[1], *(new Type));
		return this->var()->data[i * dimsData[1] + j];
	}

	template <typename Type>
	Type& Matrix<Type>::operator ()(UInt i, UInt j, UInt k)
	{
		SPADAS_ERROR_RETURNVAL(!this->vars, *(new Type));
		SPADAS_ERROR_RETURNVAL(this->var()->dims.size() != 3, *(new Type));
		UInt *dimsData = this->var()->dims.data();
		SPADAS_ERROR_RETURNVAL(i >= dimsData[0] || j >= dimsData[1] || k >= dimsData[2], *(new Type));
		return this->var()->data[(i * dimsData[1] + j) * dimsData[2] + k];
	}

	template <typename Type>
	Matrix<Type> Matrix<Type>::operator +(Matrix<Type> matrix)
	{
		SPADAS_ERROR_RETURNVAL(!this->vars || !matrix.vars, Matrix<Type>());
		SPADAS_ERROR_RETURNVAL(!isSize(matrix.size()), *this);
		Matrix<Type> out(this->var()->dims);
		Type *src1 = this->var()->data;
		Type *src2 = matrix.var()->data;
		Type *dst = out.var()->data;
		for (UInt i = 0; i < this->var()->nElems; i++)
		{
			dst[i] = src1[i] + src2[i];
		}
		return out;
	}

	template <typename Type>
	Matrix<Type> Matrix<Type>::operator -(Matrix<Type> matrix)
	{
		SPADAS_ERROR_RETURNVAL(!this->vars || !matrix.vars, Matrix<Type>());
		SPADAS_ERROR_RETURNVAL(!isSize(matrix.size()), *this);
		Matrix<Type> out(this->var()->dims);
		Type *src1 = this->var()->data;
		Type *src2 = matrix.var()->data;
		Type *dst = out.var()->data;
		for (UInt i = 0; i < this->var()->nElems; i++)
		{
			dst[i] = src1[i] - src2[i];
		}
		return out;
	}

	template <typename Type>
	Matrix<Type> Matrix<Type>::operator *(Type scale)
	{
		SPADAS_ERROR_RETURNVAL(!this->vars, Matrix<Type>());
		Matrix<Type> out(this->var()->dims);
		Type *src = this->var()->data;
		Type *dst = out.var()->data;
		for (UInt i = 0; i < this->var()->nElems; i++)
		{
			dst[i] = src[i] * scale;
		}
		return out;
	}

	template <typename Type>
	Matrix<Type> Matrix<Type>::operator *(Matrix<Type> matrix)
	{
		SPADAS_ERROR_RETURNVAL(!this->vars || !matrix.vars, Matrix<Type>());
		SPADAS_ERROR_RETURNVAL(this->var()->dims.size() != 2 || matrix.var()->dims.size() != 2, *this);
		SPADAS_ERROR_RETURNVAL(this->var()->dims[1] != matrix.var()->dims[0], *this);
		UInt outRows = this->var()->dims[0], outCols = matrix.var()->dims[1];
		Matrix<Type> out(outRows, outCols);
		Type *selfRow = this->var()->data;
		Type *inData = matrix.var()->data;
		Type *outRow = out.var()->data;
		UInt selfStep = this->var()->dims[1];
		UInt inStep = matrix.var()->dims[1];
		UInt outStep = out.var()->dims[1];
		UInt inColIndex;
		Type sum;
		for (UInt i = 0; i < outRows; i++)
		{
			for (UInt j = 0; j < outCols; j++)
			{
				inColIndex = j;
				sum = 0;
				for (UInt k = 0; k < selfStep; k++, inColIndex += inStep)
				{
					sum += selfRow[k] * inData[inColIndex];
				}
				outRow[j] = sum;
			}
			selfRow = &selfRow[selfStep];
			outRow = &outRow[outStep];
		}
		return out;
	}

	template <typename Type>
	Matrix<Type> Matrix<Type>::transpose()
	{
		SPADAS_ERROR_RETURNVAL(!this->vars, Matrix<Type>());
		SPADAS_ERROR_RETURNVAL(this->var()->dims.size() != 2, *this);
		UInt nRows = this->var()->dims[0];
		UInt nCols = this->var()->dims[1];
		Matrix<Type> out(nCols, nRows);
		for (UInt i = 0; i < nCols; i++)
		{
			Type *outRow = &out.var()->data[i * nRows];
			UInt srcIndex0 = i;
			for (UInt j = 0; j < nRows; j++)
			{
				outRow[j] = this->var()->data[srcIndex0 + j*nCols];
			}
		}
		return out;
	}

	template <typename Type>
	String Matrix<Type>::toString()
	{
		SPADAS_ERROR_RETURNVAL(!this->vars, String());
		SPADAS_ERROR_RETURNVAL(this->var()->dims.size() != 2, String());
		UInt nRows = this->var()->dims[0];
		UInt nCols = this->var()->dims[1];
		String outString = String::createWithSize(nRows * (nCols + 1) * 32);
		for (UInt i = 0; i < nRows; i++)
		{
			String rowString = String::createWithSize(nCols * 32);
			Type *rowData = &this->var()->data[i * nCols];
			for (UInt j = 0; j < nCols; j++)
			{
				rowString += rowData[j];
				rowString += '\t';
			}
			outString += rowString;
			outString += '\n';
		}
		return outString;
	}

	template <typename Type>
	void Matrix<Type>::save(Path filePath)
	{
		SPADAS_ERROR_RETURN(!this->vars);
		SPADAS_ERROR_RETURN(filePath.isNull());
		SPADAS_ERROR_RETURN(this->var()->dims.size() != 2);
		File file = File::createText(filePath);
		if (file.isNull()) return;
		UInt nRows = this->var()->dims[0];
		UInt nCols = this->var()->dims[1];
		for (UInt i = 0; i < nRows; i++)
		{
			String rowString = String::createWithSize(nCols * 32);
			Type *rowData = &this->var()->data[i * nCols];
			for (UInt j = 0; j < nCols; j++)
			{
				rowString += rowData[j];
				rowString += '\t';
			}
			file.print(rowString);
		}
	}

	// Implementation of math template functions / 数学相关函数的实现 ///////////////////////////////////////////////////////
	template <typename Type>
	Type spadas::math::min(Type a, Type b)
	{
		return a > b ? b : a;
	}

	template <typename Type>
	Type spadas::math::min(Array<Type> arr)
	{
		Type *data = arr.data();
		UInt size = arr.size();
		if (size == 0) return Type();
		Type target = data[0];
		for (UInt i = 1; i < size; i++)
		{
			if (data[i] < target) target = data[i];
		}
		return target;
	}

	template <typename Type>
	Type spadas::math::max(Type a, Type b)
	{
		return a < b ? b : a;
	}

	template <typename Type>
	Type spadas::math::max(Array<Type> arr)
	{
		Type *data = arr.data();
		UInt size = arr.size();
		if (size == 0) return Type();
		Type target = data[0];
		for (UInt i = 1; i < size; i++)
		{
			if (data[i] > target) target = data[i];
		}
		return target;
	}

	template <typename Type>
	Type spadas::math::clamp(Type number, Type lower, Type upper)
	{
		SPADAS_ERROR_RETURNVAL(lower > upper, number);
		return min(upper, max(lower, number));
	}

	// Implementation of utility template functions / 实用工具相关函数的实现 ///////////////////////////////////////////////////////
	template <typename SrcType, typename DstType>
	DstType spadas::utility::valueCast(SrcType val)
	{
		return *(DstType*)&val;
	}

	template <typename SrcType, typename DstType>
	Array<DstType> spadas::utility::valueArrayCast(Array<SrcType> src)
	{
		if (src.isEmpty()) return Array<DstType>();
		UInt size = src.size();
		Array<DstType> dst = Array<DstType>::createUninitialized(size);
		for (UInt i = 0; i < size; i++) dst.initialize(i, *(DstType*)&src[i]);
		return dst;
	}

	template <typename Type>
	Array<Type> spadas::utility::unpackKeys(Array<NumericKey<Type> > keys)
	{
		Array<Type> out = Array<Type>::createUninitialized(keys.size());
		for (UInt i = 0; i < keys.size(); i++) out.initialize(i, keys[i].value());
		return out;
	}

	template <typename Type, typename ValueType>
	Array<KeyValue<Type, ValueType> > spadas::utility::unpackKeyValues(Array<KeyValue<NumericKey<Type>, ValueType> > keyValues)
	{
		Array<KeyValue<Type, ValueType> > out = Array<KeyValue<Type, ValueType> >::createUninitialized(keyValues.size());
		for (UInt i = 0; i < keyValues.size(); i++)
		{
			out.initialize(i, KeyValue<Type, ValueType>(keyValues[i].key.value(), keyValues[i].value));
		}
		return out;
	}

	// Implementation of FlexVars functions / FlexVars函数的实现 ///////////////////////////////////////////////////////
	namespace internal
	{
		template <typename Type> class FlexHandler : public EmptyObject, public IFlexHandler
		{
		public:
			void createData(Pointer data) override
			{
				new ((Type*)data)Type();
			}
			void destroyData(Pointer data) override
			{
				((Type*)data)->~Type();
			}
		};
	}

	template <typename Type>
	FlexVars<Type>::FlexVars(Array<Int> validFlagIndices)
	{
		SPADAS_ERROR_RETURN(sizeof(Type) == 0);
		SPADAS_ERROR_RETURN(validFlagIndices.isEmpty());
		data = Binary((UInt)sizeof(Type));
		UInt maxFlagIndex = 0;
		for (UInt i = 0; i < validFlagIndices.size(); i++)
		{
			maxFlagIndex = math::max(maxFlagIndex, (UInt)validFlagIndices[i]);
		}
		flags = Array<ULong>((maxFlagIndex >> 6) + 1, 0);
		for (UInt i = 0; i < validFlagIndices.size(); i++)
		{
			UInt flagIndex = (UInt)validFlagIndices[i];
			flags[flagIndex >> 6] |= 1 << (flagIndex & 0x3f);
		}
		handler = internal::FlexHandler<Type>();
		handler->createData(data.data());
	}
	
	template <typename Type>
	FlexVars<Type>::~FlexVars()
	{
		if (handler.isValid()) handler->destroyData(data.data());
	}

	template <typename Type>
	Bool FlexVars<Type>::valid()
	{
		return handler.isValid();
	}

	template <typename Type>
	Bool FlexVars<Type>::has(Int flagIndex)
	{
		UInt flagIndexUnsigned = (UInt)flagIndex;
		UInt ulongIndex = flagIndexUnsigned >> 6;
		if (ulongIndex >= flags.size()) return FALSE;
		else return flags[ulongIndex] & (1 << (flagIndexUnsigned & 0x3f));
	}

	template <typename Type>
	Type& FlexVars<Type>::cast()
	{
		SPADAS_ERROR_RETURNVAL(handler.isNull(), *(new Type));
		return *(Type*)data.data();
	}

	// Implementation of SampleBuffer functions / SampleBuffer函数的实现 /////////////////////////////////////////////////////////
	template <typename Type>
	Enum<SampleInterpolationResult> SessionSampleBuffer::interpolate(Double offset, Type& interpolatedSample, UInt earlyThresh)
	{
		if (this->isEmpty()) return SampleInterpolationResult::Value::OutOfRange;
		if (!interpolatedSample.template is<SessionSample>()) return SampleInterpolationResult::Value::InvalidType;

		String protocol = getProtocol(FALSE);
		if (protocol.isEmpty()) return SampleInterpolationResult::Value::NoProtocol;

		SessionSample sFirst, sLast;
		this->getEarliest(sFirst);
		this->getLatest(sLast);

		if (offset > sFirst.timestamp().offset && offset <= sLast.timestamp().offset)
		{
			SessionSample sgs1, sgs2;
			if (!this->search(offset, sgs1, sgs2)) return SampleInterpolationResult::Value::OutOfRange;

			Double delta = sgs2.timestamp().offset - sgs1.timestamp().offset;
			Double w1 = (sgs2.timestamp().offset - offset) / delta;
			Double w2 = (offset - sgs1.timestamp().offset) / delta;
			SessionSample interpolatedSampleBase = interpolatedSample.template as<SessionSample>();

			if (!interpolatedSampleBase.supportInterpolation())
			{
				Bool parseResult = interpolatedSampleBase.fromSample(protocol, w1 > w2 ? sgs1 : sgs2);
				if (parseResult) interpolatedSampleBase.timestamp() = w1 > w2 ? sgs1.timestamp() : sgs2.timestamp();
				return parseResult ? SampleInterpolationResult::Value::NearestInstead : SampleInterpolationResult::Value::ParseError;
			}

			Type s1, s2;
			SessionSample s1Base = s1.template as<SessionSample>(), s2Base = s2.template as<SessionSample>();
			Bool parseResultS1 = s1Base.fromSample(protocol, sgs1);
			Bool parseResultS2 = s2Base.fromSample(protocol, sgs2);
			if (!parseResultS1 || !parseResultS2) return SampleInterpolationResult::Value::ParseError;

			FullTimestamp timestamp;
			timestamp.session = this->getCurrentSession();
			timestamp.offset = offset;
			timestamp.offsetSync = sgs1.timestamp().offsetSync == sgs2.timestamp().offsetSync ? sgs1.timestamp().offsetSync : TimeOffsetSync::Value::Interpolated;

			ULong *sgs1Times = (ULong*)&sgs1.timestamp().cpuTick;
			ULong *sgs2Times = (ULong*)&sgs2.timestamp().cpuTick;
			ULong *dstTimes = (ULong*)&timestamp.cpuTick;
			for (UInt i = 0; i < 5; i++)
			{
				ULong sgs1Time = sgs1Times[i], sgs2Time = sgs2Times[i];
				Long diffTime = (Long)sgs2Time - (Long)sgs1Time;
				dstTimes[i] = sgs1Time + (Long)(w2 * diffTime);
			}

			interpolatedSample = interpolatedSampleBase.interpolate(s1Base, w1, s2Base, w2, timestamp).as<Type>();
			interpolatedSample.timestamp() = timestamp;
			return SampleInterpolationResult::Value::OK;
		}
		else if (offset > sLast.timestamp().offset && offset < sLast.timestamp().offset + 0.001 * earlyThresh)
		{
			return SampleInterpolationResult::Value::TooEarly;
		}
		else
		{
			return SampleInterpolationResult::Value::OutOfRange;
		}
	}

	// Implementation of SampleParser functions / SampleParser函数的实现 /////////////////////////////////////////////////////////
    template <typename Type>
    class SampleParserVars : public Vars
    {
    public:
        SPADAS_VARS(SampleParser<Type>, Vars)
        Array<String> protocols;
        Int targetProtocolIndex;
        FullTimestamp lastTimestamp;

		String getProtocol(String fullProtocol)
		{
			Array<UInt> atIndices = fullProtocol.search("@");
			String protocol;
			if (atIndices.isEmpty()) protocol = fullProtocol;
			else
			{
				if (atIndices.size() != 1 || atIndices[0] == 0) {}
				else protocol = fullProtocol.span(0, atIndices[0]).clone();
			}
			return protocol;
		}
    };

    template <typename Type>
	String SampleParser<Type>::typeName() { static String name = (String)"spadas.SampleParser<" + typeid(Type).name() + ">"; return name; }

	template <typename Type>
	SampleParserVars<Type>* SampleParser<Type>::var() { return (SampleParserVars<Type>*)this->vars; }

    template <typename Type>
    SampleParser<Type>::SampleParser()
    {}

    template <typename Type>
    SampleParser<Type>::SampleParser(Array<String> protocols) : BaseObject(new SampleParserVars<Type>())
    {
        this->var()->protocols = protocols.clone();
        this->var()->targetProtocolIndex = -1;
    }

    template <typename Type>
    void SampleParser<Type>::reset()
    {
		if (!this->vars) return;
        this->var()->targetProtocolIndex = -1;
        this->var()->lastTimestamp = FullTimestamp();
    }

    template <typename Type>
    Array<Type> SampleParser<Type>::parse(SessionGeneralSampleTable table)
    {
		SPADAS_ERROR_RETURNVAL(!this->vars, Array<Type>());
        Type testObj;
        SPADAS_ERROR_RETURNVAL(!testObj.template is<SessionSample>(), Array<Type>());
        for (UInt i = 0; i < this->var()->protocols.size(); i++)
        {
            if (table.contain(this->var()->protocols[i]) && !table[this->var()->protocols[i]].isEmpty()) this->var()->targetProtocolIndex = math::max(this->var()->targetProtocolIndex, (Int)i);
        }
        if (this->var()->targetProtocolIndex == -1) return Array<Type>();
        String targetProtocol = this->var()->protocols[this->var()->targetProtocolIndex];
        if (!table.contain(targetProtocol)) return Array<Type>();
		String protocol = this->var()->getProtocol(targetProtocol);
        ArrayX<Type> buffer;
        for (auto e = table[targetProtocol].firstElem(); e.valid(); ++e)
        {
            SessionSample inputSample = e->as<SessionSample>();
            if (inputSample.timestamp() > this->var()->lastTimestamp)
            {
                Type outputSpecialSample;
                SessionSample outputSample = outputSpecialSample.template as<SessionSample>();
                if (outputSample.fromSample(protocol, inputSample))
                {
                    buffer.append(outputSpecialSample);
                    this->var()->lastTimestamp = inputSample.timestamp();
                }
            }
        }
        return buffer.toArray();
    }

    template <typename Type>
    Array<Type> SampleParser<Type>::parse(SessionMatrixSampleTable table)
    {
		SPADAS_ERROR_RETURNVAL(!this->vars, Array<Type>());
        Type testObj;
        SPADAS_ERROR_RETURNVAL(!testObj.template is<SessionSample>(), Array<Type>());
        for (UInt i = 0; i < this->var()->protocols.size(); i++)
        {
            if (table.contain(this->var()->protocols[i]) && !table[this->var()->protocols[i]].isEmpty()) this->var()->targetProtocolIndex = math::max(this->var()->targetProtocolIndex, (Int)i);
        }
        if (this->var()->targetProtocolIndex == -1) return Array<Type>();
        String targetProtocol = this->var()->protocols[this->var()->targetProtocolIndex];
        if (!table.contain(targetProtocol)) return Array<Type>();
		String protocol = this->var()->getProtocol(targetProtocol);
        ArrayX<Type> buffer;
        for (auto e = table[targetProtocol].firstElem(); e.valid(); ++e)
        {
            SessionSample inputSample = e->as<SessionSample>();
            if (inputSample.timestamp() > this->var()->lastTimestamp)
            {
                Type outputSpecialSample;
                SessionSample outputSample = outputSpecialSample.template as<SessionSample>();
                if (outputSample.fromSample(protocol, inputSample))
                {
                    buffer.append(outputSpecialSample);
                    this->var()->lastTimestamp = inputSample.timestamp();
                }
            }
        }
        return buffer.toArray();
    }

	// Implementation of SampleSync functions / SampleSync函数的实现 /////////////////////////////////////////////////////////
    template <typename Type>
    class SampleSyncVars : public Vars
    {
    public:
        SPADAS_VARS(SampleSync<Type>, Vars)
        Array<String> protocols;
        Int targetProtocolIndex;
    };

    template <typename Type>
	String SampleSync<Type>::typeName() { static String name = (String)"spadas.SampleSync<" + typeid(Type).name() + ">"; return name; }

	template <typename Type>
	SampleSyncVars<Type>* SampleSync<Type>::var() { return (SampleSyncVars<Type>*)this->vars; }

    template <typename Type>
    SampleSync<Type>::SampleSync()
    {}

    template <typename Type>
    SampleSync<Type>::SampleSync(Array<String> protocols) : BaseObject(new SampleSyncVars<Type>)
    {
        this->var()->protocols = protocols.clone();
        this->var()->targetProtocolIndex = -1;
    }

    template <typename Type>
    void SampleSync<Type>::reset()
    {
		if (!this->vars) return;
        this->var()->targetProtocolIndex = -1;
    }

    template <typename Type>
    Enum<SampleInterpolationResult> SampleSync<Type>::interpolate(SessionSampleBufferTable table, SessionIdentifier session, Double timeOffset, Type& interpolatedSample, UInt earlyThresh)
    {
		SPADAS_ERROR_RETURNVAL(!this->vars, SampleInterpolationResult::Value::OutOfRange);
        Type testObj;
        SPADAS_ERROR_RETURNVAL(!testObj.template is<SessionSample>(), SampleInterpolationResult::Value::InvalidType);
        for (UInt i = 0; i < this->var()->protocols.size(); i++)
        {
            if (table.contain(this->var()->protocols[i]) && !table[this->var()->protocols[i]].isEmpty()) this->var()->targetProtocolIndex = math::max(this->var()->targetProtocolIndex, (Int)i);
        }
        if (this->var()->targetProtocolIndex == -1) return SampleInterpolationResult::Value::OutOfRange;
        String targetProtocol = this->var()->protocols[this->var()->targetProtocolIndex];
        if (!table.contain(targetProtocol)) return SampleInterpolationResult::Value::OutOfRange;
        auto targetBuffer = table[targetProtocol];
        if (session != targetBuffer.getCurrentSession()) return SampleInterpolationResult::Value::OutOfRange;
        else return targetBuffer.interpolate(timeOffset, interpolatedSample, earlyThresh);
    }
}

#endif
