#!/usr/bin/python3
import argparse
import numpy as np
from sklearn.datasets import make_spd_matrix

parser = argparse.ArgumentParser()
parser.add_argument("--dim", default=None, help="Number of dimentions", type=int, required=False)
parser.add_argument("--from", default=None, help="Number of dimentions", type=int, required=False)
parser.add_argument("--to", default=None, help="Number of dimentions", type=int, required=False)
args = parser.parse_args()

if args.dim != None:
    matrix = make_spd_matrix(args.dim)
    print(matrix)
    string = ""
    for i in range(args.dim):
        for j in range(args.dim):
            string += str(matrix[i, j]) + "\n"
    b = np.random.randn(args.dim)
    for i in range(args.dim):
        string += str(b[i]) + "\n"

    print(string)
    with open(f"data/{args.dim}dim.txt", "w") as f:
        f.write(string)
