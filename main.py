#!/usr/bin/env python3
"""This program converts osm maps to an entry point for the dijktra program."""
from xml.etree import ElementTree as et
from math import cos, sin, atan2, pi, sqrt
import sys
from subprocess import PIPE, Popen
import pyjkstra



def degToRad(deg):
    return deg/360*2*pi

class osmMap:
    nodes = dict()
    roads = []
    # stores the distances between two nodes u<v as two lists:
    # distances[u] = ([v0, v1, …], [d0, d1, …])
    distances = dict()         

    def __init__(self, osm_file):
        try:
            self.tree = et.parse(osm_file)
            # get the roads
            self.updateRoads()
        except:
            print("Impossible to open {}.".format(osm_file))
            sys.exit()

    def updateRoads(self):
        root = self.tree.getroot()
        # get all the points
        child_id = 0
        for child in root.findall("node"):
            # fill the node dictionary with the nodes
            self.nodes[child.get("id")] = (child, child_id)
            child_id += 1

        # get all the roads (way with child that has the attribute k = hway)
        for way in root.findall('.//way/*[@k="highway"]/..'):
            self.roads.append(way)
            
    def distance(self, u, v):
        R = 6371000
        lat1 = degToRad(float(u.get("lat"))) / 2
        lat2 = degToRad(float(v.get("lat"))) / 2
        lon1 = degToRad(float(u.get("lon"))) / 2
        lon2 = degToRad(float(v.get("lon"))) / 2

        dlat = lat2-lat1
        dlon = lon2-lon1
        a = (pow(sin(dlat)/2, 2)
             + cos(lat1)*cos(lat2)*pow(sin(dlon)/2, 2))
        c = 2*atan2(sqrt(a), sqrt(1-a))
        dist = R*c
        return dist
    
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
                u_id = min(u_ref.get("ref"), v_ref.get("ref"))
                v_id = max(u_ref.get("ref"), v_ref.get("ref"))

                try:
                    index = self.distances[u_id][0].index(v_id)
                    dist = self.distances[u_id][1][index]
                except KeyError:  # self.distances[u_id] doesn't exist
                    u, u_id = self.nodes[u_id]
                    v, v_id = self.nodes[v_id]

                    dist = self.distance(u, v)
                    
                    self.distances[u_id] = ([v_id], [dist])
                    
                except ValueError:  # v_id not encountered
                    u, u_id = self.nodes[u_id]
                    v, v_id = self.nodes[v_id]

                    dist = self.distance(u, v)
                    
                    self.distances[u_id][0].append(v_id)
                    self.distances[u_id][1].append(dist)

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

if __name__ == "__main__":
    if not (2 <= len(sys.argv) <= 3):
        print("Opens an OSM data file, converts it and sends it to the" +
              "./dijkstra program.")
        print("Usage : ")
        print("\t{} osm_file [out_file]".format(sys.argv[0]))
        sys.exit()

    osm = osmMap(sys.argv[1])
    graph = osm.printGraph()

    if (len(sys.argv) == 3):
        with open(sys.argv[2], "w") as f:
            f.write(graph)
    else:
        print(graph)

    # Calling dijkstra
    # print("Calling dot to process it…")
    # call = Popen(["./dijkstra"], stdin=PIPE, stdout=PIPE)
    # (out, err) = call.communicate(input=bytes(graph, 'UTF-8'))
