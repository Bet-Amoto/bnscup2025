#pragma once
#include "Siv3D.hpp"
#include "Rarity.hpp"
#include "ItemBase.hpp"

struct Die;

enum class RollOrder {
	PRIMARY = 10,
	MID = 20,
	FINAL = 30
};


using RollFunc = std::function<int(const Die&, const Array<Die>&)>;
using DrawFunc = std::function<void(const Vec2&, const Die)>;

struct Die : ItemBase
{
	/// @brief 面
	Array<int> faces;
	Optional<int> value = none;
	RollOrder order = RollOrder::PRIMARY;
	RollFunc rollFunc;
	DrawFunc drawFunc;

	bool locked = false;

	String itemType() const override { return U"ダイス"; }

	bool apply(Status& s) override { return true; }

	std::shared_ptr<ItemBase> clone() const override
	{
		return std::make_shared<Die>(*this);
	}

	void drawIcon(const Vec2& pos) const override
	{
		//仮
		Die die = *this;
		die.value = 1;
		if (die.faces.isEmpty())die.value = 1;
		else die.value = die.faces.sorted().front();
		drawFunc(pos, die);
	}

	void roll(const Array<Die>& dices)
	{
		if (locked) return;
		value = rollFunc(*this, dices);
	}
	void clear()
	{
		value = none;
		locked = false;
	}
	void draw(const Vec2& centerPos) const
	{
		drawFunc(centerPos, *this);
	}
	void setMinVal() {
		if (faces.isEmpty())value = 1;
		else value = faces.sorted().front();
	}
};

namespace Dice{
	/// @brief 通常のダイス
	inline Die StandardDie()
	{
		Die d;
		d.name = U"新ダイス";
		d.rarity = Rarity::Common;
		d.cost = 20;
		d.description = U"新品のダイス。1～6の普通のダイス。";
		d.textureKey = U"StandardDice";

		d.faces = Array<int>{ 1, 2, 3, 4, 5, 6 };
		d.value = none;
		d.order = RollOrder::PRIMARY;
		d.rollFunc = [](const Die& self, const Array<Die>& dices) { return self.faces.choice(); };
		d.drawFunc = [](const Vec2& centerPos, const Die self)
			{
				RectF faceRect(centerPos.x - 30, centerPos.y - 30, 60, 60);
				faceRect.rounded(3).draw((self.locked || !self.value) ? ColorF{ 0.6 } : ColorF{ 1.0 });
				faceRect.rounded(3).drawFrame(1, ColorF{ 0 });
				if (self.value)FontAsset(U"Bold")(Format(self.value.value())).drawAt(faceRect.center(), ColorF{ 0.1 });
			};

		return d;
	}

	inline Die Coin()
	{
		Die d;
		d.name = U"コイン";
		d.rarity = Rarity::Common;
		d.cost = 50;
		d.description = U"二分の一の確率で1か6が出るコイン。";
		d.textureKey = U"Coin";

		d.faces = Array<int>{ 1, 6 };
		d.value = none;
		d.order = RollOrder::PRIMARY;
		d.rollFunc = [](const Die& self, const Array<Die>& dices) { return self.faces.choice(); };
		d.drawFunc = [](const Vec2& centerPos, const Die self)
			{
				Circle faceCircle(centerPos, 30);
				faceCircle.draw((self.locked || !self.value) ? HSV(54, 0.77, 0.8) : HSV(54, 0.77, 1));
				faceCircle.drawFrame(1, ColorF{ 0 });
				if (self.value)FontAsset(U"Bold")(Format(self.value.value())).drawAt(faceCircle.center, ColorF{ 0.1 });
			};

		return d;
	}

	const Array<Die> AllDice = {
		StandardDie(),
		Coin(),
	};
}
