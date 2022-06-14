#pragma once
#include "FEM_GRAPH.h"

static XMVECTOR* vertexbuf;
static XMVECTOR* vertexbufbegin;

class CMESH  {
public:

	ELEM* Links[CELL + 1];
	ELEM* buffer[CELL + 1];

	CMESH() {
		Links[VRTX] = new ELEM[20000];
		Links[LINE] = new ELEM[125000];
		Links[SIDE] = new ELEM[205000];
		Links[CELL] = new ELEM[105000];
		buffer[VRTX] = Links[VRTX];
		buffer[LINE] = Links[LINE];
		buffer[SIDE] = Links[SIDE];
		buffer[CELL] = Links[CELL];
	}

	//======================================================//
	PELEM Create() {
		buffer[VRTX]->type = VRTX;
		buffer[VRTX]->Data = vertexbuf;
		vertexbuf += 2;
		return buffer[VRTX]++;
	}
	//======================================================//
	void Bind(PELEM p1, PELEM p2) {
		PELEM np = buffer[LINE]++;
		np->type = LINE;
		p1->Links[VRTX].push_back(p2);
		p2->Links[VRTX].push_back(p1);
		np->Links[VRTX].push_back(p1);
		np->Links[VRTX].push_back(p2);
		np->Links[LINE] = p1->Links[LINE];
		ELEM::onselect(p1->Links[LINE]);
		for (auto& p : p2->Links[LINE])
			if (!p->trave)
				np->Links[LINE].push_back(p);
		ELEM::unselect(p1->Links[LINE]);
		p1->Links[LINE].push_back(np);
		p2->Links[LINE].push_back(np);
		for (auto& p : np->Links[LINE])
			p->Links[LINE].push_back(np);
		for (PELEM& p : np->Links[LINE])
			if (!p->trave2)
				Link(np, p);
		ELEM::unselect2(np->Links[LINE]);
	}
	//======================================================//
	PELEM Link(PELEM p1, PELEM p2) {

		auto n1(p1->type);
		auto n2(n1 + 1);
		PELEM np = buffer[n2];

		ELEM::onselect(p1->Links[VRTX]);
		ELEM::onselect(p2->Links[VRTX]);

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
			ELEM::unselect(p1->Links[VRTX]);
			ELEM::unselect(p2->Links[VRTX]);
			return nullptr;
		}

		np->Links[VRTX] = p1->Links[VRTX];
		ELEM::unselect(p1->Links[VRTX]);
		for (auto& p : p2->Links[VRTX])
			if (p->trave)
				np->Links[VRTX].push_back(p);
		ELEM::unselect(p2->Links[VRTX]);

		//ÑÂßÇÛÂÀÅÌ ÍÎÂÛÉ ÝËÅÌÅÍÒ Ñ ÑÓÙÅÑÒÂÓÞÙÈÌÈ
		for (PELEM& p : np->Links[n1]) {
			for (PELEM& pp : p->Links[n2])
				if (!pp->trave) {
					pp->Links[n2].push_back(np);
					np->Links[n2].push_back(pp);
					pp->trave = true;
				}
			p->Links[n2].push_back(np);
		}


		for (PELEM& p : np->Links[n1])
			ELEM::unselect(p->Links[n2]);

		buffer[n2]->type = n2;
		++buffer[n2];

		ELEM::onselect2(np->Links[n1]);

		if (n2 != CELL) {
			for (PELEM& p : np->Links[n2])
				if (!p->trave2)
					Link(np, p);
			ELEM::unselect2(np->Links[n2]);
		}

