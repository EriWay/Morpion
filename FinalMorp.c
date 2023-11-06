#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <windows.h>
#include <pthread.h>

#define MAX_SIZE 10

char board[MAX_SIZE][MAX_SIZE]; // Le tableau du jeu

// Initialiser le tableau
void initialize_board(int N) {
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            board[i][j] = ' ';
        }
    }
}

//Sélectionner taille du jeu
int game_size(){
    int taille;
    printf("Quelle taille de jeu voulez vous ?\n");
    scanf("%d",&taille);
    return taille;
}

// Afficher le tableau
void print_board(int N) {
    for (int k=0;k<N;k++){
        printf("----");
    }
    printf("-\n");
    for (int i = 0; i < N; i++) {
        for (int j = 0;j<N;j++){
            if (j==(N-1)){printf("| %c |\n", board[i][j]);}
            else{printf("| %c ", board[i][j]);}
        }
        for (int l=0;l<N;l++){
            printf("----");
        }
        printf("-\n");
    }
}

// Vérifier si le jeu est terminé
int check_game_over(int N) {
    // Vérifier les lignes
    int count = 0;
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++){
            if(board[i][j]=='X') count++;
            if(board[i][j]=='O') count--;
        }
        if(count == N || count == -N) return 1;
        count = 0;
    }
    

    // Vérifier les colonnes
    count = 0;
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++){
            if(board[j][i]=='X') count++;
            if(board[j][i]=='O') count--;
        }
        if(count == N || count == -N) return 1;
        count = 0;
    }
    

    // Vérifier les diagonales
    count = 0;
    for (int j = 0; j < N; j++){
        if(board[j][j]=='X') count++;
        if(board[j][j]=='O') count--;
    }
    if(count == N || count == -N) return 1;
    count = 0;
    for (int j = 0; j < N; j++){
        if(board[N-1-j][j] == 'X') count++;
        if(board[N-1-j][j] == 'X') count--;
    }
    if(count == N || count == -N) return 1;

    // Vérifier si le tableau est plein (match nul)
    int is_full = 1;
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            if (board[i][j] == ' ') {
                is_full = 0;
                break;
            }
        }
    }
    if (is_full) {
        return 2;
    }
    return 0;
}

void play_ia(char player, int N){
    int row, col;
    row = rand() % N;
    col = rand() % N;
    if (row < 0 || row > N-1 || col < 0 || col > N-1 || board[row][col] != ' ') {
        play_ia(player, N);
    } else {
        board[row][col] = player;
    }
}

#define TAILLE 3 //Taille de la grille
int cases_libres[TAILLE*TAILLE];//tableau qui contient les indices des cases vides en 1D
int avancement =0;//compte le nombre de coups joués
/*Principe expliqué avec TAILLE = 3
*le bot a 9 cases pour jouer (cases_libres[9] va de 0 à 8) il choisit entre 0 et 8 et place son marqueur dans la case
*le bot a pioché 5 il joue en cases_libres[5]=5 (1D) donc 5/3=1ere ligne et 5%3=2e colonne
*le tour suivant il a 8 cases (cases_libres[9] vaut:0,1,2,3,4,6,7,8) il choisit entre 0 et 7 
*il pioche 5|encore| il joue en cases_libres[5]=6 (1D) donc 6/3=2e ligne et 6%3=0e colonne
* Etc.. jusqu'a ce qu'il ne reste qu'une case dispo
*/
void play_ia2(char player, int N){
        //sizeof(cases_libres)/sizeof(cases_libres[0]) permets de récupérer la taille du tableau Exemple 9
    int case_2D = rand() % ((sizeof(cases_libres)/sizeof(cases_libres[0]))-avancement);
        //rand() % 9 retourne un entier entre 0 et 8
    //printf("case2d:%d-",case_2D);
    int val = cases_libres[case_2D];
    //printf("val:%d\n",val);
        //conversion de la valeur de la case en 1 Dimension en 2 Dimensions
    int row= val/3;
    int col= val%3;
    //printf("x:%d y:%d \n",row,col);
    board[row][col]=player;
    for(int i = case_2D;i<TAILLE*TAILLE-avancement;i++){    //Décalage du tableau pour supprimer la case occupée
        cases_libres[i]= cases_libres[i+1];
    }
    avancement++;
    Sleep(300);//Attente optionnelle pour voir ce qu'il se passe
}
//Structure pour les threads
typedef struct entree{
    char player;
    int p_num;
    int board_size;
}entree;

