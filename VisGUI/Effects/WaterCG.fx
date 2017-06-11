// Copyright (C) Vadim Zagruny
#include"Header.hlsli"

//float3 expand(float3 v)
float3 expand(float3 v)
{
	return v * 2 - 1;
}

#define REFLECTION_MAP
///////////////////////////////////////////////////////////////////////////////////////////////////

struct VS_IN
{
	float4 position : POSITION;   // in object space
	//float2 texCoord : TEXCOORD0;
};

struct v2f
{
	float4 position  : POSITION;  // in clip space

	float3 rotMatrix1 : TEXCOORD0; // first row of the 3x3 transform from tangent to obj space
	float3 rotMatrix2 : TEXCOORD1; // second row of the 3x3 transform from tangent to obj space
	float3 rotMatrix3 : TEXCOORD2; // third row of the 3x3 transform from tangent to obj space

	float3 bumpCoord0 : TEXCOORD3;
	float3 bumpCoord1 : TEXCOORD4;

	float4 eyeVectorAndCameraDistance : TEXCOORD6;
	
#ifdef REFLECTION_MAP
	float4 projectionCoord : TEXCOORD7;
#endif

};


	float	bumpScale			= 0.7;					// amount bump on waves
	float2	textureScale		= float2(2, 2);			// scaling of normal texture
	float2	bumpSpeed			= float2(0.001, 0.005);	// speed of normalmap waves
	float2	waveDirection0		= float2(-1, 0);		// direction of wave 0
	float2	waveDirection1		= float2(-0.7, 0.7);	// direction of wave 1
	float	waveFrequency		= 0.2;

	float waveAmplitude <
		string UIName = "waveAmplitude";
		> = 0.0f;		//0.3;


//wave functions
struct Wave {
  float freq;  // 2*PI / wavelength
  float amp;   // amplitude
  float phase; // speed * 2*PI / wavelength
  float2 dir;
};

v2f main_vp(VS_IN IN)
{
	float		time0X = time /1000;

	float4		cameraPositionObjectSpace = matViewInv[3] ;

	float4x4	worldViewProjMatrix = mul(mul(matWorld,matView),matProj);
	float4x4	worldViewMatrix		= mul(matWorld,matView);

	

	v2f OUT;

	#define WAVE_COUNT 2
	Wave wave[WAVE_COUNT] =
	{
		{ 1.0, 1.0, 0.5, waveDirection0 },
		{ 2.0, 0.5, 1.7, waveDirection1 }
	};

	wave[0].freq = waveFrequency;
	wave[0].amp  = waveAmplitude;
	wave[1].freq = waveFrequency * 3.0;
	wave[1].amp  = waveAmplitude * 0.33;

	float4 modifiedPosition = IN.position;

	float2 texCoord = IN.position.xz / 1000.0;

	// sum waves
	float ddx = 0.0, ddy = 0.0;
	float deriv;
	float angle;

	// wave synthesis using two sine waves at different frequencies and phase shift
	for(int i = 0; i < WAVE_COUNT; i++)
	{
		angle = dot(wave[i].dir, modifiedPosition.xy) * wave[i].freq + time0X * wave[i].phase;
		modifiedPosition.y += wave[i].amp * sin(angle);
		// calculate derivate of wave function
		deriv = wave[i].freq * wave[i].amp * cos(angle);
		ddx -= deriv * wave[i].dir.x;
		ddy -= deriv * wave[i].dir.y;
	}

	// compute the 3x3 tranform from tangent space to object space
	// first rows are the tangent and binormal scaled by the bump scale
	OUT.rotMatrix1.xyz = bumpScale * normalize(float3(0, ddx, 1));	// Tangent
	OUT.rotMatrix2.xyz = bumpScale * normalize(float3(1, ddy, 0));	// Binormal 
	OUT.rotMatrix3.xyz = normalize(float3(ddx, 1, ddy));			// Normal

	OUT.position = mul(modifiedPosition,worldViewProjMatrix);

	// calculate texture coordinates for normal map lookup
	OUT.bumpCoord0.xy = texCoord * textureScale + time0X * bumpSpeed;
	//invert bumpSpeed for opposite wave directions
	OUT.bumpCoord1.xy = texCoord * textureScale * 2.0 + time0X * (-bumpSpeed) * 2.0;

	OUT.eyeVectorAndCameraDistance =   IN.position - cameraPositionObjectSpace;// eye position in vertex space
	OUT.eyeVectorAndCameraDistance.w = mul(IN.position,worldViewMatrix).z;

	OUT.bumpCoord0.z = IN.position.x;
	OUT.bumpCoord1.z = IN.position.y;

#ifdef REFLECTION_MAP

	float4 projectionCoord = OUT.position;
	projectionCoord.xy /= projectionCoord.w;
		
	projectionCoord.xy *= float2(.5,.5);
	projectionCoord.xy += float2(.5,.5);
	
	OUT.projectionCoord = float4(projectionCoord.x * projectionCoord.w,projectionCoord.y * projectionCoord.w, projectionCoord.z, projectionCoord.w);
		
#endif


	return OUT;
}

