#include "Main.h"
#include "3ds.h"

//Libreria que usamos para el timer
#pragma comment( lib, "winmm.lib" )	

HDC			hDC=NULL;		// Dispositivo de contexto GDI
HGLRC		hRC=NULL;		// Contexto de renderizado
HWND		hWnd=NULL;		// Manejador de ventana
HINSTANCE	hInstance;		// Instancia de la aplicacion

bool	keys[256];			// Arreglo para el manejo de teclado
bool	active=TRUE;		// Bandera de ventana activa

//Variables de Windows
HWND g_hMainWindow = NULL;
OPENFILENAME ofn;
char szFileName[MAX_PATH] = "";
char szFileTitle[MAX_PATH] = "";

int glWidth;
int glHeight;
//int datos[4];

GLUquadricObj	*e;

//Variables para fmod (cambiar el tamaño de los arreglos según el número de archivos de audio a usar)
FMOD_SOUND       *sound[2] = { 0, 0 };
FMOD_CHANNEL     *channel[2] = { 0, 0 };

parametros player1;  //Variable con la que tenemos acceso a la estructura de parámetros
parametros player2;
parametros player3;

CMateriales Material;

//Nombre y ubicación de los modelos
#define FILE_NAME1c  "Modelos/bob_torso.3ds"
#define FILE_NAME2c  "Modelos/bob_piernader_a.3ds"
#define FILE_NAME3c  "Modelos/bob_piernader_b.3ds"
#define FILE_NAME4c  "Modelos/bob_piernaizq_a.3ds"
#define FILE_NAME5c  "Modelos/bob_piernaizq_b.3ds"
#define FILE_NAME6c  "Modelos/bob_brazoder_a.3ds"
#define FILE_NAME7c  "Modelos/bob_brazoizq_a.3ds"
#define FILE_NAME8c  "Modelos/bob_brazoder_b.3ds"
#define FILE_NAME9c  "Modelos/bob_brazoizq_b.3ds"

#define FILE_NAME1p  "Modelos/pat_torso.3ds"
#define FILE_NAME2p  "Modelos/pat_piernader.3ds"
#define FILE_NAME3p  "Modelos/pat_piernaizq.3ds"
#define FILE_NAME4p  "Modelos/pat_brazoder.3ds"
#define FILE_NAME5p  "Modelos/pat_brazoizq.3ds"

#define FILE_NAME1a  "Modelos/plankton_torso.3ds"
#define FILE_NAME2a  "Modelos/plankton_piernader.3ds"
#define FILE_NAME3a  "Modelos/plankton_piernaizq.3ds"
#define FILE_NAME4a  "Modelos/plankton_brazoder.3ds"
#define FILE_NAME5a  "Modelos/plankton_brazoizq.3ds"

#define FILE_NAME1f  "Modelos/fish_torso.3ds"
#define FILE_NAME2f  "Modelos/fish_pierna.3ds"
#define FILE_NAME3f  "Modelos/fish_brazoder.3ds"
#define FILE_NAME4f  "Modelos/fish_brazoizq.3ds"

#define FILE_NAME1e  "Modelos/escenario.3ds"

//Contenedores de texturas de los modelos
CTga textureModel1c[20];
CTga textureModel2c[20];
CTga textureModel3c[20];
CTga textureModel4c[20];
CTga textureModel5c[20];
CTga textureModel6c[20];
CTga textureModel7c[20];
CTga textureModel8c[20];
CTga textureModel9c[20];

CTga textureModel1p[20];
CTga textureModel2p[20];
CTga textureModel3p[20];
CTga textureModel4p[20];
CTga textureModel5p[20];

CTga textureModel1a[20];
CTga textureModel2a[20];
CTga textureModel3a[20];
CTga textureModel4a[20];
CTga textureModel5a[20];

CTga textureModel1f[20];
CTga textureModel2f[20];
CTga textureModel3f[20];
CTga textureModel4f[20];

CTga textureModel1e[20];

CLoad3DS g_Load3ds;

//Acceso a la estructura que almacena los datos de los modelos
t3DModel g_3DModel1c;
t3DModel g_3DModel2c;
t3DModel g_3DModel3c;
t3DModel g_3DModel4c;
t3DModel g_3DModel5c;
t3DModel g_3DModel6c;
t3DModel g_3DModel7c;
t3DModel g_3DModel8c;
t3DModel g_3DModel9c;

t3DModel g_3DModel1p;
t3DModel g_3DModel2p;
t3DModel g_3DModel3p;
t3DModel g_3DModel4p;
t3DModel g_3DModel5p;

t3DModel g_3DModel1a;
t3DModel g_3DModel2a;
t3DModel g_3DModel3a;
t3DModel g_3DModel4a;
t3DModel g_3DModel5a;

t3DModel g_3DModel1f;
t3DModel g_3DModel2f;
t3DModel g_3DModel3f;
t3DModel g_3DModel4f;

t3DModel g_3DModel1e;

//Contenedor de texturas adicionales
CTga textura[30];

jerarquiaModelo player1modelo;	//Acceso a la estructura con las variables de cada pieza del modelo
const int maxKF1=3;				//Num. total de KeyFrames para la secuencia 1 (caminar)
FRAME KeyFrame1[maxKF1];		//Contenedor para almacenar cada keyframe de la secuencia 1

jerarquiaModelo player2modelo;	//Acceso a la estructura con las variables de cada pieza del modelo
FRAME KeyFrame2[maxKF1];

jerarquiaModelo player3modelo;	//Acceso a la estructura con las variables de cada pieza del modelo
FRAME KeyFrame3[maxKF1];

bool play=false;//Bandera para iniciar la animación
bool play1=false;
bool play2=false;
int playIndex=0;//Auxiliar para leer la información del contenedor de keyframes
int playIndex1=0;//Auxiliar para leer la información del contenedor de keyframes
int playIndex2=0;//Auxiliar para leer la información del contenedor de keyframes
int tipoAnim=1; //Indicador del tipo de animación

CMultitexturas Multitext;

GLuint modelo1;
GLuint modelo2;
GLuint modelo3;
GLuint modelo4;

//Constantes de iluminación y materiales
GLfloat LightPos[] = { -5.0f, 65.0f, 65.0f, 1.0f};		// Posición de la luz
GLfloat LightAmb[] = { 0.8f,  0.8f, 0.8f, 1.0f};			// Valores de la componente ambiente
GLfloat LightDif[] = { 0.9f,  0.9f, 0.9f, 1.0f};			// Valores de la componente difusa
GLfloat LightSpc[] = { 0.5f,  0.5f, 0.5f, 1.0f};			// Valores de la componente especular

// Luz spotlight
GLfloat LightAmb1[] = { 0.2f, 0.2f, 0.2f, 1.0f };
GLfloat LightDif1[] = { 1.0f, 1.0f, 1.0f, 1.0f };
GLfloat LightSpc1[] = { 1.0f, 1.0f, 1.0f, 1.0f };
GLfloat LightPos1[] = { 0.0f, 2.0f, 0.0f, 1.0f };
GLfloat SpotDir[] = { 0.0f, -1.0f, 0.0f };

// Valores para la luz spotlight
GLfloat mat_specular[] = { 1.0, 1.0, 1.0, 1.0 };
GLfloat mat_shininess[] = { 50.0 };
GLfloat ambient[] = { 0.1, 0.1, 0.1, 1.0 };

GLfloat upLight = 0.0f;
GLfloat rLight = 0.0f;
GLfloat fLight = 0.0f;

CVector lightPosition;
CVector cLightPosition;

//Variables auxiliares para animación de texturas
float aText1;
float aText2;

//Acceso a la clase CFont
CFont Font;

bool displayVolume = false;

//Acceso a la clase CShadow3DS
CShadow3DS objSh;

//Posición de la luz para el espacio de objeto del modelo1
CVector objectSpaceLightPosition1;
CVector objectSpaceLightPosition2;
CVector objectSpaceLightPosition3;
CVector objectSpaceLightPosition4;
CVector objectSpaceLightPosition5;
CVector objectSpaceLightPosition6;
CVector objectSpaceLightPosition7;
CVector objectSpaceLightPosition8;
CVector objectSpaceLightPosition9;
CVector objectSpaceLightPosition10;
CVector objectSpaceLightPosition11;
CVector objectSpaceLightPosition12;
CVector objectSpaceLightPosition13;
CVector objectSpaceLightPosition14;
CVector objectSpaceLightPosition15;
CVector objectSpaceLightPosition16;
CVector objectSpaceLightPosition17;
CVector objectSpaceLightPosition18;
CVector objectSpaceLightPosition19;
CVector objectSpaceLightPosition20;
CVector objectSpaceLightPosition21;
CVector objectSpaceLightPosition22;
CVector objectSpaceLightPosition23;
CVector objectSpaceLightPosition24;

//Variables para el cálculo de transformaciones inversas para las sombras
typedef float GLvector4f[4];
typedef float GLmatrix16f[16];

//Variables para el control de fps
GLfloat Time1;
GLfloat Time2;
GLfloat DiffTime;
static int FPS = 0;

bool  framerate_limit = true;

int	  adjust=2;											// Speed Adjustment For Really Slow Video Cards

extern int datos[4];

//Variables del spline
int running;
int trayectoria;
int esperar;

int idxtp, dtidx;
int ptsXtramo;
int aw, ah;

int AuxT;

typedef GLfloat TPoint[3];

bool arriba = true;

//Variables para la animación de los puntos de control del parche de Bezier
float yp1 = 0.25;
float ypaux1;

int ran = 0;

CShader cel;
CShader spot;

// Variables para la detección de colisiones
#define maxPlanos 30
#define maxPersonajes 3
boundingplane plano[maxPlanos];
boundingsphere esfera[maxPersonajes];
ray rayo;

struct			 										// Create A Structure For The Timer Information
{
  __int64       frequency;								// Timer Frequency
  float         resolution;								// Timer Resolution
  unsigned long mm_timer_start;							// Multimedia Timer Start Value
  unsigned long mm_timer_elapsed;						// Multimedia Timer Elapsed Time
  bool			performance_timer;						// Using The Performance Timer?
  __int64       performance_timer_start;				// Performance Timer Start Value
  __int64       performance_timer_elapsed;				// Performance Timer Elapsed Time
} timer;												// Structure Is Named timer

int		steps[6]={ 1, 2, 4, 5, 10, 20 };				// Stepping Values For Slow Video Adjustment

LRESULT	CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);	// Declaracion de WndProc (Procedimiento de ventana)

void InicializaObjetosdeColision()
{
	CVector v1, v2, a, b, c, d, va, vb, vd, Normal;
	
	//Inicia planos de colisión del piso
	plano[0].A=CVector(-20.0f, 0.0f, 20.0f);
	plano[0].B=CVector( 43.0f, 0.0f, 20.0f);
	plano[0].C=CVector( 43.0f, 0.0f, -75.0f);
	plano[0].D=CVector(-20.0f, 0.0f, -75.0f);
	plano[0].Normal=CVector(0.0f,1.0f,0.0f);
	plano[0].b1=plano[0].B-plano[0].A;
	plano[0].b2=plano[0].C-plano[0].B;
	plano[0].b3=plano[0].D-plano[0].C;
	plano[0].b4=plano[0].A-plano[0].D;
	plano[0].b1Normal=Cruz(plano[0].b1, plano[0].Normal);
	plano[0].b2Normal=Cruz(plano[0].b2, plano[0].Normal);
	plano[0].b3Normal=Cruz(plano[0].b3, plano[0].Normal);
	plano[0].b4Normal=Cruz(plano[0].b4, plano[0].Normal);
	plano[0].tipo=1;

	//Inicia planos de colisión de los muros

	// Izquierdo
	plano[1].A=CVector(-16.0f,  0.0f, 20.0f);
	plano[1].B=CVector(-16.0f,  0.0f, -55.0f);
	plano[1].C=CVector(-16.0f, 15.0f, -55.0f);
	plano[1].D=CVector(-16.0f, 15.0f, 20.0f);
	plano[1].Normal=CVector(-1.0f,0.0f,0.0f);
	plano[1].b1=plano[4].B-plano[4].A;
	plano[1].b2=plano[4].C-plano[4].B;
	plano[1].b3=plano[4].D-plano[4].C;
	plano[1].b4=plano[4].A-plano[4].D;
	plano[1].b1Normal=Cruz(plano[4].b1, plano[4].Normal);
	plano[1].b2Normal=Cruz(plano[4].b2, plano[4].Normal);
	plano[1].b3Normal=Cruz(plano[4].b3, plano[4].Normal);
	plano[1].b4Normal=Cruz(plano[4].b4, plano[4].Normal);
	plano[1].tipo=2;
	
	// Derecho
	plano[2].A=CVector( 40.0f,  0.0f, -55.0f);
	plano[2].B=CVector( 40.0f,  0.0f, 20.0f);
	plano[2].C=CVector( 40.0f, 15.0f, 20.0f);
	plano[2].D=CVector( 40.0f, 15.0f, -55.0f);
	plano[2].Normal=CVector(1.0f,0.0f,0.0f);
	plano[2].b1=plano[5].B-plano[5].A;
	plano[2].b2=plano[5].C-plano[5].B;
	plano[2].b3=plano[5].D-plano[5].C;
	plano[2].b4=plano[5].A-plano[5].D;
	plano[2].b1Normal=Cruz(plano[5].b1, plano[5].Normal);
	plano[2].b2Normal=Cruz(plano[5].b2, plano[5].Normal);
	plano[2].b3Normal=Cruz(plano[5].b3, plano[5].Normal);
	plano[2].b4Normal=Cruz(plano[5].b4, plano[5].Normal);
	plano[2].tipo=2;
	
	// Frontal
	plano[3].A=CVector(-16.0f, 0.0f, -55.0f);
	plano[3].B=CVector( 40.0f, 0.0f, -55.0f);
	plano[3].C=CVector( 40.0f, 15.0f, -55.0f);
	plano[3].D=CVector(-16.0f, 15.0f, -55.0f);
	plano[3].Normal=CVector(0.0f,0.0f,1.0f);
	plano[3].b1=plano[6].B-plano[6].A;
	plano[3].b2=plano[6].C-plano[6].B;
	plano[3].b3=plano[6].D-plano[6].C;
	plano[3].b4=plano[6].A-plano[6].D;
	plano[3].b1Normal=Cruz(plano[6].b1, plano[6].Normal);
	plano[3].b2Normal=Cruz(plano[6].b2, plano[6].Normal);
	plano[3].b3Normal=Cruz(plano[6].b3, plano[6].Normal);
	plano[3].b4Normal=Cruz(plano[6].b4, plano[6].Normal);
	plano[3].tipo=2;

	// Trasero
	plano[4].A=CVector( 40.0f, 0.0f, 8.0f);
	plano[4].B=CVector( -16.0f, 0.0f, 8.0f);
	plano[4].C=CVector( -16.0f, 15.0f, 8.0f);
	plano[4].D=CVector( 40.0f, 15.0f, 8.0f);
	plano[4].Normal=CVector(0.0f,0.0f,-1.0f);
	plano[4].b1=plano[7].B-plano[7].A;
	plano[4].b2=plano[7].C-plano[7].B;
	plano[4].b3=plano[7].D-plano[7].C;
	plano[4].b4=plano[7].A-plano[7].D;
	plano[4].b1Normal=Cruz(plano[7].b1, plano[7].Normal);
	plano[4].b2Normal=Cruz(plano[7].b2, plano[7].Normal);
	plano[4].b3Normal=Cruz(plano[7].b3, plano[7].Normal);
	plano[4].b4Normal=Cruz(plano[7].b4, plano[7].Normal);
	plano[4].tipo=2;

	for(int i=0; i<maxPlanos; i++)
	{
		plano[i].PM=plano[i].A+(plano[i].C-plano[i].A)*0.5f;
	}

	//Esfera de colision de Bob
	esfera[0].radio = 2.0f;
	esfera[0].Pos = CVector( player1.PosicionObj.x, player1.PosicionObj.y+1.7f, player1.PosicionObj.z );
	esfera[0].colision = false;

	//Esfera de colision de Pat
	esfera[1].radio = 2.5f;
	esfera[1].Pos = CVector( player2.PosicionObj.x + 10.0f,  player2.PosicionObj.y+3.9f, player2.PosicionObj.z );
	esfera[1].colision = false;

	//Esfera de colision de Plankton
	esfera[2].radio = 1.0f;
	esfera[2].Pos = CVector( player3.PosicionObj.x - 10.0f,  player3.PosicionObj.y +  1.5f, player3.PosicionObj.z );
	esfera[2].colision = false;

	// Inicia rayo
	//rayo.punto = CVector( target[0], target[1], target[2] );
	//rayo.dir.ExtendVertexPos( rayo.punto, lightPosition, 50 );
}

void ActualizaObjetosDinamicosColision()
{
	// Bob
	esfera[0].Pos = CVector( player1.PosicionObj.x, player1.PosicionObj.y+1.7f, player1.PosicionObj.z );
	
	// Pat
	esfera[1].Pos = CVector( player2.PosicionObj.x + 10.0f,  player2.PosicionObj.y+3.9f, player2.PosicionObj.z );

	//Plankton
	esfera[2].Pos = CVector( player3.PosicionObj.x - 10.0f,  player3.PosicionObj.y + 1.5f, player3.PosicionObj.z );
	
	// Inicia rayo
	//rayo.punto = CVector( target[0], target[1], target[2] );
	//rayo.dir.ExtendVertexPos( rayo.punto, lightPosition, 50 );
}

void DibujaRayo()
{
	glDisable(GL_LIGHTING);
	//glDisable(GL_COLOR_MATERIAL);

	glColor3f(1.0f,0.0f,0.0f);
	glLineWidth(4.0f);

	// Dibuja rayo
	glBegin( GL_LINES );
		glVertex3f( rayo.punto.x, rayo.punto.y, rayo.punto.z );
		glVertex3f( rayo.dir.x, rayo.dir.y, rayo.dir.z );
	glEnd();

	glColor3f(1.0f,1.0f,1.0f);
	glLineWidth(1.0f);

	glEnable(GL_LIGHTING);
}
void DibujaObjetosdeColision()
{
	glDisable(GL_LIGHTING);

	glColor3f(1.0f,0.0f,0.0f);
	glLineWidth(4.0f);

	for(int i=0; i<maxPlanos; i++)
	{
		glBegin(GL_LINE_LOOP);
			glVertex3f(plano[i].A.x,plano[i].A.y,plano[i].A.z);
			glVertex3f(plano[i].B.x,plano[i].B.y,plano[i].B.z);
			glVertex3f(plano[i].C.x,plano[i].C.y,plano[i].C.z);
			glVertex3f(plano[i].D.x,plano[i].D.y,plano[i].D.z);
		glEnd();
	}
	
	glColor3f(1.0f,1.0f,1.0f);
	glLineWidth(1.0f);

	glEnable(GL_LIGHTING);
}

void DibujaEsferasColision()
{
	GLUquadricObj	*q;

	q=gluNewQuadric();

	gluQuadricDrawStyle(q, GLU_LINE);

	glDisable(GL_LIGHTING);

	glColor3f(1.0f, 1.0f, 1.0f);

	glPushMatrix();
		glTranslatef(esfera[0].Pos.x, esfera[0].Pos.y, esfera[0].Pos.z);
		glRotatef(90.0f,1.0f,0.0f,0.0f);
		gluSphere(q, esfera[0].radio, 16, 8);
	glPopMatrix();

	glPushMatrix();
		glTranslatef(esfera[1].Pos.x, esfera[1].Pos.y, esfera[1].Pos.z);
		glRotatef(90.0f,1.0f,0.0f,0.0f);
		gluSphere(q, esfera[1].radio, 16, 8);
	glPopMatrix();

	glPushMatrix();
		glTranslatef(esfera[2].Pos.x, esfera[2].Pos.y, esfera[2].Pos.z);
		glRotatef(90.0f,1.0f,0.0f,0.0f);
		gluSphere(q, esfera[2].radio, 16, 8);
	glPopMatrix();

	glEnable(GL_LIGHTING);

	gluDeleteQuadric(q);
}

void ColisionEsferaPlano(int id, int dir, parametros& player )
{
	CVector TestPoint;
	CVector PMVect;

	CVector bordeNormal;
	CVector dp;

	CVector PosAux;

	float SaveAltObj, SaveAltCam;

	float Determinante;

	SaveAltObj=player.PosicionObj.y;
	SaveAltCam=player.PosicionCam.y;

	//Se comprueba para cada plano
	for(int j=0; j<maxPlanos; j++)
	{
		if(plano[j].tipo == 2)
		{
			//TestPoint=esfera[id].Pos-plano[j].Normal*esfera[id].radio;
			TestPoint=player.PosicionObj-plano[j].Normal*esfera[id].radio;
			PMVect=TestPoint-plano[j].PM;

			float param=Punto(plano[j].Normal,PMVect);
			if(param <= 0.0001f && param > -0.4f)
			{
				//Ahora se comprueba que el punto de prueba se encuentre dentro de los límites del plano
				dp=TestPoint-plano[j].A;
				Determinante=Punto(plano[j].b1Normal, dp);
				if(Determinante > 0.001f) continue;

				dp=TestPoint-plano[j].B;
				Determinante=Punto(plano[j].b2Normal,dp);	
				if(Determinante > 0.001f) continue;

				dp=TestPoint-plano[j].C;
				Determinante=Punto(plano[j].b3Normal,dp);	
				if(Determinante > 0.001f) continue;

				dp=TestPoint-plano[j].D;
				Determinante=Punto(plano[j].b4Normal,dp);	
				if(Determinante > 0.001f) continue;

				//esfera[0].colision=true;

				if(Punto(plano[j].Normal,PMVect) < 0.0f)
				{
					float deltaV=player.VelocidadObj/10.0f;

					for(int k=0; k<10; k++)
					{
						float vel=k*deltaV;
						if(dir == 1)
							PosAux=player.PosAntObj+player.Direccion*vel;
						else if(dir == 2)
							PosAux=player.PosAntObj-player.Direccion*vel;

						CVector TestPointA=PosAux-plano[j].Normal*esfera[id].radio;
						CVector PMVectA=TestPointA-plano[j].PM;
						
						if(Punto(plano[j].Normal,PMVectA) <= 0.0001f)
						{
							//float velRet=(k+1)*deltaV;
							float velRet=k*deltaV;
							//float velRet=k*deltaV+0.001f;

							player.PosicionObj=PosAux;
							//player.Posicion=player.Posicion+plano[j].Normal*velRet;

							if(dir == 1)
								player.PosicionObj=player.PosicionObj-player.Direccion*velRet;
							else if(dir == 2)
								player.PosicionObj=player.PosicionObj+player.Direccion*velRet;
								
							player.PosicionCam=player.PosicionObj-player.Direccion*player.DistanciaCam;
							player.PosicionCam.y=player.CamaraPosAlt;

							player.ObjetivoCam=player.PosicionObj;
							player.ObjetivoCam.y=player.CamaraObjAlt;

							break;
						}
					}
				}
				
				break;
								
			}

		}
	}
	
}

int ColisionRayoEsfera(ray& a, boundingsphere& s)
{
	CVector m = a.punto - s.Pos;
	float c = Punto( m, m ) - ( s.radio * s.radio );
	if (c <= 0.0f) 
		return 1;
	CVector d = Normaliza( a.dir );
	float b = Punto( m, d );
	if (b > 0.0f) 
		return 0;
	float disc = b * b - c;
	if ( disc < 0.0f ) 
		return 0;
	return 1;
}


void ColisionEsferaEsfera( boundingsphere& a, boundingsphere& b, int dir, parametros& player )
{
	// Calcula la distancia cuadrada entre los centros
	CVector d = a.Pos - b.Pos;
	float dist2 = Punto(d, d);
	// Las esferas se intersectan si la distancia es menor a la suma cuadrada de sus radios
	float radiusSum = a.radio + b.radio;
	float colision = dist2 - radiusSum * radiusSum;
	// Robado de arriba yeah XD
	CVector PosAux;
	float deltaV = player.VelocidadObj/10.0f;

	if( dist2 <= radiusSum * radiusSum ) // si hay una colision
	{
		if(dir == 1)
			PosAux = player.PosAntObj + player.Direccion * ( deltaV + 0.0001f );
		else if(dir == 2)
			PosAux = player.PosAntObj - player.Direccion * ( deltaV + 0.0001f );

		player.PosicionObj = PosAux;

		if( colision < -2.0f )
		{
		if(dir == 1)
			player.PosicionObj = player.PosicionObj - player.Direccion * ( deltaV + 0.3f );
		else if(dir == 2)
			player.PosicionObj = player.PosicionObj + player.Direccion * ( deltaV + 0.3f );
		}
	}
}
bool InterseccionPlanoPiso(CVector P, CVector Dir, boundingplane *plano, float *Altura, CVector *Qp)
{
	float Determinante;
	CVector Normal, vA, vB, vC, vD;
	CVector dp;

	Normal=plano->Normal;
	vA=plano->A;
	vB=plano->B;
	vC=plano->C;
	vD=plano->D;

	float D=Punto(Normal*-1, vA);
	
	float denominador=Punto(Normal,Dir);

	if(fabs(denominador) < 0.0001f)
		return false;

	float numerador=-(Punto(Normal, P)+D);

	float t=numerador/denominador;

	CVector Q = P + Dir*t;

	CVector bordeNormal1=plano->b1Normal;
	CVector bordeNormal2=plano->b2Normal;
	CVector bordeNormal3=plano->b3Normal;
	CVector bordeNormal4=plano->b4Normal;

	dp=Q-vA;
	Determinante=Punto(bordeNormal1, dp);
	if(Determinante > 0.001f) return false;

	dp=Q-vB;
	Determinante=Punto(bordeNormal2,dp);	
	if(Determinante > 0.001f) return false;

	dp=Q-vC;
	Determinante=Punto(bordeNormal3,dp);	
	if(Determinante > 0.001f) return false;

	dp=Q-vD;
	Determinante=Punto(bordeNormal4,dp);	
	if(Determinante > 0.001f) return false;

	*Altura=Q.y;
	*Qp=Q;

	return true;
}

