class OStringList
{
  private:
  void init();
  int m_size;
  int m_resource;
  const char ** m_array;

  public:
  OStringList();
  OStringList(int resource);
  ~OStringList();
  void replace(int pos, const char * value);
  OStringList &append(const char * value);
  OStringList &operator << (const char * value);
  const char *operator[](int pos);
  const char *at(int pos);
  int size();
};
