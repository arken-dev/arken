#include <arken/base>
#include <boost/filesystem.hpp>
#include <boost/asio/ip/host_name.hpp>

namespace arken {

using boost::filesystem::path;

string os::abspath(const char * p)
{
  path path = boost::filesystem::absolute(p);
  return string(path.string());
}

string os::basename(const char * p)
{
  path path = boost::filesystem::absolute(p).parent_path();
  return string(path.string());
}

size_t os::atime(const char * p)
{
  std::time_t t = boost::filesystem::last_write_time(p);
  return (size_t) t;
}

bool os::copy(const char * source, const char * destination, bool force = false)
{

  if( force and os::exists(destination) ) {
    boost::filesystem::remove(destination);
  }

  if( os::isdir(source) ) {
    // TODO recusive
    boost::filesystem::copy_directory(source, destination);
  } else {
    boost::filesystem::copy(source, destination);
  }

  return true;
}

// TODO test works better
bool os::chdir(const char * dirpath)
{
  path p = boost::filesystem::system_complete(dirpath);
  return p.string().compare(os::pwd());
}

size_t os::ctime(const char * p)
{
  std::time_t t = boost::filesystem::last_write_time(p);
  return (size_t) t;
}

string os::dirpath(const char * p)
{
  path path = boost::filesystem::absolute(p).parent_path();
  return string(path.string());
}

bool os::exists(const char * path)
{
  return boost::filesystem::exists(path);
}

string os::home()
{
  const char * p = getenv("HOME");
  if( p == nullptr ) {
    return string();
  } else {
    path path = boost::filesystem::absolute(p);
    return string(path.string());
  }
}

string os::hostname()
{
  std::string hostname = boost::asio::ip::host_name();
  return string(hostname);
}

bool os::isdir(const char * path)
{
  return boost::filesystem::is_directory(path);
}

bool os::isfile(const char * path)
{
  return boost::filesystem::is_regular_file(path);
}

bool os::islink(const char * path)
{
  return boost::filesystem::is_symlink(path);
}

bool os::link(const char * source, const char * destination, bool force = false)
{
  if( force and os::exists(destination) ) {
    boost::filesystem::remove(destination);
  }
  boost::filesystem::create_symlink(source, destination);

  return os::exists(destination);
}

bool os::mkdir(const char * dirname)
{
  return boost::filesystem::create_directory(dirname);
}

bool os::mkpath(const char * dirpath)
{
  return boost::filesystem::create_directories(dirpath);
}

string os::pwd()
{
  path cwd = boost::filesystem::current_path();
  return string(cwd.string());
}

bool os::rmdir(const char * dirpath)
{
  if (boost::filesystem::is_directory(dirpath) && boost::filesystem::is_empty(dirpath)) {
    uintmax_t result = boost::filesystem::remove_all(dirpath);
    return result > 0;
  }

  return false;
}

bool os::rmpath(const char * dirpath)
{
  uintmax_t result = boost::filesystem::remove_all(dirpath);
  return result > 0;
}

string os::target(const char * p)
{
  path target = boost::filesystem::read_symlink(p);
  return string(target.string());
}

string os::tmp()
{
  path tmp = boost::filesystem::temp_directory_path();
  return string(tmp.string());
}

string os::root()
{
  path root = boost::filesystem::current_path().root_directory();
  return string(root.string());
}

} // namespace arken
