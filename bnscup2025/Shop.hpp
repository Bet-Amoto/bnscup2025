# pragma once
#include "Common.hpp"
#include "Status.hpp"
#include "DiceBox.hpp"
#include "Merchandice.hpp"
#include "Explanation.hpp"

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
	Merchandise* m_holdedItem = nullptr;
	DiceBox m_diceBox;
	const Rect viewportRect = Rect{ 240, 60, 800, 600 };
	const Rect boughtButtonRect = Rect{ 450,500,150,60 };
	const Rect cancelButtonRect = Rect{ 200,500,150,60 };
	bool m_selected = false;
};