void ColisionesPiso()
{
	float Altura;
	int index;

	CVector Normal;
	CVector Qpoint;

	CVector RayStart = CVector(player1.PosicionObj.x, player1.PosicionObj.y, player1.PosicionObj.z);
	CVector RayDir = CVector(0.0f,-1.0f,0.0f);

	for(int i=0; i<maxPlanos; i++)
	{
		if(plano[i].tipo == 1)
		{
			if(InterseccionPlanoPiso(RayStart, RayDir, &plano[i], &Altura, &Qpoint))
			{
				//player1.PosicionObj.y=Altura;

				player1.PosicionObj.y=Altura;
				player1.CamaraPosAlt=Altura+5.0f;
				player1.CamaraObjAlt=Altura+4.0f+player1.CamaraObjAltE;

				player1.PosicionCam.y=player1.CamaraPosAlt;
				player1.ObjetivoCam=player1.PosicionObj;
				player1.ObjetivoCam.y=player1.CamaraObjAlt;
			}
		}
	}

}


void TimerInit(void)
{
	memset(&timer, 0, sizeof(timer));

	if (!QueryPerformanceFrequency((LARGE_INTEGER *) &timer.frequency))
	{
		timer.performance_timer	= FALSE;
		timer.mm_timer_start	= timeGetTime();
		timer.resolution		= 1.0f/1000.0f;
		timer.frequency			= 1000;
		timer.mm_timer_elapsed	= timer.mm_timer_start;
	}
	else
	{
		QueryPerformanceCounter((LARGE_INTEGER *) &timer.performance_timer_start);
		timer.performance_timer			= TRUE;
		timer.resolution				= (float) (((double)1.0f)/((double)timer.frequency));
		timer.performance_timer_elapsed	= timer.performance_timer_start;
	}
}

float TimerGetTime()
{
	__int64 time;

	if (timer.performance_timer)
	{
		QueryPerformanceCounter((LARGE_INTEGER *) &time);
		return ( (float) ( time - timer.performance_timer_start) * timer.resolution)*1000.0f;
	}
	else
	{
		return( (float) ( timeGetTime() - timer.mm_timer_start) * timer.resolution)*1000.0f;
	}
}

void CalculateFrameRate()
{
	static float framesPerSecond    = 0.0f;
    static float lastTime			= 0.0f;
	static char strFrameRate[50] = {0};

	float currentTime = GetTickCount() * 0.001f;				

	++framesPerSecond;

	if( currentTime - lastTime > 1.0f )
    {
	    lastTime = currentTime;
		
		FPS=int(framesPerSecond);
		
	    framesPerSecond = 0;
    }
}

void VMatMult(GLmatrix16f M, GLvector4f v)
{
	GLfloat res[4];
	res[0]=M[ 0]*v[0]+M[ 4]*v[1]+M[ 8]*v[2]+M[12]*v[3];
	res[1]=M[ 1]*v[0]+M[ 5]*v[1]+M[ 9]*v[2]+M[13]*v[3];
	res[2]=M[ 2]*v[0]+M[ 6]*v[1]+M[10]*v[2]+M[14]*v[3];
	res[3]=M[ 3]*v[0]+M[ 7]*v[1]+M[11]*v[2]+M[15]*v[3];
	v[0]=res[0];
	v[1]=res[1];
	v[2]=res[2];
	v[3]=res[3];
}
struct spline {
	TPoint *ctrlpoints;
	int	    tpc;
	int	    drawp;
	int	    prec;
};

spline luzspline;
spline luzspline2;
spline luzspline3;
TPoint target;

TPoint luzsplinepoints[] = {
	{ -15.0f, 40.0f,   0.0f},
	{ -14.5f, 40.0f,  -5.8f},
	{ -13.8f, 40.0f,  -2.4f},
	{ -15.4f, 40.0f,   0.5f},
	{ -11.9f, 40.0f,  -2.0f},
	{ -10.0f, 40.0f,  -5.0f},
	{ -14.5f, 40.0f,  -8.8f},
	{  -8.2f, 40.0f,  -4.4f},
	{  -7.6f, 40.0f, -14.5f},
	{  -0.5f, 40.0f, -10.0f},
	{  -5.0f, 40.0f, -20.0f},
	{  -4.5f, 40.0f, -15.8f},
	{  -3.8f, 40.0f, -26.4f},
	{  -2.7f, 40.0f, -19.5f},
	{  -1.9f, 40.0f, -32.5f},
	{   0.0f, 40.0f, -35.0f},
};

TPoint luzsplinepoints2[] = {
	{   0.0f, 40.0f,   0.0f},
	{   1.5f, 40.0f,  -5.8f},
	{   5.8f, 40.0f,  -2.4f},
	{   2.4f, 40.0f,   0.5f},
	{  -3.9f, 40.0f,  -2.0f},
	{   0.8f, 40.0f,  -5.0f},
	{   4.5f, 40.0f,  -8.8f},
	{   8.2f, 40.0f,  -4.4f},
	{   3.6f, 40.0f, -14.5f},
	{  -0.5f, 40.0f, -10.0f},
	{   5.0f, 40.0f, -20.0f},
	{  12.5f, 40.0f, -15.8f},
	{   7.8f, 40.0f, -26.4f},
	{   9.7f, 40.0f, -19.5f},
	{  13.9f, 40.0f, -32.5f},
	{  15.0f, 40.0f, -35.0f},
};

TPoint luzsplinepoints3[] = {
	{  15.0f, 40.0f,   0.0f},
	{  14.5f, 40.0f,  -5.8f},
	{  13.8f, 40.0f,  -2.4f},
	{  15.4f, 40.0f,   0.5f},
	{  11.9f, 40.0f,  -2.0f},
	{  10.0f, 40.0f,  -5.0f},
	{  14.5f, 40.0f,  -8.8f},
	{  28.2f, 40.0f,  -4.4f},
	{  17.6f, 40.0f, -14.5f},
	{  20.5f, 40.0f, -10.0f},
	{  15.0f, 40.0f, -20.0f},
	{  24.5f, 40.0f, -15.8f},
	{  13.8f, 40.0f, -26.4f},
	{  12.7f, 40.0f, -19.5f},
	{  21.9f, 40.0f, -32.5f},
	{  30.0f, 40.0f, -35.0f},
};

#define totalCP (sizeof(luzsplinepoints)/sizeof(TPoint))

int isel;
int jsel;
// Estructura para almacenar los puntos de control de la superficie
struct Point {
	float x;
	float y;
	float z;
};

// Malla de 4x4 puntos de control que definen la forma de la superficie
Point Points[4][4] = { 
	{
		{20,0,20},{7,0,20},{-7,0,20},{-20,0,20}
	},
	{
		{20,0,7},{7,0,7},{-7,0,7},{-20,0,7}
	},
	{
		{20,0,-7},{7,0,-7},{-7,0,-7},{-20,0,-7}
	},
	{
		{20,0,-20},{7,0,-20},{-7,0,-20},{-20,0,-20}
	}
};

// Nivel de detalle de la superficie
int LOD=20;

//Prototipos de funciones
LRESULT	CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);		// Declaracion de WndProc (Procedimiento de ventana)

void spline_init( spline &sp, TPoint * ctrl, int tot, int res )
{
	sp.ctrlpoints = ctrl;
	sp.prec = res;
	sp.tpc = tot;
	sp.drawp = (tot-3)*res;
}

void spline_point( spline &sp, int indice,  TPoint P)
{
    int i, j;
	GLfloat t, t3, t2, c1, c2, c3, c4, _1_t;
	GLfloat * Pj3, * Pj2, * Pj1, * Pj0;

    indice = indice % sp.drawp;
	j = indice / sp.prec + 3;
	i = indice % sp.prec;
	Pj3=sp.ctrlpoints[j-3];
	Pj2=sp.ctrlpoints[j-2];
	Pj1=sp.ctrlpoints[j-1];
	Pj0=sp.ctrlpoints[j-0];
	t = i/(double)sp.prec;
	t2 = t*t; 
	t3 = t2*t;
	_1_t = 1-t;
	c1 = (_1_t*_1_t*_1_t) / 6;
	c2 = (3*t3-6*t2+4) / 6;
	c3 = (-3*t3+3*t2+3*t+1) / 6;
	c4 = t3 / 6;
	P[0] = c1* Pj3[0] + c2*Pj2[0] + c3*Pj1[0] + c4*Pj0[0];
	P[1] = c1* Pj3[1] + c2*Pj2[1] + c3*Pj1[1] + c4*Pj0[1];
	P[2] = c1* Pj3[2] + c2*Pj2[2] + c3*Pj1[2] + c4*Pj0[2];
}


//------------------------------------------------------------	CalcularU()
// Dados los 16 puntos de control, se pueden representar 4 curvas trazandose
// en una dirección dada de la superficie. Por ejemplo, si el valor del parámetro u
// es 0.5, equivaldría a evaluar 4 curvas separadas en u=0.5

// Esta función básicamente realiza esa tarea. Cada renglon de 4 puntos
// en la dirección u es evaluado para obtener 4 nuevos puntos.
// Esos nuevos puntos forman una curva que se puede evaluar en
// la dirección v para calcular el valor del punto final.

Point CalcularU(float t,int row) {

	// el punto final para la curva en la dirección u
	Point p;

	// el valor t invertido
	float it = 1.0f-t;

	// se calculan las funciones de mezcla
	float b0 = it*it*it;
	float b1 = 3*t*it*it;
	float b2 = 3*t*t*it;
	float b3 = t*t*t;

	// se suman los efectos de los puntos y sus respectivas funciones de mezcla (definicion de la curva en u)
	p.x = b0*Points[row][0].x +
		  b1*Points[row][1].x +
		  b2*Points[row][2].x +
		  b3*Points[row][3].x ;

	p.y = b0*Points[row][0].y +
		  b1*Points[row][1].y +
		  b2*Points[row][2].y +
		  b3*Points[row][3].y ;

	p.z = b0*Points[row][0].z +
		  b1*Points[row][1].z +
		  b2*Points[row][2].z +
		  b3*Points[row][3].z ;

	return p;
}

//------------------------------------------------------------	CalcularV()
// Habiendo generado 4 puntos en la dirección u, se necesita
// usar esos puntos para generar el punto final en la superficie
// calculando una ultima curva de bezier en la dirección v.

// Esta función toma los puntos temporales de la función calcular y genera
// el punto final para el dibujo de la superficie

Point CalcularV(float t,Point* pnts) {
	Point p;

	// el valor t invertido
	float it = 1.0f-t;

	// se calculan las funciones de mezcla
	float b0 = it*it*it;
	float b1 = 3*t*it*it;
	float b2 = 3*t*t*it;
	float b3 = t*t*t;

	// se suman los efectos de los puntos y sus respectivas funciones de mezcla (definicion de la curva en v)
	p.x = b0*pnts[0].x + 
		  b1*pnts[1].x + 
		  b2*pnts[2].x + 
		  b3*pnts[3].x ;

	p.y = b0*pnts[0].y + 
		  b1*pnts[1].y + 
		  b2*pnts[2].y + 
		  b3*pnts[3].y ;

	p.z = b0*pnts[0].z + 
		  b1*pnts[1].z + 
		  b2*pnts[2].z + 
		  b3*pnts[3].z ;

	return p;
}

//------------------------------------------------------------	Calcular()
// Para el parche de bezier se necesita calcular un conjunto de puntos
// a partir de los parámetros u y v que varian de 0 a 1. Esta función
// realiza esa evaluación usando los valores de coordenadas paramétricas
// u y v especificadas.

Point Calcular(float u,float v) {

	// primero se necesita evaluar 4 curvas en la dirección u
	// Los puntos resultantes seran almacenados en un arreglo
	// temporal
	Point temp[4];
	
	// se calcula cada punto sobre la curva u
	temp[0] = CalcularU(u,0);
	temp[1] = CalcularU(u,1);
	temp[2] = CalcularU(u,2);
	temp[3] = CalcularU(u,3);

	// teniendo los 4 puntos se pueden usar como una curva de bezier
	// para calcular la dirección v. El resultado es el punto final
	// sobre la superficie
	
	return CalcularV(v,temp);
}

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

void CargaTexturas()
{
	textura[0].LoadTGA("Texturas/t1.tga");
	textura[1].LoadTGA("Texturas/text11.tga");
	textura[2].LoadTGA("Texturas/luz.tga");
}

void DescargaTexturas()
{
	textura[0].Elimina();
	textura[1].Elimina();
	textura[2].Elimina();
}

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

	if(!g_Load3ds.Load3DSFile(FILE_NAME1p, &g_3DModel1p, textureModel1p))
		return 0;
	if(!g_Load3ds.Load3DSFile(FILE_NAME2p, &g_3DModel2p, textureModel2p))
		return 0;
	if(!g_Load3ds.Load3DSFile(FILE_NAME3p, &g_3DModel3p, textureModel3p))
		return 0;
	if(!g_Load3ds.Load3DSFile(FILE_NAME4p, &g_3DModel4p, textureModel4p))
		return 0;
	if(!g_Load3ds.Load3DSFile(FILE_NAME5p, &g_3DModel5p, textureModel5p))
		return 0;

	if(!g_Load3ds.Load3DSFile(FILE_NAME1a, &g_3DModel1a, textureModel1a))
		return 0;
	if(!g_Load3ds.Load3DSFile(FILE_NAME2a, &g_3DModel2a, textureModel2a))
		return 0;
	if(!g_Load3ds.Load3DSFile(FILE_NAME3a, &g_3DModel3a, textureModel3a))
		return 0;
	if(!g_Load3ds.Load3DSFile(FILE_NAME4a, &g_3DModel4a, textureModel4a))
		return 0;
	if(!g_Load3ds.Load3DSFile(FILE_NAME5a, &g_3DModel5a, textureModel5a))
		return 0;

	if(!g_Load3ds.Load3DSFile(FILE_NAME1f, &g_3DModel1f, textureModel1f))
		return 0;
	if(!g_Load3ds.Load3DSFile(FILE_NAME2f, &g_3DModel2f, textureModel2f))
		return 0;
	if(!g_Load3ds.Load3DSFile(FILE_NAME3f, &g_3DModel3f, textureModel3f))
		return 0;
	if(!g_Load3ds.Load3DSFile(FILE_NAME4f, &g_3DModel4f, textureModel4f))
		return 0;

	if(!g_Load3ds.Load3DSFile(FILE_NAME1e, &g_3DModel1e, textureModel1e))
		return 0;
		
	return TRUE;
}

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

	g_Load3ds.UnLoad3DSFile(&g_3DModel1p, textureModel1p);
	g_Load3ds.UnLoad3DSFile(&g_3DModel2p, textureModel2p);
	g_Load3ds.UnLoad3DSFile(&g_3DModel3p, textureModel3p);
	g_Load3ds.UnLoad3DSFile(&g_3DModel4p, textureModel4p);
	g_Load3ds.UnLoad3DSFile(&g_3DModel5p, textureModel5p);

	g_Load3ds.UnLoad3DSFile(&g_3DModel1a, textureModel1a);
	g_Load3ds.UnLoad3DSFile(&g_3DModel2a, textureModel2a);
	g_Load3ds.UnLoad3DSFile(&g_3DModel3a, textureModel3a);
	g_Load3ds.UnLoad3DSFile(&g_3DModel4a, textureModel4a);
	g_Load3ds.UnLoad3DSFile(&g_3DModel5a, textureModel5a);

	g_Load3ds.UnLoad3DSFile(&g_3DModel1f, textureModel1f);
	g_Load3ds.UnLoad3DSFile(&g_3DModel2f, textureModel2f);
	g_Load3ds.UnLoad3DSFile(&g_3DModel3f, textureModel3f);
	g_Load3ds.UnLoad3DSFile(&g_3DModel4f, textureModel4f);
	
	g_Load3ds.UnLoad3DSFile(&g_3DModel1e, textureModel1e);
}

void IniSombraVolumen()
{
	//Establece la conectividad entre caras

	//Bob
	objSh.EstableceConectividad(&g_3DModel1c);
	objSh.calculaPlano(&g_3DModel1c);

	objSh.EstableceConectividad(&g_3DModel2c);
	objSh.calculaPlano(&g_3DModel2c);

	objSh.EstableceConectividad(&g_3DModel3c);
	objSh.calculaPlano(&g_3DModel3c);

	objSh.EstableceConectividad(&g_3DModel4c);
	objSh.calculaPlano(&g_3DModel4c);

	objSh.EstableceConectividad(&g_3DModel5c);
	objSh.calculaPlano(&g_3DModel5c);

	objSh.EstableceConectividad(&g_3DModel6c);
	objSh.calculaPlano(&g_3DModel6c);

	objSh.EstableceConectividad(&g_3DModel7c);
	objSh.calculaPlano(&g_3DModel7c);

	objSh.EstableceConectividad(&g_3DModel8c);
	objSh.calculaPlano(&g_3DModel8c);

	objSh.EstableceConectividad(&g_3DModel9c);
	objSh.calculaPlano(&g_3DModel9c);

	//Patricio
	objSh.EstableceConectividad(&g_3DModel1p);
	objSh.calculaPlano(&g_3DModel1p);

	objSh.EstableceConectividad(&g_3DModel2p);
	objSh.calculaPlano(&g_3DModel2p);

	objSh.EstableceConectividad(&g_3DModel3p);
	objSh.calculaPlano(&g_3DModel3p);

	objSh.EstableceConectividad(&g_3DModel4p);
	objSh.calculaPlano(&g_3DModel4p);

	objSh.EstableceConectividad(&g_3DModel5p);
	objSh.calculaPlano(&g_3DModel5p);

	//Plankton
	objSh.EstableceConectividad(&g_3DModel1a);
	objSh.calculaPlano(&g_3DModel1a);

	objSh.EstableceConectividad(&g_3DModel2a);
	objSh.calculaPlano(&g_3DModel2a);

	objSh.EstableceConectividad(&g_3DModel3a);
	objSh.calculaPlano(&g_3DModel3a);

	objSh.EstableceConectividad(&g_3DModel4a);
	objSh.calculaPlano(&g_3DModel4a);

	objSh.EstableceConectividad(&g_3DModel5a);
	objSh.calculaPlano(&g_3DModel5a);

	//Peces
	objSh.EstableceConectividad(&g_3DModel1f);
	objSh.calculaPlano(&g_3DModel1f);

	objSh.EstableceConectividad(&g_3DModel2f);
	objSh.calculaPlano(&g_3DModel2f);

	objSh.EstableceConectividad(&g_3DModel3f);
	objSh.calculaPlano(&g_3DModel3f);

	objSh.EstableceConectividad(&g_3DModel4f);
	objSh.calculaPlano(&g_3DModel4f);
			
}

void CreaListas()
{
	//Bob
	modelo1=glGenLists(19);

	glNewList(modelo1+0,GL_COMPILE);
		g_Load3ds.Render3DSFile(&g_3DModel1c, textureModel1c, 1);
	glEndList();

	glNewList(modelo1+1,GL_COMPILE);
		g_Load3ds.Render3DSFile(&g_3DModel2c, textureModel2c, 1);
	glEndList();

	glNewList(modelo1+2,GL_COMPILE);
		g_Load3ds.Render3DSFile(&g_3DModel3c, textureModel3c, 1);
	glEndList();

	glNewList(modelo1+3,GL_COMPILE);
		g_Load3ds.Render3DSFile(&g_3DModel4c, textureModel4c, 1);
	glEndList();

	glNewList(modelo1+4,GL_COMPILE);
		g_Load3ds.Render3DSFile(&g_3DModel5c, textureModel5c, 1);
	glEndList();

	glNewList(modelo1+5,GL_COMPILE);
		g_Load3ds.Render3DSFile(&g_3DModel6c, textureModel6c, 1);
	glEndList();

	glNewList(modelo1+6,GL_COMPILE);
		g_Load3ds.Render3DSFile(&g_3DModel7c, textureModel7c, 1);
	glEndList();

	glNewList(modelo1+7,GL_COMPILE);
		g_Load3ds.Render3DSFile(&g_3DModel8c, textureModel8c, 1);
	glEndList();

	glNewList(modelo1+8,GL_COMPILE);
		g_Load3ds.Render3DSFile(&g_3DModel9c, textureModel9c, 1);
	glEndList();

	//Bob contorno

	glNewList(modelo1+10,GL_COMPILE);
		g_Load3ds.Render3DSContour(&g_3DModel1c);
	glEndList();

	glNewList(modelo1+11,GL_COMPILE);
		g_Load3ds.Render3DSContour(&g_3DModel2c);
	glEndList();

	glNewList(modelo1+12,GL_COMPILE);
		g_Load3ds.Render3DSContour(&g_3DModel3c);
	glEndList();

	glNewList(modelo1+13,GL_COMPILE);
		g_Load3ds.Render3DSContour(&g_3DModel4c);
	glEndList();

	glNewList(modelo1+14,GL_COMPILE);
		g_Load3ds.Render3DSContour(&g_3DModel5c);
	glEndList();

	glNewList(modelo1+15,GL_COMPILE);
		g_Load3ds.Render3DSContour(&g_3DModel6c);
	glEndList();

	glNewList(modelo1+16,GL_COMPILE);
		g_Load3ds.Render3DSContour(&g_3DModel7c);
	glEndList();

	glNewList(modelo1+17,GL_COMPILE);
		g_Load3ds.Render3DSContour(&g_3DModel8c);
	glEndList();

	glNewList(modelo1+18,GL_COMPILE);
		g_Load3ds.Render3DSContour(&g_3DModel9c);
	glEndList();

	//Patricio
	modelo2=glGenLists(10);

	glNewList(modelo2+0,GL_COMPILE);
		g_Load3ds.Render3DSFile(&g_3DModel1p, textureModel1p, 1);
	glEndList();

	glNewList(modelo2+1,GL_COMPILE);
		g_Load3ds.Render3DSFile(&g_3DModel2p, textureModel2p, 1);
	glEndList();

	glNewList(modelo2+2,GL_COMPILE);
		g_Load3ds.Render3DSFile(&g_3DModel3p, textureModel3p, 1);
	glEndList();

	glNewList(modelo2+3,GL_COMPILE);
		g_Load3ds.Render3DSFile(&g_3DModel4p, textureModel4p, 1);
	glEndList();

	glNewList(modelo2+4,GL_COMPILE);
		g_Load3ds.Render3DSFile(&g_3DModel5p, textureModel5p, 1);
	glEndList();

	//Wired Pat
	
	glNewList(modelo2+5,GL_COMPILE);
		g_Load3ds.Render3DSContour(&g_3DModel1p);
	glEndList();

	glNewList(modelo2+6,GL_COMPILE);
		g_Load3ds.Render3DSContour(&g_3DModel2p);
	glEndList();

	glNewList(modelo2+7,GL_COMPILE);
		g_Load3ds.Render3DSContour(&g_3DModel3p);
	glEndList();

	glNewList(modelo2+8,GL_COMPILE);
		g_Load3ds.Render3DSContour(&g_3DModel4p);
	glEndList();

	glNewList(modelo2+9,GL_COMPILE);
		g_Load3ds.Render3DSContour(&g_3DModel5p);
	glEndList();

	//Plankton
	modelo3=glGenLists(10);

	glNewList(modelo3+0,GL_COMPILE);
		g_Load3ds.Render3DSFile(&g_3DModel1a, textureModel1a, 1);
	glEndList();

	glNewList(modelo3+1,GL_COMPILE);
		g_Load3ds.Render3DSFile(&g_3DModel2a, textureModel2a, 1);
	glEndList();

	glNewList(modelo3+2,GL_COMPILE);
		g_Load3ds.Render3DSFile(&g_3DModel3a, textureModel3a, 1);
	glEndList();

	glNewList(modelo3+3,GL_COMPILE);
		g_Load3ds.Render3DSFile(&g_3DModel4a, textureModel4a, 1);
	glEndList();

	glNewList(modelo3+4,GL_COMPILE);
		g_Load3ds.Render3DSFile(&g_3DModel5a,textureModel5a, 1);
	glEndList();

	//wired plankton
	glNewList(modelo3+5,GL_COMPILE);
		g_Load3ds.Render3DSContour(&g_3DModel1a);
	glEndList();

	glNewList(modelo3+6,GL_COMPILE);
		g_Load3ds.Render3DSContour(&g_3DModel2a);
	glEndList();

	glNewList(modelo3+7,GL_COMPILE);
		g_Load3ds.Render3DSContour(&g_3DModel3a);
	glEndList();

	glNewList(modelo3+8,GL_COMPILE);
		g_Load3ds.Render3DSContour(&g_3DModel4a);
	glEndList();

	glNewList(modelo3+9,GL_COMPILE);
		g_Load3ds.Render3DSContour(&g_3DModel5a);
	glEndList();

	//Peces
	modelo4=glGenLists(8);

	glNewList(modelo4+0,GL_COMPILE);
		g_Load3ds.Render3DSFile(&g_3DModel1f, textureModel1f, 1);
	glEndList();

	glNewList(modelo4+1,GL_COMPILE);
		g_Load3ds.Render3DSFile(&g_3DModel2f, textureModel2f, 1);
	glEndList();

	glNewList(modelo4+2,GL_COMPILE);
		g_Load3ds.Render3DSFile(&g_3DModel3f, textureModel3f, 1);
	glEndList();

	glNewList(modelo4+3,GL_COMPILE);
		g_Load3ds.Render3DSFile(&g_3DModel4f, textureModel4f, 1);
	glEndList();

	//Peces Wired
	glNewList(modelo4+4,GL_COMPILE);
		g_Load3ds.Render3DSContour(&g_3DModel1f);
	glEndList();

	glNewList(modelo4+5,GL_COMPILE);
		g_Load3ds.Render3DSContour(&g_3DModel2f);
	glEndList();

	glNewList(modelo4+6,GL_COMPILE);
		g_Load3ds.Render3DSContour(&g_3DModel3f);
	glEndList();

	glNewList(modelo4+7,GL_COMPILE);
		g_Load3ds.Render3DSContour(&g_3DModel4f);
	glEndList();
}

