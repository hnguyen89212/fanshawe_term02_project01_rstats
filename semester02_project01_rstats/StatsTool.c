#include "StatsTool.h"

array_value_type getMin(const resizable_array arr)
{
	array_value_type min = arr.data[0];
	for (size_t i = 1; i < arr.nSize; i++)
		if (min >= arr.data[i])
			min = arr.data[i];
	return min;
}

array_value_type getMax(const resizable_array arr)
{
	array_value_type max = arr.data[0];
	for (size_t i = 1; i < arr.nSize; i++)
		if (max <= arr.data[i])
			max = arr.data[i];
	return max;
}

array_value_type getArithmeticMean(const resizable_array arr)
{
	array_value_type sum = 0.0;
	for (size_t i = 0; i < arr.nSize; i++)
		sum += arr.data[i];
	return sum / arr.nSize;
}

array_value_type getStatMedian(const resizable_array arr)
{
	// by def, we must sort the underlying array first
	qsort(arr.data, arr.nSize, sizeof(array_value_type), compareFunction);
	if (arr.nSize % 2 == 0)
	{
		// N is even
		size_t idx = arr.nSize / 2;
		return 0.5 * (arr.data[idx - 1] + arr.data[idx]);
	}
	else
	{
		// N is odd
		size_t idx = (arr.nSize + 1) / 2;
		return arr.data[idx - 1];
	}
}

int compareFunction(const void* a, const void* b)
{
	if (*(array_value_type*)a == *(array_value_type*)b)
		return 0;
	else
		return (*(array_value_type*)a > * (array_value_type*)b) ? 1 : -1;
}

array_value_type getVariance(const resizable_array arr, array_value_type arithMean)
{
	array_value_type var = 0.0;
	for (size_t i = 0; i < arr.nSize; i++)
		var += pow(arr.data[i] - arithMean, 2);
	return var / arr.nSize;
}

void reportModes(const resizable_array arr)
{
	size_t t = 0;
	resizable_array counts = res_array();
	push(&counts, 1);
	// an array to store distinct elements
	resizable_array distinctElements = res_array();
	// store the very first element
	push(&distinctElements, arr.data[0]);
	for (size_t i = 1; i < arr.nSize; i++)
	{
		// if this element is different from the previous element
		if (arr.data[i] != arr.data[i - 1])
		{
			// store this element
			push(&distinctElements, arr.data[i]);
			// increment counter "t"
			// push back/add new element to "counts"
			t++;
			push(&counts, 1);
		}
		else
		{
			// if this element is the same as the previous element
			// increment the # of appearances of the current element
			counts.data[t]++;
		}
	}
	array_value_type maxNumApp = getMax(counts);
	resizable_array modes = res_array();
	for (size_t i = 0; i < counts.nSize; i++)
		if (counts.data[i] == maxNumApp)
			push(&modes, distinctElements.data[i]);
	// if modes has the same size as the "distinctElements"
	// that means, every distinct element has the same # of appearances
	bool hasModes = modes.nSize < distinctElements.nSize;
	if (hasModes)
	{
		printf("Modes = { ");
		if (modes.nSize > 1)
		{
			for (size_t i = 0; i < modes.nSize - 1; i++)
				printf("%lf, ", modes.data[i]);
			printf("%lf", modes.data[modes.nSize - 1]);
		}
		else
			printf("%lf", modes.data[0]);
		printf(" } x %lf\n", maxNumApp);
	}
	else
		puts("Mode = no mode");
}

array_value_type* linearRegression(const resizable_array arr, array_value_type arithMean)
{
	array_value_type combo[] = { 0.0, 0.0 };
	// find y bar (arithmetic mean of set of array indices)
	array_value_type idxMean = 0.0;
	for (size_t i = 0; i < arr.nSize; i++)
		idxMean += i;
	idxMean /= (arr.nSize);
	// find sxy
	array_value_type sxy = 0.0;
	for (size_t i = 0; i < arr.nSize; i++)
		sxy += (arr.data[i] - arithMean) * (i - idxMean);
	// find sxx
	array_value_type sxx = 0.0;
	for (size_t i = 0; i < arr.nSize; i++)
		sxx += pow(arr.data[i] - arithMean, 2);
	// slope of regression line
	combo[0] = sxy / sxx;
	// y-intercept
	combo[1] = idxMean - combo[0] * arithMean;
	return combo;
}

