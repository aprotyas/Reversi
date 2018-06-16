#include <iostream>
#include <vector>	//	Needed for grid
#include <chrono>	//	Needed for sleep
#include <thread>	//	Needed for sleep

//	Necessary macros to make program more readable.
#define SIZE 8
#define BLACK -1
#define NONE 0
#define WHITE 1

/*
 *  Function prototypes listed below.
 *  Function names are indicative of their purpose.
 *  More explanation before the function definitions.
 */

void clear(void);
void printIntro(void);
void printRules(void);
std::vector< std::vector<int> > makeGrid(void);
void printGrid(std::vector< std::vector<int> > grid);
void playerInput(int & row, int & column);
int checkLegalMove(int row, int column);
bool	 withinGrid(int row, int column);
void performMove(int row, int column);
void compoundMove(int row, int column);
void doFlip(int row__, int rowChange, int column__, int colChange, int input, int & flips);
bool gridComplete(void);
void printResult(void);

//	Global variables to be used by multiple functions
int counter = 0;
int illegalMoveFlag = 0;
int forfeitCounter = 0;

/*
 *  Global variable of type vector of int vectors (multidimensional vector).
 *  The grid is the key behind this game. If any point on the grid has a value
 *  of 0, it means that point is unoccupied. A value of 1 indicates a WHITE tile,
 *  while a value of -1 indicates a BLACK tile.
 */
std::vector< std::vector<int> > grid = makeGrid();

int main(void){

	printIntro();	//	Prints out the welcome message.
	printRules();	//	Prints out the rules and prompts players to start.
	clear();			//	Clears the screen.
	printGrid(grid);	//	Prints the grid.

	//	Initiating type int variables representing the rows and columns.
	//	Will be used for input.
	int row = 0;
	int column = 0;

	//	This loop keeps continuing until the grid is filled (game over).
	while(!gridComplete()){
		//	The values for row and column are reset every time the loop is entered.
		row = 0;
		column = 0;
		playerInput(row, column);	//	Sets the values of row and column according to user input.

		//	If the player forfeits, forfeit counter increases by 1. In this case the move
		//	is passed on to the next player. If both players forfeit consecutively,
		//	the game is over and the scores are shown.
		if(forfeitCounter > 0){
			if(forfeitCounter == 2){
				break;
			}
			else{
				counter++;
				continue;
			}
		}
		if(checkLegalMove(row, column) == 1){
			//	If the legal move checker returns 1, it means move is illegal.
			//	The rest of the loop is skipped and the loop is re-entered.
			continue;
		}
		//	The values of row and column are reduced by 1 to reflect the actual coordinates
		//	on the grid (0 - 7), rather than what the user inputs (1 - 8).
		row -= 1;
		column -= 1;
		//	The following function call performs the actual move on the grid.
		compoundMove(row, column);
		if(illegalMoveFlag == 1){
			continue;
		}
		//	After the move is performed, the screen is cleared and the grid is updated.
		clear();
		printGrid(grid);
		counter++;	//	The value of the counter is increased by 1, indicating a move is completed.
	}

	std::cout << "Game over!\n";
	printResult();	//	Once the loop is exited (game over), the results are displayed.
	return 0;
}

void clear(void){
	//	These are ASCII Escape Sequences which clear the screen,
	//	and move the cursor to the top left of the screen.

	//	REFERENCE: I needed a way to clear the console screen
	//	so I searched on Google and found this.
	//	(2010, September 28). How to clear the console screen with ANSI (Any Language) - Other Programming Tutorials.
	//	Retrieved from http://forum.codecall.net/topic/59142-how-to-clear-the-console-screen-with-ansi-any-language/

    std::cout << "\x1B[2J\x1B[H";
    return;
}

//	This function prints the intro message simply using std::cout.
void printIntro(void){
    std::cout << " ------------------------------------------------------------------ \n";
	std::cout << " | 			  WELCOME TO OTHELLO			  | \n";
	std::cout << " ------------------------------------------------------------------ \n";
	return;
}

