#ifndef QUICKSORT_H
#define QUICKSORT_H

#include<vector>
#include<cstdlib>

template<typename T>
void swapp(std::vector<T>& vec, int loc1, int loc2)
{
	T tmp = vec[loc1];
	vec[loc1] = vec[loc2];
	vec[loc2] = tmp;
}

template<typename T, typename Comp>
void quicksort(int lf, int rt, std::vector<T>& vec, Comp& c)
{
	if(rt - lf <= 0)
		return;
	int pivot = std::rand() % (rt - lf + 1) + lf;
	if(pivot != rt)
		swapp(vec,pivot,rt);
	for(int i = lf, j = lf; j <= rt - 1; ++j)
	{
		if(!c(vec[j],vec[rt]))
		{
			if(i != j)
				swapp(vec,i,j);
			++i;
		}
		if(j == rt - 1)
			pivot = i;
	}
	swapp(vec,pivot,rt);
	if(pivot >= 1)
		quicksort(lf,pivot-1,vec,c);
	if(pivot <= rt - 1)
		quicksort(pivot + 1,rt,vec,c);
}


#endif

