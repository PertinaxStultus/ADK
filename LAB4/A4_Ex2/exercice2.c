#include <stdio.h>
#include <stdlib.h>

typedef enum {
   WHITE,
   GRAY,
   BLACK
} Color;

//struct for a node in a adjacency list
typedef struct ListNode
{
   int target;         // The targeted vertex
   struct ListNode* next;    //pointer to the next adjacent vertex
   Color color;
}ListNode;

//struct for the adjacency list for a vertex
typedef struct List
{
    struct ListNode* head;    //pointer to the head of the list

}List;

//struct for the graph representation
typedef struct graph
{
   int Ver;       //number of verteses in the graph
   int edge;      //The number of edges in the graph
   int Dir;       //To inicate if our graph is directed
   struct List* arr;  // array for our adjacent list with the size 'Ver'
}graph;

//=================Imort from A1============================
typedef struct Queue
{
   int* distances;
   int* elements; 
   int size; 
   int head;
   int tail;
}Queue;

//=======================================================
graph* CreateGraph(int n, int directed)
{

   graph* NewGraph = (graph*)malloc(sizeof(graph));   //allocates memory for the graph
   if(NewGraph == NULL)
  {
     exit(EXIT_FAILURE);   //function stops if memory fails to allocate 
  }

   NewGraph->Ver = n;     // the number of verteses in the graph
   NewGraph->Dir = directed; // set the graph type

   NewGraph->arr = (List*)malloc(n * sizeof(List));  //allocates memory for the array for the adjacency list
   if(NewGraph->arr == NULL)
  {
     exit(EXIT_FAILURE); //function stops if memory fails to allocate
  }

   for(int i = 0;i<n;i++)
  {
     NewGraph -> arr[i].head = NULL;
  }

   return NewGraph;
}

// a function that checks how may vertices a graph has
int GetNumVertices(graph *G)
{
   if(G == NULL)
  {
     printf("The graph has no vertices");    //checks if the graph as no verties, in that case returns 1(FAIL)
     return 1;
  }

   return G -> Ver;   //returns the number of vertices in the provided graph G
}



// a function that checks how may edges a graph has
int GetNumEdges(graph *G)
{
   if(G == NULL)
  {
     printf("The graph has no egdes");    //checks if the graph as no edegs, in that case returns 1(FAIL)
     return 1;
  }

   return G -> edge;  //returns the number of edges in the provided graph G
}


int IsDirected(graph* G)
{
   return G->Dir;
}



ListNode* GetInAdjs(graph* G, int u)
{
     ListNode* AdjVer = NULL;     //makes a list that sores the adjacency vertices

   for(int i = 0; i < G->Ver ;i++)   //goes trought the vertices to find the adjacent vertices of "u"
  {
     ListNode* current = G->arr[i].head;
     while(current != NULL)
    {
       if(current -> target == u)
      {
         ListNode* newNode = (ListNode*)malloc(sizeof(ListNode));   //adds the vertex i to the adjacency list go to "u" 
         newNode->target = i;
         newNode->next = AdjVer;
         AdjVer = newNode;
         break;        //break to move to the next vertex
      }
       current = current->next;
    }
  }
   return AdjVer;
}

ListNode* getOutAdjs(graph* G, int u)
{
   ListNode* outAdjs = NULL;  // a list that sores rhe outgoing adjecent vertices 
   ListNode* current = G->arr[u].head;
   while(current != NULL)
  {
     ListNode* newNdoe = (ListNode*)malloc(sizeof(ListNode));
     newNdoe->target = current->target;
     newNdoe->next = outAdjs;
     outAdjs = newNdoe;
     current = current->next;
  }
   return outAdjs;
}

int HasEdges(graph* G , int u, int v)
{
   ListNode* current = G->arr[u].head;   //goes throught the list to check if there is an ede(u,v)
   while(current != NULL)
  {
     if(current->target ==v)
    {
       return 1;  //the egde(u,v) was found
    }
  }
   return 0;  //the egde(u,v) was NOT found
}



