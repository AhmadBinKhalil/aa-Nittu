# Makefile for building sequential, FastFlow, and MPI wavefront versions

CXX     = g++
MPICXX  = mpic++
CXXFLAGS = -std=c++11 -O3
FF_INC  = -Ifastflow  # Adjust this to your FastFlow include directory

# Build targets: we now include sequential_wavefront in "all"
all: sequential_wavefront fastflow_wavefront mpi_wavefront

# Sequential version
sequential_wavefront: sequential_wavefront.cpp
	$(CXX) $(CXXFLAGS) sequential_wavefront.cpp -o sequential_wavefront

# FastFlow version
fastflow_wavefront: fastflow_wavefront.cpp
	$(CXX) $(CXXFLAGS) $(FF_INC) fastflow_wavefront.cpp -o fastflow_wavefront -pthread

# MPI version
mpi_wavefront: mpi_wavefront.cpp
	$(MPICXX) $(CXXFLAGS) mpi_wavefront.cpp -o mpi_wavefront

# Cleanup
clean:
	rm -f sequential_wavefront fastflow_wavefront mpi_wavefront
