#include <iostream>
#include "Graph.h"

class B1{
public:
	virtual void vf(){
		std::cout << "B1::vf()" << std::endl;
	}
	void f(){
		std::cout << "B1::f()" << std::endl;
	}
	virtual void pvf() = 0;


};

class B2{
public:
	virtual void pvf() = 0;
};

class D21 : public B2{
public:
	string s;
	void pvf() override {
		std::cout << s << std::endl;
	}
};

class D22 : public B2{
public:
	int i;
	void pvf() override {
		std::cout << i << std::endl;
	}
};

class D1 : public B1{
public:
	void vf(){
		std::cout << "D1::vf()" << std::endl;
	}
	void f(){
		std::cout << "D1::f()" << std::endl;
	}

};

class D2 : public D1{
public:
	void pvf() override{
		std::cout << "D2::pvf()" << std::endl;
	}
	
};

void f(B2& b)
{
	b.pvf();
}
#if 0
int main()
{
	/*
	B1 b1;
	b1.vf();
	b1.f();

	D1 d1;
	d1.vf();
	d1.f();

	B1& b1ref = d1;
	b1ref.vf();
	b1ref.f();
	*/
	D2 d2;
	d2.f();
	d2.vf();
	d2.pvf();

	D21 d21;
	d21.s = "Hello Lazar";
	f(d21);

	D22 d22;
	d22.i = 7;
	f(d22);
	

	char c;
	std::cin >> c;


}
#endif