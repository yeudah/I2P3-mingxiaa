#include <iostream>
#include <sstream>
#include <cstdint>

#include "./state.hpp"
#include "../config.hpp"


/**
 * @brief evaluate the state
 * 
 * @return int 
 */
#include <iostream>
#include <sstream>
#include <cstdint>

#include "./state.hpp"
#include "../config.hpp"
#define PAWN   0
#define KNIGHT 1
#define BISHOP 2
#define ROOK   3
#define QUEEN  4
#define KING   5

/* board representation */
#define WHITE  0
#define BLACK  1

#define WHITE_PAWN      (2*PAWN   + WHITE)
#define BLACK_PAWN      (2*PAWN   + BLACK)
#define WHITE_KNIGHT    (2*KNIGHT + WHITE)
#define BLACK_KNIGHT    (2*KNIGHT + BLACK)
#define WHITE_BISHOP    (2*BISHOP + WHITE)
#define BLACK_BISHOP    (2*BISHOP + BLACK)
#define WHITE_ROOK      (2*ROOK   + WHITE)
#define BLACK_ROOK      (2*ROOK   + BLACK)
#define WHITE_QUEEN     (2*QUEEN  + WHITE)
#define BLACK_QUEEN     (2*QUEEN  + BLACK)
#define WHITE_KING      (2*KING   + WHITE)
#define BLACK_KING      (2*KING   + BLACK)
#define EMPTY           (BLACK_KING  +  1)

#define PCOLOR(p) ((p)&1)

int side2move;
int board[30];

#define FLIP(sq) ((sq)^25)
#define OTHER(side) ((side)^ 1)

int mg_value[7] = { 0, 82, 337, 365, 477, 1025,  12000};
int eg_value[7] = { 0, 94, 281, 297, 512,  936,  12000};

/* piece/sq tables */
/* values from Rofchade: http://www.talkchess.com/forum3/viewtopic.php?f=2&t=68311&start=19 */


int mg_pawn_table[30] = {
      0,   0,   0,   0,   0,
     98, 134,  61,  95,  68,
     -6,   7,  26,  31,  65,
    -14,  13,   6,  21,  23,
    -27,  -2,  -5,  12,  17,
};

int eg_pawn_table[30] = {
      0,   0,   0,   0,   0,
    178, 173, 158, 134, 147,
     94, 100,  85,  67,  56,
     32,  24,  13,   5,  -2,
     13,   9,  -3,  -7,  -7,
};
int mg_knight_table[30] = {
    -167, -89, -34, -49,  61,
     -73, -41,  72,  36,  23,
     -47,  60,  37,  65,  84,
      -9,  17,  19,  53,  37,
     -13,   4,  16,  13,  28,
};

int eg_knight_table[30] = {
    -58, -38, -13, -28, -31,
    -25,  -8, -25, -26, -25,
    -24, -20,  10,   9,  -1,
    -17,   3,  22,  22,  11,
    -11,  10,  22,  15,   4,
};

int mg_bishop_table[30] = {
    -29,   4, -82, -37, -25,
     -3,  16,  22,  15,  15,
    -13,   0,  18,   8,  29,
    -15,  -2,   4,  17,  17,
     -5,  19,  17,  25,  32,
};

int eg_bishop_table[30] = {
    -23, -12, -14, -11, -2,
    -10,  -8,  -4,  -7,  3,
     -3,   7,  -6,   1,   2,
     -1,  -2,   8,  -2,   4,
     -3,   1,   6,  -2,   0,
};

int mg_rook_table[30] = {
      32,  42,  32,  51, 63,
      27,  32,  58,  62, 80,
      -5,  19,  26,  36, 17,
      -5,  13,  10,  25, 37,
     -26,  -9,   5,  15, 13,
};

int eg_rook_table[30] = {
       13,  10,  18,  15, 12,
       11,  13,  13,  11,  7,
        7,   7,   7,   5,  4,
        5,   3,   3,  -1, -2,
       -4,   0,  -5,  -7, -12,
};

int mg_queen_table[30] = {
     -63, -52, -58, -43, -36,
     -52, -39, -32, -27, -43,
     -40, -31, -28, -32, -29,
     -36, -26, -32, -28, -25,
     -32, -21, -23, -15, -14,
};

