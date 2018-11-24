#ifndef NONCOPYABLE_HPP
#define NONCOPYABLE_HPP

namespace tide {
namespace Engine {

class NonCopyable {
protected:
  NonCopyable() {}
  ~NonCopyable() {}
private:
  /** @brief Copy Constructor Disabled */
  NonCopyable(const NonCopyable& p_other) = delete;

  /** @brief Move Constructor Disabled */
  NonCopyable(NonCopyable&& p_other) noexcept = delete;

  /** @brief Copy Assignment Operator Disabled */
  NonCopyable& operator=(const NonCopyable& m_other) = delete;

  /** @brief Move Assignment Operator Disabled */
  NonCopyable& operator=(NonCopyable&& m_other) noexcept = delete;

};
}
}

#endif //NONCOPYABLE_HPP