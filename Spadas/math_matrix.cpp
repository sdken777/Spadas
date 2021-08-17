
#include "spadas.h"

namespace math_internal
{
	using namespace spadas;
	using namespace spadas::math;

	void getEps(Float& out)
	{
		out = FEPS;
	}
	void getEps(Double& out)
	{
		out = DEPS;
	}
	Float getThresh(Float s0)
	{
		return s0 * 0.0000001f;
	}
	Double getThresh(Double s0)
	{
		return s0 * 0.000000000000001;
	}
	inline Float sign(Float a, Float b)
	{
		return b >= 0 ? (a >= 0 ? a : -a) : (a >= 0 ? -a : a);
	}
	inline Double sign(Double a, Double b)
	{
		return b >= 0 ? (a >= 0 ? a : -a) : (a >= 0 ? -a : a);
	}
	inline Float pythag(Float a, Float b)
	{
		Float absa=math::abs(a), absb=math::abs(b);
		return (absa > absb ? absa*root(1.0f+square(absb/absa)) :
				(absb == 0.0f ? 0.0f : absb*root(1.0f+square(absa/absb))));
	}
	inline Double pythag(Double a, Double b)
	{
		Double absa=math::abs(a), absb=math::abs(b);
		return (absa > absb ? absa*root(1.0+square(absb/absa)) :
				(absb == 0.0 ? 0.0 : absb*root(1.0+square(absa/absb))));
	}
	
