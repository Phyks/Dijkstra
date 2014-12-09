#!/usr/bin/env python3
"""This program converts osm maps to an entry point for the dijktra program."""
from xml.etree import ElementTree as et
from math import cos, sin, atan2, pi, sqrt
import sys
from subprocess import PIPE, Popen


class osmMap:
    nodes = {}
    roads = []

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

    def printGraph(self):
        # create the graph in the format
        self.graph = ""
        edges = ""
        n_nodes = len(self.nodes)
        n_edges = 0
        R = 6371000

        # iterate over all ways
        for road in self.roads:
            # get all nodes in the road
            node_list = road.findall("nd")

            # calculate the distance between each two-points
            for u_ref, v_ref in zip(node_list[:-1], node_list[1:]):
                u_id = u_ref.get("ref")
                v_id = v_ref.get("ref")

                u, u_id = self.nodes[u_id]
                v, v_id = self.nodes[v_id]

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

                # for now, any path is bidirectional
                edges += "{} {} {}\n".format(u_id, v_id, dist)
                edges += "{} {} {}\n".format(v_id, u_id, dist)

                n_edges += 2

            self.graph += "{} {}\n".format(n_nodes, n_edges)
            self.graph += edges
            
        return self.graph

        
def degToRad(deg):
    return deg/360*2*pi

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
    print("Calling dot to process it…")
    call = Popen(["./dijkstra"], stdin=PIPE, stdout=PIPE)
    (out, err) = call.communicate(input=bytes(graph, 'UTF-8'))
