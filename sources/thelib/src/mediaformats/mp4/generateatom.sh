CLASSNAME="Atom`echo $1|tr '[:lower:]' '[:upper:]'`"
FILENAME="`echo $CLASSNAME|tr '[:upper:]' '[:lower:]'`.cpp"
HEADER="`echo $CLASSNAME|tr '[:upper:]' '[:lower:]'`.h"

echo "/*" >$FILENAME
echo " * Copyright (c) 2009, Gavriloaie Eugen-Andrei (shiretu@gmail.com)" >>$FILENAME
echo " * All rights reserved." >>$FILENAME
echo " *" >>$FILENAME
echo " * Redistribution and use in source and binary forms, with or without" >>$FILENAME
echo " * modification, are permitted provided that the following conditions are met:" >>$FILENAME
echo " *" >>$FILENAME
echo " *		*Redistributions of source code must retain the above copyright notice," >>$FILENAME
echo " *		 this list of conditions and the following disclaimer." >>$FILENAME
echo " *		*Redistributions in binary form must reproduce the above copyright" >>$FILENAME
echo " *		 notice, this list of conditions and the following disclaimer in the" >>$FILENAME
echo " *		 documentation and/or other materials provided with the distribution." >>$FILENAME
echo " *		*Neither the name of the DEVSS nor the names of its" >>$FILENAME
echo " *		 contributors may be used to endorse or promote products derived from" >>$FILENAME
echo " *		 this software without specific prior written permission." >>$FILENAME
echo " *" >>$FILENAME
echo " * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS \"AS IS\"" >>$FILENAME
echo " * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE" >>$FILENAME
echo " * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE" >>$FILENAME
echo " * ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE" >>$FILENAME
echo " * LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR" >>$FILENAME
echo " * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF" >>$FILENAME
echo " * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS" >>$FILENAME
echo " * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN" >>$FILENAME
echo " * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)" >>$FILENAME
echo " * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE" >>$FILENAME
echo " * POSSIBILITY OF SUCH DAMAGE." >>$FILENAME
echo " */" >>$FILENAME
echo "" >>$FILENAME
echo "#include \"mediaformats/mp4/$HEADER\"" >>$FILENAME
echo "" >>$FILENAME
echo "" >>$FILENAME
echo "$CLASSNAME::$CLASSNAME(MP4Document *pDocument, uint32_t type, uint32_t size, uint32_t start)" >>$FILENAME
echo ": BaseAtom(pDocument, type, size, start) {" >>$FILENAME
echo "" >>$FILENAME
echo "}" >>$FILENAME
echo "" >>$FILENAME
echo "$CLASSNAME::~$CLASSNAME() {" >>$FILENAME
echo "}" >>$FILENAME
echo "" >>$FILENAME
echo "bool $CLASSNAME::Read() {" >>$FILENAME
echo "    return SkipRead();" >>$FILENAME
echo "}" >>$FILENAME
echo "" >>$FILENAME
echo "std::string $CLASSNAME::Hierarchy(uint32_t indent) {" >>$FILENAME
echo "    return std::string(4 * indent, ' ') + GetTypeString();" >>$FILENAME
echo "}" >>$FILENAME
echo "" >>$FILENAME




