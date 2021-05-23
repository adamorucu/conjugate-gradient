#!/usr/bin/python3
import argparse
import numpy as np

parser = argparse.ArgumentParser()
parser.add_argument("--size", help="File containing the data", type=int, required=True)
parser.add_argument("--data", help="File containing the data", type=str, required=True)
args = parser.parse_args()

matrix = []
vec = []
vec2 = []
size = args.size
with open(args.data) as f:
    for i in range(size):
        temp = []
        for j in range(size):
            num = f.readline().strip()
            temp.append(float(num))
        matrix.append(temp)

    for i in range(size):
        vec.append(float(f.readline().strip()))

with open("result.txt") as f:
    for i in range(size):
        vec2.append(float(f.readline().strip()))

A = np.array(matrix)
b = np.array(vec)
x = np.array(vec2)
left = np.dot(A, x)

# for i in range(size):
#     print(f'b[{i}]=\t\t{b[i]}')
#     print(f'left[{i}]=\t{left[i]}')

print(max(np.abs(left-b)))
assert max(np.abs(left-b))<1e-4, "nonono"
