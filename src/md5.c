/*
 * This code implements the MD5 message-digest algorithm.
 * The algorithm is due to Ron Rivest.  This code was
 * written by Colin Plumb in 1993, no copyright is claimed.
 * This code is in the public domain; do with it what you wish.
 *  
 * Equivalent code is available from RSA Data Security, Inc.
 * This code has been tested against that, and is equivalent,
 * except that you don't need to include two pages of legalese
 * with every copy.
 *  
 * To compute the message digest of a chunk of bytes, declare an
 * MD5Context structure, pass it to MD5Init, call MD5Update as
 * needed on buffers full of bytes, and then call MD5Final, which
 * will fill a supplied 16-byte array with the digest.
 */
/*
 * Modified (2001-01-31) to work on Sparks    <gray@farlep.net>
 */
#if defined(HAVE_CONFIG_H)
# include <config.h>
#endif

#include <sys/types.h>
#include <stdint.h>
#include <string.h>             /* for memcpy() */

#include "basicauth.h"

struct MD5Context {
        uint32_t buf[4];
        uint32_t bits[2];
        unsigned char in[64];
};

typedef struct MD5Context MD5_CTX;

void MD5Init(struct MD5Context *ctx);
void MD5Update(struct MD5Context *ctx, unsigned char const *buf, unsigned len);
void MD5Final(unsigned char digest[16], struct MD5Context *ctx);
void MD5Transform(uint32_t buf[4], uint32_t const cin[16]);

void
md5_calc(unsigned char *output, unsigned char const *input,
	 unsigned int inlen)  
{
        MD5_CTX context;
	
        MD5Init(&context);
        MD5Update(&context, input, inlen);
        MD5Final(output, &context);
}


static void
bytes_encode(unsigned char *output, uint32_t *input, unsigned int len)
{
        unsigned int i, j;

        for (i = 0, j = 0; j < len; i++, j += 4) {
                output[j] = (unsigned char)(input[i] & 0xff);
                output[j+1] = (unsigned char)((input[i] >> 8) & 0xff);
                output[j+2] = (unsigned char)((input[i] >> 16) & 0xff);
                output[j+3] = (unsigned char)((input[i] >> 24) & 0xff);
        }
}

static void 
bytes_decode(uint32_t *output, unsigned char *input, unsigned int len)
{
        unsigned int i, j;
        
        for (i = 0, j = 0; j < len; i++, j += 4)
                output[i] = ((uint32_t)input[j]) |
                            (((uint32_t)input[j+1]) << 8) |
                            (((uint32_t)input[j+2]) << 16) |
                            (((uint32_t)input[j+3]) << 24);
}

/*
 * Start MD5 accumulation.  Set bit count to 0 and buffer to mysterious
 * initialization constants.
 */
void
MD5Init(struct MD5Context *ctx)
{
    	ctx->buf[0] = 0x67452301;
    	ctx->buf[1] = 0xefcdab89;
    	ctx->buf[2] = 0x98badcfe;
    	ctx->buf[3] = 0x10325476;

    	ctx->bits[0] = 0;
    	ctx->bits[1] = 0;
}

/*
 * Update context to reflect the concatenation of another buffer full
 * of bytes.
 */
void
MD5Update(struct MD5Context *ctx, unsigned char const *buf, unsigned len)
{
    	uint32_t t;

    	/* Update bitcount */

    	t = ctx->bits[0];
    	if ((ctx->bits[0] = t + ((uint32_t) len << 3)) < t)
        	ctx->bits[1]++;         /* Carry from low to high */
    	ctx->bits[1] += len >> 29;

    	t = (t >> 3) & 0x3f;        /* Bytes already in shsInfo->data */
    	/* Handle any leading odd-sized chunks */

    	if (t) {
        	unsigned char *p = (unsigned char *) ctx->in + t;
        	t = 64 - t;
        	if (len < t) {
            		memcpy(p, buf, len);
            		return;
        	}
        	memcpy(p, buf, t);
        	MD5Transform(ctx->buf, (uint32_t *) ctx->in);
        	buf += t;
        	len -= t;
    	}
    	/* Process data in 64-byte chunks */

    	while (len >= 64) {
        	memcpy(ctx->in, buf, 64);
        	MD5Transform(ctx->buf, (uint32_t const *) buf);
        	buf += 64;
        	len -= 64;
    	}

    	/* Handle any remaining bytes of data. */

    	memcpy(ctx->in, buf, len);
}

