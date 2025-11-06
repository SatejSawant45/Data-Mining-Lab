#include<bits/stdc++.h>

using namespace std;

vector<vector<double>> readCSV(string filename)
{
    vector<vector<double>> data;
    ifstream file(filename);
    /*
    ifstream objects are used to open files in read mode and extract their contents into a C++ program. 
    You can create an ifstream object and associate it with a file by passing the file path to its constructor.
    */
    string line;
    bool headerSkipped = false;

    //getline(input file , the string to store the line , delimeter to stop (by default a new line character ))

    cout<<"hi"<<endl;
    while(getline(file,line))
    {
        if(!headerSkipped)
        {
            headerSkipped = true;
            continue;

            //for skipping header
        }

        cout<<line<<" line 1"<<endl;

        stringstream ss(line);

        vector<double> row;
        string value;

        while(getline(ss,value,','))
        {
            cout<<value<<endl;
            row.push_back(stod(value));
        }

        data.push_back(row);

    
    }

    return data;
}


double EuclidianDistance(vector<double> data,vector<double> centroids)
{
    double sum = 0.0;
    for(int i=0;i<data.size();i++)
    {
        sum += pow(data[i]-centroids[i],2);
    }
    return sqrt(sum);

}

int main()
{
    string filename = "dataset.csv";

    vector<vector<double>> data = readCSV(filename);


    if(data.empty())
    {
        cout<<"Error : dataset.csv not found or empty"<<endl;
    }

    cout<<"Enter the number of clusters you want"<<endl;
    int k;
    cin>>k;
    cout<<"hi"<<endl;


    int n = data.size();
    int m = data[0].size();

    vector<vector<double>> centroids;


    // srand(42); //psudo random genterator with 42 as seed value

    //Initializing centorids randomly

    for(int i=0;i<k;i++)
    {
        int idx = rand()%n;
        centroids.push_back(data[idx]);
    }

    vector<int> labels(n);
    int max_iters = 100;


    for(int itr=0;itr<max_iters;itr++)
    {
        //Assign reach point to nearest centroid

        for(int i=0;i<n;i++)
        {
            int cluster = 0;
            double max_dist = 1000000.0;

            for(int j=0;j<k;j++)
            {
                double dist = EuclidianDistance(data[i],centroids[j]);
                if(dist<max_dist)
                {
                    max_dist = dist;
                    cluster = j;
                }

            }

            labels[i] = cluster;
        }

        // compute new centorids
        vector<vector<double>> newCentorids(k,vector<double>(m,0.0));
        vector<int> count(k,0);

        for(int i=0;i<n;i++)
        {
            int cluster = labels[i];
            for(int j=0;j<m;j++)
            {
                newCentorids[cluster][j] = newCentorids[cluster][j] + data[i][j];
            }
            count[cluster]++;
        }

        for(int j=0;j<k;j++)
        {
            if(count[j]>0)
            {
                for(int x=0;x<m;x++)
                {
                    newCentorids[j][x] = newCentorids[j][x]/count[j];
                }
            }
        }

        //check for convergance

        bool converged = true;

        for(int j=0;j<k;j++)
        {
            int dist = EuclidianDistance(centroids[j],newCentorids[j]);

            if(dist>(1e-6))
            {
                converged = false;
                break;
            }
        }

        centroids = newCentorids;

        if(converged) break;
        
    }

    cout<<"Final centorids "<<endl;

    for(int j=0;j<k;j++)
    {
        cout<<"Cluster "<<j+1<<": ";
        for(double val:centroids[j])
        {
            cout<<val<<" ";
        }
        cout<<endl;
    }

    cout<<"Cluster Assignments "<<endl;
    for(int i=0;i<n;i++)
    {
        cout<<"Data point "<<i+1<<" --> cluster "<<labels[i] + 1<<endl;
    }

    return 0;

    

}