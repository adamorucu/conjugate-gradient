#!/usr/bin/python3
import argparse
import numpy as np

def conjgrad(A, b, x):
    r = b - np.dot(A, x)
    p = r
    rsold = np.dot(np.transpose(r), r)

    for _ in range(len(b)):
        Ap = np.dot(A, p)
        alpha = rsold / np.dot(np.transpose(p), Ap)
        x = x + alpha * p
        r = r - alpha * Ap
        rsnew = np.dot(np.transpose(r), r)
        if np.sqrt(rsnew) < 1e-8:
            break
        p = r + (rsnew/rsold)*p
        rsold = rsnew
    return x

parser = argparse.ArgumentParser()
parser.add_argument("--filename", help="File to read from", type=str, required=True)
args = parser.parse_args()
print(f"File: {args.filename}")

matrix = []
vec = []
with open(args.filename) as f:
    h, w, v = f.readline().split()
    height, width, vec_sz = int(h), int(w), int(v)
    for i in range(height):
        row = f.readline().split()
        matrix.append([int(r) for r in row])

    for i in range(vec_sz):
        vec.append([int(f.readline().strip())])

A = np.array(matrix)
b = np.array(vec)
print(f'A:\n{A}')
print(f'b:\n{b}')
x0 = np.ones(b.shape)
x = conjgrad(A, b, x0)
print(f'x:\n{x}')
