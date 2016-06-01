#include <cmath>
#include <oberon/helper>

double (*cceil) (double) = &ceil;
double (*cfloor)(double) = &floor;

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
