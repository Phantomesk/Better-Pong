#include "AIE.h"
#include <iostream>

const int iScreenWidth = 900;	// screen size
const int iScreenHeight = 600;

char playerScore1[10];
char playerScore2[10];
char player2total[10];
char player1total[10];

int score1 = 0;
int score2 = 0;
int total1 = 0;
int total2 = 0;

enum GAMESTATES
{
	MAIN_MENU,
	GAMEPLAY,
	TOTALWINS,
	GAMEOVER1,	//the different gamestates of my game
	GAMEOVER2,
	EXIT_GAME
};

struct Collision
{
	float xMin;
	float yMin;
	float xMax;		//collision structure
	float yMax;
};

struct Paddle1
{
	unsigned int iPaddle1ID;
	float x;
	float y;
	Collision box;
	float ySpeed = 250.f;
	float width1;
	float height1;
	unsigned int iMoveUpKey;
	unsigned int iMoveDownKey;
	
	void SetSize(float a_width1, float a_height1)
	{
		width1 = a_width1;
		height1 = a_height1;
	}

	void SetPosition(float a_x, float a_y)
	{
		x = a_x;
		y = a_y;
	}

	void SetMovementKeys(unsigned int a_moveUp, unsigned int a_moveDown)
	{
		iMoveUpKey = a_moveUp;
		iMoveDownKey = a_moveDown;
	}
	
	void Movement(float a_fDeltaTime)
	{
		if (IsKeyDown(iMoveUpKey))
		{
			y += ySpeed * a_fDeltaTime;;
			if (y > iScreenHeight - 105)
			{
				y = iScreenHeight - 105;
			}
		}
		if (IsKeyDown(iMoveDownKey))
		{
			y -= ySpeed * a_fDeltaTime;
			if (y < height1-50)
			{
				y = height1-50;
			}
		}
		box.xMin = x - (width1 * .5f);
		box.yMin = y - (height1 * .5f);
		box.xMax = x + (width1 * .5f);
		box.yMax = y + (height1 * .5f);
		
		MoveSprite(iPaddle1ID, x, y);
	}
};
Paddle1 paddle1, paddle2;

struct Block
{
	unsigned int Blocker;
	int x = iScreenWidth*.5f;
	int y = iScreenHeight - 45;	//structure for block between score and ball
	int width = iScreenWidth;
	int height = 20.f;
};
Block wall;

struct PongBall
{
	unsigned int iBallID;
	float x;
	float y;
	Collision box;
	float xSpeed = .2f;
	float ySpeed = .2f;
	float ballWidth;
	float ballHeight;

	void SetSize(float a_ballWidth, float a_ballHeight)
	{
		ballHeight = a_ballWidth;
		ballWidth = a_ballHeight;
	}

	void SetPosition(float a_x, float a_y)
	{
		x = a_x;
		y = a_y;
	}

	void Movement()
	{

		if (x < 0)
		{
			score2 += 1;
			x = iScreenWidth * .5f;
			xSpeed = .1f;
			ySpeed = .1f;
			xSpeed *= 1;
		}
		if (x > iScreenWidth)
		{
			score1 += 1;
			x = iScreenWidth * .5f;
			xSpeed = .1f;
			ySpeed = .1f;
			xSpeed *= -1;
		}
		if (y > (wall.y - 20.f)) //upper collision
		{
			y = (wall.y - 20.f);
			ySpeed *= -1;
		}
		if (y < (ballHeight - 14.f)) //lower collision
		{
			y = (ballHeight - 14.f);
			ySpeed *= -1;
		}

		x = (xSpeed + x);
		y = (ySpeed + y);

		box.xMin = x - (ballWidth * .5f);
		box.yMin = y - (ballHeight * .5f);
		box.xMax = x + (ballWidth * .5f);
		box.yMax = y + (ballHeight * .5f);
	}
};
PongBall ball;

GAMESTATES eCurrentState = MAIN_MENU;

