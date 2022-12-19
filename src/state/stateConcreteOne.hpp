
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

/*
 * Concrete state
 */
class StateConcreteOne : public State<ConcreteContext> {
 public:
  /**
   * @brief constructor with Entry method
   * @param context - context object
   */
  StateConcreteOne(ConcreteContext* context) : State(context) {
    DoEnter();
  }

  /**
   * @brief performs main operations in state.
   * @param context reference
   * @return optional new state
   */
  [[nodiscard]] std::optional<std::unique_ptr<State<ConcreteContext>>> DoServe();

 protected:
  /**
   * @brief this method should be executed only in constructor
   */
  void DoEnter();

};

}  // end of namespace state
