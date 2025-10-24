# pragma once
# include "Common.hpp"
# include "Die.hpp"
# include "DiceBox.hpp"
# include "CategoryBox.hpp"
# include "Category.hpp"
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
	const RectF m_rollButton{ 500, 500, 150, 60 };
	void rollAllDicesButton();
	int UpperCategoriesScore() const;
	int totalScore() const;
	bool isBonus() const { return UpperCategoriesScore() > Categories::UpperSectionBonusThreshold; }
	const int maxRolls;
	int m_rollsLeft;
};
