
#include "spadas.h"

using namespace spadas;
using namespace spadas::math;

// Size2D
Size2D::Size2D() : width(0), height(0)
{
}

Bool Size2D::operator ==(Size2D size)
{
	return width == size.width && height == size.height;
}

Bool Size2D::operator !=(Size2D size)
{
	return !(width == size.width && height == size.height);
}

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
Size3D::Size3D() : width(0), height(0), depth(0)
{
}

Bool Size3D::operator ==(Size3D size)
{
	return width == size.width && height == size.height && depth == size.depth;
}

Bool Size3D::operator !=(Size3D size)
{
	return !(width == size.width && height == size.height && depth == size.depth);
}

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
Index2D::Index2D() : u(0), v(0)
{
}

Bool Index2D::operator==(Index2D index)
{
	return u == index.u && v == index.v;
}

Bool Index2D::operator!=(Index2D index)
{
	return !(u == index.u && v == index.v);
}

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
Index3D::Index3D() : u(0), v(0), w(0)
{
}

Bool Index3D::operator==(Index3D index)
{
	return u == index.u && v == index.v && w == index.w;
}

Bool Index3D::operator!=(Index3D index)
{
	return !(u == index.u && v == index.v && w == index.w);
}

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
Region::Region() : offset(0), size(0)
{
}

Region::Region(Int offset0, UInt size0) : offset(offset0), size(size0)
{
}

Bool Region::operator ==(Region region)
{
	return offset == region.offset && size == region.size;
}

Bool Region::operator !=(Region region)
{
	return !(offset == region.offset && size == region.size);
}

String Region::toString()
{
	return (String)offset + "->" + size;
}

// Region2D
Region2D::Region2D() : offsetU(0), offsetV(0), width(0), height(0)
{
}

Region2D::Region2D(CoordInt2D offset, Size2D size) : offsetU(offset.u), offsetV(offset.v), width(size.width), height(size.height)
{
}

Bool Region2D::operator ==(Region2D region)
{
	return offsetU == region.offsetU && width == region.width && offsetV == region.offsetV && height == region.height;
}

Bool Region2D::operator !=(Region2D region)
{
	return !(offsetU == region.offsetU && width == region.width && offsetV == region.offsetV && height == region.height);
}

CoordInt2D Region2D::offset()
{
    CoordInt2D out;
    out.u = offsetU;
    out.v = offsetV;
	return out;
}

Size2D Region2D::size()
{
    Size2D out;
    out.width = width;
    out.height = height;
	return out;
}

String Region2D::toString()
{
	return (String)"(" + offsetU + "->" + width + "," + offsetV + "->" + height + ")";
}

// Region3D
Region3D::Region3D() : offsetU(0), offsetV(0), offsetW(0), width(0), height(0), depth(0)
{
}

Region3D::Region3D(CoordInt3D offset, Size3D size) : offsetU(offset.u), offsetV(offset.v), offsetW(offset.w), width(size.width), height(size.height), depth(size.depth)
{
}

Bool Region3D::operator ==(Region3D region)
{
	return offsetU == region.offsetU && width == region.width && offsetV == region.offsetV && height == region.height && offsetW == region.offsetW && depth == region.depth;
}

Bool Region3D::operator !=(Region3D region)
{
	return !(offsetU == region.offsetU && width == region.width && offsetV == region.offsetV && height == region.height && offsetW == region.offsetW && depth == region.depth);
}

CoordInt3D Region3D::offset()
{
    CoordInt3D out;
    out.u = offsetU;
    out.v = offsetV;
    out.w = offsetW;
	return out;
}

Size3D Region3D::size()
{
    Size3D out;
    out.width = width;
    out.height = height;
    out.depth = depth;
	return out;
}

String Region3D::toString()
{
	return (String)"(" + offsetU + "->" + width + "," + offsetV + "->" + height + "," + offsetW + "->" + depth + ")";
}

// Vector2D
Vector2D::Vector2D() : x(0), y(0)
{
}

