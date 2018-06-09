#include <iostream>
#include<map>
#include<set>
#include<stdlib.h>
#include<stack>
#include<list>
#include<fstream>
using namespace std;

int n; // số đỉnh
int m; // số cung
int a; // điểm xuất phát của robot A
int b; // điểm xuất phát của robot B
int c; // điểm đích của robot A
int d; // điểm đích của robot B
int r; // khoảng cách đề cho

struct Arc
{
    int nod;
    int w;
};
set<int> V; // tập đỉnh
map<int,set<Arc *> > A; // ánh xạ từ 1 đỉnh sang cấu trúc Arc
map<int,int> dA; // Khoảng cách
map<int,int> pA; // Nút cha
set <int> SA; // tập đỉnh trừ đỉnh xuất phát
// Tương tự với Robot B
map<int,set<Arc *> > B; // ánh xạ từ 1 đỉnh sang cấu trúc Arc
map<int,int> dB; // Khoảng cách
map<int,int> pB; // Nút cha
set <int> SB; // tập đỉnh trừ đỉnh xuất phát
// Trung gian
map<int,set<Arc *> > C; // ánh xạ từ 1 đỉnh sang cấu trúc Arc
map<int,int> dC; // Khoảng cách
map<int,int> pC; // Nút cha
set <int> SC; // tập đỉnh trừ đỉnh xuất phát

void readFile(char *fileName)
{
    ifstream fi (fileName);
    fi >> n;
    fi >> m;
    int x,y,z;
    for(int i =0 ; i < m ; i++)
    {
        fi >> x;
        fi >> y;
        fi >> z;
        V.insert(x);
        V.insert(y);
        Arc *a1;
        a1= (Arc*)malloc(sizeof(Arc));
        a1->nod= y;
        a1->w = z;
        A[x].insert(a1);
        Arc *a2;
        a2= (Arc*)malloc(sizeof(Arc));
        a2->nod= x;
        a2->w = z;
        A[y].insert(a2);

        Arc *b1;
        b1= (Arc*)malloc(sizeof(Arc));
        b1->nod= y;
        b1->w = z;
        B[x].insert(b1);
        Arc *b2;
        b2= (Arc*)malloc(sizeof(Arc));
        b2->nod= x;
        b2->w = z;
        B[y].insert(b2);

        Arc *c1;
        c1= (Arc*)malloc(sizeof(Arc));
        c1->nod= y;
        c1->w = z;
        C[x].insert(c1);
        Arc *c2;
        c2= (Arc*)malloc(sizeof(Arc));
        c2->nod= x;
        c2->w = z;
        C[y].insert(c2);

    }
    fi >> a;
    fi >> b;
    fi >> c;
    fi >> d;
    fi >> r;
}

//Tìm chỉ số min
int findMin(map<int,set<Arc *> > &X,set <int> &S,map<int,int> &d)
{
    int min = 31011998;
    int v_min = -1;
    for(set<int> :: iterator p = S.begin();p != S.end();p++)
    {
        int v= *p;
        if(d[v]<min)
        {
            min = d[v];
            v_min=v;
        }
    }
    return v_min;
}
// Dịkstra đỉnh Robot
void dijkstra(int s,map<int,set<Arc *> > &X,set <int> &S,map<int,int> &d,map<int,int> &p)
{
    for(set<int> :: iterator pi = V.begin(); pi!= V.end(); pi++)
    {
        int x = *pi;
        d[x] = 31011998;
    }
    d[s] = 0;
    p[s] = 0;
    for(set<Arc *> :: iterator ps = X[s].begin(); ps != X[s].end();ps++)
    {
        Arc * a = *ps;
        int x= a->nod;
        int w= a->w;
        d[x]= w;
        p[x]= s;
    }
    for(set<int> :: iterator pi= V.begin(); pi!= V.end();pi++)
    {
        int v = *pi;
        if(v != s)
            S.insert(v);
    }
    while(S.size() > 0)
    {
        int v = findMin(X,S,d);
        S.erase(v);
        for(set<Arc *> :: iterator pv = X[v].begin(); pv!= X[v].end();pv++)
        {
            Arc * a = * pv;
            int x = a->nod;
            int w = a->w;
            if(d[x] > d[v] + w)
            {
                d[x] = d[v] + w;
                p[x] = v;
            }
        }
    }
}
// Danh sách các đỉnh về đích
stack<int> Path(int s, int v,map<int,int> p)
{
    stack <int> S;
    int x = v;
    while( x != s)
    {
        S.push(x);
        x = p[x];
    }
    S.push(s);
    return S;
}

