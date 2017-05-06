// -*- mode: c++ -*-
/***************************************************************************************************
 *
 * $QTCARTO_BEGIN_LICENSE:GPL3$
 *
 * Copyright (C) 2016 Fabrice Salvaire
 * Contact: http://www.fabrice-salvaire.fr
 *
 * This file is part of the QtCarto library.
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 * $QTCARTO_END_LICENSE$
 *
 **************************************************************************************************/

/**************************************************************************************************/

#ifndef __DATABASE_ROW_H__
#define __DATABASE_ROW_H__

/**************************************************************************************************/

#include <QBitArray>
#include <QJsonObject>
#include <QObject>
#include <QSqlQuery>
#include <QSqlRecord>
#include <QVariantHash>
#include <QVariantList>

/**************************************************************************************************/

class QcDatabaseSchema;

/**************************************************************************************************/

// Trait class to be used as generic row (QcRowTraits *) since QcRow is a template class

class QcRowTraits
{
public:
  QcRowTraits() {};
  // ~QcRowTraits();

  // Define row API

  virtual int schema_id() const = 0;

  // JSON Serializer
  virtual QJsonObject to_json(bool only_changed = false) const = 0;

  // Generic Variant Serializer
  virtual QVariantHash to_variant_hash(bool only_changed = false) const = 0;
  virtual QVariantList to_variant_list() const = 0;

  // SQL Serializer
  virtual QVariantHash to_variant_hash_sql(bool only_changed = false) const = 0;
  virtual QVariantList to_variant_list_sql() const = 0;

  // Query for update
  // virtual bool is_modified() const = 0;

  // Field accessor by position
  virtual QVariant field(int position) const = 0;
  virtual void set_field(int position, const QVariant & value) = 0;

  // Fixme: implement has mixin ?
  QcDatabaseSchema * database_schema() const { return m_database_schema; }
  void set_database_schema(QcDatabaseSchema * database_schema)  { m_database_schema = database_schema; }
  virtual bool exists_on_database() const {
    return m_database_schema != nullptr; // Fixme: right ??? commited !
  }

  virtual void load_foreign_keys() {}

  virtual void set_insert_id(int id) {}; // To set id field when the row was inserted

private:
  QcDatabaseSchema * m_database_schema = nullptr; // use memory !
};

/**************************************************************************************************/

template<class S>
class QcRow : public QcRowTraits
{
public:
  typedef S Schema;

  static Schema & schema() { return Schema::instance(); }
  int schema_id() const { return schema().schema_id(); }
  // static int number_of_fields() { return schema().number_of_fields(); }
  static int number_of_fields() { return Schema::NUMBER_OF_FIELDS; } // Fixme: faster ?

public:
  QcRow();
  QcRow(const QcRow & other);
  QcRow(const QJsonObject & json_object) : QcRow() {} // JSON deserializer
  QcRow(const QVariantHash & variant_hash) : QcRow() {}
  QcRow(const QVariantList & variants) : QcRow() {}
  QcRow(const QSqlRecord & record) : QcRow() {} // SQL deserializer
  QcRow(const QSqlQuery & query) : QcRow() {} // SQL deserializer
  ~QcRow() {}

  QcRow & operator=(const QcRow & other) { return *this; } // Fixme: m_bits ?

  bool operator==(const QcRow & other) { return true; } // Fixme: m_bits ?

  bool is_modified() const { return not m_bits.isNull(); }

protected:
  bool bit_status(int i) const { return m_bits[i]; }
  void set_bit(int i) { return m_bits.setBit(i); }

private:
  QBitArray m_bits;
};

/**************************************************************************************************/

template<class S>
class QcRowWithId : public QcRow<S>
{
public:
  using QcRow<S>::QcRow;
  ~QcRowWithId() {}

  void set_insert_id(int id) { set_id(id); }

  virtual int id() const = 0;
  virtual void set_id(int value) = 0;
};

/**************************************************************************************************/

/*
template<class S>
class QcRowWithId : public QcRow<S>
{
public:
  const int INVALID_ID = -1;

public:
  QcRowWithId();
  QcRowWithId(const QcRowWithId & other);
  QcRowWithId(const QJsonObject & json_object); // JSON deserializer
  QcRowWithId(const QVariantHash & variant_hash);
  QcRowWithId(const QVariantList & variants); // , bool with_id = false
  QcRowWithId(const QSqlRecord & record); // SQL deserializer
  QcRowWithId(const QSqlQuery & query); // SQL deserializer
  ~QcRowWithId();

  QcRowWithId & operator=(const QcRowWithId & other);

  bool operator==(const QcRowWithId & other);

  int id() const { return m_id; }
  void set_id(int value) { m_id = value; }
  bool exists_on_database() const { m_id != INVALID_ID; }
  void detach();

private:
  int m_id = INVALID_ID;
};
*/

/**************************************************************************************************/

#ifndef QC_MANUAL_INSTANTIATION
#include "database_row.hxx"
#endif

/**************************************************************************************************/

#endif /* __DATABASE_ROW_H__ */

/***************************************************************************************************
 *
 * End
 *
 **************************************************************************************************/