void DestruyeListas()
{
	glDeleteLists(modelo1,19);
	glDeleteLists(modelo2,10);
	glDeleteLists(modelo3,10);
	glDeleteLists(modelo4,8);
}
void InicializaParametrosdeControl()
{
	//Esta función establece los parámetros como velocidad del objeto y distancia de la cámara así como la posición y dirección iniciales
	player1.visible=true;
	player1.VelocidadObj=0.2f;
	player1.DistanciaCam=10.0f;

	player1.CamaraPosAlt=5.0f;	//Posición en y de la cámara (altura a la que se situa la cámara)
	player1.CamaraObjAlt=4.0f;	//Posición en y del objetivo de la cámara (altura a la que ve la cámara)
	player1.AngDir=90.0f;		//Este ángulo inicial hace que la dirección inicial sea paralela al eje Z y con sentido negativo
	player1.AngObj=0.0f;		//Este valor se elige dependiendo de la orientación con la que aparece el modelo en la escena al dibujarlo
								//sin aplicarle ninguna transformación (hacia adonde está volteando). Se elige un ángulo tal que al aplicarle
								//una rotación inicial con respecto al eje Y esté viendo hacia la misma dirección que la definida por AngDir
	
	player1.PosicionObj=CVector(0.0f, 0.0f, 0.0f); //Esta es la posición inicial del objeto en la escena
	player1.Direccion.x=(float)cos(player1.AngDir*PI/180.0f); //Dirección inicial definida por el ángulo inicial AngDir (x=cos(AngDir), y=0.0, z=sen(AngDir))
	player1.Direccion.y=0.0f;
	player1.Direccion.z=(float)sin(player1.AngDir*PI/180.0f);   
	player1.PosicionCam=CVector(0.0f, player1.CamaraPosAlt, 10.0f); //Posición inicial de la cámara a [DistanciaCam] unidades detrás del objeto
	player1.ObjetivoCam=player1.PosicionObj;		//La cámara ve siempre al objeto
	player1.ObjetivoCam.y=player1.CamaraObjAlt;		//Para que no vea a los "pies" del objeto (personaje)

	player1.Dir=0;
	player1.DirAnt=0;

	player1.escalaX=0.4f;
	player1.escalaY=0.4f;
	player1.escalaZ=0.4f;

	player1.CamaraObjAltE=0.0f;

	//Pat
	player2.visible=true;
	player2.VelocidadObj=0.2f;
	player2.DistanciaCam=10.0f;

	player2.CamaraPosAlt=5.0f;	//Posición en y de la cámara (altura a la que se situa la cámara)
	player2.CamaraObjAlt=4.0f;	//Posición en y del objetivo de la cámara (altura a la que ve la cámara)
	player2.AngDir=90.0f;		//Este ángulo inicial hace que la dirección inicial sea paralela al eje Z y con sentido negativo
	player2.AngObj=0.0f;		//Este valor se elige dependiendo de la orientación con la que aparece el modelo en la escena al dibujarlo
								//sin aplicarle ninguna transformación (hacia adonde está volteando). Se elige un ángulo tal que al aplicarle
								//una rotación inicial con respecto al eje Y esté viendo hacia la misma dirección que la definida por AngDir
	
	player2.PosicionObj=CVector(0.0f, 0.0f, 0.0f); //Esta es la posición inicial del objeto en la escena
	player2.Direccion.x=cosf(player2.AngDir*PI/180.0f); //Dirección inicial definida por el ángulo inicial AngDir (x=cos(AngDir), y=0.0, z=sen(AngDir))
	player2.Direccion.y=0.0f;
	player2.Direccion.z=sinf(player2.AngDir*PI/180.0f);   
	player2.PosicionCam=CVector(0.0f, player2.CamaraPosAlt, 10.0f); //Posición inicial de la cámara a [DistanciaCam] unidades detrás del objeto
	player2.ObjetivoCam=player2.PosicionObj;		//La cámara ve siempre al objeto
	player2.ObjetivoCam.y=player2.CamaraObjAlt;		//Para que no vea a los "pies" del objeto (personaje)

	player2.Dir=0;
	player2.DirAnt=0;

	player2.escalaX=0.4f;
	player2.escalaY=0.4f;
	player2.escalaZ=0.4f;

	player2.CamaraObjAltE=0.0f;

	//Plankton
	player3.visible=true;
	player3.VelocidadObj=0.2f;
	player3.DistanciaCam=10.0f;

	player3.CamaraPosAlt=5.0f;	//Posición en y de la cámara (altura a la que se situa la cámara)
	player3.CamaraObjAlt=4.0f;	//Posición en y del objetivo de la cámara (altura a la que ve la cámara)
	player3.AngDir=90.0f;		//Este ángulo inicial hace que la dirección inicial sea paralela al eje Z y con sentido negativo
	player3.AngObj=0.0f;		//Este valor se elige dependiendo de la orientación con la que aparece el modelo en la escena al dibujarlo
								//sin aplicarle ninguna transformación (hacia adonde está volteando). Se elige un ángulo tal que al aplicarle
								//una rotación inicial con respecto al eje Y esté viendo hacia la misma dirección que la definida por AngDir
	
	player3.PosicionObj=CVector(0.0f, 0.0f, 0.0f); //Esta es la posición inicial del objeto en la escena
	player3.Direccion.x=cosf(player3.AngDir*PI/180.0f); //Dirección inicial definida por el ángulo inicial AngDir (x=cos(AngDir), y=0.0, z=sen(AngDir))
	player3.Direccion.y=0.0f;
	player3.Direccion.z=sinf(player3.AngDir*PI/180.0f);   
	player3.PosicionCam=CVector(0.0f, player3.CamaraPosAlt, 10.0f); //Posición inicial de la cámara a [DistanciaCam] unidades detrás del objeto
	player3.ObjetivoCam=player3.PosicionObj;		//La cámara ve siempre al objeto
	player3.ObjetivoCam.y=player3.CamaraObjAlt;		//Para que no vea a los "pies" del objeto (personaje)

	player3.Dir=0;
	player3.DirAnt=0;

	player3.escalaX=0.4f;
	player3.escalaY=0.4f;
	player3.escalaZ=0.4f;

	player3.CamaraObjAltE=0.0f;

}

void InicializaAnim()
{
	//Se inicializan las variables de la secuencia 1
	for(int i=0; i<maxKF1; i++)
	{
		KeyFrame1[i].Angt1=0.0f;
		KeyFrame1[i].Angt2=0.0f;
		KeyFrame1[i].Angc1=0.0f;
		KeyFrame1[i].Angc2=0.0f;
		KeyFrame1[i].Angbi1=0.0f;
		KeyFrame1[i].Angbi2=0.0f;
		KeyFrame1[i].Angbib=0.0f;
		KeyFrame1[i].Angbd1=0.0f;
		KeyFrame1[i].Angbd2=0.0f;
		KeyFrame1[i].Angbdb=0.0f;
		KeyFrame1[i].Angpizq=0.0f;
		KeyFrame1[i].Angpizqb=0.0f;
		KeyFrame1[i].Angpder=0.0f;
		KeyFrame1[i].Angpderb=0.0f;
		KeyFrame1[i].Angpi=0.0f;
		KeyFrame1[i].Angpd=0.0f;
		KeyFrame1[i].Xtor=0.0f;
		KeyFrame1[i].Ytor=0.0f;
		KeyFrame1[i].Ztor=0.0f;

		KeyFrame1[i].incAngt1=false;
		KeyFrame1[i].incAngt1=false;
		KeyFrame1[i].incAngc1=false;
		KeyFrame1[i].incAngc2=false;
		KeyFrame1[i].incAngbi1=false;
		KeyFrame1[i].incAngbi2=false;
		KeyFrame1[i].incAngbib=false;
		KeyFrame1[i].incAngbd1=false;
		KeyFrame1[i].incAngbd2=false;
		KeyFrame1[i].incAngbdb=false;
		KeyFrame1[i].incAngpizq=false;
		KeyFrame1[i].incAngpizqb=false;
		KeyFrame1[i].incAngpder=false;
		KeyFrame1[i].incAngpderb=false;
		KeyFrame1[i].incAngpi=false;
		KeyFrame1[i].incAngpd=false;
		KeyFrame1[i].incXtor=false;
		KeyFrame1[i].incYtor=false;
		KeyFrame1[i].incZtor=false;

		KeyFrame2[i].Angt1=0.0f;
		KeyFrame2[i].Angt2=0.0f;
		KeyFrame2[i].Angbi1=0.0f;
		KeyFrame2[i].Angbd1=0.0f;
		KeyFrame2[i].Angpizq=0.0f;
		KeyFrame2[i].Angpder=0.0f;
		KeyFrame2[i].Xtor=0.0f;
		KeyFrame2[i].Ytor=0.0f;
		KeyFrame2[i].Ztor=0.0f;

		KeyFrame2[i].incAngt1=false;
		KeyFrame2[i].incAngt2=false;
		KeyFrame2[i].incAngbi1=false;
		KeyFrame2[i].incAngbd1=false;
		KeyFrame2[i].incAngpizq=false;
		KeyFrame2[i].incAngpder=false;
		KeyFrame2[i].incXtor=false;
		KeyFrame2[i].incYtor=false;
		KeyFrame2[i].incZtor=false;

		KeyFrame3[i].Angt1=0.0f;
		KeyFrame3[i].Angt2=0.0f;
		KeyFrame3[i].Angbi1=0.0f;
		KeyFrame3[i].Angbd1=0.0f;
		KeyFrame3[i].Angpizq=0.0f;
		KeyFrame3[i].Angpder=0.0f;
		KeyFrame3[i].Xtor=0.0f;
		KeyFrame3[i].Ytor=0.0f;
		KeyFrame3[i].Ztor=0.0f;

		KeyFrame3[i].incAngt1=false;
		KeyFrame3[i].incAngt2=false;
		KeyFrame3[i].incAngbi1=false;
		KeyFrame3[i].incAngbd1=false;
		KeyFrame3[i].incAngpizq=false;
		KeyFrame3[i].incAngpder=false;
		KeyFrame3[i].incXtor=false;
		KeyFrame3[i].incYtor=false;
		KeyFrame3[i].incZtor=false;
	}

	player1modelo.Angt1=0.0f;
	player1modelo.Angt2=0.0f;
	player1modelo.Angc1=0.0f;
	player1modelo.Angc2=0.0f;
	player1modelo.Angbi1=0.0f;
	player1modelo.Angbi2=0.0f;
	player1modelo.Angbib=0.0f;
	player1modelo.Angbd1=0.0f;
	player1modelo.Angbd2=0.0f;
	player1modelo.Angbdb=0.0f;
	player1modelo.Angpizq=0.0f;
	player1modelo.Angpizqb=0.0f;
	player1modelo.Angpder=0.0f;
	player1modelo.Angpderb=0.0f;
	player1modelo.Angpi=0.0f;
	player1modelo.Angpd=0.0f;
	player1modelo.Xtor=0.0f;
	player1modelo.Ytor=0.0f;
	player1modelo.Ztor=0.0f;

	player2modelo.Angt1=0.0f;
	player2modelo.Angt2=0.0f;
	player2modelo.Angbi1=0.0f;
	player2modelo.Angbd1=0.0f;
	player2modelo.Angpizq=0.0f;
	player2modelo.Angpder=0.0f;
	player2modelo.Xtor=0.0f;
	player2modelo.Ytor=0.0f;
	player2modelo.Ztor=0.0f;

	player3modelo.Angt1=0.0f;
	player3modelo.Angt2=0.0f;
	player3modelo.Angbi1=0.0f;
	player3modelo.Angbd1=0.0f;
	player3modelo.Angpizq=0.0f;
	player3modelo.Angpder=0.0f;
	player3modelo.Xtor=0.0f;
	player3modelo.Ytor=0.0f;
	player3modelo.Ztor=0.0f;
}

void DatosAnimacionBob()
{
	//Secuencia 1 Bob
	KeyFrame1[0].Angt1=-5.0f;
	KeyFrame1[0].Angt2=0.0f;
	KeyFrame1[0].Angc1=-25.0f;
	KeyFrame1[0].Angc2=0.0f;
	KeyFrame1[0].Angbi1=-100.0f;
	KeyFrame1[0].Angbi2=0.0f;
	KeyFrame1[0].Angbib=-30.0f;
	KeyFrame1[0].Angbd1=120.0f;
	KeyFrame1[0].Angbd2=0.0f;
	KeyFrame1[0].Angbdb=-80.0f;
	KeyFrame1[0].Angpizq=40.0f;
	KeyFrame1[0].Angpder=-60.0f;
	KeyFrame1[0].Angpizqb=30.0f;
	KeyFrame1[0].Angpderb=40.0f;
	KeyFrame1[0].Angpi=0.0f;
	KeyFrame1[0].Angpd=0.0f;
	KeyFrame1[0].Xtor=0.0f;
	KeyFrame1[0].Ytor=0.0f;
	KeyFrame1[0].Ztor=0.0f;

	KeyFrame1[1].Angt1=-5.0f;
	KeyFrame1[1].Angt2=0.0f;
	KeyFrame1[1].Angc1=-25.0f;
	KeyFrame1[1].Angc2=0.0f;
	KeyFrame1[1].Angbi1=120.0f;
	KeyFrame1[1].Angbi2=0.0f;
	KeyFrame1[1].Angbib=-80.0f;
	KeyFrame1[1].Angbd1=-100.0f;
	KeyFrame1[1].Angbd2=0.0f;
	KeyFrame1[1].Angbdb=-30.0f;
	KeyFrame1[1].Angpizq=-60.0f;
	KeyFrame1[1].Angpder=40.0f;
	KeyFrame1[1].Angpizqb=40.0f;
	KeyFrame1[1].Angpderb=30.0f;
	KeyFrame1[1].Angpi=0.0f;
	KeyFrame1[1].Angpd=0.0f;
	KeyFrame1[1].Xtor=0.0f;
	KeyFrame1[1].Ytor=0.0f;
	KeyFrame1[1].Ztor=0.0f;

	KeyFrame1[2].Angt1=-5.0f;
	KeyFrame1[2].Angt2=0.0f;
	KeyFrame1[2].Angc1=-25.0f;
	KeyFrame1[2].Angc2=0.0f;
	KeyFrame1[2].Angbi1=-100.0f;
	KeyFrame1[2].Angbi2=0.0f;
	KeyFrame1[2].Angbib=-30.0f;
	KeyFrame1[2].Angbd1=120.0f;
	KeyFrame1[2].Angbd2=0.0f;
	KeyFrame1[2].Angbdb=-80.0f;
	KeyFrame1[2].Angpizq=40.0f;
	KeyFrame1[2].Angpder=-40.0f;
	KeyFrame1[2].Angpizqb=30.0f;
	KeyFrame1[2].Angpderb=20.0f;
	KeyFrame1[2].Angpi=0.0f;
	KeyFrame1[2].Angpd=0.0f;
	KeyFrame1[2].Xtor=0.0f;
	KeyFrame1[2].Ytor=0.0f;
	KeyFrame1[2].Ztor=0.0f;
}	
void DatosAnimacionPat()
{
	//Secuencia 1 Pat
	KeyFrame2[0].Angt1=0.0f;
	KeyFrame2[0].Angt2=0.0f;
	KeyFrame2[0].Angbi1=-50.0f;
	KeyFrame2[0].Angbd1=15.0f;
	KeyFrame2[0].Angpizq=15.0f;
	KeyFrame2[0].Angpder=-15.0f;
	KeyFrame2[0].Xtor=0.0f;
	KeyFrame2[0].Ytor=0.0f;
	KeyFrame2[0].Ztor=0.0f;

	KeyFrame2[1].Angt1=0.0f;
	KeyFrame2[1].Angt2=0.0f;
	KeyFrame2[1].Angbi1=13.0f;
	KeyFrame2[1].Angbd1=-48.0f;
	KeyFrame2[1].Angpizq=-15.0f;
	KeyFrame2[1].Angpder=15.0f;
	KeyFrame2[1].Xtor=0.0f;
	KeyFrame2[1].Ytor=0.0f;
	KeyFrame2[1].Ztor=0.0f;

	KeyFrame2[2].Angt1=0.0f;
	KeyFrame2[2].Angt2=0.0f;
	KeyFrame2[2].Angbi1=-50.0f;
	KeyFrame2[2].Angbd1=15.0f;
	KeyFrame2[2].Angpizq=15.0f;
	KeyFrame2[2].Angpder=-15.0f;
	KeyFrame2[2].Xtor=0.0f;
	KeyFrame2[2].Ytor=0.0f;
	KeyFrame2[2].Ztor=0.0f;
}
void DatosAnimacionPlankton()
{
	//Secuencia 1 Plankton
	KeyFrame3[0].Angt1=0.0f;
	KeyFrame3[0].Angt2=0.0f;
	KeyFrame3[0].Angbi1=-15.0f;
	KeyFrame3[0].Angbd1=15.0f;
	KeyFrame3[0].Angpizq=15.0f;
	KeyFrame3[0].Angpder=-15.0f;
	KeyFrame3[0].Xtor=0.0f;
	KeyFrame3[0].Ytor=0.0f;
	KeyFrame3[0].Ztor=0.0f;

	KeyFrame3[1].Angt1=0.0f;
	KeyFrame3[1].Angt2=0.0f;
	KeyFrame3[1].Angbi1=15.0f;
	KeyFrame3[1].Angbd1=-15.0f;
	KeyFrame3[1].Angpizq=-15.0f;
	KeyFrame3[1].Angpder=15.0f;
	KeyFrame3[1].Xtor=0.0f;
	KeyFrame3[1].Ytor=0.0f;
	KeyFrame3[1].Ztor=0.0f;

	KeyFrame3[2].Angt1=0.0f;
	KeyFrame3[2].Angt2=0.0f;
	KeyFrame3[2].Angbi1=-15.0f;
	KeyFrame3[2].Angbd1=15.0f;
	KeyFrame3[2].Angpizq=15.0f;
	KeyFrame3[2].Angpder=-15.0f;
	KeyFrame3[2].Xtor=0.0f;
	KeyFrame3[2].Ytor=0.0f;
	KeyFrame3[2].Ztor=0.0f;
}
int InitGL(GLvoid)										// Aqui se configuran los parametros iniciales de OpenGL
{
	Multitext.InitMultitext(hWnd);

	glShadeModel(GL_SMOOTH);							// Activa Smooth Shading
	glClearColor(0.4f, 0.6f, 0.8f, 0.5f);				// Fondo azul
	glClearDepth(1.0f);									// Valor para el Depth Buffer
	glClearStencil(0);
	glEnable(GL_DEPTH_TEST);							// Activa Depth Testing
	glDepthFunc(GL_LEQUAL);								// Tipo de Depth Testing a usar
	glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);	// Correccion de cálculos de perspectiva

	glCullFace(GL_BACK);								// Set Culling Face To Back Face
	glEnable(GL_CULL_FACE);								// Enable Culling

	//Iluminación
	//Inicializa la luz
	glLightfv(GL_LIGHT0, GL_POSITION, LightPos);		// Posicion de la luz1
	glLightfv(GL_LIGHT0, GL_AMBIENT,  LightAmb);		// Componente ambiente
	glLightfv(GL_LIGHT0, GL_DIFFUSE,  LightDif);		// Componente difusa
	glLightfv(GL_LIGHT0, GL_SPECULAR, LightSpc);		// Componente especular

	glLightfv(GL_LIGHT1, GL_AMBIENT, LightAmb1);
	glLightfv(GL_LIGHT1, GL_DIFFUSE, LightDif1);
	glLightfv(GL_LIGHT1, GL_SPECULAR, LightSpc1);
	glLightfv(GL_LIGHT1, GL_POSITION, LightPos1);

	// Atenuacion
	glLightf(GL_LIGHT1, GL_CONSTANT_ATTENUATION, 1.0f);
	glLightf(GL_LIGHT1, GL_LINEAR_ATTENUATION, 0.0f);
	glLightf(GL_LIGHT1, GL_QUADRATIC_ATTENUATION, 0.0f);

	glLightf(GL_LIGHT1, GL_SPOT_CUTOFF, 90.0f );
	
	glEnable(GL_LIGHT0);								// Activa luz0
	glEnable( GL_LIGHT1 );
	glEnable(GL_LIGHTING);								// Habilita la iluminación

	// para la Spline de la trayectoria automática
	ptsXtramo = 20;
	running = 1;
	trayectoria = 1;
	esperar=0;

	spline_init(luzspline, luzsplinepoints, totalCP, ptsXtramo );
	spline_init(luzspline2, luzsplinepoints2, totalCP, ptsXtramo );
	spline_init(luzspline3, luzsplinepoints3, totalCP, ptsXtramo );

	jsel = 0;
	isel = 0;

	idxtp = 2;
	dtidx = 1;

	CargaTexturas();

	CargaModelos();
	CreaListas();
	IniSombraVolumen();

	Font.BuildFont();

	InitGLSL();
	
	e=gluNewQuadric();

	InicializaParametrosdeControl();
	InicializaAnim();
	DatosAnimacionBob();
	DatosAnimacionPat();
	DatosAnimacionPlankton();

	InicializaObjetosdeColision();

	// Shaders
	if( InitGLSL() )
	{
		cel.InitShaders( "Shaders/celshader.vert", "Shaders/celshader.frag" );
		spot.InitShaders( "Shaders/spotlight.vert", "Shaders/spotlight.frag");
	}

	return TRUE;										
}

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
    result = FMOD_System_Close(system);
    ERRCHECK(result);
    result = FMOD_System_Release(system);
    ERRCHECK(result);
}

void ControlPersonaje(int funcion)
{
	if(funcion == 1) //Giro a la derecha
	{
		player1.AngDir+=1.0f;
		if(player1.AngDir > 360.0f)
			player1.AngDir-=360.0f;

		player1.AngObj-=1.0f;
		if(player1.AngObj < 0.0f)
			player1.AngObj+=360.0f;

		player1.Direccion.x=(float)cos(player1.AngDir*PI/180.0f);
		player1.Direccion.y=0.0f;
		player1.Direccion.z=(float)sin(player1.AngDir*PI/180.0f);

		player1.PosicionCam=player1.PosicionObj-player1.Direccion*player1.DistanciaCam;
		player1.PosicionCam.y=player1.CamaraPosAlt;
		player1.ObjetivoCam=player1.PosicionObj;
		player1.ObjetivoCam.y=player1.CamaraObjAlt;
	}
	else if(funcion == 2) //Giro a la izquierda
	{
		player1.AngDir-=1.0f;
		if(player1.AngDir < 0.0f)
			player1.AngDir+=360.0f;

		player1.AngObj+=1.0f;
		if(player1.AngObj > 360.0f)
			player1.AngObj-=360.0f;

		player1.Direccion.x=(float)cos(player1.AngDir*PI/180.0f);
		player1.Direccion.y=0.0f;
		player1.Direccion.z=(float)sin(player1.AngDir*PI/180.0f);

		player1.PosicionCam=player1.PosicionObj-player1.Direccion*player1.DistanciaCam;
		player1.PosicionCam.y=player1.CamaraPosAlt;
		player1.ObjetivoCam=player1.PosicionObj;
		player1.ObjetivoCam.y=player1.CamaraObjAlt;
	}
	else if(funcion == 3) //Avanza hacia adelante
	{
		player1.PosicionObj=player1.PosicionObj+player1.Direccion*player1.VelocidadObj;
		player1.PosicionCam=player1.PosicionObj-player1.Direccion*player1.DistanciaCam;
		player1.PosicionCam.y=player1.CamaraPosAlt;
		player1.ObjetivoCam=player1.PosicionObj;
		player1.ObjetivoCam.y=player1.CamaraObjAlt;

		// Colisiones
		ColisionEsferaEsfera(esfera[0], esfera[1], 1, player1 );
		ColisionEsferaEsfera(esfera[0], esfera[2], 1, player1 );
		ColisionEsferaPlano(0, 1, player1 );

		player1.PosAntObj=player1.PosicionObj;


	}
	else if(funcion == 4) //Avanza hacia atrás
	{
		player1.PosicionObj=player1.PosicionObj-player1.Direccion*player1.VelocidadObj;
		player1.PosicionCam=player1.PosicionObj-player1.Direccion*player1.DistanciaCam;
		player1.PosicionCam.y=player1.CamaraPosAlt;
		player1.ObjetivoCam=player1.PosicionObj;
		player1.ObjetivoCam.y=player1.CamaraObjAlt;

		// Colisiones
		ColisionEsferaEsfera(esfera[0], esfera[1], 2, player1 );
		ColisionEsferaEsfera(esfera[0], esfera[2], 2, player1 );
		ColisionEsferaPlano(0, 2, player1 );

		player1.PosAntObj=player1.PosicionObj;

	}
	else if(funcion == 5) //Sube objetivo de la cámara
	{
		player1.ObjetivoCam.y+=1.0f;
		player1.CamaraObjAlt+=1.0f;
		player1.CamaraObjAltE+=1.0f;
		
	}
	else if(funcion == 6) //Baja objetivo de la cámara
	{
		player1.ObjetivoCam.y-=1.0f;
		player1.CamaraObjAlt-=1.0f;
		player1.CamaraObjAltE-=1.0f;
		
	}
	else if(funcion == 7) //Giro a la derecha
	{
		player2.AngDir+=1.0f;
		if(player2.AngDir > 360.0f)
			player2.AngDir-=360.0f;

		player2.AngObj-=1.0f;
		if(player2.AngObj < 0.0f)
			player2.AngObj+=360.0f;

		player2.Direccion.x=(float)cos(player2.AngDir*PI/180.0f);
		player2.Direccion.y=0.0f;
		player2.Direccion.z=(float)sin(player2.AngDir*PI/180.0f);

		player2.PosicionCam=player2.PosicionObj-player2.Direccion*player2.DistanciaCam;
		player2.PosicionCam.y=player2.CamaraPosAlt;
		player2.ObjetivoCam=player2.PosicionObj;
		player2.ObjetivoCam.y=player2.CamaraObjAlt;
	}
	else if(funcion == 8) //Giro a la izquierda
	{
		player2.AngDir-=1.0f;
		if(player2.AngDir < 0.0f)
			player2.AngDir+=360.0f;

		player2.AngObj+=1.0f;
		if(player2.AngObj > 360.0f)
			player2.AngObj-=360.0f;

		player2.Direccion.x=(float)cos(player2.AngDir*PI/180.0f);
		player2.Direccion.y=0.0f;
		player2.Direccion.z=(float)sin(player2.AngDir*PI/180.0f);

		player2.PosicionCam=player2.PosicionObj-player2.Direccion*player2.DistanciaCam;
		player2.PosicionCam.y=player2.CamaraPosAlt;
		player2.ObjetivoCam=player2.PosicionObj;
		player2.ObjetivoCam.y=player2.CamaraObjAlt;
	}
	else if(funcion == 9) //Avanza hacia adelante
	{
		player2.PosicionObj=player2.PosicionObj+player2.Direccion*player2.VelocidadObj;
		player2.PosicionCam=player2.PosicionObj-player2.Direccion*player2.DistanciaCam;
		player2.PosicionCam.y=player2.CamaraPosAlt;
		player2.ObjetivoCam=player2.PosicionObj;
		player2.ObjetivoCam.y=player2.CamaraObjAlt;

		// Colisiones
		ColisionEsferaEsfera(esfera[1], esfera[0], 1, player2 );
		ColisionEsferaEsfera(esfera[1], esfera[2], 1, player2 );
		ColisionEsferaPlano(1, 1, player2 );

		player2.PosAntObj=player2.PosicionObj;
	}
	else if(funcion == 10) //Avanza hacia atrás
	{
		player2.PosicionObj=player2.PosicionObj-player2.Direccion*player2.VelocidadObj;
		player2.PosicionCam=player2.PosicionObj-player2.Direccion*player2.DistanciaCam;
		player2.PosicionCam.y=player2.CamaraPosAlt;
		player2.ObjetivoCam=player2.PosicionObj;
		player2.ObjetivoCam.y=player2.CamaraObjAlt;

		// Colisiones
		ColisionEsferaEsfera(esfera[1], esfera[0], 2, player2 );
		ColisionEsferaEsfera(esfera[1], esfera[2], 2, player2 );
		ColisionEsferaPlano(1, 2, player2 );

		player2.PosAntObj=player2.PosicionObj;
	}
	else if(funcion == 11) //Giro a la derecha
	{
		player3.AngDir+=1.0f;
		if(player3.AngDir > 360.0f)
			player3.AngDir-=360.0f;

		player3.AngObj-=1.0f;
		if(player3.AngObj < 0.0f)
			player3.AngObj+=360.0f;

		player3.Direccion.x=(float)cos(player3.AngDir*PI/180.0f);
		player3.Direccion.y=0.0f;
		player3.Direccion.z=(float)sin(player3.AngDir*PI/180.0f);

		player3.PosicionCam=player3.PosicionObj-player3.Direccion*player3.DistanciaCam;
		player3.PosicionCam.y=player3.CamaraPosAlt;
		player3.ObjetivoCam=player3.PosicionObj;
		player3.ObjetivoCam.y=player3.CamaraObjAlt;
	}
	else if(funcion == 12) //Giro a la izquierda
	{
		player3.AngDir-=1.0f;
		if(player3.AngDir < 0.0f)
			player3.AngDir+=360.0f;

		player3.AngObj+=1.0f;
		if(player3.AngObj > 360.0f)
			player3.AngObj-=360.0f;

		player3.Direccion.x=(float)cos(player3.AngDir*PI/180.0f);
		player3.Direccion.y=0.0f;
		player3.Direccion.z=(float)sin(player3.AngDir*PI/180.0f);

		player3.PosicionCam=player3.PosicionObj-player3.Direccion*player3.DistanciaCam;
		player3.PosicionCam.y=player3.CamaraPosAlt;
		player3.ObjetivoCam=player3.PosicionObj;
		player3.ObjetivoCam.y=player3.CamaraObjAlt;
	}
	else if(funcion == 13) //Avanza hacia adelante
	{
		player3.PosicionObj=player3.PosicionObj+player3.Direccion*player3.VelocidadObj;
		player3.PosicionCam=player3.PosicionObj-player3.Direccion*player3.DistanciaCam;
		player3.PosicionCam.y=player3.CamaraPosAlt;
		player3.ObjetivoCam=player3.PosicionObj;
		player3.ObjetivoCam.y=player3.CamaraObjAlt;

		// Colisiones
		ColisionEsferaEsfera(esfera[2], esfera[0], 1, player3 );
		ColisionEsferaEsfera(esfera[2], esfera[1], 1, player3 );
		ColisionEsferaPlano(2, 1, player3 );

		player3.PosAntObj=player3.PosicionObj;

	}
	else if(funcion == 14) //Avanza hacia atrás
	{
		player3.PosicionObj=player3.PosicionObj-player3.Direccion*player3.VelocidadObj;
		player3.PosicionCam=player3.PosicionObj-player3.Direccion*player3.DistanciaCam;
		player3.PosicionCam.y=player3.CamaraPosAlt;
		player3.ObjetivoCam=player3.PosicionObj;
		player3.ObjetivoCam.y=player3.CamaraObjAlt;

		// Colisiones
		ColisionEsferaEsfera(esfera[2], esfera[0], 2, player3 );
		ColisionEsferaEsfera(esfera[2], esfera[1], 2, player3 );
		ColisionEsferaPlano(2, 2, player3 );

		player3.PosAntObj=player3.PosicionObj;
	}
}

