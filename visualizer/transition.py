#!/bin/python3

from math import floor
from random import random
import re


if __name__ == "__main__":
    states = []
    transitions = []
    with open("results/graph.dat", "r") as f:
        # Ignore first line
        f.readline()
        s = ""
        l = ""
        for line in f:
            line = line.strip()
            if(line.startswith("Edges:")):
                if(s != ""):
                    states.append((s, l))
                break
            if(re.match(r'^I[0-9]*:', line)):
                if(s != ""):
                    states.append((s, l))
                s = line.strip(":")
                l = ""
            elif(line != ""):
                l += line + "\\n"
        for line in f:
            a, b, c = map(str.strip, line.split("->"))
            transitions.append((a+"->"+c, b))
    # print(states)
    # print(transitions)

    with open("results/graph.dot", "w") as f:
        f.write("digraph {\n")
        for s, l in states:
            f.write("\t" + s + " [label=\"" + l.strip() + "\"];\n")
        for t, l in transitions:
            r = hex(floor(random()*200))[2:].ljust(2, '0')
            g = hex(floor(random()*200))[2:].ljust(2, '0')
            b = hex(floor(random()*200))[2:].ljust(2, '0')
            col = "#" + r + g + b
            f.write("\t" + t + " [label=\"" + l.strip() + "\"," +
                    "color=\"" + col + "\"," +
                    "fontcolor=\"" + col + "\"];\n")
        f.write("}\n")
