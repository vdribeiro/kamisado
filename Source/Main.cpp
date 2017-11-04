#include "Draw.h"

//copia tabuleiro
void copy_tab() {
	tab_aux.jogador=tab->jogador;
	tab_aux.linha=tab->linha;
	tab_aux.coluna=tab->coluna;
	tab_aux.ntorre=tab->ntorre;
	tab_aux.pc_joga=tab->pc_joga;

	tab_aux.modo=tab->modo;
	tab_aux.nivel=tab->nivel;

	tab_aux.pt1=tab->pt1;
	tab_aux.pt2=tab->pt2;

	tab_aux.tabuleiro=tab->tabuleiro;

	/*
	for (unsigned int i = 0; i < tab->tabuleiro.size(); i++)
	{
		for (unsigned int j = 0; j < tab->tabuleiro[i].size(); j++)
		{
			tab->tabuleiro[i][j]=tab_aux->tabuleiro[i][j];
		}
	}
	*/
}

void copy_tab_inverse() {
	tab->jogador=tab_aux.jogador;
	tab->linha=tab_aux.linha;
	tab->coluna=tab_aux.coluna;
	tab->ntorre=tab_aux.ntorre;
	tab->pc_joga=tab_aux.pc_joga;

	tab->modo=tab_aux.modo;
	tab->nivel=tab_aux.nivel;

	tab->pt1=tab_aux.pt1;
	tab->pt2=tab_aux.pt2;

	tab->tabuleiro=tab_aux.tabuleiro;

	/*
	for (unsigned int i = 0; i < tab->tabuleiro.size(); i++)
	{
		for (unsigned int j = 0; j < tab->tabuleiro[i].size(); j++)
		{
			tab->tabuleiro[i][j]=tab_aux->tabuleiro[i][j];
		}
	}
	*/
}

//disconnect
void off() {
	try {
		sock->desligar();
	}catch(Exception e)	{
		cerr << "Erro em socket." << endl;
		system("pause");
	}
}

void loadTime() {
	//tempo do relogio
	if (pause==0) {
		nowTime=time(NULL);
	}

	timeAux = nowTime - startTime;

	long tempol = nowTime - loseTime;
	//tempo de jogada
	if (tempol==spin_jogo) {
		joga_button->disable();
		if (tab->jogador==1) {
			tab->pt2++;
			vitoria=2;
			fimjogo=1;
		} else {
			tab->pt1++;
			vitoria=1;
			fimjogo=1;
		}
	}

	if (timeAux==10) {
		startTime=time(NULL);
		part++;
		if (part==6) {
			part=0;
			mins++;
			if (mins==100) {
				mins=0;
			}
		}
	}
}


//Gerar Display Lists
void DLists() {
	cena1 = glGenLists (1);
	glNewList(cena1, GL_COMPILE);
		chao();
		arvores();
		hospital();
		heliporto();
		holofotes();
		torre();
		glEnable(GL_AUTO_NORMAL);
		hangar();
		glDisable(GL_AUTO_NORMAL);
	glEndList();

	cout << "Display Lists geradas" << endl;
}

