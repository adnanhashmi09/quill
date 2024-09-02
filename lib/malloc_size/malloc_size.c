#ifdef __APPLE__
#include <malloc/malloc.h>
#elif defined(__linux__)
#include <malloc.h>
#endif

size_t get_malloc_size(void *ptr) {
#ifdef __APPLE__
  return malloc_size(ptr);
#elif defined(__linux__)
  return malloc_usable_size(ptr);
#else
#error "Unsupported platform"
#endif
}
