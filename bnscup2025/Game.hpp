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

	void drawFadeIn(double t) const override;

	void drawFadeOut(double t) const override;

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

	const RectF BGRectStart = Rect{ Arg::center(Scene::Center().x, -650), 550, 650 };
	const RectF BGRectEnd = Rect{ Arg::center(Scene::Center()), 550, 650 };

	bool m_isGameOver = false;
	bool m_viewBoard = false;
	void drawGameOver() const;
	Stopwatch m_gameOverAnimTimer;
	RectF ViewBoardButtonRect = RectF{ Arg::center(BGRectEnd.center().movedBy(0, -50)), 150, 50};

	bool m_isTurnEnd = false;
	void drawTurnEnd() const;
	int startedGold = 0;

	State nextScene = State::Title;
	
};
