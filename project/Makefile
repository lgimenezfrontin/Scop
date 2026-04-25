NAME = scop

CXX = c++
CXXFLAGS = -Wall -Wextra -Werror -std=c++17 -Iinclude
LDFLAGS = -lglfw -lGLEW -lGL
CFLEAKS = -Wall -Wextra -Werror -std=c++17 -g3 -fsanitize=address,leak,undefined -fno-omit-frame-pointer -Iinclude
LFLEAKS = -lglfw -lGLEW -lGL -fsanitize=address,leak,undefined

SRC_DIR = src
OBJ_DIR = obj

SRCS =	$(SRC_DIR)/main.cpp \
    	$(SRC_DIR)/App.cpp \
    	$(SRC_DIR)/Shader.cpp \
		$(SRC_DIR)/Vec3.cpp \
		$(SRC_DIR)/Mat4.cpp \
		$(SRC_DIR)/Mesh.cpp \
		$(SRC_DIR)/ObjParser.cpp \
		$(SRC_DIR)/Vec2.cpp \
		$(SRC_DIR)/Texture.cpp

OBJS = $(SRCS:$(SRC_DIR)/%.cpp=$(OBJ_DIR)/%.o)

all: $(NAME)

$(NAME): $(OBJS)
	$(CXX) $(OBJS) -o $(NAME) $(LDFLAGS)

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp
	@mkdir -p $(OBJ_DIR)
	$(CXX) $(CXXFLAGS) -c $< -o $@

leaks: fclean
	make CXXFLAGS="-Wall -Wextra -Werror -std=c++17 \
	-g3 -fsanitize=address,leak,undefined -fno-omit-frame-pointer -Iinclude" \
    LDFLAGS="-lglfw -lGLEW -lGL -fsanitize=address,leak,undefined"
	ASAN_OPTIONS=detect_leaks=1:halt_on_error=0 ./scop

clean:
	rm -rf $(OBJ_DIR)

fclean: clean
	rm -f $(NAME)

re: fclean all

.PHONY: all clean fclean re