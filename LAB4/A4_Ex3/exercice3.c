#include <stdio.h>
#include <stdlib.h>

struct Node
{
    int data;
    struct Node *next;
};

struct Graph
{
    int V;
    struct Node **adjList;
};

struct Graph *createGraph(int V)
{
    struct Graph *graph = (struct Graph *)malloc(sizeof(struct Graph));
    graph->V = V;
    graph->adjList = (struct Node **)malloc((V + 1) * sizeof(struct Node *));

    for (int i = 1; i <= V; ++i)
    {
        graph->adjList[i] = NULL;
    }

    return graph;
}

void AddDirectedEdge(struct Graph *graph, int src, int dest)
{
    struct Node *newNode = (struct Node *)malloc(sizeof(struct Node));
    newNode->data = dest;
    newNode->next = graph->adjList[src];
    graph->adjList[src] = newNode;
}

void DFS(struct Graph *graph, int v, int *visited, struct Node **stack)
{
    visited[v] = 1;

    struct Node *adjNode = graph->adjList[v];
    while (adjNode != NULL)
    {
        if (!visited[adjNode->data])
        {
            DFS(graph, adjNode->data, visited, stack);
        }
        adjNode = adjNode->next;
    }

    struct Node *temp = (struct Node *)malloc(sizeof(struct Node));
    temp->data = v;
    temp->next = *stack;
    *stack = temp;
}

void DFS_reverse(struct Graph *graph, int v, int *visited, int *result)
{
    visited[v] = 1;
    result[v] = 1;

    struct Node *adjNode = graph->adjList[v];
    while (adjNode != NULL)
    {
        if (!visited[adjNode->data])
        {
            DFS_reverse(graph, adjNode->data, visited, result);
        }
        adjNode = adjNode->next;
    }
}

void SCC(struct Graph *graph, int source, int *visitedSCC)
{
    int *visited = (int *)malloc((graph->V + 1) * sizeof(int));

    // Initialize stack
    struct Node *stack = NULL;

    for (int i = 1; i <= graph->V; ++i)
    {
        visited[i] = 0;
    }

    DFS(graph, source, visited, &stack);

    struct Graph *transposed = createGraph(graph->V);

    // Transpose only for nodes reachable from source
    for (int v = 1; v <= graph->V; ++v)
    {
        if (visited[v])
        {
            struct Node *adjNode = graph->adjList[v];
            while (adjNode != NULL)
            {
                AddDirectedEdge(transposed, adjNode->data, v);
                adjNode = adjNode->next;
            }
        }
    }

    // Reset visited array
    for (int i = 1; i <= graph->V; ++i)
    {
        visited[i] = 0;
    }

    // Initialize result array
    int *result = (int *)malloc((graph->V + 1) * sizeof(int));
    for (int i = 1; i <= graph->V; ++i)
    {
        result[i] = 0;
    }

    // Perform DFS on transposed graph
    DFS_reverse(transposed, source, visited, result);

    // Print the SCC if it hasn't been visited before
    if (!visitedSCC[source])
    {
        printf("Strongly Connected Component starting from %d: ", source);
        for (int i = 1; i <= graph->V; ++i)
        {
            if (result[i])
            {
                printf("%d ", i);
            }
        }
        printf("\n");

        // Mark all vertices in this SCC as visited
        for (int i = 1; i <= graph->V; ++i)
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
    for (int i = 1; i <= transposed->V; ++i)
    {
        struct Node *current = transposed->adjList[i];
        while (current != NULL)
        {
            struct Node *next = current->next;
            free(current);
            current = next;
        }
    }
    free(transposed->adjList);
    free(transposed);
}

int main()
{
    struct Graph *graph = createGraph(9);
    // vertex 1 connection
    AddDirectedEdge(graph, 1, 5);
    AddDirectedEdge(graph, 1, 8);
    AddDirectedEdge(graph, 1, 4);
    // vertex 2 connection
    AddDirectedEdge(graph, 2, 9);
    AddDirectedEdge(graph, 2, 5);
    // vertex 3 connection
    AddDirectedEdge(graph, 3, 4);
    // vertex 4 connection
    AddDirectedEdge(graph, 4, 8);

    AddDirectedEdge(graph, 6, 2);
    // vertex 7 connection
    AddDirectedEdge(graph, 7, 2);
    AddDirectedEdge(graph, 7, 9);
    // vertex 8 connection
    AddDirectedEdge(graph, 8, 1);
    AddDirectedEdge(graph, 8, 3);
    // vertex 9 connection
    AddDirectedEdge(graph, 9, 6);
    AddDirectedEdge(graph, 9, 3);

    // Array to keep track of visited vertices
    int *visited = (int *)malloc((graph->V + 1) * sizeof(int));
    int *visitedSCC = (int *)malloc((graph->V + 1) * sizeof(int)); // Array to keep track of visited SCCs
    for (int i = 1; i <= graph->V; ++i)
    {
        visited[i] = 0;
        visitedSCC[i] = 0; // Initialize visited SCC array
    }

    // Iterate over all vertices and find SCCs
    for (int v = 9; v <= graph->V; ++v)
    {
        if (!visited[v])
        {
            SCC(graph, v, visitedSCC);
        }
    }

    // Free allocated memory
    free(visited);
    free(visitedSCC);
    free(graph->adjList);
    free(graph);

    return 0;
}