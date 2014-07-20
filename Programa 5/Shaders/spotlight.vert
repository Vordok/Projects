varying vec4 diffuse,ambientGlobal,ambient;
varying vec3 normal,lightDir,halfVector;
varying float dist;
	
void main()
{	
	vec4 ecPos;
	vec3 aux;
		
	normal = normalize(gl_NormalMatrix * gl_Normal);
		
	/* these are the new lines of code to compute the light's direction */
	ecPos = gl_ModelViewMatrix * gl_Vertex;
	aux = vec3( gl_LightSource[1].position-ecPos );
	lightDir = normalize(aux);
	dist = length(aux);
	
	halfVector = normalize( gl_LightSource[1].halfVector.xyz );
		
	/* Compute the diffuse, ambient and globalAmbient terms */
	diffuse = gl_LightSource[ 0 ].diffuse;
		
	/* The ambient terms have been separated since one of them */
	/* suffers attenuation */
	ambient = gl_LightSource[ 1 ].ambient;
	ambientGlobal = ambient;
	
	/* Obten coordenadas de textura */
	gl_TexCoord[ 0 ] = gl_MultiTexCoord0;
		
	gl_Position = ftransform();
} 
