
/******************************************************************************/
/*                             Start of crctable.c                            */
/******************************************************************************/
/*                                                                            */
/* Author  : Ross Williams (ross@guest.adelaide.edu.au.).                     */
/* Date    : 3 June 1993.                                                     */
/* Version : 1.0.                                                             */
/* Status  : Public domain.                                                   */
/*                                                                            */
/* Description : This program writes a CRC lookup table (suitable for         */
/* inclusion in a C program) to a designated output file. The program can be  */
/* statically configured to produce any table covered by the Rocksoft^tm      */
/* Model CRC Algorithm. For more information on the Rocksoft^tm Model CRC     */
/* Algorithm, see the document titled "A Painless Guide to CRC Error          */
/* Detection Algorithms" by Ross Williams (ross@guest.adelaide.edu.au.). This */
/* document is likely to be in "ftp.adelaide.edu.au/pub/rocksoft".            */
/*                                                                            */
/* Note: Rocksoft is a trademark of Rocksoft Pty Ltd, Adelaide, Australia.    */
/*                                                                            */
/******************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include "crc.h"

/******************************************************************************/

/* TABLE PARAMETERS                                                           */
/* ================                                                           */
/* The following parameters entirely determine the table to be generated. You */
/* should need to modify only the definitions in this section before running  */
/* this program.                                                              */
/*                                                                            */
/*    TB_FILE  is the name of the output file.                                */
/*    TB_WIDTH is the table width in bytes (either 2 or 4).                   */
/*    TB_POLY  is the "polynomial", which must be TB_WIDTH bytes wide.        */
/*    TB_REVER indicates whether the table is to be reversed (reflected).     */
/*                                                                            */
/* Example:                                                                   */
/*                                                                            */
/*    #define TB_FILE   "crctable.out"                                        */
/*    #define TB_WIDTH  2                                                     */
/*    #define TB_POLY   0x8005L                                               */
/*    #define TB_REVER  TRUE                                                  */

#define TB_FILE   "crctable.out"
#define TB_WIDTH  4
#define TB_POLY   0x04C11DB7L
#define TB_REVER  TRUE

/******************************************************************************/

/* Miscellaneous definitions. */

#define LOCAL static
FILE *outfile;
#define WR(X) fprintf(outfile,(X))
#define WP(X,Y) fprintf(outfile,(X),(Y))

/******************************************************************************/

LOCAL void chk_err P_((char *));

/* If mess is non-empty, write it out and abort. Otherwise, check the error   */
/* status of outfile and abort if an error has occurred.*/

LOCAL void chk_err(char *mess)
{
	if (mess[0] != 0) { printf("%s\n", mess); exit(EXIT_FAILURE); }
	if (ferror(outfile)) { perror("chk_err");   exit(EXIT_FAILURE); }
}

/******************************************************************************/

LOCAL void chkparam P_((void));
LOCAL void chkparam()
{
	if ((TB_WIDTH != 2) && (TB_WIDTH != 4))
		chk_err("chkparam: Width parameter is illegal.");
	if ((TB_WIDTH == 2) && (TB_POLY & 0xFFFF0000L))
		chk_err("chkparam: Poly parameter is too wide.");
	if ((TB_REVER != FALSE) && (TB_REVER != TRUE))
		chk_err("chkparam: Reverse parameter is not boolean.");
}

/******************************************************************************/

LOCAL void gentable P_((void));
LOCAL void gentable()
{
	WR("/*****************************************************************/\n");
	WR("/*                                                               */\n");
	WR("/* CRC LOOKUP TABLE                                              */\n");
	WR("/* ================                                              */\n");
	WR("/* The following CRC lookup table was generated automagically    */\n");
	WR("/* by the Rocksoft^tm Model CRC Algorithm Table Generation       */\n");
	WR("/* Program V1.0 using the following model parameters:            */\n");
	WR("/*                                                               */\n");
	WP("/*    Width   : %1lu bytes.                                         */\n",
		(ulong)TB_WIDTH);
	if (TB_WIDTH == 2)
		WP("/*    Poly    : 0x%04lX                                           */\n",
		(ulong)TB_POLY);
	else
		WP("/*    Poly    : 0x%08lXL                                      */\n",
		(ulong)TB_POLY);
	if (TB_REVER)
		WR("/*    Reverse : TRUE.                                            */\n");
	else
		WR("/*    Reverse : FALSE.                                           */\n");
	WR("/*                                                               */\n");
	WR("/* For more information on the Rocksoft^tm Model CRC Algorithm,  */\n");
	WR("/* see the document titled \"A Painless Guide to CRC Error        */\n");
	WR("/* Detection Algorithms\" by Ross Williams                        */\n");
	WR("/* (ross@guest.adelaide.edu.au.). This document is likely to be  */\n");
	WR("/* in the FTP archive \"ftp.adelaide.edu.au/pub/rocksoft\".        */\n");
	WR("/*                                                               */\n");
	WR("/*****************************************************************/\n");
	WR("\n");
	switch (TB_WIDTH) {
	case 2: WR("unsigned short crctable[256] =\n{\n"); break;
	case 4: WR("unsigned long  crctable[256] =\n{\n"); break;
	default: chk_err("gentable: TB_WIDTH is invalid.");
	}
	chk_err("");

	{
		int i;
		cm_t cm;
		char *form = (TB_WIDTH == 2) ? "0x%04lX" : "0x%08lXL";
		int   perline = (TB_WIDTH == 2) ? 8 : 4;

		cm.cm_width = TB_WIDTH * 8;
		cm.cm_poly = TB_POLY;
		cm.cm_refin = TB_REVER;

		for (i = 0; i < 256; i++) {
			WR(" ");
			WP(form, (ulong)cm_tab(&cm, i));
			if (i != 255) WR(",");
			if (((i + 1) % perline) == 0) WR("\n");
			chk_err("");
		}

		WR("};\n");
		WR("\n");
		WR("/*****************************************************************/\n");
		WR("/*                   End of CRC Lookup Table                     */\n");
		WR("/*****************************************************************/\n");
		WR("");
		chk_err("");
	}
}

