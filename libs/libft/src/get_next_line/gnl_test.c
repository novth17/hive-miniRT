#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>

char *get_next_line(int fd);

int main(void)
{
    int fd = open("test_scene.rt", O_RDONLY);
    if (fd < 0)
    {
        perror("open");
        return 1;
    }

    char *line;
    while ((line = get_next_line(fd)))
    {
        printf("GOT: [%s]\n", line);
        free(line);
    }
    close(fd);
    return 0;
}