int eg_queen_table[30] = {
     -57, -40, -41, -28, -16,
     -53, -45, -32, -25, -23,
     -32, -27, -15,  -5,  -2,
     -29, -25,  -9,  -4,   3,
     -26, -16,   0,   1,   6,
};

int mg_king_table[30] = {
      -9,  24,  10, -24, -42,
       5,  29,  21, -8, -22,
     -17,  11,   4, -13, -17,
     -26,   3,  -6, -23, -24,
     -22,  -5, -10, -21, -34,
};

int eg_king_table[30] = {
     -65, -19,  18,  -1, -19,
     -56,   6,  16,  15, -14,
     -46,  13,  29,  10,   1,
     -37,   3,  23,  16,  -2,
     -30,   9,  15,   6,  -4,
};
int mg_empty[30]={0};
int eg_empty[30]={0};


    int* mg_pesto_table[7] =
{
    mg_empty,
    mg_pawn_table,
    mg_knight_table,
    mg_bishop_table,
    mg_rook_table,
    mg_queen_table,
    mg_king_table,
};

int* eg_pesto_table[7] =
{
    eg_empty,
    eg_pawn_table,
    eg_knight_table,
    eg_bishop_table,
    eg_rook_table,
    eg_queen_table,
    eg_king_table,
};

int gamephaseInc[7] = {0,0,1,1,2,4,0};
int mg_table[12][64];
int eg_table[12][64];



void State::init_tables()
{
    int pc, p, sq;
    for (p = PAWN, pc = WHITE_PAWN; p <= KING; pc += 2, p++) {
        for (sq = 0; sq < 64; sq++) {
            mg_table[pc]  [sq] = mg_value[p] + mg_pesto_table[p][sq];
            eg_table[pc]  [sq] = eg_value[p] + eg_pesto_table[p][sq];
            mg_table[pc+1][sq] = mg_value[p] + mg_pesto_table[p][FLIP(sq)];
            eg_table[pc+1][sq] = eg_value[p] + eg_pesto_table[p][FLIP(sq)];
        }
    }
}
int State::eval()
{
  init_tables();
    int mg[2];
    int eg[2];
    int gamePhase = 0;

    mg[WHITE] = 0;
    mg[BLACK] = 0;
    eg[WHITE] = 0;
    eg[BLACK] = 0;

    /* evaluate each piece */
    for (int i = 0; i < BOARD_H; i++) {
      for (int j = 0; j < BOARD_W; j++) {
          int pc = this->board.board[this->player][i][j];
          if (pc != EMPTY) {

              mg[this->player] += mg_table[pc][i*BOARD_W+j];
              eg[this->player] += eg_table[pc][i*BOARD_W+j];
              gamePhase += gamephaseInc[pc];
          }
          int oppc = this->board.board[1-this->player][i][j];
          if (oppc != EMPTY) {

              mg[1-this->player] += mg_table[oppc][i*BOARD_W+j];
              eg[1-this->player] += eg_table[oppc][i*BOARD_W+j];
              gamePhase += gamephaseInc[oppc];
          }
      }
    }

    /* tapered eval */
    int mgScore = mg[this->player] - mg[1-this->player];
    int egScore = eg[this->player] - eg[1-this->player];
    int mgPhase = gamePhase;
    if (mgPhase > 24) mgPhase = 24; /* in case of early promotion */
    int egPhase = 24 - mgPhase;
    int a=(mgScore * mgPhase + egScore * egPhase) / 24;
    this->score=a;
    return a;
}



/**
 * @brief evaluate the state
 * 
 * @return int 
 */

int State::evaluate(){
  int scores[7] = {0, 2, 6, 7, 8, 20, 100};  // Scores for pieces: pawn, rook, bishop, knight, queen, king

  int score = 0;

  for (int i = 0; i < BOARD_H; i++) {
    for (int j = 0; j < BOARD_W; j++) {
      int piece = this->board.board[this->player][i][j];
      score += scores[piece];

      int opponentPiece = this->board.board[1 - this->player][i][j];
      score -= scores[opponentPiece];
    }
  }
  this->score=score;
  return score;
}