void animacion(FRAME *KeyFrame, int maxKF , int frames)
{
	if(play)
	{	
		if((abs(KeyFrame[playIndex+1].Angt1-player1modelo.Angt1))<0.1 &&
		   (abs(KeyFrame[playIndex+1].Angt2-player1modelo.Angt2))<0.1 &&
		   (abs(KeyFrame[playIndex+1].Angc1-player1modelo.Angc1))<0.1 &&
		   (abs(KeyFrame[playIndex+1].Angc2-player1modelo.Angc2))<0.1 &&
		   (abs(KeyFrame[playIndex+1].Angbi1-player1modelo.Angbi1))<0.1 &&
		   (abs(KeyFrame[playIndex+1].Angbi2-player1modelo.Angbi2))<0.1 &&
		   (abs(KeyFrame[playIndex+1].Angbib-player1modelo.Angbib))<0.1 &&
		   (abs(KeyFrame[playIndex+1].Angbd1-player1modelo.Angbd1))<0.1 &&
		   (abs(KeyFrame[playIndex+1].Angbd2-player1modelo.Angbd2))<0.1 &&
		   (abs(KeyFrame[playIndex+1].Angbdb-player1modelo.Angbdb))<0.1 &&
		   (abs(KeyFrame[playIndex+1].Angpizq-player1modelo.Angpizq))<0.1 &&
		   (abs(KeyFrame[playIndex+1].Angpizqb-player1modelo.Angpizqb))<0.1 &&
		   (abs(KeyFrame[playIndex+1].Angpder-player1modelo.Angpder))<0.1 &&
		   (abs(KeyFrame[playIndex+1].Angpderb-player1modelo.Angpderb))<0.1 &&
		   (abs(KeyFrame[playIndex+1].Angpi-player1modelo.Angpi))<0.1 &&
		   (abs(KeyFrame[playIndex+1].Angpd-player1modelo.Angpd))<0.1 &&
		   (abs(KeyFrame[playIndex+1].Xtor-player1modelo.Xtor))<0.1 &&
		   (abs(KeyFrame[playIndex+1].Ytor-player1modelo.Ytor))<0.1 &&
		   (abs(KeyFrame[playIndex+1].Ztor-player1modelo.Ztor))<0.1 )
		{			
			playIndex++;			
			if(playIndex>maxKF-2)
			{
				playIndex=0;
				play=false;
								
			}
		}
		else
		{
			KeyFrame[playIndex].incAngt1    = (KeyFrame[playIndex+1].Angt1-KeyFrame[playIndex].Angt1)/frames;
			KeyFrame[playIndex].incAngt2    = (KeyFrame[playIndex+1].Angt2-KeyFrame[playIndex].Angt2)/frames;
			KeyFrame[playIndex].incAngc1    = (KeyFrame[playIndex+1].Angc1-KeyFrame[playIndex].Angc1)/frames;
			KeyFrame[playIndex].incAngc2    = (KeyFrame[playIndex+1].Angc2-KeyFrame[playIndex].Angc2)/frames;
			KeyFrame[playIndex].incAngbi1   = (KeyFrame[playIndex+1].Angbi1-KeyFrame[playIndex].Angbi1)/frames;
			KeyFrame[playIndex].incAngbi2   = (KeyFrame[playIndex+1].Angbi2-KeyFrame[playIndex].Angbi2)/frames;
			KeyFrame[playIndex].incAngbib   = (KeyFrame[playIndex+1].Angbib-KeyFrame[playIndex].Angbib)/frames;
			KeyFrame[playIndex].incAngbd1   = (KeyFrame[playIndex+1].Angbd1-KeyFrame[playIndex].Angbd1)/frames;
			KeyFrame[playIndex].incAngbd2   = (KeyFrame[playIndex+1].Angbd2-KeyFrame[playIndex].Angbd2)/frames;
			KeyFrame[playIndex].incAngbdb   = (KeyFrame[playIndex+1].Angbdb-KeyFrame[playIndex].Angbdb)/frames;
			KeyFrame[playIndex].incAngpizq  = (KeyFrame[playIndex+1].Angpizq-KeyFrame[playIndex].Angpizq)/frames;
			KeyFrame[playIndex].incAngpizqb = (KeyFrame[playIndex+1].Angpizqb-KeyFrame[playIndex].Angpizqb)/frames;
			KeyFrame[playIndex].incAngpder  = (KeyFrame[playIndex+1].Angpder-KeyFrame[playIndex].Angpder)/frames;
			KeyFrame[playIndex].incAngpderb = (KeyFrame[playIndex+1].Angpderb-KeyFrame[playIndex].Angpderb)/frames;
			KeyFrame[playIndex].incAngpi    = (KeyFrame[playIndex+1].Angpi-KeyFrame[playIndex].Angpi)/frames;
			KeyFrame[playIndex].incAngpd    = (KeyFrame[playIndex+1].Angpd-KeyFrame[playIndex].Angpd)/frames;
			KeyFrame[playIndex].incXtor     = (KeyFrame[playIndex+1].Xtor-KeyFrame[playIndex].Xtor)/frames;
			KeyFrame[playIndex].incYtor     = (KeyFrame[playIndex+1].Ytor-KeyFrame[playIndex].Ytor)/frames;
			KeyFrame[playIndex].incZtor     = (KeyFrame[playIndex+1].Ztor-KeyFrame[playIndex].Ztor)/frames;

			player1modelo.Angt1    += KeyFrame[playIndex].incAngt1;
			player1modelo.Angt2    += KeyFrame[playIndex].incAngt2;
			player1modelo.Angc1    += KeyFrame[playIndex].incAngc1;
			player1modelo.Angc2    += KeyFrame[playIndex].incAngc2;
			player1modelo.Angbi1   += KeyFrame[playIndex].incAngbi1;
			player1modelo.Angbi2   += KeyFrame[playIndex].incAngbi2;
			player1modelo.Angbib   += KeyFrame[playIndex].incAngbib;
			player1modelo.Angbd1   += KeyFrame[playIndex].incAngbd1;
			player1modelo.Angbd2   += KeyFrame[playIndex].incAngbd2;
			player1modelo.Angbdb   += KeyFrame[playIndex].incAngbdb;
			player1modelo.Angpizq  += KeyFrame[playIndex].incAngpizq;
			player1modelo.Angpizqb += KeyFrame[playIndex].incAngpizqb;
			player1modelo.Angpder  += KeyFrame[playIndex].incAngpder;
			player1modelo.Angpderb += KeyFrame[playIndex].incAngpderb;
			player1modelo.Angpi    += KeyFrame[playIndex].incAngpi;
			player1modelo.Angpd    += KeyFrame[playIndex].incAngpd;
			player1modelo.Xtor     += KeyFrame[playIndex].incXtor;
			player1modelo.Ytor     += KeyFrame[playIndex].incYtor;
			player1modelo.Ztor     += KeyFrame[playIndex].incZtor;
		}
	}	
}

void animacionPat(FRAME *KeyFrame, int maxKF , int frames)
{
	if(play1)
	{
		if((abs(KeyFrame[playIndex1+1].Angt1-player2modelo.Angt1))<0.1 &&
		   (abs(KeyFrame[playIndex1+1].Angt2-player2modelo.Angt2))<0.1 &&
		   (abs(KeyFrame[playIndex1+1].Angbi1-player2modelo.Angbi1))<0.1 &&
		   (abs(KeyFrame[playIndex1+1].Angbi2-player2modelo.Angbi2))<0.1 &&
		   (abs(KeyFrame[playIndex1+1].Angbd1-player2modelo.Angbd1))<0.1 &&
		   (abs(KeyFrame[playIndex1+1].Angbd2-player2modelo.Angbd2))<0.1 &&
		   (abs(KeyFrame[playIndex1+1].Angpizq-player2modelo.Angpizq))<0.1 &&
		   (abs(KeyFrame[playIndex1+1].Angpder-player2modelo.Angpder))<0.1 &&
		   (abs(KeyFrame[playIndex1+1].Xtor-player2modelo.Xtor))<0.1 &&
		   (abs(KeyFrame[playIndex1+1].Ytor-player2modelo.Ytor))<0.1 &&
		   (abs(KeyFrame[playIndex1+1].Ztor-player2modelo.Ztor))<0.1)			
		{			
			playIndex1++;			
			if(playIndex1>maxKF-2)
			{
				playIndex1=0;
				play1=false;
								
			}
		}
		else
		{
			KeyFrame[playIndex1].incAngt1    = (KeyFrame[playIndex1+1].Angt1-KeyFrame[playIndex1].Angt1)/frames;
			KeyFrame[playIndex1].incAngt2    = (KeyFrame[playIndex1+1].Angt2-KeyFrame[playIndex1].Angt2)/frames;
			KeyFrame[playIndex1].incAngbi1   = (KeyFrame[playIndex1+1].Angbi1-KeyFrame[playIndex1].Angbi1)/frames;
			KeyFrame[playIndex1].incAngbi2   = (KeyFrame[playIndex1+1].Angbi2-KeyFrame[playIndex1].Angbi2)/frames;
			KeyFrame[playIndex1].incAngbd1   = (KeyFrame[playIndex1+1].Angbd1-KeyFrame[playIndex1].Angbd1)/frames;
			KeyFrame[playIndex1].incAngbd2   = (KeyFrame[playIndex1+1].Angbd2-KeyFrame[playIndex1].Angbd2)/frames;
			KeyFrame[playIndex1].incAngpizq  = (KeyFrame[playIndex1+1].Angpizq-KeyFrame[playIndex1].Angpizq)/frames;
			KeyFrame[playIndex1].incAngpder  = (KeyFrame[playIndex1+1].Angpder-KeyFrame[playIndex1].Angpder)/frames;
			KeyFrame[playIndex1].incXtor     = (KeyFrame[playIndex1+1].Xtor-KeyFrame[playIndex1].Xtor)/frames;
			KeyFrame[playIndex1].incYtor     = (KeyFrame[playIndex1+1].Ytor-KeyFrame[playIndex1].Ytor)/frames;
			KeyFrame[playIndex1].incZtor     = (KeyFrame[playIndex1+1].Ztor-KeyFrame[playIndex1].Ztor)/frames;

			player2modelo.Angt1    += KeyFrame[playIndex1].incAngt1;
			player2modelo.Angt2    += KeyFrame[playIndex1].incAngt2;
			player2modelo.Angbi1   += KeyFrame[playIndex1].incAngbi1;
			player2modelo.Angbi2   += KeyFrame[playIndex1].incAngbi2;
			player2modelo.Angbd1   += KeyFrame[playIndex1].incAngbd1;
			player2modelo.Angbd2   += KeyFrame[playIndex1].incAngbd2;
			player2modelo.Angpizq  += KeyFrame[playIndex1].incAngpizq;
			player2modelo.Angpder  += KeyFrame[playIndex1].incAngpder;
			player2modelo.Xtor     += KeyFrame[playIndex1].incXtor;
			player2modelo.Ytor     += KeyFrame[playIndex1].incYtor;
			player2modelo.Ztor     += KeyFrame[playIndex1].incZtor;
		}
	}
}
void animacionPlankton(FRAME *KeyFrame, int maxKF , int frames)
{
	if(play2)
	{
		if((abs(KeyFrame[playIndex2+1].Angt1-player3modelo.Angt1))<0.1 &&
		   (abs(KeyFrame[playIndex2+1].Angt2-player3modelo.Angt2))<0.1 &&
		   (abs(KeyFrame[playIndex2+1].Angbi1-player3modelo.Angbi1))<0.1 &&
		   (abs(KeyFrame[playIndex2+1].Angbd1-player3modelo.Angbd1))<0.1 &&
		   (abs(KeyFrame[playIndex2+1].Angpizq-player3modelo.Angpizq))<0.1 &&
		   (abs(KeyFrame[playIndex2+1].Angpder-player3modelo.Angpder))<0.1 &&
		   (abs(KeyFrame[playIndex2+1].Xtor-player3modelo.Xtor))<0.1 &&
		   (abs(KeyFrame[playIndex2+1].Ytor-player3modelo.Ytor))<0.1 &&
		   (abs(KeyFrame[playIndex2+1].Ztor-player3modelo.Ztor))<0.1)			
		{			
			playIndex2++;			
			if(playIndex2>maxKF-2)
			{
				playIndex2=0;
				play2=false;
								
			}
		}
		else
		{
			KeyFrame[playIndex2].incAngt1    = (KeyFrame[playIndex2+1].Angt1-KeyFrame[playIndex2].Angt1)/frames;
			KeyFrame[playIndex2].incAngt2    = (KeyFrame[playIndex2+1].Angt2-KeyFrame[playIndex2].Angt2)/frames;
			KeyFrame[playIndex2].incAngbi1   = (KeyFrame[playIndex2+1].Angbi1-KeyFrame[playIndex2].Angbi1)/frames;
			KeyFrame[playIndex2].incAngbd1   = (KeyFrame[playIndex2+1].Angbd1-KeyFrame[playIndex2].Angbd1)/frames;
			KeyFrame[playIndex2].incAngpizq  = (KeyFrame[playIndex2+1].Angpizq-KeyFrame[playIndex2].Angpizq)/frames;
			KeyFrame[playIndex2].incAngpder  = (KeyFrame[playIndex2+1].Angpder-KeyFrame[playIndex2].Angpder)/frames;
			KeyFrame[playIndex2].incXtor     = (KeyFrame[playIndex2+1].Xtor-KeyFrame[playIndex2].Xtor)/frames;
			KeyFrame[playIndex2].incYtor     = (KeyFrame[playIndex2+1].Ytor-KeyFrame[playIndex2].Ytor)/frames;
			KeyFrame[playIndex2].incZtor     = (KeyFrame[playIndex2+1].Ztor-KeyFrame[playIndex2].Ztor)/frames;

			player3modelo.Angt1    += KeyFrame[playIndex2].incAngt1;
			player3modelo.Angt2    += KeyFrame[playIndex2].incAngt2;
			player3modelo.Angbi1   += KeyFrame[playIndex2].incAngbi1;
			player3modelo.Angbd1   += KeyFrame[playIndex2].incAngbd1;
			player3modelo.Angpizq  += KeyFrame[playIndex2].incAngpizq;
			player3modelo.Angpder  += KeyFrame[playIndex2].incAngpder;
			player3modelo.Xtor     += KeyFrame[playIndex2].incXtor;
			player3modelo.Ytor     += KeyFrame[playIndex2].incYtor;
			player3modelo.Ztor     += KeyFrame[playIndex2].incZtor;
		}
	}
}
void DibujaBob()
{
	glTranslatef(player1modelo.Xtor, player1modelo.Ytor, player1modelo.Ztor);
	glRotatef(player1modelo.Angt2, 0.0f, 1.0f, 0.0f);
	glRotatef(player1modelo.Angt1, 1.0f, 0.0f, 0.0f);
			
	//Torso
	glCallList(modelo1+0);
	
	//Pierna derecha
	glPushMatrix();
		glTranslatef(-1.2f, -1.3f ,0.0f);
		glRotatef(player1modelo.Angpder, 1.0f, 0.0f, 0.0f);
		glCallList(modelo1+1);
		
		//Pierna derecha_b
		glPushMatrix();
			glTranslatef(0.0f, -1.25f , 0.0f);
			glRotatef(player1modelo.Angpderb, 1.0f, 0.0f, 0.0f);
			glCallList(modelo1+2);
		glPopMatrix();

	glPopMatrix();

	//Pierna izquierda
	glPushMatrix();
		glTranslatef(1.2f, -1.3f ,0.0f);
		glRotatef(player1modelo.Angpizq, 1.0f, 0.0f, 0.0f);
		glCallList(modelo1+3);

		//Pierna izquierda_b
		glPushMatrix();
			glTranslatef(0.0f, -1.25f , 0.0f);
			glRotatef(player1modelo.Angpizqb, 1.0f, 0.0f, 0.0f);
			glCallList(modelo1+4);
		glPopMatrix();

	glPopMatrix();

	//Brazo derecho_a
	glPushMatrix();
		glTranslatef(-2.8f, 1.1f, 0.0f);
		glRotatef(player1modelo.Angbd2, 0.0f, 1.0f, 0.0f);
		glRotatef(player1modelo.Angbd1, 1.0f, 0.0f, 0.0f);
		glCallList(modelo1+5);

		//Brazo derecho_b
		glPushMatrix();
			glTranslatef(-0.35f, -1.5f, 0.0f);
			glRotatef(player1modelo.Angbdb, 1.0f, 0.0f, 0.0f);
			glCallList(modelo1+7);
		glPopMatrix();

	glPopMatrix();

	//Brazo izquierdo
	glPushMatrix();
		glTranslatef(2.8f, 1.1f, 0.0f);
		glRotatef(player1modelo.Angbi2, 0.0f, 1.0f, 0.0f);
		glRotatef(player1modelo.Angbi1, 1.0f, 0.0f, 0.0f);
		glCallList(modelo1+6);

		//Brazo izquierdo_b
		glPushMatrix();
			glTranslatef(0.35f, -1.5f, 0.0f);
			glRotatef(player1modelo.Angbib, 1.0f, 0.0f, 0.0f);
			glCallList(modelo1+8);
		glPopMatrix();

	glPopMatrix();
}

void DibujaWiredBob()
{
	glTranslatef(player1modelo.Xtor, player1modelo.Ytor, player1modelo.Ztor);
	glRotatef(player1modelo.Angt2, 0.0f, 1.0f, 0.0f);
	glRotatef(player1modelo.Angt1, 1.0f, 0.0f, 0.0f);
			
	//Torso
	glCallList(modelo1+10);
	
	//Pierna derecha
	glPushMatrix();
		glTranslatef(-1.2f, -1.3f ,0.0f);
		glRotatef(player1modelo.Angpder, 1.0f, 0.0f, 0.0f);
		glCallList(modelo1+11);
		
		//Pierna derecha_b
		glPushMatrix();
			glTranslatef(0.0f, -1.25f , 0.0f);
			glRotatef(player1modelo.Angpderb, 1.0f, 0.0f, 0.0f);
			glCallList(modelo1+12);
		glPopMatrix();

	glPopMatrix();

	//Pierna izquierda
	glPushMatrix();
		glTranslatef(1.2f, -1.3f ,0.0f);
		glRotatef(player1modelo.Angpizq, 1.0f, 0.0f, 0.0f);
		glCallList(modelo1+13);

		//Pierna izquierda_b
		glPushMatrix();
			glTranslatef(0.0f, -1.25f , 0.0f);
			glRotatef(player1modelo.Angpizqb, 1.0f, 0.0f, 0.0f);
			glCallList(modelo1+14);
		glPopMatrix();

	glPopMatrix();

	//Brazo derecho_a
	glPushMatrix();
		glTranslatef(-2.8f, 1.1f, 0.0f);
		glRotatef(player1modelo.Angbd2, 0.0f, 1.0f, 0.0f);
		glRotatef(player1modelo.Angbd1, 1.0f, 0.0f, 0.0f);
		glCallList(modelo1+15);

		//Brazo derecho_b
		glPushMatrix();
			glTranslatef(-0.35f, -1.5f, 0.0f);
			glRotatef(player1modelo.Angbdb, 1.0f, 0.0f, 0.0f);
			glCallList(modelo1+17);
		glPopMatrix();

	glPopMatrix();

	//Brazo izquierdo
	glPushMatrix();
		glTranslatef(2.8f, 1.1f, 0.0f);
		glRotatef(player1modelo.Angbi2, 0.0f, 1.0f, 0.0f);
		glRotatef(player1modelo.Angbi1, 1.0f, 0.0f, 0.0f);
		glCallList(modelo1+16);

		//Brazo izquierdo_b
		glPushMatrix();
			glTranslatef(0.35f, -1.5f, 0.0f);
			glRotatef(player1modelo.Angbib, 1.0f, 0.0f, 0.0f);
			glCallList(modelo1+18);
		glPopMatrix();

	glPopMatrix();
}

