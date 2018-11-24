#ifndef LOOP_HPP
#define LOOP_HPP

#include <chrono>  // All the time functions needed
#include <ostream> // Output Streams
#include <string>

namespace tide {
namespace Engine {

class Loop {
public:

  /* Type Aliases */
  using Clock = std::chrono::steady_clock;
  using Time  = std::chrono::time_point<Clock>;
  using NanoSeconds = std::chrono::nanoseconds;
  using Delta = std::chrono::duration<double>;

  /* Module Name */
  static const std::wstring MODULE_NAME;

  /* CTor/Xtor */

  /** @brief Only Constructor
    *
    * @param uint8_t representing frames per second.
    *
    */
  Loop(uint8_t);

  ~Loop(void);

protected:

  /* Member Functions */

  /** @brief Starts the loop */
  void start(void);

  /** @brief Stops the loop */
  void stop(void);

  /** @brief Returns the current FPS setting
    *
    * @return uint8_t containing FPS.
    *
    */
  uint8_t getFPS(void);

  /** @brief Returns current frame
    *
    * @return uint64_t containing current frame number.
    *
    */
  uint64_t getFrame(void);

  /** @brief Sets desired FPS Rate
    *
    * Given a uint8_t, this will set the current frame rate the loop is
    * running at. This will also reset the current frame back to 1.
    *
    * @param uint8_t Desired FPS
    *
    */
  void setFPS(uint8_t);

  /** @brief Returns average FPS
    *
    * This will return the current average fps the loop is running on based on
    * the frame, and the current elapsed time.
    *
    * @return double containing average fps.
    *
    */
  double getAvgFPS(void);

private:

  /* Data members */
  bool        m_isRunning{false};
  uint64_t    m_frame{1};
  uint8_t     m_fps{0};
  NanoSeconds m_targetFrameTime{0};
  Time        m_start;
  Time        m_lastHandle;
  Time        m_lastUpdate;
  Time        m_lastRefresh;

  /* Member Functions */

  /** @brief resets everything to a starting state.
    *
    * This will reset all Times to current clock and reset the frame back to 1.
    *
    */
  void reset(void);

  /* Virtual Functions */

  virtual void handle(double) = 0;
  virtual void update(double) = 0;
  virtual void render(double) = 0;
};
}
}

#endif // LOOP_HPP

