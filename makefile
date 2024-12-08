NAME = mars_lander_emulator  
CC = c++  
CFLAGS = -std=c++17  -g -I MLX42/include  

MLX_FLAGS = MLX42/build/libmlx42.a -lglfw -L"/Users/hsaktiwy/goinfre/homebrew/opt/glfw/lib"  
 
SRCS = src/main.cpp src/lander.cpp src/Terrain.cpp src/Game.cpp  src/mlx_draw_line.cpp src/Solution.cpp
OBJS = $(SRCS:.cpp=.o)  
header = src/lander.hpp src/Terrain.hpp src/Game.hpp  src/mlx_draw_line.hpp src/Solution.hpp
# Rules  
all: $(NAME)  

$(NAME): $(OBJS)  
	$(CC) $(CFLAGS) $(OBJS) -o $(NAME) $(MLX_FLAGS)

%.o: %.cpp  $(header)
	$(CC) $(CFLAGS) -c $< -o $@  

clean:
	rm -f $(OBJS)  

fclean: clean  
	rm -f $(NAME)  

re: fclean all  