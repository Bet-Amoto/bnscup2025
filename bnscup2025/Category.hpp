# pragma once
#include "Siv3d.hpp"
#include "Die.hpp"

using scoreFunc = std::function<int(const Array<Die>&)>;

struct Category
{
	String name;
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
		[](const Array<Die>& dices) -> int
		{
			return dices.filter([](const Die& d) { return d.value == 1; }).size() * 1;
		}
	};
	const Category Twos{
		U"デュース",
		[](const Array<Die>& dices) -> int
		{
			return dices.filter([](const Die& d) { return d.value == 2; }).size() * 2;
		}
	};
	const Category Threes{
		U"トレイ",
		[](const Array<Die>& dices) -> int
		{
			return dices.filter([](const Die& d) { return d.value == 3; }).size() * 3;
		}
	};
	const Category Fours{
		U"フォー",
		[](const Array<Die>& dices) -> int
		{
			return dices.filter([](const Die& d) { return d.value == 4; }).size() * 4;
		}
	};
	const Category Fives{
		U"ファイブ",
		[](const Array<Die>& dices) -> int
		{
			return dices.filter([](const Die& d) { return d.value == 5; }).size() * 5;
		}
	};
	const Category Sixes{
		U"シックス",
		[](const Array<Die>& dices) -> int
		{
			return dices.filter([](const Die& d) { return d.value == 6; }).size() * 6;
		}
	};
	const Category Yatzy{
		U"ヤッツィー",
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

			if (hasThree && hasTwo)
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
	const Category SmallStraight{
		U"スモールストレート",
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
		Sixes
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
