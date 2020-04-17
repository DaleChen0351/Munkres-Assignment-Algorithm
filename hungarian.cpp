#include "hungarian.h"
using namespace std;


//temp
int path_row_0;
int path_col_0;
#define path_len (16)
int path[path_len][2];
int path_count;



HungarianMatch::HungarianMatch(const int rows, const int columns)
{
    m_rows = rows;
    m_cols = columns;
    m_matrix = nullptr;
    rowCover = nullptr;
    colCover = nullptr;
    resize(rows,columns,(ElemType)0);

}

HungarianMatch::~HungarianMatch()
{
    if(m_matrix != nullptr)
    {
        for(int row = 0; row < m_rows; row++)
        {
            delete[]m_matrix[row];
        }

        delete[]m_matrix;
    }

    m_matrix = nullptr;

    if(colCover != nullptr)
    {
        delete[]colCover;
    }
    if(rowCover != nullptr)
    {
        delete[]rowCover;
    }

    rowCover = nullptr;
    colCover = nullptr;
}

void HungarianMatch::resize(const int rows, const int columns, const ElemType default_value)
{
    if(rows <= 0 || columns <= 0)
    {
        return;
    }
    //alloc new space 
    m_matrix = new ElemType*[rows];
    m_mask = new int*[rows];

    for(int row = 0; row < rows; row++)
    {
        m_matrix[row] = new ElemType[columns];
        m_mask[row] = new int[columns];
    }

    // alloc 
    rowCover = new bool[rows];
    colCover = new bool[columns];

    for(int row = 0; row < m_rows; row++)
    {
        for(int col = 0; col < m_cols; col++)
        {
            m_matrix[row][col] = default_value;
            m_mask[row][col] = 0;
        }
        rowCover[row] = false;
    }

    for(int col = 0; col < m_cols; col++)
    {
        colCover[col] = false;
    }
}

void HungarianMatch::Process(ElemType inMatrix[][numCols] , bool outMatch[][numCols])
{

    for(int row = 0; row < m_rows; row++)
    {
        for(int col = 0; col < m_cols; col++)
        {
            m_matrix[row][col] = inMatrix[row][col];
        }
    }
    
    run();

    
}


void HungarianMatch::step_one()
{
    int min_in_row;
    for(int row = 0; row < m_rows; row++)
    {
        min_in_row = m_matrix[row][0];
        for(int col = 0; col < m_cols; col++)
        {
            if(m_matrix[row][col] < min_in_row)
            {
                min_in_row = m_matrix[row][col];
            }
        }
        for(int col = 0; col < m_cols; col++)
        {
            m_matrix[row][col]-= min_in_row;
        }
    }

    m_step = 2;
}

void HungarianMatch::step_two()
{
    for(int row = 0; row < m_rows; row++)
    {
        for(int col = 0; col < m_cols; col++)
        {
            if(m_matrix[row][col] == 0 && rowCover[row] == false && colCover[col] == false)
            {
               m_mask[row][col] = 1;// only set a star entry for each row && col
               rowCover[row] = true;
               colCover[col] = true;
            }

        }
    }
    //clear
    for(int row = 0; row < m_rows; row++)
    {
        rowCover[row] = false;
    }
    for(int col = 0; col < m_cols; col++)
    {
        colCover[col] = false;
    }
    m_step = 3;
}

