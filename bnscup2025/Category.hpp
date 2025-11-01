# pragma once
#include "Siv3d.hpp"
#include "Die.hpp"
#include "ItemBase.hpp"

struct Status;

using scoreFunc = std::function<int(const Array<Die>&)>;

using scoreFuncWithStatus = std::function<int(const Array<Die>&, Status&)>;

using onSelectedFunc = std::function<void(const Array<Die>&, Status&)>;

enum class CategoryType
{
	Upper,
	Lower
};

struct Category : ItemBase
{
	CategoryType type = CategoryType::Upper;
	scoreFunc calculateScore;
	scoreFuncWithStatus calculateScoreWS = nullptr;
	onSelectedFunc onSelected = nullptr;
	Optional<int> score = none;
	String itemType() const override { return U"役"; }

	bool apply(Status& s) override { return true; }

	std::shared_ptr<ItemBase> clone() const override
	{
		return std::make_shared<Category>(*this);
	}

	bool operator==(const Category& rhs) const
	{
		return name == rhs.name;
	}
};

namespace Categories
{
	inline Category Ones()
	{
		Category cat;
		cat.name = U"エース";
		cat.rarity = Rarity::Common;
		cat.cost = 20;
		cat.description = U"1の目の合計点数が得点となる。";
		cat.textureKey = U"Ones";

		cat.type = CategoryType::Upper;
		cat.calculateScore = [](const Array<Die>& dices) -> int
			{
				return dices.filter([](const Die& d) { return d.value == 1; }).size() * 1;
			};

		return cat;
	}

	inline Category Twos()
	{
		Category cat;
		cat.name = U"デュース";
		cat.rarity = Rarity::Common;
		cat.cost = 50;
		cat.description = U"2の目の合計点数が得点となる。";
		cat.textureKey = U"Twos";

		cat.type = CategoryType::Upper;
		cat.calculateScore = [](const Array<Die>& dices) -> int
			{
				return dices.filter([](const Die& d) { return d.value == 2; }).size() * 2;
			};

		return cat;
	}

	inline Category Threes()
	{
		Category cat;
		cat.name = U"トレイ";
		cat.rarity = Rarity::Common;
		cat.cost = 100;
		cat.description = U"3の目の合計点数が得点となる。";
		cat.textureKey = U"Threes";

		cat.type = CategoryType::Upper;
		cat.calculateScore = [](const Array<Die>& dices) -> int
			{
				return dices.filter([](const Die& d) { return d.value == 3; }).size() * 3;
			};

		return cat;
	}

	inline Category Fours()
	{
		Category cat;
		cat.name = U"フォー";
		cat.rarity = Rarity::Common;
		cat.cost = 100;
		cat.description = U"4の目の合計点数が得点となる。";
		cat.textureKey = U"Fours";

		cat.type = CategoryType::Upper;
		cat.calculateScore = [](const Array<Die>& dices) -> int
			{
				return dices.filter([](const Die& d) { return d.value == 4; }).size() * 4;
			};

		return cat;
	}

	inline Category Fives()
	{
		Category cat;
		cat.name = U"ファイブ";
		cat.rarity = Rarity::Common;
		cat.cost = 100;
		cat.description = U"5の目の合計点数が得点となる。";
		cat.textureKey = U"Fives";

		cat.type = CategoryType::Upper;
		cat.calculateScore = [](const Array<Die>& dices) -> int
			{
				return dices.filter([](const Die& d) { return d.value == 5; }).size() * 5;
			};

		return cat;
	}

	inline Category Sixes()
	{
		Category cat;
		cat.name = U"シックス";
		cat.rarity = Rarity::Common;
		cat.cost = 100;
		cat.description = U"6の目の合計点数が得点となる。";
		cat.textureKey = U"Sixes";

		cat.type = CategoryType::Upper;
		cat.calculateScore = [](const Array<Die>& dices) -> int
			{
				return dices.filter([](const Die& d) { return d.value == 6; }).size() * 6;
			};

		return cat;
	}

	inline Category Sevens()
	{
		Category cat;
		cat.name = U"セブン";
		cat.rarity = Rarity::Rare;
		cat.cost = 200;
		cat.description = U"7の目の合計点数が得点となる。";
		cat.textureKey = U"Sevens";

		cat.type = CategoryType::Upper;
		cat.calculateScore = [](const Array<Die>& dices) -> int
			{
				return dices.filter([](const Die& d) { return d.value == 7; }).size() * 7;
			};

		return cat;
	}