Vector2D::Vector2D(Float x0, Float y0) : x(x0), y(y0)
{
}

Bool Vector2D::operator ==(Vector2D vec)
{
	return x == vec.x && y == vec.y;
}

Bool Vector2D::operator !=(Vector2D vec)
{
	return !(x == vec.x && y == vec.y);
}

Vector2D Vector2D::operator +(Vector2D vec)
{
	return Vector2D(x + vec.x, y + vec.y);
}

Vector2D Vector2D::operator -(Vector2D vec)
{
	return Vector2D(x - vec.x, y - vec.y);
}

Vector2D Vector2D::operator *(Float k)
{
	return Vector2D(x * k, y * k);
}

Float Vector2D::operator *(Vector2D vec)
{
	return x * vec.x + y * vec.y;
}

Float Vector2D::absolute()
{
	return root(x*x+y*y);
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
Vector3D::Vector3D() : x(0), y(0), z(0)
{
}

Vector3D::Vector3D(Float x0, Float y0, Float z0) : x(x0), y(y0), z(z0)
{
}

Bool Vector3D::operator ==(Vector3D point)
{
	return x == point.x && y == point.y && z == point.z;
}

Bool Vector3D::operator !=(Vector3D point)
{
	return !(x == point.x && y == point.y && z == point.z);
}

Vector3D Vector3D::operator +(Vector3D point)
{
	return Vector3D(x + point.x, y + point.y, z + point.z);
}

Vector3D Vector3D::operator -(Vector3D point)
{
	return Vector3D(x - point.x, y - point.y, z - point.z);
}

Vector3D Vector3D::operator *(Float k)
{
	return Point3D(x * k, y * k, z * k);
}

Float Vector3D::operator *(Vector3D point)
{
	return x * point.x + y * point.y + z * point.z;
}

Float Vector3D::absolute()
{
	return root(x*x+y*y+z*z);
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
Range::Range() : lower(0), upper(0)
{
}

Range::Range(Float lower0, Float upper0) : lower(lower0), upper(upper0)
{
}

Bool Range::operator ==(Range range)
{
	return lower == range.lower && upper == range.upper;
}

Bool Range::operator !=(Range range)
{
	return !(lower == range.lower && upper == range.upper);
}

String Range::toString()
{
	return (String)lower + "~" + upper;
}

// Range2D
Range2D::Range2D() : lowerX(0), upperX(0), lowerY(0), upperY(0)
{
}

Range2D::Range2D(Float lowerX0, Float upperX0, Float lowerY0, Float upperY0) : lowerX(lowerX0), upperX(upperX0), lowerY(lowerY0), upperY(upperY0)
{
}

Bool Range2D::operator ==(Range2D range)
{
	return lowerX == range.lowerX && upperX == range.upperX && lowerY == range.lowerY && upperY == range.upperY;
}

Bool Range2D::operator !=(Range2D range)
{
	return !(lowerX == range.lowerX && upperX == range.upperX && lowerY == range.lowerY && upperY == range.upperY);
}

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
Range3D::Range3D() : lowerX(0), upperX(0), lowerY(0), upperY(0), lowerZ(0), upperZ(0)
{
}

Range3D::Range3D(Float lowerX0, Float upperX0, Float lowerY0, Float upperY0, Float lowerZ0, Float upperZ0) : lowerX(lowerX0), upperX(upperX0), lowerY(lowerY0), upperY(upperY0), lowerZ(lowerZ0), upperZ(upperZ0)
{
}

Bool Range3D::operator ==(Range3D range)
{
	return lowerX == range.lowerX && upperX == range.upperX && lowerY == range.lowerY && upperY == range.upperY && lowerZ == range.lowerZ && upperZ == range.upperZ;
}

Bool Range3D::operator !=(Range3D range)
{
	return !(lowerX == range.lowerX && upperX == range.upperX && lowerY == range.lowerY && upperY == range.upperY && lowerZ == range.lowerZ && upperZ == range.upperZ);
}

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