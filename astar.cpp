#include <iostream>
#include <vector>
#include <queue>
#include <cmath>
using namespace std;

struct Vertex
{
    int id;   // vertex id
    int dist; // distance from source vertex
    int f;    // f = g + h
    int x;    // x coordinate
    int y;    // y coordinate
};
/*Edge contains the source vertex id, target vertex id, and weight. A constructor is also defined for the edge, which initializes the fields with the given values.*/

struct Edge
{
    int sid;                                                      // source vertex id
    int tid;                                                      // target vertex id
    int w;                                                        // weight
    Edge(int s, int t, int weight) : sid(s), tid(t), w(weight) {} // constructor
};

/*A Graph class is defined that has some methods. The constructor initializes the number of vertices and creates an array of vertices and an adjacency list. addEdge method adds an edge to the graph by adding the edge to the adjacency list of the source vertex. addVertex method adds the vertex id, x, and y coordinates to the corresponding vertex.*/
class Graph
{
public:
    Graph(int v)
    {
        this->v = v;               // number of vertexes
        vertexes = new Vertex[v];  // vertex array
        adj = new vector<Edge>[v]; // adjacency list
    }

    // add edge to adjacency list, start vertex, end vertex, weight
    void addEdge(int s, int t, int w)
    {
        adj[s].push_back(Edge(s, t, w));
    }

    // constructor, add vertex to vertex array, vertex id, x coordinate, y coordinate
    void addVertex(int id, int x, int y)
    {
        vertexes[id].id = id; // vertex id
        vertexes[id].x = x;   // x coordinate
        vertexes[id].y = y;   // y coordinate
    }

    /*This line defines the function name, astar, and takes two integer parameters: s and t. s represents the starting vertex, and t represents the target vertex.*/
    void astar(int s, int t)
    {
        /*This line declares two dynamic arrays of integers and booleans respectively.
        The predecessor array will store the predecessor of each vertex in the shortest path tree, while the inqueue array will keep track of whether a vertex is currently in the priority queue.*/
        int *predecessor = new int[v];
        bool *inqueue = new bool[v];

        /*This loop initializes the distance (dist) and heuristic value (f) of each vertex to infinity, sets the predecessor of each vertex to -1 (indicating that it has no predecessor yet), sets the inqueue flag of each vertex to false, and sets the id of each vertex to its index in the vertexes array. This loop essentially prepares the data structures for the A* algorithm.*/

        for (int i = 0; i < v; ++i)
        {
            vertexes[i].dist = INT_MAX;
            vertexes[i].f = INT_MAX;
            predecessor[i] = -1;
            inqueue[i] = false;
            //   vertexes[i].id = i;
        }

        /*These lines set the distance and heuristic value of the starting vertex s to 0.
        This is because we know that the distance from s to itself is 0, and
        we also set the heuristic value to 0 because we know that we have reached the target vertex.*/

        // s is the starting point of the distance
        vertexes[s].dist = 0;
        vertexes[s].f = 0;
        priority_queue<Vertex *, vector<Vertex *>, decltype(&compare)> queue(&compare);

        // pushing the starting vertex heap
        queue.push(&vertexes[s]);
        inqueue[s] = true;
        /*This is the main loop of the A* algorithm.
        While the priority queue is not empty, the algorithm pops the vertex with the lowest f value from the queue (minVertex), and checks its neighboring vertices.
        For each neighboring vertex nextVertex, the algorithm computes a tentative distance from the starting vertex s to nextVertex (`minVertex->dist + e
        */
        while (!queue.empty())
        {
            Vertex *minVertex = queue.top();
            // The vertex with the minimum f value is obtained from the priority queue.

            queue.pop(); // The minimum vertex is removed from the priority queue.

            // loop size is the number of adjacent vertices of the minimum vertex.
            for (int i = 0; i < adj[minVertex->id].size(); ++i)
            {
                Edge e = adj[minVertex->id][i];
                // The neighboring vertex is obtained from the adjacency list of the minimum vertex.
                Vertex *nextVertex = &vertexes[e.tid]; // The neighboring vertex is obtained from the adjacency list of the minimum vertex.
                if (minVertex->dist + e.w < nextVertex->dist)
                { // If the tentative distance is less than the current distance, the distance and f value of the neighboring vertex are updated, and the predecessor of the neighboring vertex is set to the minimum vertex.

                    nextVertex->dist = minVertex->dist + e.w; // the distance to the next vertex is updated

                    // update the f value of the next vertex according to the heuristic function
                    nextVertex->f = nextVertex->dist + hManhattan(*nextVertex, vertexes[t]);

                    // f = g + h The f value of the next vertex is updated as the sum of the distance to the next vertex and the heuristic estimate of the remaining distance to the destination vertex.

                    predecessor[nextVertex->id] = minVertex->id;
                    // The predecessor of the next vertex is set to the current minimum vertex.

                    // If the next vertex is already in the queue, it is pushed again with the updated f value
                    if (inqueue[nextVertex->id])
                    {
                        // The next vertex is pushed onto the priority queue.
                        queue.push(nextVertex);
                    }
                    else
                    {
                        queue.push(nextVertex);
                        inqueue[nextVertex->id] = true; // The next vertex is marked as inqueue.
                    }
                }
                if (nextVertex->id == t)
                {
                    // If the next vertex is the destination vertex, the priority queue is cleared and the while loop is terminated.
                    queue = priority_queue<Vertex *, vector<Vertex *>, decltype(&compare)>(&compare); // clear queue
                    break;
                }
            }
        }
        cout << " " << endl;
        cout << "A* search algorithm: " << s;
        // Prints the ID of the source vertex.
        print(s, t, predecessor);
        // Prints the path from the source vertex s to the destination vertex t using the predecessor array.
        delete[] predecessor;
        delete[] inqueue;
    }

