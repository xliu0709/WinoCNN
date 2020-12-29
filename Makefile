

IFLAG += -I "./include"
IFLAG += -I "/usr/include/x86_64-linux-gnu"
IFLAG += -D__SIM_FPO__ -D__SIM_OPENCV__ -D__SIM_FFT__ -D__SIM_FIR__ -D__SIM_DDS__ -D__DSP48E1__

# IFLAG += -DDEBUG_FILE_PRINT=1
IFLAG +=  -g 
CFLAG += -fPIC  -O3 -fsanitize=address
CC      = g++ 


# ALLOUT+= csim.out
ALLOUT+= single_csim.out
# ALLOUT+= model.out

all: $(ALLOUT) 


##TO BE MODIFIED START

latency.o:model/latency.cpp
	$(CC) $(GCOV)  $(CFLAG)  -o $@ -c $^     $(IFLAG)
wino.o:src/wino.cpp
	$(CC) $(GCOV)  $(CFLAG)  -o $@ -c $^     $(IFLAG)
misc.o:src/misc.cpp
	$(CC) $(GCOV)  $(CFLAG)  -o $@ -c $^     $(IFLAG)

gold.o:src_gold/wino_gold.cpp
	$(CC) $(GCOV)  $(CFLAG)  -o $@ -c $^     $(IFLAG)
buffer.o:software/buffer.cpp 
	$(CC) $(GCOV)  $(CFLAG)  -o $@ -c $^     $(IFLAG)
param.o:software/param.cpp 
	$(CC) $(GCOV)  $(CFLAG)  -o $@ -c $^     $(IFLAG)
exec.o:software/exec.cpp 
	$(CC) $(GCOV)  $(CFLAG)  -o $@ -c $^  	 $(IFLAG)
##TO BE MODIFIED END

IP_DEP += buffer.o
IP_DEP += param.o
# IP_DEP += wino.o
IP_DEP += gold.o
IP_DEP += misc.o
# IP_DEP += exec.o


main.o:./testbench/main.cpp	
	$(CC) $(GCOV)  $(CFLAG)  -I "${ASSEMBLE_SRC_ROOT}" -o $@  -c $^    $(IFLAG)
single_main.o:./testbench/single_main.cpp	
	$(CC) $(GCOV)  $(CFLAG)  -I "${ASSEMBLE_SRC_ROOT}" -o $@  -c $^    $(IFLAG)


csim.out: main.o $(IP_DEP)
	$(CC)  $(GCOV)  $(CFLAG)  $(IFLAG)  -o $@  $^ 

single_csim.out: single_main.o $(IP_DEP)
	$(CC)  $(GCOV)  $(CFLAG)  $(IFLAG)  -o $@  $^ 

model.out: latency.o $(IP_DEP)
	$(CC)  $(GCOV)  $(CFLAG)  $(IFLAG)  -o $@  $^ 

clean:
	rm -f -r csim.d 
	rm -f *.out *.gcno *.gcda *.txt *.o *.d




