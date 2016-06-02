#include <cmath>
#include <oberon/helper>

int    (*cabs)   (int)    = &abs;
double (*cacos)  (double) = &acos;
double (*casin)  (double) = &asin;
double (*catan)  (double) = &atan;
double (*catan2) (double, double) = &atan2;
double (*cceil)  (double) = &ceil;
double (*cfloor) (double) = &floor;

int math::abs(int number)
{
  return cabs(number);
}

double math::acos(double number)
{
  return cacos(number);
}

double math::asin(double number)
{
  return casin(number);
}

double math::atan(double number)
{
  return catan(number);
}

double math::atan2(double x, double y)
{
  return catan2(x, y);
}

double math::ceil(double number)
{
  return cceil(number);
}

double math::floor(double number)
{
  return cfloor(number);
}

double math::round(double number, int precision)
{
  return cfloor((number + (0.5/(pow(10, precision)))) * (pow(10, precision))) / (pow(10, precision));
}

double math::truncate(double number, int precision)
{
  return cfloor(number * (pow(10, precision))) / (pow(10, precision));
}