void DibujaPat()
{
	glTranslatef(player2modelo.Xtor, player2modelo.Ytor, player2modelo.Ztor);
	glRotatef(player2modelo.Angt2, 0.0f, 1.0f, 0.0f);
	glRotatef(player2modelo.Angt1, 1.0f, 0.0f, 0.0f);
	//Torso
	glCallList(modelo2+0);
	
	//Pierna derecha
	glPushMatrix();
		glRotatef(player2modelo.Angpder, 1.0f, 0.0f, 0.0f);
		glCallList(modelo2+1);
	glPopMatrix();

	//Pierna izquierda
	glPushMatrix();
		glRotatef(player2modelo.Angpizq, 1.0f, 0.0f, 0.0f);
		glCallList(modelo2+2);
	glPopMatrix();

	//Brazo derecho
	glPushMatrix();
		glTranslatef( 0.0f, 1.3f, 0.0f );
		glRotatef(player2modelo.Angbd1, 1.0f, 0.0f, 0.0f);
		glTranslatef( 0.0f, -1.3f, 0.0f );
		glCallList(modelo2+3);
	glPopMatrix();

	//Brazo izquierdo
	glPushMatrix();
		glTranslatef( 0.0f, 1.3f, 0.0f );
		glRotatef(player2modelo.Angbi1, 1.0f, 0.0f, 0.0f);
		glTranslatef( 0.0f, -1.3f, 0.0f );
		glCallList(modelo2+4);
	glPopMatrix();
}
void DibujaWiredPat()
{
	glTranslatef(player2modelo.Xtor, player2modelo.Ytor, player2modelo.Ztor);
	glRotatef(player2modelo.Angt2, 0.0f, 1.0f, 0.0f);
	glRotatef(player2modelo.Angt1, 1.0f, 0.0f, 0.0f);
	//Torso
	glCallList(modelo2+5);
	
	//Pierna derecha
	glPushMatrix();
		glRotatef(player2modelo.Angpder, 1.0f, 0.0f, 0.0f);
		glCallList(modelo2+6);
	glPopMatrix();

	//Pierna izquierda
	glPushMatrix();
		glRotatef(player2modelo.Angpizq, 1.0f, 0.0f, 0.0f);
		glCallList(modelo2+7);
	glPopMatrix();

	//Brazo derecho
	glPushMatrix();
		glTranslatef( 0.0f, 1.3f, 0.0f );
		glRotatef(player2modelo.Angbd1, 1.0f, 0.0f, 0.0f);
		glTranslatef( 0.0f, -1.3f, 0.0f );
		glCallList(modelo2+8);
	glPopMatrix();

	//Brazo izquierdo
	glPushMatrix();
		glTranslatef( 0.0f, 1.3f, 0.0f );
		glRotatef(player2modelo.Angbi1, 1.0f, 0.0f, 0.0f);
		glTranslatef( 0.0f, -1.3f, 0.0f );
		glCallList(modelo2+9);
	glPopMatrix();
}
void DibujaPlankton()
{
		glTranslatef(player3modelo.Xtor, player3modelo.Ytor, player3modelo.Ztor);
		glRotatef(player3modelo.Angt2, 0.0f, 1.0f, 0.0f);
		glRotatef(player3modelo.Angt1, 1.0f, 0.0f, 0.0f);
		//Torso
		glCallList(modelo3+0);
		
		//Pierna derecha
		glPushMatrix();
			glTranslatef( 0.0f, 1.5f, 0.0f );
			glRotatef(player3modelo.Angpder, 1.0f, 0.0f, 0.0f);
			glTranslatef( 0.0f, -1.5f, 0.0f );
			glCallList(modelo3+1);
		glPopMatrix();

		//Pierna izquierda
		glPushMatrix();
			glTranslatef( 0.0f, 1.5f, 0.0f );
			glRotatef(player3modelo.Angpizq, 1.0f, 0.0f, 0.0f);
			glTranslatef( 0.0f, -1.5f, 0.0f );
			glCallList(modelo3+2);
		glPopMatrix();

		//Brazo derecho
		glPushMatrix();
			glTranslatef( 0.0f, 5.5f, 0.0f );
			glRotatef(player3modelo.Angbd1, 1.0f, 0.0f, 0.0f);
			glTranslatef( 0.0f, -5.5f, 0.0f );
			glCallList(modelo3+3);
		glPopMatrix();

		//Brazo izquierdo
		glPushMatrix();
			glTranslatef( 0.0f, 5.5f, 0.0f );
			glRotatef(player3modelo.Angbi1, 1.0f, 0.0f, 0.0f);
			glTranslatef( 0.0f, -5.5f, 0.0f );
			glCallList(modelo3+4);
		glPopMatrix();
}
void DibujaWiredPlankton()
{
		glTranslatef(player3modelo.Xtor, player3modelo.Ytor, player3modelo.Ztor);
		glRotatef(player3modelo.Angt2, 0.0f, 1.0f, 0.0f);
		glRotatef(player3modelo.Angt1, 1.0f, 0.0f, 0.0f);
		//Torso
		glCallList(modelo3+5);
		
		//Pierna derecha
		glPushMatrix();
			glTranslatef( 0.0f, 1.5f, 0.0f );
			glRotatef(player3modelo.Angpder, 1.0f, 0.0f, 0.0f);
			glTranslatef( 0.0f, -1.5f, 0.0f );
			glCallList(modelo3+6);
		glPopMatrix();

		//Pierna izquierda
		glPushMatrix();
			glTranslatef( 0.0f, 1.5f, 0.0f );
			glRotatef(player3modelo.Angpizq, 1.0f, 0.0f, 0.0f);
			glTranslatef( 0.0f, -1.5f, 0.0f );
			glCallList(modelo3+7);
		glPopMatrix();

		//Brazo derecho
		glPushMatrix();
			glTranslatef( 0.0f, 5.5f, 0.0f );
			glRotatef(player3modelo.Angbd1, 1.0f, 0.0f, 0.0f);
			glTranslatef( 0.0f, -5.5f, 0.0f );
			glCallList(modelo3+8);
		glPopMatrix();

		//Brazo izquierdo
		glPushMatrix();
			glTranslatef( 0.0f, 5.5f, 0.0f );
			glRotatef(player3modelo.Angbi1, 1.0f, 0.0f, 0.0f);
			glTranslatef( 0.0f, -5.5f, 0.0f );
			glCallList(modelo3+9);
		glPopMatrix();
}
void DibujaPez()
{
	//torso
	glCallList(modelo4+0);

	//Pierna Izquierda
	glPushMatrix();
		glTranslatef( 0.5f, -0.5f, -0.25f );
		glCallList(modelo4+1);
	glPopMatrix();

	//Pierna Derecha
	glPushMatrix();
		glTranslatef( -0.5f, -0.5f, -0.25f );
		glCallList(modelo4+1);
	glPopMatrix();

	//Brazo Derecho
	glPushMatrix();
		glTranslatef( -1.3f, 1.0f, -0.25f );
		glRotatef( -90.0f, 1.0f, 0.0f, 0.0f );
		glCallList(modelo4+2);
	glPopMatrix();

	//Brazo Izquierdo
	glPushMatrix();
		glTranslatef( 1.3f, 1.0f, -0.25f );
		glRotatef( -90.0f, 1.0f, 0.0f, 0.0f );
		glCallList(modelo4+3);
	glPopMatrix();
}
void DibujaWiredPez()
{
	//torso
	glCallList(modelo4+4);

	//Pierna Izquierda
	glPushMatrix();
		glTranslatef( 0.5f, -0.5f, -0.25f );
		glCallList(modelo4+5);
	glPopMatrix();

	//Pierna Derecha
	glPushMatrix();
		glTranslatef( -0.5f, -0.5f, -0.25f );
		glCallList(modelo4+5);
	glPopMatrix();

	//Brazo Derecho
	glPushMatrix();
		glTranslatef( -1.3f, 1.0f, -0.25f );
		glRotatef( -90.0f, 1.0f, 0.0f, 0.0f );
		glCallList(modelo4+6);
	glPopMatrix();

	//Brazo Izquierdo
	glPushMatrix();
		glTranslatef( 1.3f, 1.0f, -0.25f );
		glRotatef( -90.0f, 1.0f, 0.0f, 0.0f );
		glCallList(modelo4+7);
	glPopMatrix();
}
void DibujaSombraBob()
{
	glPushMatrix();

		glTranslatef(player1.PosicionObj.x, player1.PosicionObj.y+1.7f, player1.PosicionObj.z);
		glRotatef(player1.AngObj, 0.0f, 1.0f, 0.0f);
		glScalef(player1.escalaX,player1.escalaY,player1.escalaZ);
		glTranslatef(player1modelo.Xtor, player1modelo.Ytor, player1modelo.Ztor);
		glRotatef(player1modelo.Angt2, 0.0f, 1.0f, 0.0f);
		glRotatef(player1modelo.Angt1, 1.0f, 0.0f, 0.0f);
				
		//Torso
		objSh.calculaSombraDepthPass(&g_3DModel1c, objectSpaceLightPosition1);
		
		//Pierna derecha
		glPushMatrix();
			glTranslatef(-1.2f, -1.3f ,0.0f);
			glRotatef(player1modelo.Angpder, 1.0f, 0.0f, 0.0f);
			objSh.calculaSombraDepthPass(&g_3DModel2c, objectSpaceLightPosition2);

			//Pierna derecha_b
			glPushMatrix();
				glTranslatef(0.0f, -1.25f , 0.0f);
				glRotatef(player1modelo.Angpderb, 1.0f, 0.0f, 0.0f);
				objSh.calculaSombraDepthPass(&g_3DModel3c, objectSpaceLightPosition3);
			glPopMatrix();

		glPopMatrix();

		//Pierna izquierda
		glPushMatrix();
			glTranslatef(1.2f, -1.3f ,0.0f);
			glRotatef(player1modelo.Angpizq, 1.0f, 0.0f, 0.0f);
			objSh.calculaSombraDepthPass(&g_3DModel4c, objectSpaceLightPosition4);

			//Pierna izquierda_b
			glPushMatrix();
				glTranslatef(0.0f, -1.25f , 0.0f);
				glRotatef(player1modelo.Angpizqb, 1.0f, 0.0f, 0.0f);
				objSh.calculaSombraDepthPass(&g_3DModel5c, objectSpaceLightPosition5);
			glPopMatrix();

		glPopMatrix();

		//Brazo derecho_a
		glPushMatrix();
			glTranslatef(-2.8f, 1.1f, 0.0f);
			glRotatef(player1modelo.Angbd2, 0.0f, 1.0f, 0.0f);
			glRotatef(player1modelo.Angbd1, 1.0f, 0.0f, 0.0f);
			objSh.calculaSombraDepthPass(&g_3DModel6c, objectSpaceLightPosition6);

			//Brazo derecho_b
			glPushMatrix();
				glTranslatef(-0.35f, -1.5f, 0.0f);
				glRotatef(player1modelo.Angbdb, 1.0f, 0.0f, 0.0f);
				objSh.calculaSombraDepthPass(&g_3DModel8c, objectSpaceLightPosition7);
			glPopMatrix();

		glPopMatrix();

		//Brazo izquierdo
		glPushMatrix();
			glTranslatef(2.8f, 1.1f, 0.0f);
			glRotatef(player1modelo.Angbi2, 0.0f, 1.0f, 0.0f);
			glRotatef(player1modelo.Angbi1, 1.0f, 0.0f, 0.0f);
			objSh.calculaSombraDepthPass(&g_3DModel7c, objectSpaceLightPosition8);

			//Brazo izquierdo_b
			glPushMatrix();
				glTranslatef(0.35f, -1.5f, 0.0f);
				glRotatef(player1modelo.Angbib, 1.0f, 0.0f, 0.0f);
				objSh.calculaSombraDepthPass(&g_3DModel9c, objectSpaceLightPosition9);
			glPopMatrix();

		glPopMatrix();

	glPopMatrix();
		
}

void DibujaSombraPat()
{
	glPushMatrix();

		glTranslatef(player2.PosicionObj.x + 10.0f,  player2.PosicionObj.y+3.9f, player2.PosicionObj.z);
		glRotatef(player2.AngObj, 0.0f, 1.0f, 0.0f);
		glTranslatef(player2modelo.Xtor, player2modelo.Ytor, player2modelo.Ztor);
		glRotatef(player2modelo.Angt2, 0.0f, 1.0f, 0.0f);
		glRotatef(player2modelo.Angt1, 1.0f, 0.0f, 0.0f);
		glScalef(0.7f,0.7f,0.7f);
		
		//Torso
		objSh.calculaSombraDepthPass(&g_3DModel1p, objectSpaceLightPosition10);
		
		//Pierna derecha
		glPushMatrix();
			glRotatef(player2modelo.Angpder, 1.0f, 0.0f, 0.0f);
			objSh.calculaSombraDepthPass(&g_3DModel2p, objectSpaceLightPosition11);
		glPopMatrix();

		//Pierna izquierda
		glPushMatrix();
			glRotatef(player2modelo.Angpizq, 1.0f, 0.0f, 0.0f);
			objSh.calculaSombraDepthPass(&g_3DModel3p, objectSpaceLightPosition12);
		glPopMatrix();

		//Brazo derecho
		glPushMatrix();
			glTranslatef( 0.0f, 1.3f, 0.0f );
			glRotatef(player2modelo.Angbd1, 1.0f, 0.0f, 0.0f);
			glTranslatef( 0.0f, -1.3f, 0.0f );
			objSh.calculaSombraDepthPass(&g_3DModel4p, objectSpaceLightPosition13);
		glPopMatrix();

		//Brazo izquierdo
		glPushMatrix();
			glTranslatef( 0.0f, 1.3f, 0.0f );
			glRotatef(player2modelo.Angbi1, 1.0f, 0.0f, 0.0f);
			glTranslatef( 0.0f, -1.3f, 0.0f );
			objSh.calculaSombraDepthPass(&g_3DModel5p, objectSpaceLightPosition14);
		glPopMatrix();

	glPopMatrix();
}
void DibujaSombraPlankton()
{
	glPushMatrix();
		glTranslatef(player3.PosicionObj.x - 10.0f,  player3.PosicionObj.y, player3.PosicionObj.z);
		glRotatef(player3.AngObj, 0.0f, 1.0f, 0.0f);
		glTranslatef(player3modelo.Xtor, player3modelo.Ytor, player3modelo.Ztor);
		glRotatef(player3modelo.Angt2, 0.0f, 1.0f, 0.0f);
		glRotatef(player3modelo.Angt1, 1.0f, 0.0f, 0.0f);
		glScalef(0.3f,0.3f,0.3f);

		//Torso
		objSh.calculaSombraDepthPass(&g_3DModel1a, objectSpaceLightPosition15);
		
		//Pierna derecha
		glPushMatrix();
			glTranslatef( 0.0f, 1.5f, 0.0f );
			glRotatef(player3modelo.Angpder, 1.0f, 0.0f, 0.0f);
			glTranslatef( 0.0f, -1.5f, 0.0f );
			objSh.calculaSombraDepthPass(&g_3DModel2a, objectSpaceLightPosition16);
		glPopMatrix();

		//Pierna izquierda
		glPushMatrix();
			glTranslatef( 0.0f, 1.5f, 0.0f );
			glRotatef(player3modelo.Angpizq, 1.0f, 0.0f, 0.0f);
			glTranslatef( 0.0f, -1.5f, 0.0f );
			objSh.calculaSombraDepthPass(&g_3DModel3a, objectSpaceLightPosition17);
		glPopMatrix();

		//Brazo derecho
		glPushMatrix();
			glTranslatef( 0.0f, 5.5f, 0.0f );
			glRotatef(player3modelo.Angbd1, 1.0f, 0.0f, 0.0f);
			glTranslatef( 0.0f, -5.5f, 0.0f );
			objSh.calculaSombraDepthPass(&g_3DModel4a, objectSpaceLightPosition18);
		glPopMatrix();

		//Brazo izquierdo
		glPushMatrix();
			glTranslatef( 0.0f, 5.5f, 0.0f );
			glRotatef(player3modelo.Angbi1, 1.0f, 0.0f, 0.0f);
			glTranslatef( 0.0f, -5.5f, 0.0f );
			objSh.calculaSombraDepthPass(&g_3DModel5a, objectSpaceLightPosition19);
		glPopMatrix();

	glPopMatrix();
}
void DibujaSombraPez()
{
	glPushMatrix();
		//torso
		objSh.calculaSombraDepthPass(&g_3DModel1f, objectSpaceLightPosition20);

		//Pierna Izquierda
		glPushMatrix();
			glTranslatef( 0.5f, -0.5f, -0.25f );
			objSh.calculaSombraDepthPass(&g_3DModel2f, objectSpaceLightPosition21);
		glPopMatrix();

		//Pierna Derecha
		glPushMatrix();
			glTranslatef( -0.5f, -0.5f, -0.25f );
			objSh.calculaSombraDepthPass(&g_3DModel2f, objectSpaceLightPosition22);
		glPopMatrix();

		//Brazo Derecho
		glPushMatrix();
			glTranslatef( -1.3f, 1.0f, -0.25f );
			glRotatef( -90.0f, 1.0f, 0.0f, 0.0f );
			objSh.calculaSombraDepthPass(&g_3DModel3f, objectSpaceLightPosition23);
		glPopMatrix();

		//Brazo Izquierdo
		glPushMatrix();
			glTranslatef( 1.3f, 1.0f, -0.25f );
			glRotatef( -90.0f, 1.0f, 0.0f, 0.0f );
			objSh.calculaSombraDepthPass(&g_3DModel4f, objectSpaceLightPosition24);
		glPopMatrix();

	glPopMatrix();
}
void DibujaVolumendeSombra()
{
	//Bob

	glPushMatrix();

		glTranslatef(player1.PosicionObj.x, player1.PosicionObj.y+1.7f, player1.PosicionObj.z);
		glRotatef(player1.AngObj, 0.0f, 1.0f, 0.0f);
		glScalef(player1.escalaX,player1.escalaY,player1.escalaZ);
		glTranslatef(player1modelo.Xtor, player1modelo.Ytor, player1modelo.Ztor);
		glRotatef(player1modelo.Angt2, 0.0f, 1.0f, 0.0f);
		glRotatef(player1modelo.Angt1, 1.0f, 0.0f, 0.0f);
				
		//Torso
		objSh.DrawShadowVolume(&g_3DModel1c, objectSpaceLightPosition1);
		
		//Pierna derecha
		glPushMatrix();
			glTranslatef(-1.2f, -1.3f ,0.0f);
			glRotatef(player1modelo.Angpder, 1.0f, 0.0f, 0.0f);
			objSh.DrawShadowVolume(&g_3DModel2c, objectSpaceLightPosition2);
			
			//Pierna derecha_b
			glPushMatrix();
				glTranslatef(0.0f, -1.25f , 0.0f);
				glRotatef(player1modelo.Angpderb, 1.0f, 0.0f, 0.0f);
				objSh.DrawShadowVolume(&g_3DModel3c, objectSpaceLightPosition3);
			glPopMatrix();

		glPopMatrix();

		//Pierna izquierda
		glPushMatrix();
			glTranslatef(1.2f, -1.3f ,0.0f);
			glRotatef(player1modelo.Angpizq, 1.0f, 0.0f, 0.0f);
			objSh.DrawShadowVolume(&g_3DModel4c, objectSpaceLightPosition4);

			//Pierna izquierda_b
			glPushMatrix();
				glTranslatef(0.0f, -1.25f , 0.0f);
				glRotatef(player1modelo.Angpizqb, 1.0f, 0.0f, 0.0f);
				objSh.DrawShadowVolume(&g_3DModel5c, objectSpaceLightPosition5);
			glPopMatrix();

		glPopMatrix();

		//Brazo derecho_a
		glPushMatrix();
			glTranslatef(-2.8f, 1.1f, 0.0f);
			glRotatef(player1modelo.Angbd2, 0.0f, 1.0f, 0.0f);
			glRotatef(player1modelo.Angbd1, 1.0f, 0.0f, 0.0f);
			objSh.DrawShadowVolume(&g_3DModel6c, objectSpaceLightPosition6);

			//Brazo derecho_b
			glPushMatrix();
				glTranslatef(-0.35f, -1.5f, 0.0f);
				glRotatef(player1modelo.Angbdb, 1.0f, 0.0f, 0.0f);
				objSh.DrawShadowVolume(&g_3DModel8c, objectSpaceLightPosition7);
			glPopMatrix();

		glPopMatrix();

		//Brazo izquierdo
		glPushMatrix();
			glTranslatef(2.8f, 1.1f, 0.0f);
			glRotatef(player1modelo.Angbi2, 0.0f, 1.0f, 0.0f);
			glRotatef(player1modelo.Angbi1, 1.0f, 0.0f, 0.0f);
			objSh.DrawShadowVolume(&g_3DModel7c, objectSpaceLightPosition8);

			//Brazo izquierdo_b
			glPushMatrix();
				glTranslatef(0.35f, -1.5f, 0.0f);
				glRotatef(player1modelo.Angbib, 1.0f, 0.0f, 0.0f);
				objSh.DrawShadowVolume(&g_3DModel9c, objectSpaceLightPosition9);
			glPopMatrix();

		glPopMatrix();

	glPopMatrix();

	//Pat

	glPushMatrix();

		glTranslatef(player2.PosicionObj.x + 10.0f,  player2.PosicionObj.y+3.9f, player2.PosicionObj.z);
		glRotatef(player2.AngObj, 0.0f, 1.0f, 0.0f);
		glTranslatef(player2modelo.Xtor, player2modelo.Ytor, player2modelo.Ztor);
		glRotatef(player2modelo.Angt2, 0.0f, 1.0f, 0.0f);
		glRotatef(player2modelo.Angt1, 1.0f, 0.0f, 0.0f);
		glScalef(0.7f,0.7f,0.7f);
		
		//Torso
		objSh.DrawShadowVolume(&g_3DModel1p, objectSpaceLightPosition10);
		
		//Pierna derecha
		glPushMatrix();
			glRotatef(player2modelo.Angpder, 1.0f, 0.0f, 0.0f);
			objSh.DrawShadowVolume(&g_3DModel2p, objectSpaceLightPosition11);
		glPopMatrix();

		//Pierna izquierda
		glPushMatrix();
			glRotatef(player2modelo.Angpizq, 1.0f, 0.0f, 0.0f);
			objSh.DrawShadowVolume(&g_3DModel3p, objectSpaceLightPosition12);
		glPopMatrix();

		//Brazo derecho
		glPushMatrix();
			glTranslatef( 0.0f, 1.3f, 0.0f );
			glRotatef(player2modelo.Angbd1, 1.0f, 0.0f, 0.0f);
			glTranslatef( 0.0f, -1.3f, 0.0f );
			objSh.DrawShadowVolume(&g_3DModel4p, objectSpaceLightPosition13);
		glPopMatrix();

		//Brazo izquierdo
		glPushMatrix();
			glTranslatef( 0.0f, 1.3f, 0.0f );
			glRotatef(player2modelo.Angbi1, 1.0f, 0.0f, 0.0f);
			glTranslatef( 0.0f, -1.3f, 0.0f );
			objSh.DrawShadowVolume(&g_3DModel5p, objectSpaceLightPosition14);
		glPopMatrix();

	glPopMatrix();

	//Plankton
	
	glPushMatrix();
		glTranslatef(player3.PosicionObj.x - 10.0f,  player3.PosicionObj.y, player3.PosicionObj.z);
		glRotatef(player3.AngObj, 0.0f, 1.0f, 0.0f);
		glTranslatef(player3modelo.Xtor, player3modelo.Ytor, player3modelo.Ztor);
		glRotatef(player3modelo.Angt2, 0.0f, 1.0f, 0.0f);
		glRotatef(player3modelo.Angt1, 1.0f, 0.0f, 0.0f);
		glScalef(0.3f,0.3f,0.3f);

		//Torso
		objSh.DrawShadowVolume(&g_3DModel1a, objectSpaceLightPosition15);
		
		//Pierna derecha
		glPushMatrix();
			glTranslatef( 0.0f, 1.5f, 0.0f );
			glRotatef(player3modelo.Angpder, 1.0f, 0.0f, 0.0f);
			glTranslatef( 0.0f, -1.5f, 0.0f );
			objSh.DrawShadowVolume(&g_3DModel2a, objectSpaceLightPosition16);
		glPopMatrix();

		//Pierna izquierda
		glPushMatrix();
			glTranslatef( 0.0f, 1.5f, 0.0f );
			glRotatef(player3modelo.Angpizq, 1.0f, 0.0f, 0.0f);
			glTranslatef( 0.0f, -1.5f, 0.0f );
			objSh.DrawShadowVolume(&g_3DModel3a, objectSpaceLightPosition17);
		glPopMatrix();

		//Brazo derecho
		glPushMatrix();
			glTranslatef( 0.0f, 5.5f, 0.0f );
			glRotatef(player3modelo.Angbd1, 1.0f, 0.0f, 0.0f);
			glTranslatef( 0.0f, -5.5f, 0.0f );
			objSh.DrawShadowVolume(&g_3DModel4a, objectSpaceLightPosition18);
		glPopMatrix();

		//Brazo izquierdo
		glPushMatrix();
			glTranslatef( 0.0f, 5.5f, 0.0f );
			glRotatef(player3modelo.Angbi1, 1.0f, 0.0f, 0.0f);
			glTranslatef( 0.0f, -5.5f, 0.0f );
			objSh.DrawShadowVolume(&g_3DModel5a, objectSpaceLightPosition19);
		glPopMatrix();

	glPopMatrix();
}
void ControlPuntos(int m, int n, int funcion)
{
	if(funcion == 1)
		Points[m][n].y+=1.0f;
	else if(funcion == 2)
		Points[m][n].y-=1.0f;
}

void DrawBezierPatch(int tipo)
{
	
	if(tipo == 1)
	{
		//Sin textura
		glColor3f(0.0f,0.0f,0.0f);
				
		for(int i=1;i<LOD;i++) {

			// se calcula el valor parametrico u 
			float u = (float)i/(LOD-1);
			float uant = ((float)i-1.0f)/((float)(LOD-1));

			//glBegin(GL_TRIANGLES);
			glBegin(GL_LINE_STRIP);

			for(int j=1;j<LOD;j++) {

				// se calcula el valor parametrico v
				float v = (float)j/(LOD-1);
				float vant = ((float)j-1.0f)/((float)(LOD-1));

				// se calcula el punto sobre la superficie
				Point p0 = Calcular(uant,v);
				Point p1 = Calcular(u,v);
				Point p2 = Calcular(u,vant);
				Point p3 = Calcular(uant,vant);

				//glColor3f(0.0f,0.0f,1.0f);

				// Dibuja el triangulo 1
				glTexCoord2f(uant,v); glVertex3f(p0.x,p0.y,p0.z);
				glTexCoord2f(u,v); glVertex3f(p1.x,p1.y,p1.z);
				glTexCoord2f(u,vant); glVertex3f(p2.x,p2.y,p2.z);

				//glColor3f(0.0f,1.0f,0.0f);

				// Dibuja el triangulo 2
				glTexCoord2f(u,vant); glVertex3f(p2.x,p2.y,p2.z);
				glTexCoord2f(uant,vant); glVertex3f(p3.x,p3.y,p3.z);
				glTexCoord2f(uant,v); glVertex3f(p0.x,p0.y,p0.z);
			}
			glEnd();
		
		}

		glColor3f(1.0f,1.0f,1.0f);
	}
	else if(tipo == 2)
	{
	
		//Con textura
		glEnable(GL_TEXTURE_2D);

		glAlphaFunc(GL_GREATER, 0.4f);
		glEnable(GL_ALPHA_TEST);
			
		for(int i=1;i<LOD;i++) {

			// se calcula el valor parametrico u 
			float u = (float)i/(LOD-1);
			float uant = ((float)i-1.0f)/((float)(LOD-1));

			glBindTexture(GL_TEXTURE_2D, textura[1].texID);

			glBegin(GL_TRIANGLES);

			for(int j=1;j<LOD;j++) {

				// se calcula el valor parametrico v
				float v = (float)j/(LOD-1);
				float vant = ((float)j-1.0f)/((float)(LOD-1));

				// se calcula el punto sobre la superficie
				Point p0 = Calcular(uant,v);
				Point p1 = Calcular(u,v);
				Point p2 = Calcular(u,vant);
				Point p3 = Calcular(uant,vant);
				
				// Dibuja el triangulo 1
				glTexCoord2f(1.0f-uant,v); glVertex3f(p0.x,p0.y,p0.z);
				glTexCoord2f(1.0f-u,v); glVertex3f(p1.x,p1.y,p1.z);
				glTexCoord2f(1.0f-u,vant); glVertex3f(p2.x,p2.y,p2.z);

				// Dibuja el triangulo 2
				glTexCoord2f(1.0f-u,vant); glVertex3f(p2.x,p2.y,p2.z);
				glTexCoord2f(1.0f-uant,vant); glVertex3f(p3.x,p3.y,p3.z);
				glTexCoord2f(1.0f-uant,v); glVertex3f(p0.x,p0.y,p0.z);
			}
			glEnd();
		
		}
		glDisable(GL_ALPHA_TEST);

		glDisable(GL_TEXTURE_2D);
	}
}

