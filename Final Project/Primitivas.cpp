#include "Primitivas.h"
#include "3ds.h"

CTga text[ 15 ];

void CPrimitivas::Normalize( float X1[3], float X2[3], float X3[3] )
{
	CVector v1, v2, N;
	v1.x = X2[ 0 ] - X1[ 0 ];
	v1.y = X2[ 1 ] - X1[ 1 ];
	v1.z = X2[ 2 ] - X1[ 2 ];

	v2.x = X3[ 0 ] - X1[ 0 ];
	v2.y = X3[ 1 ] - X1[ 1 ];
	v2.z = X3[ 2 ] - X1[ 2 ];

	N = Normaliza( Cruz (v1, v2) );
	glNormal3f( N.x, N.y, N.z );
}

CPrimitivas::CPrimitivas()
{
}

CPrimitivas::~CPrimitivas()
{
}

void CPrimitivas::CargaTexturas()
{
	text[ 0 ].LoadTGA("Texturas/rojo.tga");
	text[ 1 ].LoadTGA("Texturas/fuego.tga");
	text[ 2 ].LoadTGA("Texturas/martillo.tga");
	text[ 3 ].LoadTGA("Texturas/negro.tga");
	text[ 4 ].LoadTGA("Texturas/paredbodega.tga");
	text[ 5 ].LoadTGA("Texturas/puertabodega.tga");
	text[ 6 ].LoadTGA("Texturas/regaloamarillo.tga");
	text[ 7 ].LoadTGA("Texturas/regalorojo.tga");
	text[ 8 ].LoadTGA("Texturas/escenario.tga");
	text[ 11 ].LoadTGA("Texturas/cielo3.tga");
	text[ 9 ].LoadTGA("Texturas/puerta2.tga");
	text[ 10 ].LoadTGA("Texturas/puerta.tga");
	text[ 12 ].LoadTGA("Texturas/edificiomario.tga");
	text[ 13 ].LoadTGA("Texturas/edificio2.tga");
	text[ 14 ].LoadTGA("Texturas/techito.tga");

	
	
}

void CPrimitivas::DescargaTexturas()
{
	text[ 0 ].Elimina();
	text[ 1 ].Elimina();
	text[ 2 ].Elimina();
	text[ 3 ].Elimina();
	text[ 4 ].Elimina();
	text[ 5 ].Elimina();
	text[ 6 ].Elimina();
	text[ 7 ].Elimina();
	text[ 8 ].Elimina();
	text[ 9 ].Elimina();
	text[ 10 ].Elimina();
	text[ 11 ].Elimina();
	text[ 12 ].Elimina();
	text[ 13 ].Elimina();
	text[ 14 ].Elimina();
}

void CPrimitivas::glDibujaCubo(float w, float h, float l)
{
	w=w/2;
	h=h/2;
	l=l/2;

	glBegin(GL_QUADS);									
		//arriba
		glNormal3f(0.0f,1.0f,0.0f);
		glTexCoord2f( 0.0f, 0.0f ); glVertex3f( w, h,-l );					
		glTexCoord2f( 1.0f, 0.0f ); glVertex3f( -w, h,-l );					
		glTexCoord2f( 1.0f, 1.0f ); glVertex3f( -w, h, l );					
		glTexCoord2f( 0.0f, 1.0f ); glVertex3f( w, h, l );					
		//abajo
		glNormal3f(0.0f,-1.0f,0.0f);
		glTexCoord2f( 0.0f, 0.0f ); glVertex3f( w,-h, l);					
		glTexCoord2f( 1.0f, 0.0f ); glVertex3f(-w,-h, l);					
		glTexCoord2f( 1.0f, 1.0f ); glVertex3f(-w,-h,-l);					
		glTexCoord2f( 0.0f, 1.0f ); glVertex3f( w,-h,-l);					
		//frontal
		glNormal3f(0.0f,0.0f,1.0f);
		glTexCoord2f( 0.0f, 0.0f ); glVertex3f( w, h, l);					
		glTexCoord2f( 1.0f, 0.0f ); glVertex3f(-w, h, l);					
		glTexCoord2f( 1.0f, 1.0f ); glVertex3f(-w,-h, l);					
		glTexCoord2f( 0.0f, 1.0f ); glVertex3f( w,-h, l);					
		//atras
		glNormal3f(0.0f,0.0f,-1.0f);
		glTexCoord2f( 1.0f, 1.0f ); glVertex3f( w,-h,-l);					
		glTexCoord2f( 0.0f, 1.0f ); glVertex3f(-w,-h,-l);					
		glTexCoord2f( 0.0f, 0.0f ); glVertex3f(-w, h,-l);					
		glTexCoord2f( 1.0f, 0.0f ); glVertex3f( w, h,-l);					
		//izquierda
		glNormal3f(-1.0f,0.0f,0.0f);
		glTexCoord2f( 0.0f, 0.0f ); glVertex3f(-w, h, l);					
		glTexCoord2f( 1.0f, 0.0f ); glVertex3f(-w, h,-l);					
		glTexCoord2f( 1.0f, 1.0f ); glVertex3f(-w,-h,-l);					
		glTexCoord2f( 0.0f, 1.0f ); glVertex3f(-w,-h, l);					
		//derecha
		glNormal3f(1.0f,0.0f,0.0f);
		glTexCoord2f( 0.0f, 0.0f ); glVertex3f( w, h,-l);					
		glTexCoord2f( 1.0f, 0.0f ); glVertex3f( w, h, l);					
		glTexCoord2f( 1.0f, 1.0f ); glVertex3f( w,-h, l);					
		glTexCoord2f( 0.0f, 1.0f ); glVertex3f( w,-h,-l);					
	glEnd();											
}