void UpdateMainMenu();
void UpdateGamePlay();
void UpdateTotalWins();
void UpdateGameOverScreen1();
void UpdateGameOverScreen2();

bool CollisionCheck(Collision box1, Collision box2)
{
	if (box1.xMin > box2.xMin && box1.yMin > box2.yMin && box1.xMin < box2.xMax && box1.yMin < box2.yMax)
	{
		return true;
	}
	if (box1.xMax > box2.xMin && box1.yMax > box2.yMin && box1.xMax < box2.xMax && box1.yMax < box2.yMax)
	{
		return true;
	}
	if (box1.xMin > box2.xMin && box1.yMax > box2.yMin && box1.xMin < box2.xMax && box1.yMax < box2.yMax)
	{
		return true;
	}
	if (box1.xMax > box2.xMin && box1.yMin > box2.yMin && box1.xMax < box2.xMax && box1.yMin < box2.yMax)
	{
		return true;
	}
	return false;
}

int main( int argc, char* argv[] )
{

    Initialise(iScreenWidth, iScreenHeight, false, "Ping");
    
    SetBackgroundColour(SColour(0, 0, 0, 255));

	paddle1.SetSize(30.f, 100.f);
	paddle2.SetSize(30.f, 100.f);
	ball.SetSize(28.f, 28.f);

	paddle1.SetPosition(paddle1.width1, iScreenHeight*.5f);
	paddle2.SetPosition(iScreenWidth - paddle2.width1, iScreenHeight*.5f);
	ball.SetPosition(iScreenWidth*.5f, iScreenHeight*.5f);

	paddle1.SetMovementKeys('W', 'S');
	paddle2.SetMovementKeys(GLFW_KEY_UP, GLFW_KEY_DOWN);
	
	paddle1.iPaddle1ID = CreateSprite("./images/Paddle.jpg", paddle1.width1, paddle1.height1, true);
	paddle2.iPaddle1ID = CreateSprite("./images/Paddle.jpg", paddle2.width1, paddle2.height1, true);
	ball.iBallID = CreateSprite("./images/Ball.jpg", ball.ballWidth, ball.ballHeight, true);
	wall.Blocker = CreateSprite("./images/Wall.png", wall.width, wall.height, true);

    //Game Loop
    do
    {
		switch (eCurrentState)
		{
		case MAIN_MENU:
			UpdateMainMenu();
			break;
			
		case GAMEPLAY:
			UpdateGamePlay();
			break;

		case TOTALWINS:
			UpdateTotalWins();
			break;

		case GAMEOVER1:
			UpdateGameOverScreen1();
			break;

		case GAMEOVER2:
			UpdateGameOverScreen2();
			break;

		case EXIT_GAME:
			return 0;

		default:
			break;
		}
    }
	
	while(!FrameworkUpdate());

    Shutdown();

    return 0;
}

void UpdateMainMenu()
{
	DrawString("Welcome to Ping", iScreenWidth *.35f, iScreenHeight *.7f);
	DrawString("Press Enter to start.", iScreenWidth*.35f, iScreenHeight*.6f);
	DrawString("Press W for Wins", iScreenWidth * .35f, iScreenHeight * .5f);
	DrawString("Press Esc to exit.", iScreenWidth*.35f, iScreenHeight*.4f);
	if (IsKeyDown(GLFW_KEY_ENTER))
	{
		eCurrentState = GAMEPLAY;
		ClearScreen();
	}
	if (IsKeyDown('W'))
	{
		eCurrentState = TOTALWINS;
		ClearScreen();
	}
	if (IsKeyDown(GLFW_KEY_ESCAPE))
	{
		eCurrentState = EXIT_GAME;
	}
	
	ClearScreen();
}

