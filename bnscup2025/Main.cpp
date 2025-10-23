# include "Common.hpp"
# include "Title.hpp"
# include "Game.hpp"

void Main()
{
	FontAsset::Register(U"TitleFont", FontMethod::MSDF, 48, U"example/font/RocknRoll/RocknRollOne-Regular.ttf");
	FontAsset(U"TitleFont").setBufferThickness(4);

	FontAsset::Register(U"Bold", FontMethod::MSDF, 48, Typeface::Bold);
	FontAsset::Register(U"Regular", FontMethod::MSDF, 32, Typeface::Regular);
	FontAsset::Register(U"Category", FontMethod::MSDF, 28, Typeface::Bold);

	App manager;
	manager.add<Title>(State::Title);
	manager.add<Game>(State::Game);

	manager.init(State::Game);

	while (System::Update())
	{
		if (not manager.update())
		{
			break;
		}
	}
}
