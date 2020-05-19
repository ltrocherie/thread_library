#!/bin/bash

# Ce script est utilisé pour récupérer les temps d'exécutions de tous les tests présents 
# dans le répertoire test.
# Les tests sont d'abord lancés avec cette dernière puis avec pthread.

echo "Collecting Data"

echo " " > stock.txt



# Les données récupérées sont les temps d'exécution exprimées en fonction de la valeur 
# des arguments passés aux différents tests.
# Ces arguments ont été choisis de manière à observer des tendances entre les deux 
# implémentations, atteignant des valeurs volontairement élevées.


# ----------------------------threads------------------------------------

# tests réalisés sur notre interface de threads

results1=`cd .. && make` 
echo $results1 > stock.txt
echo " " > stock.txt
echo " " > donnees.txt

# données de 21-create-many.c
X_create_many=`seq 0 1000`
Y_create_many=()
echo "1001" >> donnees.txt
for nb_thread in ${X_create_many[*]}
do
    path=`./../install/bin/21-create-many $nb_thread`
    echo $path > stock.txt  
    res=`cut -d ' ' -f 8 stock.txt`
    Y_create_many[$nb_thread]=$res
done

echo " ${Y_create_many[*]} " >> donnees.txt


# données de 22-create-many-recursive.c
X_create_many_recursive=`seq 0 1000`
Y_create_many_recursive=()
echo "1001" >> donnees.txt
for nb_thread in ${X_create_many_recursive[*]}
do
    path=`./../install/bin/22-create-many-recursive $nb_thread`
    echo $path > stock.txt  
    res=`cut -d ' ' -f 8 stock.txt`
    Y_create_many_recursive[$nb_thread]=$res  
done

echo " ${Y_create_many_recursive[*]} " >> donnees.txt


# données de 23-create-many-once.c
X_create_many_once=`seq 0 1000`
Y_create_many_once=()
echo "1001" >> donnees.txt
for nb_thread in ${X_create_many_once[*]}
do
    path=`./../install/bin/23-create-many-once $nb_thread`
    echo $path > stock.txt  
    res=`cut -d ' ' -f 10 stock.txt`
    Y_create_many_once[$nb_thread]=$res  
done

echo " ${Y_create_many_once[*]} " >> donnees.txt


# données de 31-switch-many.c
X_switch_many=`seq 0 5`
X_switch_many_yield=`seq 0 500`
Y_switch_many=()
echo "6" >> donnees.txt
echo "501" >> donnees.txt
for nb_thread in ${X_switch_many[*]}
do
    for yield in ${X_switch_many_yield[*]}
    do
	path=`./../install/bin/31-switch-many $nb_thread $yield`
	echo $path > stock.txt  
	res=`cut -d ' ' -f 6 stock.txt`
	Y_switch_many[$yield]=$res
    done
    echo "$nb_thread :  ${Y_switch_many[*]} " >> donnees.txt
done


# données de 31-switch-many-join.c
X_switch_many_join=`seq 0 5`
X_switch_many_join_yield=`seq 0 500`
Y_switch_many_join=()
echo "6" >> donnees.txt
echo "501" >> donnees.txt
for nb_thread in ${X_switch_many_join[*]}
do
    for yield in X ${X_switch_many_join_yield[*]}
    do
	path=`./../install/bin/32-switch-many-join $nb_thread $yield`
	echo $path > stock.txt  
	res=`cut -d ' ' -f 9 stock.txt`
	Y_switch_many_join[$yield]=$res
    done
    echo "$nb_thread :  ${Y_switch_many_join[*]} " >> donnees.txt
done


# données de 51-fibonacci
echo " " > stock.txt
X_fibonacci=`seq 0 20`
Y_fibonacci=()
echo "21" >> donnees.txt
for ent in ${X_fibonacci[*]}
do
    res=`./../install/bin/51-fibonacci $ent | rev | cut -d ' ' -f 2 | rev | tail -n 1`
    Y_fibonacci[$ent]=$res
done

echo " ${Y_fibonacci[*]} " >> donnees.txt


