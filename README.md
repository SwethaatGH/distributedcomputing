# Distributed Computing
Find code for
1. election algorithms (ring and bully)
2. point to point MPI communication (pi calculation, matrix multiplication)
3. broadcast MPI commmunication (pi calculation, matrix multiplication)
4. centralised deadlock detection algorithm
5. distributed deadlock detection algorithms (probe based and WFG based)
with respect to concepts described in "Pradeep K Sinha , "Distributed Operating Systems: Concepts and Design", Prentice Hall of India, New Delhi, 2009"

## Commands
How to Run MPI based programs?

Setup MPI using terminal or command prompt
(for terminal)
1. ```sudo apt update```
2. ```sudo apt install openmpi-bin```
3. ```mpicc --version```
4. Compile sample program using ```mpicc -o filename filename.c```
5. Run the compiled program using ```mpiexec -n 4 ./filename```
6. If you wish to use many number of processes ```mpiexec -n 100 --oversubscribe ./filename```