	template<typename Type>
	Bool decompose(Type *uData, Type *wData, Type *vData, Int uDimAt0, Int uDimAt1)
	{
		Int m = uDimAt0, n = uDimAt1;
		Type eps;
		getEps(eps);
		Bool flag;
		Int i,its,j,jj,k,l,nm,a,b;
		Type anorm,c,f,g,h,s,scale,x,y,z;
		Array<Type> rv1(n);
		g = scale = anorm = 0.0;
		
		for (i=0;i<n;i++)
		{
			l=i+2;
			rv1[i]=scale*g;
			g=s=scale=0.0;
			
			if (i < m)
			{
				for (k=i,a=i*(n+1);k<m;k++,a+=n)
				{
					scale += math::abs(uData[a]);
				}
				if (scale != 0.0)
				{
					Type scale1 = (Type)1 / scale;
					for (k=i,a=i*(n+1);k<m;k++,a+=n)
					{
						uData[a] *= scale1;
						s += uData[a]*uData[a];
					}
					f=uData[i*(n+1)];
					g = -sign(root(s),f);
					h=f*g-s;
					uData[i*(n+1)]=f-g;
					for (j=l-1;j<n;j++)
					{
						for (s=0.0,k=i,a=i*n+i,b=i*n+j;k<m;k++,a+=n,b+=n)
						{
							s += uData[a]*uData[b];
						}
						f=s/h;
						for (k=i,a=i*n+i,b=i*n+j;k<m;k++,a+=n,b+=n)
						{
							uData[b] += f*uData[a];
						}
					}
					for (k=i,a=i*(n+1);k<m;k++,a+=n)
					{
						uData[a] *= scale;
					}
				}
			}
			wData[i]=scale *g;
			g=s=scale=0.0;
			if (i+1 <= m && i+1 != n)
			{
				Type *uRowI = &uData[i*n];
				for (k=l-1;k<n;k++)
				{
					scale += math::abs(uRowI[k]);
				}
				if (scale != 0.0)
				{
					Type scale1 = (Type)1 / scale;
					for (k=l-1;k<n;k++)
					{
						uRowI[k] *= scale1;
						s += uRowI[k]*uRowI[k];
					}
					f=uRowI[l-1];
					g = -sign(root(s),f);
					h=f*g-s;
					Type h1 = (Type)1/h;
					uRowI[l-1]=f-g;
					for (k=l-1;k<n;k++)
					{
						rv1[k]=uRowI[k]*h1;
					}
					for (j=l-1;j<m;j++)
					{
						Type *uRowJ = &uData[j*n];
						for (s=0.0,k=l-1;k<n;k++)
						{
							s += uRowJ[k]*uRowI[k];
						}
						for (k=l-1;k<n;k++)
						{
							uRowJ[k] += s*rv1[k];
						}
					}
					for (k=l-1;k<n;k++)
					{
						uRowI[k] *= scale;
					}
				}
			}
			anorm=max(anorm,(math::abs(wData[i])+math::abs(rv1[i])));
		}
		for (i=n-1;i>=0;i--)
		{
			if (i < n-1)
			{
				if (g != 0.0)
				{
					Type *uRowI = &uData[i*n];
					Type guIL1 = (Type)1/uRowI[l]/g;
					for (j=l,a=l*n+i;j<n;j++,a+=n)
					{
						vData[a]=uRowI[j]*guIL1;
					}
					for (j=l;j<n;j++)
					{
						for (s=0.0,k=l,a=l*n+j;k<n;k++,a+=n)
						{
							s += uRowI[k]*vData[a];
						}
						for (k=l,a=l*n+i,b=l*n+j;k<n;k++,a+=n,b+=n)
						{
							vData[b] += s*vData[a];
						}
					}
				}
				Type *vRowI = &vData[uDimAt1];
				for (j=l,a=l*n+i;j<n;j++,a+=n)
				{
					vRowI[j]=vData[a]=0.0;
				}
			}
			vData[i*(n+1)]=1.0;
			g=rv1[i];
			l=i;
		}
		for (i=min(m,n)-1;i>=0;i--)
		{
			l=i+1;
			g=wData[i];
			Type *uRowI = &uData[i*n];
			for (j=l;j<n;j++)
			{
				uRowI[j]=0.0;
			}
			if (g != 0.0)
			{
				g=(Type)1/g;
				Type uII1 = (Type)1/uRowI[i];
				for (j=l;j<n;j++)
				{
					for (s=0.0,k=l,a=l*n+i,b=l*n+j;k<m;k++,a+=n,b+=n)
					{
						s += uData[a]*uData[b];
					}
					f=s*uII1*g;
					for (k=i,a=i*n+i,b=i*n+j;k<m;k++,a+=n,b+=n)
					{
						uData[b] += f*uData[a];
					}
				}
				for (j=i,a=i*(n+1);j<m;j++,a+=n)
				{
					uData[a] *= g;
				}
			}
			else
			{
				for (j=i,a=i*(n+1);j<m;j++,a+=n)
				{
					uData[a]=0.0;
				}
			}
			++uData[i*(n+1)];
		}
		for (k=n-1;k>=0;k--)
		{
			for (its=0;its<30;its++)
			{
				flag=TRUE;
				for (l=k;l>=0;l--)
				{
					nm=l-1;
					if (l == 0 || math::abs(rv1[l]) <= eps*anorm)
					{
						flag=FALSE;
						break;
					}
					if (math::abs(wData[nm]) <= eps*anorm) break;
				}
				if (flag)
				{
					c=0.0;
					s=1.0;
					for (i=l;i<k+1;i++)
					{
						f=s*rv1[i];
						rv1[i]=c*rv1[i];
						if (math::abs(f) <= eps*anorm) break;
						g=wData[i];
						h=pythag(f,g);
						wData[i]=(Type)h;
						h=(Type)1/h;
						c=g*h;
						s = -f*h;
						for (j=0,a=i,b=nm;j<m;j++,a+=n,b+=n)
						{
							y=uData[b];
							z=uData[a];
							uData[b]=y*c+z*s;
							uData[a]=z*c-y*s;
						}
					}
				}
				z=wData[k];
				if (l == k)
				{
					if (z < 0.0)
					{
						wData[k] = -z;
						for (j=0,a=k;j<n;j++,a+=n)
						{
							vData[a] = -vData[a];
						}
					}
					break;
				}
				if (its == 29) return FALSE;
				x=wData[l];
				nm=k-1;
				y=wData[nm];
				g=rv1[nm];
				h=rv1[k];
				f=(Type)(((y-z)*(y+z)+(g-h)*(g+h))/(2.0*h*y));
				g=pythag(f,(Type)1.0);
				f=((x-z)*(x+z)+h*((y/(f+sign(g,f)))-h))/x;
				c=s=1.0;
				for (j=l;j<=nm;j++)
				{
					i=j+1;
					g=rv1[i];
					y=wData[i];
					h=s*g;
					g=c*g;
					z=pythag(f,h);
					rv1[j]=z;
					c=f/z;
					s=h/z;
					f=x*c+g*s;
					g=g*c-x*s;
					h=y*s;
					y *= c;
					for (jj=0,a=i,b=j;jj<n;jj++,a+=n,b+=n)
					{
						x=vData[b];
						z=vData[a];
						vData[b]=x*c+z*s;
						vData[a]=z*c-x*s;
					}
					z=pythag(f,h);
					wData[j]=z;
					if (z)
					{
						z=(Type)1/z;
						c=f*z;
						s=h*z;
					}
					f=c*g+s*y;
					x=c*y-s*g;
					for (jj=0,a=i,b=j;jj<m;jj++,a+=n,b+=n)
					{
						y=uData[b];
						z=uData[a];
						uData[b]=y*c+z*s;
						uData[a]=z*c-y*s;
					}
				}
				rv1[l]=0.0;
				rv1[k]=f;
				wData[k]=x;
			}
		}
		return TRUE;
	}
	
