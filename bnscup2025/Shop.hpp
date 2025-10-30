# pragma once
#include "Common.hpp"
#include "Status.hpp"
#include "DiceBox.hpp"
#include "Merchandice.hpp"
#include "Explanation.hpp"
#include "Item.hpp"

class Shop : public App::Scene
{
public:
	Shop(const InitData& init);
	void update() override;
	void draw() const override;

private:
	Status& m_status = getData().status;
	Array<Die> m_availableDices = m_status.availableDices;
	Array<Category> m_availableCategories = m_status.availableCategories;
	Array<Merchandise> m_merchandises;

	const Rect RerollButtonRect = Rect{ 750,600,120,60 };
	const Rect NextTurnButtonRect = Rect{ 900,580,250,100 };

	Merchandise* m_holdedItem = nullptr;
	DiceBox m_diceBox;
	const Rect viewportRect = Rect{ 240, 60, 800, 600 };
	const Rect boughtButtonRect = Rect{ 430,500,190,60 };
	const Rect cancelButtonRect = Rect{ 180,500,190,60 };
	bool m_selected = false;

	const Vec2 categorySelectRectSize = Vec2{ 500, 40 };
	Category* selectedCategory = nullptr;
	void categorySelect();
	void categorySelectDraw() const;
	void reroll();
	void clearSelect();
};
