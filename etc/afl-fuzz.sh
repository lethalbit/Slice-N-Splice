#!/bin/bash
AFL_DIR=$MESON_BUILD_ROOT/afl
AFL_SYNC_DIR=$AFL_DIR/afl_sync
AFL_TESTCASE_DIR=$AFL_DIR/afl_test

if [ ! -d $AFL_DIR  ]; then
		mkdir -p $AFL_DIR $AFL_SYNC_DIR $AFL_TESTCASE_DIR
fi
echo "Copying initial test case"
cp $MESON_BUILD_ROOT/test-objs/* $AFL_TESTCASE_DIR/

CPU_COUNT=$(cat /proc/cpuinfo | awk '/^processor/{print $3}' | tail -1)
echo "Found $CPU_COUNT CPU Cores"

echo "Tweaking System, I'mma need root for that"
sudo -v

echo "Setting core dump pattern"
echo core | sudo tee /proc/sys/kernel/core_pattern

for i in $(seq 0 $CPU_COUNT); do
	echo "Setting scaling_governor to \"performance\" for CPU $i"
	TGT_CPU="cpu$i"
	echo performance | sudo tee /sys/devices/system/cpu/$TGT_CPU/cpufreq/scaling_governor
done

echo "Dropping root, peace~"
sudo -k

for i in $(seq 1 $(($CPU_COUNT-1))); do
	SLAVE_NAME="fuzz-slave-$i"
	echo "Spawning AFL slave $SLAVE_NAME"
	afl-fuzz -i $AFL_TESTCASE_DIR -o $AFL_SYNC_DIR -S $SLAVE_NAME $1 @@ > /dev/null 2>&1 &
	SLAVE_PID=$!
	echo "Setting $SLAVE_NAME (PID: $SLAVE_PID) affinity to CPU $i"
	taskset -cp $i $SLAVE_PID
done

echo "Starting AFL Master"
afl-fuzz -i $AFL_TESTCASE_DIR -o $AFL_SYNC_DIR -M fuzz-master $1 @@

echo "Fuzzers now running...."
