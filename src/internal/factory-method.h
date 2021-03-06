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

#pragma once

#include <injeqt/exception/exception.h>
#include <injeqt/injeqt.h>
#include <injeqt/type.h>

#include "internal.h"
#include "types-by-name.h"

#include <memory>
#include <QtCore/QMetaMethod>

/**
 * @file
 * @brief Contains classes and functions for representing factory methods.
 */

class QObject;

namespace injeqt { namespace internal {

/**
 * @brief Abstraction of factory method.
 *
 * This class is used for creating objects of types configured with module::add_factory<T, F>().
 * Factory method is created from Qt type QMetaMethod. As Qt only creates QMetaMethod
 * objects for slots, signals and all methods marked with Q_INVOKABLE tag - Injeqt
 * requires that factory method is marked with that tag or is a slot. This method must return
 * pointer of type inherited from QObject and is not allowed only to have parameters with
 * default values.
 *
 * Example of valid factory method:
 *
 *     class created_object : public QObject
 *     {
 *         Q_OBJECT
 *     };
 *
 *     class with_factory_method : public QObject
 *     {
 *         Q_OBJECT
 *     public:
 *         Q_INVOKABLE created_object * create() { ... }
 *     };
 *
 * To extract constructor from a type call make_factory_method<T, F>().
 * For above example a call to make_factory_method<with_factory_method, created_object>()
 * would return valid object.
 *
 * Note that in Injeqt factory should not own created object - invoking
 * injector own lifetime of it. Method invoke(QObject *) call factory method
 * and immediately wraps returned QObject * in a std::unique_ptr that it later
 * returns.
 */
class INJEQT_INTERNAL_API factory_method final
{

public:
	/**
	 * @brief Create empty factory_method.
	 */
	factory_method();

	/**
	 * @brief Create object from QMetaMethod definition.
	 * @param parameter_type Type of retrun value of @p meta_method
	 * @param meta_method Qt meta method that should be a factory method
	 * @note Qt QMetaType system limitations with plugins disallow use of QMetaType to retreive return type from QMetaMethod
	 * @pre meta_method.methodType() == QMetaMethod::Method || meta_method.methodType() == QMetaMethod::Slot
	 * @pre meta_method.parameterCount() == 0
	 * @pre meta_method.enclosingMetaObject() != nullptr
	 * @pre !result_type.is_empty()
	 * @pre result_type.name() + "*" == std::string{result_type.typeName()}
	 */
	explicit factory_method(type result_type, QMetaMethod meta_method);

	/**
	 * @return true if factory_method is empty and does not represent method
	 */
	bool is_empty() const;

	/**
	 * @return Type of objects that owns this factory method.
	 */
	const type & object_type() const;

	/**
	 * @return Type of objects created by factory method.
	 */
	const type & result_type() const;

	/**
	 * @return Qt representation of factory method.
	 *
	 * May return empty value if is_empty()
	 */
	const QMetaMethod & meta_method() const;

	/**
	 * @param on object to call this method on
	 * @return Result on factory method called on given object.
	 * @pre !is_empty()
	 * @pre on != nullptr
	 * @pre meta_method().enclosingMetaObject() is equal to @p on type
	 *
	 * This method can be only called on valid objects with @p on parameter being
	 * the same type as object_type() returns. Invalid invocation with result
	 * in undefined behavior.
	 */
	std::unique_ptr<QObject> invoke(QObject *on) const;

private:
	type _object_type;
	type _result_type;
	QMetaMethod _meta_method;

};

INJEQT_INTERNAL_API bool operator == (const factory_method &x, const factory_method &y);
INJEQT_INTERNAL_API bool operator != (const factory_method &x, const factory_method &y);

/**
 * @brief Extract factory method from given type F that returns pointers to T.
 * @tparam t factory method must return T *
 * @tparam f type to extract constructor from
 * @pre !t.is_empty() && !t.is_qobject()
 * @pre !f.is_empty() && !f.is_qobject()
 *
 * This function looks for all methods of type F that are tagged with Q_INVOKABLE, does not
 * accepts argumetns and returns T* or pointer to type derived from T. If only one such method
 * is found it is wrapped in factory_method type and returned. In other cases an empty
 * factory_method is returned.
 */
INJEQT_INTERNAL_API factory_method make_factory_method(const types_by_name &known_types, const type &t, const type &f);

}}
