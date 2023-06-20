#include <cstdlib>
#include <math.h>
#include <limits.h>
#include <iostream>


#include "../state/state.hpp"
#include "./greedy.hpp"

//typedef std::pair<size_t, size_t> Point;
//typedef std::pair<Point, Point> Move;

int greedd(State* state, int depth, bool maximizingPlayer){

    if (depth == 0||state->legal_actions.empty()||state->game_state==WIN||state->game_state==DRAW){
        if(state->score)return state->score;
        return state->evaluate();
    }
    if (maximizingPlayer){
        int value = INT_MIN;
        for (auto &it : state->legal_actions){
            State* next = state->next_state(it);
            value = std::max(value,greedd(next, depth - 1, false) );
        }
        return value;
    }
    else {
        int value = INT_MAX;
        for (auto &it :state->legal_actions){
            State* next = state->next_state(it);
            value = std::min(value,greedd(next, depth - 1, true));
        }
  
        return value;
    }
}
/**
 * @brief Randomly get a legal action
 * 
 * @param state Now state
 * @param depth You may need this for other policy
 * @return Move 
 */
Move Greed::get_move(State *state, int depth){

  if(!state->legal_actions.size())
    state->get_legal_actions();
  Move best_action = state->legal_actions[0];
  int largest = INT_MIN, store;

  for(auto act:state->legal_actions){
    auto nextstate=state->next_state(act);
    store=greedd(nextstate,depth,true);
    if(store>largest){
        best_action=act;
        largest=store;
    }
  }
  return best_action;
}



/**
 * @brief Randomly get a legal action
 * 
 * @param state Now state
 * @param depth You may need this for other policy
 * @return Move 
 */
