#pragma once
#include <Siv3D.hpp>
#include "ItemBase.hpp"
#include "Rarity.hpp"

enum class ActivationTiming
{
	OnPurchase,      // 購入時
	OnGameStart,     // ゲーム開始時
	OnTurnStart,     // ターン開始時
	OnTurnEnd,       // ターン終了時
	OnDiceRoll,      // サイコロを振る時
	OnDiceResult,    // サイコロの結果確定時
	OnGameEnd,       // ゲーム終了時
};

struct Item;
struct Status;

void addGold(Status& s, int32 amount);

using ActivateFunc = std::function<void(Item&, ActivationTiming, Status&)>;
using ConditionFunc = std::function<bool(const Item&, ActivationTiming, const Status&)>;
using ItemDrawFunc = std::function<void(const Vec2&, const Item&)>;

struct Item : ItemBase
{
	ActivationTiming timing = ActivationTiming::OnPurchase;
	ActivateFunc activateFunc = nullptr;
	ConditionFunc conditionFunc = [](const Item& self, ActivationTiming timing, const Status& status) {
		return timing == self.timing;
		};
	ItemDrawFunc drawFunc = nullptr;

	bool isActive = true; // アイテムが有効かどうか
	Optional<int32> usageLimit = none; // 使用回数制限。noneの場合は無制限

	String itemType() const override { return U"アイテム"; }

	bool apply(Status& s) override
	{
		if (timing == ActivationTiming::OnPurchase && activateFunc)
		{
			activateFunc(*this, ActivationTiming::OnPurchase, s);
			if (usageLimit)
			{
				usageLimit.value()--;
				if (usageLimit.value() <= 0) return false; 
			}
		}
		return true;
	}

	std::shared_ptr<ItemBase> clone() const override
	{
		return std::make_shared<Item>(*this);
	}

	void drawIcon(const Vec2& pos) const override
	{
		if (drawFunc)
		{
			drawFunc(pos, *this);
		}
		else
		{
			// デフォルト描画
			RoundRect{ pos, 60, 60, 5 }.draw(ColorF{ 0.8 });
			SimpleGUI::GetFont()(name).drawAt(pos.x + 30, pos.y + 30, ColorF{ 0.1 });
		}
	}

	void activate(ActivationTiming timing, Status& status)
	{
		if (activateFunc)
		{
			activateFunc(*this, timing, status);
			if (usageLimit)
			{
				usageLimit.value()--;
				if (usageLimit.value() <= 0) isActive = false;
			}
		}
	}

	bool condition(ActivationTiming timing, const Status& status) const
	{
		return conditionFunc(*this, timing, status);
	}
};

/// @brief 指定したタイミングで発動するアイテムを抽出する
/// @param items アイテムの配列
/// @param timing 発動タイミング
/// @return 指定したタイミングで発動するアイテムの配列
inline Array<Item*> FilterItemsByTiming(Array<Item>& items, ActivationTiming timing, const Status& status)
{
	Array<Item*> result;
	for (auto& item : items)
	{
		if (item.condition(timing, status))
		{
			result.push_back(&item);
		}
	}
	return result;
}

namespace Items
{
	/// @brief 金貨袋
	inline Item GoldPouch()
	{
		Item item;
		item.name = U"金貨袋";
		item.rarity = Rarity::Common;
		item.cost = 30;
		item.description = U"小さな金貨袋。購入時に50ゴールド獲得する。";
		item.textureKey = U"GoldPouch";
		item.timing = ActivationTiming::OnPurchase;
		item.usageLimit = 1; // 1回使い切り

		item.activateFunc = [](Item& self, ActivationTiming timing, Status& status)
			{
				addGold(status, 50);
			};

		item.drawFunc = [](const Vec2& pos, const Item& self)
			{
				RoundRect{ pos, 60, 60, 5 }.draw(HSV(54, 0.77, 0.9));
				SimpleGUI::GetFont()(U"💰").drawAt(pos.x + 30, pos.y + 30);
			};

		return item;
	}

	/// @brief ラッキーチャーム
	inline Item LuckyCharm()
	{
		Item item;
		item.name = U"幸運のお守り";
		item.rarity = Rarity::Epic;
		item.cost = 80;
		item.description = U"幸運を呼ぶお守り。ゲーム開始時にランダムでダイスの目を1つ+1する。";
		item.textureKey = U"LuckyCharm";
		item.timing = ActivationTiming::OnGameStart;
		item.usageLimit = none; // 無制限

		item.activateFunc = [](Item& self, ActivationTiming timing, Status& status)
			{
				// ゲーム開始時の処理（ダイスの補正など）
				// 実装はStatusの構造に依存
			};

		item.drawFunc = [](const Vec2& pos, const Item& self)
			{
				RoundRect{ pos, 60, 60, 5 }.draw(HSV(160, 0.5, 0.9));
				SimpleGUI::GetFont()(U"🍀").drawAt(pos.x + 30, pos.y + 30);
			};

		return item;
	}

	const Array<Item> AllItems = {
		GoldPouch(),
		LuckyCharm(),
	};
}
