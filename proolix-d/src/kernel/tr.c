#include "kernel.h"

int TR (int curdev)
{
if (curdev>1) return _HDD_; /* HDD */
else return curdev; /* FDD */
}
