#!/bin/bash
###############################################################################
# Submits a job.
###############################################################################
# Is invoked with the following arguments
# $1 path to the output file

JOB_SCRIPT=runjobBatchOMP3

exp5o=/scratch/rfmw74/reluctantG2sphere-bfsimdTRIANGLEOMPTBB-3THREADS

exp5="./dem-3d-release-tbb-omp-triangle 0.1 0.05 0.01 random-velocities 500 reluctant-grid 0.001 never 9.81 sphere-bf 3"

sed "s,{OUTPUT_FILE},$exp5o,g" ${JOB_SCRIPT} > ${JOB_SCRIPT}_tmp
sed "s,{RUN},$exp5,g" ${JOB_SCRIPT}_tmp > ${JOB_SCRIPT}_tmp1
sbatch ${JOB_SCRIPT}_tmp1

rm ${JOB_SCRIPT}_tmp
rm ${JOB_SCRIPT}_tmp1










