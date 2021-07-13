#include <arken/concurrent/worker.h>


namespace arken {
namespace concurrent {

void working(worker::node * n)
{
  n->run();
  delete n;
}

void worker::perform(unsigned int cores)
{

  std::vector<std::thread *> threads;

  for(unsigned int i=0; i < cores; i++) {
    node *n = new node(this, m_fileName, i);
    threads.push_back(new std::thread(working, n));
  }

  for(std::thread *t : threads) {
    t->join();
  }

  for(std::thread *t : threads) {
    delete t;
  }

}

} // namespace concurrent
} // namespace arken
