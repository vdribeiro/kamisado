#include "Library.h"

//Iluminacao
void luzes() {
	glEnable(GL_COLOR_MATERIAL);

	// Actualizacao da posicao das fontes de luz...
	light0_position[0] = light0x;
	light0_position[1] = light0y;
	light0_position[2] = light0z;
	// por razoes de eficiencia, os restantes parametros _invariaveis_ da LIGHT0 
	// mantem os valores definidos na funcao de inicializacao
	glLightfv(GL_LIGHT0, GL_POSITION, light0_position);

	// ... e das esfera que as simbolizam
	glColor3f(1.0,1.0,0.0);		// cor amarela
	gluQuadricOrientation( glQ, GLU_INSIDE);
	glPushMatrix();
	glTranslated(light0x,light0y,light0z);
	gluSphere(glQ, symb_light0_radius, symb_light0_slices, symb_light0_stacks);
    glPopMatrix();
	gluQuadricOrientation( glQ, GLU_OUTSIDE);

	glDisable(GL_COLOR_MATERIAL);
}

//luzes da capsula
void luzescapsula() {
	glEnable(GL_COLOR_MATERIAL);

	//luz 1
	lighth1_position[0] = lighth1x;	
	lighth1_position[1] = lighth1y;	
	lighth1_position[2] = lighth1z;
	glLightf(GL_LIGHT1, GL_SPOT_CUTOFF, spot_angle);
	glLightfv(GL_LIGHT1, GL_SPOT_DIRECTION, spot_direction1);
	glLightfv(GL_LIGHT1, GL_POSITION, lighth1_position);

	glColor3f(1.0,1.0,0.0);		
	gluQuadricOrientation( glQ, GLU_INSIDE);
	glPushMatrix();
	glTranslated(lighth1x,lighth1y,lighth1z);
	gluSphere(glQ, symb_lighth_radius, symb_lighth_slices, symb_lighth_stacks);
	glPopMatrix();
	gluQuadricOrientation( glQ, GLU_OUTSIDE);

	//luz 2
	lighth2_position[0] = lighth2x;	
	lighth2_position[1] = lighth2y;	
	lighth2_position[2] = lighth2z;
	glLightf(GL_LIGHT2, GL_SPOT_CUTOFF, spot_angle);
	glLightfv(GL_LIGHT2, GL_SPOT_DIRECTION, spot_direction2);
	glLightfv(GL_LIGHT2, GL_POSITION, lighth2_position);

	glColor3f(1.0,1.0,0.0);		
	gluQuadricOrientation( glQ, GLU_INSIDE);
	glPushMatrix();
	glTranslated(lighth2x,lighth2y,lighth2z);
	gluSphere(glQ, symb_lighth_radius, symb_lighth_slices, symb_lighth_stacks);
	glPopMatrix();
	gluQuadricOrientation( glQ, GLU_OUTSIDE);

	//luz 3
	lighth3_position[0] = lighth3x;	
	lighth3_position[1] = lighth3y;	
	lighth3_position[2] = lighth3z;
	glLightf(GL_LIGHT3, GL_SPOT_CUTOFF, spot_angle);
	glLightfv(GL_LIGHT3, GL_SPOT_DIRECTION, spot_direction3);
	glLightfv(GL_LIGHT3, GL_POSITION, lighth3_position);

	glColor3f(1.0,1.0,0.0);		
	gluQuadricOrientation( glQ, GLU_INSIDE);
	glPushMatrix();
	glTranslated(lighth3x,lighth3y,lighth3z);
	gluSphere(glQ, symb_lighth_radius, symb_lighth_slices, symb_lighth_stacks);
	glPopMatrix();
	gluQuadricOrientation( glQ, GLU_OUTSIDE);

	//luz 4
	lighth4_position[0] = lighth4x;	
	lighth4_position[1] = lighth4y;	
	lighth4_position[2] = lighth4z;
	glLightf(GL_LIGHT4, GL_SPOT_CUTOFF, spot_angle);
	glLightfv(GL_LIGHT4, GL_SPOT_DIRECTION, spot_direction4);
	glLightfv(GL_LIGHT4, GL_POSITION, lighth4_position);
	
	glColor3f(1.0,1.0,0.0);		
	gluQuadricOrientation( glQ, GLU_INSIDE);
	glPushMatrix();
	glTranslated(lighth4x,lighth4y,lighth4z);
	gluSphere(glQ, symb_lighth_radius, symb_lighth_slices, symb_lighth_stacks);
	glPopMatrix();
	gluQuadricOrientation( glQ, GLU_OUTSIDE);

	glDisable(GL_COLOR_MATERIAL);
}

