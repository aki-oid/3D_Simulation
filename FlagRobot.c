/*  c6-2.c   Copyright (c) 2003 by T. HAYASHI and K. KATO  */
/*                                    All rights reserved  */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <wchar.h>   // wchar_t�Afgetws �Ȃ�
#include <locale.h>  // setlocale
#include <math.h>
#include <GL/glut.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include "read_bitmap.h"
#include "myShape.h"
#include "hiragana_Flag.h"
#include "Robot.h"
#include "Story.h"
#define KEY_ESC 27
#define XYZ_NUM 3//3������Ԃɂ�������W�`���Bxyz
#define TEXTURE_NUM 3//�g���e�N�X�`���̐�

int xBegin = 0, yBegin = 0;
int mButton;
int idling = 0;
float distance, twist, elevation, azimuth;
float theta = 15;
float robot_pos[3] = {0.0f, 0.0f, 0.0f};  // ���{�b�g�̈ʒu (x, y, z)
float robot_angle = 0.0f; 

float diffuse[] = { 1.0, 1.0, 1.0, 1.0 };//�g�U���ˌ�
float specular[] = { 0.2, 0.2, 0.2, 1.0 };//���ʔ��ː���
float ambient[] = { 0.2, 0.2, 0.2, 1.0 };//�����ː���
float shininess = 8.0;//����
float light_position[] = {4.0, 8.0, 15.0, 1.0};

static char draw_str[Input_StrSize],str[Input_StrSize];
GLuint Round_List;

void polarview();
void resetview();
void drawNormal( float*, float* );
void drawString(char *str, float x0,float y0, double w, double h);
void getInputString();
void initTexture();
int ReadBitMapData(char *pFilename,int *width,int *height,unsigned char **ppixel);

void mySpecialKey(int key, int x, int y);
void mySpecialKeyUp(int key, int x, int y);
void walkAnimation(int value);

