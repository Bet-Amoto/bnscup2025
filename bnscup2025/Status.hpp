#pragma once
#include "Siv3D.hpp"
#include "Category.hpp"
#include "Die.hpp"
#include "Rarity.hpp"
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

	Array<Die> availableDices; // ショップで購入可能なダイス
	Array<Category> availableCategories; // ショップで購入可能なカテゴリ
	int ShopDiceCount = 3;	// ショップで表示するダイスの数
	int ShopCategoryCount = 3;	// ショップで表示するカテゴリの数
	int ShopRerollPrice = 50;          // 現在のリロール価格
	int ShopRerollPriceIncrease = 10;  // リロールごとの値上げ幅
	int ShopRerollBasePrice = 50;      // リロールの初期価格（リセット用）


	DiscreteDistribution distribution{ // レアリティごとの出現確率
	{
		1000, // Common
		300,  // Rare
		100,  // Epic
		20    // Legendary
	} };

	Status()
		: dices({ Dice::HeartDie(), Dice::HeartDie(), Dice::MirageDie(), Dice::GoldDie(), Dice::QuakeDie() }),
		upperCategories(Categories::UpperCategories),
		lowerCategories(Categories::LowerCategories),
		maxRolls(3),
		upperSectionBonusThreshold(Categories::UpperSectionBonusThreshold),
		UpperSectionBonusScore(Categories::UpperSectionBonusScore),
		selectionsPerTurn(5),
		selectionsLeft(5),
		gold(0),
		quota(Quota()),
		availableDices(Dice::AllDice),
		availableCategories(Categories::AllCategories),
		ShopDiceCount(3),
		ShopCategoryCount(3),
		distribution({ 1000, 300, 100, 20 })
	{
	};

	void init()
	{
		dices = { Dice::HeartDie(), Dice::HeartDie(), Dice::MirageDie(), Dice::GoldDie(), Dice::QuakeDie() };
		upperCategories = Categories::UpperCategories;
		lowerCategories = Categories::LowerCategories;
		maxRolls = 3;
		upperSectionBonusThreshold = Categories::UpperSectionBonusThreshold;
		UpperSectionBonusScore = Categories::UpperSectionBonusScore;
		selectionsPerTurn = 5;
		selectionsLeft = 5;
		gold = 0;
		quota = Quota();
		availableDices = Dice::AllDice;
		availableCategories = Categories::AllCategories;
		ShopDiceCount = 3;
		ShopCategoryCount = 3;
		distribution = DiscreteDistribution({
			1000, // Common
			300,  // Rare
			100,  // Epic
			20    // Legendary
			});
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

inline void addGold(Status& s, int32 amount)
{
	s.gold += amount;
}
