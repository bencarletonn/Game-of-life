#include<stdio.h>
#include<stdlib.h>
#include"gol.h"


// Read in text file
void read_in_file(FILE *infile, struct universe *u) {
	char c;

	// Initial allocation
	u->grid = malloc(sizeof(char*));
	if(u->grid == NULL) {
		fprintf(stderr, "Error in malloc() \n");
		exit(1);
	}
	u->grid[0] = malloc(sizeof(char));
	if(u->grid[0] == NULL) {
		fprintf(stderr, "Error in malloc() \n");
		exit(1);
	}

	int num_rows = 1;
	int row = 0;
	int num_columns = 1;
	int column = 0;
	int final_counter = 0;

	u->aliveCells = 0;
	u->totalAliveCells = 0;

	while((c = fgetc(infile)) != EOF) {
		if(c == '\n') {
			num_rows++;
			u->grid = realloc(u->grid, num_rows * sizeof(char*));
			if(u->grid == NULL) {
				fprintf(stderr, "Error in realloc() \n");
				exit(1);
			}
			row++;
			final_counter = 0;
			continue;
		}
		if(c == '*') {
			u->aliveCells++;
		}
		num_columns++;
		u->grid[row] = realloc(u->grid[row], num_columns * sizeof(char));
		if(u->grid[row] == NULL) {
			fprintf(stderr, "Error in realloc() \n");
			exit(1);
		}
		u->grid[row][final_counter] = c;
		column++;
		final_counter++;
	}
	u->rows = row;
	u->columns = (column/row);

}


// Write out text file
void write_out_file(FILE *outfile, struct universe *u) {

	// CHANGED COLUMNS AND ROWS AROUND
	for(int i = 0; i < u->rows; i++) {
		if(i == 0) {
			i = i; // Do nothing
		}
		else {
			fprintf(outfile, "\n");
		}
		for(int j = 0; j < u->columns; j++) {
			fprintf(outfile, "%c", u->grid[i][j]);
		}
	}
}


// Check if a cell at a specified column and row is alive
int is_alive(struct universe *u, int column, int row) {

	int value = u->grid[row][column]; // Convert to ASCII value
	if(value == 46) { // ASCII .
		return(0);
	}
	else if(value == 42) { // ASCII *
		return(1);
	}
	else {
		fprintf(stderr, "Unknown cell state\n");
		exit(1);
	}
}


