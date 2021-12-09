#include <stdio.h>
#include <string.h>
#include <dirent.h>
#include <unistd.h>

#include <errno.h>
#include <stdlib.h>

// pour sau
char prevdir[100] = "";


int getcmd(char* CMD_LINE, char** CMD_ELEMS){
    
    char *token;
    //Decoupage de la notre cmd en plusieurs 'token', element
    token = strtok(CMD_LINE, " \n\t");
    CMD_ELEMS[0] = token;

    int i = 1;
    while (token != NULL)
    {      
        //printf("%s", token);
        //NULL, token a deja la cmd_line, il continu de travailler dessus
        token = strtok(NULL, " \n\t");
        CMD_ELEMS[i] = token;
        i++;
    }
    return i;
}

int eqcmd(char* cmd, char* cmdname)
{   
    //Test si cmd == cmdname
    if (strcmp(cmd, cmdname)==0){
        return 1;
    }
    else {
        return 0;   
    }
}

//EXIT du mini shell
//Return 1 
int action_exit(){
    exit(EXIT_SUCCESS);
    return EXIT_SUCCESS;
}

//Affiche le dossier de travail
int action_pwd(){
    //Initialisation du rep courant
    char repcourant[400];

    if(getcwd(repcourant,400)==NULL){
        //Affichage en cas d'erreur
        printf("Problèmes de repertoire%s", strerror(errno));
        return EXIT_FAILURE;
    }
    else {
        // Affichage du rep
        printf("%s\n", repcourant);
        return EXIT_SUCCESS;
    }


}

int action_cd(char** word_line){

    getcwd(prevdir, 100);
    printf(prevdir);
    if (eqcmd(word_line[1], "..")){
        chdir("..");
        return EXIT_SUCCESS;
    }
    else {
        // on se rend dans le repertoire choisi
        if (chdir(word_line[1])){
            // si il n'existe pas
            printf("Votre repertoire n'existe pas : %s\n", word_line[1]);
            return EXIT_FAILURE;
        } else {
            // c'est ok
            return EXIT_SUCCESS;
        }
    }
}


int action_ls(){
    //recuperation de notre repertoire courant
    char repcourant[400];
    getcwd(repcourant,400);
    struct dirent *lecture;
    DIR* rep;
    //Ouverture du flux d'annuaire de notre repertoire courant
    rep = opendir(repcourant);
    //Readdir retourne un pointeur 
    while ((lecture = readdir(rep)) != NULL)
    {
        //Affichage du dossier pointe
        printf("%s\n", lecture->d_name);
    }
    //ferme le flux d'annuaire 
    closedir(rep);
    return EXIT_SUCCESS;

}



int main(){

    // FILE *fp;
    // fp = fopen("file.txt", "r");
    // if (fp==NULL){
    //     printf("Error message:%s\n", strerror(errno));
    // }

    
    int QUIT = 0;

    //chaine input
    char input_line[4096];
    
    //tableau chaine input decoupée
    char *word_line[4096];

    //Taille du tableau de l'entree 
    int taille;

    //char* cmd[150];

    // Boucle d'interpretation des commandes
    while (!QUIT){
        printf("maxishell$ ");
        //Recuperation du flux d'entré
        fgets(input_line, 4096, stdin);
        
        // découpage des éléments de la ligne
        taille = getcmd(input_line, word_line);


        //Affichage de l'input découpé
        for (int j = 0; j < taille-1; j++){
            printf("%s\n", word_line[j]);
        }
        //printf("%d", taille);
        //Si la cmd rentrée n'est pas vide

        //printf("(%s)", word_line[0]);
        if (taille > 0){

            if(eqcmd(word_line[0], "exit")){
                //quitte le shell
                action_exit();

            } else if (eqcmd(input_line,"pwd")) {
                //Affiche dossier de travail
                action_pwd();
            } else if (eqcmd(word_line[0], "cd")){
                if(taille >= 2){
                    //Test si il y a bien deux arguments
                    //Puis test si il y a bien cd en argument
                    action_cd(word_line);
                }
            } else if (eqcmd(word_line[0], "ls")){
                //Affiche les fichiers dans le dossier courant
                action_ls();
            }
            /*
            else {
                //Affiche les erreurs ???
                action_ERREUR();
            } */
        }    

    }
    
    return(0);
}

//gcc --std=c99 -Wall -Wextra -Werror -o minishell ./minishell.c
