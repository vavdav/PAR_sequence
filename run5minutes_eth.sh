MY_PARALLEL_PROGRAM="./par new_data12-29.txt"
mpirun --mca btl tcp,self -np $NSLOTS ${MY_PARALLEL_PROGRAM}
