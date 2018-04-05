#include <iostream>
#include <string>
#include <vector>
#include <algorithm>

#define MAX_LEGAL_ACTIONS 177
#define MIN_SIZE 5
#define MAX_SIZE 7

#define DEBUG true

using namespace std;

enum ActionType { UNKNOWN = -1, MOVEANDBUILD = 0, PUSHANDBUILD = 1};
enum Direction { NONE = -1, N = 0, NE = 1, E = 2, SE = 3, S = 4, SW = 5, W = 6, NW = 7};

class Point;
class Woman;
class Action;
class Game;


Game *game;

int gridSize, unitsPerPlayer;
int legalActions;

class Point {

public:
	Point() {};
	Point(int px, int py) :	x(px), y(py){}
	Point(const Point &ppoint)
	{
		this->x = ppoint.x;
		this->y = ppoint.y;
	}

	int x;
	int y;

	bool operator==(const Point &a)
	{
		return (this->x == a.x && this->y == a.y);
	}
	bool operator!=(const Point &a)
	{
		return !(*this == a);
	}
	void set(Point p)
	{
		this->x = p.x;
		this->y = p.y;
	}
	void set(int px, int py)
	{
		this->x = px;
		this->y = py;
	}
};

class Cell : public Point {
public:
	Cell(int px = 0, int py = 0, int pheight = 0) :	Point(px, py), height(pheight){}

	int height;
};

class Woman : public Point {
public:
	//Woman(void) {};
	Woman(int px = 0, int py = 0) : Point(px, py){};

	~Woman(void) {};
	void copy(Woman a) {}

	void reset() 
	{
		visible = true;
	}
	void print() {}


	void updateWithInputs(int px, int py)
	{
		set(px, py);
	}
	bool isNeighbour(Point pbuildDest)
	{
		return (abs(pbuildDest.x - x) <= 1 && abs(pbuildDest.y - y) <= 1);
	}

	int currentHeight;
	bool visible;
};

class Action {
public:
	//Action(void) {};
	Action(ActionType ptype = UNKNOWN, int pindex = 0, Direction pdir1 = NONE, Direction pdir2 = NONE) : index(pindex), dir1(pdir1), dir2(pdir2) {};

	~Action(void) {};
	void copy(Action a){}

	void reset()
	{
		type = UNKNOWN;
		index = 0;
		dir1 = NONE;
		dir2 = NONE;
		height = 0;
		score = 0;
	}

	void print(){}

	virtual void updateWithInputs(string ptype, int pindex, string pdir1, string pdir2)
	{
		if (ptype == "MOVE&BUILD") type = MOVEANDBUILD;
		else if (ptype == "PUSH&BUILD") type = PUSHANDBUILD;
		else type = UNKNOWN;
		
		index = pindex;
		if (pdir1 == "N") dir1 = N;			 
		else if (pdir1 == "NE") dir1 = NE;	 
		else if (pdir1 == "E")  dir1 = E;	 
		else if (pdir1 == "SE") dir1 = SE;	 
		else if (pdir1 == "S")  dir1 = S;	 
		else if (pdir1 == "SW") dir1 = SW;	 
		else if (pdir1 == "W")  dir1 = W;	 
		else if (pdir1 == "NW") dir1 = NW;	 

		if (pdir2 == "N") dir2 = N;
		else if (pdir2 == "NE") dir2 = NE;
		else if (pdir2 == "E")  dir2 = E;
		else if (pdir2 == "SE") dir2 = SE;
		else if (pdir2 == "S")  dir2 = S;
		else if (pdir2 == "SW") dir2 = SW;
		else if (pdir2 == "W")  dir2 = W;
		else if (pdir2 == "NW") dir2 = NW;
	}

