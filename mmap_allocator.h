#pragma once

#include <fcntl.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

template <typename T>
class MmapAllocator {
 public:
  using value_type = T;

  MmapAllocator(char* filename = "automaton.swap",
                std::size_t file_size = 1024 * 1024 * 1024);
  ~MmapAllocator();

  static T* allocate(std::size_t n);
  static void deallocate(T* ptr, std::size_t n);

  template <typename U>
  MmapAllocator(const MmapAllocator<U>& other);

  template <typename U>
  MmapAllocator& operator=(const MmapAllocator<U>& other);

 private:
  void* ptr;  // ptr to mmap-allocated memory
  std::size_t length;
};

template <typename T>
template <typename U>
MmapAllocator<T>::MmapAllocator(const MmapAllocator<U>& other) {
  ptr = other.ptr;
}

template <typename T>
template <typename U>
MmapAllocator<T>& MmapAllocator<T>::operator=(const MmapAllocator<U>& other) {
  ptr = other.ptr;

  return *this;
}

template <typename T>
MmapAllocator<T>::MmapAllocator(char* filename, std::size_t file_size)
    : length(file_size) {
  int fd = open(filename, O_RDWR | O_CREAT | O_TRUNC, (mode_t)0777);
  ptr = mmap(0, file_size, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
  close(fd);
}

template <typename T>
MmapAllocator<T>::~MmapAllocator() {
  munmap(ptr, length);
}
