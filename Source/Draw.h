#include "Resource.h"

//Eixos
void draweixos() {
	// cilindro representativo do eixo X
	glColor3f(1.0,0.0,0.0);		// vermelho
	glPushMatrix();
	glRotated(90.0, 0.0,1.0,0.0 );
	gluCylinder(glQ, axis_radius_begin, axis_radius_end,
		             axis_lenght, axis_nslices, axis_nstacks);   // nao tem bases
	glPopMatrix();

	// cilindro representativo do eixo Y
	glColor3f(0.0,1.0,0.0);		// verde
	glPushMatrix();
	glRotated(-90.0, 1.0,0.0,0.0 );
	gluCylinder(glQ, axis_radius_begin, axis_radius_end,
		             axis_lenght, axis_nslices, axis_nstacks);   // nao tem bases
	glPopMatrix();
	
	// cilindro representativo do eixo Z
	glColor3f(0.0,0.0,1.0);		// azul
	glPushMatrix();
	// nao necessita rotacao... glRotated(...);
	gluCylinder(glQ, axis_radius_begin, axis_radius_end,
		             axis_lenght, axis_nslices, axis_nstacks);   // nao tem bases
	glPopMatrix();
}


//desenha marcador (seta)
void drawPointer()
{	
	glPushMatrix();

	glTranslated(0.0, 6.0, 0.0);
	glRotated(90.0, 1, 0, 0);
	gluCylinder_closed(glQ, 0.5, 0.0, 1.0, 100, 1);

	glTranslated(0.0, 7.0, 0.0);
	glRotated(90.0, 1.0, 0.0, 0.0);
	gluCylinder_closed(glQ, 0.25, 0.25, 1.0, 100, 1);

	glPopMatrix();
}

//desenha caixa
void drawBox(float length, float height, float width, int texture = -1, float tx=1, float ty=1)
{
	glPushMatrix();
	if(texture>-1){
		glEnable(GL_TEXTURE_2D);
		glBindTexture(GL_TEXTURE_2D, texture);
	}
	
	glBegin(GL_QUADS);
		// Bottom Face
		glTexCoord2f(tx, ty); glVertex3f(0.0, 0.0, 0.0);
		glTexCoord2f(0.0f, ty); glVertex3f(length, 0.0, 0.0);
		glTexCoord2f(0.0f, 0.0f); glVertex3f(length, 0.0, width);
		glTexCoord2f(tx, 0.0f); glVertex3f(0.0, 0.0, width);
		// Top Face
		glTexCoord2f(0.0f, ty); glVertex3f(0.0, height, 0.0);
		glTexCoord2f(0.0f, 0.0f); glVertex3f(0.0, height, width);
		glTexCoord2f(tx, 0.0f); glVertex3f(length, height, width);
		glTexCoord2f(tx, ty); glVertex3f(length, height, 0.0);
		// Front Face
		glTexCoord2f(0.0f, 0.0f); glVertex3f(0.0, 0.0,  width);
		glTexCoord2f(tx, 0.0f); glVertex3f( length, 0.0,  width);
		glTexCoord2f(tx, ty); glVertex3f( length, height,  width);
		glTexCoord2f(0.0f, ty); glVertex3f(0.0, height,  width);
		// Back Face
		glTexCoord2f(tx, 0.0f); glVertex3f(0.0, 0.0, 0.0);
		glTexCoord2f(tx, ty); glVertex3f(0.0, height, 0.0);
		glTexCoord2f(0.0f, ty); glVertex3f(length, height, 0.0);
		glTexCoord2f(0.0f, 0.0f); glVertex3f(length, 0.0, 0.0);
		// Right face
		glTexCoord2f(tx, 0.0f); glVertex3f(length, 0.0, 0.0);
		glTexCoord2f(tx, ty); glVertex3f(length, height, 0.0);
		glTexCoord2f(0.0f, ty); glVertex3f(length, height,  width);
		glTexCoord2f(0.0f, 0.0f); glVertex3f(length, 0.0,  width);
		// Left Face
		glTexCoord2f(0.0f, 0.0f); glVertex3f(0.0, 0.0, 0.0);
		glTexCoord2f(tx, 0.0f); glVertex3f(0.0, 0.0,  width);
		glTexCoord2f(tx, ty); glVertex3f(0.0, height,  width);
		glTexCoord2f(0.0f, ty); glVertex3f(0.0, height, 0.0);
	glEnd();
	
	if(texture>-1) {
		glDisable(GL_TEXTURE_2D);
	}

	glPopMatrix();
}

//desenha torre
void drawTorre(int numero, int jogador) {

	//peca
	string ktorre;
	if (jogador==1) {
		ktorre="'X";
	} else if (jogador==2) {
		ktorre="'Y";
	}

	//na peca que vamos mover, aplicamos a animacao
	if( jogador == tab_jog_ant->jogador && numero == tab_jog_ant->ntorre ){
		glPushMatrix();
		glTranslatef(anim_x,anim_y,anim_z);
	}

	stringstream ss;
	string str;
	ss << numero;
	str = ss.str();
	ktorre.append(str);
	ktorre.append("'");

	float posx=0;
	float posy=0;

	for (unsigned int  s = 0; s < tab->tabuleiro.size(); s++)
	{
		for (unsigned int v = 0; v < tab->tabuleiro[s].size(); v++)
		{
			
			if (tab->tabuleiro[s][v].compare(ktorre)==0) {	
				//printf("tab e ktorre = %s , %s\n\n",tab->tabuleiro[s][v],ktorre);
				if(primeira_jogada){
					posx=s+1;
					posy=v+1;	
				}else{
					//todas as pecas que nao sejam a que vamos mover, desenhamse no sitio onde ja estavam
					if(jogador != tab_jog_ant->jogador || numero != tab_jog_ant->ntorre){
						posx=s+1;
						posy=v+1;	
					}else{
						//se a animacao ja acabou, ja podemos desenhar a peca na sua posicao final
						if(!animate){
							posx=s+1;
							posy=v+1;
						}else{
							//percorre o tabuleiro com as posicoes das pecas antes de ser efectuada a actual jogada
							for (unsigned int  p = 0; p < tab_jog_ant->tabuleiro.size(); p++){
								for (unsigned int o = 0; o < tab_jog_ant->tabuleiro[s].size(); o++)	{
									if (tab_jog_ant->tabuleiro[p][o].compare(ktorre)==0){
										linha_actual=p+1;
										coluna_actual=o+1;
										posx=p+1;
										posy=o+1;
									}
								}
							}
						}
					}
				}
			}
		}
	}

	//altura das torres
	float h=2.8;

	//base
	myMaterial(numero);

	if ((numero==tab->ntorre) && (jogador==tab->jogador) && (inicio!=0)){
		indice_brilho += 0.1;

		for(int cor = 0; cor < 4; cor++){
			mat_ambient_aux[cor]=mat_ambient[numero][cor];
			mat_diffuse_aux[cor]=mat_diffuse[numero][cor];
			mat_specular_aux[cor]=mat_specular[numero][cor];
			mat_emission_aux[cor]=mat_emission[numero][cor];
			//mat_shininess_aux=mat_shininess[numero][0];

			mat_ambient_aux[cor] = indice_brilho;
			mat_diffuse_aux[cor] = indice_brilho;
			mat_specular_aux[cor] = indice_brilho;
			mat_emission_aux[cor] = indice_brilho;
		}

		if(indice_brilho >= 1.0) 
			indice_brilho = 0.0;

		glMaterialfv(GL_FRONT_AND_BACK,GL_AMBIENT,mat_ambient_aux);
		glMaterialfv(GL_FRONT_AND_BACK,GL_DIFFUSE,mat_diffuse_aux);
		glMaterialfv(GL_FRONT_AND_BACK,GL_SPECULAR,mat_specular_aux); 
		glMaterialfv(GL_FRONT_AND_BACK,GL_EMISSION,mat_emission_aux); 
		//glMaterialfv(GL_FRONT_AND_BACK,GL_SHININESS,mat_shininess_aux);
	}

	//if ((numero==tab->ntorre) && (jogador==tab->jogador))
	//	myMaterial(BRANCO);

	glPushMatrix();

	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, (200+numero));

	glTranslatef(-1.0,0.0,17);
	glTranslatef(posx*2,0.0,-posy*2);

	glRotated(-90,1.0,0.0,0.0);
	gluCylinder_closed(glQ, 0.8,0.5,h,25,25);

	//topo da torre
	if (jogador==1) {
		myMaterial(PRETO);
	}
	else if (jogador==2) {
		myMaterial(BRANCO);
	}
	//if ((numero==tab->ntorre) && (jogador==tab->jogador))
	//	myMaterial(BRANCO);

	glBindTexture(GL_TEXTURE_2D, 200);

	glTranslatef(0.0,0.0,h);
	gluCylinder_closed(glQ, 0.5,0.6,0.8,25,25);

	glDisable(GL_TEXTURE_2D);

	if( jogador == tab_jog_ant->jogador && numero == tab_jog_ant->ntorre ){
		glPopMatrix();
	}

	glPopMatrix();
}

