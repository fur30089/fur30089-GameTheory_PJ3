#pragma once
#include <array>
#include <iostream>
#include <iomanip>
#include <cmath>

int IndextoValue(int i){
  if (i>=3){
      return 3*(pow(2,i-3));
  }
  else{
    return i;
  }
} 

int ValuetoIndex(int i){
  if (i>=3){
      return log2(i/3)+3;
  }
  else{
    return i;
  }
} 


/**
 * array-based board for 2048
 *
 * index (1-d form):
 *  (0)  (1)  (2)  (3)
 *  (4)  (5)  (6)  (7)
 *  (8)  (9) (10) (11)
 * (12) (13) (14) (15)
 *
 */
 

class board {
public:
	typedef uint32_t cell;
	typedef std::array<cell, 3> row;
	typedef std::array<row, 2> grid;
	typedef uint64_t data;
	typedef int reward;

public:
	board() : tile(), attr(0) {}
	board(const grid& b, data v = 0) : tile(b), attr(v) {}
	board(const board& b) = default;
	board& operator =(const board& b) = default;

	operator grid&() { return tile; }
	operator const grid&() const { return tile; }
	row& operator [](unsigned i) { return tile[i]; }
	const row& operator [](unsigned i) const { return tile[i]; }
	//cell& operator ()(unsigned i) { return tile[i / 4][i % 4]; }   //???
	//const cell& operator ()(unsigned i) const { return tile[i / 4][i % 4]; }
	cell& operator ()(unsigned i) { return tile[i / 3][i % 3]; }   //???
	const cell& operator ()(unsigned i) const { return tile[i / 3][i % 3]; } 

	data info() const { return attr; }
	data info(data dat) { data old = attr; attr = dat; return old; }

public:
	bool operator ==(const board& b) const { return tile == b.tile; }
	bool operator < (const board& b) const { return tile <  b.tile; }
	bool operator !=(const board& b) const { return !(*this == b); }
	bool operator > (const board& b) const { return b < *this; }
	bool operator <=(const board& b) const { return !(b < *this); }
	bool operator >=(const board& b) const { return !(*this < b); }

public:

	/**
	 * place a tile (index value) to the specific position (1-d form index)
	 * return 0 if the action is valid, or -1 if not
	 */
	reward place(unsigned pos, cell tile) {
		if (pos >= 6) return -1;
		if (tile != 1 && tile != 2 && tile != 3) return -1;
		operator()(pos) = tile;
		return 0;
	}

	/**
	 * apply an action to the board
	 * return the reward of the action, or -1 if the action is illegal
	 */
	reward slide(unsigned opcode) {
		switch (opcode & 0b11) {
		case 0: return slide_up();
		case 1: return slide_right();
		case 2: return slide_down();
		case 3: return slide_left();
		default: return -1;
		}
	}

	reward slide_left() {
		board prev = *this;  // what is the relation between this, prev, and tile??   *this=tile??   prev:original???
    reward score = 0;
		for (int r = 0; r < 2; r++) {
			auto& row = tile[r];
      /*
      while(1){
        if (row[0]==0){
           row=row+[0]; 
        }
      }
      */
			int top = 0, hold = 0,a=1,b=0;
			for (int c = 0; c < 3; c++) {
				int tile = row[c];
    		row[c] = 0;
    		//hold = tile;
    		if (hold or b==2) {
    			if (tile == hold && tile>=3 && a && b<=1 ) {
    				row[top++] = ++tile;
    				score += IndextoValue(tile);
    				hold = 0;
    				a = 0;
    			}
    			else if ((tile == 1 && hold==2 && a && b <= 1) or (tile == 2 && hold == 1 && a && b <=1)) {
    				row[top++] = 3;
    				score += 3;
    				hold = 0;
    				a = 0;
    			}
    			else {
    				row[top++] = hold;
    				hold = tile;
    			}
    		}
    		else {
    			hold = tile;
    			b++;
    		}
		}	
    if (hold) tile[r][top] = hold;
		}
		//return (*this != prev) ? StateValue(*this)-StateValue(prev) : -1;
    return (*this != prev) ? 0 : -1;
    return -1;
	}

