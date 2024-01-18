SRC := $(wildcard src/*.c)
SRC += $(wildcard vendor/*/*.c)
OBJS := $(patsubst %.c,build/%.o,$(notdir $(SRC)))

build/vdm: init $(OBJS)
	@echo "  CC \t\e[35;1m$(notdir $@)\e[0m"
	@$(CC) $(OBJS) -o $@ -lm 

init:
	@mkdir -p build

build/%.o: src/%.c
	@echo "  CC \t$(notdir $@)"
	@$(CC) $(CCPARAMS) -o $@ -c $< -I include/

build/%.o: vendor/*/%.c
	@echo "  CC \t$(notdir $@)"
	@$(CC) $(CCPARAMS) -o $@ -c $< -I include/

run: build/vdm
	@build/vdm

clean:
	@rm -rf build/

