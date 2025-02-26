#include <stdlib.h>
#include <stdio.h>
#include <assert.h>

// Structure to hold information about the array
typedef struct {
    size_t capacity; // Total capacity of the array
    size_t size;     // Current number of elements in the array
    size_t bytes; // Size of each element in the array
} array_info;

// Macro to define an array for a specific type
#define array(T) T*

// Initializes an array for a specific type
#define array_init(type) (type *)_array_init(sizeof(type))

// Returns a pointer to the array_info structure from the given array pointer
#define array_header(ptr) ((array_info *)((char *)(ptr) - sizeof(array_info)))

// Retrieves the current size (number of elements) of the array
#define array_size(ptr) (array_header(ptr)->size)

// Adds a value to the end of the array, ensuring capacity and incrementing the size
#define array_add(arr, value) do { \
    _array_ensure_capacity((void **)&(arr), sizeof(*arr)); \
    (arr)[array_size(arr)] = (value); \
    array_header(arr)->size++; \
} while (0)

// Retrieves the current capacity of the array
#define array_capacity(ptr) (array_header(ptr)->capacity)

// Retrieves the size of elements stored in the array
#define array_bytes(ptr) (array_header(ptr)->bytes)

// Internal function to initialize an array of a specific element size
void *_array_init(size_t bytes) {
    // Allocate memory for the array metadata and initial element storage.
    array_info *info = malloc(sizeof(array_info) + (bytes * 2));
    if (!info) {
        perror("Failed to allocate memory for array");
        exit(EXIT_FAILURE);
    }
    info->capacity = 2; 
    info->size = 0;    
    info->bytes = bytes; 
    return (void *)(info + 1); // Return pointer to the first element.
}

// Ensures the array has enough capacity to add new elements, resizing if necessary.
void _array_ensure_capacity(void **arr, size_t bytes) {
    array_info *info = array_header(*arr);
    if (info->size == info->capacity) {
        size_t new_capacity = info->capacity * 2;
        array_info *new_info = realloc(info, sizeof(array_info) + new_capacity * bytes);
        if (!new_info) {
            perror("Failed to reallocate memory for array");
            exit(EXIT_FAILURE);
        }
        *arr = (void *)(new_info + 1); // Update the array pointer.
        new_info->capacity = new_capacity; 
    }
}

// Prints information about the array (capacity, size, and element size).
void print_array_info(void *arr) {
    array_info *info = array_header(arr);
    printf("Array Info:\n");
    printf("Capacity: %zu\n", info->capacity);
    printf("Size: %zu\n", info->size);
    printf("Element Size: %zu bytes\n", info->bytes);
}

// Prints an integer element.
void print_int(void *data) {
    printf("%d ", *(int *)data);
}

// Prints a char element.
void print_char(void *data) {
    printf("%c ", *(char *)data);
}

// Prints elements of the array using a specified print function.
void array_print(void *arr, void (*print_func)(void *)) {
    for (int i = 0; i < array_size(arr); ++i) {
        print_func((char *)arr + i * array_bytes(arr));
    }
}

// Destroys the array, freeing its allocated memory.
void array_destroy(void *arr) {
    if (arr == NULL) return; 
    array_info *info = array_header(arr);
    free(info); 
}

// Removes the last element from the array.
void array_pop(void *arr) {
    array_info *info = array_header(arr);
    if (info->size > 0) {
        info->size--; 
    }
}

// Main function to test the array functionality
int main() {
    // Initialize an array of integers
    array(int) arr = array_init(int);

    // Test array initialization
    array_info *info = array_header(arr);
    // Check if the capacity is 2 (initial size)
    assert(info->capacity == 2 && "Array initialization failed: incorrect capacity.");
    
    // Check if the size is 0 (no elements added yet)
    assert(info->size == 0 && "Array initialization failed: incorrect size.");
     
    // Check if the element size is correct (should be size of int)
    assert(info->bytes == sizeof(int) && "Array initialization failed: incorrect element size.");
     
    // Print a message indicating all tests passed
    printf("Array initialization passed successfully!\n");
    // // Add some integers to the array
    // array_add(arr, 10);
    // array_add(arr, 20);
    // array_add(arr, 30);
    // array_add(arr, 40);

    // // Print array information
    // print_array_info(arr);
    
    // // Print elements of the array
    // printf("Array elements: ");
    // array_print(arr, print_int);
    // printf("\n");

    // // Remove an element and print again
    // array_pop(arr);
    // printf("After popping an element:\n");
    // print_array_info(arr);
    // printf("Array elements: ");
    // array_print(arr, print_int);
    // printf("\n");

    // Destroy the array
    array_destroy(arr);

    return 0;
}