void switch_camaras(){

	glFrustum( -xy_aspect*.04, xy_aspect*.04, -.04, .04, .1, 5000.0 );

	glMatrixMode( GL_MODELVIEW );
    glLoadIdentity();

	//activa botao de zoom e traslaccao
	glTranslatef( obj_pos[0], obj_pos[1], -obj_pos[2] );

	switch (vista_jogo){
		case 0:
			premiu_1 = FALSE;
			gluLookAt(16,55,15 ,16,15,15.1 ,0,1,0); // vista do topo do tabuleiro
			break;
		case 1: 
			anim_camara = FALSE;
			premiu_1 = TRUE;
			gluLookAt(-5,25,17 ,16,15,17.1 ,0,1,0); //vista do jogador 1
			break;
		case 2: 
			if(premiu_1 == TRUE && anim_camara == FALSE){
				camx=-5.0;
				anim_camara = TRUE;
			}
			gluLookAt(camx,25,17 ,16,15,17.1 ,0,1,0); //vista do jogador 2
			break;
		case 3: 
			premiu_1 = FALSE;
			gluLookAt(16,25,20 ,16,15,10 ,0,1,0); // vista do contador e marcador
			break;
		case 4: 
			premiu_1 = FALSE;
			gluLookAt(16,25,100 ,16,20,10 ,0,1,0); // vista da parede do lado do contador
			break;
		case 5: 
			premiu_1 = FALSE;
			gluLookAt(16,25,-90 ,16,20,10 ,0,1,0); //vista da parede do lado contrario ao contador penis
			break;
	}

}
//desenha tabuleiro
void drawTabuleiro(GLenum mode) {

	//selecciona a camara especificada
	switch_camaras();

	glMultMatrixf( view_rotate );

	// tabuleiro
	if (mode == GL_SELECT)
		glPushName(0);

	myMaterial(TABULEIRO);

	glTranslatef(8.5,12.7,8.7);

	glPushMatrix();

	glEnable(GL_MAP2_VERTEX_3);
	glEnable(GL_MAP2_NORMAL);
	glEnable(GL_MAP2_COLOR_4);
	glEnable(GL_MAP2_TEXTURE_COORD_2);
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, 210);
	
	glMap2f(GL_MAP2_VERTEX_3, 0.0, 1.0, 3, 2,  0.0, 1.0, 6, 2,  &ctrlpoints_tab[0][0]);
	glMap2f(GL_MAP2_NORMAL,   0.0, 1.0, 3, 2,  0.0, 1.0, 6, 2,  &nrmlcompon_tab[0][0]);
	glMap2f(GL_MAP2_COLOR_4,  0.0, 1.0, 4, 2,  0.0, 1.0, 8, 2,  &colorpoints_tab[0][0]);
	glMap2f(GL_MAP2_TEXTURE_COORD_2,  0.0, 1.0, 2, 2,  0.0, 1.0, 4, 2,  &textpoints_tab[0][0]);
	
	glMapGrid2f(20, 0.0, 1.0, 20, 0.0, 1.0);
	glEvalMesh2(GL_FILL, 0,20, 0,20);	
	
	glDisable(GL_TEXTURE_2D);
	glDisable(GL_MAP2_VERTEX_3);
	glDisable(GL_MAP2_NORMAL);
	glDisable(GL_MAP2_COLOR_4);
	glDisable(GL_MAP2_TEXTURE_COORD_2);
	glDisable(GL_TEXTURE_2D);

	glPopMatrix();

	glPushMatrix();
	glTranslatef(0.0,0.0,16.0);
	glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE);

	//blocos do tabuleiro que especificam as casas do tabuleiro(estao invisiveis)
	for (int m=0; m!=8;m++)
	{
		for (int n=0; n!=8; n++)
		{
			if (mode == GL_SELECT) 
				glLoadName ((m+1)*10+n+1);
			glTranslatef(0.0,0.0,-2.0);
			drawBox(2,0.1,2);
		}
		glTranslatef(2.0,0.0,16.0);
	}
	glDisable(GL_BLEND);
	glPopMatrix();

	//desenhando as torres
	glPushMatrix();
	for (int i=1; i!=3;i++)
	{
		for (int j=1; j!=9;j++)
		{
			if (mode == GL_SELECT) 
				glLoadName (i*100+j);

			drawTorre(j,i);
		}
	}
	glPopMatrix();

	glPopName();
}

//desenha cerca
void drawFence(float length, float height, float width, int texture = -1, float tx=1, float ty=1)
{
	glPushMatrix();
	if(texture>-1){
		glEnable(GL_TEXTURE_2D);
		glBindTexture(GL_TEXTURE_2D, texture);
	}
	
	glBegin(GL_QUADS);
		// Front Face
		glTexCoord2f(0.0f, 0.0f); glVertex3f(0.0, 0.0,  width);
		glTexCoord2f(tx, 0.0f); glVertex3f( length, 0.0,  width);
		glTexCoord2f(tx, ty); glVertex3f( length, height,  width);
		glTexCoord2f(0.0f, ty); glVertex3f(0.0, height,  width);
	glEnd();
	glBegin(GL_QUADS);
		// Back Face
		glTexCoord2f(tx, 0.0f); glVertex3f(0.0, 0.0, 0.0);
		glTexCoord2f(tx, ty); glVertex3f(0.0, height, 0.0);
		glTexCoord2f(0.0f, ty); glVertex3f(length, height, 0.0);
		glTexCoord2f(0.0f, 0.0f); glVertex3f(length, 0.0, 0.0);
	glEnd();
	glBegin(GL_QUADS);
		// Right face
		glTexCoord2f(tx, 0.0f); glVertex3f(length, 0.0, 0.0);
		glTexCoord2f(tx, ty); glVertex3f(length, height, 0.0);
		glTexCoord2f(0.0f, ty); glVertex3f(length, height,  width);
		glTexCoord2f(0.0f, 0.0f); glVertex3f(length, 0.0,  width);
	glEnd();
	glBegin(GL_QUADS);
		// Left Face
		glTexCoord2f(0.0f, 0.0f); glVertex3f(0.0, 0.0, 0.0);
		glTexCoord2f(tx, 0.0f); glVertex3f(0.0, 0.0,  width);
		glTexCoord2f(tx, ty); glVertex3f(0.0, height,  width);
		glTexCoord2f(0.0f, ty); glVertex3f(0.0, height, 0.0);
	glEnd();
	
	if(texture>-1) {
		glDisable(GL_TEXTURE_2D);
	}

	glPopMatrix();
}