//funcao das checkboxs
void callback(int ID){
	switch (ID){
		case 1: tab->modo=modo_jogo+1;
			if (tab->modo==1) {
				joga_button->disable();
			} else if (tab->modo==2) {
				if (tab->jogador==1) {
					joga_button->disable();
				} else {
					joga_button->enable();
				}
			} else if (tab->modo==3) {
				joga_button->enable();
			} else {
				undo_button->disable();
				joga_button->disable();
			}
			if (vitoria!=0) {
				joga_button->disable();
				fimjogo=1;
			}
			break;
		case 2: tab->nivel=nivel_jogo+1;
			break;
		case 3: //mudanca de cena
			//faz reset a camera
			for (int f=0;f!=16;f++)
				view_rotate[f]=view_rotate_aux[f];
			
			for (int q=0;q!=3;q++)
				obj_pos[q]=obj_pos_aux[q];
			break;
		case 4: //funcao vistas
			//faz reset a camera
			for (int f=0;f!=16;f++)
				view_rotate[f]=view_rotate_aux[f];

			for (int q=0;q!=3;q++)
				obj_pos[q]=obj_pos_aux[q];
			break;
		case 5: //novo jogo
			startTime = time(NULL);
			nowTime = time(NULL);
			loseTime = time(NULL);
			part=0;
			mins=0;
			vitoria=0;
			fimjogo=0;
			pause=0;
			inicio=0;

			while (!movie.empty())
				movie.pop();

			while (!undos.empty())
				undos.pop();

			tab->reset();
			tab->modo=modo_jogo+1;
			tab->nivel=nivel_jogo+1;
			parse->inicia();
			//tab->printall();

			undo_button->enable();
			if (tab->modo==3)
				joga_button->enable();
			else
				joga_button->disable();

			break;
		case 6: //undo
			if (!undos.empty()) {
				if (undos.size()>1) {
					loseTime = time(NULL);
					undos.pop();
					movie.pop();
					tab_aux=undos.top();
					copy_tab_inverse();
					//tab->printall();
					if (tab->modo==3) {
						joga_button->enable();
					} else if (tab->modo==2) {
						if (tab->jogador==1) {
							joga_button->disable();
						} else {
							joga_button->enable();
						}
					} else {
						joga_button->disable();
					}
				}
			}
			break;
		case 7: //jogada do pc
			if (inicio==0) {
				inicio=1;
				copy_tab();
				undos.push(tab_aux);
				movie.push(tab_aux);
			}
			if (vitoria!=0) break;
			if (parse->deadlock()==false) {
				loseTime = time(NULL);

				tab_jog_ant->reset();
				tab_jog_ant->coluna = tab->coluna;
				tab_jog_ant->linha = tab->linha;
				tab_jog_ant->ntorre = tab->ntorre;
				tab_jog_ant->jogador = tab->jogador;
				tab_jog_ant->tabuleiro = tab->tabuleiro;
				animate = TRUE;

				parse->jogada_pc();
				obj_actual = ((tab->linha * 10) + tab->coluna);

				primeira_jogada = FALSE;

				vitoria=parse->ganhou();
				if (vitoria!=0) {
					joga_button->disable();
					if (vitoria==1) {
						tab->pt1++;
					} else tab->pt2++;
					fimjogo=1;
				}
				//tab->printall();
				copy_tab();
				undos.push(tab_aux);
				movie.push(tab_aux);
			} else {
				cout << "Deadlock - passa a vez" << endl;
			}

			if (tab->modo==2) {
				joga_button->disable();
				tab->pc_joga=0;
			} else {
				if (tab->pc_joga==0)
					tab->pc_joga=1;
				else tab->pc_joga=0;
			}
			break;
		case 8: //replay
			if (!movie.empty()) {
				if (movie.size()>1) {
					loseTime = time(NULL);
					pause=1;
					undo_button->disable();
					joga_button->disable();
					replay_button->disable();
					pause_button->disable();
					movie_aux=movie;
					anima=TRUE;
				}
			}
			//pause=0;
			//undo_button->enable();
			//joga_button->enable();
			break;
		case 9:
			off();
			exit(0);
			break;
		case 10:
			if (pause==0) {
				pause=1;
				undo_button->disable();
				joga_button->disable();
				replay_button->disable();
			} else {
				pause=0;
				undo_button->enable();
				joga_button->enable();
				replay_button->enable();
				loseTime+= time(NULL)-nowTime;
				startTime+=time(NULL)-nowTime;
			}
			break;
		case 11:
			if (spin_jogo>600) spin_jogo=600;
			if (spin_jogo<10) spin_jogo=10;
			break;
		default:
			break;
	}
}




