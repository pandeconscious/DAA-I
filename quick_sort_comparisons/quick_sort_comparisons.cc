//usage ./a.out <input_file>
//does quick sort by choosing pivot in three different ways
//assumes no repetition of elements
#include<iostream>
#include<vector>
#include<fstream>

using namespace std;

typedef vector<int>::iterator vii_t;

unsigned long num_comparison1 = 0;
unsigned long num_comparison2 = 0;
unsigned long num_comparison3 = 0;

void swap(vii_t i, vii_t j){
	int tmp = *i;
	*i = *j;
	*j = tmp;
}

//this is the code for least num of comparisions to find median of three nums
vii_t findMedian(vii_t left, vii_t middle, vii_t right){
	if(*left < *middle){
		if(*left > *right)
			return left;
		else if(*middle < *right)
			return middle;
	}
	else{
		if(*left < *right)
			return left;
		else if(*middle > *right)
			return middle;
	}
	return right;
}

void handleChoice(vii_t left, vii_t right, int choice){
	int numElements = right - left + 1;
	switch(choice){
		case 1: //the default case when the first element is the pivot
			num_comparison1 += numElements - 1;
			break;
		
		case 2: //the case when the last element is the pivot and hence to be swapped with the first element
			num_comparison2 += numElements - 1;
			swap(left, right);	
			break;
		
		case 3: //the case when the median of 3 is chose as the pivot and hence swapped
			num_comparison3 += numElements - 1;
			vii_t middle, median;
			if(numElements % 2 == 1) //odd
				middle = left + numElements/2;
			else //even
				middle = left + numElements/2 - 1;
			median = findMedian(left, middle, right); 
			swap(left, median);
			break;
		}
}

//most crucial code
vii_t myPartition(vii_t left, vii_t right){ //see DAA coursera notes for description
	vii_t i = left + 1, j = left + 1;
	while(j != right + 1){
		if(*j < *left){ //leftmost is pivot
			swap(i, j);
			++i;
		}
		++j;
	}
	swap(left, i-1);
	return i-1; //return the updated poistion of the pivot
}

void quickSort(vii_t left, vii_t right, int choice){
	if(left == right) //base case of recursion
		return;
	handleChoice(left, right, choice); //will swap elements appropriately
	vii_t pivot = myPartition(left, right);
	if(pivot == left) //one of the worst cases
		quickSort(left+1, right, choice);
	else if(pivot == right) //other wors case
		quickSort(left, right-1, choice);
	else{
		quickSort(left, pivot-1, choice);
		quickSort(pivot+1, right, choice);
	}
}

void printArr(vector<int>& arr){
	for(vii_t itr = arr.begin(); itr != arr.end(); ++itr)
		cout << *itr << " ";
	cout << endl;
}

int main(int argc, char* argv[]){
	vector<int> myArr1;
	int input;
	ifstream in(argv[1]);
	while(in >> input)
			myArr1.push_back(input);
	//cout << "Input array is:" << endl;
	//printArr(myArr1);
	vector<int>myArr2(myArr1);
	vector<int>myArr3(myArr1);
	//the three cases for quick sort chosen by 1, 2, 3 as last argument
	quickSort(myArr1.begin(), myArr1.end()-1, 1);
	//printArr(myArr1);
	cout << "Num of comparsions in case 1: " << num_comparison1 << endl;
	quickSort(myArr2.begin(), myArr2.end()-1, 2);
	//printArr(myArr2);
	cout << "Num of comparsions in case 2: " << num_comparison2 << endl;
	quickSort(myArr3.begin(), myArr3.end()-1, 3);
	//printArr(myArr3);
	cout << "Num of comparsions in case 3: " << num_comparison3 << endl;
	return 0;
}
