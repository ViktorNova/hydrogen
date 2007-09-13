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

#ifndef PATTERN_H
#define PATTERN_H

#include <hydrogen/globals.h>
#include <hydrogen/Object.h>

namespace H2Core
{

class Note;

///
/// The Pattern is a Note container.
///
class Pattern : public Object
{
public:
	std::multimap <int, Note*> note_map;

	Pattern( const std::string& name, unsigned lenght = MAX_NOTES );
	~Pattern();

	static Pattern* get_empty_pattern();
	Pattern* copy();

	void dump();

	unsigned get_lenght() {	return __lenght;	}
	void set_lenght(unsigned lenght) {	__lenght = lenght;	}

	void set_name(const std::string& name) { __name = name;	}
	const std::string& get_name() const {	return __name;	}

private:
	unsigned __lenght;
	std::string __name;
};


/// Pattern List
class PatternList : public Object
{
public:
	PatternList();
	~PatternList();

	void add( Pattern* newPattern );
	Pattern* get( int nPos );
	unsigned int getSize();
	void clear();

	void replace( Pattern* newPattern, unsigned nPos );
	int indexOf( Pattern * );

	Pattern * del( Pattern *pattern ); // returns NULL if the pattern is not in the list :)
	void del( unsigned index );

private:
	std::vector<Pattern*> list;
};

};

#endif

