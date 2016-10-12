#include <oberon/classes/OStringList.h>
#include <iostream>
#include <cstring>

void OStringList::init()
{
  if( m_size == m_resource ) {
    m_resource *= 2;
  }

  const char * *array = new const char*[m_resource];

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

OStringList::OStringList()
{

  m_array = 0;
  m_size  = 0;
  m_resource = 10;
  init();
}

OStringList::OStringList(int resource)
{
  m_array = 0;
  m_size  = 0;
  m_resource = resource;
  init();
}

OStringList::~OStringList()
{
  for(int i = 0; i < m_size; i++) {
    if( m_array[i] != 0 ) {
      delete[] m_array[i];
    }
  }
  delete[] m_array;
}

void OStringList::replace(int pos, const char * value)
{
  if( m_array[pos] != 0 ) {
    delete[] m_array[pos];
  }

  char * tmp = new char[strlen(value)+1];
  strcpy(tmp, value);
  m_array[pos] = tmp;
}

OStringList & OStringList::append(const char * value)
{
  if( m_size == m_resource ) {
    init();
  }
  char * tmp = new char[strlen(value)+1];
  strcpy(tmp, value);
  m_array[m_size] = tmp;
  ++m_size;
  return *this;
}

OStringList & OStringList::operator<<(const char * value)
{
  if( m_size == m_resource ) {
    init();
  }
  char * tmp = new char[strlen(value)+1];
  strcpy(tmp, value);
  m_array[m_size] = tmp;
  ++m_size;
  return *this;
}

const char * OStringList::operator[](int pos)
{
  return m_array[pos];
}

const char * OStringList::at(int pos)
{
  return m_array[pos];
}

int OStringList::size()
{
  return m_size;
}

char * OStringList::join(const char * separator)
{
  char * result;
  int size = 0;
  int tmp_size = 0;
  int result_size = 0;
  int size_separator = strlen(separator);

  for( int i = 0; i < m_size; i++ ) {
    size += strlen(m_array[i]);
    size += size_separator;
  }

  result = new char[size + 1];
   for( int i = 0; i < m_size; i++ ) {
     tmp_size = strlen(m_array[i]);
     for( int j = 0; j < tmp_size; j++ ) {
       result[result_size] = m_array[i][j];
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
