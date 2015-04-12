################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CC_SRCS += \
../test/main.cc 

OBJS += \
./test/main.o 

CC_DEPS += \
./test/main.d 


# Each subdirectory must supply rules for building sources it contributes
test/%.o: ../test/%.cc
	@echo 'Building file: $<'
	@echo 'Invoking: Cross G++ Compiler'
	g++ -DDEBUG -DNOJUNIOR -O0 -g3 -Wall -c -fmessage-length=0 --std=c++0x -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


