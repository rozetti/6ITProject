/* The 6IT Project. Copyright 2015 Conrad Rozetti, crz@6itproject.org. Distributed under the MIT License, see 6IT.h. */

#pragma once

int source_seek(void *context, int offset);
int source_get_char(void *context, char *c);

int load_file(struct machine_t *machine, char const *filename, char **buffer);
int open_file(struct machine_t *machine, char const *filename);
void close_file(struct machine_t *machine);