void AddDirectedEdge(graph* G, int u, int v)
{
   //checks if the provided graph is directed
   if(G->Dir != 1)
  {
     fprintf(stderr,"Graph is undirected\n");
     exit(EXIT_FAILURE);
  }
   //creates a new node for list go to vertex v
   ListNode* newNode = (ListNode*)malloc(sizeof(ListNode));
   newNode->target = v;
   newNode->next = NULL;
   // If the adjacency list is empty, set the new node as the head
   if (G->arr[u].head == NULL) {
     G->arr[u].head = newNode;
  } else {
     // Otherwise, append the new node to the end of the adjacency list
     ListNode* current = G->arr[u].head;
     while (current->next != NULL) {
       current = current->next;
    }
     current->next = newNode;
  }
   // Update the number of edges in the graph
   G->edge++;
}

//===================enqueue & dequeue (from A1)==================================
Queue* CreateQueue(int size) 
{
   Queue* Q = (Queue*)malloc(sizeof(Queue));  // Allocates memory for the queue
   if (Q == NULL) 
  {
     exit(EXIT_FAILURE); // Function stops if memory fails to allocate
  }

   Q->elements = (int*)malloc(size * sizeof(int));  // Allocates memory for the array of elements
   if (Q->elements == NULL) 
  {
     exit(EXIT_FAILURE); // Function stops if memory fails to allocate
  }
   Q->size = size; // Sets the size of the queue
   Q->head = 0;   // Initializes the head
   Q->tail = 0;   // Initializes the tail
   return Q;
}

void enqueue(Queue* Q, int x)
{
   Q->elements[Q->tail] = x;
   if(Q->tail == Q->size -1 )
  {
     Q->tail=0; 
  }
   else
  {
     Q->tail = Q->tail +1;
  }
}

int dequeue(Queue* Q)
{
   int x = Q->elements[Q->head];
   if(Q->head == Q->size -1)
  {
     Q->head=0;
  }
   else
  {
     Q->head = Q->head+1;
  }
   return x;
}
//===========================================================================
int isEmpty(Queue* Q) {
   return Q->head == Q->tail;
}

