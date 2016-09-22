################################################################################
# 自动生成的文件。不要编辑！
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
O_SRCS += \
../core/CMathndrControllor.o \
../core/main.o 

CPP_SRCS += \
../core/CAsyncController.cpp \
../core/CEventController.cpp \
../core/CEventListener.cpp \
../core/CMathndrControllor.cpp \
../core/CStreamPipe.cpp \
../core/CTcpNetProvider.cpp \
../core/CTcpProtocol.cpp \
../core/CThread.cpp \
../core/CUdpEvent.cpp \
../core/IAsync.cpp \
../core/ICallBack.cpp \
../core/IEvent.cpp \
../core/INetProvider.cpp \
../core/main.cpp 

OBJS += \
./core/CAsyncController.o \
./core/CEventController.o \
./core/CEventListener.o \
./core/CMathndrControllor.o \
./core/CStreamPipe.o \
./core/CTcpNetProvider.o \
./core/CTcpProtocol.o \
./core/CThread.o \
./core/CUdpEvent.o \
./core/IAsync.o \
./core/ICallBack.o \
./core/IEvent.o \
./core/INetProvider.o \
./core/main.o 

CPP_DEPS += \
./core/CAsyncController.d \
./core/CEventController.d \
./core/CEventListener.d \
./core/CMathndrControllor.d \
./core/CStreamPipe.d \
./core/CTcpNetProvider.d \
./core/CTcpProtocol.d \
./core/CThread.d \
./core/CUdpEvent.d \
./core/IAsync.d \
./core/ICallBack.d \
./core/IEvent.d \
./core/INetProvider.d \
./core/main.d 


# Each subdirectory must supply rules for building sources it contributes
core/%.o: ../core/%.cpp
	@echo '正在构建文件： $<'
	@echo '正在调用： GNU C++ 编译器'
	g++ -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o"$@" "$<"
	@echo '已结束构建： $<'
	@echo ' '