void display(void)
{
	if (vitoria==0)
		loadTime();

	if (anima==TRUE)
		loseTime = time(NULL);

	if (animate==TRUE)
		loseTime = time(NULL);

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
 
	// inicializacoes da matriz de visualizacao
	glMatrixMode( GL_PROJECTION );
	glLoadIdentity();
	
	//selecciona camara a ser usada
	switch_camaras();

	glMultMatrixf( view_rotate );

	// permissao de atribuicao directa de cores
	// para objectos que nao tem material atribuido, como
	// e' o caso dos eixos e da esfera que simboliza a fonte de luz...
	glColorMaterial(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE);
	glEnable(GL_COLOR_MATERIAL);

	// Funcoes para definicao dessas propriedades em glQ
	gluQuadricDrawStyle(glQ, GLU_FILL);		// GLU_FILL, GLU_LINE, GLU_SILHOUETTE, GLU_POINT
	gluQuadricNormals(glQ, GLU_SMOOTH);		// GLU_NONE, GLU_FLAT, GLU_SMOOTH
	gluQuadricOrientation(glQ, GLU_OUTSIDE);	// GLU_OUTSIDE, GLU_INSIDE
	gluQuadricTexture(glQ, GL_FALSE);		// GL_TRUE, GL_FALSE
	
	// Actualizacao da posicao das fontes de luz e das esfera que as simbolizam
	luzes();

	// inibicao de atribuicao directa de cores; os objectos que se seguem DEVEM
	// possuir materiais associados
	glDisable(GL_COLOR_MATERIAL);
	glEnable(GL_LINE_SMOOTH);
	gluQuadricTexture(glQ, GL_TRUE);

	//desenha relogio
	glPushMatrix();
	glTranslatef(5.5,12.7,3.7);
	drawClock(timeAux,part,mins,tab->pt1,tab->pt2);

	//replay?
	if ((anima==TRUE) && (animate == FALSE)){
		if (movie.size()==1) {
			movie=movie_aux;
			anima=FALSE;
			pause=0;
			tab->modo=modo_jogo+1;
			tab->nivel=nivel_jogo+1;
			undo_button->enable();
			replay_button->enable();
			pause_button->enable();
			startTime+=time(NULL)-nowTime;

			if (tab->modo==3) {
				joga_button->enable();
			} else {
				if (tab->modo==2) {
					if (tab->jogador==1) {
						joga_button->disable();
					} else {
						joga_button->enable();
					}
				} else {
					joga_button->disable();
				}
			}
		} else {
			tab_jog_ant->reset();
			tab_jog_ant->coluna = movie.front().coluna;
			tab_jog_ant->linha = movie.front().linha;
			tab_jog_ant->ntorre = movie.front().ntorre;
			tab_jog_ant->jogador = movie.front().jogador;
			tab_jog_ant->tabuleiro = movie.front().tabuleiro;

			movie.pop();

			obj_actual = ((movie.front().linha * 10) + movie.front().coluna);

			tab_aux=movie.front();

			copy_tab_inverse();

			printf("\nentrou %d\n",movie.size());
			animate = TRUE;

		}
	}

	drawTabuleiro(GL_RENDER);
	glPopMatrix();

	//AVISO: Este codigo para baixo tem de estar obrigatoriamente no final

	//fundo e cena
	drawBackground(ceu_jogo);
	drawScene();

	//pause
	if (pause==1) {
		glMatrixMode(GL_PROJECTION);
		glLoadIdentity();
		gluOrtho2D(0,DIMX,0,DIMY);
		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();
		glEnable(GL_COLOR_MATERIAL);
		glPushMatrix();
		glColor3f(1.0,1.0,0.0);
		glTranslatef(1.0,10.0,0.0);
		glLineWidth(3.0);
		glutStrokeCharacter(GLUT_STROKE_ROMAN, 'P');
		glutStrokeCharacter(GLUT_STROKE_ROMAN, 'A');
		glutStrokeCharacter(GLUT_STROKE_ROMAN, 'U');
		glutStrokeCharacter(GLUT_STROKE_ROMAN, 'S');
		glutStrokeCharacter(GLUT_STROKE_ROMAN, 'E');
		glDisable(GL_COLOR_MATERIAL);
		glPopMatrix();

	}

	// swapping the buffers causes the rendering above to be shown
	glutSwapBuffers();
   
	glFlush();
}

// ACÇÃO DO PICKING
void pickingAction(GLuint answer) {
	printf("%d\n", answer);
}

