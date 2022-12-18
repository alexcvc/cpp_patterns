//
// Copyright (c) 2022 Alexander Sacharov <a.sacharov@gmx.de>
//               All rights reserved.
//
// This work is licensed under the terms of the MIT license.
// For a copy, see <https://opensource.org/licenses/MIT>.
//

#ifndef SRC_INCLUDE_STOP_TIMER_HPP
#define SRC_INCLUDE_STOP_TIMER_HPP

//-----------------------------------------------------------------------------
// includes
//-----------------------------------------------------------------------------
#include <chrono>
#include <optional>

//----------------------------------------------------------------------------
// Public Prototypes
//----------------------------------------------------------------------------

namespace watch {

/**
* \brief This is a easy stop timer wrapper class.
* Timer allows to set timeout and to check elapsed of timer
*
*/
template <class TDuration = std::chrono::milliseconds>
class StopTimer {
 public:
  /** types */
  using Clock = std::chrono::steady_clock;
  using TimePoint = std::chrono::time_point<StopTimer::Clock, TDuration>;

  /**
   * @brief constructors
   */
  StopTimer() = default;
  StopTimer(TDuration timeout) : m_timeout_duration(timeout) {}

  /**
   * @brief get timeout
   * @tparam TUnit - to-duration unit type
   * @return duration in to-duration unit type
   */
  template <typename TUnit = TDuration>
  [[nodiscard]] TUnit Timeout() const noexcept {
    return std::chrono::duration_cast<TUnit>(m_timeout_duration);
  }

  /**
   * @brief set timeout in duration unit
   * @tparam TUnit - from-duration unit type
   * @param timeout
   */
  template <typename TUnit = TDuration>
  void SetTimeout(const TUnit& timeout) noexcept {
    m_timeout_duration = std::chrono::duration_cast<TDuration>(timeout);
  }

  /**
   * @brief is timer has been started (pushed)
   * @return true if is running, otherwise - false
   */
  [[nodiscard]] bool IsRunning() const noexcept {
    return m_is_running;
  }

  /**
   * @brief reset running flag and reset start point in timer
   */
  void Reset() noexcept {
    m_is_running = false;
    m_start_point = {};
  }

  /**
   * @brief stop running only without reset start point
   */
  void Stop() noexcept {
    m_is_running = false;
  }

  /**
   * @brief restart timer with setup start point to now time point
   * @return start time point
   */
  TimePoint Start() noexcept {
    m_is_running = true;
    m_start_point = StopTimer::CurrentTime();
    return m_start_point;
  }

  /**
   * @brief start timer with setup timeout( reset start point to now time point )
   * @tparam TUnit - duration unit
   * @param new_timeout - new timeout in duration unit
   * @return start time point
   */
  template <typename TUnit = TDuration>
  TimePoint Start(TUnit new_timeout) noexcept {
    SetTimeout<TUnit>(new_timeout);
    m_is_running = true;
    m_start_point = StopTimer::CurrentTime();
    return m_start_point;
  }

  /**
   * @brief is timeout interval elapsed that defined timeout interval was elapsed
   * @return optional boolean
   *        1. is not running - hasn't value - std::nullopt
   *        2. running but timeout is equal zero - true
   *        3. running with timeout > zero: true if timeout elapsed, otherwise - false
   */
  [[nodiscard]] std::optional<bool> IsElapsed() noexcept {
    if (!m_is_running) {
      // timer is not running
      return std::nullopt;
    } else if (m_timeout_duration.count() == 0) {
      // is running with 0 timeout
      return true;
    } else {
      return (ElapsedTime() > m_timeout_duration);
    }
  }

  /**
   * @brief elapsed timeout since start
   * @tparam TUnit - duration unit
   * @return elapsed time since start point
   */
  template <typename TUnit = TDuration>
  [[nodiscard]] TUnit ElapsedTime() noexcept {
    if (IsRunning()) {
      return std::chrono::duration_cast<TUnit>(StopTimer::CurrentTime() - m_start_point);
    } else {
      return TUnit{};
    }
  }

  /**
   * @brief left time up to timeout point
   * @tparam TUnit - duration unit
   * @return left time
   */
  template <typename TUnit = TDuration>
  [[nodiscard]] TUnit LeftTime() noexcept {
    if (IsRunning()) {
      return std::chrono::duration_cast<TUnit>(m_timeout_duration - ElapsedTime());
    } else {
      return TUnit{};
    }
  }

 private:
  /**
   * @brief current time
   * @return timepoint
   */
  [[nodiscard]] static inline TimePoint CurrentTime() noexcept {
    return std::chrono::time_point_cast<TDuration>(Clock::now());
  }

 private:
  StopTimer::TimePoint m_start_point{};  ///< start time point
  TDuration m_timeout_duration{};        ///< timeout
  bool m_is_running{false};              ///< is running
};

/**
 * @brief useful types for replace template type like standard library std::string
 * TimerMs timerMs;
 */
using TimerMs = watch::StopTimer<std::chrono::milliseconds>;

}  // namespace watch

#endif /* SRC_INCLUDE_STOP_TIMER_HPP */
