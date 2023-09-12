CXX=g++

CXXFLAGS=-std=c++11 -Wall -g

LIBRARIES=-lao -lmpg123
FFTWLIBRARY=-lfftw3
FFTW_LOCATION=/usr/include

SOURCES=projekt.cpp signal.o fourier.o
HEADERS=signal.h

TARGET=projekt

all: $(TARGET)

$(TARGET): $(SOURCES) $(HEADERS)
	$(CXX) $(CXXFLAGS) $(SOURCES) -o $(TARGET) $(LIBRARIES) -L$(FFTW_LOCATION) $(FFTWLIBRARY)

signal.o: signal.cpp $(HEADERS)
	$(CXX) $(CXXFLAGS) -c -o signal.o signal.cpp

fourier.o: fft.cpp $(HEADERS)
	$(CXX) $(CXXFLAGS) fft.cpp -c -o fourier.o

run: $(TARGET)			# Dopisać run po make albo make potem ./projekt
	valgrind --leak-check=full ./$(TARGET)

clean:
	rm -f $(TARGET) *.o
