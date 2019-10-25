#include "resizable_array.h"
#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <stdbool.h>
#include <windows.h>

// there are 10 categories from A...J
#define ROWS_COUNT 10

// ----------------------------------------------
// PRIMARAY FUNCTIONS -- DIRECT RELATION TO STATISTICS CONCEPTS
// ----------------------------------------------

/**
 * Given a data set, gets the minimum value in it.
 */
array_value_type getMin(const resizable_array arr);

/**
 * Given a data set, gets the maximum value in it.
 */
array_value_type getMax(const resizable_array arr);

/**
 * Given a data set, gets the arithmetic mean of it.
 */
array_value_type getArithmeticMean(const resizable_array arr);

/**
 * Finds statistical median of a data set.
 */
array_value_type getStatMedian(const resizable_array arr);

/**
 * A helper function to assist sorting of an array.
 * Returns
 *      0 if value stored at a == that at b
 *      1 if that at a > that at b
 *      -1 if that at a < that at b
 */
int compareFunction(const void* a, const void* b);

/**
 * Finds variance of a data set, provided that its arithmetic mean is known.
 */
array_value_type getVariance(const resizable_array arr, array_value_type arithMean);

/**
 * Reports the mode (if any) in data set.
 */
void reportModes(const resizable_array arr);

/**
 * Reports slope of regression line and y-intercept of data set.
 */
array_value_type* linearRegression(const resizable_array arr, array_value_type arithMean);

/**
 * Reports the frequency table and returns the frequencies array as double array.
 */
array_value_type* freqTable(const resizable_array arr, array_value_type min, array_value_type max);

/**
 * Plots the frequency histogram of data set, provided that the frequencies array is known.
 */
void freqHist(const array_value_type* freq);

/**
 * Plots the box plot of UNSORTED data set.
 */
void boxPlot(const resizable_array arr, array_value_type min, array_value_type max);

// ----------------------------------------------
// SECONDARY FUNCTIONS -- NO DIRECT RELATION TO STATISTICS CONCEPTS
// ----------------------------------------------

void switchToCodePage850();

void switchBackToNormalCodePage(UINT currentCodePage);

void printBackTicksAndLine(int i);

/**
 * Given an array of double, counts the number of elements that lowerBound <= x < upperBound
 */
long countElementsExclusively(const resizable_array arr, array_value_type lowerBound, array_value_type upperBound);

/**
 * Given an array of double, counts the number of elements that lowerBound <= x <= upperBound
 */
long countElementsInclusively(const resizable_array arr, array_value_type lowerBound, array_value_type upperBound);

/**
 * Prints a ruler to mark end of a major section in console app.
 */
void printRuler();

/**
 * Rounds a double to one decimal place
 */
array_value_type roundToOneDecimalPlace(array_value_type d);

/**
 * Given an array of double, finds the maximum value/element.
 */
array_value_type findMax(const array_value_type* freq);

/**
 * Rounds a double to the nearest integer
 */
int toNearestInteger(array_value_type n);

/**
 * Determines the number of digits of an integer.
 */
int getNumOfDigits(int v);