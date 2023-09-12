// Kirby
#include <iostream>
#include <random>
#include <vector>

/*Game Constants*/
const int gameBoardSize{ 10 };
enum class gameSlot { hiddenEmpty, revealedEmpty, hiddenMine, revealedMine, flaggedMine };

/*GUI Functions*/
void splashScreen();
void displayGameState(const std::vector<gameSlot>& gameBoard, bool revealMines = false);
void displayGameDone(const std::vector<gameSlot>& gameBoard);

/* Engine Functions*/

std::vector<gameSlot> boardSetup();
void changeGameState(std::vector<gameSlot>& gameBoard);
bool isGameDone(const std::vector<gameSlot>& gameBoard);

int countMines(int row, int column, const std::vector<gameSlot>& gameBoard);

int boardIndex(int row, int column);

int main()
{

	char playAgain{ 'y' };
	while (playAgain == 'y' || playAgain == 'Y')
	{
		std::vector<gameSlot> gameBoard{ boardSetup() };
		while (!isGameDone(gameBoard))
		{
			displayGameState(gameBoard);
			changeGameState(gameBoard);
		}
		displayGameDone(gameBoard);
		std::cout << "\nDo you wish to play another game? (y/n): ";
		std::cin >> playAgain;

	}
}
void splashScreen()
{
	std::cout << "Mine Sweeper!" << std::endl;
	std::cout << std::endl;
	std::cout << "Your name here (2019)" << std::endl;
	std::cout << "CPSC 2377, Game Programming" << std::endl;
	std::cout << "UALR, Computer Science Dept." << std::endl;
	std::cout << std::endl;
	std::cout << "INSTRUCTIONS:" << std::endl;
	std::cout << std::endl;
	std::cout << "Clear the minefield without hitting a mine!" << std::endl;
	system("PAUSE");
}

void displayGameState(const std::vector<gameSlot>& gameBoard, bool revealMines)
{
	char rowName{ 'A' };
	int colName{ 1 };
	system("CLS");

	std::cout << "   ";
	for (int i{ 0 }; i < gameBoardSize; i++)
	{
		std::cout << colName << " ";
		colName++;
	}
	std::cout << std::endl;
	for (int row{ 0 }; row < gameBoardSize; row++)
	{

		std::cout << rowName << "| ";
		rowName++;
		for (int column{ 0 }; column < gameBoardSize; column++)
		{
			switch (gameBoard[boardIndex(row, column)])
			{
			case gameSlot::hiddenEmpty:
				std::cout << "- ";
				break;
			case gameSlot::hiddenMine:
				if (revealMines) std::cout << "* ";
				else std::cout << "- ";
				break;
			case gameSlot::revealedMine:
				std::cout << "* ";
				break;
			case gameSlot::revealedEmpty:
			{
				int numNeighbors{ countMines(row, column, gameBoard) };

				if (numNeighbors == 0)
					std::cout << "  ";
				else
					std::cout << numNeighbors << " ";
				break;
			case gameSlot::flaggedMine://suppose to flag unmarked mines
				int numNeighbors{ countMines(row, column, gameBoard) };

				if (numNeighbors == 1)
					std::cout << "f";
			}
		}
		std::cout << "\n";
	}
}

void displayGameDone(const std::vector<gameSlot>& gameBoard)
{
	displayGameState(gameBoard, true);
	for (auto slot : gameBoard)
	{
		if (slot == gameSlot::revealedMine)
		{
			std::cout << "The mine exploded.... You are dead!!!!\n";
			std::cout << "Better luck next time!";
			return;
		}
	}
	std::cout << "You have cleared the mine field!\n";
	std::cout << "You are loved by all! Use your power for good!";
}

std::vector<gameSlot> boardSetup()
{
	std::vector<gameSlot> gameBoard(gameBoardSize * gameBoardSize);

	static std::random_device seed;
	static std::default_random_engine e(seed());
	static std::bernoulli_distribution mined(.25);

	for (auto& slot : gameBoard)
	{
		if (mined(e))
		{
			slot = gameSlot::hiddenMine;
		}
		else
		{
			slot = gameSlot::hiddenEmpty;
		}
	}
	return gameBoard;
}

int boardIndex(int row, int column)
{
	return row * gameBoardSize + column;
}

void changeGameState(std::vector<gameSlot>& gameBoard)
{

	char row{ 'Z' };
	int column{ -1 };
	int numRow{ 0 };
	do
	{
		while (row < 'A' || row > 'A' + gameBoardSize)
		{
			std::cout << "Choose a row: ";
			std::cin >> row;
		}
		numRow = row - 'A';
		while (column < 0 || column > gameBoardSize)
		{
			std::cout << "Choose a column: ";
			std::cin >> column;
			column--;
		}
	} while (gameBoard[boardIndex(numRow, column)] == gameSlot::revealedEmpty);

	switch (gameBoard[boardIndex(numRow, column)])
	{
	case gameSlot::hiddenEmpty:
		gameBoard[boardIndex(numRow, column)] = gameSlot::revealedEmpty;
		break;
	case gameSlot::hiddenMine:
		gameBoard[boardIndex(numRow, column)] = gameSlot::revealedMine;
		break;
	case gameSlot::hiddenMine:
		gameBoard[boardIndex(numRow, column)] = gameSlot::flaggedMine;
		break;//I wanted this to mark and let user know mine has been flagged
	}
}

bool isGameDone(const std::vector<gameSlot>& gameBoard)
{
	bool stillEmpty{ true };
	for (auto slot : gameBoard)
	{
		switch (slot)
		{
		case gameSlot::hiddenEmpty:
			stillEmpty = false;
			break;
		case gameSlot::revealedMine:
			return true;
			break;
		}
	}
	return stillEmpty;
}

int countMines(int row, int column, const std::vector<gameSlot>& gameBoard)
{
	int mineCount{ 0 };
	for (int neighborRow{ -1 }; neighborRow <= 1; neighborRow++)
	{
		for (int neighborColumn{ -1 }; neighborColumn <= 1; neighborColumn++)
		{
			if ((neighborColumn != 0 || neighborRow != 0)
				&& row + neighborRow >= 0 && row + neighborRow < gameBoardSize
				&& column + neighborColumn >= 0 && column + neighborColumn < gameBoardSize
				&& (gameBoard[boardIndex(row + neighborRow, column + neighborColumn)] == gameSlot::hiddenMine
					|| gameBoard[boardIndex(row + neighborRow, column + neighborColumn)] == gameSlot::revealedMine))
			{
				mineCount++;
			}
		}
	}
	return 0;
	return mineCount;
}
