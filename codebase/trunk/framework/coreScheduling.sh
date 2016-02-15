# http://stackoverflow.com/questions/9072060/one-core-exclusively-for-my-process

#run as sudo 

mkdir /cpuset 
mount -t cpuset none /cpuset/
cd /cpuset

mkdir sys                                   # create sub-cpuset for system processes
/bin/echo 0-2 > sys/cpus             # assign cpus (cores) 0-2 to this set
                                            # adjust if you have more/less cores
/bin/echo 1 > sys/cpu_exclusive
/bin/echo 0 > sys/mems     

mkdir rt                                    # create sub-cpuset for my process
/bin/echo 3 > rt/cpus                # assign cpu (core) 3 to this cpuset
                                            # adjust this to number of cores-1
/bin/echo 1 > rt/cpu_exclusive
/bin/echo 0 > rt/mems
/bin/echo 0 > rt/sched_load_balance
/bin/echo 1 > rt/mem_hardwall

# move all processes from the default cpuset to the sys-cpuset
for T in `cat tasks`; do echo "Moving " $T; /bin/echo $T > sys/tasks; done

#to add your processes to the rt-cpuset, do

# /bin/echo $PID > /cpuset/rt/tasks
