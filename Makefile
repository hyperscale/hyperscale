.PHONY: build

clean:
	@rm -rf build/

build:
	@mkdir -p build
	@find ./build -type f -name "*.gcda" -exec rm -f {} \;
	@cd build; cmake .. -DCMAKE_BUILD_TYPE=Debug
	@cd build; make

test: build
	@cd build; make test

cover: build
	@cd build; make hyperscale-coverage