// processa os hits no picking
void processHits (GLint hits, GLuint buffer[]) {
	GLuint *ptr = buffer;
	GLuint mindepth = 0xFFFFFFFF;
	GLuint *answer=NULL;
	GLuint nn;

	for (int i=0;i<hits;i++) {
		int num = *ptr; ptr++;
		GLuint z1 = *ptr; ptr++;
		ptr++;
		if (z1 < mindepth && num>0) {
			mindepth = z1;
			answer = ptr;
			nn=num;
		}
		for (int j=0; j < num; j++) 
			ptr++;
	}
	
	obj_anterior = obj_actual;
	
	// existe uma resposta
	if (answer!=NULL) {
		obj_actual = (*answer);
		pickingAction(*answer);
	}
	
}

struct g_mouseState{
	bool leftButton;
	bool rightButton;
	bool middleButton;
	int x;
	int y;
} MouseState;

/* Mouse handling */
void processMouse(int button, int state, int x, int y)
{
	if (animate==TRUE) return;
	if (fimjogo==1) return;
	GLint hits;
	GLint viewport[4];

	// update our button state
	if(button == GLUT_LEFT_BUTTON) {
		if(state == GLUT_DOWN)
			MouseState.leftButton = true;
		else
			MouseState.leftButton = false;
	}
	if(button == GLUT_RIGHT_BUTTON) {
		if(state == GLUT_DOWN)
			MouseState.rightButton = true;
		else
			MouseState.rightButton = false;
	}
	if(button == GLUT_MIDDLE_BUTTON) {
		if(state == GLUT_DOWN)
			MouseState.middleButton = true;
		else
			MouseState.middleButton = false;
	}

	// update our position so we know a delta when the mouse is moved
	MouseState.x = x;
	MouseState.y = y;
	
	if (MouseState.leftButton && !MouseState.rightButton && !MouseState.middleButton) {
		/* obrigatorio para o picking */
		// obter o viewport actual
		glGetIntegerv(GL_VIEWPORT, viewport);

		glSelectBuffer (BUFSIZE, selectBuf);
		glRenderMode (GL_SELECT);

		// inicia processo de picking
		glInitNames();
		glMatrixMode (GL_PROJECTION);
		glPushMatrix ();

		//  cria uma região de 5x5 pixels em torno do click do rato para o processo de picking
		glLoadIdentity ();
		gluPickMatrix ((GLdouble) x, (GLdouble) (window_h - y), 1.0, 1.0, viewport);

		drawTabuleiro(GL_SELECT);

		glMatrixMode (GL_PROJECTION);
		glPopMatrix ();
		glFlush ();

		hits = glRenderMode(GL_RENDER);
		processHits(hits, selectBuf);
		
		if (tab->modo==1) {

			if (obj_actual>100) {
				if (inicio==0) {
					tab->ntorre=obj_actual%10;
					inicio=1;
					copy_tab();
					undos.push(tab_aux);
					movie.push(tab_aux);
				}
			} else {
				if (inicio==0) return;
				int lin = (obj_actual/10);
				int col = (obj_actual%10);

				if (parse->valida_jogada(lin,col)) {
					if (parse->deadlock()==false) {
						tab_jog_ant->reset();
						tab_jog_ant->coluna = tab->coluna;
						tab_jog_ant->linha = tab->linha;
						tab_jog_ant->ntorre = tab->ntorre;
						tab_jog_ant->jogador = tab->jogador;
						tab_jog_ant->tabuleiro = tab->tabuleiro;
						animate = TRUE;
						parse->jogada_humana(lin,col);
						primeira_jogada = FALSE;
						vitoria=parse->ganhou();
						if (vitoria!=0) {
							joga_button->disable();
							if (vitoria==1) {
								tab->pt1++;
							} else tab->pt2++;
							fimjogo=1;
						}
						//tab->printall();
						copy_tab();
						undos.push(tab_aux);
						movie.push(tab_aux);
					} else {
						cout << "Deadlock - passa a vez" << endl;
					}

					if (tab->pc_joga==0)
						tab->pc_joga=1;
					else tab->pc_joga=0;


				}
			}
		}

		if ((tab->modo==2) || (tab->pc_joga==0)) {

			if (obj_actual>100) {
				if (inicio==0) {
					tab->ntorre=obj_actual%10;
					inicio=1;
					copy_tab();
					undos.push(tab_aux);
					movie.push(tab_aux);
				}
			} else {
				if (inicio==0) return;
				int lin = (obj_actual/10);
				int col = (obj_actual%10);

				if (parse->valida_jogada(lin,col)) {
					if (parse->deadlock()==false) {
						tab_jog_ant->reset();
						tab_jog_ant->coluna = tab->coluna;
						tab_jog_ant->linha = tab->linha;
						tab_jog_ant->ntorre = tab->ntorre;
						tab_jog_ant->jogador = tab->jogador;
						tab_jog_ant->tabuleiro = tab->tabuleiro;
						animate = TRUE;
						parse->jogada_humana(lin,col);
						primeira_jogada = FALSE;
						loseTime = time(NULL);
						vitoria=parse->ganhou();
						if (vitoria!=0) {
							joga_button->disable();
							if (vitoria==1) {
								tab->pt1++;
							} else tab->pt2++;
							fimjogo=1;
						}
						//tab->printall();
						copy_tab();
						undos.push(tab_aux);
						movie.push(tab_aux);
					} else {
						cout << "Deadlock - passa a vez" << endl;
					}

					tab->pc_joga=1;
					joga_button->enable();

				}
			}
		}
	}
	
}

