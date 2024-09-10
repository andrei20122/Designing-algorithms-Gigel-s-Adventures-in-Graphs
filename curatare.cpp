#include <bits/stdc++.h>
using namespace std;

#define pch pair<int, int>

int n, m;
char room[1000][1000];

vector<pch> bots;
vector<pch> dirtySpots;

void lee(pch start, vector<vector<int>> &distMatrix )
{   bool visited[1000][1000];
    memset(visited, 0, 1000*1000);

    queue<pch> q; q.push({start.first, start.second});

    int xs = start.first;
    int ys = start.second;
    visited[xs][ys] = true;
    distMatrix[xs][ys] = 0;

    while ( q.size() != 0 )
    {   pch crt = q.front();
        int dir = 0;
        for ( ; dir < 4; ++dir )
        {   pch nxt = crt;
            if ( dir == 0 )
                nxt.first--;
            if ( dir == 1 )
                nxt.second++;
            if ( dir == 2 )
                nxt.first++;
            if ( dir == 3 )
                nxt.second--;

            if ( !(nxt.first < 0 || nxt.second < 0
            || nxt.first >= n || nxt.second >= m
            || visited[nxt.first][nxt.second] ||
            room[nxt.first][nxt.second] == 'X') )
            {   visited[nxt.first][nxt.second] = true;
                distMatrix[nxt.first][nxt.second] =
                    distMatrix[crt.first][crt.second] + 1;
                q.push({nxt.first, nxt.second});
            }
        }
        q.pop();
    }
}



void partitioningHelper(vector<int>& v, int partitionNo,
vector<vector<vector<int>>>& partitions, int aux[], unsigned int step)
{   if ( !(step != v.size()) )
    {   vector<vector<int>> partition(partitionNo, vector<int>());
        int i = 0;
        for ( ; i < v.size(); ++i )
            partition[aux[i]].push_back(v[i]);

        partitions.push_back(partition);
        return;
    }

    if ( !(step == v.size()) )
    {   int ord = 0;
        for ( ; ord < partitionNo; )
        {   aux[step] = ord;
            partitioningHelper(v, partitionNo, partitions, aux, step + 1);
            ord++;
        }
    }
}

void partitioning(vector<int>& v, int partitionNo,
vector<vector<vector<int>>>& partitions)
{   int aux[100];
    partitioningHelper(v, partitionNo, partitions, aux, 0);
}

void permutationsGenerator(vector<int>& v, vector<vector<int>>& permutations)
{   permutations.push_back(v);
    while (next_permutation(v.begin(), v.end()))
       permutations.push_back(v);
}


int travellingSalesman(int start, vector<vector<int>>& graph,
vector<int> nodes )
{   vector<vector<int>> permutations;
    permutationsGenerator(nodes, permutations);

    int min_distance = 15685469;
    for ( vector<int> permutation : permutations )
    {   int crt = start;
        int distance = 0;
        int i = 0;
        for ( ; i < permutation.size();  )
        {   if ( graph[crt][permutation[i]] != 0 )
            {   distance += graph[crt][permutation[i]];
                crt = permutation[i];
            }
            i++;
        }

        min_distance = min(min_distance, distance);
    }
    return min_distance;
}

vector<vector<int>> fillMatrix(int n, int m, int val)
{   return vector<vector<int>>(n, vector<int>(m, 0));
}

int main()
{   ifstream in("curatare.in");
    in >> n >> m;
    for ( int i = 0; i < n; i++ )
        for ( int j = 0; j < m; j++ )
        {   in >> room[i][j];
            if ( room[i][j] == 'S' || room[i][j] == 'R' ) {
                if ( !(room[i][j] != 'S') )
                    dirtySpots.push_back({i, j});
                if ( !(room[i][j] != 'R') )
                    bots.push_back({i, j});
            }
        }
    in.close();


    vector<vector<int>> distMatrix = fillMatrix(n, m, 0);
    int s = dirtySpots.size();
    vector<vector<int>> dirtySpotsGraph = fillMatrix(s, s, 0);
    for ( unsigned int i = 0; i < dirtySpots.size();  )
    {   lee(dirtySpots[i], distMatrix);
        for ( unsigned int j = i + 1; j < dirtySpots.size();  )
        {   int x = dirtySpots[j].first;
            int y = dirtySpots[j].second;
            dirtySpotsGraph[i][j] = distMatrix[x][y];
            dirtySpotsGraph[j][i] = distMatrix[x][y];
            j++;
        }
        i++;
    }

    vector<vector<vector<int>>> robotGraphs;
    for ( unsigned int i = 0; i < bots.size(); i++ )
    {   vector<vector<int>> robotGraph = fillMatrix(s + 1, s + 1, 0);
        lee(bots[i], distMatrix);
        for ( unsigned int j = 0; j < dirtySpots.size(); j++ )
        {   int x = dirtySpots[j].first;
            int y = dirtySpots[j].second;
            robotGraph[0][j + 1] = distMatrix[x][y];
            robotGraph[j + 1][0] = distMatrix[x][y];
        }
        int j, k;
        for ( j = 0; j < dirtySpots.size(); ++j )
            for ( k = 0; k < dirtySpots.size();)
            {   robotGraph[j+1][k+1] = dirtySpotsGraph[j][k];
                robotGraph[k+1][j+1] = dirtySpotsGraph[k][j];
                ++k;
            }

        robotGraphs.push_back(robotGraph);
    }

    vector<int> dirtySpots_int(s, 0);
    for ( unsigned int i = 0; i < dirtySpots.size(); i++ )
        dirtySpots_int[i] = i + 1;

    vector<vector<vector<int>>> dirtySpotsPartitions;
    partitioning(dirtySpots_int, bots.size(), dirtySpotsPartitions);

    int minTime = 0x7fffffff;
    vector<int> cleaningTimes;
    for ( int i = 0; i < robotGraphs.size(); i++ )
        cleaningTimes.push_back(0);

    int k = 0;
    for ( ; k < dirtySpotsPartitions.size(); ++k )
    {   for ( unsigned int i = 0; i < dirtySpotsPartitions[k].size(); i++ )
            cleaningTimes[i] = travellingSalesman(0, robotGraphs[i],
            dirtySpotsPartitions[k][i]);

        int maxCleaningTime = *max_element(cleaningTimes.begin(),
        cleaningTimes.end());
        minTime = min(minTime, maxCleaningTime);
    }

    ofstream out("curatare.out");
    out << minTime;
    out.close();
    return 0;
}
