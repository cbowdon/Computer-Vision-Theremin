# Run from binaries directory

entry_point = main.o
main_assemblies = VideoProvider.o HandTracker.o PointAccountant.o Profiler.o SoundGenerator.o BgSoundGenerator.o NoteGenerator.o LinearConverter.o Note.o NoteProvider.o
main_executable = lk

test_entry_point = test.cpp
test_assemblies = PointAccountantTest.o LinearConverterTest.o NoteGeneratorTest.o NoteTest.o
test_executable = lk.test

opencv_libs = -L /usr/local/lib/ -lopencv_core -lopencv_highgui -lopencv_video -lopencv_imgproc
boost_libs = -L /usr/local/lib/ -lboost_timer -lboost_system
stk_libs = -L /usr/local/lib -lstk
defs = -D__MACOSX_CORE__

cpp = c++ -std=c++11 -stdlib=libc++
cpp_flags = -I include -I include/test -Wall 

vpath %.hpp include include/test
vpath %.cpp src src/test

%.o: %.cpp
	$(cpp) -c $< $(cpp_flags) $(defs) -o $@

all: $(entry_point) $(main_assemblies)
	ctags -R .
	$(cpp) $(cpp_flags) $^ -o $(main_executable) $(opencv_libs) $(boost_libs) $(stk_libs)

run: all
	./$(main_executable)

tests: $(test_entry_point) $(test_assemblies) $(main_assemblies)
	ctags -R .
	$(cpp) $(cpp_flags) $^ -o $(test_executable) $(opencv_libs) $(stk_libs)

run_tests: tests
	./$(test_executable)

sample: sample.cpp $(main_assemblies)
	ctags -R .
	$(cpp) $(cpp_flags) $^ -o sample $(opencv_libs) $(boost_libs) $(stk_libs)

clean:
	rm -f $(main_executable)
	rm -f $(main_assemblies)
	rm -f $(test_executable)
	rm -f $(test_assemblies)
