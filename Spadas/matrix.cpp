
#include "spadas.h"

#include "string_bz.h"

struct CvMat
{
	int type;
	int step;

	int* refcount;
	int hdr_refcount;

	union
	{
		unsigned char* ptr;
		short* s;
		int* i;
		float* fl;
		double* db;
	}
	data;

	int rows;
	int cols;
};

namespace matrix_internal
{
	using namespace spadas;

	void updateCVMat(Int *data, CvMat *mat)
	{
		mat->type = 0x42424004;
		mat->data.i = data;
	}

	void updateCVMat(Float *data, CvMat *mat)
	{
		mat->type = 0x42424005;
		mat->data.fl = data;
	}

	void updateCVMat(Double *data, CvMat *mat)
	{
		mat->type = 0x42424006;
		mat->data.db = data;
	}

	Byte getTypeID(IntMat mat)
	{
		return 1;
	}

	Byte getTypeID(FloatMat mat)
	{
		return 2;
	}

	Byte getTypeID(DoubleMat mat)
	{
		return 3;
	}

	template<typename SrcType, typename DstType>
	void matrixConvert(Matrix<SrcType> src, Matrix<DstType>& dst)
	{
		SPADAS_ERROR_RETURN(src.isNull());

		Array<UInt> size = src.size();
		dst = Matrix<DstType>(size);
		
		UInt nElems = src.nElems();

		SrcType *srcData = src.data();
		DstType *dstData = dst.data();
		for (UInt i = 0; i < nElems; i++)
		{
			dstData[i] = (DstType)srcData[i];
		}
	}
}

namespace spadas
{
    const ULong MAX_NELEMS = 100000000;
    
	template<typename Type> class MatrixVars : public Vars
	{
    public:
		Array<UInt> dims;
		UInt nElems;
		Array<Type> data0;
		Type *data;
		CvMat *cvMat;

		// 主要构造函数
		MatrixVars(Array<UInt> dims0/* 不能为空 */) : dims(dims0.clone()), cvMat(NULL)
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
                if (nElems0 > MAX_NELEMS)
                {
					SPADAS_ERROR_MSG("nElems0 > MAX_NELEMS");
                    nElems0 /= (ULong)dimsData[i];
                    for (UInt n = i; n < nDims; n++) dimsData[n] = 1;
					break;
                }
            }
            nElems = (UInt)nElems0;
            data0 = Array<Type>(nElems, 0);
            data = data0.data();
        }

		// 用于 operator[]
		MatrixVars(Array<Type> data00, UInt nDims) : dims(Array<UInt>(nDims)), data0(data00), cvMat(NULL)
		{ }
        ~MatrixVars()
        {
            if (cvMat) delete cvMat;
        }
	};
}

using namespace spadas;
using namespace spadas::console;
using namespace spadas_internal;
using namespace matrix_internal;

