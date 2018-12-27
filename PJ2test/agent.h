#pragma once
#include <string>
#include <random>
#include <sstream>
#include <map>
#include <type_traits>
#include <algorithm>
#include "board.h"
#include "action.h"
#include "weight.h"
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
 * base agent for agents with weight tables
 */
class weight_agent : public agent {  //public inheritance
public:
	weight_agent(const std::string& args = "") : agent(args) {  //constructor
		if (meta.find("init") != meta.end()) // pass init=... to initialize the weight
			init_weights(meta["init"]);
		if (meta.find("load") != meta.end()) // pass load=... to load from a specific file
			load_weights(meta["load"]);
	}
	virtual ~weight_agent() {  // ~ desconstructor??
		if (meta.find("save") != meta.end()) // pass save=... to save to a specific file
			save_weights(meta["save"]);
	}

	virtual void open_episode(const std::string& flag = "") {}
	/*
  virtual void close_episode(const std::string& flag = "") {  // need to train the value of state when game overs
		// ...

		rec.clear();
	}
 */
 
 /*
	virtual action take_action(const board& b) {  // record state reward to rec
	
		for (int op = 0; op < 4; op++) {
			action a = action::slide(op);
			board after = b;
			int reward = a.apply(after);
			//rec.push_back({ after, reward });
		}

		return a;
	}
 */
 	virtual action take_action(const board& before, std::array<int, 4> bag) {  // record state reward to rec

      //std::cout<< before;
      board::reward reward;
      board afterbest;
      int a=10,r; 
      float value,score=-10000000;  // if score < score, some error happens
      float lr=0.1/20, y=1;
      for(int i=0;i<=3;i++){   //select a move
        board after = before;
        reward = after.slide(i);
        //std::cout<< after; 
        if (reward != -1){     
          if (reward+y*estimate(after)>=score){
          r=reward;
          value=estimate(after); 
          a=i; 
          score=r+y*value; 
          afterbest=after;      
          } 
        }
      } 
      //std::cout<< afterbest; 
      //std::cout<<a<<'\n';
      //std::cout<<"reward:"<<r<<'\n'; 
      //std::cout<<"value:"<<value<<'\n';

			if (a != 10){   
      //std::cout<<"bag: "<<bag[3]<<'\n';           //training
      
      float offset=lr*(r+y*value-estimate(last));
      update(last,offset);
      last=afterbest;

      return action::slide(a); 
      } 
      //return action::slide(0); 
      else 
      {   //std::cout<<"a: "<<a<<'\n';
          //float offset=lr*(r+y*value-estimate(last));
          update(last,-50);         
          return action();  // 0 1 2 3  ileagal??
      } 
             
	}
 
	virtual bool check_for_win(const board& b) { return false; }

	float estimate(board b) {                // sum of vlue of every tuple.  map tiles to index of tuple, and get the value from the tuple index. 
		float v = 0;
    
		v += net[0][b(0) * 15 * 15 * 15 + b(1) * 15 * 15 + b(2) * 15 + b(3)];
		v += net[1][b(4) * 15 * 15 * 15 + b(5) * 15 * 15 + b(6) * 15 + b(7)];
    v += net[2][b(8) * 15 * 15 * 15 + b(9) * 15 * 15 + b(10) * 15 + b(11)];
    v += net[3][b(12) * 15 * 15 * 15 + b(13) * 15 * 15 + b(14) * 15 + b(15)];	
    
		v += net[4][b(0) * 15 * 15 * 15 + b(4) * 15 * 15 + b(8) * 15 + b(12)];
		v += net[5][b(1) * 15 * 15 * 15 + b(5) * 15 * 15 + b(9) * 15 + b(13)];
    v += net[6][b(2) * 15 * 15 * 15 + b(6) * 15 * 15 + b(10) * 15 + b(14)];
    v += net[7][b(3) * 15 * 15 * 15 + b(7) * 15 * 15 + b(11) * 15 + b(15)];	    
		
		//v += net[0][b(0)*pow(15,5)+ b(1) *pow(15,4) + b(2) *pow(15,3) + b(3)*pow(15,2)+ b(4) *15 + b(5)];    
    return v;
	}

	void update(board b, float offset) {   // update the value
		net[0][b(0) * 15 * 15 * 15 + b(1) * 15 * 15 + b(2) * 15 + b(3)] += offset / 8;
    net[1][b(4) * 15 * 15 * 15 + b(5) * 15 * 15 + b(6) * 15 + b(7)]+= offset / 8;
    net[2][b(8) * 15 * 15 * 15 + b(9) * 15 * 15 + b(10) * 15 + b(11)]+= offset / 8;
    net[3][b(12) * 15 * 15 * 15 + b(13) * 15 * 15 + b(14) * 15 + b(15)]+= offset / 8;
    
    net[4][b(0) * 15 * 15 * 15 + b(4) * 15 * 15 + b(8) * 15 + b(12)]+= offset / 8;
    net[5][b(1) * 15 * 15 * 15 + b(5) * 15 * 15 + b(9) * 15 + b(13)]+= offset / 8;
    net[6][b(2) * 15 * 15 * 15 + b(6) * 15 * 15 + b(10) * 15 + b(14)]+= offset / 8;
    net[7][b(3) * 15 * 15 * 15 + b(7) * 15 * 15 + b(11) * 15 + b(15)]+= offset / 8;
	}

  


protected:
	virtual void init_weights(const std::string& info) {
		net.emplace_back(pow(15,4)); // create an empty weight table with size 65536 net[0][XXX]
		net.emplace_back(pow(15,4)); // create an empty weight table with size 65536 net[1]
		net.emplace_back(pow(15,4)); // create an empty weight table with size 65536
		net.emplace_back(pow(15,4)); // create an empty weight table with size 65536
		net.emplace_back(pow(15,4)); // create an empty weight table with size 65536
		net.emplace_back(pow(15,4)); // create an empty weight table with size 65536
		net.emplace_back(pow(15,4)); // create an empty weight table with size 65536
		net.emplace_back(pow(15,4)); // create an empty weight table with size 65536
		// now net.size() == 8; net[0].size() == 65536; net[1].size() == 65536
	}
	virtual void load_weights(const std::string& path) {
		std::ifstream in(path, std::ios::in | std::ios::binary);
		if (!in.is_open()) std::exit(-1);
		uint32_t size;
		in.read(reinterpret_cast<char*>(&size), sizeof(size));
		net.resize(size);
		for (weight& w : net) in >> w;
		in.close();
	}
	virtual void save_weights(const std::string& path) {
		std::ofstream out(path, std::ios::out | std::ios::binary | std::ios::trunc);
		if (!out.is_open()) std::exit(-1);
		uint32_t size = net.size();
		out.write(reinterpret_cast<char*>(&size), sizeof(size));
		for (weight& w : net) out << w;
		out.close();
	}

protected:
	std::vector<weight> net;

//	struct record {
//		board after;
//		int reward;
//	};
//	std::vector<record> rec;
	board last;
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
    else if (bag[3]==0 or bag[3]==2  ){
      //std::cout<<bag[3]<<std::endl;  
      std::shuffle(spaceV[bag[3]/2].begin(), spaceV[bag[3]/2].end(), engine);
  		for (int pos : spaceV[bag[3]/2]) {
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
    else {
    //std::cout<<bag[3]<<std::endl; 
      std::shuffle(spaceH[bag[3]/2].begin(), spaceH[bag[3]/2].end(), engine);
  		for (int pos : spaceH[bag[3]/2]) {
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