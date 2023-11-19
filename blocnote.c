#include <stdio.h>
#include <stdlib.h>
#include <time.h>

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

void init_tab(int tab[9], int N){
    for(int i=0; i<N;i++){
        tab[i]=0;
    }
}
// Sélectionner taille du jeu
int game_size() {
    int taille;
    printf("Quelle taille de jeu voulez vous ?\n");
    scanf("%d", &taille);
    return taille;
}

// Afficher le tableau
void print_board(int N) {
    for (int k = 0; k < N; k++) {
        printf("----");
    }
    printf("-\n");
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            if (j == (N - 1)) {
                printf("| %c |\n", board[i][j]);
            } else {
                printf("| %c ", board[i][j]);
            }
        }
        for (int l = 0; l < N; l++) {
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
        for (int j = 0; j < N; j++) {
        if (board[i][j] == 'X')
            count++;
        if (board[i][j] == 'O')
            count--;
        }
        if (count == N || count == -N)
        return 1;
        count = 0;
    }

    // Vérifier les colonnes
    count = 0;
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
        if (board[j][i] == 'X')
            count++;
        if (board[j][i] == 'O')
            count--;
        }
        if (count == N || count == -N)
        return 1;
        count = 0;
    }

    // Vérifier les diagonales
    count = 0;
    for (int j = 0; j < N; j++) {
        if (board[j][j] == 'X')
        count++;
        if (board[j][j] == 'O')
        count--;
    }
    if (count == N || count == -N)
        return 1;
    count = 0;
    for (int j = 0; j < N; j++) {
        if (board[N - 1 - j][j] == 'X')
        count++;
        if (board[N - 1 - j][j] == 'X')
        count--;
    }
    if (count == N || count == -N)
        return 1;

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

//---------------------------------------- VARIABLES JEU-----------------------------------------------

#define TAILLE 3                   // Taille de la grille
int cases_libres[TAILLE * TAILLE]; // tableau qui contient les indices des cases vides en 1D
int avancement = 0; // compte le nombre de coups joués
char player_now;
char winner;
int firstchoice;
int firststrike;
//------------------------------------------------------------------------------------------------------

/*Principe expliqué avec TAILLE = 3
 *le bot a 9 cases pour jouer (cases_libres[9] va de 0 à 8) il choisit entre 0
 *et 8 et place son marqueur dans la case le bot a pioché 5 il joue en
 *cases_libres[5]=5 (1D) donc 5/3=1ere ligne et 5%3=2e colonne le tour suivant
 *il a 8 cases (cases_libres[9] vaut:0,1,2,3,4,6,7,8) il choisit entre 0 et 7 il
 *pioche 5|encore| il joue en cases_libres[5]=6 (1D) donc 6/3=2e ligne et 6%3=0e
 *colonne Etc.. jusqu'a ce qu'il ne reste qu'une case dispo
 */
int play_ia2(char player, int N) {
    // sizeof(cases_libres)/sizeof(cases_libres[0]) permets de récupérer la taille du tableau Exemple 9
    int case_2D = rand() % ((sizeof(cases_libres) / sizeof(cases_libres[0])) - avancement);
    // rand() % 9 retourne un entier entre 0 et 8

    int val = cases_libres[case_2D];

    // conversion de la valeur de la case de 1 Dimension a 2 Dimensions
    int row = val / TAILLE;
    int col = val % TAILLE;

    board[row][col] = player;
    for (int i = case_2D; i < TAILLE * TAILLE - avancement;
        i++) { // Décalage du tableau pour supprimer la case occupée
        cases_libres[i] = cases_libres[i + 1];
    }
    avancement++;
    return val;
}
// Structure pour les threads
typedef struct entree {
    char player;
    int p_num;
    int board_size;
} entree;

void init_input(entree *data, char c, int num, int size) {
    data->player = c;
    data->p_num = num;
    data->board_size = size;
}
pthread_mutex_t board_mutex = PTHREAD_MUTEX_INITIALIZER;

int finished;

void *play_thread(void *info) {
    int lastplayed;
    int game_over = 0;
    entree player = *((entree *)info);
    while (game_over == 0 && finished == 0) {
        pthread_mutex_lock(&board_mutex);
        if (player.player == player_now) {
        lastplayed = play_ia2(player.player, player.board_size);
        if(firstchoice==0){
            firstchoice=1;
            firststrike = lastplayed;
        }
        print_board(TAILLE);

        game_over = check_game_over(player.board_size);
        if (game_over == 1 && finished == 0) {
            finished = 1;
            printf("Joueur %c gagne !\n", player.player);
            pthread_mutex_unlock(&board_mutex);
            break;
        } else if (game_over == 2 && finished == 0) {
            finished = 1;
            printf("Match nul !\n");
            winner = '0';
            pthread_mutex_unlock(&board_mutex);
            break;
        }
        player_now = (player_now == 'X') ? 'O' : 'X';
        }
        pthread_mutex_unlock(&board_mutex);
    }

    return NULL;
}

typedef struct stats {
    char starter;
    char winner;
    double timer;
    int nb_turn;
    int firstmove;
} stats;

// Fonction principale
stats JEU() {
    stats s;
    clock_t begin = clock();
    firstchoice=0;
    finished = 0;
    avancement = 0;
    player_now = 'X';
    s.starter = player_now;

    initialize_board(TAILLE);
    for (int i = 0; i < TAILLE * TAILLE; i++) {
        cases_libres[i] = i;
    }

    pthread_t j1, j2;
    char c1 = 'X', c2 = 'O';
    entree player1, player2;
    init_input(&player1, c1, 1, TAILLE);
    init_input(&player2, c2, 2, TAILLE);

    pthread_create(&j1, NULL, play_thread, &player1);
    pthread_create(&j2, NULL, play_thread, &player2);

    pthread_join(j1, NULL);
    pthread_join(j2, NULL);
    clock_t end = clock();
    s.nb_turn = avancement;
    if(winner!='0'){
        (s.winner = (avancement % 2)==1 ? 'X' : 'O');
    }
    else{
        s.winner='0';
        winner='c';
    }
    s.firstmove=firststrike;
    s.timer = (double)(end - begin) / CLOCKS_PER_SEC;

    return s;
}

int main() {
    int X_wins = 0;
    int nb_parties = 3;
    double tempstotal = 0;
    int nb_tie = 0;
    stats statArray[nb_parties];
    srand(time(NULL));
    for (int i = 0; i < nb_parties; i++) {
        statArray[i] = JEU();
    }
    int victory[TAILLE*TAILLE];
    init_tab(victory,TAILLE*TAILLE);
    for (int i = 0; i < nb_parties; i++) {
        if (statArray[i].winner == 'X'){
            X_wins++;
        }

        if(statArray[i].winner!='0'){ 
            printf("%de took %f sec. %c wins %d coups, first:%d\n", i + 1,statArray[i].timer, statArray[i].winner, statArray[i].nb_turn,statArray[i].firstmove);
            victory[statArray[i].firstmove]++;
        }else{ 
            nb_tie++;
        }
        tempstotal += statArray[i].timer;

    }
    for(int j=0;j<TAILLE*TAILLE;j++){
        printf("\nvictoires en case %d : %f",j,(float)victory[j]/(nb_parties-nb_tie));
    }
    printf("\nParties réalisées en %f sec\nTaux de victoire de X:%f (%d)\nNombre matchs nuls:%d",tempstotal, (float)X_wins / (nb_parties-nb_tie),X_wins, nb_tie);
    return 0;
}