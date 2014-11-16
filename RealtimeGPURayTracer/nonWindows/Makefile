SRC=$(wildcard ../RealtimeGPURayTracer/*.cpp)
HEADER=$(wildcard ../RealtimeGPURayTracer/*.h)
OBJ=$(notdir $(patsubst %.cpp,%.o,$(SRC)))

CXX=g++
TARGET=rtGrt
INCLUDE=-I../RealtimeGPURayTracer -I../externalLib/include/ -I./tinyobjloader-master -I/usr/local/Cellar/libpng/1.6.13/include/
CFLAG=-std=c++11 -DGLM_FORCE_RADIANS -Wno-deprecated

all: $(TARGET)

$(TARGET): $(OBJ)
	$(CXX) -o $(TARGET) -framework OpenGL -framework GLUT -L/usr/local/Cellar/libpng/1.6.13/lib -lpng ./tinyobjloader-master/libtiny_obj_loader.a $(OBJ)

$(OBJ):%.o:../RealtimeGPURayTracer/%.cpp
	$(CXX) -c $(CFLAG) $(INCLUDE) $^ -o $@
