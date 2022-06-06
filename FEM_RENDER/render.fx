int Size1  = 500 ;
int Size2  = 500  ;
int Start  = 4081 ;
int End    = 4115 ;
//==================================================================================================================================//
//StructuredBuffer<float>	MyData:	register(t0);
//RWStructuredBuffer<float>	MyData2: register(u0);
Texture2D						txDiffuse1 : register(t0);
Texture2D						txDiffuse2 : register(t1);			//передает текстуру
SamplerState					samLinear : register(s0);			//аппроксимаци€ структуры
//==================================================================================================================================//
//==================================================================================================================================//
struct SBuffer0
{
	float4x4			World;						//матрица мира
	float4x4			View;						//матрицы вида
	float4x4			Projection;					//матрица проекции
	float4				Dir;						//направление смотр€щего	
	int					SwPick;						//выделение
	float				Blend;						//прозрачность
	float2				PosMouse;					//позици€ мыши
	int					NumCur;						//текущий номер объекта

	// 12 байт до выравнивани€ 
};
//==================================================================================================================================//
cbuffer ConstantBuffer : register (b0)
{
	SBuffer0 b0;
}
//==================================================================================================================================//
struct VS_INPUT
{
	float4 Pos	: POSITION;
	float3 Norm	: NORMAL;
};
//==================================================================================================================================//
struct PS_INPUT
{
	float4	Pos			:	SV_POSITION;				//координаты вершины
	float2	Mouse		:	TEXCOORD2;					//позици€ мыши
	int		Num			:	TEXCOORD3;					//номер объекта
	int		SwPickMesh	:	TEXCOORD4;					//выделение
	float	BlendMesh	:	TEXCOORD5;					//прозрачность
	float	Valu		:	TEXCOORD7;					//€ркость
};
//==================================================================================================================================//
PS_INPUT VS(VS_INPUT input)
{
	//вершинный шейдер//

	PS_INPUT output		=	(PS_INPUT)0;
	output.Pos			=	mul(input.Pos, b0.World);
	output.Pos			=	mul(output.Pos, b0.View);
	output.Pos			=	mul(output.Pos, b0.Projection);
	output.Mouse		=	b0.PosMouse;
	output.Num			=	b0.NumCur;
	output.SwPickMesh	=	b0.SwPick;
	output.BlendMesh	=	b0.Blend;	
	input.Norm			=	mul(input.Norm, b0.World);
	output.Valu			=	abs(dot(input.Norm, b0.Dir));
	return output;
}
//=================================================================================================================================//
float4 PS(PS_INPUT input) : SV_Target
{
	//пиксельный шейдер//

	//float4 color = saturate(input.Valu) * float4(0.5f, 0.5f, 0.5f, 1.0f);
	float4 color;
	color[0] = input.Valu;
	color[1] = input.Valu;
	color[2] = input.Valu;
	color[3] = 1.0;
	//float2 dd = {1.0,1.0};
	//dd[0] =  input.Pos[0]/1366.0;
	//dd[1] =  input.Pos[1]/768.0+input.Valu*1.0;
	
	//color *= txDiffuse1.Sample(samLinear, dd);
	//color[3] = 1.0;

	if (input.BlendMesh > 0.6)
	{
		color[0] = 0.0;
		color[1] = 0.0;
		color[2] = 1.0;
		color[3] = 1;
	}

	if ((abs(input.Mouse[0] - input.Pos[0]) < 10.0f) && (abs(input.Mouse[1] - input.Pos[1]) < 2.0f) ||
		(abs(input.Mouse[0] - input.Pos[0]) < 2.0f) && (abs(input.Mouse[1] - input.Pos[1]) < 10.0f))
		{
			//return float4(0.0f, 0.0f, 1.0f, 0.5f);
			color[1] = 1.0;
			color[3] = 1.0;
		}
		
	//color *= txDiffuse.Sample(samLinear, dd);
	//color[3] = 1.0;

	//color[1] = 0.0;
	//color[2] = 0.0;
	
	return color;
}
//===============================================================================================================================//
//[numthreads(5, 1, 1)]
//void CS(int3 DTid : SV_DispatchThreadID)
//{
//	MyData2[DTid.x] = MyData[DTid.x]*2;
//}
//===================//