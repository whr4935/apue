#include <stdio.h>
#include <string.h>

typedef enum {
    /* subtitle codecs */
    CODEC_ID_DVD_SUBTITLE = 0x17000,
    CODEC_ID_DVB_SUBTITLE,
    CODEC_ID_TEXT,  ///< raw UTF-8 text
    CODEC_ID_XSUB,
    CODEC_ID_SSA,
    CODEC_ID_MOV_TEXT,
    CODEC_ID_HDMV_PGS_SUBTITLE,
    CODEC_ID_DVB_TELETEXT,
    CODEC_ID_SRT,
    CODEC_ID_MICRODVD,
    CODEC_ID_SCTE27,
    CODEC_ID_CLOSEDCAPTION,
} SUB_TYPE;

int main(int argc, char *argv[])
{
    char buf[30];
    char* p = NULL;
    /*strncpy(buf, p, 10);*/

    printf("strlen\n");
    int d = 0;
    /*d = strlen(p);*/
    
    /*strcat(buf, p);*/


    printf("strlen = %d\n", d);

    printf("%#x\n", CODEC_ID_SCTE27);


    unsigned short k = -1;
    printf("k = %d\n", k);

    return 0;
}
