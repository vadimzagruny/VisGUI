// NoLight
//---------------------------------
//--------------------------------------------------------------------------------------
// Scene Setup
//--------------------------------------------------------------------------------------
#include"Header.hlsli"


//--------------------------------------------------------------------------------------
// Pixel Shader
//--------------------------------------------------------------------------------------
void PS( in  float2 texcord : TEXCOORD,
		in  float4  col : COLOR,
		 out float4 ocolor  : COLOR )
{
	// Sample the Texture
	float4 tex_color = tex2D( SamplerDiffuse, texcord );

	ocolor =  (tex_color)*Diffuse;
}


//--------------------------------------------------------------------------------------
// Default Technique
//--------------------------------------------------------------------------------------
technique tec0
{
    pass p0
    {
		ZEnable          = TRUE;
		ZWriteEnable     = TRUE;

		//VertexShader = compile vs_3_0 VS();
		PixelShader  = compile ps_3_0 PS();

    }
}
