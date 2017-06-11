//----------------------------------------------------------------
//
// Window Shader for models without special effects
//
// Copyright (c) Vadim Zagruny
//----------------------------------------------------------------

//----------------------------------------------------------------
// Scene Setup
//----------------------------------------------------------------
#include"Header.hlsli"


// VertexShader InOut Structs
struct VS_INPUT
{
	float3 pos      : POSITION; 
	float3 norm     : NORMAL; 
	float4 diffuse	: COLOR0;
	float3 tang		: TANGENT;
	float3 bnorm	: BINORMAL;
	float2 texcrd1  : TEXCOORD;
	float2 texcrd2  : TEXCOORD1;
};


struct VS_OUTPUT
{
	float4 pos     : POSITION;
	float2 texcrd1 : TEXCOORD0;
	float2 texcrd2 : TEXCOORD1;
	float3 eye[3]  : TEXCOORD5;
	float3 norm    : NORMAL;
	float4 IPos    : COLOR1;
	float4 IPWV    : COLOR2;
};
//-----------------------------------

//--------------------------------------------------------------------------------------
// Vertex Shader
//--------------------------------------------------------------------------------------
VS_OUTPUT VS( VS_INPUT IN)
{
		VS_OUTPUT OUT;
	//Calculate the normal (in world-space)
	OUT.norm = normalize(mul( IN.norm, matWorld ));
	OUT.IPos = mul(float4(IN.pos, 1.f), matWorld);
	if(bActiveNormalMap != false){
		float3 normal   = OUT.norm;
		float3 tangent  = mul( IN.tang,  (float3x3) matWorld );
		float3 binormal = mul( IN.bnorm, (float3x3) matWorld );

		normal   = normalize( normal );
		tangent  = normalize( tangent );
		binormal = normalize( binormal );
//
		// Calculate eye vector
		OUT.eye[0] =	normalize(matViewInv[3] - OUT.IPos);//OUT.IPos - matViewInv[3];
		OUT.eye[0] =	float3(dot(OUT.eye[0],tangent),dot(OUT.eye[0],binormal),dot(OUT.eye[0],normal));
		OUT.eye[1] =	float3(dot(lightDir,tangent),dot(lightDir,binormal),dot(lightDir,normal));
		OUT.eye[2] =	float3(dot(MoonlightDir,tangent),dot(MoonlightDir,binormal),dot(MoonlightDir,normal));
	}
	else{
		// Calculate eye vector
		OUT.eye[0] =	normalize(matViewInv[3] - OUT.IPos);//OUT.IPos - matViewInv[3];
		OUT.eye[1] =	lightDir;
		OUT.eye[2] =	MoonlightDir;
	}

	// Pass the texture coordinates to the pixel shader
	OUT.texcrd1.x = IN.texcrd1.x;
	OUT.texcrd1.y = IN.texcrd1.y;
	OUT.texcrd2 = 0;
	// Transform the vertex to clip space
	OUT.pos  = mul(float4(IN.pos, 1.f), mul(mul(matWorld, matView),matProj) );
	OUT.IPWV = mul(float4(IN.pos, 1.f), mul(matWorld,matView));

	return OUT;
}

//--------------------------------------------------------------------------------------
// Pixel Shader
//--------------------------------------------------------------------------------------
void PS( in  float3 eye[3] : TEXCOORD5,
         in  float2 texcord : TEXCOORD,
		 in  float3 w_norm  : NORMAL,
		 in  float4	IPos    : COLOR1,
		 in  float4	iPWV    : COLOR2,
         out float4 ocolor  : COLOR )
{
	// Sample the Texture
	float4 tex_color = tex2D( SamplerDiffuse, texcord );
	float4 Mask_color = tex2D( SamplerSpecular, texcord );

	//Set Light Param
	LightParam Light;

	Light.v_eye				= eye[0];
	Light.LightDirection	= eye[1];
	Light.MoonLightDir		= eye[2];
	Light.Normal			= w_norm;
	Light.MatDiffuse		= Diffuse;
	Light.MatAmbient		= Ambient;
	Light.MatSpecular		= Specular;
	Light.MatShininess		= shininess;
	Light.TC = texcord;

	float4 Color = float4(1,1,1,1);
	float4 DifColor,SpecColor;
	GetLight(Light,DifColor,SpecColor);
	
	DifColor += GetAngleIntensity(w_norm,IPos);
	Color  = tex_color * DifColor + SpecColor;
	Color.w *= tex_color.w;

	float4 cam = iPWV;
	float  fog = getFogValue(cam.z,fogDensity);

	ocolor = Color;

	//ocolor		=  lerp((Mask_color*float4(1.0,1.0,0,1.0)*tex_color),ocolor,saturate(lightDir.y));
	
	float4 WindowColor	= (Mask_color*float4(1.0,1.0,0,1.0)*tex_color)+ocolor;
	ocolor				=  lerp(WindowColor,ocolor,saturate(lightDir.y));
	
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