	inline Category Eights()
	{
		Category cat;
		cat.name = U"エイト";
		cat.rarity = Rarity::Rare;
		cat.cost = 200;
		cat.description = U"8の目の合計点数が得点となる。";
		cat.textureKey = U"Eights";

		cat.type = CategoryType::Upper;
		cat.calculateScore = [](const Array<Die>& dices) -> int
			{
				return dices.filter([](const Die& d) { return d.value == 8; }).size() * 8;
			};

		return cat;
	}

	inline Category Nines()
	{
		Category cat;
		cat.name = U"ナイン";
		cat.rarity = Rarity::Rare;
		cat.cost = 200;
		cat.description = U"9の目の合計点数が得点となる。";
		cat.textureKey = U"Nines";

		cat.type = CategoryType::Upper;
		cat.calculateScore = [](const Array<Die>& dices) -> int
			{
				return dices.filter([](const Die& d) { return d.value == 9; }).size() * 9;
			};

		return cat;
	}

	inline Category Yatzy()
	{
		Category cat;
		cat.name = U"ヤッツィー";
		cat.rarity = Rarity::Epic;
		cat.cost = 400;
		cat.description = U"5つ以上のダイスの目が同じなら50点。";
		cat.textureKey = U"Yatzy";

		cat.type = CategoryType::Lower;
		cat.calculateScore = [](const Array<Die>& dices) -> int
			{
				HashTable<int, int> counts;
				for (const auto& dice : dices)
				{
					const auto value = dice.value;
					if (not value) return 0;
					counts[value.value()]++;
				}

				for (const auto& count : counts)
				{
					if (count.second >= 5) return 50;
				}
				return 0;
			};

		return cat;
	}

	inline Category ThreeCards()
	{
		Category cat;
		cat.name = U"スリーダイス";
		cat.rarity = Rarity::Rare;
		cat.cost = 200;
		cat.description = U"3つ以上同じ目がある場合、全ダイスの合計点数が得点となる。";
		cat.textureKey = U"ThreeDice";

		cat.type = CategoryType::Lower;
		cat.calculateScore = [](const Array<Die>& dices) -> int
			{
				// 各目の出現回数をカウント
				HashTable<int, int> counts;
				for (const auto& dice : dices)
				{
					const auto value = dice.value;
					if (not value) return 0;
					counts[value.value()]++;
				}

				// 3つ以上同じ目があるかチェック
				bool hasThreeOfKind = std::any_of(counts.begin(), counts.end(),
					[](const auto& kv) { return kv.second >= 3; });

				// 3カードが成立する場合、全ダイスの合計を返す
				if (hasThreeOfKind)
				{
					int sum = 0;
					for (const auto& dice : dices)
					{
						const auto value = dice.value;
						if (value) sum += value.value();
					}
					return sum;
				}

				return 0;
			};

		return cat;
	}

	inline Category FourCards()
	{
		Category cat;
		cat.name = U"フォーダイス";
		cat.rarity = Rarity::Rare;
		cat.cost = 200;
		cat.description = U"4つ以上同じ目がある場合、全ダイスの合計点数が得点となる。";
		cat.textureKey = U"FourDice";

		cat.type = CategoryType::Lower;
		cat.calculateScore = [](const Array<Die>& dices) -> int
			{
				HashTable<int, int> counts;
				for (const auto& dice : dices)
				{
					const auto value = dice.value;
					if (not value) return 0;
					counts[value.value()]++;
				}

				bool hasFourOfKind = std::any_of(counts.begin(), counts.end(),
					[](const auto& kv) { return kv.second >= 4; });

				// 4カードが成立する場合、全ダイスの合計を返す
				if (hasFourOfKind)
				{
					int sum = 0;
					for (const auto& dice : dices)
					{
						const auto value = dice.value;
						if (value) sum += value.value();
					}
					return sum;
				}

				return 0;
			};

		return cat;
	}

	inline Category FullHouse()
	{
		Category cat;
		cat.name = U"フルハウス";
		cat.rarity = Rarity::Rare;
		cat.cost = 200;
		cat.description = U"3つ同じ目と2つ同じ目があると25点";
		cat.textureKey = U"FullHouse";

		cat.type = CategoryType::Lower;
		cat.calculateScore = [](const Array<Die>& dices) -> int
			{
				HashTable<int, int> counts;
				for (const auto& dice : dices)
				{
					const auto value = dice.value;
					if (not value) return 0;
					counts[value.value()]++;
				}
				bool hasThree = false;
				bool hasTwo = false;
				for (const auto& count : counts)
				{
					if (count.second >= 5) {
						hasThree = true;
						hasTwo = true;
						break;
					}
					if (count.second >= 3) {
						if (hasThree) hasTwo = true;
						else hasThree = true;
					}
					else if (count.second == 2) hasTwo = true;
				}

				return (hasThree && hasTwo) ? 25 : 0;
			};

		return cat;
	}

