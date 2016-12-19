namespace charon {
  namespace base {
    class math {
      public:
      static int    abs(int number);
      static double acos(double number);
      static double asin(double number);
      static double atan(double number);
      static double atan2(double x, double y);
      static double ceil(double number);
      static double cos(double number);
      static double cosh(double number);
      static double exp(double number);
      static double floor(double number);
      static double fmod(double number, double denom);
      static double round(double number, int precision);
      static double truncate(double number, int precision);
    };
  }
}
