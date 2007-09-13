/*
 * Hydrogen
 * Copyright(c) 2002-2007 by Alex >Comix< Cominu [comix@users.sourceforge.net]
 *
 * http://www.hydrogen-music.org
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY, without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 *
 */

#include <hydrogen/Object.h>

#include <QDir>
#include <iostream>
#include <pthread.h>

#ifdef WIN32
	#include <windows.h>
#else
	#include <unistd.h>
#endif

using namespace std;

unsigned int Object::__objects = 0;
bool Object::__use_log = false;
std::map<std::string, int> Object::__object_map;




/**
 * Constructor
 */
Object::Object( const std::string& class_name )
 : __class_name( class_name )
{
	++__objects;
	__logger = Logger::get_instance();

#ifdef WIN32
	Object::useVerboseLog( true );
#endif

	if (__use_log) {
		int nInstances = __object_map[ __class_name ];
		++nInstances;
		__object_map[ __class_name ] = nInstances;
	}
}


/**
 * Copy constructor
 */
Object::Object( const Object& obj )
{
#ifdef WIN32
	useVerboseLog( true );
#endif
	__class_name = obj.get_class_name();

	++__objects;
//	__class_name = obj.getClassName;
	__logger = Logger::get_instance();

	if (__use_log) {
		int nInstances = __object_map[ __class_name ];
		++nInstances;
		__object_map[ __class_name ] = nInstances;
	}
}


/**
 * Destructor
 */
Object::~Object()
{
	--__objects;

	if (__use_log) {
		int nInstances = __object_map[ __class_name ];
		--nInstances;
		__object_map[ __class_name ] = nInstances;
	}
}





/**
 * Return the number of Objects not deleted
 */
int Object::get_objects_number()
{
	return __objects;
}


/*
void Object::infoLog(const std::string& sMsg)
{
	if (__use_log) {
		__logger->info(this, sMsg);
	}
}


void Object::warningLog(const std::string& sMsg)
{
	__logger->warning(this, sMsg);
}




void Object::errorLog(const std::string& sMsg)
{
	__logger->error(this, sMsg);
}
*/


void Object::use_verbose_log( bool bUse )
{
	__use_log = bUse;
	Logger::get_instance()->__use_file = bUse;
}



bool Object::is_using_verbose_log()
{
	return __use_log;
}



void Object::print_object_map()
{
	std::cout << "[Object::print_object_map]" << std::endl;

	map<std::string, int>::iterator iter = __object_map.begin();
	int nTotal = 0;
	do {
		int nInstances = (*iter).second;
		std::string sObject = (*iter).first;
		if ( nInstances != 0 ) {
			std::cout << nInstances << "\t" << sObject << std::endl;
		}
		nTotal += nInstances;
		iter++;
	} while ( iter != __object_map.end() );

	std::cout << "Total : " << nTotal << " objects." << std::endl;
}



////////////////////////


Logger* Logger::__instance = NULL;

pthread_t loggerThread;

void* loggerThread_func(void* param)
{
	if ( param == NULL ) {
		// ??????
		return NULL;
	}

#ifdef WIN32
	::AllocConsole();
//	::SetConsoleTitle( "Hydrogen debug log" );
	freopen( "CONOUT$", "wt", stdout);
#endif

	Logger *pLogger = (Logger*)param;

	FILE *pLogFile = NULL;
	if ( pLogger->__use_file ) {
#ifdef Q_OS_MACX
	  	string sLogFilename = QDir::homePath().append("/Library/Hydrogen/hydrogen.log").toStdString();
#else
		string sLogFilename = QDir::homePath().append("/.hydrogen/hydrogen.log").toStdString();
#endif

		pLogFile = fopen( sLogFilename.c_str(), "w" );
		if ( pLogFile == NULL ) {
			std::cerr << "Error: can't open log file for writing..." << std::endl;
		}
		else {
			fprintf( pLogFile, "Start logger" );
		}
	}

	while ( pLogger->__running ) {
#ifdef WIN32
		Sleep( 1000 );
#else
		usleep( 1000000 );
#endif
		pthread_mutex_lock( &pLogger->__logger_mutex );


		vector<std::string>::iterator it;
		string tmpString;
		while ( (it  = pLogger->__msg_queue.begin() ) != pLogger->__msg_queue.end() ) {
			tmpString = *it;
			pLogger->__msg_queue.erase( it );
			printf( tmpString.c_str() );

			if ( pLogFile ) {
				fprintf( pLogFile, tmpString.c_str() );
				fflush( pLogFile );
			}
		}
		pthread_mutex_unlock( &pLogger->__logger_mutex );
	}

	if ( pLogFile ) {
		fprintf( pLogFile, "Stop logger" );
		fclose( pLogFile );
	}
#ifdef WIN32
	::FreeConsole();
#endif


#ifdef WIN32
		Sleep( 1000 );
#else
		usleep( 100000 );
#endif

	pthread_exit(NULL);
	return NULL;
}


