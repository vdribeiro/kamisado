#include "RGBpixmap.h"
#include "Socket.h"

using namespace std;

// dimensoes e localizacao da janela
#define DIMX 1200
#define DIMY 700
#define INITIALPOS_X 40
#define INITIALPOS_Y 40

// buffer para o rato
#define BUFSIZE 512

// TRUE - FALSE
//#define TRUE  = 1;
//#define FALSE = 0;

//tempo para relogio
//int mili_secs = 1000;
long startTime, nowTime,timeAux, loseTime;
int part,mins;

//animacao camaras
int premiu_1 = FALSE;
int primeira_iteracao = TRUE;
int mili_secs_camaras = 1;
int camara = 1;
int anim_camara = FALSE;
float camx = 0.0;

//animacao para replay
int anima=0;

//animacao pecas
int lin_ini_anim = 0, col_ini_anim = 0;
int mili_secs_pecas = 10, animate = FALSE;
double  obj_radius, obj_rotate, delta_radius, delta_rotate,anim_y=0,anim_x=0,anim_z=0,y_over=FALSE,y2_over=FALSE,x_over=FALSE,z_over=FALSE,iniciou_anim=FALSE;
int linha_dest = 0;
int coluna_dest = 0;
int linha_actual = 1;
int coluna_actual = 1;
int x = 0;
int desl_linha = 0;
int z = 0;
int desl_coluna = 0;
int primeira_jogada = TRUE;
int torre_anterior = -1;
int jogador_anterior = -1;
Tabuleiro * tab_jog_ant;

// aspect ratio da area de visualizacao
float xy_aspect;
int window_w=DIMX;
int window_h=DIMY;

// picking
GLuint selectBuf[BUFSIZE];
int obj_anterior = 0, obj_actual = 0;

// apontadores
Socket *sock;
Parser *parse;
Tabuleiro *tab;
Tabuleiro tab_aux,tab_aux_m;

//jogo
int vitoria=0;
int inicio=0;
int fimjogo=0;
stack<Tabuleiro> undos;
queue<Tabuleiro> movie;
queue<Tabuleiro> movie_aux;

//variaveis para criacao de botao de rotacao e de zoom
GLUI_Rotation *view_rot;
GLUI_Translation * trans_xy;
GLUI_Translation * trans_z;

// matriz de transf. geometrica utilizada pelo botao esferico
float view_rotate[16] = { 1,0,0,0,
                          0,1,0,0,
                          0,0,1,0,
                          0,0,0,1 };

float view_rotate_aux[16] = { 1,0,0,0,
							0,1,0,0,
							0,0,1,0,
							0,0,0,1 };

// vector de posicao utilizado pelo botao de afastamento
float obj_pos[] = { 0.0, 0.0, 0.0 };
float obj_pos_aux[] = { 0.0, 0.0, 0.0 };

// declarações para os tres eixos (cilindros)
double axis_radius_begin =  0.2;
double axis_radius_end   =  0.0;
double axis_lenght       = 16.0;
int axis_nslices = 8;
int axis_nstacks = 1;

// fonte (global) de luz ambiente 
float light_ambient[] = {0.6, 0.6, 0.6, 1.0}; /* Set the background ambient lighting. */

// declarações para as fontes de luz
// LIGHT0
float light0_position[] =  {0.0, 0.0, 0.0, 1.0};
float light0_ambient[] =   {0.0, 0.0, 0.0, 1.0};
float light0_diffuse[] =   {100.0, 100.0, 100.0, 1.0};
float light0_specular[] =  {100.0, 100.0, 100.0, 1.0};
float light0_kc = 0.0;
float light0_kl = 1.0;
float light0_kq = 0.0;
double light0x = 25.0;
double light0y = 400.0;
double light0z = 25.0;
double symb_light0_radius = 1.2;
int symb_light0_slices = 8;
int symb_light0_stacks =8;

// declarações para a fonte de luz dos holofotes;
//Holofote1 = esquerdo-baixo
//Holofote2 = esquerdo-alto
//Holofote3 = direito-baixo
//Holofote4 = direito-alto
float lighth1_position[]  = {0.0, 0.0, 0.0, 1.0};
float lighth2_position[]  = {0.0, 0.0, 0.0, 1.0};
float lighth3_position[]  = {0.0, 0.0, 0.0, 1.0};
float lighth4_position[]  = {0.0, 0.0, 0.0, 1.0};
float lighth_ambient[] =   {0.0, 0.0, 0.0, 1.0};
float lighth_diffuse[] =   {16.0, 16.0, 16.0, 1.0};
float lighth_specular[] =  {16.0, 16.0, 16.0, 1.0};
float lighth_kc = 0.0;
float lighth_kl = 1.0;
float lighth_kq = 0.0;

