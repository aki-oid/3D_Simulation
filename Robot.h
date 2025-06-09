/*  myShape.h   Copyright (c) 2003 by T. HAYASHI and K. KATO  */
/*                                       All rights reserved  */

//���P�b�g�Ƃ��ĕ����̐}�`�����f�����O���A���X�g������

#define PI 3.1415926534
#define RIGHT 0
#define LEFT 1
#define X 0
#define Y 1
#define Z 2
#define RANGE 15

int i;
float arm_width = 15.0;
static int str_num , motion_num, left_right, i_xyz;
int walk_frame = 0;
int is_walking = FALSE;
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
	float xyz_def[2][3];//���A���
	float xyz[2][3];
	float flag_xyz[3][3];//�������������3�_

	float angle[2];
	float axis_x[2], axis_y[2], axis_z[2];
	double length[2];
	GLUquadric* quad[2];
};

void rad_length_Leg(struct Robot_Leg* a,int n);
void rad_length_Arm(struct Robot_Arm* a,int n);
void flag_Arm(struct Robot_Arm* a);//�r�̈ʒu������̈ʒu�����߂�֐�

struct Robot_Leg Leg_LR[2];
struct Robot_Arm Arm_LR[2];

void rad_length_Leg(struct Robot_Leg* a, int n) {
	float dx, dy, dz;
	float zx = 0.0, zy = 0.0, zz = 1.0;	// Z���x�N�g��
	float dot, len1, len2;
	
	// �ʒu�ƕ����ƒ������v�Z����
	dx = a->xyz[n+1][X] - a->xyz[n][X];
	dy = a->xyz[n+1][Y] - a->xyz[n][Y];
	dz = a->xyz[n+1][Z] - a->xyz[n][Z];
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
void rad_length_Arm(struct Robot_Arm* a, int n) {
	float dx, dy, dz;
	float zx = 0.0, zy = 0.0, zz = 1.0;	// Z���x�N�g��
	float dot, len1, len2;
	
	// �ʒu�ƕ����ƒ������v�Z����
	dx = a->xyz[n+1][X] - a->xyz[n][X];
	dy = a->xyz[n+1][Y] - a->xyz[n][Y];
	dz = a->xyz[n+1][Z] - a->xyz[n][Z];
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

void flag_Arm(struct Robot_Arm* a) {
	float vx, vy, vz, length;
	float nx, ny; // �����x�N�g��

	// �x�N�g��v = P1 - P0
	vx = a->xyz[1][X] - a->xyz[0][X];
	vy = a->xyz[1][Y] - a->xyz[0][Y];
	vz = a->xyz[1][Z] - a->xyz[0][Z];

	// v�̒���
	length = sqrt(vx*vx + vy*vy);

	// �O���_�iP2�j: P2 = P1 + v
	a->flag_xyz[0][X] = a->xyz[1][X] + vx;
	a->flag_xyz[0][Y] = a->xyz[1][Y] + vy;
	a->flag_xyz[0][Z] = a->xyz[1][Z] + vz;

	// �����ȒP�ʃx�N�g���i���v���90�x�j
	nx =  vy / length;
	ny = -vx / length;

	if (a->xyz[2][Y] + ny * length >= a->flag_xyz[0][Y]) {
		nx = -nx;// ������ɍs���Ă��܂� �� ���]������
		ny = -ny;
	}
	// �����`�̊��̊p: flag_xyz[2] = P1 + ����������length�i��
	a->flag_xyz[2][X] = a->xyz[1][X] + nx * length;
	a->flag_xyz[2][Y] = a->xyz[1][Y] + ny * length;
	a->flag_xyz[2][Z] = a->xyz[1][Z];

	// �c��̊��̓_: flag_xyz[1] = flag_xyz[2] + v
	a->flag_xyz[1][X] = a->flag_xyz[2][X] + vx;
	a->flag_xyz[1][Y] = a->flag_xyz[2][Y] + vy;
	a->flag_xyz[1][Z] = a->flag_xyz[0][Z];
}

void walkAnimation(int value) {
	if (is_walking) {
		float walk_amplitude = 1.0f;
		float walk_speed = 0.005f;

		// ���E�̑������݂ɑO��ɓ�����
		for (int lr = RIGHT; lr <= LEFT; lr++) {
			float phase = (lr == RIGHT) ? 0.0f : 3.1415f; // �E���ƍ����ňʑ������炷
			Leg_LR[lr].xyz[1][Z] = Leg_LR[lr].xyz_def[1][Z] + walk_amplitude * sin(walk_frame * walk_speed * 3+ phase);
		}

		walk_frame++;
		glutPostRedisplay();
		glutTimerFunc(16, walkAnimation, 0); // ��60FPS
	}
}
GLuint createRound() {//�n��
	GLuint listID = glGenLists(1);
	glNewList(listID, GL_COMPILE);
		glPushMatrix();
			glColor3f(1.0,1.0,1.0);
			glBegin(GL_LINES);//�n�ʂ̃O���b�h*/
				for(int i= -RANGE;i<=RANGE;i+=2){
					glVertex3f((float)i,0,-RANGE);
					glVertex3f((float)i,0,RANGE);
					glVertex3f(-RANGE,0,(float)i);
					glVertex3f(RANGE,0,(float)i);
				}
			glEnd();
		glPopMatrix();
	glEndList();
	return listID;
}
