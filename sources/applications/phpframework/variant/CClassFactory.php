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
 * Description of CClassFactory
 *
 * @author Gavriloaie Eugen-Andrei (shiretu@gmail.com)
 */
abstract class CClassFactory {
    protected abstract function InstantiateObjectType($className);
    public function GetInstance($typeName, &$rawArray, &$errorDesc) {
        $result=$this->InstantiateObjectType($typeName);
        if($result==null){
            $errorDesc="This factory was unable to instantiate a class with name ".$typeName;
            return null;
        }
        if(!($result instanceof CSerializableObject)){
            $errorDesc="The instance returned is not an instance of CSerializableObject";
            return null;
        }
        if($result->Deserialize($rawArray)!==true){
            $errorDesc="Object was unable to deserialize itself. The class was: ".get_class($result);
            return null;
        }
        return $result;
    }
}
?>