    void hillClimbing(int s, int t)
    {
        int currentVertex = s;
        vector<bool> visited(v, false); // Create a boolean vector of size v (number of vertices), initializing all elements to false. This vector will be used to keep track of whether a vertex has been visited or not.

        cout << " " << endl;
        cout << "Hill climbing: " << s;

        while (currentVertex != t)
        {
            visited[currentVertex] = true; // Mark the current vertex as visited.
            int nextVertex = -1;           // Initialize the next vertex to -1.
            int minHeuristic = INT_MAX;    // Initialize the minimum heuristic value to infinity.

            for (Edge e : adj[currentVertex])
            {
                if (!visited[e.tid] && hManhattan(vertexes[e.tid], vertexes[t]) < minHeuristic) // Check if the target vertex of the edge e has not been visited and if the Manhattan distance between the target vertex and the destination vertex is less than the current minimum heuristic value.
                {
                    minHeuristic = hManhattan(vertexes[e.tid], vertexes[t]); // Update the minimum heuristic value with the Manhattan distance between the target vertex and the destination vertex.
                    nextVertex = e.tid;                                      // Update the next vertex with the target vertex of the edge e.
                }
            }

            if (nextVertex == -1)
            {
                cout << " - No path found" << endl;
                return;
            }

            cout << " -> " << nextVertex;
            currentVertex = nextVertex;
        }

        cout << endl;
    }

public:
    int v;
    Vertex *vertexes;
    vector<Edge> *adj;
    // in the heap two vertexes are compared according to their f values
    static bool compare(Vertex *v1, Vertex *v2)
    {
        return v1->f > v2->f;
    }
    // updating the vertex , they are doing the manhattan distance
    int hManhattan(Vertex v1, Vertex v2)
    {
        return abs(v1.x - v2.x) + abs(v1.y - v2.y);
        // Manhattan distance
    }
    void print(int s, int t, int *predecessor)
    {
        if (s == t)
        {
            return;
        }
        print(s, predecessor[t], predecessor);
        cout << "->" << t << " ";
    }
};

