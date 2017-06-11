
float4x4 mWorld			: WORLD;
float4x4 mView;



//-------------- CubeMapVar --------------//
float4x4 g_mProj;
texture  g_txCubeMap;
texture	 g_tx2DMap;
float    g_fReflectivity;  // Reflectivity value
//----------------------------------------//

struct VS_OUTPUT
{
    float4 Pos			: POSITION;
    float3 Normal		: TEXCOORD1;
    float4 LightDir		: TEXCOORD0;
	float2 tex_coord	: TEXCOORD2;
	float3 eyepos		: TEXCOORD3;
	float3 Reflect		: TEXCOORD4;
};


//-- CREATE TEXTURE  --//

//----- TexCubeMap ------//
samplerCUBE g_samCubeMap = 
sampler_state
{
    Texture = <g_txCubeMap>;
    MinFilter = ANISOTROPIC;
    MagFilter = ANISOTROPIC;
    MipFilter = ANISOTROPIC;
};

//----- TexCubeMap ------//
sampler2D g_sam2DMap = 
sampler_state
{
    Texture   = <g_tx2DMap>;
    MinFilter = ANISOTROPIC;
    MagFilter = ANISOTROPIC;
    MipFilter = ANISOTROPIC;
};


//-----------------------------------------------------------------------------
// Vertex Shader: HDRVertEnvMap
// Desc: Process vertex for HDR environment mapping
//-----------------------------------------------------------------------------
void HDRVertEnvMap( float4 Pos			: POSITION,
                    float3 Normal		: NORMAL,
                    out float4 oPos		: POSITION,
					out float4 oPWV		: TEXCOORD4,
					out float3 oNorm	: NORMAL,
                    out float3 oEye		: TEXCOORD5)
{
    oPos = mul( Pos,mul(mWorld,mView) );

    // Compute normal in camera space
    float3 vN = mul( Normal, mul(mWorld,mView) );
    vN = normalize( vN );

    // Obtain the reverse eye vector
    float3 vEyeR = -normalize( oPos );

	oPWV = oPos;
	oEye = vEyeR;
	oNorm = vN;

    // Apply the projection
    oPos = mul(  oPos, g_mProj );
}
//-----------------------------------------------------------------------------
// Pixel Shader: HDRPixEnvMap
// Desc: Process pixel for HDR environment mapped object
//-----------------------------------------------------------------------------
float4 HDRPixEnvMap(float4 PWV : TEXCOORD4, float3 Norm : NORMAL, float3 Eye : TEXCOORD5) : COLOR
{
	float3 EnvTex = reflect(normalize( PWV - Eye),Norm);
    return 1 * texCUBE( g_samCubeMap, EnvTex );
}

//-----------------------------------------------------------------------------
// Vertex Shader: 2DRVertEnvMap
// Desc: Process vertex for 2D environment mapping
//-----------------------------------------------------------------------------
void Vert2DEnvMap(	float4 Pos			: POSITION,
                    float3 Normal		: NORMAL,
					float2 tex_coord	: TEXCOORD5,
                    out float4 oPos		: POSITION,
                    out float2 EnvTex	: TEXCOORD5)
{

	float3 coord;
	coord.x = 1-tex_coord.x;
	coord.y = tex_coord.y;
	coord.z = 1;

	//Pos.x /= 15;
	//Pos.y /= 20;

    oPos = mul( Pos,mul(mWorld,mView) );
	oPos = mul( oPos,g_mProj);
    // Store the reflection vector in texcoord0
    EnvTex = coord;//oPos.xy/oPos.w*0.5+0.5;



    
}
//-----------------------------------------------------------------------------
// Pixel Shader: 2DPixEnvMap
// Desc: Process pixel for 2D environment mapped object
//-----------------------------------------------------------------------------
float4 Pix2DEnvMap( float2 Tex : TEXCOORD5 ) : COLOR
{	
    return  tex2D( g_sam2DMap, Tex );
}



//-----------------------------------------------------------------------------
// Technique: RenderEnvMesh
// Desc: Renders the HDR environment-mapped mesh
//-----------------------------------------------------------------------------
technique RenderHDREnvMap
{
    pass p0
    {
		CullMode		= false;
        VertexShader = compile vs_3_0 HDRVertEnvMap();
        PixelShader = compile ps_3_0 HDRPixEnvMap();
    }
}

technique Render2DEnvMap
{
    pass p0
    {
		CullMode		= ccw;
        VertexShader	= compile vs_3_0 Vert2DEnvMap();
        PixelShader		= compile ps_3_0 Pix2DEnvMap();
    }
}