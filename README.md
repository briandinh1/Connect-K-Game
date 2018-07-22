# Connect-K-Game
Variation on Connect 4 / tic tac toe style games

User is able to choose any K value, which is the amount of pieces lined up needed to win the game
Can also change the board height and width to be anything as long as they contain k
Game can be played with or without gravity, where pieces fall to the bottom or stay where placed

The game can be played with two players, one player vs the AI, or pit two AIs against each other
The best AI is implemented with iterative deepening search amd alpha beta pruning on a custom evaluation heuristic. It does better on boards with gravity than without

Varying AI difficulties can be found in the EXE files folder. Load them into the .Jar