void UpdateGamePlay()
{
	float fDeltaTime = GetDeltaTime();
	
	paddle1.Movement(fDeltaTime);
	paddle2.Movement(fDeltaTime);
	ball.Movement();

	if (CollisionCheck(ball.box, paddle1.box) && ball.xSpeed < 0)
	{
		ball.xSpeed *= -1.25;
		ball.ySpeed *= 1.25;
	}
	if (CollisionCheck(ball.box, paddle2.box) && ball.xSpeed > 0)
	{
		ball.xSpeed *= -1.25;
		ball.ySpeed *= 1.25;
	}

	MoveSprite(ball.iBallID, ball.x, ball.y);
	MoveSprite(wall.Blocker, wall.x, wall.y);

	DrawSprite(paddle1.iPaddle1ID);
	DrawSprite(paddle2.iPaddle1ID);
	DrawSprite(ball.iBallID);
	DrawSprite(wall.Blocker);

	DrawString(playerScore1, iScreenWidth*.1f, iScreenHeight - 2);
	DrawString(playerScore2, iScreenWidth*.9f, iScreenHeight - 2);
	DrawString("Press E to pause game at Main Menu", iScreenWidth*.25f, iScreenHeight - 2);

	if (IsKeyDown('E'))
	{
		eCurrentState = MAIN_MENU;
		ClearScreen();
	}
		
	itoa(score1, playerScore1, 10);
	itoa(score2, playerScore2, 10);

	if (score1 == 10)
	{
		total1 += 1;
		eCurrentState = GAMEOVER1;
	}
	if (score2 == 10)
	{
		total2 += 1;
		eCurrentState = GAMEOVER2;
	}

	itoa(total1, player1total, 10);
	itoa(total2, player2total, 10);

    ClearScreen();
}

void UpdateTotalWins()
{
	DrawString("Player 1 :", iScreenWidth*.35f, iScreenHeight*.55f);
	DrawString("Player 2 :", iScreenWidth *.35f, iScreenHeight *.45f);
	DrawString(player1total, iScreenWidth*.5f, iScreenHeight*.55f);
	DrawString(player2total, iScreenWidth *.5f, iScreenHeight *.45f);
	DrawString("Press E to go to Main Menu", iScreenWidth*.025, iScreenHeight - 2);
	DrawString("Press Enter to start game", iScreenWidth* .6f, iScreenHeight - 2);

	if (IsKeyDown('E'))
	{
		eCurrentState = MAIN_MENU;
		ClearScreen();
	}
	if (IsKeyDown(GLFW_KEY_ENTER))
	{
		score1 = 0;
		score2 = 0;

		eCurrentState = GAMEPLAY;

		ClearScreen();
	}
	
	ClearScreen();
}

void UpdateGameOverScreen1()
{
	DrawString("Player 1 has one", iScreenWidth*.35f, iScreenHeight * .7f);
	DrawString("Press Enter to start", iScreenWidth*.35f, iScreenHeight*.6f);
	DrawString("Press E to go to Main Menu", iScreenWidth*.35f, iScreenHeight*.5f);
	DrawString("Press W for Wins", iScreenWidth * .35f, iScreenHeight * .4f);

	if (IsKeyDown(GLFW_KEY_ENTER))
	{
		eCurrentState = GAMEPLAY;
	}
	if (IsKeyDown('E'))
	{
		eCurrentState = MAIN_MENU;
	}
	if (IsKeyDown('W'))
	{
		eCurrentState = TOTALWINS;
	}

	ClearScreen();
}

void UpdateGameOverScreen2()
{
	DrawString("Player 2 has one", iScreenWidth*.35f, iScreenHeight * .7f);
	DrawString("Press Enter to start new game", iScreenWidth*.35f, iScreenHeight*.6f);
	DrawString("Press E to go to Main Menu", iScreenWidth*.35f, iScreenHeight*.5f);
	DrawString("Press W for Wins", iScreenWidth * .35f, iScreenHeight * .4f);

	if (IsKeyDown(GLFW_KEY_ENTER))
	{
		eCurrentState = GAMEPLAY;
	}
	if (IsKeyDown('E'))
	{
		eCurrentState = MAIN_MENU;
	}
	if (IsKeyDown('H'))
	{
		eCurrentState = TOTALWINS;
	}

	ClearScreen();
}
