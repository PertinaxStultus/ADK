#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <stdbool.h>

// Struct for  a weighted edge in the graph
typedef struct Edge
{
    int source;
    int target;
    int weight;
} Edge;

// Struct for a directed graph
typedef struct Graph
{
    int Ver;
    int E;
    Edge *edge;
    struct Node **adjList;
} Graph;

// Struct for a node in the adjacency list
typedef struct Node
{
    int data;
    struct Node *next;
} Node;

// Function prototypes
void DFS(Graph *graph, int v, int *visited, Node **stack);
void DFS_reverse(Graph *graph, int v, int *visited, int *result);
void AddDirectedEdge(Graph *graph, int source, int target, int weight);
void SCC(Graph *graph, int source, int target, int *visitedSCC);

// Function to create a new graph
Graph *createGraph(int Ver, int E)
{
    Graph *graph = (Graph *)malloc(sizeof(Graph));
    graph->Ver = Ver;
    graph->E = 0;                                   // puts edge count to 0
    graph->edge = (Edge *)malloc(E * sizeof(Edge)); // Allocate memory for the edge array
    graph->adjList = (Node **)malloc((Ver + 1) * sizeof(Node *));
    for (int i = 0; i <= Ver; ++i)
    {
        graph->adjList[i] = NULL;
    }
    return graph;
}

// Depth-first search
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

// Function to check if the graph is strongly connected
void SCC(Graph *graph, int source, int target, int *visitedSCC)
{
    int *visited = (int *)malloc((graph->Ver + 1) * sizeof(int));

    Node *stack = NULL; // makes stack

    for (int i = 1; i <= graph->Ver; ++i)
    {
        visited[i] = 0;
    }

    DFS(graph, source, visited, &stack); // calles the DFS on the provided graph

    Graph *transposed = createGraph(graph->Ver, graph->E);

    // Transpose only for nodes reachable from source
    for (int v = 1; v <= graph->Ver; ++v)
    {
        if (visited[v])
        {
            Node *adjNode = graph->adjList[v];
            while (adjNode != NULL)
            {
                AddDirectedEdge(transposed, adjNode->data, v, 0);
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

    DFS_reverse(transposed, source, visited, result); // calles the DFS function  on the transposed graph

    // Print the SCC if it has not  been visited before and target vertex is in the SCC
    if (!visitedSCC[source] && result[target])
    {
        printf("Strongly Connected Component containing target vertex %d: ", target);
        for (int i = 1; i <= graph->Ver; ++i)
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

// Function to run the Bellman-Ford algorithm
int bellmanFord(Graph *G, int source, int target)
{
    int Ver = G->Ver;
    int E = G->E;
    int dist[Ver];

    // declares distances from source to all other vertices as INFINITY
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
                printf("Going from node %d to node %d through edge (%d, %d) with weight %d. weight gaind/lost: %d\n", u, v, u, v, weight, dist[v]);
                if (v == target)
                {
                    printf("Target node reached. Stopping the algorithm.\n");
                    return dist[target];
                }
            }
        }
    }

    // Runs the algorithm to detect negative cycles
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

    return dist[target]; // Calculate minimum weight from source to target
}

int main()
{
    int Ver = 8; // Number of vertices in the graph
    int E = 10;  // Number of edges in the graph

    Graph *graph = createGraph(Ver, E);

    // Add edges to the graph

    // Add edge            S  A  3 <-- weight
    AddDirectedEdge(graph, 0, 1, 3);

    // Add edge            S  B  5 <-- weight
    AddDirectedEdge(graph, 0, 3, 5);

    // Add edge            S  E  2 <-- weight
    AddDirectedEdge(graph, 0, 5, 2);

    // Add edge            A  B  -4 <-- weight
    AddDirectedEdge(graph, 1, 2, -4);

    // Add edge            B  G  4 <-- weight
    AddDirectedEdge(graph, 2, 7, 4);

    // Add edge            C  D  6 <-- weight
    AddDirectedEdge(graph, 3, 4, 6);

    // Add edge            D  C  -3 <-- weight
    AddDirectedEdge(graph, 4, 3, -3);

    // Add edge            D  G  8 <-- weight
    AddDirectedEdge(graph, 4, 7, 8);

    // add edge            E  F  3 <-- weight
    AddDirectedEdge(graph, 5, 6, 3);

    // add edge            F  E  -6 <-- weight
    AddDirectedEdge(graph, 6, 5, -6);

    int source = 0; // Source vertex (Harrys start pos)
    int target = 4; // Target vertex (Harrys room)

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