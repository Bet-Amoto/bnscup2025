# pragma once
# include <Siv3D.hpp>
# include "Status.hpp"
# include "transition.hpp"
// シーンのステート
enum class State
{
	Title,
	Game,
	Shop,
	GameOver
};

// 共有するデータ
struct GameData
{
	// 直前のゲームのスコア
	int64 lastScore = 0;

	// ハイスコア
	Array<int64> highScores = { 10, 8, 6, 4, 2 };

	SplitCircle sc{ Scene::Size() };

	RectSlide rec{ Scene::Size() };

	State fromState = State::Title;

	State toState = State::Title;

	Status status;
};

using App = SceneManager<State, GameData>;