void DibujaPuntosDeControlBP()
{
	glDisable( GL_LIGHTING );

	for(int i=0; i<4; i++)
	{
		for(int j=0; j<4; j++)
		{
			if(i == isel && j == jsel)
				glColor3f(0.0f,0.0f,1.0f);
			else
				glColor3f(1.0f,0.0f,0.0f);

			glPointSize(10.0f);
			glBegin(GL_POINTS);
				glVertex3f(Points[i][j].x,Points[i][j].y+0.1f,Points[i][j].z);
			glEnd();
			glPointSize(1.0f);
		}
	}

	glLineWidth(2.0f);
	glColor3f( 1.0f, 1.0f, 1.0f );

	for(int i=0; i<3; i++)
	{
		for(int j=0; j<3; j++)
		{
			glBegin(GL_LINE_LOOP);
				glVertex3f(Points[i+1][j].x,Points[i+1][j].y+0.1f,Points[i+1][j].z);
				glVertex3f(Points[i+1][j+1].x,Points[i+1][j+1].y+0.1f,Points[i+1][j+1].z);
				glVertex3f(Points[i][j+1].x,Points[i][j+1].y+0.1f,Points[i][j+1].z);
				glVertex3f(Points[i][j].x,Points[i][j].y+0.1f,Points[i][j].z);
			glEnd();
		}
	}

	glLineWidth(1.0f);

	glColor3f(1.0f,1.0f,1.0f);

	glEnable( GL_LIGHTING );
}

void AnimaBezier()
{
	if(ypaux1 == 10.5f && !arriba)
	{
		arriba = true;
	}
	else if(ypaux1 == -10.5f && arriba)
	{
		arriba = false;
	}
	else if(!arriba)
	{
		Points[1][1].y += yp1;
		Points[1][2].y += yp1;
		Points[2][1].y += yp1;
		Points[2][2].y += yp1;
		ypaux1 += 0.25;
	}
	else if(arriba)
	{
		Points[1][1].y -= yp1;
		Points[1][2].y -= yp1;
		Points[2][1].y -= yp1;
		Points[2][2].y -= yp1;
		ypaux1 -= 0.25;
	}
}

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

