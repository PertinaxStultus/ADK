#include <stdio.h>
#include <stdlib.h>

// struct for a node in a adjacency list
typedef struct ListNode
{
    int target;            // The targeted vertex
    struct ListNode *next; // pointer to the next adjacent vertex
} ListNode;

// struct for the adjacency list for a vertex
typedef struct List
{
    struct ListNode *head; // pointer to the head of the list
} List;

// struct for the graph representation
typedef struct graph
{
    int Ver;          // number of verteses in the graph
    int edge;         // The number of edges in the graph
    int Dir;          // To inicate if our graph is directed
    struct List *arr; // array for our adjacent list with the size 'Ver'
} graph;

graph *CreateGraph(int n)
{
    graph *NewGraph = (graph *)malloc(sizeof(graph)); // allocates memory for the graph
    if (NewGraph == NULL)
    {
        exit(EXIT_FAILURE); // function stops if memory fails to allocate
    }

    NewGraph->Ver = n; // the number of verteses in the graph

    NewGraph->arr = (List *)malloc(n * sizeof(List)); // allocates memory for the array for the adjacency list
    if (NewGraph->arr == NULL)
    {
        exit(EXIT_FAILURE); // function stops if memory fails to allocate
    }

    for (int i = 0; i < n; i++)
    {
        NewGraph->arr[i].head = NULL;
    }

    return NewGraph;
}
// a function that checks how may vertices a graph has
int GetNumVertices(graph *G)
{
    if (G == NULL)
    {
        printf("The graph has no vertices"); // checks if the graph as no verties, in that case returns 1(FAIL)
        return 1;
    }
    return G->Ver; // returns the number of vertices in the provided graph G
}

// a function that checks how may edges a graph has
int GetNumEdges(graph *G)
{
    if (G == NULL)
    {
        printf("The graph has no egdes"); // checks if the graph as no edegs, in that case returns 1(FAIL)
        return 1;
    }
    return G->edge; // returns the number of edges in the provided graph G
}

int IsDirected(graph *G)
{

    return G->Dir;
}

ListNode *GetInAdjs(graph *G, int u)
{

    ListNode *AdjVer = NULL; // makes a list that stores the adjacency vertices

    for (int i = 0; i < G->Ver; i++) // goes trought the vertices to find the adjacent vertices of "u"
    {
        ListNode *current = G->arr[i].head;
        while (current != NULL)
        {
            if (current->target == u)
            {
                ListNode *newNode = (ListNode *)malloc(sizeof(ListNode)); // adds the vertex i to the adjacency list go to "u"
                newNode->target = i;
                newNode->next = AdjVer;
                AdjVer = newNode;
                break; // break to move to the next vertex
            }
            current = current->next;
        }
    }
    return AdjVer;
}

ListNode *getOutAdjs(graph *G, int u)
{
    ListNode *outAdjs = NULL; // a list that sores rhe outgoing adjecent vertices

    ListNode *current = G->arr[u].head;
    while (current != NULL)
    {
        ListNode *newNdoe = (ListNode *)malloc(sizeof(ListNode));

        newNdoe->target = current->target;
        newNdoe->next = outAdjs;
        outAdjs = newNdoe;
        current = current->next;
    }
    return outAdjs;
}
ListNode *Getlinked(graph *G, int u)
{
    ListNode *linkedEdges = NULL; // Initialize the list of linked edges

    // If the graph is directed, only add outgoing edges to the list
    if (G->Dir == 1)
    {
        linkedEdges = getOutAdjs(G, u);
    }
    // If the graph is undirected, combine both incoming and outgoing edges
    else
    {
        ListNode *inAdjs = GetInAdjs(G, u);
        ListNode *outAdjs = getOutAdjs(G, u);

        // Concatenate the incoming and outgoing adjacent vertices lists
        if (outAdjs != NULL)
        {
            ListNode *current = outAdjs;
            while (current->next != NULL)
            {
                current = current->next;
            }
            current->next = inAdjs;
            linkedEdges = outAdjs;
        }
        else
        {
            linkedEdges = inAdjs;
        }
    }

    // Return the combined list of adjacent vertices
    return linkedEdges;
}

int HasEdges(graph *G, int u, int v)
{
    ListNode *current = G->arr[u].head; // goes throught the list to check if there is an ede(u,v)
    while (current != NULL)
    {
        if (current->target == v)
        {
            return 1; // the egde(u,v) was found
        }
        current = current->next;
    }
    return 0; // the egde(u,v) was NOT found
}

