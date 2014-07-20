#ifndef __PRIMITIVAS_H__
#define __PRIMITIVAS_H__

#include "Main.h"

class CPrimitivas
{
	public:
		CPrimitivas();
		~CPrimitivas(); 
		
		void CargaTexturas(void);
		void DescargaTexturas(void);
		void glDibujaCubo(float w, float h, float l);
		void glDibujaPiramide(void);
		void glDibujaCono(float r);
		void glDibujaEsfera(float r);
		void glDibujaCilindro(float r, float ri, float h);
		void DibujaDomoCielo(float radio, int paralelos, int meridianos);
		void glDibujaCubo2(float w, float h, float l);
		void glDibujaCubo3(float w, float h, float l);
		void glDibujaPrismaTri( float w, float h, float l );
		void Normalize( float X1[3], float X2[3], float X3[3] );
		// Alan
		void Esfera( float radio, int paralelos, int meridianos, int k, int l, bool text );
		void Toroide( float r1, float r2, int segmentos, int lados );
		void Cilindro( float rad, float h, int text1, int text2, int l, int lados, bool text, bool tapa );
		void Prisma( float w, float h, float l, int i, int j, bool text );
		void Plano( float w, float h, float s1, float s2, int i, bool text );
		float grad2rad( float grados );
		
	private:

		GLUquadricObj	*glq;
		
};

#endif