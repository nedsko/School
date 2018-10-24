// Authors: Jacob Hallman and Nedo Skobalj

import java.util.*;
import java.lang.Math;

public class Algorithms {
	/*
	 * Algorithms
	 */

	/**
	 * Evaluation function, same as in lab sheet.
	 *
	 * @param      gameState  The game state
	 *
	 * @return     Best score
	 */
	public static int eval(GameState gameState) {
		if (gameState.isXWin()) {
			return 1000;
		}
		if (gameState.isOWin()) {
			return -1000;
		}
		int score = 0;
		int diagonal1Score = 0;
		int diagonal2Score = 0;
		int emptyCellsDia1 = 0;
		int emptyCellsDia2 = 0;
		// Add scores for diagonals
		for (int j = 0; j < GameState.BOARD_SIZE; j++) {
			if (gameState.at(j, j) == Constants.CELL_X) {
				diagonal1Score++;
			}
			if (gameState.at(j, j) == Constants.CELL_EMPTY) {
				emptyCellsDia1++;
			}
			if (gameState.at(j, GameState.BOARD_SIZE - 1 - j) == Constants.CELL_X) {
				diagonal2Score++;
			}
			if (gameState.at(j, GameState.BOARD_SIZE - 1 - j) == Constants.CELL_EMPTY) {
				emptyCellsDia2++;
			}

		}
		// Zlatan's formula. Number of non occupied cells in a diagonal multiplied by
		// the number of occupied cells gives total score of diagonals
		score += emptyCellsDia1*diagonal1Score + emptyCellsDia2*diagonal2Score;

		// Loop through every cell to get the row and column score
		for(int i = 0; i < GameState.CELL_COUNT; i++) {
			// Get column and row representations for index
			int cellColumn = GameState.cellToCol(i);
			int cellRow = GameState.cellToRow(i);
			// Skip this cell if it contains an X.
			if (gameState.at(cellRow, cellColumn) == Constants.CELL_X) {
				continue;
			}
			// Sum the score of the row, column.
			for(int j = 0; j < GameState.BOARD_SIZE; j++) {
				if (gameState.at(cellRow, j) == Constants.CELL_X) {
					score++;
				}
				if (gameState.at(j, cellColumn) == Constants.CELL_X) {
					score++;
				}
			}
		}
		return score;
	}

	// Minimax implementation
	public static int miniMax(GameState gameState, int depth) {
	    Vector<GameState> nextStates = new Vector<GameState>();
        gameState.findPossibleMoves(nextStates);
		int bestPossible;
		int v;
	    if (nextStates.size() == 0 || depth == 0) {
	    	return eval(gameState);
	    } else { // GO DEEPER
	        depth--;
	        if (gameState.getNextPlayer() == Constants.CELL_O) {
	        	bestPossible = Integer.MIN_VALUE;
	        	for (GameState game : nextStates) {
	        		v = miniMax(game, depth);
	        		bestPossible = Math.max(bestPossible, v);
	        	}
	        } else {
	        	bestPossible = Integer.MAX_VALUE;
	        	for (GameState game : nextStates) {
	        		v = miniMax(game, depth);
	        		bestPossible = Math.min(bestPossible, v);
	        	}
	        }
	        return bestPossible;
        }
    }
    /**
     * Not really used...
     * Returns true if the cell is on the center diagonals of the board
     * @param  row    row index
     * @param  column column index
     * @return        boolean
     */
    private boolean isDiagonal(int row, int column) {
    	if (row == column) {
    		return true;
    	} else if ((row + column) == GameState.BOARD_SIZE - 1) {
    		return true;
    	} else {
    		return false;
    	}
    }
}
