#pragma once
#include "Siv3D.hpp"

struct Die;

enum class RollOrder {
	PRIMARY = 10,
	MID = 20,
	FINAL = 30
};

using RollFunc = std::function<int(const Array<Die>&)>;
using DrawFunc = std::function<void(const Vec2&, const Die)>;

struct Die
{
	Optional<int> value = none;
	RollOrder order = RollOrder::PRIMARY;
	RollFunc rollFunc;
	DrawFunc drawFunc;
	bool locked = false;
	void roll(const Array<Die>& dices)
	{
		if (locked) return;
		value = rollFunc(dices);
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
};

namespace Dice{
	const Die StandardDie{
		none,
		RollOrder::PRIMARY,
		[](const Array<Die>& dices) -> int
		{
			return Random(1, 6);
		},
		[](const Vec2& centerPos, const Die self)
		{
			RectF faceRect(centerPos.x - 30, centerPos.y - 30, 60, 60);
			faceRect.draw((self.locked || !self.value) ? ColorF{ 0.6 } : ColorF{ 1.0 });
			faceRect.drawFrame(1, ColorF{ 0 });
			if (self.value)FontAsset(U"Bold")(Format(self.value.value())).drawAt(faceRect.center(), ColorF{ 0.1 });
		}
	};

	const Die Coin{
		none,
		RollOrder::PRIMARY,
		[](const Array<Die>& dices) -> int
		{
			return RandomBool() ? 1 : 6;
		},
		[](const Vec2& centerPos, const Die self)
		{
			Circle faceCircle(centerPos, 30);
			faceCircle.draw(ColorF{ 1.0 });
			faceCircle.drawFrame(1, ColorF{ 0 });
		}
	};
}
