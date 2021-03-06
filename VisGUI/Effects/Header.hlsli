//----------------------------------------------------------------
//
// Global Include Shader
//
// Copyright (c) Vadim Zagruny
//----------------------------------------------------------------
#include "BlendFunctions.fx"
#include "DirectionalLight.fx"
#include "Fog.fx"

// Moon light direction
float3 MoonlightDir : MoonDirection<  
	string UIName = "Moon Light Direction"; 
	//string Object = "TargetLight";
	> = {0.0, 1.0, 1.0f} ;

// light direction (world space)
float3 lightDir : Direction<  
	string UIName = "Light Direction"; 
	string Object = "TargetLight";
	> = {0.0, 1.0, 0.0f} ;

//----------------------------------------------------------------------------------------
// Transformation Matrices
//---------------------------------------------------------------------------------------
matrix matWorld     : WORLD;
matrix matViewProj  : VIEWPROJECTION;
matrix matView		: VIEW;
matrix matProj		: PROJECTION;
matrix matViewInv	: ViewI;


//--------------------------------------------------------------------------------------
// Material Properties
//--------------------------------------------------------------------------------------

// Set by EffectInstance when mesh is loaded
// (Default values provided for Effect Edit)
float4 Diffuse  <
	string UIName = "Diffuse";
	> = float4( 0.47f, 0.47f, 0.47f, 1.0f );    // diffuse= float4( 1.f, 1.f, 1.f, 1.f );
float4 Ambient <
	string UIName = "Ambient";
	>  = float4( 1.f, 1.f, 1.f, 1.f );
float4 Specular  <
	string UIName = "Specular";
	> = float4( 0.2f,0.2f, 0.2f, 1.f );

// Material Power
float shininess<
	string UIName = "MatShininess";
	>  = float( 10.28f);
//----------------------------------------------------------------------------------
// Texture Parameter, annotation specifies default texture for EffectEdit
//----------------------------------------------------------------------------------

bool bActiveNormalMap	= false;
bool bActiveSpecularMap = false;

texture DiffuseMap :DiffuseMap<			
	string name		= "seafloor.dds"; 
	string UIName	= "Diffuse Texture";
	>;
texture AmbientMap :AmbientMap<			
	string name		= "seafloor.dds"; 
	string UIName	= "Ambient Texture";
	>;
texture SpecularMap :SpecularMap< 
	string name		= "seafloor.dds"; 
	string UIName	= "Specular Texture";
	>;
texture NormalMap	:BumpMap< 
	string name		= "seafloor.dds"; 
	string UIName	= "Normal Texture";
	>;
texture RefractionMap :RefractionMap< 
	string name		= "seafloor.dds"; 
	string UIName	= "Refraction Texture";
	>;
texture ReflectionMap :ReflectionMap< 
	string name		= "seafloor.dds"; 
	string UIName	= "Reflection Texture";
	>;

// Sampler, for sampling the pine texture
sampler SamplerAmbient = sampler_state		//S1
{
	texture = <AmbientMap>;
	minfilter = ANISOTROPIC;
	mipfilter = ANISOTROPIC;
	magfilter = ANISOTROPIC;

	MaxAnisotropy = 4;
};

sampler SamplerDiffuse = sampler_state			//S0
{
	texture = <DiffuseMap>;
	minfilter  = ANISOTROPIC;
	mipfilter  = ANISOTROPIC;
	magfilter  = ANISOTROPIC;

	MaxAnisotropy = 4;
};

sampler SamplerSpecular = sampler_state			//S2
{
	texture = <SpecularMap>;
	minfilter  = ANISOTROPIC;
	mipfilter  = ANISOTROPIC;
	magfilter  = ANISOTROPIC;

	MaxAnisotropy = 4;
};

