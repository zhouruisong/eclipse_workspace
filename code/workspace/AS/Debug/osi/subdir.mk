################################################################################
# 自动生成的文件。不要编辑！
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
O_SRCS += \
../osi/Osi.o 

CPP_SRCS += \
../osi/Osi.cpp 

OBJS += \
./osi/Osi.o 

CPP_DEPS += \
./osi/Osi.d 


# Each subdirectory must supply rules for building sources it contributes
osi/%.o: ../osi/%.cpp
	@echo '正在构建文件： $<'
	@echo '正在调用： GNU C++ 编译器'
	g++ -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o"$@" "$<"
	@echo '已结束构建： $<'
	@echo ' '


