//------------------------------------------------------------------------------------------------------------
//									Directional Light
//------------------------------------------------------------------------------------------------------------

int		 NumLights = 0;

float	 AttenCoef[8] ;

float4   g_vLightCol[8] ;
float4   g_vLightPos[8] ;
float4   g_vLightDir[8] ;


float P = (60.0 * (3.14/180.0));
float T = (10.0 * (3.14/180.0));

float	Phi[8]		= {60,60,60,60,60,60,60,60}; // угол в радианах 
float	Theta[8]	= {10,10,10,10,10,10,10,10}; // угол в радианах 

float   ActiveDistance = 250.0f;

// Функция расчета интенсивности света 
// в зависимости от угла альфа 
float4 GetAngleIntensity ( float3 Normal,float4 InterPos) 
{ 
	//float A = 0.005; 
	float B = 0.02; 
	float C = 0.05; 

	float4 tmpColor;
	float4 LightDir;

  for( int i = 0; i < NumLights; ++i )
    {
		float3 PixelToLight = g_vLightPos[i].xyz - InterPos.xyz; 

		if(length(PixelToLight)<ActiveDistance){
 
			float LightAtten = 1/( AttenCoef[i]*pow(length(PixelToLight),2.0) + B*length(PixelToLight) + C );

			float F				 = 1.0; 
			float AngleIntensity = 0.0; 

			PixelToLight = normalize(PixelToLight); 
			LightDir	 = normalize(g_vLightDir[i]);

			float CosAlpha = saturate(dot(g_vLightDir[i].xyz, -PixelToLight)); 
			
			float Ph = Phi[i];
			float Thet = Theta[i];

			if(CosAlpha>cos(Thet/2)) AngleIntensity	= 1.0; 
			else if(CosAlpha<cos(Ph/2)) AngleIntensity = 0.0; 
			else AngleIntensity = saturate(pow((CosAlpha - cos(Ph/2))/(cos(Thet/2) - cos(Ph/2)),F)); 
 
			float4 LightIntensity = g_vLightCol[i]*AngleIntensity*LightAtten; 
	
			tmpColor += (LightIntensity*dot(Normal, PixelToLight)); 
		}
	}

	float4 FinalColor = saturate( max(tmpColor,0) );
	
    FinalColor.a = 1; 

   return FinalColor;   
}
