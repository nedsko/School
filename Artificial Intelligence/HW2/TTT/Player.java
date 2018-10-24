import java.util.*;

public class Player {
    /**
     * Performs a move
     *
     * @param gameState
     *            the current state of the board
     * @param deadline
     *            time before which we must have returned
     * @return the next state the board is in after our move
     */
    public GameState play(final GameState gameState, final Deadline deadline) {
        Vector<GameState> nextStates = new Vector<GameState>();
        gameState.findPossibleMoves(nextStates);

        // Set depth
        final int depth = 3; // A depth above 3 will not end well...

        int bestScore = Integer.MIN_VALUE;
        GameState bestState = gameState;
        int temp;
        for (GameState game : nextStates) {
            temp = Algorithms.miniMax(game, depth);
            if (temp > bestScore) {
                bestScore = temp;
                bestState = game;
            }
        }
        return bestState;
    }
}
