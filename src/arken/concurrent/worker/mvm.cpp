#include <arken/concurrent/worker.h>


namespace arken {
namespace concurrent {

void worker::perform(unsigned int cores)
{

  std::vector<worker::node *> list;

  for(unsigned int i=0; i < cores; i++) {
    auto n = new worker::node(this, m_fileName, i, m_release);
    list.push_back(new worker::node(*n));
    core::start(n);
  }

  while( true ) {
    bool flag = true;
    for(worker::node *n : list) {
      if( n->finished() == false ) {
        flag = false;
      }
    }
    if( flag ) {
      break;
    }
    os::sleep(0.05);
  }

  for(worker::node *n : list) {
    delete n;
  }

}

} // namespace concurrent
} // namespace arken