//	This function prints the rules simply using std::cout.
//	It also receives input from user and does input validation.
//	It proceeds with the game if the user inputs 'Y' or 'y'.
void printRules(void){
	std::cout << "   			  *****  RULES *****			    \n";
	std::cout << " i)    Enter 1 to play against PC. 2 to play against a player. \n";
	std::cout << " ii)   The game is played on an 8x8 board. \n";
	std::cout << " iii)  The game is round based. \n";
	std::cout << " iv)   One player places a disc on the board each round. \n";
	std::cout << " v)    Input should be done as ROW[1-8], COLUMN[1-8].\n";
	std::cout << " vi)   Player1 = WHITE [ ● ]   ||   Player2 = BLACK [ ○ ] \n";
	std::cout << " vii)  Legal move: Disc must be adjacent to disc of \n" <<
			"       other color. There must be one of player's discs on\n       the opposite side of opponent's discs.\n";
    std::cout << " viii) Any discs of opposing color in any straight \n" <<
            "       between player's discs will FLIP COLOR. \n";
	std::cout << " ix)    Game ends when either: \n	  a) All squares are filled\n" <<
			"	  b) Both players pass their moves consecutively ( 100, 100 to pass ) \n";
	std::cout << " x)    WINNER is the player who has more discs \n       when the game ends.\n" <<
			"       If players have equal number of discs, its a DRAW.\n\n\n";

    char input;
    std::cout << " Please press the [Y] button if you are ready to start: ";
    while(!(std::cin >> input) || input != 'Y'){
        if(input == 'y'){
            break;
        }
        std::cout << " INPUT IS: " << input << std::endl;
        std::cout << " Please press the [Y] button if you are ready to start: ";
        std::cin.clear();
		while(std::cin.get() != '\n');
    }

    if(input == 'Y' || input == 'y'){
        std::cout << " Starting game...\n";
        //	REFERENCE:
        //	I needed a way to pause the program for a bit so as to make
        //	it seem interesting. First I wanted to use a for loop which counts
        //	up to a very large number, but decided to stick with this
        //	since it is more precise.

        //	Std::this_thread::sleep_for. (n.d.).
        //	Retrieved from http://www.cplusplus.com/reference/thread/this_thread/sleep_for/

        std::this_thread::sleep_for(std::chrono::seconds(1));
        return;
    }
}

std::vector< std::vector<int> > makeGrid(void){
	//	Returns a vector object.
	std::vector< std::vector<int> > grid(SIZE, std::vector<int>(SIZE));	//	(8x8 vector)
	//	NOTE: White = 1, Black = -1
	//	This is the default state of the grid at the start of the game.
	grid[3][3] = WHITE;
	grid[4][4] = WHITE;
	grid[3][4] = BLACK;
	grid[4][3] = BLACK;
	return grid;
}

//	This function prints the grid out.
//	It also prints out row and column values for users.
void printGrid(std::vector< std::vector<int> > grid){
    for(int i = 0; i < SIZE; i ++){
        if(i == 0){
            std::cout << "   ";
            for(int k = 0; k < SIZE; k++){
                std::cout << " " << k + 1 << " ";
            }
        }
        std::cout << "\n" << " " << i + 1 << " ";
        for(int j = 0; j < SIZE; j++){
            if(grid[i][j] == WHITE){
                std::cout << " ● ";
            }
            else if(grid[i][j] == BLACK){
                std::cout << " ○ ";
            }
            else{
                std::cout << " · ";
            }
        }
    }
    std::cout << "\n";
}

//	This function receives player input. If the player does not enter in the form
//	specified, then they are re-prompted. If the player entered an illegal move, they
//	are re-prompted as well.

void playerInput(int & row, int & column){
	char comma;
	if(illegalMoveFlag == 0){
		std::cout << "Enter your move, Player {" << counter%2 + 1 << "}: ";
	}
	else if(illegalMoveFlag == 1){
		std::cout << "Please enter a legal move, Player {" << counter%2 + 1 << "}: ";
	}
	while((std::cin >> row >> comma >> column) == 0 || comma != ','){
		std::cout << "Wrong input! Please enter in the format ROW [1-8], COLUMN [1-8]: ";
		std::cin.clear();
		while(std::cin.get() != '\n');
	}
	if(row == 100 && column == 100){
		forfeitCounter++;
	}
	else{
		forfeitCounter = 0;
	}
	return;
}

//	This function checks if the inputted move is legal.
//	Firstly, it checks whether the move is within the grid.
//	It think checks whether or not the move has already been made.
//	It then checks if its possible to make that move.
//	Meaning, is the new move contiguous to the existing tiles.

int checkLegalMove(int row, int column){
	int r = row - 1;
	int c = column - 1;
	//	This section carries out the same function as the withinGrid()
	//	function itself, but I like to be extra cautious...
	if(r < 0 || r >= SIZE || c < 0 || c >= SIZE){
		std::cout << "The position indicated is out of range. \n";
		illegalMoveFlag = 1;
		return illegalMoveFlag;
	}
	if(grid[r][c] == BLACK || grid[r][c] == WHITE){
		std::cout << "The position has already been used. \n";
		illegalMoveFlag = 1;
		return illegalMoveFlag;
	}
	int playerType = (counter%2 > 0)?BLACK:WHITE;
	int opposingType = (playerType == BLACK)?WHITE:BLACK;
	int rowChange, colChange;
	int rowChanging, colChanging;
	int tilesChecked = 0;
	int tilesBlank = 0;
	//	In the following for loops, the computer checks in every
	//	possible direction whether or not a legal move can be made or not.
	for(rowChange = -1; rowChange <= 1; rowChange++){
		for(colChange = -1; colChange <= 1; colChange++){
			if(rowChange == 0 && colChange == 0){
				continue;
			}
			rowChanging = r + rowChange;
			colChanging = c + colChange;
			if(withinGrid(rowChanging, colChanging)){
				tilesChecked++;
			}
			else{
				continue;
			}
			if(grid[rowChanging][colChanging] == NONE){
				tilesBlank++;
				continue;
			}
			//	I do not know if this portion works, I think the compound move function
			//	itself carries out this task. It did not seem to work on its own.
			//	The intention of this ELSE IF clause was to make sure a player can't
			//	enter a move wherever they want. There must be a tile of similar type
			//	at the end of a sequence of opposite tiles.
			else if(grid[rowChanging][colChanging] == opposingType){
				int i = rowChanging;
				int j = colChanging;
				for(i += rowChange, j += colChange; withinGrid(i, j); i += rowChange, j += colChange){
					if(grid[i][j] == NONE){
						break;
					}
					if(grid[i][j] == playerType){
						illegalMoveFlag = 0;
						return illegalMoveFlag;
					}
				}
			}
		}
	}
	//	If all the tiles around a proposed move is empty,
	//	then the move is illegal.
	if(tilesChecked == tilesBlank){
		illegalMoveFlag = 1;
		return illegalMoveFlag;
	}
	else{
		illegalMoveFlag = 0;
		return illegalMoveFlag;
	}
}

