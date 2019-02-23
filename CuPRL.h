#ifndef CUPRL_H_H
#define CUPRL_H_H

//#include "CuEnvControl.h"

#include <cuda_runtime.h>
#include <device_launch_parameters.h>
#include <device_functions.h>
#include <gdal_priv.h>
#include <string>

#include "errorhelper.h"
#include "DeviceParamType.h"

namespace CuPRL
{

	static const bool             DEFAULT_NODATA_BOOL = false;
	static const unsigned char    DEFAULT_NODATA_UCHAR = 255;
	static const char             DEFAULT_NODATA_CHAR = -128;
	static const unsigned short   DEFAULT_NODATA_USHORT = 65535;
	static const short            DEFAULT_NODATA_SHORT = -32768;
	static const unsigned int     DEFAULT_NODATA_UINT = 4294967295;
	static const int              DEFAULT_NODATA_INT = -2147483648;
	static const unsigned long    DEFAULT_NODATA_ULONG = 4294967295;
	static const long             DEFAULT_NODATA_LONG = -2147483648;
	static const float            DEFAULT_NODATA_FLOAT = -2147483648;
	static const double           DEFAULT_NODATA_DOUBLE = -2147483648;
	static const long double      DEFAULT_NODATA_LDOUBLE = -2147483648;



	enum BoundHanleType
	{
		FOCALUSE = 0,
		NOUSE
	};

	enum NoDataHandle
	{
		IGNORE,
		NOCAL
	};

	enum LocalOperType
	{
		LOCALOPER = 0,
		LOCALOPERWITHNODATA,
		LOCALOPERPARAM,
		LOCALOPERPARAMWITHNODATA,
		LOCALOPERPARAMS,
		LOCALOPERPARAMSWITHNODATA,
		LOCALOPERMULTILAYERS,
		LOCALOPERMULTILAYERSWITHNODATA
	};



	GDALDataType getGDALDataType(std::string dataTypeName)
	{
		GDALDataType gdalType = GDT_Unknown;
		if (dataTypeName == typeid(unsigned char).name()) 
		{
			gdalType = GDT_Byte;
		}
		else if (dataTypeName == typeid(unsigned short int).name()) 
		{
			gdalType = GDT_UInt16;
		}
		else if (dataTypeName == typeid(short int).name()) 
		{
			gdalType = GDT_Int16;
		}
		else if (dataTypeName == typeid(unsigned int).name()) 
		{
			gdalType = GDT_UInt32;
		}
		else if (dataTypeName == typeid(int).name()) 
		{
			gdalType = GDT_Int32;
		}
		else if (dataTypeName == typeid(float).name()) 
		{
			gdalType = GDT_Float32;
		}
		else if (dataTypeName == typeid(double).name()) 
		{
			gdalType = GDT_Float64;
		}
		return gdalType;
	}


	char* findImageTypeGDAL(std::string DstImgFileName)
	{
		int index = DstImgFileName.find_last_of('.');
		std::string dstExtension = DstImgFileName.substr(index + 1, DstImgFileName.length() - index - 1);
		char *Gtype = NULL;
		if (dstExtension == "bmp") Gtype = "BMP";
		else if (dstExtension == "jpg") Gtype = "JPEG";
		else if (dstExtension == "png") Gtype = "PNG";
		else if (dstExtension == "tif") Gtype = "GTiff";
		else if (dstExtension == "gif") Gtype = "GIF";
		else Gtype = NULL;

		return Gtype;
	}

}






#endif