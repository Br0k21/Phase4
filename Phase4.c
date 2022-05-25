#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>

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

void main(void) {
	int estimatedClasses[NB_SEC], realClasses[NB_SEC];
	int nbMov;
	Model models[NB_CLASSES];
	
	getModel(models);
	nbMov = setEstimation(estimatedClasses, realClasses, models);
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
	}
}

int setEstimation(int estimatedClasses[], int realClasses[], Model models[]){
	// récupérer ligne par ligne trainSet et la traiter
	FILE* fiTrain;
	char caracLu;
	int i = 0;

	fopen_s(&fiTrain, PATH_NAME_TRAIN, "r");
	if(fiTrain == NULL) printf("Erreur lors de l'ouverture du fichier trainSet");
	else {
		// Skiper la ligne des titres
		do {
			fread_s(&caracLu, sizeof(char), sizeof(char), 1, fiTrain);
		} while(caracLu != '\n');
		while(!feof(fiTrain)){
			int iColonne = 0;
			double testData[NB_SEC]; // Contient les données d'une ligne
			char tmp[10];
			char* str;
			// Récupérer le motionType
			if(caracLu == '\n'){
				fread_s(&caracLu, sizeof(char), sizeof(char), 1, fiTrain);
				printf("Motion type : %d\n", caracLu);
				realClasses[i] = atoi(&caracLu);
				i++;
			}
			// Skiper genre, index
			int iTok = 0;
			while(iTok < 3){
				fread_s(&caracLu, sizeof(char), sizeof(char), 1, fiTrain);
			    printf("Caractere a skipper : %c\n", caracLu);
			    if(caracLu == ',') iTok++;
			}

			fread_s(&caracLu, sizeof(char), sizeof(char), 1, fiTrain);
			while(caracLu != '\n'){
				if(caracLu == ',') {
					testData[iColonne] = strtod(tmp, &str);
					iColonne++;
					printf("Data numero %d : %f\n", iColonne+1, testData[iColonne]);
				} else {
					strcat_s(tmp, sizeof(char), &caracLu);
				}
			}

		}
	}

	return i;
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
