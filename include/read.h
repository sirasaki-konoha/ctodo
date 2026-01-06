#ifndef _INTERNAL_READ_H
#define _INTERNAL_READ_H

/**
 * Read a specifiec file
 * Please free after use
 *
 * @param file_path path to file
 * @return dynamically allocated memory reference
 */
char* read_file(const char* file_path);

#endif /* _INTERNAL_READ_H */
