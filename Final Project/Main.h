#ifndef _MAIN_H
#define _MAIN_H

#include <windows.h>		// Archivo de cabecera para Windows
#include <math.h>			// Archivo de cabecera para Funciones Matem�ticas
#include <stdio.h>			// Header File For Standard Input/Output
#include <stdlib.h>			// Header File For Standard Library
#include <fstream>
#include <vector>
#include <crtdbg.h>
#include "gl.h"				// Archivo de cabecera para la libreria OpenGL32
#include "glu.h"			// Archivo de cabecera para la libreria GLu32
#include "glaux.h"			// Archivo de cabecera para la libreria Glaux
#include "glext.h"			// Archivo de cabecera para la libreria Glext
#include "Vector.h"
#include "cargadorTGA.h"
#include "Multitexturas.h"	// Agregado para hacer el programa 7
#include "Font.h"	// Agregado para hacer el programa 7
#include "Primitivas.h" // Agregado para usar las primitivas
#include "fmod.h"
#include "fmod_errors.h"

void Camara( int funcion );
int  ManejaTeclado();
int  ManejaTeclado(FMOD_SYSTEM *system, FMOD_RESULT result);

typedef struct _frame
{
	float Angt1;
	float Angt2;
	float Angc;
	float Angbi1;
	float Angbi2;
	float Angbd1;
	float Angbd2;
	float Angpizq;
	float Angpder;
	float Angpi;
	float Angpd;

	float Xtor;
	float Ytor;
	float Ztor;

	float incAngt1;
	float incAngt2;
	float incAngc;
	float incAngbi1;
	float incAngbi2;
	float incAngbd1;
	float incAngbd2;
	float incAngpizq;
	float incAngpder;
	float incAngpi;
	float incAngpd;

	float incXtor;
	float incYtor;
	float incZtor;

}FRAME;

typedef struct vars
{
	CVector PosicionObj;	//La posici�n (x,y,z) del objeto
	CVector Direccion;		//La direcci�n en que se dirige el objeto en forma de vector=(x,y,z)
	CVector PosicionCam;    //La posici�n de la c�mara que sigue al objeto
	CVector ObjetivoCam;	//El punto (x,y,z) que est� viendo la c�mara; en este caso al objeto

	float VelocidadObj;		//La velocidad a la que se mueve el objeto
	float DistanciaCam;		//La distancia que la c�mara est� separada del objeto
	float AngDir;			//Se usa para llevar control del angulo para el c�lculo del vector de direcci�n
	float AngObj;			//Se usa para llevar control del �ngulo de rotaci�n para el modelo del objeto

	float CamaraPosAlt;		//Se usa para definir y cambiar si es necesario la altura de la c�mara
	float CamaraObjAlt;		//Se usa para definir y cambiar si es necesario la altura del objetivo de la c�mara
	
} parametros;

#endif 