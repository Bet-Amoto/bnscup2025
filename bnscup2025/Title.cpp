# include "Title.hpp"

Title::Title(const InitData& init)
	: IScene{ init }
{

}

void Title::update()
{
	// ボタンの更新
	{
		m_startTransition.update(m_startButton.mouseOver());
		m_exitTransition.update(m_exitButton.mouseOver());

		if (m_startButton.mouseOver() || m_exitButton.mouseOver())
		{
			Cursor::RequestStyle(CursorStyle::Hand);
		}
	}

	// ボタンのクリック処理
	if (m_startButton.leftClicked()) // ゲームへ
	{
		getData().status.init();
		getData().sc.init();
		changeScene(State::Game);
	}
	else if (m_exitButton.leftClicked()) // 終了
	{
		System::Exit();
	}
}

void Title::draw() const
{
	Scene::SetBackground(ColorF{ 0.2, 0.8, 0.8 });

	double t = Scene::Time() * 25;

	// 背景描画
	for (int y : step(Scene::Height() / 10))
	{
		for (int x : step(Scene::Width() / 10))
		{
			double alpha = Max(0.0, noise.octave2D0_1(x / 50.0, (y + t) / 50.0, 4) - 0.5);
			if (alpha > 0.0) Circle{ x * 10, y * 10, 20 }.draw(ColorF(1 - alpha, alpha));
		}
	}

	// タイトル描画
	FontAsset(U"TitleFont")(U"ローグライクヤッツィー")
		.drawAt(TextStyle::OutlineShadow(0.2, ColorF{ 0.2, 0.6, 0.2 }, Vec2{ 3, 3 }, ColorF{ 0.0, 0.5 }), 100, Vec2{ Scene::Center().x , 100});

	// ボタン描画
	{
		m_startButton.draw(ColorF{ 1.0, m_startTransition.value() }).drawFrame(2);
		m_exitButton.draw(ColorF{ 1.0, m_exitTransition.value() }).drawFrame(2);

		const Font& boldFont = FontAsset(U"Bold");
		boldFont(U"PLAY").drawAt(36, m_startButton.center(), ColorF{ 0.1 });
		boldFont(U"EXIT").drawAt(36, m_exitButton.center(), ColorF{ 0.1 });
	}
}

void Title::drawFadeIn(double t) const
{
	draw();
	getData().fromState = State::Title;
	getData().toState = State::Game;
	getData().sc.drawTransitionIn(t);
}

void Title::drawFadeOut(double t) const
{
	draw();
	getData().sc.drawTransitionOut(t);
}
