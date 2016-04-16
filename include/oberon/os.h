namespace oberon {
  namespace os {
    double microtime();
    char * name();
    void   sleep(double secs);
    char * uuid();
    char * read(const char * path);
  }
}
