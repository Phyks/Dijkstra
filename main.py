#!/usr/bin/env python3
"""This program converts osm maps to an entry point for the dijktra program."""
from xml.etree import ElementTree as et
from math import cos, sin, atan2, pi, sqrt
import sys
from subprocess import PIPE, Popen
import pyjkstra
import time
from api_request import distance
import api_request as ar

def tt():
    return round(time.process_time(),2)

class osmMap:
    # remaped dictionary of all nodes (0 ≤ n < #nodes)
    nodes = dict()
    inverted_dict = []
    roads = []
    # stores the distances between two nodes u<v as two lists:
    # distances[u] = ([v0, v1, …], [d0, d1, …])
    distances = dict()         

    def __init__(self, osm_file):
        try:
            self.tree = et.parse(osm_file)

        except:
            print("Impossible to open {}.".format(osm_file))
            sys.exit()
        self.updateRoads()

    def updateRoads(self):
        root = self.tree.getroot()
        # get all the points
        child_id = 0
        for child in root.findall("node"):
            id = int(child.get("id"))
            if id not in self.nodes:
                self.nodes[id] = {'id': None, 'val': None, 'roads': []}

            self.nodes[id]["id"] = child_id
            self.nodes[id]["val"] = child
            self.inverted_dict.append(int(child.get("id")))
            child_id += 1

        # get all the roads ('way' with a child that has the attribute k=hway)
        # and save the roads to which each point belongs
        for way in root.findall('.//way/*[@k="highway"]/..'):
            self.roads.append(way)
            wayname = way.find('./tag[@k="name"]')
            if wayname is not None:
                name = wayname.get("v")
            else:
                name = "Autre"

            # associate the name of the way to each of its node
            for nd in way.findall('./nd'):
                id = int(nd.get("ref"))
                self.nodes[id]["roads"].append(name)

            

    
    def parse(self, funNNode, funAdd):
        """ This method runs through the data and applies funNNode to the number
        of nodes and funAdd for each segment of each way. It doesn't return
        anything.

        funNNode: takes a number of node
        funAdd: takes two nodes and a distance
        """
        funNNode(len(self.nodes))

        # iterate over all ways
        for road in self.roads:
            # get all nodes in the road
            node_list = road.findall("nd")

            # calculate the distance between each two-points
            for u_ref, v_ref in zip(node_list[:-1], node_list[1:]):
                u_id = int(min(u_ref.get("ref"), v_ref.get("ref")))
                v_id = int(max(u_ref.get("ref"), v_ref.get("ref")))

                if u_id in self.distances:
                    if v_id in self.distances[u_id][0]:
                        index = self.distances[u_id][1].index(v_id)
                        dist = self.distances[u_id][1][index]
                    else:
                        u = self.nodes[u_id]['val'],
                        u_id = self.nodes[u_id]['id']
                        v = self.nodes[v_id]['val']
                        v_id = self.nodes[v_id]['id']
                        
                        dist = distance(u, v)

                        self.distances[u_id][0].append(v_id)
                        self.distances[u_id][1].append(dist)
                else:
                    u, u_id = self.nodes[u_id]['val'], self.nodes[u_id]['id']
                    v, v_id = self.nodes[v_id]['val'], self.nodes[v_id]['id']

                    dist = distance(u, v)
                    
                    self.distances[u_id] = ([v_id], [dist])

                # for now, any path is bidirectional
                funAdd(u_id, v_id, dist)
                funAdd(v_id, u_id, dist)

    def printGraph(self):
        graph = [""]

        def firstLine(s, n_nodes):
            s[0] += "{}\n".format(n_nodes)

        def printEdge(s, u, v, d):
            s[0] += "{} {} {}\n".format(u, v, d)

        funNNode = lambda n: firstLine(graph, n)
        funAdd = lambda u, v, d: printEdge(graph, u, v, d)

        self.parse(funNNode, funAdd)
        return graph[0]

    def real_id(self, key):
        '''Returns the real (the one in the osm file)id of a node,
        accepts None as a key.'''
        if key:
            return self.inverted_dict[key]
        else:
            return None

    def intern_id(self, key):
        ''' Return the intern id of a key.'''
        if key:
            return self.nodes[key]["id"]

    def find_road(self, key0, key1):
        ''' Returns the first road in common with key0, key1.'''
        if key0 and key1:
            r0 = self.nodes[key0]["roads"]
            r1 = self.nodes[key1]["roads"]
            road_list = list(set(r0) & set(r1))
            if len(road_list) >= 0:
                return ", ".join(road_list)
            else:
                return "Autre"
        else:
            return "Arrivée"

if __name__ == "__main__":
    if not (len(sys.argv) == 3):
        print("Opens an OSM data file, converts it and sends it to the" +
              "./dijkstra program.")
        print("Usage : ")
        print("\t{} osm_file start_address".format(sys.argv[0]))
        sys.exit()

    osm = osmMap(sys.argv[1])

    #############################
    ####    Creating graph   ####
    #############################
    print(tt(), "Creating graph…")
    graph = pyjkstra.Graph(len(osm.nodes))
    print(tt(), "Filling graph…")
    fun1 = lambda x: True
    fun2 = lambda u, v, d: graph.addEdge(u, v, d)

    osm.parse(fun1, fun2)

    ##########################################################
    ####    Looking for the closest road from endpoints   ####
    ##########################################################
    print(tt(), "Requesting address…")
    lat, lon, start = ar.coordinates(sys.argv[2])
    ar.processClosest(start, lat, lon, 10,
                      osm.intern_id, graph.addEdge)

    ntransport = 10
    print(tt(), "Getting {} nearest transports…".format(ntransport))
    transports = ar.nearestTransport(lat, lon, 1000, ntransport)
    # function that adds the edge v->u instead of u->v
    revertEdge = lambda u, v, d: graph.addEdge(v, u, d)
    reachables = []
    for node in transports:
        if (ar.processClosest(node['id'], node['lat'], node['lon'], 10,
                              osm.intern_id, revertEdge)):
            reachables.append(node)
    transports = reachables

    ############################################
    ####    Processing with the algorithm   ####
    ############################################
    print(tt(), "Finding ways…")
    start_intern = osm.intern_id(start)
    prev, dist = graph.dijkstra(start_intern)

    ###################################
    ####    Outputting the paths   ####
    ###################################

    print(tt(), "Printing ways ")
    for end in transports:
        node = osm.intern_id(end['id'])
        print("Chemin vers {}.".format(end))
        if node is None or prev[node] is None:
            print("Pas de chemin trouvé.")
            break
        
        while node != start_intern:
            c = osm.real_id(node)
            p = osm.real_id(prev[node])
            road = osm.find_road(c, p)
            print("{}\t->\t{}\t({}m)\t[{}]".format(c, p,
                                                   round(dist[node],1),
                                                   road))
            if node != start_intern:
                node = prev[node]
