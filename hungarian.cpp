#include <iostream>
using namespace std;


#define nRows (3)
#define nCols (3)
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

void step_one(int& step)
{
    int min_in_row;
    for(int row = 0; row < nRows; row++)
    {
        min_in_row = cost[row][0];
        for(int col = 0; col < nCols; col++)
        {
            if(cost[row][col] < min_in_row)
            {
                min_in_row = cost[row][col];
            }
        }
        for(int col = 0; col < nCols; col++)
        {
            cost[row][col]-=min_in_row;
        }
    }

    step = 2;
}

void step_two(int& step)
{
    for(int row = 0; row < nRows; row++)
    {
        for(int col = 0; col < nCols; col++)
        {
            if(cost[row][col] == 0 && rowCover[row] == false && colCover[col] == false)
            {
               mask[row][col] = 1;// only set a star entry for each row && col
               rowCover[row] = true;
               colCover[col] = true;
            }

        }
    }
    //clear
    for(int row = 0; row < nRows; row++)
    {
        rowCover[row] = false;
    }
    for(int col = 0; col < nCols; col++)
    {
        colCover[col] = false;
    }
    step = 3;
}

void step_three(int& step)
{
    int col_count;
    for(int row = 0; row < nRows; row++)
    {
        for(int col = 0; col < nCols; col++)
        {
            if(mask[row][col] == 1)
            {
                colCover[col] = true;
            }
        }   
    }
    col_count = 0;
    for(int col = 0; col < nCols; col++)
    {
        if(colCover[col])
        {
            col_count+=1;
        }
    }

    if(col_count >= nCols || col_count >= nRows)
    {
        step = 7;
    }
    else
    {
        step = 4;
    }
    
}
// 在整张表中寻找一个未被竖线覆盖的零点，如果找到一个，则立即返回该点的横纵坐标
void find_a_zero(int& row, int& col)
{
    int r = 0;
    int c;//?
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
            if(cost[r][c] == 0 && rowCover[r] == false && colCover[c] == false)
            {
                row = r;
                col = c;
                done = true;
            }
            c+=1;
            if(c >= nCols || done)
            {
                break;
            }
        }
        r+=1;
        if(r >= nRows)
        {
            done = true;
        }
        
    }
    
}
//确定某一行是否有标记了星号的节点
bool star_in_row(int row)
{
    bool retVal = false;
    for(int col = 0; col < nCols; col++)
    {
        if(mask[row][col] == 1)
        {
            retVal = true;
        }
    }
    return retVal;
}

void find_star_in_row(int row, int& col)
{
    col = -1;
    for(int c = 0; c < nCols; c++)
    {
        if(mask[row][c] == 1)
        {
            col = c;
        }
    }
}



void step_four(int& step)
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
            step = 6;
        }
        else
        {
            mask[row][col] = 2;//prim :non-covered zero
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
                step = 5;
                //(1,0)
                path_row_0 = row;
                path_col_0 = col;
                
            }
            
        }
        
    }
    
}

void find_star_in_col(int col, int& row)
{
    row = -1;
    for(int r = 0; r < nRows; r++)
    {
        if(mask[r][col] == 1)
        {
            row = r;
        }
    }

}

void find_prime_in_row(int row, int& col)
{
    for(int c = 0; c < nCols; c++)
    {
        if(mask[row][c] == 2)
        {
            col = c;
        }
    }
}

void augment_path()
{
    for(int p = 0; p < path_count; p++)
    {
        int p_x = path[p][0];
        int p_y = path[p][1];
        if(mask[p_x][p_y] == 1)
        {
            mask[p_x][p_y] = 0;
        }
        else
        {
            mask[p_x][p_y] = 1;
        }   
    }
}
void clear_path()
{
    for(int p = 0; p < path_len; p++)
    {
        path[p][0] = -1;
        path[p][1] = -1;
    }
    path_row_0 = -1;
    path_col_0 = -1;
}

