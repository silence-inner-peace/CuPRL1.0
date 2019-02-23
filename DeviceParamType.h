#ifndef DEVICEPARAMTYPE_H_H
#define DEVICEPARAMTYPE_H_H

#include "CuPRL.h"

struct rasterInfo
{
	int width;
	int height;
	double cellWidth;
	double cellHeight;

};

template<class WeightType>
struct nbrInfo
{
	int* nbrcood;
	int nbrsize;
	WeightType* weights;
};


struct rasterCell
{
	int x;
	int y;
	int value;
};





namespace CuPRL
{
	#define EPSINON 1e-06

	template<class DataType>
	__device__ bool isNoData(DataType value, DataType nodata)
	{
		if ((value - nodata) > -EPSINON && (value - nodata)<EPSINON)
			return true;
		else
			return false;
	};


	class paramInfo
	{
	public:




	private:

		void* m_params;
		int* m_marks;
		int paramsnum;

	};


}



#endif