void CPrimitivas::glDibujaPiramide()
{
	glBegin(GL_TRIANGLES);								// Start Drawing A Triangle
		glColor3f(0.8f,0.5f,0.2f);						// Red
		glVertex3f( 0.0f, 1.0f, 0.0f);					// Top Of Triangle (Front)
		glColor3f(0.9f,0.5f,0.1f);						// Green
		glVertex3f(-1.0f,-1.0f, 1.0f);					// Left Of Triangle (Front)
		glColor3f(0.7f,0.5f,0.3f);						// Blue
		glVertex3f( 1.0f,-1.0f, 1.0f);					// Right Of Triangle (Front)
		glColor3f(0.8f,0.5f,0.2f);						// Red
		glVertex3f( 0.0f, 1.0f, 0.0f);					// Top Of Triangle (Right)
		glColor3f(0.9f,0.5f,0.1f);						// Blue
		glVertex3f( 1.0f,-1.0f, 1.0f);					// Left Of Triangle (Right)
		glColor3f(0.8f,0.5f,0.2f);						// Green
		glVertex3f( 1.0f,-1.0f, -1.0f);					// Right Of Triangle (Right)
		glColor3f(1.0f,0.0f,0.0f);						// Red
		glVertex3f( 0.0f, 1.0f, 0.0f);					// Top Of Triangle (Back)
		glColor3f(0.0f,1.0f,0.0f);						// Green
		glVertex3f( 1.0f,-1.0f, -1.0f);					// Left Of Triangle (Back)
		glColor3f(0.0f,0.0f,1.0f);						// Blue
		glVertex3f(-1.0f,-1.0f, -1.0f);					// Right Of Triangle (Back)
		glColor3f(1.0f,0.0f,0.0f);						// Red
		glVertex3f( 0.0f, 1.0f, 0.0f);					// Top Of Triangle (Left)
		glColor3f(0.0f,0.0f,1.0f);						// Blue
		glVertex3f(-1.0f,-1.0f,-1.0f);					// Left Of Triangle (Left)
		glColor3f(0.0f,1.0f,0.0f);						// Green
		glVertex3f(-1.0f,-1.0f, 1.0f);					// Right Of Triangle (Left)
	glEnd();											
}

void CPrimitivas::glDibujaCono(float r)
{
	glq= gluNewQuadric();
	glPushMatrix();
		glRotatef(-90.0f,1.0f,0.0f,0.0f);
		gluQuadricOrientation(glq,GLU_OUTSIDE);
		gluQuadricNormals(glq,GLU_SMOOTH);
		gluCylinder(glq,r,0.1f,3.0f,24,1);
		glTranslatef(0.0f,0.0f,3.0f);
		gluDisk(glq,0.0f,0.1f,24,1);
	glPopMatrix();
}

void CPrimitivas::glDibujaEsfera(float r)
{
	glq= gluNewQuadric();
	gluQuadricNormals(glq,GLU_SMOOTH);
	gluSphere(glq,r,24,24);
}

void CPrimitivas::glDibujaCilindro(float r, float ri, float h)
{
	glq= gluNewQuadric();
	//gluQuadricDrawStyle(glq, GL_QUADS); //Alambrado
	glPushMatrix();
		glTranslatef(0.0f,-h/2,0.0f);
		glRotatef(-90.0f,1.0f,0.0f,0.0f);
		gluQuadricNormals(glq,GLU_SMOOTH);
		gluQuadricOrientation(glq, GLU_OUTSIDE);
		gluCylinder(glq,r,r,h,24,1);
		if(ri > 0.0f)
		{
			gluQuadricOrientation(glq, GLU_INSIDE);
			gluCylinder(glq,ri,ri,h,24,1);
			gluQuadricOrientation(glq, GLU_OUTSIDE);
		}
		glTranslatef(0.0f,0.0f,h);
		gluDisk(glq,ri,r,24,1);
		glRotatef(-180.0f,1.0f,0.0f,0.0f);
		glTranslatef(0.0f,0.0f,h);
		gluDisk(glq,ri,r,24,1);
	glPopMatrix();
}

