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
		printf("����> ");
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
					printf("�纹��? (Y/N)");
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
		printf(" �� X��ǥ,Y��ǥ : ");
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
		printf("���� �� �� ����\n");
		return -1;
	}
}

void delStone() {
	int i, k, isHouse, round, del, tmp, com, user;
	int locationInfo[25] = {-1};

	com = COM;
	user = USER;

	for(round=1; round<=2; round++) { // �� �� ����� ���� ó�����ְ� �������� ó�����ֹǷ� �� 2�� ó���ؾ��� 
		for(i=0; i<5; i++) {
			for(k=0; k<5; k++) {
			
				if(stone[i][k] == 0) continue; // ��ĭ�� ���õǾ��ٸ� ����� ��ġ�� ����.
				if(stone[i][k] != nowWho) continue; // ���� ���� �� �л���� ���� ���� ó������


				if(stone[i][k] != 0) {
				
					findStone(stone[i][k], i, k, 0 ,locationInfo); // �ش� ��ġ�� ������ �����鼭 ����� ������ ��ġ�� �迭�� ��������.
				}

				/*
				for(tmp=0; tmp<25; tmp++) 
					printf("[%d]\n", locationInfo[tmp]);
				*/
			
				isHouse = findHouse(locationInfo); // �װ��� ������ �Ǵ�
				//printf("********************* %d (1���߰� 0 �����߰�) ***************\n", isHouse);

				printf("\n\n");

				if(isHouse == 1) { // ���ΰ��

					for(del=0; del<25; del++) { // �ش� ����Ʈ�� �ִ� ������ ����
					
						if(locationInfo[del] == -1) continue;

						if(stone[locationInfo[del]/10][locationInfo[del]%10] == 1)
							comGet++;
						if(stone[locationInfo[del]/10][locationInfo[del]%10] == 2)
							userGet++;

						stone[locationInfo[del]/10][locationInfo[del]%10] = 0;
						save[saveCnt++] = 1000 + (locationInfo[del]/10)*100 + (locationInfo[del]%10)*10 + 0; 



					}

				}
			


				for(tmp=0; tmp<25; tmp++) // ���� �۾��� ���� �ʱ�ȭ
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

	// stoneNum : ����(1 or 2)
	// x, y : ��ǥ
	// notGo : ��� �̿������Ƿ� �������� �ǵ��� ���� �ʵ��� �ϴ� ����.
	//    4 
	//  1   3  ���� �����Ͽ� ó����
	//    2
	// *location : �������� �����ϴ� ����

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

		if(location[i] == -1)  // �迭�� ���̸� ������ ����
			continue;

		tmpX = location[i]/10;
		tmpY = location[i]%10;
		myType = stone[tmpX][tmpY];
		if(myType == 1) yourType = 2;
		if(myType == 2) yourType = 1;
			
		if(stone[tmpX-1][tmpY] == 0 && tmpX-1 >= 0) // 4������ 0(=��ĭ)�� �����Ѵٸ� �װ��� ���� �ƴϴ�.
			return 0;
		if(stone[tmpX+1][tmpY] == 0 && tmpX+1 < 5)
			return 0;
		if(stone[tmpX][tmpY-1] == 0 && tmpY-1 >= 0)
			return 0;
		if(stone[tmpX][tmpY+1] == 0 && tmpY+1 < 5)
			return 0;
		
	}
	return 1; // ���ΰ��� 1�� ����

}

void paint() {
	int mapWidth = 5;
	int mapHeight = 5;
	int i=0, j=0,k=0;

	system("cls");
	printf("���Ḧ ���Ͻø� 6 6�� �Է��ϼ���.\n");
	printf("   [SCORE] COM : %d | USR : %d \n", comGet, userGet);
	printf("   X��0   1   2   3   4\n");
	for(i=0; i<mapWidth*2+2; i++) {
		if(i==0) printf(" Y��");
		if(i==mapWidth*2+1) {
			printf("��");
			break;
		}
		if(i%2 ==1) printf("��");
		else{
			printf("��");
		}

	}
	printf("\n"); printf("�馢");
	for(k=0; k<mapWidth*2+3; k++) {
	if(k%2 == 1)
		printf("��");
	else
		printf("  ");
	}

	printf("\n");
	for(i=0; i<mapHeight; i++) {
		printf(" %d��", i);
		for(j=0; j<mapWidth; j++) {
			printf("��");

			if(stone[j][i] == 0) {
				printf("��");
			} else if(stone[j][i] == 1) {
				printf("��");
				//printf("%c", stone[j][i]);
			} else {
				printf("��");
			}
		}
		printf("����\n");

		printf("  ��");
		for(k=0; k<mapWidth*2+3; k++) {
			if(k%2 == 1)
				printf("��");
			else
				printf("  ");
		}
		printf("\n");

	}
	for(i=0; i<mapWidth*2+2; i++) {
		if(i==0) printf("  ��");
		if(i==mapWidth*2+1) {
			printf("��");
			break;
		}
		if(i%2 ==1) printf("��");
		else{
			printf("��");
		}
	}
	printf("\n");
}

void robot() {
	int lastX = (save[saveCnt-1] / 100) %10; // 1052 : x, y, type(0:��ĭ,1:��,2:��) ���� 1000�� ����
	int lastY = (save[saveCnt-1] / 10) % 10;

	int lastX_2nd = -1;
	int lastY_2nd = -1;
	int farChecked = 0;
	int i=0,j=0;
	int blackCnt=0;
	int maxBlackInfo = 100; 
	// �� 125 : 2��ġ���� �������� 5�� �־��ٴ� �ǹ�. 
	//    9ĭ�� ���簢�� �ٵ��� ��������, ���� �������� 1��°�ڸ��� �������� �ݽð�������� ���鼭 ���ڰ� Ŀ��. �ִ�ġ 8
	long seed = (long)time(NULL);
	int ranX=-1, ranY=-1;
	int w;

	nowWho = COM;
	Sleep(400);

	if(saveCnt-3>=0) {
		lastY_2nd = (save[saveCnt-3] / 10) % 10;
		lastX_2nd = (save[saveCnt-3] / 100) %10;
	}

	if(saveCnt != 1) { // �� ���������ٰ� �� ����� ���� ����
		if(isFar(lastX, lastY, lastX_2nd, lastY_2nd)) {
			lastX = lastX_2nd;
			lastY = lastY_2nd;
			farChecked = 1;
		}
	}

	if(saveCnt == 1 || farChecked == 1) { // �� ó���̰ų� ����� ������������ �� ����� ó��
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

	if(saveCnt != 1 && farChecked == 0) { // ������ �� ���� ����� ���� �� ����� ó�� 
		// �ֱ��� ��ǻ�Ͱ� �� �� ��������, ����� �� ���� ���� �� �����ٰ� ���� �ε��� �Ѵ�. 
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
			// �ش絹 ���ʺ��� �����Ͽ� �Ʒ������� �ѹ��� ���鼭 üũ��

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

	// �̵����� �ƴ� ���, ������, ��ĭ��, ���ο� ��ġ�� ���� �д�.
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

	if(lastX-1 != lastX_2nd || lastX+1 != lastX_2nd || lastY-1 != lastY_2nd || lastY+1 != lastY_2nd	) { // ����� �� ���� �����ϰ� ���� �ʾҴٸ�
		return 1; // �ָ� ��
	}
	else 
		return 0; // ������ ��


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
	printf("1. �ٵ� �α�\n");
	printf("2. ���� �ϱ�\n");
	printf("0. ��     ��\n\n");
}