	reward slide_up() {
		board prev = *this;  // what is the relation between this, prev, and tile??   *this=tile??   prev:original???
		reward score = 0;
		//	int top = 0, hold = 0,a=1,b=0;
			for (int c = 0; c < 3; c++) {
          if (tile[0][c]==0 and tile[1][c]!=0){
             std::swap(tile[0][c], tile[1][c]);              
          } 
          else if ((tile[0][c]==1 and tile[1][c]==2)or(tile[0][c]==2 and tile[1][c]==1)){
             tile[0][c]=3;
             tile[1][c]=0;
          }
          else if (tile[0][c]>=3 and tile[0][c]==tile[1][c]){
             tile[0][c]++;
             tile[1][c]=0;
          }          
		}	  
		//return (*this != prev) ? StateValue(*this)-StateValue(prev) : -1;
    return (*this != prev) ? 0 : -1;
    //return -1;
	}
 
    	
/*
float StateValue(board b){
float score=0;
for(int i=0;i<=15;i++){
  if (b(i)>=3){
  score+=pow(3,log(IndextoValue(b(i))/3)/log(2)+1);
  }  
}
return score; 
}
*/ 

 





/*
	reward slide_left() {
		board prev = *this;
		reward score = 0;
		for (int r = 0; r < 4; r++) {
			auto& row = tile[r];
			int top = 0, hold = 0;
			for (int c = 0; c < 4; c++) {
				int tile = row[c];
				if (tile == 0) continue;
				row[c] = 0;
				if (hold) {
					if (tile == hold) {
						row[top++] = ++tile;
						score += (1 << tile);
						hold = 0;
					} else {
						row[top++] = hold;
						hold = tile;
					}
				} else {
					hold = tile;
				}
			}
			if (hold) tile[r][top] = hold;
		}
		return (*this != prev) ? score : -1;
	}
*/ 
 
	reward slide_right() {
		reflect_horizontal();
		reward score = slide_left();
		reflect_horizontal();
		return score;
	}
 
	reward slide_down() {
		reflect_vertical();
		reward score = slide_up();
		reflect_vertical();
		return score;
	}
/*
	void transpose() {
		for (int r = 0; r < 3; r++) {
			for (int c = r + 1; c < 2; c++) {
				std::swap(tile[r][c], tile[c][r]);
			}
		}
	}
*/

	void reflect_horizontal() {
		for (int r = 0; r < 2; r++) {
			std::swap(tile[r][0], tile[r][2]);
			//std::swap(tile[r][1], tile[r][2]);
		}
	}

	void reflect_vertical() {
		for (int c = 0; c < 3; c++) {
			std::swap(tile[0][c], tile[1][c]);
			//std::swap(tile[1][c], tile[2][c]);
		}
	} 

	/**
	 * rotate the board clockwise by given times
	 */
/*   
	void rotate(int r = 1) {
		switch (((r % 4) + 4) % 4) {
		default:
		case 0: break;
		case 1: rotate_right(); break;
		case 2: reverse(); break;
		case 3: rotate_left(); break;
		}
	}
*/
	//void rotate_right() { transpose(); reflect_horizontal(); } // clockwise
	//void rotate_left() { transpose(); reflect_vertical(); } // counterclockwise
	//void reverse() { reflect_horizontal(); reflect_vertical(); }

public:
	friend std::ostream& operator <<(std::ostream& out, const board& b) {  //output state numbers 2x3=6
		for (int i = 0; i < 6; i++) {
			out << std::setw(std::min(i, 1)) << "" << (IndextoValue(b(i)));
		}
		return out;
	}
	friend std::istream& operator >>(std::istream& in, board& b) {   //input state numbers 2x3=6
		for (int i = 0; i < 6; i++) {
			while (!std::isdigit(in.peek()) && in.good()) in.ignore(1);
			in >> b(i);
			//b(i) = std::log2(b(i));    //value to index   need to be revised
      b(i) = ValuetoIndex(b(i)); 
		}
		return in;
	}

private:
	grid tile;
	data attr;
};
