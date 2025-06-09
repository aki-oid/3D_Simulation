#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#define FILE_HEADER_SIZE 14
#define XMAX 2000
#define YMAX 2000

FILE *fp1,*fp2;

unsigned char buf10[10];
unsigned char buf2[2];
unsigned char buf20[20];
unsigned int image_start_point,header_size;
unsigned char r1[YMAX*YMAX],g1[XMAX*YMAX],b1[XMAX*YMAX];
unsigned char r2[YMAX*YMAX],g2[XMAX*YMAX],b2[XMAX*YMAX];
unsigned int width, height;
unsigned short color;
unsigned int dumlong;

void read_file1(),write_file1(),image_filter();


main(int argc, char *argv[])
{
	int i,j,k;
	unsigned char buf[1024];
	unsigned int image_start_point,header_size;
	unsigned int dumlong;
	unsigned short color;

	if( argc < 3 ) {
		fprintf(stderr," Usage: b <original> <filename>\n");
		exit(-1);
	}

	fp1 = fopen(argv[1],"rb");
	if( fp1 == 0 ) { // �t�@�C�����J���̂Ɏ��s
		fprintf(stderr," File open error : %s\n",argv[1]);
		exit(-1);
	} else { // ������͐���
		fprintf(stderr,"File open success : %s\n",argv[1]);
	}

	fp2 = fopen(argv[2],"wb");
	if( fp1 == 0 ) {
		fprintf(stderr," File open error : %s\n",argv[2]);
		exit(-1);
	} else {
		fprintf(stderr,"File open success : %s\n",argv[2]);
	}

	read_file1();// read_file1�Ƃ����֐����Ăяo���܂��B
	fprintf(stderr," Fnish reading file\n");
	image_filter();
	fprintf(stderr," Fnish processing a filter\n");
	write_file1();// write_file1�Ƃ����֐����Ăяo���܂��B

}

void image_filter(){
	int i,j,k;

	k=0;
	for(j=0;j<height;j++){
  	  for(i=0;i<width;i++){
	        r2[k]=r1[k];
	        g2[k]=g1[k];
	        b2[k]=b1[k];
		k++;
	  }
	}
}


// read_file1�Ƃ������O�̊֐��ł��B�@void�͖߂�l�����������܂��B
// �ʏ�́@int read_file �Ƃ��āA�G���[�̏ꍇ�A-1�Ȃǂ�Ԃ��悤�ɐ݌v���܂��B
//�@���̊֐��̒��� windows��bitmap�t�@�C�����A�Ђ炷��ǂ�ł��邾���ł��B
// bitmap�t�@�C���́A�����ȏ������܂݂܂��B
// ���̊֐��ł̓t���J���[(24BIT)�ŁA���k�����̏ꍇ�����l���Ă��܂���B

