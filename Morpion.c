#include <stdio.h>
#include <stdlib.h>

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
    for (int k=0;k<N;k++)
    {
        printf("----");
    }
    printf("-\n");
    
    for (int i = 0; i < N; i++) {
        for (int j = 0;j<N;j++)
        {
            if (j==(N-1)){printf("| %c |\n", board[i][j]);}
            else{printf("| %c ", board[i][j]);}
            
        }
        for (int l=0;l<N;l++)
        {
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

// Jouer un tour
void play_turn(char player, int N) {
    int row, col;
    printf("Joueur %c, entrez la ligne et la colonne : ", player);
    scanf("%d %d", &row, &col);
    if (row < 0 || row > N-1 || col < 0 || col > N-1 || board[row][col] != ' ') {
        printf("Coup invalide. Reessayez.\n");
        play_turn(player, N);
    } else {
        board[row][col] = player;
    }
}

// Fonction principale
int main() {
    char player = 'X';
    int game_over = 0, board_size;
    board_size=game_size();
    while (board_size >= MAX_SIZE){
        printf("Essaie plus petit (inferieur a %d)\n",MAX_SIZE);
        board_size = game_size();
    }
    initialize_board(board_size);
    while (!game_over) {
        print_board(board_size);
        play_turn(player, board_size);
        game_over = check_game_over(board_size);
        if (game_over == 1) {
            printf("Joueur %c gagne !\n", player);
        } else if (game_over == 2) {
            printf("Match nul !\n");
        } else {
            player = (player == 'X') ? 'O' : 'X';
        }
    }
    print_board(board_size);
    return 0;
}
