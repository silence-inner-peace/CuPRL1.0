#include <iostream>
#include <fstream>
#include <sstream>

#include "CuLayer.h"

#include "CuEnvControl.h"
#include "LocalOperator.h"
#include "FocalOperator.h"
#include "NeighborhoodSlope.h"
#include <time.h>

#include "cputest.h"

using namespace std;
using namespace CuPRL;



int main(int argc, char* argv[])
{

	/*
	int testdata[36] = { 1, 1, 1, 1, 1, 1,
		1, 3, 3, 2, 1, 10,
		1, 1, 3, 2, 2, 2,
		1, 2, 2, 2, 2, 2,
		1, 1, 1, 2, 2, 2,
		1, 1, 1, 1, 1, 2 };


	CuLayer<int>testlayer1(testdata, 6, 6);
	testlayer1.setCellHeight(1);
	testlayer1.setCellWidth(1);
	testlayer1.setNoDataValue(10);

	printLayer(testlayer1);
	*/
	//CuEnvControl::setBlockDim(16, 16);

	CuLayer<int>testlayer1;
	testlayer1.Read("C:\\Users\\HP\\Desktop\\WH-DEM\\focalTest\\focalTest1.tif");

	//testlayer1.Read("C:\\Users\\HP\\Desktop\\WH-DEM\\focalTest\\TestRasterWH.tif");
	
	/*int max = 0;
	for (int i = 0; i < testlayer1.getHeight(); i++)
	{
		for (int j = 0; j < testlayer1.getWidth(); j++)
		{
			if (testlayer1[i*testlayer1.getWidth() + j] > max&&testlayer1[i*testlayer1[i*testlayer1.getWidth()+j]]!=10000)
			{
				max = testlayer1[i*testlayer1.getWidth() + j];
			}
		}
	}
	cout << "max=" << max << endl;
	*/
	//cout << testlayer1[0] << endl;



	clock_t t1, t2;

	t1 = clock();

	NeighborhoodSlope neiSlope;

	//CuLayer<double>testgpulayer = cuFocalOperatorFn<int, double, int, SlopeCal>(testlayer1, &neiSlope);


	NeighborhoodRect<int>neiRect(3, 3);

	//CuLayer<int>testSumLayer = testlayer1;
	CuLayer<float>testSumLayer = focalStatisticsMean<int, float, int>(testlayer1, &neiRect, NOUSE, IGNORE);

	t2 = clock();

	cout << t2 - t1 << endl;

	t1 = clock();
	
	
	//CuLayer<double>testcpulayer = CPUSlopeCal(testlayer1, &neiSlope);

	t2 = clock();

	cout << t2 - t1 << endl;
	/*
	if (compareLayer(testcpulayer, testgpulayer) == false)
	{
		cout << "result error" << endl;
	}
	else
	{
		cout << "result right" << endl;
	}
	*/
	/*
	int t = 58 + 3251 * testlayer1.getWidth();

	for (int i = -1; i < 1; i++)
	{
		for (int j = -1; j <= 1; j++)
		{
			cout << testlayer1[t + i*testlayer1.getWidth() + j] << " ";
		}
		cout << endl;
	}
	*/
	testSumLayer.Write("C:\\Users\\HP\\Desktop\\WH-DEM\\focalTest\\TestResult.tif");

	system("pause");
	return 0;
}