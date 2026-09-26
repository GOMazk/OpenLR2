attribute	highp	vec2 aPosF2 ;
attribute	mediump	vec2 aTexUV0 ;		// テクスチャ座標0
attribute	mediump	vec2 aTexUV1 ;		// テクスチャ座標1
attribute	mediump	vec2 aTexUV2 ;		// テクスチャ座標2
attribute	mediump	vec2 aTexUV3 ;		// テクスチャ座標3
attribute	mediump	vec2 aTexUV4 ;		// テクスチャ座標4
attribute	mediump	vec2 aTexUV5 ;		// テクスチャ座標5
attribute	mediump	vec2 aTexUV6 ;		// テクスチャ座標6
attribute	mediump	vec2 aTexUV7 ;		// テクスチャ座標7

varying		mediump	vec2 vTexUV0 ;
varying		mediump	vec2 vTexUV1 ;
varying		mediump	vec2 vTexUV2 ;
varying		mediump	vec2 vTexUV3 ;
varying		mediump	vec2 vTexUV4 ;
varying		mediump	vec2 vTexUV5 ;
varying		mediump	vec2 vTexUV6 ;
varying		mediump	vec2 vTexUV7 ;

// テクスチャ座標8個単純転送用頂点シェーダー
void main( void )
{
	highp vec4 lPos ;
	
	lPos.x = aPosF2.x ;
	lPos.y = aPosF2.y ;
	lPos.z = 0.0 ;
	lPos.w = 1.0 ;
	
	gl_Position = lPos ;

	vTexUV0 = aTexUV0 ;
	vTexUV1 = aTexUV1 ;
	vTexUV2 = aTexUV2 ;
	vTexUV3 = aTexUV3 ;
	vTexUV4 = aTexUV4 ;
	vTexUV5 = aTexUV5 ;
	vTexUV6 = aTexUV6 ;
	vTexUV7 = aTexUV7 ;
}
