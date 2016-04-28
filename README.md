g++ -fopenmp -o tenfile tenfile.cpp
dich: ./tenfile

mpicc -o tenfile tenfile.cpp
dich mpirun -np 4 ./tenfile# parallelprogramming
