#include <Python.h>
#include "photo2sketch.h"

int main(int argc, char *argv[]) {
    Py_Initialize();
    initphoto2sketch();
    photo2sketch("a.jpg", "a_RCC.jpg");
    Py_Finalize();
}

