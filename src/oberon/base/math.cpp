#include <cmath>
#include <oberon/helper>

double math::ceil(double number)
{
  return ceil(number);
}

double math::floor(double number)
{
  return floor(number);
}

double math::round(double number, int precision)
{
  return floor((number + (0.5/(pow(10, precision)))) * (pow(10, precision))) / (pow(10, precision));
}

double math::truncate(double number, int precision)
{
  return floor(number * (pow(10, precision))) / (pow(10, precision));
}
