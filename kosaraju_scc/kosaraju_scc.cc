//usage: ./a.out <rev_graph_file> <graph_file>

#include<iostream>
#include<fstream>
#include<sstream> //istringstream() function - basically strings can behave as streams
#include<vector>
#include<list>
#include<string>
#include<cstring>
#include<cstdlib>
#include<cstdio>
#include<algorithm>

unsigned long num_lines;//no. of lines in the code
int t; //for finishing time
int s; //for leaders, current source vertex
const int kNumOfNodes = 875714; //nodes labeled 1 to 875714
//const int kNumOfNodes = 9;
int* node2fin_time = new int[kNumOfNodes];

class DirGraph{
	private:
		std::list<int>** adj_list_;
		int* fin_time_;
		int* leader_;
		bool* explored_;

		inline void Add2List(int node1, int node2);
		inline void PrintList(std::list<int>* ptr2list);
	
	public:
		DirGraph(char* file_name);
		DirGraph(char* file_name, bool use_global_map);
		~DirGraph();
		void DisplayGraph();
		void ShowLeaders();
		void DfsLoop();
		void DFS(int i);
		void StoreFintimes();
		void ShowSizeOfSCC();
};


void DirGraph::ShowLeaders(){
	for(int i = 0; i < kNumOfNodes; ++i)
		printf("%d ", leader_[i]);
}

inline void DirGraph::Add2List(int node1, int node2){
	node1--;//need this step for indexing
	if(adj_list_[node1] == NULL)
		adj_list_[node1] = new std::list<int>;
	adj_list_[node1]->push_back(node2);
}

DirGraph::DirGraph(char* file_name){
	adj_list_ = new std::list<int>*[kNumOfNodes];
	fin_time_ = new int[kNumOfNodes];
	leader_ = new int[kNumOfNodes];
	explored_ = new bool[kNumOfNodes];

	for(int i = 0; i < kNumOfNodes; ++i){
		adj_list_[i] = NULL;
		fin_time_[i] = 0;
		leader_[i] = 0;
		explored_[i] = false;
	}
	
	FILE* fptr = fopen(file_name, "r");
	int node1, node2;
	for(unsigned long i = num_lines; i >=1; --i){
		fscanf(fptr, "%d", &node1);
		fscanf(fptr, "%d", &node2);
		//std::cout << "read " << node1 << ", " << node2 << std::endl;
		Add2List(node1, node2);
	}
	fclose(fptr);	
}

DirGraph::DirGraph(char* file_name, bool use_global_map){
	adj_list_ = new std::list<int>*[kNumOfNodes];
	fin_time_ = new int[kNumOfNodes];
	leader_ = new int[kNumOfNodes];
	explored_ = new bool[kNumOfNodes];

	for(int i = 0; i < kNumOfNodes; ++i){
		adj_list_[i] = NULL;
		fin_time_[i] = 0;
		leader_[i] = 0;
		explored_[i] = false;
	}
	
	FILE* fptr = fopen(file_name, "r");
	int node1, node2;
	for(unsigned long i = num_lines; i >=1; --i){
		fscanf(fptr, "%d", &node1);
		fscanf(fptr, "%d", &node2);
		//std::cout << "read " << node1 << ", " << node2 << std::endl;

		int modified_node1 = node2fin_time[node1-1];
		int modified_node2 = node2fin_time[node2-1];
		Add2List(modified_node1, modified_node2);
	}
	fclose(fptr);	

}

DirGraph::~DirGraph(){
	for(int i = 0; i < kNumOfNodes; ++i){
		if(adj_list_[i] != NULL)
			delete adj_list_[i];
	}

	delete adj_list_;
	delete fin_time_;
	delete leader_;
	delete explored_;
}

inline void DirGraph::PrintList(std::list<int>* ptr2list){
	for(std::list<int>::iterator itr = ptr2list->begin(); itr != ptr2list->end(); ++itr){
		printf("%d ", *itr);
	}
}

void DirGraph::DisplayGraph(){	
	for(int i = 0; i < kNumOfNodes; ++i){
		printf("%d :", i+1);
		if(adj_list_[i] != NULL)
			PrintList(adj_list_[i]);
		printf(" \n");
	}
}

void DirGraph::DFS(int i){
	explored_[i-1] = true;
	leader_[i-1] = s;
	if(adj_list_[i-1] != NULL){
		for(std::list<int>::iterator itr = adj_list_[i-1]->begin(); itr != adj_list_[i-1]->end(); ++itr){
			if(explored_[(*itr)-1] == false)
				DFS(*itr);
		}
		++t;
		fin_time_[i-1] = t;
	}
}

void DirGraph::DfsLoop(){
	t = 0;
	s = 0;
	for(int i = kNumOfNodes; i >=1; --i){
		if(explored_[i-1] == false){
			s = i;
			DFS(i);	
		}
	}
	
	//for(int i = 0; i < kNumOfNodes; ++i)
		//printf("%d ", fin_time_[i]);
		
}

void DirGraph::StoreFintimes(){
	for(int i = 0; i < kNumOfNodes; ++i){
		node2fin_time[i] = fin_time_[i];
	}
}

void DirGraph::ShowSizeOfSCC(){
	std::vector<int> arr_size_scc(kNumOfNodes);
	for(int i = 0; i < kNumOfNodes; ++i)
		arr_size_scc[i] = 0;
	
	int index;

	for(int i = 0; i <kNumOfNodes; ++i){
		index = leader_[i];
		arr_size_scc[index-1]++;
	}

	std::cout << std::endl;
	void printArr(std::vector<int>& arr);
	printArr(arr_size_scc);
	std::cout << std::endl;
	std::sort(arr_size_scc.begin(), arr_size_scc.end());
	printArr(arr_size_scc);
}

void printArr(std::vector<int>& arr){
	for(std::vector<int>::iterator itr = arr.begin(); itr != arr.end(); ++itr)
		std::cout << *itr << " ";
	std::cout << '\n';
}

unsigned NumLines(const char* file_name){
	unsigned long num_lines;
	char* command = new char[strlen(file_name) + strlen("wc -l ") + strlen(" > length.txt") + 1];
	strcpy(command, "wc -l ");
	strcat(command, file_name);
	strcat(command, " > length.txt");
	system(command);
	std::ifstream in("length.txt");
	in >> num_lines;
	in.close();
	return num_lines;
}

int main(int argc, char* argv[]){
	num_lines = NumLines(argv[1]);
	//reverse graph processing
	{
	DirGraph rev_graph(argv[1]);
	//rev_graph.DisplayGraph();
	rev_graph.DfsLoop();
	rev_graph.StoreFintimes();
	}

	//graph processing
	DirGraph my_graph(argv[2], true);//creating graphs with node no.s changed to finishing times
	//my_graph.DisplayGraph();
	my_graph.DfsLoop();
	//my_graph.ShowLeaders();
	my_graph.ShowSizeOfSCC();

	return 0;
}
