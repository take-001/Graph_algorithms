#include <iostream>
#include <vector>
#include <algorithm>
#include <utility>
#include <cstdlib>
#include <ctime>
#include <chrono>
#include <iomanip>
#include <map>

using namespace std;



void visit1(vector<bool>& R,int& M,vector<int>& psi, map<int,int>& psi_inv ,vector<vector<int>> A,int v) {
    R[v]=true;
    for(int i=0;i<A[v].size();i++) {
        if(R[A[v][i]]==false) {
            visit1(R,M,psi,psi_inv,A,A[v][i]);
        }
    }
    M=M+1;
    psi[v]=M;
    psi_inv[M]=v;

}

/*void visit2(vector<bool>& R,vector<int>&comp ,vector<vector<int>> A, int K, int v) {
    R[v]=true;
    for(int i=0;i<A[v].size();i++) {
        if(R[A[v][i]]==false) {
            visit2(R,comp,A,K,A[v][i]);
        }
    }

    comp[v]=K;
}*/

void visit2(vector<bool>& R,vector<int>&comp ,vector<vector<int>> A, int K, int v) {
    R[v]=true;
    for(int i=0;i<A.size();i++) {
        for(int j=0;j<A[i].size();j++) {
            if(R[i]==false && A[i][j]==v) {
                visit2(R,comp,A,K,i);
            }
        }
    }

    comp[v]=K;
}

void print_graph(vector<vector<int>>& A) {
    cout << "Graph Structure (Adjacency List Representation)" << endl;
    cout << "--------------------------------------------" << endl;
    for (int i = 0; i < A.size(); ++i) {
        cout << "Node " << setw(2) << i << " -> ";
        for (int j = 0; j < A[i].size(); ++j) {
            cout << setw(2) << A[i][j];
            if (j < A[i].size() - 1) cout << " -- ";
        }
        cout << endl;
    }
    cout << "--------------------------------------------" << endl;
}

void adjacency_gen(vector<vector<int>>& A, int N, int max_E,int seed) {
    int u;
    srand(seed);
    for(int i=0;i<N;i++) {
        for(int j=0;j<(rand() % (N - 1)) + 1;j++) {
            u=(rand() % (N-1)) + 1;
            if(i!=u && find(A[i].begin(),A[i].end(),u) == A[i].end()) {
                A[i].push_back(u);
            }
        }
    }
}





int main()
{
    int N=10;
    int M=-1;
    int K = -1;
    vector<bool> R(N,false);
    vector<bool> R_(N,false);
    vector<int> comp(N,0);
    vector<vector<int>> A(N);
    vector<int> psi(N); // 없어도 될 것 같은데...?
    map<int,int> psi_inv;
    adjacency_gen(A, N, 6, 3);

    for(int i=0;i<N;i++) {
        if(R[i]==false) {
            visit1(R,M, psi, psi_inv, A,i);
        }
    }

    for(int i=N-1; i>=0; i--) {
        if(R_[psi_inv[i]]==false) {
            K+=1;
            visit2(R_,comp,A,K,psi_inv[i]);
        }
    }
    print_graph(A);
    for(int i=0;i<psi_inv.size();i++) {
        cout<<comp[i]<<" ";
        cout<<"Node("<<i<<"): component "<<comp[i]<<endl;
    }






    return 0;
}
