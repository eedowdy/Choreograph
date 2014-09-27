/*
 * Copyright (c) 2014 David Wicks, sansumbrella.com
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or
 * without modification, are permitted provided that the following
 * conditions are met:
 *
 * Redistributions of source code must retain the above copyright
 * notice, this list of conditions and the following disclaimer.
 * Redistributions in binary form must reproduce the above copyright
 * notice, this list of conditions and the following disclaimer in the
 * documentation and/or other materials provided with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 * "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
 * A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
 * HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
 * SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
 * LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
 * DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
 * THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#pragma once

namespace choreograph
{

template<typename T>
class Connection;

///
/// Safe type for Choreograph outputs.
/// Disconnects applied Motion on destruction so you don't accidentally modify stale pointers.
///
template<typename T>
class Output
{
public:
  /// Disconnect on destruction.
  ~Output() { disconnect(); }

  /// Default constructor.
  Output() = default;

  /// Construct with a value.
  Output( const T &value ):
    mValue( value )
  {}

  /// Disconnect from Motion input.
  void disconnect();

  /// Returns true iff this output has a connected input.
  bool isConnected() const { return _input != nullptr; }

  /// Copy assignment takes value and any input Motion.
  /// May remove copy assignment since it has non-obvious semantics.
  Output<T>& operator= ( const Output<T> &rhs );
//  Output<T>& operator= ( const Output<T> &rhs ) = delete;

  /// Move assignment takes value and any input Motion.
  Output<T>& operator= ( Output<T> &&rhs );

  /// Copy constructor takes value and any motions.
  Output( const Output<T> &rhs );

  /// Move constructor takes value and any input Motion.
  Output( Output<T> &&rhs );

  /// Assignment operator.
	Output<T>& operator= ( T value ) { mValue = value; return *this; }

  Output<T>& operator+= ( T value ) { mValue += value; return *this; }

	/// Returns value of output.
	const T& 	value() const { return mValue; }

	/// Returns value of output.
	const T& 	operator() () const { return mValue; }

	/// Returns value of output for manipulating.
	T&				operator() () { return mValue; }

	/// Enable cast to value type.
	operator const T&()	{ return mValue; }

	/// Returns pointer to value.
	const T* 	valuePtr() const { return &mValue; }

	/// Returns pointer to value.
	T*				valuePtr() { return &mValue; }

private:
	T               mValue;
  Connection<T>  *_input = nullptr;

  /// Replaces \a rhs in its relationship to a MotionBase input.
  void supplant( const Output<T> &rhs );
  /// Connects this output to a new Connection.
  void connect( Connection<T> *input );

  friend class    Connection<T>;
};

//=================================================
// Output Implementation
//=================================================

/// Copy assignment takes value and any input Motion.
template<typename T>
Output<T>& Output<T>::operator= ( const Output<T> &rhs ) {
  if( this != &rhs ) {
    mValue = rhs.mValue;
    supplant( rhs );
  }
  return *this;
}

/// Move assignment takes value and any input Motion.
template<typename T>
Output<T>& Output<T>::operator= ( Output<T> &&rhs ) {
  if( this != &rhs ) {
    mValue = rhs.mValue;
    supplant( rhs );
  }
  return *this;
}

/// Copy constructor takes value and any motions.
template<typename T>
Output<T>::Output( const Output<T> &rhs ):
mValue( rhs.mValue )
{
  supplant( rhs );
}

/// Move constructor takes value and any input Motion.
template<typename T>
Output<T>::Output( Output<T> &&rhs ):
mValue( rhs.mValue )
{
  supplant( rhs );
}

template<typename T>
void Output<T>::disconnect()
{
  if( _input ) {
    _input->disconnect( this );
  }
}

template<typename T>
void Output<T>::supplant( const Output<T> &rhs )
{
  disconnect();
  connect( rhs._input );
}

template<typename T>
void Output<T>::connect( Connection<T> *input )
{
  if( input ) {
    input->connect( this );
  }
}

} // namespace choreograph