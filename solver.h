#pragma once
#include <iostream>
#include <algorithm>
#include <cmath>
#include "board.h"
#include <numeric>
#include "action.h"
#include <vector>
#include <time.h>



double StateValue(board b){
double score=0;
for(int i=0;i<=5;i++){
  if (b(i)>=3){
  score+=pow(3,log(IndextoValue(b(i))/3)/log(2)+1);
  }  
}
return score; 
}

double max(double  a[],int n){ 
double  max=a[0]; 
for(int i=0;i<n;i++){ // 
if(a[i]>max) 
max=a[i]; 
} 
return max; 
} 

double  avg(double  a[],int n){ 
double  sum=0; 
for(int i=0;i<n;i++) 
sum+=a[i]; 
return sum/n; 
} 


template<typename TT>                                                      //4d vector
std::vector<std::vector<std::vector<std::vector<TT> > > > make_4d_vector(double zz,double z, double y, double x, TT value)
{
    return std::vector<std::vector<std::vector<std::vector<TT> > > >(zz,std::vector<std::vector<std::vector<TT> > >(z, std::vector<std::vector<TT> >(y, std::vector<TT>(x, value))));
}


template<typename T>                                                      //3d vector
std::vector<std::vector<std::vector<T> > > make_3d_vector(double z, double y, double x, T value)
{
    return std::vector<std::vector<std::vector<T> > >(z, std::vector<std::vector<T> >(y, std::vector<T>(x, value)));
}

template<typename U>                                                      //2d vector
std::vector<std::vector<U> > make_2d_vector( double y, double x, U value)
{
    return std::vector<std::vector<U> > (y, std::vector<U>(x, value));
}


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
	typedef double value_t;

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
// TODO: explore the tree and save the result      
      board state; 
          
      state(0)=0;
      state(1)=0;
      state(2)=0;
      state(3)=0;
      state(4)=0;
      state(5)=0;  
       
      //std::cout <<"value: "<< value << '\n'; //20   
      //std::cout <<"value: "<< StateValue(state) << '\n'; //20   
      //std::cout <<"value: "<< state(0)<< '\n'; //20   
      
      //std::array<double, 3> value=TreeBefore(state,2);
      std::array<double, 3> value=TreeAfter(state,0,0);
      //std::cout<<"value:"<<state<<" +1= "<<value<<'\n';

//		std::cout << "feel free to display some messages..." << std::endl;
	}

	answer solve(const board& state, state_type type = state_type::before) {
		// TODO: find the answer in the lookup table and return it
		//       do NOT recalculate the tree at here

		// to fetch the hint 
		
		// for a legal state, return its three values.
//		return { min, avg, max };
		// for an illegal state, simply return {}
    board::cell hint = state_hint(state);
    value_t min;
    value_t avg;
    value_t max;
        
    if (type.is_before()){
    if (TableB[state(0)*pow(10,5)+state(1)*pow(10,4)+state(2)*pow(10,3)+state(3)*pow(10,2)+state(4)*10+state(5)][hint-1][0]==-1) return -1;
    min=TableB[state(0)*pow(10,5)+state(1)*pow(10,4)+state(2)*pow(10,3)+state(3)*pow(10,2)+state(4)*10+state(5)][hint-1][0];
    avg=TableB[state(0)*pow(10,5)+state(1)*pow(10,4)+state(2)*pow(10,3)+state(3)*pow(10,2)+state(4)*10+state(5)][hint-1][1];
    max=TableB[state(0)*pow(10,5)+state(1)*pow(10,4)+state(2)*pow(10,3)+state(3)*pow(10,2)+state(4)*10+state(5)][hint-1][2];
    }
    else{
     int n=0;
     for (int i=0;i<=3;i++){
     if (TableA[state(0)*pow(10,5)+state(1)*pow(10,4)+state(2)*pow(10,3)+state(3)*pow(10,2)+state(4)*10+state(5)][hint][i][1]==-1) continue;
     min=TableA[state(0)*pow(10,5)+state(1)*pow(10,4)+state(2)*pow(10,3)+state(3)*pow(10,2)+state(4)*10+state(5)][hint][i][0];
     avg=TableA[state(0)*pow(10,5)+state(1)*pow(10,4)+state(2)*pow(10,3)+state(3)*pow(10,2)+state(4)*10+state(5)][hint][i][1]; 
     max=TableA[state(0)*pow(10,5)+state(1)*pow(10,4)+state(2)*pow(10,3)+state(3)*pow(10,2)+state(4)*10+state(5)][hint][i][2];   
     n=1;   
     }
     if (n==0)  return -1; 

    
    }

     
		return {min,avg,max};
	}

