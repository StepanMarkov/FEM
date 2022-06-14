#pragma once

#include "stdafx.h"
//#include "..\\FEM\FEM_MATH.h"
//#include "..\\FEM\FEM_SOLVER.h"
//#include "..\\FEM\FEM_RESULTS.h"
#include "..\\FEM\FEM_GRAPH.h"
#include "..\\FEM\FEM_MESH.h"
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

float Geom(const XMVECTOR& X) {
	return 0.07;
}

//===========================================================================//
int main() {

	//cout << typeid(VRTX).name() << endl;

	//buffer = new ELEM[1000000]; 
	vertexbuf = new XMVECTOR[200000];
	vertexbufbegin = vertexbuf;
	CMESH Mesh;
	//PELEM Mesh = ELEM::Create();
	auto clock = __rdtsc();
	Mesh.Meshing(Geom);
	//Mesh = ELEM::Meshing(Geom);
	clock = __rdtsc() - clock;
	cout << clock << ' ' << endl;
	cout << Mesh.buffer[CELL] - Mesh.Links[CELL] << endl;
	cout << Mesh.buffer[VRTX] - Mesh.Links[VRTX] << endl;
	cout << Mesh.buffer[LINE] - Mesh.Links[LINE] << endl;
	cout << Mesh.buffer[SIDE] - Mesh.Links[SIDE] << endl;
	cout << clock / (Mesh.buffer[CELL] - Mesh.Links[CELL]) << endl;
	//ELEM::SetDataBuffer(Mesh);
	CMESH::WriteMeshVTK(Mesh, "solution.vtk", 1);
	cout << "DONE" << endl;


	//Mesh = ELEM::GetMeshSurface(Mesh);
	//ELEM::SetDataBuffer(Mesh);
    
	

	system("PAUSE");

	return 0;
}

