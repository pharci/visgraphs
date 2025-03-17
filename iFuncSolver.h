#include <iostream>
#include <string>
#define M_PI 3.14159265358979323846

class IFuncSolver {
public:
	virtual double operator()(double x) = 0;
	virtual std::string getName() = 0;
};

class LinearFunc : public IFuncSolver {
private:
	double a, b;
	std::string name = "Linear";
public:
	LinearFunc() { a = 0.0; b = 0.0; };
	LinearFunc(double a, double b) { this->a = a; this->b = b; };
	double operator()(double x) override { return a * x + b; };
	std::string getName() override { return this->name; }
};

class SquareFunc : public IFuncSolver {
private:
	double a, b, c;
	std::string name = "Square";
public:
	SquareFunc() { a = 0.0; b = 0.0; c = 0.0; };
	SquareFunc(double a, double b, double c) { this->a = a; this->b = b; this->c = c; };
	double operator()(double x) override { return a * x * x + b * x + c; };
	std::string getName() override { return this->name; }
};

class SinFunc : public IFuncSolver {
private:
	std::string name = "Sin";
public:
	double operator()(double x) override { return sin(x); };
	std::string getName() override { return this->name; }
};

class CosFunc : public IFuncSolver {
private:
	std::string name = "Cos";
public:
	double operator()(double x) override { return cos(x); };
	std::string getName() override { return this->name; }
};