// Assuming all cells outside the border are dead 
int will_be_alive(struct universe *u, int column, int row) {

	int aliveCounter = 0;
	int variableRow = 0;
	int variableColumn = 0;
	int maxColumns = (u->columns) - 1;
	int maxRows = (u->rows) - 1;

	// Check if current cell is alive
	int state;
	state = is_alive(u, column, row);

	// Check its 8 neighbours

	// Directly above
	variableRow = row - 1;
	if(variableRow < 0) {
		aliveCounter = aliveCounter; // Do nothing
	}
	else {
		aliveCounter += is_alive(u, column, variableRow);
	}
	variableRow = 0; // Set back to 0

	// Left upper diagonal
	variableRow = row - 1;
	variableColumn = column - 1;
	if(variableRow < 0 || variableColumn < 0) {
		aliveCounter = aliveCounter; // Do nothing
	}
	else {
		aliveCounter += is_alive(u, variableColumn, variableRow);
	}
	variableRow = variableColumn = 0; // Set back to 0

	// Right upper diagonal
	variableRow = row - 1;
	variableColumn = column + 1;
	if(variableRow < 0 || variableColumn > maxColumns) {
		aliveCounter = aliveCounter; // Do nothing
	}
	else {
		aliveCounter += is_alive(u, variableColumn, variableRow);
	}
	variableRow = variableColumn = 0; // Set back to 0

	// Left
	variableColumn = column - 1;
	if(variableColumn < 0) {
		aliveCounter = aliveCounter; // Do nothing
	}
	else {
		aliveCounter += is_alive(u, variableColumn, row);
	}
	variableColumn = 0; // Set back to 0

	// Right
	variableColumn = column + 1;
	if(variableColumn > maxColumns) {
		aliveCounter = aliveCounter; // Do nothing
	}
	else {
		aliveCounter += is_alive(u, variableColumn, row);
	}
	variableColumn = 0; // Set back to 0

	// Directly below
	variableRow = row + 1;
	if(variableRow > maxRows) {
		aliveCounter = aliveCounter; // Do nothing
	}
	else {
		aliveCounter += is_alive(u, column, variableRow);
	}
	variableRow = 0; // Set back to 0

	// Left lower diagonal
	variableRow = row + 1;
	variableColumn = column -1;
	if(variableRow > maxRows || variableColumn < 0) {
		aliveCounter = aliveCounter; // Do nothing
	}
	else {
		aliveCounter += is_alive(u, variableColumn, variableRow);
	}
	variableRow = variableColumn = 0; // Set back to 0

	// Right lower diagonal
	variableRow = row + 1;
	variableColumn = column + 1;
	if(variableRow > maxRows || variableColumn > maxColumns) {
		aliveCounter = aliveCounter; // Do nothing
	}
	else {
		aliveCounter += is_alive(u, variableColumn, variableRow);
	}
	variableRow = variableColumn = 0; // Set back to 0

	// Compare aliveCounter
	if(state == 0) { // If current cell is dead
		if(aliveCounter == 3) { // and has 3 living neighbours
			u->aliveCells += 1;
			u->totalAliveCells +=1;
			return(1);
		}
		else {
			return(0);
		}
	}
	else if(state == 1) { // If the current cell is living
		if(aliveCounter == 2 || aliveCounter == 3) { // and has 2 or 3 living neighbours
			u->aliveCells += 1;
			u->totalAliveCells +=1;
			return(1);
		}
		else {
			return(0);
		}
	}
	else {
		fprintf(stderr, "Unknown cell state\n");
		exit(1);
	}
}


