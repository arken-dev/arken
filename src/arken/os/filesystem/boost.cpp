#include <arken/base>
#include <boost/filesystem.hpp>
#include <boost/asio/ip/host_name.hpp>

using namespace boost::filesystem;

char * os::abspath(const char * p)
{
  path path = boost::filesystem::absolute(p);
  size_t size = path.string().size();
  char * result =  new char[size + 1];
  strcpy(result, path.string().c_str());
  result[size] = '\0';
  return result;
}

char * os::basename(const char * p)
{
  path path = boost::filesystem::absolute(p).parent_path();
  size_t size = path.string().size();
  char * result =  new char[size + 1];
  strcpy(result, path.string().c_str());
  result[size] = '\0';
  return result;
}

uint os::atime(const char * p)
{
  std::time_t t = boost::filesystem::last_write_time(p);
  return (uint) t;
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

uint os::ctime(const char * p)
{
  std::time_t t = boost::filesystem::last_write_time(p);
  return (uint) t;
}

char * os::dirpath(const char * p)
{
  path path = boost::filesystem::absolute(p).parent_path();
  size_t size = path.string().size();
  char * result =  new char[size + 1];
  strcpy(result, path.string().c_str());
  result[size] = '\0';
  return result;
}

bool os::exists(const char * path)
{
  return boost::filesystem::exists(path);
}

char * os::home()
{
  const char * p = getenv("HOME");
  if( p == nullptr ) {
    return new char{'\0'};
  } else {
    path path = boost::filesystem::absolute(p);
    size_t size = path.string().size();
    char * result =  new char[size + 1];
    strcpy(result, path.string().c_str());
    result[size] = '\0';
    return result;
  }
}

char * os::hostname()
{
  std::string hostname = boost::asio::ip::host_name();
  char * result = new char[hostname.size() + 1];
  strcpy(result, hostname.c_str());
  result[hostname.size()] = '\0';
  return result;
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

char * os::pwd()
{
  path cwd = boost::filesystem::current_path();
  size_t size = cwd.string().size();
  char * result =  new char[size + 1];
  strcpy(result, cwd.string().c_str());
  result[size] = '\0';
  return result;
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

char * os::target(const char * p)
{
  path target = boost::filesystem::read_symlink(p);
  size_t size = target.string().size();
  char * result =  new char[size + 1];
  strcpy(result, target.string().c_str());
  result[size] = '\0';
  return result;
}

char * os::temp()
{
  path path = boost::filesystem::temp_directory_path();
  size_t size = path.string().size();
  char * result =  new char[size + 1];
  strcpy(result, path.string().c_str());
  result[size] = '\0';
  return result;
}


char * os::root()
{
  path root     = boost::filesystem::current_path().root_directory();
  size_t size   = root.string().size();
  char * result =  new char[size + 1];
  strcpy(result, root.string().c_str());
  result[size] = '\0';
  return result;
}
