/* eos - A reimplementation of BioWare's Aurora engine
 * Copyright (c) 2010 Sven Hesse (DrMcCoy), Matthew Hoops (clone2727)
 *
 * The Infinity, Aurora, Odyssey and Eclipse engines, Copyright (c) BioWare corp.
 * The Electron engine, Copyright (c) Obsidian Entertainment and BioWare corp.
 *
 * This file is part of eos and is distributed under the terms of
 * the GNU General Public Licence. See COPYING for more informations.
 */

/** @file graphics/text.cpp
 *  A text object.
 */

#include "events/requests.h"

#include "graphics/text.h"
#include "graphics/font.h"

namespace Graphics {

Text::Text(const Font &font, double x, double y, const std::string &str) :
	_firstTime(true), _font(&font), _x(x), _y(y), _str(str) {

	addToQueue();
}

Text::~Text() {
}

void Text::newFrame() {
}

void Text::render() {
	glTranslated(_x, _y - _font->getScale(), -1.0);
	glScalef(_font->getScale(), _font->getScale(), _font->getScale());

	if (_firstTime) {
		// Sync, to make sure that the texture has finished loading
		RequestMan.sync();
		_firstTime = false;
	}

	_font->setTexture();

	const char *str = _str.c_str();
	while (*str) {
		double w = _font->drawCharacter(*str++);
		glTranslatef(w + _font->getSpaceR(), 0.0, 0.0);
	}
}

} // End of namespace Graphics