#include <stdio.h>
#include <stdlib.h>

char board[3][3]; // Le tableau du jeu

// Initialiser le tableau
void initialize_board() {
    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) {
            board[i][j] = ' ';
        }
    }
}

// Afficher le tableau
void print_board() {
    printf("-------------\n");
    for (int i = 0; i < 3; i++) {
        printf("| %c | %c | %c |\n", board[i][0], board[i][1], board[i][2]);
        printf("-------------\n");
    }
}

// Vérifier si le jeu est terminé
int check_game_over() {
    // Vérifier les lignes
    for (int i = 0; i < 3; i++) {
        if (board[i][0] != ' ' && board[i][0] == board[i][1] && board[i][0] == board[i][2]) {
            return 1;
        }
    }
    // Vérifier les colonnes
    for (int i = 0; i < 3; i++) {
        if (board[0][i] != ' ' && board[0][i] == board[1][i] && board[0][i] == board[2][i]) {
            return 1;
        }
    }
    // Vérifier les diagonales
    if (board[0][0] != ' ' && board[0][0] == board[1][1] && board[0][0] == board[2][2]) {
        return 1;
    }
    if (board[0][2] != ' ' && board[0][2] == board[1][1] && board[0][2] == board[2][0]) {
        return 1;
    }
    // Vérifier si le tableau est plein (match nul)
    int is_full = 1;
    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) {
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
void play_turn(char player) {
    int row, col;
    printf("Joueur %c, entrez la ligne et la colonne : ", player);
    scanf("%d %d", &row, &col);
    if (row < 0 || row > 2 || col < 0 || col > 2 || board[row][col] != ' ') {
        printf("Coup invalide. Réessayez.\n");
        play_turn(player);
    } else {
        board[row][col] = player;
    }
}

// Fonction principale
int main() {
    char player = 'X';
    int game_over = 0;
    initialize_board();
    while (!game_over) {
        print_board();
        play_turn(player);
        game_over = check_game_over();
        if (game_over == 1) {
            printf("Joueur %c a gagné !\n", player);
        } else if (game_over == 2) {
            printf("Match nul !\n");
        } else {
            player = (player == 'X') ? 'O' : 'X';
        }
    }
    print_board();
    return 0;
}