sampler SamplerNormal = sampler_state			//S2
{
	texture = <NormalMap>;
	minfilter  = ANISOTROPIC;
	mipfilter  = ANISOTROPIC;
	magfilter  = ANISOTROPIC;

	MaxAnisotropy = 4;
};
sampler SamplerReflection = sampler_state		//S3
{
	texture = <ReflectionMap>;
	minfilter  = ANISOTROPIC;
	mipfilter  = ANISOTROPIC;
	magfilter  = ANISOTROPIC;

	AddressU = Clamp;
	AddressV = Clamp;


	MaxAnisotropy = 4;
};
sampler SamplerRefraction = sampler_state		//S4
{
	texture = <RefractionMap>;
	minfilter  = ANISOTROPIC;
	mipfilter  = ANISOTROPIC;
	magfilter  = ANISOTROPIC;

	MaxAnisotropy = 4;
};

// Texxture coord displace
float4 texDisplace  = 0;
// Wind speed
float4 WindVelocity = float4(0.0,0.0,0.0,1);
// Counter
float time  <  
	string UIName = "Time"; 
	string Object = "TargetTime";
	> = 5;


//--------------------------------------------------------------------------------------
//		Ligting
//--------------------------------------------------------------------------------------

struct LightParam{

	float3 LightDirection;
	float3 MoonLightDir;
	float3 Normal;
	float3 v_eye;
	float4 MatDiffuse;
	float4 MatAmbient;
	float4 MatSpecular;
	float2 TC;

	float	MatShininess;
};

//Sun light intensity
float4 I_a  <
	string UIName = "I_a";
	> = float4( 0.5f, 0.5f, 0.5f, 1.0f );   
float4 I_d <
	string UIName = "I_d";
	>  = float4( 0.8f, 0.8f, 0.8f, 1.f );
float4 I_s  <
	string UIName = "I_s";
	> = float4( 0.5f,0.5f, 0.5f, 1.f );

//Moon light intensity
float MoonIntensityCoef = 1.0f;

float4 I_Ma  <
	string UIName = "I_Ma";
	> = float4( 0.1f, 0.1f, 0.1f, 1.0f );   
float4 I_Md <
	string UIName = "I_Md";
	>  = float4( 0.1f, 0.1f, 0.2f, 1.f );
float4 I_Ms  <
	string UIName = "I_Ms";
	> = float4( 0.2f,0.2f, 0.2f, 1.f );

