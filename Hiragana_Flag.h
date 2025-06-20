
#define MODE_select 0//選択モード
#define MODE_flag 1//手旗信号モード
#define MODE_play 2//散策モード
int mode = MODE_select;

#define  Input_StrSize 11
#define  Word_Val 72//文字の種類数
wchar_t inputString[Input_StrSize];
unsigned char revolveFlag = GL_FALSE;/*停止*/
unsigned char wordsFlag = GL_FALSE;/*文字入力済みの判断*/

struct Flag_Word {
	wchar_t word;
	int word_flag[5];
	int reverse[5];
	char roma[4];
};

struct Flag_pose {//右手左手の向き
	float pose[2];
};

struct Coordinates {// 座標を表す構造体
	float coord[3];
};

struct Flag_Word word[Word_Val] = {
	{L'あ', {9, 3, 16, 0, 0}, {0, 0, 0, 0, 0}, "A"},   // 0
	{L'い', {3, 2, 16, 0, 0}, {0, 0, 0, 0, 0}, "I"},   // 1
	{L'う', {6, 9, 16, 0, 0}, {0, 0, 0, 0, 0}, "U"},   // 2
	{L'え', {1, 2, 1, 16, 0}, {0, 1, 0, 0, 0}, "E"},   // 3
	{L'お', {1, 2, 3, 16, 0}, {0, 0, 0, 0, 0}, "O"},   // 4
	
	{L'か', {8, 3, 16, 0, 0}, {0, 0, 0, 0, 0}, "Ka"},   // 5
	{L'き', {6, 2, 16, 0, 0}, {0, 0, 0, 0, 0}, "Ki"},   // 6
	{L'く', {11, 12, 16, 0, 0}, {0, 0, 0, 0, 0}, "Ku"}, // 7
	{L'け', {7, 3, 16, 0, 0}, {0, 0, 0, 0, 0}, "Ke"},   // 8
	{L'こ', {8, 1, 16, 0, 0}, {0, 0, 0, 0, 0}, "Ko"},   // 9
	
	{L'さ', {1, 13, 16, 0, 0}, {0, 0, 0, 0, 0}, "Sa"},  // 10
	{L'し', {5, 7, 16, 0, 0}, {0, 0, 0, 0, 0}, "Shi"},  // 11
	{L'す', {1, 2, 5, 16, 0}, {0, 0, 0, 0, 0}, "Su"},   // 12
	{L'せ', {9, 7, 16, 0, 0}, {0, 0, 0, 0, 0}, "Se"},   // 13
	{L'そ', {5, 3, 16, 0, 0}, {0, 0, 0, 0, 0}, "So"},   // 14
	
	{L'た', {11, 12, 5, 16, 0}, {0, 0, 0, 0, 0}, "Ta"},  // 15
	{L'ち', {7, 2, 16, 0, 0}, {0, 1, 0, 0, 0}, "Chi"},  // 16
	{L'つ', {13, 3, 16, 0, 0}, {0, 0, 0, 0, 0}, "TSu"}, // 17
	{L'て', {6, 3, 16, 0, 0}, {0, 0, 0, 0, 0}, "Te"},   // 18
	{L'と', {2, 5, 16, 0, 0}, {0, 0, 0, 0, 0}, "To"},   // 19
	
	{L'な', {1, 3, 16, 0, 0}, {0, 0, 0, 0, 0}, "Na"},   // 20
	{L'に', {6, 16, 0, 0, 0}, {0, 0, 0, 0, 0}, "Ni"},   // 21
	{L'ぬ', {9, 4, 16, 0, 0}, {0, 0, 0, 0, 0}, "Nu"},   // 22
	{L'ね', {9, 2, 1, 16, 0}, {0, 0, 0, 0, 0}, "Ne"},   // 23
	{L'の', {3, 16, 0, 0, 0}, {0, 0, 0, 0, 0}, "No"},   // 24
	
	{L'は', {10, 16, 0, 0, 0}, {0, 0, 0, 0, 0}, "Ha"},  // 25
	{L'ひ', {1, 7, 16, 0, 0}, {0, 0, 0, 0, 0}, "Hi"},   // 26
	{L'ふ', {9, 16, 0, 0, 0}, {0, 0, 0, 0, 0}, "Fu"},   // 27
	{L'へ', {4, 16, 0, 0, 0}, {0, 0, 0, 0, 0}, "He"},   // 28
	{L'ほ', {1, 2, 10, 16, 0}, {0, 0, 0, 0, 0}, "Ho"},  // 29
	