void HungarianMatch::step_three()
{
    int col_count;
    for(int row = 0; row < m_rows; row++)
    {
        for(int col = 0; col < m_cols; col++)
        {
            if(m_mask[row][col] == 1)
            {
                colCover[col] = true;
            }
        }   
    }
    col_count = 0;
    for(int col = 0; col < m_cols; col++)
    {
        if(colCover[col])
        {
            col_count+=1;
        }
    }

    if(col_count >= m_cols || col_count >= m_rows)
    {
        m_step = 7;
    }
    else
    {
        m_step = 4;
    }
    
}
// 在整张表中寻找一个未被竖线覆盖的零点，如果找到一个，则立即返回该点的横纵坐标
void HungarianMatch::find_a_zero(int& row, int& col)
{
    int r = 0;
    int c;
    bool done;

    row = -1;
    col = -1;

    done = false;
    //search by row
    while (!done)
    {
        c = 0;
        while (true)
        {
            //find a non-covered zero
            if(m_matrix[r][c] == 0 && rowCover[r] == false && colCover[c] == false)
            {
                row = r;
                col = c;
                done = true;
            }
            c+=1;
            if(c >= m_cols || done)
            {
                break;
            }
        }
        r+=1;
        if(r >= m_rows)
        {
            done = true;
        }     
    }
    
}
//确定某一行是否有标记了星号的节点
bool HungarianMatch::star_in_row(int row)
{
    bool retVal = false;
    for(int col = 0; col < m_cols; col++)
    {
        if(m_mask[row][col] == 1)
        {
            retVal = true;
        }
    }
    return retVal;
}

void HungarianMatch::find_star_in_row(int row, int& col)
{
    col = -1;
    for(int c = 0; c < m_cols; c++)
    {
        if(m_mask[row][c] == 1)
        {
            col = c;
        }
    }
}



void HungarianMatch::step_four()
{
    int row = -1;
    int col = -1;

    bool done;
    done = false;
    while (!done)
    {
        find_a_zero(row,col);//(0,1)
        if(row == -1)
        {
            done = true;
            m_step = 6;
        }
        else
        {
            m_mask[row][col] = 2;//prim :non-covered zero
            //如果找到的零点所在的行已经有了一个零点了
            if(star_in_row(row))
            {
                find_star_in_row(row,col);
                rowCover[row] = true;
                colCover[col] = false;

            }
            else
            {
                done = true;
                m_step = 5;
                //(1,0)
                path_row_0 = row;
                path_col_0 = col;
                
            }
            
        }
        
    }
    
}

void HungarianMatch::find_star_in_col(int col, int& row)
{
    row = -1;
    for(int r = 0; r < m_rows; r++)
    {
        if(m_mask[r][col] == 1)
        {
            row = r;
        }
    }

}

void HungarianMatch::find_prime_in_row(int row, int& col)
{
    for(int c = 0; c < m_cols; c++)
    {
        if(m_mask[row][c] == 2)
        {
            col = c;
        }
    }
}

void HungarianMatch::augment_path()
{
    for(int p = 0; p < path_count; p++)
    {
        int p_x = path[p][0];
        int p_y = path[p][1];
        if(m_mask[p_x][p_y] == 1)
        {
            m_mask[p_x][p_y] = 0;
        }
        else
        {
            m_mask[p_x][p_y] = 1;
        }   
    }
}
void HungarianMatch::clear_path()
{
    for(int p = 0; p < path_len; p++)
    {
        path[p][0] = -1;
        path[p][1] = -1;
    }
    path_row_0 = -1;
    path_col_0 = -1;
}

void HungarianMatch::clear_covers()
{
    //clear
    for(int row = 0; row < m_rows; row++)
    {
        rowCover[row] = false;
    }
    for(int col = 0; col < m_cols; col++)
    {
        colCover[col] = false;
    }
}

void HungarianMatch::erase_primes()
{
    for(int row = 0; row < m_rows; row++)
    {
        for(int col = 0; col < m_cols; col++)
        {
            if(m_mask[row][col] ==2)
            {
                m_mask[row][col] = 0;
            }
        }
    }
}


void HungarianMatch::step_five()
{
    bool done = false;
    int r = -1;
    int c = -1;

    path_count = 1;
    path[path_count-1][0] = path_row_0;
    path[path_count-1][1] = path_col_0;

    done = false;
    while(!done)
    {
        find_star_in_col(path[path_count-1][1],r);
        if(r > -1)
        {
            path_count +=1;
            path[path_count-1][0] = r;
            path[path_count-1][1] = path[path_count-2][1];
        }
        else
        {
            done = true;
        }
        if(!done)
        {

            find_prime_in_row(path[path_count-1][0],c);
            path_count +=1;
            path[path_count-1][0] = path[path_count-2][0];
            path[path_count-1][1] = c;
        }
        
    }
    augment_path();
    clear_covers();
    erase_primes();//primes均去除
    //?
    clear_path();
    m_step = 3;

}

