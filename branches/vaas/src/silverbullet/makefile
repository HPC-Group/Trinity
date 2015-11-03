CC=g++
CFLAGS=-Wall -std=c++0x -Wunreachable-code
LIBS=-lpthread
INCLUDES=-I.
SRC = Core/StringTools.cpp Core/IO/FileTools.cpp \
      Core/IO/KeyValueFileParser.cpp \
      Core/Time/Timer.cpp \
      Core/Time/MessageTimer.cpp \
      Core/IO/MD5.cpp Core/IO/MemMappedFile.cpp \
      DataIO/License.cpp DataIO/Metadata.cpp \
      DataIO/DataType.cpp \
      DataIO/GeometryIO/GeometryMetadata.cpp \
      DataIO/GeometryIO/Geometry.cpp \
      DataIO/GeometryIO/VTKGeometry.cpp \
      DataIO/VolumeIO/VolumeMetadata.cpp \
      DataIO/VolumeIO/QVisVolumeMetadata.cpp \
      DataIO/VolumeIO/Volume.cpp \
      DataIO/VolumeIO/FileBasedVolume.cpp \
      DataIO/VolumeIO/QVisVolume.cpp \
      DataIO/VolumeIO/MemVolume.cpp \
      DataIO/VolumeIO/RawVolume.cpp \
      DataIO/VolumeIO/VolumeProcessor.cpp \
      DataIO/VolumeIO/VolumeTransformer.cpp \
      DataIO/VolumeIO/ResizeParams.cpp \
      main.cpp
OBJ = $(SRC:.cpp=.o)
TARGET = silverBullet

all: $(TARGET)

release: CFLAGS += -O3 -flto -DNDEBUG
release: $(TARGET)

svn:
	-svn up

$(TARGET): $(OBJ)
	$(CC) $(CFLAGS) $(INCLUDES) $(LIBS) $^ -o $@

%.o: %.cpp
	$(CC) -c $(CFLAGS) $(INCLUDES) $^ -o $@

clean:
	-rm -rf $(OBJ) $(TARGET)