void CPrimitivas::DibujaDomoCielo(float radio, int paralelos, int meridianos)
{
	float ang1, ang2;
	float a[ 3 ], b[ 3 ], c[ 3 ], d[ 3 ];
	float delta1, delta2;
	float skyColor = 0.7f;
	// Texturas
	float s1 = 0.0f, s2 = 0.0f, t1 = 0.0f, 
		  t2 = 0.0f, deltas = 0.0f, deltat = 0.0f;
	deltas = 1.0f / meridianos;
	deltat = 1.0f / ( paralelos * 0.5f );
	
	delta1 = 180.0f / paralelos;
	delta2 = 360.0f / meridianos;

	// Habilitando texturas
	glEnable( GL_TEXTURE_2D );
	glBindTexture(GL_TEXTURE_2D, text[11].texID);

	for( int i = 0; i < paralelos / 2; i++ )
	{
		for( int j = 0; j < meridianos ; j++ )
		{

		
			t1 = deltat * i;
			t2 = deltat * ( i + 1 );
			s1 = deltas * j;
			s2 = deltas * ( j + 1 );

			ang1 = i * delta1; //inc paralelos
			ang2 = j * delta2; //inc meridianos
			
			a[ 0 ] = radio * cosf( ang1 * PI / 180.0f ) * cosf( ang2 * PI / 180.0f );
			a[ 1 ] = radio * sinf( ang1 * PI / 180.0f );
			a[ 2 ] = radio * cosf( ang1 * PI / 180.0f ) * sinf( ang2 * PI / 180.0f );

			ang1 = ( i + 1 ) * delta1;

			b[ 0 ] = radio * cosf( ang1 * PI / 180.0f ) * cosf( ang2 * PI / 180.0f );
			b[ 1 ] = radio * sinf( ang1 * PI / 180.0f );
			b[ 2 ] = radio * cosf( ang1 * PI / 180.0f ) * sinf( ang2 * PI / 180.0f );

			ang2 = ( j + 1 ) * delta2;

			c[ 0 ] = radio * cosf( ang1 * PI / 180.0f ) * cosf( ang2 * PI / 180.0f );
			c[ 1 ] = radio * sinf( ang1 * PI / 180.0f );
			c[ 2 ] = radio * cosf( ang1 * PI / 180.0f ) * sinf( ang2 * PI / 180.0f );

			ang1 = i * delta1;

			d[ 0 ] = radio * cosf( ang1 * PI / 180.0f ) * cosf( ang2 * PI / 180.0f );
			d[ 1 ] = radio * sinf( ang1 * PI / 180.0f );
			d[ 2 ] = radio * cosf( ang1 * PI / 180.0f ) * sinf( ang2 * PI / 180.0f );

			//glColor3f(0.0f, 0.5f, skyColor );
			glBegin( GL_QUADS );
				glTexCoord2f( s1 * 2.0f, t2 ); glVertex3f( b[ 0 ] , b[ 1 ], b[ 2 ] );
				glTexCoord2f( s1 * 2.0f, t1 ); glVertex3f( a[ 0 ] , a[ 1 ], a[ 2 ] );
				glTexCoord2f( s2 * 2.0f, t1 ); glVertex3f( d[ 0 ] , d[ 1 ], d[ 2 ] );
				glTexCoord2f( s2 * 2.0f, t2 ); glVertex3f( c[ 0 ] , c[ 1 ], c[ 2 ] );
			glEnd();

			/*
			if(skyColor < 1.0f)
				skyColor += 0.0004f;
			else
				skyColor = 0.7f;
			*/
		}
	}
	glDisable( GL_TEXTURE_2D );
}
void CPrimitivas::glDibujaCubo2(float w, float h, float l)
{
	w=w/2;
	h=h/2;
	l=l/2;

	glBegin(GL_QUADS);									
		//arriba
		glNormal3f( 0.0f, 1.0f, 0.0f);
		glTexCoord2f( 0.0f, 0.0f ); glVertex3f( w, h,-l); // 4
		glTexCoord2f( 1.0f, 0.0f ); glVertex3f(-w, h,-l); // 3
		glTexCoord2f( 1.0f, 1.0f ); glVertex3f(-w, h, l); // 2
		glTexCoord2f( 0.0f, 1.0f ); glVertex3f( w, h, l); // 1	
		//abajo
		glNormal3f( 0.0f, -1.0f, 0.0f);
		glVertex3f( w,-h, l); // 4
		glVertex3f(-w,-h, l); // 3
		glVertex3f(-w,-h,-l); // 2
		glVertex3f( w,-h,-l); // 1	
		//frontal
		glNormal3f( 0.0f, 0.0f, 1.0f);
		glTexCoord2f( 0.0f, 0.0f ); glVertex3f( w, h, l); // 4
		glTexCoord2f( 15.0f, 0.0f ); glVertex3f(-w, h, l); // 3
		glTexCoord2f( 15.0f, 1.0f ); glVertex3f(-w,-h, l); // 2
		glTexCoord2f( 0.0f, 1.0f ); glVertex3f( w,-h, l); // 1		
		//atras
		glNormal3f( 0.0f, 0.0f, -1.0f);
		glTexCoord2f( 0.0f, 0.0f ); glVertex3f( w,-h,-l); // 4	
		glTexCoord2f( 15.0f, 0.0f ); glVertex3f(-w,-h,-l); // 3
		glTexCoord2f( 15.0f, 1.0f ); glVertex3f(-w, h,-l); // 2
		glTexCoord2f( 0.0f, 1.0f ); glVertex3f( w, h,-l); // 1															
		//izquierda
		glNormal3f( -1.0f, 0.0f, 0.0f);
		glTexCoord2f( 0.0f, 0.0f ); glVertex3f(-w, h, l); // 4
		glTexCoord2f( 15.0f, 0.0f ); glVertex3f(-w, h,-l); // 3
		glTexCoord2f( 15.0f, 1.0f ); glVertex3f(-w,-h,-l); // 2
		glTexCoord2f( 0.0f, 1.0f ); glVertex3f(-w,-h, l); // 1															
		//derecha
		glNormal3f( 1.0f, 0.0f, 0.0f);
		glTexCoord2f( 0.0f, 0.0f ); glVertex3f( w, h,-l); // 4
		glTexCoord2f( 15.0f, 0.0f ); glVertex3f( w, h, l); // 3
		glTexCoord2f( 15.0f, 1.0f ); glVertex3f( w,-h, l); // 2
		glTexCoord2f( 0.0f, 1.0f ); glVertex3f( w,-h,-l); // 1												
	glEnd();											
}

