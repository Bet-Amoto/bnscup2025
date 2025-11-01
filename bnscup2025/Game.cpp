# include "Game.hpp"

Game::Game(const InitData& init)
	: IScene{ init },
	maxRolls{ getData().status.maxRolls },
	m_rollsLeft{ getData().status.maxRolls },
	m_diceBox{ Vec2{640, 630}, getData().status.dices }
{
	Scene::SetBackground(ColorF{ 0.7,0.7,1.0 });
	for (int i : step(getData().status.upperCategories.size())) {
		m_categoryBoxes << CategoryBox{ Vec2{ 290, 100 + i * 60 }, getData().status.upperCategories[i]};
	}
	for (int i : step(getData().status.lowerCategories.size())) {
		m_categoryBoxes << CategoryBox{ Vec2{ 640, 100 + i * 60 }, getData().status.lowerCategories[i] };
	}
	for (auto& cateBox : m_categoryBoxes) {
		cateBox.reset();
	}
	m_diceBox.clear();
	m_animScoreTimer.start();
	getData().status.gameStats.lastGoldEarned = 0;

	startedGold = getData().status.gold;
}

void Game::update()
{
	if (m_isGameOver && !m_viewBoard)
	{
		if (ViewBoardButtonRect.leftClicked()) m_viewBoard = true;
		if (TitleButtonRect.leftClicked()) {
			getData().fromState = State::Game;
			getData().toState = State::Title;
			changeScene(State::Title);
		}
		return;
	}

	if (m_isTurnEnd)
	{
		if (MouseL.down()) {
			getData().fromState = State::Game;
			getData().toState = State::Shop;
			changeScene(State::Shop);
		}
		return;
	}

	rollAllDicesButton();
	m_diceBox.update(getData().status);
	for (auto& box : m_categoryBoxes) {
		if (box.mouseOver()) {
			GetExplanation().setItem(box.category);
		}
	}

	if (m_isGameOver && m_viewBoard)
	{
		if (m_viewBoard && MouseL.down()) m_viewBoard = false;
		return;
	}

	if(m_diceBox.getClickedDie())
	{
		m_diceBox.getClickedDie()->locked = !m_diceBox.getClickedDie()->locked || !m_diceBox.getClickedDie()->canUnlock;

	}

	for (auto& box : m_categoryBoxes) {
		if (box.isClicked() &&  !box.getScore() && getData().status.selectionsLeft > 0) {
			const int prov = box.getProvisionalScore(getData().status.dices, getData().status);
			box.setScore(prov);
			if (box.getCategory().onSelected)
			{
				box.getCategory().onSelected(getData().status.dices, getData().status);
			}
			m_lastScore = getData().status.quota.earned;
			m_animScoreTimer.restart();
			getData().status.quota.earned += prov;
			getData().status.selectionsLeft -= 1;
			getData().status.gameStats.lastAchievedCategory = box.category;
			getData().status.gameStats.lastAchievedDices = getData().status.dices;
			ActivateArtifactsByTiming(getData().status.artifacts, ActivationTiming::OnDiceResult, getData().status);
			m_rollsLeft = maxRolls;
			m_diceBox.clear();

		}
	}

	if (getData().status.selectionsLeft == 0 && !m_isTurnEnd && !m_isGameOver) {
		for (auto& box : m_categoryBoxes) {
			if (!box.getScore()) {
				box.setScore(0);
			}
		}
		m_rollsLeft = 0;
		getData().status.endTurn();

		if (totalScore() < getData().status.quota.target) {
			m_isGameOver = true;
		}
		else {
			m_isTurnEnd = true;
		}

	}
}

void Game::draw() const
{
	constexpr int spacing = 80;

	FontAsset(U"Bold")(U"ターン {}"_fmt(getData().status.quota.turn)).draw(32, 80, 10, ColorF{ 0.1 });
	FontAsset(U"Bold")(U"ノルマ {}"_fmt(getData().status.quota.target)).draw(32, 80, 45, ColorF{ 0.1 });
	drawScore(Vec2{ Scene::Center().x, 35 });
	FontAsset(U"Regular")(U"残り選択 {}"_fmt(getData().status.selectionsLeft)).drawAt(28, Scene::Center().x,78, ColorF{0.1});
	FontAsset(U"Bold")(U"獲得G {}"_fmt(getData().status.gameStats.lastGoldEarned)).drawAt(32, 1000, 30, ColorF{ 0.1 });
	FontAsset(U"Bold")(U"所持G {}"_fmt(getData().status.gold)).drawAt(32, 1000, 70, ColorF{ 0.1 });
	m_diceBox.draw();

	m_rollButton.draw(m_rollsLeft > 0 ? ColorF{ 1.0 } : ColorF{ 0.7 });
	FontAsset(U"Bold")(U"Roll").drawAt(m_rollButton.center(), ColorF{ 0.1 });
	FontAsset(U"Regular")(U"リロール {}回"_fmt(m_rollsLeft)).draw(24, m_rollButton.x, m_rollButton.y - 30, ColorF{ 0.1 });
	for (auto& category : m_categoryBoxes) {
		category.draw(getData().status.dices, getData().status);
	}
	m_diceBox.draw();
	FontAsset(U"Category")(U"小計 {}"_fmt(UpperCategoriesScore())).draw(32, 285, 460, ColorF{ 0.1 });
	if (isBonus()) {
		FontAsset(U"Category")(U"ボーナス +{}"_fmt(Categories::UpperSectionBonusScore)).draw(32, 415, 460, ColorF{ 1.0,1.0,0.0 });
	}

	GetExplanation().draw(Cursor::PosF());
	if(m_isGameOver && !m_viewBoard) drawGameOver();
	if (m_isTurnEnd) drawTurnEnd();
}

