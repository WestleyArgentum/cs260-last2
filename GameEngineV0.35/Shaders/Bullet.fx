float4x4 WorldViewProj : WorldViewProjection;
float4 color = {1,1,1,1};
uniform extern float fTime : TIME;
float mouse_x;
float mouse_y;
texture texture0;

sampler Sampler0
{
    Texture = texture0;
    MipFilter = Linear;
    MinFilter = Linear;
    MagFilter = Linear;
    AddressU = Clamp;
    AddressV = Clamp;
};


struct VS_INPUT
{
    float3 position	: POSITION;
	float2 tex0 : TEXCOORD0;
};

struct VS_OUTPUT
{
	float4 screenposition : POSITION;
	float2 tex0  : TEXCOORD0;
	float2 tex1  : TEXCOORD1;
};

VS_OUTPUT VertexShader0( VS_INPUT IN )
{
	VS_OUTPUT OUT;
	OUT.screenposition = mul( float4(IN.position, 1) , WorldViewProj);
	//OUT.tex0 = IN.tex0;
	IN.tex0.x -= 0.5f;
	IN.tex0.y -= 0.5f;
	
	OUT.tex0.x =  cos( fTime ) * IN.tex0.x + sin( fTime ) * IN.tex0.y;
	OUT.tex0.y = -sin( fTime ) * IN.tex0.x + cos( fTime ) * IN.tex0.y;
	
	OUT.tex1.y =  cos( fTime ) * IN.tex0.x + -sin( fTime ) * IN.tex0.y;
	OUT.tex1.x =  sin( fTime ) * IN.tex0.x +  cos( fTime ) * IN.tex0.y;
	
	OUT.tex0.x += 0.5f;
	OUT.tex0.y += 0.5f;
	
	OUT.tex1.x += 0.5f;
	OUT.tex1.y += 0.5f;
	
	return OUT;
}

float4 PixelShader0( VS_OUTPUT IN ) : COLOR
{	
	float cos_time = cos( fTime );
	float4 result = tex2D( Sampler0 , IN.tex0 ) * tex2D( Sampler0 , IN.tex1 );
	result; // *= color;
	
	//result.b       += cos_time * 0.3f;
	//result.g       += cos_time * 0.03f;
	//result.r       -= cos_time * 2;
	//result         *= 0.82f;
	
	
	return result;
}

technique Technique0
{
	pass Pass0
	{
		Lighting = FALSE;
		AlphaBlendEnable = TRUE;
		VertexShader = compile vs_2_0 VertexShader0();
		PixelShader  = compile ps_2_0 PixelShader0();
	}
}