void AddDirectedEdge(graph *G, int u, int v)
{
    // checks if the provided graph is underrected
    if (G->Dir != 0)
    {
        fprintf(stderr, "Graph is underected\n");
        exit(EXIT_FAILURE);
    }

    // creates a new node for list G to vertex u
    ListNode *newNode = (ListNode *)malloc(sizeof(ListNode));
    newNode->target = v;
    newNode->next = G->arr[u].head;
    G->arr[u].head = newNode;

    G->edge++;
}

void AddUndirectedEdge(graph *G, int u, int v)
{

    // checks if the provided graph is directed
    if (G->Dir == 1)
    {
        fprintf(stderr, "Graph is directed\n");
        exit(EXIT_FAILURE);
    }

    // creates a new node for vertex v in the adjacency list of vertex u
    ListNode *newNodeU = (ListNode *)malloc(sizeof(ListNode));
    newNodeU->target = v;
    newNodeU->next = G->arr[u].head;
    G->arr[u].head = newNodeU;

    G->edge++;
}

int main()
{

    //====================G1===========================
    // creates a graph with 9 vertices
    graph *G = CreateGraph(10);

    // vertex 1 connection(index 0)
    AddUndirectedEdge(G, 1, 5);
    AddUndirectedEdge(G, 1, 8);
    AddUndirectedEdge(G, 1, 4);
    // vertex 2 connection (index 1)
    AddUndirectedEdge(G, 2, 5);
    AddUndirectedEdge(G, 2, 6);
    AddUndirectedEdge(G, 2, 7);
    AddUndirectedEdge(G, 2, 9);
    // vertex 3 connection (index 2)
    AddUndirectedEdge(G, 3, 9);
    AddUndirectedEdge(G, 3, 8);
    AddUndirectedEdge(G, 3, 4);
    // vertex 4 connection (index 3)
    AddUndirectedEdge(G, 4, 3);
    AddUndirectedEdge(G, 4, 8);
    AddUndirectedEdge(G, 4, 1);
    // vertex 5 connection (index 4)
    AddUndirectedEdge(G, 5, 1);
    AddUndirectedEdge(G, 5, 2);
    // vertex 6 connection (index 5)
    AddUndirectedEdge(G, 6, 2);
    AddUndirectedEdge(G, 6, 9);
    // vertex 7 connection (index 6)
    AddUndirectedEdge(G, 7, 2);
    AddUndirectedEdge(G, 7, 9);
    // vertex 8 connection (index 7)
    AddUndirectedEdge(G, 8, 1);
    AddUndirectedEdge(G, 8, 4);
    AddUndirectedEdge(G, 8, 3);
    // vertex 9 connection (index 8)
    AddUndirectedEdge(G, 9, 2);
    AddUndirectedEdge(G, 9, 6);
    AddUndirectedEdge(G, 9, 7);
    AddUndirectedEdge(G, 9, 3);

    // printing the adjecency list of every vertex
    printf("Adjacecny List of the Graph(G1):\n");

    for (int i = 1; i < G->Ver; i++)
    {
        printf("Node %d: ", i);
        ListNode *current = G->arr[i].head;
        while (current != NULL)
        {
            printf("%d ", current->target);
            current = current->next;
        }
        printf("\n");
    }
    //============================G2=================================

    graph *H = CreateGraph(10);

    // vertex 1 connection
    AddDirectedEdge(H, 1, 5);
    AddDirectedEdge(H, 1, 8);
    AddDirectedEdge(H, 1, 4);
    // vertex 2 connection
    AddDirectedEdge(H, 2, 9);
    AddDirectedEdge(H, 2, 5);
    // vertex 3 connection
    AddDirectedEdge(H, 3, 4);
    // vertex 4 connection
    AddDirectedEdge(H, 4, 8);
    // vertex 5 connection
    AddDirectedEdge(H, 5, 0);
    // vertex 6 connection
    AddDirectedEdge(H, 6, 2);
    // vertex 7 connection
    AddDirectedEdge(H, 7, 2);
    AddDirectedEdge(H, 7, 9);
    // vertex 8 connection
    AddDirectedEdge(H, 8, 1);
    AddDirectedEdge(H, 8, 3);
    // vertex 9 connection
    AddDirectedEdge(H, 9, 6);
    AddDirectedEdge(H, 9, 3);

    printf("Adjacecny List of the Graph(G2):\n");
    for (int i = 1; i < H->Ver; i++)
    {
        printf("Node %d: ", i);
        ListNode *current = H->arr[i].head;
        while (current != NULL)
        {
            printf("%d ", current->target);
            current = current->next;
        }
        printf("\n");
    }

    int vertex = 9;
    printf("Edges linked to vertex %d:\n", vertex);
    ListNode *linkedEdges = Getlinked(H, vertex);
    while (linkedEdges != NULL)
    {
        printf("(%d, %d)\n", vertex, linkedEdges->target);
        linkedEdges = linkedEdges->next;
    }
    return 0;
}