void Game::rollAllDicesButton()
{
	if (m_rollButton.leftClicked() && m_rollsLeft > 0 && !m_diceBox.isRolling())
	{
		m_diceBox.roll(getData().status);
		m_rollsLeft--;
	}
}

int Game::UpperCategoriesScore() const
{
	int score = 0;
	for (const auto& box : m_categoryBoxes) {
		if (box.getCategory().type == CategoryType::Upper && box.getScore()) {
			score += box.getScore().value();
		}
	}
	return score;
}

int Game::totalScore() const {
	int score = 0;
	for (const auto& box : m_categoryBoxes) {
		if (box.getScore()) {
			score += box.getScore().value();
		}
	}

	if (isBonus()) {
		score += Categories::UpperSectionBonusScore;
	}

	return score;
}

void Game::drawScore(const Vec2& center) const {
	const double t = Min(1.0, EaseOutCubic(m_animScoreTimer.sF()));
	const int score = totalScore() * t + m_lastScore * (1.0 - t);
	const double fontSize = 48;
	FontAsset(U"Bold")(U"スコア　{}"_fmt(score)).drawAt(fontSize, center, ColorF{ 0.1 });
}

void Game::drawGameOver() const {
	Rect{ 0, 0, Scene::Size() }.draw( ColorF{0.0, 0.98} );
	FontAsset(U"Bold")(U"ゲームオーバー").drawAt(60, Scene::Center().x, 100, ColorF{1.0, 0.2, 0.2});
	FontAsset(U"Bold")(U"ターン {}"_fmt(getData().status.quota.turn)).drawAt(40, Scene::Center().x, 180, ColorF{0.9});
	FontAsset(U"Bold")(U"スコア {}"_fmt(getData().status.quota.earned)).drawAt(40, Scene::Center().x, 220, ColorF{0.9});
	TitleButtonRect.rounded(10).draw(ColorF{ 0.9 }).drawFrame(3, ColorF{0.1});
	FontAsset(U"Bold")(U"タイトルへ"_fmt(getData().status.quota.earned)).drawAt(40, TitleButtonRect.center(), ColorF{0.1});

	ViewBoardButtonRect.rounded(10).draw(ColorF{ 0.9 }).drawFrame(3, ColorF{ 0.1 });
	FontAsset(U"Bold")(U"盤面を見る"_fmt(getData().status.quota.earned)).drawAt(40, ViewBoardButtonRect.center(), ColorF{ 0.1 });

}

void Game::drawTurnEnd() const {
	Rect{ 0, 0, Scene::Size() }.draw(ColorF{ 0.0, 0.7 });
	BGRect.rounded(10).draw(ColorF{ 1.0 }).drawFrame(4, ColorF{0.3, 0.15, 0.02});
	FontAsset(U"Bold")(U"ターン{} クリア！"_fmt(getData().status.quota.turn)).drawAt( BGRect.centerX(), 80 , ColorF{0.1});
	FontAsset(U"Bold")(U"スコア {}"_fmt(getData().status.quota.earned)).drawAt(36,  BGRect.centerX(), 140 , ColorF{0.1});

	double y = 250;
	const double distY = 35;
	FontAsset(U"Bold")(U"元の所持金").draw(32, BGRect.centerX() - 220, y , ColorF{0.1});
	FontAsset(U"Bold")(U"{}G"_fmt(startedGold)).draw(32, Arg::topRight(BGRect.centerX() + 220, y), ColorF{ 0.1 });
	y += distY;

	FontAsset(U"Bold")(U"スコアボーナス").draw(32, BGRect.centerX() - 220, y, ColorF{ 0.1 });
	FontAsset(U"Bold")(U"{}G"_fmt(calcScoreToGold(getData().status, totalScore()))).draw(32, Arg::topRight(BGRect.centerX() + 220, y), ColorF{ 0.1 });
	y += distY;

	if (getData().status.gameStats.lastGoldEarned != 0) {
		const ColorF c = getData().status.gameStats.lastGoldEarned > 0 ? ColorF{ 0.1 } : ColorF{ 1.0, 0.0, 0.0 };
		FontAsset(U"Bold")(U"ゲーム中に獲得").draw(32, BGRect.centerX() - 220, y, ColorF{ 0.1 });
		FontAsset(U"Bold")(U"{}G"_fmt(getData().status.gameStats.lastGoldEarned)).draw(32, Arg::topRight(BGRect.centerX() + 220, y), c);
		y += distY;
	}

	y += 5;
	Line{ BGRect.centerX() - 220 , y , BGRect.centerX() + 220, y }.draw(2, ColorF{ 0.1 });

	FontAsset(U"Bold")(U"現在の所持金").draw(32, BGRect.centerX() - 220, y, ColorF{ 0.1 });
	FontAsset(U"Bold")(U"{}G"_fmt(getData().status.gold)).draw(32, Arg::topRight(BGRect.centerX() + 220, y), ColorF{ 0.1 });

	FontAsset(U"Regular")(U"クリックでショップへ行く").drawAt(24, BGRect.centerX(), 600, ColorF{ 0.1 });
}

void Game::drawFadeIn(double t) const
{
	draw();
	if(getData().fromState == State::Title) getData().sc.drawTransitionIn(t);
	else if(getData().fromState == State::Shop) getData().rec.drawFadeIn(t);
}

void Game::drawFadeOut(double t) const
{
	draw();
	if(getData().toState == State::Title) getData().sc.drawTransitionOut(t);
	else if (getData().toState == State::Shop) getData().rec.drawFadeIn(1 - t);
}
