#pragma once
#include <Siv3D.hpp>
#include "ItemBase.hpp"
#include "Rarity.hpp"

// 前方宣言
struct Category;
struct Die;

enum class ActivationTiming
{
	OnPurchase,      // 購入時
	OnTurnStart,     // ターン開始時
	OnTurnEnd,       // ターン終了時
	OnDiceRoll,      // サイコロを振る時
	OnDiceResult,    // サイコロの結果確定時
	OnScorePreview,  // スコアプレビュー時（予想スコア計算時）
};

struct Artifact;
struct Status;

void addGold(Status& s, int32 amount);

using ActivateFunc = std::function<void(Artifact&, ActivationTiming, Status&)>;
using ConditionFunc = std::function<bool(const Artifact&, ActivationTiming, const Status&)>;
using ItemDrawFunc = std::function<void(const Vec2&, const Artifact&)>;
// スコア修正用の関数型
using ScoreModifierFunc = std::function<int(int baseScore, const Category&, const Array<Die>&, const Status&)>;

struct Artifact : ItemBase
{
	ActivationTiming timing = ActivationTiming::OnPurchase;
	ActivateFunc activateFunc = nullptr;
	ConditionFunc conditionFunc = [](const Artifact& self, ActivationTiming timing, const Status& status) {
		return timing == self.timing;
		};
	ItemDrawFunc drawFunc = nullptr;
	ScoreModifierFunc scoreModifier;  // 予想スコア計算用の修正関数

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
		return std::make_shared<Artifact>(*this);
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

inline void ActivateArtifactsByTiming(Array<Artifact>& artifacts, ActivationTiming timing, Status& status)
{
	for (auto& item : artifacts)
	{
		if (item.condition(timing, status))
		{
			item.activate(timing, status);
		}
	}
}

namespace Items
{
	/// @brief 金貨袋
	inline Artifact GoldPouch()
	{
		Artifact item;
		item.name = U"金貨袋";
		item.rarity = Rarity::Common;
		item.cost = 30;
		item.description = U"小さな金貨袋。購入時に50ゴールド獲得する。";
		item.textureKey = U"GoldPouch";
		item.timing = ActivationTiming::OnPurchase;
		item.usageLimit = 1; // 1回使い切り

		item.activateFunc = [](Artifact& self, ActivationTiming timing, Status& status)
			{
				addGold(status, 50);
			};

		item.drawFunc = [](const Vec2& pos, const Artifact& self)
			{
				RoundRect{ pos, 60, 60, 5 }.draw(HSV(54, 0.77, 0.9));
				SimpleGUI::GetFont()(U"💰").drawAt(pos.x + 30, pos.y + 30);
			};

		return item;
	}

	/// @brief ラッキーチャーム
	inline Artifact LuckyCharm();

	/// @brief 真・エース 
	inline Artifact TrueOne();

	inline Artifact Siso();

	const Array<Artifact> AllItems = {
		GoldPouch(),
		LuckyCharm(),
		TrueOne(),
		Siso()
	};
}
