#include <mutex>
#include <ctime>

namespace arken {
namespace chrono {

static std::mutex s_chrono_mutex;

struct std::tm* localtime(const time_t* t)
{
  std::unique_lock<std::mutex> lck(s_chrono_mutex);
  return std::localtime(t);
}

char* asctime( const std::tm* time_ptr )
{
  std::unique_lock<std::mutex> lck(s_chrono_mutex);
  return std::asctime(time_ptr);
}

char *strptime(const char *buf, const char *format, struct tm *tm)
{
  std::unique_lock<std::mutex> lck(s_chrono_mutex);
  return ::strptime(buf, format, tm);
}

time_t mktime(struct std::tm *time_ptr)
{
  std::unique_lock<std::mutex> lck(s_chrono_mutex);
  return ::std::mktime(time_ptr);
}

time_t time(std::time_t* arg)
{
  std::unique_lock<std::mutex> lck(s_chrono_mutex);
  return ::std::time(arg);
}

size_t strftime( char* str, std::size_t count, const char* format, const std::tm* tp )
{
  std::unique_lock<std::mutex> lck(s_chrono_mutex);
  return ::std::strftime(str, count, format, tp);
}
}
}
