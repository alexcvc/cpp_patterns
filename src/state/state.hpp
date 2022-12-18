//
// Copyright (c) 2022 Alexander Sacharov <a.sacharov@gmx.de>
//               All rights reserved.
//
// This work is licensed under the terms of the MIT license.
// For a copy, see <https://opensource.org/licenses/MIT>.
//

#ifndef SRC_STATE_STATE_HPP
#define SRC_STATE_STATE_HPP

//------------------------------------------------------------------------------
// includes
//------------------------------------------------------------------------------
#include <memory>
#include <optional>

//------------------------------------------------------------------------------
// Declarations
//------------------------------------------------------------------------------

namespace state {

/**
 * @class Template for state
 * @brief describes interface for state in state pattern
 * @details Context stores a reference to one of the concrete state objects and delegates to it all state-specific work.
 * The context communicates with the state object via the state interface. The context exposes a setter for passing
 * it a new state object as unique pointer (smart pointer). New state should be return only with move 
 * std::make_unique<State>(context).
 * The State interface declares the state-specific methods. These methods should make sense for all concrete states
 * because you don’t want some of your states to have useless methods that will never be called.
 */
template <class TContext>
class State {
 public:
  using StateUp = std::unique_ptr<State<TContext>>;

  /**
   * @brief Constructor
   * @param context
   */
  State(TContext& context) : m_context(context) {}

  /**
   * @brief performs main operations in state.
   * @details performs any operation relates to timer or immediately
   * @param context reference
   * @return optional new state
   */
  [[nodiscard]] virtual std::optional<StateUp> DoServe() = 0;

 protected:
  TContext& m_context;

  /**
   * @brief this method should be executed only in constructor
   * @details all necessary things that are executed once in the entry are implemented here
   */
  virtual void DoEnter() = 0;
};

}  // end of namespace state

#endif /* SRC_STATE_STATE_HPP */