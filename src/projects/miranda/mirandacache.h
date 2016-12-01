#ifndef MIRANDACACHE_H
#define MIRANDACACHE_H

class MirandaCache
{

public:
  MirandaCache(const char * data, int expires);
  ~MirandaCache();
  const char * data();
  bool isExpires();

private:
  const char * m_data;
  double m_expires;

};

#endif // MIRANDACACHE_H
