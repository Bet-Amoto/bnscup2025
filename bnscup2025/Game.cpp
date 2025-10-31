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
}

void Game::update()
{
	rollAllDicesButton();
	m_diceBox.update(getData().status);
	if(m_diceBox.getClickedDie())
	{
		m_diceBox.getClickedDie()->locked = !m_diceBox.getClickedDie()->locked || !m_diceBox.getClickedDie()->canUnlock;

	}
	for (auto& box : m_categoryBoxes) {
		if (box.mouseOver()) {
			GetExplanation().setItem(box.category);
		}
		if (box.isClicked() &&  !box.getScore() && getData().status.selectionsLeft > 0) {
			const int prov = box.getProvisionalScore(getData().status.dices, getData().status);
			box.setScore(prov);
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

	if (getData().status.selectionsLeft == 0) {
		for (auto& box : m_categoryBoxes) {
			if (!box.getScore()) {
				box.setScore(0);
			}
		}
		m_rollsLeft = 0;
	}

	if (getData().status.selectionsLeft <= 0 && KeyR.down()) {
		m_rollsLeft = maxRolls;
		m_diceBox.clear();
		for (auto& box : m_categoryBoxes) {
			box.reset();
		}
		getData().status.endTurn();
	}

	if (getData().status.selectionsLeft <= 0 && KeySpace.down()) {
		changeScene(State::Shop);
		getData().status.endTurn();
	}

	if (getData().status.selectionsLeft <= 0 && ShopButtonRect.leftClicked()) {
		changeScene(State::Shop);
		getData().status.endTurn();
	}
}

void Game::draw() const
{
	constexpr int spacing = 80;
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

	drawScore(Vec2{Scene::Center().x, 40});

	FontAsset(U"Regular")(U"ターン {}"_fmt(getData().status.quota.turn)).draw(24, 20, 500, ColorF{0.1});
	FontAsset(U"Regular")(U"ノルマ {}"_fmt(getData().status.quota.target)).draw(24, 20, 530, ColorF{ 0.1 });
	FontAsset(U"Regular")(U"達成 {}"_fmt(getData().status.quota.earned)).draw(24, 160, 530, ColorF{ 0.1 });
	FontAsset(U"Regular")(U"残り選択 {}"_fmt(getData().status.selectionsLeft)).draw(24, 300, 530, ColorF{ 0.1 });
	FontAsset(U"Regular")(U"所持G {}"_fmt(getData().status.gold)).draw(24, 450, 530, ColorF{ 0.1 });

	if (getData().status.selectionsLeft <= 0) {
		FontAsset(U"Bold")(U"ショップへ").drawAt(40, ShopButtonRect.center(), ColorF{ 0.1 });
	}

	GetExplanation().draw(Cursor::PosF());
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