void init_input(entree *data, char c, int num, int size){
    data->player = c;
    data->p_num = num;
    data->board_size = size;
}
char player_now = 'X';
pthread_mutex_t board_mutex = PTHREAD_MUTEX_INITIALIZER;

int finished;

void* play_thread(void* info){
    int game_over =0;
    entree player = *((entree*)info);
    printf("- %c -\n",player.player);
    while(game_over == 0 && finished==0){
        pthread_mutex_lock(&board_mutex);
        if(player.player==player_now){
            play_ia2(player.player,player.board_size);
            print_board(player.board_size);
            
            game_over = check_game_over(player.board_size);
            if (game_over == 1 && finished ==0) {
                finished=1;
                printf("Joueur %c gagne !\n", player.player);
                pthread_mutex_unlock(&board_mutex);
                //break;
            } else if (game_over == 2 && finished==0) {
                finished=1;
                printf("Match nul !\n");
                pthread_mutex_unlock(&board_mutex);
                //break;
            }
            player_now = (player_now == 'X') ? 'O' : 'X';
        }
        pthread_mutex_unlock(&board_mutex);
    }
    pthread_mutex_unlock(&board_mutex);
    //player_now = (player_now == 'X') ? 'O' : 'X';

    return NULL;
}


typedef struct stats{
    char starter;
    char winner;
    double timer;
    int nb_turn;
}stats;

// Fonction principale
stats JEU() {
    stats s;
    clock_t begin= clock();
    finished=0;
    avancement=0;
    player_now = 'X';
    s.starter=player_now;

    initialize_board(TAILLE);
    for(int i=0;i<TAILLE*TAILLE;i++){
        cases_libres[i]=i;
    }

    pthread_t j1,j2;
    char c1='X',c2='O';
    entree player1, player2;
    init_input(&player1,c1,1,TAILLE);
    init_input(&player2,c2,2,TAILLE);
    /*player1.board_size=TAILLE;
    player1.player='X';
    player1.p_num = 1;*/
    pthread_create(&j1,NULL,play_thread,&player1);
    pthread_create(&j2,NULL,play_thread,&player2);

    pthread_join(j1,NULL);
    pthread_join(j2,NULL);
    clock_t end = clock();
    s.nb_turn=avancement;
    s.winner = (avancement%2)?'X':'O';
    s.timer=(double)(end-begin)/CLOCKS_PER_SEC;
    
    /*char player = 'X';
    int game_over = 0, board_size;
    board_size=game_size();
    while (board_size >= MAX_SIZE){
        printf("Essaie plus petit (inferieur a %d)\n",MAX_SIZE);
        board_size = game_size();
    }
    initialize_board(board_size);
    while (!game_over) {
        //print_board(board_size);
        play_ia(player, board_size);
        game_over = check_game_over(board_size);
        if (game_over == 1) {
            printf("Joueur %c gagne !\n", player);
        } else if (game_over == 2) {
            printf("Match nul !\n");
        } else {
            player = (player == 'X') ? 'O' : 'X';
        }
    }
    print_board(board_size);*/
    return s;
}

int main(){
    stats statArray[5];
    srand(time(NULL));
    for(int i=0;i<5;i++){
        statArray[i]= JEU();
    }
    for(int i=0; i<5; i++){
        printf("%de partie terminee en %f sec.\n%c gagne en %d coups\n",i+1,statArray[i].timer,statArray[i].winner,statArray[i].nb_turn);
    }
    return 0;
}