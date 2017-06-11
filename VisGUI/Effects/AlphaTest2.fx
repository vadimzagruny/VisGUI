//--------------------------------------------------------------------------------------
//
// Alpha-Tested Pine Lighting Model
// Copyright (c) Microsoft Corporation. All rights reserved.
//
//--------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------
// Scene Setup
//--------------------------------------------------------------------------------------
#include "Light.fx"
#include "DirectionalLight.fx"
#include "Fog.fx"

float4 eye_vec;
float4 view_vec = 0;
// Texxture coord displace
float texDisplace = 0;

// light direction (world space)
float3 lightDir = {0.0, 1.0, 1.0f};

// Material Power
float shininess = -1;

// Transformation Matrices
matrix matWorld     : WORLD;
matrix matViewProj  : VIEWPROJECTION;
matrix matView    : VIEW;
matrix matProj;
matrix matViewInv;

//--------------------------------------------------------------------------------------
// Material Properties
//--------------------------------------------------------------------------------------

// Set by EffectInstance when mesh is loaded
// (Default values provided for Effect Edit)
float4 Diffuse  = float4( 1.f, 1.f, 1.f, 1.f );
float4 Ambient  = float4( 1.f, 1.f, 1.f, 1.f );
float4 Specular = float4( 1.f, 1.f, 1.f, 1.f );

// Texture Parameter
texture Texture0 ;

// Sampler
sampler s0 = sampler_state
{
    texture = <Texture0>;
    minfilter = ANISOTROPIC;
    mipfilter = ANISOTROPIC;
    magfilter  = ANISOTROPIC;

  MaxAnisotropy = 4;
};


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
	onorm = normalize(mul( norm, matWorld ));

	// Calculate eye vector
	oeye = normalize(mul( pos, matWorld )  - matViewInv[3]);

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
         out float4 ocolor  : COLOR )
{
	// Sample the Texture
	float4 tex_color = tex2D( s0, texcord );

	//Set Light Param
	LightParam Light;

	Light.LightDirection	= lightDir;
	Light.Normal			= w_norm;
	Light.v_eye				= eye;
	Light.MatDiffuse		= Diffuse;
	Light.MatAmbient		= Ambient;
	Light.MatSpecular		= Specular;
	Light.MatShininess		= shininess;

	float4 FogColor;

	float4 Color = float4(1,1,1,1);
	if(lightDir.y<0){Color  =  Ambient * (I_a/2); }
	if(lightDir.y>0){Color  = GetLight(Light);}

	// Modulate the texture color by the vertex lighting

	//float4 cam = iPWV;
	//float  fog = getFogValue(cam.z,fogDensity);

	ocolor = tex_color ;//*Color*float4(0.8,0.8,0.8,1);//((Color)) * fog ;
	//ocolor =  (tex_color * (Color)) * fog ;
	//ocolor += fogColor * (1.0h - fog);
}


//--------------------------------------------------------------------------------------
// Default Technique
// Establishes Vertex and Pixel Shader
// Ensures base states are set to required values
// (Other techniques within the scene perturb these states)
//--------------------------------------------------------------------------------------
technique tec0
{
    pass p0
    {

		//NormalizeNormals = true;
		ZEnable          = TRUE;
		ZWriteEnable     = TRUE;
		//CullMode         = ccw;

		//AlphaBlendEnable = true;
		//SrcBlend = SRCALPHA;
		//DestBlend = INVSRCALPHA;
		//BlendOp = ADD;
		//FILLMODE = WIREFRAME;

		// AlphaTestEnable  = TRUE;
		//AlphaRef         = 120;
		//AlphaFunc        = GREATER;

		VertexShader = compile vs_3_0 VS();
		PixelShader  = compile ps_3_0 PS();

    }
}