//desenha relogio e pontuacao
void drawClock(int sec, int part, int min, int pt1, int pt2) {
	glPushMatrix();
	
	float length=1, height=3, width=1;

	//pontos jogador 1
	myMaterial(C2);
	drawBox(length+1, height, width);
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, 30+pt1);
	glBegin(GL_QUADS);
		// Front Face
		glTexCoord2f(0.0f, 0.0f); glVertex3f(0.0, 0.0,  width);
		glTexCoord2f(1.0f, 0.0f); glVertex3f( length+1, 0.0,  width);
		glTexCoord2f(1.0f, 1.0f); glVertex3f( length+1, height,  width);
		glTexCoord2f(0.0f, 1.0f); glVertex3f(0.0, height,  width);
	glEnd();
	glTranslatef(length,0.0,0.0);
	glDisable(GL_TEXTURE_2D);

	//segundos em 6 partes
	glTranslatef(length*2,0.0,0.0);
	myMaterial(NEON);
	drawBox(length, height, width);
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, 30+part);
	glBegin(GL_QUADS);
		// Front Face
		glTexCoord2f(0.0f, 0.0f); glVertex3f(0.0, 0.0,  width);
		glTexCoord2f(1.0f, 0.0f); glVertex3f( length, 0.0,  width);
		glTexCoord2f(1.0f, 1.0f); glVertex3f( length, height,  width);
		glTexCoord2f(0.0f, 1.0f); glVertex3f(0.0, height,  width);
	glEnd();
	glTranslatef(length,0.0,0.0);
	glDisable(GL_TEXTURE_2D);
	
	//segundo
	glTranslatef(length,0.0,0.0);
	for (int numero=0; numero!=10; numero++) {
		myMaterial(PRETO);
		if (numero==sec) {
			myMaterial(NEON);
		} 
		drawBox(length, height, width);
		glEnable(GL_TEXTURE_2D);
		glBindTexture(GL_TEXTURE_2D, 30+numero);
		glBegin(GL_QUADS);
			// Front Face
			glTexCoord2f(0.0f, 0.0f); glVertex3f(0.0, 0.0,  width);
			glTexCoord2f(1.0f, 0.0f); glVertex3f( length, 0.0,  width);
			glTexCoord2f(1.0f, 1.0f); glVertex3f( length, height,  width);
			glTexCoord2f(0.0f, 1.0f); glVertex3f(0.0, height,  width);
		glEnd();
		glTranslatef(length,0.0,0.0);
		glDisable(GL_TEXTURE_2D);
	}

	//minutos
	int div=min/10;
	int res=min%10;
	glTranslatef(length,0.0,0.0);
	myMaterial(NEON);
	drawBox(length, height, width);
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, 30+div);
	glBegin(GL_QUADS);
		// Front Face
		glTexCoord2f(0.0f, 0.0f); glVertex3f(0.0, 0.0,  width);
		glTexCoord2f(1.0f, 0.0f); glVertex3f( length, 0.0,  width);
		glTexCoord2f(1.0f, 1.0f); glVertex3f( length, height,  width);
		glTexCoord2f(0.0f, 1.0f); glVertex3f(0.0, height,  width);
	glEnd();
	glTranslatef(length,0.0,0.0);
	glDisable(GL_TEXTURE_2D);

	drawBox(length, height, width);
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, 30+res);
	glBegin(GL_QUADS);
		// Front Face
		glTexCoord2f(0.0f, 0.0f); glVertex3f(0.0, 0.0,  width);
		glTexCoord2f(1.0f, 0.0f); glVertex3f( length, 0.0,  width);
		glTexCoord2f(1.0f, 1.0f); glVertex3f( length, height,  width);
		glTexCoord2f(0.0f, 1.0f); glVertex3f(0.0, height,  width);
	glEnd();
	glTranslatef(length,0.0,0.0);
	glDisable(GL_TEXTURE_2D);

	//pontos jogador 2
	glTranslatef(length*2,0.0,0.0);
	myMaterial(C7);
	drawBox(length+1, height, width);
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, 30+pt2);
	glBegin(GL_QUADS);
		// Front Face
		glTexCoord2f(0.0f, 0.0f); glVertex3f(0.0, 0.0,  width);
		glTexCoord2f(1.0f, 0.0f); glVertex3f( length+1, 0.0,  width);
		glTexCoord2f(1.0f, 1.0f); glVertex3f( length+1, height,  width);
		glTexCoord2f(0.0f, 1.0f); glVertex3f(0.0, height,  width);
	glEnd();
	glTranslatef(length,0.0,0.0);
	glDisable(GL_TEXTURE_2D);

	glPopMatrix();
}

//desenha fundo
void drawBackground(int ceu) {
	glPushMatrix();
	myMaterial(CEU);
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, 190+ceu);
	glRotated(90,-1,0,0);
	gluSphere(glQ,2000,500,500);
	glDisable(GL_TEXTURE_2D);
	glPopMatrix();
}

void chao() {
	glEnable(GL_TEXTURE_2D);

	// 1º rectangulo (esquerdo)
	glBindTexture(GL_TEXTURE_2D, 7);
	myMaterial(RELVA);
	glBegin(GL_POLYGON);
		glNormal3d(0.0,1.0,0.0);
		glTexCoord2f(0.0,0.0);  glVertex3d(0, 0, 0);
		glTexCoord2f(8.0,0.0); glVertex3d(15, 0,  0);
		glTexCoord2f(8.0,8.0); glVertex3d(15, 0,  -30);
		glTexCoord2f(0.0,8.0);  glVertex3d(0, 0, -30);
	glEnd();

	// 2º rectangulo (centro)
	glBindTexture(GL_TEXTURE_2D, 8);
	myMaterial(CHAO);
	glBegin(GL_POLYGON);
		glNormal3d(0.0,1.0,0.0);
		glTexCoord2f(0.0,0.0);  glVertex3d(15, 0, 0);
		glTexCoord2f(8.0,0.0); glVertex3d(30, 0,  0);
		glTexCoord2f(8.0,8.0); glVertex3d(30, 0,  -30);
		glTexCoord2f(0.0,8.0);  glVertex3d(15, 0, -30);
	glEnd();
	
	// 3º rectangulo (direito)
	glBindTexture(GL_TEXTURE_2D, 9);
	myMaterial(CHAO);
	glBegin(GL_POLYGON);
		glNormal3d(0.0,1.0,0.0);
		glTexCoord2f(0.0,0.0);  glVertex3d(30, 0, 0);
		glTexCoord2f(8.0,0.0); glVertex3d(45, 0,  0);
		glTexCoord2f(8.0,8.0); glVertex3d(45, 0,  -30);
		glTexCoord2f(0.0,8.0);  glVertex3d(30, 0, -30);
	glEnd();
	glDisable(GL_TEXTURE_2D);
	myMaterial(DEFAULT);
}