void read_file1(){
	int i,j,k;
	unsigned char buf[1024];
	unsigned int width1, height1;

/* �P�D�w�b�_���̓ǂݎ��
	  1.1  �t�@�C���w�b�_�@14�o�C�g�̓ǂݎ�� */

// �o�C�i���̃t�@�C����ǂݎ��Ƃ��́@fread �֐����g���܂��B
//                     �������ނƂ��́@fwrite�֐����g���܂��B
// �A�X�L�[�̃t�@�C����ǂݎ��Ƃ��́@fgets�֐����g���܂��B
//  �@�@�@�@�@�@�@�@�@ �������ނƂ��́@fprintf ���������ȁA���g���܂��B
//
	fread(buf10,sizeof(unsigned char),10,fp1);  // 10byte
	fread(&image_start_point,sizeof(unsigned int),1,fp1); // 14byte

	//  1.2 �摜�w�b�_�̓ǂݎ��@�iimage_start_point - 14�j�o�C�g
	//  (1) �w�b�_�T�C�Y
	fread(&header_size,sizeof(unsigned int),1,fp1); //18byte
        fprintf(stderr," header_size = %ld\n", header_size);
	if( image_start_point == (14 + header_size) ) {
		fprintf(stderr," header size OK! \n");
	} else {
		fprintf(stderr," header size error \n");
		exit(-1);
	} 
	//  (2) �摜�T�C�Y�@�i���ƍ����j
	fread(&width1,sizeof(unsigned int),1,fp1); // 22byte
	fread(&height1,sizeof(unsigned int),1,fp1); //26byte
	fprintf(stderr," Width, Height = %d %d\n", width1, height1);

// �����͂�����Ƒ�؁B�ŏ��ɌŒ�ŉ�f�l���i�[����z���600x600�Œ�`���Ă��܂��B
// ����𒴂���T�C�Y�̉摜�t�@�C����ǂݎ�낤�Ƃ���Ɣz�񂪑���܂���B
// �����ɓǂݍ��ނƃ������̏������߂Ȃ��̈�֏������݂ɍs���܂��̂�
// �G���[���������܂��B
// �������ݕs�\�ȗ̈�֏����������Ƃ���ƁA�ʏ�A���A�v���O�����͗����܂��B
// segmentation violation �̂悤�ȃG���[���b�Z�[�W���o��ł��傤�B

	if( width1* height1 > XMAX*YMAX ) {
		fprintf(stderr," width1 = %d, height1 = %d. \n", width1,height1);
		fprintf(stderr," XMAX = %d, YMAX = %d. \n", XMAX,YMAX);
		fprintf(stderr," The size of the image is too Big!. You have to modify the source code.\n");
		fprintf(stderr," #define XMAX, #define YMAX define the array size.\n");
		exit(-1);
	}
	width = width1;
	height = height1;

	//  (3) �Q�o�C�g��΂��i�v���[�����j
 	fread(buf2,sizeof(char),2,fp1); //28byte
	//  (4) Color �T�C�Y�@�i���̃v���O������24bit true color �̂݁j
	fread(&color,sizeof(unsigned short),1,fp1); //30byte
	if( color == 24 ) {  // 24bit true color
		fprintf(stderr," It's 24bit true color. OK! \n");
	} else {
		fprintf(stderr," Oh! No. It's not 24bit color. I hate it.\n");
		exit(-1);
	}
	//  (5) ���k�`��
	fread(&dumlong,sizeof(unsigned int),1,fp1); //34byte
	if( dumlong == 0 ) {  // �����k
		fprintf(stderr," It's non compress data. OK! \n");
	} else {
		fprintf(stderr," Oh! No. Why did you compress the image ? %d\n",dumlong);
		exit(-1);
	}

 	fread(buf20,sizeof(char),20,fp1);

	k=0;
	for(j=0;j<height1;j++){
	    for(i=0;i<width1;i++){
		fread(&b1[k],sizeof(unsigned char),1,fp1);
		fread(&g1[k],sizeof(unsigned char),1,fp1);
		fread(&r1[k],sizeof(unsigned char),1,fp1);
		k++;
	    }
	    if( (3*width)%4 != 0 ) fread(buf,sizeof(unsigned char), (3*width)%4,fp1);
	}
}

void write_file1(){
	int i,j,k;
	unsigned int width2, height2;

/* �P�D�w�b�_���̓ǂݎ��
	  1.1  �t�@�C���w�b�_�@14�o�C�g�̓ǂݎ�� */
	fwrite(buf10,sizeof(unsigned char),10,fp2);  // 10byte
	fwrite(&image_start_point,sizeof(unsigned int),1,fp2); // 14byte

	//  1.2 �摜�w�b�_�̓ǂݎ��@�iimage_start_point - 14�j�o�C�g
	//  (1) �w�b�_�T�C�Y
	fprintf(stderr," write ; header size = %d\n", header_size);
	fwrite(&header_size,sizeof(unsigned int),1,fp2); //18byte

	//  (2) �摜�T�C�Y�@�i���ƍ����j
	width2 = width;
	height2 = height;
	fwrite(&width2,sizeof(unsigned int),1,fp2); // 22byte
	fwrite(&height2,sizeof(unsigned int),1,fp2); //26byte
	fprintf(stderr," Width, Height = %d %d\n", width2, height2);

	//  (3) �Q�o�C�g��΂��i�v���[�����j
 	fwrite(buf2,sizeof(char),2,fp2); //28byte
	//  (4) Color �T�C�Y�@�i���̃v���O������24bit true color �̂݁j
	fwrite(&color,sizeof(unsigned short),1,fp2); //30byte

	//  (5) ���k�`��
	fwrite(&dumlong,sizeof(unsigned int),1,fp2); //34byte
	if( dumlong == 0 ) {  // �����k
		fprintf(stderr," It's non compress data. OK! \n");
	} else {
		fprintf(stderr," Oh! No. Why did you compress the image ? %d\n",dumlong);
		exit(-1);
	}
	//  (6) �c��̂Q�O�o�C�g
 	fread(buf20,sizeof(char),20,fp2);  // 54byte

	k=0;
	for(j=0;j<height;j++){
	    for(i=0;i<width;i++){
		fwrite(&b2[k],sizeof(unsigned char),1,fp2);
		fwrite(&g2[k],sizeof(unsigned char),1,fp2);
		fwrite(&r2[k],sizeof(unsigned char),1,fp2);
		k++;
	    }
	    if( (3*width)%4 != 0 ) fwrite(buf2,sizeof(unsigned char), (3*width)%4,fp2);
	}
}
