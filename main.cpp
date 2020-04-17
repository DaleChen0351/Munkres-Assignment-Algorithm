#include "hungarian.h"



int main()
{
    HungarianMatch hung(numRows,numCols);
    hung.clear_covers();
    hung.clear_mask();

    float test[numRows][numCols] = {
        {1,2,3},
        {2,4,6},
        {3,6,9} // 3 4 3   min 10

    };
   
    bool match_b[numRows][numCols] = {0};
    
    hung.Process(test,match_b);
    hung.showMaskMatrix();
    

}