void CPrimitivas::glDibujaCubo3(float w, float h, float l)
{
	w=w/2;
	h=h/2;
	l=l/2;


	// Habilitando texturas
	glEnable( GL_TEXTURE_2D );
	glBindTexture(GL_TEXTURE_2D, text[ 14 ].texID);

	glBegin(GL_QUADS);									
		//arriba
		glNormal3f( 0.0f, 1.0f, 0.0f);
		glTexCoord2f( 0.0f, 0.0f ); glVertex3f( w, h,-l); // 4
		glTexCoord2f( 1.0f, 0.0f ); glVertex3f(-w, h,-l); // 3
		glTexCoord2f( 1.0f, 1.0f ); glVertex3f(-w, h, l); // 2
		glTexCoord2f( 0.0f, 1.0f ); glVertex3f( w, h, l); // 1	
	glEnd();

	glBindTexture(GL_TEXTURE_2D, text[ 13 ].texID);
	glBegin(GL_QUADS);
		//abajo
		glNormal3f( 0.0f, -1.0f, 0.0f);
		glVertex3f( w,-h, l); // 4
		glVertex3f(-w,-h, l); // 3
		glVertex3f(-w,-h,-l); // 2
		glVertex3f( w,-h,-l); // 1	
		//frontal
		glBindTexture(GL_TEXTURE_2D, text[ 13 ].texID);
		glNormal3f( 0.0f, 0.0f, 1.0f);
		glTexCoord2f( 0.0f, 0.0f ); glVertex3f( w, h, l); // 4
		glTexCoord2f( 5.0f, 0.0f ); glVertex3f(-w, h, l); // 3
		glTexCoord2f( 5.0f, 5.0f ); glVertex3f(-w,-h, l); // 2
		glTexCoord2f( 0.0f, 5.0f ); glVertex3f( w,-h, l); // 1		
		//atras
		glNormal3f( 0.0f, 0.0f, -1.0f);
		glTexCoord2f( 0.0f, 0.0f ); glVertex3f( w,-h,-l); // 4	
		glTexCoord2f( 5.0f, 0.0f ); glVertex3f(-w,-h,-l); // 3
		glTexCoord2f( 5.0f, 5.0f ); glVertex3f(-w, h,-l); // 2
		glTexCoord2f( 0.0f, 5.0f ); glVertex3f( w, h,-l); // 1																														
		//derecha
		glNormal3f( 1.0f, 0.0f, 0.0f);
		glTexCoord2f( 0.0f, 0.0f ); glVertex3f( w, h,-l); // 4
		glTexCoord2f( 5.0f, 0.0f ); glVertex3f( w, h, l); // 3
		glTexCoord2f( 5.0f, 5.0f ); glVertex3f( w,-h, l); // 2
		glTexCoord2f( 0.0f, 5.0f ); glVertex3f( w,-h,-l); // 1												
	glEnd();	

	glBindTexture(GL_TEXTURE_2D, text[ 12 ].texID);
	glBegin( GL_QUADS );
		//izquierda
		glNormal3f( -1.0f, 0.0f, 0.0f);
		glTexCoord2f( 0.0f, 0.0f ); glVertex3f(-w, h, l); // 4
		glTexCoord2f( 1.0f, 0.0f ); glVertex3f(-w, h,-l); // 3
		glTexCoord2f( 1.0f, 3.5f ); glVertex3f(-w,-h,-l); // 2
		glTexCoord2f( 0.0f, 3.5f ); glVertex3f(-w,-h, l); // 1
	glEnd();

	glDisable(GL_TEXTURE_2D);
}

void CPrimitivas::glDibujaPrismaTri( float w, float h, float l )
{
	w = w / 2.0f;
	h = h / 2.0f;
	l = l / 2.0f;

	// vertices de la cara frontal
	GLfloat vf1[] = { w, -h, l };
	GLfloat vf2[] = { w, h, l };
	GLfloat vf3[] = { -w, -h, l };

	// vertices de la cara trasera
	GLfloat vb1[] = { w,-h, -l };
	GLfloat vb2[] = { w, h , -l };
	GLfloat vb3[] = { -w, -h, -l };

	glEnable( GL_TEXTURE_2D );
	glBindTexture(GL_TEXTURE_2D, text[ 13 ].texID);
	glBegin( GL_TRIANGLES );
		// triangulo frontal
		glNormal3f( 0.0f, 0.0f, 1.0f );
		glTexCoord2f( 0.0f, 1.0f ); glVertex3fv( vf1 );
		glTexCoord2f( 1.0f, 1.0f ); glVertex3fv( vf2 );
		glTexCoord2f( 0.0f, 0.0f ); glVertex3fv( vf3 );
		// triangulo trasero
		glNormal3f( 0.0f, 0.0f, -1.0f );
		glTexCoord2f( 0.0f, 0.0f ); glVertex3fv( vb3 );
		glTexCoord2f( 1.0f, 1.0f ); glVertex3fv( vb2 );
		glTexCoord2f( 1.0f, 0.0f ); glVertex3fv( vb1 );
	glEnd();

	glBindTexture(GL_TEXTURE_2D, text[ 14 ].texID);
	glBegin( GL_QUADS );
		// Cara superior
		Normalize( vf3, vf2, vb3 );
		glTexCoord2f( 0.0f, 0.0f ); glVertex3fv( vf3 );
		glTexCoord2f( 3.0f, 0.0f ); glVertex3fv( vf2 );
		glTexCoord2f( 3.0f, 3.0f ); glVertex3fv( vb2 );
		glTexCoord2f( 0.0f, 3.0f ); glVertex3fv( vb3 );
	glEnd();

	glBindTexture(GL_TEXTURE_2D, text[ 13 ].texID);
	glBegin( GL_QUADS );
		// Cara exterior (la de la atras XD)
		glNormal3f( 1.0f, 0.0f, 0.0f );
		glTexCoord2f( 0.0f, 0.0f ); glVertex3fv( vb1 );
		glTexCoord2f( 1.0f, 0.0f ); glVertex3fv( vb2 );
		glTexCoord2f( 1.0f, 1.0f ); glVertex3fv( vf2 );
		glTexCoord2f( 0.0f, 1.0f ); glVertex3fv( vf1 );
		// Cara inferior
		glNormal3f( 0.0f, -1.0f, 0.0f );
		glTexCoord2f( 0.0f, 0.0f ); glVertex3fv( vf3 );
		glTexCoord2f( 1.0f, 0.0f ); glVertex3fv( vb3 );
		glTexCoord2f( 1.0f, 1.0f ); glVertex3fv( vb1 );
		glTexCoord2f( 0.0f, 1.0f ); glVertex3fv( vf1 );
	glEnd();
	glDisable( GL_TEXTURE_2D );
}