	inline Category SmallStraight()
	{
		Category cat;
		cat.name = U"スモールストレート";
		cat.rarity = Rarity::Rare;
		cat.cost = 200;
		cat.description = U"4つ連続した目があると30点";
		cat.textureKey = U"SmallStraight";

		cat.type = CategoryType::Lower;
		cat.calculateScore = [](const Array<Die>& dices) -> int
			{
				// ハッシュセットで重複を消す
				HashSet<int> set;

				for (const auto& dice : dices)
				{
					const auto value = dice.value;
					if (not value) return 0;
					set.emplace(value.value());
				}

				Array<int> a(set.begin(), set.end());
				a.sort();

				// 連続になった長さを計算 4以上ならSストレート成立
				int best = 0, current = 0;
				Optional<int> prev = none;
				for (int v : a)
				{
					if (!prev.has_value() || v != prev.value() + 1) current = 1;
					else current += 1;
					best = Max(best, current);
					prev = v;
				}

				return (best >= 4) ? 30 : 0;
			};

		return cat;
	}

	inline Category LargeStraight()
	{
		Category cat;
		cat.name = U"ラージストレート";
		cat.rarity = Rarity::Rare;
		cat.cost = 200;
		cat.description = U"5つ連続した目があると40点";
		cat.textureKey = U"LargeStraight";

		cat.type = CategoryType::Lower;
		cat.calculateScore = [](const Array<Die>& dices) -> int
			{
				HashSet<int> set;

				for (const auto& dice : dices)
				{
					const auto value = dice.value;
					if (not value) return 0;
					set.emplace(value.value());
				}

				Array<int> a(set.begin(), set.end());
				a.sort();

				// 連続になった長さを計算 4以上ならSストレート成立
				int best = 0, current = 0;
				Optional<int> prev = none;
				for (int v : a)
				{
					if (!prev.has_value() || v != prev.value() + 1) current = 1;
					else current += 1;
					best = Max(best, current);
					prev = v;
				}

				return (best >= 5) ? 40 : 0;
			};

		return cat;
	}

	inline Category Chance()
	{
		Category cat;
		cat.name = U"チャンス";
		cat.rarity = Rarity::Epic;
		cat.cost = 350;
		cat.description = U"出た目の合計点数が得点となる。";
		cat.textureKey = U"Chance";

		cat.type = CategoryType::Lower;
		cat.calculateScore = [](const Array<Die>& dices) -> int
			{
				int sum = 0;
				for (const auto& dice : dices)
				{
					const auto value = dice.value;
					if (not value) return 0;
					sum += value.value();
				}
				return sum;
			};

		return cat;
	}

	inline Category Hearts()
	{
		Category cat;
		cat.name = U"一心同体";
		cat.rarity = Rarity::Epic;
		cat.cost = 250;
		cat.description = U"心ダイスの出た目の合計点数が得点となる。";
		cat.textureKey = U"Hearts";

		cat.type = CategoryType::Lower;
		cat.calculateScore = [](const Array<Die>& dices) -> int
			{
				int sum = 0;
				for (const auto& dice : dices)
				{
					const auto value = dice.value;
					if (not value) return 0;
					if (dice.name == U"心ダイス") sum += value.value();
				}
				return sum;
			};

		return cat;
	}

	inline Category Exchange();  // 等価交換

	inline Category CashOut();  // 換金（スコア0でゴールド獲得）

	const Array<Category> AllCategories{
		Ones(),
		Twos(),
		Threes(),
		Fours(),
		Fives(),
		Sixes(),
		Sevens(),
		Eights(),
		Nines(),
		ThreeCards(),
		FourCards(),
		FullHouse(),
		SmallStraight(),
		LargeStraight(),
		Chance(),
		Yatzy(),
		Hearts(),
		Exchange(),
		CashOut(),
	};
	const Array<Category> UpperCategories{
		Ones(),
		Twos(),
		Threes(),
		Fours(),
		Fives(),
		Sixes(),
		Sevens(),
		Eights(),
		Nines(),
	};

	const Array<Category> DefaultUpperCategories{
		Ones(),
		Twos(),
		Threes(),
		Fours(),
		Fives(),
		Sixes(),
	};

	const Array<Category> LowerCategories{
		ThreeCards(),
		FourCards(),
		FullHouse(),
		SmallStraight(),
		LargeStraight(),
		Chance(),
		Yatzy(),
		Hearts(),
		Exchange(),
		CashOut(),
	};

	const Array<Category> DefaultLowerCategories{
		Exchange(),
		CashOut(),
		FullHouse(),
		SmallStraight(),
		LargeStraight(),
		Chance(),
		Yatzy()
	};

	const int UpperSectionBonusThreshold = 63;
	const int UpperSectionBonusScore = 35;
}
