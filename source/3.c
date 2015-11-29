#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <Windows.h>

#define USER 1;
#define COM 2;

char stone[5][5] = {0};
int save[100] = {0};
int userGet = 0;
int comGet = 0;
int firstStone = 1;
int nowWho = 0;
int infoCnt = 0;
int saveCnt = 0;

int addStone();
void paint();
void robot();
void replay();
void menu();
void delStone();
int findStone(int, int, int, int, int*);
int findHouse(int *);
int isFar(int lastX, int lastY, int lastX_2nd, int lastY_2nd);

int main()
{
	int i, j;
	int choice = 0;
	char check;

	while(1){
		system("cls");
		menu();
		printf("선택> ");
		scanf("%d", &choice);

		switch(choice){
			case 0:
				return 0;
				break;
			case 1:
				for(i=0; i<5; i++){
					for(j=0; j<5; j++){
						stone[i][j] = 0;
					}
				}
				paint();
				while(1) {
					if(addStone() == -1) break;
					delStone();
					paint();

					robot();
					paint();
					delStone();
					paint();
				}
				for(i=0; i<5; i++){
					for(j=0; j<5; j++){
						stone[i][j] = 0;
					}
				}
				break;
			case 2:
				do{
					replay();
					for(i=0; i<5; i++){
						for(j=0; j<5; j++){
							stone[i][j] = 0;
						}
					}
					printf("재복기? (Y/N)");
					scanf("%c", &check);
					if(check == 'Y' || check == 'y')	continue;
					else if(check == 'N' || check == 'n')	break;
				}while(1);
				break;
		}
	}


	return 0;
}

int addStone() {
	int x = -1;
	int y = -1;
	int i = 0;

	nowWho = USER;

	for(; i<50; i++) {
		printf(" ※ X좌표,Y좌표 : ");
		scanf("%d%d", &x, &y);

		if(x == 6 && y == 6){
			return -1;
		}
		if(firstStone == 1) {
			if(x==2 && y==2) {
				continue;
			}
		}

		if(stone[x][y] == 0) {
			if(x>=0 && x<5 && y>=0 && y<5) {
				stone[x][y] = 1;
				save[saveCnt++] = 1000 + x*100 + y*10 + 1; 
				firstStone = 0;
				break;
			}
		}
		else 
			continue;
	}
	if(i >= 50){
		printf("제한 턴 수 종료\n");
		return -1;
	}
}

void delStone() {
	int i, k, isHouse, round, del, tmp, com, user;
	int locationInfo[25] = {-1};

	com = COM;
	user = USER;

	for(round=1; round<=2; round++) { // 돌 둔 사람을 먼저 처리해주고 나머지를 처리해주므로 총 2번 처리해야함 
		for(i=0; i<5; i++) {
			for(k=0; k<5; k++) {
			
				if(stone[i][k] == 0) continue; // 빈칸이 선택되었다면 계산할 가치가 없다.
				if(stone[i][k] != nowWho) continue; // 지금 돌을 막 둔사람의 돌을 먼저 처리해줌


				if(stone[i][k] != 0) {
				
					findStone(stone[i][k], i, k, 0 ,locationInfo); // 해당 위치의 돌색과 같으면서 연결된 돌들의 위치를 배열에 저장해줌.
				}

				/*
				for(tmp=0; tmp<25; tmp++) 
					printf("[%d]\n", locationInfo[tmp]);
				*/
			
				isHouse = findHouse(locationInfo); // 그곳이 집인지 판단
				//printf("********************* %d (1집발견 0 집못발견) ***************\n", isHouse);

				printf("\n\n");

				if(isHouse == 1) { // 집인경우

					for(del=0; del<25; del++) { // 해당 리스트에 있는 돌들을 삭제
					
						if(locationInfo[del] == -1) continue;

						if(stone[locationInfo[del]/10][locationInfo[del]%10] == 1)
							comGet++;
						if(stone[locationInfo[del]/10][locationInfo[del]%10] == 2)
							userGet++;

						stone[locationInfo[del]/10][locationInfo[del]%10] = 0;
						save[saveCnt++] = 1000 + (locationInfo[del]/10)*100 + (locationInfo[del]%10)*10 + 0; 



					}

				}
			


				for(tmp=0; tmp<25; tmp++) // 다음 작업을 위한 초기화
					locationInfo[tmp] = -1;
				infoCnt = 0;
			
			}
		}

		if( nowWho == user )
			nowWho = com;
		else
			nowWho = user;
			
	}
}

