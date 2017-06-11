
//----------------------------------------------------------------
//
// SkyFall Shader
//
// Copyright (c) Vadim Zagruny
//----------------------------------------------------------------
//--------------------------------------------------------------------------------------
// Scene Setup
//--------------------------------------------------------------------------------------
#include"Header.hlsli"

//----------------------------------------------------------------
// Parameters for Sky
//----------------------------------------------------------------

float turbidity  = 4.0;
float fNebulosity = 0;

float SunLightness = 1.2;
float sunRadiusAttenuation = 1096;
float largeSunLightness = 1.0;
float largeSunRadiusAttenuation = 1024;

struct VS_IN 
{
   float4 Position  : POSITION0;
   float2 Tex		: TEXCOORD0;
   float2 Tex1		: TEXCOORD1;
};

struct VS_OUT 
{
   float4 Position  : POSITION0;
   float3 l			: TEXCOORD3;
   float3 v			: TEXCOORD4;
   float3 Color		: TEXCOORD5;
   float2 Tex		: TEXCOORD0;
   float2 Tex1		: TEXCOORD1;
   float4 PWV       : COLOR1;
};


float3 perezZenith ( float t, float thetaSun )
{
  float  pi = 3.1415926;
  float4  cx1 = float4( 0,       0.00209, -0.00375, 0.00165  );
  float4  cx2 = float4( 0.00394, -0.03202, 0.06377, -0.02903 );
  float4  cx3 = float4( 0.25886, 0.06052, -0.21196, 0.11693  );
  float4  cy1 = float4( 0.0,     0.00317, -0.00610, 0.00275  );
  float4  cy2 = float4( 0.00516, -0.04153, 0.08970, -0.04214 );
  float4  cy3 = float4( 0.26688, 0.06670, -0.26756, 0.15346  );

  float		t2    = t*t;
  float		chi   = (4.0 / 9.0 - t / 120.0 ) * (pi - 2.0 * thetaSun );
  float4	theta = float4( 1, thetaSun, thetaSun*thetaSun, thetaSun*thetaSun*thetaSun );

  float  Y = (4.0453 * t - 4.9710) * tan ( chi ) - 0.2155 * t + 2.4192;
  float  x = t2 * dot ( cx1, theta ) + t * dot ( cx2, theta ) + dot ( cx3, theta );
  float  y = t2 * dot ( cy1, theta ) + t * dot ( cy2, theta ) + dot ( cy3, theta );

  return float3( Y, x, y );
}


float3 perezFunc ( float t, float cosTheta, float cosGamma )
{
    float  gamma      = acos ( cosGamma );
    float  cosGammaSq = cosGamma * cosGamma;
    float  aY =  0.17872 * t - 1.46303;
    float  bY = -0.35540 * t + 0.42749;
    float  cY = -0.02266 * t + 5.32505;
    float  dY =  0.12064 * t - 2.57705;
    float  eY = -0.06696 * t + 0.37027;
    float  ax = -0.01925 * t - 0.25922;
    float  bx = -0.06651 * t + 0.00081;
    float  cx = -0.00041 * t + 0.21247;
    float  dx = -0.06409 * t - 0.89887;
    float  ex = -0.00325 * t + 0.04517;
    float  ay = -0.01669 * t - 0.26078;
    float  by = -0.09495 * t + 0.00921;
    float  cy = -0.00792 * t + 0.21023;
    float  dy = -0.04405 * t - 1.65369;
    float  ey = -0.01092 * t + 0.05291;

    return float3 ( (1.0 + aY * exp(bY/cosTheta)) * (1.0 + cY * exp(dY * gamma) + eY*cosGammaSq),
                    (1.0 + ax * exp(bx/cosTheta)) * (1.0 + cx * exp(dx * gamma) + ex*cosGammaSq),
                    (1.0 + ay * exp(by/cosTheta)) * (1.0 + cy * exp(dy * gamma) + ey*cosGammaSq) );
}

float3 perezSky ( float t, float cosTheta, float cosGamma, float cosThetaSun )
{
	cosThetaSun += 0.5;//рассвет перед выходом солнца (радиус освещённости неба)

    float thetaSun	= acos( cosThetaSun );
    float3 zenith   = perezZenith ( t, thetaSun );
    float3 clrYxy   = zenith * perezFunc ( t, cosTheta, cosGamma ) / perezFunc ( t, 1.0, cosThetaSun );
   
    clrYxy [0] *= smoothstep ( 0.0, 0.1, cosThetaSun );      // make sure when thetaSun > PI/2 we have black color
  
    return clrYxy;
}

float3 convertColor (float3 colorYxy)
{
    float3 clrYxy = float3( colorYxy );
    
	 // now rescale Y component

	clrYxy [0] = 1.0 - exp ( -clrYxy [0] / turbidity );

    float ratio = clrYxy [0] / clrYxy [2];  // Y / y = X + Y + Z
    float3 XYZ;

    XYZ.x = clrYxy [1] * ratio;             // X = x * ratio
    XYZ.y = clrYxy [0];                     // Y = Y
    XYZ.z = ratio - XYZ.x - XYZ.y;          // Z = ratio - X - Y

    float3 rCoeffs = float3 ( 3.240479, -1.53715, -0.49853  );
    float3 gCoeffs = float3 ( -0.969256, 1.875991, 0.041556 );
    float3 bCoeffs = float3 ( 0.055684, -0.204043, 1.057311 );

    return float3 ( dot ( rCoeffs, XYZ ), dot ( gCoeffs, XYZ ), dot ( bCoeffs, XYZ ) );
}

