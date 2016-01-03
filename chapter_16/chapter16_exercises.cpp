#include "Graph.h"
#include "Window.h"
#include "GUI.h"
#include "std_lib_facilities.h"
#include "Simple_window.h"

#ifdef _MSC_VER
#define _CRT_SECURE_NO_WARNINGS
#endif

using namespace Graph_lib;

static int* parse_coords(string s){
		stringstream ss;
		ss  << s;
		// try to parse data in (x,y) format
		char c1, c2, c3;
		int x, y;
		if (ss >> c1 >> x >> c2 >> y >> c3)
		{
			if (c1 != '(' || c2 != ',' || c3 != ')') {
				return NULL;
			}
		}
		else return NULL;
		int *xy = new int[2];
		*xy = x;
		*(xy+1) = y;
		return (xy);
}


class Hexagon : public Closed_polyline
{
	public:
	Hexagon( Point p, int length ); // where length is length from any single vertex
private:
	int l;
	Point center;
};

Hexagon::Hexagon(Point p, int length)
	:l{ length }, center{ p }
{
	Point x{ p.x - l, p.y - (int)(l*sin(60)) };
	if (x.x < 0 || x.y < 0)
		error("Invalid center for hexagon.");
	add(Point{ (center.x + l) - (int)(l*cos(3.14159/3)), center.y - (int)(l*sin(3.14159/3)) });
	add(Point{ center.x + l, center.y });
	add(Point{ center.x + l - (int)(l*cos(3.14159/3)), center.y + (int)(l*sin(3.14159/3)) });
	add(Point{ point(2).x - l, point(2).y });
	add(Point{ center.x - l, center.y });
	add(Point{ point(0).x - l, point(0).y });
}


struct My_window : Window {
	My_window(Point xy, int w, int h, const string& title) : Window{ xy, w, h, title },
		next_button{ Point{ x_max() - 140, 0 }, 70, 20, "Next", [](Address, Address pw) { reference_to<My_window>(pw).next(); } },
		quit_button{ Point{ x_max() - 70, 0 }, 70, 20, "Quit", [](Address, Address pw) { reference_to<My_window>(pw).quit(); } },
		out{ Point{ x_max() - (x_max()/2 + 100), y_max() -100  }, 500, 20, "Click a button!"  }

	{
		const int grid_tl_x = 100;
		const int grid_tl_y = 100;

		for (int i = 0; i < 4; i++)
		{
			for (int j = 0; j < 4; j++)
			{
				int current_button = i*4 + j;
				grid.push_back(new Button{ Point{ grid_tl_x + j * 40, grid_tl_y + i * 40 },40, 40, "", [](Address bw, Address pw) { reference_to<My_window>(pw).misc_button(bw); } });
				attach(grid[grid.size() - 1]);

			}
		}
		attach(out);
		attach(next_button);
		attach(quit_button);
	}


private:
	Button next_button;
	Button quit_button;

	Vector_ref<Button> grid;

	Out_box out;

	void next(){ set_label("Con gratyz you can press 'next' "); }
	void quit(){ hide(); }
	void misc_button(Address b_address) {
		ostringstream s;
		s << "You pressed button stored at : " << hex << b_address;
		Button* button = static_cast<Button*>(b_address);
		s <<  "Width of " << dec <<  button->width;
		out.put(s.str());
		redraw();
	}
	
};


struct Image_window : Window {
	Image_window(Point p, int w, int h, int iw, int ih, const string& img_s, const string& title) :
		Window{ p, w, h, title },
		img{ Point{ (w / 2) - (iw / 2), (h / 2) - (ih / 2) }, img_s }, //center image initially
		img_b{ Point{ (w / 2) - (iw / 2), (h / 2) - (ih / 2) }, iw, ih, "img_b", [](Address, Address pw){ reference_to<Image_window>(pw).move_image(); } }  // give button same dimensions and position as img
	{
		attach(img);
		attach(img_b);

	}
private:
	Button img_b;
	Image img;

	void move_image() {
		int dx = randint(0- img_b.loc.x , (x_max()- img_b.width)-img_b.loc.x);
		int dy = randint(0 - img_b.loc.y, (y_max()-img_b.height) - img_b.loc.y);
		img.move(dx, dy);
		img_b.move(dx, dy);

	}
};


