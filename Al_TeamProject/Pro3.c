#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <windows.h>

#define USER 1
#define AI 2
#define ROOM 3
#define NOROOM 4
#define FAR 5
#define NOFAR 6

char g_stone[5][5] = { 0 };
int g_save[128] = { 0 };
int g_AIPoint = 0;
int g_USERPoint = 0;
int g_recentPlayer = 0;
int g_stonPos = 0;
int g_saveNum = 0;

void display();
void addStone();
void updateStone();
void makePlatform();
void aiPlayer();
int stonePos();
int findRoom();

// ���� ============================================================================
// ===============================================================================
void main() {
	int i = 0, k = 0;

	for (i = 0; i<5; i++) {
		for (k = 0; k<5; k++) {
			g_stone[i][k] = 0;
		}
	}

	while (1) {
		makePlatform();
		addStone();
		updateStone();
	
		makePlatform();
		aiPlayer();
		updateStone();

		makePlatform();
	}
}


void display(char *p_message) {
	fputs("===================================\n", stderr);
	fputs(p_message, stderr);
	fputc('\n', stderr);
	fputs("===================================\n", stderr);
}


void addStone() {
	int x = -1;
	int y = -1;
	g_recentPlayer = USER;

	while (1) {
		printf(" X��ǥ�� �Է��ϼ��� : ");
		scanf("%d", &x);
		printf(" Y��ǥ�� �Է��ϼ��� : ");
		scanf("%d", &y);
		y = 4 - y;

		if (x == 6 && y == 6) {
			display("����ڰ� ���Ḧ �����߽��ϴ�.");
			exit(1);
		}

		if (g_stone[x][y] == 0) {
			if (x >= 0 && x<5 && y >= 0 && y<5) {
				g_stone[x][y] = 1;
				g_save[g_saveNum++] = 1000 + x * 100 + y * 10 + 1;
				break;
			}
		}
		else continue;
	}
}


void updateStone() {
	int com = AI;
	int user = USER;
	int i = 0, j = 0, k = 0, round, del, data;
	int roomData[25] = { -1 };

	for (round = 1; round <= 2; round++) {  
		for (i = 0; i<5; i++) {
			for (k = 0; k<5; k++) {
				if (g_stone[i][k] == 0) continue; 
				if (g_stone[i][k] != g_recentPlayer) continue; 
				if (g_stone[i][k] != 0) stonePos(g_stone[i][k], i, k, roomData, 0); 

				//isRoom = findRoom(roomData); // �װ��� ������ �Ǵ�
				if (findRoom(roomData) == ROOM) { // ���ΰ��
					for (del = 0; del<25; del++) { // �ش� ����Ʈ�� �ִ� ������ ����
						if (roomData[del] == -1) continue;
						data = g_stone[roomData[del] / 10][roomData[del] % 10];
						switch (data) {
						case 1:
							g_AIPoint++; break;
						case 2:
							g_USERPoint++; break;
						}
						g_stone[roomData[del] / 10][roomData[del] % 10] = 0;
						g_save[g_saveNum++] = 1000 + (roomData[del] / 10) * 100 + (roomData[del] % 10) * 10 + 0;
					}
				}
				for (j = 0; j<25; j++) roomData[j] = -1;
				g_stonPos = 0;
			}
		}
		g_recentPlayer = (g_recentPlayer == user) ? com : user;
	}
}


