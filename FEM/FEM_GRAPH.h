#pragma once
#include <vector>
#include "DirectXMath.h"
#include <string>
#include <fstream>
#include <iostream>

using namespace std;
using namespace DirectX;

//==========================================================//

class ELEM;
typedef ELEM* PELEM;
typedef vector<PELEM> VPELEM;

//==========================================================//
enum TYPES 
{ 
	VRTX,													//¬≈–ÿ»Õ¿ (Œœ–≈ƒ≈Àﬂ≈“  ŒŒ–ƒ»Õ¿“€)
	NODE,													//”«≈À
	LINE,													//√–¿Õ»
	SIDE,													//›À≈Ã≈Õ“ œŒ¬≈–’ÕŒ—“»
	CELL,													//Œ¡⁄≈ÃÕ€… ›À≈Ã≈Õ“
	MESH													//—≈“ ¿
};
//==========================================================//

//class GRAPH {
//
//};
//
//class Node : public GRAPH {
//	PELEM VRTX[NODE];
//};


static ELEM* buffer;


class ELEM {

private:


	VPELEM Links[MESH + 1];
	void* Data;
	bool trave = false;

public:

	static void onselect(VPELEM& v) {
		for (auto& p : v)
			p->trave = true;
	}

	static void unselect(VPELEM& v) {
		for (auto& p : v)
			p->trave = false;
	}

	//auto& operator() (TYPES index) {
	//	return this->Links[index];
	//}

	//======================================================//

	~ELEM()
	{
		for (int i = VRTX; i <= MESH; i++)
		{
			this->Links[i].clear();
			this->Links[i].~vector();
		}

		delete Data;
	}