void clear_covers()
{
    //clear
    for(int row = 0; row < nRows; row++)
    {
        rowCover[row] = false;
    }
    for(int col = 0; col < nCols; col++)
    {
        colCover[col] = false;
    }
}

void erase_primes()
{
    for(int row = 0; row < nRows; row++)
    {
        for(int col = 0; col < nCols; col++)
        {
            if(mask[row][col] ==2)
            {
                mask[row][col] = 0;
            }
        }
    }
}


void step_five(int& step)
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
    step = 3;

}

void find_smallest(int& minVal)
{
    for(int row = 0; row < nRows; row++)
    {
        for(int col = 0; col < nCols; col++)
        {
            if(rowCover[row] == false && colCover[col] == false)
            {
                if(minVal > cost[row][col])
                {
                    minVal = cost[row][col];
                }
            }
            
        }
    }
}
void step_six(int& step)
{
    int minVal = MAX_VAL;
    find_smallest(minVal);
    for(int row = 0; row < nRows; row++)
    {
        for(int col = 0; col < nCols; col++)
        {
            if(rowCover[row] == true)
            {
                cost[row][col] +=minVal;
            }
            if(colCover[col] == false)
            {
                cost[row][col] -= minVal;
            }
        }
    }

    step = 4;
}

void showCostMatrix()
{
    cout<<"Cost Matrix:"<<endl;
    for(int row = 0; row < nRows; row++)
    {
        for(int col = 0; col < nCols; col++)
        {
            cout<<cost[row][col]<<" ";
        }
        cout<<endl;
    }
    cout<<endl;
}

void showMaskMatrix()
{
    cout<<"Mask Matrix:"<<endl;
    for(int row = 0; row < nRows; row++)
    {
        for(int col = 0; col < nCols; col++)
        {
            cout<<mask[row][col]<<" ";
        }
        cout<<endl;
    }
    cout<<endl;
}
void clear_mask()
{
    for(int row = 0; row < nRows; row++)
    {
        for(int col = 0; col < nCols; col++)
        {
            mask[row][col] = 0;
        }
    }
}

void show_colCover()
{
    cout<<"colCover:"<<endl;
    for(int col = 0; col < nCols; col++)
    {
        cout<<colCover[col]<<" ";
    }
    cout<<endl;
    cout<<endl;
}
void show_rowCover()
{
    cout<<"rowCover:"<<endl;
    for(int row = 0; row < nRows; row++)
    {
        cout<<rowCover[row]<<" ";
    }
    cout<<endl;
    cout<<endl;
}

void step_seven()
{
    showMaskMatrix();
    showCostMatrix();
}



void Run()
{
    bool done = false;
    int step = 1;
    while(!done)
    {
        showCostMatrix();
        showMaskMatrix();
        show_rowCover();
        show_colCover();
        
        //show
        switch(step)
        {
            case 1:
                cout<<"***step 1***"<<endl;
                step_one(step);
                break;
            case 2:
                cout<<"***step 2***"<<endl;
                step_two(step);
                break;
            case 3:
                cout<<"***step 3***"<<endl;
                step_three(step);
                break;
            case 4:
                cout<<"***step 4***"<<endl;
                step_four(step);
                break;
            case 5:
                cout<<"***step 5***"<<endl;
                step_five(step);
                break;
            case 6:
                cout<<"***step 6***"<<endl;
                step_six(step);
                break;
            case 7:
                cout<<"***step 7***"<<endl;
                step_seven();
                done = true;
                break;       
        }
    }
}





int main()
{
    clear_covers();
    clear_mask();

    int test[nRows][nCols] = {
        {1,2,3},
        {2,4,6},
        {3,6,9} // 3 4 3   min 10

    };
    for(int row = 0; row < nRows; row++)
    {
        for(int col = 0; col < nCols; col++)
        {
            cost[row][col] = test[row][col];
        }
    }
    
   
    Run();

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
