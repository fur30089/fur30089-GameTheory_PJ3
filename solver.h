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
 
      //double child[3];
      //child[1]=1;
      //std::cout<<child[1]<<'\n'; 
      //TableA[1][1][1]=29;
      //std::cout<<TableA[1][1][1];


      /*
      board before;
      before(2)=2;
      before(3)=1;
      board after;      
      after(1)=2;
      after(3)=1; 
      */  
      
      
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
               
  
      
      //double value=TreeBefore(state,2);
      double value=TreeAfter(state,0,0);
      //std::cout<<"value:"<<state<<" +1= "<<value<<'\n';

      


      //std::cout << TableB[5][5] << '\n'; //20        
      //TreeBefore(before,3);
      //std::cout<<v;
      //std::cout<<boardboard;
    


      
		// TODO: explore the tree and save the result

  

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
    value_t min=60;
    value_t avg=61;
    value_t max=62;
        
    if (type.is_before()){
    avg=TableB[state(0)*pow(15,5)+state(1)*pow(15,4)+state(2)*pow(15,3)+state(3)*pow(15,2)+state(4)*15+state(5)][hint-1][1];
    }
    else{
     int n=0;
     for (int i=0;i<=3;i++){
     if (TableA[state(0)*pow(15,5)+state(1)*pow(15,4)+state(2)*pow(15,3)+state(3)*pow(15,2)+state(4)*15+state(5)][hint][i][1]==-1) continue;
     avg=TableA[state(0)*pow(15,5)+state(1)*pow(15,4)+state(2)*pow(15,3)+state(3)*pow(15,2)+state(4)*15+state(5)][hint][i][1];  
     n=1;   
     }
     if (n==0)  avg=-1; 
    }

     
		return {min,avg,max};
	}

private:
	// TODO: place your transposition table here
  
  
  std::vector<std::vector<std::vector<std::vector<double> > > > TableA = make_4d_vector<double>(pow(15,6), 4, 4,3, -1);
  //std::vector<std::vector<std::vector<double> > > TableA = make_3d_vector<double>(pow(15,6), 4, 4, -1);
  //std::cout << TableA[5][5][5] << '\n'; //20
  
  std::vector<std::vector<std::vector<double> > > TableB = make_3d_vector<double>(pow(15,6), 3, 3, -1);
  //std::vector<std::vector<double> > TableB = make_2d_vector<double>(pow(15,6), 3, -1);
  //TableB[5][5]=78.8; 
  //std::cout << TableB[5][5] << '\n'; //20  



 
