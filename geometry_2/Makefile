CFLAGS = -std=c++17 -O2 -D"NDEBUG"
LDFLAGS = -lglfw -lvulkan -ldl -lpthread -lX11 -lXxf86vm -lXrandr -lXi

VulkanTest: main.cpp
	g++ $(CFLAGS) main.cpp Vkdriver.cpp linalg/src/distance.cpp linalg/src/intersections.cpp linalg/src/plane.cpp \
linalg/src/vect.cpp linalg/src/general.cpp -o main.out $(LDFLAGS)
