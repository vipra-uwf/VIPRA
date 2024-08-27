
release:
	@echo "Building release version"
	@mkdir -p build
	@cmake -B build . -DVIPRA_BUILD_EXECUTABLE=ON -DVIPRA_BUILD_LIBRARY=OFF -DVIPRA_DEBUG_MODE=OFF -DVIPRA_PERF_TESTING=OFF -DVIPRA_USE_MPI=$(mpi) -Wno-dev
	@cmake --build build -j 8

debug:
	@echo "Building debug version"
	@mkdir -p build
	@cmake -B build . -DVIPRA_BUILD_EXECUTABLE=ON -DVIPRA_BUILD_LIBRARY=OFF -DVIPRA_DEBUG_MODE=ON -DVIPRA_PERF_TESTING=OFF -DVIPRA_USE_MPI=$(mpi) -Wno-dev
	@cmake --build build

perf:
	@echo "Building Performance Testing version"
	@mkdir -p build
	@cmake -B build . -DVIPRA_BUILD_EXECUTABLE=ON -DVIPRA_BUILD_LIBRARY=OFF -DVIPRA_DEBUG_MODE=OFF -DVIPRA_PERF_TESTING=ON -DVIPRA_USE_MPI=$(mpi) -DVIPRA_USE_GPERF=$(gperf) -Wno-dev
	@cmake --build build