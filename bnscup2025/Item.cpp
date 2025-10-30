#include "Item.hpp"
#include "Status.hpp"
namespace Items
{
	Artifact TrueOne()
	{
		Artifact item;
		item.name = U"真・エース";
		item.rarity = Rarity::Legendary;
		item.cost = 500;
		item.description = U"エースの役のスコアが10倍になる";
		item.timing = ActivationTiming::OnDiceResult;
		item.usageLimit = none;

		item.activateFunc = [](Artifact& self, ActivationTiming timing, Status& status)
			{
				if (timing != self.timing || !status.gameStats.lastAchievedCategory) {
					return;
				}
				if (status.gameStats.lastAchievedCategory->name != U"エース") {
					return;
				}
				if (status.gameStats.lastAchievedCategory->score.has_value()) {
					status.gameStats.lastAchievedCategory->score.value() *= 10;
				}
			};

		item.conditionFunc = [](const Artifact& self, ActivationTiming timing, const Status& status) {
			if (timing != self.timing || !status.gameStats.lastAchievedCategory) {
				return false;
			}
			return status.gameStats.lastAchievedCategory->name == U"エース";
			};

		item.drawFunc = [](const Vec2& pos, const Artifact& self)
			{
				RoundRect{ pos, 60, 60, 5 }.draw(HSV(40, 0.9, 1.0));
				SimpleGUI::GetFont()(U"⭐").drawAt(pos.x + 30, pos.y + 30);
			};

		return item;
	}
}
