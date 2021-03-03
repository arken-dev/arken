// Copyright 2016 The Arken Platform Authors.
// All rights reserved.
// Use of this source code is governed by a BSD-style
// license that can be found in the LICENSE file.

#include <cmath>
#include <arken/base>

int math::abs(int number)
{
  return std::abs(number);
}

double math::acos(double number)
{
  return std::acos(number);
}

double math::asin(double number)
{
  return std::asin(number);
}

double math::atan(double number)
{
  return std::atan(number);
}

double math::atan2(double x, double y)
{
  return std::atan2(x, y);
}

double math::ceil(double number)
{
  return std::ceil(number);
}

double math::cos(double number)
{
  return std::cos(number);
}

double math::cosh(double number)
{
  return std::cosh(number);
}

double math::exp(double number)
{
  return std::exp(number);
}

double math::floor(double number)
{
  return std::floor(number);
}

char * math::format(double value, int decimals, char dec_point, char thousands_sep)
{

  if( ! std::isnormal(value) ) {

    if( std::isnan(value) ) {
      return new char[4]{'n', 'a', 'n', '\0'};
    }

    if( std::isinf(value) ) {
      return new char[4]{'i', 'n', 'f', '\0'};
    }

  }

  int size = 65;
  if( value >= 1e80) {
    decimals = 2;
    size = 512;
  }

  char tmp[size];
  size_t slen;

  if( decimals > 6 || decimals == 0) {
    char fmt[65];
    sprintf(fmt, "%%.%if\n", decimals);
    slen = sprintf(tmp, fmt, math::round(value, decimals));
  } else {
    slen = sprintf(tmp, "%f", math::round(value, decimals));
  }

  char * res;
  int index = string::indexOf(tmp, ".");
  if( index == -1 ) {
    index = string::indexOf(tmp, ",");
  }
  int idx   = index;
  int len   = index;
  int t     = 0;

  if( decimals == 0 ) {
    idx = slen-1;
    len = slen-1;
    index = slen-1;
  } else {
    idx = index;
    len = index;
  }

  if(thousands_sep) {
    int ts = len;
    if( value < 0 ) {
      ts--;
    }
    t = ts/3;
    if (ts % 3 == 0) {
      t--;
    }
    len += t;
    idx += t;
  }

  if (decimals == 0) {
    res = new char[len+1];
  } else {
    len += decimals + 1;
    res  = new char[len+1];
    if ( dec_point == 0 ) {
      dec_point = '.';
    }

    // copy decimals
    for(int i=decimals; i > 0; i--) {
      res[idx+i] = tmp[index+i];
    }

    // copy decimal point
    res[idx] = dec_point;
  }

  //copy thousands
  int j = idx-1;
  int i = index-1;
  int f = 0;
  for(; i >= 0; i--) {
    if( t > 0 && f == 3 ) {
      f = 0;
      t--;
      res[j--] = thousands_sep;
    }

    res[j] = tmp[i];
    j--;
    f++;
  }
  res[len] = '\0';

  return res;
}

double math::fmod(double number, double denom)
{
  return std::fmod(number, denom);
}

double math::round(double number, int precision)
{
  double dprecision = std::pow(10, precision);
  return std::floor((number + (0.5/(dprecision))) * (dprecision)) / dprecision;
}

double math::truncate(double number, int precision)
{
  double dprecision = std::pow(10, precision);
  return std::floor(number * (dprecision)) / dprecision;
}
