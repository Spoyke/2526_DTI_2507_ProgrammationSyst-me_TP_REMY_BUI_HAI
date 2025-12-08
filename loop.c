#include <stdio.h>
#include <unistd.h>

// Program used to test the display "ensah[sign:...] % " when the child is stopped by a signal

int main() {
    printf("PID = %d\n", getpid());
    for (int i = 0; i < 999999999; i++) {
        sleep(1);
    }
}
