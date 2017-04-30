#!/bin/bash
###############################################################################
# Submits a job.
###############################################################################
# Is invoked with the following arguments
# $1 path to the output file

JOB_SCRIPT0=runjobBatch24tbb12omp2
JOB_SCRIPT1=runjobBatch24tbb12omp2
JOB_SCRIPT2=runjobBatch24tbb12omp2

exp0o=/ddn/data/rfmw74/h7X21hopperUni50k-regular-sphere-tbb12omp2
exp1o=/ddn/data/rfmw74/h7X22hopperUni50k-adaptive-sphere-tbb12omp2
exp2o=/ddn/data/rfmw74/h7X23hopperUni50k-reluctant-sphere-tbb12omp2

exp0="./dem-3d-release-tbb-omp-particle 0.1 0.001 0.1 hopperUniform50k 10 regular-grid 0.000001 never 10 true sphere 10 12"
exp1="./dem-3d-release-tbb-omp-particle 0.1 0.001 0.1 hopperUniform50k 10 adaptive-grid 0.000001 never 10 true sphere 10 12"
exp2="./dem-3d-release-tbb-omp-particle 0.1 0.001 0.1 hopperUniform50k 10 reluctant-adaptive-grid 0.000001 never 10 true sphere 10 12"

sed "s,{OUTPUT_FILE},$exp0o,g" ${JOB_SCRIPT0} > ${JOB_SCRIPT0}_tmp
sed "s,{RUN},$exp0,g" ${JOB_SCRIPT0}_tmp > ${JOB_SCRIPT0}_tmp1
sbatch ${JOB_SCRIPT0}_tmp1

rm ${JOB_SCRIPT0}_tmp
rm ${JOB_SCRIPT0}_tmp1

sed "s,{OUTPUT_FILE},$exp1o,g" ${JOB_SCRIPT1} > ${JOB_SCRIPT1}_tmp
sed "s,{RUN},$exp1,g" ${JOB_SCRIPT1}_tmp > ${JOB_SCRIPT1}_tmp1
sbatch ${JOB_SCRIPT1}_tmp1

rm ${JOB_SCRIPT1}_tmp
rm ${JOB_SCRIPT1}_tmp1

sed "s,{OUTPUT_FILE},$exp2o,g" ${JOB_SCRIPT2} > ${JOB_SCRIPT2}_tmp
sed "s,{RUN},$exp2,g" ${JOB_SCRIPT2}_tmp > ${JOB_SCRIPT2}_tmp1
sbatch ${JOB_SCRIPT2}_tmp1

rm ${JOB_SCRIPT2}_tmp
rm ${JOB_SCRIPT2}_tmp1
