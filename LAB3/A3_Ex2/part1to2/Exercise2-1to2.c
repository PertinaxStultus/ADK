#include <stdio.h>
#include <stdlib.h>

typedef struct myBST
{
    int key;
    struct myBST* right;
    struct myBST* left;
    struct myBST* parent;
} myBST;

myBST* TREE_search(myBST* x, int k)
{
    while (x != NULL && k != x->key) {      // gose throught the tree until it finds the key and untill the tree ends
        if (k < x->key) {                   // checks if the key is smaller then the current nodes key
            x = x->left;                    //in that case the function moves to the left child
        } else {
            x = x->right;                   //else it movs to the rigth child
        }
    }
    return x;                               //returns the up seachred node and null if no node was found
}

myBST* TREE_minimum(myBST* x)
{
    while(x->left != NULL)      // gose throught all the left ndoes i.e the nodes with the smalles value 
    {
        x = x->left;            //moves to the next left node
    }
    return x;                   //retuns the a pointer to the smallets node in the tree
}

myBST* TREE_maximum(myBST* x)
{
    while(x->right != NULL) //gose throught all the right nodes i.e the nodes with the gratest valuse 
    {
        x = x->right;   //moves to the next right node
    }
    return x;           // retutns the pointer to the node with the gratest value in the tree
}

myBST* TREE_predecessor(myBST* x)
{
    if(x->left != NULL)                     //checks if the left subb tree of the given tree is not empty
    {
        return TREE_maximum(x->left);       //returns the maximum key in the left sub tree
    }
    else                                     
    {
                                            //if the self sub tree is empty
        myBST* y = x->parent;               //finds the finds parent that is a right child of its parent 
        while(y != NULL && x == y->left)
        {
            // moves throught the tree until x is no longer a left child 
            x = y;
            y = y->parent;
        }
        return y;                       //retunrs the predecessor node
    }
}

void TREE_insert(myBST** T, myBST* z)
{
    // makes pointers for the function
    myBST* x = *T;                    // the current node beeing looked at
    myBST* y = NULL;                 //the pointer to keep track of the parent node

    while(x != NULL)            //gose throught the tree until it fineds a good spot for the insertion
    {
        y = x;                  //updates y to be the current node x
        if(z->key < x->key)     //if the key value of the insetion key is less then the current key then move left
        {
            x = x->left;
        }
        else            // if it has a grater value it moves to the right
        {
            x = x->right;
        }
    }
    // palaces the new node in the appropiate position of the tree
    z->parent = y;          //set the parent of the new node  
    if(y == NULL)          
    {
        *T = z; // if the tree was empty make the new tree the root of the tree
    }
    else if(z->key < y->key)
    {
        y->left = z;        // if the key of the new node is less then the key of the parent inset it as the left child
    }
    else
    {
        y->right = z;       // else if the new node is grater or equal to then the key of the parent set is as the right child 
    }
}

void transplant(myBST** T, myBST* u, myBST* v)
{
    if(u->parent == NULL)           // repalses the "u node" with the "v node"
    {
        *T = v;                     // if "u" is the root of the tree then set "v" as the new root
    }
    else if(u == u->parent->left)
    {
        u->parent->left = v;        // if "u" is the left child then set "v" as the left child of "u" parent
    }
    else
    {
        u->parent->right = v;       // if "u" is a right child then set "v" as the right child if "u" parent
    }
    if(v != NULL)                   // if "v" is not NULL then change the parent's pointer to "u" parent
    {
        v->parent = u->parent;
    }
}

void TREE_delete(myBST** T, myBST* z)
{
    if(z->left == NULL)
    {
        transplant(T, z, z->right);     // if z has no left child then replase z with its right child
    }
    else if(z->right == NULL)
    {
        transplant(T, z, z->left);      //if z has no right child then replase z with its left child
    }
    else
    {
        myBST* y = TREE_predecessor(z);
        if(y->parent != z)
        {
            transplant(T, y, y->right);     //if y is not the next child to z then replace y with its right child
            y->right = z->right;            //changes y right child to be z new right child
            y->right->parent = y;           //changes the parent pointer of y right child
        }
        transplant(T, z, y);            //transpalnts z with  z
        y->left = z->left;              //changes y left child to be z new left child
        y->left->parent = y;            //changes the parent pointer of y left child
    }
}

void inorder_tree_walk(myBST* x)
{
    if(x != NULL)       // if x(the current node) is not NULL
    {
        inorder_tree_walk(x->left);     //gose throught the left sub tree
        printf("%d ", x->key);          // prints out the key of x(the current node)
        inorder_tree_walk(x->right);   //gose throught the right sub tree
    }
}
//===========================Function for the height of our BST==============================
int TREE_height(myBST* root)
{   
    //if the tree is empty
    if(root == NULL)
    {
        return -1;  // height of a empty tree is -1(Taken from A3 description)
    }
    else
    {
        //calculates the height if the two sub trees(the tree branches)
        int left_branch = TREE_height(root->left);
        int right_branch = TREE_height(root->right);


        return (left_branch > right_branch ? left_branch : right_branch)+1;   //returns the height of the sub trees (unclear if node counds in that case awser +1) 
    }
}
//===========================================================================================


//=============================Balance factor for our BST====================================
int BF(myBST* root)
{
    if(root == NULL)
    {
        return 0;
    }
    else
    {
        int left_branch = TREE_height(root->left);
        int right_branch = TREE_height(root->right);

        return abs(left_branch - right_branch);
    }
}
//============================================================================================

int main()
{
        myBST* root = NULL;                                  // creates a BST root

    int ID[] = {2, 0, 0, 4, 0, 9, 2, 1, 7, 2, 7, 1};        // makes an array with elemnets to be inserted into the BST
    int num_count = sizeof(ID) / sizeof(ID[0]);             // calculats how many elements there are in the array
    for(int i = 0; i < num_count; i++)                      // gose throught elevry element int the ID array
    {
        myBST* newNode = (myBST*)malloc(sizeof(myBST));     // allocates memory for the new nodes 
        newNode->key = ID[i];                           //sets the key of the new node to each element in the provided array(ID)
        newNode->left = NULL;                           //sets the left child pointer to the new node to NULL
        newNode->right = NULL;                          //sets the right child pointer to the new node to NULL
        newNode->parent = NULL;                         //sets the parent pointer of the new node to null
        TREE_insert(&root, newNode);                    //puts the new node in to the BST 
    }
    printf("BST(sorted): ");
    inorder_tree_walk(root);                                    //prints out the trees nodes in a sorted manner
    printf("\n");
    printf("height: %d\n",TREE_height(root));                   //the height of the BST
    printf("The balance facor of our tree: %d",BF(root));       //balace Balance factor 
}