################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../thread/CMutexLock.cpp 

OBJS += \
./thread/CMutexLock.o 

CPP_DEPS += \
./thread/CMutexLock.d 


# Each subdirectory must supply rules for building sources it contributes
thread/%.o: ../thread/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


