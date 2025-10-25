#pragma once
#include "Siv3D.hpp"
#include "Category.hpp"
#include "Die.hpp"

struct Status
{
	Array<Die> dices;
	Array<Category> upperCategories;
	Array<Category> lowerCategories;
	int maxRolls = 3;
	int upperSectionBonusThreshold = Categories::UpperSectionBonusThreshold;
	int UpperSectionBonusScore = Categories::UpperSectionBonusScore;

	Status()
		: dices({ Dice::StandardDie, Dice::EvenDie, Dice::OddDie, Dice::Coin, Dice::HighCoin }),
		upperCategories(Categories::UpperCategories),
		lowerCategories(Categories::LowerCategories),
		maxRolls(3),
		upperSectionBonusThreshold(Categories::UpperSectionBonusThreshold),
		UpperSectionBonusScore(Categories::UpperSectionBonusScore) 
	{
	};

	void init()
	{
		dices = { Dice::StandardDie, Dice::StandardDie, Dice::EvenDie, Dice::OddDie, Dice::Coin };
		upperCategories = Categories::UpperCategories;
		lowerCategories = Categories::LowerCategories;
		maxRolls = 3;
		upperSectionBonusThreshold = Categories::UpperSectionBonusThreshold;
		UpperSectionBonusScore = Categories::UpperSectionBonusScore;
	}
};
