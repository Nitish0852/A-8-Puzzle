#include<iostream>
#include<algorithm> // for defination of sort 
#include<stdio.h>
#include<vector>   // for vector defination

using namespace std;

#define N 3	// Dimensions for Board

const bool SUCCESS=true;

class board {

public:
	int brd[N][N];
	int g,f;

	board *came_from;  // Self referential class memeber(linking class like structures)

	// Constructer
	board() {
		g=0;
		f=0;
		came_from = NULL; 
	}
	
	// Heuristic Function
	static int heuristic(board from,board to) {
		int i,j,dist=0;
		for(i=0;i<N;i++)
			for(j=0;j<N;j++)
				if(from.brd[i][j] != to.brd[i][j])
					dist++;
		return dist;
	}

	//  member-wise comparison
	bool operator ==(board a) {  
		for(int i=0;i<N;i++)
			for(int j=0;j<N;j++)
				if(this->brd[i][j] != a.brd[i][j])
					return false; 
		return true;
	}

	// Display Function
	void print() {
		for(int i=0;i<N;i++) {
			for(int j=0;j<N;j++)
				cout<<brd[i][j]<<" ";
			cout<<endl;
		}
		printf("Steps: %d\n",g);
		printf("Steps + Estimated	: %d\n\n",f);
	}
};

vector<board> output; // Dynamic Array "output" of type class board (i.e. dynamic array of class object)

// Compares two boards according to no of steps(i.e. f) 
bool lowerF(board a,board b) {    
	return a.f < b.f;
}

// Swapper function
void swap(board &a,int i,int j,int posi,int posj) {   
	int tmp;
	tmp = a.brd[i][j];
	a.brd[i][j] = a.brd[posi][posj];
	a.brd[posi][posj] = tmp;
}

// Checking for current board to be in vector stack
// To reduce Redundancy in board patterns
bool isInSet(board a,vector<board> b) {
	for(int i=0;i<b.size();i++)
		if(a == b[i])
			return true;

	return false;
}

// 
void addNeighbor(board current,board goal, int newI, int newJ, int posi, int posj,
					vector<board> &openset,vector<board> closedset) {
					
	board newboard = current;
	swap(newboard,newI,newJ,posi,posj);
		if(!isInSet(newboard,closedset)) {  // newboard not in vector stack of closedset

			int tentative_g_score = current.g + 1;

			// newboard not in vector stack of openset or current g< g of newboard
			if( !isInSet(newboard,openset) || tentative_g_score < newboard.g ) {  

				newboard.g = tentative_g_score;
				newboard.f = newboard.g + board::heuristic(newboard,goal);
                
                board *tmp = new board(); // 
                *tmp = current;
				newboard.came_from = tmp;
				openset.push_back(newboard);
			}
		}
}

void neighbors(board current,board goal, vector<board> &openset,vector<board> closedset) {
	int i,j,posi,posj;
	
	// Find the position of '0' (empty space)
	for(i=0;i<N;i++)
		for(j=0;j<N;j++)
			if(current.brd[i][j] == 0) {
				posi=i;
				posj=j;
			}

	i=posi; j=posj;

	if((i-1)>=0) {  // Left
		addNeighbor(current,goal,(i-1),j,posi,posj,openset,closedset);
	}
	
	if((i+1)<N) { // Right
		addNeighbor(current,goal,(i+1),j,posi,posj,openset,closedset);
	}

	if((j-1)>=0) { // Up
		addNeighbor(current,goal,i,(j-1),posi,posj,openset,closedset);
	}
	
	if((j+1)<N) { // Down
		addNeighbor(current,goal,i,(j+1),posi,posj,openset,closedset);
	}
}

// Path Retrace
void reconstruct_path(board current, vector<board> &came_from ) { // &came from : pointing to last board in the stack
    board *tmp = &current;
    while(tmp != NULL) {
        came_from.push_back(*tmp);
        tmp = tmp->came_from;
    }
}

// Worker Function
bool astar(board start, board goal) {
	vector<board> closedset;
	vector<board> openset;

	board current;

	start.g = 0;
	start.f = start.g + board::heuristic(start,goal);  // previous steps + heur distance 

	openset.push_back(start);  // adding given board
	
	while(!openset.empty()) {

		/* inbuilt sort function (lowerF is the compareInterval)
		   sort openset from beginning to end according to the result obtained from lowerF function
		   lowerF returns the board with lesser steps taken(i.e. f)*/
		sort(openset.begin(),openset.end(),lowerF);

		current = openset[0]; //select the board having lowest fscore value.
		
		// solvable or not
		if(current == goal) { // using overloading comparsion operater function 
			reconstruct_path(current,output);
			return SUCCESS;
		}

		openset.erase(openset.begin()); // remove starting element from openset stack
		closedset.push_back(current);   // push current to end of closeset stack 

		neighbors(current,goal,openset,closedset);
	}

	return !SUCCESS;// given board not solvable (i.e. it is from the 9!/2 unsolvables puzzles combinations)
}

int getInvCount(int arr[]) 
{ 
    int inv_count = 0; 
    for (int i = 0; i < 9 - 1; i++) 
        for (int j = i+1; j < 9; j++) 
             // Value 0 is used for empty space 
             if (arr[j] && arr[i] &&  arr[i] > arr[j]) 
                  inv_count++; 
    return inv_count; 
} 

bool isSolvable(int puzzle[3][3]) 
{ 
    // Count inversions in given 8 puzzle 
    int invCount = getInvCount((int *)puzzle); 
  
    // return true if inversion count is even. 
    return (invCount%2 == 0); 
} 

int main(int argv, char** args) {
	board start,goal;
	int i,j;
	
	freopen("in.txt","r",stdin);

	int copy_start[N][N];
	
	// Enter Given Board 
	//cout<<"Enter "<<N<<"x"<<N<<" board :\n";
	for(i=0;i<N;i++){
		for(j=0;j<N;j++){
			cin>>start.brd[i][j];
			copy_start[i][j] = start.brd[i][j];
		}
	}
			
	// Goal Board Entry
	//cout<<"Enter goal board :\n";
	for(i=0;i<N;i++)
		for(j=0;j<N;j++)
			cin>>goal.brd[i][j];

	

	if(isSolvable(copy_start)){

	if(astar(start,goal) == SUCCESS) {
        for(i=output.size()-1;i>=0;i--) {
            output[i].print();
        }
        cout<<"\nSUCCESSFUL.\nNumber of moves: "<<output.size()-1<<endl;
	}}
	else
		cout<<"\nUNSUCCESSFUL.\n";
	return 0;
}
