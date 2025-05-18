CXX       := g++
CXXFLAGS  := -std=c++17 -O2 -Wall -Wextra -MMD -MP
LDFLAGS   :=

TARGET    := mh_knapsack

SRCS      := $(wildcard *.cpp)
OBJS      := $(SRCS:.cpp=.o)
DEPS      := $(OBJS:.o=.d)

.PHONY: all clean help

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CXX) $(LDFLAGS) -o $@ $^

-include $(DEPS)

%.o: %.cpp merkle_hellman.h
	$(CXX) $(CXXFLAGS) -c $< -o $@

clean:
	rm -f $(OBJS) $(DEPS) $(TARGET)

help:
	@echo "Usage: make [target]"
	@echo "Targets:"                        
	@echo "  all     Build the executable ($(TARGET))"  
	@echo "  clean   Remove all build artifacts"     
	@echo "  help    Show this help message"         
