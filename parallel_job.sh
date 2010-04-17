<<<<<<< HEAD
#!/bin/sh

#  ===========================================================================
# |                                                                           |
# |             COMMAND FILE FOR SUBMITTING SGE JOBS                          |
# |                                                                           |
# |                                                                           |
# | SGE keyword statements begin with #$                                      |
# |                                                                           |
# | Comments begin with #                                                     |
# | Any line whose first non-blank character is a pound sign (#)              |
# | and is not a SGE keyword statement is regarded as a comment.              |
#  ===========================================================================

#  ===========================================================================
# |                                                                           |
# | Request Bourne shell as shell for job                                     |
# |                                                                           |
#  ===========================================================================
#$ -S /bin/sh

#  ===========================================================================
# |                                                                           |
# | Execute the job from the current working directory.                       |
# |                                                                           |
#  ===========================================================================
#$ -cwd

#  ===========================================================================
# |                                                                           |
# | Defines or redefines the path used for the standard error stream          |
# | of the job.                                                               |
# |                                                                           |
#  ===========================================================================
#$ -e .

#  ===========================================================================
# |                                                                           |
# | The path used for the standard output stream of the job.                  |
# |                                                                           |
#  ===========================================================================
#$ -o .

#  ===========================================================================
# |                                                                           |
# | Specifies that all environment variables active within the qsub utility   |
# | be exported to the context of the job.                                    |
# |                                                                           |
#  ===========================================================================
#$ -V

#  ===========================================================================
# |                                                                           |
# | Set network communications - over Ethernet or InfiniBand.                 |
# |   false - Network communications over Ethernet                            |
# |   true  - Network communications over Infniband                           |
# |                                                                           |
#  ===========================================================================
INFINIBAND="true"

#  ===========================================================================
# |                                                                           |
# | Parallel program with arguments.                                          |
# |                                                                           |
#  ===========================================================================
MY_PARALLEL_PROGRAM="./par graph5.txt"



#  ===========================================================================
# |                                                                           |
# | Export environment variable to execution nodes                            |
# |                                                                           |
#  ===========================================================================
# export MY_VARIABLE1="..."
# export MY_VARIABLE2="..."





# %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

#  ===========================================================================
# |                                                                           |
# | !!! Do not change !!!                                                     |
# | mpirun -np $NSLOTS                                                        |
# | !!! Do not change !!!                                                     |
# |                                                                           |
#  ===========================================================================

if [[ ${INFINIBAND} = "true" ]]
then
  #  -------------------------------------------------------------------------
  # | Set network communication openMPI between nodes - InfiniBand (openib)   |
  #  -------------------------------------------------------------------------
  mpirun -np $NSLOTS ${MY_PARALLEL_PROGRAM}
else
  #  -------------------------------------------------------------------------
  # | Set network communication openMPI between nodes - Ethernet (tcp)        |
  #  -------------------------------------------------------------------------
  mpirun --mca btl tcp,self -np $NSLOTS ${MY_PARALLEL_PROGRAM}
fi
=======
INFINIBAND="true"
MY_PARALLEL_PROGRAM="./par graph5.txt"
if 
[[ ${INFINIBAND} = "true" ]]
then
mpirun -np $NSLOTS ${MY_PARALLEL_PROGRAM}
else
mpirun --mca btl tcp,self -np $NSLOTS ${MY_PARALLEL_PROGRAM}
fi
>>>>>>> 67429a0c72421566388ab553fe9c2fb85a3aa4ef