/*
 * Final wrapup - pad to 64-byte boundary with the bit pattern 
 * 1 0* (64-bit count of bits processed, MSB-first)
 */
void
MD5Final(unsigned char digest[16], struct MD5Context *ctx)
{
    	unsigned count;
    	unsigned char *p;

    	/* Compute number of bytes mod 64 */
    	count = (ctx->bits[0] >> 3) & 0x3F;

    	/* Set the first char of padding to 0x80.  This is safe since there is
       	   always at least one byte free */
    	p = ctx->in + count;
    	*p++ = 0x80;

    	/* Bytes of padding needed to make 64 bytes */
    	count = 64 - 1 - count;

    	/* Pad out to 56 mod 64 */
    	if (count < 8) {
        	/* Two lots of padding:  Pad the first block to 64 bytes */
        	memset(p, 0, count);
        	MD5Transform(ctx->buf, (uint32_t *) ctx->in);

        	/* Now fill the next block with 56 bytes */
        	memset(ctx->in, 0, 56);
    	} else {
        	/* Pad block to 56 bytes */
        	memset(p, 0, count - 8);
    	}

    	/* Append length in bits and transform */
    	bytes_encode((unsigned char*)((uint32_t *) ctx->in + 14), ctx->bits, 8);
    	MD5Transform(ctx->buf, (uint32_t *) ctx->in);
    	bytes_encode(digest,ctx->buf,16);   
    	memset((char *) ctx, 0, sizeof(ctx));       /* In case it's sensitive */
}

/* The four core functions - F1 is optimized somewhat */

#define F1(x, y, z) (x & y | ~x & z) 
#define F2(x, y, z) F1(z, x, y)
#define F3(x, y, z) (x ^ y ^ z)
#define F4(x, y, z) (y ^ (x | ~z))

/* This is the central step in the MD5 algorithm. */
#define MD5STEP(f, w, x, y, z, data, s) \
        ( w += f(x, y, z) + data,  w = w<<s | w>>(32-s),  w += x ); 

#if 0
dump(char *label,unsigned char *p, int len)
{
        int i;
        return;
        printf("dump: %s\n", label);
        for (i=0; i<len; i++)
                printf("%x\n", p[i]);
        printf("--\n");

}
#endif

/*
 * The core of the MD5 algorithm, this alters an existing MD5 hash to
 * reflect the addition of 16 longwords of new data.  MD5Update blocks
 * the data and converts bytes into longwords for this routine.
 */
