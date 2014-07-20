/*
 * Proyecto Final - Genaro
 * Mario vs Donkey Kong en un main.cpp
 */

// Bibliotecas requeridas
#include "Main.h" // requerida por genaro XD
#include "3ds.h" // requerida por genaro tambien XD
#include "math.h" // requerida para hacer operaciones matematicas
#include <ctime> // requerida para seedear srand() con time(0)
#include "Primitivas.h"
#include "Materiales.h"

CPrimitivas Prim;
CMateriales Material;

FMOD_SOUND       *sound[3]   = { 0, 0, 0 };
FMOD_CHANNEL     *channel[3] = { 0, 0, 0 };

float rot = 0.0f; // rotacion del ventilador...borrar?
float rot2 = 45.0f;
int KeyFrameIndex = 0; // Indice para sacar las keyframes automaticamente
FILE *F; // Apuntador a un archivo donde se escriben las keyframse
float Pi = 3.14f; // Pi, el orden del caos

//variables globales
int puntos =1000000; //o cualquier número
int times;
int tiempo=350;

int lives = 3;

//Variables para animaciones
const int maxKF1 = 4; //Num. total de KeyFrames para la secuencia 1 (caminar)
const int maxKF2 = 7; //Num. total de KeyFrames para la secuencia 2 (saltar)

FRAME KeyFrame1[maxKF1]; //Contenedor para almacenar cada keyframe de la secuencia 1
FRAME KeyFrame2[maxKF2]; //Contenedor para almacenar cada keyframe de la secuencia 2

//Variables auxiliares para animacion
bool play=false; //Bandera para iniciar la animación
int playIndex=0; //Auxiliar para leer la información del contenedor de keyframes
int tipoAnim=1; //Indicador del tipo de animación: 1-caminando, 2-saltando

CFont Font;
CMultitexturas Multitext;

/* 
 * Declaren aqui los vertices globales que usaran
 * para obtener el producto cruz, utilicemos
 *				Vp1, Vp2, N
 */


/* Codigo que no se toca */

HDC			hDC=NULL;		// Dispositivo de contexto GDI
HGLRC		hRC=NULL;		// Contexto de renderizado
HWND		hWnd=NULL;		// Manejador de ventana
HINSTANCE	hInstance;		// Instancia de la aplicacion

bool	keys[256];			// Arreglo para el manejo de teclado
bool	active=TRUE;		// Bandera de ventana activa

int glWidth;
int glHeight;

GLUquadricObj	*e;

parametros player1;  //Variable con la que tenemos acceso a la estructura de parámetros

/* Hasta aqui se puede tocar */


// Nombre y ubicación del modelo de stewie
// Sustituir el modelo de Stewie por el de mario, chop, chop.

//Nombre y ubicación del modelo
#define FILE_NAME1c  "Modelos/mario_torzo.3ds"
#define FILE_NAME2c  "Modelos/mario_cabeza.3ds"
#define FILE_NAME3c  "Modelos/mario_piernader.3ds"
#define FILE_NAME4c  "Modelos/mario_piernaizq.3ds"
#define FILE_NAME5c  "Modelos/mario_pieder.3ds"
#define FILE_NAME6c  "Modelos/mario_pieizq.3ds"
#define FILE_NAME7c  "Modelos/mario_brazoder.3ds"
#define FILE_NAME8c  "Modelos/mario_brazoizq.3ds"
#define FILE_NAME9c  "Modelos/mario_arma.3ds"
#define FILE_NAME10c "Modelos/goomba_cuerpo.3ds"
#define FILE_NAME11c "Modelos/goomba_pieder.3ds"
#define FILE_NAME12c "Modelos/goomba_pieizq.3ds"

/* No tocar desde aqui... */

// Contenedor de texturas del modelo
CTga textureModel1c[20];
CTga textureModel2c[20];
CTga textureModel3c[20];
CTga textureModel4c[20];
CTga textureModel5c[20];
CTga textureModel6c[20];
CTga textureModel7c[20];
CTga textureModel8c[20];
CTga textureModel9c[20];
CTga textureModel10c[20];
CTga textureModel11c[20];
CTga textureModel12c[20];

CLoad3DS g_Load3ds;
//Acceso a la estructura que almacena los datos del modelo
t3DModel g_3DModel1c;
t3DModel g_3DModel2c;
t3DModel g_3DModel3c;
t3DModel g_3DModel4c;
t3DModel g_3DModel5c;
t3DModel g_3DModel6c;
t3DModel g_3DModel7c;
t3DModel g_3DModel8c;
t3DModel g_3DModel9c;
t3DModel g_3DModel10c;
t3DModel g_3DModel11c;
t3DModel g_3DModel12c;

// Contenedor de texturas para el escenario
CTga textureName[ 30 ];

//Variables del personaje
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

float alcance; // se usara para que mario use el martillo XD o bolas de fuego, o algo

int IncDec;

LRESULT	CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);	// Declaracion de WndProc (Procedimiento de ventana)

// Contenedor de texturas para el escenario
CTga textura[ 31 ];

// Variables para iluminacion
//GLfloat LightPos[] = { 0.0f, 500.0f, 0.0f, 1.0f};		// Posición de la luz
GLfloat LightPos[] = { 0.0f, 20.0f, 15.0f, 1.0f};		// Posición de la luz
GLfloat LightAmb[] = { 0.95f,  0.95f, 0.95f, 1.0f};		// Valores de la componente ambiente
GLfloat LightDif[] = { 0.9f,  0.9f, 0.9f, 1.0f};		// Valores de la componente difusa
GLfloat LightSpc[] = { 0.7f,  0.7f, 0.7f, 1.0f};		// Valores de la componente especular
CVector lightPosition;

// Variables para animación de texturas
// Esto lo pueden tocar, pero justificandolo
float aText1;
float aText2;

/* ...Hasta aca */

/* A esta me gusta llamarle, la funcion Gilberto XD
 * Funcion para facilitar el calculo de la normal
 * La tengo que optimizar xp
 * Pero si todo lo hacen con vectores previamente declarados
 * todo saldra bien
 */