void display(void)
{
	glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glPushMatrix ();
		polarview();//�J�����r���[
		glLightfv(GL_LIGHT0, GL_POSITION, light_position);
		
		glEnable( GL_DEPTH_TEST );
			glMaterialfv( GL_FRONT_AND_BACK, GL_DIFFUSE, diffuse );//�\�ʑ����̐ݒ�
			glMaterialfv( GL_FRONT_AND_BACK, GL_SPECULAR, specular );
			glMaterialfv( GL_FRONT_AND_BACK, GL_AMBIENT, ambient );
			//glMaterialf( GL_FRONT, GL_SHININESS, shininess );

			glEnable( GL_LIGHTING );
				glEnable(GL_COLOR_MATERIAL);
					glColorMaterial(GL_FRONT, GL_AMBIENT_AND_DIFFUSE);
					glColor3f(0.8,0.8,0.8);

					
					glPushMatrix ();
						glTranslatef(robot_pos[0], robot_pos[1], robot_pos[2]);
						glRotatef(robot_angle, 0.0f, 1.0f, 0.0f);

		//���W�ݒ�//////////////////////////////////////////////////////////////////////////////////////////////////
						for(left_right = RIGHT; left_right <= LEFT; left_right++){
							for(int j = 0;j<2;j++){//���̍��W�ݒ�
								if((Leg_LR[left_right].xyz[j][X] == 0.0)&&(Leg_LR[left_right].xyz[j][Y] == 0.0)&&(Leg_LR[left_right].xyz[j][Z] == 0.0)){
									for(i_xyz=0;i_xyz<XYZ_NUM;i_xyz++)
										Leg_LR[left_right].xyz[j][i_xyz] = Leg_LR[left_right].xyz_def[j][i_xyz];
								}
							}
							if((Arm_LR[left_right].xyz[0][X] == 0.0)&&(Arm_LR[left_right].xyz[0][Y] == 0.0)&&(Arm_LR[left_right].xyz[0][Z] == 0.0)){
								for(i_xyz=0;i_xyz<XYZ_NUM;i_xyz++)//���̍��W�ݒ�
									Arm_LR[left_right].xyz[0][i_xyz] = Arm_LR[left_right].xyz_def[0][i_xyz];
							}

							if(wordsFlag == TRUE){//���̍��W�ݒ�
								for(i_xyz=0;i_xyz<XYZ_NUM;i_xyz++)
									Arm_LR[left_right].xyz[1][i_xyz] = get_coordinates_xyz_from_char(left_right, str_num, motion_num, i_xyz);
							}else{
								for(i_xyz=0;i_xyz<XYZ_NUM;i_xyz++)
									Arm_LR[left_right].xyz[1][i_xyz] = Arm_LR[left_right].xyz_def[1][i_xyz];
							}
							flag_Arm(&Arm_LR[left_right]);//���̍��W�ݒ�
						}
		//���f�����O//////////////////////////////////////////////////////////////////////////////////////////////////
		/////��
						for(left_right = RIGHT; left_right <= LEFT; left_right++){
							glPushMatrix();//��
								glTranslatef(Leg_LR[left_right].xyz[0][X], Leg_LR[left_right].xyz[0][Y], Leg_LR[left_right].xyz[0][Z]);// �J�n�_�Ɉړ�
								rad_length_Leg(&Leg_LR[left_right],0);
								if (Leg_LR[left_right].axis_x[0] != 0.0 || Leg_LR[left_right].axis_y[0] != 0.0 || Leg_LR[left_right].axis_z[0] != 0.0)
									glRotatef(Leg_LR[left_right].angle[0], Leg_LR[left_right].axis_x[0], Leg_LR[left_right].axis_y[0], Leg_LR[left_right].axis_z[0]);

								Leg_LR[left_right].quad[0] = gluNewQuadric();
								gluCylinder(Leg_LR[left_right].quad[0], 0.25, 0.25, Leg_LR[left_right].length[0], 20, 20); // ���a0.1�A����length

								gluDeleteQuadric(Leg_LR[left_right].quad[0]); // �N���h���b�N�I�u�W�F�N�g���폜
							glPopMatrix();

							glPushMatrix();//����
								glTranslatef(Leg_LR[left_right].xyz[1][X],Leg_LR[left_right].xyz[1][Y],Leg_LR[left_right].xyz[1][Z]);
								glScalef(0.3, 0.3, 0.3);

								glutSolidSphere( 1.0, 10, 10 );
							glPopMatrix();
						}

		////����
						glPushMatrix();
							glTranslatef(0.0,2.25,0.0);
							glScalef(2.0, 1.5, 0.99);
							glutSolidCube( 1.0 );
						glPopMatrix();

		////�r
						for(left_right = RIGHT; left_right <= LEFT; left_right++){
							for(i = 0; i < 1; i++){
								glPushMatrix();//��������܂�
									glTranslatef(Arm_LR[left_right].xyz[0][X], Arm_LR[left_right].xyz[0][Y], Arm_LR[left_right].xyz[0][Z]);// �J�n�_�Ɉړ�
									rad_length_Arm(&Arm_LR[left_right],0);
									if (Arm_LR[left_right].axis_x[0] != 0.0 || Arm_LR[left_right].axis_y[0] != 0.0 || Arm_LR[left_right].axis_z[0] != 0.0)
										glRotatef(Arm_LR[left_right].angle[0], Arm_LR[left_right].axis_x[0], Arm_LR[left_right].axis_y[0], Arm_LR[left_right].axis_z[0]);

									Arm_LR[left_right].quad[0] = gluNewQuadric();// �~�����쐬
									gluCylinder(Arm_LR[left_right].quad[0], 0.25, 0.25, Arm_LR[left_right].length[0], 20, 20); // ���a0.1�A����length

									gluDeleteQuadric(Arm_LR[left_right].quad[0]); // �N���h���b�N�I�u�W�F�N�g���폜
								glPopMatrix();
							}
		glDisable( GL_DEPTH_TEST );
							glPushMatrix();//��
								if(left_right == RIGHT)
									glColor3f(1.0,0,0);
								glBegin(GL_QUADS);
									glVertex3f( Arm_LR[left_right].xyz[1][X], Arm_LR[left_right].xyz[1][Y], Arm_LR[left_right].xyz[1][Z]);
									glVertex3f( Arm_LR[left_right].flag_xyz[0][X], Arm_LR[left_right].flag_xyz[0][Y], Arm_LR[left_right].flag_xyz[0][Z]);
									glVertex3f( Arm_LR[left_right].flag_xyz[1][X], Arm_LR[left_right].flag_xyz[1][Y], Arm_LR[left_right].flag_xyz[0][Z]);
									glVertex3f( Arm_LR[left_right].flag_xyz[2][X], Arm_LR[left_right].flag_xyz[2][Y], Arm_LR[left_right].xyz[1][Z]);
								glEnd();
							glPopMatrix();
		glEnable( GL_DEPTH_TEST );
							glColor3f(0.8,0.8,0.8);
							glPushMatrix();//���
								glTranslatef(Arm_LR[left_right].xyz[1][X],Arm_LR[left_right].xyz[1][Y],Arm_LR[left_right].xyz[1][Z]);
								glScalef(0.5, 0.5, 0.5);
								glutSolidSphere( 1.0, 10, 10 );
							glPopMatrix();
						}
						
		////��
						glPushMatrix();//��
							glTranslatef(0.0,4.0,0.0);
							glScalef(3.0, 2.0, 1.99);
							glutSolidCube( 1.0 );
						glPopMatrix();

						glPushMatrix();//�X�q
							glTranslatef(0.0,5.0,0.0);
							glScalef(0.75, 0.75, 0.5);
							glutSolidCube( 1.0 );
						glPopMatrix();

		//�e�N�X�`���}�b�s���O/////////////////////////////////////////////////////////////
						glEnable(GL_TEXTURE_2D);
							glBindTexture(GL_TEXTURE_2D, 1);
							glPushMatrix();// ��̕\��
								glTranslatef(0.0, 4.0, 1.0);  
								glBegin(GL_QUADS);
									glNormal3f(0.0, 0.0, 1.0);
									glTexCoord2f(0.0, 1.0); glVertex3f(-1.5, 1.0, 0.0);
									glTexCoord2f(1.0, 1.0); glVertex3f( 1.5, 1.0, 0.0);
									glTexCoord2f(1.0, 0.0); glVertex3f( 1.5,-1.0, 0.0);
									glTexCoord2f(0.0, 0.0); glVertex3f(-1.5,-1.0, 0.0);
								glEnd();
							glPopMatrix();

							glBindTexture(GL_TEXTURE_2D, 2);
							glPushMatrix();// �e�N�X�`���t���O��
								glTranslatef(0.0, 2.25, 0.5);
								glBegin(GL_QUADS);
									glNormal3f(0.0, 0.0, 1.0);
									glTexCoord2f(0.0, 1.0); glVertex3f(-1, 0.75, 0.0);
									glTexCoord2f(1.0, 1.0); glVertex3f( 1, 0.75, 0.0);
									glTexCoord2f(1.0, 0.0); glVertex3f( 1,-0.75, 0.0);
									glTexCoord2f(0.0, 0.0); glVertex3f(-1,-0.75, 0.0);
								glEnd();
							glPopMatrix();
							
							glBindTexture(GL_TEXTURE_2D, 3);
							glPushMatrix();// �e�N�X�`���t���w��
								glTranslatef(0.0, 2.25, -0.5);
								glBegin(GL_QUADS);
									glNormal3f(0.0, 0.0, -1.0);
									glTexCoord2f(0.0, 1.0); glVertex3f(-1, 0.75, 0.0);
									glTexCoord2f(1.0, 1.0); glVertex3f( 1, 0.75, 0.0);
									glTexCoord2f(1.0, 0.0); glVertex3f( 1,-0.75, 0.0);
									glTexCoord2f(0.0, 0.0); glVertex3f(-1,-0.75, 0.0);
								glEnd();
							glPopMatrix();
						glPopMatrix();

					glDisable(GL_TEXTURE_2D);
				glDisable(GL_COLOR_MATERIAL);
			glDisable( GL_LIGHTING );

//���̑�/////////////////////////////////////////////////////////////
			glCallList(Round_List); // �f�B�X�v���C���X�g�Ƃ��Ēn�ʂ̐����Ăяo���ĕ`��
			glPushMatrix();//�_�����C���[�W
				glColor3f(1.0,1.0,0.0);
				glTranslatef(light_position[X], light_position[Y], light_position[Z]);
				glutSolidSphere( 1.0, 10, 10 );
			glPopMatrix();
		glDisable( GL_DEPTH_TEST );
	glPopMatrix ();
	
//������\��/////////////////////////////////////////////////////////////
	glPushMatrix();
		if(wordsFlag==TRUE)
			drawString(draw_str,50,400,500,500);
		else
			drawString("Robo-ta",175,400,500,500);

		if(mode!=MODE_play){
			if((revolveFlag==FALSE)&&(wordsFlag==FALSE)){
				glPushMatrix();//����
					drawString("Waiting for input",25,700,750,750);
				glPopMatrix();
			}else if(revolveFlag==FALSE){
				glPushMatrix();//����
					drawString("STOP",25,700,750,750);
				glPopMatrix();
			}
		}else{
			glPushMatrix();//����
				drawString("Walking",25,700,750,750);
			glPopMatrix();
		}
	glPopMatrix();
	
	glutSwapBuffers(); 
}

