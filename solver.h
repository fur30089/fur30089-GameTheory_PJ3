#pragma once
#include <iostream>
#include <algorithm>
#include <cmath>
#include "board.h"
#include <numeric>
#include "action.h"

class state_type {

public:
	enum type : char {
		before  = 'b',
		after   = 'a',
		illegal = 'i'
	};


public:
	state_type() : t(illegal) {}
	state_type(const state_type& st) = default;
	state_type(state_type::type code) : t(code) {}

	friend std::istream& operator >>(std::istream& in, state_type& type) {
		std::string s;
		if (in >> s) type.t = static_cast<state_type::type>((s + " ").front());
		return in;
	}

	friend std::ostream& operator <<(std::ostream& out, const state_type& type) {
		return out << char(type.t);
	}

	bool is_before()  const { return t == before; }
	bool is_after()   const { return t == after; }
	bool is_illegal() const { return t == illegal; }

private:
	type t;
};

class state_hint {
public:
	state_hint(const board& state) : state(const_cast<board&>(state)) {}

	char type() const { return state.info() ? state.info() + '0' : 'x'; }
	operator board::cell() const { return state.info(); }

public:
	friend std::istream& operator >>(std::istream& in, state_hint& hint) {
		while (in.peek() != '+' && in.good()) in.ignore(1);
		char v; in.ignore(1) >> v;
		hint.state.info(v != 'x' ? v - '0' : 0);
		return in;
	}
	friend std::ostream& operator <<(std::ostream& out, const state_hint& hint) {
		return out << "+" << hint.type();
	}

private:
	board& state;
};


class solver {
public:
	typedef float value_t;

public:
	class answer {
	public:
		answer(value_t min = 0.0/0.0, value_t avg = 0.0/0.0, value_t max = 0.0/0.0) : min(min), avg(avg), max(max) {}
	    friend std::ostream& operator <<(std::ostream& out, const answer& ans) {
	    	return !std::isnan(ans.avg) ? (out << ans.min << " " << ans.avg << " " << ans.max) : (out << "-1") << std::endl;
		}
	public:
		const value_t min, avg, max;
	};

public:
	solver(const std::string& args) {
 
    


      board root;
      board before;
      before(0)=1;
      //TreeAfter(root,0,0);
      TreeBefore(before,0);
      //std::cout<<v;
      //std::cout<<boardboard;

    /*   
     while (true) {
      agent& who = game.take_turns(play, evil);
      //action move = who.take_action(game.state());

      std::array<int, 4> bag;
      bag = game.BagRule();
      //if (who.role() == "environment") bag = game.BagRule();
      action move = who.take_action(game.state(), bag);
      //std::cout << game.state(); 
      //std::cout << move<<'\n'<<'\n'<<'\n';
		
      if (game.apply_action(move) != true) break;
			if (who.check_for_win(game.state())) break;
      //game.BagRule();
    }
    */   


      
		// TODO: explore the tree and save the result

/*    
function minimax(node, depth, maximizingPlayer) is
    if depth = 0 or node is a terminal node then
        return the heuristic value of node
    if maximizingPlayer then
        value := ?¡Û
        for each child of node do
            value := max(value, minimax(child, depth ? 1, FALSE))
        return value
    else (* minimizing player *)
        value := +¡Û
        for each child of node do
            value := min(value, minimax(child, depth ? 1, TRUE))
        return value
*/

//		std::cout << "feel free to display some messages..." << std::endl;
	}

	answer solve(const board& state, state_type type = state_type::before) {
		// TODO: find the answer in the lookup table and return it
		//       do NOT recalculate the tree at here

		// to fetch the hint 
		board::cell hint = state_hint(state);

		// for a legal state, return its three values.
//		return { min, avg, max };
		// for an illegal state, simply return {}
   
    value_t min=60;
    value_t avg=61;
    value_t max=62;
    //TableB[0][2]=119;
    std::cout<<type<<state(0)<<state(1)<<state(2)<<hint<<TableB[0][2];
     
		return {min,avg,max};
	}

private:
	// TODO: place your transposition table here
  //std::vector<std::vector<int> > TableB;
  float TableB[10][10];  //[board][hint]
  float TableA[10][10][10]; //[board][hint][last]
 
  void TreeAfter(board after,int hint,int last)
    {  
       if (hint==0){
          for (int pos=0;pos<=5;pos++) {          
            for (int tile=1;tile<=3;tile++) {
                    board before=after; 
                    before(pos) = tile;
                    std::cout<<before<<'\n';
            }
           
  		}
       
       }
    }   


  void TreeBefore(board before,int hint)
    {  
  		for (int slide=0;slide<=3;slide++) {
        board after=before;
        board::reward reward = after.slide(slide); // ?? board changes and give a reward ??
        if (reward != -1){ 
            std::cout<<after<<'\n';  
  		  }
      }

    } 





};