double lighth1x = 18.0;
double lighth1y = 4.7;
double lighth1z = -3.7;

double lighth2x = 18.0;
double lighth2y = 4.7;
double lighth2z = -11.2;

double lighth3x = 27.0;
double lighth3y = 4.7;
double lighth3z = -3.7;

double lighth4x = 27.0;
double lighth4y = 4.7;
double lighth4z = -11.2;

float spot_direction1[]  = {1.0, -1.0, -1.0 };
float spot_direction2[]  = {1.0, -1.0, 1.0  };
float spot_direction3[]  = {-1.0, -1.0, -1.0};
float spot_direction4[]  = {-1.0, -1.0, 1.0 };

double symb_lighth_radius = 0.2;
int symb_lighth_slices = 8;
int symb_lighth_stacks =8;
float spot_angle=25.5;

// variaveis para a janela
int main_window;
GLUI *glui, *glui2;
GLUquadric* glQ;	// nec. p/ criar sup. quadraticas (cilindros, esferas...)

//display lists
GLuint cena1;

// variaveis de menu
int modo_jogo=0;
int nivel_jogo=0;
int cena_jogo=0;
int vista_jogo=0;
int ceu_jogo=0;
int pause=0;
int spin_jogo=60;
GLUI_Button *undo_button;
GLUI_Button *replay_button;
GLUI_Button *joga_button;
GLUI_Button *pause_button;

// variavel para texturas
RGBpixmap pixmap;

// variaveis para materiais
float indice_brilho = 0.0;
GLfloat mat_ambient_aux[4];
GLfloat mat_diffuse_aux[4];
GLfloat mat_specular_aux[4];
GLfloat mat_emission_aux[4];
GLfloat mat_shininess_aux;

GLfloat mat_ambient[28][4]= {
{ 0.7, 0.7, 0.7, 1.0 }, //0
{ 1.0, 0.6, 0.2, 1.0 }, //1
{ 0.1, 0.5, 1.0, 1.0 }, //2
{ 0.8, 0.5, 1.0, 1.0 }, //3
{ 1.0, 0.1, 1.0, 1.0 }, //4
{ 1.0, 0.9, 0.2, 1.0 }, //5
{ 1.0, 0.3, 0.3, 1.0 }, //6
{ 0.2, 0.9, 0.0, 1.0 }, //7
{ 0.7, 0.5, 0.1, 1.0 }, //8
{ 1.0, 1.0, 1.0, 0.0 }, //9
{ 0.1, 0.1, 0.1, 0.0 }, //10
{ 1.0, 1.0, 1.0, 0.0 }, //11
{ 0.2, 0.9, 0.2, 1.0 }, //12
{ 0.5, 0.5, 0.5, 0.0 }, //13
{ 0.9, 0.5, 0.0, 1.0 }, //14
{ 1.0, 1.0, 1.0, 1.0 }, //15
{ 0.7, 0.7, 0.7, 1.0 }, //16
{ 0.0, 0.5, 0.7, 0.5 }, //17
{ 1.0, 1.0, 1.0, 1.0 }, //18
{ 0.5, 0.5, 0.5, 0.5 }, //19
{ 0.5, 0.2, 0.0, 1.0 }, //20
{ 0.1, 0.5, 0.7, 0.5 }, //21
{ 0.3, 0.3, 0.3, 0.5 }, //22
{ 1.0, 1.0, 1.0, 1.0 }};//23

GLfloat mat_diffuse[28][4]= {
{ 0.7, 0.7, 0.7, 1.0 }, //0
{ 0.4, 0.4, 0.4, 1.0 }, //1
{ 0.4, 0.4, 0.4, 1.0 }, //2
{ 0.4, 0.4, 0.4, 1.0 }, //3
{ 0.4, 0.4, 0.4, 1.0 }, //4
{ 0.4, 0.4, 0.4, 1.0 }, //5
{ 0.4, 0.4, 0.4, 1.0 }, //6
{ 0.4, 0.4, 0.4, 1.0 }, //7
{ 0.4, 0.4, 0.4, 1.0 }, //8
{ 1.0, 1.0, 1.0, 0.0 }, //9
{ 0.1, 0.1, 0.1, 0.0 }, //10
{ 1.0, 1.0, 1.0, 0.0 }, //11
{ 0.4, 0.4, 0.4, 1.0 }, //12
{ 0.1, 0.1, 0.1, 0.5 }, //13
{ 0.7, 0.7, 0.7, 1.0 }, //14
{ 0.4, 0.4, 0.4, 1.0 }, //15
{ 0.7, 0.7, 0.7, 1.0 }, //16
{ 0.5, 0.5, 0.5, 0.5 }, //17
{ 1.0, 1.0, 1.0, 1.0 }, //18
{ 0.5, 0.5, 0.5, 1.0 }, //19
{ 0.4, 0.4, 0.4, 1.0 }, //20
{ 0.3, 0.3, 0.3, 0.5 }, //21
{ 0.2, 0.2, 0.2, 0.5 }, //22
{ 1.0, 1.0, 1.0, 1.0 }};//23

