#pragma once

#include "Window.h"
#include "FSM.h"

class Game
{
public:
	Game(HINSTANCE instance, int nCmdShow);
	void	run();
private:
	enum class SceneType
	{
		InGame
	};
	FSM<Game, SceneType> fsm;
};