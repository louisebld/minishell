# Louise Bollard - Tom Thierry

1.
La fonction getcmd sert à subdiviser une chaîne rentrée en fonction des espaces.
Nous mettons le résultat dans CMD_ELEMS.
Nous avions un problème, il fallait obligatoirement mettre un espace après chaque commande pour qu'elle fonctionne.
Nous n'avions pas mis "\n" et "\t" dans les séparateurs de strtok.

1.2
Nous comparons les deux chaînes de caractères passées en paramètres avec la fonction strcmp.

2.
Nous detectons quand le premier mot de notre tableau de mot fait à partir de la ligne entrée est "exit"
Si c'est le cas on arrête la boucle while et on fait un appel à exit dans la fonction action_exit()

Dans le bash le résultat de echo $? est 0
Cela signifie que notre dernière commande a été exécutée correctement.

3.1
Nous avons utilisé getcwd(..) pour récupérer le repertoire courant

3.3

Le répertoire de travail actuel d'un processus est stocké par le noyau.
L'autre repertoire associé au processus est le / (root)

6.

L'appel système fork() permet à un processus de créer un processus similaire au processus courant (père)
Elle retourne 0 au processus fils et en cas de succès le père reçoit le pid du processus crée.
Le père reçoit -1 en cas d'échec
Si la réponse du fork() est 0 alors on peut lancer la commande d'éxécution immédiatement mais ce n'est pas automatique.

Les éléments communs au père et au fils sont : le groupe de processus (pgrp), la session et le leader du groupe de processus.
Le usr est commun également.

Le PID du fils ne change pas après un appel un exec(). Cet appel écrase le processus fils qui contient alors le code cloné du père par un nouveau programme.

La fonctionnalité exec marche : exec hellojohn
mais le exec hellojohn > test.txt marche à moitié. Nous ne sommes pas arriver à rechanger la sortie standard du coup toutes les commandes tapées après sont écrites dans le fichier.


6.1

dup2(fd,STDOUT_FILENO) permet de rediriger la sortie standard sur le fichier qu'on veut en lui donnant le file descriptor.
dup2 vient donc créer une nouvelle entrée dans la table de File Descriptor.
La sortie STDOUT est donc relié non plus à 0 mais au nouveau file descriptor, donc a notre fichier.

