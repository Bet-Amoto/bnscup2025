# include <Siv3d.hpp>

//背景
typedef struct _BGCir {
	Circle cir;
	ColorF col;
	double speed;
	int z;
	bool updown;
	_BGCir() {
		cir = Circle(Random(0, 800), Random(0, 800), Random(2, 30));
		col = ColorF(Random(0.7, 1.0), Random(0.7, 1.0), Random(0.1, 0.8), Random(0.3, 0.5));
		z = Random(0, 10);
		nomalspeed = (-5 - 20.0 / (z + 1));
		speed = nomalspeed;
		updown = RandomBool();
		if (z == 0)updown = false;
		if (z == 10)updown = true;
	}
	_BGCir(Point pos) {
		cir = Circle(pos, Random(2, 30));
		col = ColorF(Random(0.7, 1.0), Random(0.7, 1.0), Random(0.1, 0.8), Random(0.3, 0.5));
		z = Random(0, 10);
		nomalspeed = (-5 - 20.0 / (z + 1));
		speed = nomalspeed;
		updown = RandomBool();
		if (z == 0)updown = false;
		if (z == 10)updown = true;
	}
	void update() {
		speed += (nomalspeed - speed) * Scene::DeltaTime();
		cir.moveBy(0, speed * Scene::DeltaTime());
		if (renzoku) {
			deray -= Scene::DeltaTime();
			if (deray > 0)return;
			deray = 0.5;
			z += updown ? 1 : -1;
			nomalspeed = (-5 - 20.0 / (z + 1));
			if (z == 0 or z == 10)renzoku = false;
			if (Random(0, 100) < 50)renzoku = false;
		}
		else if (Random(0, 1000) < 1) {
			z += updown ? 1 : -1;
			nomalspeed = (-5 - 20.0 / (z + 1));
			deray = 20;
			renzoku = true;
		}
		if (z == 0)updown = true;
		if (z == 10)updown = false;

	}
private:
	bool renzoku = false;
	double nomalspeed;
	int deray = 0.5;
}BGCir;
class BGFill {
public:
	BGFill(Size windowsize, int circount) {
		sceneSize = windowsize;
		sceneSize.y *= 1.25;
		for (int i : step(circount)) {
			cirs << BGCir(Point(Random(0, sceneSize.x), Random(0, sceneSize.y)));
		}
		for (int i : step(5)) {
			aarea << Rect(Random(0, sceneSize.x), Random(0, 3 * sceneSize.y / 5), Random(100, 200), 5);
			kasoku << Random(0.92, 1.08);
		}
		texture = Texture{ makeimage() };
		gaussianA1 = RenderTexture{ sceneSize };
		gaussianB1 = RenderTexture{ sceneSize };
	}
	void update() {
		for (auto& c : cirs) {
			c.update();
			if (c.cir.y < -50)c = BGCir(Point(Random(0, sceneSize.x), Random(9 * sceneSize.y / 10, sceneSize.y)));
			for (int i : step(aarea.size())) {
				if (aarea[i].intersects(c.cir))c.speed *= kasoku[i];
			}
		}
	}
	void draw() const{
		Array<BGCir> tmp = cirs.sorted_by([](BGCir a, BGCir b) {return a.z > b.z; });
		int i = 0, lastz = 10;
		Shader::Copy(texture, gaussianA1);
		while (lastz > -1) {
			{
				const ScopedRenderTarget2D target{ gaussianA1 };
				while (i < cirs.size() and tmp[i].z == lastz) {
					tmp[i].cir.draw(tmp[i].col);
					i++;
				}
			}
			lastz--;
			Shader::GaussianBlur(gaussianA1, gaussianB1, gaussianA1);
		}
		gaussianA1.draw();
	}
private:
	Image makeimage() {
		Image im{ sceneSize };
		double sx = sceneSize.x;
		double sy = sceneSize.y;
		for (int x : step(im.size().x)) {
			for (int y : step(im.size().y)) {
				im[y][x] = ColorF(0.20, (sx - x) / sx * 0.10 + 0.20, (sy - y) / sy * 0.10 + 0.30);
			}
		}
		return im;
	}
	Texture texture;
	Size sceneSize;
	Array<BGCir> cirs;
	Array<Rect> aarea;
	Array<double> kasoku;
	RenderTexture gaussianA1, gaussianB1;
};
