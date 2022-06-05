#ifndef XTIME_H_INCLUDED
#define XTIME_H_INCLUDED

typedef	unsigned short u16_t;
typedef	unsigned long u32_t;
typedef	signed short s16_t;
typedef	signed long	s32_t;
typedef	signed char s8_t;
typedef	unsigned char u8_t;

signed char addhour;
signed char addwday;
signed char addmins;

	// DEF: standard signed format
	// UNDEF: non-standard unsigned format
	//#define	_XT_SIGNED

#ifdef	_XT_SIGNED
	typedef	s32_t xtime_t;
#else
//	typedef	u32_t xtime_t;
	typedef	long long xtime_t;
#endif

#define	_TBIAS_YEAR		1900

typedef struct{
    /* date and time components */
	unsigned char tm_sec;
	unsigned char tm_min;
	unsigned char tm_hour;
	unsigned char tm_mday;
	unsigned char tm_mon;
	unsigned char tm_wday;
	u16_t tm_year;
}datetime_t;

typedef char timezone_t;

//typedef struct tms ttm;

void xttotm(datetime_t *t, xtime_t secs);
xtime_t xtmtot(datetime_t const *t);

#endif
