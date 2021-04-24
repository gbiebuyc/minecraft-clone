NAME = minecraft.exe
SRC = $(wildcard src/*.c)
OBJ = $(patsubst src/%.c, obj/%.o, $(SRC))
CFLAGS =
LDFLAGS = -lopengl32 -lglu32 -lgdi32
.PHONY: all clean fclean re

all: ./obj $(NAME)

./obj:
	mkdir ./obj

$(NAME): $(OBJ)
	cc -o $@ $^ $(LDFLAGS)

./obj/%.o: ./src/%.c
	cc $(CFLAGS) -o $@ -c $<

clean:
	rm -rf $(OBJ)

fclean:
	rm -rf $(OBJ) $(NAME)

re: fclean all
