#include <arken/base>
#include <filesystem>
#include <chrono>

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <limits.h>

static
std::time_t to_time_t(std::filesystem::file_time_type tp)
{
  auto sctp = std::chrono::time_point_cast<std::chrono::system_clock::duration>
    (tp - std::filesystem::file_time_type::clock::now()
      + std::chrono::system_clock::now());
  return std::chrono::system_clock::to_time_t(sctp);
}

namespace arken {

using std::filesystem::path;

string os::abspath(const char * p)
{
  path path = std::filesystem::absolute(p);
  return string(path.string());
}

string os::basename(const char * p)
{
  path path = std::filesystem::absolute(p).filename();
  return string(path.string());
}

size_t os::atime(const char * p)
{
  auto tp = std::filesystem::last_write_time(p);
  return to_time_t(tp);
}

bool os::copy(const char * source, const char * destination, bool force = false)
{

  if( os::isdir(source) ) {
    std::filesystem::copy(source, destination,
      std::filesystem::copy_options::overwrite_existing |
      std::filesystem::copy_options::recursive);
  } else {
    std::filesystem::copy(source, destination);
  }

  return true;
}

bool os::chdir(const char * dirpath)
{
  std::filesystem::current_path(dirpath);
  path p = std::filesystem::current_path();
  return p.string().compare(os::pwd());
}

size_t os::ctime(const char * p)
{
  auto t = std::filesystem::last_write_time(p);
  return to_time_t(t);
}

string os::dirpath(const char * p)
{
  path path = std::filesystem::absolute(p).parent_path();
  return string(path.string());
}

bool os::exists(const char * path)
{
  return std::filesystem::exists(path);
}

string os::home()
{
  const char * p = getenv("HOME");
  if( p == nullptr ) {
    return {};
  } else {
    path path = std::filesystem::absolute(p);
    return string(path.string());
  }
}

string os::hostname()
{
  #define ARKEN_HOSTNAME_MAX 64
  auto hostname = new char[ARKEN_HOSTNAME_MAX+1]();
  gethostname(hostname, ARKEN_HOSTNAME_MAX+1);
  return string(std::move(hostname));
}

bool os::isdir(const char * path)
{
  return std::filesystem::is_directory(path);
}

bool os::isfile(const char * path)
{
  return std::filesystem::is_regular_file(path);
}

bool os::islink(const char * path)
{
  return std::filesystem::is_symlink(path);
}

bool os::link(const char * source, const char * destination, bool force = false)
{
  if( force and os::exists(destination) ) {
    std::filesystem::remove(destination);
  }
  std::filesystem::create_symlink(source, destination);

  return os::exists(destination);
}

bool os::mkdir(const char * dirname)
{
  return std::filesystem::create_directory(dirname);
}

bool os::mkpath(const char * dirpath)
{
  return std::filesystem::create_directories(dirpath);
}

string os::pwd()
{
  path cwd = std::filesystem::current_path();
  return string(cwd.string());
}

bool os::rmdir(const char * dirpath)
{
  if (std::filesystem::is_directory(dirpath) && std::filesystem::is_empty(dirpath)) {
    uintmax_t result = std::filesystem::remove_all(dirpath);
    return result > 0;
  }

  return false;
}

bool os::rmpath(const char * dirpath)
{
  uintmax_t result = std::filesystem::remove_all(dirpath);
  return result > 0;
}

string os::target(const char * p)
{
  path target = std::filesystem::read_symlink(p);
  return string(target.string());
}

string os::tmp()
{
  path tmp = std::filesystem::temp_directory_path();
  return string(tmp.string());
}

string os::root()
{
  path root = std::filesystem::current_path().root_directory();
  return string(root.string());
}

} // namespace arken
