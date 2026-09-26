attribute	highp	vec2	aPosF2 ;

// レンダーターゲットクリア用頂点シェーダー
void main( void )
{
	highp vec4 lPos ;
	
	lPos.x = aPosF2.x ;
	lPos.y = aPosF2.y ;
	lPos.z = 0.0 ;
	lPos.w = 1.0 ;
	
	gl_Position = lPos ;
}
