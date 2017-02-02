#REGULAR
#sphere-bf
nohup /opt/intel/mic/bin/micnativeloadex ./dem-3d-release-vecPARTICLEphi -a "0.5 0.5 0.5 random-velocities 500 adaptive-grid 0.001 never 9.81 bf" -e "OMP_NUM_THREADS=1" > bf-adaptive-PARTICLE-G0-1THREADS &
wait
nohup /opt/intel/mic/bin/micnativeloadex ./dem-3d-release-vecPARTICLEphi -a "0.5 0.5 0.5 random-velocities 500 adaptive-grid 0.001 never 9.81 bf" -e "OMP_NUM_THREADS=2" > bf-adaptive-PARTICLE-G0-2THREADS &
wait
nohup /opt/intel/mic/bin/micnativeloadex ./dem-3d-release-vecPARTICLEphi -a "0.5 0.5 0.5 random-velocities 500 adaptive-grid 0.001 never 9.81 bf" -e "OMP_NUM_THREADS=3" > bf-adaptive-PARTICLE-G0-3THREADS &
wait
nohup /opt/intel/mic/bin/micnativeloadex ./dem-3d-release-vecPARTICLEphi -a "0.5 0.5 0.5 random-velocities 500 adaptive-grid 0.001 never 9.81 bf" -e "OMP_NUM_THREADS=4" > bf-adaptive-PARTICLE-G0-4THREADS &
wait
nohup /opt/intel/mic/bin/micnativeloadex ./dem-3d-release-vecPARTICLEphi -a "0.5 0.5 0.5 random-velocities 500 adaptive-grid 0.001 never 9.81 bf" -e "OMP_NUM_THREADS=8" > bf-adaptive-PARTICLE-G0-8THREADS &
wait
nohup /opt/intel/mic/bin/micnativeloadex ./dem-3d-release-vecPARTICLEphi -a "0.5 0.5 0.5 random-velocities 500 adaptive-grid 0.001 never 9.81 bf" -e "OMP_NUM_THREADS=12" > bf-adaptive-PARTICLE-G0-12THREADS &
wait
nohup /opt/intel/mic/bin/micnativeloadex ./dem-3d-release-vecPARTICLEphi -a "0.5 0.5 0.5 random-velocities 500 adaptive-grid 0.001 never 9.81 bf" -e "OMP_NUM_THREADS=16" > bf-adaptive-PARTICLE-G0-16THREADS &
wait
nohup /opt/intel/mic/bin/micnativeloadex ./dem-3d-release-vecPARTICLEphi -a "0.5 0.5 0.5 random-velocities 500 adaptive-grid 0.001 never 9.81 bf" -e "OMP_NUM_THREADS=18" > bf-adaptive-PARTICLE-G0-18THREADS &
wait
nohup /opt/intel/mic/bin/micnativeloadex ./dem-3d-release-vecPARTICLEphi -a "0.5 0.5 0.5 random-velocities 500 adaptive-grid 0.001 never 9.81 bf" -e "OMP_NUM_THREADS=24" > bf-adaptive-PARTICLE-G0-24THREADS &
wait
nohup /opt/intel/mic/bin/micnativeloadex ./dem-3d-release-vecPARTICLEphi -a "0.5 0.5 0.5 random-velocities 500 adaptive-grid 0.001 never 9.81 bf" -e "OMP_NUM_THREADS=60" > bf-adaptive-PARTICLE-G0-60THREADS &
wait
nohup /opt/intel/mic/bin/micnativeloadex ./dem-3d-release-vecPARTICLEphi -a "0.5 0.5 0.5 random-velocities 500 adaptive-grid 0.001 never 9.81 bf" -e "OMP_NUM_THREADS=72" > bf-adaptive-PARTICLE-G0-72THREADS &
wait
nohup /opt/intel/mic/bin/micnativeloadex ./dem-3d-release-vecPARTICLEphi -a "0.5 0.5 0.5 random-velocities 500 adaptive-grid 0.001 never 9.81 bf" -e "OMP_NUM_THREADS=120" > bf-adaptive-PARTICLE-G0-120THREADS &
wait
nohup /opt/intel/mic/bin/micnativeloadex ./dem-3d-release-vecPARTICLEphi -a "0.5 0.5 0.5 random-velocities 500 adaptive-grid 0.001 never 9.81 bf" -e "OMP_NUM_THREADS=180" > bf-adaptive-PARTICLE-G0-180THREADS &
wait
nohup /opt/intel/mic/bin/micnativeloadex ./dem-3d-release-vecPARTICLEphi -a "0.5 0.5 0.5 random-velocities 500 adaptive-grid 0.001 never 9.81 bf" -e "OMP_NUM_THREADS=240" > bf-adaptive-PARTICLE-G0-240THREADS &

