#include "stdafx.h"
#include "Game.h"

Game::Game()
{
	Reset();
}

void Game::Reset()
{
	Console::SetWindowSize(WINDOW_WIDTH, WINDOW_HEIGHT);
	Console::CursorVisible(false);
	paddle.width = 12;
	paddle.height = 2;
	paddle.x_position = 32;
	paddle.y_position = 30;

	ball.visage = 'O';
	ball.color = ConsoleColor::Cyan;
	ResetBall();

	// TODO #2 - Add this brick and 4 more bricks to the vector
	bricks.clear(); // Clear vector before adding new ones

	// Iterate 5 times to create 5 bricks
	for (int i = 0; i < 5; ++i)
	{
		Box newBrick;
		newBrick.width = 10;
		newBrick.height = 2;
		newBrick.x_position = i * 12; // Position bricks with some spacing (TODO #3?)
		newBrick.y_position = 5;
		newBrick.doubleThick = true;
		newBrick.color = ConsoleColor::DarkGreen;
		bricks.push_back(newBrick);
	}
}

void Game::ResetBall()
{
	ball.x_position = paddle.x_position + paddle.width / 2;
	ball.y_position = paddle.y_position - 1;
	ball.x_velocity = rand() % 2 ? 1 : -1;
	ball.y_velocity = -1;
	ball.moving = false;
}

bool Game::Update()
{
	if (GetAsyncKeyState(VK_ESCAPE) & 0x1)
		return false;

	if (GetAsyncKeyState(VK_RIGHT) && paddle.x_position < WINDOW_WIDTH - paddle.width)
		paddle.x_position += 2;

	if (GetAsyncKeyState(VK_LEFT) && paddle.x_position > 0)
		paddle.x_position -= 2;

	if (GetAsyncKeyState(VK_SPACE) & 0x1)
		ball.moving = !ball.moving;

	if (GetAsyncKeyState('R') & 0x1)
		Reset();

	ball.Update();
	CheckCollision();
	return true;
}

//  All rendering, including text, should occur in the Render function
void Game::Render() const
{
	Console::Lock(true);
	Console::Clear();
	
	paddle.Draw();
	ball.Draw();

	// TODO #3 - Update render to render all bricks
	for (const auto& brick : bricks)
	{
		brick.Draw();
	}

	// Display Victory Text
	if (bricks.empty())
	{
		Console::SetCursorPosition(20, 15);
		Console::ForegroundColor(Yellow);
		Console::WordWrap(20, 15, 40, "VICTORY!!! Press R to Reset!");
		Console::ResetColor();
	}

	// Display Defeat Text
	if (ball.y_position >= WINDOW_HEIGHT)
	{
		Console::SetCursorPosition(20, 15);
		Console::ForegroundColor(Red);
		Console::WordWrap(20, 15, 40, "DEFEAT!!! Press R to Reset!");
		Console::ResetColor();
	}

	Console::Lock(false);
}

void Game::CheckCollision()
{
	// TODO #4 - Update collision to check all bricks
	
	// Iterate through bricks for collision check
	for (int i = 0; i < bricks.size(); ++i)
	{
		if (bricks[i].Contains(ball.x_position + ball.x_velocity, ball.y_position + ball.y_velocity))
		{
			bricks[i].color = ConsoleColor(bricks[i].color - 1);
			ball.y_velocity *= -1;

		// TODO #5 - If the ball hits the same brick 3 times (color == black), remove it from the vector
			
			// Black Color Check for brick removal
			if (bricks[i].color == ConsoleColor::Black)
			{
				bricks.erase(bricks.begin() + i);
			}

			break; // Exit collision loop
		}
	}

	// TODO #6 - If no bricks remain, pause ball and display (render) victory text with R to reset

	// Check for victory condition (no bricks left)
	if (bricks.empty())
	{
		// Pause the ball
		ball.moving = false;
	}

	// Paddle collision check
	if (paddle.Contains(ball.x_position + ball.x_velocity, ball.y_velocity + ball.y_position))
	{
		ball.y_velocity *= -1;	
	}
	

	// TODO #7 - If ball touches bottom of window, pause ball and display (render) defeat text with R to reset

	// Check for defeat condition (ball touches bottom)
	if (ball.y_position + ball.y_velocity >= WINDOW_HEIGHT)
	{
		// Pause the ball
		ball.moving = false;
	}
}
