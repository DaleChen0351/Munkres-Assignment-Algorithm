#include <iostream>


#define MAX_VAL (9999)
#define numRows 3
#define numCols 3








typedef float ElemType;
class HungarianMatch
{
public:
    HungarianMatch(const int rows, const int columns);
    ~HungarianMatch();
    void Process(ElemType inMatrix[][numCols] , bool outMatch[][numCols]);

    void clear_mask();
    void clear_covers();

    //show hungarian info
    void showCostMatrix();
    void showMaskMatrix();
    void show_rowCover();
    void show_colCover();

private:
    ElemType** m_matrix;
    int** m_mask;
    int m_rows;
    int m_cols;
    int m_step;

    bool* rowCover;
    bool* colCover;
    
    //
    void step_one();
    void step_two();
    void step_three();
    void step_four();
    void step_five();
    void step_six();
    void step_seven();
    

    void run();

    //resize
    void resize(const int rows, const int columns, const ElemType default_value);

    void find_a_zero(int& row, int& col);

    bool star_in_row(int row);

    void find_star_in_row(int row, int& col);

    void find_prime_in_row(int row, int& col);

    void augment_path();

    void clear_path();

    void erase_primes();

    void find_smallest(int& minVal);

    

    void find_star_in_col(int col, int& row);

    
    


};