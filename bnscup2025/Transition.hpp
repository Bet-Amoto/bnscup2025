# include <Siv3D.hpp>

class SplitCircle
{
private:
	Size area;					//描画エリア
	Circle split1, split2;		//背景のスプリット
	Array<Circle> arcs;			//回る線の本体
	Array<double> arcStartDeg;	//回る線のスタート位置
	Array<double> arcRotateDeg;	//回る線の回る量
	Array<double> arcLength;	//回る線の長さ
	Array<double> arcWeight;	//回る線の太さ
	Array<double> delay;		//回る線の回るディレイ
	Array<ColorF> arcColors;	//回る線の色

public:
	SplitCircle() = default;
	SplitCircle(Size size, int32 quantity = 20)
		: area{ size }
	{
		init(quantity);
	}

	//初期化関数
	void init(int32 quantity = 20)
	{
		//メンバ初期化
		arcs.clear();
		arcStartDeg.clear();
		arcRotateDeg.clear();
		arcLength.clear();
		arcWeight.clear();
		delay.clear();
		arcColors.clear();

		//ベースとなる色相(これの±40の色になる)
		double baseColor = Random(0, 360);

		for (auto i : step(quantity))
		{
			//乱数で各要素を決める
			arcStartDeg << Random(0_deg, 360_deg);
			double rotate = Random(120_deg, 1080_deg);
			arcRotateDeg << rotate;
			arcLength << rotate / 3;
			arcWeight << Random(1, 6);
			delay << Random(0.0, 0.3);
			arcColors << HSV(baseColor + Random(-40, 40), Random(0.3, 0.8), 1);

			arcs << Circle{ Scene::Center(), Random(100, (area.x + area.y) / 3) };
		}

		//背景のスプリット
		split1 = Circle{ Scene::Center(), area.x + area.y };
		split2 = Circle{ Scene::Center(), area.x + area.y };
	}

	void drawTransitionIn(double t) const
	{
		//イージング
		double e = EaseInOutQuart(t);
		//背景のスプリット描画
		split1.drawPie(180_deg - t * 70_deg, 180_deg - e * 180_deg, Palette::Darkgrey);
		split2.drawPie(-t * 70_deg, 180_deg - e * 180_deg, Palette::Darkgrey);

		double t2 = t + 1.0;

		for (auto i : step(arcs.size()))
		{
			//ディレイを考慮したイージング
			double e = EaseInOutQuart(Min(1.0 - delay[i] + t, 1.0));
			//回る線を描画
			arcs[i].drawArc(arcStartDeg[i] - e * arcRotateDeg[i] - (t2 - delay[i]) * 70_deg, sin(e * Math::Pi) * arcLength[i], arcWeight[i], arcWeight[i], arcColors[i]);
		}
	}

	void drawTransitionOut(double t) const
	{
		//イージング
		double e = EaseInOutQuart(t);
		//背景のスプリット描画
		split1.drawPie(70_deg - t * 70_deg, -e * 180_deg, Palette::Darkgrey);
		split2.drawPie(250_deg - t * 70_deg, -e * 180_deg, Palette::Darkgrey);

		for (auto i : step(arcs.size()))
		{
			//ディレイを考慮したイージング
			double e = EaseInOutQuart(Max(t - delay[i], 0.0));
			//回る線を描画
			arcs[i].drawArc(LineStyle::RoundCap, arcStartDeg[i] - e * arcRotateDeg[i] - (t - delay[i]) * 70_deg, sin(e * Math::Pi) * arcLength[i], arcWeight[i], arcWeight[i], arcColors[i]);
		}
	}
};

class RectSlide {
	Size area; //シーンの大きさ
	Array<Rect> rects;	//長方形たち
	Array<ColorF> colors; //色
public:
	RectSlide(Size s, int32 kazu = 30) :area(s)
	{
		init(kazu);
	};
	//ランダムに色を設定する
	ColorF randomcol() const {
		double c = Random(0.3, 0.7);
		if (RandomBool(0.8)) {
			return ColorF(c);
		}
		c += 0.2;
		if (RandomBool()) {
			return ColorF(c, 1, c);
		}
		return ColorF(c, c, 1);
	}

	//長方形の形、色、数を初期化
	void init(int kazu) {
		rects.clear();
		colors.clear();
		Array<int32> p;
		p << area.y;
		for (auto i : step(kazu)) {
			int32 num = p[0] * Random(0.2, 0.8);
			p << num << p[0] - num;
			p.remove_at(0);
		}
		int32 posy = 0;
		for (auto n : p) {
			rects << Rect{ -area.x, posy, area.x, n };
			posy += n;
			colors << randomcol();
		}
		rects.shuffle();
	}
	//フェードアウト用
	void drawFadeOut(double t) const {
		for (const auto& [i, rect] : Indexed(rects)) {
			const double nt = Clamp((t - (double)i * 0.6 / rects.size()) * 2.5, 0.0, 1.0);
			rect.movedBy(EaseInExpo(nt) * area.x, 0).draw(colors[i]);
		}
	}
	//フェードイン用
	void drawFadeIn(double t) const {
		for (const auto& [i, rect] : Indexed(rects)) {
			const double nt = Clamp((t - (double)i * 0.6 / rects.size()) * 2.5, 0.0, 1.0);
			rect.movedBy(area.x + EaseOutExpo(nt) * area.x, 0).draw(colors[i]);
		}
	}
};
