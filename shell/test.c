#include <stdlib.h>
#include <fcntl.h>

int main (int argc, char** argv) {
    write(1,"foo\n",4);
    return 0;
}

