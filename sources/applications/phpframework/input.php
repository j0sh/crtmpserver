<?php
/*
 * Copyright (c) 2009, Gavriloaie Eugen-Andrei (shiretu@gmail.com)
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 *              *Redistributions of source code must retain the above copyright notice,
 *               this list of conditions and the following disclaimer.
 *              *Redistributions in binary form must reproduce the above copyright
 *               notice, this list of conditions and the following disclaimer in the
 *               documentation and/or other materials provided with the distribution.
 *              *Neither the name of the DEVSS nor the names of its
 *               contributors may be used to endorse or promote products derived from
 *               this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE
 * LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
*/

require 'TestFactory.php';
require 'variant/CVariantSerializer.php';

$testInput1='
<MAP name="" isArray="false">
    <typed_map name="typed_map_instance1" isArray="false" typename="MyClass1">
        <STR name="fooString">bar</STR>
        <INT8 name="barNumber">123</INT8>
    </typed_map>
    <typed_map name="typed_map_instance2" isArray="false" typename="MyClass2">
        <STR name="barString">bar</STR>
        <INT8 name="fooNumber">123</INT8>
    </typed_map>
    <typed_map name="typed_map_instance3" isArray="false" typename="MyClass3">
        <STR name="fooString">bar</STR>
        <INT8 name="barNumber">123</INT8>
        <typed_map name="myClass2" isArray="false" typename="MyClass2">
            <STR name="barString">bar</STR>
            <INT8 name="fooNumber">123</INT8>
        </typed_map>
    </typed_map>
    <MAP name="testArray" isArray="false">
        <STR name="__index__value__0">string 0</STR>
        <STR name="__index__value__1">string 1</STR>
        <STR name="__index__value__2">string 2</STR>
        <STR name="__index__value__3">string 3</STR>
        <STR name="__index__value__4">string 4</STR>
        <STR name="gigi">stiintza</STR>
    </MAP>
    <MAP name="header" isArray="false"> 
        <UINT32 name="channelId">3</UINT32> 
        <UINT8 name="headerType">3</UINT8> 
        <BOOL name="isAbsolute">false</BOOL> 
        <UINT32 name="messageLength">242</UINT32> 
        <UINT8 name="messageType">20</UINT8> 
        <UINT32 name="streamId">0</UINT32> 
        <UINT32 name="timestamp">0</UINT32> 
    </MAP> 
    <MAP name="invoke" isArray="false">
        <!-- INT64 name="int64_key">9223372036854780000</INT64 -->
                          <!--  9223372036854775807 -->
        <!-- UINT64 name="uint64_key">18446744073709600000</UINT64 -->
                          <!--  9223372036854775807 -->
        <STR name="functionName">connect</STR>
        <DOUBLE name="id">1.000</DOUBLE> 
        <BOOL name="isFlex">false</BOOL> 
        <MAP name="parameters" isArray="false"> 
            <MAP name="__index__value__0" isArray="false"> 
                <STR name="app">vptests</STR> 
                <DOUBLE name="audioCodecs">3191.000</DOUBLE> 
                <DOUBLE name="capabilities">239.000</DOUBLE> 
                <STR name="flashVer">MAC 10,1,51,66</STR> 
                <BOOL name="fpad">false</BOOL> 
                <DOUBLE name="objectEncoding">0.000</DOUBLE> 
                <NULL name="pageUrl"></NULL> 
                <NULL name="swfUrl"></NULL> 
                <STR name="tcUrl">rtmp://localhost/vptests</STR> 
                <DOUBLE name="videoCodecs">252.000</DOUBLE> 
                <DOUBLE name="videoFunction">1.000</DOUBLE>
                <DATE name="date_key">2010-10-31</DATE>
                <TIME name="time_key">10:11:12.000</TIME>
                <TIMESTAMP name="timestamp_key">2010-10-31T10:11:12.000</TIMESTAMP>
            </MAP> 
        </MAP>
    </MAP> 
</MAP> 
';

$testInput2='<?xml version="1.0" ?><MAP isArray="false" name=""><STR name="Buggy_node">&lt;map&gt;&amp;some other xml stuff&lt;/map&gt;</STR><MAP isArray="false" name="header"><UINT32 name="channelId">3</UINT32><UINT8 name="headerType">3</UINT8><BOOL name="isAbsolute">false</BOOL><UINT32 name="messageLength">242</UINT32><UINT8 name="messageType">20</UINT8><UINT32 name="streamId">0</UINT32><UINT32 name="timestamp">0</UINT32></MAP><MAP isArray="false" name="invoke"><STR name="functionName">connect</STR><DOUBLE name="id">1.000</DOUBLE><BOOL name="isFlex">false</BOOL><MAP isArray="false" name="parameters"><MAP isArray="false" name="__index__value__0"><STR name="app">vptests</STR><DOUBLE name="audioCodecs">3191.000</DOUBLE><DOUBLE name="capabilities">239.000</DOUBLE><STR name="flashVer">MAC 10,1,51,66</STR><BOOL name="fpad">false</BOOL><DOUBLE name="objectEncoding">0.000</DOUBLE><NULL name="pageUrl" /><NULL name="swfUrl" /><STR name="tcUrl">rtmp://localhost/vptests</STR><DOUBLE name="videoCodecs">252.000</DOUBLE><DOUBLE name="videoFunction">1.000</DOUBLE></MAP></MAP></MAP></MAP>';

$classFactory=new TestFactory();
$variantSerializer=new CVariantSerializer();
$variantSerializer->classFactory=$classFactory;


if(!$variantSerializer->DeserializeFromXML($testInput1)) {
//if(!$variantSerializer->DeserializeFromXML($testInput2)) {
//if(!$variantSerializer->DeserializeFromXML(file_get_contents('php://input'))) {
    echo "DeserializeFromXML failed\n".$variantSerializer->lastErrorDesc;
    exit;
}
//echo "DeserializeFromXML completed\n";
//var_dump($variantSerializer->deserializedObject);
//exit;


if(!$variantSerializer->SerializeToXML($variantSerializer->deserializedObject)) {
    echo "SerializeToXML failed\n".$variantSerializer->lastErrorDesc;
    exit();
}
header("Content-Type: text/xml");
echo $variantSerializer->serializedXml;
?>