float2 getParallaxOcclusionMappingOffset( sampler2D map, float heightScale, float2 pomTexCoord, float3 eye, float3 normal )
{
	#ifdef DIRECT3D
		#define TEX2DGRAD tex2Dgrad
	#else
		//!!!!!
		#error getParallaxOcclusionMappingOffset OpenGL
		//#define TEX2DGRAD tex2D
	#endif

	//-------------------------------------------------------------------------------
	//SodanKerjuu: this can be calculated in vertex shader for some speed improvement
	//-------------------------------------------------------------------------------
	// Compute initial parallax displacement direction:
	float2 vParallaxDirection = normalize(  eye.xy );

	// The length of this vector determines the furthest amount of displacement:
	float fLength         = length( eye );
	float fParallaxLength = sqrt( fLength * fLength - eye.z * eye.z ) / eye.z; 

	// Compute the actual reverse parallax displacement vector:
	float2 vParallaxOffsetTS = vParallaxDirection * fParallaxLength;

	// Need to scale the amount of displacement to account for different height ranges
	// in height maps. This is controlled by an artist-editable parameter:
	vParallaxOffsetTS *= heightScale;
	//vParallaxOffsetTS *= HEIGHT_SCALE;
	//-------------------------------------------------------------------------------

	// Adaptive in-shader level-of-detail system implementation. Compute the 
	// current mip level explicitly in the pixel shader and use this information 
	// to transition between different levels of detail from the full effect to 
	// simple bump mapping. See the above paper for more discussion of the approach
	// and its benefits.

	// Compute the current gradients:
	float2 g_vTextureDims = float2(1,1);
	float2 fTexCoordsPerSize = pomTexCoord * g_vTextureDims;

	// Compute all 4 derivatives in x and y in a single instruction to optimize:
	/*
	float4 dx4 = ddx( float4( fTexCoordsPerSize, pomTexCoord ) );
	float2 dxSize = dx4.xy;
	float2 dx = dx4.zw;
	float4 dy4 = ddy( float4( fTexCoordsPerSize, pomTexCoord ) );
	float2 dySize = dy4.xy;
	float2 dy = dy4.zw;
	*/
	float2 dxSize, dySize;
	float2 dx, dy;
	float4( dxSize, dx ) = ddx( float4( fTexCoordsPerSize, pomTexCoord ) );
	float4( dySize, dy ) = ddy( float4( fTexCoordsPerSize, pomTexCoord ) );

	float  fMipLevel;      
	float  fMipLevelInt;    // mip level integer portion
	float  fMipLevelFrac;   // mip level fractional amount for blending in between levels

	float  fMinTexCoordDelta;
	float2 dTexCoords;

	// Find min of change in u and v across quad: compute du and dv magnitude across quad
	dTexCoords = dxSize * dxSize + dySize * dySize;

	// Standard mipmapping uses max here
	fMinTexCoordDelta = max( dTexCoords.x, dTexCoords.y );

	// Compute the current mip level  (* 0.5 is effectively computing a square root before )
	fMipLevel = max( 0.5 * log2( fMinTexCoordDelta ), 0 );

	// Start the current sample located at the input texture coordinate, which would correspond
	// to computing a bump mapping result:
	float2 texSample = pomTexCoord;

	float g_nLODThreshold = 4000;
	if ( fMipLevel <= (float) g_nLODThreshold )
	{
		//===============================================//
		// Parallax occlusion mapping offset computation //
		//===============================================//

		// Utilize dynamic flow control to change the number of samples per ray 
		// depending on the viewing angle for the surface. Oblique angles require 
		// smaller step sizes to achieve more accurate precision for computing displacement.
		// We express the sampling rate as a linear function of the angle between 
		// the geometric normal and the view direction ray:
		int g_nMaxSamples = 60;
		int g_nMinSamples = 20;
		int nNumSteps = (int) lerp( g_nMaxSamples, g_nMinSamples, dot( eye, normal ) );

		// Intersect the view ray with the height field profile along the direction of
		// the parallax offset ray (computed in the vertex shader. Note that the code is
		// designed specifically to take advantage of the dynamic flow control constructs
		// in HLSL and is very sensitive to specific syntax. When converting to other examples,
		// if still want to use dynamic flow control in the resulting assembly shader,
		// care must be applied.
		// 
		// In the below steps we approximate the height field profile as piecewise linear
		// curve. We find the pair of endpoints between which the intersection between the 
		// height field profile and the view ray is found and then compute line segment
		// intersection for the view ray and the line segment formed by the two endpoints.
		// This intersection is the displacement offset from the original texture coordinate.
		// See the above paper for more details about the process and derivation.
		//

		float fCurrHeight = 0.0;
		float fStepSize   = 1.0 / (float) nNumSteps;
		float fPrevHeight = 1.0;
		float fNextHeight = 0.0;

		int    nStepIndex = 0;

		float2 vTexOffsetPerStep = fStepSize * vParallaxOffsetTS;
		float2 vTexCurrentOffset = pomTexCoord;
		float  fCurrentBound     = 1.0;
		float  fParallaxAmount   = 0.0;

		float2 pt1 = 0;
		float2 pt2 = 0;

		float2 texOffset2 = 0;

		while ( nStepIndex < nNumSteps ) 
		{
			vTexCurrentOffset -= vTexOffsetPerStep;
			#define HEIGHT_MAP
			// Sample height map
			#ifdef HEIGHT_MAP
				fCurrHeight = TEX2DGRAD( map, vTexCurrentOffset, dx, dy ).r;
			#elif defined(HEIGHT_FROM_NORMAL_MAP_ALPHA)
				fCurrHeight = TEX2DGRAD( map, vTexCurrentOffset, dx, dy ).a;
			#endif
			//#ifdef HEIGHT_MAP
			//	fCurrHeight = TEX2DGRAD( heightMap, vTexCurrentOffset, dx, dy ).r;
			//#elif defined(HEIGHT_FROM_NORMAL_MAP_ALPHA)
			//	fCurrHeight = TEX2DGRAD( normalMap, vTexCurrentOffset, dx, dy ).a;
			//#endif

			fCurrentBound -= fStepSize;

			if ( fCurrHeight > fCurrentBound ) 
			{   
				pt1 = float2( fCurrentBound, fCurrHeight );
				pt2 = float2( fCurrentBound + fStepSize, fPrevHeight );

				texOffset2 = vTexCurrentOffset - vTexOffsetPerStep;

				nStepIndex = nNumSteps + 1;
				fPrevHeight = fCurrHeight;
			}
			else
			{
				nStepIndex++;
				fPrevHeight = fCurrHeight;
			}
		}   

		float fDelta2 = pt2.x - pt2.y;
		float fDelta1 = pt1.x - pt1.y;

		float fDenominator = fDelta2 - fDelta1;

		// SM 3.0 requires a check for divide by zero, since that operation will generate
		// an 'Inf' number instead of 0, as previous models (conveniently) did:
		if ( fDenominator == 0.0f )
		{
			fParallaxAmount = 0.0f;
		}
		else
		{
			fParallaxAmount = (pt1.x * fDelta2 - pt2.x * fDelta1 ) / fDenominator;
		}

		float2 vParallaxOffset = vParallaxOffsetTS * ( 1 - fParallaxAmount );

		return vParallaxOffset;

//		texCoord0 -= vParallaxOffset;
//		texCoord1 -= vParallaxOffset;
//		#ifdef TEXCOORD23
//			texCoord2 -= vParallaxOffset;
//			texCoord3 -= vParallaxOffset;
//		#endif
	}

	return float2(0,0);
}


