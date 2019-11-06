
SRCDIR = src
OBJDIR = obj
BINDIR = bin

TARGET=wsserver

SOURCES = $(wildcard $(SRCDIR)/*.cpp)
OBJECTS = $(patsubst %.cpp,%.o,$(notdir $(SOURCES))) #$(addprefix $(BINDIR)/,$(patsubst %.cpp,%.o,$(notdir $(SOURCES))))

IBOOST_PATH = ${HOME}/Projects/boost_1_71_0
LBOOST_PATH = ${HOME}/Projects/boost_1_71_0/stage/lib/

CXX = g++
CXXFLAGS = -Wall -g -I$(IBOOST_PATH) -L $(LBOOST_PATH) -lpthread -lboost_thread

all: $(TARGET)

# build target by linking object files and providing an rpath for boost libs
$(TARGET): $(OBJECTS)
	$(CXX) $(CXXFLAGS) -Wl,-rpath ./lib -o $(BINDIR)/$@ $(addprefix $(OBJDIR)/,$(OBJECTS))

$(OBJECTS): %.o: $(SRCDIR)/%.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $(OBJDIR)/$@
