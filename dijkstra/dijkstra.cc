//this is naive dijkstra implementation O(mn) without using prior queue

#include<fstream>
#include<list>
#include<string>
#include<sstream>
#include<iostream>
#include<cassert>

const int kNumNodes = 200;
//const int kNumNodes = 4;
const int kInfinity = 1000000;

struct Edge{
	int target_node;
	int weight;
	bool processed;
};

class Graph{
	private:
		std::list<Edge>* adj_list[kNumNodes+1];
		std::list<int> processed_nodes;
		int shortest_path[kNumNodes+1];
		void PrintList(std::list<Edge>* ptr_list);
		void FindShortest(int index, int& curr_node, int& curr_dist);
		void MakeProcessed(int index);

	public:
		Graph(const char* file_name);
		~Graph();
		void DisplayGraph();
		void RunDijkstra();
		void DisplayShortestPaths();
};

Graph::Graph(const char* file_name){
	//initializations
	for(int i = 0; i <= kNumNodes; ++i){
		adj_list[i] = NULL;
		shortest_path[i] = kInfinity;
	}
	shortest_path[1] = 0;
	processed_nodes.push_back(1);	

	//read graph
	std::ifstream in(file_name);
	std::string curr_line, curr_pair;
	int source_node, target_node, wt;
	while(getline(in, curr_line)){
		std::stringstream sin(curr_line);
		//std::cout << curr_line << std::endl;
		sin >> source_node;
		//std::cout << source_node << std::endl;
		while(sin >> curr_pair){
			//std::cout << curr_pair << std::endl;
			//exit(1);
			sscanf(curr_pair.c_str(), "%d,%d", &target_node, &wt);
			if(adj_list[source_node] == NULL)
				adj_list[source_node] = new std::list<Edge>;
			
			if(target_node == 1){
				Edge temp_edge = {target_node, wt, true};
				adj_list[source_node]->push_back(temp_edge);
			}
			else{
				Edge temp_edge = {target_node, wt, false};
				adj_list[source_node]->push_back(temp_edge);
			}
		}
		
	}
	in.close();
}

Graph::~Graph(){
	for(int i = 0; i <= kNumNodes; ++i){
		if(adj_list[i] != NULL)
			delete adj_list[i];
	}
}


void Graph::PrintList(std::list<Edge>* ptr_list){
	for(std::list<Edge>::iterator itr = (*ptr_list).begin(); itr != (*ptr_list).end(); ++itr){
		std::cout << itr->target_node << "," << itr->weight << " ";
	}
}

void Graph::DisplayGraph(){
	for(int i = 1; i <= kNumNodes; ++i){
		std::cout << "\n" << i << " : "; 
		if(adj_list[i] != NULL)
			PrintList(adj_list[i]);		
	}
}

void Graph::MakeProcessed(int index){
	for(int i = 1; i <= kNumNodes; ++i){
		if(adj_list[i] != NULL){
			for(std::list<Edge>::iterator itr = adj_list[i]->begin(); itr != adj_list[i]->end(); ++itr){
				if(itr->target_node == index && itr->processed == false)
					itr->processed = true;
			}
		}
	}

}

void Graph::FindShortest(int index, int& curr_node, int& curr_dist){
	if(adj_list[index] == NULL){
		curr_node = -1;
		curr_dist = kInfinity;
		return ;
	}
	
	for(std::list<Edge>::iterator itr = adj_list[index]->begin(); itr != adj_list[index]->end(); ++itr){
		if(itr->processed == false && shortest_path[index] + itr->weight < curr_dist){
			curr_dist = shortest_path[index] + itr->weight;
			curr_node = itr->target_node;
		}
	}
}

void Graph::RunDijkstra(){
	while(processed_nodes.size() < kNumNodes){
		int curr_potential_node = -1, curr_potential_dist = kInfinity, curr_closest_node = -1, curr_shortest_distance = kInfinity;
		for(std::list<int>::iterator itr = processed_nodes.begin(); itr != processed_nodes.end(); ++itr){
			FindShortest((*itr), curr_potential_node, curr_potential_dist);
			if(curr_potential_dist < curr_shortest_distance){
				curr_shortest_distance = curr_potential_dist;
				curr_closest_node = curr_potential_node;
			}
		}

		assert(curr_closest_node > 0);
		
		/*std::cout << "hi" << std::endl;
		std::cout << curr_closest_node << std::endl;
		std::cout << curr_shortest_distance << std::endl;*/
		shortest_path[curr_closest_node] = curr_shortest_distance; 
		processed_nodes.push_back(curr_closest_node);
		MakeProcessed(curr_closest_node);
	}
}

void Graph::DisplayShortestPaths(){
	for(int i = 1; i <= kNumNodes; ++i){
		std::cout << "\n" << i << " at " << shortest_path[i];
	}
}

int main(int argc, char* argv[]){
	Graph my_graph(argv[1]);
	//my_graph.DisplayGraph();
	my_graph.RunDijkstra();
	my_graph.DisplayShortestPaths();
}