void processMouseMoved(int x, int y)
{
	
	// pedido de refrescamento da janela
	glutPostRedisplay();				

}

void processPassiveMouseMoved(int x, int y)
{

	// pedido de refrescamento da janela
	glutPostRedisplay();				
}

void reshape(int w, int h)
{
	int tx, ty, tw, th;

	window_w = w;	//variaveis globais; window_h e' usado em processMouse()
	window_h = h;

	GLUI_Master.get_viewport_area( &tx, &ty, &tw, &th );
	glViewport( tx, ty, tw, th );
	// ver, nos manuais, a funcao GLUI_Master.auto_set_viewport()

	xy_aspect = (float)tw / (float)th;

	glutPostRedisplay();

}

void keyboard(unsigned char key, int x, int y)
{
	switch (key) {
		case 27:		// tecla de escape termina o programa
			exit(0);
			break;	
		default:
			break;
	}
}


void myGlutIdle( void )
{
  /* According to the GLUT specification, the current window is 
     undefined during an idle callback.  So we need to explicitly change
     it if necessary */
  if ( glutGetWindow() != main_window ) 
    glutSetWindow(main_window);  


  glutPostRedisplay();

  /****************************************************************/
  /*            This demonstrates GLUI::sync_live()               */
  /*   We change the value of a variable that is 'live' to some   */
  /*   control.  We then call sync_live, and the control          */
  /*   associated with that variable is automatically updated     */
  /*   with the new value.  This frees the programmer from having */
  /*   to always remember which variables are used by controls -  */
  /*   simply change whatever variables are necessary, then sync  */
  /*   the live ones all at once with a single call to sync_live  */
  /****************************************************************/

//  glui->sync_live();

}

