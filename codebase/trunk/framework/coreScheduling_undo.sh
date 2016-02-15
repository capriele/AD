# move tasks back from sys-cpuset to root cpuset
for T in `cat /cpuset/sys/tasks`; do echo "Moving " $T; /bin/echo $T > /cpuset/tasks; done
# remove sys-cpuset
rmdir /cpuset/sys
# move tasks back from rt-cpuset to root cpuset
for T in `cat /cpuset/rt/tasks`; do echo "Moving " $T; /bin/echo $T > /cpuset/tasks; done
# remove rt-cpuset
rmdir /cpuset/rt
# unmount and remove /cpuset
umount /cpuset
rmdir /cpuset