	void output()
	{
		if (dir1 == NONE)
		{
			cout << "GG";
		}
		else
		{
			switch (type)
			{
			case MOVEANDBUILD: cout << "MOVE&BUILD "; break;
			case PUSHANDBUILD: cout << "PUSH&BUILD "; break;
			case UNKNOWN: cout << "UNKNOWN "; break;
			default: break;
			}
			cout << index << " ";

			switch (dir1)
			{
			case N:  cout << "N ";  break;
			case NE: cout << "NE "; break;
			case E:  cout << "E ";  break;
			case SE: cout << "SE "; break;
			case S:  cout << "S ";  break;
			case SW: cout << "SW "; break;
			case W:  cout << "W ";  break;
			case NW: cout << "NW "; break;
			default: break;
			}
			switch (dir2)
			{
			case N:  cout << "N ";  break;
			case NE: cout << "NE "; break;
			case E:  cout << "E ";  break;
			case SE: cout << "SE "; break;
			case S:  cout << "S ";  break;
			case SW: cout << "SW "; break;
			case W:  cout << "W ";  break;
			case NW: cout << "NW "; break;
			default: break;
			}
			cout << endl;
		}

	}
	void outputDebug()
	{
		switch (type)
		{
			case MOVEANDBUILD: cerr << "MOVE&BUILD "; break;
			case PUSHANDBUILD: cerr << "PUSH&BUILD "; break;
			case UNKNOWN: cerr << "UNKNOWN "; break;
			default: break;
		}
		cerr << index << " ";

		switch (dir1)
		{
		case N:  cerr << "N ";  break;
		case NE: cerr << "NE "; break;
		case E:  cerr << "E ";  break;
		case SE: cerr << "SE "; break;
		case S:  cerr << "S ";  break;
		case SW: cerr << "SW "; break;
		case W:  cerr << "W ";  break;
		case NW: cerr << "NW "; break;
		default: break;
		}
		switch (dir2)
		{
		case N:  cerr << "N ";  break;
		case NE: cerr << "NE "; break;
		case E:  cerr << "E ";  break;
		case SE: cerr << "SE "; break;
		case S:  cerr << "S ";  break;
		case SW: cerr << "SW "; break;
		case W:  cerr << "W ";  break;
		case NW: cerr << "NW "; break;
		default: break;
		}
		cerr << endl;

	}

	void set(ActionType ptype, int pindex, Direction pdir1, Direction pdir2, int pheight = 0)
	{
		type = ptype;
		index = pindex;
		dir1 = pdir1;
		dir2 = pdir2;
		height = pheight;
	}

	ActionType type;
    int index;
	Direction dir1;
	Direction dir2;

	int height;

	int score;
};

class Game {
public:
	Woman *myWomen[2];
	Woman *opWomen[2];
	Action *myLegalActions[MAX_LEGAL_ACTIONS];
	Direction allDirestions[8];

	Cell **grid;

	Game()
	{
		myWomen[0] = new Woman();
		myWomen[1] = new Woman();
		opWomen[0] = new Woman();
		opWomen[1] = new Woman();
		for (int i = 0; i < MAX_LEGAL_ACTIONS; ++i)
		{
			myLegalActions[i] = new Action();
		}
		myWomen[0]->currentHeight = 0;
		myWomen[1]->currentHeight = 0;
		opWomen[0]->currentHeight = 0;
		opWomen[1]->currentHeight = 0;

		grid = new Cell*[MAX_SIZE];
		for (int i = 0; i < MAX_SIZE; i++) grid[i] = new Cell[MAX_SIZE];

		allDirestions[0] = N  ;
		allDirestions[1] = NE ;
		allDirestions[2] = E  ;
		allDirestions[3] = SE ;
		allDirestions[4] = S  ;
		allDirestions[5] = SW ;
		allDirestions[6] = W  ;
		allDirestions[7] = NW ;
	};

	void printGrid()
	{
		//cerr << "__| 0 1 2 3 4 5 6 7 8 9 101112" << endl;
		for (int i = 0; i < gridSize; i++)
		{
			if (i<10) cerr << " ";
			cerr << i << "| ";
			for (int j = 0; j < gridSize; j++)
			{
				if( grid[j][i].height != -1)
				{
				   cerr << grid[j][i].height << " ";
				}else cerr << "X ";
			}
			cerr << endl;
		}
	}

	void resetBeforeInputs()
	{
		myWomen[0]->reset();
		opWomen[0]->reset();

		for (int i = 0; i < MAX_LEGAL_ACTIONS; ++i)
		{
			myLegalActions[i]->reset();
		}
	}

	void getInputInit()
	{
		cin >> gridSize; cin.ignore();
		cin >> unitsPerPlayer; cin.ignore();
	}
	void getInputGrid()
	{
		for (int i = 0; i < gridSize; i++) 
		{
			string row;
			cin >> row; cin.ignore();

			for (int j = 0; j < row.size(); j++)
			{
				if (row[j] == 46) // 46 == "."
				{
					grid[j][i].height = -1;
				}else grid[j][i].height = (int)row[j] - 48;
			}
		}
	}
	void getInputPlayers()
	{
		for (int i = 0; i < unitsPerPlayer; i++) {
			int unitX, unitY;
			cin >> unitX >> unitY; cin.ignore();
			myWomen[i]->updateWithInputs(unitX, unitY);
			myWomen[i]->currentHeight = grid[unitX][unitY].height;
		}
		for (int i = 0; i < unitsPerPlayer; i++) {
			int otherX, otherY;
			cin >> otherX >> otherY; cin.ignore();
			if (otherX != -1 && otherY != -1)
			{
				opWomen[i]->updateWithInputs(otherX, otherY);
				opWomen[i]->currentHeight = grid[otherX][otherY].height;
			}
			else opWomen[i]->visible = false;

		}
	}
	void getInputActions()
	{
		cin >> legalActions; cin.ignore();
		for (int i = 0; i < legalActions; i++) {
			string atype;
			int index;
			string dir1, dir2;
			cin >> atype >> index >> dir1 >> dir2; cin.ignore();
			myLegalActions[i]->updateWithInputs(atype, index, dir1, dir2);
		}
	}
	void getInputs()
	{
		getInputGrid();
		getInputPlayers();
		getInputActions();
	}

