varying vec3 normal;
uniform sampler2D Texture1;
	
void main( void )
{
	float intensity;
	vec4 color;
	vec4 out_color;
	vec4 pix;
	vec3 n = normalize(normal);
	
	intensity = dot( vec3( gl_LightSource[0].position ), n );
		
	if ( intensity > 0.95 )
		color = vec4( 1.0, 1.0, 1.0, 1.0 );
	else if ( intensity > 0.6 )
		color = vec4( 0.6, 0.6, 0.6, 1.0 );
	else if ( intensity > 0.3 )
		color = vec4( 0.3, 0.3, 0.3, 1.0 );
	else
		color = vec4( 0.1, 0.1, 0.1, 1.0 );
		
	pix = texture2D( Texture1, gl_TexCoord[ 0 ].xy );
	out_color = color * pix;
		
	gl_FragColor = out_color;
} 
