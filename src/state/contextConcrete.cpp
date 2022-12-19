
//-----------------------------------------------------------------------------
// includes
//-----------------------------------------------------------------------------
#include "contextConcrete.hpp"

#include <chrono>
#include <filesystem>
#include <fstream>
#include <memory>
#include <spdlog/spdlog.h>

#include "stateConcreteOne.hpp"

using namespace state;
using state::State;

//----------------------------------------------------------------------------
// Public Function Prototypes
//----------------------------------------------------------------------------

namespace state {

std::chrono::milliseconds ConcreteContext::Serve(const std::chrono::milliseconds& next_sooner) {
  using namespace std::chrono_literals;

  auto res_sooner{next_sooner};

  // once started
  if (!m_state) {
    m_state.reset(new StateConcreteOne(this));
  }

  // handle state
  if (auto result = m_state->DoServe(); result.has_value()) {
    m_state = std::move(result.value());
  }

  // estimate next sooner for timer
  if (m_timer.IsRunning()) {
    if ( auto sooner = m_timer.LeftTime(); sooner < res_sooner ) {
      res_sooner = sooner;
    }
  }

  if (res_sooner.count() <= 0) {
    res_sooner = 1ms;
  }
  return res_sooner;
}

}  // end of namespace state