	Point getDestPoint(Direction dir, Point startP)
	{
		switch (dir)
		{
		case N:  return Point(startP.x    , startP.y - 1); break;
		case NE: return Point(startP.x + 1, startP.y - 1); break;
		case E:  return Point(startP.x + 1, startP.y    ); break;
		case SE: return Point(startP.x + 1, startP.y + 1); break;
		case S:  return Point(startP.x    , startP.y + 1); break;
		case SW: return Point(startP.x - 1, startP.y + 1); break;
		case W:  return Point(startP.x - 1, startP.y    ); break;
		case NW: return Point(startP.x - 1, startP.y - 1); break;
		}
	}

	bool canMove(Point p, int wInd)
	{
		if ((p.x < 0 || p.y < 0)
			|| (p.x >= gridSize || p.y >= gridSize)
			|| (p.x == myWomen[(wInd == 0) ? 1 : 0]->x && p.y == myWomen[(wInd == 0) ? 1 : 0]->y)
			|| (p.x == opWomen[0]->x && p.y == opWomen[0]->y)
			|| (p.x == opWomen[1]->x && p.y == opWomen[1]->y)
			|| (grid[p.x][p.y].height == 4)
			|| (grid[p.x][p.y].height == -1)) return false;
		else return true;

	}
	bool canBuild(Point p, int wInd)
	{
		if( (p.x < 0 || p.y < 0)                     
		 || (p.x >= gridSize || p.y >= gridSize)       
		 || (p.x == opWomen[(wInd == 0) ? 1:0]->x && p.y == opWomen[(wInd == 0) ? 1:0]->y)
		 || (grid[p.x][p.y].height < 0) 
		 || (grid[p.x][p.y].height == 4)
		 || (grid[p.x][p.y].height == -1)) return false;
		else return true;		
	}

