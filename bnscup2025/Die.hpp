#pragma once
#include "Siv3D.hpp"
#include "Rarity.hpp"

struct Die;

enum class RollOrder {
	PRIMARY = 10,
	MID = 20,
	FINAL = 30
};


using RollFunc = std::function<int(const Die&, const Array<Die>&)>;
using DrawFunc = std::function<void(const Vec2&, const Die)>;

struct Die
{
	/// @brief 面
	Array<int> faces;
	Optional<int> value = none;
	RollOrder order = RollOrder::PRIMARY;
	RollFunc rollFunc;
	DrawFunc drawFunc;

	String name = U"";
	Rarity rarity = Rarity::Common;
	int cost = 0;
	String discription = U"";
	String textureKey = U"";

	bool locked = false;
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
};

namespace Dice{
	/// @brief 通常のダイス
	const Die StandardDie{
		Array<int>{ 1, 2, 3, 4, 5, 6 },
		none,
		RollOrder::PRIMARY,
		[](const Die& self, const Array<Die>& dices) -> int
		{
			return self.faces.choice();
		},
		[](const Vec2& centerPos, const Die self)
		{
			RectF faceRect(centerPos.x - 30, centerPos.y - 30, 60, 60);
			faceRect.rounded(3).draw((self.locked || !self.value) ? ColorF{0.6} : ColorF{1.0});
			faceRect.rounded(3).drawFrame(1, ColorF{ 0 });
			if (self.value)FontAsset(U"Bold")(Format(self.value.value())).drawAt(faceRect.center(), ColorF{ 0.1 });
		},
		U"普通のダイス",
		Rarity::Common,
		20,
		U"普通のダイス。出目に偽りなし、1～6のまっすぐな真。",
		U"StandardDice"
	};

	/// @brief 偶数ダイス
	const Die EvenDie{
		Array<int>{ 2, 2, 4, 4, 6, 6 },
		none,
		RollOrder::PRIMARY,
		[](const Die& self, const Array<Die>& dices) -> int
		{
			return self.faces.choice();
		},
		[](const Vec2& centerPos, const Die self)
		{
			RectF faceRect(centerPos.x - 30, centerPos.y - 30, 60, 60);
			faceRect.rounded(3).draw((self.locked || !self.value) ? HSV(180, 0.12, 0.8) : HSV(180, 0.12, 1));
			faceRect.rounded(3).drawFrame(1, ColorF{ 0 });
			if (self.value)FontAsset(U"Bold")(Format(self.value.value())).drawAt(faceRect.center(), ColorF{ 0.1 });
		},
		U"偶数ダイス",
		Rarity::Common,
		50,
		U"偶数のみで構成されたダイス。普通のダイスより出目がちょっと高い。",
		U"EvenDice"
	};

	/// @brief 奇数ダイス
	const Die OddDie{
		Array<int>{ 1, 1, 3, 3, 5, 5 },
		none,
		RollOrder::PRIMARY,
		[](const Die& self, const Array<Die>& dices) -> int
		{
			return self.faces.choice();
		},
		[](const Vec2& centerPos, const Die self)
		{
			RectF faceRect(centerPos.x - 30, centerPos.y - 30, 60, 60);
			faceRect.rounded(3).draw((self.locked || !self.value) ? HSV(0, 0.12, 0.8) : HSV(0, 0.12, 1));
			faceRect.rounded(3).drawFrame(1, ColorF{ 0 });
			if (self.value)FontAsset(U"Bold")(Format(self.value.value())).drawAt(faceRect.center(), ColorF{ 0.1 });
		},
		U"奇数ダイス",
		Rarity::Common,
		50,
		U"奇数のみで構成されたダイス。普通のダイスより出目がちょっと低い。",
		U"OddDice"
	};

	/// @brief コインダイス 二分の一の確率で1か6が出る
	const Die Coin{
		Array<int>{ 1, 6 },
		none,
		RollOrder::PRIMARY,
		[](const Die& self, const Array<Die>& dices) -> int
		{
			return self.faces.choice();
		},
		[](const Vec2& centerPos, const Die self)
		{
			Circle faceCircle(centerPos, 30);
			faceCircle.draw((self.locked || !self.value) ? HSV(54, 0.77, 0.8) : HSV(54, 0.77, 1));
			faceCircle.drawFrame(1, ColorF{ 0 });
			if (self.value)FontAsset(U"Bold")(Format(self.value.value())).drawAt(faceCircle.center, ColorF{ 0.1 });
		},
		U"コイン",
		Rarity::Common,
		50,
		U"二分の一の確率で1か6が出るコイン。真か偽か、常に見極める必要がある。",
		U"Coin"
	};

	/// @brief コインダイス 二分の一の確率で1か8が出る
	const Die HighCoin{
		Array<int>{ 1, 8 },
		none,
		RollOrder::PRIMARY,
		[](const Die& self, const Array<Die>& dices) -> int
		{
			return self.faces.choice();
		},
		[](const Vec2& centerPos, const Die self)
		{
			Circle faceCircle(centerPos, 30);
			faceCircle.draw((self.locked || !self.value) ? HSV(38, 1, 0.8) : HSV(38, 1, 1));
			faceCircle.drawFrame(1, ColorF{ 0 });
			if (self.value)FontAsset(U"Bold")(Format(self.value.value())).drawAt(faceCircle.center, ColorF{ 0.1 });
		},
		U"ハイコイン",
		Rarity::Rare,
		100,
		U"少し出目が高くなったコイン。一か八か、利用者の真価が問われる。",
		U"HighCoin"
	};


}