void drawNormal( float *v0, float *v1 )
{
	float x0, y0, z0;

	glColor3f( 1.0, 0.0, 0.0 );
	glLineWidth( 2.0 );
	x0= v0[X], y0= v0[1], z0= v0[2];
	glPushMatrix();
		glTranslatef( x0, y0, z0 );
		glBegin( GL_LINES );
			glVertex3f( 0.0, 0.0, 0.0 );
			glVertex3fv( v1 );
		glEnd();
	glPopMatrix();
}

void drawString(char *str, float x0,float y0, double w, double h)
{
	int i,len;
	glMatrixMode(GL_PROJECTION);
	glPushMatrix();
		glColor3d(1.,0.,0.);
		glLoadIdentity();
		gluOrtho2D(0.,w,0.,h);
		len = strlen(str);
		for(i=0;i<len;i++){
		glRasterPos2f(x0+20*i,y0);
	//        glutStrokeString(GLUT_BITMAP_HELVETICA_18, str);
	//        glutBitmapString(GLUT_BITMAP_HELVETICA_18, str);
			glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, *str);
		str++;
		}
	//	printf(" %s %f %f %lf %lf\n",str,x0,y0,w,h);
	glPopMatrix();
	glMatrixMode(GL_MODELVIEW);
}

void idle(void)//�C�x���g���Ȃ����idle���˂Ɏ��s�����*/
{
	if(mode==MODE_select){
		while(1){
			Story_selif(0);
			printf("�����I�ǂ����I�ԁI(1�F����M�����[�h�@2�F���荞�݃��[�h)	�����p����\n �� ");
			scanf("%d",&mode);
			Story_selif(1);
			if((mode==MODE_flag)||(mode==MODE_play)){
				if(mode==MODE_flag){
					int ch;
					while ((ch = getchar()) != '\n' && ch != EOF);  // ���s��ǂݔ�΂�
				}else if(mode==MODE_play){
					Story_selif2("���荞�ݒ�\n���荞�݂��~�߂�Ƃ��́uc�{�^���v");
				}	
				break;
			}else{
				printf("\n�I������2���I����ȊO�F�߂Ȃ��I�I�F%d\n",mode);
			}
		}
	}
	if(mode==MODE_flag){
		if(wordsFlag==FALSE){
			str_num = 0;
			motion_num = 0;
			getInputString(); // �� ���͂��Ɏ擾����
			revolveFlag = !revolveFlag;
			is_walking = FALSE;
		}else {
			if((motion_num == 0)&&(idling==0)){
				Beep( 1320,100); // Hz, msec 
				strcat(draw_str,reverse_roma(str_num));
				wprintf(L" %c ",inputString[str_num]);
			}
			idling++;

			if(idling == 500){
				idling = 0;
				motion_num++; 
				if((motion_num >= 5)||(get_coordinates_xyz_from_char(RIGHT, str_num, motion_num, X) == -100)){
					str_num++;
					motion_num = 0;
				}
				if(inputString[str_num] == L'\0'){
					Beep( 1320,400); // Hz, msec 
					Story_selif(1);
					Story_selif(0);
					printf("\n���ꂪ�u%ls�v�̎���M�����I�o�������I�I\n",inputString);
					Story_selif(1);	
					for(i = 0;i < Input_StrSize; i++){
						inputString[i] =  L'\0';
						draw_str[i] =  L'\0';
					}
					wordsFlag = !wordsFlag;
					revolveFlag = !revolveFlag;
				}
			}
		}
	}
	glutPostRedisplay();//�f�B�X�v���C�R�[���o�b�N�o�b�N�֐�(display)�����s*/
}