#define INSTANTIATE_NELEMS(Type) \
template<> \
Matrix<Type>::Matrix() \
{ \
} \
template<> \
Matrix<Type>::Matrix(UInt nRows, UInt nCols) \
{ \
    nRows += (nRows == 0); \
    nCols += (nCols == 0); \
    Array<UInt> dims(2); \
    dims[0] = nRows; dims[1] = nCols; \
	setVars(new MatrixVars<Type>(dims), TRUE); \
} \
template<> \
Matrix<Type>::Matrix(UInt nRows, UInt nCols, UInt nDepths) \
{ \
    nRows += (nRows == 0); \
    nCols += (nCols == 0); \
    nDepths += (nDepths == 0); \
	Array<UInt> dims(3); \
	dims[0] = nRows; dims[1] = nCols; dims[2] = nDepths; \
	setVars(new MatrixVars<Type>(dims), TRUE); \
} \
template<> \
Matrix<Type>::Matrix(Array<UInt> dims) \
{ \
	if (dims.isEmpty()) dims = Array<UInt>::scalar(1); \
	setVars(new MatrixVars<Type>(dims), TRUE); \
} \
template<> \
Matrix<Type>::Matrix(Array<UInt> dims, Pointer raw) \
{ \
	if (dims.isEmpty() || !raw) \
	{ \
		SPADAS_ERROR_MSG("dims.isEmpty() || !raw"); \
		return; \
	} \
	for (UInt i = 0; i < dims.size(); i++) \
	{ \
		if (dims[i] == 0) \
		{ \
			SPADAS_ERROR_MSG("dims[i] == 0"); \
			return; \
		} \
	} \
	setVars(new MatrixVars<Type>(dims), TRUE); \
    utility::memoryCopy(raw, vars->data, vars->nElems * sizeof(Type)); \
} \
template<> \
Matrix<Type>::Matrix(Array<Type> arr, UInt nDims) \
{ \
    if (nDims == 0 || arr.size() == 0) \
    { \
        SPADAS_ERROR_MSG("nDims == 0 || arr.size() == 0"); \
        return; \
    } \
	Array<UInt> dims(nDims, 1); \
	dims[0] = arr.size(); \
	setVars(new MatrixVars<Type>(dims), TRUE); \
    utility::memoryCopy(arr.data(), vars->data, vars->nElems * sizeof(Type)); \
} \
template<> \
UInt Matrix<Type>::dimAt(UInt index) \
{ \
	if (!vars) \
	{ \
		SPADAS_ERROR_MSG("!vars"); \
		return 0; \
	} \
	UInt nDims = vars->dims.size(); \
	if (index >= nDims) \
	{ \
		SPADAS_ERROR_MSG("index >= nDims"); \
		return 0; \
	} \
	return vars->dims[index]; \
} \
template<> \
UInt Matrix<Type>::nDims() \
{ \
	if (!vars) \
	{ \
		SPADAS_ERROR_MSG("!vars"); \
		return 0; \
	} \
	return vars->dims.size(); \
} \
template<> \
Type& Matrix<Type>::operator ()(UInt i, UInt j) \
{ \
	if (!vars) \
	{ \
		SPADAS_ERROR_MSG("!vars"); \
		return *(new Type); \
	} \
	if (vars->dims.size() != 2) \
	{ \
		SPADAS_ERROR_MSG("vars->dims.size() != 2"); \
		return *(new Type); \
	} \
	UInt *dimsData = vars->dims.data(); \
	if (i >= dimsData[0] || j >= dimsData[1]) \
	{ \
		SPADAS_ERROR_MSG("i >= dimsData[0] || j >= dimsData[1]"); \
		return *(new Type); \
	} \
	return vars->data[i * dimsData[1] + j]; \
} \
template<> \
Type& Matrix<Type>::operator ()(UInt i, UInt j, UInt k) \
{ \
	if (!vars) \
	{ \
		SPADAS_ERROR_MSG("!vars"); \
		return *(new Type); \
	} \
	if (vars->dims.size() != 3) \
	{ \
		SPADAS_ERROR_MSG("vars->dims.size() != 3"); \
		return *(new Type); \
	} \
	UInt *dimsData = vars->dims.data(); \
	if (i >= dimsData[0] || j >= dimsData[1] || k >= dimsData[2]) \
	{ \
		SPADAS_ERROR_MSG("i >= dimsData[0] || j >= dimsData[1] || k >= dimsData[2]"); \
		return *(new Type); \
	} \
	return vars->data[(i * dimsData[1] + j) * dimsData[2] + k]; \
} \
template<> \
Matrix<Type>::Matrix(Matrix<Type> src, Region2D srcRegion) \
{ \
    srcRegion.dim0 = math::max(srcRegion.dim0, 1u); \
    srcRegion.dim1 = math::max(srcRegion.dim1, 1u); \
    if (src.nDims() != 2) \
    { \
		SPADAS_ERROR_MSG("src.nDims() != 2"); \
        return; \
    } \
    if (srcRegion.offsetI < 0 || srcRegion.offsetJ < 0 || srcRegion.offsetI + srcRegion.dim0 > src.dimAt(0) || srcRegion.offsetJ + srcRegion.dim1 > src.dimAt(1)) \
    { \
		SPADAS_ERROR_MSG("srcRegion.offsetI < 0 || srcRegion.offsetJ < 0 || srcRegion.offsetI + srcRegion.dim0 > src.dimAt(0) || srcRegion.offsetJ + srcRegion.dim1 > src.dimAt(1)"); \
        return; \
    } \
	Array<UInt> dims(2); \
	dims[0] = srcRegion.dim0; dims[1] = srcRegion.dim1; \
	setVars(new MatrixVars<Type>(dims), TRUE); \
    UInt srcDim1 = src.dimAt(1); \
    Type *srcData = &src(srcRegion.offsetI, srcRegion.offsetJ); \
    for (UInt i = 0; i < srcRegion.dim0; i++) \
    { \
        Type *srcRow = &srcData[i * srcDim1]; \
        Type *dstRow = &vars->data[i * srcRegion.dim1]; \
        for (UInt j = 0; j < srcRegion.dim1; j++) \
        { \
            dstRow[j] = srcRow[j]; \
        } \
    } \
} \
template<> \
Matrix<Type>::Matrix(Matrix<Type> src, Region3D srcRegion) \
{ \
    srcRegion.dim0 = math::max(srcRegion.dim0, 1u); \
    srcRegion.dim1 = math::max(srcRegion.dim1, 1u); \
    srcRegion.dim2 = math::max(srcRegion.dim2, 1u); \
    if (src.nDims() != 3) \
    { \
		SPADAS_ERROR_MSG("src.nDims() != 3"); \
        return; \
    } \
    if (srcRegion.offsetI < 0 || srcRegion.offsetJ < 0 || srcRegion.offsetK < 0 || srcRegion.offsetI + srcRegion.dim0 > src.dimAt(0) || srcRegion.offsetJ + srcRegion.dim1 > src.dimAt(1) || srcRegion.offsetK + srcRegion.dim2 > src.dimAt(2)) \
    { \
		SPADAS_ERROR_MSG("srcRegion.offsetI < 0 || srcRegion.offsetJ < 0 || srcRegion.offsetK < 0 || srcRegion.offsetI + srcRegion.dim0 > src.dimAt(0) || srcRegion.offsetJ + srcRegion.dim1 > src.dimAt(1) || srcRegion.offsetK + srcRegion.dim2 > src.dimAt(2)"); \
        return; \
    } \
	Array<UInt> dims(3); \
	dims[0] = srcRegion.dim0; dims[1] = srcRegion.dim1; dims[2] = srcRegion.dim2; \
	setVars(new MatrixVars<Type>(dims), TRUE); \
    UInt srcDim1 = src.dimAt(1); \
    UInt srcDim2 = src.dimAt(2); \
    Type *srcData = &src(srcRegion.offsetI, srcRegion.offsetJ, srcRegion.offsetK); \
    for (UInt i = 0; i < srcRegion.dim0; i++) \
    { \
        Type *srcSlice = &srcData[i * srcDim1 * srcDim2]; \
        Type *dstSlice = &vars->data[i * srcRegion.dim1 * srcRegion.dim2]; \
        for (UInt j = 0; j < srcRegion.dim1; j++) \
        { \
            Type *srcRow = &srcSlice[j * srcDim2]; \
            Type *dstRow = &dstSlice[j * srcRegion.dim2]; \
            for (UInt k = 0; k < srcRegion.dim2; k++) \
            { \
                dstRow[k] = srcRow[k]; \
            } \
        } \
    } \
} \
template<> \
Array<UInt> Matrix<Type>::size() \
{ \
	if (!vars) \
	{ \
		SPADAS_ERROR_MSG("!vars"); \
		return Array<UInt>(); \
	} \
	return vars->dims.clone(); \
} \
template<> \
Array<UInt> Matrix<Type>::dims() \
{ \
	if (!vars) \
	{ \
		SPADAS_ERROR_MSG("!vars"); \
		return Array<UInt>(); \
	} \
	return vars->dims.clone(); \
} \
template<> \
Bool Matrix<Type>::isSize(Array<UInt> dims) \
{ \
	if (!vars) \
	{ \
		SPADAS_ERROR_MSG("!vars"); \
		return FALSE; \
	} \
	if (vars->dims.size() != dims.size()) return FALSE; \
	UInt nDims = vars->dims.size(); \
	for (UInt i = 0; i < nDims; i++) \
	{ \
		if (vars->dims[i] != dims[i]) return FALSE; \
	} \
	return TRUE; \
} \
template<> \
Type *Matrix<Type>::data() \
{ \
	if (!vars) \
	{ \
		SPADAS_ERROR_MSG("!vars"); \
		return 0; \
	} \
	return vars->data; \
} \
template<> \
Matrix<Type> Matrix<Type>::clone() \
{ \
	if (!vars) \
	{ \
		SPADAS_ERROR_MSG("!vars"); \
		return Matrix<Type>(); \
	} \
	Matrix<Type> out(vars->dims); \
	utility::memoryCopy(vars->data, out.vars->data, vars->nElems * sizeof(Type)); \
	return out; \
} \
template<> \
void Matrix<Type>::copy(Matrix<Type> src, Region2D srcRegion, CoordInt2D thisOffset) \
{ \
	if (!vars) \
	{ \
		SPADAS_ERROR_MSG("!vars"); \
		return; \
	} \
	if (!src.vars || vars == src.vars) \
	{ \
		SPADAS_ERROR_MSG("!src.vars || vars == src.vars"); \
		return; \
	} \
    srcRegion.dim0 = math::max(srcRegion.dim0, 1u); \
    srcRegion.dim1 = math::max(srcRegion.dim1, 1u); \
    if (srcRegion.offsetI < 0 || srcRegion.offsetJ < 0 || srcRegion.offsetI + srcRegion.dim0 > src.dimAt(0) || srcRegion.offsetJ + srcRegion.dim1 > src.dimAt(1)) \
    { \
		SPADAS_ERROR_MSG("srcRegion.offsetI < 0 || srcRegion.offsetJ < 0 || srcRegion.offsetI + srcRegion.dim0 > src.dimAt(0) || srcRegion.offsetJ + srcRegion.dim1 > src.dimAt(1)"); \
        return; \
    } \
    if (thisOffset.i < 0 || thisOffset.j < 0 || thisOffset.i + srcRegion.dim0 > vars->dims[0] || thisOffset.j + srcRegion.dim1 > vars->dims[1]) \
    { \
		SPADAS_ERROR_MSG("thisOffset.i < 0 || thisOffset.j < 0 || thisOffset.i + srcRegion.dim0 > vars->dims[0] || thisOffset.j + srcRegion.dim1 > vars->dims[1]"); \
        return; \
    } \
    UInt srcDim1 = src.dimAt(1); \
    UInt dstDim1 = vars->dims[1]; \
    Type *srcData = &src(srcRegion.offsetI, srcRegion.offsetJ); \
    Type *dstData = &operator()(thisOffset.i, thisOffset.j); \
    for (UInt i = 0; i < srcRegion.dim0; i++) \
    { \
        Type *srcRow = &srcData[i * srcDim1]; \
        Type *dstRow = &dstData[i * dstDim1]; \
        for (UInt j = 0; j < srcRegion.dim1; j++) \
        { \
            dstRow[j] = srcRow[j]; \
        } \
    } \
} \
template<> \
void Matrix<Type>::copy(Matrix<Type> src, Region3D srcRegion, CoordInt3D thisOffset) \
{ \
	if (!vars) \
	{ \
		SPADAS_ERROR_MSG("!vars"); \
		return; \
	} \
	if (!src.vars || vars == src.vars) \
	{ \
		SPADAS_ERROR_MSG("!src.vars || vars == src.vars"); \
		return; \
	} \
    srcRegion.dim0 = math::max(srcRegion.dim0, 1u); \
    srcRegion.dim1 = math::max(srcRegion.dim1, 1u); \
    srcRegion.dim2 = math::max(srcRegion.dim2, 1u); \
    if (srcRegion.offsetI < 0 || srcRegion.offsetJ < 0 || srcRegion.offsetK < 0 || srcRegion.offsetI + srcRegion.dim0 > src.dimAt(0) || srcRegion.offsetJ + srcRegion.dim1 > src.dimAt(1) || srcRegion.offsetK + srcRegion.dim2 > src.dimAt(2)) \
    { \
		SPADAS_ERROR_MSG("srcRegion.offsetI < 0 || srcRegion.offsetJ < 0 || srcRegion.offsetK < 0 || srcRegion.offsetI + srcRegion.dim0 > src.dimAt(0) || srcRegion.offsetJ + srcRegion.dim1 > src.dimAt(1) || srcRegion.offsetK + srcRegion.dim2 > src.dimAt(2)"); \
        return; \
    } \
    if (thisOffset.i < 0 || thisOffset.j < 0 || thisOffset.k < 0 || thisOffset.i + srcRegion.dim0 > vars->dims[0] || thisOffset.j + srcRegion.dim1 > vars->dims[1] || thisOffset.k + srcRegion.dim2 > vars->dims[2]) \
    { \
		SPADAS_ERROR_MSG("thisOffset.i < 0 || thisOffset.j < 0 || thisOffset.k < 0 || thisOffset.i + srcRegion.dim0 > vars->dims[0] || thisOffset.j + srcRegion.dim1 > vars->dims[1] || thisOffset.k + srcRegion.dim2 > vars->dims[2]"); \
        return; \
    } \
    UInt srcDim1 = src.dimAt(1); \
    UInt srcDim2 = src.dimAt(2); \
    UInt dstDim1 = vars->dims[1]; \
    UInt dstDim2 = vars->dims[2]; \
    Type *srcData = &src(srcRegion.offsetI, srcRegion.offsetJ, srcRegion.offsetK); \
    Type *dstData = &operator()(thisOffset.i, thisOffset.j, thisOffset.k); \
    for (UInt i = 0; i < srcRegion.dim0; i++) \
    { \
        Type *srcSlice = &srcData[i * srcDim1 * srcDim2]; \
        Type *dstSlice = &dstData[i * dstDim1 * dstDim2]; \
        for (UInt j = 0; j < srcRegion.dim1; j++) \
        { \
            Type *srcRow = &srcSlice[j * srcDim2]; \
            Type *dstRow = &dstSlice[j * dstDim2]; \
            for (UInt k = 0; k < srcRegion.dim2; k++) \
            { \
                dstRow[k] = srcRow[k]; \
            } \
        } \
    } \
} \
template<> \
void Matrix<Type>::clear(Type value) \
{ \
	if (!vars) \
	{ \
		SPADAS_ERROR_MSG("!vars"); \
		return; \
	} \
	Type *data0 = vars->data; \
	for (UInt i = 0; i < vars->nElems; i++) \
	{ \
		data0[i] = value; \
	} \
} \
template<> \
UInt Matrix<Type>::nElems() \
{ \
	if (!vars) \
	{ \
		SPADAS_ERROR_MSG("!vars"); \
		return 0; \
	} \
	return vars->nElems; \
} \
template<> \
Bool Matrix<Type>::isSize(Size2D size2D) \
{ \
	if (!vars) \
	{ \
		SPADAS_ERROR_MSG("!vars"); \
		return FALSE; \
	} \
	if (vars->dims.size() != 2) \
	{ \
		return FALSE; \
	} \
	return vars->dims[0] == size2D.height && vars->dims[1] == size2D.width; \
} \
template<> \
Bool Matrix<Type>::isSize(Size3D size3D) \
{ \
	if (!vars) \
	{ \
		SPADAS_ERROR_MSG("!vars"); \
		return FALSE; \
	} \
	if (vars->dims.size() != 3) \
	{ \
		return FALSE; \
	} \
	return vars->dims[0] == size3D.height && vars->dims[1] == size3D.width && vars->dims[2] == size3D.depth; \
} \
template<> \
CvMat *Matrix<Type>::cvMat() \
{ \
	if (!vars) \
	{ \
		SPADAS_ERROR_MSG("!vars"); \
		return NULL; \
	} \
	if (vars->dims.size() != 2) \
	{ \
		SPADAS_ERROR_MSG("vars->dims.size() != 2"); \
		return NULL; \
	} \
	if (!vars->cvMat) vars->cvMat = new CvMat(); \
	updateCVMat(vars->data, vars->cvMat); \
	vars->cvMat->rows = vars->dims[0]; \
	vars->cvMat->cols = vars->dims[1]; \
	vars->cvMat->step = vars->dims[1] * sizeof(Type); \
	vars->cvMat->refcount = NULL; \
	vars->cvMat->hdr_refcount = 0; \
	return vars->cvMat; \
} \
template<> \
Matrix<Type> Matrix<Type>::operator [](UInt i) \
{ \
	if (!vars) \
	{ \
		SPADAS_ERROR_MSG("!vars"); \
		return Matrix<Type>(); \
	} \
	UInt nDims = vars->dims.size(); \
	if (nDims == 0) \
	{ \
		SPADAS_ERROR_MSG("nDims == 0"); \
		return *this; \
	} \
	if (i >= vars->dims[0]) \
	{ \
		SPADAS_ERROR_MSG("i >= vars->dims[0]"); \
		i = 0; \
	} \
	MatrixVars<Type> *newVars = new MatrixVars<Type>(vars->data0, nDims - 1); \
	UInt newVarsNDims = newVars->dims.size(); \
	for (UInt j = 0; j < newVarsNDims; j++) \
	{ \
		newVars->dims[j] = vars->dims[j + 1]; \
	} \
    newVars->nElems = vars->nElems / vars->dims[0]; \
    newVars->data = &vars->data[i * newVars->nElems]; \
	Matrix<Type> out; \
	out.setVars(newVars, TRUE); \
    return out; \
} \
template<> \
Type& Matrix<Type>::operator ()(UInt i) \
{ \
	if (!vars) \
		{ \
		SPADAS_ERROR_MSG("!vars"); \
		return *(new Type); \
	} \
	if (vars->dims.size() != 1) \
	{ \
		SPADAS_ERROR_MSG("vars->dims.size() != 1"); \
		return *(new Type); \
	} \
	if (i >= vars->dims[0]) \
	{ \
		SPADAS_ERROR_MSG("i >= vars->dims[0]"); \
		return *(new Type); \
	} \
	return vars->data[i]; \
} \
template<> \
Matrix<Type> Matrix<Type>::operator +(Matrix<Type> matrix) \
{ \
	if (!vars || !matrix.vars) \
	{ \
		SPADAS_ERROR_MSG("!vars || !matrix.vars"); \
		return Matrix<Type>(); \
	} \
	if (!isSize(matrix.size())) \
	{ \
		SPADAS_ERROR_MSG("!isSize(matrix.size())"); \
		return *this; \
	} \
	Matrix<Type> out(vars->dims); \
	Type *src1 = vars->data; \
	Type *src2 = matrix.vars->data; \
	Type *dst = out.vars->data; \
	for (UInt i = 0; i < vars->nElems; i++) \
	{ \
		dst[i] = src1[i] + src2[i]; \
	} \
	return out; \
} \
template<> \
Matrix<Type> Matrix<Type>::operator -(Matrix<Type> matrix) \
{ \
	if (!vars || !matrix.vars) \
	{ \
		SPADAS_ERROR_MSG("!vars || !matrix.vars"); \
		return Matrix<Type>(); \
	} \
	if (!isSize(matrix.size())) \
	{ \
		SPADAS_ERROR_MSG("!isSize(matrix.size())"); \
		return *this; \
	} \
	Matrix<Type> out(vars->dims); \
	Type *src1 = vars->data; \
	Type *src2 = matrix.vars->data; \
	Type *dst = out.vars->data; \
	for (UInt i = 0; i < vars->nElems; i++) \
	{ \
		dst[i] = src1[i] - src2[i]; \
	} \
	return out; \
} \
template<> \
Matrix<Type> Matrix<Type>::operator *(Type scale) \
{ \
	if (!vars) \
	{ \
		SPADAS_ERROR_MSG("!vars"); \
		return Matrix<Type>(); \
	} \
	Matrix<Type> out(vars->dims); \
	Type *src = vars->data; \
	Type *dst = out.vars->data; \
	for (UInt i = 0; i < vars->nElems; i++) \
	{ \
		dst[i] = src[i] * scale; \
	} \
	return out; \
} \
template<> \
Matrix<Type> Matrix<Type>::operator *(Matrix<Type> matrix) \
{ \
	if (!vars || !matrix.vars) \
	{ \
		SPADAS_ERROR_MSG("!vars || !matrix.vars"); \
		return Matrix<Type>(); \
	} \
	if (vars->dims.size() != 2 || matrix.vars->dims.size() != 2) \
	{ \
		SPADAS_ERROR_MSG("vars->dims.size() != 2 || matrix.vars->dims.size() != 2"); \
		return *this; \
	} \
	if (vars->dims[1] != matrix.vars->dims[0]) \
	{ \
		SPADAS_ERROR_MSG("vars->dims[1] != matrix.vars->dims[0]"); \
		return *this; \
	} \
	UInt outRows = vars->dims[0], outCols = matrix.vars->dims[1]; \
	Matrix<Type> out(outRows, outCols); \
	Type *selfRow = vars->data; \
	Type *inData = matrix.vars->data; \
	Type *outRow = out.vars->data; \
	UInt selfStep = vars->dims[1]; \
	UInt inStep = matrix.vars->dims[1]; \
	UInt outStep = out.vars->dims[1]; \
	UInt inColIndex; \
	Type sum; \
	for (UInt i = 0; i < outRows; i++) \
	{ \
		for (UInt j = 0; j < outCols; j++) \
		{ \
			inColIndex = j; \
			sum = 0; \
			for (UInt k = 0; k < selfStep; k++, inColIndex += inStep) \
			{ \
				sum += selfRow[k] * inData[inColIndex]; \
			} \
			outRow[j] = sum; \
		} \
		selfRow = &selfRow[selfStep]; \
		outRow = &outRow[outStep]; \
	} \
	return out; \
} \
template<> \
Matrix<Type> Matrix<Type>::transpose() \
{ \
	if (!vars) \
	{ \
		SPADAS_ERROR_MSG("!vars"); \
		return Matrix<Type>(); \
	} \
	if (vars->dims.size() != 2) \
	{ \
		SPADAS_ERROR_MSG("vars->dims.size() != 2"); \
		return *this; \
	} \
	UInt nRows = vars->dims[0]; \
	UInt nCols = vars->dims[1]; \
	Matrix<Type> out(nCols, nRows); \
	for (UInt i = 0; i < nCols; i++) \
	{ \
		Type *outRow = &out.vars->data[i * nRows]; \
		UInt srcIndex0 = i; \
		for (UInt j = 0; j < nRows; j++) \
		{ \
			outRow[j] = vars->data[srcIndex0 + j*nCols]; \
		} \
	} \
	return out; \
} \
template<> \
void Matrix<Type>::convertTo(IntMat& dst) \
{ \
	matrixConvert<Type, Int>(*this, dst); \
} \
template<> \
void Matrix<Type>::convertTo(FloatMat& dst) \
{ \
	matrixConvert<Type, Float>(*this, dst); \
} \
template<> \
void Matrix<Type>::convertTo(DoubleMat& dst) \
{ \
	matrixConvert<Type, Double>(*this, dst); \
} \
template<> \
String Matrix<Type>::toString(UInt nDigits) \
{ \
	if (!vars) \
	{ \
		SPADAS_ERROR_MSG("!vars"); \
		return String(); \
	} \
    if (vars->dims.size() != 2) \
    { \
		SPADAS_ERROR_MSG("vars->dims.size() != 2"); \
        return String(); \
    } \
    UInt nRows = vars->dims[0]; \
    UInt nCols = vars->dims[1]; \
    String format = (String)"%." + nDigits + "lf\t"; \
    String outString = String::createWithSize(nRows * (nCols + 1) * 32); \
    Byte *outStringData = outString.bytes(); \
    String rowString = String::createWithSize(nCols * 32); \
	Byte *rowStringData = rowString.bytes(); \
    UInt outIndex = 0; \
    for (UInt i = 0; i < nRows; i++) \
    { \
        Type *rowData = &vars->data[i * nCols]; \
        rowStringData[0] = 0; \
        UInt index = 0; \
        for (UInt j = 0; j < nCols; j++) \
        { \
            UInt length = printToString((Char*)&rowStringData[index], 32, (Char*)format.bytes(), (Double)rowData[j]); \
            index += length == UINF ? lengthChar((Char*)&rowStringData[index]) : length; \
        } \
        rowString.updateLength(); \
        UInt rowLength = rowString.length(); \
        for (UInt n = 0; n < rowLength; n++) outStringData[outIndex + n] = rowStringData[n]; \
        outStringData[outIndex + rowLength] = 10; \
        outIndex += rowLength + 1; \
    } \
    outStringData[outIndex-1] = 0; \
    outString.updateLength(); \
    return outString; \
} \
template<> \
void Matrix<Type>::save(Path filePath, UInt nDigits) \
{ \
	if (!vars) \
	{ \
		SPADAS_ERROR_MSG("!vars"); \
		return; \
	} \
	if (filePath.isNull()) \
	{ \
		SPADAS_ERROR_MSG("filePath.isNull()"); \
		return; \
	} \
    if (vars->dims.size() != 2) \
    { \
		SPADAS_ERROR_MSG("vars->dims.size() != 2"); \
        return; \
    } \
    File file = File::createText(filePath); \
    if (!file.isValid()) return; \
    UInt nRows = vars->dims[0]; \
    UInt nCols = vars->dims[1]; \
    String format = (String)"%." + nDigits + "lf\t"; \
    String rowString = String::createWithSize(nCols * 32); \
	Byte *rowStringData = rowString.bytes(); \
    for (UInt i = 0; i < nRows; i++) \
    { \
        Type *rowData = &vars->data[i * nCols]; \
        rowStringData[0] = 0; \
        UInt index = 0; \
        for (UInt j = 0; j < nCols; j++) \
        { \
            UInt length = printToString((Char*)&rowStringData[index], 32, (Char*)format.bytes(), (Double)rowData[j]); \
            index += length == UINF ? lengthChar((Char*)&rowStringData[index]) : length; \
        } \
        rowString.updateLength(); \
        file.print(rowString); \
    } \
} \
template<> \
Matrix<Type>::Matrix(Path filePath) \
{ \
	if (filePath.isNull() || !filePath.exist()) \
	{ \
		SPADAS_ERROR_MSG("filePath.isNull() || !filePath.exist()"); \
		return; \
	} \
	File file = File::openText(filePath); \
	if (file.isNull()) return; \
	ArrayX<String> rowStringsBuf(256, String()); \
	while (TRUE) \
	{ \
		String newString = file.scan(); \
		if (newString.isEmpty()) break; \
		else rowStringsBuf.append(newString); \
			} \
	Array<String> rowStrings = rowStringsBuf.toArray(); \
	UInt nRows = rowStrings.size(); \
	if (nRows == 0) \
	{ \
		SPADAS_ERROR_MSG("nRows == 0"); \
		return; \
	} \
	UInt nCols = 0; \
	Array<String> firstRowElements = rowStrings[0].split('\t'); \
	for (UInt i = 0; i < firstRowElements.size(); i++) \
	{ \
		if (!firstRowElements[i].isEmpty()) nCols++; \
	} \
	if (nCols == 0) \
	{ \
		SPADAS_ERROR_MSG("nCols == 0"); \
		return; \
	} \
	Matrix<Type> out(nRows, nCols); \
	Type *outData = out.vars->data; \
	Array<UInt> startIndices(nCols); \
	Double val; \
	for (UInt i = 0; i < nRows; i++) \
	{ \
		String rowString = rowStrings[i]; \
		UInt rowStringLength = rowString.length(); \
		Binary rowChars(rowStringLength + 1); \
		Byte *charsSrc = rowString.bytes(); \
		Byte *charsDst = rowChars.data(); \
		Type *matRow = &outData[i * nCols]; \
		startIndices[0] = 0; \
		UInt k = 1; \
		for (UInt n = 0; n < rowStringLength; n++) \
		{ \
			if (charsSrc[n] == (Byte)'\t') \
			{ \
				charsDst[n] = 0; \
				if (k < nCols) startIndices[k++] = n + 1; \
			} \
			else charsDst[n] = charsSrc[n]; \
		} \
		charsDst[rowStringLength] = 0; \
		for (UInt n = 0; n < k; n++) \
		{ \
			if (scanFromString((Char*)&charsDst[startIndices[n]], rowChars.size() - startIndices[n], "%lf", &val)) matRow[n] = (Type)val; \
		} \
	} \
	operator =(out); \
} \
template<> \
Matrix<Type>::Matrix(CvMat *cvmat) \
{ \
	if (cvmat == NULL) \
	{ \
		SPADAS_ERROR_MSG("cvmat == NULL"); \
		return; \
	} \
	if (cvmat->type != 0x42424004 && cvmat->type != 0x42424005 && cvmat->type != 0x42424006) \
	{ \
		SPADAS_ERROR_MSG("cvmat->type != 0x42424004 && cvmat->type != 0x42424005 && cvmat->type != 0x42424006"); \
		return; \
	} \
	Matrix<Type> out(cvmat->rows, cvmat->cols); \
	Type *dstData = out.vars->data; \
	UInt nElems0 = out.nElems(); \
	switch (cvmat->type) \
	{ \
		case 0x42424004: \
		{ \
			Int *srcData = cvmat->data.i; \
			for (UInt i = 0; i < nElems0; i++) \
			{ \
				dstData[i] = (Type)srcData[i]; \
			} \
		} \
			break; \
		case 0x42424005: \
		{ \
			Float *srcData = cvmat->data.fl; \
			for (UInt i = 0; i < nElems0; i++) \
			{ \
				dstData[i] = (Type)srcData[i]; \
			} \
		} \
			break; \
		case 0x42424006: \
		{ \
			Double *srcData = cvmat->data.db; \
			for (UInt i = 0; i < nElems0; i++) \
			{ \
				dstData[i] = (Type)srcData[i]; \
			} \
		} \
			break; \
	} \
	operator =(out); \
}

namespace spadas
{
	INSTANTIATE_NELEMS(Int)
	INSTANTIATE_NELEMS(Float)
	INSTANTIATE_NELEMS(Double)
}
