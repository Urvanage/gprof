#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#define MAX_N 1502

typedef struct {
    int n;
    char **map;
    int ** cst;     //
    int **visit;    // dp 역할
}MAP;

typedef struct{
    int n;
    int *arr;
}List;

int make_random_N(){
    int random = rand()%1000+100;
    return random;
}

int getNfromMAP(MAP *m){
    return m->n;
}

void allocate_arr(MAP *m){
    int len = getNfromMAP(m);
    m->map = (char**)malloc((len+2)*sizeof(char*)); //getNfromMAP;
    m->cst = (int**)malloc((len+2)*sizeof(int*));
    m->visit = (int**)malloc((len+2)*sizeof(int*));

    for(int i=0;i<len+2;i++){
        m->map[i] = (char*)malloc((len+2)*sizeof(char));
        m->cst[i] = (int*)malloc((len+2)*sizeof(int));
        m->visit[i] = (int*)malloc((len+2)*sizeof(int));
    }
}

char choosech(){
    char ch;
    int random = rand()%2;
    if(random==0){
        ch = 'R';
    }
    else{
        ch = 'D';
    }
    
    return ch;
}

int chooseint(){
    int random = rand()%500+1;
    return random;
}

void init_arr(MAP* m){
    int len = getNfromMAP(m);
    for(int i=1;i<=len+1;i++){
        for(int j=1;j<=len+1;j++){
            if(i<=len&& j<=len){
                m->map[i][j] = choosech();
                m->cst[i][j] = 0;
            }
            else{
                m->cst[i][j] = chooseint();
            }
            m->visit[i][j] = 0;

        }
    }
}

void free_arr(MAP* m){
    int len=getNfromMAP(m);
    for(int i=0;i<len+2;i++){
        free(m->map[i]);
        free(m->cst[i]);
        free(m->visit[i]);
    }
    free(m->map);
    free(m->cst);
    free(m->visit);
}

void fill_random_map(MAP *m){ 
    m->n = make_random_N();
    allocate_arr(m);
    init_arr(m);
}

void print_map(MAP *m){
   int len = getNfromMAP(m);
    for(int i=1;i<=len+1;i++){
        for(int j=1;j<=len+1;j++){
            if(i<=len && j<=len) printf("%c ",m->map[i][j]);
            else if(i!=len+1 || j!=len+1) printf("%d ",m->cst[i][j]);
        }
        printf("\n");
    }
}

void update(int x,int y, int num, MAP *m){
    int len = getNfromMAP(m);
    int nex, ney;
    if(m->map[x][y]=='R'){
        nex= x;
        ney = y+1;
    }
    else{
        nex = x+1;
        ney =y;
    }
    
    if(1<=nex && nex<= len+1 && 1<=ney && ney<= len+1){
        m->visit[nex][ney] += num;
        if(nex==len+1 || ney==len+1){
            return;
        }
        else{
            update(nex,ney,num,m);
        }
    }
}

void dfs(int x,int y, MAP *m){
    int len = getNfromMAP(m);
    int nex,ney;
    if(m->map[x][y]=='R'){
        nex = x;
        ney = y+1;
    }
    else {
        nex = x+1;
        ney =y;
    }
    if(1<=nex && nex<=len+1 && 1<=ney && ney<=len+1){
        if(nex == len+1 || ney == len+1){
            m->visit[nex][ney] = m->visit[x][y];
            return;
        }
        else{
            if(m->visit[nex][ney]==0){
                m->visit[nex][ney] = 1+m->visit[x][y];
                dfs(nex,ney,m);
            }
            else {
                m->visit[nex][ney] += m->visit[x][y];
                update(nex,ney,m->visit[x][y],m);
            }
        }
    }
}

void init_visit(MAP *m){
    int len = getNfromMAP(m);
    for(int i=0;i<len+2;i++){
        for(int j=0;j<len+2;j++){
            m->visit[i][j] = 0;
        }
    }
}

void count_total(long long int* total, MAP *m){
    int len = getNfromMAP(m);
    for(int i=1;i<=len+1;i++){
        *total = *total + m->cst[i][len+1]*m->visit[i][len+1]; 
    }
    for(int j=1;j<=len+1;j++){
        *total = *total + m->cst[len+1][j]*m->visit[len+1][j]; 
    }
    return;
    printf("%lld\n",*total);
}

void solution(MAP *m,long long int* total){

    int len = getNfromMAP(m);
    init_visit(m);

    for(int i=1;i<=len;i++){
        for(int j=1;j<=len;j++){
            if(m->visit[i][j]==0){
                m->visit[i][j]=1;
                dfs(i,j,m);
            }
        }
    }

    *total = 0;

    count_total(total,m);

}

void changedir(int x,int y,MAP *m){
    char ch = m->map[x][y];
    if(ch=='D') m->map[x][y] = 'R';
    else m->map[x][y] = 'D'; 
}

void insert(List* list, MAP * m){
    int len = getNfromMAP(m);
    list->arr = (int*)malloc(sizeof(int)*(len*2));
    list->n = 0;

    for(int i=1;i<=len+1;i++){
        list->arr[(list->n)++] = m->visit[i][len+1]*m->cst[i][len+1];
    }
    
    for(int j=1;j<=len+1;j++){
        list->arr[(list->n)++] = m->visit[len+1][j]*m->cst[len+1][j];
    }
}

int getNfromList(List *list){
    return list->n;
}

void sortList(List *list){
    int len = getNfromList(list);
    for(int i=0;i<len-1;i++){
        for(int j=i+1;j<len;j++){
            if(list->arr[i] >list->arr[j]){
                int tmp = list->arr[j];
                list->arr[j] = list->arr[i];
                list->arr[i] = tmp;
            }
        }
    }
}

void print_list (List *list){
    int len = getNfromList(list);
    for(int i=0;i<len;i++){
        printf("%lld ",list->arr[i]);
    }
}

int main(){
    srand(time(NULL));
    MAP m;
    List list;
    long long int total = 0;

    fill_random_map(&m);

    /*
    solution(&m,&total);

    int Q = chooseint()+200;

    for(int i=0;i<Q;i++){
        int x = chooseint()%m.n+1,y = chooseint()%m.n+1; 
        update(x,y,-m.visit[x][y],&m);
        changedir(x,y,&m);
        update(x,y,m.visit[x][y],&m);
        count_total(&total,&m);
    }

    print_map(&m);
  */  
    insert(&list,&m);
    sortList(&list);

    print_list(&list);

    free_arr(&m);
}