# ----------------------------pthreads------------------------------------

# Dans cette partie, les tests sont réalisés en utilisant pthread.

results2=`cd .. && make pthreads` 
echo $results2 > stock.txt
echo " " > stock.txt
echo " " > donnees_pthread.txt


# données de 21-create-many.c
X_create_many_pthread=`seq 0 1000`
Y_create_many_pthread=()
echo "1001" >> donnees_pthread.txt
for nb_thread in ${X_create_many_pthread[*]}
do
    path=`./../install/bin/21-create-many $nb_thread`
    echo $path > stock.txt  
    res=`cut -d ' ' -f 8 stock.txt`
    Y_create_many_pthread[$nb_thread]=$res
done

echo " ${Y_create_many_pthread[*]} " >> donnees_pthread.txt

# données de 22-create-many-recursive.c
X_create_many_recursive_pthread=`seq 0 1000`
Y_create_many_recursive_pthread=()
echo "1001" >> donnees_pthread.txt
for nb_thread in ${X_create_many_recursive_pthread[*]}
do
    path=`./../install/bin/22-create-many-recursive $nb_thread`
    echo $path > stock.txt  
    res=`cut -d ' ' -f 8 stock.txt`
    Y_create_many_recursive_pthread[$nb_thread]=$res  
done

echo " ${Y_create_many_recursive_pthread[*]} " >> donnees_pthread.txt


# données de 23-create-many-once.c
X_create_many_once_pthread=`seq 0 1000`
Y_create_many_once_pthread=()
echo "1001" >> donnees_pthread.txt
for nb_thread in ${X_create_many_once_pthread[*]}
do
    path=`./../install/bin/23-create-many-once $nb_thread`
    echo $path > stock.txt  
    res=`cut -d ' ' -f 10 stock.txt`
    Y_create_many_once_pthread[$nb_thread]=$res  
done

echo " ${Y_create_many_once_pthread[*]} " >> donnees_pthread.txt


# données de 31-switch-many.c
X_switch_many_pthread=`seq 0 5`
X_switch_many_yield_pthread=`seq 0 500`
Y_switch_many_pthread=()
echo "6" >> donnees_pthread.txt
echo "501" >> donnees_pthread.txt
for nb_thread in ${X_switch_many_pthread[*]}
do
    for yield in ${X_switch_many_yield_pthread[*]}
    do
	path=`./../install/bin/31-switch-many $nb_thread $yield`
	echo $path > stock.txt  
	res=`cut -d ' ' -f 6 stock.txt`
	Y_switch_many_pthread[$yield]=$res
    done
    echo "$nb_thread :  ${Y_switch_many_pthread[*]} " >> donnees_pthread.txt
done


# données de 31-switch-many-join.c
X_switch_many_join_pthread=`seq 0 5`
X_switch_many_join_yield_pthread=`seq 0 500`
Y_switch_many_join_pthread=()
echo "6" >> donnees_pthread.txt
echo "501" >> donnees_pthread.txt
for nb_thread in ${X_switch_many_join_pthread[*]}
do
    for yield in X ${X_switch_many_join_yield_pthread[*]}
    do
	path=`./../install/bin/32-switch-many-join $nb_thread $yield`
	echo $path > stock.txt  
	res=`cut -d ' ' -f 9 stock.txt`
	Y_switch_many_join_pthread[$yield]=$res
    done
    echo "$nb_thread :  ${Y_switch_many_join_pthread[*]} " >> donnees_pthread.txt
done


# données de 51-fibonacci
echo " " > stock.txt
X_fibonacci_pthread=`seq 0 20`
Y_fibonacci_pthread=()
echo "21" >> donnees_pthread.txt
for ent in ${X_fibonacci_pthread[*]}
do
    res=`./../install/bin/51-fibonacci $ent | rev | cut -d ' ' -f 2 | rev | tail -n 1`    
    Y_fibonacci_pthread[$ent]=$res    
done
echo " ${Y_fibonacci_pthread[*]} " >> donnees_pthread.txt



# Affichage dans la sortie standard
echo "Data collected"
