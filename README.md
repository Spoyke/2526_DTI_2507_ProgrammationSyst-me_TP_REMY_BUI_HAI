# ENSEASH — ENSEA Shell

Un shell Unix minimaliste développé en C dans le cadre du cours de **Programmation Système** à l'ENSEA.

**Auteurs :** Simon REMY & Christophe BUI HAI

---

## Présentation

ENSEASH est un interpréteur de commandes (shell) implémenté from scratch en C, sans utiliser la bibliothèque standard de haut niveau. Il reproduit les fonctionnalités essentielles d'un shell Unix : exécution de commandes, gestion des arguments, redirections d'entrée/sortie, et affichage du statut de sortie avec le temps d'exécution.

---

## Fonctionnalités

### Exécution de commandes
ENSEASH exécute n'importe quelle commande disponible dans le `PATH` du système. Chaque commande est lancée dans un processus fils via `fork()` + `execvp()`, ce qui permet au shell de rester actif après l'exécution.

### Gestion des arguments
Les commandes peuvent être passées avec plusieurs arguments séparés par des espaces. Par exemple :
```
enseah % ls -la /tmp
```

### Redirections
- **Redirection de la sortie standard** (`>`) : redirige la sortie d'une commande vers un fichier (créé ou écrasé).
  ```
  enseah % ls > fichiers.txt
  ```
- **Redirection de l'entrée standard** (`<`) : lit l'entrée d'une commande depuis un fichier.
  ```
  enseah % wc -l < fichiers.txt
  ```

### Affichage du statut et du temps d'exécution
Après chaque commande, le prompt indique :
- Le **code de retour** (exit) ou le **numéro de signal** (sign) ayant terminé le processus.
- Le **temps d'exécution** en millisecondes.

```
enseah [exit:0|12ms] %
enseah [sign:11|3ms] %
```

### Quitter le shell
- Taper `exit` dans le prompt.
- Envoyer `Ctrl+D` (fin de fichier sur l'entrée standard).

Dans les deux cas, le shell affiche `Bye bye...` avant de se terminer.

---

## Compilation et utilisation

### Prérequis
- Un compilateur C (`gcc` ou `clang`)
- Un système Unix/Linux

### Compilation
```bash
gcc -Wall -Wextra -o enseash main.c
```

### Lancement
```bash
./enseash
```

```
Bienvenue dans le Shell ENSEA.
Pour quitter, tapez 'exit'
enseah %
```

---

## Structure du code

| Fonction | Description |
|---|---|
| `main()` | Boucle principale du shell : lecture, parsing et exécution des commandes |
| `strslice()` | Découpe une chaîne en tableau d'arguments (délimiteur : espace) |
| `searchChar()` | Recherche un caractère dans un buffer (utilisé pour détecter `\n`) |
| `changestdout()` | Redirige la sortie standard vers un fichier via `dup2()` |
| `changestdin()` | Redirige l'entrée standard depuis un fichier via `dup2()` |

---

## Limitations connues et pistes d'amélioration

- **Pipes (`|`)** : partiellement explorés mais non implémentés. L'approche envisagée consiste à créer autant de processus fils que de segments séparés par `|`, en reliant leurs entrées/sorties avec `pipe()`.
- **Historique des commandes** : non implémenté.
- **Combinaison `<` et `>`** : les deux redirections peuvent être spécifiées dans la même commande mais le comportement n'a pas été exhaustivement testé.
- La taille du buffer d'entrée est fixée à 128 octets (`BUFFER_SIZE`).

---

## Syscalls et fonctions système utilisés

`read`, `write`, `fork`, `execvp`, `wait`, `open`, `dup2`, `clock_gettime`, `strtok`