Logger* Logger::get_instance()
{
	if ( !__instance ) {
		__instance = new Logger();
	}
	return __instance;
}


/**
 * Constructor
 */
Logger::Logger()
 : __use_file( false )
 , __running( true )
{
	pthread_attr_t attr;
	pthread_attr_init(&attr);

	pthread_mutex_init( &__logger_mutex, NULL );


	pthread_create(&loggerThread, &attr, loggerThread_func, this);
}




/**
 * Destructor
 */
Logger::~Logger()
{
	__running = false;
	pthread_join(loggerThread, NULL);

}





void Logger::info_log( const std::string& logMsg )
{
	if ( !Object::is_using_verbose_log() ) return;

	pthread_mutex_lock( &__logger_mutex );

#ifdef WIN32
	__msg_queue.push_back( logMsg + "\n" );
#else
	__msg_queue.push_back( string( "\033[32m" ) + logMsg + string("\033[0m").append("\n") );
#endif

	pthread_mutex_unlock( &__logger_mutex );

}




void Logger::warning_log( const std::string& logMsg )
{
	pthread_mutex_lock( &__logger_mutex );

#ifdef WIN32
	__msg_queue.push_back( logMsg + "\n" );
#else
	__msg_queue.push_back( string( "\033[36m" ) + logMsg + string("\033[0m").append("\n") );
#endif

	pthread_mutex_unlock( &__logger_mutex );
}



void Logger::error_log( const std::string& logMsg )
{
	pthread_mutex_lock( &__logger_mutex );

#ifdef WIN32
	__msg_queue.push_back( logMsg + "\n" );
#else
	__msg_queue.push_back( string( "\033[31m" ) + logMsg + string("\033[0m").append("\n") );
#endif

	pthread_mutex_unlock( &__logger_mutex );
}




/*
#ifndef WIN32


static const WORD bgMask( BACKGROUND_BLUE | BACKGROUND_GREEN | BACKGROUND_RED | BACKGROUND_INTENSITY );


void Logger__setREDColor()
{
	CONSOLE_SCREEN_BUFFER_INFO csbi;
	HANDLE hCon = GetStdHandle( STD_OUTPUT_HANDLE );
	GetConsoleScreenBufferInfo(hCon, &csbi);

//	WORD wRGBI = GetRGBI(fgColor, FOREGROUND_INTENSITY, FOREGROUND_RED, FOREGROUND_GREEN, FOREGROUND_BLUE);
	WORD color = 10	| 255;
	csbi.wAttributes &= bgMask;
	csbi.wAttributes |= color;

	SetConsoleTextAttribute( hCon, csbi.wAttributes );

//	wRGBI = GetRGBI(bgColor, BACKGROUND_INTENSITY, BACKGROUND_RED, BACKGROUND_GREEN, BACKGROUND_BLUE);

//	csbi.wAttributes &= fgMask;
//	csbi.wAttributes |= wRGBI;

	SetConsoleTextAttribute( hCon, csbi.wAttributes );
}


#endif
*/