#sphere-penalty
nohup /opt/intel/mic/bin/micnativeloadex ./dem-3d-release-vecPARTICLEphi -a "0.5 0.5 0.5 random-velocities 500 adaptive-grid 0.001 never 9.81 penalty" -e "OMP_NUM_THREADS=1" > penalty-adaptive-PARTICLE-G0-1THREADS &
wait
nohup /opt/intel/mic/bin/micnativeloadex ./dem-3d-release-vecPARTICLEphi -a "0.5 0.5 0.5 random-velocities 500 adaptive-grid 0.001 never 9.81 penalty" -e "OMP_NUM_THREADS=2" > penalty-adaptive-PARTICLE-G0-2THREADS &
wait
nohup /opt/intel/mic/bin/micnativeloadex ./dem-3d-release-vecPARTICLEphi -a "0.5 0.5 0.5 random-velocities 500 adaptive-grid 0.001 never 9.81 penalty" -e "OMP_NUM_THREADS=3" > penalty-adaptive-PARTICLE-G0-3THREADS &
wait
nohup /opt/intel/mic/bin/micnativeloadex ./dem-3d-release-vecPARTICLEphi -a "0.5 0.5 0.5 random-velocities 500 adaptive-grid 0.001 never 9.81 penalty" -e "OMP_NUM_THREADS=4" > penalty-adaptive-PARTICLE-G0-4THREADS &
wait
nohup /opt/intel/mic/bin/micnativeloadex ./dem-3d-release-vecPARTICLEphi -a "0.5 0.5 0.5 random-velocities 500 adaptive-grid 0.001 never 9.81 penalty" -e "OMP_NUM_THREADS=8" > penalty-adaptive-PARTICLE-G0-8THREADS &
wait
nohup /opt/intel/mic/bin/micnativeloadex ./dem-3d-release-vecPARTICLEphi -a "0.5 0.5 0.5 random-velocities 500 adaptive-grid 0.001 never 9.81 penalty" -e "OMP_NUM_THREADS=12" > penalty-adaptive-PARTICLE-G0-12THREADS &
wait
nohup /opt/intel/mic/bin/micnativeloadex ./dem-3d-release-vecPARTICLEphi -a "0.5 0.5 0.5 random-velocities 500 adaptive-grid 0.001 never 9.81 penalty" -e "OMP_NUM_THREADS=16" > penalty-adaptive-PARTICLE-G0-16THREADS &
wait
nohup /opt/intel/mic/bin/micnativeloadex ./dem-3d-release-vecPARTICLEphi -a "0.5 0.5 0.5 random-velocities 500 adaptive-grid 0.001 never 9.81 penalty" -e "OMP_NUM_THREADS=18" > penalty-adaptive-PARTICLE-G0-18THREADS &
wait
nohup /opt/intel/mic/bin/micnativeloadex ./dem-3d-release-vecPARTICLEphi -a "0.5 0.5 0.5 random-velocities 500 adaptive-grid 0.001 never 9.81 penalty" -e "OMP_NUM_THREADS=24" > penalty-adaptive-PARTICLE-G0-24THREADS &
wait
nohup /opt/intel/mic/bin/micnativeloadex ./dem-3d-release-vecPARTICLEphi -a "0.5 0.5 0.5 random-velocities 500 adaptive-grid 0.001 never 9.81 penalty" -e "OMP_NUM_THREADS=60" > penalty-adaptive-PARTICLE-G0-60THREADS &
wait
nohup /opt/intel/mic/bin/micnativeloadex ./dem-3d-release-vecPARTICLEphi -a "0.5 0.5 0.5 random-velocities 500 adaptive-grid 0.001 never 9.81 penalty" -e "OMP_NUM_THREADS=72" > penalty-adaptive-PARTICLE-G0-72THREADS &
wait
nohup /opt/intel/mic/bin/micnativeloadex ./dem-3d-release-vecPARTICLEphi -a "0.5 0.5 0.5 random-velocities 500 adaptive-grid 0.001 never 9.81 penalty" -e "OMP_NUM_THREADS=120" > penalty-adaptive-PARTICLE-G0-120THREADS &
wait
nohup /opt/intel/mic/bin/micnativeloadex ./dem-3d-release-vecPARTICLEphi -a "0.5 0.5 0.5 random-velocities 500 adaptive-grid 0.001 never 9.81 penalty" -e "OMP_NUM_THREADS=180" > penalty-adaptive-PARTICLE-G0-180THREADS &
wait
nohup /opt/intel/mic/bin/micnativeloadex ./dem-3d-release-vecPARTICLEphi -a "0.5 0.5 0.5 random-velocities 500 adaptive-grid 0.001 never 9.81 penalty" -e "OMP_NUM_THREADS=240" > penalty-adaptive-PARTICLE-G0-240THREADS &

