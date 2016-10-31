#include <puck.h>
#include <dialog.h>
#include <iostream>
#include <QDebug>
#include <QThread>
#include <OByteArray>
#include <oberon/helper>
#include <OStringList>

Puck::Puck(QApplication &app, QObject *parent) : QObject(parent)
{
  QStringList arguments = app.arguments();

  if( arguments.size() == 1 ) {
    qDebug() << "invalid file";
    throw;
  }

  m_file = arguments.at(1);

  m_watcher = new QFileSystemWatcher ;
  qDebug() << "m_file " << m_file;

  m_watcher->addPath(m_file);
  if( os::exists("app/models") ) {
    OStringList * list = os::glob("app/models", "\\.lua$", true);
    for(int i = 0; i < list->size(); i++) {
      qDebug() << list->at(i);
      m_watcher->addPath(list->at(i));
    }
    delete list;
  }
  connect(m_watcher, SIGNAL(fileChanged(QString)), this, SLOT(showFileModified(QString)));

  //m_dialog->setParams(argc, argv);

  // instance
  m_luaState = lua_open();
  luaL_openlibs(m_luaState);

  // OBERON_PATH
  int rv;
  QString dirPath;
  OByteArray oberonPath;
  OByteArray profile;
  OByteArray puck;

  dirPath = app.applicationDirPath();
  dirPath.truncate( dirPath.lastIndexOf('/') );
  oberonPath = dirPath.toLocal8Bit();
  if( strcmp(os::name(), "windows") == 0 ) {
    oberonPath = oberonPath.capitalize();
  }
  lua_pushstring(m_luaState, oberonPath);
  lua_setglobal(m_luaState, "OBERON_PATH");

  //profile
  profile = oberonPath;
  profile.append("/profile.lua");

  rv = luaL_loadfile(m_luaState, profile);
  if (rv) {
    fprintf(stderr, "%s\n", lua_tostring(m_luaState, -1));
    throw;
    //return rv;
  }

  rv = lua_pcall(m_luaState, 0, 0, lua_gettop(m_luaState) - 1);
  if (rv) {
    fprintf(stderr, "%s\n", lua_tostring(m_luaState, -1));
    throw;
    //return rv;
  }

  //puck
  profile = oberonPath;
  profile.append("/lib/puck.lua");

  rv = luaL_loadfile(m_luaState, profile);
  if (rv) {
    fprintf(stderr, "%s\n", lua_tostring(m_luaState, -1));
    throw;
    //return rv;
  }

  rv = lua_pcall(m_luaState, 0, 0, lua_gettop(m_luaState) - 1);
  if (rv) {
    fprintf(stderr, "%s\n", lua_tostring(m_luaState, -1));
    throw;
    //return rv;
  }

  m_dialog = new Dialog;

  this->run();

}

Puck::~Puck()
{

}

void Puck::showFileModified(const QString &str)
{
  qDebug() << "1showFileModified\n";
  qDebug() << str.toLocal8Bit().data();
  qDebug() << "size" << str.size();

  QString title = str.right(str.size() - str.indexOf("specs"));
  QThread::msleep(200);
  while (true) {
    if( QFile::exists(str) ) {
      m_watcher->addPath(str);
      break;
    } else {
      qDebug() << "waiting...";
      QThread::msleep(50);
    }
  }
  this->run();
}

void Puck::showDirectoryModified(const QString &str)
{
  qDebug() << "showDirectoryModified\n";
  qDebug() << str.toLocal8Bit().data();
  qDebug() << "size" << str.size();
  QThread::msleep(200);
  m_watcher->addPath(str);
}

void Puck::run()
{

  lua_settop(m_luaState, 0);
  lua_getglobal(m_luaState, "puck");
  lua_pushstring(m_luaState, m_file.toLocal8Bit().data());
  if( lua_pcall(m_luaState, 1, 1, 0 ) != 0 ) {
    m_dialog->send(m_file, lua_tostring(m_luaState, -1));
  } else {
    m_dialog->send(m_file, lua_tostring(m_luaState, 1));
  }

}
