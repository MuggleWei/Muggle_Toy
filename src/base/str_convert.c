#include "str_convert.h"

bool StrConvert_atoi(char* ptr_ch, int* ptr_i)
{
	char *p = ptr_ch;
	int sign = 1;
	int sum = 0;
	while (*p != '\0')
	{
		if (*p == '-')
		{
			if (p != ptr_ch)
			{
				return false;
			}
			sign = -1;
		}
		else if (*p >= '0' && *p <= '9')
		{
			sum = sum * 10 + *p - '0';
		}
		else
		{
			return false;
		}
		++p;
	}

	*ptr_i = sign * sum;
	return true;
}