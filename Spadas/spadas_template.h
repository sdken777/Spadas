
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
	Object<VarsType>::Object(VarsType *newVars, Bool isNew) : vars(newVars)
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
	void Object<VarsType>::setVars(VarsType *newVars, Bool isNew)
	{
		SPADAS_ERROR_RETURN(!newVars);
		if (!isNew) ((Vars*)newVars)->retain();
		if (vars) ((Vars*)vars)->release();
		vars = newVars;
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
	void Safe<Type>::setVars(VarsType* newVars, Bool isNew)
	{
		this->objSpinLock.casSpin(0, 1);
		this->obj.setVars(newVars, isNew);
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
		Vars *vars = (Vars*)this->obj.getVars();
		if (vars && !vars->isSpinLockManaged()) vars->spinEnter();
	}

	template <typename Type>
	SpinLocked<Type>::~SpinLocked()
	{
		Vars *vars = (Vars*)this->obj.getVars();
		if (vars && !vars->isSpinLockManaged()) vars->spinLeave();
	}

	template <typename Type>
	Type *SpinLocked<Type>::operator ->()
	{
		return &this->obj;
	}

	template <typename Type>
	String Interface<Type>::typeName() { static String name = (String)"spadas.Interface<" + typeid(Type).name() + ">"; return name; }

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
		else return this->vars->implementVars == target.vars->implementVars;
	}

	template <typename Type>
	Bool Interface<Type>::operator !=(Interface<Type> target)
	{
		return !(operator ==(target));
	}

	template <typename Type>
	Word Interface<Type>::getHash()
	{
		if (this->vars) return (Word)(((UInt)(PointerInt)this->vars->implementVars & 0x000ffff0) >> 4);
		else return 0;
	}

	template <typename Type>
	Type *Interface<Type>::operator ->()
	{
		SPADAS_ERROR_RETURNVAL(!this->vars, new Type());
		return this->vars->implement;
	}

	template <typename Type>
	BaseObject Interface<Type>::getObject()
	{
		BaseObject obj;
		if (!this->vars) return obj;
		obj.setVars(this->vars->implementVars, FALSE);
		return obj;
	}

	template <typename Type>
	String Optional<Type>::typeName() { static String name = (String)"spadas.Optional<" + typeid(Type).name() + ">"; return name; }

	template <typename Type> class OptionalVars : public Vars
	{
	public:
		SPADAS_VARS(Optional<Type>, Vars)
		Type val;
		OptionalVars(Type& v) : val(v)
		{}
	};

	template <typename Type>
	Optional<Type>::Optional()
	{ }

	template <typename Type>
	Optional<Type>::Optional(Type val) : Object<OptionalVars<Type> >(new OptionalVars<Type>(val), TRUE)
	{ }

	template <typename Type>
	Type Optional<Type>::value()
	{
		SPADAS_ERROR_RETURNVAL(!this->vars, Type());
		return this->vars->val;
	}

	template <typename Type>
	Type Optional<Type>::value(Type defaultValue)
	{
		if (this->vars) return this->vars->val;
		else return defaultValue;
	}

	template <typename Type>
	Type& Optional<Type>::refValue()
	{
		SPADAS_ERROR_RETURNVAL(!this->vars, *(new Type()));
		return this->vars->val;
	}

	template <typename Type>
	Type* Optional<Type>::operator ->()
	{
		SPADAS_ERROR_RETURNVAL(!this->vars, new Type());
		return &this->vars->val;
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

	template <typename Type>
	String Array<Type>::typeName() { static String name = (String)"spadas.Array<" + typeid(Type).name() + ">"; return name; }

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
		return this->vars ? this->vars->data : 0;
	}

	template <typename Type>
	UInt Array<Type>::size()
	{
		return this->vars ? this->vars->size : 0;
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
		else return Array<Type>(this->vars->data, this->vars->size);
	}

	template <typename Type>
	Type& Array<Type>::operator [](UInt index)
	{
		SPADAS_ERROR_RETURNVAL(!this->vars || index >= this->vars->size, *(new Type));
		return this->vars->data[index];
	}

	template <typename Type>
	Type& Array<Type>::first()
	{
		SPADAS_ERROR_RETURNVAL(!this->vars, *(new Type));
		return *this->vars->data;
	}

	template <typename Type>
	Type& Array<Type>::last()
	{
		SPADAS_ERROR_RETURNVAL(!this->vars, *(new Type));
		return this->vars->data[this->vars->size - 1];
	}

	template <typename Type>
	Array<Type> Array<Type>::operator +(Array<Type> arr)
	{
		if (!this->vars) return arr.clone();
		else return internal::arrayCommonPlus<Type>(this->vars->data, this->vars->size, arr);
	}

	template <typename Type>
	Array<Type>& Array<Type>::trim(UInt size)
	{
		SPADAS_ERROR_RETURNVAL(size == 0, *this);
		if (this->vars && size < this->vars->size)
		{
			if (!__is_trivial(Type))
			{
				for (UInt i = math::max(size, this->vars->reserveSize); i < this->vars->size; i++)
				{
					(&this->vars->data[i])->~Type();
				}
			}
			this->vars->size = size;
		}
		return *this;
	}

	template <typename Type>
	void Array<Type>::set(Type value)
	{
		if (!this->vars) return;
		else internal::arrayCommonSet<Type>(this->vars->data, this->vars->size, value);
	}

	template <typename Type>
	Bool Array<Type>::contain(Type val)
	{
		if (!this->vars) return FALSE;
		else
		{
			Func<Bool(Type&)> func = [val](Type& v){ return v == val; };
			return internal::arrayCommonContain<Type>(this->vars->data, this->vars->size, func);
		}
	}

	template <typename Type>
	Bool Array<Type>::containAs(Func<Bool(Type&)> func)
	{
		if (!this->vars) return FALSE;
		else return internal::arrayCommonContain<Type>(this->vars->data, this->vars->size, func);
	}

	template <typename Type>
	Array<UInt> Array<Type>::search(Type val)
	{
		if (!this->vars) return Array<UInt>();
		else
		{
			Func<Bool(Type&)> func = [val](Type& v){ return v == val; };
			return internal::arrayCommonSearch<Type>(this->vars->data, this->vars->size, func);
		}
	}

	template <typename Type>
	Array<UInt> Array<Type>::searchAs(Func<Bool(Type&)> func)
	{
		if (!this->vars) return Array<UInt>();
		else return internal::arrayCommonSearch<Type>(this->vars->data, this->vars->size, func);
	}

	template <typename Type>
	void Array<Type>::sort()
	{
		if (!this->vars) return;
		else
		{
			Func<Bool(Type&, Type&)> func = [](Type& a, Type& b){ return a > b; };
			internal::arrayCommonSort<Type>(this->vars->data, this->vars->size, func);
		}
	}

	template <typename Type>
	void Array<Type>::sortAs(Func<Bool(Type&, Type&)> func)
	{
		if (!this->vars) return;
		else internal::arrayCommonSort<Type>(this->vars->data, this->vars->size, func);
	}

	template <typename Type>
	template <typename TargetType>
	Array<TargetType> Array<Type>::convert(Func<TargetType(Type&)> func)
	{
		if (!this->vars) return Array<TargetType>();
		else return internal::arrayCommonConvert<Type, TargetType>(this->vars->data, this->vars->size, func);
	}

	template <typename Type>
	Array<ArraySpan<Type> > Array<Type>::split(Array<UInt> sizes)
	{
		SPADAS_ERROR_RETURNVAL(!this->vars, Array<ArraySpan<Type> >());
		this->vars->reserveSize = math::max(this->vars->reserveSize, this->vars->size);
		return internal::arrayCommonSplit(this->vars->data, this->vars->size, sizes, this->vars);
	}

	template <typename Type>
	template <typename TargetType>
	Array<TargetType> Array<Type>::asArray()
	{
		if (!this->vars) return Array<TargetType>();
		else return internal::arrayCommonAsArray<Type, TargetType>(this->vars->data, this->vars->size);
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
		else return internal::arrayCommonAsArray<Type, TargetType>(this->vars->data, this->vars->size, ok);
	}

	template <typename Type>
	ArraySpan<Type> Array<Type>::span(UInt index, UInt size)
	{
		if (this->vars) this->vars->reserveSize = math::max(this->vars->reserveSize, this->vars->size);
		return ArraySpan<Type>(*this, index, size);
	}

	template <typename Type>
	ArrayElem<Type> Array<Type>::firstElem()
	{
		if (this->vars) this->vars->reserveSize = math::max(this->vars->reserveSize, this->vars->size);
		return ArrayElem<Type>(*this, 0);
	}

	template <typename Type>
	ArrayElem<Type> Array<Type>::lastElem()
	{
		if (this->vars) this->vars->reserveSize = math::max(this->vars->reserveSize, this->vars->size);
		return ArrayElem<Type>(*this, this->vars ? (this->vars->size - 1) : UINF);
	}

	template <typename Type>
	void Array<Type>::copy(Array<Type> src, Region srcRegion, UInt thisOffset)
	{
		SPADAS_ERROR_RETURN(!this->vars || !src.vars || this->vars == src.vars);
		UInt srcSize = src.vars->size;
		SPADAS_ERROR_RETURN(srcRegion.offset < 0 || srcRegion.offset + srcRegion.size > srcSize);
		SPADAS_ERROR_RETURN(thisOffset + srcRegion.size > this->vars->size);
		Type *srcData = src.vars->data;
		Type *dstData = this->vars->data;
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
		new (&this->vars->data[index])Type(val);
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
			Type *srcData = arr.vars->data;
			UInt copySize = arr.vars->size;
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
		SPADAS_ERROR_RETURN(ptr == NULL);
		SPADAS_ERROR_RETURN(size == 0);
		this->source = (ULong)ptr;
		this->siz = size;
	}

	template <typename Type>
	ArraySpan<Type>::ArraySpan(Array<Type>& sourceArray, UInt offset, UInt size) : source(0), idx(0), siz(0)
	{
		ArrayVars<Type> *vars = sourceArray.getVars();
		SPADAS_ERROR_RETURN(size == 0);
		SPADAS_ERROR_RETURN(!vars);
		SPADAS_ERROR_RETURN(offset >= vars->size);
		vars->retain();
		this->source = (ULong)vars;
		this->idx = (UInt)((ULong)(vars->data + offset) - this->source);
		this->siz = math::min(size, vars->size - offset);
	}

	template <typename Type>
	ArraySpan<Type>::ArraySpan(ArraySpan<Type>& sourceSpan, UInt offset, UInt size) : source(0), idx(0), siz(0)
	{
		SPADAS_ERROR_RETURN(size == 0);
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
		if (this->source) return internal::arrayCommonContain<Type>((Type*)(this->source + this->idx), this->siz, func);
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
		if (this->source) return internal::arrayCommonSearch<Type>((Type*)(this->source + this->idx), this->siz, func);
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
		if (this->source) internal::arrayCommonSort<Type>((Type*)(this->source + this->idx), this->siz, func);
	}

	template <typename Type>
	template <typename TargetType>
	Array<TargetType> ArraySpan<Type>::convert(Func<TargetType(Type&)> func)
	{
		if (this->source) return internal::arrayCommonConvert<Type, TargetType>((Type*)(this->source + this->idx), this->siz, func);
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
	ListNode<Type>::ListNode() : Object<ListNodeVars<Type> >(new ListNodeVars<Type>(), TRUE)
	{ }

	template <typename Type>
	ListNode<Type>::ListNode(Type val) : Object<ListNodeVars<Type> >(new ListNodeVars<Type>(val), TRUE)
	{ }

	template <typename Type>
	ListNode<Type>::ListNode(ListNodeVars<Type> *vars0) : Object<ListNodeVars<Type> >(vars0, FALSE)
	{ }

	template <typename Type>
	Type& ListNode<Type>::value()
	{
		return this->vars->val;
	}

	template <typename Type>
	Type* ListNode<Type>::operator ->()
	{
		return &this->vars->val;
	}

	template <typename Type>
	Bool ListNode<Type>::hasNext()
	{
		return this->vars->nextVars != 0;
	}

	template <typename Type>
	Bool ListNode<Type>::hasPrevious()
	{
		return this->vars->previousVars != 0;
	}

	template <typename Type>
	ListNode<Type> ListNode<Type>::next()
	{
		if (this->vars->nextVars == 0) return *this;
		else return ListNode<Type>(this->vars->nextVars);
	}

	template <typename Type>
	ListNode<Type> ListNode<Type>::previous()
	{
		if (this->vars->previousVars == 0) return *this;
		else return ListNode<Type>(this->vars->previousVars);
	}

	template <typename Type>
	void ListNode<Type>::goNext()
	{
		if (this->vars->nextVars != 0) this->setVars(this->vars->nextVars, FALSE);
	}

	template <typename Type>
	void ListNode<Type>::goPrevious()
	{
		if (this->vars->previousVars != 0) this->setVars(this->vars->previousVars, FALSE);
	}

	template <typename Type>
	void ListNode<Type>::cutNext()
	{
		if (this->vars->nextVars != 0)
		{
			ListNode<Type> nextNode(this->vars->nextVars);
			this->vars->nextVars = 0;
			this->vars->release();
			nextNode.vars->previousVars = 0;
			nextNode.vars->release();
		}
	}

	template <typename Type>
	void ListNode<Type>::cutPrevious()
	{
		if (this->vars->previousVars != 0)
		{
			ListNode<Type> previousNode(this->vars->previousVars);
			this->vars->previousVars = 0;
			this->vars->release();
			previousNode.vars->nextVars = 0;
			previousNode.vars->release();
		}
	}

	template <typename Type>
	ListNode<Type> ListNode<Type>::joinNext(Type val)
	{
		cutNext();
		ListNode<Type> node(val);
		this->vars->nextVars = node.vars;
		this->vars->retain();
		node.vars->previousVars = this->vars;
		node.vars->retain();
		return node;
	}

	template <typename Type>
	void ListNode<Type>::joinNext(ListNode<Type> node)
	{
		cutNext();
		node.cutPrevious();
		this->vars->nextVars = node.vars;
		this->vars->retain();
		node.vars->previousVars = this->vars;
		node.vars->retain();
	}

	template <typename Type>
	ListNode<Type> ListNode<Type>::joinPrevious(Type val)
	{
		cutPrevious();
		ListNode<Type> node(val);
		this->vars->previousVars = node.vars;
		this->vars->retain();
		node.vars->nextVars = this->vars;
		node.vars->retain();
		return node;
	}

	template <typename Type>
	void ListNode<Type>::joinPrevious(ListNode<Type> node)
	{
		cutPrevious();
		node.cutNext();
		this->vars->previousVars = node.vars;
		this->vars->retain();
		node.vars->nextVars = this->vars;
		node.vars->retain();
	}

	template <typename Type>
	ListNode<Type> ListNode<Type>::insertNext(Type val)
	{
		ListNode<Type> node(val);
		if (this->vars->nextVars == 0)
		{
			this->vars->nextVars = node.vars;
			this->vars->retain();
			node.vars->previousVars = this->vars;
			node.vars->retain();
		}
		else
		{
			ListNodeVars<Type> *nextNextVars = this->vars->nextVars;
			nextNextVars->previousVars = node.vars;
			this->vars->nextVars = node.vars;
			node.vars->nextVars = nextNextVars;
			node.vars->previousVars = this->vars;
			node.vars->retain();
			node.vars->retain();
		}
		return node;
	}

	template <typename Type>
	void ListNode<Type>::insertNext(ListNode<Type> node)
	{
		node.cutPrevious();
		node.cutNext();
		if (this->vars->nextVars == 0)
		{
			this->vars->nextVars = node.vars;
			this->vars->retain();
			node.vars->previousVars = this->vars;
			node.vars->retain();
		}
		else
		{
			ListNodeVars<Type> *nextNextVars = this->vars->nextVars;
			nextNextVars->previousVars = node.vars;
			this->vars->nextVars = node.vars;
			node.vars->nextVars = nextNextVars;
			node.vars->previousVars = this->vars;
			node.vars->retain();
			node.vars->retain();
		}
	}

	template <typename Type>
	ListNode<Type> ListNode<Type>::insertPrevious(Type val)
	{
		ListNode<Type> node(val);
		if (this->vars->previousVars == 0)
		{
			this->vars->previousVars = node.vars;
			this->vars->retain();
			node.vars->nextVars = this->vars;
			node.vars->retain();
		}
		else
		{
			ListNodeVars<Type> *prevPrevVars = this->vars->previousVars;
			prevPrevVars->nextVars = node.vars;
			this->vars->previousVars = node.vars;
			node.vars->previousVars = prevPrevVars;
			node.vars->nextVars = this->vars;
			node.vars->retain();
			node.vars->retain();
		}
		return node;
	}

	template <typename Type>
	void ListNode<Type>::insertPrevious(ListNode<Type> node)
	{
		node.cutPrevious();
		node.cutNext();
		if (this->vars->previousVars == 0)
		{
			this->vars->previousVars = node.vars;
			this->vars->retain();
			node.vars->nextVars = this->vars;
			node.vars->retain();
		}
		else
		{
			ListNodeVars<Type> *prevPrevVars = this->vars->previousVars;
			prevPrevVars->nextVars = node.vars;
			this->vars->previousVars = node.vars;
			node.vars->previousVars = prevPrevVars;
			node.vars->nextVars = this->vars;
			node.vars->retain();
			node.vars->retain();
		}
	}

	template <typename Type>
	void ListNode<Type>::removeNext()
	{
		if (this->vars->nextVars != 0)
		{
			ListNode<Type> nextNode(this->vars->nextVars);
			nextNode.removeSelf();
		}
	}

	template <typename Type>
	void ListNode<Type>::removePrevious()
	{
		if (this->vars->previousVars != 0)
		{
			ListNode<Type> previousNode(this->vars->previousVars);
			previousNode.removeSelf();
		}
	}

	template <typename Type>
	void ListNode<Type>::removeSelf()
	{
		if (this->vars->nextVars == 0)
		{
			if (this->vars->previousVars != 0)
			{
				ListNode<Type> previousNode(this->vars->previousVars);
				this->vars->previousVars = 0;
				this->vars->release();
				previousNode.vars->nextVars = 0;
				previousNode.vars->release();
			}
		}
		else //vars->nextVars != 0
		{
			if (this->vars->previousVars == 0)
			{
				ListNode<Type> nextNode(this->vars->nextVars);
				this->vars->nextVars = 0;
				this->vars->release();
				nextNode.vars->previousVars = 0;
				nextNode.vars->release();
			}
			else //vars->previousVars != 0
			{
				ListNode<Type> previousNode(this->vars->previousVars), nextNode(this->vars->nextVars);
				previousNode.vars->nextVars = nextNode.vars;
				nextNode.vars->previousVars = previousNode.vars;
				this->vars->nextVars = 0;
				this->vars->previousVars = 0;
				this->vars->release();
				this->vars->release();
			}
		}
	}

	template <typename Type>
	ListNode<Type> ListNode<Type>::cloneList()
	{
		ListNode<Type> dst(this->vars->val);
		if (this->vars->nextVars == this->vars)
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
			if (currentNode.vars->nextVars == 0) break;
			ListNode<Type> nextNode(currentNode.vars->nextVars);
			nextNode.vars->previousVars = 0;
			nextNode.vars->release();
			currentNode.vars->nextVars = 0;
			currentNode.vars->release();
			currentNode = nextNode;
		}
		currentNode = ListNode<Type>(*this);
		while (TRUE)
		{
			if (currentNode.vars->previousVars == 0) break;
			ListNode<Type> prevNode(currentNode.vars->previousVars);
			prevNode.vars->nextVars = 0;
			prevNode.vars->release();
			currentNode.vars->previousVars = 0;
			currentNode.vars->release();
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
	TreeNode<Type>::TreeNode() : Object<TreeNodeVars<Type> >(new TreeNodeVars<Type>(), TRUE)
	{ }

	template <typename Type>
	TreeNode<Type>::TreeNode(Type val) : Object<TreeNodeVars<Type> >(new TreeNodeVars<Type>(val), TRUE)
	{ }

	template <typename Type>
	TreeNode<Type>::TreeNode(TreeNodeVars<Type> *vars0) : Object<TreeNodeVars<Type> >(vars0, FALSE)
	{ }

	template <typename Type>
	Type& TreeNode<Type>::value()
	{
		return this->vars->val;
	}

	template <typename Type>
	Type* TreeNode<Type>::operator ->()
	{
		return &this->vars->val;
	}

	template <typename Type>
	TreeNode<Type> TreeNode<Type>::root()
	{
		if (this->vars->rootVars == 0) return *this;
		else return TreeNode<Type>(this->vars->rootVars);
	}

	template <typename Type>
	Array<TreeNode<Type> > TreeNode<Type>::leaves()
	{
		Array<TreeNode<Type> > out = Array<TreeNode<Type> >::createUninitialized(this->vars->nLeaves);
		TreeNodeVars<Type> *leafVars = this->vars->leafFirstVars;
		for (UInt i = 0; i < this->vars->nLeaves; i++)
		{
			out.initialize(i, TreeNode<Type>(leafVars));
			leafVars = leafVars->nextVars;
		}
		return out;
	}

	template <typename Type>
	TreeNode<Type> TreeNode<Type>::leafAt(UInt index)
	{
		SPADAS_ERROR_RETURNVAL(index >= this->vars->nLeaves, *this);
		TreeNodeVars<Type>* leafVars = this->vars->leafFirstVars;
		for (UInt i = 0; i < index; i++) leafVars = leafVars->nextVars;
		return TreeNode<Type>(leafVars);
	}

	template <typename Type>
	UInt TreeNode<Type>::nLeaves()
	{
		return this->vars->nLeaves;
	}

	template <typename Type>
	Bool TreeNode<Type>::isLeaf(TreeNode<Type> node)
	{
		TreeNodeVars<Type> *leafVars = this->vars->leafFirstVars;
		while (leafVars)
		{
			if (leafVars == node.vars) return TRUE;
			leafVars = leafVars->nextVars;
		}
		return FALSE;
	}

	template <typename Type>
	TreeNode<Type> TreeNode<Type>::addLeaf(Type val)
	{
		TreeNode<Type> leafNode(val);
		if (this->vars->leafFirstVars == 0)
		{
			this->vars->leafFirstVars = leafNode.vars;
		}
		else
		{
			this->vars->leafLastVars->nextVars = leafNode.vars;
			leafNode.vars->prevVars = this->vars->leafLastVars;
		}
		this->vars->leafLastVars = leafNode.vars;
		leafNode.vars->rootVars = this->vars;
		leafNode.vars->retain();
		leafNode.vars->retain();
		this->vars->retain();
		this->vars->nLeaves++;
		return leafNode;
	}

	template <typename Type>
	void TreeNode<Type>::addLeaf(TreeNode<Type> leafNode)
	{
		TreeNodeVars<Type> *currentVars = this->vars;
		while (TRUE)
		{
			SPADAS_ERROR_RETURN(currentVars == leafNode.vars);
			if (currentVars->rootVars == 0) break;
			else currentVars = currentVars->rootVars;
		}
		leafNode.cutRoot();
		if (this->vars->leafFirstVars == 0)
		{
			this->vars->leafFirstVars = leafNode.vars;
		}
		else
		{
			this->vars->leafLastVars->nextVars = leafNode.vars;
			leafNode.vars->prevVars = this->vars->leafLastVars;
		}
		this->vars->leafLastVars = leafNode.vars;
		leafNode.vars->rootVars = this->vars;
		leafNode.vars->retain();
		leafNode.vars->retain();
		this->vars->retain();
		this->vars->nLeaves++;
	}

	template <typename Type>
	void TreeNode<Type>::dropLeaf(TreeNode<Type> leafNode)
	{
		if (this->vars->nLeaves == 0) return;
		Bool found = FALSE;
		TreeNodeVars<Type>* targetVars = this->vars->leafFirstVars;
		while (TRUE)
		{
			if (targetVars == leafNode.vars)
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
				this->vars->leafFirstVars = this->vars->leafLastVars = 0;
			}
			else //targetVars->prevVars != 0
			{
				this->vars->leafLastVars = targetVars->prevVars;
				targetVars->prevVars->nextVars = 0;
				targetVars->prevVars = 0;
			}
		}
		else //targetVars->nextVars != 0
		{
			if (this->vars->prevVars == 0)
			{
				this->vars->leafFirstVars = targetVars->nextVars;
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
		this->vars->release();
		targetVars->release();
		targetVars->release();
		this->vars->nLeaves--;
	}

	template <typename Type>
	void TreeNode<Type>::joinRoot(TreeNode<Type> rootNode)
	{
		rootNode.addLeaf(*this);
	}

	template <typename Type>
	void TreeNode<Type>::cutRoot()
	{
		if (this->vars->rootVars)
		{
			TreeNode<Type> rootNode(this->vars->rootVars);
			rootNode.dropLeaf(*this);
		}
	}

	template <typename Type>
	TreeNode<Type> TreeNode<Type>::globalRoot()
	{
		TreeNodeVars<Type> *currentVars = this->vars;
		while (currentVars->rootVars) currentVars = currentVars->rootVars;
		return TreeNode<Type>(currentVars);
	}

	template <typename Type>
	Array<TreeNode<Type> > TreeNode<Type>::rootChain()
	{
		UInt depth0 = depth();
		TreeNodeVars<Type> *currentVars = this->vars;
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
		TreeNodeVars<Type> *currentVars = this->vars;
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
		TreeNode<Type> node(this->vars->val);
		Array<TreeNode<Type> > srcLeaves = leaves();
		for (UInt i = 0; i < this->vars->nLeaves; i++)
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
		if (this->vars->nLeaves == 0) return;
		Array<TreeNode<Type> > leafNodes = leaves();
		this->vars->leafFirstVars->release();
		this->vars->leafLastVars->release();
		this->vars->leafFirstVars = this->vars->leafLastVars = 0;
		for (UInt i = 0; i < this->vars->nLeaves; i++)
		{
			TreeNode<Type> leafNode = leafNodes[i];
			leafNode.collapseSub();
			if (leafNode.vars->nextVars)
			{
				leafNode.vars->nextVars->release();
				leafNode.vars->nextVars = 0;
			}
			if (leafNode.vars->prevVars)
			{
				leafNode.vars->prevVars->release();
				leafNode.vars->prevVars = 0;
			}
			leafNode.vars->rootVars = 0;
			this->vars->release();
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
	GraphNode<NType, LType>::GraphNode() : Object<GraphNodeVars<NType, LType> >(new GraphNodeVars<NType, LType>(), TRUE)
	{ }

	template <typename NType, typename LType>
	GraphNode<NType, LType>::GraphNode(NType val) : Object<GraphNodeVars<NType, LType> >(new GraphNodeVars<NType, LType>(val), TRUE)
	{ }

	template <typename NType, typename LType>
	GraphNode<NType, LType>::GraphNode(GraphNodeVars<NType, LType> *vars0) : Object<GraphNodeVars<NType, LType> >(vars0, FALSE)
	{ }

	template <typename NType, typename LType>
	NType& GraphNode<NType, LType>::value()
	{
		return this->vars->val;
	}

	template <typename NType, typename LType>
	NType* GraphNode<NType, LType>::operator ->()
	{
		return &this->vars->val;
	}

	template <typename NType, typename LType>
	UInt GraphNode<NType, LType>::nLinks()
	{
		return this->vars->nLinks;
	}

	template <typename NType, typename LType>
	LType& GraphNode<NType, LType>::linkAt(UInt index)
	{
		SPADAS_ERROR_RETURNVAL(index >= this->vars->nLinks, *(new LType));
		internal::GraphLink<NType, LType> *linkInfo = this->vars->links;
		for (UInt i = 0; i < index; i++) linkInfo = linkInfo->next;
		return linkInfo->link;
	}

	template <typename NType, typename LType>
	LType& GraphNode<NType, LType>::linkWith(GraphNode<NType, LType> node)
	{
		internal::GraphLink<NType, LType> *linkInfo = this->vars->links;
		if (linkInfo != 0)
		{
			for (UInt i = 0; i < this->vars->nLinks; i++)
			{
				if (linkInfo->target == node.vars) return linkInfo->link;
				linkInfo = linkInfo->next;
			}
		}
		SPADAS_ERROR_MSG("No linkInfo->target == node.vars");
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
		if (index >= this->vars->nLinks)
		{
			SPADAS_ERROR_MSG("index >= this->vars->nLinks");
			return GraphNode<NType, LType>();
		}
		internal::GraphLink<NType, LType> *linkInfo = this->vars->links;
		for (UInt i = 0; i < index; i++) linkInfo = linkInfo->next;
		link = linkInfo->link;
		return GraphNode<NType, LType>(linkInfo->target);
	}

	template <typename NType, typename LType>
	UInt GraphNode<NType, LType>::indexOf(GraphNode<NType, LType> node)
	{
		internal::GraphLink<NType, LType> *linkInfo = this->vars->links;
		if (linkInfo == 0) return UINF;
		for (UInt i = 0; i < this->vars->nLinks; i++)
		{
			if (linkInfo->target == node.vars) return i;
			linkInfo = linkInfo->next;
		}
		return UINF;
	}

	template <typename NType, typename LType>
	void GraphNode<NType, LType>::addLinkWith(GraphNode<NType, LType> dst, LType linkToDst, LType linkFromDst)
	{
		internal::GraphLink<NType, LType> *linkInfo = this->vars->links, *previousLinkInfo = 0;
		while (linkInfo)
		{
			if (linkInfo->target == dst.vars)
			{
				linkInfo->link = linkToDst;
				linkInfo = dst.vars->links;
				while (linkInfo)
				{
					if (linkInfo->target == this->vars)
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
		internal::GraphLink<NType, LType> *newLink = new internal::GraphLink<NType, LType>(dst.vars, linkToDst);
		dst.vars->retain();
		if (!previousLinkInfo) this->vars->links = newLink;
		else previousLinkInfo->next = newLink;
		this->vars->nLinks++;
		linkInfo = dst.vars->links;
		previousLinkInfo = 0;
		while (linkInfo)
		{
			previousLinkInfo = linkInfo;
			linkInfo = linkInfo->next;
		}
		newLink = new internal::GraphLink<NType, LType>(this->vars, linkFromDst);
		this->vars->retain();
		if (!previousLinkInfo) dst.vars->links = newLink;
		else previousLinkInfo->next = newLink;
		dst.vars->nLinks++;
	}

	template <typename NType, typename LType>
	void GraphNode<NType, LType>::cutLinkWith(GraphNode<NType, LType> node)
	{
		internal::GraphLink<NType, LType> *linkInfo = this->vars->links, *previousLinkInfo = 0;
		if (linkInfo == 0) return;
		for (UInt i = 0; i < this->vars->nLinks; i++)
		{
			if (linkInfo->target == node.vars)
			{
				if (previousLinkInfo) previousLinkInfo->next = linkInfo->next;
				else this->vars->links = linkInfo->next;
				this->vars->nLinks--;
				delete linkInfo;
				node.vars->release();
				linkInfo = node.vars->links;
				previousLinkInfo = 0;
				for (UInt j = 0; j < node.vars->nLinks; j++)
				{
					if (linkInfo->target == this->vars)
					{
						if (previousLinkInfo) previousLinkInfo->next = linkInfo->next;
						else node.vars->links = linkInfo->next;
						node.vars->nLinks--;
						delete linkInfo;
						this->vars->release();
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
		UInt nLinks = this->vars->nLinks;
		if (nLinks == 0) return;
		GraphNode<NType, LType> **linkedNodes = new GraphNode<NType, LType>*[nLinks];
		for (UInt i = 0; i < nLinks; i++)
		{
			linkedNodes[i] = new GraphNode<NType, LType>(this->vars->links->target);
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
		};
	}

	template <typename Type>
	String ArrayX<Type>::typeName() { static String name = (String)"spadas.ArrayX<" + typeid(Type).name() + ">"; return name; }

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
		internal::ArrayXNode<Type>* currentNode = &this->vars->root;
		for (Int i = depth - 1; i >= 0; i--)
		{
			if (!currentNode->children[0])
			{
				Byte* leaf0Data = new Byte[sizeof(internal::ArrayXNode<Type>) + sizeof(Type) * this->vars->segmentSize];
				Byte* leaf1Data = new Byte[sizeof(internal::ArrayXNode<Type>) + sizeof(Type) * this->vars->segmentSize];
				internal::ArrayXNode<Type>* leaf0Node = 0;
				internal::ArrayXNode<Type>* leaf1Node = 0;
				if (this->vars->withDefault)
				{
					leaf0Node = new (leaf0Data)internal::ArrayXNode<Type>((Type*)&leaf0Data[sizeof(internal::ArrayXNode<Type>)], this->vars->segmentSize, *this->vars->defaultValue);
					leaf1Node = new (leaf1Data)internal::ArrayXNode<Type>((Type*)&leaf1Data[sizeof(internal::ArrayXNode<Type>)], this->vars->segmentSize, *this->vars->defaultValue);
				}
				else
				{
					leaf0Node = new (leaf0Data)internal::ArrayXNode<Type>((Type*)&leaf0Data[sizeof(internal::ArrayXNode<Type>)], this->vars->segmentSize);
					leaf1Node = new (leaf1Data)internal::ArrayXNode<Type>((Type*)&leaf1Data[sizeof(internal::ArrayXNode<Type>)], this->vars->segmentSize);
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
		UInt segment = (index >> this->vars->segmentSizePower) + 1;
		UInt localIndex = index & this->vars->segmentSizeMask;
		if (segment != this->vars->accessingSegment)
		{
			SPADAS_ERROR_RETURNVAL(index >= ARRAYX_SIZE_LIMIT, *(new Type));
			this->vars->accessingSegment = segment;
			this->vars->accessingNode = (internal::ArrayXNode<Type>*)getSegmentNode(segment);
		}
		if (index >= this->vars->size)
		{
			this->vars->size = index + 1;
			this->vars->lastSegment = segment;
			this->vars->lastNextIndex = localIndex + 1;
			this->vars->lastNode = this->vars->accessingNode;
		}
		internal::ArrayXNode<Type>& targetNode = *this->vars->accessingNode;
		if (!__is_trivial(Type) && localIndex >= targetNode.initialized)
		{
			for (UInt i = targetNode.initialized; i < this->vars->segmentSize; i++)
			{
				new (&targetNode.buffer[i])Type();
			}
			targetNode.initialized = this->vars->segmentSize;
		}
		return targetNode.buffer[localIndex];
	}

	template <typename Type>
	Bool ArrayX<Type>::isEmpty()
	{
		return !this->vars || this->vars->size == 0;
	}

	template <typename Type>
	UInt ArrayX<Type>::size()
	{
		return this->vars ? this->vars->size : 0;
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
		if (size == this->vars->size) return;
		SPADAS_ERROR_RETURN(size < this->vars->size || size > ARRAYX_SIZE_LIMIT);
		UInt index = size - 1;
		UInt segment = (index >> this->vars->segmentSizePower) + 1;
		UInt localIndex = index & this->vars->segmentSizeMask;
		this->vars->lastSegment = segment;
		if (segment == this->vars->accessingSegment)
		{
			this->vars->lastNode = this->vars->accessingNode;
		}
		else
		{
			this->vars->accessingSegment = segment;
			this->vars->lastNode = this->vars->accessingNode = (internal::ArrayXNode<Type>*)getSegmentNode(segment);
		}
		this->vars->lastNextIndex = localIndex + 1;
		this->vars->size = size;
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
		if (this->vars->lastNextIndex >= this->vars->segmentSize)
		{
			SPADAS_ERROR_RETURNVAL(this->vars->size >= ARRAYX_SIZE_LIMIT, *(new Type));
			UInt segment = (this->vars->size >> this->vars->segmentSizePower) + 1;
			this->vars->lastSegment = this->vars->accessingSegment = segment;
			this->vars->lastNode = this->vars->accessingNode = (internal::ArrayXNode<Type>*)getSegmentNode(segment);
			this->vars->lastNextIndex = 0;
		}
		internal::ArrayXNode<Type>& targetNode = *this->vars->lastNode;
		if (__is_trivial(Type) || this->vars->withDefault)
		{
			this->vars->size++;
			Type& valRef = targetNode.buffer[this->vars->lastNextIndex++];
			valRef = val;
			return valRef;
		}
		else
		{
			this->vars->size++;
			Type& valRef = targetNode.buffer[this->vars->lastNextIndex++];
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
		copyRegion(&this->vars->root, 1, out, thisRegion.offset, lastIndex, (thisRegion.offset >> this->vars->segmentSizePower) + 1, (lastIndex >> this->vars->segmentSizePower) + 1);
		return out;
	}

	template <typename Type>
	void ArrayX<Type>::copyRegion(Pointer nodePtr, UInt segment, Array<Type>& dst, UInt firstIndex, UInt lastIndex, UInt firstSegment, UInt lastSegment)
	{
		internal::ArrayXNode<Type>* node = (internal::ArrayXNode<Type>*)nodePtr;
		if (segment >= firstSegment && segment <= lastSegment)
		{
			UInt startIndex = 0, endIndex = this->vars->segmentSize - 1;
			if (segment == firstSegment) startIndex = firstIndex & this->vars->segmentSizeMask;
			if (segment == lastSegment) endIndex = lastIndex & this->vars->segmentSizeMask;
			Int indexOffset = (Int)(segment - 1) * (Int)this->vars->segmentSize - (Int)firstIndex;
			if (node)
			{
				if (!__is_trivial(Type) && node->initialized < this->vars->segmentSize)
				{
					for (UInt i = startIndex; i < node->initialized; i++) dst.initialize((UInt)(indexOffset + i), node->buffer[i]);
					for (UInt i = node->initialized; i <= endIndex; i++) dst.initialize((UInt)(indexOffset + i), Type());
				}
				else
				{
					for (UInt i = startIndex; i <= endIndex; i++) dst.initialize((UInt)(indexOffset + i), node->buffer[i]);
				}
			}
			else if (this->vars->withDefault)
			{
				for (UInt i = startIndex; i <= endIndex; i++) dst.initialize((UInt)(indexOffset + i), *this->vars->defaultValue);
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

	template <typename Type>
	String List<Type>::typeName() { static String name = (String)"spadas.List<" + typeid(Type).name() + ">"; return name; }
 
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
		return !this->vars || this->vars->size == 0;
	}

	template <typename Type>
	UInt List<Type>::size()
	{
		return this->vars ? this->vars->size : 0;
	}

	template <typename Type>
	Type& List<Type>::head()
	{
		SPADAS_ERROR_RETURNVAL(!this->vars->head, *(new Type));
		return this->vars->head->val;
	}

	template <typename Type>
	Type& List<Type>::tail()
	{
		SPADAS_ERROR_RETURNVAL(!this->vars->tail, *(new Type));
		return this->vars->tail->val;
	}

	template <typename Type>
	ListElem<Type> List<Type>::headElem()
	{
		if (this->vars && this->vars->size > 0)
		{
			SPADAS_ERROR_RETURNVAL(this->vars->elemRefs > 0, ListElem<Type>(List<Type>(), 0, UINF));
			return ListElem<Type>(*this, this->vars->head, 0);
		}
		return ListElem<Type>(List<Type>(), 0, UINF);
	}

	template <typename Type>
	ListElem<Type> List<Type>::tailElem()
	{
		if (this->vars && this->vars->size > 0)
		{
			SPADAS_ERROR_RETURNVAL(this->vars->elemRefs > 0, ListElem<Type>(List<Type>(), 0, UINF));
			return ListElem<Type>(*this, this->vars->tail, this->vars->size - 1);
		}
		return ListElem<Type>(List<Type>(), 0, UINF);
	}

	template <typename Type>
	Type& List<Type>::addToHead(Type val)
	{
		if (!this->vars) this->setVars(new ListVars<Type>(), TRUE);
		else SPADAS_ERROR_RETURNVAL(this->vars->elemRefs > 0, *(new Type));
		if (this->vars->lastSeg->used == internal::LIST_SEGMENT_SIZE)
		{
			internal::ListSegment<Type> *newSeg = this->vars->createSegment();
			newSeg->prev = this->vars->lastSeg;
			this->vars->lastSeg->next = newSeg;
			this->vars->lastSeg = newSeg;
		}
		internal::ListCell<Type> *targetCell = this->vars->lastSeg->buffer + this->vars->lastSeg->used++;
		new (targetCell)internal::ListCell<Type>(val, this->vars->lastSeg);
		if (this->vars->size > 0)
		{
			targetCell->next = this->vars->head;
			this->vars->head->prev = targetCell;
			this->vars->head = targetCell;
		}
		else
		{
			this->vars->head = this->vars->tail = targetCell;
		}
		this->vars->size++;
		return targetCell->val;
	}

	template <typename Type>
	Type& List<Type>::addToTail(Type val)
	{
		if (!this->vars) this->setVars(new ListVars<Type>(), TRUE);
		else SPADAS_ERROR_RETURNVAL(this->vars->elemRefs > 0, *(new Type));
		if (this->vars->lastSeg->used == internal::LIST_SEGMENT_SIZE)
		{
			internal::ListSegment<Type> *newSeg = this->vars->createSegment();
			newSeg->prev = this->vars->lastSeg;
			this->vars->lastSeg->next = newSeg;
			this->vars->lastSeg = newSeg;
		}
		internal::ListCell<Type> *targetCell = this->vars->lastSeg->buffer + this->vars->lastSeg->used++;
		new (targetCell)internal::ListCell<Type>(val, this->vars->lastSeg);
		if (this->vars->size > 0)
		{
			targetCell->prev = this->vars->tail;
			this->vars->tail->next = targetCell;
			this->vars->tail = targetCell;
		}
		else
		{
			this->vars->head = this->vars->tail = targetCell;
		}
		this->vars->size++;
		return targetCell->val;
	}

	template <typename Type>
	void List<Type>::removeHead()
	{
		if (!this->vars || this->vars->size == 0) return;
		SPADAS_ERROR_RETURN(this->vars->elemRefs > 0);
		internal::ListCell<Type> *targetCell = this->vars->head;
		internal::ListSegment<Type> *targetSeg = targetCell->owner;
		if (this->vars->size > 1)
		{
			this->vars->head = targetCell->next;
			this->vars->head->prev = 0;
		}
		else
		{
			this->vars->head = this->vars->tail = 0;
		}
		if (!__is_trivial(Type)) targetCell->~ListCell();
		if (++targetSeg->unused == internal::LIST_SEGMENT_SIZE) this->vars->removeSegment(targetSeg);
		this->vars->size--;
	}

	template <typename Type>
	void List<Type>::removeTail()
	{
		if (!this->vars || this->vars->size == 0) return;
		SPADAS_ERROR_RETURN(this->vars->elemRefs > 0);
		internal::ListCell<Type> *targetCell = this->vars->tail;
		internal::ListSegment<Type> *targetSeg = targetCell->owner;
		if (this->vars->size > 1)
		{
			this->vars->tail = targetCell->prev;
			this->vars->tail->next = 0;
		}
		else
		{
			this->vars->head = this->vars->tail = 0;
		}
		if (!__is_trivial(Type)) targetCell->~ListCell();
		if (++targetSeg->unused == internal::LIST_SEGMENT_SIZE) this->vars->removeSegment(targetSeg);
		this->vars->size--;
	}

	template <typename Type>
	void List<Type>::remove(Type val)
	{
		if (!this->vars || this->vars->size == 0) return;
		SPADAS_ERROR_RETURN(this->vars->elemRefs > 0);
		internal::ListCell<Type> *curCell = this->vars->head;
		while (TRUE)
		{
			Bool isHead = curCell == this->vars->head;
			Bool isTail = curCell == this->vars->tail;
			internal::ListCell<Type> *prevCell = curCell->prev;
			internal::ListCell<Type> *nextCell = curCell->next;
			internal::ListSegment<Type> *curOwner = curCell->owner;
			if (curCell->val == val)
			{
				if (isHead) this->vars->head = nextCell;
				if (isTail) this->vars->tail = prevCell;
				if (prevCell) prevCell->next = nextCell;
				if (nextCell) nextCell->prev = prevCell;
				if (!__is_trivial(Type)) curCell->~ListCell();
				if (++curOwner->unused == internal::LIST_SEGMENT_SIZE) this->vars->removeSegment(curOwner);
				this->vars->size--;
			}
			if (isTail) break;
			else curCell = nextCell;
		}
	}

	template <typename Type>
	void List<Type>::removeAs(Func<Bool(Type&)> func)
	{
		if (!this->vars || this->vars->size == 0) return;
		SPADAS_ERROR_RETURN(this->vars->elemRefs > 0);
		internal::ListCell<Type> *curCell = this->vars->head;
		while (TRUE)
		{
			Bool isHead = curCell == this->vars->head;
			Bool isTail = curCell == this->vars->tail;
			internal::ListCell<Type> *prevCell = curCell->prev;
			internal::ListCell<Type> *nextCell = curCell->next;
			internal::ListSegment<Type> *curOwner = curCell->owner;
			if (func(curCell->val))
			{
				if (isHead) this->vars->head = nextCell;
				if (isTail) this->vars->tail = prevCell;
				if (prevCell) prevCell->next = nextCell;
				if (nextCell) nextCell->prev = prevCell;
				if (!__is_trivial(Type)) curCell->~ListCell();
				if (++curOwner->unused == internal::LIST_SEGMENT_SIZE) this->vars->removeSegment(curOwner);
				this->vars->size--;
			}
			if (isTail) break;
			else curCell = nextCell;
		}
	}

	template <typename Type>
	void List<Type>::clear()
	{
		if (!this->vars) return;
		SPADAS_ERROR_RETURN(this->vars->elemRefs > 0);
		this->vars->releaseSegments();
		this->vars->firstSeg = this->vars->lastSeg = this->vars->createSegment();
	}

	template <typename Type>
	Array<Type> List<Type>::toArray()
	{
		if (!this->vars) return Array<Type>();
		Array<Type> arr = Array<Type>::createUninitialized(this->vars->size);
		internal::ListCell<Type> *curCell = this->vars->head;
		for (UInt i = 0; i < this->vars->size; i++)
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
		ListVars<Type> *listVars = list.getVars();
		if (listVars) listVars->elemRefs++;
	}

	template <typename Type>
	ListElem<Type>::ListElem(const ListElem<Type>& elem) : list(elem.list), cell(elem.cell), prevCell(elem.prevCell), nextCell(elem.nextCell), idx(elem.idx), prevIndex(elem.prevIndex), nextIndex(elem.nextIndex)
	{
		ListVars<Type> *listVars = list.getVars();
		if (listVars) listVars->elemRefs++;
	}

	template <typename Type>
	ListElem<Type>::~ListElem()
	{
		ListVars<Type> *listVars = list.getVars();
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
		ListVars<Type> *listVars = this->list.getVars();
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
		ListVars<Type> *listVars = this->list.getVars();
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
		ListVars<Type> *listVars = this->list.getVars();
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
	template <typename Type>
	String Stream<Type>::typeName() { static String name = (String)"spadas.Stream<" + typeid(Type).name() + ">"; return name; }

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
		return this->vars->capacity;
	}

	template <typename Type>
	UInt Stream<Type>::nElements()
	{
		return this->vars->nElements;
	}

	template <typename Type>
	Bool Stream<Type>::isEmpty()
	{
		return this->vars->nElements == 0;
	}

	template <typename Type>
	UInt Stream<Type>::nDiscarded()
	{
		return this->vars->nDiscarded;
	}

	template <typename Type>
	Type Stream<Type>::first()
	{
		this->vars->spinEnter();
		if (!this->vars->toDequeue)
		{
			this->vars->spinLeave();
			return Type();
		}
		else
		{
			Type output = *this->vars->toDequeue;
			this->vars->spinLeave();
			return output;
		}
	}

	template <typename Type>
	Type Stream<Type>::last()
	{
		this->vars->spinEnter();
		if (this->vars->nElements == 0)
		{
			this->vars->spinLeave();
			return Type();
		}
		else
		{
			Type output = *((this->vars->toEnqueue == this->vars->buffer ? (this->vars->buffer + this->vars->capacity) : this->vars->toEnqueue) - 1);
			this->vars->spinLeave();
			return output;
		}
	}

	template <typename Type>
	void Stream<Type>::enqueue(Type newElement)
	{
		this->vars->spinEnter();
		if (!this->vars->discardable)
		{
			while (this->vars->nElements == this->vars->capacity)
			{
				this->vars->spinLeave();
				while (this->vars->nElements == this->vars->capacity)
				{
					SPADAS_WARNING_RETURN(this->vars->terminated);
				}
				this->vars->spinEnter();
			}
		}
		if (this->vars->terminated)
		{
			this->vars->spinLeave();
			SPADAS_WARNING_MSG("this->vars->terminated");
			return;
		}
		Type *bufferEnd = this->vars->buffer + this->vars->capacity;
		if (this->vars->nElements == this->vars->capacity)
		{
			if (!__is_trivial(Type)) this->vars->toEnqueue->~Type();
			if (++this->vars->toDequeue == bufferEnd) this->vars->toDequeue = this->vars->buffer;
			this->vars->nDiscarded++;
		}
		else this->vars->nElements++;
		new (this->vars->toEnqueue)Type(newElement);
		if (!this->vars->toDequeue) this->vars->toDequeue = this->vars->toEnqueue;
		if (++this->vars->toEnqueue == bufferEnd) this->vars->toEnqueue = this->vars->buffer;
		this->vars->spinLeave();
	}

	template <typename Type>
	Bool Stream<Type>::enqueue(Type newElement, Flag interrupt)
	{
		this->vars->spinEnter();
		if (!this->vars->discardable)
		{
			while (this->vars->nElements == this->vars->capacity)
			{
				this->vars->spinLeave();
				while (this->vars->nElements == this->vars->capacity)
				{
					if (this->vars->terminated || interrupt.check()) return FALSE;
				}
				this->vars->spinEnter();
			}
		}
		if (this->vars->terminated)
		{
			this->vars->spinLeave();
			return FALSE;
		}
		Type *bufferEnd = this->vars->buffer + this->vars->capacity;
		if (this->vars->nElements == this->vars->capacity)
		{
			if (!__is_trivial(Type)) this->vars->toEnqueue->~Type();
			if (++this->vars->toDequeue == bufferEnd) this->vars->toDequeue = this->vars->buffer;
			this->vars->nDiscarded++;
		}
		else this->vars->nElements++;
		new (this->vars->toEnqueue)Type(newElement);
		if (!this->vars->toDequeue) this->vars->toDequeue = this->vars->toEnqueue;
		if (++this->vars->toEnqueue == bufferEnd) this->vars->toEnqueue = this->vars->buffer;
		this->vars->spinLeave();
		return TRUE;
	}

	template <typename Type>
	void Stream<Type>::enqueue(Array<Type> newElements)
	{
		if (newElements.isEmpty()) return;
		UInt elemSize = newElements.size();
		if (!this->vars->discardable)
		{
			for (UInt i = 0; i < elemSize; i++)
			{
				SPADAS_WARNING_RETURN(this->vars->terminated);
				enqueue(newElements[i]);
			}
		}
		else
		{
			this->vars->spinEnter();
			if (this->vars->terminated)
			{
				this->vars->spinLeave();
				SPADAS_WARNING_MSG("this->vars->terminated");
				return;
			}
			Type *bufferEnd = this->vars->buffer + this->vars->capacity;
			for (UInt i = 0; i < elemSize; i++)
			{
				if (this->vars->nElements == this->vars->capacity)
				{
					if (!__is_trivial(Type)) this->vars->toEnqueue->~Type();
					if (++this->vars->toDequeue == bufferEnd) this->vars->toDequeue = this->vars->buffer;
					this->vars->nDiscarded++;
				}
				else this->vars->nElements++;
				new (this->vars->toEnqueue)Type(newElements[i]);
				if (!this->vars->toDequeue) this->vars->toDequeue = this->vars->toEnqueue;
				if (++this->vars->toEnqueue == bufferEnd) this->vars->toEnqueue = this->vars->buffer;
			}
			this->vars->spinLeave();
		}
	}

	template <typename Type>
	Bool Stream<Type>::enqueue(Array<Type> newElements, Flag interrupt)
	{
		if (newElements.isEmpty()) return TRUE;
		UInt elemSize = newElements.size();
		if (!this->vars->discardable)
		{
			for (UInt i = 0; i < elemSize; i++)
			{
				if (!enqueue(newElements[i], interrupt)) return FALSE;
			}
			return TRUE;
		}
		else
		{
			this->vars->spinEnter();
			if (this->vars->terminated)
			{
				this->vars->spinLeave();
				return FALSE;
			}
			Type *bufferEnd = this->vars->buffer + this->vars->capacity;
			for (UInt i = 0; i < elemSize; i++)
			{
				if (this->vars->nElements == this->vars->capacity)
				{
					if (!__is_trivial(Type)) this->vars->toEnqueue->~Type();
					if (++this->vars->toDequeue == bufferEnd) this->vars->toDequeue = this->vars->buffer;
					this->vars->nDiscarded++;
				}
				else this->vars->nElements++;
				new (this->vars->toEnqueue)Type(newElements[i]);
				if (!this->vars->toDequeue) this->vars->toDequeue = this->vars->toEnqueue;
				if (++this->vars->toEnqueue == bufferEnd) this->vars->toEnqueue = this->vars->buffer;
			}
			this->vars->spinLeave();
			return TRUE;
		}
	}

	template <typename Type>
	Bool Stream<Type>::dequeueOne(Type& elem)
	{
		Bool ret = FALSE;
		this->vars->spinEnter();
		if (this->vars->nElements > 0)
		{
			elem = *this->vars->toDequeue;
			if (!__is_trivial(Type)) this->vars->toDequeue->~Type();
			Type *bufferEnd = this->vars->buffer + this->vars->capacity;
			if (++this->vars->toDequeue == bufferEnd) this->vars->toDequeue = this->vars->buffer;
			if (--this->vars->nElements == 0) this->vars->toDequeue = 0;
			ret = TRUE;
		}
		this->vars->spinLeave();
		return ret;
	}

	template <typename Type>
	Array<Type> Stream<Type>::dequeue(UInt num)
	{
		this->vars->spinEnter();
		UInt nOuts = math::min(this->vars->nElements, num);
		Array<Type> out = Array<Type>::createUninitialized(nOuts);
		Type *bufferEnd = this->vars->buffer + this->vars->capacity;
		for (UInt i = 0; i < nOuts; i++)
		{
			out.initialize(i, *this->vars->toDequeue);
			if (!__is_trivial(Type)) this->vars->toDequeue->~Type();
			if (++this->vars->toDequeue == bufferEnd) this->vars->toDequeue = this->vars->buffer;
		}
		this->vars->nElements -= nOuts;
		if (this->vars->nElements == 0) this->vars->toDequeue = 0;
		this->vars->spinLeave();
		return out;
	}

	template <typename Type>
	Array<Type> Stream<Type>::dequeueAs(Func<Bool(Type&)> func)
	{
		this->vars->spinEnter();
		ArrayX<Type> out;
		Type *bufferEnd = this->vars->buffer + this->vars->capacity;
		for (UInt i = 0; i < this->vars->nElements; i++)
		{
			if (func(*this->vars->toDequeue))
			{
				out.append(*this->vars->toDequeue);
				if (!__is_trivial(Type)) this->vars->toDequeue->~Type();
				if (++this->vars->toDequeue == bufferEnd) this->vars->toDequeue = this->vars->buffer;
			}
			else break;
		}
		this->vars->nElements -= out.size();
		if (this->vars->nElements == 0) this->vars->toDequeue = 0;
		this->vars->spinLeave();
		return out.toArray();
	}

	template <typename Type>
	Bool Stream<Type>::waitAllDequeued(Flag interrupt, Bool spin)
	{
		while (this->vars->nElements != 0)
		{
			if (interrupt.check()) return FALSE;
			if (!spin) system::wait(1);
		}
		return TRUE;
	}

	template <typename Type>
	void Stream<Type>::terminate()
	{
		this->vars->spinEnter();
		this->vars->terminated = TRUE;
		this->vars->spinLeave();
	}

	template <typename Type>
	Bool Stream<Type>::isTerminated()
	{
		return this->vars->terminated;
	}

	template <typename Type>
	void Stream<Type>::reset()
	{
		this->vars->spinEnter();
		if (!__is_trivial(Type))
		{
			Type *bufferEnd = this->vars->buffer + this->vars->capacity;
			while (this->vars->toDequeue)
			{
				this->vars->toDequeue->~Type();
				if (++this->vars->toDequeue == bufferEnd) this->vars->toDequeue = this->vars->buffer;
				if (this->vars->toDequeue == this->vars->toEnqueue) this->vars->toDequeue = 0;
			}
		}
		else this->vars->toDequeue = 0;
		this->vars->toEnqueue = this->vars->buffer;
		this->vars->nElements = 0;
		this->vars->nDiscarded = 0;
		this->vars->terminated = FALSE;
		this->vars->spinLeave();
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

	template <typename KeyType, typename ValueType>
	String Map<KeyType, ValueType>::typeName() { static String name = (String)"spadas.Map<" + typeid(KeyType).name() + "," + typeid(ValueType).name() + ">"; return name; }

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
		UInt tableSize = this->vars->table.size();
		ListNode<KeyValue<KeyType, ValueType> >** tableData = this->vars->table.data();
		for (UInt i = 0; i < tableSize; i++)
		{
			if (tableData[i])
			{
				tableData[i]->collapse();
				delete tableData[i];
				tableData[i] = 0;
			}
		}
		this->vars->size = 0;
	}

	template <typename KeyType, typename ValueType>
	Bool Map<KeyType, ValueType>::isEmpty()
	{
		return !this->vars || this->vars->size == 0;
	}

	template <typename KeyType, typename ValueType>
	UInt Map<KeyType, ValueType>::size()
	{
		return this->vars ? this->vars->size : 0;
	}

	template <typename KeyType, typename ValueType>
	Array<KeyType> Map<KeyType, ValueType>::keys()
	{
		if (!this->vars || this->vars->size == 0) return Array<KeyType>();
		Array<KeyType> out = Array<KeyType>::createUninitialized(this->vars->size);
		UInt count = 0;
		UInt tableSize = this->vars->table.size();
		ListNode<KeyValue<KeyType, ValueType> >** tableData = this->vars->table.data();
		for (UInt i = 0; i < tableSize; i++)
		{
			if (!tableData[i]) continue;
			ListNode<KeyValue<KeyType, ValueType> > node = *tableData[i];
			while (TRUE)
			{
				out.initialize(count++, node.value().key);
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
		if (!this->vars || this->vars->size == 0) return Array<ValueType>();
		Array<ValueType> out = Array<ValueType>::createUninitialized(this->vars->size);
		UInt count = 0;
		UInt tableSize = this->vars->table.size();
		ListNode<KeyValue<KeyType, ValueType> >** tableData = this->vars->table.data();
		for (UInt i = 0; i < tableSize; i++)
		{
			if (!tableData[i]) continue;
			ListNode<KeyValue<KeyType, ValueType> > node = *tableData[i];
			while (TRUE)
			{
				out.initialize(count++, node.value().value);
				if (node.hasNext()) node.goNext();
				else break;
			}
		}
		return out;
	}

	template <typename KeyType, typename ValueType>
	Array<KeyValue<KeyType, ValueType> > Map<KeyType, ValueType>::keyValues()
	{
		if (!this->vars || this->vars->size == 0) return Array<KeyValue<KeyType, ValueType> >();
		Array<KeyValue<KeyType, ValueType> > out = Array<KeyValue<KeyType, ValueType> >::createUninitialized(this->vars->size);
		UInt count = 0;
		UInt tableSize = this->vars->table.size();
		ListNode<KeyValue<KeyType, ValueType> >** tableData = this->vars->table.data();
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
		if (!this->vars || this->vars->size == 0)
		{
			keys = Array<KeyType>();
			values = Array<ValueType>();
			return;
		}
		keys = Array<KeyType>::createUninitialized(this->vars->size);
		values = Array<ValueType>::createUninitialized(this->vars->size);
		UInt count = 0;
		UInt tableSize = this->vars->table.size();
		ListNode<KeyValue<KeyType, ValueType> >** tableData = this->vars->table.data();
		for (UInt i = 0; i < tableSize; i++)
		{
			if (!tableData[i]) continue;
			ListNode<KeyValue<KeyType, ValueType> > node = *tableData[i];
			while (TRUE)
			{
				keys.initialize(count, node.value().key);
				values.initialize(count++, node.value().value);
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
		Word index = key.getHash() & this->vars->mask;
		ListNode<KeyValue<KeyType, ValueType> >* target = this->vars->table[index];
		if (!target) return FALSE;
		ListNode<KeyValue<KeyType, ValueType> > node = *target;
		while (TRUE)
		{
			if (node.value().key == key) return TRUE;
			else if (!node.hasNext()) return FALSE;
			else node.goNext();
		}
	}

	template <typename KeyType, typename ValueType>
	void Map<KeyType, ValueType>::remove(KeyType key)
	{
		if (!this->vars) return;
		Word index = key.getHash() & this->vars->mask;
		ListNode<KeyValue<KeyType, ValueType> >** tableData = this->vars->table.data();
		if (!tableData[index]) return;
		ListNode<KeyValue<KeyType, ValueType> > node = *tableData[index];
		if (node.value().key == key)
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
			this->vars->size--;
			return;
		}
		else
		{
			while (node.hasNext())
			{
				node.goNext();
				if (node.value().key == key)
				{
					node.removeSelf();
					this->vars->size--;
					return;
				}
			}
		}
	}

	template <typename KeyType, typename ValueType>
	ValueType Map<KeyType, ValueType>::get(KeyType key, ValueType defaultValue)
	{
		if (!this->vars) return defaultValue;
		Word index = key.getHash() & this->vars->mask;
		ListNode<KeyValue<KeyType, ValueType> >* target = this->vars->table[index];
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
		Word index = key.getHash() & this->vars->mask;
		ListNode<KeyValue<KeyType, ValueType> >* target = this->vars->table[index];
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
		Word index = key.getHash() & this->vars->mask;
		ListNode<KeyValue<KeyType, ValueType> >** tableData = this->vars->table.data();
		if (!tableData[index])
		{
			KeyValue<KeyType, ValueType> pair(key, value);
			tableData[index] = new ListNode<KeyValue<KeyType, ValueType> >(pair);
			this->vars->size++;
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
					this->vars->size++;
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
		Word index = key.getHash() & this->vars->mask;
		ListNode<KeyValue<KeyType, ValueType> >** tableData = this->vars->table.data();
		if (!tableData[index])
		{
			KeyValue<KeyType, ValueType> pair(key, ValueType());
			tableData[index] = new ListNode<KeyValue<KeyType, ValueType> >(pair);
			this->vars->size++;
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
					this->vars->size++;
					return newNode.value().value;
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
		this->vars->target += obj;
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
		utility::memoryCopy(raw, this->vars->data, this->vars->nElems * sizeof(Type));
	}

	template <typename Type>
	Matrix<Type>::Matrix(Array<Type> arr, UInt nDims)
	{
		SPADAS_ERROR_RETURN(nDims == 0 || arr.size() == 0);
		Array<UInt> dims(nDims, 1);
		dims[0] = arr.size();
		this->setVars(new MatrixVars<Type>(dims), TRUE);
		utility::memoryCopy(arr.data(), this->vars->data, this->vars->nElems * sizeof(Type));
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
		Type *outData = out.vars->data;
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
		Matrix<Type> out(this->vars->dims);
		utility::memoryCopy(this->vars->data, out.vars->data, this->vars->nElems * sizeof(Type));
		return out;
	}

	template <typename Type>
	template <typename TargetType>
	void Matrix<Type>::convertTo(Matrix<TargetType>& target)
	{
		SPADAS_ERROR_RETURN(!this->vars);
		target = Matrix<TargetType>(this->vars->dims);
		UInt nElements = nElems();
		Type *srcData = this->vars->data;
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
		SPADAS_ERROR_RETURN(thisOffset.i < 0 || thisOffset.j < 0 || thisOffset.i + srcRegion.dim0 > this->vars->dims[0] || thisOffset.j + srcRegion.dim1 > this->vars->dims[1]);
		UInt srcDim1 = src.dimAt(1);
		UInt dstDim1 = this->vars->dims[1];
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
		SPADAS_ERROR_RETURN(thisOffset.i < 0 || thisOffset.j < 0 || thisOffset.k < 0 || thisOffset.i + srcRegion.dim0 > this->vars->dims[0] || thisOffset.j + srcRegion.dim1 > this->vars->dims[1] || thisOffset.k + srcRegion.dim2 > this->vars->dims[2]);
		UInt srcDim1 = src.dimAt(1);
		UInt srcDim2 = src.dimAt(2);
		UInt dstDim1 = this->vars->dims[1];
		UInt dstDim2 = this->vars->dims[2];
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
		Type *data0 = this->vars->data;
		for (UInt i = 0; i < this->vars->nElems; i++)
		{
			data0[i] = value;
		}
	}

	template <typename Type>
	Array<UInt> Matrix<Type>::size()
	{
		SPADAS_ERROR_RETURNVAL(!this->vars, Array<UInt>());
		return this->vars->dims.clone();
	}

	template <typename Type>
	UInt Matrix<Type>::dimAt(UInt index)
	{
		SPADAS_ERROR_RETURNVAL(!this->vars, 0);
		UInt nDims = this->vars->dims.size();
		SPADAS_ERROR_RETURNVAL(index >= nDims, 0);
		return this->vars->dims[index];
	}

	template <typename Type>
	UInt Matrix<Type>::nDims()
	{
		SPADAS_ERROR_RETURNVAL(!this->vars, 0);
		return this->vars->dims.size();
	}

	template <typename Type>
	UInt Matrix<Type>::nElems()
	{
		SPADAS_ERROR_RETURNVAL(!this->vars, 0);
		return this->vars->nElems;
	}

	template <typename Type>
	Bool Matrix<Type>::isSize(Size2D size2D)
	{
		SPADAS_ERROR_RETURNVAL(!this->vars, FALSE);
		if (this->vars->dims.size() != 2) return FALSE;
		return this->vars->dims[0] == size2D.height && this->vars->dims[1] == size2D.width;
	}

	template <typename Type>
	Bool Matrix<Type>::isSize(Size3D size3D)
	{
		SPADAS_ERROR_RETURNVAL(!this->vars, FALSE);
		if (this->vars->dims.size() != 3) return FALSE;
		return this->vars->dims[0] == size3D.height && this->vars->dims[1] == size3D.width && this->vars->dims[2] == size3D.depth;
	}

	template <typename Type>
	Bool Matrix<Type>::isSize(Array<UInt> dims)
	{
		SPADAS_ERROR_RETURNVAL(!this->vars, FALSE);
		if (this->vars->dims.size() != dims.size()) return FALSE;
		UInt nDims = this->vars->dims.size();
		for (UInt i = 0; i < nDims; i++)
		{
			if (this->vars->dims[i] != dims[i]) return FALSE;
		}
		return TRUE;
	}

	template <typename Type>
	Type *Matrix<Type>::data()
	{
		SPADAS_ERROR_RETURNVAL(!this->vars, 0);
		return this->vars->data;
	}

	template <typename Type>
	Matrix<Type> Matrix<Type>::operator [](UInt i)
	{
		SPADAS_ERROR_RETURNVAL(!this->vars, Matrix<Type>());
		UInt nDims = this->vars->dims.size();
		SPADAS_ERROR_RETURNVAL(nDims == 0, *this);
		if (i >= this->vars->dims[0])
		{
			SPADAS_ERROR_MSG("i >= this->vars->dims[0]");
			i = 0;
		}
		MatrixVars<Type> *newVars = new MatrixVars<Type>(this->vars->data0, nDims - 1);
		UInt newVarsNDims = newVars->dims.size();
		for (UInt j = 0; j < newVarsNDims; j++)
		{
			newVars->dims[j] = this->vars->dims[j + 1];
		}
		newVars->nElems = this->vars->nElems / this->vars->dims[0];
		newVars->data = &this->vars->data[i * newVars->nElems];
		Matrix<Type> out;
		out.setVars(newVars, TRUE);
		return out;
	}

	template <typename Type>
	Type& Matrix<Type>::operator ()(UInt i)
	{
		SPADAS_ERROR_RETURNVAL(!this->vars, *(new Type));
		SPADAS_ERROR_RETURNVAL(this->vars->dims.size() != 1, *(new Type));
		SPADAS_ERROR_RETURNVAL (i >= this->vars->dims[0], *(new Type));
		return this->vars->data[i];
	}

	template <typename Type>
	Type& Matrix<Type>::operator ()(UInt i, UInt j)
	{
		SPADAS_ERROR_RETURNVAL(!this->vars, *(new Type));
		SPADAS_ERROR_RETURNVAL(this->vars->dims.size() != 2, *(new Type));
		UInt *dimsData = this->vars->dims.data();
		SPADAS_ERROR_RETURNVAL(i >= dimsData[0] || j >= dimsData[1], *(new Type));
		return this->vars->data[i * dimsData[1] + j];
	}

	template <typename Type>
	Type& Matrix<Type>::operator ()(UInt i, UInt j, UInt k)
	{
		SPADAS_ERROR_RETURNVAL(!this->vars, *(new Type));
		SPADAS_ERROR_RETURNVAL(this->vars->dims.size() != 3, *(new Type));
		UInt *dimsData = this->vars->dims.data();
		SPADAS_ERROR_RETURNVAL(i >= dimsData[0] || j >= dimsData[1] || k >= dimsData[2], *(new Type));
		return this->vars->data[(i * dimsData[1] + j) * dimsData[2] + k];
	}

	template <typename Type>
	Matrix<Type> Matrix<Type>::operator +(Matrix<Type> matrix)
	{
		SPADAS_ERROR_RETURNVAL(!this->vars || !matrix.vars, Matrix<Type>());
		SPADAS_ERROR_RETURNVAL(!isSize(matrix.size()), *this);
		Matrix<Type> out(this->vars->dims);
		Type *src1 = this->vars->data;
		Type *src2 = matrix.vars->data;
		Type *dst = out.vars->data;
		for (UInt i = 0; i < this->vars->nElems; i++)
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
		Matrix<Type> out(this->vars->dims);
		Type *src1 = this->vars->data;
		Type *src2 = matrix.vars->data;
		Type *dst = out.vars->data;
		for (UInt i = 0; i < this->vars->nElems; i++)
		{
			dst[i] = src1[i] - src2[i];
		}
		return out;
	}

	template <typename Type>
	Matrix<Type> Matrix<Type>::operator *(Type scale)
	{
		SPADAS_ERROR_RETURNVAL(!this->vars, Matrix<Type>());
		Matrix<Type> out(this->vars->dims);
		Type *src = this->vars->data;
		Type *dst = out.vars->data;
		for (UInt i = 0; i < this->vars->nElems; i++)
		{
			dst[i] = src[i] * scale;
		}
		return out;
	}

	template <typename Type>
	Matrix<Type> Matrix<Type>::operator *(Matrix<Type> matrix)
	{
		SPADAS_ERROR_RETURNVAL(!this->vars || !matrix.vars, Matrix<Type>());
		SPADAS_ERROR_RETURNVAL(this->vars->dims.size() != 2 || matrix.vars->dims.size() != 2, *this);
		SPADAS_ERROR_RETURNVAL(this->vars->dims[1] != matrix.vars->dims[0], *this);
		UInt outRows = this->vars->dims[0], outCols = matrix.vars->dims[1];
		Matrix<Type> out(outRows, outCols);
		Type *selfRow = this->vars->data;
		Type *inData = matrix.vars->data;
		Type *outRow = out.vars->data;
		UInt selfStep = this->vars->dims[1];
		UInt inStep = matrix.vars->dims[1];
		UInt outStep = out.vars->dims[1];
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
		SPADAS_ERROR_RETURNVAL(this->vars->dims.size() != 2, *this);
		UInt nRows = this->vars->dims[0];
		UInt nCols = this->vars->dims[1];
		Matrix<Type> out(nCols, nRows);
		for (UInt i = 0; i < nCols; i++)
		{
			Type *outRow = &out.vars->data[i * nRows];
			UInt srcIndex0 = i;
			for (UInt j = 0; j < nRows; j++)
			{
				outRow[j] = this->vars->data[srcIndex0 + j*nCols];
			}
		}
		return out;
	}

	template <typename Type>
	String Matrix<Type>::toString()
	{
		SPADAS_ERROR_RETURNVAL(!this->vars, String());
		SPADAS_ERROR_RETURNVAL(this->vars->dims.size() != 2, String());
		UInt nRows = this->vars->dims[0];
		UInt nCols = this->vars->dims[1];
		String outString = String::createWithSize(nRows * (nCols + 1) * 32);
		for (UInt i = 0; i < nRows; i++)
		{
			String rowString = String::createWithSize(nCols * 32);
			Type *rowData = &this->vars->data[i * nCols];
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
		SPADAS_ERROR_RETURN(this->vars->dims.size() != 2);
		File file = File::createText(filePath);
		if (file.isNull()) return;
		UInt nRows = this->vars->dims[0];
		UInt nCols = this->vars->dims[1];
		for (UInt i = 0; i < nRows; i++)
		{
			String rowString = String::createWithSize(nCols * 32);
			Type *rowData = &this->vars->data[i * nCols];
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
    };

    template <typename Type>
	String SampleParser<Type>::typeName() { static String name = (String)"spadas.SampleParser<" + typeid(Type).name() + ">"; return name; }

    template <typename Type>
    SampleParser<Type>::SampleParser()
    {}

    template <typename Type>
    SampleParser<Type>::SampleParser(Array<String> protocols) : Object<SampleParserVars<Type> >(new SampleParserVars<Type>, TRUE)
    {
        this->vars->protocols = protocols.clone();
        this->vars->targetProtocolIndex = -1;
    }

    template <typename Type>
    void SampleParser<Type>::reset()
    {
		if (!this->vars) return;
        this->vars->targetProtocolIndex = -1;
        this->vars->lastTimestamp = FullTimestamp();
    }

    template <typename Type>
    Array<Type> SampleParser<Type>::parse(SessionGeneralSampleTable table)
    {
		SPADAS_ERROR_RETURNVAL(!this->vars, Array<Type>());
        Type testObj;
        SPADAS_ERROR_RETURNVAL(!testObj.template is<SessionSample>(), Array<Type>());
        for (UInt i = 0; i < this->vars->protocols.size(); i++)
        {
            if (table.contain(this->vars->protocols[i]) && !table[this->vars->protocols[i]].isEmpty()) this->vars->targetProtocolIndex = math::max(this->vars->targetProtocolIndex, (Int)i);
        }
        if (this->vars->targetProtocolIndex == -1) return Array<Type>();
        String targetProtocol = this->vars->protocols[this->vars->targetProtocolIndex];
        if (!table.contain(targetProtocol)) return Array<Type>();
        ArrayX<Type> buffer;
        for (auto e = table[targetProtocol].firstElem(); e.valid(); ++e)
        {
            SessionSample inputSample = e->as<SessionSample>();
            if (inputSample.timestamp() > this->vars->lastTimestamp)
            {
                Type outputSpecialSample;
                SessionSample outputSample = outputSpecialSample.template as<SessionSample>();
                if (outputSample.fromSample(targetProtocol, inputSample))
                {
                    buffer.append(outputSpecialSample);
                    this->vars->lastTimestamp = inputSample.timestamp();
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
        for (UInt i = 0; i < this->vars->protocols.size(); i++)
        {
            if (table.contain(this->vars->protocols[i]) && !table[this->vars->protocols[i]].isEmpty()) this->vars->targetProtocolIndex = math::max(this->vars->targetProtocolIndex, (Int)i);
        }
        if (this->vars->targetProtocolIndex == -1) return Array<Type>();
        String targetProtocol = this->vars->protocols[this->vars->targetProtocolIndex];
        if (!table.contain(targetProtocol)) return Array<Type>();
        ArrayX<Type> buffer;
        for (auto e = table[targetProtocol].firstElem(); e.valid(); ++e)
        {
            SessionSample inputSample = e->as<SessionSample>();
            if (inputSample.timestamp() > this->vars->lastTimestamp)
            {
                Type outputSpecialSample;
                SessionSample outputSample = outputSpecialSample.template as<SessionSample>();
                if (outputSample.fromSample(targetProtocol, inputSample))
                {
                    buffer.append(outputSpecialSample);
                    this->vars->lastTimestamp = inputSample.timestamp();
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
    SampleSync<Type>::SampleSync()
    {}

    template <typename Type>
    SampleSync<Type>::SampleSync(Array<String> protocols) : Object<SampleSyncVars<Type> >(new SampleSyncVars<Type>, TRUE)
    {
        this->vars->protocols = protocols.clone();
        this->vars->targetProtocolIndex = -1;
    }

    template <typename Type>
    void SampleSync<Type>::reset()
    {
		if (!this->vars) return;
        this->vars->targetProtocolIndex = -1;
    }

    template <typename Type>
    Enum<SampleInterpolationResult> SampleSync<Type>::interpolate(SessionSampleBufferTable table, SessionIdentifier session, Double timeOffset, Type& interpolatedSample, UInt earlyThresh)
    {
		SPADAS_ERROR_RETURNVAL(!this->vars, SampleInterpolationResult::Value::OutOfRange);
        Type testObj;
        SPADAS_ERROR_RETURNVAL(!testObj.template is<SessionSample>(), SampleInterpolationResult::Value::InvalidType);
        for (UInt i = 0; i < this->vars->protocols.size(); i++)
        {
            if (table.contain(this->vars->protocols[i]) && !table[this->vars->protocols[i]].isEmpty()) this->vars->targetProtocolIndex = math::max(this->vars->targetProtocolIndex, (Int)i);
        }
        if (this->vars->targetProtocolIndex == -1) return SampleInterpolationResult::Value::OutOfRange;
        String targetProtocol = this->vars->protocols[this->vars->targetProtocolIndex];
        if (!table.contain(targetProtocol)) return SampleInterpolationResult::Value::OutOfRange;
        auto targetBuffer = table[targetProtocol];
        if (session != targetBuffer.getCurrentSession()) return SampleInterpolationResult::Value::OutOfRange;
        else return targetBuffer.interpolate(timeOffset, interpolatedSample, earlyThresh);
    }
}

#endif
