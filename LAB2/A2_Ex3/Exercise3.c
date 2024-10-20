#include <stdio.h>
#include <stdlib.h>

void insertion(int A[],int n)
{
    for(int i = 1; i<n; i++)    //goes through the array starting from the given element 
    {
       int key = A[i];          //stores the current element in the variable called key
       int j = i -1;            //makes a variable called j and sets it to the previous curent element

       while(j >= 0 && A[j]>key) 
       {
        A[j+1] = A[j];          //sets index j to the index in front of it
        j = j -1;               //decreases j to check the previous element in the next iteration 
       }
       A[j+1] = key;            //moves the key one step ahead in the provided sequence
    }
}//bytte 0 till 1 i for loopne och > till >= i while loopen

void mergeARR(int A[],int p,int q,int r)
{
//calculates the size of the right and left sub arrays
int Nl = q-p+1;         //the size of the left sub array
int Nr = r-q;           //the size of the right sub array

int L[Nl];          //creates a temporary array for the left sub array
int R[Nr];          //creates a temporary array for the right sub array

for (int i = 0;i < Nl;i++)          //puts the elements form the A array into the left sub array
{
    L[i]=A[p+i];            //puts elements from A[p] and A[q] into L[]
}

for (int j= 0;j<Nr;j++)             //puts the elements form the A array into the right sub array
{
    R[j]=A[q+j+1];
}

//creates the necessary variables for the merge procedure  
int i = 0;          //indexes the smallest remaining element in L
int j = 0;          //indexes the smallest remaining element in R
int k = p;          //indexes the location in A to fill
//merges thg  two sub arrays back into the A array

while(i < Nl && j < Nr)
{    //compares the elements form the left and right sub array 
    if(L[i]<=R[j])          //if the elements from the left sub array is smaller och equal copy the elements in to the A array 
    { 
        A[k] = L[i];
        i++;
    }
    else                    //if the elements from the right sub array is smaller och equal copy the elements in to the A array
    {               
        A[k] = R[j];
        j++;
    }
    k++;            //moves to the next key in the array A
} 
while(i <Nl)        //copes the remaining elements from the left sub array in to the A array
{
    A[k] = L[i];
    i++;
    k++; 
}

while(j <Nr)        //copes the remaining elements from the right sub array in to the A array
{
    A[k] = R[j];
    j++;
    k++;
}

}// bytte Nl till Nr i sista while-loopen

void merge(int A[], int p,int r)
{
    if(p>= r)       // if there is to few elements the function will not run
    {
        return;
    }


    int q = (p+r)/2;    //Mid point for the provided array
    merge(A,q,p);       //sorts the left half id the array form index p to index q

    merge(A,q+1,r);     //sorts the right half id the array form index q+1 to index r

    mergeARR(A,p,q,r);  //merges the two haves of the provided array

} 

int partition(int A[],int p,int r)
{
    int x = A[r];       // the pivot of the array
    int i = p-1;        //hightest index of the low partition of the array

    for(int j = p;j <= r-1; j++)    //handles each element in the array except the pivot
    {
        if(A[j] <= x)           //looks if the elements belongs into the lower partition of the array
        {
            i++;                //makes a new slot in the low partition
            //puts the elements in the low partition of the array
            int temp = A[i];    
            A[i] = A[j];
            A[j] = temp;
        }
    }
        //The pivot goes to the right side of the low partition
        int temp = A[i+1];
        A[i +1] = A[r];
        A[r] = temp;

    return i+1;     //the new index of the pivot
}

void quickSort(int A[], int p , int r)
{
    
    if (p < r)
    {
        int q = partition(A,p,r);           //partition the sub array around the pivot so it ends up in A[q]
        quickSort(A,p,q-1);                 //sorts the low partition
        quickSort(A,q+1,r);                 //sorts the hight partition
    }
    
}
// int function to look if everything compiles 
int main()
{
    int A[] = {3,1,5,10,8,7,5,2,9,6,1,2};       //makes the array A
    int n = sizeof(A) / sizeof(A[0]);           //calculates the amount of elements in array A

    int ID[] = {2,0,0,4,0,9,2,1,6,9,6,9};       //makes the array ID
    int m = sizeof(ID) / sizeof(ID[0]);         //calculates the amount of elements in array ID

    printf("=============A=============\n");
    //prints out array A after it got sorted by the Insertion algorithm
    printf("insertion: ");
    insertion(A,n);
    for( int i = 0; i < n;i++)
    {
        printf("%d",A[i]);
    }
    printf("\n");

    //prints out array A after it got sorted by the merge algorithm
    printf("merge: ");
    merge(A,0,n-1);
    for( int i = 0; i < n;i++)
    {
        printf("%d",A[i]);
    }
    
    printf("\n");

    //prints out array A after it got sorted by the quickSort algorithm
    printf("quickSort: ");
    quickSort(A,0,n-1);
    for( int i = 0; i < n;i++)
    {
        printf("%d",A[i]);
    }
    printf("\n");

    printf("=============ID=============\n");

    //prints out array ID after it got sorted by the insertion algorithm
    printf("insertion(ID): ");
    insertion(ID,m);
    for( int i = 0; i < m;i++)
    {
        printf("%d",ID[i]);
    }
    printf("\n");


    //prints out array ID after it got sorted by the merge algorithm
    printf("merge(ID): ");
    merge(ID,0,m-1);
    for( int i = 0; i < m;i++)
    {
        printf("%d",ID[i]);
    }
    printf("\n");

    //prints out array ID after it got sorted by the quickSort algorithm

    printf("quickSort(ID): ");
    quickSort(ID,0,m-1);
    for( int i = 0; i < m;i++)
    {
        printf("%d",ID[i]);
    }
    printf("\n");
    printf("============================\n");
    return 0;

}