#pragma once
#include "Siv3D.hpp"

enum class Rarity {
	Common,
	Rare,
	Epic,
	Legendary
};

const Array<Rarity> AllRarities{ Rarity::Common, Rarity::Rare, Rarity::Epic, Rarity::Legendary };
