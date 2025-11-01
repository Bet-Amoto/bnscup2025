# pragma once
# include "Common.hpp"
# include "Die.hpp"
# include "DiceBox.hpp"
# include "CategoryBox.hpp"
# include "Category.hpp"
# include "Item.hpp"
// ゲームシーン
class Game : public App::Scene
{
public:

	Game(const InitData& init);

	void update() override;

	void draw() const override;

private:
	DiceBox m_diceBox;
	Array<CategoryBox> m_categoryBoxes;
	const RectF m_rollButton{ 890, 630, 150, 60 };

	void rollAllDicesButton();
	int UpperCategoriesScore() const;
	int totalScore() const;
	bool isBonus() const { return UpperCategoriesScore() > Categories::UpperSectionBonusThreshold; }
	const int maxRolls;
	int m_rollsLeft;

	int m_lastScore = 0;
	Stopwatch m_animScoreTimer;
	void drawScore(const Vec2& center) const;

	bool m_isGameOver = false;
	bool m_viewBoard = false;
	void drawGameOver() const;
	Stopwatch m_gameOverAnimTimer;
	const Rect TitleButtonRect = Rect{ Scene::Center().x + 100,500,250,100};
	const Rect ViewBoardButtonRect = Rect{ Scene::Center().x - 350, 500, 250, 100};

	bool m_isTurnEnd = false;
	void drawTurnEnd() const;
	const Rect BGRect = Rect{ Arg::center(Scene::Center()), 550, 650 };
	int startedGold = 0;
	
};
