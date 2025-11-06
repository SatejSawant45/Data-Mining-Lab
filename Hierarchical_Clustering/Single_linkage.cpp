#include<bits/stdc++.h>

using namespace std;

vector<vector<double>> readCSV(string filename)
{
    vector<vector<double>> data;
    ifstream file(filename);
    string line;

    bool headerSkipped = false;

    while(getline(file,line))
    {
        if(!headerSkipped)
        {
            headerSkipped = true;
            continue;
        }

        vector<double> row;
        stringstream ss(line);

        string value;

        while(getline(ss,value,','))
        {
            row.push_back(stod(value));
        }

        data.push_back(row);


    }

    return data;


} 

double euclidianDistance(vector<double> p1,vector<double> p2)
{
    double sum = 0;
    for(int i=0;i<p1.size();i++)
    {
        sum = sum + pow((p1[i]-p2[i]),2);
    }

    return sqrt(sum);
    
}

pair<int,int> findClosestClusters(vector<vector<double>> &dist,vector<bool> &active)
{
    pair<int,int> closest = {-1,-1};
    double minDist = DBL_MAX;
    int n = dist.size();

    for(int i=0;i<n;i++)
    {
        if(!active[i]) continue;
        
        for(int j=i+1;j<n;j++)
        {
            if(!active[j]) continue;

            if(dist[i][j] < minDist)
            {
                minDist = dist[i][j];
                closest = {i,j};
            }
        }
    }

    return closest;
}

vector<vector<double>> getDistanceMatrix(vector<vector<double>> &data)
{
    int n = data.size();

    vector<vector<double>> dist(n,vector<double> (n,0.0));
    for(int i=0;i<n;i++)
    {
        vector<double> point1 = data[i];
        for(int j=i+1;j<n;j++)
        {
            vector<double> point2 = data[j];

            double distance = euclidianDistance(point1,point2);
            dist[i][j] = distance;
            dist[j][i] = distance;
        }
    }

    return dist;

}

void updateDistances(vector<vector<double>> &dist,int clusterA, int clusterB, vector<bool> &active)
{
    int n = dist.size();
    for(int k=0;k<n;k++)
    {
        if(!active[k] || (k==clusterA) || (k==clusterB))
        {
            continue;
        }
        else
        {
            dist[clusterA][k] = min(dist[clusterA][k],dist[clusterB][k]);
            dist[k][clusterA] = dist[clusterA][k];
        }
    }

}

void SingleLinkage(vector<vector<double>> &data)
{
    int n = data.size();
    vector<vector<double>> dist = getDistanceMatrix(data);

    vector<bool> active(n,true);
    vector<vector<int>> clusters(n);

    for(int i=0;i<n;i++) clusters[i].push_back(i);

    cout<<"Initial clusters : "<<endl;
    for(int i=0;i<n;i++)
    {
        cout<<"Cluster "<<i<<" : {"<<i<<"} "<<endl;
    }

    int step = 1;
    int clustersRemaining = n;

    while(clustersRemaining>1)
    {
        pair<int,int> pr = findClosestClusters(dist,active);
        int a = pr.first;
        int b = pr.second;

        if(a==-1 || b==-1) break;

        vector<int> clusterIndices = clusters[b];
        for(int itr=0;itr<clusterIndices.size();itr++)
        {
            clusters[a].push_back(clusterIndices[itr]);
        }

        cout<<"Merging clusters "<<a<<" and "<<b<<endl;

        active[b] = false;

        clustersRemaining--;

        updateDistances(dist,a,b,active);


    }

    

}

int main()
{
    string filename = "dataset.csv";

    vector<vector<double>> data = readCSV(filename);
    
    // for(int i=0;i<data.size();i++)
    // {
    //     for(int j=0;j<data[i].size();j++)
    //     {
    //         cout<<data[i][j]<<" ";
    //     }
    //     cout<<endl;
    // }


    if(data.empty())
    {
        cout<<"Error : 'Dataset.csv' does not exists or is empty "<<endl;
    }

    SingleLinkage(data);
}