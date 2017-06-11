//--------------------------------------------------------------------------------------
//
// Ambient lighting only
//
//--------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------
// Scene Setup
//--------------------------------------------------------------------------------------
#include"Header.hlsli"

//--------------------------------------------------------------------------------------
// Vertex Shader
//--------------------------------------------------------------------------------------
void VS( in  float3 pos      : POSITION,
         in  float3 norm     : NORMAL,
		 in	 float  diffuse	 : TEXCOORD1,
         in  float2 texcrds  : TEXCOORD,
         out float4 opos     : POSITION,
		 out float3 oeye     : TEXCOORD5,
		 out float4 oIPos    : COLOR1,
		 out float4 oIPWV    : COLOR2,
		 out float3 onorm    : NORMAL,
         out float2 otexcrds : TEXCOORD )
{
	// Transform the vertex to clip space
	opos = mul( float4(pos, 1.f), mul(mul(matWorld, matView),matProj) );

	//Calculate the normal (in world-space)
	onorm = (mul( norm, matWorld ));

	// Calculate eye vector
	oeye = (matViewInv[3] - mul( pos, matWorld ));

	// Pass the texture coordinates to the pixel shader
	otexcrds.x = texcrds.x + texDisplace;
	otexcrds.y = texcrds.y;

	oIPos = mul(float4(pos, 1.f), matWorld);
	oIPWV = mul(float4(pos, 1.f), mul(matWorld,matView));
}

//--------------------------------------------------------------------------------------
// Pixel Shader
//--------------------------------------------------------------------------------------
void PS( in   float3 eye    : TEXCOORD5,
         in  float2 texcord : TEXCOORD,
		 in  float3 w_norm  : NORMAL,
		 in  float4	IPos    : COLOR1,
		 in  float4	iPWV    : COLOR2,
         out float4 ocolor  : COLOR )
{
	// Sample the Texture
	float4 tex_color = tex2D( SamplerDiffuse, texcord );

	float4 FogColor;

	float4 Color = float4(1,1,1,1);
	if(lightDir.y<0){Color  =  Ambient * (I_Ma); }
	if(lightDir.y>0){Color  =  Ambient *I_a;}

	// Modulate the texture color by the vertex lighting

	float4 cam = iPWV;
	float  fog = getFogValue(cam.z,fogDensity);

	ocolor.w	= tex_color.w;
	ocolor.xyz	=  (tex_color * (Color)) ;
	ocolor.xyz	*= fog;
	ocolor.xyz	+= (fogColor * (1.0h - fog)).xyz;
}


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