void arvores() {
	glEnable(GL_TEXTURE_2D);
	//desenhar arvores

	//arvore grande
	glBindTexture(GL_TEXTURE_2D, 10);
	myMaterial(MADEIRA);
	glPushMatrix();
	glTranslatef(10,0,-25);
	glRotated(-90.0, 1.0,0.0,0.0 );
	gluCylinder(glQ, 0.4, 0,7, 21 , 21);  
	glPopMatrix();

	//folhas da arvore grande vista de um lado
	glBindTexture(GL_TEXTURE_2D, 7);
	myMaterial(RELVA);
	glBegin(GL_POLYGON);
		glNormal3d(0.0,1.0,0.0); 
		glTexCoord2f(0.0,0.0);  glVertex3d(8, 3,-25);	
		glTexCoord2f(0.0,2); glVertex3d(12, 3,-25);
		glTexCoord2f(1.0,2); glVertex3d(10, 7,-25);	
	glEnd();
	glBegin(GL_POLYGON);
		glNormal3d(1.0,0.0,0.0); 
		glTexCoord2f(0.0,0.0);  glVertex3d(10, 3,-27);	
		glTexCoord2f(0.0,2); glVertex3d(10, 3,-23);	
		glTexCoord2f(1.0,2); glVertex3d(10, 7,-25);	
	glEnd();

	//arvore pequena
	glBindTexture(GL_TEXTURE_2D, 10);
	myMaterial(MADEIRA);
	glPushMatrix();
	glTranslatef(3,0,-17);
	glRotated(-90.0, 1.0,0.0,0.0 );
	gluCylinder(glQ, 0.2, 0,5, 21, 21);
	glPopMatrix();

	//folhas da arvore grande vista de um lado
	glBindTexture(GL_TEXTURE_2D, 7);
	myMaterial(RELVA);
	glBegin(GL_POLYGON);
		glNormal3d(0.0,0.0,1.0);
		glTexCoord2f(0.0,0.0);  glVertex3d(2, 2,-17);
		glTexCoord2f(0.0,2); glVertex3d(4, 2,-17);
		glTexCoord2f(1.0,2); glVertex3d(3, 5,-17);
	glEnd();
	glBegin(GL_POLYGON);
		glNormal3d(1.0,0.0,0.0);
		glTexCoord2f(0.0,0.0);  glVertex3d(3, 2,-18);
		glTexCoord2f(0.0,2); glVertex3d(3, 2,-16);
		glTexCoord2f(1.0,2); glVertex3d(3, 5,-17);
	glEnd();

	//arvores redondas
	//arvore 1
	glBindTexture(GL_TEXTURE_2D, 10);
	myMaterial(MADEIRA);	
	glPushMatrix();
	glTranslatef(8,0,-16);
	glRotated(-90.0, 1.0,0.0,0.0 );
	gluCylinder(glQ, 0.6, 0,3.5, 21, 21);   
	glPopMatrix();
	
	//arvore 2
	glBindTexture(GL_TEXTURE_2D, 10);
	myMaterial(MADEIRA);
	glPushMatrix();
	glTranslatef(3,0,-27);
	glRotated(-90.0, 1.0,0.0,0.0 );
	gluCylinder(glQ, 0.6, 0,3.5, 21, 21);
	glPopMatrix();

	//folhas das arvores redondas
	//arvore 1
	glBindTexture(GL_TEXTURE_2D, 7);
	myMaterial(RELVA);
	glPushMatrix();
	glTranslatef(3,3.5,-27);
	glRotated(-90.0, 1.0,0.0,0.0 );
	gluCylinder(glQ, 2, 0,2.5, 21, 21);   
	glPopMatrix();

	//arvore 2
	glBindTexture(GL_TEXTURE_2D, 7);
	myMaterial(RELVA);
	glPushMatrix();
	glTranslatef(8,3.5,-16);
	glRotated(-90.0, 1.0,0.0,0.0 );
	gluCylinder(glQ, 2, 0,2.5, 21, 21);   
	glPopMatrix();

	glDisable(GL_TEXTURE_2D);
	myMaterial(DEFAULT);
}

void hospital() {
	glEnable(GL_TEXTURE_2D);

	//paredes do hospital (frente, direita, tras, esquerda)
	glBindTexture(GL_TEXTURE_2D, 5);
	myMaterial(PAREDE);
	//frente
	glBegin(GL_POLYGON);
		glNormal3d(0.0,0.0,1.0);
		glTexCoord2f(0.0,0.0);  glVertex3d(16.75, 0, -19);
		glTexCoord2f(3.0,0); glVertex3d(28.25, 0,  -19);
		glTexCoord2f(3.0,3); glVertex3d(28.25, 11.5,  -19);	
		glTexCoord2f(0.0,3.0);  glVertex3d(16.75, 11.5, -19);
	glEnd();

	//direita
	glBegin(GL_POLYGON);
		glNormal3d(1.0,0.0,0.0);
		glTexCoord2f(0.0,0.0);  glVertex3d(28.25, 0, -19);
		glTexCoord2f(2.0,0); glVertex3d(28.25, 0,  -26);
		glTexCoord2f(2.0,3); glVertex3d(28.25, 11.5,  -26);	
		glTexCoord2f(0.0,3.0);  glVertex3d(28.25, 11.5, -19);
	glEnd();

	//tras
	glBegin(GL_POLYGON);
		glNormal3d(0.0,0.0,-1.0);
		glTexCoord2f(0.0,0.0);  glVertex3d(28.25, 0, -26);
		glTexCoord2f(3.0,0); glVertex3d(16.75, 0,  -26);
		glTexCoord2f(3.0,3); glVertex3d(16.75, 11.5,  -26);	
		glTexCoord2f(0.0,3.0);  glVertex3d(28.25, 11.5, -26);
	glEnd();

	//esquerda
	glBegin(GL_POLYGON);
		glNormal3d(-1.0,0.0,0.0);
		glTexCoord2f(0.0,0.0);  glVertex3d(16.75, 0, -26);
		glTexCoord2f(2.0,0); glVertex3d(16.75, 0,  -19);
		glTexCoord2f(2.0,3); glVertex3d(16.75, 11.5,  -19);	
		glTexCoord2f(0.0,3.0);  glVertex3d(16.75, 11.5, -26);
	glEnd();

	//chao do hospital
	myMaterial(CHAO);
	glBindTexture(GL_TEXTURE_2D, 8);
	glBegin(GL_POLYGON);
		glNormal3d(0.0,1.0,0.0);
		glTexCoord2f(0.0,0.0);  glVertex3d(16.75, 0, -19);
		glTexCoord2f(0.0,2); glVertex3d(28.25, 0,  -19);
		glTexCoord2f(1.0,2); glVertex3d(28.25, 0,  -26);	
		glTexCoord2f(1.0,0.0);  glVertex3d(16.75, 0, -26);
	glEnd();

	//tecto do hospital
	myMaterial(TELHA);
	glBindTexture(GL_TEXTURE_2D, 11);
	glBegin(GL_POLYGON);
		glNormal3d(0.0,1.0,0.0);
		glTexCoord2f(0.0,0.0);  glVertex3d(16.75, 11.5, -19);
		glTexCoord2f(0.0,2); glVertex3d(28.25, 11.5,  -19);
		glTexCoord2f(1.0,2); glVertex3d(28.25, 11.5,  -26);	
		glTexCoord2f(1.0,0.0);  glVertex3d(16.75, 11.5, -26);
	glEnd();

	//telhado do hospital
	//base
	glBegin(GL_POLYGON);
		glNormal3d(0.0,1.0,0.0);
		glTexCoord2f(0.0,0.0);  glVertex3d(16, 11.5, -18.25);
		glTexCoord2f(0.0,2); glVertex3d(29, 11.5,  -18.25);
		glTexCoord2f(1.0,2); glVertex3d(29, 11.5,  -26.75);	
		glTexCoord2f(1.0,0.0);  glVertex3d(16, 11.5, -26.75);
	glEnd();

	//triangulos
	glBegin(GL_POLYGON);
		glNormal3d(0.0,1.0,0.0);
		glTexCoord2f(0.0,0.0);  glVertex3d(16, 11.5, -18.25);
		glTexCoord2f(0.0,2); glVertex3d(19.25, 12.5,  -22.50);
		glTexCoord2f(1.0,2); glVertex3d(16, 11.5,  -26.75);
	glEnd();
	glBegin(GL_POLYGON);
		glNormal3d(0.0,1.0,0.0);
		glTexCoord2f(0.0,0.0);  glVertex3d(29, 11.5, -18.25);
		glTexCoord2f(0.0,2); glVertex3d(29, 11.5,  -26.75);
		glTexCoord2f(1.0,2); glVertex3d(25.75, 12.5,  -22.50);
	glEnd();

	//quadrilateros 
	glBegin(GL_POLYGON);
		glNormal3d(0.0,1.0,0.0);
		glTexCoord2f(0.0,0.0);  glVertex3d(16, 11.5, -18.25);
		glTexCoord2f(0.0,2); glVertex3d(29, 11.5,  -18.25);
		glTexCoord2f(1.0,2); glVertex3d(25.75, 12.5, -22.50);	
		glTexCoord2f(1.0,0.0);  glVertex3d(19.25, 12.5, -22.50);
	glEnd();
	glBegin(GL_POLYGON);
		glNormal3d(0.0,1.0,0.0);
		glTexCoord2f(0.0,0.0);  glVertex3d(29, 11.5, -26.75);
		glTexCoord2f(0.0,2); glVertex3d(16, 11.5,  -26.75);
		glTexCoord2f(1.0,2); glVertex3d(19.25, 12.5, -22.50);	
		glTexCoord2f(1.0,0.0);  glVertex3d(25.75, 12.5, -22.50);
	glEnd();

	//Porta
	myMaterial(PAREDE);
	glBindTexture(GL_TEXTURE_2D, 10);
	glBegin(GL_POLYGON);
		glNormal3d(0.0,0.0,1.0);
		glVertex3d(21.0, 0, -18.95);
		glVertex3d(24.0, 0,  -18.95);
		glVertex3d(24.0, 3.2, -18.95);	
		glVertex3d(21.0, 3.2, -18.95);
	glEnd();
	glDisable(GL_TEXTURE_2D);

	//Nome
	
	myMaterial(NEON);
	glPushMatrix();
	glTranslatef(19,12,-19);
	glScalef(0.03, 0.03, 0.03);
    glLineWidth(4.0);
	/*
	glutStrokeCharacter(GLUT_STROKE_ROMAN, 'H');
	glutStrokeCharacter(GLUT_STROKE_ROMAN, 'S');
	glutStrokeCharacter(GLUT_STROKE_ROMAN, 'J');
	*/
	glPopMatrix();

	myMaterial(DEFAULT);
}

