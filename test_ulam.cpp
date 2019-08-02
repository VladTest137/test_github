// TestProject.cpp: определяет точку входа для консольного приложения.
//

#include <time.h>
#include <algorithm>
#include <vector>
#include <iostream>
#include <unordered_set>

#include <xmmintrin.h>

using namespace std;


// function to compute ulam Number 
void ulam1(vector<int> &arr, unsigned int max_value) 
{ 
  
    // Set to search specific Ulam number efficiently 
    unordered_set<int> s; 
  
    // push First 2 two term of the sequence 
    // in the array and set 
    // for further calculation 
  
    s.insert(arr[0]); 
    s.insert(arr[1]); 
  
    // loop to generate Ulam number 
    for (int i = 3; i < (int) max_value; i++) { 
  
        int count = 0; 
  
        // traverse the array and check if 
        // i can be reprsented as sum of 
        // two distinct element of the array 
  
        for (int j = 0; j < (int) arr.size(); j++) { 
  
            // Check if i-arr[j] exist in the array or not using set 
            // If yes, Then i can be represented as 
            // sum of arr[j] + (i- arr[j]) 
  
            if (s.find(i - arr[j]) != s.end() && arr[j] != (i - arr[j])) 
                count++; 
  
            // if Count is greater than 2 
            // break the loop 
            if (count > 2) 
                break; 
        } 
  
        // If count is 2 that means 
        // i can be represented as sum of 
        // two distinct terms of the sequence 
  
        if (count == 2) { 
            // i is ulam number 
            arr.push_back(i); 
            s.insert(i); 
        } 
    } 
} 

// CPP code to print nth 
// Ulam number 
// function to compute ulam Number 
void ulam2(vector<int> &arr, unsigned int max_value, int option) 
{ 
	// push First 2 two term of the sequence 
	// in the array 
	// for further calculation 
	vector<unsigned char>::iterator it;
	vector<unsigned char> hist_count(max_value*2+16, 0); 
	vector<unsigned int> last_change; 	
	//fill(hist_count.begin(), hist_count.end(), 5);
	unsigned int sum_value;
	unsigned int index;

	// loop to generate Ulam number 
	for (unsigned int i = 3; i < max_value; i++) { 
		// traverse the array and check if 
		// i can be reprsented as sum of 
		// two distinct element of the array 
		last_change.clear();
		last_change.reserve(arr.size() - 1);
		for (unsigned int j = 0; j < arr.size() - 1; j++) {  
			int k = arr.size() - 1;
			sum_value = arr[j] + arr[k];
			if(hist_count[sum_value]<5)
				hist_count[sum_value] ++;
			//last_change.push_back(sum_value);
		} 

		// find first index hist_count[index] == 1  
		// that means it can be represented as sum of 
		// two distinct terms of the sequence  
		if(option && arr.size() > 20000 )
		{
			int mask, accumulator;
			char* pStart, *pEnd, *pStartBl, *pEndBl;
			__m128i T1vector, one_vector8b, equal1_vector;
			one_vector8b =  _mm_set1_epi8(1);			
			index = *(arr.end()-1) +1 ;		
			pStart = (char*) &hist_count[index];
			pEnd = (char*) &hist_count[hist_count.size()-16];			
			for(; pStart < pEnd; )
			{
				pEndBl = pStart + 16*16;
				accumulator = 0;
				pStartBl = pStart;
				for(; pStart < pEndBl; pStart += 16)
				{
					T1vector = _mm_loadu_si128 ( (__m128i*) pStart);
					equal1_vector = _mm_cmpeq_epi8(T1vector, one_vector8b);
					mask = _mm_movemask_epi8(equal1_vector);
					accumulator += mask;
				}

				if(accumulator)
				{
					index = pStartBl - (char*) &hist_count[0];
					break;
				}
			}
		}
		else		
			index  = *(arr.end()-1) +1;
		// process precisely
		{   // check every element
			for (it = hist_count.begin() + index; it != hist_count.end(); ++it)
				if(*it == 1)
					break;
			index = it - hist_count.begin();
		}


		//cout << index << ' ';
		if(index > max_value) {
			cout << "maximum count = " << int(*max_element(hist_count.begin(), hist_count.end())) << endl;
			break;		
		}
		arr.push_back(index); 
	} 

} 

int period_diff(vector<int> &arr)
{
	vector<int> arr_diff; 
	vector<int>::iterator it;

	int len = arr.size();
	int start = len/10;
	int len3 = (len-start)/3;
	int pos, i;
	double abs_diff;
	for(it = arr.begin()+1; it < arr.end(); ++it)
		arr_diff.push_back(*it - *(it-1));

	for(pos = start+1; pos < len - len3; pos++) 
	{
		abs_diff = 0;
		for(i = 0; i < len3; i++)
		{
			abs_diff += (arr_diff[pos+i] - arr_diff[start+i] )*(arr_diff[pos+i] - arr_diff[start+i] );
			if(abs_diff>0)
				break;
		}
		if(abs_diff == 0)
			return (pos-start);
	 
	}
	return (-1);
}

int main(int argc, char* argv[])
{
	//int n = 7000000; 	
	int n = 4000000; 	
	int result, period;
	// Array to store Ulam Number 
	vector<int> arr; 
	clock_t start, end;
	double cpu_time_used;


	// Pre compute Ulam Number sequence 

	for(int option = 0; option <= 1; option++)
	{
		arr.clear();
		arr.push_back(1); 
		arr.push_back(2); 		
		start = clock();		
		ulam2(arr, n, option); 
		// Print nth Ulam number 
		result = *(arr.end()-1);
		period = period_diff(arr);
		std::cout << "last element = " << result << ' ' << arr.size() << " period = " << period << "\n";	
		end = clock();
		cpu_time_used = ((double) (end - start)) / CLOCKS_PER_SEC;
		cout << option << " option:  " << cpu_time_used  << " sec " << "\n";
	}

	end = clock();
	cpu_time_used = ((double) (end - start)) / CLOCKS_PER_SEC;
	cout << cpu_time_used  << " sec " << "\n";
}



