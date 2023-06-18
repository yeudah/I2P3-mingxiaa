#pragma once
#include "../state/state.hpp"
#include <utility>

 
/**
 * @brief Policy class for random policy, 
 * your policy class should have get_move method
 */
typedef std::pair<size_t, size_t> Point;
typedef std::pair<Point, Point> Move;
class MinMax{
public:
  Move duar;
  static Move get_move(State *state, int depth);
  friend int make_node(State *state, int depth, bool maximizingPlayer, int*ind);
};