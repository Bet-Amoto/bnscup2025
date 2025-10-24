# pragma once
# include <Siv3D.hpp>
#include "Status.hpp"
// シーンのステート
enum class State
{
	Title,
	Game
};

// 共有するデータ
struct GameData
{
	// 直前のゲームのスコア
	int32 lastScore = 0;

	// ハイスコア
	Array<int32> highScores = { 10, 8, 6, 4, 2 };

	Status status;
};

using App = SceneManager<State, GameData>;