//cria cilindro fechado
void gluCylinder_closed(GLUquadric * glQ, GLdouble baseRadius, GLdouble topRadius, GLdouble height, GLint slices, GLint stacks)
{
    //criar cilindro
	gluCylinder(glQ, baseRadius, topRadius, height, slices, stacks);

    //fechar cilindro
	glPushMatrix();
        gluQuadricOrientation(glQ, GLU_INSIDE);
        glTranslatef(0,0,0);
        gluDisk(glQ, 0, baseRadius, slices, stacks);

		gluQuadricOrientation(glQ, GLU_OUTSIDE);
        glTranslatef(0,0,height);
        gluDisk(glQ, 0, topRadius, slices, stacks);
    glPopMatrix();

}

//Texturas
void myTexturas() {
	//Texturas do trabalho1
	pixmap.readBMPFile("Texturas/feup.bmp");
	pixmap.setTexture(1);
	pixmap.readBMPFile("Texturas/tile.bmp");
	pixmap.setTexture(2);
	pixmap.readBMPFile("Texturas/clamp.bmp");
	pixmap.setTexture(3);
	pixmap.readBMPFile("Texturas/mandril.bmp");
	pixmap.setTexture(4);
	pixmap.readBMPFile("Texturas/janela.bmp");
	pixmap.setTexture(5);
	pixmap.readBMPFile("Texturas/heliporto.bmp");
	pixmap.setTexture(6);
	pixmap.readBMPFile("Texturas/relva.bmp");
	pixmap.setTexture(7);
	pixmap.readBMPFile("Texturas/passeio.bmp");
	pixmap.setTexture(8);
	pixmap.readBMPFile("Texturas/pista.bmp");
	pixmap.setTexture(9);
	pixmap.readBMPFile("Texturas/madeira.bmp");
	pixmap.setTexture(10);
	pixmap.readBMPFile("Texturas/telhas.bmp");
	pixmap.setTexture(11);
	pixmap.readBMPFile("Texturas/cover.bmp");
	pixmap.setTexture(12);
	pixmap.readBMPFile("Texturas/ferro.bmp");
	pixmap.setTexture(13);
	pixmap.readBMPFile("Texturas/cameo.bmp");
	pixmap.setTexture(14);
	pixmap.readBMPFile("Texturas/cruz.bmp");
	pixmap.setTexture(15);

	//numeros
	pixmap.readBMPFile("Texturas/0.bmp");
	pixmap.setTexture(30);
	pixmap.readBMPFile("Texturas/1.bmp");
	pixmap.setTexture(31);
	pixmap.readBMPFile("Texturas/2.bmp");
	pixmap.setTexture(32);
	pixmap.readBMPFile("Texturas/3.bmp");
	pixmap.setTexture(33);
	pixmap.readBMPFile("Texturas/4.bmp");
	pixmap.setTexture(34);
	pixmap.readBMPFile("Texturas/5.bmp");
	pixmap.setTexture(35);
	pixmap.readBMPFile("Texturas/6.bmp");
	pixmap.setTexture(36);
	pixmap.readBMPFile("Texturas/7.bmp");
	pixmap.setTexture(37);
	pixmap.readBMPFile("Texturas/8.bmp");
	pixmap.setTexture(38);
	pixmap.readBMPFile("Texturas/9.bmp");
	pixmap.setTexture(39);

	//tabuleiro e pecas
	pixmap.readBMPFile("Texturas/torre.bmp");
	pixmap.setTexture(200);
	pixmap.readBMPFile("Texturas/torre_laranja.bmp");
	pixmap.setTexture(201);
	pixmap.readBMPFile("Texturas/torre_azul.bmp");
	pixmap.setTexture(202);
	pixmap.readBMPFile("Texturas/torre_roxa.bmp");
	pixmap.setTexture(203);
	pixmap.readBMPFile("Texturas/torre_rosa.bmp");
	pixmap.setTexture(204);
	pixmap.readBMPFile("Texturas/torre_amarela.bmp");
	pixmap.setTexture(205);
	pixmap.readBMPFile("Texturas/torre_vermelha.bmp");
	pixmap.setTexture(206);
	pixmap.readBMPFile("Texturas/torre_verde.bmp");
	pixmap.setTexture(207);
	pixmap.readBMPFile("Texturas/torre_castanho.bmp");
	pixmap.setTexture(208);
	pixmap.readBMPFile("Texturas/bordas.bmp");
	pixmap.setTexture(209);
	pixmap.readBMPFile("Texturas/tabuleiro.bmp");
	pixmap.setTexture(210);

	//ceus
	pixmap.readBMPFile("Texturas/ceu.bmp");
	pixmap.setTexture(190);
	pixmap.readBMPFile("Texturas/ceu2.bmp");
	pixmap.setTexture(191);
	pixmap.readBMPFile("Texturas/ceu3.bmp");
	pixmap.setTexture(192);
	pixmap.readBMPFile("Texturas/ceu4.bmp");
	pixmap.setTexture(193);
	pixmap.readBMPFile("Texturas/ceu5.bmp");
	pixmap.setTexture(194);
	pixmap.readBMPFile("Texturas/ceu6.bmp");
	pixmap.setTexture(195);
	pixmap.readBMPFile("Texturas/ceu7.bmp");
	pixmap.setTexture(196);
	pixmap.readBMPFile("Texturas/ceu8.bmp");
	pixmap.setTexture(197);
	pixmap.readBMPFile("Texturas/ceu9.bmp");
	pixmap.setTexture(198);
	pixmap.readBMPFile("Texturas/ceu10.bmp");
	pixmap.setTexture(199);

	//chao
	pixmap.readBMPFile("Texturas/floor.bmp");
	pixmap.setTexture(60);
	pixmap.readBMPFile("Texturas/floor2.bmp");
	pixmap.setTexture(61);
	pixmap.readBMPFile("Texturas/floor3.bmp");
	pixmap.setTexture(62);
	pixmap.readBMPFile("Texturas/floor4.bmp");
	pixmap.setTexture(63);
	pixmap.readBMPFile("Texturas/floor5.bmp");
	pixmap.setTexture(64);
	pixmap.readBMPFile("Texturas/floor6.bmp");
	pixmap.setTexture(65);
	pixmap.readBMPFile("Texturas/floor7.bmp");
	pixmap.setTexture(66);
	pixmap.readBMPFile("Texturas/floor8.bmp");
	pixmap.setTexture(67);

	//cenario
	pixmap.readBMPFile("Texturas/deathstar.bmp");
	pixmap.setTexture(240);
	pixmap.readBMPFile("Texturas/madeira3.bmp");
	pixmap.setTexture(241);
	pixmap.readBMPFile("Texturas/metal.bmp");
	pixmap.setTexture(242);
	pixmap.readBMPFile("Texturas/metal2.bmp");
	pixmap.setTexture(243);

	//cenario dojo
	pixmap.readBMPFile("Texturas/redtatami.bmp");
	pixmap.setTexture(300);
	pixmap.readBMPFile("Texturas/blacktatami.bmp");
	pixmap.setTexture(301);
	pixmap.readBMPFile("Texturas/bluetatami.bmp");
	pixmap.setTexture(302);
	pixmap.readBMPFile("Texturas/bandeiraportugal.bmp");
	pixmap.setTexture(303);
	pixmap.readBMPFile("Texturas/lobosnegros.bmp");
	pixmap.setTexture(304);
	pixmap.readBMPFile("Texturas/sacoboxe.bmp");
	pixmap.setTexture(305);
	pixmap.readBMPFile("Texturas/paredebranca.bmp");
	pixmap.setTexture(306);
	pixmap.readBMPFile("Texturas/bandeirakorea.bmp");
	pixmap.setTexture(307);
	pixmap.readBMPFile("Texturas/marcador.bmp");
	pixmap.setTexture(308);

	//varios
	pixmap.readBMPFile("Texturas/cover.bmp");
	pixmap.setTexture(222);
}

