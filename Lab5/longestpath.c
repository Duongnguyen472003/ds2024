#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_PATH_LENGTH 1024

// Function to find the longest path in a file
char* find_longest_path(FILE* file) {
  char path[MAX_PATH_LENGTH];
  char* longest_path = NULL;  // Use a pointer to dynamically allocate memory

  while (fgets(path, MAX_PATH_LENGTH, file) != NULL) {
    int length = strlen(path);
    // Remove newline character if present
    if (path[length - 1] == '\n')
      path[length - 1] = '\0';

    if (!longest_path || length > strlen(longest_path)) {
      // Free previously allocated memory (if any)
      free(longest_path);
      longest_path = malloc(length + 1);  // Allocate memory for the path
      if (longest_path == NULL) {
        perror("Memory allocation failed");
        exit(EXIT_FAILURE);
      }
      strcpy(longest_path, path);
    }
  }

  return longest_path;  // Return the pointer to the longest path
}

int main() {
  FILE* file = fopen("test.txt", "r");
  if (file == NULL) {
    perror("Error opening file");
    return 1;
  }

  char* longest_path = find_longest_path(file);  // Call the function
  if (longest_path != NULL) {
    printf("The longest path found: %s\n", longest_path);
    free(longest_path);  // Free the allocated memory
  } else {
    printf("No paths found in the file.\n");
  }

  fclose(file);
  return 0;
}