/*  c6-2.c   Copyright (c) 2003 by T. HAYASHI and K. KATO  */
/*                                    All rights reserved  */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <wchar.h>   // wchar_t、fgetws など
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
#define XYZ_NUM 3

int xBegin = 0, yBegin = 0;
int mButton;
int idling = 0;
float distance, twist, elevation, azimuth;
float theta = 15;

float diffuse[] = { 1.0, 1.0, 1.0, 1.0 };//拡散反射光
float specular[] = { 0.2, 0.2, 0.2, 1.0 };//鏡面反射成分
float ambient[] = { 0.2, 0.2, 0.2, 1.0 };//環境反射成分
float shininess = 8.0;//環境光
float light_position[] = {4.0, 8.0, 5.0, 1.0};

int ReadBitMapData(char *pFilename,int *width,int *height,unsigned char **ppixel);
static char draw_str[input_StrSize],str[input_StrSize];
GLuint Round_List;

void polarview( void );
void resetview( void );
void drawNormal( float*, float* );

void DrawString(char *str, float x0,float y0, double w, double h);
void getInputString();

void initTexture(void);

void display(void)
{
	glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glPushMatrix ();
		polarview();//カメラビュー
		glLightfv(GL_LIGHT0, GL_POSITION, light_position);
		
		glEnable( GL_DEPTH_TEST );
			glMaterialfv( GL_FRONT_AND_BACK, GL_DIFFUSE, diffuse );//表面属性の設定
			glMaterialfv( GL_FRONT_AND_BACK, GL_SPECULAR, specular );
			glMaterialfv( GL_FRONT_AND_BACK, GL_AMBIENT, ambient );
			//glMaterialf( GL_FRONT, GL_SHININESS, shininess );

			glEnable( GL_LIGHTING );
				glEnable(GL_COLOR_MATERIAL);
					glColorMaterial(GL_FRONT, GL_AMBIENT_AND_DIFFUSE);
					glColor3f(0.8,0.8,0.8);
					
////////////////////////////////////////////////////////////////////////////////////////////////////
					glPushMatrix ();

/////左足
						if((right_Leg.xyz[0][0] == 0.0)&&(right_Leg.xyz[0][1] == 0.0)&&(right_Leg.xyz[0][2] == 0.0)){
							if((right_Leg.xyz_def[0][0] == 0.0)&&(right_Leg.xyz_def[0][1] == 0.0)&&(right_Leg.xyz_def[0][2] == 0.0)){
								right_Leg.xyz_def[0][0] = -0.75;//脛
								right_Leg.xyz_def[0][1] = 1.5;
								right_Leg.xyz_def[0][2] = 0.0;
							}
							for(i=0;i<XYZ_NUM;i++)
								right_Leg.xyz[0][i] = right_Leg.xyz_def[0][i];
						}
						if((right_Leg.xyz[1][0] == 0.0)&&(right_Leg.xyz[1][1] == 0.0)&&(right_Leg.xyz[1][2] == 0.0)){
							if((right_Leg.xyz_def[1][0] == 0.0)&&(right_Leg.xyz_def[1][1] == 0.0)&&(right_Leg.xyz_def[1][2] == 0.0)){
								right_Leg.xyz_def[1][0] = -1.0;//足先
								right_Leg.xyz_def[1][1] = 0.5;
								right_Leg.xyz_def[1][2] = 0.0;
							}
							for(i=0;i<XYZ_NUM;i++)
								right_Leg.xyz[1][i] = right_Leg.xyz_def[1][i];
						}

						glPushMatrix();//脛
							glTranslatef(right_Leg.xyz[0][0], right_Leg.xyz[0][1], right_Leg.xyz[0][2]);// 開始点に移動
							rad_length_Leg(&right_Leg,0);
							if (right_Leg.axis_x[0] != 0.0 || right_Leg.axis_y[0] != 0.0 || right_Leg.axis_z[0] != 0.0)
								glRotatef(right_Leg.angle[0], right_Leg.axis_x[0], right_Leg.axis_y[0], right_Leg.axis_z[0]);

							right_Leg.quad[0] = gluNewQuadric();
							gluCylinder(right_Leg.quad[0], 0.25, 0.25, right_Leg.length[0], 20, 20); // 半径0.1、長さlength

							gluDeleteQuadric(right_Leg.quad[0]); // クワドリックオブジェクトを削除
						glPopMatrix();

						glPushMatrix();//足先
							glTranslatef(right_Leg.xyz_def[1][0],right_Leg.xyz_def[1][1],right_Leg.xyz_def[1][2]);
							glScalef(0.5, 0.5, 0.5);

							glutSolidSphere( 1.0, 10, 10 );
						glPopMatrix();

//左足
						if((left_Leg.xyz[0][0] == 0.0)&&(left_Leg.xyz[0][1] == 0.0)&&(left_Leg.xyz[0][2] == 0.0)){
							if((left_Leg.xyz_def[0][0] == 0.0)&&(left_Leg.xyz_def[0][1] == 0.0)&&(left_Leg.xyz_def[0][2] == 0.0)){
								left_Leg.xyz_def[0][0] = 0.75;//脛
								left_Leg.xyz_def[0][1] = 1.5;
								left_Leg.xyz_def[0][2] = 0.0;
							}
							for(i=0;i<XYZ_NUM;i++)
								left_Leg.xyz[0][i] = left_Leg.xyz_def[0][i];
						}
						if((left_Leg.xyz[1][0] == 0.0)&&(left_Leg.xyz[1][1] == 0.0)&&(left_Leg.xyz[1][2] == 0.0)){
							if((left_Leg.xyz_def[1][0] == 0.0)&&(left_Leg.xyz_def[1][1] == 0.0)&&(left_Leg.xyz_def[1][2] == 0.0)){
								left_Leg.xyz_def[1][0] = 1.0;//足先
								left_Leg.xyz_def[1][1] = 0.5;
								left_Leg.xyz_def[1][2] = 0.0;
							}
							for(i=0;i<XYZ_NUM;i++)
								left_Leg.xyz[1][i] = left_Leg.xyz_def[1][i];
						}

						glPushMatrix();//脛
							glTranslatef(left_Leg.xyz[0][0], left_Leg.xyz[0][1], left_Leg.xyz[0][2]);// 開始点に移動
							rad_length_Leg(&left_Leg,0);
							if (left_Leg.axis_x[0] != 0.0 || left_Leg.axis_y[0] != 0.0 || left_Leg.axis_z[0] != 0.0)
								glRotatef(left_Leg.angle[0], left_Leg.axis_x[0], left_Leg.axis_y[0], left_Leg.axis_z[0]);

							left_Leg.quad[0] = gluNewQuadric();
							gluCylinder(left_Leg.quad[0], 0.25, 0.25, left_Leg.length[0], 20, 20); // 半径0.1、長さlength

							gluDeleteQuadric(left_Leg.quad[0]); // クワドリックオブジェクトを削除
						glPopMatrix();

						glPushMatrix();//足先
							glTranslatef(left_Leg.xyz_def[1][0],left_Leg.xyz_def[1][1],left_Leg.xyz_def[1][2]);
							glScalef(0.5, 0.5, 0.5);

							glutSolidSphere( 1.0, 10, 10 );
						glPopMatrix();

//胴体
						glPushMatrix();
							glTranslatef(0.0,2.25,0.0);
							glScalef(2.0, 1.5, 0.99);
							glutSolidCube( 1.0 );
						glPopMatrix();

//右腕
						if((right_Arm.xyz[0][0] == 0.0)&&(right_Arm.xyz[0][1] == 0.0)&&(right_Arm.xyz[0][2] == 0.0)){
							if((right_Arm.xyz_def[0][0] == 0.0)&&(right_Arm.xyz_def[0][1] == 0.0)&&(right_Arm.xyz_def[0][2] == 0.0)){
								right_Arm.xyz_def[0][0] = -1.0;//肩
								right_Arm.xyz_def[0][1] = 2.5;
								right_Arm.xyz_def[0][2] = 0.25;
							}
							for(i=0;i<XYZ_NUM;i++)
								right_Arm.xyz[0][i] = right_Arm.xyz_def[0][i];
						}
						if((right_Arm.xyz_def[1][0] == 0.0)&&(right_Arm.xyz_def[1][1] == 0.0)&&(right_Arm.xyz_def[1][2] == 0.0)){
							/*right_Arm.xyz_def[1][0] = -1.5;//肘
							right_Arm.xyz_def[1][1] = 2.0;
							right_Arm.xyz_def[1][2] = 0.0;*/
							for(i=0;i<XYZ_NUM;i++)
								right_Arm.xyz_def[1][i] = flag_locate[7][0][i];
						}
						if(wordsFlag==TRUE){
							for(i=0;i<XYZ_NUM;i++)
								right_Arm.xyz[1][i] = get_coordinates_xyz_from_char(0, str_num, motion_num, i);
						}else{
							for(i=0;i<XYZ_NUM;i++)
								right_Arm.xyz[1][i] = right_Arm.xyz_def[1][i];
						}
						/*/////////////////////////
						if((right_Arm.xyz_def[2][0] == 0.0)&&(right_Arm.xyz_def[2][1] == 0.0)&&(right_Arm.xyz_def[2][2] == 0.0)){
							for(i=0;i<XYZ_NUM;i++)//手首
								right_Arm.xyz_def[2][i] = flag_locate[7][0][i];
						}
						if(wordsFlag == TRUE){                                                                                                                                                                                                          ){
							for(i=0;i<XYZ_NUM;i++)
								right_Arm.xyz[2][i] = get_coordinates_xyz_from_char(c, 0, 0, i);
						}else{
							for(i=0;i<XYZ_NUM;i++)
								right_Arm.xyz[2][i] = right_Arm.xyz_def[2][i];
						}
						*/////////////////2/////////
						for(i=0;i<1;i++){//肩から手首まで
							glPushMatrix();
								glTranslatef(right_Arm.xyz[i][0], right_Arm.xyz[i][1], right_Arm.xyz[i][2]);// 開始点に移動
								rad_length_Arm(&right_Arm,i);
								if (right_Arm.axis_x[i] != 0.0 || right_Arm.axis_y[i] != 0.0 || right_Arm.axis_z[i] != 0.0)
									glRotatef(right_Arm.angle[i], right_Arm.axis_x[i], right_Arm.axis_y[i], right_Arm.axis_z[i]);

								right_Arm.quad[i] = gluNewQuadric();// 円柱を作成
								gluCylinder(right_Arm.quad[i], 0.25, 0.25, right_Arm.length[i], 20, 20); // 半径0.1、長さlength

								gluDeleteQuadric(right_Arm.quad[i]); // クワドリックオブジェクトを削除
							glPopMatrix();
						}
						///////////////////////////////////////2///////////////////
						glPushMatrix();//手先
							glTranslatef(right_Arm.xyz[1][0],right_Arm.xyz[1][1],right_Arm.xyz[1][2]);
							glScalef(0.5, 0.5, 0.5);

							glutSolidSphere( 1.0, 10, 10 );
						glPopMatrix();
//左腕
						if((left_Arm.xyz[0][0] == 0.0)&&(left_Arm.xyz[0][1] == 0.0)&&(left_Arm.xyz[0][2] == 0.0)){
							if((left_Arm.xyz_def[0][0] == 0.0)&&(left_Arm.xyz_def[0][1] == 0.0)&&(left_Arm.xyz_def[0][2] == 0.0)){
								left_Arm.xyz_def[0][0] = 1.0;//肩
								left_Arm.xyz_def[0][1] = 2.5;
								left_Arm.xyz_def[0][2] = 0.25;
							}
							for(i=0;i<XYZ_NUM;i++)
								left_Arm.xyz[0][i] = left_Arm.xyz_def[0][i];
						}
						if((left_Arm.xyz_def[1][0] == 0.0)&&(left_Arm.xyz_def[1][1] == 0.0)&&(left_Arm.xyz_def[1][2] == 0.0)){
							/*left_Arm.xyz_def[1][0] = 1.5;//肘
							left_Arm.xyz_def[1][1] = 2.0;
							left_Arm.xyz_def[1][2] = 0.0;*/
							for(i=0;i<XYZ_NUM;i++)
								left_Arm.xyz_def[1][i] = flag_locate[1][1][i];
						}
						if(wordsFlag==TRUE){
							
							for(i=0;i<XYZ_NUM;i++)
								left_Arm.xyz[1][i] = get_coordinates_xyz_from_char(1, str_num, motion_num, i);
						}else{
							for(i=0;i<XYZ_NUM;i++)
								left_Arm.xyz[1][i] = left_Arm.xyz_def[1][i];
						}
						/*/////////////////////////
						if((left_Arm.xyz_def[2][0] == 0.0)&&(left_Arm.xyz_def[2][1] == 0.0)&&(left_Arm.xyz_def[2][2] == 0.0)){
							for(i=0;i<XYZ_NUM;i++)//手首
								left_Arm.xyz_def[2][i] = flag_locate[1][1][i];
						}
						if(wordsFlag == TRUE){
							for(i=0;i<XYZ_NUM;i++)
								left_Arm.xyz[2][i] = get_coordinates_xyz_from_char(c, 1, 0, i);
						}else{
							for(i=0;i<XYZ_NUM;i++)
								left_Arm.xyz[2][i] = left_Arm.xyz_def[2][i];
						}
						/*////////////////2/////////
						for(i=0;i<1;i++){//肩から手首まで
							glPushMatrix();
								glTranslatef(left_Arm.xyz[i][0], left_Arm.xyz[i][1], left_Arm.xyz[i][2]);// 開始点に移動
								rad_length_Arm(&left_Arm,i);
								if (left_Arm.axis_x[i] != 0.0 || left_Arm.axis_y[i] != 0.0 || left_Arm.axis_z[i] != 0.0)
									glRotatef(left_Arm.angle[i], left_Arm.axis_x[i], left_Arm.axis_y[i], left_Arm.axis_z[i]);

								left_Arm.quad[i] = gluNewQuadric();// 円柱を作成
								gluCylinder(left_Arm.quad[i], 0.25, 0.25, left_Arm.length[i], 20, 20); // 半径0.1、長さlength

								gluDeleteQuadric(left_Arm.quad[i]); // クワドリックオブジェクトを削除
							glPopMatrix();
						}
						//////////////////////////////////2///////////////////
						glPushMatrix();//手先
							glTranslatef(left_Arm.xyz[1][0],left_Arm.xyz[1][1],left_Arm.xyz[1][2]);
							glScalef(0.5, 0.5, 0.5);

							glutSolidSphere( 1.0, 10, 10 );
						glPopMatrix();
						
//顔
						glPushMatrix();//顔
							glTranslatef(0.0,4.0,0.0);
							glScalef(3.0, 2.0, 1.99);
							glutSolidCube( 1.0 );
						glPopMatrix();

						glPushMatrix();//帽子
							glTranslatef(0.0,5.0,0.0);
							glScalef(0.75, 0.75, 0.5);
							glutSolidCube( 1.0 );
						glPopMatrix();

//////////////////////////////////////////////////////////////////////////
					glPopMatrix ();
				glDisable(GL_COLOR_MATERIAL);

				glEnable(GL_TEXTURE_2D);
					glBindTexture(GL_TEXTURE_2D, 1);
					glPushMatrix();// 顔の表情
						glTranslatef(0.0, 4.0, 1.0);  
						glBegin(GL_QUADS);
							glTexCoord2f(0.0, 1.0); glVertex3f(-1.5, 1.0, 0.0);
							glTexCoord2f(1.0, 1.0); glVertex3f( 1.5, 1.0, 0.0);
							glTexCoord2f(1.0, 0.0); glVertex3f( 1.5,-1.0, 0.0);
							glTexCoord2f(0.0, 0.0); glVertex3f(-1.5,-1.0, 0.0);
						glEnd();
					glPopMatrix();
					
					glBindTexture(GL_TEXTURE_2D, 2);
					glPushMatrix();// 服＿表
						glTranslatef(0.0,2.25,0.5);
						glBegin(GL_QUADS);
							glTexCoord2f(0.0, 1.0); glVertex3f(-1, 0.75, 0.0);
							glTexCoord2f(1.0, 1.0); glVertex3f( 1, 0.75, 0.0);
							glTexCoord2f(1.0, 0.0); glVertex3f( 1,-0.75, 0.0);
							glTexCoord2f(0.0, 0.0); glVertex3f(-1,-0.75, 0.0);
						glEnd();
					glPopMatrix();

					glBindTexture(GL_TEXTURE_2D, 3);
					glPushMatrix();// 服＿表
						glTranslatef(0.0,2.25,-0.5);
						glBegin(GL_QUADS);
							glTexCoord2f(0.0, 1.0); glVertex3f(-1, 0.75, 0.0);
							glTexCoord2f(1.0, 1.0); glVertex3f( 1, 0.75, 0.0);
							glTexCoord2f(1.0, 0.0); glVertex3f( 1,-0.75, 0.0);
							glTexCoord2f(0.0, 0.0); glVertex3f(-1,-0.75, 0.0);
						glEnd();
					glPopMatrix();
				glDisable(GL_TEXTURE_2D);
			glDisable( GL_LIGHTING );
			

			glPushMatrix();//点光源イメージ
				glColor3f(1.0,1.0,0.0);
				glTranslatef(light_position[0], light_position[1], light_position[2]);
				glutSolidSphere( 1.0, 10, 10 );
			glPopMatrix();

			glColor3f(1.0,1.0,1.0);
			glCallList(Round_List); // ディスプレイリストとしてを呼び出して描画
		glDisable( GL_DEPTH_TEST );
	glPopMatrix ();
	
	glPushMatrix();//文字
		if(wordsFlag==TRUE){
			DrawString(draw_str,140,400,500,500);
		}else{
			DrawString("Robo-ko",175,400,500,500);
		}
	glPopMatrix();
	
	glutSwapBuffers(); 
}

