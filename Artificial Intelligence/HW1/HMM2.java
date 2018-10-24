/**
 * Main class for HMM2
 * Authors: Nedo Skobalj and Jacob Hallman
 * FORWARD ALGORITHM - Calculate the probability of the made observation sequence
 */
import java.util.Scanner;
import java.util.ArrayList;

public class HMM2 {
	
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
		// Arraylist of matrices that hold alpha values. index i in the list corresponds
		// to time t in HMM. 
		ArrayList<Matrix> alphaMatrix = new ArrayList<Matrix>();

		// Initialze alpha 1. We start off by computing the probability of having observed the first observation o1 and having
		// been in any of the hidden states.  The latter probability is provided by the initial state distribution π.
		Matrix observation1 = emissMatrix.getColumn(emissSequence[0]);
		Matrix initTransposed = initMatrix.transpose();
		Matrix alpha1 = initTransposed.elementWiseMultiply(observation1);
		alphaMatrix.add(0, alpha1);
		// Marginalize over the probability of having been in any other state at time i - 1 and multiply with the matching 
		// observation probability
		for (int i = 1; i < nrOfEmissions; i++) {
			// Get previous column from alphaMatrix and transpose to be able to perform matrix multiplication
			Matrix prevTrans = alphaMatrix.get(i-1).transpose();
			// Matrix multiplication with transition matrix (equal to sum notation in lab PM)
			Matrix temp = prevTrans.multiplyMatrix(transMatrix).transpose();
			// Element wise multiply with observation column at current time
			Matrix observationColumn = emissMatrix.getColumn(emissSequence[i]);
			Matrix result = temp.elementWiseMultiply(observationColumn);
			alphaMatrix.add(i, result);
		}
		// Sum and output as single scalar
		System.out.println(alphaMatrix.get(nrOfEmissions-1).sum());
	}
}