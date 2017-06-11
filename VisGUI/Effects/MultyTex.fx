
//--------------------------------------------------------------------------------------
//
// MyltyTexturing Shader
// Copyright (c) Vadim Zagruny
//
//--------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------
// Scene Setup
//--------------------------------------------------------------------------------------


#include"Header.hlsli"

//// VertexShader InOut Structs
struct VS_INPUT
{
	float3 pos      : POSITION; 
	float3 norm     : NORMAL; 
	float3 tang		: TANGENT;
	float3 bnorm	: BINORMAL;
	float2 texcrd1  : TEXCOORD;
	float2 texcrd2  : TEXCOORD1;
};


struct VS_OUTPUT
{
	float4 opos     : POSITION;
	float2 otexcrd1 : TEXCOORD0;
	float2 otexcrd2 : TEXCOORD1;
	float3 oeye[3]	: TEXCOORD5;
	float3 onorm    : NORMAL;
	float4 oIPos    : COLOR1;
	float4 oIPWV    : COLOR2;
};
//-----------------------------------



//--------------------------------------------------------------------------------------
// Vertex Shader
//--------------------------------------------------------------------------------------
VS_OUTPUT VS( VS_INPUT IN)
{
	VS_OUTPUT OUT;

	//Calculate the normal (in world-space)
	OUT.onorm = normalize(mul( IN.norm, matWorld ));
	OUT.oIPos = mul(float4(IN.pos, 1.f), matWorld);
	
	if(bActiveNormalMap != false){

		float3 normal   = OUT.onorm;
		float3 tangent  = mul( IN.tang,  (float3x3) matWorld );
		float3 binormal = mul( IN.bnorm, (float3x3) matWorld );

		normal   = normalize( normal );
		tangent  = normalize( tangent );
		binormal = normalize( binormal );

		// Calculate eye vector
	
		OUT.oeye[0] =	normalize(matViewInv[3] - OUT.oIPos);
		OUT.oeye[0] =	float3(dot(OUT.oeye[0],tangent),dot(OUT.oeye[0],binormal),dot(OUT.oeye[0],normal));
		OUT.oeye[1] =	float3(dot(lightDir,tangent),dot(lightDir,binormal),dot(lightDir,normal));
		OUT.oeye[2] =	float3(dot(MoonlightDir,tangent),dot(MoonlightDir,binormal),dot(MoonlightDir,normal));
	}
	else{
		OUT.oeye[0] =	normalize(matViewInv[3] - OUT.oIPos);
		OUT.oeye[1] =	lightDir;
		OUT.oeye[2] =	MoonlightDir;
	}

	// Pass the texture coordinates to the pixel shader
	OUT.otexcrd1 = IN.texcrd1;
	OUT.otexcrd2 = IN.texcrd2;

	// Transform the vertex to clip space
	OUT.opos  = mul(float4(IN.pos, 1.f), mul(mul( matWorld, matView ),matProj) );
	
	OUT.oIPWV = mul(float4(IN.pos, 1.f), mul(matWorld,matView));

	return OUT;
}

//--------------------------------------------------------------------------------------
// Pixel Shader
//--------------------------------------------------------------------------------------
void PS(in  float3 eye[3]	: TEXCOORD5,
		in  float2 texcrd1	: TEXCOORD,
		in  float2 texcrd2	: TEXCOORD1,
		in  float4 iPos		: COLOR1,
		in  float4 iPWV		: COLOR2,
		in  float3 w_norm	: NORMAL,
		out float4 ocolor	: COLOR )
{
    // Sample the Texture
	float4 tex_color1 = tex2D( SamplerDiffuse, texcrd1 );
	float4 tex_color2 = tex2D( SamplerAmbient, texcrd2 );

	LightParam Light;

	Light.v_eye				= eye[0];
	Light.LightDirection	= eye[1];
	Light.MoonLightDir		= eye[2];
	Light.Normal			= w_norm;
	Light.MatDiffuse		= Diffuse;
	Light.MatAmbient		= Ambient;
	Light.MatSpecular		= Specular;
	Light.MatShininess		= shininess;
	Light.TC = texcrd1;

	float4 FogColor;

	float4 Color = float4(1,1,1,1);
	float4 DifColor,SpecColor;
	GetLight(Light,DifColor,SpecColor);

	float4 TexColor = (tex_color1 * tex_color2);
	DifColor += GetAngleIntensity(w_norm,iPos);
	Color  =  (TexColor * DifColor + SpecColor);
	Color.w *= tex_color1.w*tex_color2.w;

	// Modulate the texture color by the vertex lighting
	float4 cam = iPWV;
	float  fog = getFogValue(cam.z,fogDensity);
	
	ocolor = ( Color);
	ocolor.xyz *= fog;
	ocolor.xyz += (fogColor * (1.0h - fog)).xyz;
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
