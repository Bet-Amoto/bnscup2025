#pragma once
#include <Siv3D.hpp>
#include "Rarity.hpp"

struct Status;

struct ItemBase
{
	String name = U"";
	Rarity rarity = Rarity::Common;
	int32 cost = 0;
	String description = U"";
	String textureKey = U"";

	virtual ~ItemBase() = default;

	// 種類(カテゴリー、ダイスなど)
	virtual String itemType() const = 0;

	// 購入時の適用効果
	virtual bool apply(Status& s) = 0;

	virtual std::shared_ptr<ItemBase> clone() const = 0;

	virtual void drawIcon(const Vec2& pos) const
	{

	}
};
