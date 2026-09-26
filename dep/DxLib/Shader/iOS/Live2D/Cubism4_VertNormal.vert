attribute highp   vec3 aPos;
attribute mediump vec2 aTexUV0;
varying   mediump vec2 v_texCoord;
uniform   highp   vec4 u_matrix[ 4 ];

void main( void )
{
	highp vec4 aPos4;
	aPos4.x = aPos.x;
	aPos4.y = aPos.y;
	aPos4.z = aPos.z;
	aPos4.w = 1.0;
	gl_Position.x = dot( aPos4, u_matrix[ 0 ] );
	gl_Position.y = - dot( aPos4, u_matrix[ 1 ] );
	gl_Position.z = dot( aPos4, u_matrix[ 2 ] );
	gl_Position.w = dot( aPos4, u_matrix[ 3 ] );
	v_texCoord = aTexUV0;
	v_texCoord.y = 1.0 - v_texCoord.y;
}
