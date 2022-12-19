
#include "stateConcreteOne.hpp"

#include <chrono>
#include <memory>
#include <optional>
#include <spdlog/spdlog.h>

#include "contextConcrete.hpp"
#include "stateConcreteTwo.hpp"

using namespace std::chrono_literals;

//-----------------------------------------------------------------------------
// global Function Definitions
//-----------------------------------------------------------------------------

namespace state {

void StateConcreteOne::DoEnter() {
  spdlog::info("enter to state 1. Wait 500 ms");
  // start timer for recreating the file
  m_contextPtr->TimerRestart(500ms);
}

std::optional<std::unique_ptr<State<ConcreteContext>>> state::StateConcreteOne::DoServe() {
  if (auto is_elapsed = m_contextPtr->Timer().IsElapsed(); is_elapsed.has_value() && is_elapsed.value()) {
    // go to Idle mode
    spdlog::info("goto state 2");
    return std::make_unique<StateConcreteTwo>(m_contextPtr);
  }
  return std::nullopt;
}

}  // namespace state