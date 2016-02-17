# http://stackoverflow.com/questions/9072060/one-core-exclusively-for-my-process


# move all processes from the default cpuset to the sys-cpuset
cd /cpuset
for T in `cat tasks`; do echo "Moving " $T; /bin/echo $T > sys/tasks; done
cd -
#move all processes on dedicated list to dedicated core

while read p; do
  /bin/echo $(pidof $p) > /cpuset/rt/tasks
done <coreScheduling_DedicatedList.txt
