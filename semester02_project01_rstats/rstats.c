//#include "resizable_array.h"
#include "StatsTool.h"
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>

#pragma warning(disable: 4996)

int main(int argc, char* argv[])
{
	printf("*************************************\n");
	printf("Regression Stats @2019 Hai Nguyen\n");
	printf("*************************************\n");

	int s = 0, ch = 0;
	double temp = 0.0;
	// create resizable_array object to store data
	resizable_array harr = res_array();

	// ---------------------------------------------
	// PHASE 1: READ DATA
	// ---------------------------------------------
	FILE* fileInput = stdin;
	if (argc > 2)
	{
		// check for too many arguments
		// print user manual
		printf("Error: too many command line arguments (%d)\n", argc);
		printf("Correct usage: rstats [filename]\n");
		printf("\tWhere filename is the name of the file and file extension\n");
		printf("\tUse stdin if there is no file input\n");
		// short circuit, exit the main() program
		return EXIT_FAILURE;
	}
	else if (argc == 2)
	{
		// open file using fopen_s since fopen is depricated
		if (fopen_s(&fileInput, argv[1], "rb") != 0)
		{
			// if fopen_s returns non-zero value, then reading process fails
			printf("Error: <%s>", argv[1]);
			perror(" ");
			// short circuit, exit the main() program
			return EXIT_FAILURE;
		}
		// handle the fact that fileInput could be '0': there is no such file
		if (fileInput == 0)
			return EXIT_FAILURE;
		// as long as EOF character is not yet caught, reads the next double
		while ((s = fscanf_s(fileInput, "%lf", &temp)) == 1)
		{
			// push/add the read values from file/stdin into the resizable_array object
			// remember to pass the address of the resizable_array object
			push(&harr, temp);
		}
		// check for validation
		// if the EOF character is actually caught, input stream exits normally, then s would be -1.
		// otherwise, input stream crashes due to some invalid character, in which case, s = 0.
		if (s == 0)
		{
			// non-numeric character is caught
			printf("Error: failure reading element %lu (in zero-based).\nPlease input numeric values.\n", harr.nSize);
			return EXIT_FAILURE;
		}
	}
	else
	{
		printf("Enter white-space separated real numbers. Terminate input with EOF (CTRL + Z).\n");
		printf("For example: n1 n2 n3 ... nM ^Z (input can be in multiple lines, but ^Z must be on the same line of final value!)\n");
		while ((s = scanf_s("%lf", &temp)) == 1)
		{
			push(&harr, temp);
		}
		// check for validation
		ch = getchar();
		if (ch == 26)
		{
			// ch is a SUBSTITUTE character (https://en.wikipedia.org/wiki/Substitute_character)
			// meaning, the EOF character is actually input to exit input stream
			// thus, proceed to data procession
			;
		}
		else
		{
			// othwerwise, input stream is interrupted by some invalid character
			// hence, short circuit the main()
			printf("Error: failure reading element %lu (in zero-based).\nPlease input numeric values.\n", harr.nSize);
			return EXIT_FAILURE;
		}
	}

	// ---------------------------------------------
	// PHASE 2: PROCESS DATA
	// ---------------------------------------------

	// CLONE, so that statistical mean can be found based on "clone"
	// as "clone"s underlying array will be sorted
	// leave the underlying array of "harr" unsorted.
	resizable_array clone = res_array();
	for (size_t i = 0; i < harr.nSize; i++)
	{
		push(&clone, harr.data[i]);
	}

	printf("\n\nStandard Analysis\n");
	printf("-------------------\n");
	// # elements
	printf("Total # elements = %zu\n", harr.nSize);
	// range
	array_value_type min = getMin(harr);
	array_value_type max = getMax(harr);
	printf("Range = [ %.2lf ... %.2lf ]\n", min, max);
	// arithmetic mean
	array_value_type arithMean = getArithmeticMean(harr);
	printf("Arithmetic median = %.2lf\n", arithMean);

	// simple linear regression before the underlying array is sorted
	array_value_type* combo = linearRegression(harr, arithMean);
	printf("Linear Line Regression: slope = %.2lf, Y-Intercept = %.2lf\n", *combo, *(combo + 1));
	// statistical median --> USE CLONE
	printf("Statistical median = %.2lf\n", getStatMedian(clone));
	// variance
	array_value_type variance = getVariance(harr, arithMean);
	printf("Variance = %.2lf\n", variance);
	// standard deviation
	array_value_type stdDev = sqrt(variance);
	printf("Standard deviation = %.2lf\n", stdDev);
	// mode
	reportModes(harr);
	printf("End of standard analysis\n");
	printRuler();

	// report frequency table
	printf("\n\nFrequency Table\n");
	printf("-------------------\n");
	array_value_type* frequencies;
	frequencies = freqTable(harr, min, max);
	printf("End of frequency table\n");
	printRuler();

	// report frequency histogram
	printf("\n\nFrequency Histogram\n");
	printf("-------------------\n");
	freqHist(frequencies);
	printf("End of frequency histogram\n");
	printRuler();

	// report box plot
	printf("\n\nBox plot\n");
	printf("-------------------\n");
	boxPlot(harr, min, max);
	printf("End of box plot\n");
	printRuler();

	// ---------------------------------------------
	// PHASE 3: CLEAN UP
	// ---------------------------------------------
	fclose(fileInput);
	free_mem(&harr);
	return EXIT_SUCCESS;
}