// Using a torus topology 
int will_be_alive_torus(struct universe *u, int column, int row) {

	int aliveCounter = 0;
	int variableRow = 0;
	int variableColumn = 0;
	int maxColumns = (u->columns)-1;
	int maxRows = (u->rows)-1;

	// Check if current is alive
	int state;
	state= is_alive(u, column, row);

	// Directly above
	variableRow = row - 1;
	if(variableRow < 0) {
		variableRow = maxRows;
		aliveCounter += is_alive(u, column, variableRow);
	}
	else {
		aliveCounter += is_alive(u, column, variableRow);
	}
	variableRow = 0; // Set back to 0

	// Left upper diagonal
	variableRow = row - 1;
	variableColumn = column - 1;
	if(variableRow < 0 && variableColumn < 0) { // In the case of a corner
		aliveCounter += is_alive(u, maxColumns, maxRows);
	}
	else if(variableRow < 0 && variableColumn >= 0) { // In the case of the first row
		aliveCounter += is_alive(u, variableColumn, maxRows);
	}
	else if(variableRow >= 0 && variableColumn < 0) { // In the case of the first column
		aliveCounter += is_alive(u, maxColumns, variableRow);
	}
	else {
		aliveCounter += is_alive(u, variableColumn, variableRow);
	}
	variableColumn = variableRow = 0;

	// Right upper diagonal
	variableRow = row - 1;
	variableColumn = column + 1;
	if(variableRow < 0 && variableColumn > maxColumns) { // In the case of a corner
		aliveCounter += is_alive(u, 0, maxRows);
	}
	else if(variableRow < 0 && variableColumn <= maxColumns) { // In the case of the first row
		aliveCounter += is_alive(u, variableColumn, maxRows);
	}
	else if(variableRow >= 0 && variableColumn > maxColumns) { // In the case of the last column
		aliveCounter += is_alive(u, 0, variableRow);
	}
	else {
		aliveCounter += is_alive(u, variableColumn, variableRow);
	}
	variableColumn = variableRow = 0; // Set back to 0

	// Left
	variableColumn = column - 1;
	if(variableColumn < 0) {
		variableColumn = maxColumns;
		aliveCounter += is_alive(u, variableColumn, row);
	}
	else {
		aliveCounter += is_alive(u, variableColumn, row);
	}
	variableColumn = 0; // Set back to 0

	// Right
	variableColumn = column +1;
	if(variableColumn > maxColumns) {
		variableColumn = 0;
		aliveCounter += is_alive(u, variableColumn, row);
	}
	else {
		aliveCounter += is_alive(u, variableColumn, row);
	}
	variableColumn = 0; // Set back to 0

	// Directly below
	variableRow = row + 1;
	if(variableRow > maxRows) {
		variableRow = 0;
		aliveCounter += is_alive(u, column, variableRow);
	}
	else {
		aliveCounter += is_alive(u, column, variableRow);
	}
	variableRow = 0;

	// Left lower diagonal
	variableRow = row + 1;
	variableColumn = column -1;
	if(variableRow > maxRows && variableColumn < 0) { // In the case of a corner
		aliveCounter += is_alive(u, maxColumns, 0);
	}
	else if(variableRow > maxRows && variableColumn >= 0) { // In the case of the bottom row
		aliveCounter += is_alive(u, variableColumn, 0);
	}
	else if(variableRow <= maxRows && variableColumn < 0) { // In the case of the first column
		aliveCounter += is_alive(u, maxColumns, variableRow);
	}
	else {
		aliveCounter += is_alive(u, variableColumn, variableRow);
	}
	variableRow = variableColumn = 0; // Set back to 0

	// Right lower diagonal
	variableRow = row + 1;
	variableColumn = column + 1;
	if(variableRow > maxRows && variableColumn > maxColumns) { // In the case of the corner
		aliveCounter += is_alive(u, 0, 0);
	}
	else if(variableRow > maxRows && variableColumn <= maxColumns) { // In the case of the bottom row
		aliveCounter += is_alive(u, variableColumn, 0);
	}
	else if(variableRow <= maxRows && variableColumn > maxColumns) { // In the case of the last column
		aliveCounter += is_alive(u, 0, variableRow);
	}
	else {
		aliveCounter += is_alive(u, variableColumn, variableRow);
	}
	variableRow = variableColumn = 0; // Set back to 0

	// Compare aliveCounter
	if(state == 0) { // If current cell is dead
		if(aliveCounter == 3) { // and has 3 living neighbours
			u->aliveCells +=1;
			u->totalAliveCells +=1;
			return(1);
		}
		else {
			return(0);
		}
	}
	else if(state == 1) { // If the current cell is living
		if(aliveCounter == 2 || aliveCounter == 3) { // and has 2 or 3 living neighbours
			u->aliveCells +=1;
			u->totalAliveCells +=1;
			return(1);
		}
		else {
			return(0);
		}
	}
	else {
		fprintf(stderr, "Unknown cell state\n");
		exit(1);
	}
}


// Evolve a cell according to some rule
void evolve(struct universe *u, int (*rule)(struct universe *u, int column, int row)) {
	u->aliveCells = 0;

	// Initialise another 2D array to store the result
	char new_positions[(u->columns)][(u->rows)];

	// Scan through grid and append results to new_positions array
	for(int i = 0; i < (u->rows); i++) {
		for(int j = 0; j < (u->columns); j++) {
			if((*rule)(u, j, i) == 1) { // If cell is going to live next gen
				new_positions[j][i] = '*';
			}
			else {
				new_positions[j][i] = '.';
			}
		}
	}
	// Replace chars in u->grid with chars in new_positions
	for(int i = 0; i < (u->rows); i++) {
		for(int j = 0; j < (u->columns); j++) {
			u->grid[i][j] = new_positions[j][i];
		}
	}
}



// Print statistics
void print_statistics(struct universe *u) {

	float currentAlive = u->aliveCells;
	float averageAlive = u->totalAliveCells;
	float totalCells = (u->rows) * (u->columns);
	float avrAliveCells = (currentAlive / totalCells) * 100;
	float avrTotalAliveCells = (currentAlive / averageAlive) * 100;

	fprintf(stdout, "%0.3f%% of cells currently alive\n", avrAliveCells);
	fprintf(stdout, "%0.3f%% of cells alive on average\n", avrTotalAliveCells);
}
