#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <stdbool.h>

// Structure to represent a weighted edge in the graph
typedef struct Edge
{
    int source;
    int target;
    int weight;
} Edge;

// Structure to represent a directed graph
typedef struct Graph
{
    int Ver;
    int E;
    Edge *edge;
    struct Node **adjList; // Added for DFS and DFS_reverse
} Graph;

// Structure to represent a node in the adjacency list
typedef struct Node
{
    int data;
    struct Node *next;
} Node;

// Function prototypes
void DFS(Graph *graph, int v, int *visited, Node **stack);
void DFS_reverse(Graph *graph, int v, int *visited, int *result);
void AddDirectedEdge(Graph *graph, int source, int target, int weight); // Added declaration
void SCC(Graph *graph, int source, int target, int *visitedSCC);        // Modified declaration

// Function to create a new graph with V vertices and E edges
Graph *createGraph(int Ver, int E)
{
    Graph *graph = (Graph *)malloc(sizeof(Graph));
    graph->Ver = Ver;
    graph->E = 0;                                   // Initialize edge count to 0
    graph->edge = (Edge *)malloc(E * sizeof(Edge)); // Allocate memory for the edge array
    graph->adjList = (Node **)malloc((Ver + 1) * sizeof(Node *));
    for (int i = 0; i <= Ver; ++i)
    {
        graph->adjList[i] = NULL;
    }
    return graph;
}

// Depth-first search to mark visited nodes
void DFS_reverse(Graph *graph, int v, int *visited, int *result)
{
    visited[v] = 1;
    result[v] = 1;

    Node *adjNode = graph->adjList[v];
    while (adjNode != NULL)
    {
        if (!visited[adjNode->data])
        {
            DFS_reverse(graph, adjNode->data, visited, result);
        }
        adjNode = adjNode->next;
    }
}

// Function to find Strongly Connected Components (SCC)
void SCC(Graph *graph, int source, int target, int *visitedSCC)
{
    int *visited = (int *)malloc((graph->Ver + 1) * sizeof(int));

    // Initialize stack
    Node *stack = NULL;

    for (int i = 1; i <= graph->Ver; ++i)
    {
        visited[i] = 0;
    }

    DFS(graph, source, visited, &stack);

    Graph *transposed = createGraph(graph->Ver, graph->E);

    // Transpose only for nodes reachable from source
    for (int v = 1; v <= graph->Ver; ++v)
    {
        if (visited[v])
        {
            Node *adjNode = graph->adjList[v];
            while (adjNode != NULL)
            {
                AddDirectedEdge(transposed, adjNode->data, v, 0); // You need to implement AddDirectedEdge function
                adjNode = adjNode->next;
            }
        }
    }

    // Reset visited array
    for (int i = 1; i <= graph->Ver; ++i)
    {
        visited[i] = 0;
    }

    // Initialize result array
    int *result = (int *)malloc((graph->Ver + 1) * sizeof(int));
    for (int i = 1; i <= graph->Ver; ++i)
    {
        result[i] = 0;
    }

    // Perform DFS on transposed graph
    DFS_reverse(transposed, source, visited, result);

    // Print the SCC if it hasn't been visited before and target vertex is in the SCC
    if (!visitedSCC[source] && result[target])
    {
        printf("Strongly Connected Component containing target vertex %d: ", target);
        for (int i = 0; i <= graph->Ver; ++i) // Start from index 0
        {
            if (result[i])
            {
                printf("%d ", i);
            }
        }
        printf("\n");

        // Mark all vertices in this SCC as visited
        for (int i = 1; i <= graph->Ver; ++i)
        {
            if (result[i])
            {
                visitedSCC[i] = 1;
            }
        }
    }

    // Free allocated memory
    free(visited);
    free(result);
    free(stack);
    for (int i = 1; i <= transposed->Ver; ++i)
    {
        Node *current = transposed->adjList[i];
        while (current != NULL)
        {
            Node *next = current->next;
            free(current);
            current = next;
        }
    }
    free(transposed->adjList);
    free(transposed);
}

// Depth-first search
void DFS(Graph *graph, int v, int *visited, Node **stack)
{
    visited[v] = 1;

    Node *adjNode = graph->adjList[v];
    while (adjNode != NULL)
    {
        if (!visited[adjNode->data])
        {
            DFS(graph, adjNode->data, visited, stack);
        }
        adjNode = adjNode->next;
    }

    Node *temp = (Node *)malloc(sizeof(Node));
    temp->data = v;
    temp->next = *stack;
    *stack = temp;
}

// Function to add a directed edge to the graph
void AddDirectedEdge(Graph *graph, int source, int target, int weight)
{
    // Add the edge to the adjacency list
    Node *newNode = (Node *)malloc(sizeof(Node));
    newNode->data = target;
    newNode->next = graph->adjList[source];
    graph->adjList[source] = newNode;

    // Add the edge to the edge array
    graph->edge[graph->E].source = source;
    graph->edge[graph->E].target = target;
    graph->edge[graph->E].weight = weight;
    graph->E++; // Increment the edge count
}