int findStone(int stoneNum, int x, int y, int notGo, int *locationInfo) {

	// stoneNum : 돌색(1 or 2)
	// x, y : 좌표
	// notGo : 재귀 이용했으므로 갔던길을 되돌아 오지 않도록 하는 변수.
	//    4 
	//  1   3  으로 지정하여 처리함
	//    2
	// *location : 갔던길을 저장하는 변수

	locationInfo[infoCnt++] = x*10 + y;

	if(x-1 >= 0 && notGo != 1) {
		if(stone[x-1][y] == stoneNum) {
			findStone(stone[x-1][y], x-1, y, 3, locationInfo);
		}
	}

	if(y+1 < 5 && notGo != 2) {
		if(stone[x][y+1] == stoneNum){
			findStone(stone[x][y+1], x, y+1, 4, locationInfo);
		}
	}

	if(x+1 < 5 && notGo != 3) {
		if(stone[x+1][y] == stoneNum) {
			findStone(stone[x+1][y], x+1, y, 1, locationInfo);
		}
	}

	if(y-1 >= 0 && notGo != 4) {
		if(stone[x][y-1] == stoneNum) {
			findStone(stone[x][y-1], x, y-1, 2, locationInfo);
		}
	}


	return 0;

}

int findHouse(int *location) {
	int i;
	int tmpX=-1, tmpY=-1;
	int myType, yourType;

	for(i=0; i<25; i++) {

		if(location[i] == -1)  // 배열의 끝이면 다음거 진행
			continue;

		tmpX = location[i]/10;
		tmpY = location[i]%10;
		myType = stone[tmpX][tmpY];
		if(myType == 1) yourType = 2;
		if(myType == 2) yourType = 1;
			
		if(stone[tmpX-1][tmpY] == 0 && tmpX-1 >= 0) // 4방향중 0(=빈칸)이 존재한다면 그것은 집이 아니다.
			return 0;
		if(stone[tmpX+1][tmpY] == 0 && tmpX+1 < 5)
			return 0;
		if(stone[tmpX][tmpY-1] == 0 && tmpY-1 >= 0)
			return 0;
		if(stone[tmpX][tmpY+1] == 0 && tmpY+1 < 5)
			return 0;
		
	}
	return 1; // 집인경우는 1을 리턴

}

void paint() {
	int mapWidth = 5;
	int mapHeight = 5;
	int i=0, j=0,k=0;

	system("cls");
	printf("종료를 원하시면 6 6을 입력하세요.\n");
	printf("   [SCORE] COM : %d | USR : %d \n", comGet, userGet);
	printf("   X→0   1   2   3   4\n");
	for(i=0; i<mapWidth*2+2; i++) {
		if(i==0) printf(" Y┌");
		if(i==mapWidth*2+1) {
			printf("┐");
			break;
		}
		if(i%2 ==1) printf("┬");
		else{
			printf("─");
		}

	}
	printf("\n"); printf("↓│");
	for(k=0; k<mapWidth*2+3; k++) {
	if(k%2 == 1)
		printf("│");
	else
		printf("  ");
	}

	printf("\n");
	for(i=0; i<mapHeight; i++) {
		printf(" %d├", i);
		for(j=0; j<mapWidth; j++) {
			printf("─");

			if(stone[j][i] == 0) {
				printf("┼");
			} else if(stone[j][i] == 1) {
				printf("○");
				//printf("%c", stone[j][i]);
			} else {
				printf("●");
			}
		}
		printf("─┤\n");

		printf("  │");
		for(k=0; k<mapWidth*2+3; k++) {
			if(k%2 == 1)
				printf("│");
			else
				printf("  ");
		}
		printf("\n");

	}
	for(i=0; i<mapWidth*2+2; i++) {
		if(i==0) printf("  └");
		if(i==mapWidth*2+1) {
			printf("┘");
			break;
		}
		if(i%2 ==1) printf("┴");
		else{
			printf("─");
		}
	}
	printf("\n");
}