#hybrid-on-batches
nohup /opt/intel/mic/bin/micnativeloadex ./dem-3d-release-vecPARTICLEphi -a "0.5 0.5 0.5 random-velocities 500 adaptive-grid 0.001 never 9.81 hybrid-on-batches" -e "OMP_NUM_THREADS=1" > hybrid-on-batches-adaptive-PARTICLE-G0-1THREADS &
wait
nohup /opt/intel/mic/bin/micnativeloadex ./dem-3d-release-vecPARTICLEphi -a "0.5 0.5 0.5 random-velocities 500 adaptive-grid 0.001 never 9.81 hybrid-on-batches" -e "OMP_NUM_THREADS=2" > hybrid-on-batches-adaptive-PARTICLE-G0-2THREADS &
wait
nohup /opt/intel/mic/bin/micnativeloadex ./dem-3d-release-vecPARTICLEphi -a "0.5 0.5 0.5 random-velocities 500 adaptive-grid 0.001 never 9.81 hybrid-on-batches" -e "OMP_NUM_THREADS=3" > hybrid-on-batches-adaptive-PARTICLE-G0-3THREADS &
wait
nohup /opt/intel/mic/bin/micnativeloadex ./dem-3d-release-vecPARTICLEphi -a "0.5 0.5 0.5 random-velocities 500 adaptive-grid 0.001 never 9.81 hybrid-on-batches" -e "OMP_NUM_THREADS=4" > hybrid-on-batches-adaptive-PARTICLE-G0-4THREADS &
wait
nohup /opt/intel/mic/bin/micnativeloadex ./dem-3d-release-vecPARTICLEphi -a "0.5 0.5 0.5 random-velocities 500 adaptive-grid 0.001 never 9.81 hybrid-on-batches" -e "OMP_NUM_THREADS=8" > hybrid-on-batches-adaptive-PARTICLE-G0-8THREADS &
wait
nohup /opt/intel/mic/bin/micnativeloadex ./dem-3d-release-vecPARTICLEphi -a "0.5 0.5 0.5 random-velocities 500 adaptive-grid 0.001 never 9.81 hybrid-on-batches" -e "OMP_NUM_THREADS=12" > hybrid-on-batches-adaptive-PARTICLE-G0-12THREADS &
wait
nohup /opt/intel/mic/bin/micnativeloadex ./dem-3d-release-vecPARTICLEphi -a "0.5 0.5 0.5 random-velocities 500 adaptive-grid 0.001 never 9.81 hybrid-on-batches" -e "OMP_NUM_THREADS=16" > hybrid-on-batches-adaptive-PARTICLE-G0-16THREADS &
wait
nohup /opt/intel/mic/bin/micnativeloadex ./dem-3d-release-vecPARTICLEphi -a "0.5 0.5 0.5 random-velocities 500 adaptive-grid 0.001 never 9.81 hybrid-on-batches" -e "OMP_NUM_THREADS=18" > hybrid-on-batches-adaptive-PARTICLE-G0-18THREADS &
wait
nohup /opt/intel/mic/bin/micnativeloadex ./dem-3d-release-vecPARTICLEphi -a "0.5 0.5 0.5 random-velocities 500 adaptive-grid 0.001 never 9.81 hybrid-on-batches" -e "OMP_NUM_THREADS=24" > hybrid-on-batches-adaptive-PARTICLE-G0-24THREADS &
wait
nohup /opt/intel/mic/bin/micnativeloadex ./dem-3d-release-vecPARTICLEphi -a "0.5 0.5 0.5 random-velocities 500 adaptive-grid 0.001 never 9.81 hybrid-on-batches" -e "OMP_NUM_THREADS=60" > hybrid-on-batches-adaptive-PARTICLE-G0-60THREADS &
wait
nohup /opt/intel/mic/bin/micnativeloadex ./dem-3d-release-vecPARTICLEphi -a "0.5 0.5 0.5 random-velocities 500 adaptive-grid 0.001 never 9.81 hybrid-on-batches" -e "OMP_NUM_THREADS=72" > hybrid-on-batches-adaptive-PARTICLE-G0-72THREADS &
wait
nohup /opt/intel/mic/bin/micnativeloadex ./dem-3d-release-vecPARTICLEphi -a "0.5 0.5 0.5 random-velocities 500 adaptive-grid 0.001 never 9.81 hybrid-on-batches" -e "OMP_NUM_THREADS=120" > hybrid-on-batches-adaptive-PARTICLE-G0-120THREADS &
wait
nohup /opt/intel/mic/bin/micnativeloadex ./dem-3d-release-vecPARTICLEphi -a "0.5 0.5 0.5 random-velocities 500 adaptive-grid 0.001 never 9.81 hybrid-on-batches" -e "OMP_NUM_THREADS=180" > hybrid-on-batches-adaptive-PARTICLE-G0-180THREADS &
wait
nohup /opt/intel/mic/bin/micnativeloadex ./dem-3d-release-vecPARTICLEphi -a "0.5 0.5 0.5 random-velocities 500 adaptive-grid 0.001 never 9.81 hybrid-on-batches" -e "OMP_NUM_THREADS=240" > hybrid-on-batches-adaptive-PARTICLE-G0-240THREADS &

