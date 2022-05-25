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

void main(void) {
	Model models[NB_CLASSES];
	getModel(models);
}

void getModel(Model models[]){
	FILE* fiModel;
	char caracLu;

	fopen_s(&fiModel, PATH_NAME_MODEL, "r");

	if(fiModel == NULL) printf("Erreur lors de l'ouverture du fichier");
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
					strncat_s(line, sizeof(char), &caracLu, 1);
				} while(caracLu != '\n');
				puts(line);
			}
		}
	}
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