void heliporto() {
	//chao do heliporto
	glPushMatrix();
	myMaterial(CHAO);
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, 6);

	glEnable(GL_MAP2_VERTEX_3);
	glEnable(GL_MAP2_NORMAL);
	glEnable(GL_MAP2_COLOR_4);
	glEnable(GL_MAP2_TEXTURE_COORD_2);

	glMap2f(GL_MAP2_VERTEX_3, 0.0, 1.0, 3, 2,  0.0, 1.0, 6, 2,  &ctrlpoints_heli[0][0]);
	glMap2f(GL_MAP2_NORMAL,   0.0, 1.0, 3, 2,  0.0, 1.0, 6, 2,  &nrmlcompon_heli[0][0]);
	glMap2f(GL_MAP2_COLOR_4,  0.0, 1.0, 4, 2,  0.0, 1.0, 8, 2,  &colorpoints_heli[0][0]);
	glMap2f(GL_MAP2_TEXTURE_COORD_2,  0.0, 1.0, 2, 2,  0.0, 1.0, 4, 2,  &textpoints_heli[0][0]);

	glMapGrid2f(40, 0.0,1.0, 60, 0.0,1.0); 
	glEvalMesh2(GL_FILL, 0,40, 0,60);
	
	glDisable(GL_MAP2_VERTEX_3);
	glDisable(GL_MAP2_NORMAL);
	glDisable(GL_MAP2_COLOR_4);
	glDisable(GL_MAP2_TEXTURE_COORD_2);

	glPopMatrix();

	//4 lados
	glBindTexture(GL_TEXTURE_2D, 13);
	myMaterial(CHAO);
    glBegin(GL_POLYGON);
        glNormal3d(0.0,1.0,0.0);
        glTexCoord2f(0.0,0.0);  glVertex3d(17.25, 0, -3.75);
        glTexCoord2f(1.0,0); glVertex3d(27.75, 0,  -3.75);
        glTexCoord2f(1.0,1); glVertex3d(27.75, 1,  -3.75);
        glTexCoord2f(0.0,1.0);  glVertex3d(17.25, 1, -3.75);
    glEnd();
    glBegin(GL_POLYGON);
        glNormal3d(0.0,1.0,0.0);
        glTexCoord2f(1.0,0); glVertex3d(27.75, 0,-11.25);
        glTexCoord2f(0.0,0.0);  glVertex3d(17.25, 0,-11.25);
        glTexCoord2f(0.0,1.0);  glVertex3d(17.25, 1, -11.25);
        glTexCoord2f(1.0,1); glVertex3d(27.75, 1,  -11.25);
    glEnd();
    glBegin(GL_POLYGON);
        glNormal3d(0.0,1.0,0.0);
        glTexCoord2f(0.0,0.0);  glVertex3d(17.25, 0, -11.25);
        glTexCoord2f(1.0,0); glVertex3d(17.25, 0,  -3.75);
        glTexCoord2f(1.0,1); glVertex3d(17.25, 1,  -3.75);
        glTexCoord2f(0.0,1.0);  glVertex3d(17.25, 1, -11.25);
    glEnd();
    glBegin(GL_POLYGON);
        glNormal3d(0.0,1.0,0.0);
        glTexCoord2f(1.0,0); glVertex3d(27.75, 0,  -3.75);
        glTexCoord2f(0.0,0.0);  glVertex3d(27.75, 0, -11.25);
        glTexCoord2f(0.0,1.0);  glVertex3d(27.75, 1, -11.25);
        glTexCoord2f(1.0,1); glVertex3d(27.75, 1,  -3.75);
    glEnd();

	glDisable(GL_TEXTURE_2D);
	myMaterial(DEFAULT);
}

