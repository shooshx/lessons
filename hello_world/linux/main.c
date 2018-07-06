#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <errno.h>
#include <sys/mman.h>

int main() {
    int err = 0;
    char* buf = NULL;

    write(1, "Hello World!\n", 13);

    int fd = open("../test_file.txt", O_RDONLY);
    if (fd == -1) {
        err = errno;
        goto done;
    }
    
    struct stat st;
    fstat(fd, &st);
    buf = mmap(NULL, st.st_size, PROT_READ | PROT_WRITE, MAP_PRIVATE | MAP_ANONYMOUS, -1, 0);
    if (buf == MAP_FAILED)
        goto done;
    read(fd, buf, st.st_size);
    write(1, "file:", 5);
    write(1, buf, st.st_size);

done:
    if (buf != NULL)
        munmap(buf, st.st_size);
    if (fd != -1)
        close(fd);
    return err;
}