struct Shape_window : Window {
	Shape_window(Point p, int w, int h, const string & title) :
		Window{ p, w, h, title },
		Shape_menu{ Point{ 50, 50 }, 80, 60, Menu::vertical, "Shape menu" },
		in_xy{ Point{ 150, 20 }, 100, 20, "" }
	{
		Shape_menu.attach(new Button{ Point{ 0, 0 }, 80, 20, "Circle", [](Address, Address pw){ reference_to<Shape_window>(pw).circle_clicked(); } });
		Shape_menu.attach(new Button{ Point{ 0, 0 }, 80, 20, "20XX", [](Address, Address pw){ reference_to<Shape_window>(pw).hex_clicked(); } });
		attach(label_in);
		attach(Shape_menu);
		attach(in_xy);



	}

private:
	Menu Shape_menu;
	In_box in_xy;
	Vector_ref<Circle> circles;
	Vector_ref<Hexagon> hexagons;
	Text label_in{ Point{ 100, 30 }, "(x,y)" };

	void circle_clicked() {
		// x,y 
		int *xy = parse_coords(in_xy.get_string());
		if (!xy) return;
		circles.push_back(new Circle{ Point{ *(xy), *(xy+1) }, 100 });
		attach(circles[circles.size()-1]);
		redraw();
	}

	

	void hex_clicked() {

		Shape_menu.hide();

		int *xy = parse_coords(in_xy.get_string());

		if (!xy) return;

		hexagons.push_back(new Hexagon{ Point{ *(xy), *(xy + 1) }, 100 });
		hexagons[hexagons.size() - 1].set_fill_color(Color::cyan);
		hexagons[hexagons.size() - 1].set_color(Color::invisible);
		attach(hexagons[hexagons.size() - 1]);

		hexagons.push_back(new Hexagon{ Point{ *(xy), *(xy + 1)} , 70 });
		hexagons[hexagons.size() - 1].set_fill_color(50);
		hexagons[hexagons.size() - 1].set_color(Color::invisible);
		attach(hexagons[hexagons.size() -1 ]);
		redraw();

		//make sure  the menu is not hidden
		Shape_menu.show();

	}


		
};

struct Clock_hand : Shape {
	enum Type {
		second, minute, hour 
	};
	Clock_hand(Type t, Point p, int rr, struct tm* current_t)
		: hand{ t }, current_time(*current_t), r{rr}
	{
		if (hand == hour) {
			set_style(Line_style(Line_style::solid, 8));
		} else if (hand == minute) {
			set_style(Line_style(Line_style::solid, 5));
		}
		add(p);
		update_hand();
	}

private: 
	Type hand;
	int r;
	Point endpoint;
	struct tm current_time;

	void draw_lines();

public:	
	int radius() const
	{
		return r;
	}

	void update_hand() // update hand to current time.
	{
		// update current time;
		update_time();
		double angle = find_angle();
		//std::cout << "Angle given: " << angle << "\n";

		int x_new, y_new;
		x_new = ((double)r * cos(angle)) + double(point(0).x);
		y_new = ((double)r * sin(angle)) + double(point(0).y);
		endpoint.x = x_new;
		endpoint.y = y_new;

	}

	void update_time() {
		time_t rawtime;
		time(&rawtime);
		localtime_s( &current_time, &rawtime);
	}

	double find_angle()
	{
		const double Pi = 3.1415926;
		double angle;
		double second_bias;
		double minute_bias;
		switch (hand)
		{ 
			// shift it by pi/2 since clock starts at 12 and not 3.
			// so if the we're at the 15 sec mark for instance we get the result
			// of sin( pi/2 - pi/2) result instead of sin( pi/2 );
			// also recall axis are flipped 

			case second:
				angle = ((2.0*Pi) / 60.0) * current_time.tm_sec  - Pi/2;
				return angle;
			case minute:
				// shift minute slightly depending on current second
				second_bias = ((2.0*Pi) / 60) * ((float)current_time.tm_sec / 60.0);
				angle = ((2.0 * Pi) / 60.0) * current_time.tm_min - Pi / 2 + second_bias;;
				return angle;
			case hour:
				// shift hour slightly depending on the current minute 
				minute_bias = ((2.0*Pi) / 12) * ((float)current_time.tm_min / 60.0);
				angle = ((2.0*Pi) / 12.0) * current_time.tm_hour - Pi/2 + minute_bias; 
				return angle;
			default : 
				return -1;
		}
	}

	void draw_lines() const
	{
		if (color().visibility())
			fl_line(point(0).x, point(0).y, endpoint.x, endpoint.y);


	}
};

