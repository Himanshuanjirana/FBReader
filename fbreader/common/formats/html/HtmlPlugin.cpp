/*
 * FBReader -- electronic book reader
 * Copyright (C) 2005 Nikolay Pultsin <geometer@mawhrin.net>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
 */

#include <abstract/ZLStringUtil.h>
#include <abstract/ZLInputStream.h>

#include "HtmlPlugin.h"
#include "HtmlDescriptionReader.h"
#include "HtmlBookReader.h"
#include "../EncodingDetector.h"
#include "../../description/BookDescription.h"

bool HtmlPlugin::acceptsFile(const std::string &fileName) const {
	return ZLStringUtil::stringEndsWith(fileName, ".html");
}

bool HtmlPlugin::readDescription(const std::string &fileName, BookDescription &description) const {
	ZLInputStream *stream = ZLInputStream::createStream(fileName);
	std::string encoding = description.encoding();
	if (encoding.empty()) {
		encoding = EncodingDetector::detect(*stream);
		if (encoding.empty()) {
			delete stream;
			return false;
		}
		WritableBookDescription(description).encoding() = encoding;
	}

	HtmlDescriptionReader *reader = new HtmlDescriptionReader(description);
	reader->readDocument(*stream, encoding);
	delete reader;

	delete stream;
	return true;
}

bool HtmlPlugin::readModel(const BookDescription &description, BookModel &model) const {
	HtmlBookReader *reader = new HtmlBookReader(model);
	ZLInputStream *stream = ZLInputStream::createStream(description.fileName());
	reader->readDocument(*stream, description.encoding());
	delete stream;
	delete reader;
	return true;
}