/**
 * @brief return next state after the move
 * 
 * @param move 
 * @return State* 
 */
State* State::next_state(Move move){
  Board next = this->board;
  Point from = move.first, to = move.second;
  
  int8_t moved = next.board[this->player][from.first][from.second];
  //promotion for pawn
  if(moved == 1 && (to.first==BOARD_H-1 || to.first==0)){
    moved = 5;
  }
  if(next.board[1-this->player][to.first][to.second]){
    next.board[1-this->player][to.first][to.second] = 0;
  }
  
  next.board[this->player][from.first][from.second] = 0;
  next.board[this->player][to.first][to.second] = moved;
  
  State* next_state = new State(next, 1-this->player);
  
  if(this->game_state != WIN)
    next_state->get_legal_actions();
  return next_state;
}


static const int move_table_rook_bishop[8][7][2] = {
  {{0, 1}, {0, 2}, {0, 3}, {0, 4}, {0, 5}, {0, 6}, {0, 7}},
  {{0, -1}, {0, -2}, {0, -3}, {0, -4}, {0, -5}, {0, -6}, {0, -7}},
  {{1, 0}, {2, 0}, {3, 0}, {4, 0}, {5, 0}, {6, 0}, {7, 0}},
  {{-1, 0}, {-2, 0}, {-3, 0}, {-4, 0}, {-5, 0}, {-6, 0}, {-7, 0}},
  {{1, 1}, {2, 2}, {3, 3}, {4, 4}, {5, 5}, {6, 6}, {7, 7}},
  {{1, -1}, {2, -2}, {3, -3}, {4, -4}, {5, -5}, {6, -6}, {7, -7}},
  {{-1, 1}, {-2, 2}, {-3, 3}, {-4, 4}, {-5, 5}, {-6, 6}, {-7, 7}},
  {{-1, -1}, {-2, -2}, {-3, -3}, {-4, -4}, {-5, -5}, {-6, -6}, {-7, -7}},
};
static const int move_table_knight[8][2] = {
  {1, 2}, {1, -2},
  {-1, 2}, {-1, -2},
  {2, 1}, {2, -1},
  {-2, 1}, {-2, -1},
};
static const int move_table_king[8][2] = {
  {1, 0}, {0, 1}, {-1, 0}, {0, -1}, 
  {1, 1}, {1, -1}, {-1, 1}, {-1, -1},
};


/**
 * @brief get all legal actions of now state
 * 
 */