GLfloat mat_specular[28][4]={
{ 0.3, 0.3, 0.3, 1.0 }, //0
{ 0.5, 0.5, 0.5, 1.0 }, //1
{ 0.5, 0.5, 0.5, 1.0 }, //2
{ 0.5, 0.5, 0.5, 1.0 }, //3
{ 0.5, 0.5, 0.5, 1.0 }, //4
{ 0.5, 0.5, 0.5, 1.0 }, //5
{ 0.5, 0.5, 0.5, 1.0 }, //6
{ 0.5, 0.5, 0.5, 1.0 }, //7
{ 0.5, 0.5, 0.5, 1.0 }, //8
{ 1.0, 1.0, 1.0, 0.0 }, //9
{ 0.1, 0.1, 0.1, 0.0 }, //10
{ 1.0, 1.0, 1.0, 0.0 }, //11
{ 0.3, 0.3, 0.3, 1.0 }, //12
{ 0.1, 0.1, 0.1, 0.5 }, //13
{ 0.3, 0.3, 0.3, 1.0 }, //14
{ 0.3, 0.3, 0.3, 1.0 }, //15
{ 0.5, 0.5, 0.5, 1.0 }, //16
{ 1.0, 1.0, 1.0, 0.5 }, //17
{ 1.0, 1.0, 1.0, 1.0 }, //18
{ 0.5, 0.5, 0.5, 1.0 }, //19
{ 0.3, 0.3, 0.3, 1.0 }, //20
{ 0.8, 0.8, 0.8, 0.5 }, //21
{ 0.2, 0.2, 0.2, 0.5 }, //22
{ 1.0, 1.0, 1.0, 1.0 }};//23

GLfloat mat_emission[28][4]={
{ 0.0, 0.0, 0.0, 0.0 }, //0
{ 0.0, 0.0, 0.0, 0.0 }, //1
{ 0.0, 0.0, 0.0, 0.0 }, //2
{ 0.0, 0.0, 0.0, 0.0 }, //3
{ 0.0, 0.0, 0.0, 0.0 }, //4
{ 0.0, 0.0, 0.0, 0.0 }, //5
{ 0.0, 0.0, 0.0, 0.0 }, //6
{ 0.0, 0.0, 0.0, 0.0 }, //7
{ 0.0, 0.0, 0.0, 0.0 }, //8
{ 1.0, 1.0, 1.0, 0.0 }, //9
{ 0.1, 0.1, 0.1, 1.0 }, //10
{ 1.0, 1.0, 1.0, 1.0 }, //11
{ 0.0, 0.0, 0.0, 0.0 }, //12
{ 0.0, 0.0, 0.0, 0.0 }, //13
{ 0.0, 0.0, 0.0, 0.0 }, //14
{ 0.0, 0.0, 0.0, 0.0 }, //15
{ 0.0, 0.0, 0.0, 0.0 }, //16
{ 0.0, 0.0, 0.0, 0.0 }, //17
{ 1.0, 1.0, 1.0, 1.0 }, //18
{ 0.0, 0.0, 0.0, 0.0 }, //19
{ 0.0, 0.0, 0.0, 0.0 }, //20
{ 0.7, 0.7, 0.7, 0.5 }, //21
{ 0.0, 0.0, 0.0, 0.0 }, //22
{ 0.5, 0.5, 0.5, 0.5 }};//23

GLfloat mat_shininess[28][1]={
{128.0}, //0
{ 0.0 }, //1
{ 0.0 }, //2
{ 0.0 }, //3
{ 0.0 }, //4
{ 0.0 }, //5
{ 0.0 }, //6
{ 0.0 }, //7
{ 0.0 }, //8
{ 0.0 }, //9
{ 0.0 }, //10
{ 0.0 }, //11
{ 0.0 }, //12
{ 0.0 }, //13
{ 0.0 }, //14
{64.0 }, //15
{10.0 }, //16
{128.0}, //17
{128.0}, //18
{10.0 }, //19
{10.0 }, //20
{ 0.0 }, //21
{10.0 }, //22
{128.0}};//23