void holofotes() {
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, 13);
	//postes
	myMaterial(FERRO);
	//esquerdo-baixo
	glPushMatrix();
	glTranslatef(17.25,0,-3.75);
	glRotated(-90.0, 1.0,0.0,0.0 );
	gluCylinder(glQ,0.3, 0.3, 4.7, 8, axis_nstacks);
	glPopMatrix();

	myMaterial(FERRO);
	//direito-baixo
	glPushMatrix();
	glTranslatef(27.75,0,-3.75);
	glRotated(-90.0, 1.0,0.0,0.0 );
	gluCylinder(glQ,0.3, 0.3, 4.7, 8, axis_nstacks);
	glPopMatrix();

	myMaterial(FERRO);
	//direito-alto
	glPushMatrix();
	glTranslatef(27.75,0,-11.25);
	glRotated(-90.0, 1.0,0.0,0.0 );
	gluCylinder(glQ,0.3, 0.3, 4.7, 8, axis_nstacks);
	glPopMatrix();

	myMaterial(FERRO);
	//esquerdo-alto
	glPushMatrix();
	glTranslatef(17.25,0,-11.25);
	glRotated(-90.0, 1.0,0.0,0.0 );
	gluCylinder(glQ,0.3, 0.3, 4.7, 8, axis_nstacks);
	glPopMatrix();

	glBindTexture(GL_TEXTURE_2D, 10);

	//desenhar lampada de holofote esquerdo-baixo
	myMaterial(MADEIRA);
	glPushMatrix();
	glTranslatef(18,4.5,-3.7);
	glRotated(-45.0,0.0,0.0,1.0 );
	glRotated(-90.0, 1.0,0.0,0.0 );
	glRotated(-90.0,0.0,1.0,0.0 );
	gluCylinder(glQ, 1.3, 0.7,0.7, 21, 21);
	glPopMatrix();

	//fechar caixa holofote
	glPushMatrix();
	glTranslatef(17.5,5.0,-3.7);
	glRotated(-45.0,0.0,0.0,1.0 );
	glRotated(-90.0,0.0,1.0,0.0 );
	gluDisk(glQ, 0, 0.7, 21, 4);
	glPopMatrix();
	
	//desenhar lampada de holofote esquerdo-alto
	myMaterial(MADEIRA);
	glPushMatrix();
	glTranslatef(18,4.5,-11.2);
	glRotated(-45.0,0.0,0.0,1.0 );
	glRotated(-90.0, 1.0,0.0,0.0 );
	glRotated(-90.0,0.0,1.0,0.0 );
	gluCylinder(glQ, 1.3, 0.7,0.7, 21, 21);
	glPopMatrix();

	//fechar caixa holofote
	glPushMatrix();
	glTranslatef(17.5,5.0,-11.2);
	glRotated(-45.0,0.0,0.0,1.0 );
	glRotated(-90.0,0.0,1.0,0.0 );
	gluDisk(glQ, 0, 0.7, 21, 4);
	glPopMatrix();

	//desenhar lampada de holofote direito-baixo
	myMaterial(MADEIRA);
	glPushMatrix();
	glTranslatef(27.0,4.5,-3.7);
	glRotated(-45.0,0.0,0.0,1.0 );
	glRotated(-90.0, 1.0,0.0,0.0 );
	gluCylinder(glQ, 1.3, 0.7,0.7, 21, 21);
	glPopMatrix();

	//fechar caixa holofote
	glPushMatrix();
	glTranslatef(27.5,5.0,-3.7);
	glRotated(-45.0,0.0,0.0,1.0 );
	glRotated(-90.0,0.0,1.0,0.0 );
	glRotated(-90.0,1.0,0.0,0.0 );
	gluDisk(glQ, 0, 0.7, 21, 4);
	glPopMatrix();
	
	//desenhar lampada de holofote direito-alto
	myMaterial(MADEIRA);
	glPushMatrix();
	glTranslatef(27,4.5,-11.2);
	glRotated(-45.0,0.0,0.0,1.0 );
	glRotated(-90.0, 1.0,0.0,0.0 );
	gluCylinder(glQ, 1.3, 0.7,0.7, 21, 21);
	glPopMatrix();

	//fechar caixa holofote
	glPushMatrix();
	glTranslatef(27.5,5.0,-11.2);
	glRotated(-45.0,0.0,0.0,1.0 );
	glRotated(-90.0,0.0,1.0,0.0 );
	glRotated(-90.0,1.0,0.0,0.0 );
	gluDisk(glQ, 0, 0.7, 21, 4);
	glPopMatrix();

	glDisable(GL_TEXTURE_2D);
	myMaterial(DEFAULT);
}

void torre() {

	myMaterial(CHAO);
	//base da torre
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, 8);
	myMaterial(FERRO);
    glPushMatrix();
    glTranslatef(43,0,-25);
    glRotated(-90.0, 1.0,0.0,0.0 );
    gluCylinder(glQ,1.25,1.25,9.5,16,21);
    glPopMatrix();

    //plataforma cilindrica da torre
    glPushMatrix();
        glTranslatef(43,8.8,-25);
        glRotated(-90.0, 1.0,0.0,0.0 );
        gluCylinder_closed(glQ,4.5,4.5,0.7,121,121);
    glPopMatrix();
	glDisable(GL_TEXTURE_2D);

	myMaterial(VIDRO);
    //cupula da torre (sphere clipada e com blend)
    glPushMatrix();
    glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE);
        glPushMatrix();
        glTranslatef(43,9.5,-25);
        gluQuadricOrientation( glQ, GLU_OUTSIDE);
        glClipPlane(GL_CLIP_PLANE0,eqn);
        glEnable(GL_CLIP_PLANE0);
        gluSphere(glQ, 4.5, 50, 50);
        glDisable(GL_CLIP_PLANE0);
        glPopMatrix();
    glDisable(GL_BLEND);
    glPopMatrix();
	myMaterial(DEFAULT);
}

void hangar() {
	myMaterial(DEFAULT);

	glPushMatrix();

	glEnable(GL_MAP2_VERTEX_3);
	glEnable(GL_MAP2_NORMAL);
	glEnable(GL_MAP2_COLOR_4);
	glEnable(GL_MAP2_TEXTURE_COORD_2);
	glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, 12);

	glMap2f(GL_MAP2_VERTEX_3, 0.0, 1.0, 3, 4,  0.0, 1.0, 12, 4, &ctrlpoints_han[0][0][0]);
	glMap2f(GL_MAP2_NORMAL,   0.0, 1.0, 3, 2,  0.0, 1.0, 6, 2,  &nrmlcompon_han[0][0]);
	glMap2f(GL_MAP2_COLOR_4,  0.0, 1.0, 4, 2,  0.0, 1.0, 8, 2,  &colorpoints_han[0][0]);
	glMap2f(GL_MAP2_TEXTURE_COORD_2, 0, 1, 2, 2,0, 1, 4, 2, &texpts_han[0][0][0]);

	glEnable(GL_MAP2_TEXTURE_COORD_2);
	glEnable(GL_MAP2_VERTEX_3);
	glMapGrid2f(20, 0.0, 1.0, 20, 0.0, 1.0);
	glPushMatrix();
    glTranslatef(31,0,-27);
    glEvalMesh2(GL_FILL, 0,20, 0,20);

	glDisable(GL_TEXTURE_2D);
	glDisable(GL_MAP2_VERTEX_3);
	glDisable(GL_MAP2_NORMAL);
	glDisable(GL_MAP2_COLOR_4);
	glDisable(GL_MAP2_TEXTURE_COORD_2);
    glDisable(GL_TEXTURE_2D);

	glPopMatrix();
}

// desenha mesa
void drawMesa() {
	glPushMatrix();

	//tampo
	drawBox(26,2,30);

	//pernas
	glRotated(90,1.0,0.0,0.0);

	glPushMatrix();
	glTranslatef(2.0,2.0,0.0);
	gluCylinder_closed(glQ, 1, 1, 17, 50,50);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(24.0,2.0,0.0);
	gluCylinder_closed(glQ, 1, 1, 17, 50,50);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(2.0,28.0,0.0);
	gluCylinder_closed(glQ, 1, 1, 17, 50,50);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(24.0,28.0,0.0);
	gluCylinder_closed(glQ, 1, 1, 17, 50,50);
	glPopMatrix();
	
	glPopMatrix();
}

