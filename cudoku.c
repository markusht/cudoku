#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>

const int DIMENSION = 9;

void parseSgFromFile(char *fname, int sg[DIMENSION][DIMENSION])
{
	FILE *fp;
	char *line = NULL;
	size_t len = 0;
	ssize_t read;

	fp = fopen(fname, "r");
	if (fp == NULL) {
		printf("Failed to open file %s\nProgram Terminated\n", fname);
		exit(EXIT_FAILURE);
	}

	int r = 0;
	while ((read = getline(&line, &len, fp)) != -1){
		for (int c = 0; c < DIMENSION; ++c) {
			if (line[c])
				sg[r][c] = line[c] - '0';
			else
				exit(EXIT_FAILURE);
		}
		++r;
	}

	fclose(fp);
	free(line);
}

void fprintSg(FILE *stream, int sg[DIMENSION][DIMENSION])
{
	for (int i = 0; i < DIMENSION; ++i) {
		for (int j = 0; j < DIMENSION; ++j) {
			fprintf(stream, "%d", sg[i][j]);
		}
		fprintf(stream, "\n");
	}
}

void displaySg(FILE *stream, int sg[DIMENSION][DIMENSION])
{
	for (int i = 0; i < DIMENSION; ++i) {
		if (i % 3 == 0 || i == DIMENSION)
			fprintf(stream, "|-------|-------|-------|\n");
		for (int j = 0; j < DIMENSION; ++j) {
			if (j % 3 == 0)
				fprintf(stream, "| ");
			fprintf(stream, "%d ", sg[i][j]);
		}
		fprintf(stream, "|");
		fprintf(stream, "\n");
	}
	fprintf(stream, "|-------|-------|-------|\n");
}

int findEmpty(int sg[DIMENSION][DIMENSION], int *r, int *c)
{
	for (*r = 0; *r < DIMENSION; ++*r)
		for (*c = 0; *c < DIMENSION; ++*c)
			if (sg[*r][*c] == 0){
				return 1;
			}
	return 0;
}

int usedInRow(int sg[DIMENSION][DIMENSION], int r, int num)
{
	for (int j = 0; j < DIMENSION; ++j)
		if (sg[r][j] == num)
			return 1;
	return 0;
}

int usedInCol(int sg[DIMENSION][DIMENSION], int c, int num)
{
	for (int i = 0; i < DIMENSION; ++i)
		if (sg[i][c] == num)
			return 1;
	return 0;
}

// Checks if num is used in box with starting row bsr and starting col bsc
int usedInBox(int sg[DIMENSION][DIMENSION], int bsr, int bsc, int num)
{
	for (int r = 0; r < 3; ++r)
		for (int c = 0; c < 3; ++c)
			if (sg[r+bsr][c+bsc] == num)
				return 1;
	return 0;
}

int isSafe(int sg[DIMENSION][DIMENSION], int r, int c, int num)
{
	return !usedInRow(sg, r, num)
		&& !usedInCol(sg, c, num)
		&& !usedInBox(sg, r - (r%3), c - (c%3), num)
		&& sg[r][c] == 0;
}

int solveSudoku(int sg[DIMENSION][DIMENSION])
{
	int r, c;

	// Check if completed
	if (!findEmpty(sg, &r, &c))
		// Success
		return 1;

	// Check all nums (1-9)
	for (int num = 1; num < 10; ++num) {
		if (isSafe(sg, r, c, num)) {

			sg[r][c] = num;

			if (solveSudoku(sg))
				return 1;

			// Failure, unmake and try again
			sg[r][c] = 0;
		}
	}
	// Trigger backtracking
	return 0;
}

int main()
{
	int sg[DIMENSION][DIMENSION];
	for (int i = 0; i<DIMENSION; ++i)
		for (int j = 0; j<DIMENSION; ++j)
			sg[i][j] = 0;

	parseSgFromFile("sudoku.in", sg);

	displaySg(stdout, sg);

	printf("Solving sudoku\n");
	solveSudoku(sg);

	displaySg(stdout, sg);

	return 0;
}