	{L'ま', {9, 5, 16, 0, 0}, {0, 0, 0, 0, 0}, "Ma"},   // 30
	{L'み', {6, 1, 16, 0, 0}, {0, 0, 0, 0, 0}, "Mi"},   // 31
	{L'む', {7, 5, 16, 0, 0}, {0, 0, 0, 0, 0}, "Mu"},   // 32
	{L'め', {3, 5, 16, 0, 0}, {0, 0, 0, 0, 0}, "Me"},   // 33
	{L'も', {6, 7, 16, 0, 0}, {0, 0, 0, 0, 0}, "Mo"},   // 34

	{L'や', {8, 4, 16, 0, 0}, {0, 0, 0, 0, 0}, "Ya"},   // 35
	{L'ゆ', {9, 1, 16, 0, 0}, {0, 0, 0, 0, 0}, "Yu"},   // 36
	{L'よ', {8, 6, 16, 0, 0}, {0, 0, 0, 0, 0}, "Yo"},   // 37

	{L'ら', {5, 9, 16, 0, 0}, {0, 0, 0, 0, 0}, "Ra"},   // 38
	{L'り', {13, 16, 0, 0, 0}, {0, 0, 0, 0, 0}, "Ri"},  // 39
	{L'る', {3, 7, 16, 0, 0}, {0, 0, 0, 0, 0}, "Ru"},   // 40
	{L'れ', {7, 16, 0, 0, 0}, {0, 0, 0, 0, 0}, "Re"},   // 41
	{L'ろ', {7, 8, 16, 0, 0}, {0, 0, 0, 0, 0}, "Ro"},   // 42

	{L'わ', {2, 9, 16, 0, 0}, {0, 0, 0, 0, 0}, "Wa"},   // 43
	{L'を', {1, 9, 16, 0, 0}, {0, 0, 0, 0, 0}, "Wo"},   // 44
	{L'ん', {5, 1, 16, 0, 0}, {0, 0, 0, 0, 0}, "N"},   // 45

	{L'が', {8, 3, 14, 16, 0}, {0, 0, 0, 0, 0}, "Ga"},   // 46
	{L'ぎ', {6, 2, 14, 16, 0}, {0, 0, 0, 0, 0}, "Gi"},   // 47
	{L'ぐ', {11, 12, 14, 16, 0}, {0, 0, 0, 0, 0}, "Gu"},  // 48
	{L'げ', {7, 3, 14, 16, 0}, {0, 0, 0, 0, 0}, "Ge"},   // 49
	{L'ご', {8, 1, 14, 16, 0}, {0, 0, 0, 0, 0}, "Go"},   // 50

	{L'ざ', {1, 13, 14, 16, 0}, {0, 0, 0, 0, 0}, "Za"},  // 51
	{L'じ', {5, 7, 14, 16, 0}, {0, 0, 0, 0, 0}, "Zi"},   // 52
	{L'ず', {1, 2, 5, 14, 16}, {0, 0, 0, 0, 0}, "Zu"},   // 53
	{L'ぜ', {9, 7, 14, 16, 0}, {0, 0, 0, 0, 0}, "Ze"},   // 54
	{L'ぞ', {5, 3, 14, 16, 0}, {0, 0, 0, 0, 0}, "Zo"},   // 55

	{L'だ', {11, 12, 5, 14, 16}, {0, 0, 0, 0, 0}, "Da"},  // 56
	{L'ぢ', {7, 2, 0, 14, 16}, {0, 1, 0, 0, 0}, "Di"},   // 57
	{L'づ', {13, 3, 14, 16, 0}, {0, 0, 0, 0, 0}, "Du"},  // 58
	{L'で', {6, 3, 14, 16, 0}, {0, 0, 0, 0, 0}, "De"},   // 59
	{L'ど', {2, 5, 14, 16, 0}, {0, 0, 0, 0, 0}, "Do"},   // 60
	
	{L'ば', {10, 14, 16, 0, 0}, {0, 0, 0, 0, 0}, "Ba"},  // 61
	{L'び', {1, 7, 14, 16, 0}, {0, 0, 0, 0, 0}, "Bi"},   // 62
	{L'ぶ', {9, 14, 16, 0, 0}, {0, 0, 0, 0, 0}, "Bu"},   // 63
	{L'べ', {4, 14, 16, 0, 0}, {0, 0, 0, 0, 0}, "Be"},   // 64
	{L'ぼ', {1, 2, 10, 14, 16}, {0, 0, 0, 0, 0}, "Bo"},  // 65