	//======================================================//
	//static void Erase(PELEM p, unsigned char n)				//”ƒ¿Àﬂ≈“ ›À≈Ã≈Õ“ »« —≈“ »
	//{	
	//	for (int i = VRTX; i <= MESH; i++)					//÷» À œŒ “»œ¿Ã ›À≈Ã≈Õ“Œ¬
	//		for (PELEM pp : p->Links[i])					//÷» À œŒ —¬ﬂ«¿ÕÕ€Ã ›À≈Ã≈Õ“¿Ã
	//			pp->Links[n] -= p;							//”ƒÀﬂÀﬂÃ »« —¬ﬂ«≈…
	//
	//	if (n != MESH - 1)									//”ƒ¿Àﬂ≈Ã —“¿–ÿ»≈ ›À≈Ã≈Õ“€
	//		for (PELEM pp : p->Links[n + 1])
	//			Erase(pp, n + 1);
	//
	//	delete p;											//Œ—¬Œ¡Œ∆ƒ¿≈Ã œ¿Ãﬂ“‹
	//}
	//======================================================//
	void* GetData() {
		return this->Data;
	}
	//======================================================//
	size_t GetSize(unsigned char Type) {
		return this->Links[Type].size();
	}
	//======================================================//
	unsigned char GetType() {
		if (this->Links[MESH].size() == 0)
			return MESH;
		return Links[VRTX].size();
	}
	//======================================================//
	static PELEM Create() {
		return buffer++;
	}
	//======================================================//
	static PELEM Create(PELEM& pm) {
		pm->Links[VRTX].push_back(buffer);
		buffer->Links[MESH].push_back(pm);
		buffer->Data = new XMVECTOR;
		return buffer++;
	}
	//======================================================//
	static PELEM Create(PELEM& p, PELEM& pm) {
		pm->Links[NODE].push_back(buffer);
		buffer->Links[MESH].push_back(pm);
		p->Links[NODE].push_back(buffer);
		buffer->Links[VRTX].push_back(p);
		return buffer++;
	}
	//======================================================//
	static bool Bind(PELEM p1, PELEM p2, PELEM pm) {
		p1->Links[p2->GetType()].push_back(p2);
		p2->Links[p1->GetType()].push_back(p1);
		return Link(p1, p2, pm);
	}
	//======================================================//
	static bool Link(PELEM p1, PELEM p2, PELEM pm) {

		PELEM np = buffer;
		auto n1(p1->GetType());
		auto n2(n1 + 1);

		onselect(p1->Links[VRTX]);
		onselect(p2->Links[VRTX]);

		for (auto& p : p1->Links[n2]) {
			bool sw = true;
			for (auto& pp : p->Links[VRTX])
				if (!pp->trave) {
					sw = false;
					break;
				}
			if (sw) {
				unselect(p1->Links[VRTX]);
				unselect(p2->Links[VRTX]);
				return false;
			}
		}

		for (auto& p : p1->Links[n1]) {
			bool sw = true;
			for (auto& pp : p->Links[VRTX])
				if (!pp->trave) {
					sw = false;
					break;
				}
			if (sw)
				np->Links[n1].push_back(p);
		}

		np->Links[n1].push_back(p1);
		if (np->Links[n1].size() != n2) {
			np->Links[n1].clear();
			unselect(p1->Links[VRTX]);
			unselect(p2->Links[VRTX]);
			return false;
		}

		np->Links[VRTX] = p1->Links[VRTX];
		unselect(p1->Links[VRTX]);
		for (auto& p : p2->Links[VRTX])
			if (p->trave)
				np->Links[VRTX].push_back(p);

		unselect(p2->Links[VRTX]);

		pm->Links[n2].push_back(np);
		np->Links[MESH].push_back(pm);

		for (PELEM& p : np->Links[VRTX])
			p->Links[n2].push_back(np);
		
		//—¬ﬂ«€¬¿≈Ã ÕŒ¬€… ›À≈Ã≈Õ“ — —”Ÿ≈—“¬”ﬁŸ»Ã»
		for (PELEM& p : np->Links[n1]) {
			for (PELEM& pp : p->Links[n2]) 
				if (!pp->trave) {
					pp->Links[n2].push_back(np);
					np->Links[n2].push_back(pp);
					pp->trave = true;
				}
			p->Links[n2].push_back(np);

			//if (n2 == CELL) {
			//	if (p->Links[CELL].size() > 2) {
			//		cout << "ERR0 " << p->Links[CELL].size() << endl;
			//		cout << p1->Links[CELL].size() << endl;
			//		cout << p2->Links[CELL].size() << endl;
			//		cout << pm->Links[VRTX].size() << endl;
			//		system("PAUSE");
			//	}
			//}

		}

		for (PELEM& p : np->Links[n1])
			unselect(p->Links[n2]);

		++buffer;

		if (n2 != MESH-1)
			for (PELEM& p : np->Links[n2])
					Link(np, p, pm);

		return true;
	}
	//======================================================//

	XMVECTOR GetPosition() {
		if (this->GetType() == VRTX) 
			return *(XMVECTOR*)this->Data;

		XMVECTOR x = XMVectorZero();

		for (PELEM p : this->Links[VRTX]) 
			x += p->GetPosition();

		x /= float(this->Links[VRTX].size());

		return x;
	}

	//======================================================//

	void SetPosition(XMVECTOR x) {
		XMVECTOR* px = (XMVECTOR*)this->Data;
		*px = x;
		return;
	}

