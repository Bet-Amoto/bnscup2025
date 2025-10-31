#pragma once
#include "Siv3D.hpp"
#include "Category.hpp"
#include "Die.hpp"
#include "Item.hpp"
#include "Rarity.hpp"
/// @brief ノルマ
struct Quota
{
	int32 turn = 1;		// 何ターン目か
	int32 target = 50;  // ノルマ
	int32 earned = 0;   // そのターンに稼いだスコア
};

/// @brief ゲーム統計情報
struct GameStats
{
	int32 totalRerolls = 0;        // これまでの総リロール回数
	int32 currentGameRerolls = 0;  // 現在のゲームでのリロール回数
	int32 highScore = 0;           // ハイスコア
	int32 gamesPlayed = 0;         // プレイしたゲーム数
	int32 totalGoldEarned = 0;     // これまでに獲得した総ゴールド
	Category* lastAchievedCategory = nullptr; // 最後に達成した役
	Array<Die> lastAchievedDices; // 最後に達成した役のダイス
};

struct Status
{
	Array<Die> dices;
	Array<Category> upperCategories;
	Array<Category> lowerCategories;
	Array<Artifact> artifacts;
	int maxRolls = 3;
	int upperSectionBonusThreshold = Categories::UpperSectionBonusThreshold;
	int UpperSectionBonusScore = Categories::UpperSectionBonusScore;

	int32 selectionsPerTurn = 5; // 1ターンの役選択回数
	int32 selectionsLeft = 5;    // 残りの役選択回数
	int32 gold = 0;				 // 所持金
	Quota quota;				 // ノルマ
	GameStats gameStats;     // ゲーム統計情報


	Array<Die> availableDices; // ショップで購入可能なダイス
	Array<Category> availableCategories; // ショップで購入可能な役
	Array<Artifact> availableArtifacts; // ショップで購入可能なアイテム
	int ShopDiceCount = 2;	// ショップで表示するダイスの数
	int ShopCategoryCount = 2;	// ショップで表示するカテゴリの数
	int ShopArtifactCount = 2;
	int ShopRerollPrice = 50;          // 現在のリロール価格
	int ShopRerollPriceIncrease = 50;  // リロールごとの値上げ幅
	int ShopRerollBasePrice = 50;      // リロールの初期価格（リセット用）

	// レアリティごとの出現確率
	double rarityWeightCommon = 1000.0;     // Common
	double rarityWeightRare = 300.0;        // Rare
	double rarityWeightEpic = 100.0;        // Epic
	double rarityWeightLegendary = 20.0;    // Legendary
	DiscreteDistribution distribution{ // レアリティごとの出現確率
	{
		1000, // Common
		300,  // Rare
		100,  // Epic
		20    // Legendary
	} };

	Status()
		: dices({ Dice::HeartDie(), Dice::HeartDie(), Dice::MirageDie(), Dice::GoldDie(), Dice::QuakeDie() }),
		upperCategories(Categories::DefaultUpperCategories),
		lowerCategories(Categories::DefaultLowerCategories),
		artifacts(),
		maxRolls(3),
		upperSectionBonusThreshold(Categories::UpperSectionBonusThreshold),
		UpperSectionBonusScore(Categories::UpperSectionBonusScore),
		selectionsPerTurn(5),
		selectionsLeft(5),
		gold(0),
		quota(Quota()),
		availableDices(Dice::AllDice),
		availableCategories(Categories::AllCategories),
		availableArtifacts(Items::AllItems),
		ShopDiceCount(2),
		ShopCategoryCount(2),
		ShopArtifactCount(2),
		rarityWeightCommon(1000.0),
		rarityWeightRare(300.0),
		rarityWeightEpic(100.0),
		rarityWeightLegendary(20.0),
		distribution({ rarityWeightCommon, rarityWeightRare, rarityWeightEpic, rarityWeightLegendary })
	{
	};

	void init()
	{
		dices = { Dice::HeartDie(), Dice::HeartDie(), Dice::MirageDie(), Dice::GoldDie(), Dice::QuakeDie() };
		upperCategories = Categories::DefaultUpperCategories;
		lowerCategories = Categories::DefaultLowerCategories;
		artifacts.clear();
		maxRolls = 3;
		upperSectionBonusThreshold = Categories::UpperSectionBonusThreshold;
		UpperSectionBonusScore = Categories::UpperSectionBonusScore;
		selectionsPerTurn = 5;
		selectionsLeft = 5;
		gold = 0;
		quota = Quota();
		availableDices = Dice::AllDice;
		availableCategories = Categories::AllCategories;
		availableArtifacts = Items::AllItems;
		ShopDiceCount = 2;
		ShopCategoryCount = 2;
		ShopArtifactCount = 2;
		rarityWeightCommon = 1000.0;
		rarityWeightRare = 300.0;
		rarityWeightEpic = 100.0;
		rarityWeightLegendary = 20.0;
		updateDistribution();
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

	/// @brief レアリティの出現確率分布を更新する
	void updateDistribution()
	{
		distribution = DiscreteDistribution({
			rarityWeightCommon,
			rarityWeightRare,
			rarityWeightEpic,
			rarityWeightLegendary
		});
	}
};

inline void addGold(Status& s, int32 amount)
{
	s.gold += amount;
}
