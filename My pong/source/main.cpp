#include "AIE.h"
#include <iostream>

const int iScreenWidth = 900;
const int iScreenHeight = 600;

char playerScore1[10] = "0";
char playerScore2[10] = "0";

enum GAMESTATES
{
	MAIN_MENU,
	GAMEPLAY,
	HIGHSCORE,
	GAMEOVER,
	EXIT_GAME
};



struct Paddle1
{
	unsigned int iPaddle1ID;
	float x;
	float y;
	float ySpeed = 250.f;
	float width1;
	float height1;
	float upperBoundry = y + height1*.5f;
	float lowerBoundry = y - height1*.5f;
	float rightBoundry = y + width1*.5f;
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
		MoveSprite(iPaddle1ID, x, y);
	}
};
Paddle1 paddle1;

struct Paddle2
{
	unsigned int iPaddle2ID;
	float x;
	float y;
	float ySpeed = 250.f;
	float width2;
	float height2;
	float upperBoundry = y + height2*.5f;
	float lowerBoundry = y - height2*.5f;
	float leftBoundry = y - width2*.5f;
	unsigned int iMoveUpKey;
	unsigned int iMoveDownKey;
	
	void SetSize(float a_width1, float a_height1)
	{
		width2 = a_width1;
		height2 = a_height1;
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
			if (y < height2 - 50)
			{
				y = height2 - 50;
			}
		}
		MoveSprite(iPaddle2ID, x, y);
	}
};
Paddle2 paddle2;

struct Block
{
	unsigned int Blocker;
	int x = iScreenWidth*.5f;
	int y = iScreenHeight - 45;
	int width = iScreenWidth;
	int height = 20.f;
};
Block wall;

struct PongBall
{
	unsigned int iBallID;
	float x;
	float y;
	float xSpeed = .1f;
	float ySpeed = .1f;
	float ballWidth;
	float ballHeight;
	float rightBoundry = ballWidth*.5f;
	float leftBoundry = ballWidth*.5f;

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
		x = (xSpeed + x);
		y = (ySpeed + y);
		if (y > (wall.y - 20.f))
		{
			y = (wall.y - 20.f);
			ySpeed *= -1;
		}
		if (y < (ballHeight - 15.f))
		{
			y = (ballHeight -15.f);
			ySpeed *= -1;
		}
		if (x > iScreenWidth)
		{
			x = iScreenWidth * .5f;
			xSpeed *= -1;
			/*playerScore1[10] += 1;*/ //wont work yet makes players score go up
		}
		if (x < 0)
		{
			x = iScreenWidth * .5f;
			xSpeed *= -1;
			/*playerScore2[10] += 1;*/ //wont work yet
		}
		/*if (ball.rightBoundry >= paddle2.leftBoundry)
		{
			ball.xSpeed *= -1;
		}															//failed paddle and ball collision.
		if (ball.leftBoundry <= paddle1.rightBoundry)
		{
			ball.xSpeed *= -1;
		}*/
	}
};
PongBall ball;

GAMESTATES eCurrentState = MAIN_MENU;

void UpdateMainMenu();
void UpdateGamePlay();
void UpdateHighScore();
void UpdateGameOverScreen();

int main( int argc, char* argv[] )
{
    Initialise(iScreenWidth, iScreenHeight, false, "Ping");
    
    SetBackgroundColour(SColour(0, 0, 0, 255));

	paddle1.SetSize(30.f, 100.f);
	paddle2.SetSize(30.f, 100.f);
	ball.SetSize(28.f, 28.f);

	paddle1.SetPosition(paddle1.width1, iScreenHeight*.5f);
	paddle2.SetPosition(iScreenWidth - paddle2.width2, iScreenHeight*.5f);
	ball.SetPosition(iScreenWidth*.5f, iScreenHeight*.5f);

	paddle1.SetMovementKeys('W', 'S');
	paddle2.SetMovementKeys(GLFW_KEY_UP, GLFW_KEY_DOWN);
	
	paddle1.iPaddle1ID = CreateSprite("./images/Paddle.jpg", paddle1.width1, paddle1.height1, true);
	paddle2.iPaddle2ID = CreateSprite("./images/Paddle.jpg", paddle2.width2, paddle2.height2, true);
	ball.iBallID = CreateSprite("./images/Ball.jpg", ball.ballWidth, ball.ballHeight, true);
	wall.Blocker = CreateSprite("./images/Wall.png", wall.width, wall.height, true);

	DrawSprite(paddle1.iPaddle1ID);
	DrawSprite(paddle2.iPaddle2ID);
	DrawSprite(ball.iBallID);
	DrawSprite(wall.Blocker);

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

		case HIGHSCORE:
			UpdateHighScore();
			break;

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
	DrawString("Welcome to Pong", iScreenWidth *.37f, iScreenHeight *.6f);
	DrawString("Press Enter to start.", iScreenWidth*.37f, iScreenHeight*.5f);
	DrawString("Press H for High Score", iScreenWidth * .37f, iScreenHeight * .4f);
	if (IsKeyDown(GLFW_KEY_ENTER))
	{
		eCurrentState = GAMEPLAY;
		ClearScreen();
	}
	if (IsKeyDown('H'))
	{
		eCurrentState = HIGHSCORE;
		ClearScreen();
	}
	if (IsKeyDown(GLFW_KEY_ESCAPE))
	{
		eCurrentState = EXIT_GAME;
		Shutdown();
	}
	
	ClearScreen();
}

void UpdateGamePlay()
{
		float fDeltaTime = GetDeltaTime();

		MoveSprite(paddle1.iPaddle1ID, paddle1.x, paddle1.y);
		MoveSprite(paddle2.iPaddle2ID, paddle2.x, paddle2.y);
		MoveSprite(ball.iBallID, ball.x, ball.y);
		MoveSprite(wall.Blocker, wall.x, wall.y);

		DrawSprite(paddle1.iPaddle1ID);
		DrawSprite(paddle2.iPaddle2ID);
		DrawSprite(ball.iBallID);
		DrawSprite(wall.Blocker);

		DrawString(playerScore1, iScreenWidth*.3f, iScreenHeight - 2);
		DrawString(playerScore2, iScreenWidth*.68f, iScreenHeight - 2);

		paddle1.Movement(fDeltaTime);
		paddle2.Movement(fDeltaTime);
		ball.Movement();

		if (IsKeyDown('E'))
		{
			eCurrentState = MAIN_MENU;
			ClearScreen();
		}
		
        ClearScreen();
}

void UpdateHighScore()
{
	DrawString("1st : 25", iScreenWidth*.45f, iScreenHeight*.7f);
	DrawString("2nd : 20", iScreenWidth *.45f, iScreenHeight *.6f);
	DrawString("3rd : 15", iScreenWidth*.45f, iScreenHeight * .5f);
	DrawString("4th : 10", iScreenWidth *.45f, iScreenHeight * .4f);
	DrawString("5th : 5", iScreenWidth * .45f, iScreenHeight*.3f);
	DrawString("Press E to go to Main Menu", iScreenWidth*.025, iScreenHeight - 2);
	DrawString("Press Enter to start game", iScreenWidth* .6f, iScreenHeight - 2);
	
	if (IsKeyDown('E'))
	{
		eCurrentState = MAIN_MENU;
		ClearScreen();
	}
	if (IsKeyDown(GLFW_KEY_ENTER))
	{
		eCurrentState = GAMEPLAY;
		ClearScreen();
	}
	
	ClearScreen();
}