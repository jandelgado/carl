# Carl Music Box
# use this makefile to build Carl with platformio 
.PHONY: phony

all: phony
	pio run

lint: phony
	cpplint --root=carl --extensions=cpp,h,ino --filter=-build/include_subdir \
		$(shell find .  \( ! -regex '.*/\..*' \) \
	      -type f -a \( -name "*\.cpp" -o -name "*\.h" -o -name "*\.ino" \) )

ci: phony
	pio ci --project-conf platformio.ini .

clean: phony
	pio run --target clean

upload-makuna: phony
	pio run --target upload --environment pro16MHzatmega328-makuna

upload-dfrobot: phony
	pio run --target upload --environment pro16MHzatmega328-dfrobot

upload-powerbroker: phony
	pio run --target upload --environment pro16MHzatmega328-powerbroker

monitor: phony
	pio device monitor 

tags: phony
	ctags -R

phony:
