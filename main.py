#!/usr/bin/env python3
from xml.etree import ElementTree as et
import math

if __name__=="__main__":
    tree = et.parse("maps/5eme_petit.osm")
    root = tree.getroot()
    nodes = {}
    highway = []
    relation = []
    
    # get all the points
    for child in root.findall("node"):
        # fill the node dictionary with the nodes
        nodes[child.get("id")] = child

    # get all the roads
    for way in root.findall('.//way/*[@k="highway"]/..'):
        highway.append(way)
        road = way.find('./*[@k="name"]')
        # if road != None:
        #     print(road.get("v"))
        # else:
        #     print(way.get("id"))

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
            
            u = nodes[u_id]
            v = nodes[v_id]

            dist = math.sqrt((float(u.get("lat")) - float(v.get("lat")))**2 +
                             (float(u.get("lon")) - float(v.get("lon")))**2)
            
            edges += "{}\t{}\t{}\n".format(u_id, v_id, dist)
            edges += "{}\t{}\t{}\n".format(v_id, u_id, dist)

            n_edges += 2
            
    graph += "{} {}\n".format(n_nodes, n_edges)
    graph += edges

    print(graph)
