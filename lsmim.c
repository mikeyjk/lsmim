#include "lsmim.h"

/**
 * 'lsmim' -> 'ls mimic' -> 'mimics ls poorly'
 *
 * Actual 'ls':
 *  http://git.savannah.gnu.org/cgit/coreutils.git/tree/src/ls.c
 *
 * */

const char *defaultPath = "./"; // default directory -> current working directory

/**
 * Checks for cmd line arguments.
 * If there is one and it is -s or -S it prints the default directory in the simplified format.
 * If there is 2 and the second is -s or -S it prints the supplied directory in the simplified format.
 * If there is one it prints the requested format from q6 using the supplied directory.
 */
int main(int argc, char ** argv)
{
    switch(argc)
    {
        case(1): // default cmd line args
            expandedLS((char*)defaultPath);
            break;
        case(2): // 1 command line arg
            if(isS(argv[1])) // if argument 1 is -s or -S
            {
                // presents simple 'ls' format, a bit like ls but stripped down, for debugging/testing success accessing dirs and files
                simpleLS((char*)defaultPath);
            }
            else // else file or directory
            {
                expandedLS(argv[1]);
            }

            break;

        case(3): // 2 command line args

            if(isS(argv[2])) // 2nd argument is option
            {
                simpleLS(argv[1]);
            }
            else if(isS(argv[1])) // 1st argument is option
            {
                simpleLS(argv[2]);
            }
            else
            {
                help(); // expected either 1st or 2nd argument is option
            }
            break;

        default:

            help();
            break;

    }

    return(0);
}