void drawNormal( float *v0, float *v1 )
{
	float x0, y0, z0;

	glColor3f( 1.0, 0.0, 0.0 );
	glLineWidth( 2.0 );
	x0= v0[0], y0= v0[1], z0= v0[2];
	glPushMatrix();
		glTranslatef( x0, y0, z0 );
		glBegin( GL_LINES );
			glVertex3f( 0.0, 0.0, 0.0 );
			glVertex3fv( v1 );
		glEnd();
	glPopMatrix();
}

void DrawString(char *str, float x0,float y0, double w, double h)
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

void idle(void)//イベントがなければidleがつねに実行される*/
{
	if(wordsFlag==FALSE){
		for(i = 0;i < input_StrSize; i++){
			inputString[i] =  L'\0';
			draw_str[i] =  L'\0';
		}
		str_num = 0;
		motion_num = 0;

		getInputString(); // ← 入力を先に取得する
	}else{
		if((motion_num == 0)&&(idling==0)){
			strcat(draw_str,reverse_roma(str_num));
			wprintf(L" %c ",inputString[str_num]);
		}
		idling++;

		if(idling == 500){
			idling = 0;

			motion_num++; 
			if((motion_num >= 5)||(get_coordinates_xyz_from_char(0, str_num, motion_num, i) == -100)){
				str_num++;
				motion_num = 0;
			}
			if(inputString[str_num] == L'\0'){
				Story_selif(1);
				Story_selif(0);
				printf("\n文章「%ls」を出力し終えました\n",inputString);
				Story_selif(1);
				revolveFlag = !revolveFlag;
				wordsFlag = !wordsFlag;
			}
		}
	}
	glutPostRedisplay();//ディスプレイコールバックバック関数(display)を実行*/
}

