namespace oberon {
  namespace regex {
    bool ematch(const char * string, const char * regex);
    bool match(const char * string, const char * regex);
    char * replace(const char * string, const char * regex, const char * after);
  }
}
