#include <bits/stdc++.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
using namespace std;
 
const int n=9;
int remaining=0;
const int  num_threads=27;
int valid[num_threads]={0};
int sudoku[n][n],s2[n][n];
 
bool isSafe(int r,int c, int no){
    for (int i=0;i<9;i++){
        if (sudoku[r][i] == no) return false;
    }
    for (int i=0;i<9;i++){
        if (sudoku[i][c]==no) return false;
    }
    int iR= r- r%3, iC= c- c%3;
    for (int i=0;i<3;i++){
        for (int j = 0; j < 3; j++){
            if (sudoku[i + iR][j + iC] == no) return false;
        }
    }
    return true;
}
 
bool solveSudoku(int r,int c){
    if (r==n-1 && c==n) return true;
    if (c==n) {
        r++;
        c = 0;
    }
    if (sudoku[r][c]> 0) return solveSudoku(r,c+1);
    for (int i= 1; i<=n;i++){
        if (isSafe(r,c,i)){
            sudoku[r][c] =i;
            if (solveSudoku(r,c+1)) return true;
        }
        sudoku[r][c] = 0;
    }
    return false;
}
typedef struct {
    int row;
    int column;
} parameters;
 
void *CVal(void* p) {
    parameters *t = (parameters*) p;
    int r=t->row;
    int c= t->column;
    if (r!= 0 || c>8) {
        pthread_exit(NULL);
    }
    int val[n] = {0};
    for (int i=0; i<n; i++) {
        int tmp=sudoku[i][c];
        if (tmp< 1 || tmp> 9 || val[tmp-1]==1) pthread_exit(NULL);
        else val[tmp-1]=1;
    }
    valid[18+c]=1;
    pthread_exit(NULL);
}
void *RVal(void* p) {
    parameters *t = (parameters*) p;
    int r=t->row;
    int c=t->column;
    if (c!= 0 || r>8) {
        pthread_exit(NULL);
    }
    int val[n]={0};
    for (int i = 0; i <n; i++){
        int tmp= sudoku[r][i];
        if (tmp<1 || tmp > 9 || val[tmp- 1] == 1) pthread_exit(NULL);
        else val[tmp-1] = 1;
    }
    valid[9 + r] = 1;
    pthread_exit(NULL);
}
void *SubGVal(void* p){
    parameters *t= (parameters*) p;
    int r=t->row;
    int c=t->column;
    if (r> 6 || r% 3 != 0 || c> 6 || c% 3 != 0) {
        pthread_exit(NULL);
    }
    int val[9] = {0};
    for (int i=r;i<r+ 3;i++) {
        for (int j=c;j<c+3; j++) {
            int tmp= sudoku[i][j];
            if (tmp < 1 || tmp> 9 || val[tmp-1]==1) pthread_exit(NULL);
            else val[tmp- 1] = 1;
        }
    }
    valid[r+ c/3]=1;
    pthread_exit(NULL);
}
void validate(){
    pthread_t threads[num_threads];
    int indx = 0;
    for (int i = 0; i < 9; i++) {
        for (int j = 0; j < 9; j++) {
            if (i%3 == 0 && j%3 == 0) {
                parameters *data = (parameters *) malloc(sizeof(parameters));
                data->row = i;
                data->column = j;
                pthread_create(&threads[indx++], NULL,SubGVal, data);
            }
            if (i == 0) {
                Parameters *columnData = (parameters *) malloc(sizeof(parameters));
                columnData->row = i;
                columnData->column = j;
                pthread_create(&threads[indx++], NULL,CVal, columnData);    
            }
            if (j == 0) {
                parameters *rowData = (parameters *) malloc(sizeof(parameters));
                rowData->row = i;
                rowData->column = j;
                pthread_create(&threads[indx++], NULL, RVal, rowData);
            }
        }
    }
 
    for(int i=0; i<num_threads; i++){
        pthread_join(threads[i], NULL);        
    }
    for(int i=0; i<num_threads; i++){
        if(valid[i] == 0) {
            cout<<"\nSudoku solution is invalid!\n";
            return;
        }
    }
    cout<<"\nSudoku solution is valid!\n";
    return;
}
void input(){
    cout<<"\nPlease enter the 9x9 Sudoku :\n";
    for(int i=0;i<n;i++){
        for(int j=0;j<n;j++){
                cin>>sudoku[i][j];
                if(!sudoku[i][j]) remaining++;
        }
    }
}
void print(){
    cout<<"\nSolution of the entered Sudoku is :\n";
    for(int i=0;i<n;i++){
        for(int j=0;j<n;j++)cout<<sudoku[i][j]<<' ';
        cout<<'\n';
    }
}
void check(){
    int x;
    cout<<"\nEnter the number of values you want to update (i.e, from 0 to any other value): ";
    cin>>x;
    if(x>remaining){
        cout<<"Invalid input!"<<endl;
    }
    else{
        vector<pair<pair<int,int>,int>>vec(x);
        int tt=0;
        cout<<"\nEnter the row number, column number (follow zero based indexing) and the value in order : \n";
        for(int i=0;i<x;i++){
            cin>>vec[i].first.first>>vec[i].first.second>>vec[i].second;
            if(sudoku[vec[i].first.first][vec[i].first.second]!=vec[i].second){
                tt++;
            }
        }
        if(tt!=0){
            cout<<"\nYou have made a mistake. Check again!!\n";
        }
        else{
            cout<<"\nYes, You are correct! Keep going.\n";
            for(int i=0;i<x;i++){
                s2[vec[i].first.first][vec[i].first.second]=vec[i].second;
            }
            remaining-=x;
        }
        cout<<"\nUpdated Sudoku matrix :\n";
        for(int i=0;i<n;i++){
            for(int j=0;j<n;j++)cout<<s2[i][j]<<' ';
            cout<<endl;
        }
    }
}
int main(){
    cout<<"\nHey there! Aren't you fascinated by Sudokus? So, Let's dive into the world of Sudokus.\n";
    cout<<"\nWell you as user have got three choices, which are : \n";
    cout<<"\n*NOTE* : Here we have denoted blank spaces with '0'.\n";
    cout<<"\nChoice 1 : \n Enter a complete Sudoku (without any blank spaces) and we will tell you whether\n it follows all the rules of Sudoku or not, i.e,";
    cout<<" Whether Sudoku is Valid or Invalid\n";
    cout<<"\nChoice 2 : \n Enter a Sudoku question (i.e, you can keep blank spaces in the 9x9 matrix) and\n we will solve the Sudoku for a Valid Solution.\n";
    cout<<"\nChoice 3 : \n Enter a Sudoku question, then you can enter as many values as you want to in\n place of different blank spaces. We will verify";
    cout<<" whether the values entered uptil now are valid or not.\n";
    cout<<"\nChoice 4 : \n To EXIT the program!\n";
 
    int c;
    bool f;
    do{
        cout<<"\nEnter your choice (1/2/3/4) : ";
        cin>>c;
        switch(c){
        case 1:
            input();
            validate();
            break; 
        case 2:
            input();
            cout<<endl;
            f=solveSudoku(0,0);
            if(f) print();
            validate();
            break;
        case 3:
            input();
            for(int i=0;i<n;i++){
                for(int j=0;j<n;j++) s2[i][j]=sudoku[i][j];
            }
            f=solveSudoku(0,0);
            if(f){
                check();
                while(remaining>0){
                    cout<<"\nDo you want to continue checking? (1 to Continue/ 0 to Exit.)\n";
                    int y;
                    cin>>y;
                    if(y) check();
                    else break;
                }
            }
            else cout<<"\nYou have entered a wrong Input!\n";
            break;
 
        case 4:
            cout<<"\nExit Successful.\n";
            break;
   
        default:
            cout<<"\nYour choice is Invalid. Please make a valid one!\n";
            break;
        }
    }
    while(c!=4);
    return 0;
}
