
#include "stateConcreteTwo.hpp"

#include <chrono>
#include <memory>
#include <optional>
#include <spdlog/spdlog.h>

#include "contextConcrete.hpp"
#include "stateConcreteOne.hpp"

using namespace std::chrono_literals;

//-----------------------------------------------------------------------------
// global Function Definitions
//-----------------------------------------------------------------------------

namespace state {

void StateConcreteTwo::DoEnter() {
  spdlog::info("enter to  state 2. Wait 6 sec");
  // start timer for recreating the file
  m_contextPtr->TimerRestart(6s);
}

std::optional<std::unique_ptr<State<ConcreteContext>>> state::StateConcreteTwo::DoServe() {
  if (auto is_elapsed = m_contextPtr->Timer().IsElapsed(); is_elapsed.has_value() && is_elapsed.value()) {
    spdlog::info("goto state 1");
    // go to Idle mode
    return std::make_unique<StateConcreteOne>(m_contextPtr);
  }
  return std::nullopt;
}

}  // namespace state