void
MD5Transform(uint32_t buf[4], uint32_t const cin[16])
{
    	register uint32_t a, b, c, d;
    	uint32_t in[16];

    	bytes_decode(in, (unsigned char *) cin, 64);

    	a = buf[0];
    	b = buf[1];
    	c = buf[2];
    	d = buf[3];

    	MD5STEP(F1, a, b, c, d, in[0] + 0xd76aa478, 7);
    	MD5STEP(F1, d, a, b, c, in[1] + 0xe8c7b756, 12);
    	MD5STEP(F1, c, d, a, b, in[2] + 0x242070db, 17);
    	MD5STEP(F1, b, c, d, a, in[3] + 0xc1bdceee, 22);
    	MD5STEP(F1, a, b, c, d, in[4] + 0xf57c0faf, 7);
    	MD5STEP(F1, d, a, b, c, in[5] + 0x4787c62a, 12);
    	MD5STEP(F1, c, d, a, b, in[6] + 0xa8304613, 17);
    	MD5STEP(F1, b, c, d, a, in[7] + 0xfd469501, 22);
    	MD5STEP(F1, a, b, c, d, in[8] + 0x698098d8, 7);
    	MD5STEP(F1, d, a, b, c, in[9] + 0x8b44f7af, 12);
    	MD5STEP(F1, c, d, a, b, in[10] + 0xffff5bb1, 17);
    	MD5STEP(F1, b, c, d, a, in[11] + 0x895cd7be, 22);
    	MD5STEP(F1, a, b, c, d, in[12] + 0x6b901122, 7);
    	MD5STEP(F1, d, a, b, c, in[13] + 0xfd987193, 12);
    	MD5STEP(F1, c, d, a, b, in[14] + 0xa679438e, 17);
    	MD5STEP(F1, b, c, d, a, in[15] + 0x49b40821, 22);

    	MD5STEP(F2, a, b, c, d, in[1] + 0xf61e2562, 5);
    	MD5STEP(F2, d, a, b, c, in[6] + 0xc040b340, 9);
    	MD5STEP(F2, c, d, a, b, in[11] + 0x265e5a51, 14);
    	MD5STEP(F2, b, c, d, a, in[0] + 0xe9b6c7aa, 20);
    	MD5STEP(F2, a, b, c, d, in[5] + 0xd62f105d, 5);
    	MD5STEP(F2, d, a, b, c, in[10] + 0x02441453, 9);
    	MD5STEP(F2, c, d, a, b, in[15] + 0xd8a1e681, 14);
    	MD5STEP(F2, b, c, d, a, in[4] + 0xe7d3fbc8, 20);
    	MD5STEP(F2, a, b, c, d, in[9] + 0x21e1cde6, 5);
    	MD5STEP(F2, d, a, b, c, in[14] + 0xc33707d6, 9);
    	MD5STEP(F2, c, d, a, b, in[3] + 0xf4d50d87, 14);
    	MD5STEP(F2, b, c, d, a, in[8] + 0x455a14ed, 20);
    	MD5STEP(F2, a, b, c, d, in[13] + 0xa9e3e905, 5);
    	MD5STEP(F2, d, a, b, c, in[2] + 0xfcefa3f8, 9);
    	MD5STEP(F2, c, d, a, b, in[7] + 0x676f02d9, 14);
    	MD5STEP(F2, b, c, d, a, in[12] + 0x8d2a4c8a, 20);

    	MD5STEP(F3, a, b, c, d, in[5] + 0xfffa3942, 4);
    	MD5STEP(F3, d, a, b, c, in[8] + 0x8771f681, 11);
    	MD5STEP(F3, c, d, a, b, in[11] + 0x6d9d6122, 16);
    	MD5STEP(F3, b, c, d, a, in[14] + 0xfde5380c, 23);
    	MD5STEP(F3, a, b, c, d, in[1] + 0xa4beea44, 4);
    	MD5STEP(F3, d, a, b, c, in[4] + 0x4bdecfa9, 11);
    	MD5STEP(F3, c, d, a, b, in[7] + 0xf6bb4b60, 16);
    	MD5STEP(F3, b, c, d, a, in[10] + 0xbebfbc70, 23);
    	MD5STEP(F3, a, b, c, d, in[13] + 0x289b7ec6, 4);
    	MD5STEP(F3, d, a, b, c, in[0] + 0xeaa127fa, 11);
    	MD5STEP(F3, c, d, a, b, in[3] + 0xd4ef3085, 16);
    	MD5STEP(F3, b, c, d, a, in[6] + 0x04881d05, 23);
    	MD5STEP(F3, a, b, c, d, in[9] + 0xd9d4d039, 4);
    	MD5STEP(F3, d, a, b, c, in[12] + 0xe6db99e5, 11);
    	MD5STEP(F3, c, d, a, b, in[15] + 0x1fa27cf8, 16);
    	MD5STEP(F3, b, c, d, a, in[2] + 0xc4ac5665, 23);

    	MD5STEP(F4, a, b, c, d, in[0] + 0xf4292244, 6);
    	MD5STEP(F4, d, a, b, c, in[7] + 0x432aff97, 10);
    	MD5STEP(F4, c, d, a, b, in[14] + 0xab9423a7, 15);
    	MD5STEP(F4, b, c, d, a, in[5] + 0xfc93a039, 21);
    	MD5STEP(F4, a, b, c, d, in[12] + 0x655b59c3, 6);
    	MD5STEP(F4, d, a, b, c, in[3] + 0x8f0ccc92, 10);
    	MD5STEP(F4, c, d, a, b, in[10] + 0xffeff47d, 15);
    	MD5STEP(F4, b, c, d, a, in[1] + 0x85845dd1, 21);
    	MD5STEP(F4, a, b, c, d, in[8] + 0x6fa87e4f, 6);
    	MD5STEP(F4, d, a, b, c, in[15] + 0xfe2ce6e0, 10);
    	MD5STEP(F4, c, d, a, b, in[6] + 0xa3014314, 15);
    	MD5STEP(F4, b, c, d, a, in[13] + 0x4e0811a1, 21);
    	MD5STEP(F4, a, b, c, d, in[4] + 0xf7537e82, 6);
    	MD5STEP(F4, d, a, b, c, in[11] + 0xbd3af235, 10);
    	MD5STEP(F4, c, d, a, b, in[2] + 0x2ad7d2bb, 15);
    	MD5STEP(F4, b, c, d, a, in[9] + 0xeb86d391, 21);

    	buf[0] += a;
    	buf[1] += b;
    	buf[2] += c;
    	buf[3] += d;
}

