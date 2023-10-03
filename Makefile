# g++ compiler
CXX = /usr/bin/g++

# Compiler flags
CXXFLAGS = -Wall -fdiagnostics-color=always -std=c++11

# Include paths (including OpenCV Headers)
INCFLAGS = -I/usr/local/include/opencv4/ -Iinclude/

# Library paths
LDFLAGS = -L/usr/local/lib/ -Lobj/

# opencv libraries
LDLIBS = -lopencv_core -lopencv_highgui -lopencv_imgcodecs -lopencv_imgproc -lopencv_videoio -lopencv_video -lopencv_objdetect

# Directory for object files
OBJDIR = obj

# Directory for final executable
BINDIR = bin

all: obj bin imgDisplay vidDisplay

%.o: src/%.cpp
	$(CXX) $(INCFLAGS) -c $< -o $(OBJDIR)/$@

imgDisplay: imgDisplay.o
	$(CXX) $(CXXFLAGS) $(patsubst %.o,$(OBJDIR)/%.o,$^) -o $(BINDIR)/$@ $(LDFLAGS) $(LDLIBS)

vidDisplay: vidDisplay.o filters.o
	$(CXX) $(CXXFLAGS) $(patsubst %.o,$(OBJDIR)/%.o,$^) -o $(BINDIR)/$@ $(LDFLAGS) $(LDLIBS)

obj:
	mkdir $@

bin:
	mkdir $@

clean:
	rm -f $(OBJDIR)/*.o $(BINDIR)/* *~ 

.PHONY: all clean 
