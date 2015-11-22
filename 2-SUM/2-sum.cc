//usage ./a.out <input_file>

//solved but this is very slow - took 3 hrs; think of better solution

#include<unordered_set> //it is the hashtable of only keys (values are keys themselves)
#include<iostream>
#include<vector>

unsigned int count = 0;

int main(int argc, char* argv[]){
	long long curr_value, curr_first, curr_second;
	std::unordered_set<long long> my_table;
	//std::unordered_set<long long> my_table_of_t;
	std::vector<long long> just_list;
	FILE *fptr = fopen(argv[1], "r");
	
	while(fscanf(fptr, "%lli", &curr_value) != EOF){
		my_table.insert(curr_value);
		just_list.push_back(curr_value);	
	}

	//std::cout << *std::min_element(just_list.begin(), just_list.end()) << ", " << *std::max_element(just_list.begin(), just_list.end()) << std::endl;

	for(int t = -10000; t <= 10000; ++t){
		for(auto itr = just_list.begin(); itr != just_list.end(); ++itr){
			curr_first = *itr;
			curr_second = t - curr_first;
			if(curr_first != curr_second && my_table.find(curr_second) != my_table.end()){
				count++;
				std::cout << t << std::endl;
				break;
			}
		}
	}

	
	/*for(auto itr = just_list.begin(); itr != just_list.end(); ++itr){
		for(int t = -10000; t <= 10000; ++t){
			if(my_table_of_t.find(t) == my_table_of_t.end()){
				curr_first = *itr;
				curr_second = t - curr_first;
				if(curr_first != curr_second && my_table.find(curr_second) != my_table.end()){
					count++;
					std::cout << t << " " << curr_first << " " << curr_second << std::endl;
					my_table_of_t.insert(t);
				}
			}
		}
	}*/

	std::cout << "The count is " << count << std::endl;
	return 0;
}