int distanceR(int s,int v)
{
    dijkstra(s,C,SC,dC,pC);
    return dC[v];
}
void Path()
{
    int L[1000]={0};
    int index=0;
    stack<int> path1;
    stack<int> path2;

    dijkstra(a,A,SA,dA,pA);
    path1=Path(a,c,pA);

    dijkstra(b,B,SB,dB,pB);
    path2=Path(b,d,pB);

    if(dA[b] <= r)
    {
        cout<< "Khong the";
        return;
    }
    if(a==d && c==b){
        int count=0;
        for(set<Arc *> :: iterator p=A[a].begin();p!=A[a].end();p++){
            Arc *ab=*p;
            if(ab->nod != path1.top() && distanceR(ab->nod,path2.top())>r){
                L[index]=a;
                index++;
                L[index]=b;
                index++;
                count=1;
                dijkstra(ab->nod,A,SA,dA,pA);
                path1=Path(ab->nod,c,pA);
                break;
            }
        }
        if(count==0){
            for(set<Arc *> :: iterator p=B[b].begin();p!=B[b].end();p++){
                Arc *ab=*p;
                if(ab->nod != path2.top() && distanceR(ab->nod,path1.top())>r){
                    L[index]=a;
                    index++;
                    L[index]=b;
                    index++;
                    dijkstra(ab->nod,B,SB,dB,pB);
                    path2=Path(ab->nod,d,pB);
                    break;
                }
            }
        }
    }

    while((!path1.empty()) || (!path2.empty())){
        if(path1.empty()){
            if(distanceR(path2.top(),c) > r)
            {
                L[index]=L[index-2];
                index++;
                L[index]=path2.top();path2.pop();
                index++;
            }
            else{
                int count=0;
                for(set<Arc *> :: iterator p=B[L[index-2]].begin();p!=B[L[index-2]].end();p++){
                    Arc *a=*p;
                    if(a->nod != path2.top() && distanceR(a->nod,c)>r){
                        count=1;
                        dijkstra(a->nod,B,SB,dB,pB);
                        path2=Path(a->nod,d,pB);
                    }
                }
                if(count==0){
                    cout<< "Khong the"; return;
                }
            }
        }
        else
        {
            if(path2.empty()){
                if(distanceR(path1.top(),d) > r)
                {
                    L[index]=path1.top();path1.pop();
                    index++;
                    L[index]=L[index-2];
                    index++;
                }
                else{
                    int count=0;
                    for(set<Arc *> :: iterator p=A[L[index-2]].begin();p!=A[L[index-2]].end();p++){
                        Arc *a=*p;
                        if(a->nod != path1.top() && distanceR(a->nod,d)>r){
                            count=1;
                            dijkstra(a->nod,A,SA,dA,pA);
                            path1=Path(a->nod,c,pA);
                        }
                    }
                    if(count==0){
                        cout<< "Khong the"; return;
                    }
                }
            }
            else
            {
                //TH1: cả 2 cùng đi
                if(distanceR(path1.top(),path2.top()) > r){
                    L[index]=path1.top(); path1.pop();
                        index++;
                    L[index]=path2.top(); path2.pop();
                        index++;
                    }
                //TH2: 1 trong 2 đi
                else{
                    // ROBOT A được đi
                    if(distanceR(path1.top(),L[index - 1]) > r && distanceR(path2.top(),L[index-2])<= r){
                        L[index]=path1.top();path1.pop();
                            index++;
                        L[index]=L[index-2];
                            index++;
                    }
                    else{
                        // ROBOT B được đi
                        if(distanceR(path1.top(),L[index - 1]) <= r && distanceR(path2.top(),L[index-2]) > r){
                            L[index]=L[index-2];
                                index++;
                            L[index]=path2.top();path2.pop();
                                index++;
                        }
                        else{
                            // TH3: Cả 2 không đi
                            int count=0;
                            for(set<Arc *> :: iterator p=A[L[index-2]].begin();p!=A[L[index-2]].end();p++){
                                Arc *a=*p;
                                if(a->nod != path1.top() && distanceR(a->nod,path2.top())>r){
                                    count=1;
                                    dijkstra(a->nod,A,SA,dA,pA);
                                    path1=Path(a->nod,c,pA);
                                }
                            }
                            if(count==0){
                                for(set<Arc *> :: iterator p=B[L[index-2]].begin();p!=B[L[index-2]].end();p++){
                                    Arc *a=*p;
                                    if(a->nod != path2.top() && distanceR(a->nod,path1.top())>r){
                                        dijkstra(a->nod,B,SB,dB,pB);
                                        path2=Path(a->nod,d,pB);
                                        break;
                                    }
                                }
                            }
                        }
                    }
                }
            }
        }
    }

    for(int i =0 ; i<index;i+=2)
    {
        cout<<"("<<L[i]<<","<<L[i+1]<<")";
        cout<< "\n";
    }
}
int main()
{
    readFile("robot.txt");
    Path();
}