	//======================================================//
	XMVECTOR GetVolume() {
		unsigned int N = this->GetType();

		if (N < LINE) return XMVectorZero();

		VPELEM v	 = Links[VRTX];
		XMVECTOR x0  = v[0]->GetPosition();
		XMVECTOR x1  = v[1]->GetPosition();
		XMVECTOR x10 = x1 - x0;

		if (N == LINE) return XMVector3Length(x10);

		XMVECTOR x2 = v[2]->GetPosition();
		XMVECTOR x20 = x2 - x0;
		XMVECTOR xs = XMVector3Cross(x20, x10);

		if (N == SIDE) return XMVector3Length(xs) / 2.0;

		XMVECTOR x3  = v[3]->GetPosition();
		XMVECTOR x30 = x3 - x0;

		if (N == CELL) return XMVector3Dot(xs, x30) / 6.0;

		return XMVectorZero();
	}
	//======================================================//
	XMVECTOR GetDirection() {

		XMVECTOR x0 = Links[VRTX][0]->GetPosition();
		XMVECTOR x1 = Links[VRTX][1]->GetPosition();
		XMVECTOR x2 = Links[VRTX][2]->GetPosition();
		XMVECTOR xn = XMVector3Cross(x1 - x0, x2 - x0);

		if (Links[CELL].size() != 0) {

			onselect(Links[VRTX]);
			for (auto& p : Links[CELL][0]->Links[VRTX])
				if (!p->trave) {
					x0 -= p->GetPosition();
					break;
				}
			unselect(Links[VRTX]);

			if (XMVectorGetX(XMVector3Dot(x0, xn)) < 0.0) {
				xn = -xn;
				swap(Links[VRTX][1], Links[VRTX][2]);
			}
		}

		return XMVector3Normalize(xn);

	}
	//======================================================//
	static PELEM GetTetrahedron() {
		PELEM pm  = Create();
		PELEM vv1 = Create(pm);
		PELEM vv2 = Create(pm);
		PELEM vv3 = Create(pm);
		PELEM vv4 = Create(pm);
		PELEM nn1 = Create(vv1, pm);
		PELEM nn2 = Create(vv2, pm);
		PELEM nn3 = Create(vv3, pm);
		PELEM nn4 = Create(vv4, pm);
		Bind(nn1, nn2, pm);
		Bind(nn1, nn3, pm);
		Bind(nn1, nn4, pm);
		Bind(nn2, nn3, pm);
		Bind(nn2, nn4, pm);
		Bind(nn3, nn4, pm);

		vv1->SetPosition({1,0,0,0});
		vv2->SetPosition({0.5,float(sqrt(3.0)/2.0),0,0});
		vv3->SetPosition(XMVectorZero());
		vv4->SetPosition({0.5, float(sqrt(3.0)/2.0)/3, float(sqrt(2.0/3.0)),0});

		return pm;
	}
	//======================================================//
	static bool IsCrossSideLine(PELEM& S, PELEM& p1, PELEM& p2) {

		XMVECTOR LR1 = p1->GetPosition();
		XMVECTOR LR2 = p2->GetPosition();
		XMVECTOR SR1 = S->Links[VRTX][0]->GetPosition();
		XMVECTOR SR2 = S->Links[VRTX][1]->GetPosition();
		XMVECTOR SR3 = S->Links[VRTX][2]->GetPosition();
		LR1 -= SR1;
		LR2 -= SR1;
		SR2 -= SR1;
		SR3 -= SR1;
		SR1 = XMVectorZero();
		XMVECTOR N1 = XMVector3Cross(SR2, SR3);
		XMVECTOR LR21 = LR1 - LR2;
		XMVECTOR R = XMVector3Dot(N1, LR1);
		R /= XMVector3Dot(N1, LR21);
		if (XMVectorGetX(R) > 1.0) return false;
		if (XMVectorGetX(R) < 0.0) return false;
		R = R * LR21 + LR1;
		SR1 -= R;
		SR2 -= R;
		SR3 -= R;
		XMVECTOR N2 = XMVector3Cross(SR1, SR2);
		XMVECTOR N3 = XMVector3Cross(SR1, SR3);
		XMVECTOR N4 = XMVector3Cross(SR2, SR3);
		N1 = XMVector3Length(N1);
		N2 = XMVector3Length(N2);
		N3 = XMVector3Length(N3);
		N4 = XMVector3Length(N4);
		N1 -= N2 + N3 + N4;
		return (XMVectorGetX(N1) == 0);	
	}
	//======================================================//
	static bool IsNotBoundary(PELEM s) {
		return (s->Links[CELL].size() == 2);
	}
	//======================================================//
	bool CellCreating1(PELEM s, float(*MeshDist)(XMVECTOR x)) {
		XMVECTOR x1 = s->GetPosition();
		XMVECTOR x2 = s->GetDirection();
		XMVECTOR x3 = x1 + x2 * abs(MeshDist(x1)) * sqrt(2.0 / 3.0);
		XMVECTOR x4 = XMVector3Length(x3);
		float    f1 = XMVectorGetX(x4);
		bool	 b1 = (f1 > 1.0);
		bool	 b2 = IsNotBoundary(s);
		bool	 b3 = b1 || b2;

		if (b3) return false;
		PELEM m = this;
		PELEM nv = Create(m);
		PELEM np = Create(nv, m);
		nv->SetPosition(x3);

		for (auto& p : s->Links[VRTX])
			Bind(np, p->Links[NODE][0], this);

		return true;
	}
	//======================================================//
	bool CellCreating1(PELEM s1, PELEM s2, float(*MeshDist)(XMVECTOR x)) {
		if (IsNotBoundary(s1)) return false;
		if (IsNotBoundary(s2)) return false;

		XMVECTOR x1 = s1->GetPosition();
		XMVECTOR x2 = s1->GetDirection();
		XMVECTOR x3 = x1 + x2 * abs(MeshDist(x1)) * sqrt(2.0 / 3.0);
		XMVECTOR x4 = s2->GetPosition();
		XMVECTOR x5 = s2->GetDirection();
		XMVECTOR x6 = x4 + x5 * abs(MeshDist(x4)) * sqrt(2.0 / 3.0);
		XMVECTOR x7 = (x3 + x6) / 2.0;
		XMVECTOR x8 = XMVector3Length(x7);
		float    f1 = XMVectorGetX(x8);

		if (f1 > 1.0) return false;
		PELEM m = this;
		PELEM nv = Create(m);
		PELEM np = Create(nv, m);
		nv->SetPosition(x7);

		onselect(s1->Links[VRTX]);
			for (auto& p : s1->Links[VRTX])
				Bind(np, p->Links[NODE][0], this);
			for (auto& p : s2->Links[VRTX])
				if (!p->trave)
					Bind(np, p->Links[NODE][0], this);
		unselect(s1->Links[VRTX]);

		return true;

	}
	//======================================================//
	static PELEM Meshing(float(*MeshDist)(XMVECTOR x)) {

		PELEM pm = GetTetrahedron();
		PELEM vx1(nullptr);
		PELEM vx2(nullptr);

		float w = abs(MeshDist(XMVectorZero()));			//Œœ–≈ƒ≈Àﬂ≈Ã –¿«Ã≈– —≈“ »

		for (PELEM p : pm->Links[VRTX])						//Ã¿—ÿ“¿¡»–”≈Ã  ŒŒ–ƒ»Õ¿“€ “≈“–¿›ƒ–¿
			p->SetPosition(p->GetPosition() * w);

		auto& Vside(pm->Links[SIDE]);

		for (size_t i(0); i != Vside.size(); ++i) {
			auto j(Vside.size());

			//—Œ«ƒ¿≈Ã “≈“–¿»ƒ≈– œŒ ›À≈Ã≈Õ“” √–¿Õ»÷€		
			if (!pm->CellCreating1(Vside[i], MeshDist))
				continue;

			//÷» À œŒ ¬ÕŒ¬‹ ƒŒ¡¿¬À≈ÕÕ€Ã ›À≈Ã≈Õ“¿Ã √–¿Õ»÷€
			for (size_t k(j); k != Vside.size(); ++k) {
				PELEM s1(Vside[k]);
		
				if (IsNotBoundary(s1)) 
					continue;
				
				//÷» À œŒ —Œ—≈ƒﬂÃ –¿——Ã¿“–»¬¿≈ÃŒ… √–¿Õ»÷€
				VPELEM& vs1(s1->Links[VRTX]);

				for (PELEM s2 : s1->Links[SIDE]) {

					if (IsNotBoundary(s2)) 
						continue;

					VPELEM& vs2(s2->Links[VRTX]);
					auto  x3(s1->GetDirection());
					auto  x4(s2->GetDirection());
					
					onselect(vs1);
					for (auto& p : vs2)
						((p->trave) ? vx2 : vx1) = p;

					auto  x5(XMVector3Normalize(vx1->GetPosition() - vx2->GetPosition()));
					auto  x6(XMVector3Dot(x3, x5));
					auto  x7(XMVector3Dot(x3, x4));
					float d1(XMVectorGetX(x6));
					float d2(XMVectorGetX(x7));

	
					if ((d1 >= 0.0f) && (d2 < 0.21)) {

						unselect(vs2);

						for (auto& p : vs1)
							if (p->trave) {
								vx2 = p;
								break;
							}

						unselect(vs1);

						onselect(s1->Links[SIDE]);
						auto SideNeib(s1->Links[SIDE]);
						for (auto& p : s2->Links[SIDE])
							if (!p->trave)
								SideNeib.push_back(p);
						unselect(s1->Links[SIDE]);
						
						vx1->trave = true;
						vx2->trave = true;

						for (auto& s3 : SideNeib) {
							
							bool sw = false;
							for (auto& p : s3->Links[VRTX])
								if (p->trave) {
									sw = true;
									break;
								}
							if (sw) continue;
							
							if (IsCrossSideLine(s3, vx2, vx1))
								return pm;
						}

						vx1->trave = false;
						vx2->trave = false;

						size_t i1 = s1->Links[CELL].size();
						size_t i2 = s2->Links[CELL].size();


						if ((i1 > 2) || (i2 > 2)) {
							cout << "ERR1" << endl;
							pm->Links[SIDE] = { s1,s2 };
							return pm;
						}

						Bind(vx2->Links[NODE][0],
							vx1->Links[NODE][0], pm);

						i1 = s1->Links[CELL].size();
						i2 = s2->Links[CELL].size();

						if ((i1 > 2) || (i2 > 2)) {
							cout << "ERR2" << endl;
							pm->Links[SIDE] = { s1,s2 };
							return pm;
						}

						//¬€–¿¬Õ»¬¿≈Ã —≈“ ”
						for (PELEM& s3 : SideNeib)
							for (PELEM& p : s3->Links[VRTX])
								Aligment(MeshDist, p->Links[NODE][0]);

						break;
		
					} else 
						unselect(vs1);
				}
			}
		}

		
		//size_t i = 0;
		//
		//while (i < pm->Links[VRTX].size())
		//{
		//	PELEM p = pm->Links[VRTX][i];
		//	if (MeshDist(p->GetPosition()) < 0)
		//		Erase(p, VRTX);
		//	else i++;
		//}

		return pm;
	}
	//======================================================//
	static void Aligment(float(*MeshDist)(XMVECTOR x), PELEM p)
	{		
		XMVECTOR x0 = p->GetPosition();
		XMVECTOR x1 = x0;
		XMVECTOR xr = XMVectorZero();
		float	 sz = abs(MeshDist(x0));
		float	 ln = sz;

		for (PELEM& np : p->Links[NODE]) {
			x1 = np->GetPosition();
			x1 -= x0;
			ln = XMVectorGetX(XMVector3Length(x1));
			x1 = XMVector3Normalize(x1);
			x1 *= (ln - sz);
			xr += x1;
		}

		xr = x0 + xr / float(p->Links[NODE].size());
		p->Links[VRTX][0]->SetPosition(xr);
	}
	//======================================================//
	static void* GetIndexBuffer(PELEM pm) {
		XMVECTOR* Iterator = (XMVECTOR*)pm->Data;
		Iterator += pm->Links[VRTX].size() * 2;
		return (void*)Iterator;
	}
	//======================================================//
	static PELEM GetMeshSurface(PELEM pm1) {
		PELEM pm2 = Create();
		pm2->Links[VRTX] = pm1->Links[VRTX];

		for (PELEM s : pm1->Links[SIDE])
			if (s->Links[CELL].size() != 2) {
				for (auto& p : s->Links[LINE])
					if (!p->trave) {
						pm2->Links[LINE].push_back(p);
						p->trave = true;
					}
				pm2->Links[SIDE].push_back(s);
			}

		unselect(pm2->Links[LINE]);

		return pm2;
	}
	//======================================================//
	int GetVertexID(PELEM v) {
		return ((XMVECTOR*)v->Data - (XMVECTOR*)this->Data) / 2;
	}
	//======================================================//
	static void SetDataBuffer(PELEM pm) {
		size_t s1 = pm->Links[VRTX].size();
		size_t s2 = pm->Links[LINE].size();
		size_t s3 = pm->Links[SIDE].size();

		s1 *= sizeof(XMVECTOR) * 2;
		s2 *= sizeof(int) * LINE;
		s3 *= sizeof(int) * SIDE;
		
		size_t s4 = s1 + s2 + s3 + 1;

		pm->Data = malloc(s4);
		XMVECTOR* Iterator = (XMVECTOR*)pm->Data;

		for (PELEM p : pm->Links[VRTX]) {
			*Iterator = p->GetPosition();
			delete p->Data;
			p->Data = Iterator++;
			XMVECTOR N = XMVectorZero();
			for (PELEM s : p->Links[SIDE])
				N += s->GetDirection();
			*Iterator++ = XMVector3Normalize(N);
		}

		for (PELEM p : pm->Links[SIDE])
			p->GetDirection();

		int* Iterator2 = (int*)Iterator;
		
		for (unsigned char i = LINE; i <= SIDE; i++)
			for (PELEM p : pm->Links[i])
				for (PELEM v : p->Links[VRTX])
					*Iterator2++ = ((XMVECTOR*)v->Data - (XMVECTOR*)pm->Data)/2;

		//WriteMeshVTK(pm, "", 9000);
	}
	//======================================================//
	//static void WriteMeshVTK(PELEM PM, char* Name, int IT)
	//{
	//	ofstream Out = ofstream(ofstream(std::to_string(IT) + "MESH.vtk"));
	//
	//	Out << "# vtk DataFile Version 2.0" << "\n";
	//	Out << "Unstructured Grid Example" << "\n";
	//	Out << "ASCII" << "\n";
	//	Out << "DATASET UNSTRUCTURED_GRID" << "\n";
	//	Out << "POINTS " << PM->Links[VRTX].size() << " " << "float" << "\n";
	//
	//	for (PELEM N : PM->Links[VRTX])
	//	{
	//		XMVECTOR X = N->GetPosition();
	//		Out << XMVectorGetX(X) << " " << XMVectorGetY(X) << " " << XMVectorGetZ(X) << "\n";
	//	}
	//
	//	size_t Size1 = PM->Links[CELL].size();
	//	size_t Size2 = Size1 * 5;
	//
	//	Out << "CELLS " << Size1 << " " << Size2 << "\n";
	//
	//	for (PELEM C : PM->Links[CELL])
	//	{
	//		Out << 4 << " ";
	//
	//		for (PELEM P : C->Links[VRTX])
	//			Out << PM->GetVertexID(P) << " ";
	//
	//		Out << "\n";
	//	}
	//
	//	Out << "CELL_TYPES " << Size1 << "\n";
	//
	//	for (PELEM C : PM->Links[CELL])
	//		Out << "10" << "\n";
	//}
	
	
};