//Materiais
void myMaterial(int mat){
	glDisable(GL_COLOR_MATERIAL);
	glMaterialfv(GL_FRONT_AND_BACK,GL_AMBIENT,mat_ambient[mat]);
	glMaterialfv(GL_FRONT_AND_BACK,GL_DIFFUSE,mat_diffuse[mat]);
    glMaterialfv(GL_FRONT_AND_BACK,GL_SPECULAR,mat_specular[mat]); 
    glMaterialfv(GL_FRONT_AND_BACK,GL_EMISSION,mat_emission[mat]); 
	glMaterialfv(GL_FRONT_AND_BACK,GL_SHININESS,mat_shininess[mat]);
}

//animacao
void myUpdateTransforms_camaras(int dummy){

	if(anim_camara == TRUE){
		view_rot->disable();
		trans_xy->disable();
		trans_z->disable();
		if(camx <=40){
			camx+=0.1;
		}
		else{
			anim_camara = FALSE;
			premiu_1 = FALSE;
		}
	}else{
		view_rot->enable();
		trans_xy->enable();
		trans_z->enable();
	}
	glutTimerFunc(mili_secs_camaras, myUpdateTransforms_camaras, 0);

}


void myInitTransforms()
{
	anim_y=0;
	anim_x=0;
	anim_z=0;
	y_over=FALSE;
	//y2_over=FALSE;
	x_over=FALSE;
	z_over=FALSE;
	iniciou_anim=FALSE;
	x=0;z=0;desl_linha=0;desl_coluna=0;
	linha_dest=0;coluna_dest=0;

}
void myUpdateTransforms(int dummy)
{
	if(animate){
		//printf("animx,y,z = %f,%f,%f\n",anim_x,anim_y,anim_z);
		if((obj_actual > 0 && obj_actual <= 88) || iniciou_anim == TRUE){	
			//dados de para onde se vai mover o obj
		linha_dest = obj_actual/10;
		coluna_dest = obj_actual%10;
		//linha_actual = tab_jog_ant->linha;
		//coluna_actual = tab_jog_ant->coluna;
		x = linha_dest-linha_actual+1;
		desl_linha = x + x -2;
		z = coluna_dest-coluna_actual+1;
		desl_coluna = -(z + z -2);
		/*printf("destinos lin,col %d ,%d\n",linha_dest, coluna_dest);
		printf("desl_linha %d , jog_ant_linha %d \n",desl_linha,tab_jog_ant->linha);
		printf("desl_coluna %d , jog_ant_coluna %d\n\n",desl_coluna,tab_jog_ant->linha);
*/
		//sobe até y=5
		if(anim_y < 5 && y_over == FALSE){
			anim_y+=0.1;
			iniciou_anim=TRUE;
		}else{
			y_over = TRUE;
		}

		//se já está em y=5, avanca ate x=dest
		if(y_over == TRUE){
			if(desl_linha >0)
				if(anim_x < desl_linha){
					anim_x+=0.1;
				}else{
					x_over = TRUE;
				}
			if(desl_linha < 0)
				if(anim_x > desl_linha){
					anim_x-=0.1;
				}else{
					x_over = TRUE;
				}
		}

		if(x_over == TRUE){
			if(desl_coluna < 0)
				if(anim_z > desl_coluna){
					anim_z-=0.1;
				}else{
					z_over = TRUE;
				}
			if(desl_coluna > 0)
				if(anim_z < desl_coluna){
					anim_z+=0.1;
				}else{
					z_over = TRUE;
				}
		}
		//no caso de nao haverem os 2 deslocamentos
		if(desl_coluna == 0 && x_over == TRUE)
			z_over = TRUE;

		//se já está em x=2 desce até y=0
		if(z_over == TRUE)
			if(anim_y > 0.0){
				anim_y -= 0.1;
			}else{
				y2_over = TRUE;
				animate = FALSE;
			}
		}

		if(animate == FALSE){
			myInitTransforms();
		}
	}
	glutTimerFunc(mili_secs_pecas, myUpdateTransforms, 0);
}

