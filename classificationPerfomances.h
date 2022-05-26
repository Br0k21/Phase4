#ifndef CLASSIFICATIONPERFOMANCES_H
#define CLASSIFICATIONPERFOMANCES_H

#define NB_CLASSES 6
#define MAX_TESTS 10000

#include <stdio.h>

// Display
void displayResultByClasses(int realClasses[], int estimateClasses[], int nbTests);
void displayAccuracy(int realClasses[], int estimatedClasses[], int nbTests);
void displayConfusionMatrix(int realClasses[], int estimateClasses[], int nbTests);
// Business
double percentage(int movesByClass[], int wellPlacedMovByClasses[], int iClass);
void countTotalByMov(int realClasses[], int nbTests, int movesByClass[]);
void countWellPlacedByMov(int realClasses[], int estimateClasses[], int nbTests, int wellPlacedMovesByClass[]);
void createMatrix(int realClasses[], int estimatedClasses[], int nbTests, int matrix[][NB_CLASSES]);





//Display
void displayResultByClasses(int realClasses[], int estimateClasses[], int nbTests) {
	// Display First Line
	printf("%6s | %12s | %12s | %12s\n", "classe", "bien classes", "total", "Pourcentage");

	// Initialiser movesByClass
	int movesByClass[NB_CLASSES];
	countTotalByMov(realClasses, nbTests, movesByClass);

	// Initialiser wellPlacedMovesByClass
	int wellPlacedMovesByClass[NB_CLASSES];
	countWellPlacedByMov(realClasses, estimateClasses, nbTests, wellPlacedMovesByClass);

	// Afficher chaque classes
	for (int i = 0; i < NB_CLASSES; i++) {
		if(movesByClass[i] != 0)
			printf("%6d | %12d | %12d | %.2f%%\n", i + 1, wellPlacedMovesByClass[i], movesByClass[i], percentage(movesByClass, wellPlacedMovesByClass, i));
	}
}
void displayAccuracy(int realClasses[], int estimatedClasses[], int nbTests) {
	double accuracy = 0;
	int wellPlacedMovesByClass[NB_CLASSES];
	
	countWellPlacedByMov(realClasses, estimatedClasses, nbTests, wellPlacedMovesByClass);

	for (int i = 0; i < NB_CLASSES; i++)
	{
		accuracy += wellPlacedMovesByClass[i];
	}

	accuracy = accuracy / nbTests * 100;

	printf("L'accuracy est de %.2f%%\n", accuracy);
}
void displayConfusionMatrix(int realClasses[], int estimateClasses[], int nbTests) {
	// Initalisation de la matrice
	int matrix[NB_CLASSES][NB_CLASSES];
	createMatrix(realClasses, estimateClasses, nbTests, matrix);

	// Affichage de l'en-tête
	printf("Matrice de confusion :\n\n");

	// Affichage des numéros de classes
	printf("  ");
	for (int i = 0; i < NB_CLASSES; i++) {
		printf("%3d ", i + 1);
	}
	printf("\n");
	printf("\n");

	// Affichage de la matrice
	for (int iLigne = 0; iLigne < NB_CLASSES; iLigne++) {
		printf("%d ", iLigne + 1);
		for (int iColonne = 0; iColonne < NB_CLASSES; iColonne++) {
			printf("%3d ", matrix[iLigne][iColonne]);
		}
		printf("\n");
	}
}

// Business
double percentage(int movesByClass[], int wellPlacedMovByClasses[], int iClass) {
	if (movesByClass[iClass] != 0) return ((double)wellPlacedMovByClasses[iClass] * 100) / movesByClass[iClass];
	return 0;
}
void countTotalByMov(int realClasses[], int nbTests, int movesByClass[]) {
	// Initialiser à zéro
	for (int i = 0; i < NB_CLASSES; i++) movesByClass[i] = 0;
	for (int i = 0; i < nbTests; i++) movesByClass[realClasses[i] - 1]++;
}
void countWellPlacedByMov(int realClasses[], int estimateClasses[], int nbTests, int wellPlacedMovesByClass[]) {
    // Initialiser à zéro
    for (int i = 0; i < NB_CLASSES; i++) wellPlacedMovesByClass[i] = 0;
    for (int i = 0; i < nbTests; i++) {
        if (estimateClasses[i] == realClasses[i]) wellPlacedMovesByClass[realClasses[i] - 1]++;
    }
}
void createMatrix(int realClasses[], int estimatedClasses[], int nbTests, int matrix[][NB_CLASSES]) {
	// Initalisation à 0 de la matrice
	for (int iLigne = 0; iLigne < NB_CLASSES; iLigne++) {
		for (int iColonne = 0; iColonne < NB_CLASSES; iColonne++) {
			matrix[iLigne][iColonne] = 0;
		}
	}

	// Ajout des valeurs
	for (int iTest = 0; iTest < nbTests; iTest++) {
		matrix[estimatedClasses[iTest] - 1][realClasses[iTest] - 1]++;
	}
}


#endif
