#/usr/bin/env python3
import sys
from subprocess import PIPE, Popen

PROG = "./dijkstra"
OUT = "raw_out"
PNG = "out.png"

if __name__ == "__main__":
    argv = sys.argv
    if (len(argv) != 2):
        print("Usage:")
        print("\t{} graph".format(argv[0]))
        sys.exit()

    print("Calling \"{} {}\"…".format(PROG, argv[1]))
    call = Popen([PROG, argv[1]], stdin=PIPE, stdout=PIPE)
    (out, err) = call.communicate()

    print("Saving it to {}…".format(OUT))
    with open(OUT, "bw") as file:
        file.write(out)


    print("Generating the dot file…")
    string = "digraph "+argv[1].split("/")[1]+" {\n"
    print("->Parsing the output of {}…".format(PROG))
    routes=[]
    for line in out.decode().split("\n"):
        tmp = line.split("\t")
        if len(tmp) == 3:       # do not read last line
            u = tmp[0]
            dist = tmp[1]
            prev = tmp[2]
            string += "\t{} [label=\"{}({})\"];\n".format(u, u, dist)
            if prev != "-":
                string += "\t{} -> {} [label={},".format(prev,u,dist) + \
                          "color=red,fontcolor=red];\n"
                routes.append((prev,u))
                
    print("->Adding missing routes…")
    with open(argv[1], "r") as graph_file:
        n,e = graph_file.readline().split()
        for line in graph_file.readlines():
            u,v,d = line.split()
            if((u,v) not in routes):
                string += "\t{}->{} [label=\"{}\"];\n".format(u, v, d)
            
        
    string += "}"
    
    print("Resulting .dot command:")
    print(string)

    print("Calling dot to process it…")
    call = Popen(["dot", "-Tpng"], stdin=PIPE, stdout=PIPE)
    (out, err) = call.communicate(input=bytes(string, 'UTF-8'))
    
    print("Saving to {}…".format(PNG))
    with open(PNG, "bw") as file:
        file.write(out)
    
