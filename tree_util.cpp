#include "tree_util.h"
#include <cmath>

unsigned int util::max_size_tree(unsigned short level)
{
	return pow(2, level) - 1;
}

unsigned int util::thread_count(unsigned short level)
{
	return pow(2, level + 1) - 1;
}
