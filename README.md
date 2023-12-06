# TP de Synthèse – Ensea in the Shell

## 1. Affichage d’un message d’accueil : 
Pour faire l'affichge de ce message nous avons utilisé la fonction :   
#### ssize_t write(int fd, const void buf[.count], size_t count);
- La sortie standard STDOUT_FILENO.

- Le message est passé sous forme d'une chaîne de caractères déclarée en #define MESSAGE dans main.c.

- La taille de ce message est calculée par la fonction strlen(MESSAGE).
## 2. Exécution de la commande saisie et retour au prompt :
le but ici est de lire une commande saisie et ensuite executer cette commande (simple sans argument), pour la lecture de cette commande nous avons utilisé la fonction suivante :
#### ssize_t read(int fd, void buf[.count], size_t count); 

Cette fonction fonctionne de la même manière que la fonction write() sauf que au lieu d'affchier le message sur la sortie standard, on récupère la commande saisie sur l'entrée standard STDIN_FILENO.

On crée un processus enfant à l'aide de la fonction fork(), puis utiliser wait() dans le processus parent pour attendre la fin de l'exécution du processus enfant. Après cela, on exécute le processus enfant en utilisant execvp(). un messgae d'erreur s'affichera dans le cas où on est ni dans le processus père ou ni dans le processus enfant. 

## 3.Gestion de la sortie du shell avec la commande “exit” ou un <ctrl>+d :

#### int strcmp(const char *s1, const char *s2);
Cette fonction nous renvoie zéro quand les deux chaînes de caractères sont égales. On arrête le code quand la commande d'entrée est égale à "exit" avec strcmp() ou quand (cmd_size==0) pour la commande entrée est Ctrl+D.

## 4.Affichage du code de retour (ou du signal) de la commande précédente dans le prompt :  

Dans cette partie on s'intéresse de gestion des statuts de fin de processus :

- la fonction WIFEXITED(status) vérifie si le processus enfant s'est terminé normalement (par exemple, en utilisant la fonction exit() ou return dans le code).

- WEXITSTATUS(status) extrait la valeur de sortie du processus enfant qui indique la raison de la fin du processus (le code de sortie).

- WIFSIGNALED(status) vérifie si le processus enfant s'est terminé à cause d'un signal.

- WTERMSIG(status) récupère le numéro du signal qui a causé la terminaison du processus enfant. 

## 5.Mesure du temps d’exécution de la commande en utilisant l’appel clock_gettime :

start_counting et end_counting sont des structures timespec vont nous permettre de enregistrer le temps de début et de fin de l'exécution des commandes. donc l'aide de ses deux structures et clock_gettime  on mesure Le temps avant et après l'exécution de la commande dans le processus enfant. 

Le temps est calculé en millisecondes en divisant le résultat donné par tv_nsec(nanosecondes) par 1000000.

## 6.Exécution d’une commande complexe (avec arguments) :

notre but ici est de diviser une chaîne de caractères string_to_split en tokens (morceaux) en utilisant le délimiteur delim(espace ou " ").

pour faire cela on va utiliser la fonction :

#### char *strtok(char *restrict str, const char *restrict delim);
la fonction strtok prend en arguments la chaîne à découper avec le séparateur (" "), où Chaque token est stocké dans une variable token.
