#ifndef __CACTUS_DATETIME_H__
#define __CACTUS_DATETIME_H__




#define LONG_LONG long long




inline static int
unixtime_to_filetime(time_t time, FILETIME *ft)
{
    ULARGE_INTEGER tmp;

    tmp.QuadPart = ((LONG_LONG)time + (LONG_LONG)((1970-1601)*365.2425) * 24 * 60 * 60) * 10 * 1000 * 1000;
    ft->dwLowDateTime = tmp.LowPart;
    ft->dwHighDateTime = tmp.HighPart;
    return 0;
}

/* License: Ruby's */
inline static int
unixtime_to_systemtime(const time_t t, SYSTEMTIME *st)
{
    FILETIME ft;
    if (unixtime_to_filetime(t, &ft)) return -1;
    if (!FileTimeToSystemTime(&ft, st)) return -1;
    return 0;
}

/* License: Ruby's */
inline static void
systemtime_to_tm(const SYSTEMTIME *st, struct tm *t)
{
    int y = st->wYear, m = st->wMonth, d = st->wDay;
    t->tm_sec  = st->wSecond;
    t->tm_min  = st->wMinute;
    t->tm_hour = st->wHour;
    t->tm_mday = st->wDay;
    t->tm_mon  = st->wMonth - 1;
    t->tm_year = y - 1900;
    t->tm_wday = st->wDayOfWeek;
    switch (m) {
      case 1:
          break;
      case 2:
          d += 31;
          break;
      default:
          d += 31 + 28 + (!(y % 4) && ((y % 100) || !(y % 400)));
          d += ((m - 3) * 153 + 2) / 5;
          break;
    }
    t->tm_yday = d - 1;
}

/* License: Ruby's */
static int
systemtime_to_localtime(TIME_ZONE_INFORMATION *tz, SYSTEMTIME *gst, SYSTEMTIME *lst)
{
    TIME_ZONE_INFORMATION stdtz;
    SYSTEMTIME sst;

    if (!SystemTimeToTzSpecificLocalTime(tz, gst, lst)) return -1;
    if (!tz) {
        GetTimeZoneInformation(&stdtz);
        tz = &stdtz;
    }
    if (tz->StandardBias == tz->DaylightBias) return 0;
    if (!tz->StandardDate.wMonth) return 0;
    if (!tz->DaylightDate.wMonth) return 0;
    if (tz != &stdtz) stdtz = *tz;

    stdtz.StandardDate.wMonth = stdtz.DaylightDate.wMonth = 0;
    if (!SystemTimeToTzSpecificLocalTime(&stdtz, gst, &sst)) return 0;
    if (lst->wMinute == sst.wMinute && lst->wHour == sst.wHour)
        return 0;
    return 1;
}




#endif // __CACTUS_DATETIME_H__
