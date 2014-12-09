#!/usr/bin/env python3
"""This program converts osm maps to an entry point for the dijktra program."""
from xml.etree import ElementTree as et
from math import cos, sin, atan2, pi, sqrt
import sys

def degToRad(deg):
    return deg/360*2*pi

if __name__ == "__main__":
    if not (2 <= len(sys.argv) <= 3):
        print("Usage : ")
        print("\t{} osm_file [out_file] \n".format(sys.argv[0]))
        sys.exit()

    osm_file = sys.argv[1]
    try:
        tree = et.parse(osm_file)
    except:
        print("Impossible to open %s.", osm_file)
        sys.exit()
        
    root = tree.getroot()
    nodes = {}
    highway = []
    relation = []
    
    # get all the points
    child_id = 0
    for child in root.findall("node"):
        # fill the node dictionary with the nodes
        nodes[child.get("id")] = (child, child_id)
        child_id += 1

    # get all the roads (way with child that has the attribute k = hway)
    for way in root.findall('.//way/*[@k="highway"]/..'):
        highway.append(way)
        road = way.find('./*[@k="name"]')
        
    # create the graph in the format
    graph = ""
    edges = ""
    n_nodes = len(nodes)
    n_edges = 0

    # iterate over all ways
    for way in highway:
        # get all nodes in the way
        node_list = way.findall("nd")

        # calculate the distance between each two-points
        for u_ref, v_ref in zip(node_list[:-1], node_list[1:]):
            u_id = u_ref.get("ref")
            v_id = v_ref.get("ref")

            u,u_id = nodes[u_id]
            v,v_id = nodes[v_id]

            lat1 = degToRad(float(u.get("lat"))) / 2
            lat2 = degToRad(float(v.get("lat"))) / 2
            lon1 = degToRad(float(u.get("lon"))) / 2
            lon2 = degToRad(float(v.get("lon"))) / 2
            lon = (lon1+lon2) / 2

            dlat = lat2-lat1
            dlon = lon2-lon1
            a = pow(sin(dlat)/2, 2) + cos(lat1)*cos(lat2)*pow(sin(dlon)/2, 2)
            c = 2*atan2(sqrt(a), sqrt(1-a))
            R = 6371000
            dist = R*c

            # for now, any path is bidirectional
            edges += "{} {} {}\n".format(u_id, v_id, dist)
            edges += "{} {} {}\n".format(v_id, u_id, dist)

            n_edges += 2

    graph += "{} {}\n".format(n_nodes, n_edges)
    graph += edges

    if (len(sys.argv) == 3):
        with open(sys.argv[2], "w") as f:
            f.write(graph)
    else:
        print(graph)
