#include <arken/concurrent/core.h>

namespace arken {
namespace concurrent {

core::core(uint32_t max)
{
  m_max = max;
}

core::~core()
{
  for( size_t i=0; i< workers().size(); i++ ) {
    queue().push(nullptr);
    actives()++;
    condition().notify_one();
  }

  {
    std::unique_lock<std::mutex> lck(mutex());
    for (std::pair<std::thread::id, concurrent::base *> element : running()) {
      element.second->finished(true);
    }
  }

  while( actives() > 0 ) {
    os::sleep(0.005);
  }

  for( size_t i=0; i< workers().size(); i++ ) {
    workers().at(i).detach();
  }
}

core & core::instance()
{
  static core core(os::cores());
  return core;
}

std::queue<concurrent::base *> & core::queue()
{
  return instance().m_queue;
}

std::mutex& core::mutex()
{
  return instance().m_mutex;
}

std::vector<std::thread>& core::workers()
{
  return instance().m_workers;
}

std::condition_variable  & core::condition()
{
  return instance().m_condition;
}

std::atomic<uint32_t>  & core::actives()
{
  return instance().m_actives;
}

std::atomic<uint32_t>  & core::max()
{
  return instance().m_max;
}

std::unordered_map<string, string> & core::waiting()
{
  return instance().m_waiting;
}

std::unordered_map<std::thread::id, concurrent::base *> & core::running()
{
  return instance().m_running;
}

void core::working()
{

  while( true ) {
    std::unique_ptr<concurrent::base>ptr{get()};

    if(!ptr) {
      std::unique_lock<std::mutex> lck(mutex());
      actives()--;
      return;
    }

    ptr->run();
    ptr->finished(true);

    std::unique_lock<std::mutex> lck(mutex());
    actives()--;
    running().erase(std::this_thread::get_id());
  } // while

} // core::working


void core::start(concurrent::base * ptr)
{
  std::unique_lock<std::mutex> lck(mutex());

  if( workers().size() < max() && (workers().size() - actives()) == 0 ) {
    workers().push_back(std::thread(working));
  }

  queue().push(ptr);
  condition().notify_one();
  actives()++;
  waiting()[ptr->uuid()] = ptr->inspect();
}

base * core::get()
{
  std::unique_lock<std::mutex> lck(mutex());
  condition().wait(lck, []{ return ! queue().empty(); });
  concurrent::base * ptr = queue().front();
  queue().pop();

  if( ptr ) {
    waiting().erase(ptr->uuid());
    running()[std::this_thread::get_id()] = ptr;
  }

  return ptr;
}

} // namespace concurrent
} // namespace arken
