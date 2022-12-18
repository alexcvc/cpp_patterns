
#pragma once

//------------------------------------------------------------------------------
// includes
//------------------------------------------------------------------------------
#include "contextConcrete.hpp"
#include "state.hpp"

//------------------------------------------------------------------------------
// Defines and macros
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Declarations
//------------------------------------------------------------------------------

namespace state {

class StateConcreteTwo : public State<ConcreteContext> {
 public:
  /**
   * @brief constructor with Entry method
   * @param observer - observer object
   */
  StateConcreteTwo(ConcreteContext& observer) : State(observer) {
    DoEnter();
  }

  /**
   * @brief performs main operations in state.
   * @param context reference
   * @return optional new state
   */
  [[nodiscard]] std::optional<StateUp> DoServe();

 protected:
  /**
   * @brief this method should be executed only in constructor
   */
  void DoEnter();

};

}  // end of namespace state