void inicializacao()
{
	glQ = gluNewQuadric();

	glFrontFace(GL_CCW);		// Front faces defined using a counterclockwise rotation
	glDepthFunc(GL_LEQUAL);		// Por defeito e GL_LESS
	glEnable(GL_DEPTH_TEST);	// Use a depth (z) buffer to draw only visible objects

	//glEnable(GL_CULL_FACE);		// Face Culling para aumentar a velocidade
	//glCullFace(GL_BACK);		// GL_FRONT, GL_BACK, GL_FRONT_AND_BACK

	// Define que modelo de iluminacao utilizar; consultar o manual de referencia
	glLightModelfv(GL_LIGHT_MODEL_AMBIENT, light_ambient);  // define luz ambiente
	glLightModelf (GL_LIGHT_MODEL_TWO_SIDE, GL_FALSE);
	glLightModeli (GL_LIGHT_MODEL_LOCAL_VIEWER, 1);

	// por defeito a cor de fundo e o preto
	glClearColor(0.0,0.0,0.0,1.0);    // cor de fundo

	// declaracoes para a fonte luz GL_LIGHT0
	glLightfv(GL_LIGHT0, GL_AMBIENT, light0_ambient);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, light0_diffuse);
	glLightfv(GL_LIGHT0, GL_SPECULAR, light0_specular);
	glLightf(GL_LIGHT0, GL_CONSTANT_ATTENUATION,  light0_kc);
	glLightf(GL_LIGHT0, GL_LINEAR_ATTENUATION,    light0_kl);
	glLightf(GL_LIGHT0, GL_QUADRATIC_ATTENUATION, light0_kq);
	//glLightfv(GL_LIGHT0, GL_POSITION, light0_position);

	// declaracoes para a fonte luz GL_LIGHT1
	glLightfv(GL_LIGHT1, GL_AMBIENT, lighth_ambient);
	glLightfv(GL_LIGHT1, GL_DIFFUSE, lighth_diffuse);
	glLightfv(GL_LIGHT1, GL_SPECULAR, lighth_specular);
	glLightf(GL_LIGHT1, GL_CONSTANT_ATTENUATION,  lighth_kc);
	glLightf(GL_LIGHT1, GL_LINEAR_ATTENUATION,    lighth_kl);
	glLightf(GL_LIGHT1, GL_QUADRATIC_ATTENUATION, lighth_kq);

	// declaracoes para a fonte luz GL_LIGHT2
	glLightfv(GL_LIGHT2, GL_AMBIENT, lighth_ambient);
	glLightfv(GL_LIGHT2, GL_DIFFUSE, lighth_diffuse);
	glLightfv(GL_LIGHT2, GL_SPECULAR, lighth_specular);
	glLightf(GL_LIGHT2, GL_CONSTANT_ATTENUATION,  lighth_kc);
	glLightf(GL_LIGHT2, GL_LINEAR_ATTENUATION,    lighth_kl);
	glLightf(GL_LIGHT2, GL_QUADRATIC_ATTENUATION, lighth_kq);

	// declaracoes para a fonte luz GL_LIGHT3
	glLightfv(GL_LIGHT3, GL_AMBIENT, lighth_ambient);
	glLightfv(GL_LIGHT3, GL_DIFFUSE, lighth_diffuse);
	glLightfv(GL_LIGHT3, GL_SPECULAR, lighth_specular);
	glLightf(GL_LIGHT3, GL_CONSTANT_ATTENUATION,  lighth_kc);
	glLightf(GL_LIGHT3, GL_LINEAR_ATTENUATION,    lighth_kl);
	glLightf(GL_LIGHT3, GL_QUADRATIC_ATTENUATION, lighth_kq);

	// declaracoes para a fonte luz GL_LIGHT4
	glLightfv(GL_LIGHT4, GL_AMBIENT, lighth_ambient);
	glLightfv(GL_LIGHT4, GL_DIFFUSE, lighth_diffuse);
	glLightfv(GL_LIGHT4, GL_SPECULAR, lighth_specular);
	glLightf(GL_LIGHT4, GL_CONSTANT_ATTENUATION,  lighth_kc);
	glLightf(GL_LIGHT4, GL_LINEAR_ATTENUATION,    lighth_kl);
	glLightf(GL_LIGHT4, GL_QUADRATIC_ATTENUATION, lighth_kq);

	// Permitir calculos de iluminacao
	glEnable(GL_LIGHTING);
	// "Acender" as fontes de luz
	glEnable(GL_LIGHT0);

	//Declaracoes para shading
	glShadeModel(GL_SMOOTH);			// GL_FLAT / GL_SMOOTH
	glPolygonMode(GL_FRONT, GL_FILL);	// preence a face da frente dos poligonos
	//glPolygonMode(GL_FRONT, GL_LINE);	// desenha arestas dos poligonos

	// Texturas a utilizar (ver classe RGBpixmap)
	myTexturas();

	//Display Lists
	DLists();

	myInitTransforms();					// inicializa transf. geom. da animacao.
}

