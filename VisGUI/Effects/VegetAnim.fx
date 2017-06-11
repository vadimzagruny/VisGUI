
// Vegetation Animation
#include"Header.hlsli"

// VertexShader InOut Structs
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


VS_OUTPUT VS( VS_INPUT IN )
{
	VS_OUTPUT OUT;

	float4 windAnimationParameters = float4(-WindVelocity.x*5,WindVelocity.z*5,0.5,0.5);	//xy = windSpeed, zw = waveSpeed
	float4 mainBendingParameters	= float4(0.03,0.005,1,1);	//float4: bendScale, bendVariation, bendFrequency, UNUSED
	float4 detailBendingParameters	= float4(0.01,0.5,1,0.2);	//float4: branchAmplitude, detailAmplitude, branchFrequency, detailFrequency

	float time0X = time/1000;
	float3 objectWorldPosition;
	
	objectWorldPosition = matWorld[3].xyz;

	float3 localPosition = IN.pos.xyz;

	float2 windSpeed = windAnimationParameters.xy;
	float2 waveSpeed = windAnimationParameters.zw;

	//Main Bending

	float bendScale = mainBendingParameters.x;
	float bendVariation = mainBendingParameters.y;
	float bendFrequency = mainBendingParameters.z;
	
	//calculate main bending strength
	float mainBendingPhase = sin(time0X * bendFrequency * length(windSpeed) + dot(objectWorldPosition, 1));
	float variationPhase = (dot(cos(time0X), mainBendingPhase) * .25f + .5f);
	float bendStrength = bendScale + bendVariation * variationPhase * mainBendingPhase;
	
	//calculate vertex distance and bending factor
	float fLength = length(localPosition.xyz);
	float fBF = localPosition.y * bendStrength;
	
	//smooth bending factor and increase its nearby height limit.
	fBF += 1.0;
	fBF *= fBF;
	fBF  = fBF * fBF - fBF;
	
	//displace position
	float3 vNewPos = localPosition;
	vNewPos.xz += windSpeed * fBF;

	//rescale - this keeps the plant parts from "stretching" by shortening the z (height) while they move about the xy.
	localPosition = normalize(vNewPos.xyz) * fLength;

	//Detail Bending

	#ifdef DETAIL_BENDING
		float4 vertColor= tex2D(SamplerDiffuse,texcrds);

		//parameters
		float branchAmplitude	= detailBendingParameters.x;
		float leafAmplitude		= detailBendingParameters.y;
		float branchFrequency	= detailBendingParameters.z;
		float leafFrequency		= detailBendingParameters.w;

		//calculate different phases
		float fTreePhase = dot(IN.pos.xyz, 1);
		float fBranchPhase = vertColor.g + fTreePhase;
		float fLeafPhase = dot(IN.pos.xyz, fBranchPhase);

		//make some waves
		float2 vWavesIn = waveSpeed * (time0X + float2(fLeafPhase * leafFrequency, fBranchPhase * branchFrequency ));
		float4 vWaves = frac( vWavesIn.xxyy * float4(1.975, 0.793, 0.375, 0.193) );

		//apply the triangle wave and smooth function
		float4 triangleWave = abs( frac( vWaves + 0.5 ) * 2.0 - 1.0 );
		vWaves = triangleWave * triangleWave * (3.0 - 2.0 * triangleWave);
		
		//calculate resulting wave strength for two dimensions
		float2 vWavesSum = vWaves.xz + vWaves.yw;  

		localPosition.xyz += vWavesSum.xxy * float3(diffuse.x * leafAmplitude * IN.norm.xy, diffuse.z * branchAmplitude);
	#endif

	float3 worldPosition =  localPosition;

	//-----------------------------------------------------
	OUT.onorm = (mul( IN.norm, matWorld ));
	OUT.oIPos = mul(float4(IN.pos, 1.f), matWorld);
//
	if(bActiveNormalMap != false){
		float3 normal   = OUT.onorm;
		float3 tangent  = mul( IN.tang,  (float3x3) matWorld );
		float3 binormal = mul( IN.bnorm, (float3x3) matWorld );

		normal   = normalize( normal );
		tangent  = normalize( tangent );
		binormal = normalize( binormal );
//
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
	OUT.otexcrd1.x = IN.texcrd1.x;
	OUT.otexcrd1.y = IN.texcrd1.y;
	OUT.otexcrd2 = 0;

	OUT.opos  = mul(float4(worldPosition, 1.f), mul(mul(matWorld, matView),matProj) );
	OUT.oIPWV = mul(float4(IN.pos, 1.f), mul(matWorld,matView));

	return OUT;
}

//--------------------------------------------------------------------------------------
// Pixel Shader
//--------------------------------------------------------------------------------------
void PS( VS_OUTPUT IN, out float4 ocolor:COLOR0)
{
	// Sample the Texture
	float4 tex_color = tex2D( SamplerDiffuse, IN.otexcrd1 );

	//Set Light Param
	LightParam Light;

	Light.v_eye				= IN.oeye[0];
	Light.LightDirection	= IN.oeye[1];
	Light.MoonLightDir		= IN.oeye[2];
	Light.Normal			= IN.onorm;
	Light.MatDiffuse		= Diffuse;
	Light.MatAmbient		= Ambient;
	Light.MatSpecular		= Specular;
	Light.MatShininess		= shininess;
	Light.TC = IN.otexcrd1;
	float4 FogColor;

	float4 Color = float4(1,1,1,1);
	float4 DifColor,SpecColor;
	GetLight(Light,DifColor,SpecColor);

	DifColor += GetAngleIntensity(IN.onorm,IN.oIPos);
	Color  =  tex_color * DifColor + SpecColor;
	Color.w *= tex_color.w;

	float4 cam = IN.oIPWV;
	float  fog = getFogValue(cam.z,fogDensity);

	ocolor		=  ((Color))  ;
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