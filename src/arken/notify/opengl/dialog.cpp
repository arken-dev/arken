#include <mutex>
#include <queue>
#include <thread>
#include <arken/mvm.h>
#include <GL/freeglut.h>
#include <GL/glut.h>
#include <iostream>
#include <thread>
#include <string>
#include <arken/base>
#include "dialog.h"

using arken::mvm;

static std::mutex s_mtx;
static std::queue<NotifyNode *> s_queue;
static double s_time = 0;
std::string message = "";

Dialog *dialog = nullptr;

void run()
{
  std::cout << "before create dialog" << dialog << std::endl;
  glutInit(&mvm::s_argc, mvm::s_argv);
  dialog = new Dialog();
  std::cout << "after create dialog" << dialog << std::endl;
  glutMainLoop();
}

void load()
{
  if( dialog == nullptr ) {
    std::cout << "run... " << std::endl;
    new std::thread(run);
  }
} //NOLINT(clang-analyzer-cplusplus.NewDeleteLeaks)

void RenderString(GLdouble x, GLdouble y, const std::string &string)
{
  glColor3d(3.0, 3.0, 3.0);
  std::cout << "x " << x << std::endl;
  std::cout << "y " << y << std::endl;

  glRasterPos2f(x-0.99, y+0.7);
  //glRasterPos2d(x, y);
  int count = 0;
  float position = 0.7;
  for (int n = 0; n<string.size(); ++n) {

    if ( string[n] == '\n' ) {
      while( count < 49 )  {
        glutBitmapCharacter(GLUT_BITMAP_8_BY_13, ' ');
        count++;
      }
    } else {
      glutBitmapCharacter(GLUT_BITMAP_8_BY_13, string[n]);
    }

    count++;
    if( count >= 49) {
      count = 0;
      position -= 0.30;
      std::cout << "teste" << position << std::endl;
      glRasterPos2f(x-0.99, y+position);
    }

    //glutBitmapCharacter(GLUT_BITMAP_HELVETICA_12, string[n]);
    //if( n >
    //glutBitmapCharacter(GLUT_BITMAP_9_BY_15, string[n]);
  }
}

void displayMe(void)
{
    glClear(GL_COLOR_BUFFER_BIT);
    RenderString(0, 0, message.data());
    //glBegin(GL_POLYGON);
    //    glVertex3f(0.0, 0.0, 0.0);
    //    glVertex3f(0.5, 0.0, 0.0);
    //    glVertex3f(0.5, 0.5, 0.0);
    //    glVertex3f(0.0, 0.5, 0.0);
    glEnd();
    glFlush();
}

void timer(int )
{
  std::cout << "IN DEVELPOMENT timer " << std::endl;
    if(!glutGetWindow())
        return ;
    std::cout << "s_time " << s_time << std::endl;

    std::unique_lock<std::mutex> lck(s_mtx);

    if( os::microtime() - s_time >= 10 ) {
      glutHideWindow();
    }

    if( !s_queue.empty() ) {
      s_time = os::microtime();
      NotifyNode * node = s_queue.front();
      s_queue.pop();

      message.clear();
      message.append(node->m_title);
      message.append("\n\n");
      message.append(node->m_message);

      glutHideWindow();
      glutDisplayFunc(displayMe);
      glutShowWindow();
    }

    /*
    glutInitWindowSize(400, 100);
    //glutInitWindowPosition(100, 100);
    //std::cout << "height" << glutGet(GLUT_SCREEN_HEIGHT) << std::endl;
    //std::cout << "width" << glutGet(GLUT_SCREEN_WIDTH) << std::endl;

    glutInitWindowPosition (
      (glutGet(GLUT_SCREEN_WIDTH)-400),
      0//(glutGet(GLUT_SCREEN_HEIGHT)-100)
    );
    glutCreateWindow("lelelele");
    glutDisplayFunc(displayMe);
    glutTimerFunc(5000, timer, 1);

    //glutMainLoopEvent();
    */
  glutTimerFunc(100, timer, 1);
}

NotifyNode::NotifyNode(const char * status, const char * title, const char * message) {
  m_status  = status;
  m_title   = title;
  m_message = message;
}

Dialog::Dialog()
{
  std::cout << "construct Diaglog" << std::endl;
  glutInitDisplayMode(GLUT_SINGLE | GLUT_DEPTH);
  //glutInitDisplayMode(GLUT_SINGLE | GLUT_DEPTH | GLUT_FULL_SCREEN);//GLUT_RGBA|GLUT_SINGLE|GLUT_DEPTH|GLUT_BORDERLESS|GLUT_CAPTIONLESS);
  glutInitWindowSize(400, 100);
  //glutInitWindowPosition(100, 100);
  //std::cout << "height" << glutGet(GLUT_SCREEN_HEIGHT) << std::endl;
  //std::cout << "width" << glutGet(GLUT_SCREEN_WIDTH) << std::endl;

  glutInitWindowPosition (
    (glutGet(GLUT_SCREEN_WIDTH)-400),
    0//(glutGet(GLUT_SCREEN_HEIGHT)-100)
  );

  glutCreateWindow("Notify");
  glutHideWindow();
  //glutIconifyWindow();

  //glutDisplayFunc(displayMe);
  s_time = os::microtime();
  glutTimerFunc(100, timer, 1);
}

void Dialog::send(NotifyNode* node)
{

  std::unique_lock<std::mutex> lck(s_mtx);

  std::cout << " before thread " << dialog << " app " << std::endl;

  load();

  s_queue.push(node);
}
