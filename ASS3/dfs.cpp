#include<iostream>
#include<vector>
#include<stack>
#include<queue>
using namespace std;
void dfs(vector<int> graph[], bool visited[], int path[], int s)
{
    visited[s] = true;
    cout << s << " ";
    for (int i = 0; i < graph[s].size(); i++)
    {
        int v = graph[s][i];
        if (!visited[v])
        {
            path[v] = s;
            dfs(graph, visited, path, v);
        }
    }
}
void dfs_stack(vector<int> graph[], bool visited[], int path[], int s)
{
    stack<int> st;
    st.push(s);
    visited[s] = true;
    cout << s << " ";
    while(!st.empty())
    {
        int temp = st.top();
        cout << temp << " ";
        st.pop();
        for (int i = 0; i < graph[temp].size(); i++)
        {
            int v = graph[temp][i];
            if (!visited[v])
            {
                visited[v] = true;
                path[v] = temp;
                st.push(v);
            }
        }
    }
}
void bfs(vector<int> graph[], bool visited[], int path[], int s)
{
    queue<int> q;
    q.push(s);
    visited[s] = 1;
    while (!q.empty())
    {
        int temp = q.front();
        cout << temp << " ";
        q.pop();
        for (int i = 0; i < graph[temp].size(); i++)
        {
            int v = graph[temp][i];
            if (!visited[v])
            {
                visited[v] = true;
                q.push(v);
                path[v] = temp;
            }
        }
    }
}
void printPath(int s, int f, int path[])
{
    if (s == f) cout << f << " ";
    else 
    {
        if (path[f] == -1)
        {
            cout << "No path" << endl;
        }
        else 
        {
            printPath(s, path[f], path);
            cout << f << " ";
        }
    }
}
int interpolationSearch(int arr[], int left, int right, int x)
{
    if (left <= right && x >= arr[left] && x <= arr[right])
    {
        int pos = left + (((double)(right - left)/(arr[right] - arr[left]))*(x - arr[left]));
        if(arr[pos]==x)
        {
            cout << "We traverse on index: " << pos<< endl;
            return pos;
        }
        else if(arr[pos]<x)
        {
            cout << "We traverse on index: " << pos<< endl;
            return interpolationSearch(arr,pos+1,right,x);
        }
        else
        {
            cout << "We traverse on index: " << pos<< endl;
            return interpolationSearch(arr,left,pos-1,x);
        }
    }
    return -1;
}
int main()
{
    int n; cin >> n;
    int i = n;
    int m = 0;
    while (i > 0)
    {
        i--;
        m += n;
    }
    cout << m;
}