#pragma once
#define LOG_LEV_TRACE     0
#define LOG_LEV_DEBUG     1
#define LOG_LEV_INFO      2
#define LOG_LEV_WARN      3
#define LOG_LEV_ERROR     4
#define LOG_LEV_CRITICAL  5

void log_init(int loglevel, const char* filename);      // if filename NULL then log only to console
bool log_initialized(void);
void log_trace(const char* message, ...);
void log_debug(const char* message, ...);
void log_info(const char* message, ...);
void log_warn(const char* message, ...);
void log_error(const char* message, ...);
void log_critical(const char* message, ...);


#ifdef SWSLOG_IMPLEMENTATION
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <time.h>
#include <stdarg.h>

bool initializatied = false;
bool filelogging = false;
int levellog = LOG_LEV_INFO;
FILE* logfile = NULL;

const char* loglevelstr[] = {
  "TRACE",
  "DEBUG",
  "INFO",
  "WARNING",
  "ERROR",
  "CRITICAL"
};

void log_init(int loglevel, const char* filename)
{
  if (filename != NULL)
  {
    logfile = fopen(filename, "w");
    filelogging = true;
  }
  else
  {
    filelogging = false;
  }
  levellog = loglevel;
  initializatied = true;
  log_info("Initialized logging system");
}

bool log_initialized(void)
{
  return initializatied;
}

void log_fmt(int level, const char* message, va_list args)
{
  if (level < levellog)
    return;
  time_t now;
  time(&now);     
  char date[80];
  struct tm* info;
  info = localtime(&now);
  strftime(date, 80,"%x - %H:%M:%S", info);
  //date[strlen(date) - 1] = '\0';
  if (filelogging)
  {
    fprintf(logfile, "%s :: [%s] => ", date, loglevelstr[level]);
    vfprintf(logfile, message, args);
    fprintf(logfile, "\n");
    fflush(logfile);
  }  
  printf("%s :: [%s] => ", date, loglevelstr[level]);  
  vprintf(message, args);     
  printf("\n");
}

void log_trace(const char* message, ...)
{
  va_list args;
  va_start(args, message);
  log_fmt(0, message, args);
  va_end(args);
}
void log_debug(const char* message, ...)
{
  va_list args;
  va_start(args, message);
  log_fmt(1, message, args);
  va_end(args);
}
void log_info(const char* message, ...)
{
  va_list args;
  va_start(args, message);
  log_fmt(2, message, args);
  va_end(args);
}
void log_warn(const char* message, ...)
{
  va_list args;
  va_start(args, message);
  log_fmt(3, message, args);
  va_end(args);
}
void log_error(const char* message, ...)
{
  va_list args;
  va_start(args, message);
  log_fmt(4, message, args);
  va_end(args);
}
void log_critical(const char* message, ...)
{
  va_list args;
  va_start(args, message);
  log_fmt(5, message, args);
  va_end(args);
  exit(99);
}

#endif