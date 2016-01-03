#include "Graph.h"
#include "Simple_window.h"

using namespace Graph_lib;
// chapter 14 exercises
class Smiley : public Circle
{
public:
	Smiley() {}
	Smiley(Point p, int radius )
	: r{ radius } {
		center = p;
		add(Point{ p.x - r, p.y - r });
		add(Point{ p.x - r / 2, p.y - r / 2 });
		add(Point{ point(0).x + (r / 3), point(0).y + (r/3) });
		add(Point{ point(0).x + ((4*r) / 3), point(0).y + ((r)/3) });
	}
	void draw_lines() const;
protected:
	Point center;
	int r;
};

void Smiley::draw_lines() const
{
	if (color().visibility())
	{
		fl_arc(point(0).x, point(0).y, r + r, r + r, 0, 360);
		fl_arc(point(1).x, point(1).y, r ,r , 180, 360);
		fl_arc(point(2).x, point(2).y, (r / 3),(r/3), 0, 360);
		fl_arc(point(3).x, point(3).y, (r / 3),(r/3), 0, 360);
	}
}

class smileyHat : public Smiley{

public:
	smileyHat(Point p, int radius, int hatlength)
		:r{ radius }, center{ p }, hlength{ hatlength }
	{
		add(Point{ p.x - r, p.y - r });
		add(Point{ p.x - r / 2, p.y - r / 2 });
		add(Point{ point(0).x + (r / 3), point(0).y + (r/3) });
		add(Point{ point(0).x + ((4*r) / 3), point(0).y + ((r)/3) });

		//add brim of hat
		add(Point{ point(0).x, point(0).y });
		add(Point{ point(0).x + 2 * r, point(0).y });

		//add hat
	}
	void draw_lines() const
	{
		Graph_lib::Rectangle rect{ Point{ point(4).x + (r/2), point(4).y - hlength }, hlength, r };
		if (color().visibility())
		{
			fl_arc(point(0).x, point(0).y, r + r, r + r, 0, 360);
			fl_arc(point(1).x, point(1).y, r ,r , 180, 360);
			fl_arc(point(2).x, point(2).y, (r / 3),(r/3), 0, 360);
			fl_arc(point(3).x, point(3).y, (r / 3),(r/3), 0, 360);
			fl_line( point(4).x, point(4).y, point(5).x, point(5).y );
			rect.draw_lines();
		}
	}

private:
	Point center;
	int r;
	int hlength;
};




class Frowny : public Circle
{
public:
	Frowny(Point p, int radius )
	: r{ radius } {
		center = p;
		add(Point{ p.x - r, p.y - r });
		add(Point{ p.x - r / 2, p.y  });
		add(Point{ point(0).x + (r / 3), point(0).y + (r/3) });
		add(Point{ point(0).x + ((4*r) / 3), point(0).y + ((r)/3) });
	}
	void draw_lines() const;
protected:
	Point center;
	int r;
};


void Frowny::draw_lines() const
{
	if (color().visibility())
	{
		fl_arc(point(0).x, point(0).y, r + r, r + r, 0, 360);
		fl_arc(point(1).x, point(1).y, r ,r , 0, 180);
		fl_arc(point(2).x, point(2).y, (r / 3),(r/3), 0, 360);
		fl_arc(point(3).x, point(3).y, (r / 3),(r/3), 0, 360);
	}
}

class Abstract
{
	virtual void doShit() = 0;
};

class Immobile_Circle : public Circle
{
public:
	Immobile_Circle(Point p, int rr)
		:r{ rr }
	{
		add(Point{ p.x - r, p.y - r });
	}
	
	void move(int dx, int dy) override
	{
		// do nothing
		std::cout << "Can't move me bruh. ";
	}
	void draw_lines() const;

private:
	int r;

};

void Immobile_Circle::draw_lines() const
{
	if (color().visibility())
	{
		fl_arc(point(0).x, point(0).y, r + r, r + r, 0, 360);
	}
}

struct Striped_rectangle : Rectangle{
	Striped_rectangle(Point p, int ww, int hh) : Rectangle(p, ww, hh){}
	Striped_rectangle(Point x, Point y) : Rectangle(x, y){}
	void draw_lines() const;
};

void Striped_rectangle::draw_lines() const
{
	if (fill_color().visibility())
	{
		fl_color(fill_color().as_int());
		for (int i = point(0).x + 2; i < point(0).x + width(); i+=3)
			fl_line(i, point(0).y, i , point(0).y+ height());
		fl_color(color().as_int());
	}
	if(color().visibility())
	{
		fl_color(color().as_int());
		fl_rect(point(0).x, point(0).y, width(), height());
	}

}

struct Striped_circle : Circle{
	Striped_circle(Point p, int rr) : Circle(p, rr){}
	void draw_lines() const;
};

void Striped_circle::draw_lines() const
{
	if (fill_color().visibility())
	{
		fl_color(fill_color().as_int());
		// use unit circle
		for (double i = 3.14159; i > 0; i -= 0.01)
			fl_line(center().x - (radius()*cos(i)), center().y - (radius()*sin(i)), center().x - (radius()*cos(i)), center().y + (radius()*sin(i)));
	
		fl_color(color().as_int());
	}

	if (color().visibility())
	{
		fl_color(color().as_int());
		fl_arc(point(0).x, point(0).y, radius()*2, radius()*2, 0, 360);
	}
}

struct Binary_tree : Shape
{
	Binary_tree(Point p, int ll)
		:l{ ll }
	{
		add(Point{ p.x, p.y });
	}

	int levels() const { return l; }
	void draw_lines() const;
private:
	int l;
};

void Binary_tree::draw_lines() const
{
	Vector_ref<Circle> nodes;
	if (color().visibility())
	{
		fl_color(color().as_int());
		for (int i = 0; i < l; i++)
		{
			for (int j = i; j >= 0; j--)
			{
				nodes.push_back(new Circle{ Point{ point(0).x + j * 150 - i * 75, point(0).y + i * 150 }, 25 });
				if (fill_color().visibility())
					nodes[nodes.size()-1].set_fill_color(fill_color());;
				nodes[nodes.size() - 1].draw_lines();
			}
		}
		fl_color(color().as_int());
	}
}



#if 0

int main()
{
	Simple_window win{ Point{ 0, 0 }, 1000, 800, "Ex14" };
	Smiley s1{ Point{ 300, 300 }, 100 };
	Frowny f1{ Point{ 500, 300 }, 100 };
	smileyHat s1h{ Point{ 300, 100 }, 50, 50 };
	Immobile_Circle imCircle{ Point{ 50, 50 }, 50 };
	Striped_rectangle sr{ Point{ 600, 600 }, 200, 50 };
	Striped_circle sc{ Point{ 700, 500 }, 50 };

	Binary_tree bt{ Point{ 340, 222 }, 3 };

	sc.set_fill_color(Color::cyan);
	sr.set_fill_color(Color::cyan);
	bt.set_fill_color(Color::cyan);
	imCircle.move(50, 50);

	win.attach(s1);
	win.attach(f1);
	win.attach(s1h);
	win.attach(imCircle);
	win.attach(sr);
	win.attach(sc);
	win.attach(bt);




	win.wait_for_button();

}
#endif