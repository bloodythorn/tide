#include "tide/engine/loop.hpp"

#include <cmath>  // std::pow
#include <thread> // std::this_thread::sleep_for

#include "tide/engine/logger.hpp"

namespace tide{ namespace Engine {

const std::wstring Loop::MODULE_NAME{L"Loop"};

Loop::Loop(uint8_t p_fps) { setFPS(p_fps); }

Loop::~Loop(void) { }

double Loop::getAvgFPS(void) {
  return 1.0f / Delta(((Clock::now() - m_start) / m_frame)).count();
}

uint64_t Loop::getFrame(void) { return m_frame; }

void Loop::reset(void) {
  m_frame = 1;
  m_start = m_lastHandle = m_lastUpdate = m_lastRefresh = Clock::now();
}

void Loop::setFPS(uint8_t p_fps) {
  reset();
  m_fps = p_fps;
  m_targetFrameTime =
    NanoSeconds(
      static_cast<uint32_t>(
        std::pow(10,9) / static_cast<double>(p_fps)
      )
    );
}

void Loop::start(void) {
  LOG_DEBUG(MODULE_NAME) << __func__ << ":Starting Loop.";
  reset();

  /* Framing Lambda */
  auto frame = [&](){
    auto delta = Clock::now() - m_lastHandle; m_lastHandle = Clock::now();
    handle(Delta(delta).count());
    delta = Clock::now() - m_lastUpdate; m_lastUpdate = Clock::now();
    update(Delta(delta).count());
    m_lastUpdate = Clock::now();
    delta = Clock::now() - m_lastRefresh; m_lastRefresh = Clock::now();
    render(Delta(delta).count());
    ++m_frame;
  };

  m_isRunning = true;
  while(m_isRunning) {
    auto runTime = Clock::now() - m_start;
    auto nextFrame = m_targetFrameTime * m_frame;

    if(runTime > nextFrame) { frame(); }
    else { std::this_thread::sleep_for(nextFrame - runTime); frame(); }
  }
}

void Loop::stop(void) { m_isRunning = false; }

}/*Engine*/}/*tide*/