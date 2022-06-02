/*
 * cli_options.h
 *
 *  Created on: May 12, 2021
 *      Author: me
 */

#ifndef INC_CLI_OPTIONS_H_
#define INC_CLI_OPTIONS_H_

typedef enum{
	ERRORWR	=0,
	WRITE_OK	=1,
	READ_OK
}respondCLI_t;
void help(int argc, const char * const * argv);
void clear(int argc, const char * const * argv);
void list(int argc, const char * const * argv);
void man(int argc, const char * const * argv);
void prolongationsExec(int argc, const char * const * argv);
void catver(int argc, const char *const* argv);

void uid(int argc, const char *const* argv);	//уникальный серийник флешь
void reg( int argc , const char * const * argv );

void eraseAll( int argc , const char * const * argv );

void jedec( int argc , const char * const * argv );

void cat( int argc , const char * const * argv );
void cathex( int argc , const char * const * argv ); ///выводит содержимое флеш в шесн

void echo( int argc , const char * const * argv );

//void binfw( int argc , const char * const * argv );

#endif /* INC_CLICOMMANDS_H_ */