#hybrid-on-triangles
nohup /opt/intel/mic/bin/micnativeloadex ./dem-3d-release-vecPARTICLEphi -a "0.5 0.5 0.5 random-velocities 500 adaptive-grid 0.001 never 9.81 hybrid-on-triangle-pairs" -e "OMP_NUM_THREADS=1" > hybrid-on-triangle-pairs-adaptive-PARTICLE-G0-1THREADS &
wait
nohup /opt/intel/mic/bin/micnativeloadex ./dem-3d-release-vecPARTICLEphi -a "0.5 0.5 0.5 random-velocities 500 adaptive-grid 0.001 never 9.81 hybrid-on-triangle-pairs" -e "OMP_NUM_THREADS=2" > hybrid-on-triangle-pairs-adaptive-PARTICLE-G0-2THREADS &
wait
nohup /opt/intel/mic/bin/micnativeloadex ./dem-3d-release-vecPARTICLEphi -a "0.5 0.5 0.5 random-velocities 500 adaptive-grid 0.001 never 9.81 hybrid-on-triangle-pairs" -e "OMP_NUM_THREADS=3" > hybrid-on-triangle-pairs-adaptive-PARTICLE-G0-3THREADS &
wait
nohup /opt/intel/mic/bin/micnativeloadex ./dem-3d-release-vecPARTICLEphi -a "0.5 0.5 0.5 random-velocities 500 adaptive-grid 0.001 never 9.81 hybrid-on-triangle-pairs" -e "OMP_NUM_THREADS=4" > hybrid-on-triangle-pairs-adaptive-PARTICLE-G0-4THREADS &
wait
nohup /opt/intel/mic/bin/micnativeloadex ./dem-3d-release-vecPARTICLEphi -a "0.5 0.5 0.5 random-velocities 500 adaptive-grid 0.001 never 9.81 hybrid-on-triangle-pairs" -e "OMP_NUM_THREADS=8" > hybrid-on-triangle-pairs-adaptive-PARTICLE-G0-8THREADS &
wait
nohup /opt/intel/mic/bin/micnativeloadex ./dem-3d-release-vecPARTICLEphi -a "0.5 0.5 0.5 random-velocities 500 adaptive-grid 0.001 never 9.81 hybrid-on-triangle-pairs" -e "OMP_NUM_THREADS=12" > hybrid-on-triangle-pairs-adaptive-PARTICLE-G0-12THREADS &
wait
nohup /opt/intel/mic/bin/micnativeloadex ./dem-3d-release-vecPARTICLEphi -a "0.5 0.5 0.5 random-velocities 500 adaptive-grid 0.001 never 9.81 hybrid-on-triangle-pairs" -e "OMP_NUM_THREADS=16" > hybrid-on-triangle-pairs-adaptive-PARTICLE-G0-16THREADS &
wait
nohup /opt/intel/mic/bin/micnativeloadex ./dem-3d-release-vecPARTICLEphi -a "0.5 0.5 0.5 random-velocities 500 adaptive-grid 0.001 never 9.81 hybrid-on-triangle-pairs" -e "OMP_NUM_THREADS=18" > hybrid-on-triangle-pairs-adaptive-PARTICLE-G0-18THREADS &
wait
nohup /opt/intel/mic/bin/micnativeloadex ./dem-3d-release-vecPARTICLEphi -a "0.5 0.5 0.5 random-velocities 500 adaptive-grid 0.001 never 9.81 hybrid-on-triangle-pairs" -e "OMP_NUM_THREADS=24" > hybrid-on-triangle-pairs-adaptive-PARTICLE-G0-24THREADS &
wait
nohup /opt/intel/mic/bin/micnativeloadex ./dem-3d-release-vecPARTICLEphi -a "0.5 0.5 0.5 random-velocities 500 adaptive-grid 0.001 never 9.81 hybrid-on-triangle-pairs" -e "OMP_NUM_THREADS=60" > hybrid-on-triangle-pairs-adaptive-PARTICLE-G0-60THREADS &
wait
nohup /opt/intel/mic/bin/micnativeloadex ./dem-3d-release-vecPARTICLEphi -a "0.5 0.5 0.5 random-velocities 500 adaptive-grid 0.001 never 9.81 hybrid-on-triangle-pairs" -e "OMP_NUM_THREADS=72" > hybrid-on-triangle-pairs-adaptive-PARTICLE-G0-72THREADS &
wait
nohup /opt/intel/mic/bin/micnativeloadex ./dem-3d-release-vecPARTICLEphi -a "0.5 0.5 0.5 random-velocities 500 adaptive-grid 0.001 never 9.81 hybrid-on-triangle-pairs" -e "OMP_NUM_THREADS=120" > hybrid-on-triangle-pairs-adaptive-PARTICLE-G0-120THREADS &
wait
nohup /opt/intel/mic/bin/micnativeloadex ./dem-3d-release-vecPARTICLEphi -a "0.5 0.5 0.5 random-velocities 500 adaptive-grid 0.001 never 9.81 hybrid-on-triangle-pairs" -e "OMP_NUM_THREADS=180" > hybrid-on-triangle-pairs-adaptive-PARTICLE-G0-180THREADS &
wait
nohup /opt/intel/mic/bin/micnativeloadex ./dem-3d-release-vecPARTICLEphi -a "0.5 0.5 0.5 random-velocities 500 adaptive-grid 0.001 never 9.81 hybrid-on-triangle-pairs" -e "OMP_NUM_THREADS=240" > hybrid-on-triangle-pairs-adaptive-PARTICLE-G0-240THREADS &

