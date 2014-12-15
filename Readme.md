# Algorithme de Dijkstra #
Nous avons implémenté l'algorithme de Dijkstra utilisant les tas de
Fibonacci.

## Compilation des sources ##
Il faut utiliser l'outils `make` pour compiler toutes les sources. Les
cibles disponibles sont :

1. `dijkstra_fib`, un utilitaire C qui lit un graphe, fait tourner
l'algorithe à partir du point 0 dessus et renvoie le résultat en
utilisant les tas de Fibonacci ;
2. `dijkstra_queue`, un utilitaire C qui lit un graphe, fait tourner
l'algorithe à partir du point 0 dessus et renvoie le résultat en
utilisant une queue naïve ;
3. `pyfib` qui génère un wrapper Python3 pour l'algorithme avec les
   tas ;
4. `pyqueue` qui génère un wrapper Python3 pour l'algorithme avec les
queues ;
5. `test_fib`, `test_queue` teste toutes les entrées du dossier tests/ et compare à
la sortie attendue dans le dossier tests_outputs/ ;

## Utilisation des binaires ##
Les binaires `dijkstra_*` prennent en entrée un graphe (depuis un
fichier ou depuis `stdin`). Ils donnent en sortie les distances et les
prédecesseurs de tous les nœuds.

## Utilisation des scripts python ##
Un script python appelé `path_find.py` qui prend en entrée une carte
OSM ainsi qu'une adresse de départ (par exemple : "45 rue d'Ulm,
Paris"). Il est de plus possible de spécifier un `dump_file` dans
lequel sont stockés quelques méta-données sur les transports publiques
les plus proches de l'adresse.