void myKbd( unsigned char key, int x, int y )
{
	if((mode!=MODE_select)&&((key == 'w') ||(key == 's')||(key == 'd')|| (key == 'a'))){
		float step = 0.2f;

		if (key == 'w') {
			robot_pos[2] -= step;
			if (robot_pos[2]<=-RANGE)
				robot_pos[2]=-RANGE;
			robot_angle = 180.0f;
		} else if (key == 's') {
			robot_pos[2] += step;
			if (robot_pos[2]>=RANGE)
				robot_pos[2]=RANGE;
			robot_angle = 0.0f;
		} else if (key == 'd') {
			robot_pos[0] += step;
			if (robot_pos[0]>=RANGE)
				robot_pos[0]=RANGE;
			robot_angle = 90.0f;
		} else if (key == 'a') {
			robot_pos[0] -= step;
			if (robot_pos[0]<=-RANGE)
				robot_pos[0]=-RANGE;
			robot_angle = -90.0f;
		}
		is_walking = TRUE;
		walk_frame = 0;
		glutTimerFunc(0, walkAnimation, 0);
		glutPostRedisplay();
	}

	switch( key ) {
		case 'R':
			is_walking = FALSE;
			resetview();
			glutPostRedisplay();
			break;
		case 'c':
			if(mode==MODE_play)
				Story_selif2("(���U���B�B�B)\n�V�l�I�������荞�݂͏I��������H\n���낻��{��ֈڂ邩");
				mode = MODE_select;
			break;
		case ' ':
			revolveFlag = !revolveFlag;
			if(revolveFlag == GL_TRUE)
				glutIdleFunc(idle); //idle���J��Ԃ����s����悤�ɐݒ�
			else
				glutIdleFunc(NULL);//�A�j���[�V�������Ƃ߂�
			break;
		case KEY_ESC:
			Story_selif2("�ȏ�Ŗ{�u�`���I���ɂ���I���U�I");
			exit( 0 );
	}
}
void myKeyboardUp(unsigned char key, int x, int y) {
	switch (key) {
		case 'w':
		case 'a':
		case 's':
		case 'd':
			is_walking = FALSE;
			// �������̈ʒu�ɖ߂�
			for (int lr = RIGHT; lr <= LEFT; lr++) {
				for (int i = 0; i < XYZ_NUM; i++) {
					Leg_LR[lr].xyz[1][i] = Leg_LR[lr].xyz_def[1][i];
				}
			}
			glutPostRedisplay();
			break;
	}
}