double TreeAfter(board after,int hint,int last)
    {  
       if (hint==0){
          int i=0;
          double Vavg=0; 
          for (int pos=0;pos<=5;pos++) {          
            for (int tile=1;tile<=3;tile++) {
                    board before=after; 
                    before(pos) = tile;
                    Vavg=allhint(before,hint,i,Vavg); 
                    i++;
            }           
  		    }
             TableA[after(0)*pow(15,5)+after(1)*pow(15,4)+after(2)*pow(15,3)+after(3)*pow(15,2)+after(4)*15+after(5)][hint][last][1]=Vavg;
             std::cout<<"a "<<after<<" +"<<hint<<"= "<<Vavg<<'\n'; 
             return TableA[after(0)*pow(15,5)+after(1)*pow(15,4)+after(2)*pow(15,3)+after(3)*pow(15,2)+after(4)*15+after(5)][hint][last][1];     
       }   
         
       else{
          if (last==0){
             int i=0;
             double Vavg=0; 
             for (int pos=3;pos<=5;pos++) {                     
                board before=after; 
                if (before(pos) != 0) continue;   
                before(pos) = hint;
                Vavg=allhint(before,hint,i,Vavg);
                i++;
                //std::cout<<before<<'\n';            
               }
             TableA[after(0)*pow(15,5)+after(1)*pow(15,4)+after(2)*pow(15,3)+after(3)*pow(15,2)+after(4)*15+after(5)][hint][last][1]=Vavg;
             std::cout<<"a "<<after<<" +"<<hint<<"= "<<Vavg<<'\n'; 
             return TableA[after(0)*pow(15,5)+after(1)*pow(15,4)+after(2)*pow(15,3)+after(3)*pow(15,2)+after(4)*15+after(5)][hint][last][1];               
            }
          else if (last==1){
             int i=0;
             double Vavg=0; 
             for (int pos=0;pos<=3;pos=pos+3) {           
                board before=after; 
                if (before(pos) != 0) continue;   
                before(pos) = hint;
                Vavg=allhint(before,hint,i,Vavg);
                i++;
                //std::cout<<before<<'\n';            
               }
             TableA[after(0)*pow(15,5)+after(1)*pow(15,4)+after(2)*pow(15,3)+after(3)*pow(15,2)+after(4)*15+after(5)][hint][last][1]=Vavg;
             std::cout<<"a "<<after<<" +"<<hint<<"= "<<Vavg<<'\n'; 
             return TableA[after(0)*pow(15,5)+after(1)*pow(15,4)+after(2)*pow(15,3)+after(3)*pow(15,2)+after(4)*15+after(5)][hint][last][1];  
            }
          else if (last==2){
             int i=0;
             double Vavg=0; 
             for (int pos=0;pos<=2;pos++) {         
                board before=after; 
                if (before(pos) != 0) continue;   
                before(pos) = hint;
                Vavg=allhint(before,hint,i,Vavg);
                i++;
                //std::cout<<before<<'\n';            
               }
             TableA[after(0)*pow(15,5)+after(1)*pow(15,4)+after(2)*pow(15,3)+after(3)*pow(15,2)+after(4)*15+after(5)][hint][last][1]=Vavg;
             std::cout<<"a "<<after<<" +"<<hint<<"= "<<Vavg<<'\n'; 
             return TableA[after(0)*pow(15,5)+after(1)*pow(15,4)+after(2)*pow(15,3)+after(3)*pow(15,2)+after(4)*15+after(5)][hint][last][1];       
            }
          else if (last==3){
             int i=0;
             double Vavg=0; 
             for (int pos=2;pos<=5;pos=pos+3) {          
                board before=after; 
                if (before(pos) != 0) continue;   
                before(pos) = hint;
                Vavg=allhint(before,hint,i,Vavg);
                i++;
                //std::cout<<before<<'\n';            
               }
             TableA[after(0)*pow(15,5)+after(1)*pow(15,4)+after(2)*pow(15,3)+after(3)*pow(15,2)+after(4)*15+after(5)][hint][last][1]=Vavg;
             std::cout<<"a "<<after<<" +"<<hint<<"= "<<Vavg<<'\n'; 
             return TableA[after(0)*pow(15,5)+after(1)*pow(15,4)+after(2)*pow(15,3)+after(3)*pow(15,2)+after(4)*15+after(5)][hint][last][1];                 
           }        
      }
}
       

  double allhint(board before, int hint, int i, double Vavg)
      { int total=0;
       for (int pos=0;pos<=5;pos++) {         
          total+=IndextoValue(before(pos));    
       }
       if (total%6==0){
          Vavg=Vavg*i*3;
          for (int hint_=1;hint_<=3;hint_++){
             //std::cout<<before<<" +"<<hint_<<'\n';
             if (TableB[before(0)*pow(15,5)+before(1)*pow(15,4)+before(2)*pow(15,3)+before(3)*pow(15,2)+before(4)*15+before(5)][hint_-1][1]!=-1){
                 Vavg+=TableB[before(0)*pow(15,5)+before(1)*pow(15,4)+before(2)*pow(15,3)+before(3)*pow(15,2)+before(4)*15+before(5)][hint_-1][1];
             }
             else{
             Vavg+=TreeBefore(before,hint_);
                     
          }
         }
         return Vavg/((i+1)*3);
       }
       else if (total%6==1){
          Vavg=Vavg*i*2;
          for (int hint_=2;hint_<=3;hint_++){
             //std::cout<<before<<" +"<<hint_<<'\n';
             if (TableB[before(0)*pow(15,5)+before(1)*pow(15,4)+before(2)*pow(15,3)+before(3)*pow(15,2)+before(4)*15+before(5)][hint_-1][1]!=-1){
                 Vavg+=TableB[before(0)*pow(15,5)+before(1)*pow(15,4)+before(2)*pow(15,3)+before(3)*pow(15,2)+before(4)*15+before(5)][hint_-1][1];
             }
             else{
             Vavg+=TreeBefore(before,hint_);
                     
          }
         }
         return Vavg/((i+1)*2);
       } 
       else if (total%6==2){
          Vavg=Vavg*i*2;
          for (int hint_=1;hint_<=3;hint_=hint_+2){
             //std::cout<<before<<" +"<<hint_<<'\n';
             if (TableB[before(0)*pow(15,5)+before(1)*pow(15,4)+before(2)*pow(15,3)+before(3)*pow(15,2)+before(4)*15+before(5)][hint_-1][1]!=-1){
                 Vavg+=TableB[before(0)*pow(15,5)+before(1)*pow(15,4)+before(2)*pow(15,3)+before(3)*pow(15,2)+before(4)*15+before(5)][hint_-1][1];
             }
             else{
             Vavg+=TreeBefore(before,hint_);
                    
          }
         } 
         return Vavg/((i+1)*2);
       }                    
      else if (total%6==3 & (hint==3 or hint==0) ){
          Vavg=Vavg*i*2;
          for (int hint_=1;hint_<=2;hint_++){
             //std::cout<<before<<" +"<<hint_<<'\n';
             if (TableB[before(0)*pow(15,5)+before(1)*pow(15,4)+before(2)*pow(15,3)+before(3)*pow(15,2)+before(4)*15+before(5)][hint_-1][1]!=-1){
                 Vavg+=TableB[before(0)*pow(15,5)+before(1)*pow(15,4)+before(2)*pow(15,3)+before(3)*pow(15,2)+before(4)*15+before(5)][hint_-1][1];
             }
             else{
             Vavg+=TreeBefore(before,hint_);
          }           
          }
         return Vavg/((i+1)*2);
          }
      else if (total%6==3 & hint!=3){
             Vavg=Vavg*i;
             int hint_=3;
             //std::cout<<before<<" +"<<hint_<<'\n';
             if (TableB[before(0)*pow(15,5)+before(1)*pow(15,4)+before(2)*pow(15,3)+before(3)*pow(15,2)+before(4)*15+before(5)][hint_-1][1]!=-1){
                 Vavg+=TableB[before(0)*pow(15,5)+before(1)*pow(15,4)+before(2)*pow(15,3)+before(3)*pow(15,2)+before(4)*15+before(5)][hint_-1][1];
             }
             else{
             Vavg+=TreeBefore(before,hint_);
                    
          }
          return Vavg/(i+1);
         }            
                    
      else if (total%6==4){
             Vavg=Vavg*i;       
             int hint_=2;
             //std::cout<<before<<" +"<<hint_<<'\n';
             if (TableB[before(0)*pow(15,5)+before(1)*pow(15,4)+before(2)*pow(15,3)+before(3)*pow(15,2)+before(4)*15+before(5)][hint_-1][1]!=-1){
                 Vavg+=TableB[before(0)*pow(15,5)+before(1)*pow(15,4)+before(2)*pow(15,3)+before(3)*pow(15,2)+before(4)*15+before(5)][hint_-1][1];
             }
             else{
             Vavg+=TreeBefore(before,hint_);       
          }
          return Vavg/(i+1);
         }              
      else if (total%6==5){
             Vavg=Vavg*i;
             int hint_=1;
             //std::cout<<before<<" +"<<hint_<<'\n';
             if (TableB[before(0)*pow(15,5)+before(1)*pow(15,4)+before(2)*pow(15,3)+before(3)*pow(15,2)+before(4)*15+before(5)][hint_-1][1]!=-1){
                 Vavg+=TableB[before(0)*pow(15,5)+before(1)*pow(15,4)+before(2)*pow(15,3)+before(3)*pow(15,2)+before(4)*15+before(5)][hint_-1][1];
             }
             else{
             Vavg+=TreeBefore(before,hint_);      
          }
          return Vavg/(i+1);
         }                                                      

  }

  
 
 
 
  double TreeBefore(board before,int hint)
    { 
       int N_Beforechildnode=0; 
       double Vmax=-100000000;
  		for (int slide=3;slide>=0;slide=slide-1) {
        board after=before;
        board::reward reward = after.slide(slide); 
        if (reward != -1){
           /*    
             if (TableA[after(0)*pow(15,5)+after(1)*pow(15,4)+after(2)*pow(15,3)+after(3)*pow(15,2)+after(4)*15+after(5)][hint][slide]!=-1){
                 Vmax=std::max(Vmax,TableA[after(0)*pow(15,5)+after(1)*pow(15,4)+after(2)*pow(15,3)+after(3)*pow(15,2)+after(4)*15+after(5)][hint][slide]);                 
             }  
           */         
            // else{
             Vmax=std::max(Vmax,TreeAfter(after,hint,slide));
             //}
        N_Beforechildnode++; 
  		  }
      }
      
      if (N_Beforechildnode!=0){ // non terminal state
      TableB[before(0)*pow(15,5)+before(1)*pow(15,4)+before(2)*pow(15,3)+before(3)*pow(15,2)+before(4)*15+before(5)][hint-1][1]=Vmax;

      std::cout<<"b "<<before<<" +"<<hint<<"= "<<Vmax<<'\n';
      return Vmax;
      }
      else{ //terminal state
        TableB[before(0)*pow(15,5)+before(1)*pow(15,4)+before(2)*pow(15,3)+before(3)*pow(15,2)+before(4)*15+before(5)][hint-1][1]=StateValue(before);
        std::cout<<"b "<<before<<" +"<<hint<<"= "<<StateValue(before)<<" terminal"<<'\n';
        return StateValue(before);    
      } 
    } 





};
