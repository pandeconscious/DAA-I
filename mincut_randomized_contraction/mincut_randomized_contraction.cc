//usage ./a.out <input_file>
//code for randomized mincut finding algorithm - karger's algo 

#include<iostream>
#include<string>
#include<list>
#include<fstream>
#include<cassert>
#include<vector>
#include<cstdlib> //rand()
#include<ctime> //time()
#include<algorithm>

const unsigned long kNumOfCalls = 4000000; //no of times the randomized contraction subroutine is called

typedef std::vector<int>::iterator ViiT;
typedef std::string::iterator SiT;
typedef std::list<int>::iterator LiiT;

//print a vector of int
void printVec(std::vector<int>& arr){
	for(ViiT itr = arr.begin(); itr != arr.end(); ++itr)
		std::cout << *itr << " ";
	std::cout << std::endl;
}

//print a list of int
void printList(std::list<int>& arr){
	for(LiiT itr = arr.begin(); itr != arr.end(); ++itr)
		std::cout << *itr << " ";
	std::cout << std::endl;
}


struct HeadPtr{//this type points to the head of the list and the data it contains is the vertex number
	int vertex_;
	std::list<int>* ptr_adj_list_;
};

typedef std::list<HeadPtr>::iterator LhiT;

class MyGraph{
	private:
		int num_vertices_;
		std::list<HeadPtr> adj_list_;//adjacency list
		void RandomSelect(int& smaller_vertex, int& larger_vertex); //two select 2 vertices for merging
		void ReplaceVertex(int small, int large);// replace all large vertices with small vertices 
	public:
		MyGraph(char* file_name);
		~MyGraph();
		void DisplayGraph();
		unsigned long NumEdges();
		unsigned long RandomContraction();
		LhiT FindVertex(int vert);//return iterator to the desired header vertex
};

MyGraph::MyGraph(char* file_name){
	std::ifstream in(file_name);
	std::string s;
	num_vertices_ = 0;
	std::vector<int> list_count_adj_vert;

	//count num of vertices and num of adjacent vertices to each vertex
	while(getline(in, s)){
		++num_vertices_;
		int count_adj_vert = 0;
		for(SiT it = s.begin(); it != s.end(); ++it){
			if(*it == '\t')
				count_adj_vert++;
		}
		list_count_adj_vert.push_back(--count_adj_vert); //the input file has extra \t at end
	}
	//std::cout << num_vertices_ << std::endl;
	//printVec(list_count_adj_vert);
	
	
	//read file again to store graph
	std::ifstream in2(file_name);
	for(int i=1; i <= num_vertices_; ++i){
		int temp;
		in2 >> temp;
		HeadPtr head = {temp, NULL};
		std::list<int>* ptr_list = new std::list<int>;
		for(int j = 1; j <= list_count_adj_vert[i-1]; ++j){
			in2 >> temp;
			(*ptr_list).push_back(temp);
		}
		head.ptr_adj_list_ = ptr_list;
		adj_list_.push_back(head);
	}
}

MyGraph::~MyGraph(){
	for(LhiT it = adj_list_.begin(); it != adj_list_.end(); ++it)
		delete (*it).ptr_adj_list_;
}

void MyGraph::DisplayGraph(){
	std::cout << "The graph is: " << std::endl;
	for(LhiT it = adj_list_.begin(); it != adj_list_.end(); ++it){
		std::cout << (*it).vertex_ << " ";
		printList(*((*it).ptr_adj_list_));	
	}
}

unsigned long MyGraph::NumEdges(){
	unsigned long two_times_num_edges = 0;
	for(LhiT it = adj_list_.begin(); it != adj_list_.end(); ++it)
		two_times_num_edges += (*it).ptr_adj_list_->size();
	return two_times_num_edges/2;
}

void MyGraph::RandomSelect(int& smaller_vertex, int& larger_vertex){
	unsigned long twice_num_edges = 2*NumEdges();
	unsigned long edge_index = 1 + (rand() % twice_num_edges);
	int head_vertex, other_vertex;//the two chosen vertices one is always head 	
		
	for(LhiT it = adj_list_.begin(); it != adj_list_.end(); ++it){
		if((*it).ptr_adj_list_->size() >= edge_index){
			head_vertex = (*it).vertex_;
			LiiT itr = (*it).ptr_adj_list_->begin();
			std::advance(itr, edge_index-1); //itr points to the edge_index(th) element
			other_vertex = *itr;
			break;
		}
		else
			edge_index -= (*it).ptr_adj_list_->size();	
	}
	
	if(head_vertex < other_vertex){
		smaller_vertex = head_vertex;
		larger_vertex = other_vertex;
	}
	else{
		smaller_vertex = other_vertex;
		larger_vertex = head_vertex;
	}
		
}

LhiT MyGraph::FindVertex(int vert){
	for(LhiT it = adj_list_.begin(); it != adj_list_.end(); ++it){
		if((*it).vertex_ == vert)
			return it;
	}
}

void MyGraph::ReplaceVertex(int small, int large){
	for(LhiT it = adj_list_.begin(); it != adj_list_.end(); ++it){
		std::list<int>* ptr_curr_list = (*it).ptr_adj_list_;
		std::replace((*ptr_curr_list).begin(), (*ptr_curr_list).end(), large, small);
	}
}

unsigned long MyGraph::RandomContraction(){
	while(num_vertices_ > 2){
		int smaller_vertex, larger_vertex; //two vertices to be chosen for merging - smaller larger is chosen by the label
		RandomSelect(smaller_vertex, larger_vertex);	
		LhiT itr_smaller_vertex = FindVertex(smaller_vertex);
		LhiT itr_larger_vertex = FindVertex(larger_vertex);
		std::list<int> list_2_append(*((*itr_larger_vertex).ptr_adj_list_));
		adj_list_.erase(itr_larger_vertex);
		
		//merge (splice actually) the two lists to the smaller list
		std::list<int>* ptr_target_list = (*itr_smaller_vertex).ptr_adj_list_;
		(*ptr_target_list).splice((*ptr_target_list).end(), list_2_append);

		//remove all the elements in the list that have values of smaller_vertex and larger_vertex -- equivalent to self-loop removal step
		(*ptr_target_list).remove(smaller_vertex);
		(*ptr_target_list).remove(larger_vertex);
	
		//for rest of the graph replace all the occurrences of the larger vertex by the smaller vertex -- we keep only the smaller vertex name for consistency after merging the two vertices
		ReplaceVertex(smaller_vertex, larger_vertex);	
		--num_vertices_;
	}
	return NumEdges();
}

int main(int argc, char* argv[]){
	srand(time(NULL));
	assert(argc == 2);
	MyGraph my_graph(argv[1]);
	//my_graph.DisplayGraph();
	//std::cout << "Num of edges " << my_graph.NumEdges() << std::endl;
	//std::cout << "Size of mincut: " << my_graph.RandomContraction() << std::endl;
	unsigned long mincut = my_graph.NumEdges();
	unsigned long current_cut; 
	for (int i = 1; i <= kNumOfCalls; ++i){
		current_cut = my_graph.RandomContraction();
		if(current_cut < mincut)
			mincut = current_cut;
	}
	std::cout << "Possible mincut is : " << mincut << std::endl;
	return 0;
}
