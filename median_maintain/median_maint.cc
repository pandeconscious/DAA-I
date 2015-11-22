//Usage: ./a.out <file_name>

#include<fstream>
#include<queue>
#include<vector>
#include<iostream>

unsigned int median_sum = 0;

//this MyComparison class is sort of boilerplate - remember it
class MyComparison{
	private:
		bool max; //max = true then it acts as max_heap; if false then as min_heap
	public:
		MyComparison(const bool& max = true){//meaning default value is for max-heap
			this->max = max;
		}
		
		//rhs has higher priority than lhs
		bool operator()(const unsigned int& lhs, const unsigned int& rhs) const{
			if(max)
				return lhs < rhs;
			else
				return lhs > rhs;
		}
};

void insert_correctly(const int& int_read, std::priority_queue<unsigned int, std::vector<unsigned int>, MyComparison>& max_heap, std::priority_queue<unsigned int, std::vector<unsigned int>, MyComparison>& min_heap){
	//handle boundary cases first 
	
	//both heaps are empty
	if(max_heap.empty() && min_heap.empty())
		max_heap.push(int_read);
	
	//only min_hap empty
	else if(min_heap.empty() && max_heap.size() == 1){
		if(int_read > max_heap.top())
			min_heap.push(int_read);
		else{
			max_heap.push(int_read);
			min_heap.push(max_heap.top());
			max_heap.pop();
		}
	}
	
	//note max_heap being empty and min_heap being non empty doesn't arise because the first element always goes to max heap and one element may be moved out of a heap only if one element has been added to the heap
	
	else{ //both are non-empty
		if(int_read < min_heap.top()){
			max_heap.push(int_read);
			if(max_heap.size() == min_heap.size() + 2){
				min_heap.push(max_heap.top());
				max_heap.pop();
			}
		}
		else{
			min_heap.push(int_read);
			if(min_heap.size() == max_heap.size() + 2){
				max_heap.push(min_heap.top());
				min_heap.pop();
			}
		}
	}
}

unsigned int get_median(std::priority_queue<unsigned int, std::vector<unsigned int>, MyComparison>& max_heap, std::priority_queue<unsigned int, std::vector<unsigned int>, MyComparison>& min_heap){
	if(max_heap.empty() && min_heap.empty()){
		std::cout << "both heaps are empty" << std::endl;
		exit(1);
	}
	else if(min_heap.empty() && max_heap.size() == 1){
		std::cout << max_heap.top() << std::endl;
		return max_heap.top();
	}
	else{//max_heap empty and min_heap empty case not possible due to my implemetation so we go to both non empty case
		if(max_heap.size() == min_heap.size()){ //even no of elements
			std::cout << max_heap.top() << std::endl;
			return max_heap.top(); //as required by assignment
		}
		else if(max_heap.size() == min_heap.size() + 1){
			std::cout << max_heap.top() << std::endl;
			return max_heap.top();
		}
		else{
			std::cout << min_heap.top() << std::endl;
			return min_heap.top();
		}
	}
}

int main(int argc, char* argv[]){
	std::ifstream in(argv[1]);
	unsigned int int_read;
	std::priority_queue<unsigned int, std::vector<unsigned int>, MyComparison> min_heap(MyComparison(false));
	std::priority_queue<unsigned int, std::vector<unsigned int>, MyComparison> max_heap;
	while(in >> int_read){
		insert_correctly(int_read, max_heap, min_heap);
		median_sum += get_median(max_heap, min_heap);
	}
	std::cout << "answer is " << median_sum % 10000<< std::endl;
}
