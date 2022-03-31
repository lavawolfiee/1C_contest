#pragma once

#include <fcntl.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

#include <iostream>
#include <memory>

template <typename T>
class MmapAllocator {
 public:
  using value_type = T;

  MmapAllocator(const char* filename = "automaton.swap",
                std::size_t file_size = 3ll * 1024ll * 1024ll * 1024ll);

  T* allocate(std::size_t n);
  void deallocate(T* ptr, std::size_t n);

  template <typename U>
  MmapAllocator(const MmapAllocator<U>& other);

  template <typename U>
  MmapAllocator& operator=(const MmapAllocator<U>& other);

 private:
  struct MmapAllocatorData {
    void* start = nullptr;
    void* ptr = nullptr;  // ptr to mmap-allocated memory
    std::size_t length = 0;
    std::size_t used = 0;

    MmapAllocatorData(const char* filename, std::size_t file_size);
    ~MmapAllocatorData();

    template <typename U>
    explicit MmapAllocatorData(
        typename MmapAllocator<U>::MmapAllocatorData&& other);

    template <typename U>
    MmapAllocatorData& operator=(
        typename MmapAllocator<U>::MmapAllocatorData&& other);

    template <typename U>
    explicit MmapAllocatorData(
        const typename MmapAllocator<U>::MmapAllocatorData& other) = delete;

    template <typename U>
    MmapAllocatorData& operator=(
        const typename MmapAllocator<U>::MmapAllocatorData& other) = delete;
  };

  std::shared_ptr<MmapAllocatorData> data;

  template <typename>
  friend class MmapAllocator;
};

template <typename T>
MmapAllocator<T>::MmapAllocatorData::MmapAllocatorData(const char* filename,
                                                       std::size_t file_size)
    : length(file_size) {
  int fd = open(filename, O_RDWR | O_CREAT | O_TRUNC, (mode_t)0777);
  ftruncate(fd, 0);
  ftruncate(fd, file_size);
  ptr = mmap(nullptr, file_size, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
  start = ptr;
  close(fd);
}

template <typename T>
MmapAllocator<T>::MmapAllocatorData::~MmapAllocatorData() {
  msync(ptr, length, MS_SYNC);
  munmap(ptr, length);
}

template <typename T>
template <typename U>
MmapAllocator<T>::MmapAllocatorData::MmapAllocatorData(
    typename MmapAllocator<U>::MmapAllocatorData&& other) {
  *this = other;
}

template <typename T>
template <typename U>
typename MmapAllocator<T>::MmapAllocatorData&
MmapAllocator<T>::MmapAllocatorData::operator=(
    typename MmapAllocator<U>::MmapAllocatorData&& other) {
  ptr = std::move(other.ptr);
  length = std::move(other.length);

  return *this;
}

template <typename T>
template <typename U>
MmapAllocator<T>::MmapAllocator(const MmapAllocator<U>& other) {
  *this = other;
}

template <typename T>
template <typename U>
MmapAllocator<T>& MmapAllocator<T>::operator=(const MmapAllocator<U>& other) {
  data = std::reinterpret_pointer_cast<MmapAllocator<T>::MmapAllocatorData>(
      other.data);

  return *this;
}

template <typename T>
MmapAllocator<T>::MmapAllocator(const char* filename, std::size_t file_size)
    : data(std::make_shared<MmapAllocatorData>(filename, file_size)) {}

template <typename T>
T* MmapAllocator<T>::allocate(std::size_t n) {
  T* old_ptr = (T*)data->ptr;
  data->ptr = (void*)(((T*)data->ptr) + n);
  data->used += n * sizeof(T);

  if (data->ptr >= (char*)data->start + data->length) {
    throw std::runtime_error("Ran out of memory in swap file");
  }

  return old_ptr;
}

template <typename T>
void MmapAllocator<T>::deallocate(T* ptr_, std::size_t n) {
  data->used -= n * sizeof(T);
}
