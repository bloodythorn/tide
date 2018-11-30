#include "tide/games/cards/playingcard.hpp"

namespace tide { namespace Games { namespace Cards {

PlayingCard::PlayingCard() { }

PlayingCard::PlayingCard(const PlayingCard& p_ot) {
  typeid(p_ot).name();
}

PlayingCard::PlayingCard(PlayingCard&& p_ot) {
  typeid(p_ot).name();
}

PlayingCard::~PlayingCard() { }

PlayingCard& PlayingCard::operator=(const PlayingCard& p_ot) {
  typeid(p_ot).name();
  return *this;
}

PlayingCard& PlayingCard::operator=(PlayingCard&& p_ot) {
  typeid(p_ot).name();
  if(this == &p_ot) return *this;
  return *this;
}

}/*Cards*/}/*Games*/}/*tide*/