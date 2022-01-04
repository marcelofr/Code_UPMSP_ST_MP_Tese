Instances

We have two instances types:
-Set 1 consisting of instances with up to 10 jobs and up to two machines;
-Set 2 consisting of instances with up to 750 jobs and up to 20 machines.

Each instance is a text file with the following format:

-----------------------begin file---------------------

n 6
m 2
n_day 1
hl 1439
o 3
rate_in_peak 0.47753
rate_off_peak 0.32282
max_cost 649

peak_start
1080

peak_end
1259

v
1.2
1
0.8

lambda
1.5
1
0.6

pi
189
185

processing
12	20	
34	86	
65	88	
27	8	
5	92	
50	67	

setup
0	5	2	4	2	4	
4	0	4	6	3	2	
8	5	0	8	3	5	
2	4	3	0	4	4	
2	4	2	8	0	5	
8	8	7	7	4	0	

0	5	6	7	1	8	
2	0	6	6	2	7	
2	5	0	2	5	8	
5	3	5	0	3	4	
5	1	6	6	0	5	
2	5	6	5	5	0

-----------------------end file---------------------

in which:
-'n' is the number of jobs;
-'m' is the number of machines;
-'n_day' is the number of days;
-'hl' is the number of intervals in the planning horizon;
-'o' is the number of operation mode;
-'rate_in_peak' is the energy tax in peak hour ($/kwh);
-'rate_off_peak' is the energy tax off peak hour ($/kwh);
-'max_cost' is the limit value for makespan;
-'peak_start' is a set containing the peak hours start time for each day of the planning horizon;
-'peak_end' is a set containing the peak hours end time for each day of the planning horizon;
-'v' is the set containing the multiplication factors that relates the operation mode to the job execution speed;
-'lambda' is the set containing the multiplication factors that relates the operation mode to the energy consumption;
-'pi' is the set containing the consumption of each machine;
-'processing' is the set containing the processing time of each job. In which, each line represents the job j, and each column represents the machine i;
-'setup' is the set containing the setup time between all pair of jobs in each machine. In which, each line represents the current job i, and each column represents the next job k, and each block represents the machine i.
