#include "Shop.hpp"

Shop::Shop(const InitData& init)
	: IScene{ init },
	m_status{ getData().status },
	m_diceBox{ Vec2{ 100, 300 }, m_status.dices }
{
	Scene::SetBackground(ColorF{ 0.9, 0.9, 0.8 });
	int marchX = 100;
	for(const auto i: step(m_status.ShopDiceCount))
	{
		const Rarity rarity = DiscreteSample(AllRarities, m_status.distribution);
		const Array<Die> filteredDices = m_status.availableDices.filter([&](const Die& d) { return d.rarity == rarity; });
		if (not filteredDices.empty())
		{
			const Die dice = filteredDices.choice();
			m_merchandises << Merchandise{ dice.clone(), Vec2{ marchX, 200 } };
			marchX += 150;
		}
	}
	for (const auto i : step(m_status.ShopCategoryCount))
	{
		const Rarity rarity = DiscreteSample(AllRarities, m_status.distribution);
		const Array<Category> filteredCategories = m_status.availableCategories.filter([&](const Category& c) { return c.rarity == rarity; });
		if (not filteredCategories.empty())
		{
			const Category category = filteredCategories.choice();
			m_merchandises << Merchandise{ category.clone(), Vec2{ marchX, 200 } };
			marchX += 150;
		}
	}
	for (auto& die : m_status.dices) {
		die.setMinVal();
	}
	m_status.gold = 10000;
}

void Shop::update()
{
	if (KeySpace.down())
	{
		changeScene(State::Game);
	}

	if(m_holdedItem)
	{
		const Transformer2D t{ Mat3x2::Identity(), Mat3x2::Translate(240,60) };
		const auto item = m_holdedItem->getItem();
		if(item && item->itemType() == U"ダイス")
		{
			m_diceBox.update();
			const auto clickedDie = m_diceBox.getClickedDie();
			if (clickedDie)
			{
				m_selected = true;
				for(auto& die : m_status.dices)
				{
					if(&die != clickedDie)
					{
						die.locked = true;
					}
					else {
						die.locked = false;
					}
				}
			}
			if(boughtButtonRect.leftClicked() && m_selected)
			{
				for (auto& die : m_status.dices)
				{
					if (!die.locked) {
						die = *std::dynamic_pointer_cast<Die>(item->clone());
						die.setMinVal();
						for(auto& d : m_status.dices)
						{
							d.locked = false;
						}
						m_status.gold -= item->cost;
						m_holdedItem->setSoldOut(true);
						break;
					}
				}
				m_holdedItem = nullptr;
			}
			
		}
		else if(item && item->itemType() == U"カテゴリー")
		{
			m_holdedItem = nullptr;
		}
		else {
			m_holdedItem = nullptr;
		}

		if(cancelButtonRect.leftClicked())
		{
			m_holdedItem = nullptr;
		}
		return;
	}

	for(auto& merch : m_merchandises)
	{
		merch.update();
		if (merch.isClicked() && !merch.isSoldOut())
		{
			const auto item = merch.getItem();
			if (item)
			{
				if (m_status.gold >= item->cost)
				{
					m_holdedItem = &merch;
					m_selected = false;
				}
			}
		}
	}
}

void Shop::draw() const
{
	FontAsset(U"Bold")(U"ショップ").drawAt(Scene::CenterF().x, 50, ColorF{ 0.1 });
	for (const auto& merch : m_merchandises)
	{
		merch.draw();
	}

	if (m_holdedItem) {
		const Transformer2D t{ Mat3x2::Identity(), Mat3x2::Translate(240,60) };

		Rect{ 0,0,Scene::Size() }.draw(ColorF{ 0, 0, 0, 0.5 });
		viewportRect.rounded(10).draw(ColorF{ 1.0 }).drawFrame(3, ColorF{ 0.1 });
		{
			const ScopedViewport2D viewport{ viewportRect };
			boughtButtonRect.rounded(5).draw(ColorF{ 0.2, 0.8, 0.2 });
			FontAsset(U"Bold")(U"購入する").drawAt(32, boughtButtonRect.center(), ColorF{ 1.0 });
			cancelButtonRect.rounded(5).draw(ColorF{ 0.8, 0.2, 0.2 });
			FontAsset(U"Bold")(U"キャンセル").drawAt(32, cancelButtonRect.center(), ColorF{ 1.0 });

			const auto item = m_holdedItem->getItem();
			if (item && item->itemType() == U"ダイス")
			{
				m_diceBox.draw(viewportRect.stretched(-10));
			}
			GetExplanation().draw(Cursor::PosF(), viewportRect.stretched(-10));
		}
	}
	else
	{
		GetExplanation().draw(Cursor::PosF());
	}

}
