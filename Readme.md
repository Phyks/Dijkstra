# Algorithm de Dijkstra #
Nous avons implémenté l'algorithme de Dijkstra utilisant les tas de
Fibonacci. Toutes les options de compilation sont accessibles via la
commande `make` avec les bonnes cibles. Les binaires suivants peuvent
être compilés:

1. `dijkstra_fib` et `dijkstra_queue` /via/ les commandes `make
   dijkstra_fib` et `make dijkstra_queue`. Le binaire généré est un
   programme C capable de parser un fichier de graphe. Il n'échoue pas
   sur des graphes non connexes. Sa sortie donne pour chaque nœud la
   distance au point de départ (codé en dur à 0) ainsi que son
   prédecesseur. 
2. `pyfib` et `pyqueue` génèrent un wrapper Python3 de l'algorithme. Il
   est alors possible d'utiliser le script `path_finder.py` qui prend en
   entrée une carte (des cartes sont disponibles dans le dossier
   `maps`, se méfier des grosses cartes sur une petite configuration)
   ainsi qu'une adresse, et renvoie la liste des transports en commun
   les plus proches. Par exemple : `python3 path_finder.py
   maps/5eme_grand.osm "45 rue d'Ulm,Paris"`
   NB : le premier appel est susceptible d'être longue, le script
   allant récupérer des listes de station sur un serveur assez
   lent. Pour éviter d'avoir un second appel aussi lent, renseigner
   exactement la même adresse.
