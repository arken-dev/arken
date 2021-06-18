#include <arken/concurrent/shared.h>

namespace arken {
namespace concurrent {

Shared::Shared()
{
  m_mapString = std::shared_ptr<std::unordered_map<string, string>>(new std::unordered_map<string, string>);
  m_mapNumber = std::shared_ptr<std::unordered_map<string, double>>(new std::unordered_map<string, double>);
  m_mapBool   = std::shared_ptr<std::unordered_map<string, bool>>(new std::unordered_map<string, bool>);
  m_mutex     = std::shared_ptr<std::mutex>(new std::mutex);
};

Shared::Shared( const Shared & obj)
{
  m_mapString = obj.m_mapString;
  m_mapNumber = obj.m_mapNumber;
  m_mapBool   = obj.m_mapBool;
  m_mutex     = obj.m_mutex;
}

// NUMBER
double Shared::getNumber(string key)
{
  std::unique_lock<std::mutex> lck(*m_mutex);
  if( m_mapNumber->count(key) ) {
    return m_mapNumber->at(key);
  } else {
    return 0;
  }
}

void Shared::setNumber(string key, double value)
{
  std::unique_lock<std::mutex> lck(*m_mutex);
  (*m_mapNumber)[key] = value;
}

double Shared::increment(string key, double value)
{
  std::unique_lock<std::mutex> lck(*m_mutex);

  if( m_mapNumber->count(key) ) {
    value += m_mapNumber->at(key);
    (*m_mapNumber)[key] = value;
  } else {
    (*m_mapNumber)[key] = value;
  }

  return value;
}

// STRING
string Shared::getString(string key)
{
  std::unique_lock<std::mutex> lck(*m_mutex);
  if( m_mapString->count(key) ) {
    return m_mapString->at(key);
  } else {
    return {};
  }
}

void Shared::setString(string key, string value)
{
  std::unique_lock<std::mutex> lck(*m_mutex);
  (*m_mapString)[key] = value;
}

string Shared::append(string key, string value)
{
  std::unique_lock<std::mutex> lck(*m_mutex);
  if( m_mapString->count(key) ) {
    string v = std::move(m_mapString->at(key));
    v.append(value);
    (*m_mapString)[key] = std::move(v);
  } else {
    (*m_mapString)[key] = value;
  }

  return value;
}

// BOOL
bool Shared::getBool(string key)
{
  std::unique_lock<std::mutex> lck(*m_mutex);
  if( m_mapBool->count(key) ) {
    return m_mapBool->at(key);
  } else {
    return false;
  }

  return true;
}

void Shared::setBool(string key, bool value)
{
  std::unique_lock<std::mutex> lck(*m_mutex);
  (*m_mapBool)[key] = value;

}

bool Shared::toggle(string key)
{
  std::unique_lock<std::mutex> lck(*m_mutex);
  bool result = false;
  if( m_mapBool->count(key) ) {
    result = ! m_mapBool->at(key);
    (*m_mapBool)[key] = result;
  }
  return result;
}

} // namespace concurrent
} // namespace arken