struct Clock_window : Window
{
	/*
		creates second, minute, and hour hands
		refreshes current time and changes their pivots based on current time
		(doesn't increment on its own) - this way the clock is still accurate after the system goes to sleep or hangs.
	*/
	Clock_window(Point p, int x, int h, const string& title,string img_name) : Window{ p, x, h, title },
		bg{ Point{ clock_center.x - 300 / 2, clock_center.y - 300 / 2 }, img_name}
	{
		
		time_t rawtime = time(0);
		struct tm* now = localtime(&rawtime);

		attach(bg);

		hands.push_back(new Clock_hand(Clock_hand::second, clock_center, 150, now));
		hands[0].set_color(Color::red);
		attach(hands[0]);

		hands.push_back(new Clock_hand(Clock_hand::minute,clock_center, 150, now));
		attach(hands[1]);

		hands.push_back(new Clock_hand(Clock_hand::hour, clock_center, 100, now));
		attach(hands[2]);

		Fl::add_timeout(0.001, Timer_cb, (void*)this);
	}

	

	void update_hands() {
		//debug purposes	
		time_t rawtime = time(0);
		struct tm* current_time = localtime(&rawtime);
		std::cout << asctime(current_time);
		for (int i = 0; i < hands.size(); i++)
		{
			hands[i].update_time();
			hands[i].update_hand();
		}
		redraw();
	}



private:

	Point clock_center{ 150, 150 };
	Image bg;
	Vector_ref<Clock_hand> hands;
	static void Timer_cb(Address pw) {
		reference_to<Clock_window>(pw).update_hands();
		Fl::repeat_timeout(0.25, Timer_cb,pw); // decreasing timeout time increases accuracy at cost of performance
	}


};


struct Bounce_window : Window
{
	Bounce_window(Point p, int w, int h, int img_ww, int img_hh, const string& title, string img_name) : Window{ p, w, h, title },
		airplane_img{ p, 100 ,100 },
		start{ Point{ x_max() - 200, 10 }, 100, 20, "Start", [](Address, Address pw) { reference_to<Bounce_window>(pw).start_fly(); } },
		stop{ Point{ x_max() - 100, 10 }, 100, 20, "Stop", [](Address, Address pw) { reference_to<Bounce_window>(pw).stop_fly(); } },
		img_h{ img_hh }, img_w{ img_ww }

	{
		attach(airplane_img);
		attach(start);
		attach(stop);

		Fl::add_timeout(0.25, tick_cb, (void*)this);
	}

	// Callback functions 
	void start_fly() {
		update_velocity();
		isFlying = true;
	}
	void stop_fly() {
		isFlying = false;
	}

	void update() {
		// move the ship around if isFlying is true;
		if (isFlying) {
			const int margin = 4; // how close it should get to the edge before changing direction
			while (out_of_bounds())
			{
				update_velocity();
			}
			airplane_img.move(v.x, v.y);
		}
		redraw();
	}

	double* rand_xy(int min = -100, int max = 100)
	{
		double* xy = new double(2);
		(*xy) = randint(-100, 100);
		*(xy + 1) = randint(-100, 100);
		return xy;
	}

	int out_of_bounds(){
		if ((airplane_img.point(0).x + v.x + img_w) > x_max() || (airplane_img.point(0).x + v.x < 0))
			return true;
		if ((airplane_img.point(0).y + v.y + img_h) > y_max() || (airplane_img.point(0).y + v.y < 0))
			return true;
		return false;
	}

	void update_velocity(int min = -100, int max = 100)
	{
		double* xy = rand_xy(min, max);
		v.x = *xy;
		v.y = *(xy + 1);
	}
	
	static void tick_cb(Address pw) {
		// just wait for other callbacks to happen.
		// while updating the ship	
		reference_to<Bounce_window>(pw).update();
		Fl::repeat_timeout(0.05, tick_cb, pw);
	}
	

private:
	Graph_lib::Rectangle airplane_img;

	int img_w, img_h;
	Button start;
	Button stop;
	bool isFlying = false;

	struct velocity {
		double x;
		double y;
	};

	velocity v;

	
};

int main()
{
	try{
		// shape menu
		Shape_window win{ Point{ 0, 0 }, 800, 800, "OHoho!" };

		// analog clock
		//Clock_window win{ Point{ 0, 0 }, 310, 310, "MontClock", "Clock_bg.gif" };

		// box flying around window
		//Bounce_window win{ Point{ 0, 0 }, 800, 800, 100, 100,  "What's happening", "Clock_bg.gif" };

		return gui_main();
	}
	catch (exception & e)
	{
		cerr << "Exception : " << e.what() << '\n';
		return 1;
	}
	

}