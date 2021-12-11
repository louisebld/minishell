#include <stdio.h>
#include <string.h>
#include <dirent.h>
#include <unistd.h>
//struct stat file
#include <sys/stat.h>
#include <errno.h>
#include <stdlib.h>
#include <fcntl.h>

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

    //printf(prevdir);
    if (eqcmd(word_line[1], "..")){
        getcwd(prevdir, 100);
        chdir("..");
        return EXIT_SUCCESS;
    }

    else if (eqcmd(word_line[1], "-")){
        char tmp[100];
        getcwd(tmp, 100);
        //printf("(%s)",prevdir);

        chdir(prevdir);
        return EXIT_SUCCESS;
    }

    else {
        getcwd(prevdir, 100);
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


int action_ls(char** word_line){
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
        //Test si l'on veut afficher des infos sur les fichiers
        //On test si il exite un attribut à la commande et si c'est le bon 
        if (word_line[1] != NULL){
            if (eqcmd(word_line[1], "-info")){
                //On recupere les infos du fichier grace a stat()
                struct stat filest;
                stat(lecture->d_name, &filest);
                //Affichage des infos
                printf((filest.st_mode & S_IROTH) ? "r" : "-"); //protec U G O
                printf("    %d", filest.st_mode); //Taille du fichier
                printf("    %ld     ", filest.st_blocks); //Nb de block
            }
        }
        printf("%s\n", lecture->d_name); //Nom du fichier      
        
    }
    printf("\n");
    //ferme le flux d'annuaire 
    closedir(rep);
    return EXIT_SUCCESS;

}

int action_exec(char** word_line){

    int pid = fork();
    char* filename = word_line[1];
    char *args[] = { filename, NULL };


    if (pid==0){
        //execv(filename, word_line);
        execv(filename, args);
        exit(EXIT_SUCCESS);
    }


//1. pid du processus fils p our le p ère
//2. 0 pour le fls
    return EXIT_SUCCESS;
}

int action_exec_red(char** word_line){

    char* filenameredirection = word_line[3];
    // on ouvre le fichier en lecture et en écriture
    int newdescriptor =  open(filenameredirection, O_RDWR);
    //printf("des1%d", newdescriptor);
    if (newdescriptor==-1){
        // on crée le fichier en écriture
        fopen(filenameredirection, "w");
        // on re récupère le filedescriptor du fichier
        newdescriptor =  open(filenameredirection, O_RDWR);
    }
    //printf("des2%d", newdescriptor);

    // on change la sortie standard par notre fichier
    dup2(newdescriptor, STDOUT_FILENO);

    int pid = fork();
    char* filename = word_line[1];
    char *args[] = { filename, NULL };

    if (pid==0){
        execv(filename, args);
        exit(EXIT_SUCCESS);
    }

    // on remet la sortie standard
    // marche pas ça sort dans le fichier ..
    // le execv bloque tout
    // printf("passage");
    dup(1);
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

        // for (int j = 0; j < taille-1; j++){
        //     printf("%d : %s\n",j, word_line[j]);
        // }

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
                    //Et test si il y a bien cd en argument
                    action_cd(word_line);
                }
            } else if (eqcmd(word_line[0], "ls")){
                //Affiche les fichiers dans le dossier courant
                action_ls(word_line);
            }

             else if (eqcmd(word_line[0], "exec")){

                 if (taille>=2){

                     if (taille==3){
                        action_exec(word_line);

                     }
                        //action_exec(word_line);
                     // Redirection
                     else if (eqcmd(word_line[2], ">")){
                        action_exec_red(word_line);

                     }
                     else {
                         printf("Mauvais arguments");
                     }

                 }
                 else {
                     printf("%s", "Pas de nom de fichier");
                 }
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
