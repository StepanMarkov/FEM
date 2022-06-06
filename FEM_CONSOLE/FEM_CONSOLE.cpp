#pragma once

#include "stdafx.h"
//#include "..\\FEM\FEM_MATH.h"
//#include "..\\FEM\FEM_SOLVER.h"
//#include "..\\FEM\FEM_RESULTS.h"
#include "..\\FEM\FEM_GRAPH.h"
#include <iostream>
#include <fstream>
#include <ctime>
#include <stdarg.h>
#include "DirectXMath.h"
#include <vector>
#include <list>
#include <deque>
#include <intrin.h>

using namespace std;
using namespace DirectX;

float Geom(XMVECTOR X)
{
	//if (XMVectorGetX(X) > -0.9) return -0.11;
	return 0.08;
}

//===========================================================================//
int main() {

	//cout << typeid(VRTX).name() << endl;

	buffer = new ELEM[500000];
	PELEM Mesh = ELEM::Create();
	auto clock = __rdtsc();
	Mesh = ELEM::Meshing(Geom);
	clock = __rdtsc() - clock;
	cout << clock << ' ' << endl;
	cout << Mesh->GetSize(CELL) << endl;
	cout << clock / Mesh->GetSize(CELL) << endl;



	//Mesh = ELEM::GetMeshSurface(Mesh);
	//ELEM::SetDataBuffer(Mesh);
    
	

	system("PAUSE");

	return 0;
}

