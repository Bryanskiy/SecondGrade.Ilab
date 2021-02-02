CFLAGS = -std=c++17 -O2 -D"NDEBUG" 
LDFLAGS = -lglfw -lvulkan -ldl -lpthread -lX11 -lXrandr -lXi

VulkanTest: main.cpp
	g++ $(CFLAGS) main.cpp Vkdriver.cpp linalg/src/distance.cpp linalg/src/intersections.cpp linalg/src/plane.cpp \
linalg/src/vect.cpp linalg/src/general.cpp linalg/src/space.cpp -o main.out $(LDFLAGS)