void myKbd( unsigned char key, int x, int y )
{
	switch( key ) {
	case 'R':
		resetview();
		glutPostRedisplay();
		break;
	case ' ':
		revolveFlag = !revolveFlag;
		if(revolveFlag == GL_TRUE)
			glutIdleFunc(idle); //idleを繰り返し実行するように設定
		else
			glutIdleFunc(NULL);//アニメーションをとめる
		break;
	case KEY_ESC:
		exit( 0 );
	}
}

void myMouse(int button, int state, int x, int y)
{
	if ((wordsFlag == GL_TRUE)&&(state == GLUT_DOWN)) {
		xBegin = x;
		yBegin = y;
		mButton = button;
	}
}

void myMotion(int x, int y)
{
	if(wordsFlag == GL_TRUE){
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

void initTexture1_face( void)
{
	unsigned char *image1[1];
	int imageHeightb, imageWidthb;
	
	if(!ReadBitMapData("face.bmp",&imageWidthb,&imageHeightb,image1)){	
		printf("Error! \n");
		exit(0);
	}
	glBindTexture(GL_TEXTURE_2D,1);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S,GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T,GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER,GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER,GL_NEAREST);
	glTexImage2D(GL_TEXTURE_2D, 0, 4,imageWidthb ,imageHeightb,0,
								GL_RGBA, GL_UNSIGNED_BYTE, *image1);
	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_DECAL);
}
void initTexture2_body1( void)
{
	unsigned char *image2[1];
	int imageHeightb, imageWidthb;
	
	if(!ReadBitMapData("body1.bmp",&imageWidthb,&imageHeightb,image2)){	
		printf("Error! \n");
		exit(0);
	}
	glBindTexture(GL_TEXTURE_2D,2);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S,GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T,GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER,GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER,GL_NEAREST);
	glTexImage2D(GL_TEXTURE_2D, 0, 4,imageWidthb ,imageHeightb,0,
								GL_RGBA, GL_UNSIGNED_BYTE, *image2);
	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_DECAL);
}
void initTexture3_body2( void)
{
	unsigned char *image3[1];
	int imageHeightb, imageWidthb;
	
	if(!ReadBitMapData("body2.bmp",&imageWidthb,&imageHeightb,image3)){	
		printf("Error! \n");
		exit(0);
	}
	glBindTexture(GL_TEXTURE_2D,3);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S,GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T,GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER,GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER,GL_NEAREST);
	glTexImage2D(GL_TEXTURE_2D, 0, 4,imageWidthb ,imageHeightb,0,
								GL_RGBA, GL_UNSIGNED_BYTE, *image3);
	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_DECAL);
}

