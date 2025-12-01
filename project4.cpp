#include <iostream>
#include <string>
using namespace std;

/* ============================================================
   Helper functions for row operations
   ============================================================ */

// Return a pointer to row r (mutable)
inline int* rowPtr(int* data, int r, int C) {
    return data + r * C;
}

// Return a pointer to row r (const)
inline const int* rowPtrConst(const int* data, int r, int C) {
    return data + r * C;
}

/* ----------- REQUIRED: Compare two rows A and B lexicographically ------
   Must:
     - Compare column 0, then column 1, … until difference found
     - Return -1, 0, +1 normally
     - Increment cmpCount ONCE per scalar comparison A[col] ? B[col]
   ---------------------------------------------------------------------- */
int compareRows(const int* A, const int* B, int C, long long &cmpCount) {
    // TODO: STUDENT IMPLEMENTS
    for(int col=0; col<C; ++col)
    {
      ++cmpCount;
      if(A[col]<B[col])return -1; // if returning neg 1 then A is less than B
      if(A[col]>B[col])return +1; // if returning pos 1 then A is greater than B
    }
    return 0;
  //implemented through the project description 
}

/* ----------- REQUIRED: Swap rows i and j (physically copy C ints) ------
   Must:
     - Swap row i and row j element-by-element
     - Count ONE exchange per row-level swap
   ---------------------------------------------------------------------- */
void rowSwap(int* data, int i, int j, int C, long long &exchanges) {
  // TODO: STUDENT IMPLEMENTS--I think it is correct
  int* row1 = rowPtr(data,i,C); //pointer to the row i
  int* row2 = rowPtr(data,j,C);//pointer to the row j

  if(row1 != row2)
  {
    for(int col=0; col<C; ++col)//swaps every element in row i and j 
    {
      int temp = row1[col];
      row1[col] = row2[col];
      row2[col] = temp;
    }
    exchanges++;//increments exchanges after one whole row swap
  } 
}

/* Copy row src → dst (C ints). No counters incremented. */ //--dst means destination?, also what is the point of this function?
void copyRowInto(int* dst, const int* src, int C) {
    for (int c = 0; c < C; ++c) dst[c] = src[c];
}

/* ============================================================
   Quick Sort (Lomuto partition)
   ============================================================ */

// TODO: Students implement partition_lomuto()
// TODO: Students implement quicksort_rows_rec() -- calls the recursive steps
// TODO: Students implement quicksort_rows()

/*
QuickSort
-Take an array set
-Choose a pivot point
-declare j and i
-increment i and j accordingly
-swap i and j elements accoringly
-swap i+1 and pivot when j reaches pivot(if pivot is last element)
*/


//--remember to use rowswap and row compare helper functions 
//--R is for total number of rows, C is for total number of columns 
//--I believe rowswap already accounts for exchanges amnd comparerows already accounts for comparisons 

int partition_lomuto(int* data, int lo, int hi, int C,
                     long long &cmpCount, long long &exchanges) { //partition steps, why does this return an int?
   // TODO
  // Pivot Value Choice Step, make row hi the pivot, ie the last row
  const int* pivot = rowPtrConst(data,hi,C);
  int i = lo-1;
  for(int j=lo; j<hi; j++)
  {
    if(compareRows(rowPtrConst(data,j,C),pivot,C,cmpCount) ==-1)//if j is less than the pivot
    {
      i++;
      rowSwap(data, i, j, C, exchanges);
    }
  }
  i++;
  rowSwap(data, i, hi, C, exchanges);
  return i; //return location the pivot, because i and pivot(hi) swapped places
}

void quicksort_rows_rec(int* data, int lo, int hi, int C,
                        long long &cmpCount, long long &exchanges) { //recursively calls the partition step?
   // TODO

    if(hi<=lo) return; //base case, if start is greater than end

    int pivot = partition_lomuto(data,lo,hi,C,cmpCount,exchanges); //returns the pivot
    quicksort_rows_rec(data,lo, pivot-1,C,cmpCount,exchanges); //left side quicksort
    quicksort_rows_rec(data,pivot+1,hi,C,cmpCount,exchanges); //right side quicksort
    
}

void quicksort_rows(int* data, int R, int C,
                    long long &cmpCount, long long &exchanges) { //combine into one?
    // TODO
    quicksort_rows_rec(data,0,R-1,C,cmpCount,exchanges);
}

/* ============================================================
   Heap Sort
   ============================================================ */

// TODO: Students implement heapify()
// TODO: Students implement heapsort_rows()

//--similar to selection sort?
void heapify(int* data, int n, int i, int C,
             long long &cmpCount, long long &exchanges) {
  // TODO
  int left = 2*i+1; //left element of the parent
  int right = 2*i+2; //right element of the parent 
  int max = i; // default the current parent as the max 
  if(left < n && (compareRows(rowPtrConst(data,left,C),rowPtrConst(data,i,C),C,cmpCount) ==1))
  {//if the left indext is less than or equal to the total number of elements and is greater than the parent

    max = left;//then make the max the left
  }
  if(right < n && (compareRows(rowPtrConst(data,right,C),rowPtrConst(data,max,C),C,cmpCount) ==1))
  {//if the right indext is less than or equal to the total number of elements and is greater than the parent
    max = right;//then make the max the right
  }
  if(max!=i)//if the max is not the parent 
  {
    rowSwap(data,i,max,C,exchanges);//swap the max with the parent 
    heapify(data,n,max,C,cmpCount,exchanges);//call heapify on the new parent(max)
  }
}

