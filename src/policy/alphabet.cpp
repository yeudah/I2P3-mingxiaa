#include <cstdlib>
#include <math.h>
#include <limits.h>
#include <iostream>


#include "../state/state.hpp"
#include "./alphabet.hpp"

//typedef std::pair<size_t, size_t> Point;
//typedef std::pair<Point, Point> Move;

int alphaa(State* state, int depth, bool maximizingPlayer, int a,int b){

    if (depth == 0||state->legal_actions.empty()||state->game_state==WIN){
        if(state->score)return state->score;
        return state->evaluate();
    }
    if (maximizingPlayer){
        int value = INT_MIN;
        for (auto &it : state->legal_actions){
            State* next = state->next_state(it);
            value = std::max(value,alphaa(next, depth - 1, false, a,b) );
            a=std::max(a,value);
            if(a>=b)break;
        }
        return value;
    }
    else {
        int value = INT_MAX;
        for (auto &it :state->legal_actions){
            State* next = state->next_state(it);
            value = std::min(value,alphaa(next, depth - 1, true,a,b));
            b=std::min(b,value);
            if(b<=a)break;
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
Move Alpha::get_move(State *state, int depth){

  if(!state->legal_actions.size())
    state->get_legal_actions();
  Move best_action = state->legal_actions[0];
  int largest = INT_MIN, store;
  int a=INT_MIN,b=INT_MAX;

  for(auto act:state->legal_actions){
    auto nextstate=state->next_state(act);
    store=alphaa(nextstate,depth,true,a,b);
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
