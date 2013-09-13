#!/bin/sh

if [ $# -ne 2 ] && [ $# -ne 1 ]; then
    echo "Needs 1 or 2 arguments.";
    echo "ARG1: Time for sim.";
    echo "ARG2: (Optional) Job name. If left unused, name is created from current time.";
else
    if [ $# -ne 2 ]; then
        JOB_NAME=job_`date +%Y%d%m%H%M%S`
    else
        JOB_NAME=$2
    fi;
    echo $JOB_NAME
    LD_PRELOAD=./gprof-helper.so
    mkdir -p jobs/$JOB_NAME
    make clean; make -j4 && octave gen_particles.m && time ./mpm_2d -o jobs/$JOB_NAME generated_grid.txt generated_particles.txt $1;
    tar --exclude=jobs --exclude-vcs -cvzf jobs/$JOB_NAME.tar.gz ../$(basename `pwd`)
fi
