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
from os.path import isfile
try:
    import cpickle as pic
except:
    import pickle as pic


def tt():
    return round(time.process_time(), 2)


def pickle(pickleFile, values):
    with open(pickleFile, "bw") as output:
        pic.dump(values, output)


def unpickle(pickleFile):
    if not isfile(pickleFile):
        return None
    with open(pickleFile, "br") as input:
        return pic.load(input)


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
        try:
            if key:
                return self.nodes[key]["id"]
            else:
                return None
        except:
            return None

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
    if not (len(sys.argv) >= 3):
        print("Opens an OSM data file, converts it and sends it to the" +
              "./dijkstra program.")
        print("Usage : ")
        print("\t{} osm_file start_address [dump file]".format(sys.argv[0]))
        sys.exit()

    mapPath = sys.argv[1]
    address = sys.argv[2]
    if len(sys.argv) >= 4:
        pickleFile = sys.argv[3]
    else:
        pickleFile = ".pickle"

    # create an osmMap object
    osm = osmMap(sys.argv[1])
    graph = pyjkstra.Graph(len(osm.nodes))

    # Fill it with data from the map
    print(tt(), "Filling graph…")
    fun1 = lambda x: True
    fun2 = lambda u, v, d: graph.addEdge(u, v, d)
    osm.parse(fun1, fun2)

    # Check for the existence of a .pickle file and if yes, compare the
    # address in it
    data = unpickle(pickleFile)
    # if the address was already got
    args = []
    reachables = {}
    if data and data['address'] == address:
        print(tt(), "Loading from file…")
        lat, lon, start = data['start']
        reachables = data['reachables']
        args = data['args']

    else:
        print(tt(), "Requesting address and nearest transports…")
        lat, lon, start = ar.coordinates(address)
        transports = ar.nearestTransport(lat, lon, 500, -1)
        nodes = ar.nearestRoad(lat, lon, radius=10, number_to_get=2)
        args += [{'from': start,
                  'to': id,
                  'd': dist} for id, dist in nodes]


        # find the nearest nodes
        reachables = []
        for node in transports:
            road = ar.nearestRoad(node['lat'], node['lon'],
                                  radius=10, number_to_get=2)
            # store them from road to node (endpoint)
            args += [{'from': id,
                      'to': node['id'],
                      'dist': d} for id, d in road]
            for id, d in road:
                if id in osm.nodes:
                    reachables.append(node)
                    break

        print(tt(), "Saving it to {}…".format(pickleFile))
        data = {'address': address,
                'start': (lat, lon, start),
                'reachables': reachables,
                'args': args}
        pickle(pickleFile, data)

    # link the start point to the nearest road
    ar.processClosest(args, osm.intern_id, graph.addEdge)

    print("Départ:\t{}".format(start))
    for station in reachables:
        print("Station:\t{}".format(station['id']))
    # Calculate all paths
    print(tt(), "Finding ways…")
    start_intern = osm.intern_id(start)
    prev, dist = graph.dijkstra(start_intern)

    # Printing out
    print(tt(), "Printing ways ")
    for end in reachables:
        node = osm.intern_id(end['id'])

        print("Chemin vers {}.".format(end))
        if node is None or prev[node] is None:
            print("Pas de chemin trouvé.")
            continue

        while node != start_intern:
            c = osm.real_id(node)
            p = osm.real_id(prev[node])
            if p == start:
                road = osm.find_road(c, c)
            elif c == end['id']:
                road = osm.find_road(p, p)
            else:
                road = osm.find_road(c, p)

            print("{}\t->\t{}\t({}m)\t[{}]".format(c, p,
                                                   round(dist[node], 1),
                                                   road))
            if node != start_intern:
                node = prev[node]
        print()