///////////////////////////////////////////////////////////////////////////////////////////////////

float4 deepColor <
	string UIName = "deepColor";
	> = float4(0.3,0.42,0.5,0.2);

float4 shallowColor		 <
	string UIName = "shallowColor";
	> = float4(0.3,0.52,0.61,0.2); 

float4 reflectionColor	 <
	string UIName = "reflectionColor";
	> = float4(0.7f,0.7f,0.7f,0.5f); 

void main_fp(
			 in v2f IN,
			 out float4 oColor : COLOR
			)
{
	float reflectionAmount = 1;	// reflection factor
	float waterAmount = 0.6;	// "Amount of water color"
	float fresnelPower = 2;		// fresnel fresnel effect
	float fresnelBias = 0.328;	// fresnel coefficient

	// sum 3 normals
	float3 t0 = expand(tex2D(SamplerRefraction, IN.bumpCoord0.xy).xyz);
	float3 t1 = expand(tex2D(SamplerRefraction, IN.bumpCoord1.xy).xyz);
	
	float3 normal = t0 + t1;

	float3x3 m; // tangent to world matrix
	m[0] = IN.rotMatrix1;
	m[1] = IN.rotMatrix2;
	m[2] = IN.rotMatrix3;

	normal = normalize( mul( normal, m ) );

	normal = float3( normal.x, normal.z, normal.y ); // flip z and y 

	float3 eyeVector = normalize(IN.eyeVectorAndCameraDistance.xyz);
	float cameraDistance = IN.eyeVectorAndCameraDistance.w;

	//fresnel coefficient
	float facing = 1.0 - max(dot(eyeVector, normal), 0);
	float fresnel = saturate(fresnelBias + pow(facing, fresnelPower));

	float4 waterColor = lerp(shallowColor, deepColor, (float)facing) * waterAmount;
	float4 reflection = float4(1,1,1,1);

#ifdef REFLECTION_MAP
	IN.projectionCoord.xy += normal.xy;
	reflection = (float4)tex2Dproj(SamplerReflection, IN.projectionCoord);
#endif

#ifdef FRESNEL_POWER
	reflection = lerp(waterColor, reflection * reflectionColor, (float)fresnel) * reflectionAmount;
	float4 resultColor =  waterColor + reflection;
#else
	float4 resultColor = lerp( waterColor , reflection,0.85);
#endif

	resultColor.w = 0.8;
//  Fog Calculate
	float4 FogColor;
	float  fog = getFogValue(cameraDistance,fogDensity);

	resultColor.xyz *= fog;
	resultColor.xyz += fogColor * (1.0h - fog);

	oColor = resultColor;

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

		VertexShader = compile vs_3_0 main_vp();
		PixelShader  = compile ps_3_0 main_fp();

    }
}