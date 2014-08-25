/*
 * %injeqt copyright begin%
 * Copyright 2014 Rafał Malinowski (rafal.przemyslaw.malinowski@gmail.com)
 * %injeqt copyright end%
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA
 */

/** @file */

#include "type.h"

#include <QtCore/QMetaObject>

namespace injeqt { namespace v1 {

type::type(const QMetaObject *meta_object) :
	_meta_object{meta_object}
{
}

std::string type::name() const
{
	return _meta_object->className();
}

const QMetaObject * type::meta_object() const
{
	return _meta_object;
}

/**
 * @brief Throw an exception if type t is not valid.
 * @param t type to validate
 * @throws invalid_type_exception
 *
 * Call to validate type t. If t does not represent a QObject-derived type, an
 * exception of type invalid_type_exception is thrown. If t is valid, this
 * function returns.
 */
void validate(const type &t)
{
	if (!t.meta_object())
		throw invalid_type_exception{};
	if (!t.meta_object()->superClass())
		throw invalid_type_exception{};
}

bool operator == (const type &x, const type &y)
{
	return x.meta_object() == y.meta_object();
}

bool operator != (const type &x, const type &y)
{
	return !(x == y);
}

bool operator < (const type &x, const type &y)
{
	return x.meta_object() < y.meta_object();
}

bool operator > (const type &x, const type &y)
{
	return y < x;
}

bool operator <= (const type &x, const type &y)
{
	return !(y < x);
}

bool operator >= (const type &x, const type &y)
{
	return !(x < y);
}

}}
