
release:
	@echo "Building release version"
	@mkdir -p build
	@cmake -B build . -DVIPRA_BUILD_EXECUTABLE=ON -DVIPRA_BUILD_LIBRARY=OFF -DVIPRA_DEBUG_MODE=OFF -DVIPRA_PERF_TESTING=OFF
	@cmake --build build

debug:
	@echo "Building debug version"
	@mkdir -p build
	@cmake -B build . -DVIPRA_BUILD_EXECUTABLE=ON -DVIPRA_BUILD_LIBRARY=OFF -DVIPRA_DEBUG_MODE=ON -DVIPRA_PERF_TESTING=OFF
	@cmake --build build

perf:
	@echo "Building Performance Testing version"
	@mkdir -p build
	@cmake -B build . -DVIPRA_BUILD_EXECUTABLE=ON -DVIPRA_BUILD_LIBRARY=OFF -DVIPRA_DEBUG_MODE=OFF -DVIPRA_PERF_TESTING=ON
	@cmake --build build