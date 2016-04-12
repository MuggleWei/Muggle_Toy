#ifndef __MUGGLE_STRING_UTILS_H__
#define __MUGGLE_STRING_UTILS_H__

#include "base/base.h"
#include <string>
#include <vector>

NS_MUGGLE_BEGIN

class MG_DLL StringUtils
{
public:
	static std::string ToString(int val);
	static std::string ToString(float val);
	static std::string ToString(double val);

	static bool IsNumber(const std::string& str);

	/*********
	// note: this don't catch exception, 
	// you can call IsNumber() before convert or catch exception like follow:
	try
	{
		int val = muggle::StringUtils::ToInt(str);
		MLOG("%d\n", val);
	}
	catch (const std::invalid_argument& ia)
	{
		MLOG("Invalid argument: %s\n", ia.what());
		MASSERT(0);
	}
	*****/
	static int ToInt(const std::string& str);
	static float ToFloat(const std::string& str);
	static double ToDouble(const std::string& str);

	static void Split(const std::string& str, const char c, std::vector<std::string>& out_vec);
	static std::string Replace(const std::string& str, const std::string& old_val, const std::string& new_val);
};

NS_MUGGLE_END

#endif