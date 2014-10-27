/**
 * StringWriterUtils.java
 * Chilli Source
 * Created by Ian Copland on 27/10/2014
 * 
 * The MIT License (MIT)
 * 
 * Copyright (c) 2014 Tag Games Limited
 * 
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 * 
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 * 
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
 */

package com.chilliworks.chillisource.toolutils;

import java.io.IOException;
import java.io.OutputStream;

/**
 * A collection of convenience methods for writing strings to an output stream.
 * 
 * @author Ian Copland
 */
public final class StringWriterUtils
{
	/**
	 * Writes a UTF8 string to the given output stream. This will not be
	 * null terminated.
	 * 
	 * @param in_stream - The stream to write the string to.
	 * @param String - The string value.
	 * 
	 * @throws IOException - Any IO Exception thrown by the given stream.
	 */
	public static void writeUTF8String(OutputStream in_stream, String in_value) throws IOException
	{
		in_stream.write(StringUtils.stringToUTF8Bytes(in_value));
	}
	/**
	 * Writes a null terminated UTF8 string to the given output stream.
	 * 
	 * @param in_stream - The stream to write the string to.
	 * @param String - The string value.
	 * 
	 * @throws IOException - Any IO Exception thrown by the given stream.
	 */
	public static void writeUTF8StringNullTerminated(OutputStream in_stream, String in_value) throws IOException
	{
		writeUTF8String(in_stream, in_value);
		
		byte[] nullTerminator = new byte[1];
		nullTerminator[0] = 0;
		in_stream.write(nullTerminator);
	}
}
