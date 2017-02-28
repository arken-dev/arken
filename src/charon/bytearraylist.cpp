// Copyright 2016 The Charon Platform Authors.
// All rights reserved.
// Use of this source code is governed by a BSD-style
// license that can be found in the LICENSE file.

#include <charon/base>
#include <iostream>
#include <cstring>

void ByteArrayList::init()
{
  m_cursor = 0;

  if( m_size == m_resource ) {
    m_resource *= 2;
  }

  QByteArray * *array = new QByteArray*[m_resource];

  if( m_size > 0 ) {
    for(int i = 0; i < m_size; i++) {
      array[i] = m_array[i];
    }
  }

  if( m_array != 0 ) {
    delete m_array;
  }

  m_array = array;

  for(int i = m_size; i < m_resource; i++) {
    m_array[i] = 0;
  }
}

ByteArrayList::ByteArrayList()
{

  m_array = 0;
  m_size  = 0;
  m_resource = 10;
  init();
}

ByteArrayList::ByteArrayList(int resource)
{
  m_array = 0;
  m_size  = 0;
  m_resource = resource;
  init();
}

ByteArrayList::~ByteArrayList()
{

  for(int i = 0; i < m_size; i++) {
    if( m_array[i] != 0 ) {
      delete m_array[i];
    }
  }

  delete[] m_array;
}

void ByteArrayList::replace(int pos, const char * value)
{
  if( m_array[pos] != 0 ) {
    delete m_array[pos];
  }

  //char * tmp = new char[strlen(value)+1];
  //strcpy(tmp, value);
  m_array[pos] = new QByteArray(value);//tmp;
}

ByteArrayList & ByteArrayList::append(const char * value)
{
  if( m_size == m_resource ) {
    init();
  }
  //char * tmp = new char[strlen(value)+1];
  //strcpy(tmp, value);
  m_array[m_size] = new QByteArray(value);//tmp;
  ++m_size;
  return *this;
}

ByteArrayList & ByteArrayList::append(const char * value, int len)
{
  if( m_size == m_resource ) {
    init();
  }
  //char * tmp = new char[len+1];
  //strncpy(tmp, value, len);
  //tmp[len] = '\0';
  m_array[m_size] = new QByteArray(value, len);//tmp;
  ++m_size;
  return *this;
}

ByteArrayList & ByteArrayList::operator<<(const char * value)
{
  if( m_size == m_resource ) {
    init();
  }
  //char * tmp = new char[strlen(value)+1];
  //strcpy(tmp, value);
  m_array[m_size] = new QByteArray(value);//tmp;
  ++m_size;
  return *this;
}

const char * ByteArrayList::operator[](int pos)
{
  return at(pos); //m_array[pos]->data();
}

const char * ByteArrayList::at(int pos)
{
  if( pos > m_size ) {
    return 0;
  }
  QByteArray * ba = m_array[pos];
  if( ba == 0 || ba->size() == 0 ) {
    return 0;
  } else {
    return m_array[pos]->data();
  }
}

const char * ByteArrayList::at(int pos, int * len)
{
  if( pos > m_size ) {
    * len = 0;
    return 0;
  }
  QByteArray * ba = m_array[pos];
  if( ba == 0 || ba->size() == 0 ) {
    * len = 0;
    return 0;
  } else {
    *len = ba->size();
    return ba->data();
  }
}

const char * ByteArrayList::first()
{
  return at(0);// m_array[0]->data();
}

const char * ByteArrayList::last()
{
  return at(m_size-1);//m_array[m_size-1]->data();
}

int ByteArrayList::size()
{
  return m_size;
}

char * ByteArrayList::join(const char * separator)
{
  char * result;
  int size = 0;
  int tmp_size = 0;
  int result_size = 0;
  int size_separator = strlen(separator);

  for( int i = 0; i < m_size; i++ ) {
    size += m_array[i]->size();
    size += size_separator;
  }

  result = new char[size + 1];
   for( int i = 0; i < m_size; i++ ) {
     tmp_size = m_array[i]->size();
     for( int j = 0; j < tmp_size; j++ ) {
       result[result_size] = m_array[i]->data()[j];
       result_size ++;
     }
     if( i < m_size -1 ) {
       for( int h = 0; h < size_separator; h++ ) {
         result[result_size] = separator[h];
         result_size ++;
       }
     }
  }
  result[result_size] = '\0';

  return result;
}

const char * ByteArrayList::each()
{
  const char * result = at(m_cursor);
  m_cursor++;
  if( m_cursor > m_size ) {
    m_cursor = 0;
  }
  return result;
}
