#ifndef FOCALOPERATORDEVICE_H_H
#define FOCALOPERATORDEVICE_H_H


#include "CuPRL.h"
#include "DeviceParamType.h"






template<class DataInType,class DataOutType>
class FocalStatisticsSum
{
public:
	__device__ DataOutType operator()(DataInType* input, int x, int y, int width, int height, int* nbrcoord, int nbrsize, int boundhandle,int nodatahandle)
	{
		DataOutType outvalue = 0;

		if (nodatahandle == 0)
		{
			for (int idxnbr = 0; idxnbr < nbrsize; idxnbr++)
			{

				int nbridx_x = x + nbrcoord[idxnbr * 2];
				int nbridx_y = y + nbrcoord[idxnbr * 2 + 1];

				if (nbridx_x >= 0 && nbridx_x < width && nbridx_y >= 0 && nbridx_y < height)
				{
					outvalue += input[nbridx_y*width + nbridx_x];
				}
				else
				{
					if (boundhandle == 0)
					{
						outvalue += input[y*width + x];
					}
				}
			}

		}

		
		return outvalue;
	}
};

template<class DataInType, class DataOutType>
class FocalStatisticsMean
{
public:
	__device__ DataOutType operator()(DataInType* input, int x, int y, int width, int height, int* nbrcoord, int nbrsize, int boundhandle,int nodatahandle)
	{
		DataOutType outvalue = 0;

		for (int idxnbr = 0; idxnbr < nbrsize; idxnbr++)
		{

			int nbridx_x = x + nbrcoord[idxnbr * 2];
			int nbridx_y = y + nbrcoord[idxnbr * 2 + 1];

			if (nbridx_x >= 0 && nbridx_x < width && nbridx_y >= 0 && nbridx_y < height)
			{
				outvalue += input[nbridx_y*width + nbridx_x];
			}
			else
			{
				if (boundhandle == 0)
				{
					outvalue += input[y*width + x];
				}
			}
		}
		outvalue /= nbrsize;
		return outvalue;
	}
};

template<class DataInType,class DataOutType>
class FocalStatisticsMaximum
{
public:
	__device__ DataOutType operator()(DataInType* input, int x, int y, int width, int height, int* nbrcoord, int nbrsize,int boundHandle,int nodatahandle)
	{

		DataOutType outvalue = input[y*width + x];

		for (int idxnbr = 0; idxnbr < nbrsize; idxnbr++)
		{

			int	nbridx_x = x + nbrcoord[idxnbr * 2];
			int nbridx_y = y + nbrcoord[idxnbr * 2 + 1];

			if (nbridx_x >= 0 && nbridx_x < width && nbridx_y >= 0 && nbridx_y < height)
			{
				if (outvalue < input[nbridx_y*width + nbridx_x])
					outvalue = input[nbridx_y*width + nbridx_x];
			}
		}
		return outvalue;
	}
};

template<class DataInType,class DataOutType>
class FocalStatisticsMinimum
{
public:
	__device__ DataOutType operator()(DataInType* input, int x, int y, int width, int height, int* nbrcoord, int nbrsize)
	{

		DataOutType outvalue = input[y*width + x];

		for (int idxnbr = 0; idxnbr < nbrsize; idxnbr++)
		{

			int	nbridx_x = x + nbrcoord[idxnbr * 2];
			int nbridx_y = y + nbrcoord[idxnbr * 2 + 1];

			if (nbridx_x >= 0 && nbridx_x < width && nbridx_y >= 0 && nbridx_y < height)
			{
				if (outvalue > input[nbridx_y*width + nbridx_x])
					outvalue = input[nbridx_y*width + nbridx_x];
			}
		}
		return outvalue;
	}
};

template<class DataInType,class DataOutType>
class FocalStatisticsRange
{
public:
	__device__ DataOutType operator()(DataInType* input, int x, int y, int width, int height, int* nbrcoord, int nbrsize,int boundHandle)
	{

		DataOutType minvalue = input[y*width + x];
		DataOutType maxvalue = input[y*width + x];

		for (int idxnbr = 0; idxnbr < nbrsize; idxnbr++)
		{

			int	nbridx_x = x + nbrcoord[idxnbr * 2];
			int nbridx_y = y + nbrcoord[idxnbr * 2 + 1];

			if (nbridx_x >= 0 && nbridx_x < width && nbridx_y >= 0 && nbridx_y < height)
			{
				if (maxvalue<input[nbridx_y*width + nbridx_x])
					maxvalue = input[nbridx_y*width + nbridx_x];

				if (minvalue > input[nbridx_y*width + nbridx_x])
					minvalue = input[nbridx_y*width + nbridx_x];
			}
		}
		return maxvalue - minvalue;
	}
};



