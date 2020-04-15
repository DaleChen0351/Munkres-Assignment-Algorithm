#include <iostream>

#define numberRows (3)
#define numberCols (3)
#define MAX_VAL (9999)

int cost[nRows][nCols];
int mask[nRows][nCols];

bool rowCover[nRows];
bool colCover[nCols];
//temp
int path_row_0;
int path_col_0;
#define path_len (16)
int path[path_len][2];
int path_count;




class HungarianMatch
{
public:
    HungarianMatch(const int rows, const int cols);
    void process(float* inMatrix,float* outMatch,int& rows, int& cols);

private:
    int nRows;
    int nCols;
    


};