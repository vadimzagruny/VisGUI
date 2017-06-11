//
//	Blend Functions
//
float4 blendOpaque(float4 srcColor, float4 destColor)
{
	return srcColor;
}

float4 blendAdd(float4 srcColor, float4 destColor)
{
	return srcColor + destColor;
}

float4 blendModulate(float4 srcColor, float4 destColor)
{
	return srcColor * destColor;
}

float4 blendColorBlend(float4 srcColor, float4 destColor)
{
	return srcColor * srcColor + destColor * (1 - srcColor);
}

float4 blendAlphaBlend(float4 srcColor, float4 destColor)
{
	return srcColor * srcColor.a + destColor * (1 - srcColor.a);
}

// Terrain Blending

// Blend two Textures
float3 blend2(float4 DiffMap1,float Alpha1, float4 DiffMap2, float Alpha2)
{
	return  DiffMap1.rgb * Alpha1 + DiffMap2.rgb * Alpha2;
}

// Blend Three Textures
float3 blend3(float4 DiffMap1,float Alpha1, float4 DiffMap2, float Alpha2,  float4 DiffMap3, float Alpha3)
{
	return  DiffMap1.rgb * Alpha1 + DiffMap2.rgb * Alpha2 + DiffMap3.rgb * Alpha3;
}

// Blend Four Textures
float3 blend4(float4 DiffMap1,float Alpha1, float4 DiffMap2, float Alpha2,  float4 DiffMap3, float Alpha3,  float4 DiffMap4, float Alpha4)
{
	return  (DiffMap1.rgb * Alpha1 + DiffMap2.rgb * Alpha2 + DiffMap3.rgb * Alpha3 + DiffMap4.rgb * Alpha4)/((Alpha1 + Alpha2 + Alpha3 + Alpha4)*2);
}

// Blend Four Textures
float3 MapBlend4(float4 DiffMap1,float Alpha1, float4 DiffMap2, float Alpha2,  float4 DiffMap3, float Alpha3,  float4 DiffMap4, float Alpha4)
{
	float depth = 0.2;

	float ma = max(max(max(DiffMap1.a + Alpha1,DiffMap2.a + Alpha2),DiffMap3.a + Alpha3),DiffMap4.a + Alpha4) - depth;

	float d1 = max(DiffMap1.a + Alpha1 - ma,0);
	float d2 = max(DiffMap2.a + Alpha2 - ma,0);
	float d3 = max(DiffMap3.a + Alpha3 - ma,0);
	float d4 = max(DiffMap4.a + Alpha4 - ma,0);

	return  (DiffMap1.rgb * d1 + DiffMap2.rgb * d2 + DiffMap3.rgb * d3 + DiffMap4.rgb * d4)/(d1 + d2 + d3 + d4)*0.5;
	

}
