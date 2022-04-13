#pragma once

/**
 * @brief Simply initializes the name member to the name of the project
 */
typedef struct {
	const char* name;
} Library;

/**
 * @brief Creates an instance of library with the name of the project
 */
Library CreateLibrary(void);

/**
 * @brief Destroys resources held by the library
 */
void DestroyLibrary(Library* lib);
