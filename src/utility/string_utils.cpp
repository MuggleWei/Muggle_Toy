#include "string_utils.h"
#include <sstream>

NS_MUGGLE_BEGIN

std::string StringUtils::ToString(int val)
{
	return std::to_string(val);
}
std::string StringUtils::ToString(float val)
{
	return std::to_string(val);
}
std::string StringUtils::ToString(double val)
{
	return std::to_string(val);
}

bool StringUtils::IsNumber(const std::string& str)
{
	std::istringstream iss(str);
	float f;
	iss >> std::noskipws >> f;
	return iss.eof() && !iss.fail();
}

int StringUtils::ToInt(const std::string& str)
{
	return std::stoi(str);
}
float StringUtils::ToFloat(const std::string& str)
{
	return std::stof(str);
}
double StringUtils::ToDouble(const std::string& str)
{
	return std::stod(str);
}

void StringUtils::Split(const std::string& str, const char c, std::vector<std::string>& vec)
{
	vec.clear();

	const char* start = str.c_str();
	const char *p, *q;
	ptrdiff_t diff;

	p = q = start;
	while (*p)
	{
		if (*p == c)
		{
			diff = p - q;
			if (diff != 0)
			{
				vec.push_back(str.substr((size_t)(q - start), (size_t)diff));
			}
			++p;
			q = p;
			continue;
		}

		++p;
	}

	diff = p - q;
	if (diff != 0)
	{
		vec.push_back(str.substr((size_t)(q - start), (size_t)diff));
	}
}
std::string StringUtils::Replace(const std::string& str, const std::string& old_val, const std::string& new_val)
{
	std::string new_str;

	size_t start_pos = 0, pos = 0;	
	size_t old_val_len = old_val.size();
	size_t len = str.size();
	const char* p_start = str.c_str();
	while ((pos = str.find(old_val.c_str(), start_pos)) != std::string::npos)
	{
		new_str.append(p_start + start_pos, pos - start_pos);
		new_str.append(new_val);
		start_pos = pos + old_val_len;
		if (start_pos >= len)
		{
			break;
		}
	}

	if (start_pos < len)
	{
		new_str.append(p_start + start_pos);
	}

	return new_str;
}

NS_MUGGLE_END