void makePlatform() {
	int i = 0, j = 0, k = 0;
	int platformWidth = 10;
	int platformHeight = 10;

	system("cls");
	fprintf(stderr, "\t    <����>\n\t��� : %d AI : %d", g_USERPoint, g_AIPoint);
	fputs("\n===================================\n\n", stderr);

	printf(" X �� 0   1   2   3   4\n");
	for (i = 0; i<platformWidth + 2; i++) {
		if (i == 0) printf(" Y��");
		if (i == platformWidth + 1) {
			printf("��");
			break;
		}
		if (i % 2 == 1) printf("��");
		else printf("��");
	}
	printf("\n"); printf("�馢");

	for (k = 0; k<platformWidth + 3; k++) {
		if (k % 2 == 1) printf("��");
		else printf("  ");
	}
	printf("\n");

	for (i = 0; i<platformHeight / 2; i++) {
	//for (i = platformHeight / 2-1; i>=0; i--) {
		printf(" %d��", 4 - i);
		for (j = 0; j<platformHeight / 2; j++) {
			printf("��");

			if (g_stone[j][i] == 0) printf("��");
			else if (g_stone[j][i] == 1) printf("��");
			else printf("��");
		}
		printf("����\n");

		printf("  ��");
		for (k = 0; k<platformWidth + 3; k++) {
			if (k % 2 == 1) printf("��");
			else printf("  ");
		}
		printf("\n");
	}

	for (i = 0; i<platformWidth + 2; i++) {
		if (i == 0) printf("  ��");
		if (i == platformWidth + 1) {
			printf("��");
			break;
		}
		if (i % 2 == 1) printf("��");
		else printf("��");
	}
	printf("\n");
	display("\t  <������>\n     (X : 9 Y :9)�� �Է� �ϼ���");
}


int stonePos(int p_stoneNum, int x, int y, int *roomData, int checkRoot) {

	// p_stoneNum : ����(1 or 2)
	// x, y : ��ǥ
	// checkRoot : ��� �̿������Ƿ� �������� �ǵ��� ���� �ʵ��� �ϴ� ����.
	//    4 
	//  1   3  ���� �����Ͽ� ó����
	//    2
	// *location : �������� �����ϴ� ����

	roomData[g_stonPos++] = x * 10 + y;

	if (x - 1 >= 0 && checkRoot != 1) {
		if (g_stone[x - 1][y] == p_stoneNum) stonePos(g_stone[x - 1][y], x - 1, y, roomData, 3);
	}

	if (y + 1 < 5 && checkRoot != 2) {
		if (g_stone[x][y + 1] == p_stoneNum) stonePos(g_stone[x][y + 1], x, y + 1, roomData, 4);
	}

	if (x + 1 < 5 && checkRoot != 3) {
		if (g_stone[x + 1][y] == p_stoneNum) stonePos(g_stone[x + 1][y], x + 1, y, roomData, 1);
	}

	if (y - 1 >= 0 && checkRoot != 4) {
		if (g_stone[x][y - 1] == p_stoneNum) stonePos(g_stone[x][y - 1], x, y - 1, roomData, 2);
	}
	return 0;

}


int findRoom(int *p_roomPos) {
	int i = 0, x = -1, y = -1;
	int playerA, playerB;

	for (i = 0; i<25; i++) {
		if (p_roomPos[i] == -1)  continue;

		x = p_roomPos[i] / 10;
		y = p_roomPos[i] % 10;
		playerA = g_stone[x][y];

		playerB = (playerA == 1) ? 2 : 1;
		//if (myType == 1) yourType = 2;
		//if (myType == 2) yourType = 1;

		// 4������ 0(=��ĭ)�� �����Ѵٸ� �װ��� ���� �ƴϴ�.
		if ((g_stone[x - 1][y] == 0) && (x - 1 >= 0)) return NOROOM;
		if ((g_stone[x + 1][y] == 0) && (x + 1 < 5)) return NOROOM;
		if ((g_stone[x][y - 1] == 0) && (y - 1 >= 0)) return NOROOM;
		if ((g_stone[x][y + 1] == 0) && (y + 1 < 5)) return NOROOM;
	}
	return ROOM;
}

