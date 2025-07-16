#define SPADAS_ENABLE_MACROS
#include "spadas.h"

using namespace spadas;

void exampleMatrix()
{
	// Initialize a matrix / 矩阵初始化
	DoubleMat a(3, 3);
	a(0, 0) = 1; a(0, 1) = 2; a(0, 2) = 3;
	a(1, 0) = 4; a(1, 1) = 5; a(1, 2) = 7;
	a(2, 0) = 8; a(2, 1) = 10; a(2, 2) = 15;
	console::print("a=");
	console::print(a);

	DoubleMat x(3, 1);
	x(0, 0) = 11;
	x(1, 0) = 12;
	x(2, 0) = 13;
	console::print("x=");
	console::print(x);

	// Matrix multiplication / 矩阵相乘
	DoubleMat b = a * x;
	console::print("b=a*x=");
	console::print(b);
	
	// Matrix inversion / 矩阵求逆
	DoubleMat aInv = math::inverse(a);
	console::print("aInv=");
	console::print(aInv);

	console::print("a*aInv=");
	console::print(a * aInv);

	// Equation solving / 方程求解
	Array<Double> bArr(b.dimAt(0));
	for (UInt i = 0; i < b.dimAt(0); i++) bArr[i] = b(i, 0);

	Array<Double> x2 = math::solve(a, bArr);
	console::print("Solution of a*x=b: " cat String::merge(x2, ", "));
}