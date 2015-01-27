################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../src/IDDFS.cpp \
../src/gameBoard.cpp \
../src/gameNode.cpp \
../src/gameTree.cpp \
../src/globals.cpp \
../src/main.cpp \
../src/playGame.cpp \
../src/superRandomPlayer.cpp 

OBJS += \
./src/IDDFS.o \
./src/gameBoard.o \
./src/gameNode.o \
./src/gameTree.o \
./src/globals.o \
./src/main.o \
./src/playGame.o \
./src/superRandomPlayer.o 

CPP_DEPS += \
./src/IDDFS.d \
./src/gameBoard.d \
./src/gameNode.d \
./src/gameTree.d \
./src/globals.d \
./src/main.d \
./src/playGame.d \
./src/superRandomPlayer.d 


# Each subdirectory must supply rules for building sources it contributes
src/%.o: ../src/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: Cygwin C++ Compiler'
	g++ -O3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


