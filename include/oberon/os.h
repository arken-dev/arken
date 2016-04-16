namespace oberon {
  namespace os {
    int    cores();
    double microtime();
    char * name();
    void   sleep(double secs);
    char * uuid();
    char * read(const char * path);
  }
}
