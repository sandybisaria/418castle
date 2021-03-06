# Requires the ortools package

from __future__ import print_function
import csv
import decimal
from ortools.graph import pywrapgraph
import os
import random
import statistics
import subprocess
import sys
import time

# Based on GeeksforGeeks
class GFG:
    def __init__(self,graph):
         
        # residual graph
        self.graph = graph 
        self.ppl = len(graph)
        self.jobs = len(graph[0])
 
    # A DFS based recursive function
    # that returns true if a matching 
    # for vertex u is possible
    def bpm(self, u, matchR, seen):
 
        # Try every job one by one
        for v in range(self.jobs):
 
            # If applicant u is interested 
            # in job v and v is not seen
            if self.graph[u][v] and seen[v] == False:
                 
                # Mark v as visited
                seen[v] = True
 
                '''If job 'v' is not assigned to
                   an applicant OR previously assigned 
                   applicant for job v (which is matchR[v]) 
                   has an alternate job available. 
                   Since v is marked as visited in the 
                   above line, matchR[v]  in the following
                   recursive call will not get job 'v' again'''
                if matchR[v] == -1 or self.bpm(matchR[v], 
                                               matchR, seen):
                    matchR[v] = u
                    return True
        return False
 
    # Returns maximum number of matching 
    def maxBPM(self):
        '''An array to keep track of the 
           applicants assigned to jobs. 
           The value of matchR[i] is the 
           applicant number assigned to job i, 
           the value -1 indicates nobody is assigned.'''
        matchR = [-1] * self.jobs
         
        # Count of jobs assigned to applicants
        result = 0
        for i in range(self.ppl):
             
            # Mark all jobs as not seen for next applicant.
            seen = [False] * self.jobs
             
            # Find if the applicant 'u' can get a job
            if self.bpm(i, matchR, seen):
                result += 1
        return result

def generate_test_case(n):
    a = 0
    b = random.randrange(1, n)

    test_filename = 'btest_n{0}_b{1}_r{2}.txt'.format(n, b, random.randint(100, 999))
    test_file = open(test_filename, 'w')

    test_file.write('{0} {1}\n'.format(n, b))

    edmonds_graph = [[0 for j in range(b, n)] for i in range(b)] # Used for BPM calculation
    edges = 0

    for i in range(b):
        density = 1 - random.random() # Per-vertex density
        for j in range(b, n):
            if random.random() < density:
                test_file.write('{0} {1} {2}\n'.format(i, j, 1))
                edmonds_graph[i][j-b] = 1
                edges += 1

    maxbpm = GFG(edmonds_graph).maxBPM()
    test_file.write('{0}'.format(maxbpm))

    # print('Maximum BPM:', maxbpm)
    test_file.close()

    return test_filename, edges

def main():
    with open('dump-bmp-seq.csv', 'w', newline='') as csvfile:
        csvwriter = csv.writer(csvfile, delimiter=' ', quotechar='|', quoting=csv.QUOTE_MINIMAL)
        for n in range(10, 1001):
            for it in range(5):
                test_filename, e = generate_test_case(n)
                command = ['./bmp-seq', '-g', test_filename]

                samples = []
                for file_it in range(1): # Median of samples
	                output = subprocess.check_output(command).decode()
	                output_lines = output.split('\n')
	                assert(output_lines[1] == 'Passed')
	                samples.append(decimal.Decimal(output_lines[2]))

                mid = statistics.median(samples)
                print('n={0} e={1} t={2}'.format(n, e, mid))
                csvwriter.writerow([n, e, mid])

                os.remove(test_filename)

if __name__ == '__main__':
	main()