float4 GetLight(LightParam In,out float4 Diffuse,out float4 Specular)
{
	//float2 TC = getParallaxOcclusionMappingOffset(SamplerNormal,0.05,In.TC,In.v_eye,In.Normal);

	float4 normal = float4(normalize(In.Normal),1);
	float4 specLevel = float4(1,1,1,1);

	if(bActiveNormalMap ){	
		normal = (2 * ( tex2D(SamplerNormal,In.TC )-0.5));
	}
	if(bActiveSpecularMap){
		specLevel = tex2D( SamplerSpecular, In.TC );
	}
	
	float3 eyen   = normalize(In.v_eye);	
	
	float4 Color = float4(1,1,1,1);

	if(lightDir.y<0){
		
		float4 Diff		= max(0,(dot(normal.xyz*1.5, In.MoonLightDir)));
		float3 RefVec	= reflect(-eyen,normal.xyz);
		float4 LumS		= pow(saturate(dot(RefVec, In.MoonLightDir)), In.MatShininess);

		Diffuse		= Diff * I_Md * MoonIntensityCoef + I_Ma;
		Specular	= LumS * I_Ms * In.MatSpecular * specLevel;
			
	}
	if(lightDir.y>0){

		float4 Diff		= max(0,(dot(normal.xyz*1.2, In.LightDirection)));
		float3 RefVec	= reflect(-eyen,normal.xyz);
		float4 LumS		= pow(saturate(dot(RefVec, In.LightDirection)), In.MatShininess);
			
		Diffuse		= (Diff * In.MatDiffuse * I_d + In.MatAmbient * I_a );
		Specular	= LumS * I_s * In.MatSpecular * specLevel;
			
	}
	return Color;
}