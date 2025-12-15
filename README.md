# [Programmation système] Compte rendu 

## Simon REMY et Christophe

### Question 1 :

La première question consiste en afficher un texte au lancement du programme. Pour cela, on a définie les numéro des fichiers d'entrée, de sortie et d'erreur standard. On utilise la fonction ```write``` pour écrire dans le fichier de sortie standard soit le terminal pour afficher le message voulu : 

<img width="428" height="94" alt="image" src="https://github.com/user-attachments/assets/ad09f1b2-d246-4b4d-b426-0268213200cc" />

### Question 2 :

Pour cette question, plutôt que d'utiliser le fichier de sortie standard, on lit l'entrée standard avec la fonction ```read``` qui nous permet de stocker, dans un tableau de caractère ce que l'utilisateur écrit. Une fois l'entrée stocké, on peut utiliser la fonction ```execlp``` qui exécute la commande stockée dans buffer. A noter qu'il faut appeler cette fonction dans un autre processus (un processus fils) que celui qui exécute le code car ```execlp``` va substituer le processus actuel par le processus exécutant la commande, ce qui nous empêche de rester dans le code du projet et ainsi de reboucler sur le message ```enseah %``` après ```execlp``` (avec une boucle while(1)).

<img width="549" height="132" alt="image" src="https://github.com/user-attachments/assets/93d740e2-45f9-47f2-bc41-6489738f8808" />

### Question 3 :

### Question 4 :

Cette question consiste à regarder si le processus fils s'est terminé normalement (avec un return) ou par un signal. Pour cela on utilise la valeur de retour du fils (status) pour changer l'affichage du terminal. Il y a quatres fonctions utiles vu en TD : ```WIFEXITED``` qui indique si le processus s'est arrêté normalement, ```WIFSIGNALED``` qui indique si le processus s'est arrêté avec un signal (kill par exemple), ```WEXITSTATUS```  et ```WTERMSIG``` qui traduisentt status en un entier correspondant à la valeur de retour du return ou du signal de fin du processus.

<img width="738" height="68" alt="image" src="https://github.com/user-attachments/assets/12ba0b0d-dd2e-441b-a55b-761fbccbd7b0" />

### Question 5 : 

### Question 6 :

Pour gérer des commandes avec plusieurs arguments, il faut étudier l'entrée de l'utilisateur en séparant ce string en un tableau de string composé de tous les éléments du buffer, séparé à chaque caractère espace. Par exemple, on transforme ```"ls -a"``` en ```["ls", "-a"]```. En adaptant la commande de cette façon, on peut appelér la fonction ```execlp``` en donnant directement le tableau des arguments.

Pour faire cette fonction, on utilise la fonction ```strtok``` qui renvoie un pointeur vers la chaîne de caractère allant du début de buffer jusqu'au premier espace ' '. Il suffit ensuite de stocké cette chaîne de caractère et de réutiliser ```strtok``` jusqu'à ce qu'il n'y ait plus de séparation possible.

<img width="729" height="119" alt="image" src="https://github.com/user-attachments/assets/008339d4-550c-4fbe-b35d-807052cd42bb" />

### Question 7 :

Cette question nous demande de rediriger les entrées et sorties des processus vers un fichier défini par l'utilisateur. Pour cela, on utilise la fonction de la question 6 et on regarde si le caractère '<' ou '>' apparaît dans la liste des arguments. Si c'est le cas, alors on change la sortie standard (>) ou l'entrée standard (<) en utilisant la fonction ```dup2``` qui permet ici de changer le file descriptor de l'entrée ou la sortie standard. Avec cette fonction, on peut faire la redirection voulu. Par exemple, en faisant ```ls > filelist.txt``` on modifie la sortie standard par ```filelist.txt``` qui est ouvert ou créé (s'il n'existait pas) et qui stocke la sortie de ```ls```. Et inversement pour par exemple : ```wc -l < filelist.txt```, la commande ```wc -l``` va compter le nombre de ligne qu'il y a dans ```filelist.txt```. Enfin, on remplace le caractère '<' ou '>' par NULL, ce qui indique à ```execlp``` qu'il s'agit de la fin de la commande. Les arguments suivants seront considérés comme une autre commande. 

<img width="727" height="233" alt="image" src="https://github.com/user-attachments/assets/b5cf7049-7736-4980-8e08-f3ae61c2bf9a" />

### Question 8 :

On a commencer à travailler sur le code de la gestion du pipe '|' mais on a pas réussi à temps de finir le code. L'idée derrière le code est de créer un processus fils de plus qu'il y a de '|' et ensuite utiliser la fonction ```pipe``` pour rediriger la sortie du premier fils vers l'entrée du second fils et attendre la fils du dernier fils qui exécute la dernière commande avant de retrouner dans le processus père et reboucler. 