	Action selectBestAction()
	{
		for (int w = 0; w < 2; w++)
		{
			for (int i = 0; i < legalActions; i++)
			{
				if (myLegalActions[i]->index != w) continue;
				if (myLegalActions[i]->type != MOVEANDBUILD) continue;

				Point dest = getDestPoint(myLegalActions[i]->dir1, (Point)*myWomen[w]);
				Point buildDest = getDestPoint(myLegalActions[i]->dir2, dest);
				
				// Dont build if op can score right after
				if (opWomen[0]->currentHeight == 2 && opWomen[0]->isNeighbour(buildDest) && grid[buildDest.x][buildDest.y].height == 2) continue;
				if (opWomen[1]->currentHeight == 2 && opWomen[1]->isNeighbour(buildDest) && grid[buildDest.x][buildDest.y].height == 2) continue;

				// Dont build if op can jump on it right after
				if (opWomen[0]->visible && opWomen[0]->isNeighbour(buildDest)) continue;
				if (opWomen[1]->visible && opWomen[1]->isNeighbour(buildDest)) continue;

				myLegalActions[i]->score += 20 * (grid[dest.x][dest.y].height - myWomen[w]->currentHeight);
				if (grid[dest.x][dest.y].height == 3) myLegalActions[i]->score += 10; // Bonus for scoring

				// Build so you can jump on it right after
				if (myWomen[w]->currentHeight == grid[buildDest.x][buildDest.y].height && grid[buildDest.x][buildDest.y].height != 3) myLegalActions[i]->score += 1;
				
				// -1 if build too high
				if (grid[buildDest.x][buildDest.y].height + 1 - myWomen[w]->currentHeight > 2) myLegalActions[i]->score -= 1;

				// DONT MOVE TO GET PUSHED RIGHT AFTER
				if (opWomen[0]->visible && opWomen[0]->isNeighbour(dest) && grid[dest.x][dest.y].height != 3) myLegalActions[i]->score = 0;
				if (opWomen[1]->visible && opWomen[1]->isNeighbour(dest) && grid[dest.x][dest.y].height != 3) myLegalActions[i]->score = 0;

				// BETTER SCORE IF MORE MOVE POSSIBLE
				grid[buildDest.x][buildDest.y].height++;
				int possibleDest = 0;
				for (int d = 0; d < 8; d++)
				{
					Point testDest = getDestPoint(allDirestions[d], dest);
					if (canMove(testDest, w) && grid[testDest.x][testDest.y].height <= myWomen[w]->currentHeight + 1) possibleDest++;
				}
				myLegalActions[i]->score += possibleDest;
				if (possibleDest == 0) myLegalActions[i]->score = -10000;

				grid[buildDest.x][buildDest.y].height--;

				// BETTER SCORE IF BUILD ON SPOTS WHERE THERE ARE FEW POSSIBLE MOVES (prevent being stuck ourself)
				int possibleBuildDest = 0;
				for (int d = 0; d < 8; d++)
				{
					Point testDest = getDestPoint(allDirestions[d], buildDest);
					if (canMove(testDest, w) && grid[testDest.x][testDest.y].height <= myWomen[w]->currentHeight + 1) possibleBuildDest++;
				}
				if (grid[buildDest.x][buildDest.y].height == 3) myLegalActions[i]->score -= possibleBuildDest;

				// Check if we reduce move possibilities for my other woman
				int possibleDestBefore = 0;
				for (int d = 0; d < 8; d++)
				{
					Point testDest = getDestPoint(allDirestions[d], (Point)*myWomen[(w == 0) ? 1 : 0]);
					if (canMove(testDest, w) && grid[testDest.x][testDest.y].height <= myWomen[(w == 0) ? 1 : 0]->currentHeight + 1) possibleDestBefore++;
				}
				grid[buildDest.x][buildDest.y].height++;
				int possibleDestAfter = 0;
				for (int d = 0; d < 8; d++)
				{
					Point testDest = getDestPoint(allDirestions[d], (Point)*myWomen[(w == 0) ? 1 : 0]);
					if (canMove(testDest, w) && grid[testDest.x][testDest.y].height <= myWomen[(w == 0) ? 1 : 0]->currentHeight + 1) possibleDestAfter++;
				}
				grid[buildDest.x][buildDest.y].height--;
				
				if (possibleDestBefore > 0 && possibleDestAfter == 0) myLegalActions[i]->score = 0;

				//if (DEBUG && myLegalActions[i]->dir1 == N  && myLegalActions[i]->dir2 == S) cerr << "0000 myLegalActions[i]->score     " << myLegalActions[i]->score << endl;
				//if (myLegalActions[i]->score < 0) myLegalActions[i]->score = 0;
			}


			for (int i = 0; i < legalActions; i++)
			{
				if (myLegalActions[i]->index != w) continue;
				if (myLegalActions[i]->type != PUSHANDBUILD) continue;

				Point dest = getDestPoint(myLegalActions[i]->dir1, (Point)*myWomen[w]);
				Point pushDest = getDestPoint(myLegalActions[i]->dir2, dest);

				// PUSH ON ANOTHER WOMEN
				if ((opWomen[0]->x == pushDest.x && opWomen[0]->y == pushDest.y) ||
					(opWomen[1]->x == pushDest.x && opWomen[1]->y == pushDest.y) ||
					(myWomen[(w == 0) ? 1 : 0]->x == pushDest.x && myWomen[(w == 0) ? 1 : 0]->y == pushDest.y))
				{
					continue;
				}


				if(grid[dest.x][dest.y].height == 3) myLegalActions[i]->score += 50;
				myLegalActions[i]->score += 100 * (grid[dest.x][dest.y].height - grid[pushDest.x][pushDest.y].height);

				//if (DEBUG && myLegalActions[i]->dir1 == NE)
				//{
				//	myLegalActions[i]->outputDebug();
				//	cerr << "push score     " << myLegalActions[i]->score << endl;
				//}
			}
		}

		Action bestAction;
		int bestScore = -100;
		for (int i = 0; i < legalActions; i++)
		{
			if (myLegalActions[i]->score > bestScore)
			{
				bestScore = myLegalActions[i]->score;		
				bestAction.score = myLegalActions[i]->score;
				bestAction.set(myLegalActions[i]->type, myLegalActions[i]->index, myLegalActions[i]->dir1, myLegalActions[i]->dir2);
			}
		}
		return bestAction;
	}

};

/**
 * Auto-generated code below aims at helping you parse
 * the standard input according to the problem statement.
 **/
int main()
{
	game = new Game();
	game->getInputInit();

	// game loop
    while (1) {

		game->resetBeforeInputs();
		game->getInputs();

		if(DEBUG) game->printGrid();
		
		Action finalAction = game->selectBestAction();

		if(DEBUG) cerr << "Score " << finalAction.score << endl;
		finalAction.output();
    }
}
