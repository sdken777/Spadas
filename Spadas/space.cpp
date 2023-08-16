
#include "spadas.h"

using namespace spadas;
using namespace spadas::math;

// Size2D
Size2D Size2D::wh(UInt width, UInt height)
{
    Size2D out;
    out.width = width;
    out.height = height;
    return out;
}

Size2D Size2D::dims(UInt dim0, UInt dim1)
{
    Size2D out;
    out.dim0 = dim0;
    out.dim1 = dim1;
    return out;
}

String Size2D::toString()
{
	return (String)width + "x" + height;
}

// Size3D
Size3D Size3D::whd(UInt width, UInt height, UInt depth)
{
    Size3D out;
    out.width = width;
    out.height = height;
    out.depth = depth;
    return out;
}

Size3D Size3D::dims(UInt dim0, UInt dim1, UInt dim2)
{
    Size3D out;
    out.dim0 = dim0;
    out.dim1 = dim1;
    out.dim2 = dim2;
    return out;
}

String Size3D::toString()
{
	return (String)width + "x" + height + "x" + depth;
}

// Index2D
Index2D Index2D::uv(Int u, Int v)
{
    Index2D out;
    out.u = u;
    out.v = v;
    return out;
}

Index2D Index2D::ij(Int i, Int j)
{
    Index2D out;
    out.i = i;
    out.j = j;
    return out;
}

String Index2D::toString()
{
	return (String)"(" + u + "," + v + ")";
}

// Index3D
Index3D Index3D::uvw(Int u, Int v, Int w)
{
    Index3D out;
    out.u = u;
    out.v = v;
    out.w = w;
    return out;
}

Index3D Index3D::ijk(Int i, Int j, Int k)
{
    Index3D out;
    out.i = i;
    out.j = j;
    out.k = k;
    return out;
}

String Index3D::toString()
{
	return (String)"(" + u + "," + v + "," + w + ")";
}

// Region
String Region::toString()
{
	return (String)offset + "->" + size;
}

// Region2D
String Region2D::toString()
{
	return (String)"(" + offsetU + "->" + width + "," + offsetV + "->" + height + ")";
}

// Region3D
String Region3D::toString()
{
	return (String)"(" + offsetU + "->" + width + "," + offsetV + "->" + height + "," + offsetW + "->" + depth + ")";
}

// Vector2D
Float Vector2D::absolute()
{
	return root(x * x + y * y);
}

Vector2D Vector2D::uv(Float u, Float v)
{
    Vector2D out;
    out.u = u;
    out.v = v;
    return out;
}

Vector2D Vector2D::ij(Float i, Float j)
{
    Vector2D out;
    out.i = i;
    out.j = j;
    return out;
}

String Vector2D::toString()
{
	return (String)"(" + x + "," + y + ")";
}

// Vector3D
Float Vector3D::absolute()
{
	return root(x * x + y * y + z * z);
}

Vector3D Vector3D::uvw(Float u, Float v, Float w)
{
    Vector3D out;
    out.u = u;
    out.v = v;
    out.w = w;
    return out;
}

Vector3D Vector3D::ijk(Float i, Float j, Float k)
{
    Vector3D out;
    out.i = i;
    out.j = j;
    out.k = k;
    return out;
}

String Vector3D::toString()
{
	return (String)"(" + x + "," + y + "," + z + ")";
}

// Range
String Range::toString()
{
	return (String)lower + "~" + upper;
}

// Range2D
Range2D Range2D::uv(Float lowerU, Float upperU, Float lowerV, Float upperV)
{
    Range2D out;
    out.lowerU = lowerU;
    out.upperU = upperU;
    out.lowerV = lowerV;
    out.upperV = upperV;
    return out;
}

Range2D Range2D::ij(Float lowerI, Float upperI, Float lowerJ, Float upperJ)
{
    Range2D out;
    out.lowerI = lowerI;
    out.upperI = upperI;
    out.lowerJ = lowerJ;
    out.upperJ = upperJ;
    return out;
}

String Range2D::toString()
{
	return (String)"(" + lowerX + "~" + upperX + "," + lowerY + "~" + upperY + ")";
}

// Range3D
Range3D Range3D::uvw(Float lowerU, Float upperU, Float lowerV, Float upperV, Float lowerW, Float upperW)
{
    Range3D out;
    out.lowerU = lowerU;
    out.upperU = upperU;
    out.lowerV = lowerV;
    out.upperV = upperV;
    out.lowerW = lowerW;
    out.upperW = upperW;
    return out;
}

Range3D Range3D::ijk(Float lowerI, Float upperI, Float lowerJ, Float upperJ, Float lowerK, Float upperK)
{
    Range3D out;
    out.lowerI = lowerI;
    out.upperI = upperI;
    out.lowerJ = lowerJ;
    out.upperJ = upperJ;
    out.lowerK = lowerK;
    out.upperK = upperK;
    return out;
}

String Range3D::toString()
{
	return (String)"(" + lowerX + "~" + upperX + "," + lowerY + "~" + upperY + "," + lowerZ + "~" + upperZ + ")";
}