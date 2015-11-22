//code to count no. of inversions in an array - uses merge sort
#include<iostream>
#include<vector>

using namespace std;
unsigned long countInv = 0;
typedef vector<int>::iterator vii_t;

//merge two sorted arrays
void mergeStep(vii_t itr_l, vii_t mid_itr, vii_t itr_r){
	vector<int> leftArr (itr_l, mid_itr+1); //open interval on the right boundary
	vector<int> rightArr (mid_itr+1, itr_r+1);
	vii_t i = leftArr.begin();
	vii_t j = rightArr.begin();
	
	for(vii_t itr = itr_l; itr <= itr_r; itr++){
		
		if(i != leftArr.end() && j != rightArr.end()){
			if(*i <= *j){//if smaller element on the current index of left array
				*itr = *i;
				++i;
			}
			else{//if smaller element on the current index of right array
				*itr = *j;
				countInv += leftArr.end() - i; //key step for counting inversions --- increment count of inversions by the no. of elements to the right of the current index in the left array if the element at current index in the right array is smaller than that on the left array  
				++j;
			}
		}

		else if(i == leftArr.end()){
			*itr = *j;
			++j;	
		}

		else{//if j == rightArr.end()
			*itr = *i;
			++i;

		}
	}
}

void mergeAndCountInv(vii_t itr_l, vii_t itr_r){
	if(itr_l == itr_r)
		return;
	else{
		vii_t mid_itr = itr_l + (itr_r - itr_l)/2;
		mergeAndCountInv(itr_l, mid_itr);
		mergeAndCountInv(mid_itr + 1, itr_r);
		mergeStep(itr_l, mid_itr, itr_r);
	}
}

void printArr(vector<int>& arr){
	cout << "\n" << "Sorted array by Merge Sort:" << endl;
	for(vii_t itr = arr.begin(); itr != arr.end(); ++itr)
		cout << *itr << " ";
}


//wrapper function
void countInversions(vector<int>& arr){
	mergeAndCountInv(arr.begin(), arr.end()-1);
	//printArr(arr);
	cout << "Num of inversions: " << countInv << endl;
}

int main(){
	vector<int> inArr;
	int inVal;
	//store values from input
	while(cin >> inVal){
		inArr.push_back(inVal);	
	}
	//cout << inArr.end() - inArr.begin() << endl;
	//exit(0);
	countInversions(inArr);
	return 0;
}
