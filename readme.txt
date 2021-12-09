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




6.

L'appel système fork() permet à un processus de créer un processus similaire au processus courant (père)
Elle retourne 0 au processus fils et en cas de succès le père reçoit le pid du processus crée.
Le père reçoit -1 en cas d'échec