//Primitivas Alan
float CPrimitivas::grad2rad(float grados)
{
	return (float)(grados*PI)/180;
}
void CPrimitivas::Esfera( float radio, int paralelos, int meridianos, int k, int l, bool b_text )
{
	float ang1, ang2;
	float a[ 3 ], b[ 3 ], c[ 3 ], d[ 3 ];
	float delta1, delta2;
	float s1, s2, t1, t2, deltas, deltat;
	CVector V1, V2, N;

	deltas = 1.0f / meridianos;
	deltat = 1.0f / ( paralelos * 0.5f );
	
	delta1 = 180.0f / paralelos;
	delta2 = 360.0f / meridianos;

	s1 = 0.0f;
	s2 = 0.0f;
	t1 = 0.0f;
	t2 = 0.0f;

	for( int i = l; i < paralelos / 2 ; i++ )
	{
		for( int j = 0; j <= meridianos ; j++ )
		{
			ang1 = i * delta1; //inc paralelos
			ang2 = j * delta2; //inc meridianos

			t1 = deltat * i;
			s1 = deltas * j;
			t2 = deltat * ( i + 1 );
			s2 = deltas * ( j + 1 );

			a[ 0 ] = radio * cosf( ang1 * PI / 180.0f ) * cosf( ang2 * PI / 180.0f );
			a[ 1 ] = radio * sinf( ang1 * PI / 180.0f );
			a[ 2 ] = radio * cosf( ang1 * PI / 180.0f ) * sinf( ang2 * PI / 180.0f );

			ang1 = ( i + 1 ) * delta1;
			
			b[ 0 ] = radio * cosf( ang1 * PI / 180.0f ) * cosf( ang2 * PI / 180.0f );
			b[ 1 ] = radio * sinf( ang1 * PI / 180.0f );
			b[ 2 ] = radio * cosf( ang1 * PI / 180.0f ) * sinf( ang2 * PI / 180.0f );

			ang2 = ( j + 1 ) * delta2;

			c[ 0 ] = radio * cosf( ang1 * PI / 180.0f ) * cosf( ang2 * PI / 180.0f );
			c[ 1 ] = radio * sinf( ang1 * PI / 180.0f );
			c[ 2 ] = radio * cosf( ang1 * PI / 180.0f ) * sinf( ang2 * PI / 180.0f );

			ang1 = i * delta1;

			d[ 0 ] = radio * cosf( ang1 * PI / 180.0f ) * cosf( ang2 * PI / 180.0f );
			d[ 1 ] = radio * sinf( ang1 * PI / 180.0f );
			d[ 2 ] = radio * cosf( ang1 * PI / 180.0f ) * sinf( ang2 * PI / 180.0f );

				V1.x = d[ 0 ] - a[ 0 ]; 
				V1.y = d[ 1 ] - a[ 1 ];
				V1.z = d[ 2 ] - a[ 2 ];

				V2.x = b[ 0 ] - a[ 0 ];
				V2.y = b[ 1 ] - a[ 1 ];
				V2.z = b[ 2 ] - a[ 2 ];

				N = Normaliza( Cruz( V2, V1 ) );

			if( b_text == false )
			{
				glBegin( GL_QUADS );
					glNormal3f( N.x, N.y, N.z );
					glVertex3f( a[ 0 ] , a[ 1 ], a[ 2 ] );
					glVertex3f( b[ 0 ] , b[ 1 ], b[ 2 ] );
					glVertex3f( c[ 0 ] , c[ 1 ], c[ 2 ] );
					glVertex3f( d[ 0 ] , d[ 1 ], d[ 2 ] );
				glEnd();
			}		
			else if( b_text == true )
			{
				glEnable( GL_TEXTURE_2D );

				glBindTexture( GL_TEXTURE_2D, text[k].texID);

				glBegin( GL_QUADS );
				glTexCoord2f(s1, t2); glVertex3f( b[ 0 ] , b[ 1 ], b[ 2 ] );
				glTexCoord2f(s1, t1); glVertex3f( a[ 0 ] , a[ 1 ], a[ 2 ] );
				glTexCoord2f(s2, t1); glVertex3f( d[ 0 ] , d[ 1 ], d[ 2 ] );
				glTexCoord2f(s2, t2); glVertex3f( c[ 0 ] , c[ 1 ], c[ 2 ] );
				glEnd();

				glDisable( GL_TEXTURE_2D );
			}
		}
	}

}
void CPrimitivas::Toroide( float r1, float r2, int segmentos, int lados )
{
	float a[ 3 ], b[ 3 ], c[ 3 ], d[ 3 ];
	float incrtheta = 360.0f / segmentos;
	float incrphi = 360.0f / lados;
	float theta;
	float phi;
	int i, j;
	CVector V1, V2, N;

	for(i = 0; i < segmentos; i++)
	{
		for(j = 0; j < lados; j++)
		{
			theta = i * incrtheta;
			phi = j * incrphi;

			a[ 0 ] = (r1+(r2*cosf(grad2rad(theta))))*cosf(grad2rad(phi));
			a[ 1 ] = r2*sinf(grad2rad(theta));
			a[ 2 ] = (r1+(r2*cosf(grad2rad(theta))))*sinf(grad2rad(phi));

			theta = ( i + 1 ) * incrtheta;

			b[ 0 ] = (r1+(r2*cosf(grad2rad(theta))))*cosf(grad2rad(phi));
			b[ 1 ] = r2*sinf(grad2rad(theta));
			b[ 2 ] = (r1+(r2*cosf(grad2rad(theta))))*sinf(grad2rad(phi));

			phi = ( j + 1 ) * incrphi;

			c[ 0 ] = (r1+(r2*cosf(grad2rad(theta))))*cosf(grad2rad(phi));
			c[ 1 ] = r2*sinf(grad2rad(theta));
			c[ 2 ] = (r1+(r2*cosf(grad2rad(theta))))*sinf(grad2rad(phi));

			theta = i * incrtheta;

			d[ 0 ] = (r1+(r2*cosf(grad2rad(theta))))*cosf(grad2rad(phi));
			d[ 1 ] = r2*sinf(grad2rad(theta));
			d[ 2 ] = (r1+(r2*cosf(grad2rad(theta))))*sinf(grad2rad(phi));

			V1.x = d[ 0 ] - a[ 0 ]; 
			V1.y = d[ 1 ] - a[ 1 ];
			V1.z = d[ 2 ] - a[ 2 ];

			V2.x = b[ 0 ] - a[ 0 ];
			V2.y = b[ 1 ] - a[ 1 ];
			V2.z = b[ 2 ] - a[ 2 ];

			N = Normaliza( Cruz( V2, V1 ) );

			glBegin(GL_QUADS);
				glNormal3f( N.x, N.y, N.z );
				glVertex3f(a[ 0 ], a[ 1 ], a[ 2 ]);
				glVertex3f(b[ 0 ], b[ 1 ], b[ 2 ]);
				glVertex3f(c[ 0 ], c[ 1 ], c[ 2 ]);
				glVertex3f(d[ 0 ], d[ 1 ], d[ 2 ]);
			glEnd();
		}
	}
}
void CPrimitivas::Cilindro( float rad, float h, int text1, int text2, int l, int lados, bool b_text, bool tapa )
{
	int   j, k;
	float radio, alt;
	float ang, delta, deltas, s1, s2, s3, s4, t1, t2;
	float a[ 3 ], b[ 3 ], c[ 3 ], d[ 3 ], o[ 3 ];
	CVector V1, V2, N, N1, N2;

	radio = rad;
	alt = h;

	delta = 360.0f / lados;
	deltas= 6.0f / lados;

	j = text1;
	k = text2;

	for( int i = l; i < lados ; i++ )
	{
		s1 = i * deltas;
		s2 = ( i + 1 ) * deltas;

		ang = i * delta;
		s3 = ( 0.5 * cosf( ang * PI / 180.0f ) ) + 0.5f;
		t1 = ( 0.5 * sinf( ang * PI / 180.0f ) ) + 0.5f;

		a[ 0 ] = radio * cosf( ang * PI / 180.0f );
		a[ 1 ] = - alt / 2.0f;
		a[ 2 ] = radio * sinf( ang * PI / 180.0f );

		b[ 0 ] = radio * cosf( ang * PI / 180.0f );
		b[ 1 ] = alt / 2.0f;
		b[ 2 ] = radio * sinf( ang * PI / 180.0f );

		ang = ( i + 1 ) * delta;
		s4 = ( 0.5 * cosf( ang * PI / 180.0f ) ) + 0.5f;
		t2 = ( 0.5 * sinf( ang * PI / 180.0f ) ) + 0.5f;

		c[ 0 ] = radio * cosf( ang * PI / 180.0f );
		c[ 1 ] = alt / 2.0f;
		c[ 2 ] = radio * sin( ang * PI / 180.0f );

		d[ 0 ] = radio * cosf( ang * PI / 180.0f );
		d[ 1 ] = -alt / 2.0f;
		d[ 2 ] = radio * sinf( ang * PI / 180.0f );

		V1.x = d[ 0 ] - a[ 0 ]; 
		V1.y = d[ 1 ] - a[ 1 ];
		V1.z = d[ 2 ] - a[ 2 ];

		V2.x = b[ 0 ] - a[ 0 ];
		V2.y = b[ 1 ] - a[ 1 ];
		V2.z = b[ 2 ] - a[ 2 ];

		N = Normaliza( Cruz( V2, V1 ) );

		N1 = N;

		if( b_text == false )
		{
			glBegin(GL_QUADS);
				glNormal3f( N1.x, N1.y, N1.z );
				glVertex3f( a[ 0 ], a[ 1 ], a[ 2 ] );
				glVertex3f( b[ 0 ], b[ 1 ], b[ 2 ] );
				glVertex3f( c[ 0 ], c[ 1 ], c[ 2 ] );
				glVertex3f( d[ 0 ], d[ 1 ], d[ 2 ] );
			glEnd();

			if( tapa == true )
			{
				//Tapa superior
				o[ 0 ] = 0.0f;
				o[ 1 ] = alt / 2.0f;
				o[ 2 ] = 0.0f;

				V1.x = o[ 0 ] - b[ 0 ]; 
				V1.y = o[ 1 ] - b[ 1 ];
				V1.z = o[ 2 ] - b[ 2 ];

				V2.x = c[ 0 ] - b[ 0 ];
				V2.y = c[ 1 ] - b[ 1 ];
				V2.z = c[ 2 ] - b[ 2 ];

				N = Normaliza( Cruz( V1, V2 ) );

				N2 = N;

				glBegin(GL_TRIANGLES);
					glNormal3f( N2.x, N2.y, N2.z );
					glVertex3f( b[ 0 ], b[ 1 ], b[ 2 ] );
					glVertex3f( o[ 0 ], o[ 1 ], o[ 2 ] );
					glVertex3f( c[ 0 ], c[ 1 ], c[ 2 ] );
				glEnd();

				//Tapa inferior
				o[ 0 ] = 0.0f;
				o[ 1 ] = - alt / 2.0f;
				o[ 2 ] = 0.0f;

				V1.x = d[ 0 ] - a[ 0 ]; 
				V1.y = d[ 1 ] - a[ 1 ];
				V1.z = d[ 2 ] - a[ 2 ];

				V2.x = o[ 0 ] - a[ 0 ];
				V2.y = o[ 1 ] - a[ 1 ];
				V2.z = o[ 2 ] - a[ 2 ];

				N = Normaliza( Cruz( V1, V2 ) );

				glBegin(GL_TRIANGLES);
					glNormal3f( N.x, N.y, N.z );
					glVertex3f( a[ 0 ], a[ 1 ], a[ 2 ] );
					glVertex3f( d[ 0 ], d[ 1 ], d[ 2 ] );
					glVertex3f( o[ 0 ], o[ 1 ], o[ 2 ] );
				glEnd();
			}
		}
		else if( b_text == true )
		{
			glEnable(GL_TEXTURE_2D);

			glBindTexture(GL_TEXTURE_2D, text[j].texID);

			glBegin(GL_QUADS);
				glNormal3f( N1.x, N1.y, N1.z );
				glTexCoord2f( s2, 0.0f ); glVertex3f( a[ 0 ], a[ 1 ], a[ 2 ] );
				glTexCoord2f( s2, 1.0f ); glVertex3f( b[ 0 ], b[ 1 ], b[ 2 ] );
				glTexCoord2f( s1, 1.0f ); glVertex3f( c[ 0 ], c[ 1 ], c[ 2 ] );
				glTexCoord2f( s1, 0.0f ); glVertex3f( d[ 0 ], d[ 1 ], d[ 2 ] );
			glEnd();

			if( tapa == true );
			{
				//Tapa superior
				o[ 0 ] = 0.0f;
				o[ 1 ] = alt / 2.0f;
				o[ 2 ] = 0.0f;

				V1.x = o[ 0 ] - b[ 0 ]; 
				V1.y = o[ 1 ] - b[ 1 ];
				V1.z = o[ 2 ] - b[ 2 ];

				V2.x = c[ 0 ] - b[ 0 ];
				V2.y = c[ 1 ] - b[ 1 ];
				V2.z = c[ 2 ] - b[ 2 ];

				N = Normaliza( Cruz( V1, V2 ) );

				N2 = N;

				glBindTexture(GL_TEXTURE_2D, text[k].texID);

				glBegin(GL_TRIANGLES);
					glNormal3f( N2.x, N2.y, N2.z );
					glTexCoord2f( s3, t1 ); glVertex3f( b[ 0 ], b[ 1 ], b[ 2 ] );
					glTexCoord2f( 0.5f, 0.5f ); glVertex3f( o[ 0 ], o[ 1 ], o[ 2 ] );
					glTexCoord2f( s4, t2 ); glVertex3f( c[ 0 ], c[ 1 ], c[ 2 ] );
				glEnd();

				//Tapa inferior
				o[ 0 ] = 0.0f;
				o[ 1 ] = - alt / 2.0f;
				o[ 2 ] = 0.0f;

				V1.x = d[ 0 ] - a[ 0 ]; 
				V1.y = d[ 1 ] - a[ 1 ];
				V1.z = d[ 2 ] - a[ 2 ];

				V2.x = o[ 0 ] - a[ 0 ];
				V2.y = o[ 1 ] - a[ 1 ];
				V2.z = o[ 2 ] - a[ 2 ];

				N = Normaliza( Cruz( V1, V2 ) );

				glBegin(GL_TRIANGLES);
					glNormal3f( N.x, N.y, N.z );
					glTexCoord2f( s3, t1 ); glVertex3f( a[ 0 ], a[ 1 ], a[ 2 ] );
					glTexCoord2f( s4, t2 ); glVertex3f( d[ 0 ], d[ 1 ], d[ 2 ] );
					glTexCoord2f( 0.5f, 0.5f ); glVertex3f( o[ 0 ], o[ 1 ], o[ 2 ] );
				glEnd();
			}

			glDisable(GL_TEXTURE_2D);
		}		
	}
}
void CPrimitivas::Prisma( float w, float h, float l, int i, int j, bool b_text )
{
	if( b_text == false )
	{
		glBegin(GL_QUADS);									
			//arriba
			glNormal3f(0.0f,1.0f,0.0f);
			glVertex3f( w, h,-l);					
			glVertex3f(-w, h,-l);					
			glVertex3f(-w, h, l);					
			glVertex3f( w, h, l);					
			//abajo
			glNormal3f(0.0f,-1.0f,0.0f);
			glVertex3f( w,-h, l);					
			glVertex3f(-w,-h, l);					
			glVertex3f(-w,-h,-l);					
			glVertex3f( w,-h,-l);					
			//frontal
			glNormal3f(0.0f,0.0f,1.0f);
			glVertex3f( w, h, l);					
			glVertex3f(-w, h, l);					
			glVertex3f(-w,-h, l);					
			glVertex3f( w,-h, l);					
			//atras
			glNormal3f(0.0f,0.0f,-1.0f);
			glVertex3f( w,-h,-l);					
			glVertex3f(-w,-h,-l);					
			glVertex3f(-w, h,-l);					
			glVertex3f( w, h,-l);					
			//izquierda
			glNormal3f(-1.0f,0.0f,0.0f);
			glVertex3f(-w, h, l);					
			glVertex3f(-w, h,-l);					
			glVertex3f(-w,-h,-l);					
			glVertex3f(-w,-h, l);					
			//derecha
			glNormal3f(1.0f,0.0f,0.0f);
			glVertex3f( w, h,-l);					
			glVertex3f( w, h, l);					
			glVertex3f( w,-h, l);					
			glVertex3f( w,-h,-l);					
		glEnd();	
	}
	else if( b_text == true )
	{
		glEnable( GL_TEXTURE_2D );

		glBindTexture( GL_TEXTURE_2D, text[i].texID );

		glBegin(GL_QUADS);									
			//arriba
			glNormal3f(0.0f,1.0f,0.0f);
			glTexCoord2f( 1.0f, 0.0f ); glVertex3f( w, h,-l);					
			glTexCoord2f( 0.0f, 0.0f ); glVertex3f(-w, h,-l);					
			glTexCoord2f( 0.0f, 1.0f ); glVertex3f(-w, h, l);					
			glTexCoord2f( 1.0f, 1.0f ); glVertex3f( w, h, l);	
		glEnd();	

		glBegin(GL_QUADS);
			//abajo
			glNormal3f(0.0f,-1.0f,0.0f);
			glTexCoord2f( 1.0f, 1.0f ); glVertex3f( w,-h, l);					
			glTexCoord2f( 0.0f, 1.0f ); glVertex3f(-w,-h, l);					
			glTexCoord2f( 0.0f, 0.0f ); glVertex3f(-w,-h,-l);					
			glTexCoord2f( 1.0f, 0.0f ); glVertex3f( w,-h,-l);	
		glEnd();	

		glBegin(GL_QUADS);
			//atras
			glNormal3f(0.0f,0.0f,-1.0f);
			glTexCoord2f( 1.0f, 0.0f ); glVertex3f( w,-h,-l);					
			glTexCoord2f( 0.0f, 0.0f ); glVertex3f(-w,-h,-l);					
			glTexCoord2f( 0.0f, 1.0f ); glVertex3f(-w, h,-l);					
			glTexCoord2f( 1.0f, 1.0f ); glVertex3f( w, h,-l);	
		glEnd();	

		glBegin(GL_QUADS);
			//izquierda
			glNormal3f(-1.0f,0.0f,0.0f);
			glTexCoord2f( 1.0f, 1.0f ); glVertex3f(-w, h, l);					
			glTexCoord2f( 1.0f, 0.0f ); glVertex3f(-w, h,-l);					
			glTexCoord2f( 0.0f, 0.0f ); glVertex3f(-w,-h,-l);					
			glTexCoord2f( 0.0f, 1.0f ); glVertex3f(-w,-h, l);	
		glEnd();	

		glBegin(GL_QUADS);
			//derecha
			glNormal3f(1.0f,0.0f,0.0f);
			glTexCoord2f( 1.0f, 0.0f ); glVertex3f( w, h,-l);					
			glTexCoord2f( 1.0f, 1.0f ); glVertex3f( w, h, l);					
			glTexCoord2f( 0.0f, 1.0f ); glVertex3f( w,-h, l);					
			glTexCoord2f( 0.0f, 0.0f ); glVertex3f( w,-h,-l);					
		glEnd();	

		glBindTexture( GL_TEXTURE_2D, text[j].texID );

		glBegin(GL_QUADS);
			//frontal
			glNormal3f(0.0f,0.0f,1.0f);
			glTexCoord2f( 1.0f, 1.0f ); glVertex3f( w, h, l);					
			glTexCoord2f( 0.0f, 1.0f ); glVertex3f(-w, h, l);					
			glTexCoord2f( 0.0f, 0.0f ); glVertex3f(-w,-h, l);					
			glTexCoord2f( 1.0f, 0.0f ); glVertex3f( w,-h, l);	
		glEnd();

		glDisable( GL_TEXTURE_2D );
	}
}

void CPrimitivas::Plano( float w, float h, float s1, float s2, int i, bool b_text )
{	
	if( b_text == false )
	{
		glBegin( GL_QUADS );
			glNormal3f( 0.0f, 0.0f, 1.0f );
			glVertex3f( w, h, 0 );					
			glVertex3f( -w, h, 0 );					
			glVertex3f( -w,-h, 0 );					
			glVertex3f( w,-h, 0 );
		glEnd();
	}
	else if( b_text == true )
	{
		glEnable( GL_TEXTURE_2D );

		glBindTexture( GL_TEXTURE_2D, text[i].texID );
		
		glBegin( GL_QUADS );
			glNormal3f( 0.0f, 0.0f, 1.0f );
			glTexCoord2f( s2, 1.0f  ); glVertex3f( w, h, 0 );					
			glTexCoord2f( s1, 1.0f ); glVertex3f( -w, h, 0 );					
			glTexCoord2f( s1, 0.0f ); glVertex3f( -w,-h, 0 );					
			glTexCoord2f( s2, 0.0f ); glVertex3f( w,-h, 0 );
		glEnd();

		glDisable( GL_TEXTURE_2D );
	}
}