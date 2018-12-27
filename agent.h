#pragma once
#include <string>
#include <random>
#include <sstream>
#include <map>
#include <type_traits>
#include <algorithm>
#include "board.h"
#include "action.h"
#include <fstream>
#include <math.h>
class agent {
public:
	agent(const std::string& args = "") {       //constructor
		std::stringstream ss("name=unknown role=unknown " + args);
		for (std::string pair; ss >> pair; ) {
			std::string key = pair.substr(0, pair.find('='));
			std::string value = pair.substr(pair.find('=') + 1);
			meta[key] = { value };
		}
	}
	virtual ~agent() {}  //desconstructor
	virtual void open_episode(const std::string& flag = "") {}
	virtual void close_episode(const std::string& flag = "") {}
	virtual action take_action(const board& b, std::array<int, 4> bag) { return action(); }
	virtual bool check_for_win(const board& b) { return false; }

public:
	virtual std::string property(const std::string& key) const { return meta.at(key); }
	virtual void notify(const std::string& msg) { meta[msg.substr(0, msg.find('='))] = { msg.substr(msg.find('=') + 1) }; }
	virtual std::string name() const { return property("name"); }
	virtual std::string role() const { return property("role"); }

protected:
	typedef std::string key;
	struct value {
		std::string value;
		operator std::string() const { return value; }
		template<typename numeric, typename = typename std::enable_if<std::is_arithmetic<numeric>::value, numeric>::type>
		operator numeric() const { return numeric(std::stod(value)); }
	};
	std::map<key, value> meta;
};

class random_agent : public agent {    
public:
	random_agent(const std::string& args = "") : agent(args) {
		if (meta.find("seed") != meta.end())
			engine.seed(int(meta["seed"]));
	}
	virtual ~random_agent() {}

protected:
	std::default_random_engine engine;
};







/**
 * base agent for agents with a learning rate
 */
class learning_agent : public agent {
public:
	learning_agent(const std::string& args = "") : agent(args), alpha(0.1f) {
		if (meta.find("alpha") != meta.end())
			alpha = float(meta["alpha"]);
	}
	virtual ~learning_agent() {}

protected:
	float alpha;
};




/**
 * random environment
 * add a new random tile to an empty cell
 * 2-tile: 90%
 * 4-tile: 10%
 */
 
/* 
class rndenv : public random_agent {
public:
	rndenv(const std::string& args = "") : random_agent("name=random role=environment " + args),
		space({ 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15 }), popup(0, 9) {}

	virtual action take_action(const board& after) {
		std::shuffle(space.begin(), space.end(), engine);
		for (int pos : space) {
			if (after(pos) != 0) continue;
			board::cell tile = popup(engine) ? 1 : 2;
			return action::place(pos, tile);
		}
		return action();
	}

private:
	std::array<int, 16> space;
	std::uniform_int_distribution<int> popup;
};
*/



/*
// 4x4
class rndenv : public random_agent {
public:
	rndenv(const std::string& args = "") : random_agent("name=random role=environment " + args),
		space({ 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15 }), space2({0,1,2}),space3({std::array<int, 4>{12,13,14,15},std::array<int, 4>{0,4,8,12},std::array<int, 4>{0,1,2,3},std::array<int, 4>{3,7,11,15}}) ,popup(0, 9) {}

	virtual action take_action(const board& after,std::array<int, 4> bag) {
    //int bag[3]={0,1,1};
     
		//std::shuffle(space.begin(), space.end(), engine);
    std::shuffle(space2.begin(), space2.end(), engine);
    //std::shuffle(space3[bag[3]].begin(), space3[bag[3]].end(), engine);
    board::cell tile;
    if (bag[3]==10){ 
      std::shuffle(space.begin(), space.end(), engine);
  		for (int pos : space) {
  			if (after(pos) != 0) continue;
           
            for (int i : space2) {
                 if (bag[i] == 0)
                    tile = i+1;
                    //int tile=1;
            }
           
        //board::cell tile = popup(engine) ? 1 : 2;
  			return action::place(pos, tile);
  		}
    }
    else{ 
      std::shuffle(space3[bag[3]].begin(), space3[bag[3]].end(), engine);
  		for (int pos : space3[bag[3]]) {
  			if (after(pos) != 0) continue;
           
            for (int i : space2) {
                 if (bag[i] == 0)
                    tile = i+1;
                    //int tile=1;
            }
           
        //board::cell tile = popup(engine) ? 1 : 2;
  			return action::place(pos, tile);
  		}
    }    
		return action();
	}
private:
	std::array<int, 16> space;   //declare space here??
  std::array<int, 3> space2;
  //char space3[4][4];
  std::array<std::array<int, 4>,4> space3;
	std::uniform_int_distribution<int> popup;
}; 
*/