void DibujaLuz(CVector l)
{
	lightPosition.x=LightPos[0];
	lightPosition.y=LightPos[1];
	lightPosition.z=LightPos[2];

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

void DibujaLuz2(CVector l)
{
	
	// Agregadas para el Programa 2, Paso 12
	cLightPosition.x=LightPos1[ 0 ];
	cLightPosition.y=LightPos1[ 1 ];
	cLightPosition.z=LightPos1[ 2 ];

	//Dibuja una esfera que representa la fuente luminosa
	glDisable(GL_LIGHTING);									// Deshabilita iluminación
	//glDisable(GL_COLOR_MATERIAL);
	glPushMatrix();
		glTranslatef(l.x, l.y, l.z);// Traslada a la posicion de la luz
		glColor4f(1.0f, 0.9f, 0.0f, 1.0f);					// Color amarillo
		gluSphere(e, 1.0f, 16, 8);							// Dibuja la esfera
	glPopMatrix();
	glEnable(GL_LIGHTING);									// Habilita Iluminación

	glColor3f(1.0f, 1.0f, 1.0f);
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

void DibujaTextos()
{
	glDisable(GL_DEPTH_TEST);							// Desactiva la prueba de profundidad
	glMatrixMode(GL_PROJECTION);						// Selecciona la matriz de proyección
	glPushMatrix();										// Guarda la matriz de proyección
	glLoadIdentity();									// Limpia la matriz de proyección
	glOrtho(0,glWidth,0,glHeight,-1,1);					// Crea una proyección ortogonal
	glMatrixMode(GL_MODELVIEW);							// Selecciona la matriz de modelo de vista
	glPushMatrix();										// Guarda la matriz de modelo de vista
	
	glDisable(GL_LIGHTING);
	glLoadIdentity();

	glColor3f(1.0f,1.0f,1.0f);
	
	glEnable(GL_TEXTURE_2D);

	glAlphaFunc(GL_GREATER, 0.8f);
	glEnable(GL_ALPHA_TEST);

	Font.glPrint((1.2f/640.0f)*glWidth, glWidth*0.05f,glHeight*0.85f,"FPS %d",FPS);
	/*Font.glPrint((1.2f/640.0f)*glWidth, glWidth*0.05f,glHeight*0.80f,"Objetos %d", datos[0]);
	Font.glPrint((1.2f/640.0f)*glWidth, glWidth*0.05f,glHeight*0.75f,"Poligonos %d", datos[1]);
	Font.glPrint((1.2f/640.0f)*glWidth, glWidth*0.05f,glHeight*0.70f,"Vertices %d", datos[2]);
	Font.glPrint((1.2f/640.0f)*glWidth, glWidth*0.05f,glHeight*0.65f,"Materiales %d", datos[3]);*/
								
	glDisable(GL_ALPHA_TEST);
	glDisable(GL_TEXTURE_2D);

	glColor3f(1.0f,1.0f,1.0f);

	glEnable(GL_LIGHTING);
	glMatrixMode(GL_PROJECTION);						// Selecciona la matriz de proyección
	glPopMatrix();										// Recupera la anterior matriz de proyección
	glMatrixMode(GL_MODELVIEW);							// Selecciona la matriz de modelo de vista
	glPopMatrix();										// Recupera la anterior matriz de modelo de vista
	glEnable(GL_DEPTH_TEST);							// Activa la prueba de profundidad
		
}

void ActualizaLuz()
{
	GLmatrix16f inverseModelMatrix;
	GLmatrix16f inverseModelMatrix2;
	GLmatrix16f inverseModelMatrix3;
	GLmatrix16f inverseModelMatrix4;
	GLmatrix16f inverseModelMatrix5;
	GLmatrix16f inverseModelMatrix6;
	GLmatrix16f inverseModelMatrix7;
	GLmatrix16f inverseModelMatrix8;
	GLmatrix16f inverseModelMatrix9;
	GLmatrix16f inverseModelMatrix10;
	GLmatrix16f inverseModelMatrix11;
	GLmatrix16f inverseModelMatrix12;
	GLmatrix16f inverseModelMatrix13;
	GLmatrix16f inverseModelMatrix14;
	GLmatrix16f inverseModelMatrix15;
	GLmatrix16f inverseModelMatrix16;
	GLmatrix16f inverseModelMatrix17;
	GLmatrix16f inverseModelMatrix18;
	GLmatrix16f inverseModelMatrix19;
	GLmatrix16f inverseModelMatrix20;
	GLmatrix16f inverseModelMatrix21;
	GLmatrix16f inverseModelMatrix22;
	GLmatrix16f inverseModelMatrix23;
	GLmatrix16f inverseModelMatrix24;

	GLvector4f  lightposition;
	GLvector4f  lightposition2;
	GLvector4f  lightposition3;
	GLvector4f  lightposition4;
	GLvector4f  lightposition5;
	GLvector4f  lightposition6;
	GLvector4f  lightposition7;
	GLvector4f  lightposition8;
	GLvector4f  lightposition9;
	GLvector4f  lightposition10;
	GLvector4f  lightposition11;
	GLvector4f  lightposition12;
	GLvector4f  lightposition13;
	GLvector4f  lightposition14;
	GLvector4f  lightposition15;
	GLvector4f  lightposition16;
	GLvector4f  lightposition17;
	GLvector4f  lightposition18;
	GLvector4f  lightposition19;
	GLvector4f  lightposition20;
	GLvector4f  lightposition21;
	GLvector4f  lightposition22;
	GLvector4f  lightposition23;
	GLvector4f  lightposition24;

	lightPosition.x=LightPos[0];
	lightPosition.y=LightPos[1];
	lightPosition.z=LightPos[2];
	
	//Bob

	//Modelo 1 (Torso)

	//Aquí se determina a partir de la inversa de la matriz de modelo de vista el espacio de objeto para la luz
	lightposition[0]=lightPosition.x;
	lightposition[1]=lightPosition.y;
	lightposition[2]=lightPosition.z;

	//Se obtiene la matriz inversa de modelo de vista para el modelo 1 aplicando sus transformaciones en orden inverso 
	//y signos opuestos para los parámetros
	glPushMatrix();
		glLoadIdentity();
		glRotatef(-player1modelo.Angt1, 1.0f, 0.0f, 0.0f);
		glRotatef(-player1modelo.Angt2, 0.0f, 1.0f, 0.0f);
		glRotatef(-player1.AngObj, 0.0f, 1.0f, 0.0f);
		glGetFloatv(GL_MODELVIEW_MATRIX, inverseModelMatrix);
	glPopMatrix();

	//Guarda las posiciones de la luz para el espacio de objeto del objeto de sombra 1
	VMatMult(inverseModelMatrix, lightposition);
	
	objectSpaceLightPosition1.x=lightposition[0];
	objectSpaceLightPosition1.y=lightposition[1];
	objectSpaceLightPosition1.z=lightposition[2];

	//Modelo 2 (Pierna derecha)
	
	//Aquí se determina a partir de la inversa de la matriz de modelo de vista el espacio de objeto para la luz
	lightposition2[0]=lightPosition.x;
	lightposition2[1]=lightPosition.y;
	lightposition2[2]=lightPosition.z;

	//Se obtiene la matriz inversa de modelo de vista para el modelo 2 aplicando sus transformaciones en orden inverso 
	//y signos opuestos para los parámetros
	glPushMatrix();
		glLoadIdentity();
		glRotatef(-player1modelo.Angpder, 1.0f, 0.0f, 0.0f);
		glRotatef(-player1modelo.Angt1, 1.0f, 0.0f, 0.0f);
		glRotatef(-player1modelo.Angt2, 0.0f, 1.0f, 0.0f);
		glRotatef(-player1.AngObj, 0.0f, 1.0f, 0.0f);
		glGetFloatv(GL_MODELVIEW_MATRIX, inverseModelMatrix2);
	glPopMatrix();

	//Guarda las posiciones de la luz para el espacio de objeto del objeto de sombra 2
	VMatMult(inverseModelMatrix2, lightposition2);
	
	objectSpaceLightPosition2.x=lightposition2[0];
	objectSpaceLightPosition2.y=lightposition2[1];
	objectSpaceLightPosition2.z=lightposition2[2];
	

	//Modelo 3 (Pierna derecha_b)
	
	//Aquí se determina a partir de la inversa de la matriz de modelo de vista el espacio de objeto para la luz
	lightposition3[0]=lightPosition.x;
	lightposition3[1]=lightPosition.y;
	lightposition3[2]=lightPosition.z;

	//Se obtiene la matriz inversa de modelo de vista para el modelo 3 aplicando sus transformaciones en orden inverso 
	//y signos opuestos para los parámetros
	glPushMatrix();
		glLoadIdentity();
		glRotatef(-player1modelo.Angpderb, 1.0f, 0.0f, 0.0f);
		glRotatef(-player1modelo.Angpder, 1.0f, 0.0f, 0.0f);
		glRotatef(-player1modelo.Angt1, 1.0f, 0.0f, 0.0f);
		glRotatef(-player1modelo.Angt2, 0.0f, 1.0f, 0.0f);
		glRotatef(-player1.AngObj, 0.0f, 1.0f, 0.0f);
		glGetFloatv(GL_MODELVIEW_MATRIX, inverseModelMatrix3);
	glPopMatrix();

	//Guarda las posiciones de la luz para el espacio de objeto del objeto de sombra 3
	VMatMult(inverseModelMatrix3, lightposition3);
	
	objectSpaceLightPosition3.x=lightposition3[0];
	objectSpaceLightPosition3.y=lightposition3[1];
	objectSpaceLightPosition3.z=lightposition3[2];
	

	//Modelo 4 (Pierna izquierda)

	//Aquí se determina a partir de la inversa de la matriz de modelo de vista el espacio de objeto para la luz
	lightposition4[0]=lightPosition.x;
	lightposition4[1]=lightPosition.y;
	lightposition4[2]=lightPosition.z;

	//Se obtiene la matriz inversa de modelo de vista para el modelo 4 aplicando sus transformaciones en orden inverso 
	//y signos opuestos para los parámetros
	glPushMatrix();
		glLoadIdentity();
		glRotatef(-player1modelo.Angpizq, 1.0f, 0.0f, 0.0f);
		glRotatef(-player1modelo.Angt1, 1.0f, 0.0f, 0.0f);
		glRotatef(-player1modelo.Angt2, 0.0f, 1.0f, 0.0f);
		glRotatef(-player1.AngObj, 0.0f, 1.0f, 0.0f);
		glGetFloatv(GL_MODELVIEW_MATRIX, inverseModelMatrix4);
	glPopMatrix();

	//Guarda las posiciones de la luz para el espacio de objeto del objeto de sombra 4
	VMatMult(inverseModelMatrix4, lightposition4);
	
	objectSpaceLightPosition4.x=lightposition4[0];
	objectSpaceLightPosition4.y=lightposition4[1];
	objectSpaceLightPosition4.z=lightposition4[2];

	//Modelo 5 (Pierna izquierda_b)

	//Aquí se determina a partir de la inversa de la matriz de modelo de vista el espacio de objeto para la luz
	lightposition5[0]=lightPosition.x;
	lightposition5[1]=lightPosition.y;
	lightposition5[2]=lightPosition.z;

	//Se obtiene la matriz inversa de modelo de vista para el modelo 5 aplicando sus transformaciones en orden inverso 
	//y signos opuestos para los parámetros
	glPushMatrix();
		glLoadIdentity();
		glRotatef(-player1modelo.Angpizqb, 1.0f, 0.0f, 0.0f);
		glRotatef(-player1modelo.Angpizq, 1.0f, 0.0f, 0.0f);
		glRotatef(-player1modelo.Angt1, 1.0f, 0.0f, 0.0f);
		glRotatef(-player1modelo.Angt2, 0.0f, 1.0f, 0.0f);
		glRotatef(-player1.AngObj, 0.0f, 1.0f, 0.0f);
		glGetFloatv(GL_MODELVIEW_MATRIX, inverseModelMatrix5);
	glPopMatrix();

	//Guarda las posiciones de la luz para el espacio de objeto del objeto de sombra 5
	VMatMult(inverseModelMatrix5, lightposition5);
	
	objectSpaceLightPosition5.x=lightposition5[0];
	objectSpaceLightPosition5.y=lightposition5[1];
	objectSpaceLightPosition5.z=lightposition5[2];

	//Modelo 6 (Brazo derecho_a)

	//Aquí se determina a partir de la inversa de la matriz de modelo de vista el espacio de objeto para la luz
	lightposition6[0]=lightPosition.x;
	lightposition6[1]=lightPosition.y;
	lightposition6[2]=lightPosition.z;

	//Se obtiene la matriz inversa de modelo de vista para el modelo 6 aplicando sus transformaciones en orden inverso 
	//y signos opuestos para los parámetros
	glPushMatrix();
		glLoadIdentity();
		glRotatef(-player1modelo.Angbd1, 1.0f, 0.0f, 0.0f);
		glRotatef(-player1modelo.Angbd2, 1.0f, 1.0f, 0.0f);
		glRotatef(-player1modelo.Angt1, 1.0f, 0.0f, 0.0f);
		glRotatef(-player1modelo.Angt2, 0.0f, 1.0f, 0.0f);
		glRotatef(-player1.AngObj, 0.0f, 1.0f, 0.0f);
		glGetFloatv(GL_MODELVIEW_MATRIX, inverseModelMatrix6);
	glPopMatrix();

	//Guarda las posiciones de la luz para el espacio de objeto del objeto de sombra 6
	VMatMult(inverseModelMatrix6, lightposition6);
	
	objectSpaceLightPosition6.x=lightposition6[0];
	objectSpaceLightPosition6.y=lightposition6[1];
	objectSpaceLightPosition6.z=lightposition6[2];

	//Modelo 7 (Brazo derecho_b)

	//Aquí se determina a partir de la inversa de la matriz de modelo de vista el espacio de objeto para la luz
	lightposition7[0]=lightPosition.x;
	lightposition7[1]=lightPosition.y;
	lightposition7[2]=lightPosition.z;

	//Se obtiene la matriz inversa de modelo de vista para el modelo 7 aplicando sus transformaciones en orden inverso 
	//y signos opuestos para los parámetros
	glPushMatrix();
		glLoadIdentity();
		glRotatef(-player1modelo.Angbdb, 1.0f, 0.0f, 0.0f);
		glRotatef(-player1modelo.Angbd1, 1.0f, 0.0f, 0.0f);
		glRotatef(-player1modelo.Angbd2, 1.0f, 1.0f, 0.0f);
		glRotatef(-player1modelo.Angt1, 1.0f, 0.0f, 0.0f);
		glRotatef(-player1modelo.Angt2, 0.0f, 1.0f, 0.0f);
		glRotatef(-player1.AngObj, 0.0f, 1.0f, 0.0f);
		glGetFloatv(GL_MODELVIEW_MATRIX, inverseModelMatrix7);
	glPopMatrix();

	//Guarda las posiciones de la luz para el espacio de objeto del objeto de sombra 7
	VMatMult(inverseModelMatrix7, lightposition7);
	
	objectSpaceLightPosition7.x=lightposition7[0];
	objectSpaceLightPosition7.y=lightposition7[1];
	objectSpaceLightPosition7.z=lightposition7[2];

	//Modelo 8 (Brazo izquierdo_a)

	//Aquí se determina a partir de la inversa de la matriz de modelo de vista el espacio de objeto para la luz
	lightposition8[0]=lightPosition.x;
	lightposition8[1]=lightPosition.y;
	lightposition8[2]=lightPosition.z;

	//Se obtiene la matriz inversa de modelo de vista para el modelo 8 aplicando sus transformaciones en orden inverso 
	//y signos opuestos para los parámetros
	glPushMatrix();
		glLoadIdentity();
		glRotatef(-player1modelo.Angbi1, 1.0f, 0.0f, 0.0f);
		glRotatef(-player1modelo.Angbi2, 0.0f, 1.0f, 0.0f);
		glRotatef(-player1modelo.Angt1, 1.0f, 0.0f, 0.0f);
		glRotatef(-player1modelo.Angt2, 0.0f, 1.0f, 0.0f);
		glRotatef(-player1.AngObj, 0.0f, 1.0f, 0.0f);
		glGetFloatv(GL_MODELVIEW_MATRIX, inverseModelMatrix8);
	glPopMatrix();

	//Guarda las posiciones de la luz para el espacio de objeto del objeto de sombra 8
	VMatMult(inverseModelMatrix8, lightposition8);
	
	objectSpaceLightPosition8.x=lightposition8[0];
	objectSpaceLightPosition8.y=lightposition8[1];
	objectSpaceLightPosition8.z=lightposition8[2];
	
	//Modelo 9 (Brazo izquierdo_b)

	//Aquí se determina a partir de la inversa de la matriz de modelo de vista el espacio de objeto para la luz
	lightposition9[0]=lightPosition.x;
	lightposition9[1]=lightPosition.y;
	lightposition9[2]=lightPosition.z;

	//Se obtiene la matriz inversa de modelo de vista para el modelo 9 aplicando sus transformaciones en orden inverso 
	//y signos opuestos para los parámetros
	glPushMatrix();
		glLoadIdentity();
		glRotatef(-player1modelo.Angbib, 1.0f, 0.0f, 0.0f);
		glRotatef(-player1modelo.Angbi1, 1.0f, 0.0f, 0.0f);
		glRotatef(-player1modelo.Angbi2, 0.0f, 1.0f, 0.0f);
		glRotatef(-player1modelo.Angt1, 1.0f, 0.0f, 0.0f);
		glRotatef(-player1modelo.Angt2, 0.0f, 1.0f, 0.0f);
		glRotatef(-player1.AngObj, 0.0f, 1.0f, 0.0f);
		glGetFloatv(GL_MODELVIEW_MATRIX, inverseModelMatrix9);
	glPopMatrix();

	//Guarda las posiciones de la luz para el espacio de objeto del objeto de sombra 9
	VMatMult(inverseModelMatrix9, lightposition9);
	
	objectSpaceLightPosition9.x=lightposition9[0];
	objectSpaceLightPosition9.y=lightposition9[1];
	objectSpaceLightPosition9.z=lightposition9[2];

	//Pat

	//Modelo 1 (Torso)

	//Aquí se determina a partir de la inversa de la matriz de modelo de vista el espacio de objeto para la luz
	lightposition10[0]=lightPosition.x;
	lightposition10[1]=lightPosition.y;
	lightposition10[2]=lightPosition.z;

	//Se obtiene la matriz inversa de modelo de vista para el modelo 1 aplicando sus transformaciones en orden inverso 
	//y signos opuestos para los parámetros
	glPushMatrix();
		glLoadIdentity();
		glRotatef(-player2modelo.Angt1, 1.0f, 0.0f, 0.0f);
		glRotatef(-player2modelo.Angt2, 0.0f, 1.0f, 0.0f);
		glRotatef(-player2.AngObj, 0.0f, 1.0f, 0.0f);
		glGetFloatv(GL_MODELVIEW_MATRIX, inverseModelMatrix10);
	glPopMatrix();

	//Guarda las posiciones de la luz para el espacio de objeto del objeto de sombra 1
	VMatMult(inverseModelMatrix10, lightposition10);
	
	objectSpaceLightPosition10.x=lightposition10[0];
	objectSpaceLightPosition10.y=lightposition10[1];
	objectSpaceLightPosition10.z=lightposition10[2];

	//Modelo 2 (Pierna derecha)
	
	//Aquí se determina a partir de la inversa de la matriz de modelo de vista el espacio de objeto para la luz
	lightposition11[0]=lightPosition.x;
	lightposition11[1]=lightPosition.y;
	lightposition11[2]=lightPosition.z;

	//Se obtiene la matriz inversa de modelo de vista para el modelo 2 aplicando sus transformaciones en orden inverso 
	//y signos opuestos para los parámetros
	glPushMatrix();
		glLoadIdentity();
		glRotatef(-player2modelo.Angpder, 1.0f, 0.0f, 0.0f);
		glRotatef(-player2modelo.Angt1, 1.0f, 0.0f, 0.0f);
		glRotatef(-player2modelo.Angt2, 0.0f, 1.0f, 0.0f);
		glRotatef(-player2.AngObj, 0.0f, 1.0f, 0.0f);
		glGetFloatv(GL_MODELVIEW_MATRIX, inverseModelMatrix11);
	glPopMatrix();

	//Guarda las posiciones de la luz para el espacio de objeto del objeto de sombra 2
	VMatMult(inverseModelMatrix11, lightposition11);
	
	objectSpaceLightPosition11.x=lightposition11[0];
	objectSpaceLightPosition11.y=lightposition11[1];
	objectSpaceLightPosition11.z=lightposition11[2];
	

	//Modelo 3 (Pierna izquierda)
	
	//Aquí se determina a partir de la inversa de la matriz de modelo de vista el espacio de objeto para la luz
	lightposition12[0]=lightPosition.x;
	lightposition12[1]=lightPosition.y;
	lightposition12[2]=lightPosition.z;

	//Se obtiene la matriz inversa de modelo de vista para el modelo 3 aplicando sus transformaciones en orden inverso 
	//y signos opuestos para los parámetros
	glPushMatrix();
		glLoadIdentity();
		glRotatef(-player2modelo.Angpizq, 1.0f, 0.0f, 0.0f);
		glRotatef(-player2modelo.Angt1, 1.0f, 0.0f, 0.0f);
		glRotatef(-player2modelo.Angt2, 0.0f, 1.0f, 0.0f);
		glRotatef(-player2.AngObj, 0.0f, 1.0f, 0.0f);
		glGetFloatv(GL_MODELVIEW_MATRIX, inverseModelMatrix12);
	glPopMatrix();

	//Guarda las posiciones de la luz para el espacio de objeto del objeto de sombra 3
	VMatMult(inverseModelMatrix12, lightposition12);
	
	objectSpaceLightPosition12.x=lightposition12[0];
	objectSpaceLightPosition12.y=lightposition12[1];
	objectSpaceLightPosition12.z=lightposition12[2];
	

	//Modelo 4 (Brazo Derecho)

	//Aquí se determina a partir de la inversa de la matriz de modelo de vista el espacio de objeto para la luz
	lightposition13[0]=lightPosition.x;
	lightposition13[1]=lightPosition.y;
	lightposition13[2]=lightPosition.z;

	//Se obtiene la matriz inversa de modelo de vista para el modelo 4 aplicando sus transformaciones en orden inverso 
	//y signos opuestos para los parámetros
	glPushMatrix();
		glLoadIdentity();
		glRotatef(-player2modelo.Angbd2, 0.0f, 0.0f, 1.0f);
		glRotatef(-player2modelo.Angbd1, 1.0f, 0.0f, 0.0f);
		glRotatef(-player2modelo.Angt1, 1.0f, 0.0f, 0.0f);
		glRotatef(-player2modelo.Angt2, 0.0f, 1.0f, 0.0f);
		glRotatef(-player2.AngObj, 0.0f, 1.0f, 0.0f);
		glGetFloatv(GL_MODELVIEW_MATRIX, inverseModelMatrix13);
	glPopMatrix();

	//Guarda las posiciones de la luz para el espacio de objeto del objeto de sombra 4
	VMatMult(inverseModelMatrix13, lightposition13);
	
	objectSpaceLightPosition13.x=lightposition13[0];
	objectSpaceLightPosition13.y=lightposition13[1];
	objectSpaceLightPosition13.z=lightposition13[2];

	//Modelo 5 (Brazo izquierdo)

	//Aquí se determina a partir de la inversa de la matriz de modelo de vista el espacio de objeto para la luz
	lightposition14[0]=lightPosition.x;
	lightposition14[1]=lightPosition.y;
	lightposition14[2]=lightPosition.z;

	//Se obtiene la matriz inversa de modelo de vista para el modelo 5 aplicando sus transformaciones en orden inverso 
	//y signos opuestos para los parámetros
	glPushMatrix();
		glLoadIdentity();
		glRotatef(-player2modelo.Angbi2, 0.0f, 0.0f, 1.0f);
		glRotatef(-player2modelo.Angbi1, 1.0f, 0.0f, 0.0f);
		glRotatef(-player2modelo.Angt1, 1.0f, 0.0f, 0.0f);
		glRotatef(-player2modelo.Angt2, 0.0f, 1.0f, 0.0f);
		glRotatef(-player2.AngObj, 0.0f, 1.0f, 0.0f);
		glGetFloatv(GL_MODELVIEW_MATRIX, inverseModelMatrix14);
	glPopMatrix();

	//Guarda las posiciones de la luz para el espacio de objeto del objeto de sombra 5
	VMatMult(inverseModelMatrix14, lightposition14);
	
	objectSpaceLightPosition14.x=lightposition14[0];
	objectSpaceLightPosition14.y=lightposition14[1];
	objectSpaceLightPosition14.z=lightposition14[2];

	//Plankton

	//Modelo 1 (Torso)

	//Aquí se determina a partir de la inversa de la matriz de modelo de vista el espacio de objeto para la luz
	lightposition15[0]=lightPosition.x;
	lightposition15[1]=lightPosition.y;
	lightposition15[2]=lightPosition.z;

	//Se obtiene la matriz inversa de modelo de vista para el modelo 1 aplicando sus transformaciones en orden inverso 
	//y signos opuestos para los parámetros
	glPushMatrix();
		glLoadIdentity();
		glRotatef(-player3modelo.Angt1, 1.0f, 0.0f, 0.0f);
		glRotatef(-player3modelo.Angt2, 0.0f, 1.0f, 0.0f);
		glRotatef(-player3.AngObj, 0.0f, 1.0f, 0.0f);
		glGetFloatv(GL_MODELVIEW_MATRIX, inverseModelMatrix15);
	glPopMatrix();

	//Guarda las posiciones de la luz para el espacio de objeto del objeto de sombra 1
	VMatMult(inverseModelMatrix15, lightposition15);
	
	objectSpaceLightPosition15.x=lightposition15[0];
	objectSpaceLightPosition15.y=lightposition15[1];
	objectSpaceLightPosition15.z=lightposition15[2];

	//Modelo 2 (Pierna derecha)
	
	//Aquí se determina a partir de la inversa de la matriz de modelo de vista el espacio de objeto para la luz
	lightposition16[0]=lightPosition.x;
	lightposition16[1]=lightPosition.y;
	lightposition16[2]=lightPosition.z;

	//Se obtiene la matriz inversa de modelo de vista para el modelo 2 aplicando sus transformaciones en orden inverso 
	//y signos opuestos para los parámetros
	glPushMatrix();
		glLoadIdentity();
		glRotatef(-player3modelo.Angpder, 1.0f, 0.0f, 0.0f);
		glRotatef(-player3modelo.Angt1, 1.0f, 0.0f, 0.0f);
		glRotatef(-player3modelo.Angt2, 0.0f, 1.0f, 0.0f);
		glRotatef(-player3.AngObj, 0.0f, 1.0f, 0.0f);
		glGetFloatv(GL_MODELVIEW_MATRIX, inverseModelMatrix16);
	glPopMatrix();

	//Guarda las posiciones de la luz para el espacio de objeto del objeto de sombra 2
	VMatMult(inverseModelMatrix16, lightposition16);
	
	objectSpaceLightPosition16.x=lightposition16[0];
	objectSpaceLightPosition16.y=lightposition16[1];
	objectSpaceLightPosition16.z=lightposition16[2];
	

	//Modelo 3 (Pierna izquierda)
	
	//Aquí se determina a partir de la inversa de la matriz de modelo de vista el espacio de objeto para la luz
	lightposition17[0]=lightPosition.x;
	lightposition17[1]=lightPosition.y;
	lightposition17[2]=lightPosition.z;

	//Se obtiene la matriz inversa de modelo de vista para el modelo 3 aplicando sus transformaciones en orden inverso 
	//y signos opuestos para los parámetros
	glPushMatrix();
		glLoadIdentity();
		glRotatef(-player3modelo.Angpizq, 1.0f, 0.0f, 0.0f);
		glRotatef(-player3modelo.Angt1, 1.0f, 0.0f, 0.0f);
		glRotatef(-player3modelo.Angt2, 0.0f, 1.0f, 0.0f);
		glRotatef(-player3.AngObj, 0.0f, 1.0f, 0.0f);
		glGetFloatv(GL_MODELVIEW_MATRIX, inverseModelMatrix17);
	glPopMatrix();

	//Guarda las posiciones de la luz para el espacio de objeto del objeto de sombra 3
	VMatMult(inverseModelMatrix17, lightposition17);
	
	objectSpaceLightPosition17.x=lightposition17[0];
	objectSpaceLightPosition17.y=lightposition17[1];
	objectSpaceLightPosition17.z=lightposition17[2];
	

	//Modelo 4 (Brazo Derecho)

	//Aquí se determina a partir de la inversa de la matriz de modelo de vista el espacio de objeto para la luz
	lightposition18[0]=lightPosition.x;
	lightposition18[1]=lightPosition.y;
	lightposition18[2]=lightPosition.z;

	//Se obtiene la matriz inversa de modelo de vista para el modelo 4 aplicando sus transformaciones en orden inverso 
	//y signos opuestos para los parámetros
	glPushMatrix();
		glLoadIdentity();
		glRotatef(-player3modelo.Angbd1, 1.0f, 0.0f, 0.0f);
		glRotatef(-player3modelo.Angt1, 1.0f, 0.0f, 0.0f);
		glRotatef(-player3modelo.Angt2, 0.0f, 1.0f, 0.0f);
		glRotatef(-player3.AngObj, 0.0f, 1.0f, 0.0f);
		glGetFloatv(GL_MODELVIEW_MATRIX, inverseModelMatrix18);
	glPopMatrix();

	//Guarda las posiciones de la luz para el espacio de objeto del objeto de sombra 4
	VMatMult(inverseModelMatrix18, lightposition18);
	
	objectSpaceLightPosition18.x=lightposition18[0];
	objectSpaceLightPosition18.y=lightposition18[1];
	objectSpaceLightPosition18.z=lightposition18[2];

	//Modelo 5 (Brazo izquierdo)

	//Aquí se determina a partir de la inversa de la matriz de modelo de vista el espacio de objeto para la luz
	lightposition19[0]=lightPosition.x;
	lightposition19[1]=lightPosition.y;
	lightposition19[2]=lightPosition.z;

	//Se obtiene la matriz inversa de modelo de vista para el modelo 5 aplicando sus transformaciones en orden inverso 
	//y signos opuestos para los parámetros
	glPushMatrix();
		glLoadIdentity();
		glRotatef(-player3modelo.Angbi1, 1.0f, 0.0f, 0.0f);
		glRotatef(-player3modelo.Angt1, 1.0f, 0.0f, 0.0f);
		glRotatef(-player3modelo.Angt2, 0.0f, 1.0f, 0.0f);
		glRotatef(-player3.AngObj, 0.0f, 1.0f, 0.0f);
		glGetFloatv(GL_MODELVIEW_MATRIX, inverseModelMatrix19);
	glPopMatrix();

	//Guarda las posiciones de la luz para el espacio de objeto del objeto de sombra 5
	VMatMult(inverseModelMatrix19, lightposition19);
	
	objectSpaceLightPosition19.x=lightposition19[0];
	objectSpaceLightPosition19.y=lightposition19[1];
	objectSpaceLightPosition19.z=lightposition19[2];

	//Pez

	//Modelo 1 (Torso)

	//Aquí se determina a partir de la inversa de la matriz de modelo de vista el espacio de objeto para la luz
	lightposition20[0]=lightPosition.x;
	lightposition20[1]=lightPosition.y;
	lightposition20[2]=lightPosition.z;

	//Se obtiene la matriz inversa de modelo de vista para el modelo 1 aplicando sus transformaciones en orden inverso 
	//y signos opuestos para los parámetros
	glPushMatrix();
		glLoadIdentity();
		glGetFloatv(GL_MODELVIEW_MATRIX, inverseModelMatrix20);
	glPopMatrix();

	//Guarda las posiciones de la luz para el espacio de objeto del objeto de sombra 1
	VMatMult(inverseModelMatrix20, lightposition20);
	
	objectSpaceLightPosition20.x=lightposition20[0];
	objectSpaceLightPosition20.y=lightposition20[1];
	objectSpaceLightPosition20.z=lightposition20[2];

	//Modelo 2 (Pierna derecha)
	
	//Aquí se determina a partir de la inversa de la matriz de modelo de vista el espacio de objeto para la luz
	lightposition21[0]=lightPosition.x;
	lightposition21[1]=lightPosition.y;
	lightposition21[2]=lightPosition.z;

	//Se obtiene la matriz inversa de modelo de vista para el modelo 2 aplicando sus transformaciones en orden inverso 
	//y signos opuestos para los parámetros
	glPushMatrix();
		glLoadIdentity();
		glGetFloatv(GL_MODELVIEW_MATRIX, inverseModelMatrix21);
	glPopMatrix();

	//Guarda las posiciones de la luz para el espacio de objeto del objeto de sombra 2
	VMatMult(inverseModelMatrix21, lightposition21);
	
	objectSpaceLightPosition21.x=lightposition21[0];
	objectSpaceLightPosition21.y=lightposition21[1];
	objectSpaceLightPosition21.z=lightposition21[2];
	

	//Modelo 3 (Pierna izquierda)
	
	//Aquí se determina a partir de la inversa de la matriz de modelo de vista el espacio de objeto para la luz
	lightposition22[0]=lightPosition.x;
	lightposition22[1]=lightPosition.y;
	lightposition22[2]=lightPosition.z;

	//Se obtiene la matriz inversa de modelo de vista para el modelo 3 aplicando sus transformaciones en orden inverso 
	//y signos opuestos para los parámetros
	glPushMatrix();
		glLoadIdentity();
		glGetFloatv(GL_MODELVIEW_MATRIX, inverseModelMatrix22);
	glPopMatrix();

	//Guarda las posiciones de la luz para el espacio de objeto del objeto de sombra 3
	VMatMult(inverseModelMatrix22, lightposition22);
	
	objectSpaceLightPosition22.x=lightposition22[0];
	objectSpaceLightPosition22.y=lightposition22[1];
	objectSpaceLightPosition22.z=lightposition22[2];
	

	//Modelo 4 (Brazo Derecho)

	//Aquí se determina a partir de la inversa de la matriz de modelo de vista el espacio de objeto para la luz
	lightposition23[0]=lightPosition.x;
	lightposition23[1]=lightPosition.y;
	lightposition23[2]=lightPosition.z;

	//Se obtiene la matriz inversa de modelo de vista para el modelo 4 aplicando sus transformaciones en orden inverso 
	//y signos opuestos para los parámetros
	glPushMatrix();
		glLoadIdentity();
		glRotatef( 90.0f, 1.0f, 0.0f, 0.0f );
		glGetFloatv(GL_MODELVIEW_MATRIX, inverseModelMatrix23);
	glPopMatrix();

	//Guarda las posiciones de la luz para el espacio de objeto del objeto de sombra 4
	VMatMult(inverseModelMatrix23, lightposition23);
	
	objectSpaceLightPosition23.x=lightposition23[0];
	objectSpaceLightPosition23.y=lightposition23[1];
	objectSpaceLightPosition23.z=lightposition23[2];

	//Modelo 5 (Brazo izquierdo)

	//Aquí se determina a partir de la inversa de la matriz de modelo de vista el espacio de objeto para la luz
	lightposition24[0]=lightPosition.x;
	lightposition24[1]=lightPosition.y;
	lightposition24[2]=lightPosition.z;

	//Se obtiene la matriz inversa de modelo de vista para el modelo 5 aplicando sus transformaciones en orden inverso 
	//y signos opuestos para los parámetros
	glPushMatrix();
		glLoadIdentity();
		glRotatef( 90.0f, 1.0f, 0.0f, 0.0f );
		glGetFloatv(GL_MODELVIEW_MATRIX, inverseModelMatrix24);
	glPopMatrix();

	//Guarda las posiciones de la luz para el espacio de objeto del objeto de sombra 5
	VMatMult(inverseModelMatrix24, lightposition24);
	
	objectSpaceLightPosition24.x=lightposition24[0];
	objectSpaceLightPosition24.y=lightposition24[1];
	objectSpaceLightPosition24.z=lightposition24[2];
}

void calculaNormal(float x1, float y1, float z1, float x2, float y2, float z2, float x3, float y3, float z3){
	CVector v1, v2, N;
	v1.x = x2 - x1;
	v1.y = y2 - y1;
	v1.z = z2 - z1;

	v2.x = x3 - x1;
	v2.y = y3 - y1;
	v2.z = z3 - z1;

	N = Normaliza( Cruz (v1, v2) );
	glNormal3f( N.x, N.y, N.z );
}
float Proyecta( float a, float b )
{
	float aux;
	// Calcula un vector de la posicion de la luz hasta el vertice del circulo
	aux = a - b;

	return aux;

}
void DibujaAlarma()
{
	int   lados;
	float radio, alt;
	float ang, delta;
	CVector norm;
	float a[ 3 ], b[ 3 ], c[ 3 ], d[ 3 ], o[ 3 ], aux[ 3 ];
	float j = 0.0f;

	lados = 30;
	radio = 2.0f;

	delta = 360.0f / lados;

	// alpha
	glEnable(GL_BLEND);		// Turn Blending On
	glDisable(GL_DEPTH_TEST);	// Turn Depth Testing Off
	glColor4f(1.0f,1.0f,1.0f,0.5f);			// Full Brightness, 50% Alpha ( NEW )
	glBlendFunc(GL_SRC_ALPHA,GL_ONE);		// Blending Function For Translucency Based On Source Alpha Value ( NEW )


	for(int i=0; i<lados; i++)
	{
		ang=i*delta;
		a[0]=radio*cosf(ang*PI/180.0f);
		a[1]=0.0f;
		a[2]=radio*sinf(ang*PI/180.0f);

		b[0] = a[0];
		b[1] = -40.0f;
		b[2] = a[2];

		aux[ 0 ] = Proyecta( a[ 0 ], lightPosition.x );
		aux[ 1 ] = Proyecta( a[ 1 ], lightPosition.y );
		aux[ 2 ] = Proyecta( a[ 2 ], lightPosition.z );

		// Normaliza este vector y multiplicalo por veinte
		norm = CVector( aux[ 0 ], aux[ 1 ], aux[ 2 ] );
		norm = Normaliza( norm );
	
		b[ 0 ] = 100 * norm.x + a[ 0 ];
		b[ 1 ] = 100 * norm.y + a[ 1 ];
		b[ 2 ] = 100 * norm.z + a[ 2 ];


		ang=(i+1)*delta;
		c[0]=radio*cosf(ang*PI/180.0f);
		c[1]=-40.0f;
		c[2]=radio*sinf(ang*PI/180.0f);

		d[0]=c[0];
		d[1]=0.0f;
		d[2]=c[2];

		aux[ 0 ] = Proyecta( d[ 0 ], lightPosition.x );
		aux[ 1 ] = Proyecta( d[ 1 ], lightPosition.y );
		aux[ 2 ] = Proyecta( d[ 2 ], lightPosition.z );

		// Normaliza este vector y multiplicalo por veinte
		norm = CVector( aux[ 0 ], aux[ 1 ], aux[ 2 ] );
		norm = Normaliza( norm );

		c[ 0 ] = 100 * norm.x + d[ 0 ];
		c[ 1 ] = 100 * norm.y + d[ 1 ];
		c[ 2 ] = 100 * norm.z + d[ 2 ];

		//Dibuja cara
		glEnable(GL_TEXTURE_2D);
		glBindTexture(GL_TEXTURE_2D, textura[2].texID);	
			
		glBegin(GL_QUADS);
			calculaNormal(a[0],a[1],a[2],d[0],d[1],d[2],c[0],c[1],c[2]);
			glTexCoord2f(1.0f*(i+1)/6, 1.0f);	glVertex3f(a[0],a[1],a[2]);
			glTexCoord2f(1.0f*i/6, 1.0f);	glVertex3f(d[0],d[1],d[2]);
			glTexCoord2f(1.0f*i/6, 0.0f);;	glVertex3f(c[0],c[1],c[2]);
			glTexCoord2f(1.0f*(i+1)/6, 0.0f);	glVertex3f(b[0],b[1],b[2]);
		glEnd();

		
		glEnable(GL_LIGHTING);
	
		glDisable(GL_LIGHTING);
		o[0]=0.0f;
		o[1]=0.0f;
		o[2]=0.0f;
		//glBindTexture(GL_TEXTURE_2D, textura[8].texID);
		glBegin(GL_TRIANGLES);
			calculaNormal(d[0],d[1],d[2],a[0],a[1],a[2],o[0],o[1],o[2]);
			glTexCoord2f(0.5+0.5*cos( (360*(i))/23 * PI / 180.0f )+j/30, 0.5+0.5*sin( (360*(i))/30 * PI / 180.0f )); glVertex3f(a[0],d[1],a[2]);
			glTexCoord2f(0.5+0.5*cos( (360*(i+1))/23 * PI / 180.0f )+j/30, 0.5+0.5*sin( (360*(i+1))/30 * PI / 180.0f )); glVertex3f(d[0],d[1],d[2]);
			glTexCoord2f(0.5f+j/18, 0.5f);	glVertex3f(o[0],o[1],o[2]);
		glEnd();

		// Tapa inferior

		aux[ 0 ] = Proyecta( o[ 0 ], lightPosition.x );
		aux[ 1 ] = Proyecta( o[ 1 ], lightPosition.y );
		aux[ 2 ] = Proyecta( o[ 2 ], lightPosition.z );

		// Normaliza este vector y multiplicalo por veinte
		norm = CVector( aux[ 0 ], aux[ 1 ], aux[ 2 ] );
		norm = Normaliza( norm );

		o[ 0 ] = 100 * norm.x + o[ 0 ];
		o[ 1 ] = 100 * norm.y + o[ 1 ];
		o[ 2 ] = 100 * norm.z + o[ 2 ];


		LightPos1[ 0 ] = o[ 0 ];
		LightPos1[ 1 ] = 1.0f;
		LightPos1[ 2 ] = o[ 2 ];

		glBegin(GL_TRIANGLES);
			calculaNormal(b[0],b[1],b[2],c[0],c[1],c[2],o[0],o[1],o[2]);
			glTexCoord2f(0.5+0.5*cos( (360*(i+1+j))/30 * PI / 180.0f ), 0.5+0.5*sin( (360*(i+1+j))/30 * PI / 180.0f )); glVertex3f(c[0],c[1],c[2]);
			glTexCoord2f(0.5+0.5*cos( (360*(i+j))/30 * PI / 180.0f ), 0.5+0.5*sin( (360*(i+j))/30 * PI / 180.0f )); glVertex3f(b[0],b[1],b[2]);
			glTexCoord2f(0.5f, 0.5f);	glVertex3f(o[0],o[1],o[2]);
		glEnd();

	glEnable(GL_LIGHTING);


	}
	j=j+0.025;
	if (j>360)
		j=0;

	glDisable(GL_BLEND);		// Turn Blending Off
	glEnable(GL_DEPTH_TEST);	// Turn Depth Testing On
}
void DibujaPersonajes()
{
	glEnable(GL_NORMALIZE);

	cel.TurnOn();

	glPushMatrix();
		glTranslatef(player1.PosicionObj.x, player1.PosicionObj.y+1.7f, player1.PosicionObj.z);
		glRotatef(player1.AngObj, 0.0f, 1.0f, 0.0f);
		glScalef(player1.escalaX,player1.escalaY,player1.escalaZ);
		DibujaBob();
	glPopMatrix();

	glPushMatrix();
		glTranslatef(player2.PosicionObj.x + 10.0f,  player2.PosicionObj.y+3.9f, player2.PosicionObj.z);
		glRotatef(player2.AngObj, 0.0f, 1.0f, 0.0f);
		glScalef(0.7f,0.7f,0.7f);
		DibujaPat();
	glPopMatrix();

	glPushMatrix();
		glTranslatef(player3.PosicionObj.x - 10.0f,  player3.PosicionObj.y, player3.PosicionObj.z);
		glRotatef(player3.AngObj, 0.0f, 1.0f, 0.0f);
		glScalef(0.3f,0.3f,0.3f);
		DibujaPlankton();
	glPopMatrix();

	glPushMatrix();
		glTranslatef(35.0f, 3.0f, -40.0f);
		DibujaPez();
	glPopMatrix();

	glPushMatrix();
		glTranslatef(35.0f, 3.0f, -28.0f);
		glRotatef(180, 0.0f, 1.0f, 0.0f);
		DibujaPez();
	glPopMatrix();

	glPushMatrix();
		glTranslatef(35.0f, 3.0f, -12.0f);
		DibujaPez();
	glPopMatrix();

	glPushMatrix();
		glTranslatef(35.0f, 3.0f, 0.0f);
		glRotatef(180, 0.0f, 1.0f, 0.0f);
		DibujaPez();
	glPopMatrix();

	cel.TurnOff();

	glDisable(GL_LIGHTING);
	glEnable( GL_CULL_FACE );
	glCullFace( GL_FRONT );
	glPolygonMode( GL_FRONT_AND_BACK, GL_LINE );
	glLineWidth( 2.5f );

	glPushMatrix();
		glTranslatef(player1.PosicionObj.x, player1.PosicionObj.y+1.7f, player1.PosicionObj.z);
		glRotatef(player1.AngObj, 0.0f, 1.0f, 0.0f);
		glScalef(player1.escalaX,player1.escalaY,player1.escalaZ);
		DibujaWiredBob();
	glPopMatrix();

	glPushMatrix();
		glTranslatef(player2.PosicionObj.x + 10.0f,  player2.PosicionObj.y+3.9f, player2.PosicionObj.z);
		glRotatef(player2.AngObj, 0.0f, 1.0f, 0.0f);
		glScalef(0.7f,0.7f,0.7f);
		DibujaWiredPat();
	glPopMatrix();

	/*glPushMatrix();
		glTranslatef(player3.PosicionObj.x - 10.0f,  player3.PosicionObj.y, player3.PosicionObj.z);
		glRotatef(player3.AngObj, 0.0f, 1.0f, 0.0f);
		glScalef(0.3f,0.3f,0.3f);
		DibujaWiredPlankton();
	glPopMatrix();

	glPushMatrix();
		glTranslatef(35.0f, 3.0f, -40.0f);
		DibujaWiredPez();
	glPopMatrix();

	glPushMatrix();
		glTranslatef(35.0f, 3.0f, -28.0f);
		glRotatef(180, 0.0f, 1.0f, 0.0f);
		DibujaWiredPez();
	glPopMatrix();

	glPushMatrix();
		glTranslatef(35.0f, 3.0f, -12.0f);
		DibujaWiredPez();
	glPopMatrix();

	glPushMatrix();
		glTranslatef(35.0f, 3.0f, 0.0f);
		glRotatef(180, 0.0f, 1.0f, 0.0f);
		DibujaWiredPez();
	glPopMatrix();*/

	glLineWidth(1.0f);
	glCullFace(GL_BACK);
	glPolygonMode( GL_FRONT_AND_BACK, GL_FILL );
	glEnable(GL_LIGHTING);

	glDisable(GL_NORMALIZE);
}
void DibujaEscena()
{
	g_Load3ds.Render3DSFile(&g_3DModel1e, textureModel1e, 1);
}
int RenderizaEscena(GLvoid)								// Aqui se dibuja todo lo que aparecera en la ventana
{
	TPoint P;

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
	glLoadIdentity();
	
	gluLookAt(10.0f, 45.0f, 45.0f, 10.0f, 25.0f, 0.0f, 0, 1, 0);

	//gluLookAt(0.0f, 100.0f, -20.0f, 0.0f, 0.0f, 0.0f, 0, 0, -1);
	
	/*gluLookAt(player1.PosicionCam.x, player1.PosicionCam.y, player1.PosicionCam.z, 
			  player1.ObjetivoCam.x, player1.ObjetivoCam.y, player1.ObjetivoCam.z, 
			  0.0f, 1.0f, 0.0f);*/
	

	//Se actualizan los parámetros de iluminación
	glLightfv(GL_LIGHT0, GL_POSITION, LightPos);		// Posicion de la luz1
	glLightfv(GL_LIGHT0, GL_AMBIENT,  LightAmb);		// Componente ambiente
	glLightfv(GL_LIGHT0, GL_DIFFUSE,  LightDif);		// Componente difusa
	glLightfv(GL_LIGHT0, GL_SPECULAR, LightSpc);		// Componente especular
	
	ActualizaLuz();
		
	// Se dibuja el modelo con la iluminación desactivada
	// Se renderizan todas las partes oscuras de la escena.
	glDisable(GL_LIGHT0);
	DibujaEscena();
	
	// Se desactiva la máscara de color para renderizar la escena en negro
	glColorMask(GL_FALSE, GL_FALSE, GL_FALSE, GL_FALSE);
	//Se desactiva la escritura en el buffer de profundidad
	glDepthMask(GL_FALSE);
	// Se activa el face culling
	glEnable(GL_CULL_FACE);
	// Se habilita la prueba del buffer stencil
	glEnable(GL_STENCIL_TEST);
	
	//Se inicializa el buffer stencil con una máscara de ceros.
	glStencilFunc(GL_ALWAYS, 0, 0);

	//En este punto se aplica el depth pass (z-pass) o depth fail (z-fail)
	DibujaSombraBob();
	DibujaSombraPat();
	DibujaSombraPlankton();
	glPushMatrix();
		glTranslatef(35.0f, 3.0f, -40.0f);
		DibujaSombraPez();
	glPopMatrix();
	glPushMatrix();
		glTranslatef(35.0f, 3.0f, -28.0f);
		DibujaSombraPez();
	glPopMatrix();
	glPushMatrix();
		glTranslatef(35.0f, 3.0f, -12.0f);
		DibujaSombraPez();
	glPopMatrix();
	glPushMatrix();
		glTranslatef(35.0f, 3.0f, 0.0f);
		DibujaSombraPez();
	glPopMatrix();
			
	// Se habilitan de nuevo los buffers de profundidad y color.
	glDepthFunc(GL_LEQUAL);
	glDepthMask(GL_TRUE);
	glColorMask(GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE);
	//Se reestablecen los valores default de operaciones del stencil buffer y de face culling.
	glStencilOp(GL_KEEP, GL_KEEP, GL_KEEP);
	glCullFace(GL_BACK);
	glDisable(GL_CULL_FACE);

	// Finalmente se dibuja todo lo que está en las áreas iluminadas de la escena. Para eso ahora
	// Los puntos que se dibujan son todos aquellos en donde el buffer stencil es cero, es decir, las
	// partes de la escena que no estan en el volumen de sombra.
	glEnable(GL_LIGHT0);
	glEnable( GL_LIGHT1 );
	glStencilFunc(GL_EQUAL, 0, ~0);

	spot.TurnOn();
	DibujaEscena();
	spot.TurnOff();
	

	// Se desactiva la prueba de profundidad y del buffer stencil ya que no se utilizarán mas.
	glDisable(GL_STENCIL_TEST);
		
	DibujaPersonajes();
	DibujaLuz(lightPosition);

	DibujaLuz2( cLightPosition );
	DibujaTextos();
	
	// Esta parte opcional muestra la silueta extruida que crea el volumen de sombra.
	if(displayVolume == true)
	{
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);     //Para que muestre el volumen en alambrado
		DibujaVolumendeSombra();
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);     //Volvemos al modo sólido de nuevo
	}

	if(running) 
	{	
		idxtp += dtidx;
		if( idxtp >= luzspline.drawp - 10 || idxtp < 2 )
		{ 
			// ¿final o principio?
  			dtidx = -dtidx; // cambia el sentido de la camara
			//esperar = 50;
			ran = rand()%3;
		}
		switch(ran)
		{
			case 0:	
				spline_point(luzspline, idxtp, target);
				break;
			case 1:
				spline_point(luzspline2, idxtp, target);
				break;
			case 2:
				spline_point(luzspline3, idxtp, target);
				break;
		}
	}

	// Trayectoria del spline
	if(trayectoria == 1)
	{
		glDisable(GL_TEXTURE_2D);
		glDisable(GL_LIGHTING);
		glDisable(GL_COLOR_MATERIAL);
		glColor3f(1.0f, 1.0f, 1.0f);
		glBegin(GL_LINE_STRIP);
			for(int i=0; i < luzspline.drawp; i++ ) 
			{
				spline_point( luzspline, i, P );
				glVertex3fv( P );
			}
		glEnd();

		glBegin(GL_LINE_STRIP);
			for(int i=0; i < luzspline.drawp; i++ ) 
			{
				spline_point( luzspline2, i, P );
				glVertex3fv( P );
			}
		glEnd();

		glBegin(GL_LINE_STRIP);
			for(int i=0; i < luzspline.drawp; i++ ) 
			{
				spline_point( luzspline3, i, P );
				glVertex3fv( P );
			}
		glEnd();

		for (int i=0; i<luzspline.tpc; i++ )
		{
			glColor3f(1.0f,1.0f,1.0f);
			glPushMatrix();
				glTranslatef(luzspline.ctrlpoints[i][0],
							luzspline.ctrlpoints[i][1],
							luzspline.ctrlpoints[i][2]);
				glPointSize(10.0f);
				glColor3f(1.0f,0.0f,0.0f);
				glBegin(GL_POINTS);
					glVertex3f(0.0f,0.0f,0.0f);
				glEnd();
							
			glPopMatrix();

			glPushMatrix();
				glTranslatef(luzspline2.ctrlpoints[i][0],
							luzspline2.ctrlpoints[i][1],
							luzspline2.ctrlpoints[i][2]);
				glPointSize(10.0f);
				glColor3f(1.0f,0.0f,0.0f);
				glBegin(GL_POINTS);
					glVertex3f(0.0f,0.0f,0.0f);
				glEnd();
							
			glPopMatrix();

			glPushMatrix();
				glTranslatef(luzspline3.ctrlpoints[i][0],
							luzspline3.ctrlpoints[i][1],
							luzspline3.ctrlpoints[i][2]);
				glPointSize(10.0f);
				glColor3f(1.0f,0.0f,0.0f);
				glBegin(GL_POINTS);
					glVertex3f(0.0f,0.0f,0.0f);
				glEnd();
							
			glPopMatrix();

			glColor3f(1.0f,1.0f,1.0f);
		}
	}

	AnimaBezier();

	glPushMatrix();
		glTranslatef( 35.0f, 4.5f, -34.0f );
		glScalef(0.2f,0.2f,0.2f);
		DibujaPuntosDeControlBP();
		DrawBezierPatch(2);
	glPopMatrix();

	glPushMatrix();
		glTranslatef( 35.0f, 4.5f, -6.0f );
		glScalef(0.2f,0.2f,0.2f);
		DibujaPuntosDeControlBP();
		DrawBezierPatch(2);
	glPopMatrix();

	glPushMatrix();
		glTranslatef(target[0], target[1], target[2]);
		DibujaAlarma();
	glPopMatrix();

	// Colisiones
	ActualizaObjetosDinamicosColision();
	ColisionesPiso();

	CalculateFrameRate();

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
		1,											// 16Bit Stencil Buffer
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

	//variables de fmod
	FMOD_SYSTEM      *system;
	FMOD_RESULT       result;
    unsigned int      version;

	unsigned int ms = 0;
    unsigned int lenms = 0;
    int          playing = 0;
    int          paused = 0;
    int          channelsplaying = 0;

	// Crea la ventana OpenGL
	if (!CreateGLWindow("Computación Gráfica Avanzada",640,480,32))
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
/*
	//Carga de los archivos de audio
    result = FMOD_System_CreateSound(system, "Audio/iteza.mp3", FMOD_HARDWARE, 0, &sound[0]);
    ERRCHECK(result);
    result = FMOD_Sound_SetMode(sound[0], FMOD_LOOP_NORMAL); //Se repetira en un loop
    ERRCHECK(result);										

    result = FMOD_System_CreateSound(system, "Audio/explode.wav", FMOD_SOFTWARE, 0, &sound[1]);
    ERRCHECK(result);

	//Asignación a canales y configuración
	//Música de fondo
	result = FMOD_System_PlaySound(system, FMOD_CHANNEL_FREE, sound[0], 0, &channel[0]);
    ERRCHECK(result);
	result = FMOD_Channel_SetPaused(channel[0], FALSE); //Inicialmente activo
	ERRCHECK(result);
	result = FMOD_Channel_SetVolume(channel[0], 0.3f);
	ERRCHECK(result);

	result = FMOD_System_PlaySound(system, FMOD_CHANNEL_FREE, sound[1], 0, &channel[1]);
    ERRCHECK(result);
	result = FMOD_Channel_SetPaused(channel[1], TRUE); //Inicialmente pausado
	ERRCHECK(result);*/

	TimerInit();

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
			//FMOD_System_Update(system);

			// Dibuja la escena. 
			if (active)								// Si está activo el programa...
			{
				float start=TimerGetTime();

				if (keys[VK_ESCAPE])				// Si se ha presionado ESC
				{
					done=TRUE;						// ESC indica el termino del programa
				}
				else								// De lo contrario, actualiza la pantalla
				{
					RenderizaEscena();				// Dibuja la escena
					if(play || play1 || play2)
					{
						if(tipoAnim == 1)
							animacion(KeyFrame1, maxKF1 , 18);
							animacionPat(KeyFrame2, maxKF1 , 18);
							animacionPlankton(KeyFrame3, maxKF1, 18);
					}
					SwapBuffers(hDC);				// Intercambia los Buffers (Double Buffering)
				}

				if(!ManejaTeclado()) return 0;
				//ManejaTeclado(system, result);

				while(TimerGetTime()<start+float(steps[adjust]*2.0f)) {}	// Desperdicia ciclos si es demasiado rápido
			}
			
		}
	}

	// Finalización del programa
	//DescargaTexturas();
	DescargaModelos();
	DestruyeListas();
	Font.DestroyFont();
	//LiberaSonido(system, result);
	KillGLWindow();									// Destruye la ventana
	return (msg.wParam);							// Sale del programa
}

