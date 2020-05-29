#ifndef APP_FRAMEWORK_INCLUDE_APP_FRAMEWORK_QUEUES_NAMEDSTDDEQUEUE_HH_
#define APP_FRAMEWORK_INCLUDE_APP_FRAMEWORK_QUEUES_NAMEDSTDDEQUEUE_HH_

/**
 *
 * @file A deque
 *
 * This is part of the DUNE DAQ Application Framework, copyright 2020.
 * Licensing/copyright details are in the COPYING file that you should have
 * received with this code.
 */

#include "app-framework-base/NamedQueueI.hh"

#include <atomic>
#include <cassert>
#include <chrono>
#include <condition_variable>
#include <deque>
#include <functional>
#include <iostream>
#include <mutex>
#include <sstream>
#include <string>
#include <thread>
#include <type_traits>
#include <utility>

namespace appframework {

template<class T>
class NamedStdDeQueue
  : public NamedQueueI<T>
{
public:
  using value_type = T;
  using duration_type = typename NamedQueueI<T>::duration_type;

  // This is needed in order for all signatures of the functions of
  // this name in the base class to be accessible here, given that a
  // subset are overridden

  // using QueueSink<T, DurationType>::push;

  NamedStdDeQueue(const std::string& name, size_t size);

  bool can_pop() const noexcept override { return fSize.load() > 0; }
  value_type pop(const duration_type&)
    override; // Throws std::runtime_error if a timeout occurs

  bool can_push() const noexcept override
  {
    return fSize.load() < fMaxSize;
  }
  void push(value_type&&, const duration_type&)
    override; // Throws std::runtime_error if a timeout occurs

  // Delete the copy and move operations since various member data instances
  // (e.g., of std::mutex or of std::atomic) aren't copyable or movable

  NamedStdDeQueue(const NamedStdDeQueue&) = delete;
  NamedStdDeQueue& operator=(const NamedStdDeQueue&) = delete;
  NamedStdDeQueue(NamedStdDeQueue&&) = delete;
  NamedStdDeQueue& operator=(NamedStdDeQueue&&) = delete;

private:
  void try_lock_for(std::unique_lock<std::mutex>&, const duration_type&);
  
  const size_t fMaxSize;

  std::deque<value_type> fDeque;
  std::atomic<size_t> fSize = 0;

  std::mutex fMutex;
  std::condition_variable fNoLongerFull;
  std::condition_variable fNoLongerEmpty;
};

#include "detail/NamedStdDeQueue.icc"

} // namespace appframework

#endif // APP_FRAMEWORK_INCLUDE_APP_FRAMEWORK_QUEUES_NAMEDSTDDEQUEUE_HH_