void DibujaBodega();
void Normalize( float X1[3], float X2[3], float X3[3] ){
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

/* Esto tampoco se toca */
GLvoid ReSizeGLScene(GLsizei width, GLsizei height)		// Redimensiona e inicializa la ventana
{
	if (height==0)							// Para que no se presente una division por cero
	{
		height=1;							// la altura se iguala a 1
	}

	glViewport(0,0,width,height);					// Resetea el puerto de vista

	glMatrixMode(GL_PROJECTION);					// Selecciona la Matriz de Proyeccion
	glLoadIdentity();								// Resetea la Matriz de Proyeccion

	// Calcula el radio de aspecto o proporcion de medidas de la ventana
	gluPerspective(45.0f,(GLfloat)width/(GLfloat)height,0.1f,2000.0f);
	
	glMatrixMode(GL_MODELVIEW);							// Selecciona la Matriz de Vista de Modelo
	glLoadIdentity();									// Resetea la Matriz de Vista de Modelo

	glWidth=width;
	glHeight=height;
}
/* No tocar */

// Esta funcion se puede cambiar, supongo, si me dicen porque
int CargaModelos()
{
if(!g_Load3ds.Load3DSFile(FILE_NAME1c, &g_3DModel1c, textureModel1c))
		return 0;
	if(!g_Load3ds.Load3DSFile(FILE_NAME2c, &g_3DModel2c, textureModel2c))
		return 0;
	if(!g_Load3ds.Load3DSFile(FILE_NAME3c, &g_3DModel3c, textureModel3c))
		return 0;
	if(!g_Load3ds.Load3DSFile(FILE_NAME4c, &g_3DModel4c, textureModel4c))
		return 0;
	if(!g_Load3ds.Load3DSFile(FILE_NAME5c, &g_3DModel5c, textureModel5c))
		return 0;
	if(!g_Load3ds.Load3DSFile(FILE_NAME6c, &g_3DModel6c, textureModel6c))
		return 0;
	if(!g_Load3ds.Load3DSFile(FILE_NAME7c, &g_3DModel7c, textureModel7c))
		return 0;
	if(!g_Load3ds.Load3DSFile(FILE_NAME8c, &g_3DModel8c, textureModel8c))
		return 0;
	if(!g_Load3ds.Load3DSFile(FILE_NAME9c, &g_3DModel9c, textureModel9c))
		return 0;
	if(!g_Load3ds.Load3DSFile(FILE_NAME10c, &g_3DModel10c, textureModel10c))
		return 0;
	if(!g_Load3ds.Load3DSFile(FILE_NAME11c, &g_3DModel11c, textureModel11c))
		return 0;
	if(!g_Load3ds.Load3DSFile(FILE_NAME12c, &g_3DModel12c, textureModel12c))
		return 0;
	return TRUE;

}

// Si le hacen cambios a esto le tienen que hacer cambios a lo de arriba
void DescargaModelos()
{
	g_Load3ds.UnLoad3DSFile(&g_3DModel1c, textureModel1c);
	g_Load3ds.UnLoad3DSFile(&g_3DModel2c, textureModel2c);
	g_Load3ds.UnLoad3DSFile(&g_3DModel3c, textureModel3c);
	g_Load3ds.UnLoad3DSFile(&g_3DModel4c, textureModel4c);
	g_Load3ds.UnLoad3DSFile(&g_3DModel5c, textureModel5c);
	g_Load3ds.UnLoad3DSFile(&g_3DModel6c, textureModel6c);
	g_Load3ds.UnLoad3DSFile(&g_3DModel7c, textureModel7c);
	g_Load3ds.UnLoad3DSFile(&g_3DModel8c, textureModel8c);
	g_Load3ds.UnLoad3DSFile(&g_3DModel9c, textureModel9c);
	g_Load3ds.UnLoad3DSFile(&g_3DModel10c, textureModel10c);
	g_Load3ds.UnLoad3DSFile(&g_3DModel11c, textureModel11c);
	g_Load3ds.UnLoad3DSFile(&g_3DModel12c, textureModel12c);
	
}

/* No tocar sin razon XD supongo que se tocara para ajustar al Mario */
void InicializaParametrosdeControl()
{
	//Esta función establece los parámetros como velocidad del objeto y distancia de la cámara así como la posición y dirección iniciales

	player1.VelocidadObj = 0.6f;
	player1.DistanciaCam = 15.0f;

	player1.CamaraPosAlt = 5.0f;	//Posición en y de la cámara (altura a la que se situa la cámara)
	player1.CamaraObjAlt = 4.0f;	//Posición en y del objetivo de la cámara (altura a la que ve la cámara)
	player1.AngDir = 270.0f;		//Este ángulo inicial hace que la dirección inicial sea paralela al eje Z y con sentido negativo
	player1.AngObj = 0.0f;		//Este valor se elige dependiendo de la orientación con la que aparece el modelo en la escena al dibujarlo
								//sin aplicarle ninguna transformación (hacia adonde está volteando). Se elige un ángulo tal que al aplicarle
								//una rotación inicial con respecto al eje Y esté viendo hacia la misma dirección que la definida por AngDir
	
	player1.PosicionObj = CVector( 0.0f, 0.0f, 50.0f); //Esta es la posición inicial del objeto en la escena
	player1.Direccion = CVector( 0.0f, 0.0f, -1.0f);   //Dirección inicial definida por el ángulo inicial AngDir (x=cos(AngDir), y=0.0, z=sen(AngDir))
	player1.PosicionCam = CVector( 0.0f, player1.CamaraPosAlt, player1.DistanciaCam + 50.0f ); //Posición inicial de la cámara a [DistanciaCam] unidades detrás del objeto
	player1.ObjetivoCam = player1.PosicionObj;		//La cámara ve siempre al objeto
	player1.ObjetivoCam.y = player1.CamaraObjAlt;		//Para que no vea a los "pies" del objeto (personaje)
}

//FMOD LULZ
void ERRCHECK(FMOD_RESULT result)
{
    if (result != FMOD_OK)
    {
        printf("FMOD error! (%d) %s\n", result, FMOD_ErrorString(result));
        exit(-1);
    }
}

void LiberaSonido(FMOD_SYSTEM *system, FMOD_RESULT result)
{
	result = FMOD_Sound_Release(sound[0]);
    ERRCHECK(result);
    result = FMOD_Sound_Release(sound[1]);
    ERRCHECK(result);
	result = FMOD_Sound_Release(sound[2]);
    ERRCHECK(result);
    result = FMOD_System_Close(system);
    ERRCHECK(result);
    result = FMOD_System_Release(system);
    ERRCHECK(result);
}

// Texturas que habra que recargar...tu tum pas
void CargaTexturas()
{
	textura[ 0 ].LoadTGA("Texturas/platav1.tga");
	textura[ 1 ].LoadTGA("Texturas/platav.tga");
	textura[ 2 ].LoadTGA("Texturas/cuboverde1.tga");
	textura[ 3 ].LoadTGA("Texturas/cuboverde.tga");
	textura[ 4 ].LoadTGA("Texturas/cuborojo1.tga");
	textura[ 5 ].LoadTGA("Texturas/platar.tga");
	textura[ 6 ].LoadTGA("Texturas/platar1.tga");
	textura[ 7 ].LoadTGA("Texturas/escav.tga");
	textura[ 8 ].LoadTGA("Texturas/escar.tga");
	textura[ 9 ].LoadTGA("Texturas/escav1.tga");
	textura[ 10 ].LoadTGA("Texturas/cuborojo.tga");
	textura[ 11 ].LoadTGA("Texturas/escar1.tga");
	textura[ 12 ].LoadTGA("Texturas/cuboazul.tga");
	textura[ 13 ].LoadTGA("Texturas/escaa.tga");
	textura[ 14 ].LoadTGA("Texturas/escaa1.tga");
	textura[ 10 ].LoadTGA("Texturas/cuborojo.tga");
	textura[ 18 ].LoadTGA( "Texturas/stewie.tga" );
	textura[ 19 ].LoadTGA( "Texturas/suelo2.tga" );
	textura[ 20 ].LoadTGA( "Texturas/pared.tga" );
	textura[ 21 ].LoadTGA("Texturas/escenario.tga");
	textura[ 22 ].LoadTGA("Texturas/edMario.tga");
	textura[ 23 ].LoadTGA("Texturas/edMaba.tga");
	textura[ 30 ].LoadTGA("Texturas/marioind.tga");
	//textura[ 26 ].LoadTGA("Texturas/escenario.tga");
	//textura[ 27 ].LoadTGA("Texturas/escenario.tga");
	//textura[ 28 ].LoadTGA("Texturas/escenario.tga");
}

// Igual que arriba, para descargar nuestras texturas
void DescargaTexturas()
{
	textura[ 0 ].Elimina();
	textura[ 1 ].Elimina();
	textura[ 2 ].Elimina();
	textura[ 3 ].Elimina();
	textura[ 4 ].Elimina();
	textura[ 5 ].Elimina();
	textura[ 6 ].Elimina();
	textura[ 7 ].Elimina();
	textura[ 8 ].Elimina();
	textura[ 9 ].Elimina();
	textura[ 10 ].Elimina();
	textura[ 11 ].Elimina();
	textura[ 12 ].Elimina();
	textura[ 13 ].Elimina();
	textura[ 14 ].Elimina();
	textura[ 15 ].Elimina();
	textura[ 16 ].Elimina();
	textura[ 17 ].Elimina();
	textura[ 18 ].Elimina();
	textura[ 19 ].Elimina();
	textura[ 20 ].Elimina();
}

void InicializaAnim()
{
	//Se inicializan las variables de la secuencia 1
	for(int i=0; i < maxKF1; i++)
	{
		KeyFrame1[ i ].Angt1 = 0.0f;
		KeyFrame1[ i ].Angt2 = 0.0f;
		KeyFrame1[ i ].Angc = 0.0f;
		KeyFrame1[ i ].Angbi1 = 0.0f;
		KeyFrame1[ i ].Angbi2 = 0.0f;
		KeyFrame1[ i ].Angbd1 = 0.0f;
		KeyFrame1[ i ].Angbd2 = 0.0f;
		KeyFrame1[ i ].Angpizq = 0.0f;
		KeyFrame1[ i ].Angpder = 0.0f;
		KeyFrame1[ i ].Angpi = 0.0f;
		KeyFrame1[ i ].Angpd = 0.0f;
		KeyFrame1[ i ].Xtor = 0.0f;
		KeyFrame1[ i ].Ytor = 0.0f;
		KeyFrame1[ i ].Ztor = 0.0f;

		KeyFrame1[ i ].incAngt1=false;
		KeyFrame1[ i ].incAngt1=false;
		KeyFrame1[ i ].incAngc=false;
		KeyFrame1[ i ].incAngbi1=false;
		KeyFrame1[ i ].incAngbi2=false;
		KeyFrame1[ i ].incAngbd1=false;
		KeyFrame1[ i ].incAngbd2=false;
		KeyFrame1[ i ].incAngpizq=false;
		KeyFrame1[ i ].incAngpder=false;
		KeyFrame1[ i ].incAngpi=false;
		KeyFrame1[ i ].incAngpd=false;
		KeyFrame1[ i ].incXtor=false;
		KeyFrame1[ i ].incYtor=false;
		KeyFrame1[ i ].incZtor=false;
	}

	// Se inicializan las variables de la secuencia 2
	for(int i = 0; i < maxKF2; i++)
	{
		KeyFrame2[ i ].Angt1 = 0.0f;
		KeyFrame2[ i ].Angt2 = 0.0f;
		KeyFrame2[ i ].Angc = 0.0f;
		KeyFrame2[ i ].Angbi1 = 0.0f;
		KeyFrame2[ i ].Angbi2 = 0.0f;
		KeyFrame2[ i ].Angbd1 = 0.0f;
		KeyFrame2[ i ].Angbd2 = 0.0f;
		KeyFrame2[ i ].Angpizq = 0.0f;
		KeyFrame2[ i ].Angpder = 0.0f;
		KeyFrame2[ i ].Angpi = 0.0f;
		KeyFrame2[ i ].Angpd = 0.0f;
		KeyFrame2[ i ].Xtor = 0.0f;
		KeyFrame2[ i ].Ytor = 0.0f;
		KeyFrame2[ i ].Ztor = 0.0f;

		KeyFrame2[ i ].incAngt1 = false;
		KeyFrame2[ i ].incAngt1 = false;
		KeyFrame2[ i ].incAngc = false;
		KeyFrame2[ i ].incAngbi1 =false;
		KeyFrame2[ i ].incAngbi2 = false;
		KeyFrame2[ i ].incAngbd1 = false;
		KeyFrame2[ i ].incAngbd2 = false;
		KeyFrame2[ i ].incAngpizq = false;
		KeyFrame2[ i ].incAngpder = false;
		KeyFrame2[ i ].incAngpi = false;
		KeyFrame2[ i ].incAngpd = false;
		KeyFrame2[ i ].incXtor = false;
		KeyFrame2[ i ].incYtor = false;
		KeyFrame2[ i ].incZtor = false;

	}
}

// Estos son los datos de nuestras KeyFrames, cambiar es necesario
void DatosAnimacion()
{
	//caminar
	KeyFrame1[0].Angt2=10.0;
	KeyFrame1[0].Angc=0.0;
	KeyFrame1[0].Angbi1=0.0;
	KeyFrame1[0].Angbi2=0.0;
	KeyFrame1[0].Angbd1=0.0;
	KeyFrame1[0].Angbd2=0.0;
	KeyFrame1[0].Angpizq=0.0;
	KeyFrame1[0].Angpder=0.0;
	KeyFrame1[0].Angpi=0.0;
	KeyFrame1[0].Angpd=0.0;
	KeyFrame1[0].Xtor=0.0;
	KeyFrame1[0].Ytor=0.0;
	KeyFrame1[0].Ztor=0.0;


	KeyFrame1[1].Angt1 = 0.0f;
	KeyFrame1[1].Angt2 = 12.0f;
	KeyFrame1[1].Angc = 0.0f;
	KeyFrame1[1].Angbi1 = -0.0f;
	KeyFrame1[1].Angbi2 = 43.0f;
	KeyFrame1[1].Angbd1 = 0.0f;
	KeyFrame1[1].Angbd2 = -60.0f;
	KeyFrame1[1].Angpizq = -26.0f;
	KeyFrame1[1].Angpder = 62.0f;
	KeyFrame1[1].Angpi = -0.0f;
	KeyFrame1[1].Xtor = 0.0f;
	KeyFrame1[1].Ytor = 0.0f;

	
	KeyFrame1[2].Angt1 = 0.0f;
	KeyFrame1[2].Angt2 = 10.0f;
	KeyFrame1[2].Angc = 0.0f;
	KeyFrame1[2].Angbi1 = -0.0f;
	KeyFrame1[2].Angbi2 = -71.0f;
	KeyFrame1[2].Angbd1 = -7.0f;
	KeyFrame1[2].Angbd2 = 84.0f;
	KeyFrame1[2].Angpizq = 59.0f;
	KeyFrame1[2].Angpder = -20.0f;
	KeyFrame1[2].Angpi = -0.0f;
	KeyFrame1[2].Xtor = 0.0f;
	KeyFrame1[2].Ytor = 0.0f;
	KeyFrame1[2].Ztor = 0.0f;

	KeyFrame1[3].Angt2=12.0;
	KeyFrame1[3].Angc=0.0;
	KeyFrame1[3].Angbi1=0.0;
	KeyFrame1[3].Angbi2=0.0;
	KeyFrame1[3].Angbd1=0.0;
	KeyFrame1[3].Angbd2=0.0;
	KeyFrame1[3].Angpizq=0.0;
	KeyFrame1[3].Angpder=0.0;
	KeyFrame1[3].Angpi=0.0;
	KeyFrame1[3].Angpd=0.0;
	KeyFrame1[3].Xtor=0.0;
	KeyFrame1[3].Ytor=0.0;
	KeyFrame1[3].Ztor=0.0;


	KeyFrame1[3].Angt2=10.0;
	KeyFrame1[3].Angc=0.0;
	KeyFrame1[3].Angbi1=0.0;
	KeyFrame1[3].Angbi2=0.0;
	KeyFrame1[3].Angbd1=0.0;
	KeyFrame1[3].Angbd2=0.0;
	KeyFrame1[3].Angpizq=0.0;
	KeyFrame1[3].Angpder=0.0;
	KeyFrame1[3].Angpi=0.0;
	KeyFrame1[3].Angpd=0.0;
	KeyFrame1[3].Xtor=0.0;
	KeyFrame1[3].Ytor=0.0;
	KeyFrame1[3].Ztor=0.0;



	//saltar
	KeyFrame2[0].Angt2=0.0;
	KeyFrame2[0].Angc=0.0;
	KeyFrame2[0].Angbi1=0.0;
	KeyFrame2[0].Angbi2=0.0;
	KeyFrame2[0].Angbd1=0.0;
	KeyFrame2[0].Angbd2=0.0;
	KeyFrame2[0].Angpizq=0.0;
	KeyFrame2[0].Angpder=0.0;
	KeyFrame2[0].Angpi=0.0;
	KeyFrame2[0].Angpd=0.0;
	KeyFrame2[0].Xtor=0.0;
	KeyFrame2[0].Ytor=0.0;
	KeyFrame2[0].Ztor=0.0;


	KeyFrame2[1].Angt2=0.0;
	KeyFrame2[1].Angc=0.0;
	KeyFrame2[1].Angbi1=-9.0;
	KeyFrame2[1].Angbi2=50.0;
	KeyFrame2[1].Angbd1=1.0;
	KeyFrame2[1].Angbd2=-27.0;
	KeyFrame2[1].Angpizq=20.0;
	KeyFrame2[1].Angpder=-58.0;
	KeyFrame2[1].Angpi=-4.0;
	KeyFrame2[1].Angpd=-5.0;
	KeyFrame2[1].Xtor=0.0;
	KeyFrame2[1].Ytor=0.5;
	KeyFrame2[1].Ztor=0.8;

	KeyFrame2[2].Angt2=0.0;
	KeyFrame2[2].Angc=0.0;
	KeyFrame2[2].Angbi1=-5.0;
	KeyFrame2[2].Angbi2=-4.0;
	KeyFrame2[2].Angbd1=3.0;
	KeyFrame2[2].Angbd2=-158.0;
	KeyFrame2[2].Angpizq=3.0;
	KeyFrame2[2].Angpder=-10.0;
	KeyFrame2[2].Angpi=3.0;
	KeyFrame2[2].Angpd=-0.5;
	KeyFrame2[2].Xtor=0.0;
	KeyFrame2[2].Ytor=9.3;
	KeyFrame2[2].Ztor=0.0;

	KeyFrame2[3].Angt2=0.0;
	KeyFrame2[3].Angc=0.0;
	KeyFrame2[3].Angbi1=-1.0;
	KeyFrame2[3].Angbi2=-42.0;
	KeyFrame2[3].Angbd1=5.0;
	KeyFrame2[3].Angbd2=-158.0;
	KeyFrame2[3].Angpizq=-32.0;
	KeyFrame2[3].Angpder=30.0;
	KeyFrame2[3].Angpi=10.0;
	KeyFrame2[3].Angpd=4.0;
	KeyFrame2[3].Xtor=0.0;
	KeyFrame2[3].Ytor=12.3;
	KeyFrame2[3].Ztor=0.0;

	KeyFrame2[4].Angt2=0.0;
	KeyFrame2[4].Angc=0.0;
	KeyFrame2[4].Angbi1=-1.0;
	KeyFrame2[4].Angbi2=-42.0;
	KeyFrame2[4].Angbd1=5.0;
	KeyFrame2[4].Angbd2=-158.0;
	KeyFrame2[4].Angpizq=-32.0;
	KeyFrame2[4].Angpder=30.0;
	KeyFrame2[4].Angpi=10.0;
	KeyFrame2[4].Angpd=4.0;
	KeyFrame2[4].Xtor=0.0;
	KeyFrame2[4].Ytor=9.3;
	KeyFrame2[4].Ztor=0.0;

	KeyFrame2[5].Angt2=0.0;
	KeyFrame2[5].Angc=0.0;
	KeyFrame2[5].Angbi1=13.0;
	KeyFrame2[5].Angbi2=-2.0;
	KeyFrame2[5].Angbd1=-12.0;
	KeyFrame2[5].Angbd2=-22.0;
	KeyFrame2[5].Angpizq=-30.0;
	KeyFrame2[5].Angpder=-40.0;
	KeyFrame2[5].Angpi=7.0;
	KeyFrame2[5].Angpd=6.0;
	KeyFrame2[5].Xtor=0.0;
	KeyFrame2[5].Ytor=0.0;
	KeyFrame2[5].Ztor=0.8;


	KeyFrame2[6].Angt2=0.0;
	KeyFrame2[6].Angc=0.0;
	KeyFrame2[6].Angbi1=0.0;
	KeyFrame2[6].Angbi2=0.0;
	KeyFrame2[6].Angbd1=0.0;
	KeyFrame2[6].Angbd2=0.0;
	KeyFrame2[6].Angpizq=0.0;
	KeyFrame2[6].Angpder=0.0;
	KeyFrame2[6].Angpi=0.0;
	KeyFrame2[6].Angpd=0.0;
	KeyFrame2[6].Xtor=0.0;
	KeyFrame2[6].Ytor=0.0;
	KeyFrame2[6].Ztor=0.0;


}

// Poner atencion a este
int InitGL( GLvoid )										// Aqui se configuran los parametros iniciales de OpenGL
{
	glShadeModel( GL_SMOOTH );							// Activa Smooth Shading
	glClearColor( 0.451f, 0.647f, 0.996f, 0.5f );				// Fondo negro
	glClearDepth( 1.0f );									// Valor para el Depth Buffer
	glEnable( GL_DEPTH_TEST );							// Activa Depth Testing
	glDepthFunc( GL_LEQUAL );								// Tipo de Depth Testing a usar
	glHint( GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST );	// Correccion de cálculos de perspectiva

	glCullFace( GL_BACK );								// Set Culling Face To Back Face
	glEnable( GL_CULL_FACE );								// Enable Culling

	glLightfv( GL_LIGHT0, GL_POSITION, LightPos );		// Posicion de la luz1
	glLightfv( GL_LIGHT0, GL_AMBIENT,  LightAmb );		// Componente ambiente
	glLightfv( GL_LIGHT0, GL_DIFFUSE,  LightDif );		// Componente difusa
	glLightfv( GL_LIGHT0, GL_SPECULAR, LightSpc );		// Componente especular

	// Bright white light - full intensity RGB values
	GLfloat ambientLight[] = { 1.0f, 1.0f, 1.0f, 1.0f };

	glEnable( GL_LIGHT0 );					// Activa luz2
	glEnable( GL_LIGHTING );					// Habilita la iluminación

	// Set light model to use ambient light specified by ambientLight[]
	glLightModelfv( GL_LIGHT_MODEL_AMBIENT, ambientLight );

	CargaTexturas();
	Prim.CargaTexturas();
	Font.BuildFont();
	Multitext.InitMultitext( hWnd );

	aText1 = 0.0f;
	aText2 = 0.0f;


	if( !CargaModelos() )
		return FALSE;

	e = gluNewQuadric();

	InicializaParametrosdeControl();
	// Ordenes :3
	Angt1 = 0.0f;
	Angt2 = 0.0f;
	Angc = 0.0f;
	Angbi1 = 0.0f;
	Angbi2 = 0.0f;
	Angbd1 = 0.0f;
	Angbd2 = 0.0f;
	Angpizq = 0.0f;
	Angpder = 0.0f;
	Angpi = 0.0f;
	Angpd = 0.0f;

	Xtor = 0.0f;
	Ytor = 0.0f;
	Ztor = 0.0f;

	alcance = 0.0f;

	IncDec = 0; //0: Incrementa valores, 1: Decrementa valores

	InicializaAnim();
	//Descomentar la sig. línea hasta que se hayan definido las variables de todos los keyframes de las secuencias
	DatosAnimacion();

	return TRUE;										
}

void animacion( FRAME *KeyFrame, int maxKF , int frames )
{
	if( play )
	{		
		if(( abs( KeyFrame[ playIndex + 1 ].Angt1 - Angt1 )) < 0.1 &&
		   ( abs( KeyFrame[ playIndex + 1 ].Angt2 - Angt2 )) < 0.1 &&
		   ( abs( KeyFrame[ playIndex + 1 ].Angc - Angc )) < 0.1 &&
		   ( abs( KeyFrame[ playIndex + 1 ].Angbi1 - Angbi1 )) < 0.1 &&
		   ( abs( KeyFrame[ playIndex + 1 ].Angbi2 - Angbi2 )) < 0.1 &&
		   ( abs( KeyFrame[ playIndex + 1 ].Angbd1 - Angbd1 )) < 0.1 &&
		   ( abs( KeyFrame[ playIndex + 1 ].Angbd2 - Angbd2 )) < 0.1 &&
		   ( abs( KeyFrame[ playIndex + 1 ].Angpizq - Angpizq )) < 0.1 &&
		   ( abs( KeyFrame[ playIndex + 1 ].Angpder - Angpder )) < 0.1 &&
		   ( abs( KeyFrame[ playIndex + 1 ].Angpi - Angpi )) < 0.1 &&
		   ( abs( KeyFrame[ playIndex + 1 ].Angpd - Angpd )) < 0.1 &&
		   ( abs( KeyFrame[ playIndex + 1 ].Xtor - Xtor )) < 0.1 &&
		   ( abs( KeyFrame[ playIndex + 1 ].Ytor - Ytor )) < 0.1 &&
		   ( abs( KeyFrame[ playIndex + 1 ].Ztor - Ztor )) < 0.1 )
		{			
			playIndex++;			
			if( playIndex > maxKF - 2 )
			{
				playIndex = 0;
				play = false;
								
			}
		}
		else
		{
			KeyFrame[ playIndex ].incAngt1   = ( KeyFrame[ playIndex + 1 ].Angt1 - KeyFrame[ playIndex ].Angt1 ) / frames;
			KeyFrame[ playIndex ].incAngt2   = ( KeyFrame[ playIndex + 1 ].Angt2 - KeyFrame[ playIndex ].Angt2 ) / frames;
			KeyFrame[ playIndex ].incAngc    = ( KeyFrame[ playIndex + 1 ].Angc - KeyFrame[ playIndex ].Angc ) / frames;
			KeyFrame[ playIndex ].incAngbi1  = ( KeyFrame[ playIndex + 1 ].Angbi1 - KeyFrame[ playIndex ].Angbi1 ) / frames;
			KeyFrame[ playIndex ].incAngbi2  = ( KeyFrame[ playIndex + 1 ].Angbi2 - KeyFrame[ playIndex ].Angbi2 ) / frames;
			KeyFrame[ playIndex ].incAngbd1  = ( KeyFrame[ playIndex + 1 ].Angbd1 - KeyFrame[ playIndex ].Angbd1 ) / frames;
			KeyFrame[ playIndex ].incAngbd2  = ( KeyFrame[ playIndex + 1 ].Angbd2 - KeyFrame[ playIndex ].Angbd2 ) / frames;
			KeyFrame[ playIndex ].incAngpizq = ( KeyFrame[ playIndex + 1 ].Angpizq - KeyFrame[ playIndex ].Angpizq ) / frames;
			KeyFrame[ playIndex ].incAngpder = ( KeyFrame[ playIndex + 1 ].Angpder - KeyFrame[ playIndex ].Angpder ) / frames;
			KeyFrame[ playIndex ].incAngpi   = ( KeyFrame[ playIndex + 1 ].Angpi - KeyFrame[ playIndex ].Angpi ) / frames;
			KeyFrame[ playIndex ].incAngpd   = ( KeyFrame[ playIndex + 1 ].Angpd - KeyFrame[ playIndex ].Angpd ) / frames;
			KeyFrame[ playIndex ].incXtor    = ( KeyFrame[ playIndex + 1 ].Xtor - KeyFrame[ playIndex ].Xtor ) / frames;
			KeyFrame[ playIndex ].incYtor    = ( KeyFrame[ playIndex + 1 ].Ytor - KeyFrame[ playIndex ].Ytor ) / frames;
			KeyFrame[ playIndex ].incZtor    = ( KeyFrame[ playIndex + 1 ].Ztor - KeyFrame[ playIndex ].Ztor ) / frames;
			
			Angt1   += KeyFrame[ playIndex ].incAngt1;
			Angt2   += KeyFrame[ playIndex ].incAngt2;
			Angc    += KeyFrame[ playIndex ].incAngc;
			Angbi1  += KeyFrame[ playIndex ].incAngbi1;
			Angbi2  += KeyFrame[ playIndex ].incAngbi2;
			Angbd1  += KeyFrame[ playIndex ].incAngbd1;
			Angbd2  += KeyFrame[ playIndex ].incAngbd2;
			Angpizq += KeyFrame[ playIndex ].incAngpizq;
			Angpder += KeyFrame[ playIndex ].incAngpder;
			Angpi   += KeyFrame[ playIndex ].incAngpi;
			Angpd   += KeyFrame[ playIndex ].incAngpd;
			Xtor    += KeyFrame[ playIndex ].incXtor;
			Ytor    += KeyFrame[ playIndex ].incYtor;
			Ztor    += KeyFrame[ playIndex ].incZtor;

			// Salto falso de mario
			player1.PosicionObj.y += KeyFrame[ playIndex ].incYtor;
			player1.CamaraPosAlt += KeyFrame[ playIndex ].incYtor;
			player1.CamaraObjAlt += KeyFrame[ playIndex ].incYtor;

			player1.PosicionCam.y = player1.CamaraPosAlt;
			player1.ObjetivoCam = player1.PosicionObj;
			player1.ObjetivoCam.y = player1.CamaraObjAlt;

		}
	}
}

/* Este si debe de ser modificado, para, pues, ya saben, hacer que el Mario se adapte a nuestras condiciones
 * luego discutimos como hacer esto bien.
 * 
 */
void ControlPersonaje( int funcion )
{
	if( funcion == 1 ) // Giro a la derecha
	{

		player1.AngDir += 1.25f;
		if(player1.AngDir > 360.0f)
			player1.AngDir -= 360.0f;
		
		player1.AngObj -= 1.25f;
		if( player1.AngObj < 0.0f )
			player1.AngObj += 360.0f;

		player1.Direccion.x = cosf( player1.AngDir * PI / 180.0f );
		player1.Direccion.y = 0.0f;
		player1.Direccion.z = sinf( player1.AngDir * PI / 180.0f );

		player1.PosicionCam = player1.PosicionObj - player1.Direccion * player1.DistanciaCam;
		player1.PosicionCam.y = player1.CamaraPosAlt;
		player1.ObjetivoCam = player1.PosicionObj;
		player1.ObjetivoCam.y = player1.CamaraObjAlt;
	}
	else if( funcion == 2 ) // Giro a la izquierda
	{
		player1.AngDir -= 1.25f;
		if( player1.AngDir < 0.0f )
			player1.AngDir += 360.0f;
		
		player1.AngObj += 1.25f;
		if( player1.AngObj > 360.0f )
			player1.AngObj -= 360.0f;

		player1.Direccion.x = cosf( player1.AngDir * PI / 180.0f );
		player1.Direccion.y = 0.0f;
		player1.Direccion.z = sinf( player1.AngDir * PI / 180.0f );

		player1.PosicionCam = player1.PosicionObj - player1.Direccion * player1.DistanciaCam;
		player1.PosicionCam.y = player1.CamaraPosAlt;
		player1.ObjetivoCam = player1.PosicionObj;
		player1.ObjetivoCam.y = player1.CamaraObjAlt;

	}
	// Acelera, aqui se implementan (quiza del peor modo posible) el ascenso y descenso de una rampa
	else if(funcion == 3)  
	{
			player1.PosicionObj = player1.PosicionObj + player1.Direccion * player1.VelocidadObj;

			player1.PosicionCam = player1.PosicionObj - player1.Direccion * player1.DistanciaCam;
			player1.PosicionCam.y = player1.CamaraPosAlt;
			player1.ObjetivoCam = player1.PosicionObj;
			player1.ObjetivoCam.y = player1.CamaraObjAlt;
	}
	else if( funcion == 4 ) //Reversa, al igual que Acelera, una serie de IFs anidados que resuelven el problema
	{
			player1.PosicionObj = player1.PosicionObj - player1.Direccion * player1.VelocidadObj;

			player1.PosicionCam = player1.PosicionObj - player1.Direccion * player1.DistanciaCam;
			player1.PosicionCam.y = player1.CamaraPosAlt;
			player1.ObjetivoCam = player1.PosicionObj;
			player1.ObjetivoCam.y = player1.CamaraObjAlt;
	}
	else if(funcion == 5) //Sube objetivo de la cámara
	{
		player1.ObjetivoCam.y += 1.0f;
		player1.CamaraObjAlt += 1.0f;
	}
	else if(funcion == 6) //Baja objetivo de la cámara
	{
		player1.ObjetivoCam.y -= 1.0f;
		player1.CamaraObjAlt -= 1.0f;
	}

}

// Esta deberia de ser DibujaBolaFuego o algo asi... XD
void DibujaRayo()
{
	glDisable( GL_LIGHTING );
	glEnable( GL_BLEND );
	glBlendFunc( GL_ONE, GL_ONE );

	glColor4f( 0.9f, 0.9f,0.0f,0.5f);

	glPushMatrix();
		gluCylinder( e, 0.07f, 0.07f, 0.2f, 24, 1);
		glTranslatef( 0.0f, 0.0f, 0.2f );
		gluDisk( e, 0.0f, 0.07f, 24,1 );
		glRotatef( -180.0f, 1.0f, 0.0f, 0.0f);
		glTranslatef( 0.0f, 0.0f, 0.2f );
		gluDisk( e, 0.0f, 0.07f, 24, 1 );
	glPopMatrix();

	glColor4f( 1.0f, 1.0f, 1.0f, 1.0f );

	glDisable( GL_BLEND );
	glEnable( GL_LIGHTING );
}

// Modificar coordenadas aqui para dibujar a Mario y no a Stewie
void DibujaPersonaje()
{
	//Torso
	glPushMatrix();
	glTranslatef(Xtor,0.0,0.0);
	glTranslatef(0.0,Ytor,0.0);
	glTranslatef(0.0,0.5,Ztor);
	glTranslatef(0.0,0.5,0.0);
		glRotatef(Angt1,0.0,1.0,0.0);
		glRotatef(Angt2,1.0,0.0,0.0);
		g_Load3ds.Render3DSFile(&g_3DModel1c, textureModel1c, 1);
		//Cabeza
		glPushMatrix();
			glRotatef(Angc,0.0,1.0,0.0);
			g_Load3ds.Render3DSFile(&g_3DModel2c, textureModel2c, 1);
		glPopMatrix();
		//Pierna derecha
		glPushMatrix();
			glTranslatef(-0.85,-0.2,1.6);
			glRotatef(Angpder,1.0,0.0,0.0);
			g_Load3ds.Render3DSFile(&g_3DModel3c, textureModel3c, 1);
			//Pie derecho
			glPushMatrix();
				glTranslatef(-0.2,-1.5,0.20);
				glRotatef(Angpd+35,0.0,1.0,0.0);
				g_Load3ds.Render3DSFile(&g_3DModel5c, textureModel5c, 1);
			glPopMatrix();	
		glPopMatrix();
	//	Pierna izquierda
		glPushMatrix();
			glTranslatef(0.35,-0.2,1.6);
			glTranslatef(0.0,0.3,0.0);
			glRotatef(Angpizq,1.0,0.0,0.0);
			glTranslatef(0.0,-0.3,0.0);
			g_Load3ds.Render3DSFile(&g_3DModel4c, textureModel4c, 1);
			//Pie izquierdo
			glPushMatrix();
				glTranslatef(0.5,-1.5,0.15);
				glRotatef(Angpi-45,0.0,1.0,0.0);
				g_Load3ds.Render3DSFile(&g_3DModel6c, textureModel6c, 1);
			glPopMatrix();
		glPopMatrix();
//		Brazo derecho
		glPushMatrix();
			glTranslatef(-1.05,1.4,2.15);
			glRotatef(Angbd2,1.0,0.0,0.0);
			glRotatef(Angbd1,0.0,0.0,1.0);
			g_Load3ds.Render3DSFile(&g_3DModel7c, textureModel7c, 1);
		glPopMatrix();
		//Brazo izquierdo
		glPushMatrix();
			glTranslatef(0.47,1.71,1.85);
			glRotatef(Angbi2,1.0,0.0,0.0);
			glRotatef(Angbi1,0.0,0.0,1.0);
			g_Load3ds.Render3DSFile(&g_3DModel8c, textureModel8c, 1);
		glPopMatrix();
	glPopMatrix();

}
void DibujaGoomba()
{
	//Torso
	glPushMatrix();
//	glRotatef(Angg1,0.0,1.0,0.0);
//	glRotatef(Angg2,1.0,0.0,0.0);
	g_Load3ds.Render3DSFile(&g_3DModel10c, textureModel10c, 1);
			//Pie derecho
			glPushMatrix();
				glTranslatef(-0.8,-0.2,0.0);
	//			glRotatef(Angpgd,1.0,0.0,0.0);
				g_Load3ds.Render3DSFile(&g_3DModel11c, textureModel11c, 1);
			glPopMatrix();	
			//Pie izquierdo
			glPushMatrix();
				glTranslatef(0.8,-0.2,0.0);
	//			glRotatef(Angpgi,1.0,0.0,0.0);
				g_Load3ds.Render3DSFile(&g_3DModel12c, textureModel12c, 1);
			glPopMatrix();
	glPopMatrix();
}

// Funcion para obtener colores al azar
float randColor()
{
	return rand() / ( ( float )( RAND_MAX ) + 1 );
}
void DibujaLuz(CVector l)
{
	//Dibuja una esfera que representa la fuente luminosa
	glDisable(GL_LIGHTING);									// Deshabilita iluminación
	glPushMatrix();
		glTranslatef(l.x, l.y, l.z);// Traslada a la posicion de la luz
		glColor4f(1.0f, 0.9f, 0.0f, 1.0f);					// Color amarillo
		gluSphere(e, 1.0f, 16, 8);							// Dibuja la esfera
	glPopMatrix();
	glEnable(GL_LIGHTING);									// Habilita Iluminación

	glColor3f(1.0f, 1.0f, 1.0f);
}
// Aqui me quede limpiando
void DibujaEjes()
{
	glColor3f(1.0f,0.0f,0.0f);

	glBegin(GL_LINES);
		//Eje X
		glVertex3f(-100.0f, 0.0f, 0.0f);
		glVertex3f( 100.0f, 0.0f, 0.0f);

		//Eje Y
		glVertex3f(0.0f, -100.0f, 0.0f);
		glVertex3f(0.0f,  100.0f, 0.0f);

		//Eje Z
		glVertex3f(0.0f, 0.0f, -100.0f);
		glVertex3f(0.0f, 0.0f,  100.0f);
	glEnd();

	glColor3f(1.0f,1.0f,1.0f);
}

void ActualizaAnimText()
{
	if(aText1 < 1.0f)
		aText1+=0.01f;
	else
		aText1=0.0f;

	if(aText2 < 1.0f)
		aText2+=0.001f;
	else
		aText2=0.0f;
}

// Cambiar por los necesarios, alguien vaya extrayendo los indicadores 2D
void DibujaIndicadores2D()
{
	glDisable(GL_DEPTH_TEST);
	glMatrixMode(GL_PROJECTION);
	glPushMatrix();
	glLoadIdentity();
	glOrtho(0,glWidth,0,glHeight,-1,1);
	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();
	glDisable(GL_LIGHTING);
	glLoadIdentity();

	glColor3f(1.0,1.0,1.0);
	
	glEnable(GL_TEXTURE_2D);

	glAlphaFunc(GL_GREATER, 0.8f);
	glEnable(GL_ALPHA_TEST);

	glBindTexture(GL_TEXTURE_2D, textura[30].texID);

    //Dibujar el indicador de energia del personaje aquí

	glPushMatrix();
		glTranslatef( glWidth * 0.02, glHeight * 0.90, 0.0f );
		glBegin(GL_QUADS);
			glTexCoord2f( 1.0f, 0.0f ); glVertex2i( 0, 0 );
			glTexCoord2f( 0.0f, 0.0f ); glVertex2i( 50, 0 );
			glTexCoord2f( 0.0f, 1.0f ); glVertex2i( 50, 50 );
			glTexCoord2f( 1.0f, 1.0f ); glVertex2i( 0, 50 );
		glEnd();
	glPopMatrix();


    //Dibujar los textos aquí
	Font.glPrint(glHeight/220,glWidth*.30f, glHeight*.92f,"Score:%d",puntos);
	Font.glPrint(glHeight/262,glWidth*.8f, glHeight*.9f,"Time:%d sec",tiempo);
	Font.glPrint(glHeight/220, glWidth*.1f, glHeight*.9f,"x %d", lives);

	glDisable(GL_ALPHA_TEST);
	glDisable(GL_TEXTURE_2D);
	
	glColor3f(1.0,1.0,1.0);
	
	glEnable(GL_LIGHTING);
	glMatrixMode(GL_PROJECTION);
	glPopMatrix();
	glMatrixMode(GL_MODELVIEW);
	glPopMatrix();
	glEnable(GL_DEPTH_TEST);
		
}


// Esta función es la encargada de dibujar el suelo, muy importante :3
void DibujaAlbin()
{
	// Dibuja la superficie del suelo
	glDisable( GL_LIGHTING );
	glEnable( GL_TEXTURE_2D );
	glBindTexture( GL_TEXTURE_2D, textura[ 19 ].texID );
	Prim.glDibujaCubo2( 300.0f, 0.0f, 190.0f );
	glDisable(GL_TEXTURE_2D);
	glEnable( GL_LIGHTING );

	// Dibuja el volumen en el suelo
	glDisable( GL_LIGHTING );
	glEnable( GL_TEXTURE_2D );
	glBindTexture( GL_TEXTURE_2D, textura[ 20 ].texID );
	glPushMatrix();
		glTranslatef( 0.0f, -10.1f, 0.0f );
		Prim.glDibujaCubo2( 300.0f, 20.0f, 190.0f );
	glPopMatrix();
	glDisable(GL_TEXTURE_2D);
	glEnable( GL_LIGHTING );

	// Mario Toy Factory
	glEnable( GL_TEXTURE_2D );
	glEnable(GL_ALPHA_TEST);
	glAlphaFunc(GL_GREATER, 0.8f);
	glBindTexture( GL_TEXTURE_2D, textura[ 23 ].texID );
	glPushMatrix();
		glTranslatef( -70.0f, 5.0f, -40.0f );
		glBegin( GL_QUADS );
			Prim.glDibujaCubo( 50.0f, 10.0f, 25.0f );
			glTranslatef( 0.0f, 50.0f, 0.0f );
			glBindTexture( GL_TEXTURE_2D, textura[ 22 ].texID );
			Prim.glDibujaCubo( 30.0f, 100.0f, 20.5f );
		glEnd();
	glPopMatrix();
	glDisable(GL_TEXTURE_2D);
	glDisable(GL_ALPHA_TEST);

	// Otras fabricas genericas

	// Bodega chica izq
	glPushMatrix();
		glTranslatef( 15.0f, 0.5f, -40.0f );
		Prim.glDibujaCubo3( 10.0f, 25.0f, 20.0f );
		glTranslatef( 0.0f, 17.5f, 0.0f );
		Prim.glDibujaPrismaTri( 10.0f, 10.0f, 20.0f );
	glPopMatrix();

	// Bodega central con techitos triangulares
	glPushMatrix();
		glTranslatef( 57.25f, 15.5f, -40.0f );
		Prim.glDibujaCubo3( 74.75f, 35.0f, 20.0f );
		glTranslatef( 0.0f, 25.0f, 0.0f );
		// Parte esferica de la fabrica -> ya no, no era esferica XD ups ya es noche
		glPushMatrix();
			glTranslatef( -30.0f, 0.0f, 0.0f );
			Prim.glDibujaPrismaTri( 15.0f, 15.0f, 20.0f );
			glTranslatef( 37.5f, 0.0f, 0.0f );
			//glRotatef( 90.0f, 1.0f, 0.0f, 0.0f );
			Prim.glDibujaCubo3( 60.0f, 15.0f, 20.0f );
		glPopMatrix();
		// Otras Partes triangulares de la fabrica
		glTranslatef( 0.0f, 15.0f, 0.0f );
		Prim.glDibujaPrismaTri( 15.0f, 15.0f, 20.0f );
		glTranslatef( 15.0f, 0.0f, 0.0f );
		Prim.glDibujaPrismaTri( 15.0f, 15.0f, 20.0f );
		glTranslatef( 15.0f, 0.0f, 0.0f );
		Prim.glDibujaPrismaTri( 15.0f, 15.0f, 20.0f );
	glPopMatrix();

	// Dibuja Puerta
	glPushMatrix();
		glTranslatef( 72.0f, 9.0f, -29.9f );
		Prim.Plano( 12.0f, 9.0f, 0.0f, 1.0f, 10, true ); 
	glPopMatrix();

	// Dibuja Otra Puerta
	glPushMatrix();
		glTranslatef( 48.0f, 9.0f, -29.9f );
		Prim.Plano( 12.0f, 9.0f, 0.0f, 1.0f, 10, true ); 
	glPopMatrix();

	// 3 Bodegas genericas en el fondo
	glPushMatrix();
		glTranslatef( -40.0f, 5.0f, -80.0f );
		glScalef( 0.6f, 0.6f, 0.6f );
		DibujaBodega();
		glTranslatef( 40.0f, 0.0f, 0.0f );
		DibujaBodega();
		glTranslatef( 40.0f, 0.0f, 0.0f );
		DibujaBodega();
	glPopMatrix();

	// Dibujando el domo
	glPushMatrix();
		glDisable( GL_LIGHTING );
		glRotatef( 90.0f, 0.0f, 1.0f, 0.0f );
		glTranslatef( 0.0f, -30.0f, 0.0f);
		Prim.DibujaDomoCielo( 190.0f, 30, 30 );
		glEnable( GL_LIGHTING );
	glPopMatrix();

	// Dibujando el resto del escenario de atras en forma de una pared
	/* 
	glDisable( GL_LIGHTING );
	glEnable( GL_TEXTURE_2D );
	glBindTexture( GL_TEXTURE_2D, textura[ 21 ].texID );
	glPushMatrix();
		glDisable(	GL_LIGHTING );
		glRotatef( 180.0f, 0.0f, 0.0f, 1.0f );
		glTranslatef( 0.0f, -50.0f, -90.0f );
		Prim.glDibujaCubo( 300.0f, 200.0f, 0.0f );
		glEnable( GL_LIGHTING );
	glPopMatrix();
	glDisable(GL_TEXTURE_2D);
	glEnable( GL_LIGHTING );
	*/

}

void DibujaEscenario1()
{
	glEnable(GL_TEXTURE_2D);
	//CubosRojosAbajo
		glBindTexture(GL_TEXTURE_2D, textura[10].texID);
		//Cara delantera
		glBegin(GL_QUADS);
		glNormal3f( 0.0f,0.0f,1.0f );
		glTexCoord2f(0.0f, 0.0f); glVertex3f( -1.5f, 0.0f, 1.5f );
		glTexCoord2f(1.0f, 0.0f); glVertex3f( 1.5f, 0.0f, 1.5f );
		glTexCoord2f(1.0f, 3.0f); glVertex3f( 1.5f, 9.0f, 1.5f );
		glTexCoord2f(0.0f, 3.0f); glVertex3f( -1.5f, 9.0f, 1.5f );
		glEnd();
		//Cara derecha
		glBegin(GL_QUADS);
		glNormal3f( 1.0f,0.0f,0.0f );
		glTexCoord2f(0.0f, 0.0f); glVertex3f( 1.5f, 0.0f, 1.5f );
		glTexCoord2f(1.0f, 0.0f); glVertex3f( 1.5f, 0.0f, -1.5f );
		glTexCoord2f(1.0f, 3.0f); glVertex3f( 1.5f, 9.0f, -1.5f );
		glTexCoord2f(0.0f, 3.0f); glVertex3f( 1.5f, 9.0f, 1.5f );
		glEnd();
		//Cara izquierda
		glBegin(GL_QUADS);
		glNormal3f( -1.0f,0.0f,0.0f );
		glTexCoord2f(1.0f, 0.0f); glVertex3f( -1.5f, 0.0f, 1.5f );
		glTexCoord2f(1.0f, 3.0f); glVertex3f( -1.5f, 9.0f, 1.5f );
		glTexCoord2f(0.0f, 3.0f); glVertex3f( -1.5f, 9.0f, -1.5f );
		glTexCoord2f(0.0f, 0.0f); glVertex3f( -1.5f, 0.0f, -1.5f );
		glEnd();
		//Cara trasera
		glBegin(GL_QUADS);
		glNormal3f( 0.0f,0.0f,-1.0f );
		glTexCoord2f(1.0f, 0.0f); glVertex3f( -1.5f, 0.0f, -1.5f );
		glTexCoord2f(1.0f, 3.0f); glVertex3f( -1.5f, 9.0f, -1.5f );
		glTexCoord2f(0.0f, 3.0f); glVertex3f( 1.5f, 9.0f, -1.5f );
		glTexCoord2f(0.0f, 0.0f); glVertex3f( 1.5f, 0.0f, -1.5f );
		glEnd();
		//Cara superior
		glBindTexture(GL_TEXTURE_2D, textura[4].texID);
		glBegin(GL_QUADS);
		glNormal3f( 0.0f,1.0f,0.0f );
		glTexCoord2f(0.0f, 0.0f); glVertex3f( -1.5f, 9.0f, 1.5f );
		glTexCoord2f(1.0f, 0.0f); glVertex3f( 1.5f, 9.0f, 1.5f );
		glTexCoord2f(1.0f, 1.0f); glVertex3f( 1.5f, 9.0f, -1.5f );
		glTexCoord2f(0.0f, 1.0f); glVertex3f( -1.5f, 9.0f, -1.5f );
		glEnd();
		//Cara inferior
		glBindTexture(GL_TEXTURE_2D, textura[4].texID);
		glBegin(GL_QUADS);
		glNormal3f( 0.0f,-1.0f,0.0f );
		glTexCoord2f(0.0f, 0.0f); glVertex3f( -1.5f, 0.0f, 1.5f );
		glTexCoord2f(1.0f, 0.0f); glVertex3f( -1.5f, 0.0f, -1.5f );
		glTexCoord2f(1.0f, 1.0f); glVertex3f( 1.5f, 0.0f, -1.5f );
		glTexCoord2f(0.0f, 1.0f); glVertex3f( 1.5f, 0.0f, 1.5f );
		glEnd();

	//CubosAzulesAbajo
		glPushMatrix();
		glTranslatef(12.0,0,0);
		glBindTexture(GL_TEXTURE_2D, textura[12].texID);
		//Cara delantera
		glBegin(GL_QUADS);
		glNormal3f( 0.0f,0.0f,1.0f );
		glTexCoord2f(0.0f, 0.0f); glVertex3f( -1.5f, 0.0f, 1.5f );
		glTexCoord2f(1.0f, 0.0f); glVertex3f( 1.5f, 0.0f, 1.5f );
		glTexCoord2f(1.0f, 3.0f); glVertex3f( 1.5f, 9.0f, 1.5f );
		glTexCoord2f(0.0f, 3.0f); glVertex3f( -1.5f, 9.0f, 1.5f );
		glEnd();
		//Cara derecha
		glBegin(GL_QUADS);
		glNormal3f( 1.0f,0.0f,0.0f );
		glTexCoord2f(0.0f, 0.0f); glVertex3f( 1.5f, 0.0f, 1.5f );
		glTexCoord2f(1.0f, 0.0f); glVertex3f( 1.5f, 0.0f, -1.5f );
		glTexCoord2f(1.0f, 3.0f); glVertex3f( 1.5f, 9.0f, -1.5f );
		glTexCoord2f(0.0f, 3.0f); glVertex3f( 1.5f, 9.0f, 1.5f );
		glEnd();
		//Cara izquierda
		glBegin(GL_QUADS);
		glNormal3f( -1.0f,0.0f,0.0f );
		glTexCoord2f(1.0f, 0.0f); glVertex3f( -1.5f, 0.0f, 1.5f );
		glTexCoord2f(1.0f, 3.0f); glVertex3f( -1.5f, 9.0f, 1.5f );
		glTexCoord2f(0.0f, 3.0f); glVertex3f( -1.5f, 9.0f, -1.5f );
		glTexCoord2f(0.0f, 0.0f); glVertex3f( -1.5f, 0.0f, -1.5f );
		glEnd();
		//Cara trasera
		glBegin(GL_QUADS);
		glNormal3f( 0.0f,0.0f,-1.0f );
		glTexCoord2f(1.0f, 0.0f); glVertex3f( -1.5f, 0.0f, -1.5f );
		glTexCoord2f(1.0f, 3.0f); glVertex3f( -1.5f, 9.0f, -1.5f );
		glTexCoord2f(0.0f, 3.0f); glVertex3f( 1.5f, 9.0f, -1.5f );
		glTexCoord2f(0.0f, 0.0f); glVertex3f( 1.5f, 0.0f, -1.5f );
		glEnd();
		//Cara superior
		glBegin(GL_QUADS);
		glNormal3f( 0.0f,1.0f,0.0f );
		glTexCoord2f(0.0f, 0.0f); glVertex3f( -1.5f, 9.0f, 1.5f );
		glTexCoord2f(1.0f, 0.0f); glVertex3f( 1.5f, 9.0f, 1.5f );
		glTexCoord2f(1.0f, 1.0f); glVertex3f( 1.5f, 9.0f, -1.5f );
		glTexCoord2f(0.0f, 1.0f); glVertex3f( -1.5f, 9.0f, -1.5f );
		glEnd();
		//Cara inferior
		glBegin(GL_QUADS);
		glNormal3f( 0.0f,-1.0f,0.0f );
		glTexCoord2f(0.0f, 0.0f); glVertex3f( -1.5f, 0.0f, 1.5f );
		glTexCoord2f(1.0f, 0.0f); glVertex3f( -1.5f, 0.0f, -1.5f );
		glTexCoord2f(1.0f, 1.0f); glVertex3f( 1.5f, 0.0f, -1.5f );
		glTexCoord2f(0.0f, 1.0f); glVertex3f( 1.5f, 0.0f, 1.5f );
		glEnd();
		glPopMatrix();

//CubosAzulesEnmedio
		glPushMatrix();
		glTranslatef(-12.0,0.0,0.0);
		glBindTexture(GL_TEXTURE_2D, textura[12].texID);
		//Cara delantera
		glBegin(GL_QUADS);
		glNormal3f( 0.0f,0.0f,1.0f );
		glTexCoord2f(0.0f, 0.0f); glVertex3f( 10.5f, 15.0f, 1.5f );
		glTexCoord2f(1.0f, 0.0f); glVertex3f( 13.5f, 15.0f, 1.5f );
		glTexCoord2f(1.0f, 3.0f); glVertex3f( 13.5f, 23.0f, 1.5f );
		glTexCoord2f(0.0f, 3.0f); glVertex3f( 10.5f, 23.0f, 1.5f );
		glEnd();
		//Cara derecha
		glBegin(GL_QUADS);
		glNormal3f( 1.0f,0.0f,0.0f );
		glTexCoord2f(0.0f, 0.0f); glVertex3f( 13.5f, 15.0f, 1.5f );
		glTexCoord2f(1.0f, 0.0f); glVertex3f( 13.5f, 15.0f, -1.5f );
		glTexCoord2f(1.0f, 3.0f); glVertex3f( 13.5f, 23.0f, -1.5f );
		glTexCoord2f(0.0f, 3.0f); glVertex3f( 13.5f, 23.0f, 1.5f );
		glEnd();
		//Cara izquierda
		glBegin(GL_QUADS);
		glNormal3f( -1.0f,0.0f,0.0f );
		glTexCoord2f(1.0f, 0.0f); glVertex3f( 10.5f, 15.0f, 1.5f );
		glTexCoord2f(1.0f, 3.0f); glVertex3f( 10.5f, 23.0f, 1.5f );
		glTexCoord2f(0.0f, 3.0f); glVertex3f( 10.5f, 23.0f, -1.5f );
		glTexCoord2f(0.0f, 0.0f); glVertex3f( 10.5f, 15.0f, -1.5f );
		glEnd();
		//Cara trasera
		glBegin(GL_QUADS);
		glNormal3f( 0.0f,0.0f,-1.0f );
		glTexCoord2f(1.0f, 0.0f); glVertex3f( 10.5f, 15.0f, -1.5f );
		glTexCoord2f(1.0f, 3.0f); glVertex3f( 10.5f, 23.0f, -1.5f );
		glTexCoord2f(0.0f, 3.0f); glVertex3f( 13.5f, 23.0f, -1.5f );
		glTexCoord2f(0.0f, 0.0f); glVertex3f( 13.5f, 15.0f, -1.5f );
		glEnd();
		//Cara superior
		glBindTexture(GL_TEXTURE_2D, textura[4].texID);
		glBegin(GL_QUADS);
		glNormal3f( 0.0f,1.0f,0.0f );
		glTexCoord2f(0.0f, 0.0f); glVertex3f( 10.5f, 23.0f, 1.5f );
		glTexCoord2f(1.0f, 0.0f); glVertex3f( 13.5f, 23.0f, 1.5f );
		glTexCoord2f(1.0f, 1.0f); glVertex3f( 13.5f, 23.0f, -1.5f );
		glTexCoord2f(0.0f, 1.0f); glVertex3f( 10.5f, 23.0f, -1.5f );
		glEnd();
		//Cara inferior
		glBindTexture(GL_TEXTURE_2D, textura[4].texID);
		glBegin(GL_QUADS);
		glNormal3f( 0.0f,-1.0f,0.0f );
		glTexCoord2f(0.0f, 0.0f); glVertex3f( 10.5f, 15.0f, 1.5f );
		glTexCoord2f(1.0f, 0.0f); glVertex3f( 10.5f, 15.0f, -1.5f );
		glTexCoord2f(1.0f, 1.0f); glVertex3f( 13.5f, 15.0f, -1.5f );
		glTexCoord2f(0.0f, 1.0f); glVertex3f( 13.5f, 15.0f, 1.5f );
		glEnd();
		glPopMatrix();

	//CubosVerdesEnmedio
		glBindTexture(GL_TEXTURE_2D, textura[3].texID);
		//Cara delantera
		glBegin(GL_QUADS);
		glNormal3f( 0.0f,0.0f,1.0f );
		glTexCoord2f(0.0f, 0.0f); glVertex3f( -1.5f, 9.0f, 1.5f );
		glTexCoord2f(5.0f, 0.0f); glVertex3f( 13.5f, 9.0f, 1.5f );
		glTexCoord2f(5.0f, 2.0f); glVertex3f( 13.5f, 15.0f, 1.5f );
		glTexCoord2f(0.0f, 2.0f); glVertex3f( -1.5f, 15.0f, 1.5f );
		glEnd();
		//Cara derecha
		glBegin(GL_QUADS);
		glNormal3f( 1.0f,0.0f,0.0f );
		glTexCoord2f(0.0f, 0.0f); glVertex3f( 13.5f, 9.0f, 1.5f );
		glTexCoord2f(1.0f, 0.0f); glVertex3f( 13.5f, 9.0f, -1.5f );
		glTexCoord2f(1.0f, 2.0f); glVertex3f( 13.5f, 15.0f, -1.5f );
		glTexCoord2f(0.0f, 2.0f); glVertex3f( 13.5f, 15.0f, 1.5f );
		glEnd();
		//Cara izquierda
		glBegin(GL_QUADS);
		glNormal3f( -1.0f,0.0f,0.0f );
		glTexCoord2f(1.0f, 0.0f); glVertex3f( -1.5f, 9.0f, 1.5f );
		glTexCoord2f(1.0f, 2.0f); glVertex3f( -1.5f, 15.0f, 1.5f );
		glTexCoord2f(0.0f, 2.0f); glVertex3f( -1.5f, 15.0f, -1.5f );
		glTexCoord2f(0.0f, 0.0f); glVertex3f( -1.5f, 9.0f, -1.5f );
		glEnd();
		//Cara trasera
		glBegin(GL_QUADS);
		glNormal3f( 0.0f,0.0f,-1.0f );
		glTexCoord2f(5.0f, 0.0f); glVertex3f( -1.5f, 9.0f, -1.5f );
		glTexCoord2f(5.0f, 2.0f); glVertex3f( -1.5f, 15.0f, -1.5f );
		glTexCoord2f(0.0f, 2.0f); glVertex3f( 13.5f, 15.0f, -1.5f );
		glTexCoord2f(0.0f, 0.0f); glVertex3f( 13.5f, 9.0f, -1.5f );
		glEnd();
		//Cara superior
		glBindTexture(GL_TEXTURE_2D, textura[2].texID);
		glBegin(GL_QUADS);
		glNormal3f( 0.0f,1.0f,0.0f );
		glTexCoord2f(0.0f, 0.0f); glVertex3f( -1.5f, 15.0f, 1.5f );
		glTexCoord2f(5.0f, 0.0f); glVertex3f( 13.5f, 15.0f, 1.5f );
		glTexCoord2f(5.0f, 1.0f); glVertex3f( 13.5f, 15.0f, -1.5f );
		glTexCoord2f(0.0f, 1.0f); glVertex3f( -1.5f, 15.0f, -1.5f );
		glEnd();
		//Cara inferior
		glBindTexture(GL_TEXTURE_2D, textura[2].texID);
		glBegin(GL_QUADS);
		glNormal3f( 0.0f,-1.0f,0.0f );
		glTexCoord2f(0.0f, 0.0f); glVertex3f( -1.5f, 9.0f, 1.5f );
		glTexCoord2f(1.0f, 0.0f); glVertex3f( -1.5f, 9.0f, -1.5f );
		glTexCoord2f(1.0f, 5.0f); glVertex3f( 13.5f, 9.0f, -1.5f );
		glTexCoord2f(0.0f, 5.0f); glVertex3f( 13.5f, 9.0f, 1.5f );
		glEnd();
	//CubosVerdesAbajo
		glBindTexture(GL_TEXTURE_2D, textura[3].texID);
		//Cara delantera
		glBegin(GL_QUADS);
		glNormal3f( 0.0f,0.0f,1.0f );
		glTexCoord2f(0.0f, 0.0f); glVertex3f( -43.5f, 0.0f, 1.5f );
		glTexCoord2f(5.0f, 0.0f); glVertex3f( -28.5f, 0.0f, 1.5f );
		glTexCoord2f(5.0f, 1.0f); glVertex3f( -28.5f, 3.0f, 1.5f );
		glTexCoord2f(0.0f, 1.0f); glVertex3f( -43.5f, 3.0f, 1.5f );
		glEnd();
		//Cara derecha
		glBegin(GL_QUADS);
		glNormal3f( 1.0f,0.0f,0.0f );
		glTexCoord2f(0.0f, 0.0f); glVertex3f( -28.5f, 0.0f, 1.5f );
		glTexCoord2f(1.0f, 0.0f); glVertex3f( -28.5f, 0.0f, -1.5f );
		glTexCoord2f(1.0f, 1.0f); glVertex3f( -28.5f, 3.0f, -1.5f );
		glTexCoord2f(0.0f, 1.0f); glVertex3f( -28.5f, 3.0f, 1.5f );
		glEnd();
		//Cara izquierda
		glBegin(GL_QUADS);
		glNormal3f( -1.0f,0.0f,0.0f );
		glTexCoord2f(1.0f, 0.0f); glVertex3f( -43.5f, 0.0f, 1.5f );
		glTexCoord2f(1.0f, 1.0f); glVertex3f( -43.5f, 3.0f, 1.5f );
		glTexCoord2f(0.0f, 1.0f); glVertex3f( -43.5f, 3.0f, -1.5f );
		glTexCoord2f(0.0f, 0.0f); glVertex3f( -43.5f, 0.0f, -1.5f );
		glEnd();
		//Cara trasera
		glBegin(GL_QUADS);
		glNormal3f( 0.0f,0.0f,-1.0f );
		glTexCoord2f(5.0f, 0.0f); glVertex3f( -43.5f, 0.0f, -1.5f );
		glTexCoord2f(5.0f, 1.0f); glVertex3f( -43.5f, 3.0f, -1.5f );
		glTexCoord2f(0.0f, 1.0f); glVertex3f( -28.5f, 3.0f, -1.5f );
		glTexCoord2f(0.0f, 0.0f); glVertex3f( -28.5f, 0.0f, -1.5f );
		glEnd();
		//Cara superior
		glBindTexture(GL_TEXTURE_2D, textura[2].texID);
		glBegin(GL_QUADS);
		glNormal3f( 0.0f,1.0f,0.0f );
		glTexCoord2f(0.0f, 0.0f); glVertex3f( -43.5f, 3.0f, 1.5f );
		glTexCoord2f(5.0f, 0.0f); glVertex3f( -28.5f, 3.0f, 1.5f );
		glTexCoord2f(5.0f, 1.0f); glVertex3f( -28.5f, 3.0f, -1.5f );
		glTexCoord2f(0.0f, 1.0f); glVertex3f( -43.5f, 3.0f, -1.5f );
		glEnd();
		//Cara inferior
		glBindTexture(GL_TEXTURE_2D, textura[2].texID);
		glBegin(GL_QUADS);
		glNormal3f( 0.0f,-1.0f,0.0f );
		glTexCoord2f(0.0f, 0.0f); glVertex3f( -43.5f, 0.0f, 1.5f );
		glTexCoord2f(1.0f, 0.0f); glVertex3f( -43.5f, 0.0f, -1.5f );
		glTexCoord2f(1.0f, 5.0f); glVertex3f( -28.5f, 0.0f, -1.5f );
		glTexCoord2f(0.0f, 5.0f); glVertex3f( -28.5f, 0.0f, 1.5f );
		glEnd();
	//CubosRojosEnmedio
		glBindTexture(GL_TEXTURE_2D, textura[10].texID);
		//Cara delantera
		glBegin(GL_QUADS);
		glNormal3f( 0.0f,0.0f,1.0f );
		glTexCoord2f(0.0f, 0.0f); glVertex3f( 10.5f, 15.0f, 1.5f );
		glTexCoord2f(1.0f, 0.0f); glVertex3f( 13.5f, 15.0f, 1.5f );
		glTexCoord2f(1.0f, 3.0f); glVertex3f( 13.5f, 23.0f, 1.5f );
		glTexCoord2f(0.0f, 3.0f); glVertex3f( 10.5f, 23.0f, 1.5f );
		glEnd();
		//Cara derecha
		glBegin(GL_QUADS);
		glNormal3f( 1.0f,0.0f,0.0f );
		glTexCoord2f(0.0f, 0.0f); glVertex3f( 13.5f, 15.0f, 1.5f );
		glTexCoord2f(1.0f, 0.0f); glVertex3f( 13.5f, 15.0f, -1.5f );
		glTexCoord2f(1.0f, 3.0f); glVertex3f( 13.5f, 23.0f, -1.5f );
		glTexCoord2f(0.0f, 3.0f); glVertex3f( 13.5f, 23.0f, 1.5f );
		glEnd();
		//Cara izquierda
		glBegin(GL_QUADS);
		glNormal3f( -1.0f,0.0f,0.0f );
		glTexCoord2f(1.0f, 0.0f); glVertex3f( 10.5f, 15.0f, 1.5f );
		glTexCoord2f(1.0f, 3.0f); glVertex3f( 10.5f, 23.0f, 1.5f );
		glTexCoord2f(0.0f, 3.0f); glVertex3f( 10.5f, 23.0f, -1.5f );
		glTexCoord2f(0.0f, 0.0f); glVertex3f( 10.5f, 15.0f, -1.5f );
		glEnd();
		//Cara trasera
		glBegin(GL_QUADS);
		glNormal3f( 0.0f,0.0f,-1.0f );
		glTexCoord2f(1.0f, 0.0f); glVertex3f( 10.5f, 15.0f, -1.5f );
		glTexCoord2f(1.0f, 3.0f); glVertex3f( 10.5f, 23.0f, -1.5f );
		glTexCoord2f(0.0f, 3.0f); glVertex3f( 13.5f, 23.0f, -1.5f );
		glTexCoord2f(0.0f, 0.0f); glVertex3f( 13.5f, 15.0f, -1.5f );
		glEnd();
		//Cara superior
		glBindTexture(GL_TEXTURE_2D, textura[4].texID);
		glBegin(GL_QUADS);
		glNormal3f( 0.0f,1.0f,0.0f );
		glTexCoord2f(0.0f, 0.0f); glVertex3f( 10.5f, 23.0f, 1.5f );
		glTexCoord2f(1.0f, 0.0f); glVertex3f( 13.5f, 23.0f, 1.5f );
		glTexCoord2f(1.0f, 1.0f); glVertex3f( 13.5f, 23.0f, -1.5f );
		glTexCoord2f(0.0f, 1.0f); glVertex3f( 10.5f, 23.0f, -1.5f );
		glEnd();
		//Cara inferior
		glBindTexture(GL_TEXTURE_2D, textura[4].texID);
		glBegin(GL_QUADS);
		glNormal3f( 0.0f,-1.0f,0.0f );
		glTexCoord2f(0.0f, 0.0f); glVertex3f( 10.5f, 15.0f, 1.5f );
		glTexCoord2f(1.0f, 0.0f); glVertex3f( 10.5f, 15.0f, -1.5f );
		glTexCoord2f(1.0f, 1.0f); glVertex3f( 13.5f, 15.0f, -1.5f );
		glTexCoord2f(0.0f, 1.0f); glVertex3f( 13.5f, 15.0f, 1.5f );
		glEnd();
	//CubosRojosArriba
		glBindTexture(GL_TEXTURE_2D, textura[10].texID);
		//Cara delantera
		glBegin(GL_QUADS);
		glNormal3f( 0.0f,0.0f,1.0f );
		glTexCoord2f(0.0f, 0.0f); glVertex3f( -43.5f, 23.0f, 1.5f );
		glTexCoord2f(3.0f, 0.0f); glVertex3f( -34.5f, 23.0f, 1.5f );
		glTexCoord2f(3.0f, 2.0f); glVertex3f( -34.5f, 29.0f, 1.5f );
		glTexCoord2f(0.0f, 2.0f); glVertex3f( -43.5f, 29.0f, 1.5f );
		glEnd();
		//Cara derecha
		glBegin(GL_QUADS);
		glNormal3f( 1.0f,0.0f,0.0f );
		glTexCoord2f(0.0f, 0.0f); glVertex3f( -34.5f, 23.0f, 1.5f );
		glTexCoord2f(1.0f, 0.0f); glVertex3f( -34.5f, 23.0f, -1.5f );
		glTexCoord2f(1.0f, 2.0f); glVertex3f( -34.5f, 29.0f, -1.5f );
		glTexCoord2f(0.0f, 2.0f); glVertex3f( -34.5f, 29.0f, 1.5f );
		glEnd();
		//Cara izquierda
		glBegin(GL_QUADS);
		glNormal3f( -1.0f,0.0f,0.0f );
		glTexCoord2f(1.0f, 0.0f); glVertex3f( -43.5f, 23.0f, 1.5f );
		glTexCoord2f(1.0f, 2.0f); glVertex3f( -43.5f, 29.0f, 1.5f );
		glTexCoord2f(0.0f, 2.0f); glVertex3f( -43.5f, 29.0f, -1.5f );
		glTexCoord2f(0.0f, 0.0f); glVertex3f( -43.5f, 23.0f, -1.5f );
		glEnd();
		//Cara trasera
		glBegin(GL_QUADS);
		glNormal3f( 0.0f,0.0f,-1.0f );
		glTexCoord2f(3.0f, 0.0f); glVertex3f( -43.5f, 23.0f, -1.5f );
		glTexCoord2f(3.0f, 2.0f); glVertex3f( -43.5f, 29.0f, -1.5f );
		glTexCoord2f(0.0f, 2.0f); glVertex3f( -34.5f, 29.0f, -1.5f );
		glTexCoord2f(0.0f, 0.0f); glVertex3f( -34.5f, 23.0f, -1.5f );
		glEnd();
		//Cara superior
		glBindTexture(GL_TEXTURE_2D, textura[4].texID);
		glBegin(GL_QUADS);
		glNormal3f( 0.0f,1.0f,0.0f );
		glTexCoord2f(0.0f, 0.0f); glVertex3f( -43.5f, 29.0f, 1.5f );
		glTexCoord2f(3.0f, 0.0f); glVertex3f( -34.5f, 29.0f, 1.5f );
		glTexCoord2f(3.0f, 1.0f); glVertex3f( -34.5f, 29.0f, -1.5f );
		glTexCoord2f(0.0f, 1.0f); glVertex3f( -43.5f, 29.0f, -1.5f );
		glEnd();
		//Cara inferior
		glBindTexture(GL_TEXTURE_2D, textura[4].texID);
		glBegin(GL_QUADS);
		glNormal3f( 0.0f,-1.0f,0.0f );
		glTexCoord2f(0.0f, 0.0f); glVertex3f( -43.5f, 23.0f, 1.5f );
		glTexCoord2f(1.0f, 0.0f); glVertex3f( -43.5f, 23.0f, -1.5f );
		glTexCoord2f(1.0f, 3.0f); glVertex3f( -34.5f, 23.0f, -1.5f );
		glTexCoord2f(0.0f, 3.0f); glVertex3f( -34.5f, 23.0f, 1.5f );
		glEnd();
	//CubosVerdesArriba
		glBindTexture(GL_TEXTURE_2D, textura[3].texID);
		//Cara delantera
		glBegin(GL_QUADS);
		glNormal3f( 0.0f,0.0f,1.0f );
		glTexCoord2f(0.0f, 0.0f); glVertex3f( -10.5f, 23.0f, 1.5f );
		glTexCoord2f(8.0f, 0.0f); glVertex3f( 13.5f, 23.0f, 1.5f );
		glTexCoord2f(8.0f, 1.0f); glVertex3f( 13.5f, 26.0f, 1.5f );
		glTexCoord2f(0.0f, 1.0f); glVertex3f( -10.5f, 26.0f, 1.5f );
		glEnd();
		//Cara derecha
		glBegin(GL_QUADS);
		glNormal3f( 1.0f,0.0f,0.0f );
		glTexCoord2f(0.0f, 0.0f); glVertex3f( 13.5f, 23.0f, 1.5f );
		glTexCoord2f(1.0f, 0.0f); glVertex3f( 13.5f, 23.0f, -1.5f );
		glTexCoord2f(1.0f, 1.0f); glVertex3f( 13.5f, 26.0f, -1.5f );
		glTexCoord2f(0.0f, 1.0f); glVertex3f( 13.5f, 26.0f, 1.5f );
		glEnd();
		//Cara izquierda
		glBegin(GL_QUADS);
		glNormal3f( -1.0f,0.0f,0.0f );
		glTexCoord2f(1.0f, 0.0f); glVertex3f( -10.5f, 23.0f, 1.5f );
		glTexCoord2f(1.0f, 1.0f); glVertex3f( -10.5f, 26.0f, 1.5f );
		glTexCoord2f(0.0f, 1.0f); glVertex3f( -10.5f, 26.0f, -1.5f );
		glTexCoord2f(0.0f, 0.0f); glVertex3f( -10.5f, 23.0f, -1.5f );
		glEnd();
		//Cara trasera
		glBegin(GL_QUADS);
		glNormal3f( 0.0f,0.0f,-1.0f );
		glTexCoord2f(8.0f, 0.0f); glVertex3f( -10.5f, 23.0f, -1.5f );
		glTexCoord2f(8.0f, 1.0f); glVertex3f( -10.5f, 26.0f, -1.5f );
		glTexCoord2f(0.0f, 1.0f); glVertex3f( 13.5f, 26.0f, -1.5f );
		glTexCoord2f(0.0f, 0.0f); glVertex3f( 13.5f, 23.0f, -1.5f );
		glEnd();
		//Cara superior
		glBindTexture(GL_TEXTURE_2D, textura[2].texID);
		glBegin(GL_QUADS);
		glNormal3f( 0.0f,1.0f,0.0f );
		glTexCoord2f(0.0f, 0.0f); glVertex3f( -10.5f, 26.0f, 1.5f );
		glTexCoord2f(8.0f, 0.0f); glVertex3f( 13.5f, 26.0f, 1.5f );
		glTexCoord2f(8.0f, 1.0f); glVertex3f( 13.5f, 26.0f, -1.5f );
		glTexCoord2f(0.0f, 1.0f); glVertex3f( -10.5f, 26.0f, -1.5f );
		glEnd();
		//Cara inferior
		glBindTexture(GL_TEXTURE_2D, textura[2].texID);
		glBegin(GL_QUADS);
		glNormal3f( 0.0f,-1.0f,0.0f );
		glTexCoord2f(0.0f, 0.0f); glVertex3f( -10.5f, 23.0f, 1.5f );
		glTexCoord2f(1.0f, 0.0f); glVertex3f( -10.5f, 23.0f, -1.5f );
		glTexCoord2f(1.0f, 8.0f); glVertex3f( 13.5f, 23.0f, -1.5f );
		glTexCoord2f(0.0f, 8.0f); glVertex3f( 13.5f, 23.0f, 1.5f );
		glEnd();
	//PlataformaVerdeArribaIzq
		//Cara delantera
		glBindTexture(GL_TEXTURE_2D, textura[1].texID);
		glBegin(GL_QUADS);
		glNormal3f( 0.0f,0.0f,1.0f );
		glTexCoord2f(0.0f, 0.0f); glVertex3f( -43.5f, 21.5f, 1.5f );
		glTexCoord2f(10.0f, 0.0f); glVertex3f( -10.5f, 21.5f, 1.5f );
		glTexCoord2f(10.0f, 1.0f); glVertex3f( -10.5f, 23.0f, 1.5f );
		glTexCoord2f(0.0f, 1.0f); glVertex3f( -43.5f, 23.0f, 1.5f );
		glEnd();
		//Cara derecha
		glBegin(GL_QUADS);
		glNormal3f( 1.0f,0.0f,0.0f );
		glTexCoord2f(0.0f, 0.0f); glVertex3f( -10.5f, 21.5f, 1.5f );
		glTexCoord2f(1.0f, 0.0f); glVertex3f( -10.5f, 21.5f, -1.5f );
		glTexCoord2f(1.0f, 1.0f); glVertex3f( -10.5f, 23.0f, -1.5f );
		glTexCoord2f(0.0f, 1.0f); glVertex3f( -10.5f, 23.0f, 1.5f );
		glEnd();
		//Cara izquierda
		glBegin(GL_QUADS);
		glNormal3f( -1.0f,0.0f,0.0f );
		glTexCoord2f(1.0f, 0.0f); glVertex3f( -43.5f, 21.5f, 1.5f );
		glTexCoord2f(1.0f, 1.0f); glVertex3f( -43.5f, 23.0f, 1.5f );
		glTexCoord2f(0.0f, 1.0f); glVertex3f( -43.5f, 23.0f, -1.5f );
		glTexCoord2f(0.0f, 0.0f); glVertex3f( -43.5f, 21.5f, -1.5f );
		glEnd();
		//Cara trasera
		glBegin(GL_QUADS);
		glNormal3f( 0.0f,0.0f,-1.0f );
		glTexCoord2f(10.0f, 0.0f); glVertex3f( -43.5f, 21.5f, -1.5f );
		glTexCoord2f(10.0f, 1.0f); glVertex3f( -43.5f, 23.0f, -1.5f );
		glTexCoord2f(0.0f, 1.0f); glVertex3f( -10.5f, 23.0f, -1.5f );
		glTexCoord2f(0.0f, 0.0f); glVertex3f( -10.5f, 21.5f, -1.5f );
		glEnd();
		//Cara superior
		glBindTexture(GL_TEXTURE_2D, textura[0].texID);
		glBegin(GL_QUADS);
		glNormal3f( 0.0f,1.0f,0.0f );
		glTexCoord2f(0.0f, 0.0f); glVertex3f( -43.5f, 23.0f, 1.5f );
		glTexCoord2f(10.0f, 0.0f); glVertex3f( -10.5f, 23.0f, 1.5f );
		glTexCoord2f(10.0f, 1.0f); glVertex3f( -10.5f, 23.0f, -1.5f );
		glTexCoord2f(0.0f, 1.0f); glVertex3f( -43.5f, 23.0f, -1.5f );
		glEnd();
		//Cara inferior
		glBegin(GL_QUADS);
		glNormal3f( 0.0f,-1.0f,0.0f );
		glTexCoord2f(0.0f, 0.0f); glVertex3f( -43.5f, 21.5f, 1.5f );
		glTexCoord2f(1.0f, 0.0f); glVertex3f( -43.5f, 21.5f, -1.5f );
		glTexCoord2f(1.0f, 10.0f); glVertex3f( -10.5f, 21.5f, -1.5f );
		glTexCoord2f(0.0f, 10.0f); glVertex3f( -10.5f, 21.5f, 1.5f );
		glEnd();
	//PlataformaVerdeArribaDer
		glBindTexture(GL_TEXTURE_2D, textura[1].texID);
		//Cara delantera
		glBegin(GL_QUADS);
		glNormal3f( 0.0f,0.0f,1.0f );
		glTexCoord2f(0.0f, 0.0f); glVertex3f( 13.5f, 21.5f, 1.5f );
		glTexCoord2f(4.0f, 0.0f); glVertex3f( 25.5f, 21.5f, 1.5f );
		glTexCoord2f(4.0f, 1.0f); glVertex3f( 25.5f, 23.0f, 1.5f );
		glTexCoord2f(0.0f, 1.0f); glVertex3f( 13.5f, 23.0f, 1.5f );
		glEnd();
		//Cara derecha
		glBegin(GL_QUADS);
		glNormal3f( 1.0f,0.0f,0.0f );
		glTexCoord2f(0.0f, 0.0f); glVertex3f( 25.5f, 21.5f, 1.5f );
		glTexCoord2f(1.0f, 0.0f); glVertex3f( 25.5f, 21.5f, -1.5f );
		glTexCoord2f(1.0f, 1.0f); glVertex3f( 25.5f, 23.0f, -1.5f );
		glTexCoord2f(0.0f, 1.0f); glVertex3f( 25.5f, 23.0f, 1.5f );
		glEnd();
		//Cara izquierda
		glBegin(GL_QUADS);
		glNormal3f( -1.0f,0.0f,0.0f );
		glTexCoord2f(1.0f, 0.0f); glVertex3f( 13.5f, 21.5f, 1.5f );
		glTexCoord2f(1.0f, 1.0f); glVertex3f( 13.5f, 23.0f, 1.5f );
		glTexCoord2f(0.0f, 1.0f); glVertex3f( 13.5f, 23.0f, -1.5f );
		glTexCoord2f(0.0f, 0.0f); glVertex3f( 13.5f, 21.5f, -1.5f );
		glEnd();
		//Cara trasera
		glBegin(GL_QUADS);
		glNormal3f( 0.0f,0.0f,-1.0f );
		glTexCoord2f(4.0f, 0.0f); glVertex3f( 13.5f, 21.5f, -1.5f );
		glTexCoord2f(4.0f, 1.0f); glVertex3f( 13.5f, 23.0f, -1.5f );
		glTexCoord2f(0.0f, 1.0f); glVertex3f( 25.5f, 23.0f, -1.5f );
		glTexCoord2f(0.0f, 0.0f); glVertex3f( 25.5f, 21.5f, -1.5f );
		glEnd();
		//Cara superior
		glBindTexture(GL_TEXTURE_2D, textura[0].texID);
		glBegin(GL_QUADS);
		glNormal3f( 0.0f,1.0f,0.0f );
		glTexCoord2f(0.0f, 0.0f); glVertex3f( 13.5f, 23.0f, 1.5f );
		glTexCoord2f(4.0f, 0.0f); glVertex3f( 25.5f, 23.0f, 1.5f );
		glTexCoord2f(4.0f, 1.0f); glVertex3f( 25.5f, 23.0f, -1.5f );
		glTexCoord2f(0.0f, 1.0f); glVertex3f( 13.5f, 23.0f, -1.5f );
		glEnd();
		//Cara inferior
		glBegin(GL_QUADS);
		glNormal3f( 0.0f,-1.0f,0.0f );
		glTexCoord2f(0.0f, 0.0f); glVertex3f( 13.5f, 21.5f, 1.5f );
		glTexCoord2f(1.0f, 0.0f); glVertex3f( 13.5f, 21.5f, -1.5f );
		glTexCoord2f(1.0f, 4.0f); glVertex3f( 25.5f, 21.5f, -1.5f );
		glTexCoord2f(0.0f, 4.0f); glVertex3f( 25.5f, 21.5f, 1.5f );
		glEnd();
	//PlataformaVerdeAbajoIzq
		//Cara delantera
		glBindTexture(GL_TEXTURE_2D, textura[1].texID);
		glBegin(GL_QUADS);
		glNormal3f( 0.0f,0.0f,1.0f );
		glTexCoord2f(0.0f, 0.0f); glVertex3f( -28.5f, 10.5f, 1.5f );
		glTexCoord2f(7.0f, 0.0f); glVertex3f( -1.5f, 10.5f, 1.5f );
		glTexCoord2f(7.0f, 1.0f); glVertex3f( -1.5f, 12.0f, 1.5f );
		glTexCoord2f(0.0f, 1.0f); glVertex3f( -28.5f, 12.0f, 1.5f );
		glEnd();
		//Cara derecha
		glBegin(GL_QUADS);
		glNormal3f( 1.0f,0.0f,0.0f );
		glTexCoord2f(0.0f, 0.0f); glVertex3f( -1.5f, 10.5f, 1.5f );
		glTexCoord2f(1.0f, 0.0f); glVertex3f( -1.5f, 10.5f, -1.5f );
		glTexCoord2f(1.0f, 1.0f); glVertex3f( -1.5f, 12.0f, -1.5f );
		glTexCoord2f(0.0f, 1.0f); glVertex3f( -1.5f, 12.0f, 1.5f );
		glEnd();
		//Cara izquierda
		glBegin(GL_QUADS);
		glNormal3f( -1.0f,0.0f,0.0f );
		glTexCoord2f(1.0f, 0.0f); glVertex3f( -28.5f, 10.5f, 1.5f );
		glTexCoord2f(1.0f, 1.0f); glVertex3f( -28.5f, 12.0f, 1.5f );
		glTexCoord2f(0.0f, 1.0f); glVertex3f( -28.5f, 12.0f, -1.5f );
		glTexCoord2f(0.0f, 0.0f); glVertex3f( -28.5f, 10.5f, -1.5f );
		glEnd();
		//Cara trasera
		glBegin(GL_QUADS);
		glNormal3f( 0.0f,0.0f,-1.0f );
		glTexCoord2f(7.0f, 0.0f); glVertex3f( -28.5f, 10.5f, -1.5f );
		glTexCoord2f(7.0f, 1.0f); glVertex3f( -28.5f, 12.0f, -1.5f );
		glTexCoord2f(0.0f, 1.0f); glVertex3f( -1.5f, 12.0f, -1.5f );
		glTexCoord2f(0.0f, 0.0f); glVertex3f( -1.5f, 10.5f, -1.5f );
		glEnd();
		//Cara superior
		glBindTexture(GL_TEXTURE_2D, textura[0].texID);
		glBegin(GL_QUADS);
		glNormal3f( 0.0f,1.0f,0.0f );
		glTexCoord2f(0.0f, 0.0f); glVertex3f( -28.5f, 12.0f, 1.5f );
		glTexCoord2f(7.0f, 0.0f); glVertex3f( -1.5f, 12.0f, 1.5f );
		glTexCoord2f(7.0f, 1.0f); glVertex3f( -1.5f, 12.0f, -1.5f );
		glTexCoord2f(0.0f, 1.0f); glVertex3f( -28.5f, 12.0f, -1.5f );
		glEnd();
		//Cara inferior
		glBegin(GL_QUADS);
		glNormal3f( 0.0f,-1.0f,0.0f );
		glTexCoord2f(0.0f, 0.0f); glVertex3f( -28.5f, 10.5f, 1.5f );
		glTexCoord2f(1.0f, 0.0f); glVertex3f( -28.5f, 10.5f, -1.5f );
		glTexCoord2f(1.0f, 7.0f); glVertex3f( -1.5f, 10.5f, -1.5f );
		glTexCoord2f(0.0f, 7.0f); glVertex3f( -1.5f, 10.5f, 1.5f );
		glEnd();
	//PlataformaRojaAbajoIzq
		//Cara delantera
		glBindTexture(GL_TEXTURE_2D, textura[5].texID);
		glBegin(GL_QUADS);
		glNormal3f( 0.0f,0.0f,1.0f );
		glTexCoord2f(0.0f, 0.0f); glVertex3f( -43.5f, 10.5f, 1.5f );
		glTexCoord2f(5.0f, 0.0f); glVertex3f( -28.5f, 10.5f, 1.5f );
		glTexCoord2f(5.0f, 1.0f); glVertex3f( -28.5f, 12.0f, 1.5f );
		glTexCoord2f(0.0f, 1.0f); glVertex3f( -43.5f, 12.0f, 1.5f );
		glEnd();
		//Cara derecha
		glBegin(GL_QUADS);
		glNormal3f( 1.0f,0.0f,0.0f );
		glTexCoord2f(0.0f, 0.0f); glVertex3f( -28.5f, 10.5f, 1.5f );
		glTexCoord2f(1.0f, 0.0f); glVertex3f( -28.5f, 10.5f, -1.5f );
		glTexCoord2f(1.0f, 1.0f); glVertex3f( -28.5f, 12.0f, -1.5f );
		glTexCoord2f(0.0f, 1.0f); glVertex3f( -28.5f, 12.0f, 1.5f );
		glEnd();
		//Cara izquierda
		glBegin(GL_QUADS);
		glNormal3f( -1.0f,0.0f,0.0f );
		glTexCoord2f(1.0f, 0.0f); glVertex3f( -43.5f, 10.5f, 1.5f );
		glTexCoord2f(1.0f, 1.0f); glVertex3f( -43.5f, 12.0f, 1.5f );
		glTexCoord2f(0.0f, 1.0f); glVertex3f( -43.5f, 12.0f, -1.5f );
		glTexCoord2f(0.0f, 0.0f); glVertex3f( -43.5f, 10.5f, -1.5f );
		glEnd();
		//Cara trasera
		glBegin(GL_QUADS);
		glNormal3f( 0.0f,0.0f,-1.0f );
		glTexCoord2f(5.0f, 0.0f); glVertex3f( -43.5f, 10.5f, -1.5f );
		glTexCoord2f(5.0f, 1.0f); glVertex3f( -43.5f, 12.0f, -1.5f );
		glTexCoord2f(0.0f, 1.0f); glVertex3f( -28.5f, 12.0f, -1.5f );
		glTexCoord2f(0.0f, 0.0f); glVertex3f( -28.5f, 10.5f, -1.5f );
		glEnd();
		//Cara superior
		glBindTexture(GL_TEXTURE_2D, textura[6].texID);
		glBegin(GL_QUADS);
		glNormal3f( 0.0f,1.0f,0.0f );
		glTexCoord2f(0.0f, 0.0f); glVertex3f( -43.5f, 12.0f, 1.5f );
		glTexCoord2f(5.0f, 0.0f); glVertex3f( -28.5f, 12.0f, 1.5f );
		glTexCoord2f(5.0f, 1.0f); glVertex3f( -28.5f, 12.0f, -1.5f );
		glTexCoord2f(0.0f, 1.0f); glVertex3f( -43.5f, 12.0f, -1.5f );
		glEnd();
		//Cara inferior
		glBegin(GL_QUADS);
		glNormal3f( 0.0f,-1.0f,0.0f );
		glTexCoord2f(0.0f, 0.0f); glVertex3f( -43.5f, 10.5f, 1.5f );
		glTexCoord2f(1.0f, 0.0f); glVertex3f( -43.5f, 10.5f, -1.5f );
		glTexCoord2f(1.0f, 5.0f); glVertex3f( -28.5f, 10.5f, -1.5f );
		glTexCoord2f(0.0f, 5.0f); glVertex3f( -28.5f, 10.5f, 1.5f );
		glEnd();
	//PlataformaVerdeAbajoDer
		glBindTexture(GL_TEXTURE_2D, textura[1].texID);
		//Cara delantera
		glBegin(GL_QUADS);
		glNormal3f( 0.0f,0.0f,1.0f );
		glTexCoord2f(0.0f, 0.0f); glVertex3f( 13.5f, 10.5f, 1.5f );
		glTexCoord2f(4.0f, 0.0f); glVertex3f( 25.5f, 10.5f, 1.5f );
		glTexCoord2f(4.0f, 1.0f); glVertex3f( 25.5f, 12.0f, 1.5f );
		glTexCoord2f(0.0f, 1.0f); glVertex3f( 13.5f, 12.0f, 1.5f );
		glEnd();
		//Cara derecha
		glBegin(GL_QUADS);
		glNormal3f( 1.0f,0.0f,0.0f );
		glTexCoord2f(0.0f, 0.0f); glVertex3f( 25.5f, 10.5f, 1.5f );
		glTexCoord2f(1.0f, 0.0f); glVertex3f( 25.5f, 10.5f, -1.5f );
		glTexCoord2f(1.0f, 1.0f); glVertex3f( 25.5f, 12.0f, -1.5f );
		glTexCoord2f(0.0f, 1.0f); glVertex3f( 25.5f, 12.0f, 1.5f );
		glEnd();
		//Cara izquierda
		glBegin(GL_QUADS);
		glNormal3f( -1.0f,0.0f,0.0f );
		glTexCoord2f(1.0f, 0.0f); glVertex3f( 13.5f, 10.5f, 1.5f );
		glTexCoord2f(1.0f, 1.0f); glVertex3f( 13.5f, 12.0f, 1.5f );
		glTexCoord2f(0.0f, 1.0f); glVertex3f( 13.5f, 12.0f, -1.5f );
		glTexCoord2f(0.0f, 0.0f); glVertex3f( 13.5f, 10.5f, -1.5f );
		glEnd();
		//Cara trasera
		glBegin(GL_QUADS);
		glNormal3f( 0.0f,0.0f,-1.0f );
		glTexCoord2f(4.0f, 0.0f); glVertex3f( 13.5f, 10.5f, -1.5f );
		glTexCoord2f(4.0f, 1.0f); glVertex3f( 13.5f, 12.0f, -1.5f );
		glTexCoord2f(0.0f, 1.0f); glVertex3f( 25.5f, 12.0f, -1.5f );
		glTexCoord2f(0.0f, 0.0f); glVertex3f( 25.5f, 10.5f, -1.5f );
		glEnd();
		//Cara superior
		glBindTexture(GL_TEXTURE_2D, textura[0].texID);
		glBegin(GL_QUADS);
		glNormal3f( 0.0f,1.0f,0.0f );
		glTexCoord2f(0.0f, 0.0f); glVertex3f( 13.5f, 12.0f, 1.5f );
		glTexCoord2f(4.0f, 0.0f); glVertex3f( 25.5f, 12.0f, 1.5f );
		glTexCoord2f(4.0f, 1.0f); glVertex3f( 25.5f, 12.0f, -1.5f );
		glTexCoord2f(0.0f, 1.0f); glVertex3f( 13.5f, 12.0f, -1.5f );
		glEnd();
		//Cara inferior
		glBegin(GL_QUADS);
		glNormal3f( 0.0f,-1.0f,0.0f );
		glTexCoord2f(0.0f, 0.0f); glVertex3f( 13.5f, 10.5f, 1.5f );
		glTexCoord2f(1.0f, 0.0f); glVertex3f( 13.5f, 10.5f, -1.5f );
		glTexCoord2f(1.0f, 4.0f); glVertex3f( 25.5f, 10.5f, -1.5f );
		glTexCoord2f(0.0f, 4.0f); glVertex3f( 25.5f, 10.5f, 1.5f );
		glEnd();
//EscaleraVerde
	//verticalder
		//Cara delantera
		glBindTexture(GL_TEXTURE_2D, textura[7].texID);
		glBegin(GL_QUADS);
		glNormal3f( 0.0f,0.0f,1.0f );
		glTexCoord2f(0.0f, 0.0f); glVertex3f( 18.6f, 0.0f, 1.55f );
		glTexCoord2f(1.0f, 0.0f); glVertex3f( 19.0f, 0.0f, 1.55f );
		glTexCoord2f(1.0f, 16.0f); glVertex3f( 19.0f, 23.0f, 1.55f );
		glTexCoord2f(0.0f, 16.0f); glVertex3f( 18.6f, 23.0f, 1.55f );
		glEnd();
		//Cara derecha
		glBegin(GL_QUADS);
		glNormal3f( 1.0f,0.0f,0.0f );
		glTexCoord2f(0.0f, 0.0f); glVertex3f( 19.0f, 0.0f, 1.55f );
		glTexCoord2f(1.0f, 0.0f); glVertex3f( 19.0f, 0.0f, 1.1f );
		glTexCoord2f(1.0f, 16.0f); glVertex3f( 19.0f, 23.0f, 1.1f );
		glTexCoord2f(0.0f, 16.0f); glVertex3f( 19.0f, 23.0f, 1.55f );
		glEnd();
		//Cara izquierda
		glBegin(GL_QUADS);
		glNormal3f( -1.0f,0.0f,0.0f );
		glTexCoord2f(1.0f, 0.0f); glVertex3f( 18.6f, 0.0f, 1.55f );
		glTexCoord2f(1.0f, 16.0f); glVertex3f( 18.6f, 23.0f, 1.55f );
		glTexCoord2f(0.0f, 16.0f); glVertex3f( 18.6f, 23.0f, 1.1f );
		glTexCoord2f(0.0f, 0.0f); glVertex3f( 18.6f, 0.0f, 1.1f );
		glEnd();
		//Cara trasera
		glBegin(GL_QUADS);
		glNormal3f( 0.0f,0.0f,-1.0f );
		glTexCoord2f(1.0f, 0.0f); glVertex3f( 18.6f, 0.0f, 1.1f );
		glTexCoord2f(1.0f, 16.0f); glVertex3f( 18.6f, 23.0f, 1.1f );
		glTexCoord2f(0.0f, 16.0f); glVertex3f( 19.0f, 23.0f, 1.1f );
		glTexCoord2f(0.0f, 0.0f); glVertex3f( 19.0f, 0.0f, 1.1f );
		glEnd();
		//Cara superior
		glBindTexture(GL_TEXTURE_2D, textura[9].texID);
		glBegin(GL_QUADS);
		glNormal3f( 0.0f,1.0f,0.0f );
		glTexCoord2f(0.0f, 0.0f); glVertex3f( 18.6f, 23.0f, 1.55f );
		glTexCoord2f(1.0f, 0.0f); glVertex3f( 19.0f, 23.0f, 1.55f );
		glTexCoord2f(1.0f, 1.0f); glVertex3f( 19.0f, 23.0f, 1.1f );
		glTexCoord2f(0.0f, 1.0f); glVertex3f( 18.6f, 23.0f, 1.1f );
		glEnd();
		//Cara inferior
		glBegin(GL_QUADS);
		glNormal3f( 0.0f, -1.0f, 0.0f );
		glTexCoord2f(0.0f, 0.0f); glVertex3f( 18.6f, 0.0f, 1.55f );
		glTexCoord2f(1.0f, 0.0f); glVertex3f( 18.6f, 0.0f, 1.1f );
		glTexCoord2f(1.0f, 1.0f); glVertex3f( 19.0f, 0.0f, 1.1f );
		glTexCoord2f(0.0f, 1.0f); glVertex3f( 19.0f, 0.0f, 1.55f );
		glEnd();
	//verticalizq
		//Cara delantera
		glBindTexture(GL_TEXTURE_2D, textura[7].texID);
		glBegin(GL_QUADS);
		glNormal3f( 0.0f,0.0f,1.0f );
		glTexCoord2f(0.0f, 0.0f); glVertex3f( 16.0f, 0.0f, 1.55f );
		glTexCoord2f(1.0f, 0.0f); glVertex3f( 16.4f, 0.0f, 1.55f );
		glTexCoord2f(1.0f, 16.0f); glVertex3f( 16.4f, 23.0f, 1.55f );
		glTexCoord2f(0.0f, 16.0f); glVertex3f( 16.0f, 23.0f, 1.55f );
		glEnd();
		//Cara derecha
		glBegin(GL_QUADS);
		glNormal3f( 1.0f,0.0f,0.0f );
		glTexCoord2f(0.0f, 0.0f); glVertex3f( 16.4f, 0.0f, 1.55f );
		glTexCoord2f(1.0f, 0.0f); glVertex3f( 16.4f, 0.0f, 1.1f );
		glTexCoord2f(1.0f, 16.0f); glVertex3f( 16.4f, 23.0f, 1.1f );
		glTexCoord2f(0.0f, 16.0f); glVertex3f( 16.4f, 23.0f, 1.55f );
		glEnd();
		//Cara izquierda
		glBegin(GL_QUADS);
		glNormal3f( -1.0f,0.0f,0.0f );
		glTexCoord2f(1.0f, 0.0f); glVertex3f( 16.0f, 0.0f, 1.55f );
		glTexCoord2f(1.0f, 16.0f); glVertex3f( 16.0f, 23.0f, 1.55f );
		glTexCoord2f(0.0f, 16.0f); glVertex3f( 16.0f, 23.0f, 1.1f );
		glTexCoord2f(0.0f, 0.0f); glVertex3f( 16.0f, 0.0f, 1.1f );
		glEnd();
		//Cara trasera
		glBegin(GL_QUADS);
		glNormal3f( 0.0f,0.0f,-1.0f );
		glTexCoord2f(1.0f, 0.0f); glVertex3f( 16.0f, 0.0f, 1.1f );
		glTexCoord2f(1.0f, 16.0f); glVertex3f( 16.0f, 23.0f, 1.1f );
		glTexCoord2f(0.0f, 16.0f); glVertex3f( 16.4f, 23.0f, 1.1f );
		glTexCoord2f(0.0f, 0.0f); glVertex3f( 16.4f, 0.0f, 1.1f );
		glEnd();
		glBindTexture(GL_TEXTURE_2D, textura[9].texID);
		//Cara superior
		glBegin(GL_QUADS);
		glNormal3f( 0.0f,1.0f,0.0f );
		glTexCoord2f(0.0f, 0.0f); glVertex3f( 16.0f, 23.0f, 1.55f );
		glTexCoord2f(1.0f, 0.0f); glVertex3f( 16.4f, 23.0f, 1.55f );
		glTexCoord2f(1.0f, 1.0f); glVertex3f( 16.4f, 23.0f, 1.1f );
		glTexCoord2f(0.0f, 1.0f); glVertex3f( 16.0f, 23.0f, 1.1f );
		glEnd();
		//Cara inferior
		glBegin(GL_QUADS);
		glNormal3f( 0.0f, -1.0f, 0.0f );
		glTexCoord2f(0.0f, 0.0f); glVertex3f( 16.0f, 0.0f, 1.55f );
		glTexCoord2f(1.0f, 0.0f); glVertex3f( 16.0f, 0.0f, 1.1f );
		glTexCoord2f(1.0f, 1.0f); glVertex3f( 16.4f, 0.0f, 1.1f );
		glTexCoord2f(0.0f, 1.0f); glVertex3f( 16.4f, 0.0f, 1.55f );
		glEnd();
	//Escalon1
		//Cara delantera
		glBegin(GL_QUADS);
		glNormal3f( 0.0f,0.0f,1.0f );
		glTexCoord2f(0.0f, 0.0f); glVertex3f( 16.4f, 1.0f, 1.55f );
		glTexCoord2f(1.0f, 0.0f); glVertex3f( 18.6f, 1.0f, 1.55f );
		glTexCoord2f(1.0f, 1.0f); glVertex3f( 18.6f, 1.4f, 1.55f );
		glTexCoord2f(0.0f, 1.0f); glVertex3f( 16.4f, 1.4f, 1.55f );
		glEnd();
		//Cara derecha
		glBegin(GL_QUADS);
		glNormal3f( 1.0f,0.0f,0.0f );
		glTexCoord2f(0.0f, 0.0f); glVertex3f( 18.6f, 1.0f, 1.55f );
		glTexCoord2f(1.0f, 0.0f); glVertex3f( 18.6f, 1.0f, 1.1f );
		glTexCoord2f(1.0f, 1.0f); glVertex3f( 18.6f, 1.4f, 1.1f );
		glTexCoord2f(0.0f, 1.0f); glVertex3f( 18.6f, 1.4f, 1.55f );
		glEnd();
		//Cara izquierda
		glBegin(GL_QUADS);
		glNormal3f( -1.0f,0.0f,0.0f );
		glTexCoord2f(1.0f, 0.0f); glVertex3f( 16.4f, 1.0f, 1.55f );
		glTexCoord2f(1.0f, 1.0f); glVertex3f( 16.4f, 1.4f, 1.55f );
		glTexCoord2f(0.0f, 1.0f); glVertex3f( 16.4f, 1.4f, 1.1f );
		glTexCoord2f(0.0f, 0.0f); glVertex3f( 16.4f, 1.0f, 1.1f );
		glEnd();
		//Cara trasera
		glBegin(GL_QUADS);
		glNormal3f( 0.0f,0.0f,-1.0f );
		glTexCoord2f(1.0f, 0.0f); glVertex3f( 16.4f, 1.0f, 1.1f );
		glTexCoord2f(1.0f, 1.0f); glVertex3f( 16.4f, 1.4f, 1.1f );
		glTexCoord2f(0.0f, 1.0f); glVertex3f( 18.6f, 1.4f, 1.1f );
		glTexCoord2f(0.0f, 0.0f); glVertex3f( 18.6f, 1.0f, 1.1f );
		glEnd();
		//Cara superior
		glBegin(GL_QUADS);
		glNormal3f( 0.0f,1.0f,0.0f );
		glTexCoord2f(0.0f, 0.0f); glVertex3f( 16.4f, 1.4f, 1.55f );
		glTexCoord2f(1.0f, 0.0f); glVertex3f( 18.6f, 1.4f, 1.55f );
		glTexCoord2f(1.0f, 1.0f); glVertex3f( 18.6f, 1.4f, 1.1f );
		glTexCoord2f(0.0f, 1.0f); glVertex3f( 16.4f, 1.4f, 1.1f );
		glEnd();
		//Cara inferior
		glBegin(GL_QUADS);
		glNormal3f( 0.0f, -1.0f, 0.0f );
		glTexCoord2f(0.0f, 0.0f); glVertex3f( 16.4f, 1.0f, 1.55f );
		glTexCoord2f(1.0f, 0.0f); glVertex3f( 16.4f, 1.0f, 1.1f );
		glTexCoord2f(1.0f, 1.0f); glVertex3f( 18.6f, 1.0f, 1.1f );
		glTexCoord2f(0.0f, 1.0f); glVertex3f( 18.6f, 1.0f, 1.55f );
		glEnd();
	//Escalon2
		//Cara delantera
		glBegin(GL_QUADS);
		glNormal3f( 0.0f,0.0f,1.0f );
		glTexCoord2f(0.0f, 0.0f); glVertex3f( 16.4f, 2.4f, 1.55f );
		glTexCoord2f(1.0f, 0.0f); glVertex3f( 18.6f, 2.4f, 1.55f );
		glTexCoord2f(1.0f, 1.0f); glVertex3f( 18.6f, 2.8f, 1.55f );
		glTexCoord2f(0.0f, 1.0f); glVertex3f( 16.4f, 2.8f, 1.55f );
		glEnd();
		//Cara derecha
		glBegin(GL_QUADS);
		glNormal3f( 1.0f,0.0f,0.0f );
		glTexCoord2f(0.0f, 0.0f); glVertex3f( 18.6f, 2.4f, 1.55f );
		glTexCoord2f(1.0f, 0.0f); glVertex3f( 18.6f, 2.4f, 1.1f );
		glTexCoord2f(1.0f, 1.0f); glVertex3f( 18.6f, 2.8f, 1.1f );
		glTexCoord2f(0.0f, 1.0f); glVertex3f( 18.6f, 2.8f, 1.55f );
		glEnd();
		//Cara izquierda
		glBegin(GL_QUADS);
		glNormal3f( -1.0f,0.0f,0.0f );
		glTexCoord2f(1.0f, 0.0f); glVertex3f( 16.4f, 2.4f, 1.55f );
		glTexCoord2f(1.0f, 1.0f); glVertex3f( 16.4f, 2.8f, 1.55f );
		glTexCoord2f(0.0f, 1.0f); glVertex3f( 16.4f, 2.8f, 1.1f );
		glTexCoord2f(0.0f, 0.0f); glVertex3f( 16.4f, 2.4f, 1.1f );
		glEnd();
		//Cara trasera
		glBegin(GL_QUADS);
		glNormal3f( 0.0f,0.0f,-1.0f );
		glTexCoord2f(1.0f, 0.0f); glVertex3f( 16.4f, 2.4f, 1.1f );
		glTexCoord2f(1.0f, 0.0f); glVertex3f( 16.4f, 2.8f, 1.1f );
		glTexCoord2f(0.0f, 1.0f); glVertex3f( 18.6f, 2.8f, 1.1f );
		glTexCoord2f(0.0f, 1.0f); glVertex3f( 18.6f, 2.4f, 1.1f );
		glEnd();
		//Cara superior
		glBegin(GL_QUADS);
		glNormal3f( 0.0f,1.0f,0.0f );
		glTexCoord2f(0.0f, 0.0f); glVertex3f( 16.4f, 2.8f, 1.55f );
		glTexCoord2f(1.0f, 0.0f); glVertex3f( 18.6f, 2.8f, 1.55f );
		glTexCoord2f(1.0f, 1.0f); glVertex3f( 18.6f, 2.8f, 1.1f );
		glTexCoord2f(0.0f, 1.0f); glVertex3f( 16.4f, 2.8f, 1.1f );
		glEnd();
		//Cara inferior
		glBegin(GL_QUADS);
		glNormal3f( 0.0f, -1.0f, 0.0f );
		glTexCoord2f(0.0f, 0.0f); glVertex3f( 16.4f, 2.4f, 1.55f );
		glTexCoord2f(1.0f, 0.0f); glVertex3f( 16.4f, 2.4f, 1.1f );
		glTexCoord2f(1.0f, 1.0f); glVertex3f( 18.6f, 2.4f, 1.1f );
		glTexCoord2f(0.0f, 1.0f); glVertex3f( 18.6f, 2.4f, 1.55f );
		glEnd();
	//Escalon3
		//Cara delantera
		glBegin(GL_QUADS);
		glNormal3f( 0.0f,0.0f,1.0f );
		glTexCoord2f(0.0f, 0.0f); glVertex3f( 16.4f, 3.7f, 1.55f );
		glTexCoord2f(1.0f, 0.0f); glVertex3f( 18.6f, 3.7f, 1.55f );
		glTexCoord2f(1.0f, 1.0f); glVertex3f( 18.6f, 4.2f, 1.55f );
		glTexCoord2f(0.0f, 1.0f); glVertex3f( 16.4f, 4.2f, 1.55f );
		glEnd();
		//Cara derecha
		glBegin(GL_QUADS);
		glNormal3f( 1.0f,0.0f,0.0f );
		glTexCoord2f(0.0f, 0.0f); glVertex3f( 18.6f, 3.7f, 1.55f );
		glTexCoord2f(1.0f, 0.0f); glVertex3f( 18.6f, 3.7f, 1.1f );
		glTexCoord2f(1.0f, 1.0f); glVertex3f( 18.6f, 4.2f, 1.1f );
		glTexCoord2f(0.0f, 1.0f); glVertex3f( 18.6f, 4.2f, 1.55f );
		glEnd();
		//Cara izquierda
		glBegin(GL_QUADS);
		glNormal3f( -1.0f,0.0f,0.0f );
		glTexCoord2f(1.0f, 0.0f); glVertex3f( 16.4f, 3.7f, 1.55f );
		glTexCoord2f(1.0f, 1.0f); glVertex3f( 16.4f, 4.2f, 1.55f );
		glTexCoord2f(0.0f, 1.0f); glVertex3f( 16.4f, 4.2f, 1.1f );
		glTexCoord2f(0.0f, 0.0f); glVertex3f( 16.4f, 3.7f, 1.1f );
		glEnd();
		//Cara trasera
		glBegin(GL_QUADS);
		glNormal3f( 0.0f,0.0f,-1.0f );
		glTexCoord2f(1.0f, 0.0f); glVertex3f( 16.4f, 3.7f, 1.1f );
		glTexCoord2f(1.0f, 1.0f); glVertex3f( 16.4f, 4.2f, 1.1f );
		glTexCoord2f(0.0f, 1.0f); glVertex3f( 18.6f, 4.2f, 1.1f );
		glTexCoord2f(0.0f, 0.0f); glVertex3f( 18.6f, 3.7f, 1.1f );
		glEnd();
		//Cara superior
		glBegin(GL_QUADS);
		glNormal3f( 0.0f,1.0f,0.0f );
		glTexCoord2f(0.0f, 0.0f); glVertex3f( 16.4f, 4.2f, 1.55f );
		glTexCoord2f(1.0f, 0.0f); glVertex3f( 18.6f, 4.2f, 1.55f );
		glTexCoord2f(1.0f, 1.0f); glVertex3f( 18.6f, 4.2f, 1.1f );
		glTexCoord2f(0.0f, 1.0f); glVertex3f( 16.4f, 4.2f, 1.1f );
		glEnd();
		//Cara inferior
		glBegin(GL_QUADS);
		glNormal3f( 0.0f, -1.0f, 0.0f );
		glTexCoord2f(0.0f, 0.0f); glVertex3f( 16.4f, 3.7f, 1.55f );
		glTexCoord2f(1.0f, 0.0f); glVertex3f( 16.4f, 3.7f, 1.1f );
		glTexCoord2f(1.0f, 1.0f); glVertex3f( 18.6f, 3.7f, 1.1f );
		glTexCoord2f(0.0f, 1.0f); glVertex3f( 18.6f, 3.7f, 1.55f );
		glEnd();
	//Escalon4
		//Cara delantera
		glBegin(GL_QUADS);
		glNormal3f( 0.0f,0.0f,1.0f );
		glTexCoord2f(0.0f, 0.0f); glVertex3f( 16.4f, 5.2f, 1.55f );
		glTexCoord2f(1.0f, 0.0f); glVertex3f( 18.6f, 5.2f, 1.55f );
		glTexCoord2f(1.0f, 1.0f); glVertex3f( 18.6f, 5.6f, 1.55f );
		glTexCoord2f(0.0f, 1.0f); glVertex3f( 16.4f, 5.6f, 1.55f );
		glEnd();
		//Cara derecha
		glBegin(GL_QUADS);
		glNormal3f( 1.0f,0.0f,0.0f );
		glTexCoord2f(0.0f, 0.0f); glVertex3f( 18.6f, 5.2f, 1.55f );
		glTexCoord2f(1.0f, 0.0f); glVertex3f( 18.6f, 5.2f, 1.1f );
		glTexCoord2f(1.0f, 1.0f); glVertex3f( 18.6f, 5.6f, 1.1f );
		glTexCoord2f(0.0f, 1.0f); glVertex3f( 18.6f, 5.6f, 1.55f );
		glEnd();
		//Cara izquierda
		glBegin(GL_QUADS);
		glNormal3f( -1.0f,0.0f,0.0f );
		glTexCoord2f(1.0f, 0.0f); glVertex3f( 16.4f, 5.2f, 1.55f );
		glTexCoord2f(1.0f, 1.0f); glVertex3f( 16.4f, 5.6f, 1.55f );
		glTexCoord2f(0.0f, 1.0f); glVertex3f( 16.4f, 5.6f, 1.1f );
		glTexCoord2f(0.0f, 0.0f); glVertex3f( 16.4f, 5.2f, 1.1f );
		glEnd();
		//Cara trasera
		glBegin(GL_QUADS);
		glNormal3f( 0.0f,0.0f,-1.0f );
		glTexCoord2f(1.0f, 0.0f); glVertex3f( 16.4f, 5.2f, 1.1f );
		glTexCoord2f(1.0f, 1.0f); glVertex3f( 16.4f, 5.6f, 1.1f );
		glTexCoord2f(0.0f, 1.0f); glVertex3f( 18.6f, 5.6f, 1.1f );
		glTexCoord2f(0.0f, 0.0f); glVertex3f( 18.6f, 5.2f, 1.1f );
		glEnd();
		//Cara superior
		glBegin(GL_QUADS);
		glNormal3f( 0.0f,1.0f,0.0f );
		glTexCoord2f(0.0f, 0.0f); glVertex3f( 16.4f, 5.6f, 1.55f );
		glTexCoord2f(1.0f, 0.0f); glVertex3f( 18.6f, 5.6f, 1.55f );
		glTexCoord2f(1.0f, 1.0f); glVertex3f( 18.6f, 5.6f, 1.1f );
		glTexCoord2f(0.0f, 1.0f); glVertex3f( 16.4f, 5.6f, 1.1f );
		glEnd();
		//Cara inferior
		glBegin(GL_QUADS);
		glNormal3f( 0.0f, -1.0f, 0.0f );
		glTexCoord2f(0.0f, 0.0f); glVertex3f( 16.4f, 5.2f, 1.55f );
		glTexCoord2f(1.0f, 0.0f); glVertex3f( 16.4f, 5.2f, 1.1f );
		glTexCoord2f(1.0f, 1.0f); glVertex3f( 18.6f, 5.2f, 1.1f );
		glTexCoord2f(0.0f, 1.0f); glVertex3f( 18.6f, 5.2f, 1.55f );
		glEnd();
	//Escalon5
		//Cara delantera
		glBegin(GL_QUADS);
		glNormal3f( 0.0f,0.0f,1.0f );
		glTexCoord2f(0.0f, 0.0f); glVertex3f( 16.4f, 6.6f, 1.55f );
		glTexCoord2f(1.0f, 0.0f); glVertex3f( 18.6f, 6.6f, 1.55f );
		glTexCoord2f(1.0f, 1.0f); glVertex3f( 18.6f, 7.0f, 1.55f );
		glTexCoord2f(0.0f, 1.0f); glVertex3f( 16.4f, 7.0f, 1.55f );
		glEnd();
		//Cara derecha
		glBegin(GL_QUADS);
		glNormal3f( 1.0f,0.0f,0.0f );
		glTexCoord2f(0.0f, 0.0f); glVertex3f( 18.6f, 6.6f, 1.55f );
		glTexCoord2f(1.0f, 0.0f); glVertex3f( 18.6f, 6.6f, 1.1f );
		glTexCoord2f(1.0f, 1.0f); glVertex3f( 18.6f, 7.0f, 1.1f );
		glTexCoord2f(0.0f, 1.0f); glVertex3f( 18.6f, 7.0f, 1.55f );
		glEnd();
		//Cara izquierda
		glBegin(GL_QUADS);
		glNormal3f( -1.0f,0.0f,0.0f );
		glTexCoord2f(1.0f, 0.0f); glVertex3f( 16.4f, 6.6f, 1.55f );
		glTexCoord2f(1.0f, 1.0f); glVertex3f( 16.4f, 7.0f, 1.55f );
		glTexCoord2f(0.0f, 1.0f); glVertex3f( 16.4f, 7.0f, 1.1f );
		glTexCoord2f(0.0f, 0.0f); glVertex3f( 16.4f, 6.6f, 1.1f );
		glEnd();
		//Cara trasera
		glBegin(GL_QUADS);
		glNormal3f( 0.0f,0.0f,-1.0f );
		glTexCoord2f(1.0f, 0.0f); glVertex3f( 16.4f, 6.6f, 1.1f );
		glTexCoord2f(1.0f, 1.0f); glVertex3f( 16.4f, 7.0f, 1.1f );
		glTexCoord2f(0.0f, 1.0f); glVertex3f( 18.6f, 7.0f, 1.1f );
		glTexCoord2f(0.0f, 0.0f); glVertex3f( 18.6f, 6.6f, 1.1f );
		glEnd();
		//Cara superior
		glBegin(GL_QUADS);
		glNormal3f( 0.0f,1.0f,0.0f );
		glTexCoord2f(0.0f, 0.0f); glVertex3f( 16.4f, 7.0f, 1.55f );
		glTexCoord2f(1.0f, 0.0f); glVertex3f( 18.6f, 7.0f, 1.55f );
		glTexCoord2f(1.0f, 1.0f); glVertex3f( 18.6f, 7.0f, 1.1f );
		glTexCoord2f(0.0f, 1.0f); glVertex3f( 16.4f, 7.0f, 1.1f );
		glEnd();
		//Cara inferior
		glBegin(GL_QUADS);
		glNormal3f( 0.0f, -1.0f, 0.0f );
		glTexCoord2f(0.0f, 0.0f); glVertex3f( 16.4f, 6.6f, 1.55f );
		glTexCoord2f(1.0f, 0.0f); glVertex3f( 16.4f, 6.6f, 1.1f );
		glTexCoord2f(1.0f, 1.0f); glVertex3f( 18.6f, 6.6f, 1.1f );
		glTexCoord2f(0.0f, 1.0f); glVertex3f( 18.6f, 6.6f, 1.55f );
		glEnd();
	//Escalon6
		//Cara delantera
		glBegin(GL_QUADS);
		glNormal3f( 0.0f,0.0f,1.0f );
		glTexCoord2f(0.0f, 0.0f); glVertex3f( 16.4f, 8.0f, 1.55f );
		glTexCoord2f(1.0f, 0.0f); glVertex3f( 18.6f, 8.0f, 1.55f );
		glTexCoord2f(1.0f, 1.0f); glVertex3f( 18.6f, 8.4f, 1.55f );
		glTexCoord2f(0.0f, 1.0f); glVertex3f( 16.4f, 8.4f, 1.55f );
		glEnd();
		//Cara derecha
		glBegin(GL_QUADS);
		glNormal3f( 1.0f,0.0f,0.0f );
		glTexCoord2f(0.0f, 0.0f); glVertex3f( 18.6f, 8.0f, 1.55f );
		glTexCoord2f(1.0f, 0.0f); glVertex3f( 18.6f, 8.0f, 1.1f );
		glTexCoord2f(1.0f, 1.0f); glVertex3f( 18.6f, 8.4f, 1.1f );
		glTexCoord2f(0.0f, 1.0f); glVertex3f( 18.6f, 8.4f, 1.55f );
		glEnd();
		//Cara izquierda
		glBegin(GL_QUADS);
		glNormal3f( -1.0f,0.0f,0.0f );
		glTexCoord2f(1.0f, 0.0f); glVertex3f( 16.4f, 8.0f, 1.55f );
		glTexCoord2f(1.0f, 1.0f); glVertex3f( 16.4f, 8.4f, 1.55f );
		glTexCoord2f(0.0f, 1.0f); glVertex3f( 16.4f, 8.4f, 1.1f );
		glTexCoord2f(0.0f, 0.0f); glVertex3f( 16.4f, 8.0f, 1.1f );
		glEnd();
		//Cara trasera
		glBegin(GL_QUADS);
		glNormal3f( 0.0f,0.0f,-1.0f );
		glTexCoord2f(1.0f, 0.0f); glVertex3f( 16.4f, 8.0f, 1.1f );
		glTexCoord2f(1.0f, 1.0f); glVertex3f( 16.4f, 8.4f, 1.1f );
		glTexCoord2f(0.0f, 1.0f); glVertex3f( 18.6f, 8.4f, 1.1f );
		glTexCoord2f(0.0f, 0.0f); glVertex3f( 18.6f, 8.0f, 1.1f );
		glEnd();
		//Cara superior
		glBegin(GL_QUADS);
		glNormal3f( 0.0f,1.0f,0.0f );
		glTexCoord2f(0.0f, 0.0f); glVertex3f( 16.4f, 8.4f, 1.55f );
		glTexCoord2f(1.0f, 0.0f); glVertex3f( 18.6f, 8.4f, 1.55f );
		glTexCoord2f(1.0f, 1.0f); glVertex3f( 18.6f, 8.4f, 1.1f );
		glTexCoord2f(0.0f, 1.0f); glVertex3f( 16.4f, 8.4f, 1.1f );
		glEnd();
		//Cara inferior
		glBegin(GL_QUADS);
		glNormal3f( 0.0f, -1.0f, 0.0f );
		glTexCoord2f(0.0f, 0.0f); glVertex3f( 16.4f, 8.0f, 1.55f );
		glTexCoord2f(1.0f, 0.0f); glVertex3f( 16.4f, 8.0f, 1.1f );
		glTexCoord2f(1.0f, 1.0f); glVertex3f( 18.6f, 8.0f, 1.1f );
		glTexCoord2f(0.0f, 1.0f); glVertex3f( 18.6f, 8.0f, 1.55f );
		glEnd();
	//Escalon7
		//Cara delantera
		glBegin(GL_QUADS);
		glNormal3f( 0.0f,0.0f,1.0f );
		glTexCoord2f(0.0f, 0.0f); glVertex3f( 16.4f, 9.4f, 1.55f );
		glTexCoord2f(1.0f, 0.0f); glVertex3f( 18.6f, 9.4f, 1.55f );
		glTexCoord2f(1.0f, 1.0f); glVertex3f( 18.6f, 9.8f, 1.55f );
		glTexCoord2f(0.0f, 1.0f); glVertex3f( 16.4f, 9.8f, 1.55f );
		glEnd();
		//Cara derecha
		glBegin(GL_QUADS);
		glNormal3f( 1.0f,0.0f,0.0f );
		glTexCoord2f(0.0f, 0.0f); glVertex3f( 18.6f, 9.4f, 1.55f );
		glTexCoord2f(1.0f, 0.0f); glVertex3f( 18.6f, 9.4f, 1.1f );
		glTexCoord2f(1.0f, 1.0f); glVertex3f( 18.6f, 9.8f, 1.1f );
		glTexCoord2f(0.0f, 1.0f); glVertex3f( 18.6f, 9.8f, 1.55f );
		glEnd();
		//Cara izquierda
		glBegin(GL_QUADS);
		glNormal3f( -1.0f,0.0f,0.0f );
		glTexCoord2f(1.0f, 0.0f); glVertex3f( 16.4f, 9.4f, 1.55f );
		glTexCoord2f(1.0f, 1.0f); glVertex3f( 16.4f, 9.8f, 1.55f );
		glTexCoord2f(0.0f, 1.0f); glVertex3f( 16.4f, 9.8f, 1.1f );
		glTexCoord2f(0.0f, 0.0f); glVertex3f( 16.4f, 9.4f, 1.1f );
		glEnd();
		//Cara trasera
		glBegin(GL_QUADS);
		glNormal3f( 0.0f,0.0f,-1.0f );
		glTexCoord2f(1.0f, 0.0f); glVertex3f( 16.4f, 9.4f, 1.1f );
		glTexCoord2f(1.0f, 1.0f); glVertex3f( 16.4f, 9.8f, 1.1f );
		glTexCoord2f(0.0f, 1.0f); glVertex3f( 18.6f, 9.8f, 1.1f );
		glTexCoord2f(0.0f, 0.0f); glVertex3f( 18.6f, 9.4f, 1.1f );
		glEnd();
		//Cara superior
		glBegin(GL_QUADS);
		glNormal3f( 0.0f,1.0f,0.0f );
		glTexCoord2f(0.0f, 0.0f); glVertex3f( 16.4f, 9.8f, 1.55f );
		glTexCoord2f(1.0f, 0.0f); glVertex3f( 18.6f, 9.8f, 1.55f );
		glTexCoord2f(1.0f, 1.0f); glVertex3f( 18.6f, 9.8f, 1.1f );
		glTexCoord2f(0.0f, 1.0f); glVertex3f( 16.4f, 9.8f, 1.1f );
		glEnd();
		//Cara inferior
		glBegin(GL_QUADS);
		glNormal3f( 0.0f, -1.0f, 0.0f );
		glTexCoord2f(0.0f, 0.0f); glVertex3f( 16.4f, 9.4f, 1.55f );
		glTexCoord2f(1.0f, 0.0f); glVertex3f( 16.4f, 9.4f, 1.1f );
		glTexCoord2f(1.0f, 1.0f); glVertex3f( 18.6f, 9.4f, 1.1f );
		glTexCoord2f(0.0f, 1.0f); glVertex3f( 18.6f, 9.4f, 1.55f );
		glEnd();
	//Escalon8
		//Cara delantera
		glBegin(GL_QUADS);
		glNormal3f( 0.0f,0.0f,1.0f );
		glTexCoord2f(0.0f, 0.0f); glVertex3f( 16.4f, 13.0f, 1.55f );
		glTexCoord2f(1.0f, 0.0f); glVertex3f( 18.6f, 13.0f, 1.55f );
		glTexCoord2f(1.0f, 1.0f); glVertex3f( 18.6f, 13.4f, 1.55f );
		glTexCoord2f(0.0f, 1.0f); glVertex3f( 16.4f, 13.4f, 1.55f );
		glEnd();
		//Cara derecha
		glBegin(GL_QUADS);
		glNormal3f( 1.0f,0.0f,0.0f );
		glTexCoord2f(1.0f, 0.0f); glVertex3f( 18.6f, 13.0f, 1.55f );
		glTexCoord2f(1.0f, 1.0f); glVertex3f( 18.6f, 13.0f, 1.1f );
		glTexCoord2f(0.0f, 1.0f); glVertex3f( 18.6f, 13.4f, 1.1f );
		glTexCoord2f(0.0f, 0.0f); glVertex3f( 18.6f, 13.4f, 1.55f );
		glEnd();
		//Cara izquierda
		glBegin(GL_QUADS);
		glNormal3f( -1.0f,0.0f,0.0f );
		glTexCoord2f(1.0f, 0.0f); glVertex3f( 16.4f, 13.0f, 1.55f );
		glTexCoord2f(1.0f, 1.0f); glVertex3f( 16.4f, 13.4f, 1.55f );
		glTexCoord2f(0.0f, 1.0f); glVertex3f( 16.4f, 13.4f, 1.1f );
		glTexCoord2f(0.0f, 0.0f); glVertex3f( 16.4f, 13.0f, 1.1f );
		glEnd();
		//Cara trasera
		glBegin(GL_QUADS);
		glNormal3f( 0.0f,0.0f,-1.0f );
		glTexCoord2f(1.0f, 0.0f); glVertex3f( 16.4f, 13.0f, 1.1f );
		glTexCoord2f(1.0f, 1.0f); glVertex3f( 16.4f, 13.4f, 1.1f );
		glTexCoord2f(0.0f, 1.0f); glVertex3f( 18.6f, 13.4f, 1.1f );
		glTexCoord2f(0.0f, 0.0f); glVertex3f( 18.6f, 13.0f, 1.1f );
		glEnd();
		//Cara superior
		glBegin(GL_QUADS);
		glNormal3f( 0.0f,1.0f,0.0f );
		glTexCoord2f(0.0f, 0.0f); glVertex3f( 16.4f, 13.4f, 1.55f );
		glTexCoord2f(1.0f, 0.0f); glVertex3f( 18.6f, 13.4f, 1.55f );
		glTexCoord2f(1.0f, 1.0f); glVertex3f( 18.6f, 13.4f, 1.1f );
		glTexCoord2f(0.0f, 1.0f); glVertex3f( 16.4f, 13.4f, 1.1f );
		glEnd();
		//Cara inferior
		glBegin(GL_QUADS);
		glNormal3f( 0.0f, -1.0f, 0.0f );
		glTexCoord2f(0.0f, 0.0f); glVertex3f( 16.4f, 13.0f, 1.55f );
		glTexCoord2f(1.0f, 0.0f); glVertex3f( 16.4f, 13.0f, 1.1f );
		glTexCoord2f(1.0f, 1.0f); glVertex3f( 18.6f, 13.0f, 1.1f );
		glTexCoord2f(0.0f, 1.0f); glVertex3f( 18.6f, 13.0f, 1.55f );
		glEnd();
	//Escalon9
		//Cara delantera
		glBegin(GL_QUADS);
		glNormal3f( 0.0f,0.0f,1.0f );
		glTexCoord2f(0.0f, 0.0f); glVertex3f( 16.4f, 14.4f, 1.55f );
		glTexCoord2f(1.0f, 0.0f); glVertex3f( 18.6f, 14.4f, 1.55f );
		glTexCoord2f(1.0f, 1.0f); glVertex3f( 18.6f, 14.8f, 1.55f );
		glTexCoord2f(0.0f, 1.0f); glVertex3f( 16.4f, 14.8f, 1.55f );
		glEnd();
		//Cara derecha
		glBegin(GL_QUADS);
		glNormal3f( 1.0f,0.0f,0.0f );
		glTexCoord2f(0.0f, 0.0f); glVertex3f( 18.6f, 14.4f, 1.55f );
		glTexCoord2f(1.0f, 0.0f); glVertex3f( 18.6f, 14.4f, 1.1f );
		glTexCoord2f(1.0f, 1.0f); glVertex3f( 18.6f, 14.8f, 1.1f );
		glTexCoord2f(0.0f, 1.0f); glVertex3f( 18.6f, 14.8f, 1.55f );
		glEnd();
		//Cara izquierda
		glBegin(GL_QUADS);
		glNormal3f( -1.0f,0.0f,0.0f );
		glTexCoord2f(1.0f, 0.0f); glVertex3f( 16.4f, 14.4f, 1.55f );
		glTexCoord2f(1.0f, 1.0f); glVertex3f( 16.4f, 14.8f, 1.55f );
		glTexCoord2f(0.0f, 1.0f); glVertex3f( 16.4f, 14.8f, 1.1f );
		glTexCoord2f(0.0f, 0.0f); glVertex3f( 16.4f, 14.4f, 1.1f );
		glEnd();
		//Cara trasera
		glBegin(GL_QUADS);
		glNormal3f( 0.0f,0.0f,-1.0f );
		glTexCoord2f(1.0f, 0.0f); glVertex3f( 16.4f, 14.4f, 1.1f );
		glTexCoord2f(1.0f, 1.0f); glVertex3f( 16.4f, 14.8f, 1.1f );
		glTexCoord2f(0.0f, 1.0f); glVertex3f( 18.6f, 14.8f, 1.1f );
		glTexCoord2f(0.0f, 0.0f); glVertex3f( 18.6f, 14.4f, 1.1f );
		glEnd();
		//Cara superior
		glBegin(GL_QUADS);
		glNormal3f( 0.0f,1.0f,0.0f );
		glTexCoord2f(0.0f, 0.0f); glVertex3f( 16.4f, 14.8f, 1.55f );
		glTexCoord2f(1.0f, 0.0f); glVertex3f( 18.6f, 14.8f, 1.55f );
		glTexCoord2f(1.0f, 1.0f); glVertex3f( 18.6f, 14.8f, 1.1f );
		glTexCoord2f(0.0f, 1.0f); glVertex3f( 16.4f, 14.8f, 1.1f );
		glEnd();
		//Cara inferior
		glBegin(GL_QUADS);
		glNormal3f( 0.0f, -1.0f, 0.0f );
		glTexCoord2f(0.0f, 0.0f); glVertex3f( 16.4f, 14.4f, 1.55f );
		glTexCoord2f(1.0f, 0.0f); glVertex3f( 16.4f, 14.4f, 1.1f );
		glTexCoord2f(1.0f, 1.0f); glVertex3f( 18.6f, 14.4f, 1.1f );
		glTexCoord2f(0.0f, 1.0f); glVertex3f( 18.6f, 14.4f, 1.55f );
		glEnd();
	//Escalon10
		//Cara delantera
		glBegin(GL_QUADS);
		glNormal3f( 0.0f,0.0f,1.0f );
		glTexCoord2f(0.0f, 0.0f); glVertex3f( 16.4f, 15.8f, 1.55f );
		glTexCoord2f(1.0f, 0.0f); glVertex3f( 18.6f, 15.8f, 1.55f );
		glTexCoord2f(1.0f, 1.0f); glVertex3f( 18.6f, 16.2f, 1.55f );
		glTexCoord2f(0.0f, 1.0f); glVertex3f( 16.4f, 16.2f, 1.55f );
		glEnd();
		//Cara derecha
		glBegin(GL_QUADS);
		glNormal3f( 1.0f,0.0f,0.0f );
		glTexCoord2f(0.0f, 0.0f); glVertex3f( 18.6f, 15.8f, 1.55f );
		glTexCoord2f(1.0f, 0.0f); glVertex3f( 18.6f, 15.8f, 1.1f );
		glTexCoord2f(1.0f, 1.0f); glVertex3f( 18.6f, 16.2f, 1.1f );
		glTexCoord2f(0.0f, 1.0f); glVertex3f( 18.6f, 16.2f, 1.55f );
		glEnd();
		//Cara izquierda
		glBegin(GL_QUADS);
		glNormal3f( -1.0f,0.0f,0.0f );
		glTexCoord2f(1.0f, 0.0f); glVertex3f( 16.4f, 15.8f, 1.55f );
		glTexCoord2f(1.0f, 1.0f); glVertex3f( 16.4f, 16.2f, 1.55f );
		glTexCoord2f(0.0f, 1.0f); glVertex3f( 16.4f, 16.2f, 1.1f );
		glTexCoord2f(0.0f, 0.0f); glVertex3f( 16.4f, 15.8f, 1.1f );
		glEnd();
		//Cara trasera
		glBegin(GL_QUADS);
		glNormal3f( 0.0f,0.0f,-1.0f );
		glTexCoord2f(1.0f, 0.0f); glVertex3f( 16.4f, 15.8f, 1.1f );
		glTexCoord2f(1.0f, 1.0f); glVertex3f( 16.4f, 16.2f, 1.1f );
		glTexCoord2f(0.0f, 1.0f); glVertex3f( 18.6f, 16.2f, 1.1f );
		glTexCoord2f(0.0f, 0.0f); glVertex3f( 18.6f, 15.8f, 1.1f );
		glEnd();
		//Cara superior
		glBegin(GL_QUADS);
		glNormal3f( 0.0f,1.0f,0.0f );
		glTexCoord2f(0.0f, 0.0f); glVertex3f( 16.4f, 16.2f, 1.55f );
		glTexCoord2f(1.0f, 0.0f); glVertex3f( 18.6f, 16.2f, 1.55f );
		glTexCoord2f(1.0f, 1.0f); glVertex3f( 18.6f, 16.2f, 1.1f );
		glTexCoord2f(0.0f, 1.0f); glVertex3f( 16.4f, 16.2f, 1.1f );
		glEnd();
		//Cara inferior
		glBegin(GL_QUADS);
		glNormal3f( 0.0f, -1.0f, 0.0f );
		glTexCoord2f(0.0f, 0.0f); glVertex3f( 16.4f, 15.8f, 1.55f );
		glTexCoord2f(1.0f, 0.0f); glVertex3f( 16.4f, 15.8f, 1.1f );
		glTexCoord2f(1.0f, 1.0f); glVertex3f( 18.6f, 15.8f, 1.1f );
		glTexCoord2f(0.0f, 1.0f); glVertex3f( 18.6f, 15.8f, 1.55f );
		glEnd();
	//Escalon11
		//Cara delantera
		glBegin(GL_QUADS);
		glNormal3f( 0.0f,0.0f,1.0f );
		glTexCoord2f(0.0f, 0.0f); glVertex3f( 16.4f, 17.2f, 1.55f );
		glTexCoord2f(1.0f, 0.0f); glVertex3f( 18.6f, 17.2f, 1.55f );
		glTexCoord2f(1.0f, 1.0f); glVertex3f( 18.6f, 17.6f, 1.55f );
		glTexCoord2f(0.0f, 1.0f); glVertex3f( 16.4f, 17.6f, 1.55f );
		glEnd();
		//Cara derecha
		glBegin(GL_QUADS);
		glNormal3f( 1.0f,0.0f,0.0f );
		glTexCoord2f(0.0f, 0.0f); glVertex3f( 18.6f, 17.2f, 1.55f );
		glTexCoord2f(1.0f, 0.0f); glVertex3f( 18.6f, 17.2f, 1.1f );
		glTexCoord2f(1.0f, 1.0f); glVertex3f( 18.6f, 17.6f, 1.1f );
		glTexCoord2f(0.0f, 1.0f); glVertex3f( 18.6f, 17.6f, 1.55f );
		glEnd();
		//Cara izquierda
		glBegin(GL_QUADS);
		glNormal3f( -1.0f,0.0f,0.0f );
		glTexCoord2f(1.0f, 0.0f); glVertex3f( 16.4f, 17.2f, 1.55f );
		glTexCoord2f(1.0f, 1.0f); glVertex3f( 16.4f, 17.6f, 1.55f );
		glTexCoord2f(0.0f, 1.0f); glVertex3f( 16.4f, 17.6f, 1.1f );
		glTexCoord2f(0.0f, 0.0f); glVertex3f( 16.4f, 17.2f, 1.1f );
		glEnd();
		//Cara trasera
		glBegin(GL_QUADS);
		glNormal3f( 0.0f,0.0f,-1.0f );
		glTexCoord2f(1.0f, 0.0f); glVertex3f( 16.4f, 17.2f, 1.1f );
		glTexCoord2f(1.0f, 1.0f); glVertex3f( 16.4f, 17.6f, 1.1f );
		glTexCoord2f(0.0f, 1.0f); glVertex3f( 18.6f, 17.6f, 1.1f );
		glTexCoord2f(0.0f, 0.0f); glVertex3f( 18.6f, 17.2f, 1.1f );
		glEnd();
		//Cara superior
		glBegin(GL_QUADS);
		glNormal3f( 0.0f,1.0f,0.0f );
		glTexCoord2f(0.0f, 0.0f); glVertex3f( 16.4f, 17.6f, 1.55f );
		glTexCoord2f(1.0f, 0.0f); glVertex3f( 18.6f, 17.6f, 1.55f );
		glTexCoord2f(1.0f, 1.0f); glVertex3f( 18.6f, 17.6f, 1.1f );
		glTexCoord2f(0.0f, 1.0f); glVertex3f( 16.4f, 17.6f, 1.1f );
		glEnd();
		//Cara inferior
		glBegin(GL_QUADS);
		glNormal3f( 0.0f, -1.0f, 0.0f );
		glTexCoord2f(0.0f, 0.0f); glVertex3f( 16.4f, 17.2f, 1.55f );
		glTexCoord2f(1.0f, 0.0f); glVertex3f( 16.4f, 17.2f, 1.1f );
		glTexCoord2f(1.0f, 1.0f); glVertex3f( 18.6f, 17.2f, 1.1f );
		glTexCoord2f(0.0f, 1.0f); glVertex3f( 18.6f, 17.2f, 1.55f );
		glEnd();
	//Escalon12
		//Cara delantera
		glBegin(GL_QUADS);
		glNormal3f( 0.0f,0.0f,1.0f );
		glTexCoord2f(1.0f, 0.0f); glVertex3f( 16.4f, 18.6f, 1.55f );
		glTexCoord2f(1.0f, 1.0f); glVertex3f( 18.6f, 18.6f, 1.55f );
		glTexCoord2f(0.0f, 1.0f); glVertex3f( 18.6f, 19.0f, 1.55f );
		glTexCoord2f(0.0f, 0.0f); glVertex3f( 16.4f, 19.0f, 1.55f );
		glEnd();
		//Cara derecha
		glBegin(GL_QUADS);
		glNormal3f( 1.0f,0.0f,0.0f );
		glTexCoord2f(0.0f, 0.0f); glVertex3f( 18.6f, 18.6f, 1.55f );
		glTexCoord2f(1.0f, 0.0f); glVertex3f( 18.6f, 18.6f, 1.1f );
		glTexCoord2f(1.0f, 1.0f); glVertex3f( 18.6f, 19.0f, 1.1f );
		glTexCoord2f(0.0f, 1.0f); glVertex3f( 18.6f, 19.0f, 1.55f );
		glEnd();
		//Cara izquierda
		glBegin(GL_QUADS);
		glNormal3f( -1.0f,0.0f,0.0f );
		glTexCoord2f(1.0f, 0.0f); glVertex3f( 16.4f, 18.6f, 1.55f );
		glTexCoord2f(1.0f, 1.0f); glVertex3f( 16.4f, 19.0f, 1.55f );
		glTexCoord2f(0.0f, 1.0f); glVertex3f( 16.4f, 19.0f, 1.1f );
		glTexCoord2f(0.0f, 0.0f); glVertex3f( 16.4f, 18.6f, 1.1f );
		glEnd();
		//Cara trasera
		glBegin(GL_QUADS);
		glNormal3f( 0.0f,0.0f,-1.0f );
		glTexCoord2f(1.0f, 0.0f); glVertex3f( 16.4f, 18.6f, 1.1f );
		glTexCoord2f(1.0f, 1.0f); glVertex3f( 16.4f, 19.0f, 1.1f );
		glTexCoord2f(0.0f, 1.0f); glVertex3f( 18.6f, 19.0f, 1.1f );
		glTexCoord2f(0.0f, 0.0f); glVertex3f( 18.6f, 18.6f, 1.1f );
		glEnd();
		//Cara superior
		glBegin(GL_QUADS);
		glNormal3f( 0.0f,1.0f,0.0f );
		glTexCoord2f(0.0f, 0.0f); glVertex3f( 16.4f, 19.0f, 1.55f );
		glTexCoord2f(1.0f, 0.0f); glVertex3f( 18.6f, 19.0f, 1.55f );
		glTexCoord2f(1.0f, 1.0f); glVertex3f( 18.6f, 19.0f, 1.1f );
		glTexCoord2f(0.0f, 1.0f); glVertex3f( 16.4f, 19.0f, 1.1f );
		glEnd();
		//Cara inferior
		glBegin(GL_QUADS);
		glNormal3f( 0.0f, -1.0f, 0.0f );
		glTexCoord2f(0.0f, 0.0f); glVertex3f( 16.4f, 18.6f, 1.55f );
		glTexCoord2f(1.0f, 0.0f); glVertex3f( 16.4f, 18.6f, 1.1f );
		glTexCoord2f(1.0f, 1.0f); glVertex3f( 18.6f, 18.6f, 1.1f );
		glTexCoord2f(0.0f, 1.0f); glVertex3f( 18.6f, 18.6f, 1.55f );
		glEnd();
	//Escalon13
		//Cara delantera
		glBegin(GL_QUADS);
		glNormal3f( 0.0f,0.0f,1.0f );
		glTexCoord2f(0.0f, 0.0f); glVertex3f( 16.4f, 20.0f, 1.55f );
		glTexCoord2f(1.0f, 0.0f); glVertex3f( 18.6f, 20.0f, 1.55f );
		glTexCoord2f(1.0f, 1.0f); glVertex3f( 18.6f, 20.4f, 1.55f );
		glTexCoord2f(0.0f, 1.0f); glVertex3f( 16.4f, 20.4f, 1.55f );
		glEnd();
		//Cara derecha
		glBegin(GL_QUADS);
		glNormal3f( 1.0f,0.0f,0.0f );
		glTexCoord2f(0.0f, 0.0f); glVertex3f( 18.6f, 20.0f, 1.55f );
		glTexCoord2f(1.0f, 0.0f); glVertex3f( 18.6f, 20.0f, 1.1f );
		glTexCoord2f(1.0f, 1.0f); glVertex3f( 18.6f, 20.4f, 1.1f );
		glTexCoord2f(0.0f, 1.0f); glVertex3f( 18.6f, 20.4f, 1.55f );
		glEnd();
		//Cara izquierda
		glBegin(GL_QUADS);
		glNormal3f( -1.0f,0.0f,0.0f );
		glTexCoord2f(1.0f, 0.0f); glVertex3f( 16.4f, 20.0f, 1.55f );
		glTexCoord2f(1.0f, 1.0f); glVertex3f( 16.4f, 20.4f, 1.55f );
		glTexCoord2f(0.0f, 1.0f); glVertex3f( 16.4f, 20.4f, 1.1f );
		glTexCoord2f(0.0f, 0.0f); glVertex3f( 16.4f, 20.0f, 1.1f );
		glEnd();
		//Cara trasera
		glBegin(GL_QUADS);
		glNormal3f( 0.0f,0.0f,-1.0f );
		glTexCoord2f(1.0f, 0.0f); glVertex3f( 16.4f, 20.0f, 1.1f );
		glTexCoord2f(1.0f, 1.0f); glVertex3f( 16.4f, 20.4f, 1.1f );
		glTexCoord2f(0.0f, 1.0f); glVertex3f( 18.6f, 20.4f, 1.1f );
		glTexCoord2f(0.0f, 0.0f); glVertex3f( 18.6f, 20.0f, 1.1f );
		glEnd();
		//Cara superior
		glBegin(GL_QUADS);
		glNormal3f( 0.0f,1.0f,0.0f );
		glTexCoord2f(0.0f, 0.0f); glVertex3f( 16.4f, 20.4f, 1.55f );
		glTexCoord2f(1.0f, 0.0f); glVertex3f( 18.6f, 20.4f, 1.55f );
		glTexCoord2f(1.0f, 1.0f); glVertex3f( 18.6f, 20.4f, 1.1f );
		glTexCoord2f(0.0f, 1.0f); glVertex3f( 16.4f, 20.4f, 1.1f );
		glEnd();
		//Cara inferior
		glBegin(GL_QUADS);
		glNormal3f( 0.0f, -1.0f, 0.0f );
		glTexCoord2f(0.0f, 0.0f); glVertex3f( 16.4f, 20.0f, 1.55f );
		glTexCoord2f(1.0f, 0.0f); glVertex3f( 16.4f, 20.0f, 1.1f );
		glTexCoord2f(1.0f, 1.0f); glVertex3f( 18.6f, 20.0f, 1.1f );
		glTexCoord2f(0.0f, 1.0f); glVertex3f( 18.6f, 20.0f, 1.55f );
		glEnd();
//EscaleraAzul
	//verticalder
		//Cara delantera
		glPushMatrix();
		glTranslatef(-39.0,0.0,0.0);
		glBindTexture(GL_TEXTURE_2D, textura[13].texID);
		glBegin(GL_QUADS);
		glNormal3f( 0.0f,0.0f,1.0f );
		glTexCoord2f(0.0f, 0.0f); glVertex3f( 18.6f, 0.0f, 1.55f );
		glTexCoord2f(1.0f, 0.0f); glVertex3f( 19.0f, 0.0f, 1.55f );
		glTexCoord2f(1.0f, 16.0f); glVertex3f( 19.0f, 23.0f, 1.55f );
		glTexCoord2f(0.0f, 16.0f); glVertex3f( 18.6f, 23.0f, 1.55f );
		glEnd();
		//Cara derecha
		glBegin(GL_QUADS);
		glNormal3f( 1.0f,0.0f,0.0f );
		glTexCoord2f(0.0f, 0.0f); glVertex3f( 19.0f, 0.0f, 1.55f );
		glTexCoord2f(1.0f, 0.0f); glVertex3f( 19.0f, 0.0f, 1.1f );
		glTexCoord2f(1.0f, 16.0f); glVertex3f( 19.0f, 23.0f, 1.1f );
		glTexCoord2f(0.0f, 16.0f); glVertex3f( 19.0f, 23.0f, 1.55f );
		glEnd();
		//Cara izquierda
		glBegin(GL_QUADS);
		glNormal3f( -1.0f,0.0f,0.0f );
		glTexCoord2f(1.0f, 0.0f); glVertex3f( 18.6f, 0.0f, 1.55f );
		glTexCoord2f(1.0f, 16.0f); glVertex3f( 18.6f, 23.0f, 1.55f );
		glTexCoord2f(0.0f, 16.0f); glVertex3f( 18.6f, 23.0f, 1.1f );
		glTexCoord2f(0.0f, 0.0f); glVertex3f( 18.6f, 0.0f, 1.1f );
		glEnd();
		//Cara trasera
		glBegin(GL_QUADS);
		glNormal3f( 0.0f,0.0f,-1.0f );
		glTexCoord2f(1.0f, 0.0f); glVertex3f( 18.6f, 0.0f, 1.1f );
		glTexCoord2f(1.0f, 16.0f); glVertex3f( 18.6f, 23.0f, 1.1f );
		glTexCoord2f(0.0f, 16.0f); glVertex3f( 19.0f, 23.0f, 1.1f );
		glTexCoord2f(0.0f, 0.0f); glVertex3f( 19.0f, 0.0f, 1.1f );
		glEnd();
		//Cara superior
		glBindTexture(GL_TEXTURE_2D, textura[14].texID);
		glBegin(GL_QUADS);
		glNormal3f( 0.0f,1.0f,0.0f );
		glTexCoord2f(0.0f, 0.0f); glVertex3f( 18.6f, 23.0f, 1.55f );
		glTexCoord2f(1.0f, 0.0f); glVertex3f( 19.0f, 23.0f, 1.55f );
		glTexCoord2f(1.0f, 1.0f); glVertex3f( 19.0f, 23.0f, 1.1f );
		glTexCoord2f(0.0f, 1.0f); glVertex3f( 18.6f, 23.0f, 1.1f );
		glEnd();
		//Cara inferior
		glBegin(GL_QUADS);
		glNormal3f( 0.0f, -1.0f, 0.0f );
		glTexCoord2f(0.0f, 0.0f); glVertex3f( 18.6f, 0.0f, 1.55f );
		glTexCoord2f(1.0f, 0.0f); glVertex3f( 18.6f, 0.0f, 1.1f );
		glTexCoord2f(1.0f, 1.0f); glVertex3f( 19.0f, 0.0f, 1.1f );
		glTexCoord2f(0.0f, 1.0f); glVertex3f( 19.0f, 0.0f, 1.55f );
		glEnd();
	//verticalizq
		//Cara delantera
		glBindTexture(GL_TEXTURE_2D, textura[13].texID);
		glBegin(GL_QUADS);
		glNormal3f( 0.0f,0.0f,1.0f );
		glTexCoord2f(0.0f, 0.0f); glVertex3f( 16.0f, 0.0f, 1.55f );
		glTexCoord2f(1.0f, 0.0f); glVertex3f( 16.4f, 0.0f, 1.55f );
		glTexCoord2f(1.0f, 16.0f); glVertex3f( 16.4f, 23.0f, 1.55f );
		glTexCoord2f(0.0f, 16.0f); glVertex3f( 16.0f, 23.0f, 1.55f );
		glEnd();
		//Cara derecha
		glBegin(GL_QUADS);
		glNormal3f( 1.0f,0.0f,0.0f );
		glTexCoord2f(0.0f, 0.0f); glVertex3f( 16.4f, 0.0f, 1.55f );
		glTexCoord2f(1.0f, 0.0f); glVertex3f( 16.4f, 0.0f, 1.1f );
		glTexCoord2f(1.0f, 16.0f); glVertex3f( 16.4f, 23.0f, 1.1f );
		glTexCoord2f(0.0f, 16.0f); glVertex3f( 16.4f, 23.0f, 1.55f );
		glEnd();
		//Cara izquierda
		glBegin(GL_QUADS);
		glNormal3f( -1.0f,0.0f,0.0f );
		glTexCoord2f(1.0f, 0.0f); glVertex3f( 16.0f, 0.0f, 1.55f );
		glTexCoord2f(1.0f, 16.0f); glVertex3f( 16.0f, 23.0f, 1.55f );
		glTexCoord2f(0.0f, 16.0f); glVertex3f( 16.0f, 23.0f, 1.1f );
		glTexCoord2f(0.0f, 0.0f); glVertex3f( 16.0f, 0.0f, 1.1f );
		glEnd();
		//Cara trasera
		glBegin(GL_QUADS);
		glNormal3f( 0.0f,0.0f,-1.0f );
		glTexCoord2f(1.0f, 0.0f); glVertex3f( 16.0f, 0.0f, 1.1f );
		glTexCoord2f(1.0f, 16.0f); glVertex3f( 16.0f, 23.0f, 1.1f );
		glTexCoord2f(0.0f, 16.0f); glVertex3f( 16.4f, 23.0f, 1.1f );
		glTexCoord2f(0.0f, 0.0f); glVertex3f( 16.4f, 0.0f, 1.1f );
		glEnd();
		glBindTexture(GL_TEXTURE_2D, textura[14].texID);
		//Cara superior
		glBegin(GL_QUADS);
		glNormal3f( 0.0f,1.0f,0.0f );
		glTexCoord2f(0.0f, 0.0f); glVertex3f( 16.0f, 23.0f, 1.55f );
		glTexCoord2f(1.0f, 0.0f); glVertex3f( 16.4f, 23.0f, 1.55f );
		glTexCoord2f(1.0f, 1.0f); glVertex3f( 16.4f, 23.0f, 1.1f );
		glTexCoord2f(0.0f, 1.0f); glVertex3f( 16.0f, 23.0f, 1.1f );
		glEnd();
		//Cara inferior
		glBegin(GL_QUADS);
		glNormal3f( 0.0f, -1.0f, 0.0f );
		glTexCoord2f(0.0f, 0.0f); glVertex3f( 16.0f, 0.0f, 1.55f );
		glTexCoord2f(1.0f, 0.0f); glVertex3f( 16.0f, 0.0f, 1.1f );
		glTexCoord2f(1.0f, 1.0f); glVertex3f( 16.4f, 0.0f, 1.1f );
		glTexCoord2f(0.0f, 1.0f); glVertex3f( 16.4f, 0.0f, 1.55f );
		glEnd();
	//Escalon1
		//Cara delantera
		glBegin(GL_QUADS);
		glNormal3f( 0.0f,0.0f,1.0f );
		glTexCoord2f(0.0f, 0.0f); glVertex3f( 16.4f, 1.0f, 1.55f );
		glTexCoord2f(1.0f, 0.0f); glVertex3f( 18.6f, 1.0f, 1.55f );
		glTexCoord2f(1.0f, 1.0f); glVertex3f( 18.6f, 1.4f, 1.55f );
		glTexCoord2f(0.0f, 1.0f); glVertex3f( 16.4f, 1.4f, 1.55f );
		glEnd();
		//Cara derecha
		glBegin(GL_QUADS);
		glNormal3f( 1.0f,0.0f,0.0f );
		glTexCoord2f(0.0f, 0.0f); glVertex3f( 18.6f, 1.0f, 1.55f );
		glTexCoord2f(1.0f, 0.0f); glVertex3f( 18.6f, 1.0f, 1.1f );
		glTexCoord2f(1.0f, 1.0f); glVertex3f( 18.6f, 1.4f, 1.1f );
		glTexCoord2f(0.0f, 1.0f); glVertex3f( 18.6f, 1.4f, 1.55f );
		glEnd();
		//Cara izquierda
		glBegin(GL_QUADS);
		glNormal3f( -1.0f,0.0f,0.0f );
		glTexCoord2f(1.0f, 0.0f); glVertex3f( 16.4f, 1.0f, 1.55f );
		glTexCoord2f(1.0f, 1.0f); glVertex3f( 16.4f, 1.4f, 1.55f );
		glTexCoord2f(0.0f, 1.0f); glVertex3f( 16.4f, 1.4f, 1.1f );
		glTexCoord2f(0.0f, 0.0f); glVertex3f( 16.4f, 1.0f, 1.1f );
		glEnd();
		//Cara trasera
		glBegin(GL_QUADS);
		glNormal3f( 0.0f,0.0f,-1.0f );
		glTexCoord2f(1.0f, 0.0f); glVertex3f( 16.4f, 1.0f, 1.1f );
		glTexCoord2f(1.0f, 1.0f); glVertex3f( 16.4f, 1.4f, 1.1f );
		glTexCoord2f(0.0f, 1.0f); glVertex3f( 18.6f, 1.4f, 1.1f );
		glTexCoord2f(0.0f, 0.0f); glVertex3f( 18.6f, 1.0f, 1.1f );
		glEnd();
		//Cara superior
		glBegin(GL_QUADS);
		glNormal3f( 0.0f,1.0f,0.0f );
		glTexCoord2f(0.0f, 0.0f); glVertex3f( 16.4f, 1.4f, 1.55f );
		glTexCoord2f(1.0f, 0.0f); glVertex3f( 18.6f, 1.4f, 1.55f );
		glTexCoord2f(1.0f, 1.0f); glVertex3f( 18.6f, 1.4f, 1.1f );
		glTexCoord2f(0.0f, 1.0f); glVertex3f( 16.4f, 1.4f, 1.1f );
		glEnd();
		//Cara inferior
		glBegin(GL_QUADS);
		glNormal3f( 0.0f, -1.0f, 0.0f );
		glTexCoord2f(0.0f, 0.0f); glVertex3f( 16.4f, 1.0f, 1.55f );
		glTexCoord2f(1.0f, 0.0f); glVertex3f( 16.4f, 1.0f, 1.1f );
		glTexCoord2f(1.0f, 1.0f); glVertex3f( 18.6f, 1.0f, 1.1f );
		glTexCoord2f(0.0f, 1.0f); glVertex3f( 18.6f, 1.0f, 1.55f );
		glEnd();
	//Escalon2
		//Cara delantera
		glBegin(GL_QUADS);
		glNormal3f( 0.0f,0.0f,1.0f );
		glTexCoord2f(0.0f, 0.0f); glVertex3f( 16.4f, 2.4f, 1.55f );
		glTexCoord2f(1.0f, 0.0f); glVertex3f( 18.6f, 2.4f, 1.55f );
		glTexCoord2f(1.0f, 1.0f); glVertex3f( 18.6f, 2.8f, 1.55f );
		glTexCoord2f(0.0f, 1.0f); glVertex3f( 16.4f, 2.8f, 1.55f );
		glEnd();
		//Cara derecha
		glBegin(GL_QUADS);
		glNormal3f( 1.0f,0.0f,0.0f );
		glTexCoord2f(0.0f, 0.0f); glVertex3f( 18.6f, 2.4f, 1.55f );
		glTexCoord2f(1.0f, 0.0f); glVertex3f( 18.6f, 2.4f, 1.1f );
		glTexCoord2f(1.0f, 1.0f); glVertex3f( 18.6f, 2.8f, 1.1f );
		glTexCoord2f(0.0f, 1.0f); glVertex3f( 18.6f, 2.8f, 1.55f );
		glEnd();
		//Cara izquierda
		glBegin(GL_QUADS);
		glNormal3f( -1.0f,0.0f,0.0f );
		glTexCoord2f(1.0f, 0.0f); glVertex3f( 16.4f, 2.4f, 1.55f );
		glTexCoord2f(1.0f, 1.0f); glVertex3f( 16.4f, 2.8f, 1.55f );
		glTexCoord2f(0.0f, 1.0f); glVertex3f( 16.4f, 2.8f, 1.1f );
		glTexCoord2f(0.0f, 0.0f); glVertex3f( 16.4f, 2.4f, 1.1f );
		glEnd();
		//Cara trasera
		glBegin(GL_QUADS);
		glNormal3f( 0.0f,0.0f,-1.0f );
		glTexCoord2f(1.0f, 0.0f); glVertex3f( 16.4f, 2.4f, 1.1f );
		glTexCoord2f(1.0f, 0.0f); glVertex3f( 16.4f, 2.8f, 1.1f );
		glTexCoord2f(0.0f, 1.0f); glVertex3f( 18.6f, 2.8f, 1.1f );
		glTexCoord2f(0.0f, 1.0f); glVertex3f( 18.6f, 2.4f, 1.1f );
		glEnd();
		//Cara superior
		glBegin(GL_QUADS);
		glNormal3f( 0.0f,1.0f,0.0f );
		glTexCoord2f(0.0f, 0.0f); glVertex3f( 16.4f, 2.8f, 1.55f );
		glTexCoord2f(1.0f, 0.0f); glVertex3f( 18.6f, 2.8f, 1.55f );
		glTexCoord2f(1.0f, 1.0f); glVertex3f( 18.6f, 2.8f, 1.1f );
		glTexCoord2f(0.0f, 1.0f); glVertex3f( 16.4f, 2.8f, 1.1f );
		glEnd();
		//Cara inferior
		glBegin(GL_QUADS);
		glNormal3f( 0.0f, -1.0f, 0.0f );
		glTexCoord2f(0.0f, 0.0f); glVertex3f( 16.4f, 2.4f, 1.55f );
		glTexCoord2f(1.0f, 0.0f); glVertex3f( 16.4f, 2.4f, 1.1f );
		glTexCoord2f(1.0f, 1.0f); glVertex3f( 18.6f, 2.4f, 1.1f );
		glTexCoord2f(0.0f, 1.0f); glVertex3f( 18.6f, 2.4f, 1.55f );
		glEnd();
	//Escalon3
		//Cara delantera
		glBegin(GL_QUADS);
		glNormal3f( 0.0f,0.0f,1.0f );
		glTexCoord2f(0.0f, 0.0f); glVertex3f( 16.4f, 3.7f, 1.55f );
		glTexCoord2f(1.0f, 0.0f); glVertex3f( 18.6f, 3.7f, 1.55f );
		glTexCoord2f(1.0f, 1.0f); glVertex3f( 18.6f, 4.2f, 1.55f );
		glTexCoord2f(0.0f, 1.0f); glVertex3f( 16.4f, 4.2f, 1.55f );
		glEnd();
		//Cara derecha
		glBegin(GL_QUADS);
		glNormal3f( 1.0f,0.0f,0.0f );
		glTexCoord2f(0.0f, 0.0f); glVertex3f( 18.6f, 3.7f, 1.55f );
		glTexCoord2f(1.0f, 0.0f); glVertex3f( 18.6f, 3.7f, 1.1f );
		glTexCoord2f(1.0f, 1.0f); glVertex3f( 18.6f, 4.2f, 1.1f );
		glTexCoord2f(0.0f, 1.0f); glVertex3f( 18.6f, 4.2f, 1.55f );
		glEnd();
		//Cara izquierda
		glBegin(GL_QUADS);
		glNormal3f( -1.0f,0.0f,0.0f );
		glTexCoord2f(1.0f, 0.0f); glVertex3f( 16.4f, 3.7f, 1.55f );
		glTexCoord2f(1.0f, 1.0f); glVertex3f( 16.4f, 4.2f, 1.55f );
		glTexCoord2f(0.0f, 1.0f); glVertex3f( 16.4f, 4.2f, 1.1f );
		glTexCoord2f(0.0f, 0.0f); glVertex3f( 16.4f, 3.7f, 1.1f );
		glEnd();
		//Cara trasera
		glBegin(GL_QUADS);
		glNormal3f( 0.0f,0.0f,-1.0f );
		glTexCoord2f(1.0f, 0.0f); glVertex3f( 16.4f, 3.7f, 1.1f );
		glTexCoord2f(1.0f, 1.0f); glVertex3f( 16.4f, 4.2f, 1.1f );
		glTexCoord2f(0.0f, 1.0f); glVertex3f( 18.6f, 4.2f, 1.1f );
		glTexCoord2f(0.0f, 0.0f); glVertex3f( 18.6f, 3.7f, 1.1f );
		glEnd();
		//Cara superior
		glBegin(GL_QUADS);
		glNormal3f( 0.0f,1.0f,0.0f );
		glTexCoord2f(0.0f, 0.0f); glVertex3f( 16.4f, 4.2f, 1.55f );
		glTexCoord2f(1.0f, 0.0f); glVertex3f( 18.6f, 4.2f, 1.55f );
		glTexCoord2f(1.0f, 1.0f); glVertex3f( 18.6f, 4.2f, 1.1f );
		glTexCoord2f(0.0f, 1.0f); glVertex3f( 16.4f, 4.2f, 1.1f );
		glEnd();
		//Cara inferior
		glBegin(GL_QUADS);
		glNormal3f( 0.0f, -1.0f, 0.0f );
		glTexCoord2f(0.0f, 0.0f); glVertex3f( 16.4f, 3.7f, 1.55f );
		glTexCoord2f(1.0f, 0.0f); glVertex3f( 16.4f, 3.7f, 1.1f );
		glTexCoord2f(1.0f, 1.0f); glVertex3f( 18.6f, 3.7f, 1.1f );
		glTexCoord2f(0.0f, 1.0f); glVertex3f( 18.6f, 3.7f, 1.55f );
		glEnd();
	//Escalon4
		//Cara delantera
		glBegin(GL_QUADS);
		glNormal3f( 0.0f,0.0f,1.0f );
		glTexCoord2f(0.0f, 0.0f); glVertex3f( 16.4f, 5.2f, 1.55f );
		glTexCoord2f(1.0f, 0.0f); glVertex3f( 18.6f, 5.2f, 1.55f );
		glTexCoord2f(1.0f, 1.0f); glVertex3f( 18.6f, 5.6f, 1.55f );
		glTexCoord2f(0.0f, 1.0f); glVertex3f( 16.4f, 5.6f, 1.55f );
		glEnd();
		//Cara derecha
		glBegin(GL_QUADS);
		glNormal3f( 1.0f,0.0f,0.0f );
		glTexCoord2f(0.0f, 0.0f); glVertex3f( 18.6f, 5.2f, 1.55f );
		glTexCoord2f(1.0f, 0.0f); glVertex3f( 18.6f, 5.2f, 1.1f );
		glTexCoord2f(1.0f, 1.0f); glVertex3f( 18.6f, 5.6f, 1.1f );
		glTexCoord2f(0.0f, 1.0f); glVertex3f( 18.6f, 5.6f, 1.55f );
		glEnd();
		//Cara izquierda
		glBegin(GL_QUADS);
		glNormal3f( -1.0f,0.0f,0.0f );
		glTexCoord2f(1.0f, 0.0f); glVertex3f( 16.4f, 5.2f, 1.55f );
		glTexCoord2f(1.0f, 1.0f); glVertex3f( 16.4f, 5.6f, 1.55f );
		glTexCoord2f(0.0f, 1.0f); glVertex3f( 16.4f, 5.6f, 1.1f );
		glTexCoord2f(0.0f, 0.0f); glVertex3f( 16.4f, 5.2f, 1.1f );
		glEnd();
		//Cara trasera
		glBegin(GL_QUADS);
		glNormal3f( 0.0f,0.0f,-1.0f );
		glTexCoord2f(1.0f, 0.0f); glVertex3f( 16.4f, 5.2f, 1.1f );
		glTexCoord2f(1.0f, 1.0f); glVertex3f( 16.4f, 5.6f, 1.1f );
		glTexCoord2f(0.0f, 1.0f); glVertex3f( 18.6f, 5.6f, 1.1f );
		glTexCoord2f(0.0f, 0.0f); glVertex3f( 18.6f, 5.2f, 1.1f );
		glEnd();
		//Cara superior
		glBegin(GL_QUADS);
		glNormal3f( 0.0f,1.0f,0.0f );
		glTexCoord2f(0.0f, 0.0f); glVertex3f( 16.4f, 5.6f, 1.55f );
		glTexCoord2f(1.0f, 0.0f); glVertex3f( 18.6f, 5.6f, 1.55f );
		glTexCoord2f(1.0f, 1.0f); glVertex3f( 18.6f, 5.6f, 1.1f );
		glTexCoord2f(0.0f, 1.0f); glVertex3f( 16.4f, 5.6f, 1.1f );
		glEnd();
		//Cara inferior
		glBegin(GL_QUADS);
		glNormal3f( 0.0f, -1.0f, 0.0f );
		glTexCoord2f(0.0f, 0.0f); glVertex3f( 16.4f, 5.2f, 1.55f );
		glTexCoord2f(1.0f, 0.0f); glVertex3f( 16.4f, 5.2f, 1.1f );
		glTexCoord2f(1.0f, 1.0f); glVertex3f( 18.6f, 5.2f, 1.1f );
		glTexCoord2f(0.0f, 1.0f); glVertex3f( 18.6f, 5.2f, 1.55f );
		glEnd();
	//Escalon5
		//Cara delantera
		glBegin(GL_QUADS);
		glNormal3f( 0.0f,0.0f,1.0f );
		glTexCoord2f(0.0f, 0.0f); glVertex3f( 16.4f, 6.6f, 1.55f );
		glTexCoord2f(1.0f, 0.0f); glVertex3f( 18.6f, 6.6f, 1.55f );
		glTexCoord2f(1.0f, 1.0f); glVertex3f( 18.6f, 7.0f, 1.55f );
		glTexCoord2f(0.0f, 1.0f); glVertex3f( 16.4f, 7.0f, 1.55f );
		glEnd();
		//Cara derecha
		glBegin(GL_QUADS);
		glNormal3f( 1.0f,0.0f,0.0f );
		glTexCoord2f(0.0f, 0.0f); glVertex3f( 18.6f, 6.6f, 1.55f );
		glTexCoord2f(1.0f, 0.0f); glVertex3f( 18.6f, 6.6f, 1.1f );
		glTexCoord2f(1.0f, 1.0f); glVertex3f( 18.6f, 7.0f, 1.1f );
		glTexCoord2f(0.0f, 1.0f); glVertex3f( 18.6f, 7.0f, 1.55f );
		glEnd();
		//Cara izquierda
		glBegin(GL_QUADS);
		glNormal3f( -1.0f,0.0f,0.0f );
		glTexCoord2f(1.0f, 0.0f); glVertex3f( 16.4f, 6.6f, 1.55f );
		glTexCoord2f(1.0f, 1.0f); glVertex3f( 16.4f, 7.0f, 1.55f );
		glTexCoord2f(0.0f, 1.0f); glVertex3f( 16.4f, 7.0f, 1.1f );
		glTexCoord2f(0.0f, 0.0f); glVertex3f( 16.4f, 6.6f, 1.1f );
		glEnd();
		//Cara trasera
		glBegin(GL_QUADS);
		glNormal3f( 0.0f,0.0f,-1.0f );
		glTexCoord2f(1.0f, 0.0f); glVertex3f( 16.4f, 6.6f, 1.1f );
		glTexCoord2f(1.0f, 1.0f); glVertex3f( 16.4f, 7.0f, 1.1f );
		glTexCoord2f(0.0f, 1.0f); glVertex3f( 18.6f, 7.0f, 1.1f );
		glTexCoord2f(0.0f, 0.0f); glVertex3f( 18.6f, 6.6f, 1.1f );
		glEnd();
		//Cara superior
		glBegin(GL_QUADS);
		glNormal3f( 0.0f,1.0f,0.0f );
		glTexCoord2f(0.0f, 0.0f); glVertex3f( 16.4f, 7.0f, 1.55f );
		glTexCoord2f(1.0f, 0.0f); glVertex3f( 18.6f, 7.0f, 1.55f );
		glTexCoord2f(1.0f, 1.0f); glVertex3f( 18.6f, 7.0f, 1.1f );
		glTexCoord2f(0.0f, 1.0f); glVertex3f( 16.4f, 7.0f, 1.1f );
		glEnd();
		//Cara inferior
		glBegin(GL_QUADS);
		glNormal3f( 0.0f, -1.0f, 0.0f );
		glTexCoord2f(0.0f, 0.0f); glVertex3f( 16.4f, 6.6f, 1.55f );
		glTexCoord2f(1.0f, 0.0f); glVertex3f( 16.4f, 6.6f, 1.1f );
		glTexCoord2f(1.0f, 1.0f); glVertex3f( 18.6f, 6.6f, 1.1f );
		glTexCoord2f(0.0f, 1.0f); glVertex3f( 18.6f, 6.6f, 1.55f );
		glEnd();
	//Escalon6
		//Cara delantera
		glBegin(GL_QUADS);
		glNormal3f( 0.0f,0.0f,1.0f );
		glTexCoord2f(0.0f, 0.0f); glVertex3f( 16.4f, 8.0f, 1.55f );
		glTexCoord2f(1.0f, 0.0f); glVertex3f( 18.6f, 8.0f, 1.55f );
		glTexCoord2f(1.0f, 1.0f); glVertex3f( 18.6f, 8.4f, 1.55f );
		glTexCoord2f(0.0f, 1.0f); glVertex3f( 16.4f, 8.4f, 1.55f );
		glEnd();
		//Cara derecha
		glBegin(GL_QUADS);
		glNormal3f( 1.0f,0.0f,0.0f );
		glTexCoord2f(0.0f, 0.0f); glVertex3f( 18.6f, 8.0f, 1.55f );
		glTexCoord2f(1.0f, 0.0f); glVertex3f( 18.6f, 8.0f, 1.1f );
		glTexCoord2f(1.0f, 1.0f); glVertex3f( 18.6f, 8.4f, 1.1f );
		glTexCoord2f(0.0f, 1.0f); glVertex3f( 18.6f, 8.4f, 1.55f );
		glEnd();
		//Cara izquierda
		glBegin(GL_QUADS);
		glNormal3f( -1.0f,0.0f,0.0f );
		glTexCoord2f(1.0f, 0.0f); glVertex3f( 16.4f, 8.0f, 1.55f );
		glTexCoord2f(1.0f, 1.0f); glVertex3f( 16.4f, 8.4f, 1.55f );
		glTexCoord2f(0.0f, 1.0f); glVertex3f( 16.4f, 8.4f, 1.1f );
		glTexCoord2f(0.0f, 0.0f); glVertex3f( 16.4f, 8.0f, 1.1f );
		glEnd();
		//Cara trasera
		glBegin(GL_QUADS);
		glNormal3f( 0.0f,0.0f,-1.0f );
		glTexCoord2f(1.0f, 0.0f); glVertex3f( 16.4f, 8.0f, 1.1f );
		glTexCoord2f(1.0f, 1.0f); glVertex3f( 16.4f, 8.4f, 1.1f );
		glTexCoord2f(0.0f, 1.0f); glVertex3f( 18.6f, 8.4f, 1.1f );
		glTexCoord2f(0.0f, 0.0f); glVertex3f( 18.6f, 8.0f, 1.1f );
		glEnd();
		//Cara superior
		glBegin(GL_QUADS);
		glNormal3f( 0.0f,1.0f,0.0f );
		glTexCoord2f(0.0f, 0.0f); glVertex3f( 16.4f, 8.4f, 1.55f );
		glTexCoord2f(1.0f, 0.0f); glVertex3f( 18.6f, 8.4f, 1.55f );
		glTexCoord2f(1.0f, 1.0f); glVertex3f( 18.6f, 8.4f, 1.1f );
		glTexCoord2f(0.0f, 1.0f); glVertex3f( 16.4f, 8.4f, 1.1f );
		glEnd();
		//Cara inferior
		glBegin(GL_QUADS);
		glNormal3f( 0.0f, -1.0f, 0.0f );
		glTexCoord2f(0.0f, 0.0f); glVertex3f( 16.4f, 8.0f, 1.55f );
		glTexCoord2f(1.0f, 0.0f); glVertex3f( 16.4f, 8.0f, 1.1f );
		glTexCoord2f(1.0f, 1.0f); glVertex3f( 18.6f, 8.0f, 1.1f );
		glTexCoord2f(0.0f, 1.0f); glVertex3f( 18.6f, 8.0f, 1.55f );
		glEnd();
	//Escalon7
		//Cara delantera
		glBegin(GL_QUADS);
		glNormal3f( 0.0f,0.0f,1.0f );
		glTexCoord2f(0.0f, 0.0f); glVertex3f( 16.4f, 9.4f, 1.55f );
		glTexCoord2f(1.0f, 0.0f); glVertex3f( 18.6f, 9.4f, 1.55f );
		glTexCoord2f(1.0f, 1.0f); glVertex3f( 18.6f, 9.8f, 1.55f );
		glTexCoord2f(0.0f, 1.0f); glVertex3f( 16.4f, 9.8f, 1.55f );
		glEnd();
		//Cara derecha
		glBegin(GL_QUADS);
		glNormal3f( 1.0f,0.0f,0.0f );
		glTexCoord2f(0.0f, 0.0f); glVertex3f( 18.6f, 9.4f, 1.55f );
		glTexCoord2f(1.0f, 0.0f); glVertex3f( 18.6f, 9.4f, 1.1f );
		glTexCoord2f(1.0f, 1.0f); glVertex3f( 18.6f, 9.8f, 1.1f );
		glTexCoord2f(0.0f, 1.0f); glVertex3f( 18.6f, 9.8f, 1.55f );
		glEnd();
		//Cara izquierda
		glBegin(GL_QUADS);
		glNormal3f( -1.0f,0.0f,0.0f );
		glTexCoord2f(1.0f, 0.0f); glVertex3f( 16.4f, 9.4f, 1.55f );
		glTexCoord2f(1.0f, 1.0f); glVertex3f( 16.4f, 9.8f, 1.55f );
		glTexCoord2f(0.0f, 1.0f); glVertex3f( 16.4f, 9.8f, 1.1f );
		glTexCoord2f(0.0f, 0.0f); glVertex3f( 16.4f, 9.4f, 1.1f );
		glEnd();
		//Cara trasera
		glBegin(GL_QUADS);
		glNormal3f( 0.0f,0.0f,-1.0f );
		glTexCoord2f(1.0f, 0.0f); glVertex3f( 16.4f, 9.4f, 1.1f );
		glTexCoord2f(1.0f, 1.0f); glVertex3f( 16.4f, 9.8f, 1.1f );
		glTexCoord2f(0.0f, 1.0f); glVertex3f( 18.6f, 9.8f, 1.1f );
		glTexCoord2f(0.0f, 0.0f); glVertex3f( 18.6f, 9.4f, 1.1f );
		glEnd();
		//Cara superior
		glBegin(GL_QUADS);
		glNormal3f( 0.0f,1.0f,0.0f );
		glTexCoord2f(0.0f, 0.0f); glVertex3f( 16.4f, 9.8f, 1.55f );
		glTexCoord2f(1.0f, 0.0f); glVertex3f( 18.6f, 9.8f, 1.55f );
		glTexCoord2f(1.0f, 1.0f); glVertex3f( 18.6f, 9.8f, 1.1f );
		glTexCoord2f(0.0f, 1.0f); glVertex3f( 16.4f, 9.8f, 1.1f );
		glEnd();
		//Cara inferior
		glBegin(GL_QUADS);
		glNormal3f( 0.0f, -1.0f, 0.0f );
		glTexCoord2f(0.0f, 0.0f); glVertex3f( 16.4f, 9.4f, 1.55f );
		glTexCoord2f(1.0f, 0.0f); glVertex3f( 16.4f, 9.4f, 1.1f );
		glTexCoord2f(1.0f, 1.0f); glVertex3f( 18.6f, 9.4f, 1.1f );
		glTexCoord2f(0.0f, 1.0f); glVertex3f( 18.6f, 9.4f, 1.55f );
		glEnd();
	//Escalon8
		//Cara delantera
		glBegin(GL_QUADS);
		glNormal3f( 0.0f,0.0f,1.0f );
		glTexCoord2f(0.0f, 0.0f); glVertex3f( 16.4f, 13.0f, 1.55f );
		glTexCoord2f(1.0f, 0.0f); glVertex3f( 18.6f, 13.0f, 1.55f );
		glTexCoord2f(1.0f, 1.0f); glVertex3f( 18.6f, 13.4f, 1.55f );
		glTexCoord2f(0.0f, 1.0f); glVertex3f( 16.4f, 13.4f, 1.55f );
		glEnd();
		//Cara derecha
		glBegin(GL_QUADS);
		glNormal3f( 1.0f,0.0f,0.0f );
		glTexCoord2f(1.0f, 0.0f); glVertex3f( 18.6f, 13.0f, 1.55f );
		glTexCoord2f(1.0f, 1.0f); glVertex3f( 18.6f, 13.0f, 1.1f );
		glTexCoord2f(0.0f, 1.0f); glVertex3f( 18.6f, 13.4f, 1.1f );
		glTexCoord2f(0.0f, 0.0f); glVertex3f( 18.6f, 13.4f, 1.55f );
		glEnd();
		//Cara izquierda
		glBegin(GL_QUADS);
		glNormal3f( -1.0f,0.0f,0.0f );
		glTexCoord2f(1.0f, 0.0f); glVertex3f( 16.4f, 13.0f, 1.55f );
		glTexCoord2f(1.0f, 1.0f); glVertex3f( 16.4f, 13.4f, 1.55f );
		glTexCoord2f(0.0f, 1.0f); glVertex3f( 16.4f, 13.4f, 1.1f );
		glTexCoord2f(0.0f, 0.0f); glVertex3f( 16.4f, 13.0f, 1.1f );
		glEnd();
		//Cara trasera
		glBegin(GL_QUADS);
		glNormal3f( 0.0f,0.0f,-1.0f );
		glTexCoord2f(1.0f, 0.0f); glVertex3f( 16.4f, 13.0f, 1.1f );
		glTexCoord2f(1.0f, 1.0f); glVertex3f( 16.4f, 13.4f, 1.1f );
		glTexCoord2f(0.0f, 1.0f); glVertex3f( 18.6f, 13.4f, 1.1f );
		glTexCoord2f(0.0f, 0.0f); glVertex3f( 18.6f, 13.0f, 1.1f );
		glEnd();
		//Cara superior
		glBegin(GL_QUADS);
		glNormal3f( 0.0f,1.0f,0.0f );
		glTexCoord2f(0.0f, 0.0f); glVertex3f( 16.4f, 13.4f, 1.55f );
		glTexCoord2f(1.0f, 0.0f); glVertex3f( 18.6f, 13.4f, 1.55f );
		glTexCoord2f(1.0f, 1.0f); glVertex3f( 18.6f, 13.4f, 1.1f );
		glTexCoord2f(0.0f, 1.0f); glVertex3f( 16.4f, 13.4f, 1.1f );
		glEnd();
		//Cara inferior
		glBegin(GL_QUADS);
		glNormal3f( 0.0f, -1.0f, 0.0f );
		glTexCoord2f(0.0f, 0.0f); glVertex3f( 16.4f, 13.0f, 1.55f );
		glTexCoord2f(1.0f, 0.0f); glVertex3f( 16.4f, 13.0f, 1.1f );
		glTexCoord2f(1.0f, 1.0f); glVertex3f( 18.6f, 13.0f, 1.1f );
		glTexCoord2f(0.0f, 1.0f); glVertex3f( 18.6f, 13.0f, 1.55f );
		glEnd();
	//Escalon9
		//Cara delantera
		glBegin(GL_QUADS);
		glNormal3f( 0.0f,0.0f,1.0f );
		glTexCoord2f(0.0f, 0.0f); glVertex3f( 16.4f, 14.4f, 1.55f );
		glTexCoord2f(1.0f, 0.0f); glVertex3f( 18.6f, 14.4f, 1.55f );
		glTexCoord2f(1.0f, 1.0f); glVertex3f( 18.6f, 14.8f, 1.55f );
		glTexCoord2f(0.0f, 1.0f); glVertex3f( 16.4f, 14.8f, 1.55f );
		glEnd();
		//Cara derecha
		glBegin(GL_QUADS);
		glNormal3f( 1.0f,0.0f,0.0f );
		glTexCoord2f(0.0f, 0.0f); glVertex3f( 18.6f, 14.4f, 1.55f );
		glTexCoord2f(1.0f, 0.0f); glVertex3f( 18.6f, 14.4f, 1.1f );
		glTexCoord2f(1.0f, 1.0f); glVertex3f( 18.6f, 14.8f, 1.1f );
		glTexCoord2f(0.0f, 1.0f); glVertex3f( 18.6f, 14.8f, 1.55f );
		glEnd();
		//Cara izquierda
		glBegin(GL_QUADS);
		glNormal3f( -1.0f,0.0f,0.0f );
		glTexCoord2f(1.0f, 0.0f); glVertex3f( 16.4f, 14.4f, 1.55f );
		glTexCoord2f(1.0f, 1.0f); glVertex3f( 16.4f, 14.8f, 1.55f );
		glTexCoord2f(0.0f, 1.0f); glVertex3f( 16.4f, 14.8f, 1.1f );
		glTexCoord2f(0.0f, 0.0f); glVertex3f( 16.4f, 14.4f, 1.1f );
		glEnd();
		//Cara trasera
		glBegin(GL_QUADS);
		glNormal3f( 0.0f,0.0f,-1.0f );
		glTexCoord2f(1.0f, 0.0f); glVertex3f( 16.4f, 14.4f, 1.1f );
		glTexCoord2f(1.0f, 1.0f); glVertex3f( 16.4f, 14.8f, 1.1f );
		glTexCoord2f(0.0f, 1.0f); glVertex3f( 18.6f, 14.8f, 1.1f );
		glTexCoord2f(0.0f, 0.0f); glVertex3f( 18.6f, 14.4f, 1.1f );
		glEnd();
		//Cara superior
		glBegin(GL_QUADS);
		glNormal3f( 0.0f,1.0f,0.0f );
		glTexCoord2f(0.0f, 0.0f); glVertex3f( 16.4f, 14.8f, 1.55f );
		glTexCoord2f(1.0f, 0.0f); glVertex3f( 18.6f, 14.8f, 1.55f );
		glTexCoord2f(1.0f, 1.0f); glVertex3f( 18.6f, 14.8f, 1.1f );
		glTexCoord2f(0.0f, 1.0f); glVertex3f( 16.4f, 14.8f, 1.1f );
		glEnd();
		//Cara inferior
		glBegin(GL_QUADS);
		glNormal3f( 0.0f, -1.0f, 0.0f );
		glTexCoord2f(0.0f, 0.0f); glVertex3f( 16.4f, 14.4f, 1.55f );
		glTexCoord2f(1.0f, 0.0f); glVertex3f( 16.4f, 14.4f, 1.1f );
		glTexCoord2f(1.0f, 1.0f); glVertex3f( 18.6f, 14.4f, 1.1f );
		glTexCoord2f(0.0f, 1.0f); glVertex3f( 18.6f, 14.4f, 1.55f );
		glEnd();
	//Escalon10
		//Cara delantera
		glBegin(GL_QUADS);
		glNormal3f( 0.0f,0.0f,1.0f );
		glTexCoord2f(0.0f, 0.0f); glVertex3f( 16.4f, 15.8f, 1.55f );
		glTexCoord2f(1.0f, 0.0f); glVertex3f( 18.6f, 15.8f, 1.55f );
		glTexCoord2f(1.0f, 1.0f); glVertex3f( 18.6f, 16.2f, 1.55f );
		glTexCoord2f(0.0f, 1.0f); glVertex3f( 16.4f, 16.2f, 1.55f );
		glEnd();
		//Cara derecha
		glBegin(GL_QUADS);
		glNormal3f( 1.0f,0.0f,0.0f );
		glTexCoord2f(0.0f, 0.0f); glVertex3f( 18.6f, 15.8f, 1.55f );
		glTexCoord2f(1.0f, 0.0f); glVertex3f( 18.6f, 15.8f, 1.1f );
		glTexCoord2f(1.0f, 1.0f); glVertex3f( 18.6f, 16.2f, 1.1f );
		glTexCoord2f(0.0f, 1.0f); glVertex3f( 18.6f, 16.2f, 1.55f );
		glEnd();
		//Cara izquierda
		glBegin(GL_QUADS);
		glNormal3f( -1.0f,0.0f,0.0f );
		glTexCoord2f(1.0f, 0.0f); glVertex3f( 16.4f, 15.8f, 1.55f );
		glTexCoord2f(1.0f, 1.0f); glVertex3f( 16.4f, 16.2f, 1.55f );
		glTexCoord2f(0.0f, 1.0f); glVertex3f( 16.4f, 16.2f, 1.1f );
		glTexCoord2f(0.0f, 0.0f); glVertex3f( 16.4f, 15.8f, 1.1f );
		glEnd();
		//Cara trasera
		glBegin(GL_QUADS);
		glNormal3f( 0.0f,0.0f,-1.0f );
		glTexCoord2f(1.0f, 0.0f); glVertex3f( 16.4f, 15.8f, 1.1f );
		glTexCoord2f(1.0f, 1.0f); glVertex3f( 16.4f, 16.2f, 1.1f );
		glTexCoord2f(0.0f, 1.0f); glVertex3f( 18.6f, 16.2f, 1.1f );
		glTexCoord2f(0.0f, 0.0f); glVertex3f( 18.6f, 15.8f, 1.1f );
		glEnd();
		//Cara superior
		glBegin(GL_QUADS);
		glNormal3f( 0.0f,1.0f,0.0f );
		glTexCoord2f(0.0f, 0.0f); glVertex3f( 16.4f, 16.2f, 1.55f );
		glTexCoord2f(1.0f, 0.0f); glVertex3f( 18.6f, 16.2f, 1.55f );
		glTexCoord2f(1.0f, 1.0f); glVertex3f( 18.6f, 16.2f, 1.1f );
		glTexCoord2f(0.0f, 1.0f); glVertex3f( 16.4f, 16.2f, 1.1f );
		glEnd();
		//Cara inferior
		glBegin(GL_QUADS);
		glNormal3f( 0.0f, -1.0f, 0.0f );
		glTexCoord2f(0.0f, 0.0f); glVertex3f( 16.4f, 15.8f, 1.55f );
		glTexCoord2f(1.0f, 0.0f); glVertex3f( 16.4f, 15.8f, 1.1f );
		glTexCoord2f(1.0f, 1.0f); glVertex3f( 18.6f, 15.8f, 1.1f );
		glTexCoord2f(0.0f, 1.0f); glVertex3f( 18.6f, 15.8f, 1.55f );
		glEnd();
	//Escalon11
		//Cara delantera
		glBegin(GL_QUADS);
		glNormal3f( 0.0f,0.0f,1.0f );
		glTexCoord2f(0.0f, 0.0f); glVertex3f( 16.4f, 17.2f, 1.55f );
		glTexCoord2f(1.0f, 0.0f); glVertex3f( 18.6f, 17.2f, 1.55f );
		glTexCoord2f(1.0f, 1.0f); glVertex3f( 18.6f, 17.6f, 1.55f );
		glTexCoord2f(0.0f, 1.0f); glVertex3f( 16.4f, 17.6f, 1.55f );
		glEnd();
		//Cara derecha
		glBegin(GL_QUADS);
		glNormal3f( 1.0f,0.0f,0.0f );
		glTexCoord2f(0.0f, 0.0f); glVertex3f( 18.6f, 17.2f, 1.55f );
		glTexCoord2f(1.0f, 0.0f); glVertex3f( 18.6f, 17.2f, 1.1f );
		glTexCoord2f(1.0f, 1.0f); glVertex3f( 18.6f, 17.6f, 1.1f );
		glTexCoord2f(0.0f, 1.0f); glVertex3f( 18.6f, 17.6f, 1.55f );
		glEnd();
		//Cara izquierda
		glBegin(GL_QUADS);
		glNormal3f( -1.0f,0.0f,0.0f );
		glTexCoord2f(1.0f, 0.0f); glVertex3f( 16.4f, 17.2f, 1.55f );
		glTexCoord2f(1.0f, 1.0f); glVertex3f( 16.4f, 17.6f, 1.55f );
		glTexCoord2f(0.0f, 1.0f); glVertex3f( 16.4f, 17.6f, 1.1f );
		glTexCoord2f(0.0f, 0.0f); glVertex3f( 16.4f, 17.2f, 1.1f );
		glEnd();
		//Cara trasera
		glBegin(GL_QUADS);
		glNormal3f( 0.0f,0.0f,-1.0f );
		glTexCoord2f(1.0f, 0.0f); glVertex3f( 16.4f, 17.2f, 1.1f );
		glTexCoord2f(1.0f, 1.0f); glVertex3f( 16.4f, 17.6f, 1.1f );
		glTexCoord2f(0.0f, 1.0f); glVertex3f( 18.6f, 17.6f, 1.1f );
		glTexCoord2f(0.0f, 0.0f); glVertex3f( 18.6f, 17.2f, 1.1f );
		glEnd();
		//Cara superior
		glBegin(GL_QUADS);
		glNormal3f( 0.0f,1.0f,0.0f );
		glTexCoord2f(0.0f, 0.0f); glVertex3f( 16.4f, 17.6f, 1.55f );
		glTexCoord2f(1.0f, 0.0f); glVertex3f( 18.6f, 17.6f, 1.55f );
		glTexCoord2f(1.0f, 1.0f); glVertex3f( 18.6f, 17.6f, 1.1f );
		glTexCoord2f(0.0f, 1.0f); glVertex3f( 16.4f, 17.6f, 1.1f );
		glEnd();
		//Cara inferior
		glBegin(GL_QUADS);
		glNormal3f( 0.0f, -1.0f, 0.0f );
		glTexCoord2f(0.0f, 0.0f); glVertex3f( 16.4f, 17.2f, 1.55f );
		glTexCoord2f(1.0f, 0.0f); glVertex3f( 16.4f, 17.2f, 1.1f );
		glTexCoord2f(1.0f, 1.0f); glVertex3f( 18.6f, 17.2f, 1.1f );
		glTexCoord2f(0.0f, 1.0f); glVertex3f( 18.6f, 17.2f, 1.55f );
		glEnd();
	//Escalon12
		//Cara delantera
		glBegin(GL_QUADS);
		glNormal3f( 0.0f,0.0f,1.0f );
		glTexCoord2f(1.0f, 0.0f); glVertex3f( 16.4f, 18.6f, 1.55f );
		glTexCoord2f(1.0f, 1.0f); glVertex3f( 18.6f, 18.6f, 1.55f );
		glTexCoord2f(0.0f, 1.0f); glVertex3f( 18.6f, 19.0f, 1.55f );
		glTexCoord2f(0.0f, 0.0f); glVertex3f( 16.4f, 19.0f, 1.55f );
		glEnd();
		//Cara derecha
		glBegin(GL_QUADS);
		glNormal3f( 1.0f,0.0f,0.0f );
		glTexCoord2f(0.0f, 0.0f); glVertex3f( 18.6f, 18.6f, 1.55f );
		glTexCoord2f(1.0f, 0.0f); glVertex3f( 18.6f, 18.6f, 1.1f );
		glTexCoord2f(1.0f, 1.0f); glVertex3f( 18.6f, 19.0f, 1.1f );
		glTexCoord2f(0.0f, 1.0f); glVertex3f( 18.6f, 19.0f, 1.55f );
		glEnd();
		//Cara izquierda
		glBegin(GL_QUADS);
		glNormal3f( -1.0f,0.0f,0.0f );
		glTexCoord2f(1.0f, 0.0f); glVertex3f( 16.4f, 18.6f, 1.55f );
		glTexCoord2f(1.0f, 1.0f); glVertex3f( 16.4f, 19.0f, 1.55f );
		glTexCoord2f(0.0f, 1.0f); glVertex3f( 16.4f, 19.0f, 1.1f );
		glTexCoord2f(0.0f, 0.0f); glVertex3f( 16.4f, 18.6f, 1.1f );
		glEnd();
		//Cara trasera
		glBegin(GL_QUADS);
		glNormal3f( 0.0f,0.0f,-1.0f );
		glTexCoord2f(1.0f, 0.0f); glVertex3f( 16.4f, 18.6f, 1.1f );
		glTexCoord2f(1.0f, 1.0f); glVertex3f( 16.4f, 19.0f, 1.1f );
		glTexCoord2f(0.0f, 1.0f); glVertex3f( 18.6f, 19.0f, 1.1f );
		glTexCoord2f(0.0f, 0.0f); glVertex3f( 18.6f, 18.6f, 1.1f );
		glEnd();
		//Cara superior
		glBegin(GL_QUADS);
		glNormal3f( 0.0f,1.0f,0.0f );
		glTexCoord2f(0.0f, 0.0f); glVertex3f( 16.4f, 19.0f, 1.55f );
		glTexCoord2f(1.0f, 0.0f); glVertex3f( 18.6f, 19.0f, 1.55f );
		glTexCoord2f(1.0f, 1.0f); glVertex3f( 18.6f, 19.0f, 1.1f );
		glTexCoord2f(0.0f, 1.0f); glVertex3f( 16.4f, 19.0f, 1.1f );
		glEnd();
		//Cara inferior
		glBegin(GL_QUADS);
		glNormal3f( 0.0f, -1.0f, 0.0f );
		glTexCoord2f(0.0f, 0.0f); glVertex3f( 16.4f, 18.6f, 1.55f );
		glTexCoord2f(1.0f, 0.0f); glVertex3f( 16.4f, 18.6f, 1.1f );
		glTexCoord2f(1.0f, 1.0f); glVertex3f( 18.6f, 18.6f, 1.1f );
		glTexCoord2f(0.0f, 1.0f); glVertex3f( 18.6f, 18.6f, 1.55f );
		glEnd();
	//Escalon13
		//Cara delantera
		glBegin(GL_QUADS);
		glNormal3f( 0.0f,0.0f,1.0f );
		glTexCoord2f(0.0f, 0.0f); glVertex3f( 16.4f, 20.0f, 1.55f );
		glTexCoord2f(1.0f, 0.0f); glVertex3f( 18.6f, 20.0f, 1.55f );
		glTexCoord2f(1.0f, 1.0f); glVertex3f( 18.6f, 20.4f, 1.55f );
		glTexCoord2f(0.0f, 1.0f); glVertex3f( 16.4f, 20.4f, 1.55f );
		glEnd();
		//Cara derecha
		glBegin(GL_QUADS);
		glNormal3f( 1.0f,0.0f,0.0f );
		glTexCoord2f(0.0f, 0.0f); glVertex3f( 18.6f, 20.0f, 1.55f );
		glTexCoord2f(1.0f, 0.0f); glVertex3f( 18.6f, 20.0f, 1.1f );
		glTexCoord2f(1.0f, 1.0f); glVertex3f( 18.6f, 20.4f, 1.1f );
		glTexCoord2f(0.0f, 1.0f); glVertex3f( 18.6f, 20.4f, 1.55f );
		glEnd();
		//Cara izquierda
		glBegin(GL_QUADS);
		glNormal3f( -1.0f,0.0f,0.0f );
		glTexCoord2f(1.0f, 0.0f); glVertex3f( 16.4f, 20.0f, 1.55f );
		glTexCoord2f(1.0f, 1.0f); glVertex3f( 16.4f, 20.4f, 1.55f );
		glTexCoord2f(0.0f, 1.0f); glVertex3f( 16.4f, 20.4f, 1.1f );
		glTexCoord2f(0.0f, 0.0f); glVertex3f( 16.4f, 20.0f, 1.1f );
		glEnd();
		//Cara trasera
		glBegin(GL_QUADS);
		glNormal3f( 0.0f,0.0f,-1.0f );
		glTexCoord2f(1.0f, 0.0f); glVertex3f( 16.4f, 20.0f, 1.1f );
		glTexCoord2f(1.0f, 1.0f); glVertex3f( 16.4f, 20.4f, 1.1f );
		glTexCoord2f(0.0f, 1.0f); glVertex3f( 18.6f, 20.4f, 1.1f );
		glTexCoord2f(0.0f, 0.0f); glVertex3f( 18.6f, 20.0f, 1.1f );
		glEnd();
		//Cara superior
		glBegin(GL_QUADS);
		glNormal3f( 0.0f,1.0f,0.0f );
		glTexCoord2f(0.0f, 0.0f); glVertex3f( 16.4f, 20.4f, 1.55f );
		glTexCoord2f(1.0f, 0.0f); glVertex3f( 18.6f, 20.4f, 1.55f );
		glTexCoord2f(1.0f, 1.0f); glVertex3f( 18.6f, 20.4f, 1.1f );
		glTexCoord2f(0.0f, 1.0f); glVertex3f( 16.4f, 20.4f, 1.1f );
		glEnd();
		//Cara inferior
		glBegin(GL_QUADS);
		glNormal3f( 0.0f, -1.0f, 0.0f );
		glTexCoord2f(0.0f, 0.0f); glVertex3f( 16.4f, 20.0f, 1.55f );
		glTexCoord2f(1.0f, 0.0f); glVertex3f( 16.4f, 20.0f, 1.1f );
		glTexCoord2f(1.0f, 1.0f); glVertex3f( 18.6f, 20.0f, 1.1f );
		glTexCoord2f(0.0f, 1.0f); glVertex3f( 18.6f, 20.0f, 1.55f );
		glEnd();
		glPopMatrix();
//EscaleraRoja
	//verticalder
		glBindTexture(GL_TEXTURE_2D, textura[8].texID);
		//Cara delantera
		glBegin(GL_QUADS);
		glNormal3f( 0.0f,0.0f,1.0f );
		glTexCoord2f(0.0f, 0.0f); glVertex3f( -31.5f, 3.0f, 1.55f );
		glTexCoord2f(1.0f, 0.0f); glVertex3f( -31.1f, 3.0f, 1.55f );
		glTexCoord2f(1.0f, 14.0f); glVertex3f( -31.1f, 23.0f, 1.55f );
		glTexCoord2f(0.0f, 14.0f); glVertex3f( -31.5f, 23.0f, 1.55f );
		glEnd();
		//Cara derecha
		glBegin(GL_QUADS);
		glNormal3f( 1.0f,0.0f,0.0f );
		glTexCoord2f(0.0f, 0.0f); glVertex3f( -31.1f, 3.0f, 1.55f );
		glTexCoord2f(1.0f, 0.0f); glVertex3f( -31.1f, 3.0f, 1.1f );
		glTexCoord2f(1.0f, 14.0f); glVertex3f( -31.1f, 23.0f, 1.1f );
		glTexCoord2f(0.0f, 14.0f); glVertex3f( -31.1f, 23.0f, 1.55f );
		glEnd();
		//Cara izquierda
		glBegin(GL_QUADS);
		glNormal3f( -1.0f,0.0f,0.0f );
		glTexCoord2f(1.0f, 0.0f); glVertex3f( -31.5f, 3.0f, 1.55f );
		glTexCoord2f(1.0f, 14.0f); glVertex3f( -31.5f, 23.0f, 1.55f );
		glTexCoord2f(0.0f, 14.0f); glVertex3f( -31.5f, 23.0f, 1.1f );
		glTexCoord2f(0.0f, 0.0f); glVertex3f( -31.5f, 3.0f, 1.1f );
		glEnd();
		//Cara trasera
		glBegin(GL_QUADS);
		glNormal3f( 0.0f,0.0f,-1.0f );
		glTexCoord2f(1.0f, 0.0f); glVertex3f( -31.5f, 3.0f, 1.1f );
		glTexCoord2f(1.0f, 14.0f); glVertex3f( -31.5f, 23.0f, 1.1f );
		glTexCoord2f(0.0f, 14.0f); glVertex3f( -31.1f, 23.0f, 1.1f );
		glTexCoord2f(0.0f, 0.0f); glVertex3f( -31.1f, 3.0f, 1.1f );
		glEnd();
		glBindTexture(GL_TEXTURE_2D, textura[11].texID);
		//Cara superior
		glBegin(GL_QUADS);
		glNormal3f( 0.0f,1.0f,0.0f );
		glTexCoord2f(0.0f, 0.0f); glVertex3f( -31.5f, 23.0f, 1.55f );
		glTexCoord2f(1.0f, 0.0f); glVertex3f( -31.1f, 23.0f, 1.55f );
		glTexCoord2f(1.0f, 1.0f); glVertex3f( -31.1f, 23.0f, 1.1f );
		glTexCoord2f(0.0f, 1.0f); glVertex3f( -31.5f, 23.0f, 1.1f );
		glEnd();
		//Cara inferior
		glBegin(GL_QUADS);
		glNormal3f( 0.0f, -1.0f, 0.0f );
		glTexCoord2f(0.0f, 0.0f); glVertex3f( -31.5f, 3.0f, 1.55f );
		glTexCoord2f(1.0f, 0.0f); glVertex3f( -31.5f, 3.0f, 1.1f );
		glTexCoord2f(1.0f, 1.0f); glVertex3f( -31.1f, 3.0f, 1.1f );
		glTexCoord2f(0.0f, 1.0f); glVertex3f( -31.1f, 3.0f, 1.55f );
		glEnd();
	//verticalizq
		//Cara delantera
		glBindTexture(GL_TEXTURE_2D, textura[8].texID);
		glBegin(GL_QUADS);
		glNormal3f( 0.0f,0.0f,1.0f );
		glTexCoord2f(0.0f, 0.0f); glVertex3f( -34.5f, 3.0f, 1.55f );
		glTexCoord2f(1.0f, 0.0f); glVertex3f( -34.1f, 3.0f, 1.55f );
		glTexCoord2f(1.0f, 14.0f); glVertex3f( -34.1f, 23.0f, 1.55f );
		glTexCoord2f(0.0f, 14.0f); glVertex3f( -34.5f, 23.0f, 1.55f );
		glEnd();
		//Cara derecha
		glBegin(GL_QUADS);
		glNormal3f( 1.0f,0.0f,0.0f );
		glTexCoord2f(0.0f, 0.0f); glVertex3f( -34.1f, 3.0f, 1.55f );
		glTexCoord2f(1.0f, 0.0f); glVertex3f( -34.1f, 3.0f, 1.1f );
		glTexCoord2f(1.0f, 14.0f); glVertex3f( -34.1f, 23.0f, 1.1f );
		glTexCoord2f(0.0f, 14.0f); glVertex3f( -34.1f, 23.0f, 1.55f );
		glEnd();
		//Cara izquierda
		glBegin(GL_QUADS);
		glNormal3f( -1.0f,0.0f,0.0f );
		glTexCoord2f(1.0f, 0.0f); glVertex3f( -34.5f, 3.0f, 1.55f );
		glTexCoord2f(1.0f, 14.0f); glVertex3f( -34.5f, 23.0f, 1.55f );
		glTexCoord2f(0.0f, 14.0f); glVertex3f( -34.5f, 23.0f, 1.1f );
		glTexCoord2f(0.0f, 0.0f); glVertex3f( -34.5f, 3.0f, 1.1f );
		glEnd();
		//Cara trasera
		glBegin(GL_QUADS);
		glNormal3f( 0.0f,0.0f,-1.0f );
		glTexCoord2f(1.0f, 0.0f); glVertex3f( -34.5f, 3.0f, 1.1f );
		glTexCoord2f(1.0f, 14.0f); glVertex3f( -34.5f, 23.0f, 1.1f );
		glTexCoord2f(0.0f, 14.0f); glVertex3f( -34.1f, 23.0f, 1.1f );
		glTexCoord2f(0.0f, 0.0f); glVertex3f( -34.1f, 3.0f, 1.1f );
		glEnd();
		//Cara superior
		glBindTexture(GL_TEXTURE_2D, textura[11].texID);
		glBegin(GL_QUADS);
		glNormal3f( 0.0f,1.0f,0.0f );
		glTexCoord2f(0.0f, 0.0f); glVertex3f( -34.5f, 23.0f, 1.55f );
		glTexCoord2f(1.0f, 0.0f); glVertex3f( -34.1f, 23.0f, 1.55f );
		glTexCoord2f(1.0f, 1.0f); glVertex3f( -34.1f, 23.0f, 1.1f );
		glTexCoord2f(0.0f, 1.0f); glVertex3f( -34.5f, 23.0f, 1.1f );
		glEnd();
		//Cara inferior
		glBegin(GL_QUADS);
		glNormal3f( 0.0f, -1.0f, 0.0f );
		glTexCoord2f(0.0f, 0.0f); glVertex3f( -34.5f, 3.0f, 1.55f );
		glTexCoord2f(1.0f, 0.0f); glVertex3f( -34.5f, 3.0f, 1.1f );
		glTexCoord2f(1.0f, 1.0f); glVertex3f( -34.1f, 3.0f, 1.1f );
		glTexCoord2f(0.0f, 1.0f); glVertex3f( -34.1f, 3.0f, 1.55f );
		glEnd();
	//Escalon1
		//Cara delantera
		glBegin(GL_QUADS);
		glNormal3f( 0.0f,0.0f,1.0f );
		glTexCoord2f(0.0f, 0.0f); glVertex3f( -34.1f, 3.7f, 1.55f );
		glTexCoord2f(1.0f, 0.0f); glVertex3f( -31.5f, 3.7f, 1.55f );
		glTexCoord2f(1.0f, 1.0f); glVertex3f( -31.5f, 4.2f, 1.55f );
		glTexCoord2f(0.0f, 1.0f); glVertex3f( -34.1f, 4.2f, 1.55f );
		glEnd();
		//Cara derecha
		glBegin(GL_QUADS);
		glNormal3f( 1.0f,0.0f,0.0f );
		glTexCoord2f(0.0f, 0.0f); glVertex3f( -31.5f, 3.7f, 1.55f );
		glTexCoord2f(1.0f, 0.0f); glVertex3f( -31.5f, 3.7f, 1.1f );
		glTexCoord2f(1.0f, 1.0f); glVertex3f( -31.5f, 4.2f, 1.1f );
		glTexCoord2f(0.0f, 1.0f); glVertex3f( -31.5f, 4.2f, 1.55f );
		glEnd();
		//Cara izquierda
		glBegin(GL_QUADS);
		glNormal3f( -1.0f,0.0f,0.0f );
		glTexCoord2f(1.0f, 0.0f); glVertex3f( -34.1f, 3.7f, 1.55f );
		glTexCoord2f(1.0f, 1.0f); glVertex3f( -34.1f, 4.2f, 1.55f );
		glTexCoord2f(0.0f, 1.0f); glVertex3f( -34.1f, 4.2f, 1.1f );
		glTexCoord2f(0.0f, 0.0f); glVertex3f( -34.1f, 3.7f, 1.1f );
		glEnd();
		//Cara trasera
		glBegin(GL_QUADS);
		glNormal3f( 0.0f,0.0f,-1.0f );
		glTexCoord2f(1.0f, 0.0f); glVertex3f( -34.1f, 3.7f, 1.1f );
		glTexCoord2f(1.0f, 1.0f); glVertex3f( -34.1f, 4.2f, 1.1f );
		glTexCoord2f(0.0f, 1.0f); glVertex3f( -31.5f, 4.2f, 1.1f );
		glTexCoord2f(0.0f, 0.0f); glVertex3f( -31.5f, 3.7f, 1.1f );
		glEnd();
		//Cara superior
		glBegin(GL_QUADS);
		glNormal3f( 0.0f,1.0f,0.0f );
		glTexCoord2f(0.0f, 0.0f); glVertex3f( -34.1f, 4.2f, 1.55f );
		glTexCoord2f(1.0f, 0.0f); glVertex3f( -31.5f, 4.2f, 1.55f );
		glTexCoord2f(1.0f, 1.0f); glVertex3f( -31.5f, 4.2f, 1.1f );
		glTexCoord2f(0.0f, 1.0f); glVertex3f( -34.1f, 4.2f, 1.1f );
		glEnd();
		//Cara inferior
		glBegin(GL_QUADS);
		glNormal3f( 0.0f, -1.0f, 0.0f );
		glTexCoord2f(0.0f, 0.0f); glVertex3f( -34.1f, 3.7f, 1.55f );
		glTexCoord2f(1.0f, 0.0f); glVertex3f( -34.1f, 3.7f, 1.1f );
		glTexCoord2f(1.0f, 1.0f); glVertex3f( -31.5f, 3.7f, 1.1f );
		glTexCoord2f(0.0f, 1.0f); glVertex3f( -31.5f, 3.7f, 1.55f );
		glEnd();
	//Escalon2
		//Cara delantera
		glBegin(GL_QUADS);
		glNormal3f( 0.0f,0.0f,1.0f );
		glTexCoord2f(0.0f, 0.0f); glVertex3f( -34.1f, 5.2f, 1.55f );
		glTexCoord2f(1.0f, 0.0f); glVertex3f( -31.5f, 5.2f, 1.55f );
		glTexCoord2f(1.0f, 1.0f); glVertex3f( -31.5f, 5.6f, 1.55f );
		glTexCoord2f(0.0f, 1.0f); glVertex3f( -34.1f, 5.6f, 1.55f );
		glEnd();
		//Cara derecha
		glBegin(GL_QUADS);
		glNormal3f( 1.0f,0.0f,0.0f );
		glTexCoord2f(0.0f, 0.0f); glVertex3f( -31.5f, 5.2f, 1.55f );
		glTexCoord2f(1.0f, 0.0f); glVertex3f( -31.5f, 5.2f, 1.1f );
		glTexCoord2f(1.0f, 1.0f); glVertex3f( -31.5f, 5.6f, 1.1f );
		glTexCoord2f(0.0f, 1.0f); glVertex3f( -31.5f, 5.6f, 1.55f );
		glEnd();
		//Cara izquierda
		glBegin(GL_QUADS);
		glNormal3f( -1.0f,0.0f,0.0f );
		glTexCoord2f(1.0f, 0.0f); glVertex3f( -34.1f, 5.2f, 1.55f );
		glTexCoord2f(1.0f, 1.0f); glVertex3f( -34.1f, 5.6f, 1.55f );
		glTexCoord2f(0.0f, 1.0f); glVertex3f( -34.1f, 5.6f, 1.1f );
		glTexCoord2f(0.0f, 0.0f); glVertex3f( -34.1f, 5.2f, 1.1f );
		glEnd();
		//Cara trasera
		glBegin(GL_QUADS);
		glNormal3f( 0.0f,0.0f,-1.0f );
		glTexCoord2f(1.0f, 0.0f); glVertex3f( -34.1f, 5.2f, 1.1f );
		glTexCoord2f(1.0f, 1.0f); glVertex3f( -34.1f, 5.6f, 1.1f );
		glTexCoord2f(0.0f, 1.0f); glVertex3f( -31.5f, 5.6f, 1.1f );
		glTexCoord2f(0.0f, 0.0f); glVertex3f( -31.5f, 5.2f, 1.1f );
		glEnd();
		//Cara superior
		glBegin(GL_QUADS);
		glNormal3f( 0.0f,1.0f,0.0f );
		glTexCoord2f(0.0f, 0.0f); glVertex3f( -34.1f, 5.6f, 1.55f );
		glTexCoord2f(1.0f, 0.0f); glVertex3f( -31.5f, 5.6f, 1.55f );
		glTexCoord2f(1.0f, 1.0f); glVertex3f( -31.5f, 5.6f, 1.1f );
		glTexCoord2f(0.0f, 1.0f); glVertex3f( -34.1f, 5.6f, 1.1f );
		glEnd();
		//Cara inferior
		glBegin(GL_QUADS);
		glNormal3f( 0.0f, -1.0f, 0.0f );
		glTexCoord2f(0.0f, 0.0f); glVertex3f( -34.1f, 5.2f, 1.55f );
		glTexCoord2f(1.0f, 0.0f); glVertex3f( -34.1f, 5.2f, 1.1f );
		glTexCoord2f(1.0f, 1.0f); glVertex3f( -31.5f, 5.2f, 1.1f );
		glTexCoord2f(0.0f, 1.0f); glVertex3f( -31.5f, 5.2f, 1.55f );
		glEnd();
	//Escalon3
		//Cara delantera
		glBegin(GL_QUADS);
		glNormal3f( 0.0f,0.0f,1.0f );
		glTexCoord2f(0.0f, 0.0f); glVertex3f( -34.1f, 6.6f, 1.55f );
		glTexCoord2f(1.0f, 0.0f); glVertex3f( -31.5f, 6.6f, 1.55f );
		glTexCoord2f(1.0f, 1.0f); glVertex3f( -31.5f, 7.0f, 1.55f );
		glTexCoord2f(0.0f, 1.0f); glVertex3f( -34.1f, 7.0f, 1.55f );
		glEnd();
		//Cara derecha
		glBegin(GL_QUADS);
		glNormal3f( 1.0f,0.0f,0.0f );
		glTexCoord2f(0.0f, 0.0f); glVertex3f( -31.5f, 6.6f, 1.55f );
		glTexCoord2f(1.0f, 0.0f); glVertex3f( -31.5f, 6.6f, 1.1f );
		glTexCoord2f(1.0f, 1.0f); glVertex3f( -31.5f, 7.0f, 1.1f );
		glTexCoord2f(0.0f, 1.0f); glVertex3f( -31.5f, 7.0f, 1.55f );
		glEnd();
		//Cara izquierda
		glBegin(GL_QUADS);
		glNormal3f( -1.0f,0.0f,0.0f );
		glTexCoord2f(1.0f, 0.0f); glVertex3f( -34.1f, 6.6f, 1.55f );
		glTexCoord2f(1.0f, 1.0f); glVertex3f( -34.1f, 7.0f, 1.55f );
		glTexCoord2f(0.0f, 1.0f); glVertex3f( -34.1f, 7.0f, 1.1f );
		glTexCoord2f(0.0f, 0.0f); glVertex3f( -34.1f, 6.6f, 1.1f );
		glEnd();
		//Cara trasera
		glBegin(GL_QUADS);
		glNormal3f( 0.0f,0.0f,-1.0f );
		glTexCoord2f(1.0f, 0.0f); glVertex3f( -34.1f, 6.6f, 1.1f );
		glTexCoord2f(1.0f, 1.0f); glVertex3f( -34.1f, 7.0f, 1.1f );
		glTexCoord2f(0.0f, 1.0f); glVertex3f( -31.5f, 7.0f, 1.1f );
		glTexCoord2f(0.0f, 0.0f); glVertex3f( -31.5f, 6.6f, 1.1f );
		glEnd();
		//Cara superior
		glBegin(GL_QUADS);
		glNormal3f( 0.0f,1.0f,0.0f );
		glTexCoord2f(0.0f, 0.0f); glVertex3f( -34.1f, 7.0f, 1.55f );
		glTexCoord2f(1.0f, 0.0f); glVertex3f( -31.5f, 7.0f, 1.55f );
		glTexCoord2f(1.0f, 1.0f); glVertex3f( -31.5f, 7.0f, 1.1f );
		glTexCoord2f(0.0f, 1.0f); glVertex3f( -34.1f, 7.0f, 1.1f );
		glEnd();
		//Cara inferior
		glBegin(GL_QUADS);
		glNormal3f( 0.0f, -1.0f, 0.0f );
		glTexCoord2f(0.0f, 0.0f); glVertex3f( -34.1f, 6.6f, 1.55f );
		glTexCoord2f(1.0f, 0.0f); glVertex3f( -34.1f, 6.6f, 1.1f );
		glTexCoord2f(1.0f, 1.0f); glVertex3f( -31.5f, 6.6f, 1.1f );
		glTexCoord2f(0.0f, 1.0f); glVertex3f( -31.5f, 6.6f, 1.55f );
		glEnd();
	//Escalon4
		//Cara delantera
		glBegin(GL_QUADS);
		glNormal3f( 0.0f,0.0f,1.0f );
		glTexCoord2f(0.0f, 0.0f); glVertex3f( -34.1f, 8.0f, 1.55f );
		glTexCoord2f(1.0f, 0.0f); glVertex3f( -31.5f, 8.0f, 1.55f );
		glTexCoord2f(1.0f, 1.0f); glVertex3f( -31.5f, 8.4f, 1.55f );
		glTexCoord2f(0.0f, 1.0f); glVertex3f( -34.1f, 8.4f, 1.55f );
		glEnd();
		//Cara derecha
		glBegin(GL_QUADS);
		glNormal3f( 1.0f,0.0f,0.0f );
		glTexCoord2f(0.0f, 0.0f); glVertex3f( -31.5f, 8.0f, 1.55f );
		glTexCoord2f(1.0f, 0.0f); glVertex3f( -31.5f, 8.0f, 1.1f );
		glTexCoord2f(1.0f, 1.0f); glVertex3f( -31.5f, 8.4f, 1.1f );
		glTexCoord2f(0.0f, 1.0f); glVertex3f( -31.5f, 8.4f, 1.55f );
		glEnd();
		//Cara izquierda
		glBegin(GL_QUADS);
		glNormal3f( -1.0f,0.0f,0.0f );
		glTexCoord2f(1.0f, 0.0f); glVertex3f( -34.1f, 8.0f, 1.55f );
		glTexCoord2f(1.0f, 1.0f); glVertex3f( -34.1f, 8.4f, 1.55f );
		glTexCoord2f(0.0f, 1.0f); glVertex3f( -34.1f, 8.4f, 1.1f );
		glTexCoord2f(0.0f, 0.0f); glVertex3f( -34.1f, 8.0f, 1.1f );
		glEnd();
		//Cara trasera
		glBegin(GL_QUADS);
		glNormal3f( 0.0f,0.0f,-1.0f );
		glTexCoord2f(1.0f, 0.0f); glVertex3f( -34.1f, 8.0f, 1.1f );
		glTexCoord2f(1.0f, 1.0f); glVertex3f( -34.1f, 8.4f, 1.1f );
		glTexCoord2f(0.0f, 1.0f); glVertex3f( -31.5f, 8.4f, 1.1f );
		glTexCoord2f(0.0f, 0.0f); glVertex3f( -31.5f, 8.0f, 1.1f );
		glEnd();
		//Cara superior
		glBegin(GL_QUADS);
		glNormal3f( 0.0f,1.0f,0.0f );
		glTexCoord2f(0.0f, 0.0f); glVertex3f( -34.1f, 8.4f, 1.55f );
		glTexCoord2f(1.0f, 0.0f); glVertex3f( -31.5f, 8.4f, 1.55f );
		glTexCoord2f(1.0f, 1.0f); glVertex3f( -31.5f, 8.4f, 1.1f );
		glTexCoord2f(0.0f, 1.0f); glVertex3f( -34.1f, 8.4f, 1.1f );
		glEnd();
		//Cara inferior
		glBegin(GL_QUADS);
		glNormal3f( 0.0f, -1.0f, 0.0f );
		glTexCoord2f(0.0f, 0.0f); glVertex3f( -34.1f, 8.0f, 1.55f );
		glTexCoord2f(1.0f, 0.0f); glVertex3f( -34.1f, 8.0f, 1.1f );
		glTexCoord2f(1.0f, 1.0f); glVertex3f( -31.5f, 8.0f, 1.1f );
		glTexCoord2f(0.0f, 1.0f); glVertex3f( -31.5f, 8.0f, 1.55f );
		glEnd();
	//Escalon5
		//Cara delantera
		glBegin(GL_QUADS);
		glNormal3f( 0.0f,0.0f,1.0f );
		glTexCoord2f(0.0f, 0.0f); glVertex3f( -34.1f, 9.4f, 1.55f );
		glTexCoord2f(1.0f, 0.0f); glVertex3f( -31.5f, 9.4f, 1.55f );
		glTexCoord2f(1.0f, 1.0f); glVertex3f( -31.5f, 9.8f, 1.55f );
		glTexCoord2f(0.0f, 1.0f); glVertex3f( -34.1f, 9.8f, 1.55f );
		glEnd();
		//Cara derecha
		glBegin(GL_QUADS);
		glNormal3f( 1.0f,0.0f,0.0f );
		glTexCoord2f(0.0f, 0.0f); glVertex3f( -31.5f, 9.4f, 1.55f );
		glTexCoord2f(1.0f, 0.0f); glVertex3f( -31.5f, 9.4f, 1.1f );
		glTexCoord2f(1.0f, 1.0f); glVertex3f( -31.5f, 9.8f, 1.1f );
		glTexCoord2f(0.0f, 1.0f); glVertex3f( -31.5f, 9.8f, 1.55f );
		glEnd();
		//Cara izquierda
		glBegin(GL_QUADS);
		glNormal3f( -1.0f,0.0f,0.0f );
		glTexCoord2f(1.0f, 0.0f); glVertex3f( -34.1f, 9.4f, 1.55f );
		glTexCoord2f(1.0f, 1.0f); glVertex3f( -34.1f, 9.8f, 1.55f );
		glTexCoord2f(0.0f, 1.0f); glVertex3f( -34.1f, 9.8f, 1.1f );
		glTexCoord2f(0.0f, 0.0f); glVertex3f( -34.1f, 9.4f, 1.1f );
		glEnd();
		//Cara trasera
		glBegin(GL_QUADS);
		glNormal3f( 0.0f,0.0f,-1.0f );
		glTexCoord2f(1.0f, 0.0f); glVertex3f( -34.1f, 9.4f, 1.1f );
		glTexCoord2f(1.0f, 1.0f); glVertex3f( -34.1f, 9.8f, 1.1f );
		glTexCoord2f(0.0f, 1.0f); glVertex3f( -31.5f, 9.8f, 1.1f );
		glTexCoord2f(0.0f, 0.0f); glVertex3f( -31.5f, 9.4f, 1.1f );
		glEnd();
		//Cara superior
		glBegin(GL_QUADS);
		glNormal3f( 0.0f,1.0f,0.0f );
		glTexCoord2f(0.0f, 0.0f); glVertex3f( -34.1f, 9.8f, 1.55f );
		glTexCoord2f(1.0f, 0.0f); glVertex3f( -31.5f, 9.8f, 1.55f );
		glTexCoord2f(1.0f, 1.0f); glVertex3f( -31.5f, 9.8f, 1.1f );
		glTexCoord2f(0.0f, 1.0f); glVertex3f( -34.1f, 9.8f, 1.1f );
		glEnd();
		//Cara inferior
		glBegin(GL_QUADS);
		glNormal3f( 0.0f, -1.0f, 0.0f );
		glTexCoord2f(0.0f, 0.0f); glVertex3f( -34.1f, 9.4f, 1.55f );
		glTexCoord2f(1.0f, 0.0f); glVertex3f( -34.1f, 9.4f, 1.1f );
		glTexCoord2f(1.0f, 1.0f); glVertex3f( -31.5f, 9.4f, 1.1f );
		glTexCoord2f(0.0f, 1.0f); glVertex3f( -31.5f, 9.4f, 1.55f );
		glEnd();
	//Escalon6
		//Cara delantera
		glBegin(GL_QUADS);
		glNormal3f( 0.0f,0.0f,1.0f );
		glTexCoord2f(0.0f, 0.0f); glVertex3f( -34.1f, 13.0f, 1.55f );
		glTexCoord2f(1.0f, 0.0f); glVertex3f( -31.5f, 13.0f, 1.55f );
		glTexCoord2f(1.0f, 1.0f); glVertex3f( -31.5f, 13.4f, 1.55f );
		glTexCoord2f(0.0f, 1.0f); glVertex3f( -34.1f, 13.4f, 1.55f );
		glEnd();
		//Cara derecha
		glBegin(GL_QUADS);
		glNormal3f( 1.0f,0.0f,0.0f );
		glTexCoord2f(0.0f, 0.0f); glVertex3f( -31.5f, 13.0f, 1.55f );
		glTexCoord2f(1.0f, 0.0f); glVertex3f( -31.5f, 13.0f, 1.1f );
		glTexCoord2f(1.0f, 1.0f); glVertex3f( -31.5f, 13.4f, 1.1f );
		glTexCoord2f(0.0f, 1.0f); glVertex3f( -31.5f, 13.4f, 1.55f );
		glEnd();
		//Cara izquierda
		glBegin(GL_QUADS);
		glNormal3f( -1.0f,0.0f,0.0f );
		glTexCoord2f(1.0f, 0.0f); glVertex3f( -34.1f, 13.0f, 1.55f );
		glTexCoord2f(1.0f, 1.0f); glVertex3f( -34.1f, 13.4f, 1.55f );
		glTexCoord2f(0.0f, 1.0f); glVertex3f( -34.1f, 13.4f, 1.1f );
		glTexCoord2f(0.0f, 0.0f); glVertex3f( -34.1f, 13.0f, 1.1f );
		glEnd();
		//Cara trasera
		glBegin(GL_QUADS);
		glNormal3f( 0.0f,0.0f,-1.0f );
		glTexCoord2f(1.0f, 0.0f); glVertex3f( -34.1f, 13.0f, 1.1f );
		glTexCoord2f(1.0f, 1.0f); glVertex3f( -34.1f, 13.4f, 1.1f );
		glTexCoord2f(0.0f, 1.0f); glVertex3f( -31.5f, 13.4f, 1.1f );
		glTexCoord2f(0.0f, 0.0f); glVertex3f( -31.5f, 13.0f, 1.1f );
		glEnd();
		//Cara superior
		glBegin(GL_QUADS);
		glNormal3f( 0.0f,1.0f,0.0f );
		glTexCoord2f(0.0f, 0.0f); glVertex3f( -34.1f, 13.4f, 1.55f );
		glTexCoord2f(1.0f, 0.0f); glVertex3f( -31.5f, 13.4f, 1.55f );
		glTexCoord2f(1.0f, 1.0f); glVertex3f( -31.5f, 13.4f, 1.1f );
		glTexCoord2f(0.0f, 1.0f); glVertex3f( -34.1f, 13.4f, 1.1f );
		glEnd();
		//Cara inferior
		glBegin(GL_QUADS);
		glNormal3f( 0.0f, -1.0f, 0.0f );
		glTexCoord2f(0.0f, 0.0f); glVertex3f( -34.1f, 13.0f, 1.55f );
		glTexCoord2f(1.0f, 0.0f); glVertex3f( -34.1f, 13.0f, 1.1f );
		glTexCoord2f(1.0f, 1.0f); glVertex3f( -31.5f, 13.0f, 1.1f );
		glTexCoord2f(0.0f, 1.0f); glVertex3f( -31.5f, 13.0f, 1.55f );
		glEnd();
	//Escalon7
		//Cara delantera
		glBegin(GL_QUADS);
		glNormal3f( 0.0f,0.0f,1.0f );
		glTexCoord2f(0.0f, 0.0f); glVertex3f( -34.1f, 14.4f, 1.55f );
		glTexCoord2f(1.0f, 0.0f); glVertex3f( -31.5f, 14.4f, 1.55f );
		glTexCoord2f(1.0f, 1.0f); glVertex3f( -31.5f, 14.8f, 1.55f );
		glTexCoord2f(0.0f, 1.0f); glVertex3f( -34.1f, 14.8f, 1.55f );
		glEnd();
		//Cara derecha
		glBegin(GL_QUADS);
		glNormal3f( 1.0f,0.0f,0.0f );
		glTexCoord2f(0.0f, 0.0f); glVertex3f( -31.5f, 14.4f, 1.55f );
		glTexCoord2f(1.0f, 0.0f); glVertex3f( -31.5f, 14.4f, 1.1f );
		glTexCoord2f(1.0f, 1.0f); glVertex3f( -31.5f, 14.8f, 1.1f );
		glTexCoord2f(0.0f, 1.0f); glVertex3f( -31.5f, 14.8f, 1.55f );
		glEnd();
		//Cara izquierda
		glBegin(GL_QUADS);
		glNormal3f( -1.0f,0.0f,0.0f );
		glTexCoord2f(1.0f, 0.0f); glVertex3f( -34.1f, 14.4f, 1.55f );
		glTexCoord2f(1.0f, 1.0f); glVertex3f( -34.1f, 14.8f, 1.55f );
		glTexCoord2f(0.0f, 1.0f); glVertex3f( -34.1f, 14.8f, 1.1f );
		glTexCoord2f(0.0f, 0.0f); glVertex3f( -34.1f, 14.4f, 1.1f );
		glEnd();
		//Cara trasera
		glBegin(GL_QUADS);
		glNormal3f( 0.0f,0.0f,-1.0f );
		glTexCoord2f(1.0f, 0.0f); glVertex3f( -34.1f, 14.4f, 1.1f );
		glTexCoord2f(1.0f, 1.0f); glVertex3f( -34.1f, 14.8f, 1.1f );
		glTexCoord2f(0.0f, 1.0f); glVertex3f( -31.5f, 14.8f, 1.1f );
		glTexCoord2f(0.0f, 0.0f); glVertex3f( -31.5f, 14.4f, 1.1f );
		glEnd();
		//Cara superior
		glBegin(GL_QUADS);
		glNormal3f( 0.0f,1.0f,0.0f );
		glTexCoord2f(0.0f, 0.0f); glVertex3f( -34.1f, 14.8f, 1.55f );
		glTexCoord2f(1.0f, 0.0f); glVertex3f( -31.5f, 14.8f, 1.55f );
		glTexCoord2f(1.0f, 1.0f); glVertex3f( -31.5f, 14.8f, 1.1f );
		glTexCoord2f(0.0f, 1.0f); glVertex3f( -34.1f, 14.8f, 1.1f );
		glEnd();
		//Cara inferior
		glBegin(GL_QUADS);
		glNormal3f( 0.0f, -1.0f, 0.0f );
		glTexCoord2f(0.0f, 0.0f); glVertex3f( -34.1f, 14.4f, 1.55f );
		glTexCoord2f(1.0f, 0.0f); glVertex3f( -34.1f, 14.4f, 1.1f );
		glTexCoord2f(1.0f, 1.0f); glVertex3f( -31.5f, 14.4f, 1.1f );
		glTexCoord2f(0.0f, 1.0f); glVertex3f( -31.5f, 14.4f, 1.55f );
		glEnd();
	//Escalon8
		//Cara delantera
		glBegin(GL_QUADS);
		glNormal3f( 0.0f,0.0f,1.0f );
		glTexCoord2f(0.0f, 0.0f); glVertex3f( -34.1f, 15.8f, 1.55f );
		glTexCoord2f(1.0f, 0.0f); glVertex3f( -31.5f, 15.8f, 1.55f );
		glTexCoord2f(1.0f, 1.0f); glVertex3f( -31.5f, 16.2f, 1.55f );
		glTexCoord2f(0.0f, 1.0f); glVertex3f( -34.1f, 16.2f, 1.55f );
		glEnd();
		//Cara derecha
		glBegin(GL_QUADS);
		glNormal3f( 1.0f,0.0f,0.0f );
		glTexCoord2f(0.0f, 0.0f); glVertex3f( -31.5f, 15.8f, 1.55f );
		glTexCoord2f(1.0f, 0.0f); glVertex3f( -31.5f, 15.8f, 1.1f );
		glTexCoord2f(1.0f, 1.0f); glVertex3f( -31.5f, 16.2f, 1.1f );
		glTexCoord2f(0.0f, 1.0f); glVertex3f( -31.5f, 16.2f, 1.55f );
		glEnd();
		//Cara izquierda
		glBegin(GL_QUADS);
		glNormal3f( -1.0f,0.0f,0.0f );
		glTexCoord2f(1.0f, 0.0f); glVertex3f( -34.1f, 15.8f, 1.55f );
		glTexCoord2f(1.0f, 1.0f); glVertex3f( -34.1f, 16.2f, 1.55f );
		glTexCoord2f(0.0f, 1.0f); glVertex3f( -34.1f, 16.2f, 1.1f );
		glTexCoord2f(0.0f, 0.0f); glVertex3f( -34.1f, 15.8f, 1.1f );
		glEnd();
		//Cara trasera
		glBegin(GL_QUADS);
		glNormal3f( 0.0f,0.0f,-1.0f );
		glTexCoord2f(1.0f, 0.0f); glVertex3f( -34.1f, 15.8f, 1.1f );
		glTexCoord2f(1.0f, 1.0f); glVertex3f( -34.1f, 16.2f, 1.1f );
		glTexCoord2f(0.0f, 1.0f); glVertex3f( -31.5f, 16.2f, 1.1f );
		glTexCoord2f(0.0f, 0.0f); glVertex3f( -31.5f, 15.8f, 1.1f );
		glEnd();
		//Cara superior
		glBegin(GL_QUADS);
		glNormal3f( 0.0f,1.0f,0.0f );
		glTexCoord2f(0.0f, 0.0f); glVertex3f( -34.1f, 16.2f, 1.55f );
		glTexCoord2f(1.0f, 0.0f); glVertex3f( -31.5f, 16.2f, 1.55f );
		glTexCoord2f(1.0f, 1.0f); glVertex3f( -31.5f, 16.2f, 1.1f );
		glTexCoord2f(0.0f, 1.0f); glVertex3f( -34.1f, 16.2f, 1.1f );
		glEnd();
		//Cara inferior
		glBegin(GL_QUADS);
		glNormal3f( 0.0f, -1.0f, 0.0f );
		glTexCoord2f(0.0f, 0.0f); glVertex3f( -34.1f, 15.8f, 1.55f );
		glTexCoord2f(1.0f, 0.0f); glVertex3f( -34.1f, 15.8f, 1.1f );
		glTexCoord2f(1.0f, 1.0f); glVertex3f( -31.5f, 15.8f, 1.1f );
		glTexCoord2f(0.0f, 1.0f); glVertex3f( -31.5f, 15.8f, 1.55f );
		glEnd();
	//Escalon9
		//Cara delantera
		glBegin(GL_QUADS);
		glNormal3f( 0.0f,0.0f,1.0f );
		glTexCoord2f(0.0f, 0.0f); glVertex3f( -34.1f, 17.2f, 1.55f );
		glTexCoord2f(1.0f, 0.0f); glVertex3f( -31.5f, 17.2f, 1.55f );
		glTexCoord2f(1.0f, 1.0f); glVertex3f( -31.5f, 17.6f, 1.55f );
		glTexCoord2f(0.0f, 1.0f); glVertex3f( -34.1f, 17.6f, 1.55f );
		glEnd();
		//Cara derecha
		glBegin(GL_QUADS);
		glNormal3f( 1.0f,0.0f,0.0f );
		glTexCoord2f(0.0f, 0.0f); glVertex3f( -31.5f, 17.2f, 1.55f );
		glTexCoord2f(1.0f, 0.0f); glVertex3f( -31.5f, 17.2f, 1.1f );
		glTexCoord2f(1.0f, 1.0f); glVertex3f( -31.5f, 17.6f, 1.1f );
		glTexCoord2f(0.0f, 1.0f); glVertex3f( -31.5f, 17.6f, 1.55f );
		glEnd();
		//Cara izquierda
		glBegin(GL_QUADS);
		glNormal3f( -1.0f,0.0f,0.0f );
		glTexCoord2f(1.0f, 0.0f); glVertex3f( -34.1f, 17.2f, 1.55f );
		glTexCoord2f(1.0f, 1.0f); glVertex3f( -34.1f, 17.6f, 1.55f );
		glTexCoord2f(0.0f, 1.0f); glVertex3f( -34.1f, 17.6f, 1.1f );
		glTexCoord2f(0.0f, 0.0f); glVertex3f( -34.1f, 17.2f, 1.1f );
		glEnd();
		//Cara trasera
		glBegin(GL_QUADS);
		glNormal3f( 0.0f,0.0f,-1.0f );
		glTexCoord2f(1.0f, 0.0f); glVertex3f( -34.1f, 17.2f, 1.1f );
		glTexCoord2f(1.0f, 1.0f); glVertex3f( -34.1f, 17.6f, 1.1f );
		glTexCoord2f(0.0f, 1.0f); glVertex3f( -31.5f, 17.6f, 1.1f );
		glTexCoord2f(0.0f, 0.0f); glVertex3f( -31.5f, 17.2f, 1.1f );
		glEnd();
		//Cara superior
		glBegin(GL_QUADS);
		glNormal3f( 0.0f,1.0f,0.0f );
		glTexCoord2f(0.0f, 0.0f); glVertex3f( -34.1f, 17.6f, 1.55f );
		glTexCoord2f(1.0f, 0.0f); glVertex3f( -31.5f, 17.6f, 1.55f );
		glTexCoord2f(1.0f, 1.0f); glVertex3f( -31.5f, 17.6f, 1.1f );
		glTexCoord2f(0.0f, 1.0f); glVertex3f( -34.1f, 17.6f, 1.1f );
		glEnd();
		//Cara inferior
		glBegin(GL_QUADS);
		glNormal3f( 0.0f, -1.0f, 0.0f );
		glTexCoord2f(0.0f, 0.0f); glVertex3f( -34.1f, 17.2f, 1.55f );
		glTexCoord2f(1.0f, 0.0f); glVertex3f( -34.1f, 17.2f, 1.1f );
		glTexCoord2f(1.0f, 1.0f); glVertex3f( -31.5f, 17.2f, 1.1f );
		glTexCoord2f(0.0f, 1.0f); glVertex3f( -31.5f, 17.2f, 1.55f );
		glEnd();
	//Escalon10
		//Cara delantera
		glBegin(GL_QUADS);
		glNormal3f( 0.0f,0.0f,1.0f );
		glTexCoord2f(0.0f, 0.0f); glVertex3f( -34.1f, 18.6f, 1.55f );
		glTexCoord2f(1.0f, 0.0f); glVertex3f( -31.5f, 18.6f, 1.55f );
		glTexCoord2f(1.0f, 1.0f); glVertex3f( -31.5f, 19.0f, 1.55f );
		glTexCoord2f(0.0f, 1.0f); glVertex3f( -34.1f, 19.0f, 1.55f );
		glEnd();
		//Cara derecha
		glBegin(GL_QUADS);
		glNormal3f( 1.0f,0.0f,0.0f );
		glTexCoord2f(0.0f, 0.0f); glVertex3f( -31.5f, 18.6f, 1.55f );
		glTexCoord2f(1.0f, 0.0f); glVertex3f( -31.5f, 18.6f, 1.1f );
		glTexCoord2f(1.0f, 1.0f); glVertex3f( -31.5f, 19.0f, 1.1f );
		glTexCoord2f(0.0f, 1.0f); glVertex3f( -31.5f, 19.0f, 1.55f );
		glEnd();
		//Cara izquierda
		glBegin(GL_QUADS);
		glNormal3f( -1.0f,0.0f,0.0f );
		glTexCoord2f(1.0f, 0.0f); glVertex3f( -34.1f, 18.6f, 1.55f );
		glTexCoord2f(1.0f, 1.0f); glVertex3f( -34.1f, 19.0f, 1.55f );
		glTexCoord2f(0.0f, 1.0f); glVertex3f( -34.1f, 19.0f, 1.1f );
		glTexCoord2f(0.0f, 0.0f); glVertex3f( -34.1f, 18.6f, 1.1f );
		glEnd();
		//Cara trasera
		glBegin(GL_QUADS);
		glNormal3f( 0.0f,0.0f,-1.0f );
		glTexCoord2f(1.0f, 0.0f); glVertex3f( -34.1f, 18.6f, 1.1f );
		glTexCoord2f(1.0f, 1.0f); glVertex3f( -34.1f, 19.0f, 1.1f );
		glTexCoord2f(0.0f, 1.0f); glVertex3f( -31.5f, 19.0f, 1.1f );
		glTexCoord2f(0.0f, 0.0f); glVertex3f( -31.5f, 18.6f, 1.1f );
		glEnd();
		//Cara superior
		glBegin(GL_QUADS);
		glNormal3f( 0.0f,1.0f,0.0f );
		glTexCoord2f(0.0f, 0.0f); glVertex3f( -34.1f, 19.0f, 1.55f );
		glTexCoord2f(1.0f, 0.0f); glVertex3f( -31.5f, 19.0f, 1.55f );
		glTexCoord2f(1.0f, 1.0f); glVertex3f( -31.5f, 19.0f, 1.1f );
		glTexCoord2f(0.0f, 1.0f); glVertex3f( -34.1f, 19.0f, 1.1f );
		glEnd();
		//Cara inferior
		glBegin(GL_QUADS);
		glNormal3f( 0.0f, -1.0f, 0.0f );
		glTexCoord2f(0.0f, 0.0f); glVertex3f( -34.1f, 18.6f, 1.55f );
		glTexCoord2f(1.0f, 0.0f); glVertex3f( -34.1f, 18.6f, 1.1f );
		glTexCoord2f(1.0f, 1.0f); glVertex3f( -31.5f, 18.6f, 1.1f );
		glTexCoord2f(0.0f, 1.0f); glVertex3f( -31.5f, 18.6f, 1.55f );
		glEnd();
	//Escalon11
		//Cara delantera
		glBegin(GL_QUADS);
		glNormal3f( 0.0f,0.0f,1.0f );
		glTexCoord2f(0.0f, 0.0f); glVertex3f( -34.1f, 20.0f, 1.55f );
		glTexCoord2f(1.0f, 0.0f); glVertex3f( -31.5f, 20.0f, 1.55f );
		glTexCoord2f(1.0f, 1.0f); glVertex3f( -31.5f, 20.4f, 1.55f );
		glTexCoord2f(0.0f, 1.0f); glVertex3f( -34.1f, 20.4f, 1.55f );
		glEnd();
		//Cara derecha
		glBegin(GL_QUADS);
		glNormal3f( 1.0f,0.0f,0.0f );
		glTexCoord2f(0.0f, 0.0f); glVertex3f( -31.5f, 20.0f, 1.55f );
		glTexCoord2f(1.0f, 0.0f); glVertex3f( -31.5f, 20.0f, 1.1f );
		glTexCoord2f(1.0f, 1.0f); glVertex3f( -31.5f, 20.4f, 1.1f );
		glTexCoord2f(0.0f, 1.0f); glVertex3f( -31.5f, 20.4f, 1.55f );
		glEnd();
		//Cara izquierda
		glBegin(GL_QUADS);
		glNormal3f( -1.0f,0.0f,0.0f );
		glTexCoord2f(1.0f, 0.0f); glVertex3f( -34.1f, 20.0f, 1.55f );
		glTexCoord2f(1.0f, 1.0f); glVertex3f( -34.1f, 20.4f, 1.55f );
		glTexCoord2f(0.0f, 1.0f); glVertex3f( -34.1f, 20.4f, 1.1f );
		glTexCoord2f(0.0f, 0.0f); glVertex3f( -34.1f, 20.0f, 1.1f );
		glEnd();
		//Cara trasera
		glBegin(GL_QUADS);
		glNormal3f( 0.0f,0.0f,-1.0f );
		glTexCoord2f(1.0f, 0.0f); glVertex3f( -34.1f, 20.0f, 1.1f );
		glTexCoord2f(1.0f, 1.0f); glVertex3f( -34.1f, 20.4f, 1.1f );
		glTexCoord2f(0.0f, 1.0f); glVertex3f( -31.5f, 20.4f, 1.1f );
		glTexCoord2f(0.0f, 0.0f); glVertex3f( -31.5f, 20.0f, 1.1f );
		glEnd();
		//Cara superior
		glBegin(GL_QUADS);
		glNormal3f( 0.0f,1.0f,0.0f );
		glTexCoord2f(0.0f, 0.0f); glVertex3f( -34.1f, 20.4f, 1.55f );
		glTexCoord2f(1.0f, 0.0f); glVertex3f( -31.5f, 20.4f, 1.55f );
		glTexCoord2f(1.0f, 1.0f); glVertex3f( -31.5f, 20.4f, 1.1f );
		glTexCoord2f(0.0f, 1.0f); glVertex3f( -34.1f, 20.4f, 1.1f );
		glEnd();
		//Cara inferior
		glBegin(GL_QUADS);
		glNormal3f( 0.0f, -1.0f, 0.0f );
		glTexCoord2f(0.0f, 0.0f); glVertex3f( -34.1f, 20.0f, 1.55f );
		glTexCoord2f(1.0f, 0.0f); glVertex3f( -34.1f, 20.0f, 1.1f );
		glTexCoord2f(1.0f, 1.0f); glVertex3f( -31.5f, 20.0f, 1.1f );
		glTexCoord2f(0.0f, 1.0f); glVertex3f( -31.5f, 20.0f, 1.55f );
		glEnd();
}
void DibujaLlave()
{
	Material.SeleccionaMaterial( 10 );
	glPushMatrix();
		glTranslatef( -1.4f, -3.5f, 0.0f );
		Prim.Prisma( 2.0, 0.5, 0.8, 0, 0, false );
	glPopMatrix();

	glPushMatrix();
		glTranslatef( 0.0f, 6.0f, 0.0f );
		glRotatef( 90.0f, 1.0f, 0.0f, 0.0f );
		Prim.Toroide( 2.5f, 1.0f, 24, 20 );
	glPopMatrix();

	Prim.Cilindro( 1.0f, 8.0f, 0, 0, 0, 23, false, true );
	Material.SeleccionaMaterial( 0 );		
}
void DibujaRegalo( int i)
{
	Prim.Prisma( 2.0f, 2.0f, 2.0f, i, i, true );
}

void DibujaMartillo()
{
	Material.SeleccionaMaterial( 8 );
	Prim.Cilindro( 1.0f, 11.0f, 2, 3, 0, 23, false, true );
	Material.SeleccionaMaterial( 0 );

	//Material.SeleccionaMaterial( 14 );
	glPushMatrix();
		glTranslatef( 0.0f, 3.0f, 0.0f );
		glRotatef( 90.0f, 0.0f, 0.0f, 1.0f );
		Prim.Cilindro( 2.0, 8.0f, 2, 3, 0, 23, true, true );
	glPopMatrix();
	//Material.SeleccionaMaterial( 0 );
	
}
void DibujaSwitch()
{
	glDisable( GL_LIGHTING );
	glColor3f( 0.0f, 0.0f, 0.7f );
	Prim.Esfera( 4.0f, 30, 23, 0, 5, false );
	glPushMatrix();
		glTranslatef( 0.0f, 0.51f, 0.0f );
		Prim.Cilindro( 3.48f, 3.0f, 0, 0, 0, 23, false, false );
	glPopMatrix();
	glColor3f( 1.0f, 1.0f, 1.0f );
	glEnable( GL_LIGHTING );
	
}
void DibujaBarril()
{
	glDisable( GL_LIGHTING );
	glDisable(GL_CULL_FACE);
	glAlphaFunc( GL_GREATER, 0.8f );
	glEnable( GL_ALPHA_TEST );

	glPushMatrix();
		glTranslatef( 0.0f, 3.5f, 0.0f );
		Prim.Plano( 2.9f, 2.9f, 0.0f, 1.0f, 1, true );
	glPopMatrix();

	glPushMatrix();
		glTranslatef( 0.0f, 3.5f, 0.0f );
		glRotatef( 90.0f, 0.0f, 1.0f, 0.0f );
		Prim.Plano( 2.9f, 2.9f, 0.0f, 1.0f, 1, true );
	glPopMatrix();

	glDisable( GL_ALPHA_TEST );
	glEnable(GL_CULL_FACE);
	glEnable( GL_LIGHTING );

	Material.SeleccionaMaterial( 14 );
	Prim.Cilindro( 3.0f, 4.5f, 3, 3, 0, 50, false, true );
	Material.SeleccionaMaterial( 0 );
}
void DibujaBodega()
{
	glPushMatrix();
		glTranslatef( 0.0f, 10.0f, 0.0f );
		glRotatef( 90.0f, 1.0f, 0.0f, 0.0f );
		glScalef( 1.0f, 1.0f, 0.6f );
		Prim.Cilindro( 20.0f, 50.0f, 0, 4, 18, 36, true, true );
	glPopMatrix();

	Prim.Prisma( 20.0f, 10.0f, 25.0f, 4, 5, true );
}	
void DibujaPuerta()
{
	glDisable( GL_LIGHTING );
	glDisable(GL_CULL_FACE);
	glAlphaFunc( GL_GREATER, 0.8f );
	glEnable( GL_ALPHA_TEST );

	Prim.Plano( 10.0f, 10.0f, 0.0f, 1.0f, 9, true ); 

	glDisable( GL_ALPHA_TEST );
	glEnable( GL_CULL_FACE );
	glEnable( GL_LIGHTING );
}
int RenderizaEscena(GLvoid)		// Aqui se dibuja todo lo que aparecera en la ventana
{

	// Seed para la funcion rand
	srand( (unsigned)time(0) );

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glLoadIdentity();
	
	gluLookAt(player1.PosicionCam.x, player1.PosicionCam.y, player1.PosicionCam.z, 
			  player1.ObjetivoCam.x, player1.ObjetivoCam.y, player1.ObjetivoCam.z, 
			  0.0f, 1.0f, 0.0f);

	//Para evitar que la posición de la luz se afecte por el movimiento de la cámara se redefine su posición aquí
	lightPosition.x=LightPos[0];
	lightPosition.y=LightPos[1];
	lightPosition.z=LightPos[2];

	glLightfv( GL_LIGHT0, GL_POSITION, LightPos );		// Posicion de la luz0
	
	DibujaAlbin();

	glPushMatrix();
		glTranslatef( 48.0f, 47.0f, 25.0f );
		glScalef( 0.4f, 0.4f, 0.4f );
		glRotatef( rot, 0.0f, 1.0f, 0.0f );
		DibujaLlave();
	glPopMatrix();

	glPushMatrix();
		glTranslatef( 10.0f, 36.5f, 25.0f );
		glScalef( 0.7f, 0.7f, 0.7f );
		DibujaSwitch();
	glPopMatrix();

	glPushMatrix();
		glTranslatef( 20.0f, 78.8f, 25.0f );
		glScalef( 0.7f, 0.7f, 0.7f );
		DibujaSwitch();
	glPopMatrix();

	glPushMatrix();
		glTranslatef( -87.0f, 90.0f, 25.0f );
		glRotatef( rot, 0.0f, 1.0f, 0.0f );
		DibujaRegalo( 6 );
	glPopMatrix();


	glPushMatrix();
		glTranslatef( -87.0f, 20.0f, 25.0f );
		glRotatef( rot, 0.0f, 1.0f, 0.0f );
		DibujaRegalo( 7 );
	glPopMatrix();

	glPushMatrix();
		glTranslatef( -87.0f, 47.0f, 25.0f );
		glScalef( 0.5f, 0.5f, 0.5f );
		glRotatef( rot, 0.0f, 1.0f, 0.0f );
		DibujaMartillo();
	glPopMatrix();

	glPushMatrix();
		glTranslatef( 65.0f, 90.0f, 25.0f );
		glScalef( 0.5f, 0.5f, 0.5f );
		glRotatef( rot, 0.0f, 1.0f, 0.0f );
		DibujaMartillo();
	glPopMatrix();

	glPushMatrix();
		glTranslatef( 10.0f, rot2, 25.0f );
		glRotatef( rot, 0.0f, 1.0f, 0.0f );
		DibujaBarril();
	glPopMatrix();

	glPushMatrix();
		glTranslatef( 48.0f, 10.0f, 25.0f );
		DibujaPuerta();
	glPopMatrix();

	// Dibuja escenario
	glDisable( GL_LIGHTING );
	glPushMatrix();
		glTranslatef( 30.0f, 0.0f, 25.0f );
		glScalef( 3.0f, 3.0f, 5.0f );
		DibujaEscenario1();
	glPopMatrix();
	//DibujaEjes();
	glEnable( GL_LIGHTING );

	glPushMatrix();
		glTranslatef(player1.PosicionObj.x, player1.PosicionObj.y + 1.0f, player1.PosicionObj.z);
		glRotatef(player1.AngObj + 180.0f, 0.0f, 1.0f, 0.0f);
		glScalef(0.6f,0.6f,0.6f);
		glCullFace(GL_FRONT);//para que funcione nuestro modelo de mario
		DibujaPersonaje();
		glCullFace(GL_BACK);
	glPopMatrix();
	glPushMatrix();
		glTranslatef( -20.0f, 36.6f, 25.0f );
		glRotatef(90, 0.0f, 1.0f, 0.0f);
		glCullFace(GL_FRONT);
		DibujaGoomba();
		glCullFace(GL_BACK);
	glPopMatrix();

	// Dibuja la luz global
	DibujaLuz(lightPosition);
	// Agregado para el programa 7
	DibujaIndicadores2D();
	// ActualizaAnimText();

	rot += 1.0;
	if( rot >= 360.0f)
		rot = 0.0f;

	rot2 += 0.2f;
	if( rot2 >= 60.0f)
		rot2 = 45.0f; 
				
	return TRUE;
}

GLvoid KillGLWindow(GLvoid)								// Elimina la ventana apropiadamente
{
	if (hRC)											// Si existe un contexto de renderizado...
	{
		if (!wglMakeCurrent(NULL,NULL))					// Si no se pueden liberar los contextos DC y RC...
		{
			MessageBox(NULL,"Falla al liberar DC y RC.","Error de finalización",MB_OK | MB_ICONINFORMATION);
		}

		if (!wglDeleteContext(hRC))						// Si no se puede eliminar el RC?
		{
			MessageBox(NULL,"Falla al liberar el contexto de renderizado.","Error de finalización",MB_OK | MB_ICONINFORMATION);
		}
		hRC=NULL;										// Se pone RC en NULL
	}

	if (hDC && !ReleaseDC(hWnd,hDC))					// Si no se puede eliminar el DC
	{
		MessageBox(NULL,"Falla al liberar el contexto de renderizado.","Error de finalización",MB_OK | MB_ICONINFORMATION);
		hDC=NULL;										// Se pone DC en NULL
	}

	if (hWnd && !DestroyWindow(hWnd))					// Si no se puede destruir la ventana
	{
		MessageBox(NULL,"No se pudo liberar hWnd.","Error de finalización",MB_OK | MB_ICONINFORMATION);
		hWnd=NULL;										// Se pone hWnd en NULL
	}

	if (!UnregisterClass("OpenGL",hInstance))			// Si no se puede eliminar el registro de la clase
	{
		MessageBox(NULL,"No se pudo eliminar el registro de la clase.","Error de finalización",MB_OK | MB_ICONINFORMATION);
		hInstance=NULL;									// Se pone hInstance en NULL
	}
}

//	Este código crea la ventana de OpenGL.  Parámetros:					
//	title			- Titulo en la parte superior de la ventana			
//	width			- Ancho de la ventana								
//	height			- Alto de la ventana								
//	bits			- Número de bits a usar para el color (8/16/24/32)	
  
BOOL CreateGLWindow(char* title, int width, int height, int bits)
{
	GLuint	PixelFormat;				// Guarda el resultado despues de determinar el formato a usar
	WNDCLASS	wc;						// Estructura de la clase ventana
	DWORD		dwExStyle;				// Estilo extendido de ventana
	DWORD		dwStyle;				// Estilo de ventana
	RECT		WindowRect;				// Guarda los valores Superior Izquierdo / Inferior Derecho del rectángulo
	WindowRect.left=(long)0;			// Inicia el valor Izquierdo a 0
	WindowRect.right=(long)width;		// Inicia el valor Derecho al ancho especificado
	WindowRect.top=(long)0;				// Inicia el valor Superior a 0
	WindowRect.bottom=(long)height;		// Inicia el valor Inferior al alto especificado

	hInstance			= GetModuleHandle(NULL);				// Guarda una instancia de la ventana
	wc.style			= CS_HREDRAW | CS_VREDRAW | CS_OWNDC;	// Redibuja el contenido de la ventana al redimensionarla
	wc.lpfnWndProc		= (WNDPROC) WndProc;					// Maneja los mensajes para WndProc
	wc.cbClsExtra		= 0;									// Ningun dato extra para la clase
	wc.cbWndExtra		= 0;									// Ningun dato extra para la ventana
	wc.hInstance		= hInstance;							// Inicia la instancia
	wc.hIcon			= LoadIcon(NULL, IDI_WINLOGO);			// Carga el ícono por defecto
	wc.hCursor			= LoadCursor(NULL, IDC_ARROW);			// Carga el puntero de flecha
	wc.hbrBackground	= NULL;									// No se requiere ningun fondo
	wc.lpszMenuName		= NULL;									// No hay menú en la ventana
	wc.lpszClassName	= "OpenGL";								// Fija el nombre de la clase.

	if (!RegisterClass(&wc))									// Intenta registrar la clase de ventana
	{
		MessageBox(NULL,"Failed To Register The Window Class.","ERROR",MB_OK|MB_ICONEXCLAMATION);
		return FALSE;											
	}
		
	dwExStyle=WS_EX_APPWINDOW | WS_EX_WINDOWEDGE;					// Estilo extendido de ventana
	dwStyle=WS_OVERLAPPEDWINDOW;									// Estilo de ventana

	AdjustWindowRectEx(&WindowRect, dwStyle, FALSE, dwExStyle);		// Ajusta la ventana al tamaño especificado

	// Crea la ventana
	if (!(hWnd=CreateWindowEx(	dwExStyle,							// Estilo extendido para la ventana
								"OpenGL",							// Nombre de la clase
								title,								// Título de la ventana
								dwStyle |							// Definición del estilo de la ventana
								WS_CLIPSIBLINGS |					// Estilo requerido de la ventana
								WS_CLIPCHILDREN,					// Estilo requerido de la ventana
								0, 0,								// Posición de la ventana
								WindowRect.right-WindowRect.left,	// Calcula el ancho de la ventana
								WindowRect.bottom-WindowRect.top,	// Calcula el alto de la ventana
								NULL,								// No hay ventana superior
								NULL,								// No hay menú
								hInstance,							// Instancia
								NULL)))								// No se pasa nada a WM_CREATE
	{
		KillGLWindow();												// Resetea el despliegue
		MessageBox(NULL,"Error al crear la ventana.","ERROR",MB_OK|MB_ICONEXCLAMATION);
		return FALSE;								
	}

	static	PIXELFORMATDESCRIPTOR pfd=				// pfd Tells Windows How We Want Things To Be
	{
		sizeof(PIXELFORMATDESCRIPTOR),				// Size Of This Pixel Format Descriptor
		1,											// Version Number
		PFD_DRAW_TO_WINDOW |						// Format Must Support Window
		PFD_SUPPORT_OPENGL |						// Format Must Support OpenGL
		PFD_DOUBLEBUFFER,							// Must Support Double Buffering
		PFD_TYPE_RGBA,								// Request An RGBA Format
		bits,										// Select Our Color Depth
		0, 0, 0, 0, 0, 0,							// Color Bits Ignored
		0,											// No Alpha Buffer
		0,											// Shift Bit Ignored
		0,											// No Accumulation Buffer
		0, 0, 0, 0,									// Accumulation Bits Ignored
		16,											// 16Bit Z-Buffer (Depth Buffer)  
		0,											// No Stencil Buffer
		0,											// No Auxiliary Buffer
		PFD_MAIN_PLANE,								// Main Drawing Layer
		0,											// Reserved
		0, 0, 0										// Layer Masks Ignored
	};
	
	if (!(hDC=GetDC(hWnd)))							// Si no se creo el contexto de dispositivo...
	{
		KillGLWindow();								// Resetea el despliegue
		MessageBox(NULL,"No se puede crear un contexto de dispositivo GL.","ERROR",MB_OK|MB_ICONEXCLAMATION);
		return FALSE;								
	}

	if (!(PixelFormat=ChoosePixelFormat(hDC,&pfd)))	// Si Windows no encontró un formato de pixel compatible
	{
		KillGLWindow();								// Resetea el despliegue
		MessageBox(NULL,"No se puede encontrar un formato de pixel compatible.","ERROR",MB_OK|MB_ICONEXCLAMATION);
		return FALSE;								
	}

	if(!SetPixelFormat(hDC,PixelFormat,&pfd))		// Si no se pudo habilitar el formato de pixel
	{
		KillGLWindow();								// Resetea el despliegue
		MessageBox(NULL,"No se puede usar el formato de pixel.","ERROR",MB_OK|MB_ICONEXCLAMATION);
		return FALSE;								
	}

	if (!(hRC=wglCreateContext(hDC)))				// Si no se creo el contexto de renderizado
	{
		KillGLWindow();								// Resetea el despliegue
		MessageBox(NULL,"No se puede crear un contexto de renderizado GL.","ERROR",MB_OK|MB_ICONEXCLAMATION);
		return FALSE;								
	}

	if(!wglMakeCurrent(hDC,hRC))					// Si no se puede activar el contexto de renderizado
	{
		KillGLWindow();								// Resetea el despliegue
		MessageBox(NULL,"No se puede usar el contexto de renderizado GL.","ERROR",MB_OK|MB_ICONEXCLAMATION);
		return FALSE;								
	}

	ShowWindow(hWnd,SW_SHOW);				// Muestra la ventana
	SetForegroundWindow(hWnd);				// Le da la prioridad mas alta
	SetFocus(hWnd);							// Pasa el foco del teclado a la ventana
	ReSizeGLScene(width, height);			// Inicia la perspectiva para la ventana OGL

	if (!InitGL())							// Si no se inicializa la ventana creada
	{
		KillGLWindow();						// Resetea el despliegue
		MessageBox(NULL,"Falla en la inicialización.","ERROR",MB_OK|MB_ICONEXCLAMATION);
		return FALSE;								
	}

	return TRUE;							// Todo correcto
}

LRESULT CALLBACK WndProc(	HWND	hWnd,	// Manejador para esta ventana
							UINT	uMsg,	// Mensaje para esta ventana
							WPARAM	wParam,	// Información adicional del mensaje
							LPARAM	lParam)	// Información adicional del mensaje
{
	switch (uMsg)							// Revisa los mensajes de la ventana
	{
		case WM_ACTIVATE:					// Revisa el mensaje de activación de ventana
		{
			if (!HIWORD(wParam))			// Revisa el estado de minimización
			{
				active=TRUE;				// El programa está activo
			}
			else
			{
				active=FALSE;				// El programa no está activo
			}

			return 0;						// Regresa al ciclo de mensajes
		}

		case WM_SYSCOMMAND:					// Intercepta comandos del sistema
		{
			switch (wParam)					// Revisa llamadas del sistema
			{
				case SC_SCREENSAVE:			// ¿Screensaver tratando de iniciar?
				case SC_MONITORPOWER:		// ¿Monitor tratando de entrar a modo de ahorro de energía?
				return 0;					// Evita que suceda
			}
			break;							// Sale del caso
		}

		case WM_CLOSE:						// Si se recibe un mensaje de cerrar...
		{
			PostQuitMessage(0);				// Se manda el mensaje de salida
			return 0;						// y se regresa al ciclo
		}

		case WM_KEYDOWN:					// Si se está presionando una tecla...
		{
			keys[wParam] = TRUE;			// Si es así, se marca como TRUE
			return 0;						// y se regresa al ciclo
		}

		case WM_KEYUP:						// ¿Se ha soltado una tecla?
		{
			keys[wParam] = FALSE;			// Si es así, se marca como FALSE
			return 0;						// y se regresa al ciclo
		}

		case WM_SIZE:						// Si se redimensiona la ventana...
		{
			ReSizeGLScene(LOWORD(lParam),HIWORD(lParam));  	// LoWord=Width, HiWord=Height
			return 0;						// y se regresa al ciclo
		}
	}

	// Pasa todos los mensajes no considerados a DefWindowProc
	return DefWindowProc(hWnd,uMsg,wParam,lParam);
}

// Este es el punto de entrada al programa; la función principal 
int WINAPI WinMain(	HINSTANCE	hInstance,			// Instancia
					HINSTANCE	hPrevInstance,		// Instancia previa
					LPSTR		lpCmdLine,			// Parametros de la linea de comandos
					int			nCmdShow)			// Muestra el estado de la ventana
{
	MSG		msg;									// Estructura de mensajes de la ventana
	BOOL	done=FALSE;								// Variable booleana para salir del ciclo

	FMOD_SYSTEM      *system;
	FMOD_RESULT       result;
    unsigned int      version;

	unsigned int ms = 0;
    unsigned int lenms = 0;
    int          playing = 0;
    int          paused = 0;
    int          channelsplaying = 0;

	// Crea la ventana OpenGL
	if (!CreateGLWindow("Mario vs Donkey Kong",640,480,16))
	{
		return 0;									// Salir del programa si la ventana no fue creada
	}

	//Se inicia el sistema de sonido de FMOD
	result = FMOD_System_Create(&system);
    ERRCHECK(result);

    result = FMOD_System_GetVersion(system, &version);
    ERRCHECK(result);

    if (version < FMOD_VERSION)
    {
        printf("Error!  You are using an old version of FMOD %08x.  This program requires %08x\n", version, FMOD_VERSION);
        return 0;
    }

    result = FMOD_System_Init(system, 32, FMOD_INIT_NORMAL, NULL);
    ERRCHECK(result);

	result = FMOD_System_CreateSound(system, "Audio/ThemeSongLulz.mp3", FMOD_HARDWARE, 0, &sound[0]);
    ERRCHECK(result);
    result = FMOD_Sound_SetMode(sound[0], FMOD_LOOP_NORMAL);
	ERRCHECK(result);
		    
    result = FMOD_System_CreateSound(system, "Audio/Pasos.wav", FMOD_HARDWARE, 0, &sound[1]);
	result = FMOD_Sound_SetMode(sound[1], FMOD_LOOP_NORMAL);
    ERRCHECK(result);

	// AM I DOING IT RITE?
	result = FMOD_System_CreateSound(system, "Audio/Salto2.mp3", FMOD_HARDWARE, 0, &sound[2]);
	result = FMOD_Sound_SetMode(sound[2], FMOD_LOOP_NORMAL);
    ERRCHECK(result);

	result = FMOD_System_PlaySound(system, FMOD_CHANNEL_FREE, sound[0], 0, &channel[0]);
    ERRCHECK(result);
	result = FMOD_Channel_SetPaused(channel[0], FALSE);
	ERRCHECK(result);
	result = FMOD_Channel_SetVolume(channel[0], 1.0f);
	ERRCHECK(result);
	
	result = FMOD_System_PlaySound(system, FMOD_CHANNEL_FREE, sound[1], 0, &channel[1]);
    ERRCHECK(result);
	result = FMOD_Channel_SetPaused(channel[1], TRUE);
	ERRCHECK(result);

	// IM IN UR BASE KILLING UR DUDES
	result = FMOD_System_PlaySound(system, FMOD_CHANNEL_FREE, sound[2], 0, &channel[2]);
    ERRCHECK(result);
	result = FMOD_Channel_SetPaused(channel[2], TRUE);
	ERRCHECK(result);

	while(!done)									// Mientras done=FALSE
	{
		if (PeekMessage(&msg,NULL,0,0,PM_REMOVE))	// Revisa si hay mensajes en espera
		{
			if (msg.message==WM_QUIT)				// Si se ha recibido el mensje de salir...
			{
				done=TRUE;							// Entonces done=TRUE
			}
			else									// Si no, Procesa los mensajes de la ventana
			{
				TranslateMessage(&msg);				// Traduce el mensaje
				DispatchMessage(&msg);				// Envia el mensaje
			}
		}
		else										// Si no hay mensajes...
		{
			// Dibuja la escena. 
			if (active)								// Si está activo el programa...
			{
				if (keys[VK_ESCAPE])				// Si se ha presionado ESC
				{
					done=TRUE;						// ESC indica el termino del programa
				}
				else								// De lo contrario, actualiza la pantalla
				{
					FMOD_System_Update(system);		// BUILD ADDITIONAL PYLONZZZ
					RenderizaEscena();				// Dibuja la escena
					// Dibuja la escena
					times=times+1;
					if (times%60 == 0)
					tiempo--;

					if(play)
					{
						if(tipoAnim == 1) //caminar
							animacion(KeyFrame1, maxKF1 , 6);
						else if(tipoAnim == 2) //saltar
						{
							animacion(KeyFrame2, maxKF2 , 6);
						}
					}
					SwapBuffers(hDC);				// Intercambia los Buffers (Double Buffering)
				}

				if(!ManejaTeclado()) return 0;
				if(!ManejaTeclado(system, result)) return 0; // WE GOT SIGNAL?!
			}
			
		}
	}

	// Agregado para el programa 7
	Font.DestroyFont();
	// Finalización del programa
	DescargaModelos();
	DescargaTexturas();
	KillGLWindow();									// Destruye la ventana
	LiberaSonido(system, result);
	return (msg.wParam);							// Sale del programa
}

int ManejaTeclado()
{
	if(keys[VK_UP])
	{		
		ControlPersonaje(3);
	}

	if(keys[VK_DOWN])
	{
		ControlPersonaje(4);
	}
	if(keys[VK_LEFT])
	{
		ControlPersonaje(2);
	}
	if(keys[VK_RIGHT])
	{
		ControlPersonaje(1);
	}

	//Al presionar cualquiera de las teclas de dirección se iniciará la animación de caminar
	//Al soltar la tecla presionada se detiene la animación.
	if(GetKeyState(VK_UP) & 0x80) //Si está presionada la tecla UP
	{
		ControlPersonaje(3);

		if(!(GetKeyState(VK_LCONTROL) & 0x80))
		{
			if(play==false) //Para que la asignación de valores siguiente solo se haga una vez y empiece la animación
			{
				//Se le asignan a las variables del personaje los 
				//valores almacenados en el primer keyframe para que
				//inicie desde ahí la animación.
				Angt1   = KeyFrame1[0].Angt1;
				Angt2   = KeyFrame1[0].Angt2;
				Angc    = KeyFrame1[0].Angc;
				Angbi1  = KeyFrame1[0].Angbi1;
				Angbi2  = KeyFrame1[0].Angbi2;
				Angbd1  = KeyFrame1[0].Angbd1;
				Angbd2  = KeyFrame1[0].Angbd2;
				Angpizq = KeyFrame1[0].Angpizq;
				Angpder = KeyFrame1[0].Angpder;
				Angpi   = KeyFrame1[0].Angpi;
				Angpd   = KeyFrame1[0].Angpd;
				Xtor    = KeyFrame1[0].Xtor;
				Ytor    = KeyFrame1[0].Ytor;
				Ztor    = KeyFrame1[0].Ztor;

				play = true;
				playIndex = 0;
				tipoAnim = 1;
					
			}
		}
	}

	if(GetKeyState(VK_DOWN) & 0x80) //Si está presionada la tecla DOWN
	{
		ControlPersonaje(4);
			
		if(play==false) //Para que la asignación de valores siguiente solo se haga una vez y empiece la animación
		{
			//Se le asignan a las variables del personaje los 
			//valores almacenados en el primer keyframe para que
			//inicie desde ahí la animación.
			Angt1   = KeyFrame1[0].Angt1;
			Angt2   = KeyFrame1[0].Angt2;
			Angc    = KeyFrame1[0].Angc;
			Angbi1  = KeyFrame1[0].Angbi1;
			Angbi2  = KeyFrame1[0].Angbi2;
			Angbd1  = KeyFrame1[0].Angbd1;
			Angbd2  = KeyFrame1[0].Angbd2;
			Angpizq = KeyFrame1[0].Angpizq;
			Angpder = KeyFrame1[0].Angpder;
			Angpi   = KeyFrame1[0].Angpi;
			Angpd   = KeyFrame1[0].Angpd;
			Xtor    = KeyFrame1[0].Xtor;
			Ytor    = KeyFrame1[0].Ytor;
			Ztor    = KeyFrame1[0].Ztor;

			play=true;
			playIndex=0;
			tipoAnim=1;
				
		}
	}

	if(GetKeyState(VK_LEFT) & 0x80) //Si está presionada la tecla LEFT
	{
		ControlPersonaje(2);
			
		if(play==false) //Para que la asignación de valores siguiente solo se haga una vez y empiece la animación
		{
			//Se le asignan a las variables del personaje los 
			//valores almacenados en el primer keyframe para que
			//inicie desde ahí la animación.
			Angt1   = KeyFrame1[0].Angt1;
			Angt2   = KeyFrame1[0].Angt2;
			Angc    = KeyFrame1[0].Angc;
			Angbi1  = KeyFrame1[0].Angbi1;
			Angbi2  = KeyFrame1[0].Angbi2;
			Angbd1  = KeyFrame1[0].Angbd1;
			Angbd2  = KeyFrame1[0].Angbd2;
			Angpizq = KeyFrame1[0].Angpizq;
			Angpder = KeyFrame1[0].Angpder;
			Angpi   = KeyFrame1[0].Angpi;
			Angpd   = KeyFrame1[0].Angpd;
			Xtor    = KeyFrame1[0].Xtor;
			Ytor    = KeyFrame1[0].Ytor;
			Ztor    = KeyFrame1[0].Ztor;

			play=true;
			playIndex=0;
			tipoAnim=1;
				
		}
	}

	if(GetKeyState(VK_RIGHT) & 0x80) //Si está presionada la tecla RIGHT
	{
		ControlPersonaje(1);
			
		if(play==false) //Para que la asignación de valores siguiente solo se haga una vez y empiece la animación
		{
			//Se le asignan a las variables del personaje los 
			//valores almacenados en el primer keyframe para que
			//inicie desde ahí la animación.
			Angt1   = KeyFrame1[0].Angt1;
			Angt2   = KeyFrame1[0].Angt2;
			Angc    = KeyFrame1[0].Angc;
			Angbi1  = KeyFrame1[0].Angbi1;
			Angbi2  = KeyFrame1[0].Angbi2;
			Angbd1  = KeyFrame1[0].Angbd1;
			Angbd2  = KeyFrame1[0].Angbd2;
			Angpizq = KeyFrame1[0].Angpizq;
			Angpder = KeyFrame1[0].Angpder;
			Angpi   = KeyFrame1[0].Angpi;
			Angpd   = KeyFrame1[0].Angpd;
			Xtor    = KeyFrame1[0].Xtor;
			Ytor    = KeyFrame1[0].Ytor;
			Ztor    = KeyFrame1[0].Ztor;

			play=true;
			playIndex=0;
			tipoAnim=1;
				
		}
	}

	if( (GetAsyncKeyState(VK_LCONTROL) & 1) ==1 ) //Si se presiona la tecla LCONTROL
	{

		//Para que la asignación de valores siguiente solo se haga una vez y empiece la animación
		if(Ytor < 0.1f) //Brinca solo si no está en el aire
		{
			//Se le asignan a las variables del personaje los 
			//valores almacenados en el primer keyframe para que
			//inicie desde ahí la animación.
			Angt1   = KeyFrame2[0].Angt1;
			Angt2   = KeyFrame2[0].Angt2;
			Angc    = KeyFrame2[0].Angc;
			Angbi1  = KeyFrame2[0].Angbi1;
			Angbi2  = KeyFrame2[0].Angbi2;
			Angbd1  = KeyFrame2[0].Angbd1;
			Angbd2  = KeyFrame2[0].Angbd2;
			Angpizq = KeyFrame2[0].Angpizq;
			Angpder = KeyFrame2[0].Angpder;
			Angpi   = KeyFrame2[0].Angpi;
			Angpd   = KeyFrame2[0].Angpd;
			Xtor    = KeyFrame2[0].Xtor;
			Ytor    = KeyFrame2[0].Ytor;
			Ztor    = KeyFrame2[0].Ztor;
			
			play = true;
			playIndex = 0;
			tipoAnim = 2;

		}

	}

	//Para que al soltar la tecla presionada el personaje no quede en una posición
	//intermedia de la animación se asignan los valores originales a las variables
	if(!(GetKeyState(VK_UP) & 0x80 || GetKeyState(VK_DOWN) & 0x80
	|| GetKeyState(VK_LEFT) & 0x80  || GetKeyState(VK_RIGHT) & 0x80)) //Si no está presionada alguna de esas teclas
	{

		if(tipoAnim == 1) //Para que no afecte la posición si está detenido pero saltando*
		{
			play=false;
			Angt1   = 0.0f;
			Angt2   = 0.0f;
			Angc    = 0.0f;
			Angbi1  = 0.0f;
			Angbi2  = 0.0f;
			Angbd1  = 0.0f;
			Angbd2  = 0.0f;
			Angpizq = 0.0f;
			Angpder = 0.0f;
			Angpi   = 0.0f;
			Angpd   = 0.0f;
			Xtor    = 0.0f;
			Ytor    = 0.0f;
			Ztor    = 0.0f;
		}
		
		//*(Para ello todas las variables del último frame de la secuencia de salto deben ser igual a cero)
	}

	if(keys[VK_PRIOR])
	{
		player1.PosicionObj.y+=0.4f;
		player1.CamaraPosAlt+=0.4f;
		player1.CamaraObjAlt+=0.4f;

		player1.PosicionCam.y=player1.CamaraPosAlt;
		player1.ObjetivoCam=player1.PosicionObj;
		player1.ObjetivoCam.y=player1.CamaraObjAlt;
	}
	if(keys[VK_NEXT])
	{
		player1.PosicionObj.y-=0.4f;
		player1.CamaraPosAlt-=0.4f;
		player1.CamaraObjAlt-=0.4f;

		player1.PosicionCam.y=player1.CamaraPosAlt;
		player1.ObjetivoCam=player1.PosicionObj;
		player1.ObjetivoCam.y=player1.CamaraObjAlt;
	}

	if(keys[VK_HOME])
	{
		ControlPersonaje(5);
	}

	if(keys[VK_END])
	{
		ControlPersonaje(6);
	}

	if((GetAsyncKeyState('D')&1) ==1)
	{
		if(IncDec == 0)
			IncDec=1;
		else if(IncDec == 1)
			IncDec=0;
	}

	//Ang1. Torso
	if(keys['Q'])
	{
		if(IncDec == 0)
			Angt1 += 1.0f;
		else if(IncDec == 1)
			Angt1 -= 1.0f;
	}

	//Ang2. Torso
	if(keys['W'])
	{
		if(IncDec == 0)
			Angt2 += 1.0f;
		else if(IncDec == 1)
			Angt2 -= 1.0f;
	}
	
	//Ang. Cabeza
	if(keys['E'])
	{
		if(IncDec == 0)
			Angc+=1.0f;
		else if(IncDec == 1)
			Angc-=1.0f;
	}

	//Ang. 1 Brazo izq
	if(keys['R'])
	{
		if(IncDec == 0)
			Angbi1+=1.0f;
		else if(IncDec == 1)
			Angbi1-=1.0f;
	}

	//Ang. 2 Brazo izq
	if(keys['T'])
	{
		if(IncDec == 0)
			Angbi2+=1.0f;
		else if(IncDec == 1)
			Angbi2-=1.0f;
	}
	
	//Ang. 1 Brazo der
	if(keys['Y'])
	{
		if(IncDec == 0)
			Angbd1+=1.0f;
		else if(IncDec == 1)
			Angbd1-=1.0f;
	}
	
	//Ang. 2 Brazo der
	if(keys['U'])
	{
		if(IncDec == 0)
			Angbd2+=1.0f;
		else if(IncDec == 1)
			Angbd2-=1.0f;
	}
	
	//Ang. Pierna izq
	if(keys['I'])
	{
		if(IncDec == 0)
			Angpizq+=1.0f;
		else if(IncDec == 1)
			Angpizq-=1.0f;
	}
	
	//Ang. Pierna der
	if(keys['O'])
	{
		if(IncDec == 0)
			Angpder+=1.0f;
		else if(IncDec == 1)
			Angpder-=1.0f;
	}
	
	//Ang. Pie izq
	if(keys['K'])
	{
		if(IncDec == 0)
			Angpi+=1.0f;
		else if(IncDec == 1)
			Angpi-=1.0f;
	}
	
	//Ang. Pie der
	if(keys['L'])
	{
		if(IncDec == 0)
			Angpd+=1.0f;
		else if(IncDec == 1)
			Angpd-=1.0f;
	}
	
	//Pos. X torso
	if(keys['G'])
	{
		if(IncDec == 0)
			Xtor+=0.5f;
		else if(IncDec == 1)
			Xtor-=0.5f;
	}

	//Pos. Y torso
	if(keys['H'])
	{
		if(IncDec == 0)
			Ytor+=0.1f;
		else if(IncDec == 1)
			Ytor-=0.1f;
	}
	
	//Pos. Z torso
	if(keys['J'])
	{
		if(IncDec == 0)
			Ztor+=0.5f;
		else if(IncDec == 1)
			Ztor-=0.5f;
	}

	if(GetKeyState('A') & 0x80)
	{
		if(alcance < 150.0f)
			alcance+=5.0f;
		//else
		//	alcance=0.0f;
	}
	else
		alcance=0.0f;

	if((GetAsyncKeyState(' ')&1) == 1)
	{
		if(KeyFrameIndex == 0)
			if(!(F=fopen("Datos.txt","w"))) exit(0);
	
		if(KeyFrameIndex < maxKF2)
		{
			fprintf(F,"\t// KEYFRAME %d\n\n", KeyFrameIndex + 1 );
			fprintf(F,"\tKeyFrame2[ %d ].Angt1 = %.1ff;\n", KeyFrameIndex, Angt1);
			fprintf(F,"\tKeyFrame2[ %d ].Angt2 = %.1ff;\n", KeyFrameIndex, Angt2);
			fprintf(F,"\tKeyFrame2[ %d ].Angc = %.1ff;\n", KeyFrameIndex, Angc);
			fprintf(F,"\tKeyFrame2[ %d ].Angbi1 = %.1ff;\n", KeyFrameIndex, Angbi1);
			fprintf(F,"\tKeyFrame2[ %d ].Angbi2 = %.1ff;\n", KeyFrameIndex, Angbi2);
			fprintf(F,"\tKeyFrame2[ %d ].Angbd1 = %.1ff;\n", KeyFrameIndex, Angbd1);
			fprintf(F,"\tKeyFrame2[ %d ].Angbd2 = %.1ff;\n", KeyFrameIndex, Angbd2);
			fprintf(F,"\tKeyFrame2[ %d ].Angpizq = %.1ff;\n", KeyFrameIndex, Angpizq);
			fprintf(F,"\tKeyFrame2[ %d ].Angpder = %.1ff;\n", KeyFrameIndex, Angpder);
			fprintf(F,"\tKeyFrame2[ %d ].Angpi = %.1ff;\n", KeyFrameIndex, Angpi);
			fprintf(F,"\tKeyFrame2[ %d ].Xtor = %.1ff;\n", KeyFrameIndex, Xtor);
			fprintf(F,"\tKeyFrame2[ %d ].Ytor = %.1ff;\n", KeyFrameIndex, Ytor);
			fprintf(F,"\tKeyFrame2[ %d ].Ztor = %.1ff;\n", KeyFrameIndex, Ztor);
			fprintf(F,"\n");

			KeyFrameIndex++;
		}
		else
			fclose(F);
	}

	//Controles de la iluminación
	if (keys['Z'])
		LightPos[0] += 1.0f; //Hacia la derecha

	if (keys['X'])
		LightPos[0] -= 1.0f; //Hacia la izquierda

	if (keys['C'])
		LightPos[1] += 1.0f; //Hacia arriba

	if (keys['V'])
		LightPos[1] -= 1.0f; //Hacia abajo

	if (keys['B'])
		LightPos[2] += 1.0f; //Hacia adelante

	if (keys['N'])
		LightPos[2] -= 1.0f; //Hacia atrás

	return TRUE;
}

int ManejaTeclado(FMOD_SYSTEM *system, FMOD_RESULT result)
{
	if((GetKeyState(VK_UP) & 0x80 || GetKeyState(VK_DOWN) & 0x80
	   || GetKeyState(VK_LEFT) & 0x80  || GetKeyState(VK_RIGHT) & 0x80))
	{
		FMOD_Channel_SetPaused(channel[1], FALSE);
	}
	else if(!(GetKeyState(VK_UP) & 0x80 || GetKeyState(VK_DOWN) & 0x80
		 || GetKeyState(VK_LEFT) & 0x80  || GetKeyState(VK_RIGHT) & 0x80))
	{
		FMOD_Channel_SetPaused(channel[1], TRUE);
	}

	// Tolly not 4am code...OR IS IT?!
	if( GetKeyState( VK_LCONTROL ) & 0x80 )
	{
		FMOD_Channel_SetPaused(channel[2], FALSE);
	}
	else if(!(GetKeyState(VK_LCONTROL) & 0x80 ))
	{
		FMOD_Channel_SetPaused(channel[2], TRUE);
	}
	
	return 1;
}