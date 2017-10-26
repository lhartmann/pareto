CXXFLAGS+=-O3

testbench: testbench.cpp pareto.h

clean:
	rm -f pareto.txt pareto_data.txt testbench