void robot() {
	int lastX = (save[saveCnt-1] / 100) %10; // 1052 : x, y, type(0:빈칸,1:흑,2:백) 순서 1000은 더미
	int lastY = (save[saveCnt-1] / 10) % 10;

	int lastX_2nd = -1;
	int lastY_2nd = -1;
	int farChecked = 0;
	int i=0,j=0;
	int blackCnt=0;
	int maxBlackInfo = 100; 
	// ㄴ 125 : 2위치까지 검은색이 5개 있었다는 의미. 
	//    9칸의 정사각형 바둑판 기준으로, 가장 왼쪽위가 1번째자리를 시작으로 반시계반향으로 돌면서 숫자가 커짐. 최대치 8
	long seed = (long)time(NULL);
	int ranX=-1, ranY=-1;
	int w;

	nowWho = COM;
	Sleep(400);

	if(saveCnt-3>=0) {
		lastY_2nd = (save[saveCnt-3] / 10) % 10;
		lastX_2nd = (save[saveCnt-3] / 100) %10;
	}

	if(saveCnt != 1) { // 동 떨어진덴다가 둘 경우의 사전 설정
		if(isFar(lastX, lastY, lastX_2nd, lastY_2nd)) {
			lastX = lastX_2nd;
			lastY = lastY_2nd;
			farChecked = 1;
		}
	}

	if(saveCnt == 1 || farChecked == 1) { // 맨 처음이거나 사람이 동떨어진데다 둘 경우의 처리
		if(lastX-1 >=0 && stone[lastX-1][lastY] == 0) {
			stone[lastX-1][lastY] = 2;
			save[saveCnt++] = 1000 + (lastX-1)*100 + (lastY)*10 + 2; 
			return;
		}
		if(lastY-1 >=0 && stone[lastX][lastY-1] == 0) {
			stone[lastX][lastY-1] = 2;
			save[saveCnt++] = 1000 + (lastX)*100 + (lastY-1)*10 + 2; 
			return;
		}
		if(lastX+1 <= 4 && stone[lastX+1][lastY] == 0) {
			stone[lastX+1][lastY] = 2;
			save[saveCnt++] = 1000 + (lastX+1)*100 + (lastY)*10 + 2; 
			return;
		}
		if(lastY+1 <=4 && stone[lastX][lastY+1] == 0) {
			stone[lastX][lastY+1] = 2;
			save[saveCnt++] = 1000 + (lastX)*100 + (lastY+1)*10 + 2; 
			return;
		}
	}

	if(saveCnt != 1 && farChecked == 0) { // 이전의 둔 곳과 비슷한 곳에 둔 경우의 처리 
		// 최근의 컴퓨터가 둔 돌 기준으로, 사람이 둔 돌이 많은 쪽 옆에다가 돌을 두도록 한다. 
		if(saveCnt-2 >= 0) {  
			lastY_2nd = (save[saveCnt-2] / 10) % 10;
			lastX_2nd = (save[saveCnt-2] / 100) %10;

			if(stone[lastX_2nd-1][lastY_2nd-1] == 1 && stone[lastX_2nd-1][lastY_2nd-1] != 2) {
				blackCnt++;
				if(maxBlackInfo%10 < blackCnt) 
					maxBlackInfo = (maxBlackInfo/100)*100 + 10 +blackCnt;
			}
			else blackCnt = 0;
			if(stone[lastX_2nd-1][lastY_2nd] == 1 && stone[lastX_2nd-1][lastY_2nd] != 2) {
				blackCnt++;
				if(maxBlackInfo%10 < blackCnt) 
					maxBlackInfo = (maxBlackInfo/100)*100 + 20 +blackCnt;
			}
			else blackCnt = 0;
			if(stone[lastX_2nd-1][lastY_2nd+1] == 1 && stone[lastX_2nd-1][lastY_2nd+1] != 2) {
				blackCnt++;
				if(maxBlackInfo%10 < blackCnt) 
					maxBlackInfo = (maxBlackInfo/100)*100 + 30 +blackCnt;
			}
			else blackCnt = 0;
			if(stone[lastX_2nd][lastY_2nd+1] == 1 && stone[lastX_2nd][lastY_2nd+1] != 2) {
				blackCnt++;
				if(maxBlackInfo%10 < blackCnt) 
					maxBlackInfo = (maxBlackInfo/100)*100 + 40 +blackCnt;
			}
			else blackCnt = 0;
			if(stone[lastX_2nd+1][lastY_2nd+1] == 1 && stone[lastX_2nd+1][lastY_2nd+1] != 2) {
				blackCnt++;
				if(maxBlackInfo%10 < blackCnt) 
					maxBlackInfo = (maxBlackInfo/100)*100 + 50 +blackCnt;
			}
			else blackCnt = 0;
			if(stone[lastX_2nd+1][lastY_2nd] == 1 && stone[lastX_2nd+1][lastY_2nd] != 2) {
				blackCnt++;
				if(maxBlackInfo%10 < blackCnt) 
					maxBlackInfo = (maxBlackInfo/100)*100 + 60 +blackCnt;
			}
			else blackCnt = 0;
			if(stone[lastX_2nd+1][lastY_2nd-1] == 1 && stone[lastX_2nd+1][lastY_2nd-1] != 2) {
				blackCnt++;
				if(maxBlackInfo%10 < blackCnt) 
					maxBlackInfo = (maxBlackInfo/100)*100 + 70 +blackCnt;
			}
			else blackCnt = 0;
			if(stone[lastX_2nd][lastY_2nd-1] == 1 && stone[lastX_2nd][lastY_2nd-1] != 2) {
				blackCnt++;
				if(maxBlackInfo%10 < blackCnt) 
					maxBlackInfo = (maxBlackInfo/100)*100 + 80 +blackCnt;
			}
			else blackCnt = 0;
			// 해당돌 왼쪽부터 시작하여 아래쪽으로 한바퀴 돌면서 체크함

			switch( (maxBlackInfo/10)%10 ) {
			case 0:
				break;
			case 1:
				stone[lastX_2nd-1][lastY_2nd] = 2;
				save[saveCnt++] = 1000 + (lastX_2nd-1)*100 + (lastY_2nd)*10 + 2; 
				return;
			case 2:
				stone[lastX_2nd-1][lastY_2nd+1] = 2;
				save[saveCnt++] = 1000 + (lastX_2nd-1)*100 + (lastY_2nd+1)*10 + 2; 
				return;				
			case 3:
				stone[lastX_2nd][lastY_2nd+1] = 2;
				save[saveCnt++] = 1000 + (lastX_2nd)*100 + (lastY_2nd+1)*10 + 2; 
				return;
			case 4:
				stone[lastX_2nd+1][lastY_2nd+1] = 2;
				save[saveCnt++] = 1000 + (lastX_2nd+1)*100 + (lastY_2nd+1)*10 + 2; 
				return;
			case 5:
				stone[lastX_2nd+1][lastY_2nd] = 2;
				save[saveCnt++] = 1000 + (lastX_2nd+1)*100 + (lastY_2nd)*10 + 2; 
				return;
			case 6:
				stone[lastX_2nd+1][lastY_2nd-1] = 2;
				save[saveCnt++] = 1000 + (lastX_2nd+1)*100 + (lastY_2nd-1)*10 + 2; 
				return;
			case 7:
				stone[lastX_2nd][lastY_2nd-1] = 2;
				save[saveCnt++] = 1000 + (lastX_2nd)*100 + (lastY_2nd-1)*10 + 2; 
				return;
			case 8:
				stone[lastX_2nd-1][lastY_2nd-1] = 2;
				save[saveCnt++] = 1000 + (lastX_2nd-1)*100 + (lastY_2nd-1)*10 + 2; 
				return;

			}
		}
	}

	// 이도저도 아닌 경우, 랜덤한, 빈칸의, 새로운 위치에 돌을 둔다.
	srand(seed);

	for(w=0; w<25; w++) {
		ranX = rand()%5;
		ranY = rand()%5;

		if(stone[ranX][ranY] == 0 ) {
			if(stone[ranX-1][ranY] != 1 && stone[ranX][ranY-1] != 1 && stone[ranX][ranY+1] != 1 && stone[ranX+1][ranY] != 1 ) {
				stone[ranX][ranY] = 2;
				save[saveCnt++] = 1000 + (ranX)*100 + (ranY)*10 + 2; 
				return;
			}
		}
	}
	
}

int isFar(int lastX, int lastY, int lastX_2nd, int lastY_2nd) {

	if(lastX-1 != lastX_2nd || lastX+1 != lastX_2nd || lastY-1 != lastY_2nd || lastY+1 != lastY_2nd	) { // 사람이 두 돌을 인접하게 두지 않았다면
		return 1; // 멀리 둠
	}
	else 
		return 0; // 가까이 둠


}

void replay(void){
	int i;
	int x;
	int y;
	int turn;
	
	for(i=0; i<100; i++){
		if(save[i] > 0){
			x = (save[i]/100)%10;
			y = (save[i]/10)%10;
			turn = save[i]%10;
			
			stone[x][y] = turn;
			paint();
			Sleep(1000);
		}
	}
}

void menu(void){
	printf("1. 바둑 두기\n");
	printf("2. 복기 하기\n");
	printf("0. 종     료\n\n");
}