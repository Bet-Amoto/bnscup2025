#include "Shop.hpp"

Shop::Shop(const InitData& init)
	: IScene{ init },
	m_status{ getData().status }
{
	Scene::SetBackground(ColorF{ 0.9, 0.9, 0.8 });
}

void Shop::update()
{
	if (KeySpace.down())
	{
		changeScene(State::Game);
	}
}

void Shop::draw() const
{
	FontAsset(U"TitleFont")(U"ショップへようこそ！").drawAt(Scene::CenterF().x, 100, ColorF{ 0.1 });
	FontAsset(U"Regular")(U"スペースキーを押してゲームに戻る").drawAt(Scene::CenterF().x, 300, ColorF{ 0.1 });
}
