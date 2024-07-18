
extern "C"
{
#include "3party/svd.h"
}

#include "spadas.h"

using namespace spadas;

void spadas::math::decomposeSVD(FloatMat src, FloatMat& u, FloatMat& s, FloatMat& v)
{
	SPADAS_ERROR_RETURN(src.isNull());
	SPADAS_ERROR_RETURN(src.nDims() != 2 || src.dimAt(0) == 0 || src.dimAt(1) == 0);

	UInt rows = src.dimAt(0);
	UInt cols = src.dimAt(1);

	Array<Double*> w(rows + cols);
	for (UInt i = 0; i < w.size(); i++) w[i] = new Double[cols];

	for (UInt i = 0; i < rows; i++)
	{
		for (UInt j = 0; j < cols; j++)
		{
			w[i][j] = (Double)src(i, j);
		}
	}

	Array<Double> sArr(cols);
	UInt rank = (UInt)svd(w.data(), sArr.data(), rows, cols);

	s = FloatMat(cols, cols);
	for (UInt i = 0; i < cols; i++)
	{
		sArr[i] = i < rank ? math::root(sArr[i]) : 0;
		s(i, i) = (Float)sArr[i];
	}

	u = FloatMat(rows, cols);
	for (UInt i = 0; i < rows; i++)
	{
		for (UInt j = 0; j < cols; j++)
		{
			if (sArr[j] == 0) u(i, j) = 0;
			else u(i, j) = (Float)(w[i][j] / sArr[j]);
		}
	}

	v = FloatMat(cols, cols);
	for (UInt i = 0; i < cols; i++)
	{
		for (UInt j = 0; j < cols; j++)
		{
			v(i, j) = (Float)w[rows + i][j];
		}
	}

	for (UInt i = 0; i < w.size(); i++) delete[] w[i];
}
void spadas::math::decomposeSVD(DoubleMat src, DoubleMat& u, DoubleMat& s, DoubleMat& v)
{
	SPADAS_ERROR_RETURN(src.isNull());
	SPADAS_ERROR_RETURN(src.nDims() != 2 || src.dimAt(0) == 0 || src.dimAt(1) == 0);

	UInt rows = src.dimAt(0);
	UInt cols = src.dimAt(1);

	Array<Double*> w(rows + cols);
	for (UInt i = 0; i < w.size(); i++) w[i] = new Double[cols];

	for (UInt i = 0; i < rows; i++)
	{
		for (UInt j = 0; j < cols; j++)
		{
			w[i][j] = src(i, j);
		}
	}

	Array<Double> sArr(cols);
	UInt rank = (UInt)svd(w.data(), sArr.data(), rows, cols);

	s = DoubleMat(cols, cols);
	for (UInt i = 0; i < cols; i++)
	{
		sArr[i] = i < rank ? math::root(sArr[i]) : 0;
		s(i, i) = sArr[i];
	}

	u = DoubleMat(rows, cols);
	for (UInt i = 0; i < rows; i++)
	{
		for (UInt j = 0; j < cols; j++)
		{
			if (sArr[j] == 0) u(i, j) = 0;
			else u(i, j) = w[i][j] / sArr[j];
		}
	}

	v = DoubleMat(cols, cols);
	for (UInt i = 0; i < cols; i++)
	{
		for (UInt j = 0; j < cols; j++)
		{
			v(i, j) = w[rows + i][j];
		}
	}

	for (UInt i = 0; i < w.size(); i++) delete[] w[i];
}

FloatMat spadas::math::inverse(FloatMat src)
{
	SPADAS_ERROR_RETURNVAL(src.isNull(), FloatMat());
	SPADAS_ERROR_RETURNVAL(src.nDims() != 2 || src.dimAt(0) == 0 || src.dimAt(1) == 0, FloatMat());

	FloatMat u, s, v;
	decomposeSVD(src, u, s, v);
	
	Double thresh = s.data()[0] * 0.0000001f;
	
	UInt dim = s.dimAt(0);
	FloatMat s1(dim, dim);
	s1.set(0);
	for (UInt i = 0; i < dim; i++)
	{
		s1(i, i) = math::abs(s(i, i)) <= thresh ? 0 : (Float)1 / s(i, i);
	}
	
	return v * s1 * u.transpose();
}
DoubleMat spadas::math::inverse(DoubleMat src)
{
	SPADAS_ERROR_RETURNVAL(src.isNull(), DoubleMat());
	SPADAS_ERROR_RETURNVAL(src.nDims() != 2 || src.dimAt(0) == 0 || src.dimAt(1) == 0, DoubleMat());

	DoubleMat u, s, v;
	decomposeSVD(src, u, s, v);

	Double thresh = s.data()[0] * 0.0000001f;
	
	UInt dim = s.dimAt(0);
	DoubleMat s1(dim, dim);
	s1.set(0);
	for (UInt i = 0; i < dim; i++)
	{
		s1(i, i) = math::abs(s(i, i)) <= thresh ? 0 : (Double)1 / s(i, i);
	}
	
	return v * s1 * u.transpose();
}

Array<Float> spadas::math::solve(FloatMat a, Array<Float> b)
{
	SPADAS_ERROR_RETURNVAL(a.isNull(), Array<Float>());
	SPADAS_ERROR_RETURNVAL(a.nDims() != 2 || a.dimAt(0) == 0 || a.dimAt(1) == 0, Array<Float>());
	SPADAS_ERROR_RETURNVAL(a.dimAt(0) != b.size(), Array<Float>());

	FloatMat x = inverse(a) * FloatMat(b);
	UInt outSize = x.dimAt(0);
	
	Array<Float> out(outSize);
	for (UInt i = 0; i < outSize; i++)
	{
		out[i] = x.data()[i];
	}
	return out;
}
Array<Double> spadas::math::solve(DoubleMat a, Array<Double> b)
{
	SPADAS_ERROR_RETURNVAL(a.isNull(), Array<Double>());
	SPADAS_ERROR_RETURNVAL(a.nDims() != 2 || a.dimAt(0) == 0 || a.dimAt(1) == 0, Array<Double>());
	SPADAS_ERROR_RETURNVAL(a.dimAt(0) != b.size(), Array<Double>());

	DoubleMat x = inverse(a) * DoubleMat(b);
	UInt outSize = x.dimAt(0);
	
	Array<Double> out(outSize);
	for (UInt i = 0; i < outSize; i++)
	{
		out[i] = x.data()[i];
	}
	return out;
}