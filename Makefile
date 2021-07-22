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

build/bin/hyperscale-test: build

debug: build/bin/hyperscale-test
	@lldb ./$<

lex-dev-002: build
	@./build/bin/hyperscale debug lexer dev/002.hps

parse-dev-002: build
	@./build/bin/hyperscale debug parser dev/002.hps

ir-dev-002: build
	@./build/bin/hyperscale debug ir dev/002.hps 2> dev/002.ll
	@llc -filetype=obj -march=x86-64 -o dev/002.o dev/002.ll
	@gcc dev/002.o -o dev/002
	@./dev/002



lex-dev-004: build
	@./build/bin/hyperscale debug lexer dev/004.hps

parse-dev-004: build
	@./build/bin/hyperscale debug parser dev/004.hps


ir-dev-001: build
	@./build/bin/hyperscale debug ir dev/001.hps 2> dev/001.ll
	@llc -filetype=obj -march=x86-64 -o dev/001.o dev/001.ll
	@gcc dev/001.o -o dev/001
	@./dev/001

