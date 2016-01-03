#include "Graph.h"
#include "Simple_window.h"
#include "Window.h"

double one(double x) { return 1; }
double slope(double x) { return x / 2; }
double square(double x) { return x * x; }

struct Distribution {
	int year, young, middle, old;
};

class Scale {
	int cbase;
	int vbase;
	double scale;
public:
	Scale(int b, int vb, double s) : cbase{ b }, vbase{ vb }, scale{ s } {}
	int operator()(int v) const { return cbase + (v - vbase)*scale; }
};

istream& operator>>(istream& is, Distribution& d)
// assume formate: ( year : young middle old )
{
	char ch1 = 0;
	char ch2 = 0;
	char ch3 = 0;
	Distribution dd;

	if (is >> ch1 >> dd.year
		>> ch2 >> dd.young >> dd.middle >> dd.old
		>> ch3) {
		if (ch1 != '(' || ch2 != ':' || ch3 != ')'){
			is.clear(ios_base::failbit);
			return is;
		}
	}
	else
		return is;
	d = dd;
	return is;
}

using namespace Graph_lib;

int main()
{
	try{
		string file_name = "japanese-age-data.txt";
		ifstream ifs{ file_name };
		if (!ifs) error("Can't open ", file_name);

		const int base_year = 1960;
		const int end_year = 2040;

		const int xmax = 600;
		const int ymax = 400;

		const int xoffset = 100;
		const int yoffset = 60;

		const int xspace = 40;
		const int yspace = 40;

		const int xlength = xmax - xoffset - xspace;
		const int ylength = ymax - yoffset - yspace;

		const double xscale = double(xlength) / (end_year - base_year);
		const double yscale = double(ylength) / 100;

		const int current = 2015;




		Scale xs{ xoffset, base_year, xscale };
		Scale ys{ ymax - yoffset, 0, -yscale };

		Simple_window win{ Point{ 100, 100 }, xmax, ymax, "Aging Japan" };
		Axis x{ Axis::x, Point{ xoffset, ymax - yoffset }, xlength, (end_year - base_year) / 10,
			"year  1960    1970    1980    1990    2000    2010    2020    2030    2040" };
		x.label.move(-100, 0);
		x.label.set_font_size(16);

		Axis y{ Axis::y, Point{ xoffset, ymax - yoffset }, ylength, 10, " % of population" };
		y.label.set_font_size(16);


		Line current_year{ Point{ xs(current), ys(0) }, Point{ xs(current), ys(100) } };
		current_year.set_style(Line_style::dash);

		Open_polyline children;
		Open_polyline adults;
		Open_polyline aged;

		for (Distribution d; ifs >> d;) {
			if (d.year < base_year || end_year < d.year)
				error("year out of range");
			if (d.young + d.middle + d.old != 100)
				error("percentages don't add up");
			const int x = xs(d.year);
			children.add(Point{ x, ys(d.young) });
			adults.add(Point{ x, ys(d.middle) });
			aged.add(Point{ x, ys(d.old) });
		}

		Text children_label{ Point{ 0, children.point(0).y }, "age 0-14" };
		children.set_color(Color::red);
		children_label.set_color(Color::red);
		children_label.set_font_size(9);

		Text adults_label{ Point{ 20, adults.point(0).y }, "age 15-64" };
		adults.set_color(Color::blue);
		adults_label.set_color(Color::blue);
		adults_label.set_font_size(9);

		Text aged_label{ Point{ 20, aged.point(0).y }, "age 65+" };
		aged.set_color(Color::dark_green);
		aged_label.set_color(Color::dark_green);
		aged_label.set_font_size(9);

		win.attach(children);
		win.attach(adults);
		win.attach(aged);

		win.attach(children_label);
		win.attach(adults_label);
		win.attach(aged_label);

		win.attach(x);
		win.attach(y);
		win.attach(current_year);

		win.wait_for_button();
	}

	catch (exception& e)
	{
		return 1;

	}
	catch (...)
	{
		return 2;

	}
	/*

	const int xmax = 600;
	const int ymax = 400;

	const int x_orig = xmax / 2;
	const int y_orig = ymax / 2;
	const Point orig{ x_orig, y_orig };

	const int n_points = 400;

	const int r_min = -10;
	const int r_max = 11;

	const double x_scale = 30;
	const double y_scale = 30;



	Function s{ one, (double)r_min,(double) r_max, orig, n_points, x_scale, y_scale };
	Function s2{ slope,(double) r_min,(double) r_max, orig, n_points, x_scale, y_scale };
	Function s3{ square, (double)r_min, (double)r_max, orig, n_points, x_scale, y_scale };

	win.attach(s);
	win.attach(s2);
	win.attach(s3);
	*/




}