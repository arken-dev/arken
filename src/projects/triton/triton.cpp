#include <triton.h>
#include <QDebug>
#include <oberon/helper>
#include <OStringList>


QQueue<QByteArray *> * Triton::s_queue = s_queue = new QQueue<QByteArray *>();
QMutex Triton::s_mutex;

int Triton::s_ok      = 0;
int Triton::s_error   = 0;
int Triton::s_pending = 0;
QByteArray * Triton::s_result = new QByteArray();


Triton::Triton(int argc, char * argv[], const char * path, QByteArray fileName)
{
  m_argc = argc;
  m_argv = argv;
  m_path = path;
  m_fileName = fileName;

  m_state = Oberon::init(m_argc, m_argv, m_path);
  triton_register(m_state);
}

Triton::~Triton()
{

}

void Triton::run()
{

  lua_settop(m_state, 0);
  lua_getglobal(m_state, "OBERON_PATH");

  int rv;
  rv = luaL_loadfile(m_state, m_fileName);
  if (rv) {
    fprintf(stderr, "%s\n", lua_tostring(m_state, -1));
    throw;
  }

  rv = lua_pcall(m_state, 0, 0, lua_gettop(m_state) - 1);
  if (rv) {
    fprintf(stderr, "%s\n", lua_tostring(m_state, -1));
    throw;
  }

  while( true ) {
    QByteArray * file_name = dequeue();
    if( file_name == 0 ) {
      break;
    }
    lua_settop(m_state, 0);
    lua_getglobal(m_state, "triton_run");

    lua_pushstring(m_state, *file_name);

    if( lua_pcall(m_state, 1, 0, lua_gettop(m_state) - 1 ) != 0 ) {
      fprintf(stderr, "%s\n", lua_tostring(m_state, -1));
      throw;
    }
  }

}

void Triton::init(QStringList list)
{
  if( s_queue == 0 ) {
    s_queue = new QQueue<QByteArray *>();
    for(int i=1; i < list.size(); i++) {
      QFileInfo fileInfo(list.at(i));
      if( fileInfo.isDir() ) {
        OStringList * l = os::glob(list.at(i).toLocal8Bit().data(), ".*.lua$", true);
        for(int j=0; j < l->size(); j++) {
          s_queue->append(new QByteArray(l->at(j)));
        }
      } else {
        s_queue->append(new QByteArray(list.at(i).toLocal8Bit()));
      }
    }
  }
}

QByteArray * Triton::dequeue()
{

  QMutexLocker ml(&s_mutex);

  if( s_queue->empty() ) {
    return 0;
  } else {
    return s_queue->dequeue();
  }
}

int Triton::ok()
{
  return s_ok;
}

int Triton::error()
{
  return s_error;
}

int Triton::pending()
{
  return s_pending;
}

void Triton::addOk()
{
  QMutexLocker ml(&s_mutex);
  s_ok++;
}

void Triton::addError()
{
  QMutexLocker ml(&s_mutex);
  s_error++;
}

void Triton::addPending()
{
  QMutexLocker ml(&s_mutex);
  s_pending++;
}

void Triton::appendResult(const char * result)
{
  QMutexLocker ml(&s_mutex);
  s_result->append(result);
}

void Triton::appendPath(const char * path)
{
  s_queue->append(new QByteArray(path));
}

QByteArray * Triton::result()
{
  return s_result;
}
