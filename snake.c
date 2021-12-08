#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include <conio.h>
#include <stdlib.h>
#include <time.h>
#include<stdbool.h>

const int ROWS = 25;
const int COLS = 75;
const int START_X = 12;
const int START_Y = 37;
const double INCREAS_SPEED = 0.03;
const double FASTEST_SPEED = 0.1;

/* NOTICE
The head of the snake is the tail of the linked list
and the tail of the snake is the head of the linked-list
*/

/*
The program implements the game snake.
the user needs to press the direction change keys('d' ,'a', 's', 'w') if he wants the snake to move in another direction.
The snake increases by 1 (@) and its speed increases every 5 moves.
The game is over if the snake touches one of the walls or itself.
*/

typedef struct node
{
	int x;
	int y;
	struct node* next;
} NODE;

typedef struct snake
{
	NODE* head;
	NODE* tail;
	int size;
} SNAKE;



// Turn escape codes ON.
void init(void);

// Moves the cursor to position (x,y) on screen.
// Parameters:
//     x: the row of the posiiton.
//     y: the column of the posiiton.
void gotoxy(int x, int y);

// Delays the execution of the program.
// Parameters:
//     secs: the length of the delay in seconds. 
void sleep(float secs);


///////// **** MY FUNCTIONS **** /////////////

//The function gets length and width and prints a hollow frame of #
void drawFrame(int len, int width);

//The function adds a NODE to the head (of the snake)
void addToHead(SNAKE* snake, int newValX, int newValY);

//The function removes the tail (of the snake) from the struct NODE
void removeTail(SNAKE* snake);

//The function calculates the next head Position(x,y) of the snake
void calculateNextHeadPos(SNAKE* snake, char direction, int* x, int* y);

//The function moves and prints the snake (@). (meaning it adds if necessary new head by calling other functions)
//It also checks if the future move of the head position is vallid. 
void moveAndPrintSnake(SNAKE* snake, char direction, bool shouldExpan, bool* isValidMove);

//The function creates the first NODE of the snake by getting the wanted start place on board game
void initSnake(SNAKE* snake, int firstX, int firstY);

//The function gets current direction and returns the new direction
//It reads the key that the player hit. 
//if hit 'd'-goes left
//if hit 'a'-goes right
//if hit 'w'-goes up
//if hit 's'-goes down
char getDirection(char currentDirec);

//The function checks if the snake touched one of the walls
//if so returns true
bool snakeTouchWall(SNAKE snake);

//The function checks if the snake touched itself
//if so returns true
bool snakeTouchSnake(SNAKE snake);

//The function claculats snakes speed
void snakeSpeed(double* currentSpeed, bool shouldSpeed);

//free the NODE list
void freeNodeList(NODE* head);


int main()
{
	init();
	drawFrame(ROWS, COLS);
	bool continuePlaying = true;
	bool didTouchWall = false;
	bool didTouchSnake;
	bool shouldExpan = false;
	bool shouldSpeed = false;

	double currentSpeed = 1;
	int countSnakeStep = 0;
	char snakeDirec = 'R';

	// init initial state
	SNAKE snake;
	gotoxy(START_X, START_Y);
	initSnake(&snake, START_X, START_Y);
	moveAndPrintSnake(&snake, snakeDirec, shouldExpan, &continuePlaying);

	while (continuePlaying)
	{
		sleep(currentSpeed);

		snakeDirec = getDirection(snakeDirec);

		countSnakeStep++;
		if (countSnakeStep % 5 == 0)
		{
			shouldExpan = true;
			shouldSpeed = true;
			snake.size++;
		}

		moveAndPrintSnake(&snake, snakeDirec, shouldExpan ,&continuePlaying);
		
		didTouchSnake = snakeTouchSnake(snake);
		didTouchWall = snakeTouchWall(snake);

		snakeSpeed(&currentSpeed, shouldSpeed);
		shouldExpan = false;
		shouldSpeed = false;

		if ((didTouchSnake) || (didTouchWall))
		{
			continuePlaying = false;
		}
		if (!continuePlaying)
		{
			init();
			drawFrame(ROWS, COLS);
			freeNodeList((snake.tail));
			gotoxy(START_X, START_Y);
			printf("GAME OVER");
		}
		gotoxy(26, 1);
	}
}

void freeNodeList(NODE* head)
{
	NODE* tmp;

	while (head != NULL)
	{
		tmp = head;
		head = head->next;
		free(tmp);
	}
}

void snakeSpeed(double* currentSpeed, bool shouldSpeed)
{
	if ((shouldSpeed)&&(*currentSpeed> FASTEST_SPEED))
	{
		*currentSpeed = *currentSpeed - INCREAS_SPEED;
	}
}

