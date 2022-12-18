
/************************************************************************/ /**
* @file
* @brief   Concrete context.
****************************************************************************/

#ifndef SRC_STATE_CONTEXT_CONCRETE_HPP
#define SRC_STATE_CONTEXT_CONCRETE_HPP

//-----------------------------------------------------------------------------
// includes
//-----------------------------------------------------------------------------
#include <filesystem>
#include <optional>
#include <stopTimer.hpp>

#include "context.hpp"

//----------------------------------------------------------------------------
// Public Function Prototypes
//----------------------------------------------------------------------------

namespace state {

/**
* @class Concrete Context
*/
class ConcreteContext : public Context {
  friend class State<ConcreteContext>;

 public:
  /**
   * @brief constructor
   */
  ConcreteContext() : Context() {}

  /**
   * @brief Serves some work.
   * @details Serves some work in the state. In a concrete implementation this works in a thread.
   * @param next_sooner is the interval of the nearest service.
   * @return interval of the nearest service
   */
  [[nodiscard]] std::chrono::milliseconds Serve(const std::chrono::milliseconds& next_sooner);

  /**
   * @brief get reference to timer
   * @return timer reference
   */
  [[nodiscard]] watch::TimerMs& Timer() {
    return m_timer;
  }

  /**
   * @brief restart timer with new duration
   */
  void TimerRestart(const std::chrono::milliseconds timeout) {
    m_timer.Start(timeout);
  }

  /**
   * @brief stop timer
   */
  void TimerStop() {
    m_timer.Reset();
  }

 protected:
  watch::TimerMs m_timer;     ///< timer

};

}  // end of namespace state

#endif /* SRC_STATE_CONTEXT_CONCRETE_HPP */