	{L'ぱ', {10, 15, 16, 0, 0}, {0, 0, 0, 0, 0}, "Pa"},  // 66
	{L'ぴ', {1, 7, 15, 16, 0}, {0, 0, 0, 0, 0}, "Pi"},   // 67
	{L'ぷ', {9, 15, 16, 0, 0}, {0, 0, 0, 0, 0}, "Pu"},   // 68
	{L'ぺ', {4, 15, 16, 0, 0}, {0, 0, 0, 0, 0}, "Pe"},   // 69
	{L'ぽ', {1, 2, 10, 15, 16}, {0, 0, 0, 0, 0}, "Po"},  // 70

	{L'ー', {2, 16, 0, 0, 0}, {0, 0, 0, 0, 0}, "-"}  // 71
};

struct Flag_pose flag_pose[16] = {
//{右手、左手}
	{6,2},    // 1
	{4,0},    // 2
	{7,3},    // 3
	{5,1},    // 4
	{3,5},    // 5

	{6,5},    // 6
	{4,2},    // 7
	{6,0},    // 8
	{6,7},    // 9
	{5,3},    // 10

	{3,3},      // 11->12
	{7,7},     // 11->12
	{4,4},    // 13
	{0,3},    // 14
	{5,0},     // 15
	{0,0}     // 16　文字区切りの形
};

float flag_locate[8][2][3] = {//{右手、左手}	x:-1+7/4sinx, y:2.5+7/2cosx
	{{-1.25, 0.75, 0.5}, {1.25, 0.75, 0.5}},    // 0
	{{0.52, 1.62, 0.5}, {2.23, 1.26, 0.25}},    // 1	120
	{{0.75, 2.50, 0.5}, {2.75, 2.50, 0.25}},    // 2		90
	{{0.51, 3.38, 1.2}, {2.52, 3.38, 0.25}},   // 3		60
	{{-1.88, 4.02, 0.25}, {1.88, 4.02, 0.25}},    // 4			-30
	{{-2.52, 3.38, 0.25}, {-0.51, 3.38, 1.2}},    // 5		-60
	{{-2.75, 2.50, 0.25}, {-0.75, 2.5, 0.5}},    // 6	-90
	{{-2.23, 1.26, 0.25}, {-0.52, 1.62, 0.5}}     // 7		-135
};

int check_chars() {
	int i, j;
	int ret = TRUE;

	for (j = 0; j < Input_StrSize && inputString[j] != L'\0'; j++) {
		int found = FALSE;
		for (i = 0; i < Word_Val; i++) {
			if (word[i].word == inputString[j]) {
				found = TRUE;
				break;
			}
		}
		if (!found) {
			wprintf(L"ひらがなではっきり言え！！：%lc\n", inputString[j]);
			ret = FALSE;
		}
	}
	return ret;
}

float get_coordinates_xyz_from_char(int hand, int str_number, int motion_number, int xyz_number) {//文字から座標を取得
	struct Coordinates current_coord;
	int posing, locationing, i;
	float ret;

	for(i=0;i<Word_Val;i++){
		if(word[i].word == inputString[str_number])
			break;
	}
	if(i<Word_Val){//入力された文字が見つかった場合
		posing = word[i].word_flag[motion_number];//左右の手の組み合わせ
		if(posing==0){
			current_coord.coord[xyz_number] = -100;//空白
		}else{//指定された片方の手の位置
			if(word[i].reverse[motion_number]==1){
				if(hand==0)//左右の反転
					locationing = flag_pose[posing-1].pose[1];
				else
					locationing = flag_pose[posing-1].pose[0];
			}else{
				locationing = flag_pose[posing-1].pose[hand];
			}
			current_coord.coord[xyz_number] = flag_locate[locationing][hand][xyz_number];
		}
	}
	ret = current_coord.coord[xyz_number];
	return ret;
}

char * reverse_roma(int str_number) {//文字から座標を取得
	int i;
	char *ret;

	for(i=0;i<Word_Val;i++){
		if(word[i].word == inputString[str_number])
			break;    
	}
	if(i<Word_Val){//入力された文字が見つかった場合
		ret = word[i].roma;
	}else{//見つからなかった場合
		ret = "//";
	}
	return ret;
}
