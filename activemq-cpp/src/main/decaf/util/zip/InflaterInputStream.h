/*
 * Licensed to the Apache Software Foundation (ASF) under one or more
 * contributor license agreements.  See the NOTICE file distributed with
 * this work for additional information regarding copyright ownership.
 * The ASF licenses this file to You under the Apache License, Version 2.0
 * (the "License"); you may not use this file except in compliance with
 * the License.  You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#ifndef _DECAF_UTIL_ZIP_INFLATERINPUTSTREAM_H_
#define _DECAF_UTIL_ZIP_INFLATERINPUTSTREAM_H_

#include <decaf/util/Config.h>

#include <decaf/io/FilterInputStream.h>
#include <decaf/io/IOException.h>
#include <decaf/util/zip/Inflater.h>

#include <vector>

namespace decaf {
namespace util {
namespace zip {

    /**
     * A FilterInputStream that decompresses data read from the wrapped InputStream instance.
     *
     * @since 1.0
     */
    class DECAF_API InflaterInputStream : public decaf::io::FilterInputStream {
    protected:

        /**
         * The Inflater instance to use.
         */
        Inflater* inflater;

        /**
         * The buffer to hold chunks of data read from the stream before inflation.
         */
        std::vector<unsigned char> buff;

        /**
         * The amount of data currently stored in the input buffer.
         */
        int length;

        bool ownInflater;
        bool atEOF;

        static const std::size_t DEFAULT_BUFFER_SIZE;

    public:

        /**
         * Create an instance of this class with a default inflater and buffer size.
         *
         * @param inputStream
         *      The InputStream instance to wrap.
         * @param own
         *      Should this Filter take ownership of the InputStream pointer (defaults to false).
         */
        InflaterInputStream( decaf::io::InputStream* inputStream, bool own = false );

        /**
         * Creates a new InflaterInputStream with a user supplied Inflater and a default buffer size.
         * When the user supplied a Inflater instance the InflaterInputStream does not take ownership
         * of the Inflater pointer, the caller is still responsible for deleting the Inflater.
         *
         * @param inputStream
         *      The InputStream instance to wrap.
         * @param inflater
         *      The user supplied Inflater to use for decompression. (
         * @param own
         *      Should this filter take ownership of the InputStream pointer (default is false).
         *
         * @throws NullPointerException if the Inflater given is NULL.
         */
        InflaterInputStream( decaf::io::InputStream* inputStream, Inflater* inflater, bool own = false );

        /**
         * Creates a new DeflateOutputStream with a user supplied Inflater and specified buffer size.
         * When the user supplied a Inflater instance the InflaterInputStream does not take ownership
         * of the Inflater pointer, the caller is still responsible for deleting the Inflater.
         *
         * @param inputStream
         *      The InputStream instance to wrap.
         * @param inflater
         *      The user supplied Inflater to use for decompression.
         * @param bufferSize
         *      The size of the input buffer.
         * @param own
         *      Should this filter take ownership of the InputStream pointer (default is false).
         *
         * @throws NullPointerException if the Inflater given is NULL.
         * @throws IllegalArgumentException if the bufferSize value is zero.
         */
        InflaterInputStream( decaf::io::InputStream* inputStream, Inflater* inflater,
                              std::size_t bufferSize, bool own = false );

        virtual ~InflaterInputStream();

        /**
         * {@inheritDoc}
         *
         * Until EOF this method always returns 1, thereafter it always returns 0.
         */
        virtual std::size_t available() const throw ( decaf::io::IOException );

        /**
         * {@inheritDoc}
         *
         * Closes any resources associated with this InflaterInputStream.
         */
        virtual void close() throw ( decaf::io::IOException );

        /**
         * {@inheritDoc}
         *
         * Skips the specified amount of uncompressed input data.
         */
        virtual std::size_t skip( std::size_t num )
            throw ( decaf::io::IOException,
                    decaf::lang::exceptions::UnsupportedOperationException );

        /**
         * {@inheritDoc}
         *
         * Does nothing.
         */
        virtual void mark( int readLimit );

        /**
         * {@inheritDoc}
         *
         * Always throws an IOException when called.
         */
        virtual void reset() throw ( decaf::io::IOException );

        /**
         * {@inheritDoc}
         *
         * Always returns false.
         */
        virtual bool markSupported() const;

    protected:

        /**
         * Fills the input buffer with the next chunk of data.
         *
         * @throws IOException if an I/O error occurs.
         */
        virtual void fill() throw( decaf::io::IOException );

    protected:

        virtual int doReadByte() throw ( decaf::io::IOException );

        virtual int doReadArrayBounded( unsigned char* buffer, std::size_t size,
                                        std::size_t offset, std::size_t length )
            throw ( decaf::io::IOException,
                    decaf::lang::exceptions::IndexOutOfBoundsException,
                    decaf::lang::exceptions::NullPointerException );

    };

}}}

#endif /* _DECAF_UTIL_ZIP_INFLATERINPUTSTREAM_H_ */