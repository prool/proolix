#include <prool.h>

#include <conf.c>
#include "kernel.h"
/****************************************************************************/
int tolower (int ch)
{
if ((ch>='A')&&(ch<='Z'))return ch + 'a' - 'A'; else return ch;
/*                              ^^^^^^^^^^^^^
                                portions (C) Borland Intl., 1988 */
}
/****************************************************************************/
int toupper (int ch)
{
if ((ch>='a')&&(ch<='z'))return ch + 'A' - 'a'; else return ch;
/*                              ^^^^^^^^^^^^^
                                portions (C) Borland Intl., 1988 */
}