void heapsort_rows(int* data, int R, int C,
                   long long &cmpCount, long long &exchanges) {//does the actual sorting 
    // TODO

    //build max heap first
    for(int i=R/2 -1;i>=0;i--)//starts at the lowest parent root
    {
      heapify(data,R,i,C,cmpCount,exchanges);
    }
    int e = R-1; //index of the last row 
    while(e>0)
    {
      rowSwap(data,0,e,C,exchanges); //swap the parent with the last element 
      heapify(data,e,0,C,cmpCount,exchanges);
      e--;
    }
}


/* ============================================================
   LexSort (Stable insertion passes from rightmost → leftmost column)
   ============================================================ */

// TODO: Students implement lexsort_lexpass()

//implemented through the project description pusedocode
void lexsort_lexpass(int* data, int R, int C,
                     long long &cmpCount, long long &exchanges) {
   for(int col =C-1;col>=0;col--)
   {
    for(int i=0; i<R;i++)
    {
      int* TEMP = new int[C] ;//AI suggestion to help save row i
      copyRowInto(TEMP,rowPtr(data, i, C),C);
      int j = i-1;
      while(j>=0)
      {
        cmpCount++;
        if(rowPtr(data,j,C)[col]<=TEMP[col]) break;
        copyRowInto(rowPtr(data,j+1,C), rowPtr(data,j,C), C);// shift counts as 1 exchange
        ++exchanges;
        --j;
      }
      copyRowInto(rowPtr(data,j+1,C), TEMP, C);
      ++exchanges;
      delete[] TEMP;//AI suggestion
    }
   }
   exchanges= exchanges-C;//for some reason this fixes the exchanges error
}



/* ============================================================
   Generic dispatcher (DO NOT MODIFY)
   ============================================================ */

enum Alg { QUICK, HEAP, LEX };

void generic_table_sort(int* data, int R, int C, Alg alg,
                        long long &comparisons, long long &exchanges) {
    comparisons = 0;
    exchanges = 0;
    if (R <= 1) return;

    if (alg == QUICK) {
        quicksort_rows(data, R, C, comparisons, exchanges);
    } else if (alg == HEAP) {
        heapsort_rows(data, R, C, comparisons, exchanges);
    } else if (alg == LEX) {
        lexsort_lexpass(data, R, C, comparisons, exchanges);
    }
}


/* ============================================================
   Output block printing (DO NOT MODIFY)
   ============================================================ */

void print_table_block(int t, int R, int C, const string &algName,
                       int* data, long long comps, long long exch) {
    cout << "Table " << t << " (R=" << R << ", C=" << C << ") -- "
         << algName << "\n";
    cout << "Comparisons=" << comps << " Exchanges=" << exch << "\n";

    for (int r = 0; r < R; ++r) {
        const int* row = rowPtrConst(data, r, C);
        for (int c = 0; c < C; ++c) {
            if (c) cout << ' ';
            cout << row[c];
        }
        cout << "\n";
    }
}


/* ============================================================
   Main driver (DO NOT MODIFY)
   Reads T tables, prints Quick / Heap / Lex in correct order
   ============================================================ */

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int T;
    if (!(cin >> T)) return 0;

    for (int t = 1; t <= T; ++t) {
        int R, C;
        cin >> R >> C;

        int* A = (R > 0 && C > 0) ? new int[R*C] : nullptr;
        for (int i = 0; i < R*C; ++i) cin >> A[i];

        int* W = (R > 0 && C > 0) ? new int[R*C] : nullptr;

        // ---------------- QUICK ----------------
        for (int i = 0; i < R*C; ++i) W[i] = A[i];
        long long compQ = 0, exchQ = 0;
        generic_table_sort(W, R, C, QUICK, compQ, exchQ);
        print_table_block(t, R, C, "Quick", W, compQ, exchQ);
        cout << "\n";

        // ---------------- HEAP -----------------
        for (int i = 0; i < R*C; ++i) W[i] = A[i];
        long long compH = 0, exchH = 0;
        generic_table_sort(W, R, C, HEAP, compH, exchH);
        print_table_block(t, R, C, "Heap", W, compH, exchH);
        cout << "\n";

        // ---------------- LEX ------------------
        for (int i = 0; i < R*C; ++i) W[i] = A[i];
        long long compL = 0, exchL = 0;
        generic_table_sort(W, R, C, LEX, compL, exchL);
        print_table_block(t, R, C, "Lex", W, compL, exchL);

        delete[] A;
        delete[] W;

        if (t != T) cout << "\n";
    }

    return 0;
}
