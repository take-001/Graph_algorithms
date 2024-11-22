#include <iostream>
#include <vector>
#include <algorithm>
#include <utility>
#include <cstdlib>
#include <ctime>
#include <chrono>

using namespace std;
using namespace std::chrono;


typedef pair<int,int> Edge;


void Complete_Graph_generator(vector<vector<Edge>>& graph, int V) {
    srand(time(0));

    graph.assign(V, vector<Edge>());

    for (int u = 0; u < V; u++) {
        for (int v = 0; v < u; v++) {
            int weight = rand() % 10 + 1;
            graph[u].push_back({weight, v});
            graph[v].push_back({weight, u});
        }
    }
}

Edge delete_min(vector<Edge>& heap) {
    pop_heap(heap.begin(), heap.end(), greater<Edge>());
    Edge min_edge = heap.back();
    heap.pop_back();

    return min_edge;
}


void insert(vector<Edge>& heap ,int w, int weight) {
    heap.push_back({weight, w});
    push_heap(heap.begin(), heap.end(), greater<Edge>());

}


void decrease_key(vector<Edge>& heap, int w, int weight) {
    for(int i=0; i<heap.size(); i++) {

        if(heap[i].second ==w) {
            heap[i].first = weight;


            while(i>0 && heap[i].first<heap[(i-1)/2].first) {
                swap(heap[i], heap[(i-1)/2]);
                i = (i-1)/2;
            }

            break;
        }
    }

}
void print_graph(vector<vector<Edge>>& graph) {

    for(int i=0; i<graph.size(); i++) {
        for(int j=0; j<graph[i].size(); j++) {
            cout<<"("<<i<<","<<graph[i][j].second<<"):"<<graph[i][j].first<<endl;

        }
    }

}


void print_mst(vector<Edge> tree, vector<int> cost_, int option,int exp_mode ) {
    if(exp_mode==0 && option==0) {
        cout<<"[Prim with heap]"<<endl;
        int cost =0;
        for(int i=1; i<tree.size(); i++) {
            cout<<"Edge("<<i<< "): "<<"("<<tree[i].first<<","<<tree[i].second<<")"<<endl;
            cost+=cost_[i];
        }
        cout<<"Total Edge Cost: "<<cost<<endl;
    }
    else if(exp_mode==0&&option==1) {
        cout<<"[Prim with array]"<<endl;
        int cost =0;
        for(int i=1; i<tree.size(); i++) {
            cout<<"Edge("<<i<< "): "<<"("<<tree[i].first<<","<<tree[i].second<<")"<<endl;
            cost+=cost_[i];
        }
        cout<<"Total Edge Cost: "<<cost<<endl;
    }



}

Edge find_pred(vector<vector<Edge>>& graph, Edge e, vector<bool> T) {

    for (int i = 0; i < graph[e.second].size(); i++) {
        if (graph[e.second][i].first == e.first && T[graph[e.second][i].second]) {

            return graph[e.second][i];
        }
    }
    return {-100,-1};
}

/**************************************PRIM with HEAP***************************************/
vector<int> prim_mst_heap(vector<vector<Edge>>& graph, int V, bool exp_mode) {
    vector<int> key(V, INT_MAX);
    vector<Edge> heap;
    vector<pair<int,int>> tree;
    vector<int> cost;
    vector<bool> T(V,false);
    vector<int> operation (4,0);
    bool heap_empty = false;
    int prev_v=0;

    int v = 0;
    heap.push_back({0, 0});

    T[v]=true;
    while(!all_of(T.begin(), T.end(),[](bool value){return value;})&& !heap.empty()) {
        for (auto& e : graph[v]) {
            if(!T[e.second]&&e.first < key[e.second]&& key[e.second] != INT_MAX) {
                key[e.second] = e.first;
                decrease_key(heap, e.second, e.first);
                operation[2]+=1;

            }
            if(!T[e.second]&& key[e.second] == INT_MAX) {
                key[e.second] = e.first;
                insert(heap, e.second, e.first);
                operation[3]+=1;
            }
        }
        Edge min_edge = delete_min(heap);
        operation[0]+=1;
        operation[1]+=1;



        v=min_edge.second;
        T[v]=true;
        auto pred = find_pred(graph, min_edge,T);

        tree.push_back({pred.second,v});
        cost.push_back(min_edge.first);
    }
    if(!exp_mode) {print_mst(tree, cost,0,0);}

    return operation;
}