void State::get_legal_actions(){
  // [Optional]
  // This method is not very efficient
  // You can redesign it
  this->game_state = NONE;
  std::vector<Move> all_actions;
  auto self_board = this->board.board[this->player];
  auto oppn_board = this->board.board[1 - this->player];
  
  int now_piece, oppn_piece;
  for(int i=0; i<BOARD_H; i+=1){
    for(int j=0; j<BOARD_W; j+=1){
      if((now_piece=self_board[i][j])){
        // std::cout << this->player << "," << now_piece << ' ';
        switch (now_piece){
          case 1: //pawn
            if(this->player && i<BOARD_H-1){
              //black
              if(!oppn_board[i+1][j] && !self_board[i+1][j])
                all_actions.push_back(Move(Point(i, j), Point(i+1, j)));
              if(j<BOARD_W-1 && (oppn_piece=oppn_board[i+1][j+1])>0){
                all_actions.push_back(Move(Point(i, j), Point(i+1, j+1)));
                if(oppn_piece==6){
                  this->game_state = WIN;
                  this->legal_actions = all_actions;
                  return;
                }
              }
              if(j>0 && (oppn_piece=oppn_board[i+1][j-1])>0){
                all_actions.push_back(Move(Point(i, j), Point(i+1, j-1)));
                if(oppn_piece==6){
                  this->game_state = WIN;
                  this->legal_actions = all_actions;
                  return;
                }
              }
            }else if(!this->player && i>0){
              //white
              if(!oppn_board[i-1][j] && !self_board[i-1][j])
                all_actions.push_back(Move(Point(i, j), Point(i-1, j)));
              if(j<BOARD_W-1 && (oppn_piece=oppn_board[i-1][j+1])>0){
                all_actions.push_back(Move(Point(i, j), Point(i-1, j+1)));
                if(oppn_piece==6){
                  this->game_state = WIN;
                  this->legal_actions = all_actions;
                  return;
                }
              }
              if(j>0 && (oppn_piece=oppn_board[i-1][j-1])>0){
                all_actions.push_back(Move(Point(i, j), Point(i-1, j-1)));
                if(oppn_piece==6){
                  this->game_state = WIN;
                  this->legal_actions = all_actions;
                  return;
                }
              }
            }
            break;
          
          case 2: //rook
          case 4: //bishop
          case 5: //queen
            int st, end;
            switch (now_piece){
              case 2: st=0; end=4; break; //rook
              case 4: st=4; end=8; break; //bishop
              case 5: st=0; end=8; break; //queen
              default: st=0; end=-1;
            }
            for(int part=st; part<end; part+=1){
              auto move_list = move_table_rook_bishop[part];
              for(int k=0; k<std::max(BOARD_H, BOARD_W); k+=1){
                int p[2] = {move_list[k][0] + i, move_list[k][1] + j};
                
                if(p[0]>=BOARD_H || p[0]<0 || p[1]>=BOARD_W || p[1]<0) break;
                now_piece = self_board[p[0]][p[1]];
                if(now_piece) break;
                
                all_actions.push_back(Move(Point(i, j), Point(p[0], p[1])));
                
                oppn_piece = oppn_board[p[0]][p[1]];
                if(oppn_piece){
                  if(oppn_piece==6){
                    this->game_state = WIN;
                    this->legal_actions = all_actions;
                    return;
                  }else
                    break;
                };
              }
            }
            break;
          
          case 3: //knight
            for(auto move: move_table_knight){
              int x = move[0] + i;
              int y = move[1] + j;
              
              if(x>=BOARD_H || x<0 || y>=BOARD_W || y<0) continue;
              now_piece = self_board[x][y];
              if(now_piece) continue;
              all_actions.push_back(Move(Point(i, j), Point(x, y)));
              
              oppn_piece = oppn_board[x][y];
              if(oppn_piece==6){
                this->game_state = WIN;
                this->legal_actions = all_actions;
                return;
              }
            }
            break;
          
          case 6: //king
            for(auto move: move_table_king){
              int p[2] = {move[0] + i, move[1] + j};
              
              if(p[0]>=BOARD_H || p[0]<0 || p[1]>=BOARD_W || p[1]<0) continue;
              now_piece = self_board[p[0]][p[1]];
              if(now_piece) continue;
              
              all_actions.push_back(Move(Point(i, j), Point(p[0], p[1])));
              
              oppn_piece = oppn_board[p[0]][p[1]];
              if(oppn_piece==6){
                this->game_state = WIN;
                this->legal_actions = all_actions;
                return;
              }
            }
            break;
        }
      }
    }
  }
  std::cout << "\n";
  this->legal_actions = all_actions;
}


const char piece_table[2][7][5] = {
  {" ", "♙", "♖", "♘", "♗", "♕", "♔"},
  {" ", "♟", "♜", "♞", "♝", "♛", "♚"}
};
/**
 * @brief encode the output for command line output
 * 
 * @return std::string 
 */
std::string State::encode_output(){
  std::stringstream ss;
  int now_piece;
  for(int i=0; i<BOARD_H; i+=1){
    for(int j=0; j<BOARD_W; j+=1){
      if((now_piece = this->board.board[0][i][j])){
        ss << std::string(piece_table[0][now_piece]);
      }else if((now_piece = this->board.board[1][i][j])){
        ss << std::string(piece_table[1][now_piece]);
      }else{
        ss << " ";
      }
      ss << " ";
    }
    ss << "\n";
  }
  return ss.str();
}


/**
 * @brief encode the state to the format for player
 * 
 * @return std::string 
 */
std::string State::encode_state(){
  std::stringstream ss;
  ss << this->player;
  ss << "\n";
  for(int pl=0; pl<2; pl+=1){
    for(int i=0; i<BOARD_H; i+=1){
      for(int j=0; j<BOARD_W; j+=1){
        ss << int(this->board.board[pl][i][j]);
        ss << " ";
      }
      ss << "\n";
    }
    ss << "\n";
  }
  return ss.str();
}
