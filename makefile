CXX=clang++-9
CXXVERSION=c++2a
SOURCE_PATH=sources
OBJECT_PATH=objects
CXXFLAGS=-std=$(CXXVERSION) -Werror -Wsign-conversion -I$(SOURCE_PATH) 
TIDY_FLAGS=-extra-arg=-std=$(CXXVERSION) -checks=bugprone-*,clang-analyzer-*,cppcoreguidelines-*,performance-*,portability-*,readability-*,-cppcoreguidelines-pro-bounds-pointer-arithmetic,-cppcoreguidelines-owning-memory --warnings-as-errors=*
VALGRIND_FLAGS=-v --leak-check=full --show-leak-kinds=all  --error-exitcode=99

SOURCES=$(wildcard $(SOURCE_PATH)/*.cpp)
HEADERS=$(wildcard $(SOURCE_PATH)/*.hpp)
OBJECTS=$(subst sources/,objects/,$(subst .cpp,.o,$(SOURCES)))

# run: myshell test
run: myshell 	

myshell:myshell.o $(OBJECTS) 
	$(CXX) $(CXXFLAGS) $^ -o  $@ 

test:TestRunner.o Test.o $(OBJECTS)
	$(CXX) $(CXXFLAGS) $^ -o  $@ 

%.o: %.cpp $(HEADERS)
	$(CXX) $(CXXFLAGS) --compile $< -o $@ 

$(OBJECT_PATH)/%.o: $(SOURCE_PATH)/%.cpp $(HEADERS)
	$(CXX) $(CXXFLAGS) --compile $< -o $@ 

tidy:
	clang-tidy $(SOURCES) $(TIDY_FLAGS) --

valgrind: myshell test
	valgrind --tool=memcheck $(VALGRIND_FLAGS) ./myshell 
	# valgrind --tool=memcheck $(VALGRIND_FLAGS) ./test 2>&1 | { egrep "lost| at " || true; }


clean:
	rm -f $(OBJECTS) *.o *.a *.so *.out test myshell myfile colors.txt