void aiPlayer() {
	int isFar = NOFAR;
	int blackCnt = 0;
	int maxBlackInfo = 100;
	// �� 125 : 2��ġ���� �������� 5�� �־��ٴ� �ǹ�. 
	//    9ĭ�� ���簢�� �ٵ��� ��������, ���� �������� 1��°�ڸ��� �������� �ݽð�������� ���鼭 ���ڰ� Ŀ��. �ִ�ġ 8
	long seed = (long)time(NULL);
	int ranX = -1, ranY = -1;
	int i = 0;

	int recentX = (g_save[g_saveNum - 1] / 100) % 10; // 1052 : x, y, type(0:��ĭ,1:��,2:��) ���� 1000�� ����
	int recentY = (g_save[g_saveNum - 1] / 10) % 10;
	int befroeRecentX = -1, befroeRecentY = -1;

	g_recentPlayer = AI;
	Sleep(2000);
	// 3�� �̻��� ���� ���� ���� ��
	if (g_saveNum - 3 >= 0) {
		befroeRecentX = (g_save[g_saveNum - 3] / 100) % 10;
		befroeRecentY = (g_save[g_saveNum - 3] / 10) % 10;
	}

	if (g_saveNum != 1) { // �� ���������ٰ� �� ����� ���� ����
		if (recentX - 1 != befroeRecentX || recentX + 1 != befroeRecentX || recentY - 1 != befroeRecentY || recentY + 1 != befroeRecentY) { // ����� �� ���� �����ϰ� ���� �ʾҴٸ�
			recentX = befroeRecentX;
			recentY = befroeRecentY;
			isFar = FAR;
		}
	}

	if (g_saveNum == 1 || isFar == FAR) { // �� ó���̰ų� ����� ������������ �� ����� ó��
		if (recentX - 1 >= 0) {
			if (g_stone[recentX - 1][recentY] == 0) {
				g_stone[recentX - 1][recentY] = 2;
				g_save[g_saveNum++] = 1000 + (recentX - 1) * 100 + (recentY)* 10 + 2;
				return;
			}
			else if (g_stone[recentX][recentY - 1] == 0) {
				g_stone[recentX][recentY - 1] = 2;
				g_save[g_saveNum++] = 1000 + (recentX)* 100 + (recentY - 1) * 10 + 2;
				return;
			}
		}
		
		if (recentX + 1 <= 4) {
			if (g_stone[recentX + 1][recentY] == 0) {
				g_stone[recentX + 1][recentY] = 2;
				g_save[g_saveNum++] = 1000 + (recentX + 1) * 100 + (recentY)* 10 + 2;
				return;
			}
			else if (g_stone[recentX][recentY + 1] == 0) {
				g_stone[recentX][recentY + 1] = 2;
				g_save[g_saveNum++] = 1000 + (recentX)* 100 + (recentY + 1) * 10 + 2;
				return;
			}
		}
	}

	if (g_saveNum != 1 && isFar == NOFAR) { // ������ �� ���� ����� ���� �� ����� ó�� 
		// �ֱ��� ��ǻ�Ͱ� �� �� ��������, ����� �� ���� ���� �� �����ٰ� ���� �ε��� �Ѵ�. 
		if (g_saveNum - 2 >= 0) {
			befroeRecentY = (g_save[g_saveNum - 2] / 10) % 10;
			befroeRecentX = (g_save[g_saveNum - 2] / 100) % 10;

			if (g_stone[befroeRecentX - 1][befroeRecentY - 1] == 1 && g_stone[befroeRecentX - 1][befroeRecentY - 1] != 2) {
				blackCnt++;
				if (maxBlackInfo % 10 < blackCnt)
					maxBlackInfo = (maxBlackInfo / 100) * 100 + 10 + blackCnt;
			}
			else blackCnt = 0;

			if (g_stone[befroeRecentX - 1][befroeRecentY] == 1 && g_stone[befroeRecentX - 1][befroeRecentY] != 2) {
				blackCnt++;
				if (maxBlackInfo % 10 < blackCnt)
					maxBlackInfo = (maxBlackInfo / 100) * 100 + 20 + blackCnt;
			}
			else blackCnt = 0;

			if (g_stone[befroeRecentX - 1][befroeRecentY + 1] == 1 && g_stone[befroeRecentX - 1][befroeRecentY + 1] != 2) {
				blackCnt++;
				if (maxBlackInfo % 10 < blackCnt)
					maxBlackInfo = (maxBlackInfo / 100) * 100 + 30 + blackCnt;
			}
			else blackCnt = 0;

			if (g_stone[befroeRecentX][befroeRecentY + 1] == 1 && g_stone[befroeRecentX][befroeRecentY + 1] != 2) {
				blackCnt++;
				if (maxBlackInfo % 10 < blackCnt)
					maxBlackInfo = (maxBlackInfo / 100) * 100 + 40 + blackCnt;
			}
			else blackCnt = 0;

			if (g_stone[befroeRecentX + 1][befroeRecentY + 1] == 1 && g_stone[befroeRecentX + 1][befroeRecentY + 1] != 2) {
				blackCnt++;
				if (maxBlackInfo % 10 < blackCnt)
					maxBlackInfo = (maxBlackInfo / 100) * 100 + 50 + blackCnt;
			}
			else blackCnt = 0;

			if (g_stone[befroeRecentX + 1][befroeRecentY] == 1 && g_stone[befroeRecentX + 1][befroeRecentY] != 2) {
				blackCnt++;
				if (maxBlackInfo % 10 < blackCnt)
					maxBlackInfo = (maxBlackInfo / 100) * 100 + 60 + blackCnt;
			}
			else blackCnt = 0;

			if (g_stone[befroeRecentX + 1][befroeRecentY - 1] == 1 && g_stone[befroeRecentX + 1][befroeRecentY - 1] != 2) {
				blackCnt++;
				if (maxBlackInfo % 10 < blackCnt)
					maxBlackInfo = (maxBlackInfo / 100) * 100 + 70 + blackCnt;
			}
			else blackCnt = 0;

			if (g_stone[befroeRecentX][befroeRecentY - 1] == 1 && g_stone[befroeRecentX][befroeRecentY - 1] != 2) {
				blackCnt++;
				if (maxBlackInfo % 10 < blackCnt)
					maxBlackInfo = (maxBlackInfo / 100) * 100 + 80 + blackCnt;
			}
			else blackCnt = 0;
			// �ش絹 ���ʺ��� �����Ͽ� �Ʒ������� �ѹ��� ���鼭 üũ��

			switch ((maxBlackInfo / 10) % 10) {
			case 0:
				break;
			case 1:
				g_stone[befroeRecentX - 1][befroeRecentY] = 2;
				g_save[g_saveNum++] = 1000 + (befroeRecentX - 1) * 100 + (befroeRecentY)* 10 + 2;
				return;
			case 2:
				g_stone[befroeRecentX - 1][befroeRecentY + 1] = 2;
				g_save[g_saveNum++] = 1000 + (befroeRecentX - 1) * 100 + (befroeRecentY + 1) * 10 + 2;
				return;
			case 3:
				g_stone[befroeRecentX][befroeRecentY + 1] = 2;
				g_save[g_saveNum++] = 1000 + (befroeRecentX)* 100 + (befroeRecentY + 1) * 10 + 2;
				return;
			case 4:
				g_stone[befroeRecentX + 1][befroeRecentY + 1] = 2;
				g_save[g_saveNum++] = 1000 + (befroeRecentX + 1) * 100 + (befroeRecentY + 1) * 10 + 2;
				return;
			case 5:
				g_stone[befroeRecentX + 1][befroeRecentY] = 2;
				g_save[g_saveNum++] = 1000 + (befroeRecentX + 1) * 100 + (befroeRecentY)* 10 + 2;
				return;
			case 6:
				g_stone[befroeRecentX + 1][befroeRecentY - 1] = 2;
				g_save[g_saveNum++] = 1000 + (befroeRecentX + 1) * 100 + (befroeRecentY - 1) * 10 + 2;
				return;
			case 7:
				g_stone[befroeRecentX][befroeRecentY - 1] = 2;
				g_save[g_saveNum++] = 1000 + (befroeRecentX)* 100 + (befroeRecentY - 1) * 10 + 2;
				return;
			case 8:
				g_stone[befroeRecentX - 1][befroeRecentY - 1] = 2;
				g_save[g_saveNum++] = 1000 + (befroeRecentX - 1) * 100 + (befroeRecentY - 1) * 10 + 2;
				return;

			}
		}
	}

	// �̵����� �ƴ� ���, ������, ��ĭ��, ���ο� ��ġ�� ���� �д�.
	srand(seed);

	for (i = 0; i<25; i++) {
		ranX = rand() % 5;
		ranY = rand() % 5;

		if (g_stone[ranX][ranY] == 0) {
			if (g_stone[ranX - 1][ranY] != 1 && g_stone[ranX][ranY - 1] != 1 && g_stone[ranX][ranY + 1] != 1 && g_stone[ranX + 1][ranY] != 1) {
				g_stone[ranX][ranY] = 2;
				g_save[g_saveNum++] = 1000 + (ranX)* 100 + (ranY)* 10 + 2;
				return;
			}
		}
	}

}