class SlopeCal
{
public:
	__device__ double operator()(int* focal, int x, int y,nbrInfo<int>nbrinfo,rasterInfo rasterinfo, int noDataValue)
	{
		int focalValue = *focal;
		if (focalValue == noDataValue)
			return noDataValue;
		int nbrsize = nbrinfo.nbrsize;
		int* nbrcood = nbrinfo.nbrcood;
		int* weights = nbrinfo.weights;
		int width = rasterinfo.width;
		int height = rasterinfo.height;
		double xz = 0;
		for (int i = 0; i < nbrsize / 2; i++)
		{
			int cx = x + nbrcood[i * 2];
			int cy = y + nbrcood[i * 2 + 1];
			if (cx < 0 || cx >= width || cy < 0 || cy >= height)
			{
				xz += focalValue*weights[i];
				continue;
			}
			int curnbr = focal[nbrcood[i * 2] + nbrcood[i * 2 + 1] * width];
			if (curnbr == noDataValue)
				xz += focalValue*weights[i];
			else
				xz += curnbr*weights[i];
		}
		xz /= (nbrsize*rasterinfo.cellWidth / 2);
		double yz = 0;
		for (int i = nbrsize / 2; i < nbrsize; i++)
		{
			int cx = x + nbrcood[i * 2];
			int cy = y + nbrcood[i * 2 + 1];
			if (cx < 0 || cx >= width || cy < 0 || cy >= height)
			{
				yz += focalValue*weights[i];
				continue;
			}
			int curnbr = focal[nbrcood[i * 2] + nbrcood[i * 2 + 1] * width];
			if (curnbr == noDataValue)
				yz += focalValue*weights[i];
			else
				yz += curnbr*weights[i];
		}
		yz /= (nbrsize*rasterinfo.cellHeight / 2);
		double slopevalue = atan(sqrt(xz*xz + yz*yz))*57.29578;
		return slopevalue;
	}

};



/*-------------------------------------------------------------------------
//focalÄ£°å
template<class DataInType,class DataOutType,class WeightType>
class FocalStatistics
{
public:
	__device__ DataOutType operator()(DataInType* focal, int x, int y, int width, int height, int* nbrcoord, int* weights, int nbrsize, int boundhandle)
	{

	}
};
---------------------------------------------------------------------------*/



template<class DataInType,class DataOutType,class WeightType,class Oper>
__global__ void G_FocalStatistics(DataInType *input, DataOutType *output, int width, int height, int *nbrcoord, WeightType *weights, int nbrsize, Oper op, int boundhandle)
{
	int x_idx = blockIdx.x*blockDim.x + threadIdx.x;
	if (x_idx >= width)
		return;
	int y_idx = blockIdx.y*blockDim.y + threadIdx.y;
	if (y_idx >= height)
		return;


	output[y_idx*width + x_idx] = op(input, x_idx, y_idx, width, height, nbrcoord, weights, nbrsize, boundhandle);
}


template<class DataType,class Oper>
__global__ void G_FocalStatistics(DataType *input, DataType *output, int width, int height, int *nbrcoord, int nbrsize, Oper op)
{
	int x_idx = blockIdx.x*blockDim.x + threadIdx.x;
	if (x_idx >= width)
		return;
	int y_idx = blockIdx.y*blockDim.y + threadIdx.y;
	if (y_idx >= height)
		return;


	output[y_idx*width + x_idx] = op(input, x_idx, y_idx, width, height, nbrcoord, nbrsize);
}



template<class DataInType,class DataOutType, class Oper>
__global__ void G_FocalOperator(DataInType *input, DataOutType *output, int width, int height, int *nbrcoord, int nbrsize, DataInType noDataValue,double cellSize, Oper op)
{
	int x_idx = blockIdx.x*blockDim.x + threadIdx.x;
	if (x_idx >= width)
		return;
	int y_idx = blockIdx.y*blockDim.y + threadIdx.y;
	if (y_idx >= height)
		return;


	output[y_idx*width + x_idx] = op(input, x_idx, y_idx, width, height, nbrcoord, nbrsize, noDataValue, cellSize);
}