int main(int argc, char* argv[])
{
	try{
		sock = new Socket();
		sock->connectar();
	}catch(Exception e)
	{
		cerr << "Erro em socket." << endl;
		system("pause");
		return 0;
	}

	tab = new Tabuleiro();
	tab_jog_ant = new Tabuleiro();
	tab_aux.reset();
	tab_aux.pt1=0;
	tab_aux.pt2=0;

	try {
		parse = new Parser(sock,tab);
		parse->inicia();
	} catch(Exception e)	{
		cerr << "Erro no parser." << endl;
		system("pause");
		return 0;
	}

	copy_tab();
	//undos.push(tab_aux);
	//movie.push(tab_aux);

	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGBA);
	glutInitWindowSize (DIMX, DIMY);
	glutInitWindowPosition (INITIALPOS_X, INITIALPOS_Y);
	main_window = glutCreateWindow ("LAIG projecto 3");

	glutDisplayFunc(display);
	GLUI_Master.set_glutReshapeFunc(reshape);
	GLUI_Master.set_glutKeyboardFunc (keyboard);
	GLUI_Master.set_glutMouseFunc(processMouse);
	glutMotionFunc(processMouseMoved);
	glutPassiveMotionFunc(processPassiveMouseMoved);   
	GLUI_Master.set_glutSpecialFunc( NULL );

	/*** Create the bottom subwindow ***/
	glui2 = GLUI_Master.create_glui_subwindow( main_window, GLUI_SUBWINDOW_TOP );
	/*
		GLUI_SUBWINDOW_RIGHT
		GLUI_SUBWINDOW_LEFT
		GLUI_SUBWINDOW_TOP
		GLUI_SUBWINDOW_BOTTOM
	*/
	glui2->set_main_gfx_window( main_window );

	/* Some menu functions - http://glui.sourceforge.net/
	glui2->add_column( false );
	glui2->add_button("botao");
	glui2->add_edittext("text");
	glui2->add_listbox("lista");
	glui2->add_panel("panel");
	glui2->add_rollout("rollout");
	glui2->add_spinner("spinner");
	*/

	//Camara
	GLUI_Rollout *controlo = glui2->add_rollout("Camara",0);

	view_rot = glui2->add_rotation_to_panel(controlo, "Rotacao", view_rotate );
    view_rot->set_spin( 1.0 );
	glui2->add_column_to_panel(controlo, false );
    
    trans_xy = glui2->add_translation_to_panel(controlo, "Translacao", GLUI_TRANSLATION_XY, &obj_pos[0] );
    trans_xy->set_speed( 0.5 );
	glui2->add_column_to_panel(controlo, false );

	trans_z = glui2->add_translation_to_panel(controlo, "Zoom", GLUI_TRANSLATION_Z, &obj_pos[2] );
    trans_z->set_speed( 0.5 );

	glui2->add_column( false );

	//Opcoes
	GLUI_Rollout *opcoes = glui2->add_rollout("Menu",0);
	GLUI_Button *new_game_button = glui2->add_button_to_panel(opcoes,"NEW GAME", 5,callback);
	undo_button= glui2->add_button_to_panel(opcoes,"UNDO", 6,callback);
	pause_button = glui2->add_button_to_panel(opcoes,"PAUSE", 10,callback);
	joga_button = glui2->add_button_to_panel(opcoes,"JOGADA", 7,callback);
	replay_button = glui2->add_button_to_panel(opcoes,"REPLAY", 8,callback);
	GLUI_Button *exit_button = glui2->add_button_to_panel(opcoes,"EXIT",9,callback);

	glui2->add_column( false );

	//Jogo
	GLUI_Rollout *jogo = glui2->add_rollout("Jogo",0);
	GLUI_Panel * mode = glui2->add_panel_to_panel(jogo,"Modo de jogo");
	GLUI_RadioGroup * modo = glui2->add_radiogroup_to_panel(mode,&modo_jogo,1,callback);
	glui2->add_radiobutton_to_group( modo, "HUMANO VS HUMANO");
	glui2->add_radiobutton_to_group( modo, "HUMANO VS PC");
	glui2->add_radiobutton_to_group( modo, "PC VS PC");

	GLUI_Spinner *spin = glui2->add_spinner_to_panel(jogo,"Tempo de jogada",2,&spin_jogo,11,callback);

	glui2->add_column_to_panel(jogo,false);
	
	GLUI_Panel * level = glui2->add_panel_to_panel(jogo,"Nivel do jogo");
	GLUI_RadioGroup * nivel = glui2->add_radiogroup_to_panel(level,&nivel_jogo,2,callback);
	glui2->add_radiobutton_to_group( nivel,"FACIL");
	glui2->add_radiobutton_to_group( nivel,"DIFICIL");

	glui2->add_column( false );

	//Cenario e Vistas
	GLUI_Rollout *cena = glui2->add_rollout("Ambiente",0);
	GLUI_Panel * cenario = glui2->add_panel_to_panel(cena,"Cenario");
	GLUI_RadioGroup * listacena = glui2->add_radiogroup_to_panel(cenario,&cena_jogo,3,callback);
	glui2->add_radiobutton_to_group( listacena,"Vazio");
	glui2->add_radiobutton_to_group( listacena,"Time Capsule");
	glui2->add_radiobutton_to_group( listacena,"Dojo Taekwondo");

	glui2->add_column_to_panel(cena,false);

	GLUI_Panel * ceus = glui2->add_panel_to_panel(cena,"Fundo");
	GLUI_RadioGroup * sky = glui2->add_radiogroup_to_panel(ceus, &ceu_jogo,11,callback);
	glui2->add_radiobutton_to_group( sky, "Limpo");
	glui2->add_radiobutton_to_group( sky, "Entardecer");
	glui2->add_radiobutton_to_group( sky, "Dia");
	glui2->add_radiobutton_to_group( sky, "Sobre as Nuvens");
	glui2->add_radiobutton_to_group( sky, "Galactico");
	glui2->add_radiobutton_to_group( sky, "Espacial");

	glui2->add_column_to_panel(cena,false);

	GLUI_Panel * vistas = glui2->add_panel_to_panel(cena,"Vistas");
	GLUI_RadioGroup * radiog = glui2->add_radiogroup_to_panel(vistas, &vista_jogo,4,callback);
	glui2->add_radiobutton_to_group( radiog, "Tabuleiro");
	glui2->add_radiobutton_to_group( radiog, "Jogador 1");
	glui2->add_radiobutton_to_group( radiog, "Jogador 2");
	glui2->add_radiobutton_to_group( radiog, "Tempo e Pontuacao");
	glui2->add_radiobutton_to_group( radiog, "Frente");
	glui2->add_radiobutton_to_group( radiog, "Tras");

	glui2->add_column( false );

	GLUI_Rollout *creditos = glui2->add_rollout("Creditos",0);
	glui2->add_statictext_to_panel(creditos,"LAIG - 3MIEIC1");
	glui2->add_statictext_to_panel(creditos,"Vitor Ribeiro");
	glui2->add_statictext_to_panel(creditos,"ei07038");
	glui2->add_statictext_to_panel(creditos,"Tadeu Marques");
	glui2->add_statictext_to_panel(creditos,"ei08031");

	glui2->add_column( false );

	//FULL SCREEN
	//glutFullScreen();

	/* We register the idle callback with GLUI, not with GLUT */
	GLUI_Master.set_glutIdleFunc( myGlutIdle );

	startTime = time(NULL);
	nowTime = time(NULL);
	loseTime = time(NULL);
	part=0;
	mins=0;
	vitoria=0;
	fimjogo=0;
	pause=0;
	joga_button->disable();

	glutTimerFunc(mili_secs_camaras, myUpdateTransforms_camaras, 0);
	glutTimerFunc(mili_secs_pecas, myUpdateTransforms, 0);
	
    inicializacao();

	glutMainLoop();

	off();

	return 0;
}