void HungarianMatch::find_smallest(int& minVal)
{
    for(int row = 0; row < m_rows; row++)
    {
        for(int col = 0; col < m_cols; col++)
        {
            if(rowCover[row] == false && colCover[col] == false)
            {
                if(minVal > m_matrix[row][col])
                {
                    minVal = m_matrix[row][col];
                }
            }
            
        }
    }
}
void HungarianMatch::step_six()
{
    int minVal = MAX_VAL;
    find_smallest(minVal);
    for(int row = 0; row < m_rows; row++)
    {
        for(int col = 0; col < m_cols; col++)
        {
            if(rowCover[row] == true)
            {
                m_matrix[row][col] +=minVal;
            }
            if(colCover[col] == false)
            {
                m_matrix[row][col] -= minVal;
            }
        }
    }

    m_step = 4;
}

void HungarianMatch::showCostMatrix()
{
    cout<<"Cost Matrix:"<<endl;
    for(int row = 0; row < m_rows; row++)
    {
        for(int col = 0; col < m_cols; col++)
        {
            cout<<m_matrix[row][col]<<" ";
        }
        cout<<endl;
    }
    cout<<endl;
}

void HungarianMatch::showMaskMatrix()
{
    cout<<"Mask Matrix:"<<endl;
    for(int row = 0; row < m_rows; row++)
    {
        for(int col = 0; col < m_cols; col++)
        {
            cout<<m_mask[row][col]<<" ";
        }
        cout<<endl;
    }
    cout<<endl;
}
void HungarianMatch::clear_mask()
{
    for(int row = 0; row < m_rows; row++)
    {
        for(int col = 0; col < m_cols; col++)
        {
            m_mask[row][col] = 0;
        }
    }
}

void HungarianMatch::show_colCover()
{
    cout<<"colCover:"<<endl;
    for(int col = 0; col < m_cols; col++)
    {
        cout<<colCover[col]<<" ";
    }
    cout<<endl;
    cout<<endl;
}
void HungarianMatch::show_rowCover()
{
    cout<<"rowCover:"<<endl;
    for(int row = 0; row < m_rows; row++)
    {
        cout<<rowCover[row]<<" ";
    }
    cout<<endl;
    cout<<endl;
}

void HungarianMatch::step_seven()
{
    showMaskMatrix();
    showCostMatrix();
}



void HungarianMatch::run()
{
    bool done = false;
    m_step = 1;
    while(!done)
    {
        showCostMatrix();
        showMaskMatrix();
        show_rowCover();
        show_colCover();
        
        //show
        switch(m_step)
        {
            case 1:
                cout<<"***step 1***"<<endl;
                step_one();
                break;
            case 2:
                cout<<"***step 2***"<<endl;
                step_two();
                break;
            case 3:
                cout<<"***step 3***"<<endl;
                step_three();
                break;
            case 4:
                cout<<"***step 4***"<<endl;
                step_four();
                break;
            case 5:
                cout<<"***step 5***"<<endl;
                step_five();
                break;
            case 6:
                cout<<"***step 6***"<<endl;
                step_six();
                break;
            case 7:
                cout<<"***step 7***"<<endl;
                step_seven();
                done = true;
                break;       
        }
    }
}







// float 


// test 1
// int test[nRows][nCols] = {
//         {82,83,69,92},
//         {77,37,49,92},
//         {11,69,5,86},
//         {8,9,98,23},// 69 37 11 23    140

//     };

// test 2
// int test[nRows][nCols] = {
//     {108,125,150},
//     {150,135,175},
//     {122,148,250} // 122 135 150    min 407

// };

// test 3
// int test[nRows][nCols] = {
//         {1,2,3},
//         {2,4,6},
//         {3,6,9} // 3 4 3   min 10

//     };
