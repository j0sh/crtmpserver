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

/**
 * Description of CVariantSerializer
 *
 * @author Gavriloaie Eugen-Andrei (shiretu@gmail.com)
 */
class CVariantSerializer {
    public $classFactory;
    public $lastErrorDesc;
    public $deserializedObject;
    public $serializedXml;

    public function DeserializeFromXML($rawXml) {
        if(!is_string($rawXml)) {
            $this->DeserializeError("rawXml is not a string");
            return false;
        }
        if($rawXml=="") {
            $this->DeserializeError("rawXml is an empty string");
            return false;
        }
        $this->deserializedObject=null;
        $this->lastErrorDesc="";
        if($this->classFactory!=null) {
            if(!($this->classFactory instanceof CClassFactory)) {
                $this->DeserializeError("the class factory is not an instance of CClassFactory");
                return false;
            }
        }
        try {
            $xmlDoc = new DOMDocument();
            $xmlDoc->loadXML($rawXml);
            if($xmlDoc->documentElement==null) {
                $this->DeserializeError("provided xml doesn't have any content");
                return false;
            }
            if(!$this->DeserializeFromXMLNode($xmlDoc->documentElement,$this->deserializedObject)) {
                $this->DeserializeError("Error occured while deserializing the xml");
                return false;
            }
            return true;
        } catch (Exception $exc) {
            $this->DeserializeError("Exception occured while deserializing the xml:\n".$exc->getTraceAsString());
            return false;
        }
    }

    public function SerializeToXML($obj) {
        $xmlDoc = new DOMDocument();
        $node=$this->SerializeToXMLNode($obj,"",$xmlDoc);
        if($node==null) {
            $this->SerializeError("SerializeToXMLNode failed");
            return false;
        }
        $xmlDoc->appendChild($node);
        $this->serializedXml=$xmlDoc->saveXML();
        return true;
    }

    private function SerializeToXMLNode($obj,$name,&$xmlDoc) {
        if(is_array($obj)) {
            $node=$xmlDoc->createElement("MAP", "");
            $node->appendChild($xmlDoc->createAttribute("name"));
            $node->setAttribute("name", $name);
            $isArray=count($obj)!=0;
            foreach($obj as $k => $v) {
                $subnodeName="";
                if(is_numeric($k)) {
                    $subnodeName="__index__value__".$k;
                }else if(is_string($k)) {
                    $isArray&=false;
                    $subnodeName=$k;
                }else {
                    $this->SerializeError("Key must be either string or numeric");
                    return null;
                }
                $subNode=$this->SerializeToXMLNode($v,$subnodeName,$xmlDoc);
                if($subNode==null) {
                    $this->SerializeError("SerializeToXMLNode failed");
                    return null;
                }
                $node->appendChild($subNode);
            }
            $node->appendChild($xmlDoc->createAttribute("isArray"));
            $node->setAttribute("isArray", $isArray?"true":"false");
            return $node;
        }else if(is_object($obj)) {
            $node=null;
            if(get_class($obj)=="DateTime") {
                $strVal=$obj->format("Y-m-d\TH:i:s.000");
                if(substr($strVal, 0,10)=="1970-01-01") {
                    $node=$xmlDoc->createElement("TIME",substr($strVal, 11));
                }else {
                    if(substr($strVal, 11)=="00:00:00.000") {
                        $node=$xmlDoc->createElement("DATE",substr($strVal,0,10));
                    }else {
                        $node=$xmlDoc->createElement("TIMESTAMP",$strVal);
                    }
                }
                $node->appendChild($xmlDoc->createAttribute("name"));
                $node->setAttribute("name", $name);
            }else {
                if(!($obj instanceof CSerializableObject)) {
                    $this->SerializeError("Encountered an object which doesn't implements CSerializableObject: ".get_class($obj));
                    return null;
                }
                $rawArray=array();
                if($obj->Serialize($rawArray)!==true) {
                    $this->SerializeError("CSerializableObject failed to serialize. Class name: ".get_class($obj));
                    return null;
                }

                $node=$xmlDoc->createElement("TYPED_MAP", "");
                $node->appendChild($xmlDoc->createAttribute("name"));
                $node->setAttribute("name", $name);
                $node->appendChild($xmlDoc->createAttribute("typeName"));
                $node->setAttribute("typeName", get_class($obj));
                $node->appendChild($xmlDoc->createAttribute("isArray"));
                $node->setAttribute("isArray", "false");
                foreach($rawArray as $k => $v) {
                    $subnodeName="";
                    if(is_numeric($k)) {
                        $subnodeName="__index__value__".$k;
                    }else if(is_string($k)) {
                        $subnodeName=$k;
                    }else {
                        $this->SerializeError("Key must be either string or numeric");
                        return null;
                    }
                    $subNode=$this->SerializeToXMLNode($v,$subnodeName,$xmlDoc);
                    if($subNode==null) {
                        $this->SerializeError("SerializeToXMLNode failed");
                        return null;
                    }
                    $node->appendChild($subNode);
                }
                return $node;
            }
            return $node;
        }else if(is_string($obj)) {
            $node=$xmlDoc->createElement("STR", "");
            $node->appendChild($xmlDoc->createTextNode($obj));
            $node->appendChild($xmlDoc->createAttribute("name"));
            $node->setAttribute("name", $name);
            return $node;
        }else if(is_numeric($obj)) {
            $node=$xmlDoc->createElement("DOUBLE", $obj);
            $node->appendChild($xmlDoc->createAttribute("name"));
            $node->setAttribute("name", $name);
            return $node;
        }else if(is_bool($obj)) {
            $node=$xmlDoc->createElement("BOOL", $obj?"true":"false");
            $node->appendChild($xmlDoc->createAttribute("name"));
            $node->setAttribute("name", $name);
            return $node;
        }else if($obj==null) {
            $node=$xmlDoc->createElement("NULL", "");
            $node->appendChild($xmlDoc->createAttribute("name"));
            $node->setAttribute("name", $name);
            return $node;
        }else {
            $this->SerializeError("This is not a serializable object");
            return null;
        }
    }

