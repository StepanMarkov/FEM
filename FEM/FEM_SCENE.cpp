#include "stdafx.h"
#include "FEM_SCENE.h"
#include <ctime>
#include <d3dcompiler.h>
#include <D3D11.h>

#define KEYDOWN( name, key ) ( name[key] & 0x80 )
#define BUTTONDOWN(name, key) ( name.rgbButtons[key] & 0x80 )

//==================================================================================//
CSCENE::CSCENE()
{
	ClearColor[0] = 0.0f;
	ClearColor[1] = 0.0f;
	ClearColor[2] = 0.0f;
	ClearColor[3] = 0.0f;

	//ifstream In("Параметры.txt");
	//string value;
	//In >> value; In >> width; In >> height;
	//In >> value; In >> Tex1; In >> Tex2;
	//In.close();

	ifstream In("render.fx");
	string value;
	In >> value; In >> value; In >> value; In >> width; In >> value;
	In >> value; In >> value; In >> value; In >> height; In >> value;
	In >> value; In >> value; In >> value; In >> Tex1; In >> value;
	In >> value; In >> value; In >> value; In >> Tex2; In >> value;
	In.close();


	g_World			= XMMatrixIdentity			();
	Eye				= XMVectorSet				(-5.0f, -5.0f, 4.0f, 0.0f);
	At				= XMVectorSet				(1.0f, 0.0f, 0.0f, 0.0f);
	Up				= XMVectorSet				(0.0f, 0.0f, 1.0f, 0.0f);
	g_View			= XMMatrixLookAtLH			(Eye, At, Up);
	g_Projection	= XMMatrixPerspectiveFovLH	(XM_PIDIV4, width / (FLOAT)height, 0.01f, 100.0f);

	g_driverType			= D3D_DRIVER_TYPE_HARDWARE;
	g_featureLevel			= D3D_FEATURE_LEVEL_11_0;
	g_hInst					= nullptr;
	g_hWnd					= nullptr;
	g_pd3dDevice			= nullptr;
	g_pImmediateContext		= nullptr;
	g_pSwapChain			= nullptr;
	g_pRenderTargetView		= nullptr;
	g_pVertexShader			= nullptr;
	g_pComputeShader		= nullptr;
	g_pPixelShader			= nullptr;
	g_pVertexLayout			= nullptr;
	g_pVertexBuffer			= nullptr;
	g_pIndexBuffer			= nullptr;
	g_pConstantsBuffer		= nullptr;
	g_pCSInputBuffer		= nullptr;
	g_pCSOutputBuffer		= nullptr;
	g_pCSOutputBufferResult = nullptr;
	g_pDepthStencil			= nullptr;
	g_pDepthStencilView		= nullptr;
	g_pTextureRV			= nullptr;
	g_pTextureRV2			= nullptr;
	g_pCSUOV				= nullptr;
	g_pSamplerLinear		= nullptr;
	g_pBlendState			= nullptr;
	Mesh = nullptr;
	keyboardDevice = nullptr;
	time = 0;

}
//===================================================================================================================//
float Geom (XMVECTOR X)
{
	return 0.08;
}
//===================================================================================================================//
void CSCENE::ScriptInit()
{
	//Mesh = ELEM::Create({}, nullptr);
	//buffer = new ELEM[300000];
	//Mesh = ELEM::Meshing(Geom);
	//Mesh = ELEM::GetMeshSurface(Mesh);
	//ELEM::SetDataBuffer(Mesh);

	DATA DT;
	DT.NumMesh	= 0;
	DT.Pos		= XMVectorZero();
	DT.Orient	= XMVectorZero();
	
	DT.SwPick = new UINT32(0);
	DT.Blend = new float(1.0f);
	this->Data.push_back(DT);
	//DT.Pos = XMVectorSetX(DT.Pos, 1.0);
	//this->Data.push_back(DT);
}
//===================================================================================================================//
void CSCENE::Script()
{
	Data[0].Orient = XMVectorSetX(Data[0].Orient, 0.001f * time);
}
//===========================================================================================================================================//
HRESULT CSCENE::ComputeShaderInit()
{
	////-------------------------------------------------------------------------------------//
	
	//float MyData[] = { 1.0, 3.0, 4.0, 5.0 ,6.0 };
	//D3D11_SUBRESOURCE_DATA	MyData2;
	//MyData2.pSysMem = MyData;
	//MyData2.SysMemPitch = 0;
	//MyData2.SysMemSlicePitch = 0;
	//
	//D3D11_BUFFER_DESC constantDataDesc;	
	//
	//constantDataDesc.Usage					=	D3D11_USAGE_DYNAMIC;
	//constantDataDesc.ByteWidth				=	sizeof(float) * 5;
	//constantDataDesc.BindFlags				=	D3D11_BIND_SHADER_RESOURCE;
	//constantDataDesc.CPUAccessFlags			=	D3D11_CPU_ACCESS_WRITE;
	//constantDataDesc.StructureByteStride	=	sizeof(float);
	//constantDataDesc.MiscFlags				=	D3D11_RESOURCE_MISC_BUFFER_STRUCTURED;
	//
	//g_pd3dDevice->CreateBuffer(&constantDataDesc, &MyData2, &g_pCSInputBuffer);
	//
	////-------------------------------------------------------------------------------------//
	//
	//D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc;
	//
	//srvDesc.Format					= DXGI_FORMAT_UNKNOWN;
	//srvDesc.ViewDimension			= D3D11_SRV_DIMENSION_BUFFEREX;
	//srvDesc.BufferEx.FirstElement	= 0;
	//srvDesc.BufferEx.Flags			= 0;
	//srvDesc.BufferEx.NumElements	= 5;
	//
	////g_pd3dDevice->CreateShaderResourceView(g_pCSInputBuffer, &srvDesc, &g_pTextureRV);
	//
	////-------------------------------------------------------------------------------------//
	//
	//D3D11_BUFFER_DESC outputDesc;
	//outputDesc.Usage				= D3D11_USAGE_DEFAULT;
	//outputDesc.ByteWidth			= sizeof(float) * 5;
	//outputDesc.BindFlags			= D3D11_BIND_UNORDERED_ACCESS;
	//outputDesc.CPUAccessFlags		= 0;
	//outputDesc.StructureByteStride	= sizeof(float);
	//outputDesc.MiscFlags			= D3D11_RESOURCE_MISC_BUFFER_STRUCTURED;
	//
	//g_pd3dDevice->CreateBuffer(&outputDesc, &MyData2, &g_pCSOutputBuffer);
	//
	//outputDesc.Usage				= D3D11_USAGE_STAGING;
	//outputDesc.BindFlags			= 0;
	//outputDesc.CPUAccessFlags		= D3D11_CPU_ACCESS_READ;
	//
	//g_pd3dDevice->CreateBuffer(&outputDesc, &MyData2, &g_pCSOutputBufferResult);
	//
	//
	////-------------------------------------------------------------------------------------//
	//
	//D3D11_UNORDERED_ACCESS_VIEW_DESC uavDesc;
	//uavDesc.Buffer.FirstElement = 0;
	//uavDesc.Buffer.Flags		= 0;
	//uavDesc.Buffer.NumElements	= 5;
	//uavDesc.Format				= DXGI_FORMAT_UNKNOWN;
	//uavDesc.ViewDimension		= D3D11_UAV_DIMENSION_BUFFER;
	//
	//g_pd3dDevice->CreateUnorderedAccessView(g_pCSOutputBuffer, &uavDesc, &g_pCSUOV);
	//
	////-------------------------------------------------------------------------------------//
	//
	//
	//
	//ID3DBlob* pCSBlob = nullptr;
	//ID3DBlob* errorBlob = nullptr;
	//
	//D3DCompileFromFile
	//(
	//	L"render.fx",
	//	NULL,
	//	D3D_COMPILE_STANDARD_FILE_INCLUDE,
	//	"CS",
	//	"cs_5_0",
	//	D3DCOMPILE_ENABLE_STRICTNESS,
	//	0,
	//	&pCSBlob,
	//	&errorBlob
	//);
	//
	//g_pd3dDevice->CreateComputeShader
	//(
	//	pCSBlob->GetBufferPointer(),
	//	pCSBlob->GetBufferSize(),
	//	nullptr,
	//	&g_pComputeShader
	//);
	//
	//g_pImmediateContext->CSSetShader(g_pComputeShader, nullptr, 0);
	//g_pImmediateContext->CSSetShaderResources(0, 1, &g_pTextureRV);
	//g_pImmediateContext->CSSetUnorderedAccessViews(0, 1, &g_pCSUOV, 0);
	//g_pImmediateContext->Dispatch(1, 1, 1);
	//
	//g_pImmediateContext->CopyResource(g_pCSOutputBufferResult, g_pCSOutputBuffer);
	//D3D11_MAPPED_SUBRESOURCE mappedResource;
	//g_pImmediateContext->Map(g_pCSOutputBufferResult, 0, D3D11_MAP_READ, 0, &mappedResource);
	//float* dataView = reinterpret_cast<float*>(mappedResource.pData);
	//g_pImmediateContext->Unmap(g_pCSOutputBufferResult, 0);
	//
	//if (dataView[1]==6.0) Beep(523, 500);


	return S_OK;
}
//===================================================================================================================//
HRESULT CSCENE::ShaderInit()
{
	ID3DBlob* pVSBlob = nullptr;
	ID3DBlob* pPSBlob = nullptr;
	
	D3D11_INPUT_ELEMENT_DESC layout[] =
	{
		{ 
			"POSITION",
			0, 
			DXGI_FORMAT_R32G32B32_FLOAT, 
			0, 
			0,  
			D3D11_INPUT_PER_VERTEX_DATA, 
			0 
		},

		{ 
			"NORMAL",
			0, 
			DXGI_FORMAT_R32G32B32_FLOAT, 
			0, 
			12, 
			D3D11_INPUT_PER_VERTEX_DATA, 
			0 
		},
	};

	UINT numElements = ARRAYSIZE(layout);

	D3DCompileFromFile
	(
		L"render.fx", 
		NULL,
		NULL, 
		"VS", 
		"vs_4_0", 
		D3D10_SHADER_ENABLE_STRICTNESS, 
		0, 
		&pVSBlob,
		NULL
	);

	g_pd3dDevice->CreateVertexShader
	(
		pVSBlob->GetBufferPointer(), 
		pVSBlob->GetBufferSize(), 
		nullptr, 
		&g_pVertexShader
	);

	g_pd3dDevice->CreateInputLayout
	(
		layout, 
		numElements, 
		pVSBlob->GetBufferPointer(), 
		pVSBlob->GetBufferSize(), 
		&g_pVertexLayout
	);

	D3DCompileFromFile
	(
		L"render.fx", 
		NULL, 
		NULL, 
		"PS", 
		"ps_4_0",
		D3D10_SHADER_ENABLE_STRICTNESS, 
		0, 
		&pPSBlob, 
		NULL
	);

	g_pd3dDevice->CreatePixelShader
	(
		pPSBlob->GetBufferPointer(), 
		pPSBlob->GetBufferSize(), 
		nullptr, 
		&g_pPixelShader
	);

	g_pImmediateContext->IASetInputLayout(g_pVertexLayout);

	return S_OK;
}
//===========================================================================================================================================//
HRESULT CSCENE::VertexBufferInit()
{
	UINT stride = sizeof(XMVECTOR) * 2;
	UINT offset = 0;
	D3D11_BUFFER_DESC	bd;
	D3D11_SUBRESOURCE_DATA	InitData;
	
	ZeroMemory(&bd		,	sizeof(bd		));
	ZeroMemory(&InitData,	sizeof(InitData	));
	
	bd.Usage				=	D3D11_USAGE_DEFAULT;
	bd.ByteWidth			=   stride * Mesh->GetSize(VRTX);
	bd.BindFlags			=	D3D11_BIND_VERTEX_BUFFER;
	bd.CPUAccessFlags		=	0;
	bd.MiscFlags			=	0;
	bd.StructureByteStride	=   stride;
	
	InitData.pSysMem = Mesh->GetData();
	InitData.SysMemPitch		=	0;
	InitData.SysMemSlicePitch	=	0;
	
	g_pd3dDevice->CreateBuffer
	(
		&bd, 
		&InitData, 
		&g_pVertexBuffer
	);

	g_pImmediateContext->IASetVertexBuffers
	(
		0, 
		1, 
		&g_pVertexBuffer, 
		&stride, 
		&offset
	);

	return S_OK;
}
//===========================================================================================================================================//
void CSCENE::UpdateConstantBuffer(int i)
{
	cb.mWorld = Data[i].mWorld;												//отправляем расположение объекта
	cb.SwPick = *Data[i].SwPick;											//отправляем в константный буфер выделение объекта
	cb.Blend  = *Data[i].Blend;												//альфа-канал
	cb.NumCur = i;															//номер объекта


	g_pImmediateContext->UpdateSubresource
	(
		g_pConstantsBuffer, 
		0, nullptr, 
		&cb, 
		0, 
		0
	);

	return;
}
//==========================================================================================================================================//
HRESULT CSCENE::SetViewPort()
{
	HRESULT hr = S_OK;

	D3D11_VIEWPORT vp;
	
	vp.Width = (FLOAT)width;
	vp.Height = (FLOAT)height;
	vp.MinDepth = 0.0f;
	vp.MaxDepth = 1.0f;
	vp.TopLeftX = 0.0f;
	vp.TopLeftY = 0.0f;
	
	g_pImmediateContext->RSSetViewports(1, &vp);

	return S_OK;
}
//==========================================================================================================================================//
HRESULT CSCENE::ConstantBufferInit()
{
	D3D11_BUFFER_DESC bd;
	bd.Usage               = D3D11_USAGE_DEFAULT;
	bd.ByteWidth           = sizeof(ConstantBuffer);
	bd.BindFlags           = D3D11_BIND_CONSTANT_BUFFER;
	bd.CPUAccessFlags      = 0;
	bd.MiscFlags           = 0;
	bd.StructureByteStride = sizeof(ConstantBuffer);

	g_pd3dDevice->CreateBuffer
	(
		&bd, 
		nullptr,
		&g_pConstantsBuffer
	);
	
	return S_OK;
}
//==========================================================================================================================================//
HRESULT CSCENE::TextureInit()
{	
	D3D11_SAMPLER_DESC sampDesc;	
	ZeroMemory(&sampDesc, sizeof(sampDesc));
	
	//sampDesc.Filter			= D3D11_FILTER_MIN_MAG_MIP_POINT;

	sampDesc.Filter = D3D11_FILTER_ANISOTROPIC;
	sampDesc.AddressU		=	D3D11_TEXTURE_ADDRESS_WRAP;
	sampDesc.AddressV		=	D3D11_TEXTURE_ADDRESS_WRAP;
	sampDesc.AddressW		=	D3D11_TEXTURE_ADDRESS_WRAP;
	sampDesc.ComparisonFunc	=	D3D11_COMPARISON_NEVER;
	sampDesc.MinLOD			=	0;
	sampDesc.MaxLOD			=	D3D11_FLOAT32_MAX;
	
	g_pd3dDevice->CreateSamplerState(&sampDesc, &g_pSamplerLinear);
	//DirectX::CreateWICTextureFromFile(g_pd3dDevice, L"image.jpg", nullptr, &g_pTextureRV);
	//DirectX::CreateWICTextureFromFile(g_pd3dDevice, L"image2.jpeg", nullptr, &g_pTextureRV2);
	//g_pImmediateContext->PSSetShaderResources(0, 1, &g_pTextureRV);
	//g_pImmediateContext->PSSetShaderResources(1, 1, &g_pTextureRV2);
	g_pImmediateContext->PSSetSamplers(0, 1, &g_pSamplerLinear);



	return S_OK;
}
//==========================================================================================================================================//
HRESULT CSCENE::BlendInit()
{
	D3D11_RENDER_TARGET_BLEND_DESC	rtbd;
	D3D11_BLEND_DESC				blendDesc;

	ZeroMemory(&rtbd		,	sizeof(rtbd		));
	ZeroMemory(&blendDesc	,	sizeof(blendDesc));

	rtbd.BlendEnable			=	true;
	rtbd.SrcBlend				=	D3D11_BLEND_SRC_ALPHA;
	rtbd.DestBlend				=	D3D11_BLEND_INV_SRC_ALPHA;
	rtbd.BlendOp				=	D3D11_BLEND_OP_ADD;
	rtbd.SrcBlendAlpha			=	D3D11_BLEND_SRC_ALPHA;
	rtbd.DestBlendAlpha			=	D3D11_BLEND_DEST_ALPHA;
	rtbd.BlendOpAlpha			=	D3D11_BLEND_OP_ADD;
	rtbd.RenderTargetWriteMask	=	D3D11_COLOR_WRITE_ENABLE_ALL;

	blendDesc.AlphaToCoverageEnable		=	false;
	blendDesc.IndependentBlendEnable	=	false;
	blendDesc.RenderTarget[0]			=	rtbd;

	g_pd3dDevice->CreateBlendState(&blendDesc, &g_pBlendState);
	
	return S_OK;
}
//==========================================================================================================================================//
HRESULT CSCENE::DepthInit()
{
	D3D11_TEXTURE2D_DESC			descDepth;
	D3D11_DEPTH_STENCIL_VIEW_DESC	descDSV;

	ZeroMemory(&descDepth	,	sizeof(descDepth));
	ZeroMemory(&descDSV		,	sizeof(descDSV	));

	descDepth.Width					=	width;
	descDepth.Height				=	height;
	descDepth.MipLevels				=	1;
	descDepth.ArraySize				=	1;
	descDepth.Format				=	DXGI_FORMAT_D32_FLOAT_S8X24_UINT;
	descDepth.SampleDesc.Count		=	1;
	descDepth.SampleDesc.Quality	=	0;
	descDepth.Usage					=	D3D11_USAGE_DEFAULT;
	descDepth.BindFlags				=	D3D11_BIND_DEPTH_STENCIL;
	descDepth.CPUAccessFlags		=	0;
	descDepth.MiscFlags				=	0;
	descDSV.Format					=	descDepth.Format;
	descDSV.ViewDimension			=	D3D11_DSV_DIMENSION_TEXTURE2D;
	descDSV.Texture2D.MipSlice		=	0;

	g_pd3dDevice->CreateTexture2D
	(
		&descDepth, 
		nullptr, 
		&g_pDepthStencil
	);

	g_pd3dDevice->CreateDepthStencilView
	(
		g_pDepthStencil, 
		&descDSV,
		&g_pDepthStencilView
	);

	return S_OK;
}
//==========================================================================================================================================//
void CSCENE::Render()
{
	this->time = float(clock());																							//определяем текущее время	
	this->KeyBoardUpdate();																									//обновляем состояние клавиатуры
	this->MouseUpdate();																									//обновляем состояние мыши
	this->Script();																											//выполняем скрипт сцены
	this->UpdateMatrixWorld();																								//обновляем матрицы мира
	this->UpdateConstantBuffer();																			//обновляем константный буфер


	//g_pImmediateContext->PSSetShaderResources(1, 1, &g_pTextureRV2);

	g_pImmediateContext->ClearRenderTargetView
	(
		g_pRenderTargetView, 
		ClearColor
	);												//очищаем экран
	
	g_pImmediateContext->ClearDepthStencilView
	(
		g_pDepthStencilView, 
		D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 
		1.0f, 
		0
	);												//очищаем буфер глубин
		
	size_t s1 = Mesh->GetSize(LINE) * LINE;
	size_t s2 = Mesh->GetSize(SIDE) * SIDE;

	for (int i = 0; i < Data.size(); i++)
	{
		*Data[i].Blend = 1.0;
		UpdateConstantBuffer(i);					//обновляем константный буфер для объекта

		g_pImmediateContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_LINELIST);
		g_pImmediateContext->DrawIndexed(s1, 0, 0);
		
		*Data[i].Blend = 0.5;
		UpdateConstantBuffer(i);

		g_pImmediateContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
		g_pImmediateContext->DrawIndexed(s2, s1, 0);
	}

	g_pSwapChain->Present(0, 0);																							//выводим на экране

	return;
}
//==========================================================================================================================================//
HRESULT CSCENE::SetKeyboard()
{
	LPDIRECTINPUT8 directInputObject;

	DirectInput8Create
	(
		g_hInst, 
		DIRECTINPUT_VERSION, 
		IID_IDirectInput8, 
		(void**)&directInputObject,
		0
	);
	
	directInputObject->CreateDevice
	(
		GUID_SysKeyboard,
		&keyboardDevice, 
		0
	);
	
	keyboardDevice->SetDataFormat(&c_dfDIKeyboard);
	
	keyboardDevice->SetCooperativeLevel
	(
		g_hWnd, 
		DISCL_FOREGROUND | DISCL_NONEXCLUSIVE
	);
	
	keyboardDevice->Acquire();

	return S_OK;
}
//==========================================================================================================================================//
HRESULT CSCENE::SetMouse()
{
	LPDIRECTINPUT8 directInputObject;
	
	DirectInput8Create
	(
		g_hInst, 
		DIRECTINPUT_VERSION, 
		IID_IDirectInput8, 
		(void**)&directInputObject, 
		0
	);
	
	
	directInputObject->CreateDevice
	(
		GUID_SysMouse,
		&mouseDevice, 
		0
	);
	
	mouseDevice->SetDataFormat(&c_dfDIMouse);
	
	mouseDevice->SetCooperativeLevel
	(
		g_hWnd,
		DISCL_FOREGROUND | DISCL_NONEXCLUSIVE
	);
	
	mouseDevice->Acquire();

	return S_OK;
}
//==========================================================================================================================================//
void CSCENE::MouseUpdate()
{
	DIMOUSESTATE mouseState;

	mouseDevice->GetDeviceState(sizeof(mouseState), (LPVOID)&mouseState);
	//----------------------------------------------------------------------------------------------------//

	if ((mouseState.lX != 0) || (mouseState.lY != 0))
	{
		XMVECTOR dV	=	XMVectorSubtract	(At, Eye);
		dV			=	XMVector3Normalize	(dV);
		XMVECTOR V	=	XMVector3Cross		(Up, dV);
		At			=	XMVectorAdd			(At, V * float(mouseState.lX) / 100.0f);
		V			=	XMVector3Cross		(V, dV);
		At			=	XMVectorAdd			(At, V * float(mouseState.lY) / 100.0f);
		g_View		=	XMMatrixLookAtLH	(Eye, At, Up);
	}
	//------------------------------------------------------------------------------------------------------//
	if (BUTTONDOWN(mouseState, 0))
	{
		char buffer[256];
		POINT pt;
		HDC hDC;
		DWORD color;
		int NumM;

		GetCursorPos(&pt);
		ScreenToClient(g_hWnd, &pt);

		hDC   = GetDC(g_hWnd);
		color = GetPixel(hDC, pt.x, pt.y);
		NumM  = int(float(GetRValue(color) / 255.0f*16.0f));

		keyboardDevice->GetDeviceState(sizeof(buffer), (LPVOID)&buffer);

		if (!KEYDOWN(buffer, DIK_LSHIFT))
			
			for (auto DT : Data)
				*DT.SwPick = 0;

		if (GetGValue(color) == 255)
			if (NumM < Data.size())
				*Data[NumM].SwPick = 1;
	}
	//-----------------------------------------------------------------------------------------------------------------------------------//
	if (BUTTONDOWN(mouseState, 1))
	{
		POINT pt;
		HDC hDC;
		DWORD color;
		int NumM;

		GetCursorPos(&pt);
		ScreenToClient(g_hWnd, &pt);
		
		hDC   = GetDC(g_hWnd);
		color = GetPixel(hDC, pt.x, pt.y);
		NumM  = int(float(GetRValue(color) / 255.0f*16.0f));
		
		if (GetGValue(color) == 255)
			//SwPick[NumM] = 0;
			*Data[NumM].SwPick = 0;
	}
	//----------------------------------------------------------------------------------------------------------------------------------//

	SetCursorPos(width / 2, height / 2);
}
//==========================================================================================================================================//
void CSCENE::KeyBoardUpdate()
{
	char buffer[256];

	keyboardDevice->GetDeviceState(sizeof(buffer), (LPVOID)&buffer);

	//---------------------------------------------------------------------//
	if (KEYDOWN(buffer, DIK_A))
	{
		XMVECTOR V		=	XMVectorSubtract	(At, Eye);
		V				=	XMVector3Normalize	(V);
		V				=	XMVector3Cross		(V, Up);
		Eye				=	XMVectorAdd			(Eye, V * 0.01f);
		At				=	XMVectorAdd			(At, V * 0.01f);
		g_View			=	XMMatrixLookAtLH	(Eye, At, Up);
	}
	//---------------------------------------------------------------------//
	if (KEYDOWN(buffer, DIK_D))
	{
		XMVECTOR V		=	XMVectorSubtract	(At, Eye);
		V				=	XMVector3Normalize	(V);
		V				=	XMVector3Cross		(V, Up);
		Eye				=	XMVectorAdd			(Eye, V * (-0.01f));
		At				=	XMVectorAdd			(At, V * (-0.01f));
		g_View			=	XMMatrixLookAtLH	(Eye, At, Up);
	}
	//---------------------------------------------------------------------//	
	if (KEYDOWN(buffer, DIK_W))
	{
		XMVECTOR V		=	XMVectorSubtract(At, Eye);
		V				=	XMVector3Normalize(V);
		Eye				=	XMVectorAdd(Eye, V * 0.01f);
		At				=	XMVectorAdd(At, V * 0.01f);
		g_View			=	XMMatrixLookAtLH(Eye, At, Up);
	}
	//---------------------------------------------------------------------//
	if (KEYDOWN(buffer, DIK_S))
	{
		XMVECTOR V		=	XMVectorSubtract(At, Eye);
		V				=	XMVector3Normalize(V);
		Eye				=	XMVectorAdd(Eye, V * (-0.01f));
		At				=	XMVectorAdd(At, V * (-0.01f));
		g_View			=	XMMatrixLookAtLH(Eye, At, Up);
	}
	//---------------------------------------------------------------------//
	if (KEYDOWN(buffer, DIK_ESCAPE))
	{
		PostMessage(g_hWnd, WM_QUIT, NULL, NULL);
	}
	//---------------------------------------------------------------------//

	return;
}
//==========================================================================================================================================//
HRESULT CSCENE::TargetViewInit()
{
	D3D_DRIVER_TYPE	driverTypes[] =
	{	
		D3D_DRIVER_TYPE_HARDWARE, 
		D3D_DRIVER_TYPE_WARP, 
		D3D_DRIVER_TYPE_REFERENCE 
	};

	D3D_FEATURE_LEVEL featureLevels[] = 
	{ 
		D3D_FEATURE_LEVEL_11_0,
		D3D_FEATURE_LEVEL_10_1, 
		D3D_FEATURE_LEVEL_10_0
	};

	UINT	createDeviceFlags		=	0;
	UINT	numDriverTypes			=	ARRAYSIZE(driverTypes);
	UINT	numFeatureLevels		=	ARRAYSIZE(featureLevels);
	ID3D11Texture2D* pBackBuffer	=	nullptr;
	DXGI_SWAP_CHAIN_DESC sd;

	ZeroMemory(&sd, sizeof(sd));

	sd.BufferCount							=	1;
	sd.BufferDesc.Width						=	width;
	sd.BufferDesc.Height					=	height;
	sd.BufferDesc.Format					=	DXGI_FORMAT_R8G8B8A8_UNORM;
	sd.BufferDesc.RefreshRate.Numerator		=	75;
	sd.BufferDesc.RefreshRate.Denominator	=	1;
	sd.BufferUsage							=	DXGI_USAGE_RENDER_TARGET_OUTPUT;
	sd.OutputWindow							=	g_hWnd;
	sd.SampleDesc.Count						=	1;
	sd.SampleDesc.Quality					=	0;
	sd.Windowed								=	TRUE;

	for (UINT driverTypeIndex = 0; 
	driverTypeIndex < numDriverTypes; 
	driverTypeIndex++)
	{
		g_driverType = driverTypes[driverTypeIndex];
		
		D3D11CreateDeviceAndSwapChain
		(
			nullptr, 
			g_driverType, 
			nullptr, 
			createDeviceFlags,
			featureLevels,
			numFeatureLevels,
			D3D11_SDK_VERSION,
			&sd, 
			&g_pSwapChain,
			&g_pd3dDevice, 
			&g_featureLevel, 
			&g_pImmediateContext
		);
	}

    g_pSwapChain->GetBuffer
	(
		0, 
		__uuidof(ID3D11Texture2D), 
		(LPVOID*)&pBackBuffer
	);

    g_pd3dDevice->CreateRenderTargetView
	(
		pBackBuffer, 
		nullptr, 
		&g_pRenderTargetView
	);

    pBackBuffer->Release();

	return S_OK;
}
//==========================================================================================================================================//
HRESULT CSCENE::IndexBufferInit()
{
	D3D11_BUFFER_DESC		bd;
	D3D11_SUBRESOURCE_DATA	InitData;
	size_t s1 = Mesh->GetSize(LINE) * LINE;
	size_t s2 = Mesh->GetSize(SIDE) * SIDE;
	
	ZeroMemory(&bd		,	sizeof(bd		));
	ZeroMemory(&InitData,	sizeof(InitData	));

	bd.Usage					=	D3D11_USAGE_DEFAULT;
	bd.ByteWidth				=	sizeof(UINT32) * UINT32(s1+s2);
	bd.BindFlags				=	D3D11_BIND_INDEX_BUFFER; 
	bd.CPUAccessFlags			=	0;
	bd.MiscFlags				=	0;
	bd.StructureByteStride		=	sizeof(UINT32);
	//InitData.pSysMem			=	ELEM::GetIndexBuffer(Mesh);
	InitData.SysMemPitch		=	0;
	InitData.SysMemSlicePitch	=	0;

	g_pd3dDevice->CreateBuffer(&bd, &InitData, &g_pIndexBuffer);
	g_pImmediateContext->IASetIndexBuffer(g_pIndexBuffer, DXGI_FORMAT_R32_UINT, 0);

	return S_OK;
}
//==========================================================================================================================================//
HRESULT CSCENE::Initialization()
{
	ScriptInit();
	TargetViewInit();
	DepthInit();
	SetViewPort();
	SetKeyboard();
	SetMouse();
	BlendInit();
	ShaderInit();
	VertexBufferInit();
	ConstantBufferInit();
	TextureInit();
	IndexBufferInit();
	ComputeShaderInit();

	g_pImmediateContext->OMSetRenderTargets		(1, &g_pRenderTargetView, g_pDepthStencilView);
	g_pImmediateContext->VSSetShader			(g_pVertexShader, nullptr, 0);
	g_pImmediateContext->VSSetConstantBuffers	(0, 1, &g_pConstantsBuffer);
	g_pImmediateContext->PSSetShader			(g_pPixelShader, nullptr, 0);
	g_pImmediateContext->PSSetSamplers			(0, 1, &g_pSamplerLinear);
	g_pImmediateContext->OMSetBlendState		(g_pBlendState, ClearColor, 0xFFFFFFFF);

	UpdateMatrixWorld();
	time = float(clock()) / 1000.0f;

	return S_OK;
}
//==========================================================================================================================================//
void CSCENE::UpdateConstantBuffer(void)
{
	GetCursorPos(&pt);
	ScreenToClient(g_hWnd, &pt);

	cb.mView		=	XMMatrixTranspose(g_View);
	cb.mProjection	=	XMMatrixTranspose(g_Projection);
	cb.mPosMouse[0]	=	float(pt.x);
	cb.mPosMouse[1]	=	float(pt.y);
	cb.Dir			=	XMVectorSubtract(Eye, At);
	cb.Dir = XMVectorSetW(cb.Dir, 0.0);
	cb.Dir			=	XMVector3Normalize(cb.Dir);

}
//==========================================================================================================================================//
void CSCENE::UpdateMatrixWorld(void)
{
	for (auto& DT : Data)
	{
		g_World		= XMMatrixRotationZ(XMVectorGetX(DT.Orient));
		g_World		= XMMatrixMultiply(g_World, XMMatrixRotationY(XMVectorGetY(DT.Orient)));
		g_World		= XMMatrixMultiply(g_World, XMMatrixRotationZ(XMVectorGetZ(DT.Orient)));
		g_World		= XMMatrixMultiply(g_World, XMMatrixTranslationFromVector(DT.Pos));
		DT.mWorld	= XMMatrixTranspose(g_World);
	}
}
//==========================================================================================================================================//
CSCENE::~CSCENE()
{
	if (g_pImmediateContext)	g_pImmediateContext	->ClearState();
	if (g_pConstantsBuffer)		g_pConstantsBuffer	->Release();
	if (g_pVertexBuffer)		g_pVertexBuffer		->Release();
	if (g_pVertexLayout)		g_pVertexLayout		->Release();
	if (g_pVertexShader)		g_pVertexShader		->Release();
	if (g_pPixelShader)			g_pPixelShader		->Release();
	if (g_pRenderTargetView)	g_pRenderTargetView	->Release();
	if (g_pSwapChain)			g_pSwapChain		->Release();
	if (g_pImmediateContext)	g_pImmediateContext	->Release();
	if (g_pd3dDevice)			g_pd3dDevice		->Release();
}
//==========================================================================================================================================//