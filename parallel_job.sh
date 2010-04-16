INFINIBAND="true"
MY_PARALLEL_PROGRAM="./par graph5.txt"
if 
[[ ${INFINIBAND} = "true" ]]
then
mpirun -np $NSLOTS ${MY_PARALLEL_PROGRAM}
else
mpirun --mca btl tcp,self -np $NSLOTS ${MY_PARALLEL_PROGRAM}
fi
