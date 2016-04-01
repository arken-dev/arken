namespace oberon {
  namespace os {
    double microtime();
    void   sleep(double secs);
    char * uuid();
    char * read(const char * path);
  }
}