    private function DeserializeError($description) {
        $this->deserializedObject=null;
        $this->lastErrorDesc.="\n".$description;
    }

    private function SerializeError($description) {
        $this->serializedXml=null;
        $this->lastErrorDesc.="\n".$description;
    }

    private function GetAttribute($node, $name) {
        foreach($node->attributes as $attribute) {
            if(strtolower($attribute->name)==strtolower($name)) {
                return $attribute->value;
            }
        }
        return "";
    }

    private function DeserializeFromXMLNode($node, &$result) {
        switch(strtolower($node->tagName)) {
            case "map":
            case "typed_map":
                $result=array();
                for($subNode=$node->firstChild;$subNode!=null;$subNode=$subNode->nextSibling) {
                    if($subNode->nodeType!=XML_ELEMENT_NODE) {
                        continue;
                    }
                    $key=$this->GetAttribute($subNode, "name");
                    if(strpos($key, "__index__value__")===0) {
                        $key=intval(substr($key, 16));
                    }
                    if(!$this->DeserializeFromXMLNode($subNode,$result[$key])) {
                        $this->DeserializeError("Error occured while deserializing the xml: DeserializeFromXMLNode");
                        return false;
                    }
                }
                if(strtolower($node->tagName)=="typed_map") {
                    $typeName=$this->GetAttribute($node, "typename");
                    if($this->classFactory!=null) {
                        $errorDesc="";
                        $result=$this->classFactory->GetInstance($typeName,$result,$errorDesc);
                        if($result==null) {
                            $this->DeserializeError("Error occured while deserializing the xml: classFactory->GetInstance returned null. Error was: ".$errorDesc);
                            return false;
                        }
                    }
                }
                return true;
            case "null":
            case "undefined":
                $result=null;
                return true;
            case "bool":
                $result=strtolower($node->nodeValue)=="true";
                return true;
            case "int8":
            case "int16":
            case "int32":
            case "int64":
            case "uint8":
            case "uint16":
            case "uint32":
            case "uint64":
                $result=intval($node->nodeValue);
                return true;
            case "double":
                $result=floatval($node->nodeValue);
                return true;
            case "str":
                $result=$node->nodeValue;
                return true;
            case "date":
                $result=DateTime::createFromFormat("Y-m-d H:i:s", $node->nodeValue." 00:00:00", new DateTimeZone("UTC"));
                return true;
            case "time":
                $result=DateTime::createFromFormat("Y-m-d H:i:s.000", "1970-01-01 ".$node->nodeValue, new DateTimeZone("UTC"));
                return true;
            case "timestamp":
                $result=DateTime::createFromFormat("Y-m-d\TH:i:s.000", $node->nodeValue, new DateTimeZone("UTC"));
                return true;
            default:
                $this->DeserializeError("Error occured while deserializing the xml: ".$node->tagName." not implemented yet");
                return false;
        }
    }
}

?>