/******************************************************************************/

// main()
// {
// 	printf("\n");
// 	printf("Rocksoft^tm Model CRC Algorithm Table Generation Program V1.0\n");
// 	printf("-------------------------------------------------------------\n");
// 	printf("Output file is \"%s\".\n", TB_FILE);
// 	chkparam();
// 	outfile = fopen(TB_FILE, "w"); chk_err("");
// 	gentable();
// 	if (fclose(outfile) != 0)
// 		chk_err("main: Couldn't close output file.");
// 	printf("\nSUCCESS: The table has been successfully written.\n");
// }


/******************************************************************************/
/*                             End of crctable.c                              */
/******************************************************************************/

/******************************************************************************/
/*                             Start of crcmodel.c                            */
/******************************************************************************/
/*                                                                            */
/* Author : Ross Williams (ross@guest.adelaide.edu.au.).                      */
/* Date   : 3 June 1993.                                                      */
/* Status : Public domain.                                                    */
/*                                                                            */
/* Description : This is the implementation (.c) file for the reference       */
/* implementation of the Rocksoft^tm Model CRC Algorithm. For more            */
/* information on the Rocksoft^tm Model CRC Algorithm, see the document       */
/* titled "A Painless Guide to CRC Error Detection Algorithms" by Ross        */
/* Williams (ross@guest.adelaide.edu.au.). This document is likely to be in   */
/* "ftp.adelaide.edu.au/pub/rocksoft".                                        */
/*                                                                            */
/* Note: Rocksoft is a trademark of Rocksoft Pty Ltd, Adelaide, Australia.    */
/*                                                                            */
/******************************************************************************/
/*                                                                            */
/* Implementation Notes                                                       */
/* --------------------                                                       */
/* To avoid inconsistencies, the specification of each function is not echoed */
/* here. See the header file for a description of these functions.            */
/* This package is light on checking because I want to keep it short and      */
/* simple and portable (i.e. it would be too messy to distribute my entire    */
/* C culture (e.g. assertions package) with this package.                     */
/*                                                                            */
/******************************************************************************/

/******************************************************************************/

/* The following definitions make the code more readable. */

#define BITMASK(X) (1L << (X))
#define MASK32 0xFFFFFFFFL
#define LOCAL static

/******************************************************************************/

LOCAL ulong reflect P_((ulong v, int b));

/* Returns the value v with the bottom b [0,32] bits reflected. */
/* Example: reflect(0x3e23L,3) == 0x3e26						*/
LOCAL ulong reflect(ulong v, int b)
{
	int   i;
	ulong t = v;
	for (i = 0; i < b; i++) {
		if (t & 1L)
			v |= BITMASK((b - 1) - i);
		else
			v &= ~BITMASK((b - 1) - i);
		t >>= 1;
	}
	return v;
}

/******************************************************************************/

LOCAL ulong widmask P_((p_cm_t));

/* Returns a longword whose value is (2^p_cm->cm_width)-1.     */
/* The trick is to do this portably (e.g. without doing <<32). */
LOCAL ulong widmask(p_cm_t p_cm)
{
	return (((1L << (p_cm->cm_width - 1)) - 1L) << 1) | 1L;
}

/******************************************************************************/

void cm_ini(p_cm_t p_cm)
{
	p_cm->cm_reg = p_cm->cm_init;
}

/******************************************************************************/

void cm_nxt(p_cm_t p_cm, int ch)
{
	int   i;
	ulong uch = (ulong)ch;
	ulong topbit = BITMASK(p_cm->cm_width - 1);

	if (p_cm->cm_refin) uch = reflect(uch, 8);
	p_cm->cm_reg ^= (uch << (p_cm->cm_width - 8));
	for (i = 0; i < 8; i++) {
		if (p_cm->cm_reg & topbit)
			p_cm->cm_reg = (p_cm->cm_reg << 1) ^ p_cm->cm_poly;
		else
			p_cm->cm_reg <<= 1;
		p_cm->cm_reg &= widmask(p_cm);
	}
}

/******************************************************************************/

