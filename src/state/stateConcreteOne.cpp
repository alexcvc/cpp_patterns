
#include "stateConcreteOne.hpp"

#include <chrono>
#include <memory>
#include <optional>

#include "contextConcrete.hpp"
#include "stateConcreteTwo.hpp"

using namespace std::chrono_literals;

//-----------------------------------------------------------------------------
// global Function Definitions
//-----------------------------------------------------------------------------

namespace state {

void StateConcreteOne::DoEnter() {
  // start timer for recreating the file
  m_context.TimerRestart(2s);
}

std::optional<std::unique_ptr<State<ConcreteContext>>> state::StateConcreteOne::DoServe() {
  if (auto is_elapsed = m_context.Timer().IsElapsed(); is_elapsed.has_value() && is_elapsed.value()) {
    // go to Idle mode
    return std::make_unique<StateConcreteTwo>(m_context);
  }
  return std::nullopt;
}

}  // namespace state