float3 Test(float3 inp)
{
	return float3(inp.x * 2,inp.y * 2, inp.z * 2);
}


float2	clouds (float t, float2 vel , float2 Tex)
{
    float2 	dt   = float2 ( vel.x, 0.061 ) * t;
    float2 	tex1 = Tex.xy + dt; 
	float2	n1   = tex2D ( SamplerAmbient, tex1       ).xy / 2.0;
	float2	n2   = tex2D ( SamplerAmbient, tex1 * 2.0 ).xy / 4.0;
	float2	n3   = tex2D ( SamplerAmbient, tex1 * 4.0 ).xy / 8.0;
	float2	n4   = tex2D ( SamplerAmbient, tex1 * 8.0 ).xy / 16.0;
	
	return (n1 + n2 + n3 + n4) / (0.5 + 0.25 + 0.125 + 0.0625 );		// return normalized sum
}

float	density ( float v, float d  )
{
	return clamp ( (1.0 + d)*pow ( v, 4.0 ) - 0.3, 0.0, 1.0 );
}



//--------------------------------------------------------------------------------------
// Vertex Shader
//--------------------------------------------------------------------------------------
VS_OUT vs_main( VS_IN Input )
{
   VS_OUT Output;

   float3 WorldPos = mul(Input.Position,matWorld);

   float3 l = float3(lightDir.x,lightDir.z,lightDir.y);
   float3 v = float3(WorldPos.x,WorldPos.z,WorldPos.y) - matViewInv[3].xzy;

   Output.v		=  matViewInv[3].xzy - float3(WorldPos.x,WorldPos.z,WorldPos.y) ;
   Output.l		= l;
   Output.Tex	= Input.Tex;
   Output.Tex1	= Input.Tex1;
   l			= normalize(l);
   v			= normalize(v);
   float3 Col	= perezSky ( turbidity, max ( v.z, 0.0 ) + 0.05, dot (l, v ), l.z );

   Output.Color		= Col;
   Output.Position  = mul( Input.Position,  mul(mul( matWorld, matView ),matProj) );
   Output.PWV		= mul( Input.Position,  mul(matWorld,matView));
   return( Output );
}


struct PS_INPUT 
{
	float3 v		: TEXCOORD4;
	float3 l		: TEXCOORD3;
    float3 ColorYxy : TEXCOORD5;
	float2 Tex		: TEXCOORD0;
	float2 Tex1		: TEXCOORD1;
	float4 IPWV     : COLOR1;
};

//--------------------------------------------------------------------------------------
// Pixel Shader
//--------------------------------------------------------------------------------------
float4 ps_main( PS_INPUT Input ) : COLOR0
{  

	Input.l = normalize(Input.l);
	Input.v = normalize(Input.v);

	// Само солнце 
	float sunHighlight = 0.3*pow(max(0, dot(Input.l, -Input.v)), sunRadiusAttenuation) * SunLightness;         
	// Подсветка солнцем атмосферы – ореол света вокруг диска
	float largeSunHighlight = pow(max(0,dot(Input.l, -Input.v)), largeSunRadiusAttenuation) * largeSunLightness;

	sunHighlight += largeSunHighlight;

	float3 convColor = convertColor(Input.ColorYxy);

	float4 Color = float4 ( clamp ( convColor, 0.0, 1.0 ), 1.0 );

	float4 OutColor = 0;

	if(Input.l.z<0.00){ OutColor = sunHighlight + Color + tex2D(SamplerDiffuse,Input.Tex);}
	if(Input.l.z>0.00){ OutColor = sunHighlight + Color;}

	//  SET CLOUDS

	float3 ResCol = 1;
	if (Input.l.z<=  0.5) ResCol=lerp(float3(1.0,0.7,0.7),float3(1.0,1.0,1.0),Input.l.z*(1/0.5));
	if (Input.l.z<= -0.45) {ResCol= OutColor.rgb + float3(0.1,0.1,0.1);}

	float2	n1  = clouds ( time/20000, float2 ( WindVelocity.x, WindVelocity.z ),Input.Tex1); 
	
	//ResCol *= lerp(float3(0.8,0.8,0.8),float3(0.7,0.7,0.7),fNebulosity);

	float4 cam = Input.IPWV;
	float  fog = getFogValue(cam.z/20,fogDensity);
	
	OutColor =  OutColor + density ( n1.x, fNebulosity ) * float4(ResCol,1.5) ;
	
	OutColor.xyz *= fog ;
	OutColor.xyz += fogColor * (1.0h - fog);
	return  OutColor;

}

//--------------------------------------------------------------------------------------
// Perez_Sky Technique
//--------------------------------------------------------------------------------------
technique Tec0
{
   pass Pass_0
   {
	  ZEnable          = TRUE;
	  ZWriteEnable     = TRUE;
	  CullMode         = ccw;

      VertexShader = compile vs_3_0 vs_main();
      PixelShader  = compile ps_3_0 ps_main();
   }

}