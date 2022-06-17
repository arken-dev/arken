#include <arken/concurrent/shared.h>

namespace arken {
namespace concurrent {

Shared::Shared()
{
  m_info  = std::shared_ptr<string>(new string);
  m_map   = std::shared_ptr<std::unordered_map<string, data>>(new std::unordered_map<string, data>);
  m_mutex = std::shared_ptr<std::mutex>(new std::mutex);
};

Shared::Shared(const Shared & obj)
{
  m_info  = obj.m_info;
  m_map   = obj.m_map;
  m_mutex = obj.m_mutex;
}

void Shared::info(string info)
{
  std::unique_lock<std::mutex> lck(*m_mutex);
  *m_info = info;
}

string Shared::info()
{
  std::unique_lock<std::mutex> lck(*m_mutex);
  return *m_info;
}

// NUMBER
double Shared::getNumber(string key)
{
  std::unique_lock<std::mutex> lck(*m_mutex);
  if( m_map->count(key) ) {
    return m_map->at(key).m_number;
  } else {
    return 0;
  }
}

void Shared::setNumber(string key, double value)
{
  std::unique_lock<std::mutex> lck(*m_mutex);
  (*m_map)[key].m_number = value;
}

double Shared::increment(string key, double value)
{
  std::unique_lock<std::mutex> lck(*m_mutex);

  if( m_map->count(key) ) {
    value += m_map->at(key).m_number;
    (*m_map)[key].m_number = value;
  } else {
    (*m_map)[key].m_number = value;
  }

  return value;
}

// STRING
string Shared::getString(string key)
{
  std::unique_lock<std::mutex> lck(*m_mutex);
  if( m_map->count(key) ) {
    return m_map->at(key).m_string;
  } else {
    return {};
  }
}

void Shared::setString(string key, string value)
{
  std::unique_lock<std::mutex> lck(*m_mutex);
  (*m_map)[key].m_string = value;
}

string Shared::append(string key, string value)
{
  std::unique_lock<std::mutex> lck(*m_mutex);
  if( m_map->count(key) ) {
    string v = std::move(m_map->at(key).m_string);
    v.append(value);
    (*m_map)[key].m_string = std::move(v);
  } else {
    (*m_map)[key].m_string = value;
  }

  return value;
}

// BOOL
bool Shared::getBool(string key)
{
  std::unique_lock<std::mutex> lck(*m_mutex);
  if( m_map->count(key) ) {
    return m_map->at(key).m_bool;
  } else {
    return false;
  }

  return true;
}

void Shared::setBool(string key, bool value)
{
  std::unique_lock<std::mutex> lck(*m_mutex);
  (*m_map)[key].m_bool = value;
}

bool Shared::toggle(string key)
{
  std::unique_lock<std::mutex> lck(*m_mutex);
  bool result = false;
  if( m_map->count(key) ) {
    result = ! m_map->at(key).m_bool;
    (*m_map)[key].m_bool = result;
  }
  return result;
}

Shared & Shared::global()
{
  static Shared instance;
  return instance;
}

void Shared::put(string key, string value)
{
  this->setString(key, value);
}

void Shared::put(string key, double value)
{
  this->setNumber(key, value);
}

void Shared::put(string key, bool value)
{
  this->setBool(key, value);
}

short Shared::flag(string key)
{
  std::unique_lock<std::mutex> lck(*m_mutex);
  if( m_map->count(key) ) {
    return m_map->at(key).m_flag;
  } else {
    return 0;
  }
}

} // namespace concurrent
} // namespace arken
