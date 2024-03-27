
release:
	@echo "Building release version"
	@mkdir -p build
	@cmake -B build . -DVIPRA_BUILD_EXECUTABLE=ON -DVIPRA_BUILD_LIBRARY=OFF -DVIPRA_DEBUG_MODE=OFF
	@cmake --build build