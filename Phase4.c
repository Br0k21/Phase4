#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>

#include "classificationPerfomances.h"

#define NB_CLASSES 6
#define BUFFER_SIZE 50000
#define NB_SEC 1000
#define PATH_NAME_MODEL "E:\\Phase1\\model.csv"
#define PATH_NAME_TRAIN "E:\\Phase1\\trainSet.csv"
#define PATH_NAME_TEST "E:\\Phase1\\testSet.csv"

typedef struct model Model;
struct model {
	int motionType;
	double averages[NB_SEC];
	int nbValues;
	double stds[NB_SEC];
	double globalAverage;
};

void getModel(Model models[]);
int decomposition(char line[], double data[]);
int setEstimation(int estimatedClasses[], int realClasses[], Model models[]);
int getEstimationI1(double data[], Model models[]);
int getEstimationI2(double data[], Model models[]);
int getEstimationI3(double data[], Model models[]);
void addEstimation(int estimatedClasses[], int e1, int e2, int e3, int i);

void main(void) {
	int estimatedClasses[NB_SEC], realClasses[NB_SEC];
	int nbMov;
	Model models[NB_CLASSES];
	
	getModel(models);
	nbMov = setEstimation(estimatedClasses, realClasses, models);

	system("cls");
	displayResultByClasses(realClasses, estimatedClasses, nbMov);
	displayAccuracy(realClasses, estimatedClasses, nbMov);
	displayConfusionMatrix(realClasses, estimatedClasses, nbMov);
}

void getModel(Model models[]){
	FILE* fiModel;
	char caracLu;

	fopen_s(&fiModel, PATH_NAME_MODEL, "r");

	if(fiModel == NULL) printf("Erreur lors de l'ouverture du fichier model");
	else {
		// Skiper la ligne des titres
		do {
			fread_s(&caracLu, sizeof(char), sizeof(char), 1, fiModel);
		} while(caracLu != '\n');
		

		for(int iMov = 0; iMov < NB_CLASSES; iMov++) {
			for(int iData = 0; iData < 3; iData++){
				char line[BUFFER_SIZE];
				int charCount = 0;

				do {
					fread_s(&caracLu, sizeof(char), sizeof(char), 1, fiModel);
					//putchar(caracLu);
					charCount += sprintf_s(line + charCount, BUFFER_SIZE - charCount, "%c", caracLu);
				} while(caracLu != '\n');
				// 3 types différents
				// Averages
				// Stds
				// global averages
				switch(iData) {
					case 0:
						models[iMov].motionType = decomposition(line, models[iMov].averages);
						break;
					case 1:
						models[iMov].motionType = decomposition(line, models[iMov].stds);
						break;
					case 2:
						char* token = NULL;
						char* nextToken = NULL;
						char s[2] = ",";
						token = strtok_s(line, s, &nextToken); 
						token = strtok_s(NULL, s, &nextToken);
						models[iMov].globalAverage = atof(token);
				}
			}
		}

		fclose(fiModel);
	}
}

