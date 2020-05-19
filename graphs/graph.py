# Ce script va traçer differents graphes qui vont contenir des courbes
# pour les differents tests en fonction du temps d'exécution pour les deux
# bibliothèques thread et pthread

import numpy as np
import matplotlib.pyplot as plt


## Récupération des données

# Collecte des données de notre interface
file1 = open("donnees.txt","r")
donnees = file1.readlines()
file1.close()

# Collecte des données de pthread
file2 = open("donnees_pthread.txt","r")
donnees_pthread = file2.readlines()
file2.close()

print("Drawing data")

## Tracage les courbes
def donnee_utile(donnees,i) :
    tableau = donnees[i].split(" ")
    length = len(tableau)
    return tableau[1:length-1]

# tracer les deux courbes de create-many
tab = donnee_utile(donnees,2)
tab_pthread = donnee_utile(donnees_pthread,2)
X_create_many = np.array(range(int(donnees[1])))
Y_create_many = np.array(list(map(int, tab)))
Y_create_many_pthread = np.array(list(map(int, tab_pthread)))
plt.subplot(221)
plt.plot(X_create_many,Y_create_many, label="thread")
plt.plot(X_create_many, Y_create_many_pthread, label="pthread")
plt.title("courbe de create-many")
plt.xlabel("nombre de threads crées")
plt.ylabel("temps d'exécution en us")
plt.legend()
#plt.show()


# tracer les deux courbes de create-many-recursive
tab = donnee_utile(donnees,4)
tab_pthread = donnee_utile(donnees_pthread,4)
X_create_many_recursive = np.array(range(int(donnees[3])))
Y_create_many_recursive = np.array(list(map(int, tab)))
Y_create_many_recursive_pthread = np.array(list(map(int, tab_pthread)))
plt.subplot(222)
plt.plot(X_create_many_recursive,Y_create_many_recursive, label="thread")
plt.plot(X_create_many_recursive,Y_create_many_recursive_pthread, label="pthread")
plt.title("courbe de create-many-recursive")
plt.xlabel("nombre de threads crées")
plt.ylabel("temps d'exécution en us")
plt.legend()
#plt.show()

# tracer les deux courbes de create-many-once

tab = donnee_utile(donnees,6)
tab_pthread = donnee_utile(donnees_pthread,6)
X_create_many_once = np.array(range(int(donnees[5])))
Y_create_many_once = np.array(list(map(int, tab)))
Y_create_many_once_pthread = np.array(list(map(int, tab_pthread)))
plt.subplot(223)
plt.plot(X_create_many_once,Y_create_many_once, label="thread")
plt.plot(X_create_many_once,Y_create_many_once_pthread, label="pthread")
plt.title("courbe de create-many-once",y=-0.3)
plt.xlabel("nombre de threads crées")
plt.ylabel("temps d'exécution en us")
plt.legend()
#plt.show()


# tracer les deux courbes de resultat de fibonacci
tab = donnee_utile(donnees,len(donnees)-1)
tab_pthread = donnee_utile(donnees_pthread,len(donnees)-1)
X_fibonacci = np.array(range(int(donnees[len(donnees)-2])))
Y_fibonacci = np.array(list(map(float, tab)))
Y_fibonacci_pthread = np.array(list(map(float, tab_pthread)))
plt.subplot(224)
plt.plot(X_fibonacci,Y_fibonacci, label="thread")
plt.plot(X_fibonacci,Y_fibonacci_pthread, label="pthread")
plt.title("courbe de fibonacci",y=-0.3)
plt.xlabel("nombre en entré")
plt.ylabel("temps d'exécution en s")
plt.legend()
plt.show()



# tracer les deux courbes de resultats de switch-many
nb_thread_switch_many = int(donnees[7])
print(nb_thread_switch_many)
nb_yield_switch_many = int(donnees[8])
X  = range(nb_yield_switch_many)
j = 331 # pour le cas de 6 tests de threads 
for i in range(9,9+nb_thread_switch_many) :
    a_thread = donnees[i][5:].split(" ")
    a_pthread = donnees_pthread[i][5:].split(" ")
    Y_thread = list(map(int, a_thread[:nb_yield_switch_many]))
    Y_pthread = list(map(int, a_pthread[:nb_yield_switch_many]))
    print(j)
    plt.subplot(j)
    if j == 333 :
        j += 3
        
    j += 1
    plt.plot(X,Y_thread, label="thread")
    plt.plot(X,Y_pthread, label="pthread")
    plt.title("courbe de switch-many pour " + str(i-9) + " thread")
    plt.xlabel("nombre de yield")
    plt.ylabel("temps en us")
    plt.legend()
plt.show()

# transition entre donnees de switch-many et switch-many-join
indice_debut = 8+nb_thread_switch_many +1
nb_thread_switch_many_join = int(donnees[indice_debut])
nb_yield_switch_many_join = int(donnees[indice_debut+1])

# tracer les deux courbes de resultats de switch-many-join
X = range(nb_yield_switch_many_join)
j = 331 # pour le cas de 6 tests de threads 
for i in range(indice_debut+2,indice_debut+2+nb_thread_switch_many_join) :
    a_thread = donnees[i][5:].split(" ")
    a_pthread = donnees_pthread[i][5:].split(" ")
    Y_thread = list(map(int, a_thread[:nb_yield_switch_many_join]))
    Y_pthread = list(map(int, a_pthread[:nb_yield_switch_many_join]))
    plt.subplot(j)
    if j == 333 :
        j += 3
    j += 1
    plt.plot(X,Y_thread, label="thread")
    plt.plot(X,Y_pthread, label="pthread")
    plt.title("cb de switch-many-join pour " + str(i-indice_debut-2) + " thread")
    plt.xlabel("nombre de yield")
    plt.ylabel("temps en us")
    plt.legend()
plt.show()

# transition entre donnees de switch-many_join et switch-many-cascade
# indice_debut = indice_debut+nb_thread_switch_many_join +2
# nb_thread_switch_many_cascade = int(donnees[indice_debut])
# nb_yield_switch_many_cascade = int(donnees[indice_debut+1])

# tracer les deux courbes de resultats de switch-many-cascade
# X = range(nb_yield_switch_many_cascade)
# j = 331 # pour le cas de 6 tests de threads 
# for i in range(indice_debut+2,indice_debut+2+nb_thread_switch_many_cascade) :
#     a_thread = donnees[i][5:].split(" ")
#     a_pthread = donnees_pthread[i][5:].split(" ")
#     Y_thread = list(map(int, a_thread[:nb_yield_switch_many_cascade]))
#     Y_pthread = list(map(int, a_pthread[:nb_yield_switch_many_cascade]))
#     plt.subplot(j)
#     if j == 333 :
#         j += 3
#     j += 1
#     plt.plot(X,Y_thread, label="thread")
#     plt.plot(X,Y_pthread, label="pthread")
#     plt.title("cb de switch-many-cascade pour " + str(i-indice_debut-2) + " thread")
#     plt.xlabel("nombre de yield")
#     plt.ylabel("temps en us")
#     plt.legend()

print("Done")