	template <typename Type>
	void reorder(Type *uData, Type *wData, Type *vData, Int uDimAt0, Int uDimAt1){
		
		Int m = uDimAt0, n = uDimAt1;
		Int i,j,k,s,inc=1,a,b;
		Type sw;
		Array<Type>su(m), sv(n);
		
		do
		{
			inc *= 3;
			inc++;
		}
		while (inc <= n);
		
		do
		{
			inc /= 3;
			for (i=inc;i<n;i++)
			{
				sw = wData[i];
				for (k=0,a=i;k<m;k++,a+=n)
				{
					su[k] = uData[a];
				}
				for (k=0,a=i;k<n;k++,a+=n)
				{
					sv[k] = vData[a];
				}
				j = i;
				while (wData[j-inc] < sw)
				{
					wData[j] = wData[j-inc];
					for (k=0,a=j,b=j-inc;k<m;k++,a+=n,b+=n)
					{
						uData[a] = uData[b];
					}
					for (k=0,a=j,b=j-inc;k<n;k++,a+=n,b+=n)
					{
						vData[a] = vData[b];
					}
					j -= inc;
					if (j < inc) break;
				}
				wData[j] = sw;
				for (k=0,a=j;k<m;k++,a+=n)
				{
					uData[a] = su[k];
				}
				for (k=0,a=j;k<n;k++,a+=n)
				{
					vData[a] = sv[k];
				}
			}
		} while (inc > 1);
		
		for (k=0;k<n;k++)
		{
			s=0;
			for (i=0,a=k;i<m;i++,a+=n)
			{
				if (uData[a] < 0.) s++;
			}
			for (j=0,a=k;j<n;j++,a+=n)
			{
				if (vData[a] < 0.) s++;
			}
			if (s > (m+n)/2)
			{
				for (i=0,a=k;i<m;i++,a+=n)
				{
					uData[a] = -uData[a];
				}
				for (j=0,a=k;j<n;j++,a+=n)
				{
					vData[a] = -vData[a];
				}
			}
		}
	}
}

using namespace spadas;
using namespace math_internal;

void spadas::math::decomposeSVD(FloatMat src, FloatMat& u, FloatMat& s, FloatMat& v)
{
	SPADAS_ERROR_RETURN(src.isNull());
	SPADAS_ERROR_RETURN(src.nDims() != 2 || src.dimAt(0) == 0 || src.dimAt(1) == 0);

	u = src.clone();
	v = FloatMat(src.dimAt(1), src.dimAt(1));
	Array<Float> arrS(src.dimAt(1));

	decompose(u.data(), arrS.data(), v.data(), (Int)u.dimAt(0), (Int)u.dimAt(1));
	reorder(u.data(), arrS.data(), v.data(), (Int)u.dimAt(0), (Int)u.dimAt(1));
	
	s = FloatMat(src.dimAt(1), src.dimAt(1));
	for (UInt i = 0; i < src.dimAt(1); i++)
	{
		s(i, i) = arrS[i];
	}
}
void spadas::math::decomposeSVD(DoubleMat src, DoubleMat& u, DoubleMat& s, DoubleMat& v)
{
	SPADAS_ERROR_RETURN(src.isNull());
	SPADAS_ERROR_RETURN(src.nDims() != 2 || src.dimAt(0) == 0 || src.dimAt(1) == 0);

	u = src.clone();
	v = DoubleMat(src.dimAt(1), src.dimAt(1));
	Array<Double> arrS(src.dimAt(1));

	decompose(u.data(), arrS.data(), v.data(), (Int)u.dimAt(0), (Int)u.dimAt(1));
	reorder(u.data(), arrS.data(), v.data(), (Int)u.dimAt(0), (Int)u.dimAt(1));
	
	s = DoubleMat(src.dimAt(1), src.dimAt(1));
	for (UInt i = 0; i < src.dimAt(1); i++)
	{
		s(i, i) = arrS[i];
	}
}

FloatMat spadas::math::inverse(FloatMat src)
{
	SPADAS_ERROR_RETURNVAL(src.isNull(), FloatMat());
	SPADAS_ERROR_RETURNVAL(src.nDims() != 2 || src.dimAt(0) == 0 || src.dimAt(1) == 0, FloatMat());

	FloatMat u, s, v;
	decomposeSVD(src, u, s, v);
	
	Float thresh = getThresh(s.data()[0]);
	
	UInt dim = s.dimAt(0);
	FloatMat s1(dim, dim);
	s1.clear(0);
	for (UInt i = 0; i < dim; i++)
	{
		s1(i, i) = s(i, i) <= thresh ? 0 : (Float)1 / s(i, i);
	}
	
	return v * s1 * u.transpose();
}
DoubleMat spadas::math::inverse(DoubleMat src)
{
	SPADAS_ERROR_RETURNVAL(src.isNull(), DoubleMat());
	SPADAS_ERROR_RETURNVAL(src.nDims() != 2 || src.dimAt(0) == 0 || src.dimAt(1) == 0, DoubleMat());

	DoubleMat u, s, v;
	decomposeSVD(src, u, s, v);
	
	Double thresh = getThresh(s.data()[0]);
	
	UInt dim = s.dimAt(0);
	DoubleMat s1(dim, dim);
	s1.clear(0);
	for (UInt i = 0; i < dim; i++)
	{
		s1(i, i) = s(i, i) <= thresh ? 0 : (Double)1 / s(i, i);
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