void cm_blk(p_cm_t p_cm, p_ubyte_ blk_adr, ulong blk_len)
{
	while (blk_len--) cm_nxt(p_cm, *blk_adr++);
}

/******************************************************************************/

ulong cm_crc(p_cm_t p_cm)
{
	if (p_cm->cm_refot)
		return p_cm->cm_xorot ^ reflect(p_cm->cm_reg, p_cm->cm_width);
	else
		return p_cm->cm_xorot ^ p_cm->cm_reg;
}

/******************************************************************************/

ulong cm_tab(p_cm_t p_cm, int index)
{
	int   i;
	ulong r;
	ulong topbit = BITMASK(p_cm->cm_width - 1);
	ulong inbyte = (ulong)index;

	if (p_cm->cm_refin) inbyte = reflect(inbyte, 8);
	r = inbyte << (p_cm->cm_width - 8);
	for (i = 0; i < 8; i++)
		if (r & topbit)
			r = (r << 1) ^ p_cm->cm_poly;
		else
			r <<= 1;
	if (p_cm->cm_refin) r = reflect(r, p_cm->cm_width);
	return r & widmask(p_cm);
}

/******************************************************************************/
/*                             End of crcmodel.c                              */
/******************************************************************************/
#define		CRC8_POLY	0x07
#define		CRC16_POLY	0x1021
#define		CRC32_POLY	0x04C11DB7

int crc_bits_alg(char *pdata, int len, int crc, int width, int poly)
{
	int i;
	int offset = 1 << width - 1;
	
	while (len--) {
		crc ^= (*pdata++ << width - 8);
		for (i = 0; i < 8; ++i) {
			if (crc & offset)
				crc = (crc << 1) ^ poly;
			else
				crc <<= 1;
		}
	}

	return crc;
}

int crc_table[256];

int crc_create_tabel(int width, int poly)
{
	int i, n;
	int offset = 1 << width - 1;
	int crc;
	int mask;

	for (i = 0; i < 256; ++i) {
		crc = i << width-8;
		for (n = 0; n < 8; ++n) {
			if (crc & offset)
				crc = (crc << 1) ^ poly;
			else
				crc <<= 1;
		}

		mask = 1 << 32;
		mask = ~0 << 31;
		mask = ~0 << 8;
		mask = ((~0L) << width);		//左移32位不可移植
//		crc_table[i] = crc & (width % 32 ? (~(~0 << width)) : ~0);
		crc_table[i] = crc & (((1 << width - 1) - 1) << 1 | 1L);
	}

	return 0;
}

int crc_table_alg(char *pdata, int len, int crc, int width, int *table)
{
	//每次处理一个字节数据，先在后面补上width-8个0，然后左移8位，自动补上剩下的8个0
	//本来数据应该分4次一个字节一个字节地移动到最高位，但由于数据后面带的是0，真正
	//有效果的是数据字节本身所在的那一列，所以直接把数据字节与crc寄存器异或即可。

	while (len--) {
		crc = (crc << 8) ^ table[(((crc >> (width-8))&0xFF) ^ *pdata++) & 0xFF];
	}

	return crc;
}


int test_crc()
{
	char pdata[] = "1";
	int len = strlen(pdata);
	cm_t cm, *p_cm = &cm;
	int crc;
	int i;

#if 1
	pdata[0] = 0xF6;
	crc = crc_bits_alg(pdata, len, 0, 8, CRC8_POLY);
	printf("crc = 0x%X\n", (unsigned char)crc);

	crc_create_tabel(8, CRC8_POLY);
	crc = crc_table_alg(pdata, len, 0, 8, crc_table);
	printf("crc = 0x%X\n", (unsigned char)crc);

	printf("random number: %d\n");
	crc = 246;
	for (i = 0; i < 40; ++i) {
		crc = crc_table_alg(&crc, 1, 0, 8, crc_table);
		crc &= 0xFF;
		printf("%d ", (unsigned char)crc);
		if (i && i % 10 == 0)
			printf("\n");
	}
	printf("\n");

#elif 0
	crc = crc_bits_alg(pdata, len, 0, 16, CRC16_POLY);
	printf("crc = 0x%X\n", (unsigned short)crc);

	crc_create_tabel(16, CRC16_POLY);
	crc = crc_table_alg(pdata, len, 0, 16, crc_table);
	printf("crc = 0x%X\n", (unsigned short)crc);

#elif 0
	crc = crc_bits_alg(pdata, len, 0, 32, CRC32_POLY);
	printf("crc = 0x%0X\n", crc);

	crc_create_tabel(32, CRC32_POLY);
	crc = crc_table_alg(pdata, len, 0, 32, crc_table);
	printf("crc = 0x%0X\n", crc);
#endif

	p_cm->cm_width = 32;                                       
	p_cm->cm_poly = CRC32_POLY;
	p_cm->cm_init = 0L;
	p_cm->cm_refin = FALSE;
	p_cm->cm_refot = FALSE;
	p_cm->cm_xorot = FALSE;

	cm_ini(p_cm);
	cm_blk(p_cm, pdata, len);

	crc = cm_crc(p_cm);
	printf("crc = 0x%X\n", crc);

	return 0;
}
