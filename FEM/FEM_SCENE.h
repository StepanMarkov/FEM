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
	UINT32	 NumMesh;				//номер сетки
	XMVECTOR Pos;				    //позиция сетки
	XMVECTOR Orient;				//ориентация сетки
	UINT32*		SwPick;				//выделение объектов
	float*		Blend;				//прозрачность объектов
	XMMATRIX	mWorld;				//расположение и ориентация объекта
};
//=========================================================================================================================================================//
struct ConstantBuffer				//структура для константного буфера
{
	XMMATRIX	mWorld;				//матрица мира
	XMMATRIX	mView;				//матрица вида
	XMMATRIX	mProjection;		//матрица проекции
	XMVECTOR	Dir;
	int			SwPick;				//выделение объекта
	float		Blend;				//прозрачность объекта
	float		mPosMouse[2];		//позиция мыши
	int			NumCur;				//номер объекта, от которого передается mWorld, SwPick, Blend
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
	ID3D11VertexShader			*		g_pVertexShader;								//переменная для вершинного шейдера
	ID3D11PixelShader			*		g_pPixelShader;									//переменная для пиксельного шейдера
	ID3D11InputLayout			*		g_pVertexLayout;								//переменная для структуры данных вершин
	ID3D11Buffer				*		g_pVertexBuffer;								//буфер вершин
	ID3D11Buffer				*		g_pIndexBuffer;									//буффер индексов вершин
	ID3D11Buffer				*		g_pConstantsBuffer;								//константный буфер
	ID3D11Buffer				*		g_pCSInputBuffer;
	ID3D11Buffer				*		g_pCSOutputBuffer;
	ID3D11Buffer				*		g_pCSOutputBufferResult;
	ID3D11Texture2D				*		g_pDepthStencil;
	ID3D11DepthStencilView		*		g_pDepthStencilView;							//переменная для реализации буфера глубин
	ID3D11ShaderResourceView	*		g_pTextureRV;									//переменная для текстуры, передаваемой в шейдеры 
	ID3D11ShaderResourceView	*		g_pTextureRV2;
	vector<ID3D11ShaderResourceView*>   vg_pTextureRV;                 
	ID3D11UnorderedAccessView	*		g_pCSUOV;
	ID3D11SamplerState			*		g_pSamplerLinear;								//переменная для интерполяции текстуры
	ID3D11BlendState			*		g_pBlendState;									//переменная для прозрачности

	int NumT = 0;

	LPDIRECTINPUTDEVICE8				keyboardDevice;									//клавиатура
	LPDIRECTINPUTDEVICE8				mouseDevice;									//мышь

	HRESULT								SetKeyboard();									//инициализация клавиатуры
	HRESULT								SetMouse();										//инициализация мыши
	void								MouseUpdate();									//обновление состояния мыши
	void								KeyBoardUpdate();								//обновление состояния клавиатуры
	HRESULT								TargetViewInit();								//подключение видеокарты и иициализация вывода изображения
	HRESULT								ShaderInit();									//инициализация вида
	HRESULT								ComputeShaderInit();
	HRESULT								VertexBufferInit();								//инициализация буфера вершин
	HRESULT								ConstantBufferInit();							//инициализация константного буфера
	HRESULT								TextureInit();									//инициализация текстуры
	void								UpdateConstantBuffer(int i);					//обновление данных, передаваемых в константный буфер, связанных с объектом
	HRESULT								SetViewPort();									//задаем вьюпорт
	HRESULT								BlendInit();									//инициализация эффекта прозрачности
	HRESULT								DepthInit();									//инициализация буфера глубины
	HRESULT								IndexBufferInit();								//инициализация буфера индексов
	void								UpdateConstantBuffer();							//обновление части данных константного буфера, несвязанных с объектом
	void								UpdateMatrixWorld();							//обновление матриц мира объектов

	XMMATRIX							g_World;										//матрицы мира
	XMMATRIX							g_View;											//матрица вида
	XMMATRIX							g_Projection;									//матрица проекции
	ConstantBuffer						cb;												//структура для константного буфера
	POINT								pt;												//для определения позиции мыши
	

public:

	float								time;
	PELEM								Mesh;
	vector<DATA>						Data;											//интерфейс использования буфера
	HINSTANCE							g_hInst;
	HWND								g_hWnd;

	float								ClearColor[4];									//цвет заднего фона
	XMVECTOR							Eye;											//расположение откуда смотрит камера
	XMVECTOR							At;												//расположение куда смотрит камера
	XMVECTOR							Up;												//направление верха (0,0,1)
	UINT								width;											//ширина вью-порта/окна
	UINT								height;											//высота вью-порта/окна
	
	void								ScriptInit();									//инициализация скрипта
	void								Script();										//скрипт сцены
	void								Render();										//отрисовка сцены
    HRESULT								Initialization();								//инициализация сцены
	int Tex1 = 0;
	int Tex2 = 0;

	CSCENE();
	~CSCENE();

};
//==========================================================================================================================================================//