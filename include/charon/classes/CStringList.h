#ifndef _CSTRING_LIST_HEADER_
#define _CSTRING_LIST_HEADER_

class CStringList
{
  private:
  void init();
  int m_size;
  int m_resource;
  const char ** m_array;

  public:
  CStringList();
  CStringList(int resource);
  ~CStringList();
  void replace(int pos, const char * value);
  CStringList &append(const char * value);
  CStringList &operator << (const char * value);
  const char *operator[](int pos);
  const char *at(int pos);
  const char *first();
  const char *last();
  char * join(const char * separator);
  int size();
};

#endif