/**************************************PRIM with Array***************************************/
void prim_mst_array(vector<vector<Edge>>& graph, int V, bool exp_mode) {
    vector<int> key(V, INT_MAX);
    vector<Edge> array;
    vector<pair<int,int>> tree;
    vector<int> cost;
    vector<bool> T(V,false);

    int prev_v =0;

    bool array_empty = false;

    int v = 0;
    T[v]=true;
    array.push_back({0, 0});
    while(!all_of(T.begin(), T.end(),[](bool value){return value;})&& !array.empty()) {
        for (auto& e : graph[v]) {
            if(!T[e.second]&&e.first < key[e.second] && key[e.second] != INT_MAX ) {
                for(int i=0; i<array.size(); i++) {
                    if(array[i].second == e.second) {
                        array[i].first = e.first;
                        key[e.second] = e.first;


                    }
                }



            }
            if(!T[e.second]&&key[e.second] == INT_MAX) {
                array.push_back({e.first, e.second});
                key[e.second] = e.first;
            }

        }

        auto min_pt = min_element(array.begin(), array.end(), [](Edge e1, Edge e2){return e1.first < e2.first;});
        Edge min_edge = *min_pt;
        array.erase(min_pt);


        v=min_edge.second;
        T[v]=true;
        auto pred = find_pred(graph, min_edge,T);
        tree.push_back({pred.second,v});
        cost.push_back(min_edge.first);





    }

    if(!exp_mode) {print_mst(tree, cost,1,0);}

}


vector<float> exp_heap(int iter, int V) {
    vector<vector<Edge>> graph(V);

    vector<int> res(4);

    int FindMin=0;
    int DeleteMin=0;
    int DecreaseKey=0;
    int insert =0;

    for(int i=iter; i>0; i--) {
        Complete_Graph_generator(graph,V);
        res=prim_mst_heap(graph, V, true);
        FindMin+=res[0];
        DeleteMin+=res[1];
        DecreaseKey+=res[2];
        insert +=res[3];
    }

    vector<float> exp_res={(float)FindMin/iter,(float)DeleteMin/iter, (float)DecreaseKey/iter, (float)insert/iter};
    return exp_res;
}

vector<float> exp_heap_array(int iter, int V) {
    vector<vector<Edge>> graph(V);
    vector<float> running_time(2,0);

    for(int i=iter; i>0; i--) {
        Complete_Graph_generator(graph,V);
        auto start_heap = high_resolution_clock::now();
        prim_mst_heap(graph, V, true);
        auto end_heap = high_resolution_clock::now();

        auto start_array = high_resolution_clock::now();
        prim_mst_array(graph, V, true);
        auto end_array = high_resolution_clock::now();

        auto duration_heap = duration_cast<microseconds>(end_heap - start_heap);
        auto duration_array = duration_cast<microseconds>(end_array - start_array);

        running_time[0] += duration_heap.count();
        running_time[1] += duration_array.count();
    }
    running_time[0]/=iter;
    running_time[1]/=iter;

    cout <<"Avg runtime(heap): "<< running_time[0] << endl;
    cout <<"Avg runtime(array): "<< running_time[1] << endl;

    return running_time;
}

int main() {
    int V = 2000;
    vector<float> exp_res_heap(4);
    vector<string> operation = {"FindMin", "DeleteMin", "DecreaseKey", "Insert"};
    vector<int> res(4);
    vector<vector<Edge>> graph(V);
    Complete_Graph_generator(graph,V);


    //MST with Heap🔥 example result
    res= prim_mst_heap(graph,V,false);
    cout<<""<<endl;
    //MST with Array😓 example result
    prim_mst_array(graph,V,false);

    //count average of each operations on 100 different instances
    exp_res_heap=exp_heap(100,V);
    cout<<""<<endl;
    for(int i=0; i<exp_res_heap.size(); i++) {
        cout<<"Avg_"<<operation[i]<<": "<<exp_res_heap[i]<<" ";
    }
    cout<<""<<endl;
    exp_heap_array(100,V);





    return 0;
}
