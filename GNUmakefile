# Run from binaries directory

CXX = c++
CXXFLAGS = -std=c++11 -stdlib=libc++ -Wall -D__MACOSX_CORE__
LDFLAGS = -I include -I include/test

OPENCV_LIBS = -L /usr/local/lib/ -lopencv_core -lopencv_highgui -lopencv_video -lopencv_imgproc
BOOST_LIBS = -L /usr/local/lib/ -lboost_timer -lboost_system
STK_LIBS = -L /usr/local/lib -lstk

RM = rm -f
MV = mv a.out

entry_point = main.cpp
main_assemblies = VideoProvider.o HandTracker.o PointAccountant.o Profiler.o SoundGenerator.o BgSoundGenerator.o NoteGenerator.o LinearConverter.o Note.o NoteProvider.o TrackFile.o
main_executable = main.lk

test_entry_point = test.cpp
test_assemblies = PointAccountantTest.o LinearConverterTest.o NoteGeneratorTest.o NoteTest.o InterpolationTest.o TrackFileTest.o StatisticsTest.o
test_executable = test.lk

sample_entry_point = sample.cpp
sample_assemblies = TrackFile.o VideoProvider.o
sample_executable = sample.lk

vpath %.hpp include include/test
vpath %.cpp src src/test

%.o: %.cpp %.hpp
	$(CXX) -c $< $(CXXFLAGS) $(LDFLAGS) -o $@

all: $(entry_point) $(main_assemblies)
	ctags -R .
	$(CXX) $(CXXFLAGS) $(LDFLAGS) $(OPENCV_LIBS) $(BOOST_LIBS) $(STK_LIBS) $^
	$(MV) $(main_executable)

run: all
	./$(main_executable)

tests: $(test_entry_point) $(test_assemblies) $(main_assemblies) interpolation.hpp statistics.hpp
	ctags -R .
	$(CXX) $(CXXFLAGS) $(LDFLAGS) $(OPENCV_LIBS) $(STK_LIBS) $^
	$(MV) $(test_executable)

run_tests: tests
	./$(test_executable)

sample: $(sample_entry_point) $(sample_assemblies) interpolation.hpp
	ctags -R .
	$(CXX) $(CXXFLAGS) $(LDFLAGS) $(OPENCV_LIBS) $(BOOST_LIBS) $(STK_LIBS) $^
	$(MV) $(sample_executable)

clean:
	$(RM) $(main_executable)
	$(RM) $(main_assemblies)
	$(RM) $(test_executable)
	$(RM) $(test_assemblies)
	$(RM) $(sample_assemblies)
	$(RM) $(sample_executable)
	$(RM) a.out
