# Projet système - Threads en espace utilisateurs (2A ENSEIRB-MATMECA)

### Présentation

Ce projet a pour but de réaliser une bibliothèque de *threads* en espace utilisateur en langage **C**, avec pour objectif d'avoir un comportement proche de la bibliothèque **pthread**. Pour cela, de nombreuses fonctions ont été implémentées, les fonctions utilisateur étant les fonctions suivantes : 

-   thread_t thread_self(void);
-   int thread_create(thread_t *newthread, void *(*func)(void *), void *funcarg);
-   int thread_yield(void);
-   int thread_join(thread_t thread, void **retval);
-   void thread_exit(void *retval);

Ces fonctions s'utilisent comme les fonctions équivalentes de la bibliothèque **pthread**.
Des fonctionnalités de *mutex* ont également été implémentées avec les fonctions suivantes :

-   int thread_mutex_init(thread_mutex_t *mutex);
-   int thread_mutex_destroy(thread_mutex_t *mutex);
-   int thread_mutex_lock(thread_mutex_t *mutex);
-   int thread_mutex_unlock(thread_mutex_t *mutex);

Comme pour les manipulations de *threads*, la manipulation de *mutex* se réalise de la même manière qu'avec la bibliothèque **pthread**.

Ce projet a été réalisé de Février 2020 à Mai 2020, dans le cadre du projet de Système d'Exploitation (IT202) de l'ENSEIRB-MATMECA.

### Compilation

Ce projet possède une recette **Makefile** pour la compilation.

Pour compiler le projet : taper la commande `make` à la racine du projet.

Dans le répertoire **install/** se trouvent maintenant plusieurs dossiers.

- Le premier est le répertoire **lib/**. Celui-ci contient la bibliothèque de ce projet sous forme statique.
- Le deuxième est le répertoire **bin/**. Il contient l'ensemble des tests fournis par les encadrants compilés et prêts à être directement exécutés.
- Le dernier est le répertoire **custom_bin/**. Il contient l'ensemble des tests réalisés par l'équipe de développement compilés et prêt à être directement exécutés.

Une commande `make clean` peut également être lancée pour nettoyer le répertoire **install/**.

### Tests

Pour nous permettre de valider le comportement de la bibliothèque de *threads*, tous les tests fournis par les encadrants doivent avoir le même comportement que **pthread** et empêcher toute fuite mémoire. Cette dernière métrique a été mesurée grâçe à **Valgrind**.

Une vérification de l'exécution normale de la bibliothèque de ce projet peut être réalisée en tapant les commandes `make check` (pour les tests fournis) et `make check_customs` (pour les tests écrits).

De plus, le Makefile possède une commande `make graphs` qui permet de tracer des graphes, réalisés en **Python**. Ces graphes mesurent les différences de temps d'exécution entre la bibliothèque réalisé dans ce projet et la bibliothèque **pthread**.

### Arborescence

Ce projet se découpe en plusieurs répertoires :
- Le répertoire **src/** contient le code source du projet
- Le répertoire **tests/** contient les tests fournis par l'équipe encadrante
- Le répertoire **custom_tests/** contient les tests réalisés par l'équipe de développement
- Le répertoire **graphs/** contient l'ensemble des scripts nécessaires à l'exécution de la règle `make graphs`

### Remerciements

Merci à l'équipe de développement : 

- Sidi Abdel Malick
- Zineb Baroudi
- Juliette Deguillaume
- Kais-Khan Hadi
- Lucas Trocherie