// Function to run the Bellman-Ford algorithm to find the minimum weight path
int bellmanFord(Graph *G, int source, int target)
{
    int Ver = G->Ver;
    int E = G->E;
    int dist[Ver];

    // Initialize distances from source to all other vertices as INFINITY
    for (int i = 0; i < Ver; ++i)
        dist[i] = INT_MAX;
    dist[source] = 0;

    // Relax all edges |V| - 1 times
    for (int i = 0; i < Ver - 1; ++i)
    {
        for (int j = 0; j < E; ++j)
        {
            int u = G->edge[j].source;
            int v = G->edge[j].target;
            int weight = G->edge[j].weight;
            if (dist[u] != INT_MAX && dist[u] + weight < dist[v])
            {
                dist[v] = dist[u] + weight;
                printf("Going from node %d to node %d through edge (%d, %d) with weight %d. Cumulative weight: %d\n", u, v, u, v, weight, dist[v]);
                if (v == target)
                {
                    printf("Target node reached. Stopping the algorithm.\n");
                    return dist[target];
                }
            }
        }
    }

    // Run the algorithm for an additional iteration to detect negative cycles
    for (int i = 0; i < Ver; ++i)
    {
        for (int j = 0; j < E; ++j)
        {
            int u = G->edge[j].source;
            int v = G->edge[j].target;
            int weight = G->edge[j].weight;
            if (dist[u] != INT_MAX && dist[u] + weight < dist[v])
            {
                printf("Negative cycle detected at node %d through edge (%d, %d) with weight %d\n", v, u, v, weight);
                return -1; // Negative cycle detected
            }
        }
    }

    // Calculate minimum weight from source to target
    return dist[target];
}

int main()
{
    int Ver = 12; // Number of vertices in the graph
    int E = 21;   // Number of edges in the graph

    Graph *graph = createGraph(Ver, E);

    // Add edge            1  13: 2 <-- weight
    AddDirectedEdge(graph, 0, 12, 2);

    // Add edge            1  5  2 <-- weight
    AddDirectedEdge(graph, 0, 4, 1);

    // Add edge            1  2  2 <-- weight
    AddDirectedEdge(graph, 0, 1, 2);

    // Add edge            2  6  -1 <-- weight
    AddDirectedEdge(graph, 1, 5, -1);

    // Add edge            3  8  3 <-- weight
    AddDirectedEdge(graph, 2, 7, 3);

    // Add edge            4  13  1 <-- weight
    AddDirectedEdge(graph, 3, 12, 1);

    // Add edge            5  13  -1 <-- weight
    AddDirectedEdge(graph, 4, 12, -1);

    // Add edge            5  3  -2 <-- weight
    AddDirectedEdge(graph, 4, 2, -2);

    // add edge            6  9  -2 <-- weight
    AddDirectedEdge(graph, 5, 8, -2);

    // add edge            7  2  -2 <-- weight
    AddDirectedEdge(graph, 6, 1, -2);

    // add edge            7  8  1 <-- weight
    AddDirectedEdge(graph, 6, 7, 1);

    // add edge            8  5  -2 <-- weight
    AddDirectedEdge(graph, 7, 4, -2);

    // add edge            9  10 -1 <-- weight
    AddDirectedEdge(graph, 8, 9, -1);

    // add egde            9  1  2 <-- weight
    AddDirectedEdge(graph, 8, 0, 2);

    // add egde            10 4  2 <-- weight
    AddDirectedEdge(graph, 9, 3, 2);

    // add egde            11  3  -1 <-- weight
    AddDirectedEdge(graph, 10, 2, -1);

    // add egde            11  7  7 <-- weight
    AddDirectedEdge(graph, 10, 6, 7);

    // add egde            12  4  3 <-- weight
    AddDirectedEdge(graph, 11, 3, 3);

    // add egde            12  13  3 <-- weight
    AddDirectedEdge(graph, 11, 12, 3);

    // add egde            12  11  3 <-- weight
    AddDirectedEdge(graph, 11, 10, 3);

    // add egde            13  3  2 <-- weight
    AddDirectedEdge(graph, 12, 2, 2);

    int source = 11; // Source vertex 12(harrys start pos)
    int target = 1;  // Target vertex 2 (Harrys room)

    int minWeight = bellmanFord(graph, source, target);
    if (minWeight == -1)
        printf("Graph contains negative cycle\n");
    else if (minWeight == INT_MAX)
        printf("No path from node %d to node %d\n", source, target);
    else
        printf("Minimum weight from node %d to node %d: %d\n", source, target, minWeight);

    // Call the SCC function
    int *visitedSCC = (int *)malloc((graph->Ver + 1) * sizeof(int));
    for (int i = 0; i <= graph->Ver; ++i)
    {
        visitedSCC[i] = 0;
    }
    for (int i = 0; i < graph->Ver; ++i)
    {
        SCC(graph, i, target, visitedSCC);
    }
    free(visitedSCC);

    return 0;
}