private:
	// TODO: place your transposition table here
  
  
  std::vector<std::vector<std::vector<std::vector<double> > > > TableA = make_4d_vector<double>(pow(10,6), 4, 4,3, -1);
  //std::vector<std::vector<std::vector<double> > > TableA = make_3d_vector<double>(pow(10,6), 4, 4, -1);
  //std::cout << TableA[5][5][5] << '\n'; //20
  
  std::vector<std::vector<std::vector<double> > > TableB = make_3d_vector<double>(pow(10,6), 3, 3, -1);
  //std::vector<std::vector<double> > TableB = make_2d_vector<double>(pow(10,6), 3, -1);
  //TableB[5][5]=78.8; 
  //std::cout << TableB[5][5] << '\n'; //20  



 
std::array<double, 3> TreeAfter(board after,int hint,int last)
    {  
       if (hint==0){
          int i=0;
          std::array<double, 3> Value={1000000,0,-1000000}; 
          for (int pos=0;pos<=5;pos++) {          
            for (int tile=1;tile<=3;tile++) {
                    board before=after; 
                    before(pos) = tile;
                    Value=allhint(before,hint,i,Value); 
                    i++;
            }           
  		    }
             TableA[after(0)*pow(10,5)+after(1)*pow(10,4)+after(2)*pow(10,3)+after(3)*pow(10,2)+after(4)*10+after(5)][hint][last]={Value[0],Value[1],Value[2]};
             //std::cout<<"a "<<after<<" +"<<hint<<"= "<<Value[1]<<'\n'; 
             return Value;     
       }   
         
       else{
          if (last==0){
             int i=0;
             std::array<double, 3> Value={1000000,0,-1000000}; 
             for (int pos=3;pos<=5;pos++) {                     
                board before=after; 
                if (before(pos) != 0) continue;   
                before(pos) = hint;
                Value=allhint(before,hint,i,Value);
                i++;
                //std::cout<<before<<'\n';            
               }
             TableA[after(0)*pow(10,5)+after(1)*pow(10,4)+after(2)*pow(10,3)+after(3)*pow(10,2)+after(4)*10+after(5)][hint][last]={Value[0],Value[1],Value[2]};
             //std::cout<<"a "<<after<<" +"<<hint<<"= "<<Value[1]<<'\n'; 
             return Value;                
            }
          else if (last==1){
             int i=0;
             std::array<double, 3> Value={1000000,0,-1000000}; 
             for (int pos=0;pos<=3;pos=pos+3) {           
                board before=after; 
                if (before(pos) != 0) continue;   
                before(pos) = hint;
                Value=allhint(before,hint,i,Value);
                i++;
                //std::cout<<before<<'\n';            
               }
             TableA[after(0)*pow(10,5)+after(1)*pow(10,4)+after(2)*pow(10,3)+after(3)*pow(10,2)+after(4)*10+after(5)][hint][last]={Value[0],Value[1],Value[2]};
             //std::cout<<"a "<<after<<" +"<<hint<<"= "<<Value[1]<<'\n'; 
             return Value;   
            }
          else if (last==2){
             int i=0;
             std::array<double, 3> Value={1000000,0,-1000000};  
             for (int pos=0;pos<=2;pos++) {         
                board before=after; 
                if (before(pos) != 0) continue;   
                before(pos) = hint;
                Value=allhint(before,hint,i,Value);
                i++;
                //std::cout<<before<<'\n';            
               }
             TableA[after(0)*pow(10,5)+after(1)*pow(10,4)+after(2)*pow(10,3)+after(3)*pow(10,2)+after(4)*10+after(5)][hint][last]={Value[0],Value[1],Value[2]};
             //std::cout<<"a "<<after<<" +"<<hint<<"= "<<Value[1]<<'\n'; 
             return Value;        
            }
          else if (last==3){
             int i=0;
             std::array<double, 3> Value={1000000,0,-1000000}; 
             for (int pos=2;pos<=5;pos=pos+3) {          
                board before=after; 
                if (before(pos) != 0) continue;   
                before(pos) = hint;
                Value=allhint(before,hint,i,Value);
                i++;
                //std::cout<<before<<'\n';            
               }
             TableA[after(0)*pow(10,5)+after(1)*pow(10,4)+after(2)*pow(10,3)+after(3)*pow(10,2)+after(4)*10+after(5)][hint][last]={Value[0],Value[1],Value[2]};
             //std::cout<<"a "<<after<<" +"<<hint<<"= "<<Value[1]<<'\n'; 
             return Value;                
           }        
      }
}
       

  std::array<double, 3>  allhint(board before, int hint, int i, std::array<double, 3> Value)
      { int total=0;
       for (int pos=0;pos<=5;pos++) {         
          total+=IndextoValue(before(pos));    
       }
       
       if (total%6==0){
          Value[1]=Value[1]*i*3;
          for (int hint_=1;hint_<=3;hint_++){
             Value=Fvalue(before,hint_,Value);           
         }
         return {Value[0],Value[1]/((i+1)*3),Value[2]};
       }
       else if (total%6==1){
          Value[1]=Value[1]*i*2;
          for (int hint_=2;hint_<=3;hint_++){
             Value=Fvalue(before,hint_,Value);           
         }
         return {Value[0],Value[1]/((i+1)*2),Value[2]};
       } 
       else if (total%6==2){
          Value[1]=Value[1]*i*2;
          for (int hint_=1;hint_<=3;hint_=hint_+2){
             Value=Fvalue(before,hint_,Value);           
         }
         return {Value[0],Value[1]/((i+1)*2),Value[2]};
       }                    
      else if (total%6==3 & (hint==3 or hint==0) ){
          Value[1]=Value[1]*i*2;
          for (int hint_=1;hint_<=2;hint_++){
             Value=Fvalue(before,hint_,Value);           
         }
         return {Value[0],Value[1]/((i+1)*2),Value[2]};
          }
      else if (total%6==3 & hint!=3){
             Value[1]=Value[1]*i;
             int hint_=3;
             Value=Fvalue(before,hint_,Value);           
        
         return {Value[0],Value[1]/((i+1)),Value[2]};
         }            
                    
      else if (total%6==4){
             Value[1]=Value[1]*i;       
             int hint_=2;
             Value=Fvalue(before,hint_,Value);           
         
         return {Value[0],Value[1]/((i+1)),Value[2]};
         }              
      else if (total%6==5){
             Value[1]=Value[1]*i;
             int hint_=1;
             Value=Fvalue(before,hint_,Value);           
         
         return {Value[0],Value[1]/((i+1)),Value[2]};
         }                                                      

  }

  
 