void myMouse(int button, int state, int x, int y)
{
	if (((mode==MODE_play)||(wordsFlag == GL_TRUE))&&(state == GLUT_DOWN)) {
		xBegin = x;
		yBegin = y;
		mButton = button;
	}
}

void myMotion(int x, int y)
{
	if((mode==MODE_play)||(wordsFlag == GL_TRUE)){
		int xDisp, yDisp;
		xDisp = x - xBegin;
		yDisp = y - yBegin;
		switch(mButton){
		case GLUT_LEFT_BUTTON:
			azimuth += (double) xDisp/2.0;
			elevation -= (double) yDisp/2.0;
			break;
		case GLUT_MIDDLE_BUTTON:
			twist = fmod (twist + xDisp, 360.0);
			break;
		case GLUT_RIGHT_BUTTON:
			distance -= (double) yDisp/40.0;
			twist += xDisp/2.0;
			break;
		}
		xBegin = x;
		yBegin = y;
		glutPostRedisplay();
	}
}

void initTexture(void)
{
	struct {
		const char *filename;
		unsigned char *image;
		int width, height;
	}
	textures[TEXTURE_NUM] = {
		{"face.bmp", NULL, 0, 0},
		{"body1.bmp", NULL, 0, 0},
		{"body2.bmp", NULL, 0, 0}
	};

	for (int i = 0; i < TEXTURE_NUM; i++) {
		if (!ReadBitMapData((char *)textures[i].filename, &textures[i].width, &textures[i].height, &textures[i].image)) {
			printf("Error! : %s\n", textures[i].filename);
			exit(1);
		}

		glBindTexture(GL_TEXTURE_2D, i + 1); // �e�N�X�`��ID��1����
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexImage2D(GL_TEXTURE_2D, 0, 4, textures[i].width, textures[i].height, 0,
					GL_RGBA, GL_UNSIGNED_BYTE, textures[i].image);
		glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
	}
}

