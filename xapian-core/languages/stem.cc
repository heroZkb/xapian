/** @file stem.cc
 *  @brief Implementation of Xapian::Stem API class.
 */
/* Copyright (C) 2007 Olly Betts
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License as
 * published by the Free Software Foundation; either version 2 of the
 * License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301 USA
 */

#include <config.h>

#include <xapian/error.h>
#include <xapian/stem.h>

#include "steminternal.h"

#include "allsnowballheaders.h"

#include <string>

using namespace std;

namespace Xapian {

StemBase::StemBase()
{
}

StemBase::~StemBase()
{
}


StemSnowball::StemSnowball(const std::string &language)
	: internal(0)
{
    if (language.empty()) return;
    switch (language[0]) {
	case 'd':
	    if (language == "da" || language == "danish") {
		internal = new InternalStemDanish;
		return;
	    }
	    if (language == "dutch") {
		internal = new InternalStemDutch;
		return;
	    }
	    if (language == "de") {
		internal = new InternalStemGerman;
		return;
	    }
	    break;
	case 'e':
	    if (language == "en" || language == "english") {
		internal = new InternalStemEnglish;
		return;
	    }
	    if (language == "es") {
		internal = new InternalStemSpanish;
		return;
	    }
	    break;
	case 'f':
	    if (language == "fi" || language == "finnish") {
		internal = new InternalStemFinnish;
		return;
	    }
	    if (language == "fr" || language == "french") {
		internal = new InternalStemFrench;
		return;
	    }
	    break;
	case 'g':
	    if (language == "german") {
		internal = new InternalStemGerman;
		return;
	    }
	    if (language == "german2") {
		internal = new InternalStemGerman2;
		return;
	    }
	    break;
	case 'h':
	    if (language == "hu" || language == "hungarian") {
		internal = new InternalStemHungarian;
		return;
	    }
	    break;
	case 'i':
	    if (language == "it" || language == "italian") {
		internal = new InternalStemItalian;
		return;
	    }
	    break;
	case 'k':
	    if (language == "kraaij_pohlmann") {
		internal = new InternalStemKraaij_pohlmann;
		return;
	    }
	    break;
	case 'l':
	    if (language == "lovins") {
		internal = new InternalStemLovins;
		return;
	    }
	    break;
	case 'n':
	    if (language == "no" || language == "norwegian") {
		internal = new InternalStemNorwegian;
		return;
	    }
	    break;
	case 'p':
	    if (language == "pt" || language == "portuguese") {
		internal = new InternalStemPortuguese;
		return;
	    }
	    if (language == "porter") {
		internal = new InternalStemPorter;
		return;
	    }
	    break;
	case 'r':
	    if (language == "ru" || language == "russian") {
		internal = new InternalStemRussian;
		return;
	    }
	    if (language == "ro" || language == "romanian") {
		internal = new InternalStemRomanian;
		return;
	    }
	    break;
	case 's':
	    if (language == "spanish") {
		internal = new InternalStemSpanish;
		return;
	    }
	    if (language == "sv" || language == "swedish") {
		internal = new InternalStemSwedish;
		return;
	    }
	    break;
	case 't':
	    if (language == "tr" || language == "turkish") {
		internal = new InternalStemTurkish;
		return;
	    }
	    break;
    }
    throw Xapian::InvalidArgumentError("Language code " + language + " unknown");
}

StemSnowball::~StemSnowball()
{
    delete internal;
}

string
StemSnowball::operator()(const std::string &word) const
{
    if (word.empty()) return word;
    return internal->operator()(word);
}

string
StemSnowball::get_description() const
{
    string desc = "Xapian::StemSnowball(\"";
    if (internal) {
	desc += internal->get_description();
	desc += "\")";
    } else {
	desc += "none)";
    }
    return desc;
}

string
StemSnowball::get_available_languages()
{
    return LANGSTRING;
}



StemWithNoStemList::StemWithNoStemList(
    const Xapian::Internal::RefCntPtr<Xapian::StemBase> & stemmer_)
{
    init_stemmer(stemmer_);
}

StemWithNoStemList::StemWithNoStemList(const Stem & stemmer_)
{
    init_stemmer(stemmer_);
}

StemWithNoStemList::~StemWithNoStemList()
{
}

void
StemWithNoStemList::init_stemmer(
    const Xapian::Internal::RefCntPtr<Xapian::StemBase> & stemmer_)
{
    stemmer = stemmer_;
    if (!stemmer.get())
	throw Xapian::InvalidArgumentError("NULL pointer was supplied to StemWithNoStemList");
}

void
StemWithNoStemList::init_stemmer(const Stem & stemmer_)
{
    stemmer = stemmer_.internal;
    if (!stemmer.get())
	throw Xapian::InvalidArgumentError("NULL pointer was supplied to StemWithNoStemList");
}

string
StemWithNoStemList::operator()(const std::string &word) const
{
    if (word.empty()) return word;
    if (nostemwords.find(word) != nostemwords.end()) return word;
    return stemmer->operator()(word);
}

string
StemWithNoStemList::get_description() const
{
    string desc = "Xapian::StemWithNoStemList(\"";
    desc += stemmer->get_description();
    desc += "\",[";
    int count;
    std::set<std::string>::const_iterator i;
    for (i = nostemwords.begin(), count = 3; i != nostemwords.end();
	 ++i, ++count) {
	desc += *i;
	desc += ",";
    }
    desc += "])";
    return desc;
}

}
