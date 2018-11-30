#ifndef PLAYINGCARD_HPP
#define PLAYINGCARD_HPP

#include <ostream>

namespace tide { namespace Games { namespace Cards {

class PlayingCard {
public:

  /* Ctor/Xtor */
  PlayingCard();

  PlayingCard(const PlayingCard& p_ot);

  PlayingCard(PlayingCard&& p_ot);

  ~PlayingCard();

  /* Operator Overloads */
  PlayingCard& operator=(const PlayingCard& p_ot);

  PlayingCard& operator=(PlayingCard&& p_ot);

private:

  friend std::ostream& operator<<(std::ostream& p_os, const PlayingCard& p_ot) {
    p_os << typeid(p_ot).name();
    return p_os;
  }
  friend std::wostream& operator<<(std::wostream& p_os, const PlayingCard& p_ot) {
    p_os << typeid(p_ot).name();
    return p_os;
  }
};

}/*Cards*/}/*Games*/}/*tide*/

#endif // PLAYINGCARD_HPP
