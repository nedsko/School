/**
 * Main class for HMM3
 * Authors: Nedo Skobalj and Jacob Hallman
 * VITERBI ALGORITHM  Instead of assuming that any state has led to the current estimate, we
 * only take the most likely predecessor into account.
 * Compute the most likely sequence of hidden states given the observations.
 */
import java.util.Scanner;
import java.util.ArrayList;
import java.util.HashMap;

public class HMM3 {
	public static void main(String[] args) {
		// Read all input and crete matrices
		Scanner sc = new Scanner(System.in);
		// First comes the transition matrix
		int transRowSize = sc.nextInt();
		int transColumnSize = sc.nextInt();
		Matrix transMatrix = new Matrix(transRowSize, transColumnSize);
		for (int i = 0; i < transRowSize ; i++) {
			for (int j = 0; j < transColumnSize ; j++) {
				transMatrix.addValue(i, j, sc.nextDouble());
			}
		}
		// Then the emission matrix
		int emissRowSize = sc.nextInt();
		int emissColumnSize = sc.nextInt();
		Matrix emissMatrix = new Matrix(emissRowSize, emissColumnSize);
		for (int i = 0; i < emissRowSize ; i++) {
			for (int j = 0; j < emissColumnSize ; j++) {
				emissMatrix.addValue(i, j, sc.nextDouble());
			}
		}
		// And the initial state 
		int initRowSize = sc.nextInt();
		int initColumnSize = sc.nextInt();
		Matrix initMatrix = new Matrix(initRowSize, initColumnSize);
		for (int i = 0; i < initRowSize ; i++) {
			for (int j = 0; j < initColumnSize ; j++) {
				initMatrix.addValue(i, j, sc.nextDouble());
			}
		}
		// Read emission sequence
		int nrOfEmissions = sc.nextInt();
		int[] emissSequence = new int[nrOfEmissions];
		for (int i = 0; i < nrOfEmissions; i++) {
			emissSequence[i] = sc.nextInt();
		}
		// HashMap of HashMaps that hold delta values and their preceeding states.
		HashMap<Integer, HashMap<Integer, Integer>> deltaMap = new HashMap<Integer, HashMap<Integer, Integer>>();
		for (int i = 0; i < nrOfEmissions; i++) {
			deltaMap.put(i, new HashMap<Integer, Integer>());
		}
		ArrayList<Matrix> deltaList = new ArrayList<Matrix>();


		// Initialze delta 1. (same as alpha 1)
		Matrix observation1 = emissMatrix.getColumn(emissSequence[0]);
		Matrix initTransposed = initMatrix.transpose();
		Matrix delta1 = initTransposed.elementWiseMultiply(observation1);
		deltaList.add(0, delta1);


		// stateSequence holds the most likely sequence of states that has been
		// computed. 
		int[] stateSequence = new int[nrOfEmissions];
		// Update delta values 
		for (int i = 1; i < nrOfEmissions; i++) {
			Matrix deltaValue = new Matrix(transRowSize, 1);
			int observation = emissSequence[i]; // Next emission in the emission sequence
			Matrix prevProb = deltaList.get(i-1); // Previous delta
			Matrix observationColumn = emissMatrix.getColumn(observation); 
			// Compute max for each state (column)
			for (int j = 0; j < transColumnSize; j++) {
				Matrix transColumn = transMatrix.getColumn(j);
				double obsValue = observationColumn.getValue(j, 0);
				Matrix partResult = prevProb.elementWiseMultiply(transColumn);
				// Call max and get the max value + its index
				double[] rowResult = partResult.scalarMultiply(obsValue).max();
				// Add max value to current column
				deltaValue.addValue(j, 0, rowResult[0]);
				// Save index for this j
				deltaMap.get(i).put(j, (int)rowResult[1]);
			}
			deltaList.add(i, deltaValue);
		}

		Matrix lastDelta = deltaList.get(deltaList.size()-1);
		// Find max for last column and backtrack from there
		int lastState = lastDelta.maxValueRowIndex();
		stateSequence[nrOfEmissions-1] = lastState;
		for (int i = nrOfEmissions-1; i > 0; i--) {
			stateSequence[i-1] = deltaMap.get(i).get(lastState);
			lastState = deltaMap.get(i).get(lastState);
		}
		for (int k = 0; k < deltaList.size(); k++) {
			System.out.print(stateSequence[k] + " ");
		}
	}
	/*
	private transferIndexes(int timeStep, int nrOfStates) {
		Matrix delta = deltaList.get(timeStep);
		for (int i = 0; i < nrOfStates; i++) {
			deltaMap.get(timeStep).put(i, delta.get)
		}
	}*/
}