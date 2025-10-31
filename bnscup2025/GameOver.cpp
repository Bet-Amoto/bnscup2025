# include "GameOver.hpp"

GameOver::GameOver(const InitData& init)
	: IScene{ init }
	, status{ getData().status }
{
	Scene::SetBackground(ColorF{ 0.1, 0.1, 0.1 });
}

void GameOver::update()
{
	if (KeySpace.down())
	{
		changeScene(State::Title);
	}
}

void GameOver::draw() const
{
	FontAsset(U"Bold")(U"ゲームオーバー").drawAt(Scene::CenterF().x, 200, ColorF{ 1.0, 0.2, 0.2 });
	FontAsset(U"Regular")(U"ハイスコア: {}"_fmt(status.gameStats.highScore)).drawAt(Scene::CenterF().x, 300, ColorF{ 1.0 });
	FontAsset(U"Regular")(U"最終ターン: {}"_fmt(status.quota.turn)).drawAt(Scene::CenterF().x, 400, ColorF{ 1.0 });
	FontAsset(U"Regular")(U"スペースキーを押してタイトルに戻る").drawAt(Scene::CenterF().x, 500, ColorF{ 1.0 });
}
