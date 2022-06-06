#pragma once

#define DIRECTINPUT_VERSION 0x0800 

#include "FEM_GRAPH.h"
#include <vector>
#include <windows.h>
#include <d3d11.h>
#include "DirectXMath.h"
#include "dinput.h"

using namespace std;
using namespace DirectX;

//=========================================================================================================================================================//
struct DATA
{
	UINT32	 NumMesh;				//����� �����
	XMVECTOR Pos;				    //������� �����
	XMVECTOR Orient;				//���������� �����
	UINT32*		SwPick;				//��������� ��������
	float*		Blend;				//������������ ��������
	XMMATRIX	mWorld;				//������������ � ���������� �������
};
//=========================================================================================================================================================//
struct ConstantBuffer				//��������� ��� ������������ ������
{
	XMMATRIX	mWorld;				//������� ����
	XMMATRIX	mView;				//������� ����
	XMMATRIX	mProjection;		//������� ��������
	XMVECTOR	Dir;
	int			SwPick;				//��������� �������
	float		Blend;				//������������ �������
	float		mPosMouse[2];		//������� ����
	int			NumCur;				//����� �������, �� �������� ���������� mWorld, SwPick, Blend
};
//========================================================================================================================================================//
class CSCENE
{

private:

	D3D_DRIVER_TYPE						g_driverType;
	D3D_FEATURE_LEVEL					g_featureLevel;
	ID3D11Device				*		g_pd3dDevice;
	ID3D11DeviceContext			*		g_pImmediateContext;
	IDXGISwapChain				*		g_pSwapChain;
	ID3D11RenderTargetView		*		g_pRenderTargetView;
	ID3D11ComputeShader			*		g_pComputeShader;
	ID3D11VertexShader			*		g_pVertexShader;								//���������� ��� ���������� �������
	ID3D11PixelShader			*		g_pPixelShader;									//���������� ��� ����������� �������
	ID3D11InputLayout			*		g_pVertexLayout;								//���������� ��� ��������� ������ ������
	ID3D11Buffer				*		g_pVertexBuffer;								//����� ������
	ID3D11Buffer				*		g_pIndexBuffer;									//������ �������� ������
	ID3D11Buffer				*		g_pConstantsBuffer;								//����������� �����
	ID3D11Buffer				*		g_pCSInputBuffer;
	ID3D11Buffer				*		g_pCSOutputBuffer;
	ID3D11Buffer				*		g_pCSOutputBufferResult;
	ID3D11Texture2D				*		g_pDepthStencil;
	ID3D11DepthStencilView		*		g_pDepthStencilView;							//���������� ��� ���������� ������ ������
	ID3D11ShaderResourceView	*		g_pTextureRV;									//���������� ��� ��������, ������������ � ������� 
	ID3D11ShaderResourceView	*		g_pTextureRV2;
	vector<ID3D11ShaderResourceView*>   vg_pTextureRV;                 
	ID3D11UnorderedAccessView	*		g_pCSUOV;
	ID3D11SamplerState			*		g_pSamplerLinear;								//���������� ��� ������������ ��������
	ID3D11BlendState			*		g_pBlendState;									//���������� ��� ������������

	int NumT = 0;

	LPDIRECTINPUTDEVICE8				keyboardDevice;									//����������
	LPDIRECTINPUTDEVICE8				mouseDevice;									//����

	HRESULT								SetKeyboard();									//������������� ����������
	HRESULT								SetMouse();										//������������� ����
	void								MouseUpdate();									//���������� ��������� ����
	void								KeyBoardUpdate();								//���������� ��������� ����������
	HRESULT								TargetViewInit();								//����������� ���������� � ������������ ������ �����������
	HRESULT								ShaderInit();									//������������� ����
	HRESULT								ComputeShaderInit();
	HRESULT								VertexBufferInit();								//������������� ������ ������
	HRESULT								ConstantBufferInit();							//������������� ������������ ������
	HRESULT								TextureInit();									//������������� ��������
	void								UpdateConstantBuffer(int i);					//���������� ������, ������������ � ����������� �����, ��������� � ��������
	HRESULT								SetViewPort();									//������ �������
	HRESULT								BlendInit();									//������������� ������� ������������
	HRESULT								DepthInit();									//������������� ������ �������
	HRESULT								IndexBufferInit();								//������������� ������ ��������
	void								UpdateConstantBuffer();							//���������� ����� ������ ������������ ������, ����������� � ��������
	void								UpdateMatrixWorld();							//���������� ������ ���� ��������

	XMMATRIX							g_World;										//������� ����
	XMMATRIX							g_View;											//������� ����
	XMMATRIX							g_Projection;									//������� ��������
	ConstantBuffer						cb;												//��������� ��� ������������ ������
	POINT								pt;												//��� ����������� ������� ����
	

public:

	float								time;
	PELEM								Mesh;
	vector<DATA>						Data;											//��������� ������������� ������
	HINSTANCE							g_hInst;
	HWND								g_hWnd;

	float								ClearColor[4];									//���� ������� ����
	XMVECTOR							Eye;											//������������ ������ ������� ������
	XMVECTOR							At;												//������������ ���� ������� ������
	XMVECTOR							Up;												//����������� ����� (0,0,1)
	UINT								width;											//������ ���-�����/����
	UINT								height;											//������ ���-�����/����
	
	void								ScriptInit();									//������������� �������
	void								Script();										//������ �����
	void								Render();										//��������� �����
    HRESULT								Initialization();								//������������� �����
	int Tex1 = 0;
	int Tex2 = 0;

	CSCENE();
	~CSCENE();

};
//==========================================================================================================================================================//