//cenario 0
void drawEmptySpace() {
	glPushMatrix();

	glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, 10);
	myMaterial(MADEIRA);
	glTranslatef(3.5,10.5,0.0);
	drawMesa();
	glDisable(GL_TEXTURE_2D);

	glPopMatrix();
}

//capsula
void drawCapsule() {
	glPushMatrix();
	luzescapsula();

	//cena trab1
	glCallList(cena1);

	//base da capsula
	myMaterial(MADEIRA);
	glPushMatrix();
	glTranslatef(-3.5,-10.5,-35.0);
	drawBox(52,10,40);
	glPopMatrix();

	//capsula
	myMaterial(TRANSPARENTE);
	glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE);
	glPushMatrix();
	glTranslatef(-2.5,11.5,-15.0);
	glRotated(90.0,0.0,1.0,0.0);
	gluCylinder_closed(glQ,20,20,50,100,100);
	glPopMatrix();

	glDisable(GL_BLEND);

	glPopMatrix();
}

//cenario 1
void drawTimeCapsule() {
	glPushMatrix();

	//mesa
	glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, 10);
	glTranslatef(3.5,10.5,0.0);
	myMaterial(MADEIRA);
	drawMesa();

	//chao
	glPushMatrix();
	glTranslatef(-250.0,-16.0,-250.0);
	glRotated(90,1.0,0.0,0.0);
	myMaterial(DEFAULT);
	drawBox(500,500,5,243,5,10);
	glTranslatef(0.0,5.0,0.0);
	glRotated(90,-1.0,0.0,0.0);
	glTranslatef(0.0,0.0,-5.0);
	drawFence(500,15,500,242,5,5);
	glPopMatrix();
	
	//nave
	glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, 240);
	myMaterial(DEFAULT);
	gluSphere(glQ,500,100,100);
	glDisable(GL_TEXTURE_2D);

	glTranslatef(-250,0,-35);
	for (int p=0;p!=8;p++) {
		glTranslatef(52,0,0);
		drawCapsule();
	}
	glPopMatrix();
}


//cadeira
void drawCadeira(){
//desenha suportes cilindricos do suporte para as costas
	glPushMatrix();
	glTranslatef(24.0,17.0,0.0);
	glRotated(90,1.0,0.0,0.0);
	gluCylinder_closed(glQ, 1, 1, 17, 50,50);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(2.0,17.0,0.0);
	glRotated(90,1.0,0.0,0.0);
	gluCylinder_closed(glQ, 1, 1, 17, 50,50);
	glPopMatrix();

	//desenha a cadeira onde sentamos(mesa)
	glPushMatrix();
		glTranslatef(0,15,0);
		drawBox(26,10,2);
	glPopMatrix();
	drawMesa();

}

