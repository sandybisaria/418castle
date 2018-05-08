# Requires the ortools package

from __future__ import print_function
import csv
from ortools.graph import pywrapgraph
import os
import random
import subprocess
import sys
import time

def generate_test_case(n):
    max_flow = pywrapgraph.SimpleMaxFlow()
    pair = random.sample(range(n), 2)
    source = pair[0]
    sink = pair[1]

    # Parameters can be hardcoded if required
    density = 1 - random.random()
    maxcap = random.randint(n, n**2)
    mincap = int(maxcap * (1 - 1/density))

    edges = 0

    test_filename = 'test_n{0}_mc{1}.txt'.format(n, maxcap)
    test_file = open(test_filename, 'w')

    test_file.write('{0}\n'.format(n))
    test_file.write('{0} {1}\n'.format(source, sink))

    for i in range(n):
        if i == sink: continue
        for j in range(n):
            if j == source or j == i: continue
            capacity = random.randint(mincap, maxcap)
            if capacity > 0:
                max_flow.AddArcWithCapacity(i, j, capacity)
                test_file.write('{0} {1} {2}\n'.format(i, j, capacity))
                edges += 1

    if max_flow.Solve(source, sink) == max_flow.OPTIMAL:
        test_file.write('{0}'.format(max_flow.OptimalFlow()))
    else:
        print('There was an issue with the max flow input.')
        test_file.write('{0}'.format(-1))

    test_file.close()

    return test_filename, edges

def main():
    with open('dump-cuda_03.csv', 'w', newline='') as csvfile:
        csvwriter = csv.writer(csvfile, delimiter=' ', quotechar='|', quoting=csv.QUOTE_MINIMAL)
        for n in range(938, 1001):
            for it in range(2):
                test_filename, e = generate_test_case(n)
                command = ['./flow-cuda', '-g', test_filename]

                output = subprocess.check_output(command).decode()
                output_lines = output.split('\n')
                if output_lines[1] == 'Passed':
                    csvwriter.writerow([n, e, output_lines[2]])
                    print('n={0} e={1} t={2}'.format(n, e, output_lines[2]))

                os.remove(test_filename)


if __name__ == '__main__':
    main()