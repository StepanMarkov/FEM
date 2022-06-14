#pragma once
#include <vector>
#include "DirectXMath.h"
#include <string>
#include <fstream>
#include <iostream>

using namespace std;
using namespace DirectX;
typedef float(*GEOM)(const XMVECTOR& x);

//==========================================================//

class ELEM;
typedef ELEM* PELEM;
typedef vector<PELEM> VPELEM;

//==========================================================//
enum TYPES { 
	VRTX = 1,
	LINE,
	SIDE,
	CELL
};
//==========================================================//

class ELEM {

public:

	VPELEM Links[CELL+1];
	XMVECTOR* Data;
	char type;
	bool trave = false;
	bool trave2 = false;
	bool trave3 = false;

	ELEM() {
		Links[VRTX].reserve(50);
		Links[LINE].reserve(50);
		Links[SIDE].reserve(50);
		Links[CELL].reserve(50);
	}

	inline static void onselect(VPELEM& v) {
		for (auto& p : v)
			p->trave = true;
	}

	inline static void unselect(VPELEM& v) {
		for (auto& p : v)
			p->trave = false;
	}

	inline static void onselect2(VPELEM& v) {
		for (auto& p : v)
			p->trave2 = true;
	}

	inline static void unselect2(VPELEM& v) {
		for (auto& p : v)
			p->trave2 = false;
	}

	//======================================================//
	inline XMVECTOR* GetData() const {
		return this->Data;
	}
	//======================================================//
	inline size_t GetSize(const char Type)  {
		return this->Links[Type].size();
	}
	//======================================================//

	XMVECTOR GetPosition() {
		if (type == VRTX) 
			return *this->Data;
		XMVECTOR x(XMVectorZero());
		for (auto& p : this->Links[VRTX]) 
			x += *p->Data;
		x /= static_cast<float>(type);
		return x;
	}

	//======================================================//
	void inline SetPosition(const XMVECTOR& x) {
		*Data = x;
	}
	//======================================================//
	XMVECTOR GetDirection() {

		XMVECTOR x0 = Links[VRTX][0]->GetPosition();
		XMVECTOR& x1 = *(Links[VRTX][1]->Data);
		XMVECTOR& x2 = *(Links[VRTX][2]->Data);
		XMVECTOR xn = XMVector3Cross(x1 - x0, x2 - x0);
		
		if ((Links[CELL].size() != 0) && (!trave3) ) {

			trave3 = true;
			onselect(Links[VRTX]);
			for (auto& p : Links[CELL][0]->Links[VRTX])
				if (!p->trave) {
					x0 -= *p->Data;
					break;
				}
			unselect(Links[VRTX]);

			if (XMVectorGetX(XMVector3Dot(x0, xn)) < 0.0) {
				xn = -xn;
				PELEM p = Links[VRTX][1];
				Links[VRTX][1] = Links[VRTX][2];
				Links[VRTX][2] = p;
			}
		}

		return XMVector3Normalize(xn);

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
		N1 = XMVector3LengthSq(N1);
		N2 = XMVector3LengthSq(N2);
		N3 = XMVector3LengthSq(N3);
		N4 = XMVector3LengthSq(N4);
		N1 -= N2 + N3 + N4;
		return (XMVectorGetX(N1) == 0);	
	}
	//======================================================//
	inline bool IsNotBoundary() {
		return (this->Links[CELL].size() >= LINE);
	}
	//======================================================//
	void Aligment(const GEOM& MeshDist) {
		XMVECTOR& x0 = *Data;
		XMVECTOR x1, xr = XMVectorZero();
		float	 sz = abs(MeshDist(x0));
		float	 ln = sz;

		for (auto& np : Links[VRTX]) {
			x1 = *np->Data;
			x1 -= x0;
			ln = XMVectorGetX(XMVector3Length(x1));
			x1 *= (ln - sz) / ln;
			xr += x1;
		}

		xr = x0 + xr / float(Links[VRTX].size());
		SetPosition(xr);
	}	
};