int main()
{
    // add 14 vertexes and 14 edges to the graph
    // please check the attach pdf file for the graph
    Graph g(14);
    g.addEdge(0, 1, 20);
    g.addEdge(0, 5, 60);
    g.addEdge(0, 6, 60);
    g.addEdge(0, 4, 60);
    g.addEdge(1, 0, 20);
    g.addEdge(1, 2, 20);
    g.addEdge(2, 1, 20);
    g.addEdge(2, 3, 10);
    g.addEdge(3, 2, 10);
    g.addEdge(3, 12, 40);
    g.addEdge(3, 13, 30);
    g.addEdge(12, 3, 40);
    g.addEdge(12, 4, 40);
    g.addEdge(13, 3, 30);
    g.addEdge(13, 6, 50);
    g.addEdge(6, 13, 50);
    g.addEdge(6, 0, 60);
    g.addEdge(6, 7, 70);
    g.addEdge(7, 6, 70);
    g.addEdge(7, 11, 50);
    g.addEdge(11, 7, 50);
    g.addEdge(11, 10, 60);
    g.addEdge(10, 11, 60);
    g.addEdge(10, 9, 60);
    g.addEdge(9, 10, 60);
    g.addEdge(9, 8, 50);
    g.addEdge(8, 9, 50);
    g.addEdge(8, 4, 50);
    g.addEdge(4, 8, 50);
    g.addEdge(4, 0, 60);
    g.addEdge(4, 12, 40);
    g.addEdge(5, 0, 60);
    g.addEdge(5, 10, 50);
    g.addEdge(5, 9, 80);
    g.addEdge(9, 5, 80);
    g.addEdge(5, 8, 70);
    g.addEdge(8, 5, 70);
    g.addVertex(0, 320, 630);
    g.addVertex(1, 300, 630);
    g.addVertex(2, 280, 625);
    g.addVertex(3, 270, 630);
    g.addVertex(4, 320, 700);
    g.addVertex(5, 360, 620);
    g.addVertex(6, 320, 590);
    g.addVertex(7, 370, 580);
    g.addVertex(8, 350, 730);
    g.addVertex(9, 390, 620);
    g.addVertex(10, 400, 620);
    g.addVertex(11, 420, 580);
    g.addVertex(12, 270, 700);
    g.addVertex(13, 270, 590);

    // g.astar(3, 10);
    // g.hillClimbing(3, 10);

    // g.astar(12, 11);
    // g.hillClimbing(12, 11);

    // g.astar(13, 9);        // 13->3 ->2 ->1 ->0 ->5 ->9  : 220
    // g.hillClimbing(13, 9); // 13 -> 6 -> 7 -> 11 -> 10 -> 9 : 290
    g.astar(0, 10);

    g.astar(3, 9);
    g.hillClimbing(3, 9);

    // g.astar(9, 13);
    // g.hillClimbing(9, 13);

    /*
    find shortest path from vertex 0 to vertex 4 using hill climbing algorithm
    find shortest path from vertex 0 to vertex 4 using A* algorithm
    in the example you could see from 0 ->5 -> 10 with red line
    run the program and see the result, type : g++ -std=c++11 astar.cpp; and type:  ./a.out or you have code runner, just control+alt+n
    */

    // example 2:
    Graph g1(9);

    g1.addEdge(0, 1, 2);
    g1.addEdge(0, 2, 4);
    g1.addEdge(0, 3, 5);
    g1.addEdge(1, 4, 7);
    g1.addEdge(1, 5, 8);
    g1.addEdge(2, 6, 9);
    g1.addEdge(3, 7, 10);
    g1.addEdge(3, 8, 12);
    g1.addEdge(7, 8, 2);

    g1.addVertex(0, 0, 0);
    g1.addVertex(1, 2, 2);
    g1.addVertex(2, 4, 1);
    g1.addVertex(3, 5, 4);
    g1.addVertex(4, 7, 2);
    g1.addVertex(5, 8, 4);
    g1.addVertex(6, 6, 6);
    g1.addVertex(7, 3, 7);
    g1.addVertex(8, 4, 8);

    g1.astar(0, 6);
    g1.hillClimbing(0, 6);
    g1.astar(0, 4);
    g1.hillClimbing(0, 4);
    g1.astar(0, 8);
    g1.hillClimbing(0, 8);

    /*
         how to print the vertex distance check
          g1.astar(0,4);  // find shortest path from vertex 0 to vertex 4 using A* algorithm
            for (int i = 0; i < 6; ++i) {
                cout << "vertex " << i << " distance x coordinate : " << g.vertexes[i].x << endl;
                cout << "vertex " << i << " distance y coordinate : " << g.vertexes[i].y << endl;
            }
            for (int i = 0; i < 5; ++i) {
                cout << "vertex " << i << " distance x coordiante : " << g1.vertexes[i].y << endl;
                cout << "vertex " << i << " distance y coordinate : " << g1.vertexes[i].y << endl;
            }
         */
    return 0;
}

// Note that the C++ code uses a priority queue of pointers to Vertex structs instead of Vertex objects, since we need to use a custom comparator function to compare Vertex objects based on their f values. Also, the code uses the `INT_MAX` constant to initialize the distances and f values of the vertices instead of `this.v`, since `this` is not a valid keyword in C++.

/*This is an implementation of the A* algorithm for finding the shortest path between two vertices in a graph. The A* algorithm is a variant of Dijkstra's algorithm that uses heuristics to improve performance. The heuristic function used in this implementation is the Manhattan distance, which is the sum of the absolute differences in x and y coordinates between two vertices.

The graph is represented using an adjacency list and a vertex array. The adjacency list stores the edges for each vertex, and the vertex array stores the properties of each vertex, including its id, x and y coordinates, distance from the source vertex, and f value (which is the sum of the distance and the heuristic).

The A* algorithm starts by initializing the distance and f values of all vertices to infinity, except for the source vertex, which is set to 0. It also initializes an array to store the predecessor of each vertex and a boolean array to keep track of which vertices are currently in the priority queue. The algorithm then adds the source vertex to the priority queue and continues by selecting the vertex with the lowest f value and exploring its neighbors. For each neighbor, the algorithm updates its distance and f value if a shorter path is found and adds it to the priority queue if it is not already there.

The algorithm continues until the destination vertex is reached or the priority queue becomes empty.

Once the destination vertex is reached, the algorithm constructs the shortest path by backtracking from the destination vertex to the source vertex using the predecessor array.

Two example graphs are provided in the main function to test the implementation.


to run this program
type in the terminal:
g++ --std=c++11 astar.cpp
./a.out

or easily run this program in code runner

1. you will see the path result
2. you will check the heuristic function of A* search algorithm, it will show mathanden distance between different vertex

*/