//desenhar o Dojo
void drawDojo(float dimx, float dimy, float dimz){
	glPushMatrix();
	//paredes da casa (frente, direita, tras, esquerda)
	
	myMaterial(PAREDE);

	//marcador
	glPushMatrix();
	glBindTexture(GL_TEXTURE_2D, 308);
		glTranslatef(dimx*0.4,dimy*0.4,dimz-0.1);
		drawBox(10,5,0.1);
	glPopMatrix();

	//cadeiras
	glBindTexture(GL_TEXTURE_2D, 10);
	glPushMatrix();
	glTranslatef(2,2,2);
	glRotatef(45,0,1,0);
	glScalef(0.1,0.1,0.1);
	drawCadeira();
	glPopMatrix();

	glPushMatrix();
	glTranslatef(37,2,2);
	glRotatef(-45,0,1,0);
	glScalef(0.1,0.1,0.1);
	drawCadeira();
	glPopMatrix();

	glPushMatrix();
	glTranslatef(2,2,37);
	glRotatef(135,0,1,0);
	glScalef(0.1,0.1,0.1);
	drawCadeira();
	glPopMatrix();

	glPushMatrix();
	glTranslatef(37,2,37);
	glRotatef(-135,0,1,0);
	glScalef(0.1,0.1,0.1);
	drawCadeira();
	glPopMatrix();

	//placard lobos
	glBindTexture(GL_TEXTURE_2D, 304);
	glBegin(GL_POLYGON);
		glNormal3d(0.0,0.0,-1.0);
		glTexCoord2f(1,0); glVertex3d(dimy-(dimy*0.1), dimy*0.1, 0.1);
		glTexCoord2f(0,0); glVertex3d(dimy*0.1, dimy*0.1,  0.1);
		glTexCoord2f(0,1); glVertex3d(dimy*0.1, dimy-(dimy*0.1),  0.1);	
		glTexCoord2f(1,1); glVertex3d(dimy-(dimy*0.1), dimy-(dimy*0.1), 0.1);
	glEnd();

	//bandeira port
	glBindTexture(GL_TEXTURE_2D, 303);
	glBegin(GL_POLYGON);
		glNormal3d(0.0,0.0,-1.0);
		glTexCoord2f(1,0); glVertex3d(dimx*0.45, dimy*0.6, 0.1);
		glTexCoord2f(0,0); glVertex3d(dimx*0.3, dimy*0.6,  0.1);
		glTexCoord2f(0,1); glVertex3d(dimx*0.3, dimy-(dimy*0.1),  0.1);	
		glTexCoord2f(1,1); glVertex3d(dimx*0.45, dimy-(dimy*0.1), 0.1);
	glEnd();

	//bandeira korea
	glBindTexture(GL_TEXTURE_2D, 307);
	glBegin(GL_POLYGON);
		glNormal3d(0.0,0.0,-1.0);
		glTexCoord2f(1,0); glVertex3d(dimx*0.75, dimy*0.6, 0.1);
		glTexCoord2f(0,0); glVertex3d(dimx*0.6, dimy*0.6,  0.1);
		glTexCoord2f(0,1); glVertex3d(dimx*0.6, dimy-(dimy*0.1),  0.1);	
		glTexCoord2f(1,1); glVertex3d(dimx*0.75, dimy-(dimy*0.1), 0.1);
	glEnd();

	glBindTexture(GL_TEXTURE_2D, 306);
	//frente
	glBegin(GL_POLYGON);
		glNormal3d(0.0,0.0,1.0);
		glTexCoord2f(0.0,0);  glVertex3d(0, 0, dimz);
		glTexCoord2f(3.0,0); glVertex3d(dimx, 0,  dimz);
		glTexCoord2f(3.0,1); glVertex3d(dimx, dimy,  dimz);	
		glTexCoord2f(0.0,1);  glVertex3d(0, dimy, dimz);
	glEnd();

	//direita
	glBegin(GL_POLYGON);
		glNormal3d(1.0,0.0,0.0);
		glTexCoord2f(0.0,0);  glVertex3d(dimx, 0, dimz);
		glTexCoord2f(2.0,0); glVertex3d(dimx, 0,  0);
		glTexCoord2f(2.0,1); glVertex3d(dimx, dimy,  0);	
		glTexCoord2f(0.0,1);  glVertex3d(dimx, dimy, dimz);
	glEnd();

	//tras
	glBegin(GL_POLYGON);
		glNormal3d(0.0,0.0,-1.0);
		glTexCoord2f(0.0,0);  glVertex3d(dimx, 0, 0);
		glTexCoord2f(3.0,0); glVertex3d(0, 0,  0);
		glTexCoord2f(3.0,1); glVertex3d(0, dimy,  0);	
		glTexCoord2f(0.0,1);  glVertex3d(dimx, dimy, 0);
	glEnd();

	//esquerda
	glBegin(GL_POLYGON);
		glNormal3d(-1.0,0.0,0.0);
		glTexCoord2f(0.0,0);  glVertex3d(0, 0, 0);
		glTexCoord2f(2.0,0); glVertex3d(0, 0,  dimz);
		glTexCoord2f(2.0,1); glVertex3d(0, dimy,  dimz);	
		glTexCoord2f(0.0,1);  glVertex3d(0, dimy, 0);
	glEnd();

	//chao preto
	myMaterial(CHAO);
	glBindTexture(GL_TEXTURE_2D, 301);
	glBegin(GL_POLYGON);
		glNormal3d(0.0,1.0,0.0);
		glTexCoord2f(0.0,0.0);  glVertex3d(0, 0, dimz);
		glTexCoord2f(0.0,2); glVertex3d(dimx, 0,  dimz);
		glTexCoord2f(1.0,2); glVertex3d(dimx, 0,  0);	
		glTexCoord2f(1.0,0.0);  glVertex3d(0, 0, 0);
	glEnd();

	//chao azul
	glBindTexture(GL_TEXTURE_2D, 302);
	glBegin(GL_POLYGON);
		glNormal3d(0.0,1.0,0.0);
		glTexCoord2f(0.0,0.0);  glVertex3d(dimx*0.1, 0.1, dimz*0.9);
		glTexCoord2f(0.0,2); glVertex3d(dimx*0.9, 0.1,  dimz*0.9);
		glTexCoord2f(1.0,2); glVertex3d(dimx*0.9, 0.1,  dimz*0.1);	
		glTexCoord2f(1.0,0.0);  glVertex3d(dimx*0.1, 0.1, dimz*0.1);
	glEnd();

	//chao vermelho
	glBindTexture(GL_TEXTURE_2D, 300);
	glBegin(GL_POLYGON);
		glNormal3d(0.0,1.0,0.0);
		glTexCoord2f(0.0,0.0);  glVertex3d(dimx*0.4, 0.2, dimz*0.6);
		glTexCoord2f(0.0,2); glVertex3d(dimx*0.6, 0.2,  dimz*0.6);
		glTexCoord2f(1.0,2); glVertex3d(dimx*0.6, 0.2,  dimz*0.4);	
		glTexCoord2f(1.0,0.0);  glVertex3d(dimx*0.4, 0.2, dimz*0.4);
	glEnd();

	//tecto
	myMaterial(TELHA);
	glBindTexture(GL_TEXTURE_2D, 11);
	glBegin(GL_POLYGON);
		glNormal3d(0.0,1.0,0.0);
		glTexCoord2f(0.0,0.0);  glVertex3d(0, dimy, dimz);
		glTexCoord2f(0.0,2); glVertex3d(dimx, dimy,  dimz);
		glTexCoord2f(1.0,2); glVertex3d(dimx, dimy,  0);	
		glTexCoord2f(1.0,0.0);  glVertex3d(0, dimy, 0);
	glEnd();

	//telhado do hospital
	//triangulos
	glBegin(GL_POLYGON);
		glNormal3d(0.0,1.0,0.0);
		glTexCoord2f(0.0,0.0);  glVertex3d(0, dimy, 0);
		glTexCoord2f(0.0,2); glVertex3d(dimx*0.25, dimy+(dimy*0.3),  dimz*0.5);
		glTexCoord2f(1.0,2); glVertex3d(0, dimy,  dimz);
	glEnd();
	glBegin(GL_POLYGON);
		glNormal3d(0.0,1.0,0.0);
		glTexCoord2f(0.0,0.0);  glVertex3d(dimx, dimy, 0);
		glTexCoord2f(0.0,2); glVertex3d(dimx-(dimx*0.25), dimy+(dimy*0.3),  dimz*0.5);
		glTexCoord2f(1.0,2); glVertex3d(dimx, dimy, dimz);
	glEnd();

	//quadrilateros 
	glBegin(GL_POLYGON);
		glNormal3d(0.0,1.0,0.0);
		glTexCoord2f(0.0,0.0);  glVertex3d(0, dimy, 0);
		glTexCoord2f(0.0,2); glVertex3d((dimx*0.25), dimy+(dimy*0.3),  dimz*0.5);
		glTexCoord2f(1.0,2); glVertex3d(dimx-(dimx*0.25), dimy+(dimy*0.3), dimz*0.5);	
		glTexCoord2f(1.0,0.0);  glVertex3d(dimx, dimy, 0);
	glEnd();
	glBegin(GL_POLYGON);
		glNormal3d(0.0,1.0,0.0);
		glTexCoord2f(0.0,0.0);  glVertex3d(0 , dimy, dimz);
		glTexCoord2f(0.0,2); glVertex3d((dimx*0.25), dimy+(dimy*0.3),  dimz*0.5);
		glTexCoord2f(1.0,2); glVertex3d(dimx-(dimx*0.25), dimy+(dimy*0.3), dimz*0.5);	
		glTexCoord2f(1.0,0.0);  glVertex3d(dimx, dimy, dimz);
	glEnd();

	//Porta
	myMaterial(PAREDE);
	glBindTexture(GL_TEXTURE_2D, 10);
	glBegin(GL_POLYGON);
		glNormal3d(0.0,0.0,1.0);
		glVertex3d((dimx*0.5)-(dimx*0.5*0.2), 0, dimz+0.1);
		glVertex3d((dimx*0.5)+(dimx*0.5*0.2), 0,  dimz+0.1);
		glVertex3d((dimx*0.5)+(dimx*0.5*0.2), dimy*0.7, dimz+0.1);	
		glVertex3d((dimx*0.5)-(dimx*0.5*0.2), dimy*0.7, dimz+0.1);
	glEnd();

	glPopMatrix();

}
//cenario 2
void drawHome() {
	glPushMatrix();
	glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, 10);
	myMaterial(MADEIRA);
	glTranslatef(3.5,10.5,0.0);
	drawMesa();

	glTranslatef(-90,-17,-90);
	glScalef(5,5,5);
	drawDojo(40,13,40);
	glDisable(GL_TEXTURE_2D);

	glPopMatrix();
}


//desenha cenario
void drawScene() {
	glPushMatrix();

	if (cena_jogo==0) {
		glEnable(GL_LIGHT0);
		glDisable(GL_LIGHT1);
		glDisable(GL_LIGHT2);
		glDisable(GL_LIGHT3);
		glDisable(GL_LIGHT4);
		glDisable(GL_LIGHT5);
		glDisable(GL_LIGHT6);
		glDisable(GL_LIGHT7);

		drawEmptySpace();

	} else if (cena_jogo==1) {
		glEnable(GL_LIGHT0);
		glEnable(GL_LIGHT1);
		glEnable(GL_LIGHT2);
		glEnable(GL_LIGHT3);
		glEnable(GL_LIGHT4);
		glDisable(GL_LIGHT5);
		glDisable(GL_LIGHT6);
		glDisable(GL_LIGHT7);
		
		drawTimeCapsule();
		
	} else if (cena_jogo==2) {
		glEnable(GL_LIGHT0);
		glDisable(GL_LIGHT1);
		glDisable(GL_LIGHT2);
		glDisable(GL_LIGHT3);
		glDisable(GL_LIGHT4);
		glDisable(GL_LIGHT5);
		glDisable(GL_LIGHT6);
		glDisable(GL_LIGHT7);
		
		drawHome();

	}

	glPopMatrix();
}
