#include <stdio.h>
#include <stdlib.h>
#include <string.h>


int loadInputFile();
int nextLine(char *dst);


//#define DEBUG
#ifdef DEBUG
#define INPUT_FILENAME "input1.txt"
#define TARGET 10
#else
#define INPUT_FILENAME "input.txt"
#define TARGET 2000000
#endif

#define BUFROWLEN 7000000
#define ROWOFFSET 1500000
int bufTargetRow[BUFROWLEN]; /*gross overestimation based based on xmin, xmax, ymin, ymax precomputations*/
int *targetRow = bufTargetRow + ROWOFFSET; /* allow indexes [-1500000, 4500000)*/

char line[100];
int main()
{
    if (loadInputFile() < 0)
    {
        fprintf(stderr, "Can't open: " INPUT_FILENAME "\n");
        return 1;
    }

    int sx, sy, bx, by, dist, x;
    char *pstr;
    while (nextLine(line))
    {
        /* read signal - beacon*/
        pstr = line;
        pstr = strchr(pstr, '=');
        sx = (int) strtol(pstr+1, &pstr, 10);
        pstr = strchr(pstr, '=');
        sy = (int) strtol(pstr+1, &pstr, 10);
        pstr = strchr(pstr, '=');
        bx = (int) strtol(pstr+1, &pstr, 10);
        pstr = strchr(pstr, '=');
        by = (int) strtol(pstr+1, &pstr, 10);

        /* manh distance s-b*/
        dist = abs(sx - bx) + abs (sy - by);

        /* max x distance in target row in which no beacons are present*/
        dist -= abs(TARGET - sy);
        if(dist >= 0){
            x = sx - dist;
            targetRow[x]++; /*+1 kronecked delta*/
            x = sx + dist;
            targetRow[x]--; /*-1 kronecked delta*/
        }
    }

    int sum = 0;
    int lim = BUFROWLEN-ROWOFFSET, prev, curr = 0;
    for(x = -ROWOFFSET; x < lim; x++)
    {
        prev = curr;
        curr = targetRow[x]+prev;
        if(curr>0)
            sum++;
    }

    printf("%d\n", sum);


}

#define BUFSIZE 2200

char fileBuf[BUFSIZE];
char *pbuf = fileBuf;
char *pbufend = fileBuf;

int loadInputFile()
{
    FILE *pf = fopen(INPUT_FILENAME, "r");
    if (!pf)
        return -1;
    pbuf = fileBuf;
    pbufend = fileBuf + fread(fileBuf, 1, BUFSIZE, pf);
    fclose(pf);

    return 0;
}

int nextLine(char *dst)
{
    char c;
    char *orig = dst;

    // read line:
    while (pbuf < pbufend && (c = *(pbuf++)) != '\n')
    {
        *(dst++) = c;
    }
    *dst = '\0';
    // return line length
    return dst - orig;
}