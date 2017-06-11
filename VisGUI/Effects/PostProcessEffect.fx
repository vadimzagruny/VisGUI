texture offscreenTexture;
bool IsBWActivated;
bool IsInverseActivated;
bool IsEDGEEnchancement;
float Brightness;

sampler2D PostProcessSampler = sampler_state
{
	Texture   = <offscreenTexture>;
	MinFilter = Linear;
	MagFilter = Linear;
	AddressU = Clamp;
	AddressV = Clamp;
};

struct PS_INPUT
{
	float4 color : COLOR;
	float2 uv0 : TEXCOORD0;
};

struct PS_OUTPUT
{
	float4 Color   : COLOR0;
};


//float3 LuminanceConv = { 0.2125f, 0.7154f, 0.0721f };
float3 LuminanceConv = {0.3, 0.59, 0.11};
float2 Shift = {0.002, -0.002};
//int pos=0;
PS_OUTPUT PostProcessPS(in PS_INPUT input)
{
	PS_OUTPUT output = (PS_OUTPUT)0;
	float4 color;
	//float p=float(input.uv0[0]) % 0.01;
	float rnd =0;//0.01-0.01*p;//0.0;//
	
	
	if (IsBWActivated)
	{
	    color = dot( (float3)tex2D( PostProcessSampler , input.uv0), LuminanceConv) +rnd; 
		if(IsEDGEEnchancement)
			color += - 0.2* dot( (float3)tex2D( PostProcessSampler , input.uv0 + Shift), LuminanceConv);
	}
	else
		color = tex2D( PostProcessSampler, input.uv0 );

	if (IsInverseActivated)
		output.Color = 1-color;
	else
		output.Color = color;

	output.Color.rgb += Brightness + rnd ;
		
	return output;
}



//-----------------------------------------------------------------------------

technique PostProcessT
{
    pass p0
    {
        PixelShader = compile ps_2_0 PostProcessPS();
        //ZEnable = false;
    }
}
