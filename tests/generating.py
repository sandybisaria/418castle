# Requires the ortools package

from __future__ import print_function
from ortools.graph import pywrapgraph
import random
import sys
import time


def main():
    max_flow = pywrapgraph.SimpleMaxFlow()

    if len(sys.argv) >= 2:
        n = int(sys.argv[1])
    else:
        print('Missing args: python generating.py n')
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
        # print('n:', n, 'd:', density)
        # print('Max flow:', max_flow.OptimalFlow())
        # print('')
        # print('  Arc    Flow / Capacity')
        # for i in range(max_flow.NumArcs()):
        #     print('%1s -> %1s   %3s  / %3s' % (
        #         max_flow.Tail(i),
        #         max_flow.Head(i),
        #         max_flow.Flow(i),
        #         max_flow.Capacity(i)))
        test_file.write('{0}'.format(max_flow.OptimalFlow()))
    else:
        print('There was an issue with the max flow input.')
        test_file.write('{0}'.format(-1))

    print('Edge count:', edges)
    print('Max:', maxcap)
    test_file.close()

if __name__ == '__main__':
  main()