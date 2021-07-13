#include <arken/concurrent/worker.h>

namespace arken {
namespace concurrent {

void worker::perform(unsigned int cores)
{

  std::vector<worker::node *> list;

  for(unsigned int i=0; i < cores; i++) {
    worker::node *n = new worker::node(this, m_fileName, i);
    list.push_back(n);
    mvm::concurrent(n);
  }

  while( true ) {
    bool flag = true;
    for(worker::node *n : list) {
      if( n->finished() == false ) {
        flag = false;
      }
    }
    if( flag ) {
      return;
    }
    os::sleep(0.05);
  }

}

} // namespace concurrent
} // namespace arken
