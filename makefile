SRC := $(wildcard src/*.c)
SRC += $(wildcard vendor/*.c)
OBJS := $(patsubst %.c,build/%.o,$(notdir $(SRC)))

vdm: build/ $(OBJS)
	@echo "  CC \t\033[35;1m$(notdir $@)\033[0m"
	@$(CC) $(OBJS) -o $@ -lm -lpam

build/:
	@mkdir -p build

build/%.o: src/%.c
	@echo "  CC \t$(notdir $@)"
	@$(CC) $(CCPARAMS) -o $@ -c $< -I include/ -I .

build/%.o: vendor/%.c
	@echo "  CC \t$(notdir $@)"
	@$(CC) $(CCPARAMS) -o $@ -c $< -I include/ -I .

run: vdm
	@./vdm

install: vdm
	@sudo systemctl stop mydm
	@sudo cp vdm /usr/bin/mydm
	@sudo systemctl start mydm

clean:
	@rm -rf build/