std::array<double, 3> Fvalue(board before,int hint_,std::array<double, 3> Value){
         if (TableB[before(0)*pow(10,5)+before(1)*pow(10,4)+before(2)*pow(10,3)+before(3)*pow(10,2)+before(4)*10+before(5)][hint_-1][1]!=-1){
             Value[1]+=TableB[before(0)*pow(10,5)+before(1)*pow(10,4)+before(2)*pow(10,3)+before(3)*pow(10,2)+before(4)*10+before(5)][hint_-1][1];
         }
         else{
         Value[1]+=TreeBefore(before,hint_)[1];                   
         }
         if (TableB[before(0)*pow(10,5)+before(1)*pow(10,4)+before(2)*pow(10,3)+before(3)*pow(10,2)+before(4)*10+before(5)][hint_-1][0]!=-1){
             Value[0]=std::min(Value[0],TableB[before(0)*pow(10,5)+before(1)*pow(10,4)+before(2)*pow(10,3)+before(3)*pow(10,2)+before(4)*10+before(5)][hint_-1][0]); }                      
         else{
         Value[0]=std::min(Value[0],TreeBefore(before,hint_)[0]);
         }             
         if (TableB[before(0)*pow(10,5)+before(1)*pow(10,4)+before(2)*pow(10,3)+before(3)*pow(10,2)+before(4)*10+before(5)][hint_-1][2]!=-1){
             Value[2]=std::max(Value[2],TableB[before(0)*pow(10,5)+before(1)*pow(10,4)+before(2)*pow(10,3)+before(3)*pow(10,2)+before(4)*10+before(5)][hint_-1][2]); }                      
         else{
         Value[2]=std::max(Value[2],TreeBefore(before,hint_)[2]);
         }  

return Value;
} 
 
 
 
std::array<double, 3> TreeBefore(board before,int hint)
    { 
       int N_Beforechildnode=0; 
       std::array<double, 3> Value={0,-1000000000,0};
  		for (int slide=3;slide>=0;slide=slide-1) {
        board after=before;
        board::reward reward = after.slide(slide); 
        if (reward != -1){               
             if (TableA[after(0)*pow(10,5)+after(1)*pow(10,4)+after(2)*pow(10,3)+after(3)*pow(10,2)+after(4)*10+after(5)][hint][slide][1]!=-1){
                 if (TableA[after(0)*pow(10,5)+after(1)*pow(10,4)+after(2)*pow(10,3)+after(3)*pow(10,2)+after(4)*10+after(5)][hint][slide][1]>Value[1]){
                    Value[0]=TableA[after(0)*pow(10,5)+after(1)*pow(10,4)+after(2)*pow(10,3)+after(3)*pow(10,2)+after(4)*10+after(5)][hint][slide][0];
                    Value[1]=TableA[after(0)*pow(10,5)+after(1)*pow(10,4)+after(2)*pow(10,3)+after(3)*pow(10,2)+after(4)*10+after(5)][hint][slide][1];
                    Value[2]=TableA[after(0)*pow(10,5)+after(1)*pow(10,4)+after(2)*pow(10,3)+after(3)*pow(10,2)+after(4)*10+after(5)][hint][slide][2];
                 }            
             }                      
             else{
                 if (TreeAfter(after,hint,slide)[1]>Value[1]){
                    Value=TreeAfter(after,hint,slide);
                 }  
             }   
        N_Beforechildnode++; 
  		  }
      }
      
      if (N_Beforechildnode!=0){ // non terminal state
      TableB[before(0)*pow(10,5)+before(1)*pow(10,4)+before(2)*pow(10,3)+before(3)*pow(10,2)+before(4)*10+before(5)][hint-1]={Value[0],Value[1],Value[2]};

      //std::cout<<"b "<<before<<" +"<<hint<<"= "<<Value<<'\n';
      //std::cout<<"b "<<before<<" +"<<hint<<'\n';
      return Value;
      }
      else{ //terminal state
        TableB[before(0)*pow(10,5)+before(1)*pow(10,4)+before(2)*pow(10,3)+before(3)*pow(10,2)+before(4)*10
        +before(5)][hint-1]={StateValue(before),StateValue(before),StateValue(before)};
        //std::cout<<"b "<<before<<" +"<<hint<<"= "<<StateValue(before)<<" terminal"<<'\n';
        return {StateValue(before),StateValue(before),StateValue(before)};    
      } 
    } 





};
