# OpenMP ARM Evaluation & Power analysis

This repository contains the following structure of files:
```bash
├── arduino_meas_power - Arduino code responsible for reading RPI2 power through the sensor
│   └── meas_power_rpi
├── cifar_original - CNN using the CMSIS lib from ARM
│   ├── CMSIS_5 
│   ├── ml-cnn_cifar
│   └── scripts
└── generic_alg - Machine learning algorithms ANN/RF/SVM
    └── src
```

## Arduino power measurement 
The `arduino_meas_power` contains the firmware responsible for measuring the power of the RPI2 through a I2C sensor connected to the power supply of the target. The meas_power_rpi.ino was coded using the Arduino latest IDE (2022) and the [Adafruit library](https://learn.adafruit.com/adafruit-ina219-current-sensor-breakout/downloads) for the INA219 sensor.

## Generic Machine learning algorithms
In the folder `generic_alg` it is added three different **machine learning algorithms**, the SVM, RandomForest and the NeuralNet. All of this algs. can be easily compiled in the Raspberry Pi 2 target with the following command from the root repository folder:
```bash
make -C generic_alg all CC=arm-linux-gnueabi-gcc
```
If you are in the linux machine and needs to use the docker to compile and run [x86], this can be done through the following command:
```bash
make -C generic_alg all _CC=gcc
```
Which will use the GCC as the compiler targeting the run/execution through the docker environment. Firstly, it'll download the docker image and then build the three algorithms. To run the binaries, the makefile target is:
```bash
make -C generic_alg run _CC=gcc
```
To change the algorithm that is executed, the line 23 of the makefile (generic_alg/makefile) needs to be changed by pointing the different executable as exemplified with `output/alg_name`. Once the run target is called through the makefile, the following output must be observed:
```bash
...
[2] === [2]
[2] === [2]
[2] === [2]
[2] === [2]
[2] === [2]
[2] === [2]
[2] === [2]
[Serial] Total execution time: 0.149 ms
[Parallel] Total execution time: 2.409 ms
[Score] 100.00% ---> 0.00% of parallel results are wrong!
```
### Tweak of parameters

The number of threads defined in `generic_alg/makefile` line 10, can be changed from 1 to any number if the user wants to check the impact through different threads.
```bash
_CC			=		arm-linux-gnueabi-gcc
CC			=		docker run --rm -v "$(abspath .)":/src -w /src aignacio/arm_gcc $(_CC)
THREADS	=		10  <----------------------------------------------------------------
CFLAGS	:=	-Wall -g -lm -fopenmp -I. -DTHREADS=$(THREADS)
ODIR		:=	output
_OBJ		+=	$(patsubst src/%,%,$(basename $(wildcard src/*.c)))
OBJ			:=	$(patsubst %,$(
```

## CNN - CIFAR 10 / ARM CMSIS lib

The CNN -  CIFAR 10 can also be executed into a docker environment or straight in the final target (Raspberry Pi 2), the commands for each are showned down below:
```bash
#For docker gcc exec [x86]
make -C cifar_original/ml-cnn_cifar all _CC_ARM=gcc USE_DOCKER=1
#Compiling for the RPI2 - ARM arch
make -C cifar_original/ml-cnn_cifar all
```
 If compiled for x86, to execute the command is:
```bash
cd cifar_original/ml-cnn_cifar
make run _CC_ARM=gcc USE_DOCKER=1
```

### Tweaking app parameters

As this code was used to benchmark the performance vs power while using OpenMP, the following parameters are available in the `cifar_original/ml-cnn_cifar/Makefile` and can be changed by the user for profiling the app. The app.c (cifar_original/ml-cnn_cifar/app.c) was modified as well to turn-on the GPIO26 of the Raspberry Pi 2 to trigger an Arduino IRQ that starts the power measuring. 

```bash
THREADS						?= 4   # Num of threads dispatched by OMP
NUM_RUNS					?= 10  # Num of runs to compute average of proc
ENABLE_SERIAL_RUN ?= 1 
ENABLE_PARAL_RUN	?= 1
ENABLE_PROFIL_RUN	?= 0
```