/* APR-specific code */

/*
 * Define the Magic String prefix that identifies a password as being
 * hashed using our algorithm.
 */
static const char *const apr1_id = "$apr1$";

/*
 * The following MD5 password encryption code was largely borrowed from
 * the FreeBSD 3.0 /usr/src/lib/libcrypt/crypt.c file, which is
 * licenced as stated at the top of this file.
 */

static void 
to64(char *s, unsigned long v, int n)
{
	static unsigned char itoa64[] =         /* 0 ... 63 => ASCII - 64 */
            "./0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz";

    	while (--n >= 0) {
        	*s++ = itoa64[v&0x3f];
        	v >>= 6;
    	}
}

#define APR_MD5_DIGESTSIZE 16

char *
apr_md5_encode(const char *pw, const char *salt, char *result, size_t nbytes)
{
    	/*
     	* Minimum size is 8 bytes for salt, plus 1 for the trailing NUL,
        * plus 4 for the '$' separators, plus the password hash itself.
        * Let's leave a goodly amount of leeway.
        */

    	char passwd[120], *p;
    	const char *sp, *ep;
    	unsigned char final[APR_MD5_DIGESTSIZE];
    	ssize_t sl, pl, i;
    	MD5_CTX ctx, ctx1;
    	unsigned long l;

    	/* 
     	* Refine the salt first.  It's possible we were given an already-hashed
    	* string as the salt argument, so extract the actual salt value from it
    	* if so.  Otherwise just use the string up to the first '$' as the salt.
     	*/
    	sp = salt;

    	/*
     	* If it starts with the magic string, then skip that.
     	*/
    	if (!strncmp(sp, apr1_id, strlen(apr1_id))) {
        	sp += strlen(apr1_id);
   	}

    	/*
     	* It stops at the first '$' or 8 chars, whichever comes first
     	*/
    	for (ep = sp; (*ep != '\0') && (*ep != '$') && (ep < (sp + 8)); ep++) 
        	;

    	/*
     	* Get the length of the true salt
     	*/
    	sl = ep - sp;

    	/*
     	* 'Time to make the doughnuts..'
     	*/
    	MD5Init(&ctx);
    
   	/*
     	* The password first, since that is what is most unknown
     	*/
    	MD5Update(&ctx, pw, strlen(pw));

    	/*
     	* Then our magic string
     	*/
    	MD5Update(&ctx, apr1_id, strlen(apr1_id));

    	/*
     	* Then the raw salt
     	*/
    	MD5Update(&ctx, sp, sl);

    	/*
     	* Then just as many characters of the MD5(pw, salt, pw)
     	*/
    	MD5Init(&ctx1);
    	MD5Update(&ctx1, pw, strlen(pw));
    	MD5Update(&ctx1, sp, sl);
    	MD5Update(&ctx1, pw, strlen(pw));
    	MD5Final(final, &ctx1);
    	for (pl = strlen(pw); pl > 0; pl -= APR_MD5_DIGESTSIZE) {
       		MD5Update(&ctx, final,
                          (pl > APR_MD5_DIGESTSIZE) ? APR_MD5_DIGESTSIZE : pl);
    	}

    	/*
     	* Don't leave anything around in vm they could use.
     	*/
    	memset(final, 0, sizeof(final));

    	/*
     	* Then something really weird...
     	*/
    	for (i = strlen(pw); i != 0; i >>= 1) {
        	if (i & 1) 
            		MD5Update(&ctx, final, 1);
        	else
            		MD5Update(&ctx, pw, 1);
    	}

    	/*
     	* Now make the output string.  We know our limitations, so we
     	* can use the string routines without bounds checking.
     	*/
    	strcpy(passwd, apr1_id);
    	strncat(passwd, sp, sl);
    	strcat(passwd, "$");

    	MD5Final(final, &ctx);

    	/*
    	* And now, just to make sure things don't run too fast..
     	* On a 60 Mhz Pentium this takes 34 msec, so you would
     	* need 30 seconds to build a 1000 entry dictionary...
     	*/
    	for (i = 0; i < 1000; i++) {
        	MD5Init(&ctx1);
         	/*
          	* apr_md5_final clears out ctx1.xlate at the end of each loop,
          	* so need to to set it each time through
          	*/
        	if (i & 1)
            		MD5Update(&ctx1, pw, strlen(pw));
        	else 
            		MD5Update(&ctx1, final, APR_MD5_DIGESTSIZE);
        	if (i % 3)
            		MD5Update(&ctx1, sp, sl);

        	if (i % 7) 
            		MD5Update(&ctx1, pw, strlen(pw));

        	if (i & 1)
            		MD5Update(&ctx1, final, APR_MD5_DIGESTSIZE);
        	else 
            		MD5Update(&ctx1, pw, strlen(pw));
        	MD5Final(final,&ctx1);
    	}

    	p = passwd + strlen(passwd);

    	l = (final[ 0]<<16) | (final[ 6]<<8) | final[12]; to64(p, l, 4); p += 4;
    	l = (final[ 1]<<16) | (final[ 7]<<8) | final[13]; to64(p, l, 4); p += 4;
    	l = (final[ 2]<<16) | (final[ 8]<<8) | final[14]; to64(p, l, 4); p += 4;
    	l = (final[ 3]<<16) | (final[ 9]<<8) | final[15]; to64(p, l, 4); p += 4;
    	l = (final[ 4]<<16) | (final[10]<<8) | final[ 5]; to64(p, l, 4); p += 4;
    	l =                    final[11]                ; to64(p, l, 2); p += 2;
    	*p = '\0';

    	/*
     	* Don't leave anything around in vm they could use.
     	*/
    	memset(final, 0, sizeof(final));

	i = strlen(passwd);
	if (i >= nbytes)
		i = nbytes - 1;
	memcpy(result, passwd, i);
	result[i] = 0;
	return result;
}

#ifdef STANDALONE
int
main(int argc, char **argv)
{
        unsigned char result[120];
	if (argc != 3)
		exit(1);
	apr_md5_encode(argv[1], argv[2], result, sizeof(result));
	printf("%s\n",result);
}
#endif
