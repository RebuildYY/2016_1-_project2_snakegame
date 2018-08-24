#include<stdio.h>
#include<stdlib.h>
#include<time.h>
#include<signal.h>
#include<termios.h>
#include<sys/time.h>
#include<unistd.h>
char c[100][100];
int snake[100][2];
int cx, cy, time_out, game_over, score = 0, s = -1, q = -1;
void makeFood(int n, int m, int *x, int *y) {
	int a, b;
	srand(time(NULL));
	while (1) {
		a = rand();
		(*x) = ((a%n) + 1);
		b = rand();
		(*y) = ((b%m) + 1);
		if (c[*x][*y] == '0') {
			c[*x][*y] = 'F';
			break;
		}
	}
}
void init(int n, int m) {
	cx = (n / 2) + 1;
	cy = (m / 2) + 1;
	game_over = 0;
	time_out = 0;
	int  x, y;
	c[cx][cy] = '@';
	makeFood(n, m, &x, &y);
}
void drawBoard(int n, int m)
{
	system("clear");
	puts("");
	printf(" Snake Game |Score : %d|\n", score);
	puts("");
	int  i = 1, j = 1;
	for (i = 1; i <= n; i++) {
		for (j = 1; j <= m + 1; j++) {
			if (c[i][j] == '1') { printf("* "); }
			else if (c[i][j] == '0') { printf("  "); }
			else if (c[i][j] == '@') { printf("@ "); }
			else if (c[i][j] == 'F') { printf("F "); }
			else { printf("\n"); }
		}
	}
	printf("\n");
}
int temp_do_count = 0;
void Do(int sig) {
	//      system("clear");
	//      printf("Alarm %d!\n", ++temp_do_count);
	int  i;
	snake[0][0] = cx;
	snake[0][1] = cy;
	c[snake[score][0]][snake[score][1]] = '0';
	for (i = score; i>0; i--) {
		snake[i][0] = snake[i - 1][0];
		snake[i][1] = snake[i - 1][1];
	}
	if (sig == 0) {
		if (q == 2) {
			sig = 2;
			s = 2;
		}
	}
	else if (sig == 1) {
		if (q == 3) {
			sig = 3;
			s = 3;
		}
	}
	else if (sig == 2) {
		if (q == 0) {
			sig = 0;
			s = 0;
		}
	}
	else if (sig == 3) {
		if (q == 1) {
			sig = 1;
			s = 1;
		}
	}
	switch (sig) {
	case 0: (cy++);
		break;
	case 1: (cx--);
		break;
	case 2: (cy--);
		break;
	case 3: (cx++);
		break;
	}
	snake[0][0] = cx;
	snake[0][1] = cy;
	if (c[snake[0][0]][snake[0][1]] == 'F') {
		score++;
		c[snake[0][0]][snake[0][1]] = '@';
		c[snake[score][0]][snake[score][1]] = '@';
	}
	else if (c[cx][cy] == '1' || c[cx][cy] == '@') {
		game_over = 1;
	}
	else {
		c[snake[0][0]][snake[0][1]] = '@';
	}
}

int getch(void) {
	char ch;
	int error;
	static struct termios Otty, Ntty;
	fflush(stdout);
	tcgetattr(0, &Otty);
	Ntty = Otty;
	Ntty.c_iflag = 0;
	Ntty.c_oflag = 0;
	Ntty.c_lflag &= ~ICANON;
#if 1
	Ntty.c_lflag &= ~ECHO;
#else
	Ntty, c_lflag |= ECHO;
#endif
	Ntty.c_cc[VMIN] = 0;
	Ntty.c_cc[VTIME] = 1;
#if 1
#define FLAG TCSAFLUSH
#else
#define FLAG TCSANOW
#endif
	if (0 == (error = tcsetattr(0, FLAG, &Ntty)))
	{
		error = read(0, &ch, 1);
		error += tcsetattr(0, FLAG, &Otty);
	}
	return (error == 1 ? (int)ch : -1);
}
int GetCommand() {
	int ch = getch();
	switch (ch) {
	case 'd':
	case 'D': return 0;
	case 'w':
	case 'W': return 1;
	case 'a':
	case 'A': return 2;
	case 's':
	case 'S': return 3;
	default: return -1;
	}
	return -1;
}
int main(void) {
	int n, m, command, x, y, n1, n2, i = 1, j = 1;
	FILE *fp;
	fp = fopen("board.txt", "r");
	fscanf(fp, "%d %d", &n, &m);
	while (i <= n) {
		j = 1;
		while (j <= m + 1) {
			c[i][j] = fgetc(fp);
			j++;
		}
		i++;
	}
	fclose(fp);
	init(n, m);
	srand(time(NULL));
	static struct sigaction act, oact;
	act.sa_handler = Do;
	sigaction(SIGALRM, &act, NULL);
	do {
		if (time_out == 0) {
			ualarm(300000, 0);
			time_out = 1;
		}
		command = GetCommand();
		if (command >= 0) {
			s = command;
		}
		n1 = score;
		Do(s);
		n2 = score;
		if ((n2)-(n1) == 1) {
			makeFood(n, m, &x, &y);
		}
		drawBoard(n, m);
		q = s;
	} while (!game_over);
	system("clear");
	printf("game over!\n");
}