int setEstimation(int estimatedClasses[], int realClasses[], Model models[]){
	// récupérer ligne par ligne testSet et la traiter
	FILE* fiTest;
	char caracLu;
	int iLigne = 0;

	int estimatedClasseI1, estimatedClasseI2, estimatedClasseI3;

	fopen_s(&fiTest, PATH_NAME_TEST, "r");
	if(fiTest == NULL) printf("Erreur lors de l'ouverture du fichier trainSet");
	else {
		// Skiper la ligne des titres
		do {
			fread_s(&caracLu, sizeof(char), sizeof(char), 1, fiTest);
		} while(caracLu != '\n');
		fread_s(&caracLu, sizeof(char), sizeof(char), 1, fiTest);

		while(!feof(fiTest)){
			int iColonne = 0;
			int charCount = 0;
			double testData[NB_SEC]; // Contient les données d'une ligne
			char tmp[10];


			// Récupérer le motionType
			if(iLigne == 0)
				fread_s(&caracLu, sizeof(char), sizeof(char), 1, fiTest);
			realClasses[iLigne] = atoi(&caracLu);
			


			// Skiper genre, index
			int iTok = 0;
			while(iTok < 3){
				fread_s(&caracLu, sizeof(char), sizeof(char), 1, fiTest);
			    if(caracLu == ',') iTok++;
			}


			// Récupérer une ligne de données
			while(caracLu != '\n'){
				fread_s(&caracLu, sizeof(char), sizeof(char), 1, fiTest);
				//putchar(caracLu);
				if(caracLu == ',') {
					char* str;
					testData[iColonne] = strtod(tmp, &str);
					strcpy_s(tmp, sizeof(char), "");
					charCount = 0;
					iColonne++;
				} else {
					charCount += sprintf_s(tmp + charCount, sizeof(tmp) - charCount, "%c", caracLu);
				}
			}


			// Traiter la ligne de données
			// Distance par rapport a la moyenne
			estimatedClasseI1 = getEstimationI1(testData, models);
			// Distance par rapport à l'ecart type
			estimatedClasseI2 = getEstimationI2(testData, models);
			// Par rapport à la moyenne générale
			estimatedClasseI3 = getEstimationI3(testData, models);
			
			addEstimation(estimatedClasses, estimatedClasseI1, estimatedClasseI2, estimatedClasseI3, iLigne);

			iLigne++;
			fread_s(&caracLu, sizeof(char), sizeof(char), 1, fiTest);
		}

		fclose(fiTest);
	}

	return iLigne;
}

void addEstimation(int estimatedClasses[], int e1, int e2, int e3, int i){
	estimatedClasses[i] = (e2 == e3 ? e2 : e1);
}


int getEstimationI1(double data[], Model models[]) {
	int estimatedClasse;
	double distanceMin = 99999;

	for(int iClass = 0; iClass < NB_CLASSES; iClass++){
		double distance = 0;
		for(int iValue = 0; iValue < NB_SEC; iValue++){
			distance += pow((data[iValue] - models[iClass].averages[iValue]),2);
		}
		distance = sqrt(distance);

		if(distance < distanceMin) {
			distanceMin = distance;
			estimatedClasse = models[iClass+1].motionType;
		}
	}

	return estimatedClasse;
}

int getEstimationI2(double data[], Model models[]){
	int estimatedClasse;
	double distanceMin = 99999;

	for(int iClass = 0; iClass < NB_CLASSES; iClass++){
		double distance = 0;
		for(int iValue = 0; iValue < NB_SEC; iValue++){
			distance += pow((data[iValue] - models[iClass].stds[iValue]),2);
		}
		distance = sqrt(distance);

		if(distance < distanceMin) {
			distanceMin = distance;
			estimatedClasse = models[iClass+1].motionType;
		}
	}

	return estimatedClasse;
}

int getEstimationI3(double data[], Model models[]) {
	int estimatedClasse;
	double average = 0;
	double distanceMin = 9999;
	double distance;

	for(int iData = 0; iData < NB_SEC; iData++) average += data[iData];

	average /= NB_SEC;

	for(int iClass = 0; iClass < NB_CLASSES; iClass++) {

		distance = pow((average - models[iClass].globalAverage),2);
		distance = sqrt(distance);

		if(distance < distanceMin) {
			distanceMin = distance;
			estimatedClasse = models[iClass+1].motionType;
		}
	}

	return estimatedClasse;
}

int decomposition(char line[], double data[]) {
	int i = 0;
	char* token = NULL;
	char* nextToken = NULL;
	char s[2] = ",";
	int mvt;
	token = strtok_s(line, s, &nextToken); 
	mvt = atoi(line);
	token = strtok_s(NULL, s, &nextToken);
	while (token != NULL)
	{
		data[i] = atof(token);
		i++;
		token = strtok_s(NULL, s, &nextToken);
	}
	return mvt;
}