template<class DataInType, class DataOutType,class WeightType, class Oper>
__global__ void G_FocalOperator(DataInType *input, DataOutType *output, int width, int height, int *nbrcoord, WeightType* weights,int nbrsize, DataInType noDataValue, double cellWidth,double cellHeight, Oper op)
{
	int x_idx = blockIdx.x*blockDim.x + threadIdx.x;
	if (x_idx >= width)
		return;
	int y_idx = blockIdx.y*blockDim.y + threadIdx.y;
	if (y_idx >= height)
		return;
	nbrInfo<WeightType>nbrinfo;
	nbrinfo.weights = weights;
	nbrinfo.nbrcood = nbrcoord;
	nbrinfo.nbrsize = nbrsize;

	rasterInfo rasterinfo;
	rasterinfo.cellHeight = cellHeight;
	rasterinfo.cellWidth = cellWidth;
	rasterinfo.height = height;
	rasterinfo.width = width;


	output[y_idx*width + x_idx] = op(input + x_idx + y_idx*width, x_idx, y_idx, nbrinfo, rasterinfo, noDataValue);
}




/*--------------------------------------------------------------------------------------
template<class DataInType,class DataOutType,class Oper>
class UserOperator
{
public:
__device__ DataOutType operator()(DataInType* value,int cellX,int cellY,int *nbrcoord,int nbrsize,int boundHandle){};
};
---------------------------------------------------------------------------------------*/
template<class DataInType,class DataOutType,class Oper>
__global__ void G_FocalOper(DataInType *input, DataOutType *output, int width, int height, int *nbrcoords, int nbrsize, DataInType nodataIn, DataOutType nodataOut,int boundHandle,int nodataHandle, Oper op)
{
	int x_idx = blockIdx.x*blockDim.x + threadIdx.x;
	if (x_idx >= width)
		return;
	int y_idx = blockIdx.y*blockDim.y + threadIdx.y;
	if (y_idx >= height)
		return;

	
	if (isNoData(input[y_idx*width + x_idx], nodataIn))
	{
		output[y_idx*width + x_idx] = nodataOut;
	}
	else
	{
		output[y_idx*width + x_idx] = op(input + x_idx + y_idx*width, x_idx, y_idx, width, height, nbrcoords, nbrsize, boundHandle, nodataHandle);
	}

}

template<class DataInType,class DataOutType,class WeightType,class Oper>
__global__ void G_FocalOperWeight(DataInType *input, DataOutType *output, int width, int height, int *nbrcoords, WeightType* weights, int nbrsize, DataInType nodataIn, DataOutType nodataOut, int boundHandle, Oper op)
{
	int x_idx = blockIdx.x*blockDim.x + threadIdx.x;
	if (x_idx >= width)
		return;
	int y_idx = blockIdx.y*blockDim.y + threadIdx.y;
	if (y_idx >= height)
		return;

	nbrInfo<WeightType>nbrinfo;
	nbrinfo.weights = weights;
	nbrinfo.nbrcood = nbrcoords;
	nbrinfo.nbrsize = nbrsize;


	if (isNoData(input[y_idx*width + x_idx], nodataIn))
	{
		output[y_idx*width + x_idx] = nodataOut;
	}
	else
	{
		output[y_idx*width + x_idx] = op(input + x_idx + y_idx*width, x_idx, y_idx, width, height, nbrinfo, boundHandle);
	}
}

template<class DataInType,class DataOutType,class WeightType,class Oper>
__global__ void G_FocalOperWeight(DataInType *input, DataOutType *output, int width, int height, double cellWidth, double cellHeight, int *nbrcoords, WeightType* weights, int nbrsize, DataInType nodataIn, DataOutType nodataOut, int boundHandle, Oper op)
{

	int x_idx = blockIdx.x*blockDim.x + threadIdx.x;
	if (x_idx >= width)
		return;
	int y_idx = blockIdx.y*blockDim.y + threadIdx.y;
	if (y_idx >= height)
		return;
	nbrInfo<WeightType>nbrinfo;
	nbrinfo.weights = weights;
	nbrinfo.nbrcood = nbrcoords;
	nbrinfo.nbrsize = nbrsize;

	rasterInfo rasterinfo;
	rasterinfo.cellHeight = cellHeight;
	rasterinfo.cellWidth = cellWidth;
	rasterinfo.height = height;
	rasterinfo.width = width;

	if (isNoData(input[y_idx*width + x_idx], nodataIn))
	{
		output[y_idx*width + x_idx] = nodataOut;
	}
	else
	{
		output[y_idx*width + x_idx] = op(input + x_idx + y_idx*width, x_idx, y_idx, nbrinfo, rasterinfo);
	}



}



#endif