# include "Game.hpp"

Game::Game(const InitData& init)
	: IScene{ init }
{
	Scene::SetBackground(ColorF{ 0.7,0.7,1.0 });
	for (size_t i = 0; i < 5; ++i)
	{
		m_dices << Dice{ Vec2{ 100 + i * 80, 500 } };
	}
	for (const auto& [index, category] : Indexed(Categories::UpperCategories))
	{
		m_categoryBoxes << CategoryBox{ Vec2{ 50, 50 + index * 60 }, category };
	}
	for (const auto& [index, category] : Indexed(Categories::LowerCategories))
	{
		m_categoryBoxes << CategoryBox{ Vec2{ 400, 50 + index * 60 }, category };
	}
}

void Game::update()
{
	rollAllDicesButton();
	for(auto& dice : m_dices)
	{
		if (dice.isClicked())
		{
			dice.setLocked(!dice.isLocked());
		}
	}

	for (auto& box : m_categoryBoxes) {
		if (box.isClicked() &&  !box.getScore()) {
			box.setScore(box.getProvisionalScore(m_dices));
			m_rollsLeft = maxRolls;
			for (auto& dice : m_dices) {
				dice.Clear();
			}
		}
	}

	if (KeyR.down()) {
		m_rollsLeft = maxRolls;
		for (auto& dice : m_dices) {
			dice.Clear();
		}
		for (auto& box : m_categoryBoxes) {
			box.reset();
		}
	}
}

void Game::draw() const
{
	constexpr int spacing = 80;
	for (size_t i = 0; i < m_dices.size(); ++i)
	{
		m_dices[i].draw();
	}

	m_rollButton.draw(m_rollsLeft > 0 ? ColorF{ 1.0 } : ColorF{ 0.7 });
	FontAsset(U"Bold")(U"Roll").drawAt(m_rollButton.center(), ColorF{ 0.1 });
	FontAsset(U"Regular")(U"リロール {}回"_fmt(m_rollsLeft)).draw(24, m_rollButton.x, m_rollButton.y - 30, ColorF{ 0.1 });

	for (auto& box : m_categoryBoxes) {
		box.draw(m_dices);
	}
	FontAsset(U"Category")(U"小計 {}"_fmt(UpperCategoriesScore())).draw(32, 55, 410, ColorF{ 0.1 });
	if (isBonus()) {
		FontAsset(U"Category")(U"ボーナス +{}"_fmt(Categories::UpperSectionBonusScore)).draw(32, 190, 410, ColorF{ 1.0,1.0,0.0 });
	}
	FontAsset(U"Bold")(U"スコア　{}"_fmt(totalScore())).drawAt(Scene::CenterF().x, 22, ColorF{ 0.1 });
}

void Game::rollAllDicesButton()
{
	if (m_rollButton.leftClicked() && m_rollsLeft > 0)
	{
		for (auto& dice : m_dices)
		{
			dice.roll();
		}
		m_rollsLeft--;
	}
}

int Game::UpperCategoriesScore() const
{
	int score = 0;
	for (const auto& box : m_categoryBoxes) {
		if (Categories::UpperCategories.contains(box.getCategory()) && box.getScore()) {
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