array_value_type* freqTable(const resizable_array arr, array_value_type min, array_value_type max)
{
	array_value_type* frequencies = malloc(ROWS_COUNT * sizeof(array_value_type));
	// check if memory block assignment is successful
	if (frequencies == NULL)
	{
		printf("ERROR: There is not enough space...");
		return NULL;
	}
	array_value_type interval = (max - min) / ROWS_COUNT;
	array_value_type lower = min;
	array_value_type upper;
	long count = 0L;
	int maxCountDigits = getNumOfDigits(toNearestInteger(max));
	for (size_t i = 0; i < ROWS_COUNT; i++)
	{
		upper = min + (i * 1.0 + 1) * interval;
		if (i < 9)
		{
			count = countElementsExclusively(arr, lower, upper);
			frequencies[i] = count * 100.0 / arr.nSize;
			printf("%c [ %*.2lf ... %*.2lf ) = %*ld : %*.2lf%%\n", (char)(i + 65), maxCountDigits, lower, maxCountDigits, upper, maxCountDigits, count, maxCountDigits, frequencies[i]);
		}
		else
		{
			count = countElementsInclusively(arr, lower, upper);
			frequencies[i] = count * 100.0 / arr.nSize;
			printf("%c [ %*.2lf ... %*.2lf ] = %*ld : %*.2lf%%\n", (char)(i + 65), maxCountDigits, lower, maxCountDigits, upper, maxCountDigits, count, maxCountDigits, frequencies[i]);
		}
		lower = upper;
	}
	return frequencies;
}

void freqHist(const array_value_type* freq)
{
	// setup
	UINT currentCodePage = GetConsoleOutputCP();
	switchToCodePage850();
	// header
	printf("GRP%%\t\t");
	for (int i = 0; i <= 100; i += 5)
	{
		printf("%d", i);
		for (int j = 0; j < 5; j++)
			printf(" ");
	}
	printf("\n");
	// back ticks and line under header
	printBackTicksAndLine(0);
	// body -- i refers to each row index
	for (int i = 0; i < ROWS_COUNT; i++)
	{
		// print the alphabetical category
		printf("%c ", (char)(i + 65));
		// print the % numerics
		printf("%*.2lf%%\t", 5, *(freq + i));
		// I need to round the current frequency to one decimal place to draw the bar
		array_value_type da = roundToOneDecimalPlace(*(freq + i));
		// print the bar. A bar is divided into 2 parts, majority and minority.
		int len = toNearestInteger(da);
		int min = len % 5;
		int maj = len - min;
		int inc = (int)(0.4 * maj - 1);
		// but first, print the vertical segment
		printf("\xB3");
		// print majority of bar
		for (int k = 0; k < maj + inc - 1; k++)
			printf("\xDC");
		// print minority of bar
		for (int k = 0; k < min; k++)
			printf("\xDC");
		// end of a row --> new line
		printf("\n");
	}
	// back ticks and line
	printBackTicksAndLine(1);
	// finish up, reset to the current code page
	switchBackToNormalCodePage(currentCodePage);
}

