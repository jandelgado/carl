# Carl Music Box
# use this makefile to build Carl with platformio 
.PHONY: phony


all: phony
	pio run

lint: phony
	cpplint --root=src --extensions=cpp,h,ino --filter=-build/include_subdir \
		$(shell find src  \( ! -regex '.*/\..*' \) \
	      -type f -a \( -name "*\.cpp" -o -name "*\.h" -o -name "*\.ino" \) )

ci: phony
	pio ci --project-conf platformio.ini src

clean: phony
	pio run --target clean

upload: phony
	pio run --target upload 

monitor: phony
	pio device monitor 

tags: phony
	ctags -R

phony:
