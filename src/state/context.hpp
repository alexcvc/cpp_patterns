//
// Copyright (c) 2022 Alexander Sacharov <a.sacharov@gmx.de>
//               All rights reserved.
//
// This work is licensed under the terms of the MIT license.
// For a copy, see <https://opensource.org/licenses/MIT>.
//

/************************************************************************/ /**
* @file
* @brief Context interface for Context class in states.
* @details Context stores a reference to one of the concrete state objects
* and delegates to it all state-specific work. The context communicates with
* the state object via the state interface. The context exposes a setter for
* passing it a new state object.
****************************************************************************/

#ifndef SRC_STATE_CONTEXT_HPP
#define SRC_STATE_CONTEXT_HPP

//-----------------------------------------------------------------------------
// includes
//-----------------------------------------------------------------------------
#include <chrono>
#include "state.hpp"

//----------------------------------------------------------------------------
// Public Function Prototypes
//----------------------------------------------------------------------------

namespace state {

/**
* @class Context
* @brief Context interface for Context class in states
*/
class Context {
  friend class State<Context>;

 public:
  Context() {}

  /**
   * @brief Serves some work in the state. In a concrete implementation this works in a thread.
   * @param next_sooner is the interval of the nearest service.
   * @return interval of the nearest service
   */
  [[nodiscard]] virtual std::chrono::milliseconds Serve(const std::chrono::milliseconds& next_sooner) = 0;

 protected:
  std::unique_ptr<State<Context>> m_state;  ///< current state
};

}  // namespace state

#endif /* SRC_STATE_CONTEXT_HPP */
