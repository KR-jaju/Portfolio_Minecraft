#pragma once

#include "Window.h"
#include "FSM.h"

/*

TODO: 상태머신을 통한 씬 전환을 상상했으나 시간이 없었다.

*/

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