void boxPlot(const resizable_array arr, array_value_type min, array_value_type max)
{
	// setup
	UINT currentCodePage = GetConsoleOutputCP();
	switchToCodePage850();
	// HEADING
	printf("Idx\t\t%.2lf", min);
	for (size_t i = 0; i < ROWS_COUNT; i++)
		for (int j = 0; j < ROWS_COUNT - 1; j++)
			printf(" ");
	printf("     %.2lf\n", max);
	// upper axis
	for (int i = 0; i < 16; i++)
		printf("\xC4");
	for (size_t i = 0; i < ROWS_COUNT; i++)
	{
		printf("\xDA");
		for (size_t j = 0; j < ROWS_COUNT - 1; j++)
			printf("\xC4");
	}
	printf("\xDA");
	for (size_t j = 0; j < 5; j++)
		printf("\xC4");
	printf("\n");
	//
	array_value_type interval = (max - min) / ROWS_COUNT;
	array_value_type lower = min;
	array_value_type upper;
	long count = 0L;
	size_t from = 0;
	size_t to = 0;
	for (size_t i = 0; i < ROWS_COUNT; i++)
	{
		upper = min + (i * 1.0 + 1) * interval;
		// temporary bucket
		resizable_array bucket = res_array();
		if (i < 9)
			count = countElementsExclusively(arr, lower, upper);
		else
			count = countElementsInclusively(arr, lower, upper);
		to = from + count;
		// populate the bucket
		for (size_t idx = from; idx < to; idx++)
			push(&bucket, arr.data[idx]);
		// PLOT
		array_value_type bucketMin = getMin(bucket);
		array_value_type bucketMax = getMax(bucket);
		printf("%d  \t\t", i);
		printf("\xB3");
		// blanks
		size_t blanks = (size_t)(100.0 * (bucketMin - min) / (max - min));
		for (size_t j = 0; j < blanks; j++)
			printf(" ");
		// squares
		size_t squares = (size_t)(100.0 * (bucketMax - min) / (max - min) - blanks);
		if (squares < 10)
			for (size_t j = 0; j < squares + 1; j++)
				printf("\xDC");
		else
			for (size_t j = 0; j < squares; j++)
				printf("\xDC");
		printf("\n");
		// update from, to indexing variables
		from = to;
		// update lower to upper, for next round
		lower = upper;
		// free the bucket
		free_mem(&bucket);
	}
	// lower axis
	for (int i = 0; i < 16; i++)
		printf("\xC4");
	for (size_t i = 0; i < ROWS_COUNT; i++)
	{
		printf("\xC0");
		for (size_t j = 0; j < ROWS_COUNT - 1; j++)
			printf("\xC4");
	}
	printf("\xC0");
	for (size_t j = 0; j < 5; j++)
		printf("\xC4");
	printf("\n");
	// finish up, reset to the current code page
	switchBackToNormalCodePage(currentCodePage);
}

// ----------------------------------------------
// SECONDARY
// ----------------------------------------------

void switchToCodePage850()
{
	UINT currentCodePage = GetConsoleOutputCP();
	if (!SetConsoleOutputCP(850))
		puts("Error: SetConsoleOutputCP please.\n");
}

void switchBackToNormalCodePage(UINT currentCodePage)
{
	if (!SetConsoleOutputCP(currentCodePage))
		puts("Error: SetConsoleOutputCP please.\n");
}

void printBackTicksAndLine(int k)
{
	for (int i = 0; i < 16; i++)
		printf("\xC4");
	for (int i = 0; i <= 100; i += 5)
	{
		// k == 1 --> line and backticks under the table
		if (k == 1)
			printf("\xC0");
		// k == 0 --> line and backticks under the header
		else
			printf("\xDA");
		if (i >= 10)
			for (int j = 0; j < 6; j++)
				printf("\xC4");
		else
			for (int j = 0; j < 5; j++)
				printf("\xC4");
	}
	printf("\n");
}

long countElementsExclusively(const resizable_array arr, array_value_type lowerBound, array_value_type upperBound)
{
	long count = 0L;
	for (size_t i = 0; i < arr.nSize; i++)
		if (arr.data[i] >= lowerBound && arr.data[i] < upperBound)
			count++;
	return count;
}

long countElementsInclusively(const resizable_array arr, array_value_type lowerBound, array_value_type upperBound)
{
	long count = 0L;
	for (size_t i = 0; i < arr.nSize; i++)
		if (arr.data[i] >= lowerBound && arr.data[i] <= upperBound)
			count++;
	return count;
}

void printRuler()
{
	UINT currentCodePage = GetConsoleOutputCP();
	if (!SetConsoleOutputCP(850))
		puts("Error: SetConsoleOutputCP please.\n");
	for (int i = 0; i < 100; i++)
		printf("\xCD");
	if (!SetConsoleOutputCP(currentCodePage))
		puts("Error: SetConsoleOutputCP please.\n");
}

array_value_type roundToOneDecimalPlace(array_value_type d)
{
	array_value_type dr = d;
	dr *= 10;
	dr += 0.5;
	dr = (int)(dr);
	dr = (array_value_type)(dr) / 10.0;
	return dr;
}

array_value_type findMax(const array_value_type* freq)
{
	array_value_type max = *(freq + 0);
	for (size_t i = 1; i < ROWS_COUNT; i++)
		if (max <= *(freq + i))
			max = *(freq + i);
	return max;
}

int toNearestInteger(array_value_type n)
{
	return (int)(round(n));
}

int getNumOfDigits(int v)
{
	int nd = 0;
	while (v != 0)
	{
		v /= 10;
		nd++;
	}
	// account for "." (decimal point) and 2 digits after it and the minus sign --> plus 4
	return nd + 4;
}