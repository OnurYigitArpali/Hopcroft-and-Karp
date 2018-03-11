//
//  main.cpp
//  dfa equality
//
//  Created by Onur Yiğit Arpalı on 19/12/2016.
//  Copyright © 2016 Onur Yiğit Arpalı. All rights reserved.
//

#include <iostream>
#include <fstream>
#include <istream>
#include <sstream>
#include <string>
#include <utility>
#include <vector>
#include <stack>
using namespace std;
enum SetType{
    NORMAL_SET,
    FINISH_SET
};

class State{
public:
    int info;
    int rank;
    int parent;
    SetType type;
    State();
};
State::State(){
    info = int();
    rank = int();
    type = NORMAL_SET;
    parent = int();
}
int stringordertoint(int symbol,const string orders,int statenumber,int numberofState_1){
    int dest;
    std::istringstream ss1(orders);
    ss1 >> dest;
    for(int i=0;i<symbol;i++){
        ss1 >> dest;
    }
    if(statenumber>=numberofState_1){
        dest +=numberofState_1;
    }
    return dest;
}
vector<int> extractFinishState(string inp1,string inp2,int numberofStates_1){
    vector<int> finStates;
    string str_tmp;
    int int_tmp;
    std::istringstream ss1(inp1);
    ss1 >> int_tmp;
    while(ss1)
    {
        finStates.push_back(int_tmp);
        ss1 >> int_tmp;
    }
    std::istringstream ss2(inp2);
    ss2 >> int_tmp;
    int_tmp += numberofStates_1;
    while(ss2)
    {
        finStates.push_back(int_tmp);
        ss2 >> int_tmp;
        int_tmp += numberofStates_1;
    }
    
    return finStates;
}
vector<State> Make_Set(int x,vector<int> finishesStates){
    vector<State> main_vector;
    for(int i=0 ; i<x ;i++){
        State* node = new State();
        node->info = i;
        node->rank = 0;
        node->parent = i;
        main_vector.push_back(*node);
    }
    for(int i=0;i<finishesStates.size();i++)
        main_vector[finishesStates[i]].type = FINISH_SET;
    return main_vector;
}
int Find_Set(int x,vector<State> &vec){
    if(vec[x].info != vec[vec[x].parent].info)
        vec[x].parent = Find_Set(vec[vec[x].parent].info,vec);
    return vec[x].parent;
}

void Link(int x,int y,vector<State> &vec){
    if(vec[x].rank > vec[y].rank)
        vec[y].parent = vec[x].info;
    else{
        vec[x].parent = vec[y].info;
        if(vec[x].rank == vec[y].rank)
            vec[y].rank +=1;
    }
}

void Union(int x,int y,vector<State> &vec){
    Link(Find_Set(vec[x].info,vec),Find_Set(vec[y].info,vec),vec);
}

void dfa_equality(int numberofStates_1 , string finishState_1 , int startState_1 , int numberofSymbols_1 , vector<string> stateOrders ,
                  int numberofStates_2 , string finishState_2 , int startState_2 , int numberofSymbols_2 ){
    
    int totalnumberofState = numberofStates_1 + numberofStates_2;
    int tmpstate_1=0,tmpstate_2=0;
    vector<int> finishStates = extractFinishState(finishState_1,finishState_2,numberofStates_1);
    //Path comparisondaki pairleri tutan stack
    stack< pair<int,int> > pairs;
    
    if(numberofSymbols_1!=numberofSymbols_2){
        cout<<"notequal"<<endl;
        return;
    }
    //tüm stateleri tutan vector
    vector<State> main_vector = Make_Set(totalnumberofState,finishStates);
    
    
    pair<int,int> start_pair = make_pair(startState_1, startState_2+numberofStates_1);
    pairs.push(start_pair);
    
    Union(main_vector[start_pair.first].info,main_vector[start_pair.second].info,main_vector);
    
    while (!pairs.empty()) {
        pair<int,int> tmp = pairs.top();
        pairs.pop();
        
        for(int i=0 ; i<numberofSymbols_1 ; i++){
            
            tmpstate_1 = Find_Set(main_vector[stringordertoint(i,stateOrders[tmp.first],tmp.first,numberofStates_1)].info,main_vector);
            tmpstate_2 = Find_Set(main_vector[stringordertoint(i,stateOrders[tmp.second],tmp.second,numberofStates_1)].info,main_vector);
            
            if(tmpstate_1 != (tmpstate_2)){
                Union(main_vector[tmpstate_1].info, main_vector[tmpstate_2].info,main_vector);
                pair<int,int> newpair = make_pair(tmpstate_1, tmpstate_2);
                pairs.push(newpair);
            }
            
        }
        
    }
    for(int j=0; j<totalnumberofState; j++){
        if(main_vector[j].type != main_vector[main_vector[j].parent].type){
            cout<<"notequal"<<endl;
            return;
        }
    }
    cout<<"equal"<<endl;
    return;
}

int main(int argc, const char * argv[]) {
    
    int numberofStates_1 = 0,numberofSymbols_1 = 0,startState_1 = 0,
    numberofSymbols_2 = 0,numberofStates_2 = 0,startState_2 = 0;
    string statesinOrder_1,statesinOrder_2,finishState_1,finishState_2;
    vector<string> statesOrder;
    int linenumber = 1;
    
    ifstream file_1(argv[1]);
    ifstream file_2(argv[2]);
    
    for( string line; getline( file_1, line ); )
    {
        switch (linenumber) {
            case 1:
                numberofStates_1 = atoi(line.c_str());
                linenumber++;
                break;
            case 2:
                numberofSymbols_1 = atoi(line.c_str());
                linenumber++;
                break;
            case 3:
                startState_1 = atoi(line.c_str());
                linenumber++;
                break;
            case 4:
                finishState_1.assign(line);
                linenumber++;
                break;
            default:
                statesinOrder_1.assign(line);
                statesOrder.push_back(statesinOrder_1);
                linenumber++;
                break;
        }
        
    }
    linenumber = 1;
    
    for( string line; getline( file_2, line ); )
    {
        switch (linenumber) {
            case 1:
                numberofStates_2 = atoi(line.c_str());
                linenumber++;
                break;
            case 2:
                numberofSymbols_2 = atoi(line.c_str());
                linenumber++;
                break;
            case 3:
                startState_2 = atoi(line.c_str());
                linenumber++;
                break;
            case 4:
                finishState_2.assign(line);
                linenumber++;
                break;
            default:
                statesinOrder_2.assign(line);
                statesOrder.push_back(statesinOrder_2);
                linenumber++;
                break;
        }
        
    }
    file_1.close();
    file_2.close();
    
    dfa_equality(numberofStates_1, finishState_1, startState_1, numberofSymbols_1, statesOrder,
                 numberofStates_2, finishState_2, startState_2, numberofSymbols_2);
    
    
    return 0;
}