enum mat_enum{
	DEFAULT,C1,C2,C3,C4,C5,C6,C7,C8,TABULEIRO,PRETO,BRANCO,
	RELVA,TRANSPARENTE,TELHA,PAREDE,CHAO,VIDRO,NEON,FERRO,MADEIRA,
	CABINE,FERROB,CEU};	

// tabuleiro
int cores_tabuleiro[8][8]={
	{C1,C2,C3,C4,C5,C6,C7,C8},
	{C6,C1,C4,C7,C2,C5,C8,C3},
	{C7,C4,C1,C6,C3,C8,C5,C2},
	{C4,C3,C2,C1,C8,C7,C6,C5},
	{C5,C6,C7,C8,C1,C2,C3,C4},
	{C2,C5,C8,C3,C6,C1,C4,C7},
	{C3,C8,C5,C2,C7,C4,C1,C6},
	{C8,C7,C6,C5,C4,C3,C2,C1}};

// plano para cortar sphere
GLdouble eqn[4] = {0.0,1.0,0.0,0.0};

// Evaluators
// Tabuleiro
GLfloat ctrlpoints_tab[4][3] = {
{ 0.0, 0.0, 0.0 },
{ 0.0, 0.0, 16.0}, 
{16.0, 0.0, 0.0 },
{16.0, 0.0, 16.0}};

GLfloat nrmlcompon_tab[4][3] = {
{0.0, 1.0, 0.0},
{0.0, 1.0, 0.0}, 
{0.0, 1.0, 0.0},
{0.0, 1.0, 0.0}};

GLfloat colorpoints_tab[4][4] = {
{0.0, 0.0, 0.0, 0},
{0.0, 0.0, 0.0, 0}, 
{0.0, 0.0, 0.0, 0},
{0.0, 0.0, 0.0, 0}};

GLfloat textpoints_tab[4][2] = {
{0.0, 0.0},
{1.0, 0.0}, 
{0.0, 1.0},
{1.0, 1.0}};

// Heliporto
GLfloat ctrlpoints_heli[4][3] = {	
{17.25, 1, -3.75 },
{17.25, 1, -11.25}, 
{27.75, 1, -3.75 },
{27.75, 1, -11.25}};

GLfloat nrmlcompon_heli[4][3] = {
{ 0.0, 1.0, 0.0},
{ 0.0, 1.0, 0.0}, 
{ 0.0, 1.0, 0.0},
{ 0.0, 1.0, 0.0}};

GLfloat colorpoints_heli[4][4] = {
{0.0, 0.0, 0.0, 0},
{0.0, 0.0, 0.0, 0}, 
{0.0, 0.0, 0.0, 0},
{0.0, 0.0, 0.0, 0}};

GLfloat textpoints_heli[4][2] = {
{ 0.0, 0.0},
{ 0.0, 1.0}, 
{ 1.0, 0.0},
{ 1.0, 1.0}};

// Hangar
GLfloat ctrlpoints_han[4][4][3]= {
{{9.3,0.0,0.0  },{9.3,6.0,0.0  },{0.0,6.0,0.0  },{0.0,0.0,0.0  }},
{{9.3,0.0,3.625},{9.3,6.0,3.625},{0.0,6.0,3.625},{0.0,0.0,3.625}},
{{9.3,0.0,7.25 },{9.3,6.0,7.25 },{0.0,6.0,7.25 },{0.0,0.0,7.25 }},
{{9.3,0.0,14.5 },{9.3,6.0,14.5 },{0.0,6.0,14.5 },{0.0,0.0,14.5 }}};

GLfloat nrmlcompon_han[4][3] = {
{ 0.0, 1.0, 0.0},
{ 0.0, 1.0, 0.0}, 
{ 0.0, 1.0, 0.0},
{ 0.0, 1.0, 0.0}};

GLfloat colorpoints_han[4][4] = {
{0.0, 0.0, 0.0, 0},
{0.0, 0.0, 0.0, 0}, 
{0.0, 0.0, 0.0, 0},
{0.0, 0.0, 0.0, 0}};

GLfloat texpts_han[2][2][2] = {
{{0.0, 0.0}, {1.0, 0.0}},
{{0.0, 1.0}, {1.0, 1.0}}
};