void myInit (char *progname)
{
	glutInitWindowPosition(0, 0);
	glutInitWindowSize( 750, 750);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH);
	glutCreateWindow(progname);
	glClearColor (0.0, 0.25, 0.75, 1.0);//�w�i�F
	glutKeyboardFunc( myKbd );
	glutKeyboardUpFunc(myKeyboardUp);//�ʏ�L�[�𗣂����Ƃ�
	glutMouseFunc( myMouse );
	glutMotionFunc( myMotion );
	resetview();
	
	initTexture();
	Story_selif2("�悭�����V�l�I\n�����͊C������M���u���̉��ł���I�I\n�����ɗ��Ă��ꂽ���ƐS�������v���I�I");
	Story_selif2("�V�l�ɂ�2�I����������I�I�I\n1�ڂ͑����u����M�����w�ԁv�A\n2�ڂ̓E�H�[�~���O�A�b�v�Ƃ��Ắu���荞�݁v");
	glShadeModel( GL_SMOOTH );
	glEnable( GL_LIGHT0 );
	Round_List = createRound();
	{//�e�֐ߍ��W�̏�����
		Leg_LR[RIGHT].xyz_def[0][X] = -0.75;//��
		Leg_LR[RIGHT].xyz_def[0][Y] = 1.5;
		Leg_LR[RIGHT].xyz_def[0][Z] = 0.0;
		Leg_LR[RIGHT].xyz_def[1][X] = -1.0;//����
		Leg_LR[RIGHT].xyz_def[1][Y] = 0.5;
		Leg_LR[RIGHT].xyz_def[1][Z] = 0.0;

		Arm_LR[RIGHT].xyz_def[0][X] = -1.0;//��
		Arm_LR[RIGHT].xyz_def[0][Y] = 2.5;
		Arm_LR[RIGHT].xyz_def[0][Z] = 0.25;

		for(i_xyz = 0; i_xyz < XYZ_NUM; i_xyz++){
			Leg_LR[LEFT].xyz_def[0][i_xyz] = Leg_LR[RIGHT].xyz_def[0][i_xyz];
			Leg_LR[LEFT].xyz_def[1][i_xyz] = Leg_LR[RIGHT].xyz_def[1][i_xyz];
			Arm_LR[LEFT].xyz_def[0][i_xyz] = Arm_LR[RIGHT].xyz_def[0][i_xyz];
			if(i_xyz == X){
				Leg_LR[LEFT].xyz_def[0][i_xyz] *= -1.0;//��
				Leg_LR[LEFT].xyz_def[1][i_xyz] *= -1.0;//����
				Arm_LR[LEFT].xyz_def[0][i_xyz] *= -1.0;//��
			}

			Arm_LR[LEFT].xyz_def[1][i_xyz] = flag_locate[1][LEFT][i_xyz];//���
			Arm_LR[RIGHT].xyz_def[1][i_xyz] = flag_locate[7][RIGHT][i_xyz];
		}
	}
}

void myReshape(int width, int height)
{
	float aspect = (float) width / (float) height;

	glViewport(0, 0, width, height);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(60.0, aspect, 1.0, 100.0);
	glMatrixMode(GL_MODELVIEW);
}

void polarview( void )
{
	glTranslatef( -robot_pos[0]*2/3, 0.0, -distance);
	glRotatef( -twist, 0.0, 0.0, 1.0);
	glRotatef( -elevation, 1.0, 0.0, 0.0);
	glRotatef( -azimuth, 0.0, 1.0, 0.0);
}

void resetview( void )
{
	distance = RANGE*4/3;
	twist = 0.0;
	elevation = -15.0;
	azimuth = 0.0;

	robot_angle = 0.0f;         // Z��������������
	robot_pos[0] = 0;         // X�}�C�i�X�����ֈړ�
	robot_pos[2] = 0;         // Z�v���X�����֌��
}

void getInputString() {
	int check=FALSE;
	setlocale(LC_ALL, "");  // ���{��Ή��̃��P�[���ݒ�
	while(1) 
	{
		Story_selif(0);
		wprintf(L"\n�����I�m�肽��������������Ă݂�I�i�Ђ炪��%d�����\���j\n �� ",Input_StrSize-1);
		fgetws(inputString, Input_StrSize, stdin);  // ���͂��擾�i���{��OK�j
		inputString[wcscspn(inputString, L"\n")] = L'\0';
		check = check_chars();
		if(check==TRUE)
			break;
		Story_selif(1);
	};
	wordsFlag = !wordsFlag;
	Story_selif(0);
}

int main(int argc, char** argv)
{
	glutInit(&argc, argv);
	myInit(argv[0]);
	glutReshapeFunc (myReshape);
	glutDisplayFunc(display);
	glutIdleFunc(idle);
	glutMainLoop(); 
	return(0);
}	
