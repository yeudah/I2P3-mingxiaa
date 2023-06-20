#pragma once
#include "../state/state.hpp"
#include <utility>

 
/**
 * @brief Policy class for random policy, 
 * your policy class should have get_move method
 */

class Sub{
public:
  static Move get_move(State *state, int depth);
  friend int subb(State *state, int depth, bool maximizingPlayer,int a,int b);
};