// 2x3
class rndenv : public random_agent {
public:
	rndenv(const std::string& args = "") : random_agent("name=random role=environment " + args),
		space({ 0, 1, 2, 3, 4, 5}), space2({0,1,2}),spaceV({std::array<int, 3>{3,4,5},std::array<int, 3>{0,1,2}}),spaceH({std::array<int, 2>{0,3},std::array<int, 2>{2,5}}) ,popup(0, 9) {}
   //space3({std::array<int, 4>{12,13,14,15},std::array<int, 4>{0,4,8,12},std::array<int, 4>{0,1,2,3},std::array<int, 4>{3,7,11,15}})

	virtual action take_action(const board& after,std::array<int, 4> bag) {
    
    //std::shuffle(space2.begin(), space2.end(), engine);
    if (bag[3]==10){ 
      //std::shuffle(space.begin(), space.end(), engine);
  		for (int pos : space) {          
            for (int i : space2) {
                    board before=after; 
                    before(pos) = i+1;
                    std::cout<<before<<'\n';
            }
           
  		}
    }
    else if (bag[3]==0 or bag[3]==2  ){
      //std::cout<<bag[3]<<std::endl;  
  		for (int pos : spaceV[bag[3]/2]) {
  			if (after(pos) != 0) continue;
           
            for (int i : space2) {
                 //if (bag[i] == 0)
            }
           
        //board::cell tile = popup(engine) ? 1 : 2;
  			//return action::place(pos, tile);
  		}
    } 
    else {
    //std::cout<<bag[3]<<std::endl; 
  		for (int pos : spaceH[bag[3]/2]) {
  			if (after(pos) != 0) continue;
           
            for (int i : space2) {
                 //if (bag[i] == 0)
                    //tile = i+1;
                    //int tile=1;
            }
           
        //board::cell tile = popup(engine) ? 1 : 2;
  			//return action::place(pos, tile);
  		}
    }        
		return action();
	}
private:
	std::array<int, 6> space;   //declare space here??
  std::array<int, 3> space2;
  //char space3[4][4];
  std::array<std::array<int, 2>,2> spaceH;
  std::array<std::array<int, 3>,2> spaceV;
	std::uniform_int_distribution<int> popup;
}; 






/**
 * dummy player
 * select a legal action randomly
 */
 
 
class player : public random_agent {
public:
	player(const std::string& args = "") : random_agent("name=dummy role=player " + args),
		opcode({ 0,1,2,3 }) {}

	virtual action take_action(const board& before, std::array<int, 4> bag) {
		//std::cout<< before;
    std::shuffle(opcode.begin(), opcode.end(), engine);
		for (int op : opcode) {
      board after=before;
      board::reward reward = after.slide(op); // ?? board changes and give a reward ??
	    //std::cout<<reward<<std::endl;
      if (reward != -1){ 
      //std::cout<< after<<std::endl;
      //std::cout<<reward<<std::endl;
      return action::slide(op);   // ?? return move such as #U #L??
		  }
    }
		return action();  // 0 1 2 3  ileagal??
	}

private:
	std::array<int, 4> opcode;
};
 




/*
class player : public random_agent {
public:
	player(const std::string& args = "") : random_agent("name=dummy role=player " + args),
		opcode({ 0, 1, 2, 3 }) {}

	virtual action take_action(const board& before, std::array<int, 4> bag) {
		//board original = before;
    //action a;
		//int reward = a.apply(original);


		// std::shuffle(opcode.begin(), opcode.end(), engine);
		
		//for (int op : opcode) 
		//	board::reward reward = board(before).slide(op); // give a reward ??
			
			
			board after = before;
      std::cout<< after;
			board::reward reward = after.slide(0);
			std::cout<< after;
      std::cout<< reward;
			
			
      //board::reward reward0=std::max(board(before).slide(0),board(before).slide(1));
      //board::reward reward2=std::max(board(before).slide(2),board(before).slide(3));
      //reward=std::max(reward0,reward2);
     
 
			if (reward != -1) return action::slide(0);   // ?? return move such as #U #L??  board change??
      //return action::slide(0); 
      else return action();  // 0 1 2 3  ileagal??
	}

private:
	std::array<int, 4> opcode;
};
*/