#pragma once
#include "Siv3D.hpp"

enum class Rarity {
	Common,
	Rare,
	Epic,
	Legendary
};

[[nodiscard]] inline String RarityToString(Rarity r) {
	switch (r) {
	case Rarity::Common:    return U"Common";
	case Rarity::Rare:      return U"Rare";
	case Rarity::Epic:      return U"Epic";
	case Rarity::Legendary: return U"Legendary";
	}
	return U"?";
}

[[nodiscard]] inline ColorF RarityToColor(Rarity r) {
	switch (r) {
	case Rarity::Common:    return ColorF{ 0.85 };
	case Rarity::Rare:      return ColorF{ 0.6, 0.8, 1.0 };
	case Rarity::Epic:      return ColorF{ 0.8, 0.6, 1.0 };
	case Rarity::Legendary: return ColorF{ 1.0, 0.85, 0.4 };
	}
	return ColorF{ 0.5 };
}

const Array<Rarity> AllRarities{ Rarity::Common, Rarity::Rare, Rarity::Epic, Rarity::Legendary };

inline Rarity pickRarity(int turn)
{
	DiscreteDistribution dist{ // レアリティごとの出現確率
	{
		0.64, // Common
		0.24, // Rare
		0.10, // Epic
		0.02  // Legendary
	} };

	return DiscreteSample(AllRarities, dist);
}
