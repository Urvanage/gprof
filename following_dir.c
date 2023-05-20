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

int make_random_N(){
    int random = rand()%1000+100;
    return random;
}

int getNfromMAP(MAP *m){
    return m->n;
}

void allocate_arr(MAP *m){
    m->map = (char**)malloc((getNfromMAP(m)+2)*sizeof(char*)); //getNfromMAP;
    m->cst = (int**)malloc((getNfromMAP(m)+2)*sizeof(int*));
    m->visit = (int**)malloc((getNfromMAP(m)+2)*sizeof(int*));

    for(int i=0;i<getNfromMAP(m)+2;i++){
        m->map[i] = (char*)malloc((getNfromMAP(m)+2)*sizeof(char));
        m->cst[i] = (int*)malloc((getNfromMAP(m)+2)*sizeof(int));
        m->visit[i] = (int*)malloc((getNfromMAP(m)+2)*sizeof(int));
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
    for(int i=1;i<=getNfromMAP(m)+1;i++){
        for(int j=1;j<=getNfromMAP(m)+1;j++){
            if(i<=getNfromMAP(m) && j<=getNfromMAP(m)){
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

    for(int i=0;i<getNfromMAP(m)+2;i++){
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

/*void print_map(MAP *m){
    printf("%d\n",getNfromMAP(m));
    for(int i=1;i<=getNfromMAP(m)+1;i++){
        for(int j=1;j<=getNfromMAP(m)+1;j++){
            if(i<=getNfromMAP(m) && j<=getNfromMAP(m)) printf("%c ",m->map[i][j]);
            else printf("%d ",m->cst[i][j]);
        }
        printf("\n");
    }
}*/

void update(int x,int y, int num, MAP *m){
    int nex, ney;
    if(m->map[x][y]=='R'){
        nex= x;
        ney = y+1;
    }
    else{
        nex = x+1;
        ney =y;
    }
    
    if(1<=nex && nex<= getNfromMAP(m)+1 && 1<=ney && ney<= getNfromMAP(m)+1){
        m->visit[nex][ney] += num;
        if(nex==getNfromMAP(m)+1 || ney==getNfromMAP(m)+1){
            return;
        }
        else{
            update(nex,ney,num,m);
        }
    }
}

void dfs(int x,int y, MAP *m){
    int nex,ney;
    if(m->map[x][y]=='R'){
        nex = x;
        ney = y+1;
    }
    else {
        nex = x+1;
        ney =y;
    }
    if(1<=nex && nex<=getNfromMAP(m)+1 && 1<=ney && ney<=getNfromMAP(m)+1){
        if(nex == getNfromMAP(m)+1 || ney == getNfromMAP(m)+1){
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
    for(int i=0;i<getNfromMAP(m)+2;i++){
        for(int j=0;j<getNfromMAP(m)+2;j++){
            m->visit[i][j] = 0;
        }
    }
}

void count_total(long long int* total, MAP *m){
    for(int i=1;i<=getNfromMAP(m)+1;i++){
        *total = *total + m->cst[i][getNfromMAP(m)+1]*m->visit[i][getNfromMAP(m)+1]; 
    }
       for(int j=1;j<=getNfromMAP(m)+1;j++){
        *total = *total + m->cst[getNfromMAP(m)+1][j]*m->visit[getNfromMAP(m)+1][j]; 
    }
    return;
}

void solution(MAP *m){

    init_visit(m);

    for(int i=1;i<=getNfromMAP(m);i++){
        for(int j=1;j<=getNfromMAP(m);j++){
            if(m->visit[i][j]==0){
                m->visit[i][j]=1;
                dfs(i,j,m);
            }
        }
    }

    long long int *total;
    *total = 0;

    count_total(total,m);
    printf("%lld\n",*total);
}

void changedir(int x,int y,MAP *m){
    char ch = m->map[x][y];
    if(ch=='D') m->map[x][y] = 'R';
    else m->map[x][y] = 'D'; 
}

int main(){
    srand(time(NULL));
    MAP m;

    fill_random_map(&m);

    solution(&m);

    int Q = chooseint();
    for(int i=0;i<Q;i++){
        int x,y; 
        scanf("%d %d",&x,&y);
        changedir(x,y,&m);
        solution(&m);
    }
}