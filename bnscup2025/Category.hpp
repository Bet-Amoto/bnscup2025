# pragma once
#include "Common.hpp"
#include "Dice.hpp"

using scoreFunc = std::function<int(const Array<Dice>&)>;

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
		[](const Array<Dice>& dices) -> int
		{
			return dices.filter([](const Dice& d) { return d.getValue() == 1; }).size() * 1;
		}
	};
	const Category Twos{
		U"デュース",
		[](const Array<Dice>& dices) -> int
		{
			return dices.filter([](const Dice& d) { return d.getValue() == 2; }).size() * 2;
		}
	};
	const Category Threes{
		U"トレイ",
		[](const Array<Dice>& dices) -> int
		{
			return dices.filter([](const Dice& d) { return d.getValue() == 3; }).size() * 3;
		}
	};
	const Category Fours{
		U"フォー",
		[](const Array<Dice>& dices) -> int
		{
			return dices.filter([](const Dice& d) { return d.getValue() == 4; }).size() * 4;
		}
	};
	const Category Fives{
		U"ファイブ",
		[](const Array<Dice>& dices) -> int
		{
			return dices.filter([](const Dice& d) { return d.getValue() == 5; }).size() * 5;
		}
	};
	const Category Sixes{
		U"シックス",
		[](const Array<Dice>& dices) -> int
		{
			return dices.filter([](const Dice& d) { return d.getValue() == 6; }).size() * 6;
		}
	};
	const Category Yatzy{
		U"ヤッツィー",
		[](const Array<Dice>& dices) -> int
		{
			const auto firstValue = dices[0].getValue();
			if (not firstValue) return 0;
			bool allSame = dices.all([firstValue](const Dice& d) { return d.getValue() == firstValue; });
			return allSame ? 50 : 0;
		}
	};
	const Category threeCards{
		U"スリーダイス",
		[](const Array<Dice>& dices) -> int
		{
			// 各目の出現回数をカウント
			std::array<int, 6> counts = {};
			for (const auto& dice : dices)
			{
				const auto value = dice.getValue();
				if (not value) return 0;
				counts[value.value() - 1]++;
			}

			// 3つ以上同じ目があるかチェック
			bool hasThreeOfKind = std::any_of(counts.begin(), counts.end(),
				[](int count) { return count >= 3; });

			// 3カードが成立する場合、全ダイスの合計を返す
			if (hasThreeOfKind)
			{
				int sum = 0;
				for (const auto& dice : dices)
				{
					const auto value = dice.getValue();
					if (value) sum += value.value();
				}
				return sum;
			}

			return 0;
		}
	};
	const Category fourCards{
		U"フォーダイス",
		[](const Array<Dice>& dices) -> int
		{
			std::array<int, 6> counts = {};
			for (const auto& dice : dices)
			{
				const auto value = dice.getValue();
				if (not value) return 0;
				counts[value.value() - 1]++;
			}
			bool hasThreeOfKind = std::any_of(counts.begin(), counts.end(),
				[](int count) { return count >= 4; });

			// 3カードが成立する場合、全ダイスの合計を返す
			if (hasThreeOfKind)
			{
				int sum = 0;
				for (const auto& dice : dices)
				{
					const auto value = dice.getValue();
					if (value) sum += value.value();
				}
				return sum;
			}

			return 0;
		}
	};
	const Category FullHouse{
		U"フルハウス",
		[](const Array<Dice>& dices) -> int
		{
			std::array<int, 6> counts = {};
			for (const auto& dice : dices)
			{
				const auto value = dice.getValue();
				if (not value) return 0;
				counts[value.value() - 1]++;
			}
			bool hasThree = false;
			bool hasTwo = false;
			for (const auto count : counts)
			{
				if (count == 3) hasThree = true;
				if (count == 2) hasTwo = true;
			}
			if (hasThree && hasTwo)
			{
				int sum = 0;
				for (const auto& dice : dices)
				{
					const auto value = dice.getValue();
					if (value) sum += value.value();
				}
				return sum;
			}
			return 0;
		}
	};
	const Category SmallStraight{
		U"スモールストレート",
		[](const Array<Dice>& dices) -> int
		{
			std::array<bool, 6> present = {};
			for (const auto& dice : dices)
			{
				const auto value = dice.getValue();
				if (not value) return 0;
				present[value.value() - 1] = true;
			}
			if (   (present[0] && present[1] && present[2] && present[3])
				|| (present[1] && present[2] && present[3] && present[4])
				|| (present[2] && present[3] && present[4] && present[5]))
			{
				return 30;
			}
			return 0;
		}
	};
	const Category LargeStraight{
		U"ラージストレート",
		[](const Array<Dice>& dices) -> int
		{
			std::array<bool, 6> present = {};
			for (const auto& dice : dices)
			{
				const auto value = dice.getValue();
				if (not value) return 0;
				present[value.value() - 1] = true;
			}
			if (   (present[0] && present[1] && present[2] && present[3] && present[4])
				|| (present[1] && present[2] && present[3] && present[4] && present[5]))
			{
				return 40;
			}
			return 0;
		}
	};
	const Category Chance{
		U"チャンス",
		[](const Array<Dice>& dices) -> int
		{
			int sum = 0;
			for (const auto& dice : dices)
			{
				const auto value = dice.getValue();
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
