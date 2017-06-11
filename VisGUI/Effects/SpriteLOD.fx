// SpriteLOD
//---------------------------------
//--------------------------------------------------------------------------------------
// Scene Setup
//--------------------------------------------------------------------------------------
#include"Header.hlsli"

// VertexShader InOut Structs
struct VS_INPUT
{
	float4 pos      : POSITION; 
	float2 Depth	: TEXCOORD1;
	float3 normal   : NORMAL; 
	float4 diffuse	: COLOR0;
	float3 tang		: TANGENT;
	float3 bnorm	: BINORMAL;
	float2 texcrd1  : TEXCOORD;
};


struct VS_OUTPUT
{
	float4 pos     : POSITION;
	float4 Col     : COLOR0;
	float4 IPos    : TEXCOORD1;
	float4 IPWV    : COLOR2;
	float2 texcrd1 : TEXCOORD;
};
//-----------------------------------

//--------------------------------------------------------------------------------------
// Vertex Shader
//--------------------------------------------------------------------------------------
VS_OUTPUT VS( VS_INPUT IN)
{
	VS_OUTPUT OUT;
	
	OUT.texcrd1.x = IN.texcrd1.x;
	OUT.texcrd1.y = IN.texcrd1.y;

	float3 vRight = float3(matView[0][0],matView[1][0],matView[2][0]);
	float3 vUp    = float3(matView[0][1],matView[1][1],matView[2][1]);

	float3 pos = (IN.Depth.x * normalize(vRight) - IN.Depth.y * normalize(vUp)) + IN.pos.xyz;

	// Transform the vertex to clip space
	OUT.pos  = mul(float4(pos,1), mul(mul(matWorld, matView),matProj));
	OUT.IPos = mul(float4(pos,1), matView);
	OUT.IPWV = mul(float4(pos,1), mul(matWorld,matView));

	OUT.Col = IN.diffuse;

	return OUT;
}

//--------------------------------------------------------------------------------------
// Pixel Shader
//--------------------------------------------------------------------------------------
void PS( 
         in  float2 texcord : TEXCOORD,
		 in  float4	Col     : COLOR0,
		 in  float4	IPos    : TEXCOORD1,
		 in  float4	iPWV    : COLOR2,
         out float4 ocolor  : COLOR
		 ,in float f :VFACE)
{
	// Sample the Texture
	float4 tex_color = tex2D( SamplerDiffuse, texcord );

	float  fog = getFogValue(IPos.z,fogDensity);

	float4 Color = float4(1,1,1,1);
	if(lightDir.y<0){Color  =  Ambient *(I_Ma); }
	if(lightDir.y>0){Color  =  Ambient * I_a;}

	ocolor		=  (tex_color * Color);

	ocolor.xyz	*= fog;
	ocolor.xyz	+= (fogColor * (1.0h - fog)).xyz;

}


//--------------------------------------------------------------------------------------
// Default Technique
//--------------------------------------------------------------------------------------
technique tec0
{
    pass p0
    {
		//NormalizeNormals = true;
		ZEnable          = TRUE;
		ZWriteEnable     = TRUE;

		VertexShader = compile vs_3_0 VS();
		PixelShader  = compile ps_3_0 PS();

    }
}