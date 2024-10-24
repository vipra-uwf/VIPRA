
threads ?= 1

release:
	@echo "Building release version"
	@mkdir -p build
	@cmake -B build . -DVIPRA_MAIN_FILE=main.cpp -DVIPRA_EXECUTABLE_PATH=../ -DVIPRA_BUILD_EXECUTABLE=ON -DVIPRA_DEBUG_MODE=OFF -DVIPRA_PERF_TESTING=OFF -DVIPRA_USE_MPI=$(mpi) -Wno-dev
	@cmake --build build -j $(threads)

debug:
	@echo "Building debug version"
	@mkdir -p build
	@cmake -B build . -DVIPRA_MAIN_FILE=main.cpp -DVIPRA_EXECUTABLE_PATH=../ -DVIPRA_BUILD_EXECUTABLE=ON -DVIPRA_DEBUG_MODE=ON -DVIPRA_PERF_TESTING=OFF -DVIPRA_USE_MPI=$(mpi) -Wno-dev
	@cmake --build build -j $(threads)

perf:
	@echo "Building Performance Testing version"
	@mkdir -p build
	@cmake -B build . -DVIPRA_MAIN_FILE=main.cpp -DVIPRA_EXECUTABLE_PATH=../ -DVIPRA_BUILD_EXECUTABLE=ON -DVIPRA_DEBUG_MODE=OFF -DVIPRA_PERF_TESTING=ON -DVIPRA_USE_MPI=$(mpi) -DVIPRA_USE_GPERF=$(gperf) -Wno-dev
	@cmake --build build -j $(threads)

testing:
	@echo "Building release version"
	@mkdir -p build
	@cmake -B build . -DVIPRA_BUILD_EXECUTABLE=OFF -DVIPRA_DEBUG_MODE=OFF -DVIPRA_PERF_TESTING=OFF -DVIPRA_USE_MPI=OFF -DVIPRA_TESTING=ON -Wno-dev
	@cmake --build build -j $(threads)