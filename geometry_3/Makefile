.PHONY: all random

CFLAGS = -std=c++17 -O2 -D"NDEBUG"
LDFLAGS = -lglfw -lvulkan -ldl -lpthread -lX11 -lXrandr -lXi

all: 
	g++ $(CFLAGS) main.cpp camera/camera.cpp app/app.cpp Vkdriver/Vkdriver.cpp linalg/src/distance.cpp linalg/src/intersections.cpp linalg/src/plane.cpp \
linalg/src/vect.cpp linalg/src/general.cpp linalg/src/space.cpp -o main.out $(LDFLAGS)

random: 
	g++ $(CFLAGS) tests/generator.cpp camera/camera.cpp app/app.cpp Vkdriver/Vkdriver.cpp linalg/src/distance.cpp linalg/src/intersections.cpp linalg/src/plane.cpp \
linalg/src/vect.cpp linalg/src/general.cpp linalg/src/space.cpp -o random.out $(LDFLAGS)