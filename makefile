

release:
	@echo "Building release version"
	@mkdir -p build
	@cmake -B build . -DBUILD_EXECUTABLE=ON -DBUILD_LIBRARY=OFF
	@cmake --build build