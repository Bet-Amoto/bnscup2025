# pragma once
#include "Siv3d.hpp"
#include "Die.hpp"

using scoreFunc = std::function<int(const Array<Die>&)>;

enum class CategoryType
{
	Upper,
	Lower
};

struct Category
{
	String name = U"";
	String discription = U"";
	CategoryType type = CategoryType::Upper;
	Rarity rarity = Rarity::Common;
	int cost = 0;
	scoreFunc calculateScore;

	bool operator==(const Category& rhs) const
	{
		return name == rhs.name;
	}
};

namespace Categories
{
	const Category Ones{
		U"エース",
		U"1の目の合計点数が得点となる。",
		CategoryType::Upper,
		Rarity::Common,
		100,
		[](const Array<Die>& dices) -> int
		{
			return dices.filter([](const Die& d) { return d.value == 1; }).size() * 1;
		}
	};
	const Category Twos{
		U"デュース",
		U"2の目の合計点数が得点となる。",
		CategoryType::Upper,
		Rarity::Common,
		100,
		[](const Array<Die>& dices) -> int
		{
			return dices.filter([](const Die& d) { return d.value == 2; }).size() * 2;
		}
	};
	const Category Threes{
		U"トレイ",
		U"3の目の合計点数が得点となる。",
		CategoryType::Upper,
		Rarity::Common,
		100,
		[](const Array<Die>& dices) -> int
		{
			return dices.filter([](const Die& d) { return d.value == 3; }).size() * 3;
		}
	};
	const Category Fours{
		U"フォー",
		U"4の目の合計点数が得点となる。",
		CategoryType::Upper,
		Rarity::Common,
		100,
		[](const Array<Die>& dices) -> int
		{
			return dices.filter([](const Die& d) { return d.value == 4; }).size() * 4;
		}
	};
	const Category Fives{
		U"ファイブ",
		U"5の目の合計点数が得点となる。",
		CategoryType::Upper,
		Rarity::Common,
		100,
		[](const Array<Die>& dices) -> int
		{
			return dices.filter([](const Die& d) { return d.value == 5; }).size() * 5;
		}
	};
	const Category Sixes{
		U"シックス",
		U"6の目の合計点数が得点となる。",
		CategoryType::Upper,
		Rarity::Common,
		100,
		[](const Array<Die>& dices) -> int
		{
			return dices.filter([](const Die& d) { return d.value == 6; }).size() * 6;
		}
	};
	const Category Sevens{
		U"セブン",
		U"7の目の合計点数が得点となる。",
		CategoryType::Upper,
		Rarity::Rare,
		200,
		[](const Array<Die>& dices) -> int
		{
			return dices.filter([](const Die& d) { return d.value == 7; }).size() * 7;
		}
	};
	const Category Eights{
		U"エイト",
		U"8の目の合計点数が得点となる。",
		CategoryType::Upper,
		Rarity::Rare,
		200,
		[](const Array<Die>& dices) -> int
		{
			return dices.filter([](const Die& d) { return d.value == 8; }).size() * 8;
		}
	};
	const Category Nines{
		U"ナイン",
		U"9の目の合計点数が得点となる。",
		CategoryType::Upper,
		Rarity::Rare,
		200,
		[](const Array<Die>& dices) -> int
		{
			return dices.filter([](const Die& d) { return d.value == 9; }).size() * 9;
		}
	};
	const Category Yatzy{
		U"ヤッツィー",
		U"全てのダイスの目が同じなら50点。",
		CategoryType::Lower,
		Rarity::Epic,
		400, 
		[](const Array<Die>& dices) -> int
		{
			const auto firstValue = dices[0].value;
			if (not firstValue) return 0;
			bool allSame = dices.all([firstValue](const Die& d) { return d.value == firstValue; });
			return allSame ? 50 : 0;
		}
	};
	const Category threeCards{
		U"スリーダイス",
		U"3つ以上同じ目がある場合、全ダイスの合計点数が得点となる。",
		CategoryType::Lower,
		Rarity::Rare,
		200,
		[](const Array<Die>& dices) -> int
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
		}
	};
	const Category fourCards{
		U"フォーダイス",
		U"4つ以上同じ目がある場合、全ダイスの合計点数が得点となる。",
		CategoryType::Lower,
		Rarity::Rare,
		200,
		[](const Array<Die>& dices) -> int
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
		}
	};
	const Category FullHouse{
		U"フルハウス",
		U"3つ同じ目と2つ同じ目があると25点",
		CategoryType::Lower,
		Rarity::Rare,
		200,
		[](const Array<Die>& dices) -> int
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
				if (count.second == 3) hasThree = true;
				else if (count.second == 2) hasTwo = true;
			}

			return (hasThree && hasTwo) ? 25 : 0;
		}
	};
	const Category SmallStraight{
		U"スモールストレート",
		U"4つ連続した目があると30点",
		CategoryType::Lower,
		Rarity::Rare,
		200,
		[](const Array<Die>& dices) -> int
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
		}
	};
	const Category LargeStraight{
		U"ラージストレート",
		U"5つ連続した目があると40点",
		CategoryType::Lower,
		Rarity::Epic,
		300,
		[](const Array<Die>& dices) -> int
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
		}
	};
	const Category Chance{
		U"チャンス",
		U"出た目の合計点数が得点となる。",
		CategoryType::Lower,
		Rarity::Rare,
		200,
		[](const Array<Die>& dices) -> int
		{
			int sum = 0;
			for (const auto& dice : dices)
			{
				const auto value = dice.value;
				if (not value) return 0;
				sum += value.value();
			}
			return sum;
		}
	};

	const Array<Category> AllCategories{
		Ones,
		Twos,
		Threes,
		Fours,
		Fives,
		Sixes,
		Sevens,
		Eights,
		Nines,
		threeCards,
		fourCards,
		FullHouse,
		SmallStraight,
		LargeStraight,
		Chance,
		Yatzy
	};
	const Array<Category> UpperCategories{
		Ones,
		Twos,
		Threes,
		Fours,
		Fives,
		Sixes,
		Sevens,
		Eights,
		Nines,
	};
	const Array<Category> LowerCategories{
		threeCards,
		fourCards,
		FullHouse,
		SmallStraight,
		LargeStraight,
		Chance,
		Yatzy
	};

	const int UpperSectionBonusThreshold = 63;
	const int UpperSectionBonusScore = 35;
}