void myInit (char *progname)
{
	glutInitWindowPosition(0, 0);
	glutInitWindowSize( 600, 600);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH);
	glutCreateWindow(progname);
	glClearColor (0.5, 0.5, 0.5, 1.0);//背景色
	glutKeyboardFunc( myKbd );
	glutMouseFunc( myMouse );
	glutMotionFunc( myMotion );
	resetview();
	
	initTexture1_face();
	initTexture2_body1();
	initTexture3_body2();
	Story_selif(0);
	Story_selif(-1);
	Story_selif(1);
	glShadeModel( GL_SMOOTH );
	glEnable( GL_LIGHT0 );
	Round_List = createRound();
}

void myReshape(int width, int height)
{
	float aspect = (float) width / (float) height;

	glViewport(0, 0, width, height);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(60.0, aspect, 1.0, 40.0);
	glMatrixMode(GL_MODELVIEW);
}

void polarview( void )
{
	glTranslatef( 0.0, 0.0, -distance);
	glRotatef( -twist, 0.0, 0.0, 1.0);
	glRotatef( -elevation, 1.0, 0.0, 0.0);
	glRotatef( -azimuth, 0.0, 1.0, 0.0);
}

void resetview( void )
{
	distance = 20.0;
	twist = 0.0;
	elevation = -15.0;
	azimuth = 0.0;
}

void getInputString() {
    setlocale(LC_ALL, "");  // 日本語対応のロケール設定

    wprintf(L"\n文字列を入力してください（ひらがな%d字以内）: ",input_StrSize);
    fgetws(inputString, input_StrSize, stdin);  // 入力を取得（日本語OK）
	wordsFlag = !wordsFlag;
    inputString[wcscspn(inputString, L"\n")] = L'\0';
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
