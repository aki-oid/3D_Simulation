/*  myShape.h   Copyright (c) 2003 by T. HAYASHI and K. KATO  */
/*                                       All rights reserved  */

#define PI 3.1415926534
//���P�b�g�Ƃ��ĕ����̐}�`�����f�����O���A���X�g������

int i;
float arm_width = 15.0;
static int str_num , motion_num ;

struct Robot_Leg
{
	float xyz_def[2][3];//�t�����A����
	float xyz[2][3];

	float angle[2];
	float axis_x[2], axis_y[2], axis_z[2];
	float length[2];
	GLUquadric* quad[2];
};
struct Robot_Arm
{
	float xyz_def[3][3];//���A�I�A���
	float xyz[3][3];

	float angle[2];
	float axis_x[2], axis_y[2], axis_z[2];
	double length[2];
	GLUquadric* quad[2];
};

void rad_length_Leg(struct Robot_Leg* a,int n);
void rad_length_Arm(struct Robot_Arm* a,int n);

struct Robot_Leg right_Leg, left_Leg;
struct Robot_Arm right_Arm, left_Arm;


GLuint createRound() {//�n��
	GLuint listID = glGenLists(1);
	glNewList(listID, GL_COMPILE);
		glPushMatrix();
			glBegin(GL_LINES);//�n�ʂ̃O���b�h*/
				for(int i= -35;i<36;i+=2){
					glVertex3f((float)i,0,-35.0);
					glVertex3f((float)i,0,35.0);
					glVertex3f(-50.0,0,(float)i);
					glVertex3f(50.0,0,(float)i);
				}
			glEnd();
		glPopMatrix();
	glEndList();
	return listID;
}

void rad_length_Leg(struct Robot_Leg* a,int n) {
	float dx, dy, dz;
	float zx = 0.0, zy = 0.0, zz = 1.0;	// Z���x�N�g��
	float dot, len1, len2;
	
	// �ʒu�ƕ����ƒ������v�Z����
	dx = a->xyz[n+1][0] - a->xyz[n][0];
	dy = a->xyz[n+1][1] - a->xyz[n][1];
	dz = a->xyz[n+1][2] - a->xyz[n][2];
	a->length[n] = sqrt(dx * dx + dy * dy + dz * dz); // �����i�����j

	// ��]�� (Z�x�N�g�� �~ (dx, dy, dz)) �O��
	a->axis_x[n] = zy * dz - zz * dy;
	a->axis_y[n] = zz * dx - zx * dz;
	a->axis_z[n] = zx * dy - zy * dx;

	// ��]�p (Z�x�N�g���ƃ^�[�Q�b�g�x�N�g���̂Ȃ��p)
	dot = zx * dx + zy * dy + zz * dz; // ����
	len1 = sqrt(zx * zx + zy * zy + zz * zz); // Z�x�N�g���̒����i��1�j
	len2 = sqrt(dx * dx + dy * dy + dz * dz); // �ڕW�x�N�g���̒���
	a->angle[n] = acos(dot / (len1 * len2)) * 180.0 / PI; // acos�����W�A�����x�ɕϊ�
}
void rad_length_Arm(struct Robot_Arm* a,int n) {
	float dx, dy, dz;
	float zx = 0.0, zy = 0.0, zz = 1.0;	// Z���x�N�g��
	float dot, len1, len2;
	
	// �ʒu�ƕ����ƒ������v�Z����
	dx = a->xyz[n+1][0] - a->xyz[n][0];
	dy = a->xyz[n+1][1] - a->xyz[n][1];
	dz = a->xyz[n+1][2] - a->xyz[n][2];
	a->length[n] = sqrt(dx * dx + dy * dy + dz * dz); // �����i�����j

	// ��]�� (Z�x�N�g�� �~ (dx, dy, dz)) �O��
	a->axis_x[n] = zy * dz - zz * dy;
	a->axis_y[n] = zz * dx - zx * dz;
	a->axis_z[n] = zx * dy - zy * dx;

	// ��]�p (Z�x�N�g���ƃ^�[�Q�b�g�x�N�g���̂Ȃ��p)
	dot = zx * dx + zy * dy + zz * dz; // ����
	len1 = sqrt(zx * zx + zy * zy + zz * zz); // Z�x�N�g���̒����i��1�j
	len2 = sqrt(dx * dx + dy * dy + dz * dz); // �ڕW�x�N�g���̒���
	a->angle[n] = acos(dot / (len1 * len2)) * 180.0 / PI; // acos�����W�A�����x�ɕϊ�
}
