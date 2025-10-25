#pragma once
#include "Siv3D.hpp"
#include "Category.hpp"
#include "Die.hpp"

/// @brief ノルマ
struct Quota
{
	int32 turn = 1;		// 何ターン目か
	int32 target = 50;  // ノルマ
	int32 earned = 0;   // そのターンに稼いだスコア
};

struct Status
{
	Array<Die> dices;
	Array<Category> upperCategories;
	Array<Category> lowerCategories;
	int maxRolls = 3;
	int upperSectionBonusThreshold = Categories::UpperSectionBonusThreshold;
	int UpperSectionBonusScore = Categories::UpperSectionBonusScore;

	int32 selectionsPerTurn = 5; // 1ターンの役選択回数
	int32 selectionsLeft = 5;    // 残りの役選択回数
	int32 gold = 0;				 // 所持金
	Quota quota;				 // ノルマ

	Status()
		: dices({ Dice::StandardDie, Dice::EvenDie, Dice::OddDie, Dice::Coin, Dice::HighCoin }),
		upperCategories(Categories::UpperCategories),
		lowerCategories(Categories::LowerCategories),
		maxRolls(3),
		upperSectionBonusThreshold(Categories::UpperSectionBonusThreshold),
		UpperSectionBonusScore(Categories::UpperSectionBonusScore),
		selectionsPerTurn(5),
		selectionsLeft(5),
		gold(0),
		quota(Quota())
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
		selectionsPerTurn = 5;
		selectionsLeft = 5;
		gold = 0;
		quota = Quota();
	}

	void beginTurn()
	{
		selectionsLeft = selectionsPerTurn;
		quota.earned = 0;
	}

	void endTurn()
	{
		gold += quota.earned;
		quota.turn += 1;
		quota.target *= 1.1;
		beginTurn();
	}
};