//	This function simply returns a boolean value indicating
//	whether or not the inputted move is within the grid.
//	This is a helper function and is also required for other uses.
bool	 withinGrid(int row, int column){
	if(row < 0 || row >= SIZE || column < 0 || column >= SIZE){
		return false;
	}
	return (row >= 1 && column >= 1 && row <= SIZE && column <= SIZE);
}

//	This function simply changes the value of a tile to the corresponding
//	player's tile.

void performMove(int row, int column){
	int input = (counter%2>0)?BLACK:WHITE;
	grid[row][column] = input;
}

//	This is essentially the most important function in this program. Firstly,
//	based on the counter number (player number), it determines which value it must
//	input. Then, it moves in every direction to perform any possible flips.
//	I also initialized a FLIPCOUNTER which increments by 1 whenever a flip occurs.
//	If no flip occurs, it means the move was illegal to begin with and the while
//	loop in the main function is repeated.

void compoundMove(int row, int column){
	int input = (counter%2>0)?BLACK:WHITE;

//	performMove(row, column);
	int flips = 0;
	for(int rowChange = -1; rowChange <= 1; rowChange++){
		for(int colChange = -1; colChange <= 1; colChange++){
			//	No need to do flips if both rowChange and colChange = 0, meaning
			//	the program is looking at the tile on which the player made his
			//	move.
			if(rowChange != 0 || colChange != 0){
				doFlip(row, rowChange, column, colChange, input, flips);
			}
		}
	}
	if(flips == 0){
		illegalMoveFlag = 1;
		return;
	}
	else{
		//	If the move is not illegal, the "simple move" of the player
		//	is performed after all the necessary flips.
		performMove(row, column);
	}
}

//	This function actually performs all the flips we see in the game.
//	It has a lot of arguments. It takes the flips variable by reference since we want
//	to change the flips variable from the compoundMove() function. I do not
//	use the global ROW and COLUMN variables because there is some syntactical
//	confusion in one of the function calls within this function. The other
//	arguments are straight forward to understand.

void doFlip(int row__, int rowChange, int column__, int colChange, int input, int & flips){
	int r, c;
	//	This goes through the tiles in the direction as specified from the compoundMove() function,
	//	and if it finds a similar type of tile at one end, it flips all the tiles IN BETWEEN.
	//	This is why the inner for loop REVERSES (r -= rowChange, c -= colChange).
	for(r = row__ + rowChange, c = column__ + colChange; withinGrid(r, c); r += rowChange, c += colChange){ 
		if(grid[r][c] == NONE){
			return;
		}
		else if(grid[r][c] == input){
			for(r -= rowChange, c -= colChange; r != row__ || c != column__; r -= rowChange, c -= colChange){
				flips++;
				performMove(r,c);	//	Performs the basic move. Flip.
			}
			return;
		}
	}
}

//	This function returns a boolean value indicating whether or not all the
//	tiles on the grid have been filled. It checks that by looping through each
//	tile. This required two for loops (one for rows, one for columns).

bool gridComplete(void){
	for(int row = 0; row < SIZE; row++){
		for(int column = 0; column < SIZE; column++){
			if(grid[row][column] == NONE){
				return false;
			}
		}
	}
	return true;
}

//	This function calculates the score by looping through each tile.
//	The final message is printed based on the scores.

void printResult(void){
	int score_1 = 0;
	int score_2 = 0;

	for(int row = 0; row < SIZE; row++){
		for(int column = 0; column < SIZE; column++){
			if(grid[row][column] == WHITE){
				score_1 += 1;
			}
			else if(grid[row][column] == BLACK){
				score_2 += 1;
			}
		}
	}

	std::cout << "Player 1 has " << score_1 << " tiles.\n";
	std::cout << "Player 2 has " << score_2 << " tiles.\n";

	if(score_1 > score_2){
		std::cout << "Player 1 wins!\n";
	}
	else if(score_2 > score_1){
		std::cout << "Player 2 wins!\n";
	}
	else{
		std::cout << "The game is a draw!\n";
	}
}
