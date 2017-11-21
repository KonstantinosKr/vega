#!/bin/bash
###############################################################################
# Submits a job.
###############################################################################
# Is invoked with the following arguments
# $1 path to the output file

JOB_SCRIPT0=runjobBatch1
JOB_SCRIPT1=runjobBatch2
JOB_SCRIPT2=runjobBatch3
JOB_SCRIPT3=runjobBatch4
JOB_SCRIPT4=runjobBatch6
JOB_SCRIPT5=runjobBatch8
JOB_SCRIPT6=runjobBatch16

exp0o=/scratch/rfmw74/reluctanthybrid-on-batchesOMPTRIANGLE1_2000
exp1o=/scratch/rfmw74/reluctanthybrid-on-batchesOMPTRIANGLE2_2000
exp2o=/scratch/rfmw74/reluctanthybrid-on-batchesOMPTRIANGLE3_2000
exp3o=/scratch/rfmw74/reluctanthybrid-on-batchesOMPTRIANGLE4_2000
exp4o=/scratch/rfmw74/reluctanthybrid-on-batchesOMPTRIANGLE6_2000
exp5o=/scratch/rfmw74/reluctanthybrid-on-batchesOMPTRIANGLE8_2000
exp6o=/scratch/rfmw74/reluctanthybrid-on-batchesOMPTRIANGLE16_2000

exp0="./dem-3d-release-omp-triangle 0.1 freefallWithGranulates 5000 reluctant-grid 0.0001 never 10 true hybrid-on-batches 2000 1"
exp1="./dem-3d-release-omp-triangle 0.1 freefallWithGranulates 5000 reluctant-grid 0.0001 never 10 true hybrid-on-batches 2000 2"
exp2="./dem-3d-release-omp-triangle 0.1 freefallWithGranulates 5000 reluctant-grid 0.0001 never 10 true hybrid-on-batches 2000 3"
exp3="./dem-3d-release-omp-triangle 0.1 freefallWithGranulates 5000 reluctant-grid 0.0001 never 10 true hybrid-on-batches 2000 4"
exp4="./dem-3d-release-omp-triangle 0.1 freefallWithGranulates 5000 reluctant-grid 0.0001 never 10 true hybrid-on-batches 2000 6"
exp5="./dem-3d-release-omp-triangle 0.1 freefallWithGranulates 5000 reluctant-grid 0.0001 never 10 true hybrid-on-batches 2000 8"
exp6="./dem-3d-release-omp-triangle 0.1 freefallWithGranulates 5000 reluctant-grid 0.0001 never 10 true hybrid-on-batches 2000 16"

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

sed "s,{OUTPUT_FILE},$exp3o,g" ${JOB_SCRIPT3} > ${JOB_SCRIPT3}_tmp
sed "s,{RUN},$exp3,g" ${JOB_SCRIPT3}_tmp > ${JOB_SCRIPT3}_tmp1
sbatch ${JOB_SCRIPT3}_tmp1

rm ${JOB_SCRIPT3}_tmp
rm ${JOB_SCRIPT3}_tmp1

sed "s,{OUTPUT_FILE},$exp4o,g" ${JOB_SCRIPT4} > ${JOB_SCRIPT4}_tmp
sed "s,{RUN},$exp4,g" ${JOB_SCRIPT4}_tmp > ${JOB_SCRIPT4}_tmp1
sbatch ${JOB_SCRIPT4}_tmp1

rm ${JOB_SCRIPT4}_tmp
rm ${JOB_SCRIPT4}_tmp1

sed "s,{OUTPUT_FILE},$exp5o,g" ${JOB_SCRIPT5} > ${JOB_SCRIPT5}_tmp
sed "s,{RUN},$exp5,g" ${JOB_SCRIPT5}_tmp > ${JOB_SCRIPT5}_tmp1
sbatch ${JOB_SCRIPT5}_tmp1

rm ${JOB_SCRIPT5}_tmp
rm ${JOB_SCRIPT5}_tmp1

sed "s,{OUTPUT_FILE},$exp6o,g" ${JOB_SCRIPT6} > ${JOB_SCRIPT6}_tmp
sed "s,{RUN},$exp6,g" ${JOB_SCRIPT6}_tmp > ${JOB_SCRIPT6}_tmp1
sbatch ${JOB_SCRIPT6}_tmp1

rm ${JOB_SCRIPT6}_tmp
rm ${JOB_SCRIPT6}_tmp1