void BFS(graph* G, int s) {
    //allcoerar minne för "distance" fältet och sätter till 0
    int* distances = (int*)malloc(G->Ver * sizeof(int));
    for (int i = 0; i < G->Ver; i++) {
        distances[i] = 0;
    }
    // målar alla nåder vita(WHITE)
    int nodes[9];
    for (int u = 0; u < G->Ver; u++) {
        nodes[u] = 0;
     /*ListNode* node = G->arr[u].head;
     while (node != NULL) 
    {
       node->color = WHITE;
       node = node->next;
    }*/
    }
    Queue* Q = CreateQueue(G->Ver);
    enqueue(Q, s);
    // markera källnoden som GRAY och sen distansen till 0 
    nodes[s - 1] = 1;
    distances[s - 1] = 0;//här sätts inte nod s till grå, utan det är den första noden i nod s adjacency list som blir grå, så för s = 9 blir nod 3 grå
                        //ändrat från G->arr[s].head->color till att använda array, s-1 pga nollindexering

   /*
   LÄS ALLA ANDRA KOMMENTARER FÖRST FÖR ATT FÅ ÖVERBLICK ÖVER PROBLEMEN
   På grund av sättet er graf är uppbyggd blir detta lite krångligt. När ni lägger till kanter skapas en ny nod. Därför kommer vi ha flera noder med samma värde. Detta är inte fel
   men det blir krångligt när vi ska färgmarkera noderna. För ni kan tänka er att när vi tex gråmarkerar nod 3 när vi börjar med s = 9 så är det bara nod 3 i adj.list för 9 som
   gråmarkeras, men nod 3 kan finnas i flera kopior i andra adj.list (tex hos nod 8). 

   Alltså:
   adj.list[9] = {3, 6}
   adj.list[8] = {1, 3}

   nod 3 i 9 != nod 3 i 8 rent minnesmässigt, så om man gråmarkerar i adj.list[9] blir den inte gråmarkerad i adj.list[8]

   Sen har vi även problemet att ni inte har något bra sätt att komma åt en nod. Head-pekaren ger er den första noden i adj.list men inte själva noden som listan tillhör. Alltså ger
   tex adj.list[9].head = nod 3. Ni kan om ni vill skriva en sorts search-funktion som plockar fram noden ni faktiskt vill färgmarkera.

   Det jag rekommenderar (och det jag gjort) är att gå på index istället i en vanlig array. Så vi har nodes[9] och lagrar markeringen i den. Med 0 = vit, 1 = grå, 2 = svart skulle
   arrayen till att börja med vara fylld av nollor, sedan när 9 besöks så blir det nodes[9] = {0,0,0,0,0,0,0,0,1} (9 grå) osv. Denna array kan användas i alla if-satser istället för 
   de faktiska noderna
   */
    while (!isEmpty(Q)) 
    {
        int u = dequeue(Q);
        printf("Processing vertex %d\n", u);

        if (G->arr[u].head == NULL)  //denna if-sats får lämnas orörd eftersom vi faktiskt kollar på minne
        {
            printf("Vertex %d has no outgoing connections\n", u);
            printf("Node marked black %d\n", u);
            nodes[u - 1] = 2;// Mark the vertex as visited, här är det inte huvudet som ska sättas som svart, utan noden själv
                                      //ändrat från G->arr[u].head->color till nodes[u-1]
            continue; // Skip processing further
        }

        ListNode* adj = G->arr[u].head;
        while (adj != NULL)
        {
            int v = adj->target;
            if (nodes[v - 1] == 0)//här vill vi inte kontrollera mot bara huvudet, utan använda adj som stegar igenom nodens adjacency list
                                //(jag har ändrat från G->arr[v].head->color till nodes[v-1] i villkoret), nollindexering 
            {
                printf("Visiting adjacent vertex %d\n", v);
                nodes[v - 1] = 1;//här sätts fel nod till grå, det är inte v utan den första i v:s adj.list som blir grå
                                       //jag har ändrat från G->arr[v].head->color till nodes[v-1] (och struntat i enumen, om ni fortfarande vill ha den kan ni ändra), nollindexering

                distances[v - 1] = distances[u - 1] + 1; // uppdaterar distansen från källan till v
                enqueue(Q, v);
            }
            adj = adj->next;
        }
      // Mark current vertex as BLACK when all its adjacent vertices are visited
    //här vill vi inte sätta listans huvud till svart, utan vi vill sätta noden som listan tillhör till svart
    //tex så är G->arr[u].head = nod 2 för u = 6. Det är nod 6 som ska svartmärkas, inte nod 2

      //G->arr[u].head->color = BLACK;
        nodes[u - 1] = 2;
        printf("Node marked black %d\n", u);

    //printf("Node marked black %d\n", G->arr[u].head->target);
    //printen ovan ger lite insikt i hur det går, vid körning syns det att fel nod svartmarkeras
    }

    // skiver ut distancsen från source noden v
    printf("Distances from source node %d:\n", s);
    for (int i = 0; i < G->Ver; i++)
    {
      if(i == s-1 || distances[i] != 0) // så programmet inte skriver ut kopplingar till nod 10
      {
         printf("Node %d: Distance = %d\n", i + 1, distances[i]);
      }
      
    }
    //frigör allokerat minne för "distances" fältet and "queue"
    free(distances);
    free(Q->elements);
    free(Q);
}
//===========================================================================

int main()

{
   // Create a directed graph with 10 vertices
   graph* G = CreateGraph(10, 1);
   // Add edges according to the provided connections
   // Node 1 connections: 4, 8, 5
   AddDirectedEdge(G, 1, 4);
   AddDirectedEdge(G, 1, 8);
   AddDirectedEdge(G, 1, 5);
   // Node 2 connections: 5, 9
   AddDirectedEdge(G, 2, 5);
   AddDirectedEdge(G, 2, 9);
   // Node 3 connection: 4
   AddDirectedEdge(G, 3, 4);
   // Node 4 connection: 8
   AddDirectedEdge(G, 4, 8);
   // Node 5 connection: 0
   //AddDirectedEdge(G, 5, 0); denna rad ger segfault där uppe, det finns en kant till 0 så if-satsen för att kolla om det inte finns kanter fungerar ej
   // Node 6 connection: 2
   AddDirectedEdge(G, 6, 2);
   // Node 7 connections: 9, 2
   AddDirectedEdge(G, 7, 9);
   AddDirectedEdge(G, 7, 2);
   // Node 8 connection: 3
   AddDirectedEdge(G, 8, 3);
   AddDirectedEdge(G, 8, 1);
   // Node 9 connections: 3, 6
   AddDirectedEdge(G, 9, 3);
   AddDirectedEdge(G, 9, 6);
   // Perform BFS from source vertex 1 then 9 
   BFS(G, 9);
   return 0;
}