int ManejaTeclado()
{
	CHAR Buffer[_MAX_PATH]; 

	if(GetKeyState(VK_UP) & 0x80)
	{
		player1.Dir=1;

		ControlPersonaje(3);
		
		if(play==false)
		{
			player1modelo.Angt1    = KeyFrame1[0].Angt1;
			player1modelo.Angt2    = KeyFrame1[0].Angt2;
			player1modelo.Angc1    = KeyFrame1[0].Angc1;
			player1modelo.Angc2    = KeyFrame1[0].Angc2;
			player1modelo.Angbi1   = KeyFrame1[0].Angbi1;
			player1modelo.Angbi2   = KeyFrame1[0].Angbi2;
			player1modelo.Angbib   = KeyFrame1[0].Angbib;
			player1modelo.Angbd1   = KeyFrame1[0].Angbd1;
			player1modelo.Angbd2   = KeyFrame1[0].Angbd2;
			player1modelo.Angbdb   = KeyFrame1[0].Angbdb;
			player1modelo.Angpizq  = KeyFrame1[0].Angpizq;
			player1modelo.Angpder  = KeyFrame1[0].Angpder;
			player1modelo.Angpizqb = KeyFrame1[0].Angpizqb;
			player1modelo.Angpderb = KeyFrame1[0].Angpderb;
			player1modelo.Angpi    = KeyFrame1[0].Angpi;
			player1modelo.Angpd    = KeyFrame1[0].Angpd;
			player1modelo.Xtor     = KeyFrame1[0].Xtor;
			player1modelo.Ytor     = KeyFrame1[0].Ytor;
			player1modelo.Ztor     = KeyFrame1[0].Ztor;

			play=true;
			playIndex=0;
			tipoAnim=1;
			
		}
	}

	if(GetKeyState(VK_DOWN) & 0x80)
	{
		player1.Dir=2;

		ControlPersonaje(4);

		if(play==false)
		{
			player1modelo.Angt1    = KeyFrame1[0].Angt1;
			player1modelo.Angt2    = KeyFrame1[0].Angt2;
			player1modelo.Angc1    = KeyFrame1[0].Angc1;
			player1modelo.Angc2    = KeyFrame1[0].Angc2;
			player1modelo.Angbi1   = KeyFrame1[0].Angbi1;
			player1modelo.Angbi2   = KeyFrame1[0].Angbi2;
			player1modelo.Angbib   = KeyFrame1[0].Angbib;
			player1modelo.Angbd1   = KeyFrame1[0].Angbd1;
			player1modelo.Angbd2   = KeyFrame1[0].Angbd2;
			player1modelo.Angbdb   = KeyFrame1[0].Angbdb;
			player1modelo.Angpizq  = KeyFrame1[0].Angpizq;
			player1modelo.Angpder  = KeyFrame1[0].Angpder;
			player1modelo.Angpizqb = KeyFrame1[0].Angpizqb;
			player1modelo.Angpderb = KeyFrame1[0].Angpderb;
			player1modelo.Angpi    = KeyFrame1[0].Angpi;
			player1modelo.Angpd    = KeyFrame1[0].Angpd;
			player1modelo.Xtor     = KeyFrame1[0].Xtor;
			player1modelo.Ytor     = KeyFrame1[0].Ytor;
			player1modelo.Ztor     = KeyFrame1[0].Ztor;

			play=true;
			playIndex=0;
			tipoAnim=1;
			
		}
	}

	if(GetKeyState(VK_LEFT) & 0x80)
	{
		ControlPersonaje(2);

		if(play==false)
		{
			player1modelo.Angt1    = KeyFrame1[0].Angt1;
			player1modelo.Angt2    = KeyFrame1[0].Angt2;
			player1modelo.Angc1    = KeyFrame1[0].Angc1;
			player1modelo.Angc2    = KeyFrame1[0].Angc2;
			player1modelo.Angbi1   = KeyFrame1[0].Angbi1;
			player1modelo.Angbi2   = KeyFrame1[0].Angbi2;
			player1modelo.Angbib   = KeyFrame1[0].Angbib;
			player1modelo.Angbd1   = KeyFrame1[0].Angbd1;
			player1modelo.Angbd2   = KeyFrame1[0].Angbd2;
			player1modelo.Angbdb   = KeyFrame1[0].Angbdb;
			player1modelo.Angpizq  = KeyFrame1[0].Angpizq;
			player1modelo.Angpder  = KeyFrame1[0].Angpder;
			player1modelo.Angpizqb = KeyFrame1[0].Angpizqb;
			player1modelo.Angpderb = KeyFrame1[0].Angpderb;
			player1modelo.Angpi    = KeyFrame1[0].Angpi;
			player1modelo.Angpd    = KeyFrame1[0].Angpd;
			player1modelo.Xtor     = KeyFrame1[0].Xtor;
			player1modelo.Ytor     = KeyFrame1[0].Ytor;
			player1modelo.Ztor     = KeyFrame1[0].Ztor;

			play=true;
			playIndex=0;
			tipoAnim=1;
			
		}
	}

	if(GetKeyState(VK_RIGHT) & 0x80)
	{
		ControlPersonaje(1);

		if(play==false)
		{
			player1modelo.Angt1    = KeyFrame1[0].Angt1;
			player1modelo.Angt2    = KeyFrame1[0].Angt2;
			player1modelo.Angc1    = KeyFrame1[0].Angc1;
			player1modelo.Angc2    = KeyFrame1[0].Angc2;
			player1modelo.Angbi1   = KeyFrame1[0].Angbi1;
			player1modelo.Angbi2   = KeyFrame1[0].Angbi2;
			player1modelo.Angbib   = KeyFrame1[0].Angbib;
			player1modelo.Angbd1   = KeyFrame1[0].Angbd1;
			player1modelo.Angbd2   = KeyFrame1[0].Angbd2;
			player1modelo.Angbdb   = KeyFrame1[0].Angbdb;
			player1modelo.Angpizq  = KeyFrame1[0].Angpizq;
			player1modelo.Angpder  = KeyFrame1[0].Angpder;
			player1modelo.Angpizqb = KeyFrame1[0].Angpizqb;
			player1modelo.Angpderb = KeyFrame1[0].Angpderb;
			player1modelo.Angpi    = KeyFrame1[0].Angpi;
			player1modelo.Angpd    = KeyFrame1[0].Angpd;
			player1modelo.Xtor     = KeyFrame1[0].Xtor;
			player1modelo.Ytor     = KeyFrame1[0].Ytor;
			player1modelo.Ztor     = KeyFrame1[0].Ztor;

			play=true;
			playIndex=0;
			tipoAnim=1;
			
		}
	}

	if(!(GetKeyState(VK_UP) & 0x80 || GetKeyState(VK_DOWN) & 0x80
		 || GetKeyState(VK_LEFT) & 0x80  || GetKeyState(VK_RIGHT) & 0x80))
	{
		player1.Dir=0;

		play=false;

		player1modelo.Angt1    = 0.0f;
		player1modelo.Angt2    = 0.0f;
		player1modelo.Angc1    = 0.0f;
		player1modelo.Angc2    = 0.0f;
		player1modelo.Angbi1   = 0.0f;
		player1modelo.Angbi2   = 0.0f;
		player1modelo.Angbib   = 0.0f;
		player1modelo.Angbd1   = 0.0f;
		player1modelo.Angbd2   = 0.0f;
		player1modelo.Angbdb   = 0.0f;
		player1modelo.Angpizq  = 0.0f;
		player1modelo.Angpder  = 0.0f;
		player1modelo.Angpizqb = 0.0f;
		player1modelo.Angpderb = 0.0f;
		player1modelo.Angpi    = 0.0f;
		player1modelo.Angpd    = 0.0f;
		player1modelo.Xtor     = 0.0f;
		player1modelo.Ytor     = 0.0f;
		player1modelo.Ztor     = 0.0f;
		
	}
		

	if(keys[VK_PRIOR])
	{
		if(player1.PosicionObj.y < 20.0f)
		{
			player1.PosicionObj.y+=0.4f;
			player1.CamaraPosAlt+=0.4f;
			player1.CamaraObjAlt+=0.4f;

			player1.PosicionCam.y=player1.CamaraPosAlt;
			player1.ObjetivoCam=player1.PosicionObj;
			player1.ObjetivoCam.y=player1.CamaraObjAlt;
		}
	}
	if(keys[VK_NEXT])
	{
		if(player1.PosicionObj.y > 0.0f)
		{
			player1.PosicionObj.y-=0.4f;
			player1.CamaraPosAlt-=0.4f;
			player1.CamaraObjAlt-=0.4f;

			player1.PosicionCam.y=player1.CamaraPosAlt;
			player1.ObjetivoCam=player1.PosicionObj;
			player1.ObjetivoCam.y=player1.CamaraObjAlt;
		}
	}

	if(keys[VK_HOME])
	{
		ControlPersonaje(5);
	}

	if(keys[VK_END])
	{
		ControlPersonaje(6);
	}

	//Controles Pat
	if (GetKeyState('D') & 0x80)
	{
		ControlPersonaje(7);

		if(play1==false)
		{
			player2modelo.Angt1    = KeyFrame2[0].Angt1;
			player2modelo.Angt2    = KeyFrame2[0].Angt2;
			player2modelo.Angbi1   = KeyFrame2[0].Angbi1;
			player2modelo.Angbd1   = KeyFrame2[0].Angbd1;
			player2modelo.Angpizq  = KeyFrame2[0].Angpizq;
			player2modelo.Angpder  = KeyFrame2[0].Angpder;
			player2modelo.Xtor     = KeyFrame2[0].Xtor;
			player2modelo.Ytor     = KeyFrame2[0].Ytor;
			player2modelo.Ztor     = KeyFrame2[0].Ztor;

			play1=true;
			playIndex1=0;
			tipoAnim=1;	
		}
	}

	if (GetKeyState('S') & 0x80)
	{
		player2.Dir=2;

		ControlPersonaje(10);

		if(play1==false)
		{
			player2modelo.Angt1    = KeyFrame2[0].Angt1;
			player2modelo.Angt2    = KeyFrame2[0].Angt2;
			player2modelo.Angbi1   = KeyFrame2[0].Angbi1;
			player2modelo.Angbd1   = KeyFrame2[0].Angbd1;
			player2modelo.Angpizq  = KeyFrame2[0].Angpizq;
			player2modelo.Angpder  = KeyFrame2[0].Angpder;
			player2modelo.Xtor     = KeyFrame2[0].Xtor;
			player2modelo.Ytor     = KeyFrame2[0].Ytor;
			player2modelo.Ztor     = KeyFrame2[0].Ztor;

			play1=true;
			playIndex1=0;
			tipoAnim=1;	
		}
	}

	if (GetKeyState('A') & 0x80)
	{
		ControlPersonaje(8);

		if(play1==false)
		{
			player2modelo.Angt1    = KeyFrame2[0].Angt1;
			player2modelo.Angt2    = KeyFrame2[0].Angt2;
			player2modelo.Angbi1   = KeyFrame2[0].Angbi1;
			player2modelo.Angbd1   = KeyFrame2[0].Angbd1;
			player2modelo.Angpizq  = KeyFrame2[0].Angpizq;
			player2modelo.Angpder  = KeyFrame2[0].Angpder;
			player2modelo.Xtor     = KeyFrame2[0].Xtor;
			player2modelo.Ytor     = KeyFrame2[0].Ytor;
			player2modelo.Ztor     = KeyFrame2[0].Ztor;

			play1=true;
			playIndex1=0;
			tipoAnim=1;	
		}
	}

	if (GetKeyState('W') & 0x80)
	{
		player2.Dir=1;

		ControlPersonaje(9);

		if(play1==false)
		{
			player2modelo.Angt1    = KeyFrame2[0].Angt1;
			player2modelo.Angt2    = KeyFrame2[0].Angt2;
			player2modelo.Angbi1   = KeyFrame2[0].Angbi1;
			player2modelo.Angbd1   = KeyFrame2[0].Angbd1;
			player2modelo.Angpizq  = KeyFrame2[0].Angpizq;
			player2modelo.Angpder  = KeyFrame2[0].Angpder;
			player2modelo.Xtor     = KeyFrame2[0].Xtor;
			player2modelo.Ytor     = KeyFrame2[0].Ytor;
			player2modelo.Ztor     = KeyFrame2[0].Ztor;

			play1=true;
			playIndex1=0;
			tipoAnim=1;	
		}
	}

	if(!(GetKeyState('A') & 0x80 || GetKeyState('S') & 0x80
		 || GetKeyState('D') & 0x80  || GetKeyState('W') & 0x80))
	{
		player2.Dir=0;

		play1=false;

		player2modelo.Angt1    = 0.0f;
		player2modelo.Angt2    = 0.0f;
		player2modelo.Angbi1   = 0.0f;
		player2modelo.Angbd1   = 0.0f;
		player2modelo.Angpizq  = 0.0f;
		player2modelo.Angpder  = 0.0f;
		player2modelo.Xtor     = 0.0f;
		player2modelo.Ytor     = 0.0f;
		player2modelo.Ztor     = 0.0f;	
	}

	//Controles Plankton
	if (GetKeyState('H') & 0x80)
	{
		ControlPersonaje(11);

		if(play2==false)
		{
			player3modelo.Angt1    = KeyFrame3[0].Angt1;
			player3modelo.Angt2    = KeyFrame3[0].Angt2;
			player3modelo.Angbi1   = KeyFrame3[0].Angbi1;
			player3modelo.Angbd1   = KeyFrame3[0].Angbd1;
			player3modelo.Angpizq  = KeyFrame3[0].Angpizq;
			player3modelo.Angpder  = KeyFrame3[0].Angpder;
			player3modelo.Xtor     = KeyFrame3[0].Xtor;
			player3modelo.Ytor     = KeyFrame3[0].Ytor;
			player3modelo.Ztor     = KeyFrame3[0].Ztor;

			play2=true;
			playIndex2=0;
			tipoAnim=1;	
		}
	}

	if (GetKeyState('G') & 0x80)
	{
		player3.Dir = 2.0f;

		ControlPersonaje(14);

		if(play2==false)
		{
			player3modelo.Angt1    = KeyFrame3[0].Angt1;
			player3modelo.Angt2    = KeyFrame3[0].Angt2;
			player3modelo.Angbi1   = KeyFrame3[0].Angbi1;
			player3modelo.Angbd1   = KeyFrame3[0].Angbd1;
			player3modelo.Angpizq  = KeyFrame3[0].Angpizq;
			player3modelo.Angpder  = KeyFrame3[0].Angpder;
			player3modelo.Xtor     = KeyFrame3[0].Xtor;
			player3modelo.Ytor     = KeyFrame3[0].Ytor;
			player3modelo.Ztor     = KeyFrame3[0].Ztor;

			play2=true;
			playIndex2=0;
			tipoAnim=1;	
		}
	}

	if (GetKeyState('F') & 0x80)
	{
		ControlPersonaje(12);

		if(play2==false)
		{
			player3modelo.Angt1    = KeyFrame3[0].Angt1;
			player3modelo.Angt2    = KeyFrame3[0].Angt2;
			player3modelo.Angbi1   = KeyFrame3[0].Angbi1;
			player3modelo.Angbd1   = KeyFrame3[0].Angbd1;
			player3modelo.Angpizq  = KeyFrame3[0].Angpizq;
			player3modelo.Angpder  = KeyFrame3[0].Angpder;
			player3modelo.Xtor     = KeyFrame3[0].Xtor;
			player3modelo.Ytor     = KeyFrame3[0].Ytor;
			player3modelo.Ztor     = KeyFrame3[0].Ztor;

			play2=true;
			playIndex2=0;
			tipoAnim=1;	
		}
	}

	if (GetKeyState('T') & 0x80)
	{
		player3.Dir = 1.0f;

		ControlPersonaje(13);

		if(play2==false)
		{
			player3modelo.Angt1    = KeyFrame3[0].Angt1;
			player3modelo.Angt2    = KeyFrame3[0].Angt2;
			player3modelo.Angbi1   = KeyFrame3[0].Angbi1;
			player3modelo.Angbd1   = KeyFrame3[0].Angbd1;
			player3modelo.Angpizq  = KeyFrame3[0].Angpizq;
			player3modelo.Angpder  = KeyFrame3[0].Angpder;
			player3modelo.Xtor     = KeyFrame3[0].Xtor;
			player3modelo.Ytor     = KeyFrame3[0].Ytor;
			player3modelo.Ztor     = KeyFrame3[0].Ztor;

			play2=true;
			playIndex2=0;
			tipoAnim=1;	
		}
	}

	if(!(GetKeyState('F') & 0x80 || GetKeyState('G') & 0x80
		 || GetKeyState('H') & 0x80  || GetKeyState('T') & 0x80))
	{
		player3.Dir=0.0f;

		play2=false;

		player3modelo.Angt1    = 0.0f;
		player3modelo.Angt2    = 0.0f;
		player3modelo.Angbi1   = 0.0f;
		player3modelo.Angbd1   = 0.0f;
		player3modelo.Angpizq  = 0.0f;
		player3modelo.Angpder  = 0.0f;
		player3modelo.Xtor     = 0.0f;
		player3modelo.Ytor     = 0.0f;
		player3modelo.Ztor     = 0.0f;	
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

	/*if((GetAsyncKeyState(VK_F1)&1) ==1)
	{
		ZeroMemory(&ofn, sizeof(ofn));
		ofn.lStructSize = sizeof(ofn);
		ofn.hwndOwner = g_hMainWindow;
		ofn.lpstrFilter = "Archivos de modelo 3ds (*.3ds)\0*.3ds\0Todos los archivos (*.*)\0*.*\0";
		ofn.lpstrFile = szFileName;
		ofn.nMaxFile = MAX_PATH;
		ofn.lpstrFileTitle = szFileTitle;
		ofn.nMaxFileTitle = MAX_PATH;
		ofn.Flags = OFN_EXPLORER | OFN_FILEMUSTEXIST | OFN_HIDEREADONLY;
		ofn.lpstrDefExt = "3ds";
		ofn.lpstrInitialDir = _getcwd( Buffer, _MAX_PATH );					
			
		if(GetOpenFileName(&ofn))
		{
			g_3DModel1.numOfObjects=0;
			g_3DModel1.numOfMaterials=0;
			
			g_Load3ds.Load3DSFile(szFileName, &g_3DModel1, textureModel1);

		}
	}*/

	if((GetAsyncKeyState(VK_RETURN)&1) ==1)
	{
		if(displayVolume == false)
			displayVolume=true;
		else
			displayVolume=false;
	}

	return TRUE;
}

//Control de sonidos por teclado
void ManejaTeclado(FMOD_SYSTEM *system, FMOD_RESULT result)
{
	if((GetAsyncKeyState(VK_SPACE)&1) ==1) //Otra forma de usar el teclado: Detecta el evento de pulsar la tecla
	{
		result = FMOD_System_PlaySound(system, FMOD_CHANNEL_FREE, sound[1], 0, &channel[1]);
        ERRCHECK(result);
	}
	
}