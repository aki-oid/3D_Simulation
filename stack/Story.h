#include <stdio.h>

static int story_num = 0;


void Story_selif(int story) {
	if(story == 0){
		printf("\n***********************\n\n\n");
	}

	if(story == -1)
		printf("��낵����");

	
	if(story == 1){
		printf("\n\n\n");
		printf("***********************");
	}
}