		return np;
	}
	//======================================================//
	void GetTetrahedron() {
		PELEM vv1 = Create();
		PELEM vv2 = Create();
		PELEM vv3 = Create();
		PELEM vv4 = Create();
		Bind(vv1, vv2);
		Bind(vv1, vv3);
		Bind(vv1, vv4);
		Bind(vv2, vv3);
		Bind(vv2, vv4);
		Bind(vv3, vv4);

		vv1->SetPosition({1,0,0,0});
		vv2->SetPosition({0.5,float(sqrt(3.0)/2.0),0,0});
		vv3->SetPosition(XMVectorZero());
		vv4->SetPosition({0.5, float(sqrt(3.0)/2.0)/3, float(sqrt(2.0/3.0)),0});
	}
	//=============================================================================//
	bool CellCreating(PELEM s, GEOM MeshDist) {
		XMVECTOR x1 = s->GetPosition();
		XMVECTOR x2 = s->GetDirection();
		x2 *= abs(MeshDist(x1)) * sqrt(2.0 / 3.0);
		x1 += x2;
		x2 = XMVector3LengthSq(x1);
		if (XMVectorGetX(x2) > 1.0)
			return false;
		PELEM nv = Create();
		nv->SetPosition(x1);
		for (auto& p : s->Links[VRTX])
			Bind(nv, p);
		return true;
	}
	//=============================================================================//
	void Meshing(GEOM MeshDist) {

		GetTetrahedron();
		PELEM vx1(nullptr);
		PELEM vx2(nullptr);

		float w = abs(MeshDist(XMVectorZero()));			//ÎÏÐÅÄÅËßÅÌ ÐÀÇÌÅÐ ÑÅÒÊÈ


		for (PELEM p : Links[CELL][0].Links[VRTX])							//ÌÀÑØÒÀÁÈÐÓÅÌ ÊÎÎÐÄÈÍÀÒÛ ÒÅÒÐÀÝÄÐÀ
			p->SetPosition(p->GetPosition() * w);

		for (auto s(Links[SIDE]); s != buffer[SIDE]; ++s) {

			if (s->IsNotBoundary())
				continue;
			
			auto ss(buffer[SIDE]);
			
			if (!CellCreating(s, MeshDist))
				continue;

			for (auto s1(ss); s1 != buffer[SIDE]; ++s1) {
				if (s1->IsNotBoundary())
					continue;

				VPELEM& vs1(s1->Links[VRTX]);

				for (PELEM s2 : s1->Links[SIDE]) {

					if (s2->IsNotBoundary())
						continue;

					VPELEM& vs2(s2->Links[VRTX]);
					auto  x3(s1->GetDirection());
					auto  x4(s2->GetDirection());

					ELEM::onselect(vs1);
					for (auto& p : vs2)
						((p->trave) ? vx2 : vx1) = p;

					auto  x5(XMVector3Normalize(vx1->GetPosition() - vx2->GetPosition()));
					auto  x6(XMVector3Dot(x3, x5));
					auto  x7(XMVector3Dot(x3, x4));

					if ((XMVectorGetX(x6) >= 0) && (XMVectorGetX(x7) < 0.21)) {

						ELEM::unselect(vs2);

						for (auto& p : vs1)
							if (p->trave) {
								vx2 = p;
								break;
							}

						ELEM::unselect(vs1);

						ELEM::onselect(s1->Links[SIDE]);
						auto SideNeib(s1->Links[SIDE]);
						for (auto& p : s2->Links[SIDE])
							if (!p->trave)
								SideNeib.push_back(p);
						ELEM::unselect(s1->Links[SIDE]);

						Bind(vx2, vx1);

						//ÂÛÐÀÂÍÈÂÀÅÌ ÑÅÒÊÓ
						for (PELEM& s3 : s1->Links[SIDE])
							for (PELEM& p : s3->Links[VRTX])
								p->Aligment(MeshDist);

						break;

					}
					else
						ELEM::unselect(vs1);
				}

			}
		}
		return;
	}
	//======================================================//
	static int GetVertexID(PELEM v) {
		return ((XMVECTOR*)v->Data - vertexbufbegin) / 2;
	}
	//======================================================//
	static void WriteMeshVTK(CMESH& PM, char* Name, int IT) {
		ofstream Out = ofstream(ofstream(std::to_string(IT) + Name));

		Out << "# vtk DataFile Version 2.0" << "\n";
		Out << "Unstructured Grid Example" << "\n";
		Out << "ASCII" << "\n";
		Out << "DATASET UNSTRUCTURED_GRID" << "\n";
		//Out << "POINTS " << PM.Links[VRTX].size() << " " << "float" << "\n";
		Out << "POINTS " << PM.buffer[VRTX] - PM.Links[VRTX]  << " " << "float" << "\n";

		//for (PELEM N : PM.Links[VRTX]) {
		for (auto N(PM.Links[VRTX]); N != PM.buffer[VRTX]; ++N) {
			XMVECTOR X = N->GetPosition();
			Out << XMVectorGetX(X) << " " << XMVectorGetY(X) << " " << XMVectorGetZ(X) << "\n";
		}

		//size_t Size1 = PM.Links[CELL].size();
		size_t Size1 = PM.buffer[CELL] - PM.Links[CELL];
		size_t Size2 = Size1 * 5;

		Out << "CELLS " << Size1 << " " << Size2 << "\n";

		//for (PELEM C : PM.Links[CELL]) {
		for (auto C(PM.Links[CELL]); C != PM.buffer[CELL]; ++C) {
		Out << 4 << " ";

			for (PELEM P : C->Links[VRTX])
				Out << GetVertexID(P) << " ";

			Out << "\n";
		}

		Out << "CELL_TYPES " << Size1 << "\n";

		//for (PELEM C : PM.Links[CELL])
		for (auto C(PM.Links[CELL]); C != PM.buffer[CELL]; ++C) 
			Out << "10" << "\n";
	}
};