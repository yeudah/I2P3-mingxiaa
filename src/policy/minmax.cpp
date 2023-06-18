#include <cstdlib>
#include <limits.h>


#include "../state/state.hpp"
#include "./minmax.hpp"

//typedef std::pair<size_t, size_t> Point;
//typedef std::pair<Point, Point> Move;

int make_node(State* state, int depth, bool maximizingPlayer, int *index){
  
 
    if (depth == 0 || state->legal_actions.empty()){
        return state->evaluate();
    }
    
    if (maximizingPlayer){
        int value = INT_MIN;
       
        int a=0;
        for (auto &it : state->legal_actions){
            State* potential_state = state->next_state(it);

            int val = make_node(potential_state, depth - 1, false,index);
            if (value < val){
        
                value = val;
               *index=a;
            }
            a++;
        }
        
        return value;
    }
    else {
        int value = INT_MAX;
        int a=0;

        for (auto &it :state->legal_actions){
            State* potential_state = state->next_state(it);
            
            int val = make_node(potential_state, depth - 1, true,index);
            if (value > val){
              
                value = val;
                *index=a;

            }
            a++;
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
Move MinMax::get_move(State *state, int depth){
  if(!state->legal_actions.size())
    state->get_legal_actions();
  
  auto actions = state->legal_actions;
  int index =0;
  make_node(state,depth,true,&index);
  return actions[index];
}



/**
 * @brief Randomly get a legal action
 * 
 * @param state Now state
 * @param depth You may need this for other policy
 * @return Move 
 */

