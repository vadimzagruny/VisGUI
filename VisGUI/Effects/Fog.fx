//--------------------------------------------------------------------------------------
//
//--------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------
// Scene Setup
//--------------------------------------------------------------------------------------

#define FOG_EXP

 float4 fogColor  = float4(0.8,0.8,0.8,0.0);
 float4 fogParams = float4(0.005,1,1,1);
float fogDensity = 0.005;

float getFogValue(float eyeDistance, float Density)
{
	float fog;

	#ifdef FOG_LINEAR
		fog = ( fogParams.z - eyeDistance ) * fogParams.w; // / ( fogParams.z - fogParams.y );
	#endif

	#ifdef FOG_EXP

		//float fogDensity = fogParams.x;
		fog = 1.0f / exp( ((eyeDistance) * Density ) * log( 2.718281828f ) );
	#endif

	#ifdef FOG_EXP2
		float fogDensity = fogParams.x;
		float m = eyeDistance * Density;
		fog = 1.0f / exp( ( m * m ) * log( 2.718281828f ) );
	#endif

	return /*1 - */saturate(fog );
}