bool snakeTouchSnake(SNAKE snake)
{
	int i;
	for (i = 1; i < snake.size; i++, snake.tail = snake.tail->next)
	{
		if ((snake.tail != snake.head) && (snake.tail->x == snake.head->x) && (snake.tail->y == snake.head->y))
		{
			return true;
		}
	}
	return false;
}

bool snakeTouchWall(SNAKE snake)
{
	if (snake.head->y == COLS)
	{
		return true; // GAME OVER
	}
	if (snake.head->y == 1)
	{
		return true; // GAME OVER
	}
	if (snake.head->x == ROWS)
	{
		return true; // GAME OVER
	}
	if (snake.head->x == 1)
	{
		return true; // GAME OVER
	}

	return false; // did not touch any wall
}

char getDirection(char currentDirec)
{
	char snakeDirec;
	if (_kbhit())
	{
			snakeDirec = _getch();

			switch (snakeDirec)
			{
			case 'd':
				snakeDirec = 'R';
				break;
			case 'a':
				snakeDirec = 'L';
				break;
			case 'w':
				snakeDirec = 'U';
				break;
			case 's':
				snakeDirec = 'D';
				break;
			default:
				return(currentDirec);
				break;
			}
			return(snakeDirec);
	}	
	else
	{
		return(currentDirec);
	}
}


void initSnake(SNAKE* snake, int firstX, int firstY)
{
	snake->head = NULL;
	snake->tail = NULL;
	addToHead(snake, firstX, firstY);
	snake->tail = snake->head;
	snake->size = 1;
}


void moveAndPrintSnake(SNAKE* snake, char direction, bool shouldExpan, bool* isValidMove)
{
	int x;
	int y;

	calculateNextHeadPos(snake, direction, &x, &y);
	if ((snake->size == 2)&&(snake->tail->x==x)&&(snake->tail->y==y))
	{
		*isValidMove = false;
		return;
	}

	if ((shouldExpan)&&(snake->tail->x == x) && (snake->tail->y == y))
	{
		*isValidMove = false;
		return;
	}

	else if (!shouldExpan)
	{
		gotoxy(snake->tail->x, snake->tail->y);
		printf(" ");
		removeTail(snake);
	}
	addToHead(snake, x, y);
	gotoxy(snake->head->x, snake->head->y);
	printf("@");
}

void calculateNextHeadPos(SNAKE* snake, char direction, int* x, int* y)
{
	if (direction == 'R')
	{
		*x = snake->head->x;
		*y = snake->head->y + 1;
	}
	if (direction == 'L')
	{
		*x = snake->head->x;
		*y = snake->head->y - 1;
	}
	if (direction == 'U')
	{
		*x = snake->head->x - 1;
		*y = snake->head->y;
	}
	if (direction == 'D')
	{
		*x = snake->head->x + 1;
		*y = snake->head->y;
	}
}

void addToHead(SNAKE* snake, int newValX, int newValY)
{
	NODE * tmp;
	tmp = (NODE*)malloc(sizeof(NODE));
	tmp->x = newValX;
	tmp->y = newValY;
	tmp->next = NULL;
	if (snake->head == NULL)
	{
		snake->head = tmp;
		snake->tail = tmp;
	}
	else
	{
		snake->head->next = tmp;
		snake->head = tmp;
	}
}

void removeTail(SNAKE* snake)
{
	if (snake->tail == NULL)
	{
		return;
	}
	NODE* tmp = snake->tail->next;
	free(snake->tail);
	snake->tail = tmp;

	if (tmp == NULL)
	{
		snake->head = NULL;
	}
}

void drawFrame(int len, int width)
{
	int i, j, k;

	for (i = 0; i < width; i++) //first row
	{
		printf("#");
	}
	printf("\n");

	for (j = 0; j < len - 2; j++)
	{
		printf("#");
		for (k = 0; k < width - 2; k++)
		{
			printf(" ");
		}
		printf("#\n");
	}

	for (i = 0; i < width; i++) //last row
	{
		printf("#");
	}

}

// Turn escape codes ON.
void init(void) {
	system("cls");
}

// Moves the cursor to position (x,y) on screen.
// Parameters:
//     x: the row of the posiiton.
//     y: the column of the posiiton.
void gotoxy(int x, int y) {
	printf("\x1b[%d;%df", x, y);
}

// Delays the execution of the program.
// Parameters:
//     secs: the length of the delay in seconds. 
void sleep(float secs) {
	clock_t clocks_start = clock();

	while (clock() - clocks_start < secs*CLOCKS_PER_SEC)
		;
}
